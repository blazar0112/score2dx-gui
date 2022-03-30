#include "gui/Activity/ActivityManager.hpp"

#include <QDebug>

#include "ies/Common/IntegralRangeUsing.hpp"
#include "ies/StdUtil/Find.hxx"
#include "ies/Time/TimeUtilFormat.hxx"

#include "fmt/format.h"

#include "score2dx/Iidx/Version.hpp"
#include "score2dx/Analysis/Analyzer.hpp"

#include "gui/Activity/ChartActivityListModel.hpp"

namespace s2Time = ies::Time;

namespace gui
{

ActivityManager::
ActivityManager(Core &core, QObject *parent)
:   QObject(parent),
    mGuiCore(core)
{
}

void
ActivityManager::
updateActivity(const QString &iidxId,
               const QString &playStyleQStr,
               const QString &date)
{
    if (iidxId.isEmpty()||playStyleQStr.isEmpty()||date.isEmpty())
    {
        return;
    }

    auto playStyle = score2dx::ToPlayStyle(playStyleQStr.toStdString());

    auto isoDate = date.toStdString();
    auto beginDateTime = isoDate+" 00:00";
    auto endDateTime = isoDate+" 23:59";

    auto findVersionIndex = score2dx::FindVersionIndexFromDateTime(beginDateTime);
    if (!findVersionIndex)
    {
        qDebug() << "cannot find version index from " << date;
        return;
    }
    auto activeVersionIndex = findVersionIndex.value();

    mGuiCore.AnalyzeActivity(iidxId.toStdString(), beginDateTime, endDateTime);

    auto* activityAnalysisPtr = mGuiCore.GetScore2dxCore().FindActivityAnalysis(iidxId.toStdString());
    if (!activityAnalysisPtr)
    {
        qDebug() << "Cannot find ActivityAnalysis for player " << iidxId;
        return;
    }

    mActivityPlayStyle = playStyleQStr;
    mActivityDate = date;

    auto &activityAnalysis = *activityAnalysisPtr;
    auto &styleActivity = activityAnalysis.ActivityByDateTime.at(playStyle);

    std::vector<ActivityData> activityList;
    std::vector<std::vector<ChartActivityData>> musicChartActivityList;

    std::size_t musicScoreCount = 0;
    for (auto &[dateTime, musicScoreById] : styleActivity)
    {
        auto tokens = ies::SplitString(" ", dateTime);
        if (tokens[0]!=isoDate)
        {
            throw std::runtime_error("daily activity has incorrect date in datetime "+dateTime);
        }

        musicScoreCount += musicScoreById.size();
    }

    activityList.reserve(musicScoreCount);
    musicChartActivityList.resize(musicScoreCount);

    auto* scoreAnalysisPtr = mGuiCore.GetScore2dxCore().FindAnalysis(iidxId.toStdString());
    if (!scoreAnalysisPtr)
    {
        qDebug() << "Cannot find ScoreAnalysis for player " << iidxId;
        return;
    }

    auto &scoreAnalysis = *scoreAnalysisPtr;
    auto &database = mGuiCore.GetScore2dxCore().GetMusicDatabase();

    std::size_t index = 0;
    for (auto &[dateTime, musicScoreById] : styleActivity)
    {
        auto tokens = ies::SplitString(" ", dateTime);
        auto time = tokens[1];
        //! @todo fix ist script so it does not contain seconds.
        if (time.size()!=5)
        {
            time = time.substr(0, 5);
        }

        for (auto &[musicId, musicScore] : musicScoreById)
        {
            auto &snapshotData = activityAnalysis.ActivitySnapshotByDateTime.at(playStyle).at(dateTime).at(musicId);

            auto allChartScoreSame = true;
            for (auto &[difficulty, chartScorePtr] : musicScore.GetChartScores())
            {
                auto* previousChartScorePtr = snapshotData.PreviousMusicScore->GetChartScore(difficulty);
                if (!previousChartScorePtr)
                {
                    musicScore.Print();
                    qDebug() << "previous difficulty not found";
                    continue;
                }

                if (chartScorePtr!=previousChartScorePtr)
                {
                    allChartScoreSame = false;
                    break;
                }
            }

            if (allChartScoreSame)
            {
                continue;
            }

            auto versionIndex = musicId/1000;
            auto versionString = score2dx::ToVersionString(versionIndex);
            auto title = database.GetLatestMusicInfo(musicId).GetField(score2dx::MusicInfoField::Title);

            activityList.emplace_back();
            auto &activityData = activityList.back();

            activityData.Data[static_cast<int>(ActivityDataRole::time)] = time.c_str();
            activityData.Data[static_cast<int>(ActivityDataRole::title)] = title.c_str();
            activityData.Data[static_cast<int>(ActivityDataRole::version)] = versionString.c_str();
            activityData.Data[static_cast<int>(ActivityDataRole::previousPlayCount)] = QString::number(snapshotData.PreviousMusicScore->GetPlayCount());
            activityData.Data[static_cast<int>(ActivityDataRole::playCount)] = QString::number(musicScore.GetPlayCount());

            if (&musicScore!=snapshotData.CurrentMusicScore)
            {
                throw std::runtime_error("&musicScore!=snapshotData.CurrentMusicScore");
            }

            auto &chartActivityList = musicChartActivityList.at(index);
            chartActivityList.reserve(score2dx::DifficultySmartEnum::Size());

            for (auto &[difficulty, chartScorePtr] : musicScore.GetChartScores())
            {
                auto &chartScore = *chartScorePtr;
                auto* previousChartScorePtr = snapshotData.PreviousMusicScore->GetChartScore(difficulty);
                if (!previousChartScorePtr)
                {
                    musicScore.Print();
                    qDebug() << "previous difficulty not found";
                    continue;
                }

                if (chartScore==*previousChartScorePtr)
                {
                    continue;
                }

                chartActivityList.emplace_back();
                auto &chartActivity = chartActivityList.back();
                auto styleDifficulty = score2dx::ConvertToStyleDifficulty(playStyle, difficulty);

                auto findChartInfo = database.FindChartInfo(musicId, styleDifficulty, activeVersionIndex);
                if (!findChartInfo)
                {
                    qDebug() << "cannot find chart info music id" << musicId << "[" << ToString(styleDifficulty).c_str() << "].";
                    continue;
                }

                auto &chartInfo = *findChartInfo;
                auto &previousChartScore = *previousChartScorePtr;

                chartActivity.Data[static_cast<int>(ChartActivityDataRole::level)] = QString::number(chartInfo.Level);
                chartActivity.Data[static_cast<int>(ChartActivityDataRole::difficulty)] = ToString(static_cast<score2dx::DifficultyAcronym>(difficulty)).c_str();
                chartActivity.Data[static_cast<int>(ChartActivityDataRole::previousClear)] = ToPrettyString(previousChartScore.ClearType).c_str();
                chartActivity.Data[static_cast<int>(ChartActivityDataRole::previousScore)] = QString::number(previousChartScore.ExScore);
                chartActivity.Data[static_cast<int>(ChartActivityDataRole::previousDjLevel)] = ToString(previousChartScore.DjLevel).c_str();
                chartActivity.Data[static_cast<int>(ChartActivityDataRole::previousScoreLevelDiff)] = "";
                if (previousChartScore.ExScore!=0)
                {
                    auto previousScoreLevelDiff = score2dx::ToScoreLevelDiffString(chartInfo.Note, previousChartScore.ExScore);
                    chartActivity.Data[static_cast<int>(ChartActivityDataRole::previousScoreLevelDiff)] = previousScoreLevelDiff.c_str();
                }
                chartActivity.Data[static_cast<int>(ChartActivityDataRole::previousMiss)] = "N/A";
                if (previousChartScore.MissCount)
                {
                    chartActivity.Data[static_cast<int>(ChartActivityDataRole::previousMiss)] = QString::number(previousChartScore.MissCount.value());
                }

                chartActivity.Data[static_cast<int>(ChartActivityDataRole::newRecordClear)] = "";
                chartActivity.Data[static_cast<int>(ChartActivityDataRole::newRecordScore)] = "";
                chartActivity.Data[static_cast<int>(ChartActivityDataRole::newRecordDjLevel)] = "";
                chartActivity.Data[static_cast<int>(ChartActivityDataRole::newRecordScoreLevelDiff)] = "";
                chartActivity.Data[static_cast<int>(ChartActivityDataRole::newRecordMiss)] = "";

                if (chartScore.ClearType>previousChartScore.ClearType)
                {
                    chartActivity.Data[static_cast<int>(ChartActivityDataRole::newRecordClear)] = ToPrettyString(chartScore.ClearType).c_str();;
                }

                if (chartScore.ExScore>previousChartScore.ExScore)
                {
                    chartActivity.Data[static_cast<int>(ChartActivityDataRole::newRecordScore)] = QString::number(chartScore.ExScore);
                    auto scoreLevelDiff = score2dx::ToScoreLevelDiffString(chartInfo.Note, chartScore.ExScore);
                    chartActivity.Data[static_cast<int>(ChartActivityDataRole::newRecordScoreLevelDiff)] = scoreLevelDiff.c_str();
                }

                if (chartScore.DjLevel>previousChartScore.DjLevel)
                {
                    chartActivity.Data[static_cast<int>(ChartActivityDataRole::newRecordDjLevel)] = ToString(chartScore.DjLevel).c_str();
                }

                if (chartScore.MissCount
                    && (!previousChartScore.MissCount || chartScore.MissCount<previousChartScore.MissCount))
                {
                    chartActivity.Data[static_cast<int>(ChartActivityDataRole::newRecordMiss)] = QString::number(chartScore.MissCount.value());
                }

                auto betterScore = std::max(chartScore.ExScore, previousChartScore.ExScore);
                auto betterMiss = previousChartScore.MissCount;
                if (!previousChartScore.MissCount.has_value())
                {
                    betterMiss = chartScore.MissCount;
                }
                if (chartScore.MissCount.has_value() && previousChartScore.MissCount.has_value()
                    && chartScore.MissCount < previousChartScore.MissCount)
                {
                    betterMiss = chartScore.MissCount;
                }

                chartActivity.Data[static_cast<int>(ChartActivityDataRole::careerDiffableBestScoreDiff)] = "N/A";
                chartActivity.Data[static_cast<int>(ChartActivityDataRole::careerDiffableBestMissDiff)] = "N/A";

                auto findBestScoreData = ies::Find(scoreAnalysis.MusicBestScoreData, musicId);
                if (!findBestScoreData)
                {
                    qDebug() << "cannot find best score data of music id" << musicId << "[" << ToString(styleDifficulty).c_str() << "].";
                    continue;
                }

                auto &bestScoreData = (findBestScoreData.value()->second).at(playStyle);
                auto* bestChartScorePtr = bestScoreData.GetVersionBestMusicScore().GetChartScore(difficulty);
                if (!bestChartScorePtr)
                {
                    qDebug() << "cannot find chart score music id" << musicId << "[" << ToString(styleDifficulty).c_str() << "].";
                    continue;
                }

                if (auto* findCareerDiffableBestScore =
                        bestScoreData.FindDiffableChartScoreRecord(score2dx::DiffableBestScoreType::ExScore, difficulty))
                {
                    auto &careerBestDiffableScoreRecord = *findCareerDiffableBestScore;
                    auto scoreDiff = betterScore-careerBestDiffableScoreRecord.ChartScoreProp.ExScore;
                    auto scoreDiffStr = fmt::format("{:+d}", scoreDiff);

                    chartActivity.Data[static_cast<int>(ChartActivityDataRole::careerDiffableBestScoreDiff)] =
                        scoreDiffStr.c_str();
                }

                if (auto* findCareerDiffableBestMiss =
                        bestScoreData.FindDiffableChartScoreRecord(score2dx::DiffableBestScoreType::Miss, difficulty))
                {
                    auto &careerBestDiffableMissRecord = *findCareerDiffableBestMiss;
                    //'' fail at this version, has previous two miss record, it's possible.
                    if (!betterMiss.has_value() || !careerBestDiffableMissRecord.ChartScoreProp.MissCount.has_value())
                    {
                        continue;
                    }

                    auto missDiff = betterMiss.value()-careerBestDiffableMissRecord.ChartScoreProp.MissCount.value();
                    auto missDiffStr = fmt::format("{:+d}", missDiff);

                    chartActivity.Data[static_cast<int>(ChartActivityDataRole::careerDiffableBestMissDiff)] =
                        missDiffStr.c_str();
                }
            }

            ++index;
        }
    }

    mActivityListModel.ResetModel(std::move(activityList),
                                  std::move(musicChartActivityList));

    emit activityPlayStyleChanged();
}

QString
ActivityManager::
findActivityDateType(const QString &iidxId,
                     const QString &playStyleQStr,
                     const QString &date)
const
{
    if (iidxId.isEmpty()||playStyleQStr.isEmpty()||date.isEmpty())
    {
        return {};
    }

    auto* activityAnalysisPtr = mGuiCore.GetScore2dxCore().FindVersionActivityAnalysis(iidxId.toStdString());
    if (!activityAnalysisPtr)
    {
        qDebug() << "Cannot find ActivityAnalysis for player " << iidxId;
        return {};
    }

    auto &versionActivityAnalysis = *activityAnalysisPtr;

    auto playStyle = score2dx::ToPlayStyle(playStyleQStr.toStdString());

    auto isoDate = date.toStdString();
    auto dateTime = isoDate+" 00:00";

    auto versionDateType = score2dx::FindVersionDateType(dateTime);
    if (versionDateType==score2dx::VersionDateType::VersionBegin)
    {
        return ToString(ActivityDateType::VersionBegin).c_str();
    }
    if (versionDateType==score2dx::VersionDateType::VersionEnd)
    {
        return ToString(ActivityDateType::VersionEnd).c_str();
    }

    for (auto &[dateTime, musicScoreById] : versionActivityAnalysis.ActivityByDateTime.at(playStyle))
    {
        if (ies::Find(dateTime, isoDate))
        {
            return ToString(ActivityDateType::HasActivity).c_str();
        }
    }

    return ToString(ActivityDateType::NoActivity).c_str();
}

QString
ActivityManager::
getVersionDateTimeRange(const QString &date)
const
{
    if (date.isEmpty())
    {
        return "Version: N/A";
    }
    auto dateTime = date.toStdString()+" 00:00";
    auto findVersionIndex = score2dx::FindVersionIndexFromDateTime(dateTime);
    if (!findVersionIndex)
    {
        return "Version: N/A";
    }
    auto versionIndex = findVersionIndex.value();
    auto dateTimeRange = score2dx::GetVersionDateTimeRange(versionIndex);
    auto &begin = dateTimeRange.at(ies::RangeSide::Begin);
    auto &end = dateTimeRange.at(ies::RangeSide::End);
    auto text = "Version: "+score2dx::VersionNames.at(versionIndex)
                +" ("+score2dx::ToVersionString(versionIndex)
                +") ["+begin
                +", "+end
                +"]";
    return text.c_str();
}

}
