#include "gui/Activity/ActivityManager.hpp"

#include <QDebug>

#include "icl_s2/Common/IntegralRangeUsing.hpp"
#include "icl_s2/StdUtil/Find.hxx"
#include "icl_s2/Time/TimeUtilFormat.hxx"

#include "fmt/format.h"

#include "score2dx/Iidx/Version.hpp"
#include "score2dx/Analysis/Analyzer.hpp"

#include "gui/Activity/MusicActivityListModel.hpp"

namespace s2Time = icl_s2::Time;

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

    std::string testDate = "2021-11-20";
    auto beginDateTime = testDate+" 00:00";
    auto endDateTime = testDate+" 23:59";

    auto activeVersionIndex = score2dx::FindVersionIndexFromDateTime(beginDateTime);

    mGuiCore.AnalyzeActivity(iidxId.toStdString(), beginDateTime, endDateTime);

    auto* activityAnalysisPtr = mGuiCore.GetScore2dxCore().FindActivityAnalysis(iidxId.toStdString());
    if (!activityAnalysisPtr)
    {
        qDebug() << "Cannot find ActivityAnalysis for player " << iidxId;
        return;
    }

    auto &activityAnalysis = *activityAnalysisPtr;

    auto playStyle = score2dx::ToPlayStyle(playStyleQStr.toStdString());

    auto &styleActivity = activityAnalysis.ActivityByDateTime.at(playStyle);

    std::vector<ActivityData> activityList;
    std::vector<std::vector<MusicActivityData>> musicActivityList;

    std::size_t count = 0;
    for (auto &[dateTime, musicScoreById] : styleActivity)
    {
        auto tokens = icl_s2::SplitString(" ", dateTime);
        if (tokens[0]!=testDate)
        {
            throw std::runtime_error("daily activity has incorrect date in datetime "+dateTime);
        }

        count += musicScoreById.size();
    }

    activityList.reserve(count);
    musicActivityList.resize(count);

    std::size_t index = 0;
    for (auto &[dateTime, musicScoreById] : styleActivity)
    {
        auto tokens = icl_s2::SplitString(" ", dateTime);
        auto &time = tokens[1];

        for (auto &[musicId, musicScore] : musicScoreById)
        {
            activityList.emplace_back();
            auto &activityData = activityList.back();

            activityData.Data[static_cast<int>(ActivityDataRole::time)] = time.c_str();

            auto &database = mGuiCore.GetScore2dxCore().GetMusicDatabase();
            auto title = database.GetLatestMusicInfo(musicId).GetField(score2dx::MusicInfoField::Title);

            activityData.Data[static_cast<int>(ActivityDataRole::title)] = title.c_str();
            auto versionIndex = musicId/1000;
            auto versionString = score2dx::ToVersionString(versionIndex);
            activityData.Data[static_cast<int>(ActivityDataRole::version)] = versionString.c_str();

            auto &snapshotData = activityAnalysis.ActivitySnapshotByDateTime.at(playStyle).at(dateTime).at(musicId);

            activityData.Data[static_cast<int>(ActivityDataRole::previousPlayCount)] = QString::number(snapshotData.PreviousMusicScore->GetPlayCount());
            activityData.Data[static_cast<int>(ActivityDataRole::playCount)] = QString::number(musicScore.GetPlayCount());

            if (&musicScore!=snapshotData.CurrentMusicScore)
            {
                throw std::runtime_error("&musicScore!=snapshotData.CurrentMusicScore");
            }

            auto &musicActivity = musicActivityList.at(index);
            musicActivity.reserve(score2dx::DifficultySmartEnum::Size());

            for (auto &[difficulty, chartScore] : musicScore.GetChartScores())
            {
                auto* previousChartScorePtr = snapshotData.PreviousMusicScore->FindChartScore(difficulty);
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

                musicActivity.emplace_back();
                auto &chartActivity = musicActivity.back();
                auto styleDifficulty = score2dx::ConvertToStyleDifficulty(playStyle, difficulty);
                chartActivity.Data[static_cast<int>(MusicActivityDataRole::styleDifficulty)] = ToString(styleDifficulty).c_str();

                auto findChartInfo = database.FindChartInfo(versionIndex, title, styleDifficulty, activeVersionIndex);
                if (!findChartInfo)
                {
                    qDebug() << "cannot find chart info music id" << musicId << "[" << ToString(styleDifficulty).c_str() << "].";
                    continue;
                }

                auto &chartInfo = findChartInfo.value();
                auto &previousChartScore = *previousChartScorePtr;

                chartActivity.Data[static_cast<int>(MusicActivityDataRole::level)] = QString::number(chartInfo.Level);
                chartActivity.Data[static_cast<int>(MusicActivityDataRole::difficulty)] = ToString(difficulty).c_str();
                chartActivity.Data[static_cast<int>(MusicActivityDataRole::previousClear)] = ToPrettyString(previousChartScore.ClearType).c_str();
                chartActivity.Data[static_cast<int>(MusicActivityDataRole::previousScore)] = QString::number(previousChartScore.ExScore);
                chartActivity.Data[static_cast<int>(MusicActivityDataRole::previousMiss)] = "N/A";
                if (previousChartScore.MissCount)
                {
                    chartActivity.Data[static_cast<int>(MusicActivityDataRole::previousMiss)] = QString::number(previousChartScore.MissCount.value());
                }

                chartActivity.Data[static_cast<int>(MusicActivityDataRole::newRecordClear)] = "";
                chartActivity.Data[static_cast<int>(MusicActivityDataRole::newRecordScore)] = "";
                chartActivity.Data[static_cast<int>(MusicActivityDataRole::newRecordMiss)] = "";

                if (chartScore.ClearType>previousChartScore.ClearType)
                {
                    chartActivity.Data[static_cast<int>(MusicActivityDataRole::newRecordClear)] = ToPrettyString(chartScore.ClearType).c_str();;
                }
                if (chartScore.ExScore>previousChartScore.ExScore)
                {
                    chartActivity.Data[static_cast<int>(MusicActivityDataRole::newRecordScore)] = QString::number(chartScore.ExScore);
                }
                if (chartScore.MissCount &&
                        (!previousChartScore.MissCount.has_value()
                            || chartScore.MissCount>previousChartScore.MissCount))
                {
                    chartActivity.Data[static_cast<int>(MusicActivityDataRole::newRecordMiss)] = QString::number(chartScore.MissCount.value());
                }
            }

            ++index;
        }
    }

    mActivityListModel.ResetModel(std::move(activityList),
                                  std::move(musicActivityList));
}

}
