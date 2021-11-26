#include "gui/Graph/GraphManager.hpp"

#include <functional>
#include <iostream>

#include <QDateTime>
#include <QDebug>
#include <QtCharts/QLegendMarker>

#include "icl_s2/Common/IntegralRangeUsing.hpp"
#include "icl_s2/String/RecursiveReplace.hpp"

#include "score2dx/Iidx/Version.hpp"
#include "score2dx/Score/ScoreLevel.hpp"

namespace
{

QDateTime
ToQDateTime(const std::string &dateTime)
{
    auto tokens = icl_s2::SplitString("- :", dateTime, 5);

    QDateTime qDateTime;
    qDateTime.setDate({std::stoi(tokens[0]), std::stoi(tokens[1]), std::stoi(tokens[2])});
    qDateTime.setTime({std::stoi(tokens[3]), std::stoi(tokens[4])});

    return qDateTime;
}

}

namespace gui
{

GraphManager::
GraphManager(const score2dx::Core &core, QObject *parent)
:   QObject(parent),
    mCore(core)
{
    for (auto versionIndex : IndexRange{17, score2dx::GetLatestVersionIndex()})
    {
        mTimelineBeginVersionList << score2dx::VersionNames.at(versionIndex).c_str();
    }
}

void
GraphManager::
setup(QtCharts::QLegend* legend,
      QtCharts::QAbstractSeries* scoreSeries,
      QtCharts::QAbstractAxis* dateTimeAxis,
      QtCharts::QAbstractAxis* scoreAxis,
      QtCharts::QAbstractAxis* versionCategoryAxis,
      QtCharts::QAbstractSeries* scatterSeriesScoreLevel,
      QtCharts::QAbstractAxis* scoreLevelAxis)
{
    mLegend = legend;

    mScoreSeries = static_cast<QtCharts::QXYSeries*>(scoreSeries);
    mDateTimeAxis = static_cast<QtCharts::QDateTimeAxis*>(dateTimeAxis);
    mScoreAxis = static_cast<QtCharts::QValueAxis*>(scoreAxis);

    mVersionCategoryAxis = static_cast<QtCharts::QCategoryAxis*>(versionCategoryAxis);

    mScatterSeriesScoreLevel = static_cast<QtCharts::QScatterSeries*>(scatterSeriesScoreLevel);
    mScoreLevelAxis = static_cast<QtCharts::QValueAxis*>(scoreLevelAxis);

    InitializeChart();
}

void
GraphManager::
updatePlayerScore(const QString &iidxIdQStr,
                  const QString &playStyleQStr,
                  int musicId,
                  const QString &difficultyQStr)
{
    if (iidxIdQStr.isEmpty()||playStyleQStr.isEmpty()||difficultyQStr.isEmpty())
    {
        return;
    }

    if (!mScoreSeries||!mScoreAxis||!mScatterSeriesScoreLevel||!mScoreLevelAxis)
    {
        return;
    }

    auto &series = *mScoreSeries;
    series.clear();

    auto &scoreAxis = *mScoreAxis;
    scoreAxis.setMin(0);
    scoreAxis.setMax(20);

    auto &scoreLevelSeries = *mScatterSeriesScoreLevel;
    scoreLevelSeries.clear();

    auto &scoreLevelAxis = *mScoreLevelAxis;
    scoreLevelAxis.setMin(0);
    scoreLevelAxis.setMax(20);

    auto &playerScore = mCore.GetPlayerScores().at(iidxIdQStr.toStdString());
    auto playStyle = score2dx::ToPlayStyle(playStyleQStr.toStdString());
    auto difficulty = score2dx::ToDifficulty(difficultyQStr.toStdString());
    auto chartScores = playerScore.GetChartScores(musicId, playStyle, difficulty);

    auto info = mCore.GetMusicDatabase().GetLatestMusicInfo(musicId);
    auto chartInfo = info.FindChartInfo(playStyle, difficulty);
    if (!chartInfo)
    {
        std::cout << "Cannot find chart info of Music [" << musicId
                  << "]["+ToString(playStyle)+"]["+ToString(difficulty)+"]"
                  << std::endl;
        return;
    }

    auto note = chartInfo->Note;
    auto maxScore = 2*note;

    scoreAxis.setMin(0);
    scoreAxis.setMax(maxScore);
    scoreLevelAxis.setMin(0);
    scoreLevelAxis.setMax(maxScore);

    std::vector<QPointF> scoreLevelPointList;
    scoreLevelPointList.reserve(score2dx::ScoreLevelSmartEnum::Size()+1);

    //'' need both series and list model 1-1 'duplicate' data.
    //'' series to have point in chart to know position in chart.
    //'' list model to drive qml repeater.
    for (auto scoreLevel : score2dx::ScoreLevelSmartEnum::ToRange())
    {
        auto keyScore = score2dx::FindKeyScore(note, scoreLevel);
        scoreLevelSeries.append(1, keyScore);
        scoreLevelPointList.emplace_back(QPointF(1, keyScore));
    }
    auto halfKeyScore = score2dx::FindHalfKeyScore(note, score2dx::ScoreLevel::Max);
    scoreLevelSeries.append(1, halfKeyScore);
    scoreLevelPointList.emplace_back(QPointF(1, halfKeyScore));
    mScoreLevelListModel.ResetList(scoreLevelPointList);

    std::vector<GraphAnalysisData> analysisList;
    analysisList.reserve(chartScores.size());

    //'' chartScore may be empty, because no difficulty is played.
    //'' if any difficulty is played, then a ChartScore is created.
    //'' but looking difficulty may not be played yet, so 0 ExScore.
    auto isFirst = true;
    GraphAnalysisData bestRecordData;

    //! @brief Map of {GraphAnalysisType, IsBetter(GraphAnalysisType, currentData)}.
    std::map<GraphAnalysisType, std::function<bool(GraphAnalysisType, const GraphAnalysisData &)>> compareBestFunctions
    {
        {   GraphAnalysisType::Clear,
            [&bestRecordData](GraphAnalysisType analysisType, const GraphAnalysisData &currentData) -> bool
            {
                auto &previousRecord = bestRecordData.GetRecord(analysisType).Record;
                if (previousRecord.isEmpty())
                {
                    return true;
                }

                auto previousClear = score2dx::ToClearType(previousRecord.toStdString());
                auto &currentRecord = currentData.GetRecord(analysisType).Record;
                auto currentClear = score2dx::ToClearType(currentRecord.toStdString());

                return static_cast<int>(currentClear)>static_cast<int>(previousClear);
            }
        },
        {   GraphAnalysisType::Score,
            [&bestRecordData](GraphAnalysisType analysisType, const GraphAnalysisData &currentData) -> bool
            {
                auto &previousRecord = bestRecordData.GetRecord(analysisType).Record;
                if (previousRecord.isEmpty())
                {
                    return true;
                }

                auto previousScore = previousRecord.toInt();
                auto &currentRecord = currentData.GetRecord(analysisType).Record;
                auto currentScore = currentRecord.toInt();

                return currentScore>previousScore;
            }
        },
        {   GraphAnalysisType::DjLevel,
            [&bestRecordData](GraphAnalysisType analysisType, const GraphAnalysisData &currentData) -> bool
            {
                auto &previousRecord = bestRecordData.GetRecord(analysisType).Record;
                if (previousRecord.isEmpty())
                {
                    return true;
                }

                auto previousDjLevel = score2dx::ToDjLevel(previousRecord.toStdString());
                auto &currentRecord = currentData.GetRecord(analysisType).Record;
                auto currentDjLevel = score2dx::ToDjLevel(currentRecord.toStdString());

                return static_cast<int>(currentDjLevel)>static_cast<int>(previousDjLevel);
            }
        },
        {   GraphAnalysisType::MissCount,
            [&bestRecordData](GraphAnalysisType analysisType, const GraphAnalysisData &currentData) -> bool
            {
                auto &previousRecord = bestRecordData.GetRecord(analysisType).Record;
                auto &currentRecord = currentData.GetRecord(analysisType).Record;

                if (previousRecord.isEmpty()&&!currentRecord.isEmpty())
                {
                    return true;
                }

                if (currentRecord.isEmpty())
                {
                    return false;
                }

                auto previousMiss = previousRecord.toInt();
                auto currentMiss = currentRecord.toInt();

                return previousMiss>currentMiss;
            }
        }
    };

    for (auto &[dateTime, chartScorePtr] : chartScores)
    {
        auto &chartScore = *chartScorePtr;

        //'' always record first unless it's NO PLAY.
        if (isFirst&&chartScore.ClearType==score2dx::ClearType::NO_PLAY)
        {
            continue;
        }

        //'' skip non-first 0 score entry.
        if (!isFirst&&chartScore.ExScore==0)
        {
            continue;
        }

        auto qDateTime = ToQDateTime(dateTime);
        series.append(qDateTime.toMSecsSinceEpoch(), chartScore.ExScore);

        analysisList.emplace_back();
        auto &analysisData = analysisList.back();

        analysisData.ScoreLevelRangeDiff = score2dx::ToScoreLevelDiffString(note, chartScore.ExScore).c_str();

        //'' note: to compare clear type, record here is not space separated
        //'' convert to space separated after all data constructed.
        analysisData.GetRecord(GraphAnalysisType::Clear).Record = ToString(chartScore.ClearType).c_str();
        analysisData.GetRecord(GraphAnalysisType::Score).Record = std::to_string(chartScore.ExScore).c_str();
        analysisData.GetRecord(GraphAnalysisType::DjLevel).Record = ToString(chartScore.DjLevel).c_str();
        if (chartScore.MissCount)
        {
            analysisData.GetRecord(GraphAnalysisType::MissCount).Record = std::to_string(chartScore.MissCount.value()).c_str();
        }

        for (auto analysisType : GraphAnalysisTypeSmartEnum::ToRange())
        {
            auto &compareFunction = compareBestFunctions[analysisType];
            if (compareFunction(analysisType, analysisData))
            {
                auto &currentRecord = analysisData.GetRecord(analysisType);
                auto &bestRecord = bestRecordData.GetRecord(analysisType);

                currentRecord.PreviousRecord = bestRecord.Record;
                bestRecord.Record = currentRecord.Record;
                currentRecord.NewRecord = true;
            }
        }

        isFirst = false;
    }

    for (auto &analysisData : analysisList)
    {
        auto &clearRecord = analysisData.GetRecord(GraphAnalysisType::Clear);
        if (!clearRecord.Record.isEmpty())
        {
            auto underscoreStr = clearRecord.Record.toStdString();
            auto spaceSeparated = ToSpaceSeparated(score2dx::ToClearType(underscoreStr));
            icl_s2::RecursiveReplace(spaceSeparated, " CLEAR", "");
            clearRecord.Record = spaceSeparated.c_str();
        }
        if (!clearRecord.PreviousRecord.isEmpty())
        {
            auto underscoreStr = clearRecord.PreviousRecord.toStdString();
            auto spaceSeparated = ToSpaceSeparated(score2dx::ToClearType(underscoreStr));
            icl_s2::RecursiveReplace(spaceSeparated, " CLEAR", "");
            clearRecord.PreviousRecord = spaceSeparated.c_str();
        }
    }

    //'' always nodify last element.
    if (!analysisList.empty())
    {
        auto &back = analysisList.back();
        for (auto analysisType : GraphAnalysisTypeSmartEnum::ToRange())
        {
            back.GetRecord(analysisType).NewRecord = true;
        }
    }

    mGraphAnalysisListModel.ResetList(analysisList);
}

void
GraphManager::
updateTimelineBeginVersion(const QString &timelineBeginVersion)
{
    auto findVersionIndex = score2dx::FindVersionIndex(timelineBeginVersion.toStdString());
    if (!findVersionIndex)
    {
        qDebug() << "cannot find version index of " << timelineBeginVersion;
        return;
    }

    auto versionIndex = findVersionIndex.value();
    auto versionDateTimeRange = score2dx::GetVersionDateTimeRange(versionIndex);
    auto versionBeginDateTime = ToQDateTime(versionDateTimeRange.at(icl_s2::RangeSide::Begin));

    if (mDateTimeAxis)
    {
        auto &dateTimeAxis = *mDateTimeAxis;
        dateTimeAxis.setMin(versionBeginDateTime);
    }

    if (mVersionCategoryAxis)
    {
        auto &versionAxis = *mVersionCategoryAxis;
        versionAxis.setMin(versionBeginDateTime.toMSecsSinceEpoch());
        for (auto &versionName : score2dx::VersionNames)
        {
            versionAxis.remove(versionName.c_str());
        }

        for (auto versionIndex : IndexRange{versionIndex, score2dx::VersionNames.size()})
        {
            auto &versionName = score2dx::VersionNames[versionIndex];

            auto versionDateTimeRange = score2dx::GetVersionDateTimeRange(versionIndex);
            if (versionDateTimeRange.empty())
            {
                continue;
            }

            auto &versionEndDateTime = versionDateTimeRange.at(icl_s2::RangeSide::End);
            auto endMSecs = versionAxis.max();
            if (!versionEndDateTime.empty())
            {
                auto qVersionEndDateTime = ToQDateTime(versionEndDateTime);

                if (qVersionEndDateTime<versionBeginDateTime)
                {
                    continue;
                }

                endMSecs = qVersionEndDateTime.toMSecsSinceEpoch();
            }

            versionAxis.append(versionName.c_str(), endMSecs);
        }

        //qDebug() << "updateTimelineBeginVersion versionAxis count " << versionAxis.count();
    }
}

void
GraphManager::
InitializeChart()
{
    auto findBeginVersionIndex = score2dx::FindVersionIndex("copula");
    if (!findBeginVersionIndex)
    {
        qDebug() << "InitializeChart(): not findBeginVersionIndex";
        return;
    }
    auto beginVersionIndex = findBeginVersionIndex.value();
    auto beginVersionDateTimeRange = score2dx::GetVersionDateTimeRange(beginVersionIndex);
    auto beginDateTime = ToQDateTime(beginVersionDateTimeRange.at(icl_s2::RangeSide::Begin));

    auto latestVersionDateTimeRange = score2dx::GetVersionDateTimeRange(score2dx::GetLatestVersionIndex());
    auto latestVersionBeginDateTime = ToQDateTime(latestVersionDateTimeRange.at(icl_s2::RangeSide::Begin));
    auto endDateTime = latestVersionBeginDateTime.addYears(1);

    if (mLegend)
    {
        auto &legend = mLegend;
        auto markers = legend->markers();
        markers[1]->setVisible(false);
    }

    if (mScoreSeries)
    {
        auto &scoreSeries = *mScoreSeries;
        scoreSeries.setPointLabelsVisible(true);
        scoreSeries.setPointLabelsFormat("@yPoint");
    }

    if (mScatterSeriesScoreLevel)
    {
        auto &scoreLevelSeries = *mScatterSeriesScoreLevel;
        scoreLevelSeries.setVisible(false);
    }

    if (mScoreAxis)
    {
        auto &scoreAxis = *mScoreAxis;
        scoreAxis.setGridLineVisible(false);
        scoreAxis.setLineVisible(false);
        //scoreAxis.setLabelFormat("%d");
        scoreAxis.setMin(0);
        scoreAxis.setMax(4000);
        scoreAxis.setTickCount(2);
        scoreAxis.setLabelsVisible(false);
    }

    if (mScoreLevelAxis)
    {
        auto &scoreLevelAxis = *mScoreLevelAxis;
        scoreLevelAxis.setGridLineVisible(false);
        scoreLevelAxis.setLineVisible(false);
        scoreLevelAxis.setMin(0);
        scoreLevelAxis.setMax(4000);
        scoreLevelAxis.setTickCount(2);
        scoreLevelAxis.setLabelsVisible(false);
    }

    if (mDateTimeAxis)
    {
        auto &dateTimeAxis = *mDateTimeAxis;
        dateTimeAxis.setMin(beginDateTime);
        dateTimeAxis.setMax(endDateTime);
        dateTimeAxis.setFormat("yyyy-MM");
        //dateTimeAxis.setTitleText("DateTime");
        dateTimeAxis.setTickCount(10);
        dateTimeAxis.setGridLineVisible(false);
        dateTimeAxis.setLineVisible(false);
    }

    if (mVersionCategoryAxis)
    {
        auto &versionAxis = *mVersionCategoryAxis;
        versionAxis.setMin(beginDateTime.toMSecsSinceEpoch());
        versionAxis.setMax(endDateTime.toMSecsSinceEpoch());
        //versionAxis.setTitleText("Version");
        versionAxis.setGridLineColor({"cyan"});
        versionAxis.setLineVisible(false);
        versionAxis.setLabelsColor({"cyan"});

        for (auto versionIndex : IndexRange{0, score2dx::VersionNames.size()})
        {
            auto versionDateTimeRange = score2dx::GetVersionDateTimeRange(versionIndex);
            if (versionDateTimeRange.empty())
            {
                continue;
            }

            auto qVersionEndDateTime = endDateTime;
            auto &versionEndDateTime = versionDateTimeRange.at(icl_s2::RangeSide::End);
            if (!versionEndDateTime.empty())
            {
                qVersionEndDateTime = ToQDateTime(versionEndDateTime);
            }

            if (qVersionEndDateTime<beginDateTime)
            {
                continue;
            }

            auto &versionName = score2dx::VersionNames[versionIndex];

            versionAxis.append(versionName.c_str(), qVersionEndDateTime.toMSecsSinceEpoch());
        }

        //qDebug() << "InitializeChart versionAxis count " << versionAxis.count();
    }
}

GraphAnalysisListModel &
GraphManager::
GetGraphAnalysisListModel()
{
    return mGraphAnalysisListModel;
}

ScoreLevelListModel &
GraphManager::
GetScoreLevelListModel()
{
    return mScoreLevelListModel;
}

}
