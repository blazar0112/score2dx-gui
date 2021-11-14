#include "gui/Statistics/StatisticsManager.hpp"

#include <vector>

#include <QDebug>

#include "icl_s2/Common/IntegralRangeUsing.hpp"
#include "icl_s2/StdUtil/Find.hxx"
#include "icl_s2/Time/TimeUtilFormat.hxx"

#include "fmt/format.h"

#include "score2dx/Iidx/Version.hpp"
#include "score2dx/Analysis/Analyzer.hpp"

namespace s2Time = icl_s2::Time;

namespace
{

//! @brief Return {Foreground, Background} color code.
std::pair<QString, QString>
GetColors(score2dx::Difficulty difficulty)
{
    switch (difficulty)
    {
        case score2dx::Difficulty::Beginner:
            return {"white", "#93ff00"};
        case score2dx::Difficulty::Normal:
            return {"white", "#86cfff"};
        case score2dx::Difficulty::Hyper:
            return {"white", "#ffb746"};
        case score2dx::Difficulty::Another:
            return {"white", "#fa5758"};
        case score2dx::Difficulty::Leggendaria:
            return {"white", "#f500ff"};
    }

    return {"black", "white"};
}

//! @brief Return {Foreground, Background} color code.
std::pair<QString, QString>
GetColors(score2dx::ClearType clearType)
{
    switch (clearType)
    {
        case score2dx::ClearType::NO_PLAY:
            return {"gray", "black"};
        case score2dx::ClearType::FAILED:
            return {"red", "black"};
        case score2dx::ClearType::ASSIST_CLEAR:
            return {"white", "purple"};
        case score2dx::ClearType::EASY_CLEAR:
            return {"white", "green"};
        case score2dx::ClearType::CLEAR:
            return {"white", "dodgerblue"};
        case score2dx::ClearType::HARD_CLEAR:
            return {"white", "red"};
        case score2dx::ClearType::EX_HARD_CLEAR:
            return {"red", "yellow"};
        case score2dx::ClearType::FULLCOMBO_CLEAR:
            return {"black", "cyan"};
    }

    return {"black", "white"};
}

//! @brief Return {Foreground, Background} color code.
std::pair<QString, QString>
GetColors(score2dx::DjLevel djLevel)
{
    switch (djLevel)
    {
        case score2dx::DjLevel::F:
        case score2dx::DjLevel::E:
        case score2dx::DjLevel::D:
        case score2dx::DjLevel::C:
        case score2dx::DjLevel::B:
            return {"black", "white"};
        case score2dx::DjLevel::A:
            return {"white", "#16A085"};
        case score2dx::DjLevel::AA:
            return {"black", "#E6E6E6"};
        case score2dx::DjLevel::AAA:
            return {"white", "#F5B041"};
    }

    return {"black", "white"};
}

//! @brief Return {Foreground, Background} color code.
std::pair<QString, QString>
GetColors(score2dx::StatisticScoreLevelRange scoreLevel)
{
    switch (scoreLevel)
    {
        case score2dx::StatisticScoreLevelRange::AMinus:
            return {"black", "white"};
        case score2dx::StatisticScoreLevelRange::AEqPlus:
            return {"white", "#16A085"};
        case score2dx::StatisticScoreLevelRange::AAMinus:
            return {"white", "#57E1C6"};
        case score2dx::StatisticScoreLevelRange::AAEqPlus:
            return {"black", "#E6E6E6"};
        case score2dx::StatisticScoreLevelRange::AAAMinus:
            return {"white", "#CDC17B"};
        case score2dx::StatisticScoreLevelRange::AAAEqPlus:
            return {"white", "#F5B041"};
        case score2dx::StatisticScoreLevelRange::MaxMinus:
            return {"white", "red"};
        case score2dx::StatisticScoreLevelRange::Max:
            return {"black", "cyan"};
    }

    return {"black", "white"};
}

}

namespace gui
{

StatisticsManager::
StatisticsManager(const score2dx::Core &core, QObject *parent)
:   QObject(parent),
    mCore(core)
{
    auto &activeVersions = mCore.GetMusicDatabase().GetActiveVersions();
    auto firstActiveVersionIndex = activeVersions.begin()->first;

    for (auto versionIndex : ReverseIndexRange{firstActiveVersionIndex, mCore.GetActiveVersionIndex()+1})
    {
        mActiveVersionList << score2dx::ToVersionString(versionIndex).c_str();
    }

    static const std::array<std::string, StatsMusicDataRoleSmartEnum::Size()> headerStrings
    {
        "Ver",
        "C",
        "Lv",
        "",
        "Title",
        "DJ Lv",
        "Score",
        "Diff",
        "PB\nVer",
        "PB\nScore"
    };

    std::vector<StatsMusicData> musicHeader(1);
    auto &statsMusicData = musicHeader.front();

    for (auto roleIndex : IndexRange{0, StatsMusicDataRoleSmartEnum::Size()})
    {
        statsMusicData.Data[roleIndex] = headerStrings[roleIndex].c_str();
    }

    mMusicListHeaderModel.ResetModel(std::move(musicHeader));
}

void
StatisticsManager::
updateDifficultyVersionList()
{
    mDifficultyVersionList.clear();
    for (auto versionIndex : ReverseIndexRange{0, mCore.GetActiveVersionIndex()+1})
    {
        mDifficultyVersionList << score2dx::ToVersionString(versionIndex).c_str();
    }
    emit difficultyVersionListChanged();
}

void
StatisticsManager::
updateStatsTable(const QString &iidxId,
                 const QString &playStyleQStr,
                 const QString &tableTypeQStr,
                 const QString &difficultyVersionQStr,
                 const QString &columnTypeQStr,
                 const QString &valueTypeQStr)
{
    if (iidxId.isEmpty()||playStyleQStr.isEmpty()||tableTypeQStr.isEmpty()
        ||difficultyVersionQStr.isEmpty()||columnTypeQStr.isEmpty()||valueTypeQStr.isEmpty())
    {
        return;
    }

    auto* scoreAnalysisPtr = mCore.FindAnalysis(iidxId.toStdString());
    if (!scoreAnalysisPtr)
    {
        qDebug() << "Cannot find ScoreAnalysis for player " << iidxId;
        return;
    }

    auto &scoreAnalysis = *scoreAnalysisPtr;

    auto playStyle = score2dx::ToPlayStyle(playStyleQStr.toStdString());
    auto tableType = ToStatsTableType(tableTypeQStr.toStdString());

    std::size_t rowCount = 0;
    std::size_t columnCount = 0;

    //'' Table Rows: rows from tableType's target enum, plus ColSum row.
    if (tableType==StatsTableType::Level)
    {
        rowCount = static_cast<std::size_t>(score2dx::MaxLevel+1);
    }
    else
    {
        //'' exclude Beginner.
        rowCount = score2dx::DifficultySmartEnum::Size()-1+1;
    }

    //'' Table Columns: columns from columnType's target enum, plus RowSum and Total columns.
    auto statsColumnType = ToStatsColumnType(columnTypeQStr.toStdString());
    switch (statsColumnType)
    {
        case StatsColumnType::Clear:
        {
            columnCount = score2dx::ClearTypeSmartEnum::Size()+2;
            break;
        }
        case StatsColumnType::DjLevel:
        {
            columnCount = score2dx::DjLevelSmartEnum::Size()+2;
            break;
        }
        case StatsColumnType::ScoreLevel:
        {
            columnCount = score2dx::StatisticScoreLevelRangeSmartEnum::Size()+2;
            break;
        }
    }

    std::vector<std::vector<StatsTableData>> horizontalHeader(1, std::vector<StatsTableData>(columnCount));
    std::vector<std::vector<StatsTableData>> verticalHeader(rowCount, std::vector<StatsTableData>(1));
    std::vector<std::vector<StatsTableData>> table(rowCount, std::vector<StatsTableData>(columnCount));

    //'' Setup vertical header for each row.
    if (tableType==StatsTableType::Level)
    {
        for (auto row : IndexRange{0, score2dx::MaxLevel})
        {
            auto &data = verticalHeader[row][0].Data;
            data[static_cast<int>(StatsTableDataRole::display)] = QString::number(row+1);
            data[static_cast<int>(StatsTableDataRole::foreground)] = "black";
            data[static_cast<int>(StatsTableDataRole::background)] = "#85C1E9";
        }
    }
    else
    {
        for (auto difficulty : score2dx::DifficultySmartEnum::ToRange())
        {
            if (difficulty==score2dx::Difficulty::Beginner) { continue; }
            auto styleDifficulty = score2dx::ConvertToStyleDifficulty(playStyle, difficulty);
            auto [foregroundColor, backgroundColor] = GetColors(difficulty);
            auto row = static_cast<int>(difficulty)-1;

            auto &data = verticalHeader[row][0].Data;
            data[static_cast<int>(StatsTableDataRole::display)] = ToString(styleDifficulty).c_str();
            data[static_cast<int>(StatsTableDataRole::foreground)] = foregroundColor;
            data[static_cast<int>(StatsTableDataRole::background)] = backgroundColor;
        }
    }
    verticalHeader[rowCount-1][0].Data[static_cast<int>(StatsTableDataRole::display)] = "ColSum";
    verticalHeader[rowCount-1][0].Data[static_cast<int>(StatsTableDataRole::foreground)] = "white";
    verticalHeader[rowCount-1][0].Data[static_cast<int>(StatsTableDataRole::background)] = "#5D6D7E";

    //'' Setup horizontal header for each column.
    switch (statsColumnType)
    {
        case StatsColumnType::Clear:
        {
            for (auto clear : score2dx::ClearTypeSmartEnum::ToRange())
            {
                auto column = static_cast<int>(clear);
                auto [foregroundColor, backgroundColor] = GetColors(clear);

                auto &data = horizontalHeader[0][column].Data;
                data[static_cast<int>(StatsTableDataRole::display)] = ToPrettyString(clear).c_str();
                data[static_cast<int>(StatsTableDataRole::foreground)] = foregroundColor;
                data[static_cast<int>(StatsTableDataRole::background)] = backgroundColor;
            }
            break;
        }
        case StatsColumnType::DjLevel:
        {
            for (auto djLevel : score2dx::DjLevelSmartEnum::ToRange())
            {
                auto column = static_cast<int>(djLevel);
                auto [foregroundColor, backgroundColor] = GetColors(djLevel);

                auto &data = horizontalHeader[0][column].Data;
                data[static_cast<int>(StatsTableDataRole::display)] = ToString(djLevel).c_str();
                data[static_cast<int>(StatsTableDataRole::foreground)] = foregroundColor;
                data[static_cast<int>(StatsTableDataRole::background)] = backgroundColor;
            }
            break;
        }
        case StatsColumnType::ScoreLevel:
        {
            for (auto scoreLevel : score2dx::StatisticScoreLevelRangeSmartEnum::ToRange())
            {
                auto column = static_cast<int>(scoreLevel);
                auto [foregroundColor, backgroundColor] = GetColors(scoreLevel);

                auto &data = horizontalHeader[0][column].Data;
                data[static_cast<int>(StatsTableDataRole::display)] = ToPrettyString(scoreLevel).c_str();
                data[static_cast<int>(StatsTableDataRole::foreground)] = foregroundColor;
                data[static_cast<int>(StatsTableDataRole::background)] = backgroundColor;
            }
            break;
        }
    }
    //'' RowSum column:
    horizontalHeader[0][columnCount-2].Data[static_cast<int>(StatsTableDataRole::display)] = "RowSum";
    horizontalHeader[0][columnCount-2].Data[static_cast<int>(StatsTableDataRole::foreground)] = "white";
    horizontalHeader[0][columnCount-2].Data[static_cast<int>(StatsTableDataRole::background)] = "#5D6D7E";
    //'' Total column:
    horizontalHeader[0][columnCount-1].Data[static_cast<int>(StatsTableDataRole::display)] = "Total";
    horizontalHeader[0][columnCount-1].Data[static_cast<int>(StatsTableDataRole::foreground)] = "black";
    horizontalHeader[0][columnCount-1].Data[static_cast<int>(StatsTableDataRole::background)] = "#F5B7B1";

    //'' Total column (the rightest column) alwasy display count.
    //'' value in each cell and sums can choose count or percentage.
    //! @brief Sum of each row cell value, display at right side, size = rowCount-1 (exclude ColSum row).
    std::vector<std::size_t> rowSums(rowCount-1, 0);
    //! @brief Total of each row cell count, display at right side, size = rowCount-1 (exclude ColSum row).
    std::vector<std::size_t> rowTotalCounts(rowCount-1, 0);
    //! @brief Sum of each column cell value, display at bottom side, size = columnCount-1.
    //! (include RowSum column, exclude Total column)
    std::vector<std::size_t> columnSums(columnCount-1, 0);

    auto difficultyVersionIndex = difficultyVersionQStr.toULongLong();
    auto valueType = ToStatsValueType(valueTypeQStr.toStdString());

    auto ToValueText = [](StatsValueType valueType, std::size_t value, std::size_t count)
    -> QString
    {
        double percentage = 0.0;
        if (count!=0)
        {
            percentage = static_cast<double>(value)*100/count;
        }
        auto text = QString::number(value);
        if (valueType==StatsValueType::Percentage)
        {
            text = fmt::format("{:.2f}%", percentage).c_str();
        }

        return text;
    };

    //'' Setup table content:
    for (auto row : IndexRange{0, rowCount-1})
    {
        const score2dx::Statistics* statisticsPtr = nullptr;
        switch (tableType)
        {
            case StatsTableType::Level:
            {
                statisticsPtr = &scoreAnalysis.StatisticsByStyleLevel.at(playStyle)[row+1];
                break;
            }
            case StatsTableType::AllDifficulty:
            {
                auto styleDifficulty = score2dx::ConvertToStyleDifficulty(playStyle, static_cast<score2dx::Difficulty>(row+1));
                statisticsPtr = &scoreAnalysis.StatisticsByStyleDifficulty.at(styleDifficulty);
                break;
            }
            case StatsTableType::VersionDifficulty:
            {
                auto styleDifficulty = score2dx::ConvertToStyleDifficulty(playStyle, static_cast<score2dx::Difficulty>(row+1));
                statisticsPtr = &scoreAnalysis.StatisticsByVersionStyleDifficulty.at(difficultyVersionIndex).at(styleDifficulty);
                break;
            }
        }

        if (!statisticsPtr)
        {
            throw std::runtime_error("statisticsPtr is nullptr.");
        }

        auto &statistics = *statisticsPtr;
        rowTotalCounts[row] = statistics.ChartIdList.size();

        for (auto column : IndexRange{0, columnCount-2})
        {
            std::size_t value = 0;

            switch (statsColumnType)
            {
                case StatsColumnType::Clear:
                {
                    value = statistics.ChartIdListByClearType.at(static_cast<score2dx::ClearType>(column)).size();
                    break;
                }
                case StatsColumnType::DjLevel:
                {
                    value = statistics.ChartIdListByDjLevel.at(static_cast<score2dx::DjLevel>(column)).size();
                    break;
                }
                case StatsColumnType::ScoreLevel:
                {
                    value = statistics.ChartIdListByScoreLevelRange.at(static_cast<score2dx::StatisticScoreLevelRange>(column)).size();
                    break;
                }
            }

            columnSums[column] += value;
            rowSums[row] += value;
            auto text = ToValueText(valueType, value, rowTotalCounts[row]);

            auto &data = table[row][column].Data;
            data[static_cast<int>(StatsTableDataRole::display)] = text;
            //data[static_cast<int>(StatsTableDataRole::foreground)] = foregroundColor;
            //data[static_cast<int>(StatsTableDataRole::background)] = backgroundColor;
        }

        auto value = rowSums[row];
        columnSums[columnCount-2] += value;
        auto text = ToValueText(valueType, value, rowTotalCounts[row]);

        auto &rowSumData = table[row][columnCount-2].Data;
        rowSumData[static_cast<int>(StatsTableDataRole::display)] = text;
        //rowSumData[static_cast<int>(StatsTableDataRole::foreground)] = foregroundColor;
        //rowSumData[static_cast<int>(StatsTableDataRole::background)] = backgroundColor;

        auto &rowCountData = table[row][columnCount-1].Data;
        rowCountData[static_cast<int>(StatsTableDataRole::display)] = QString::number(rowTotalCounts[row]);
        //rowCountData[static_cast<int>(StatsTableDataRole::foreground)] = foregroundColor;
        //rowCountData[static_cast<int>(StatsTableDataRole::background)] = backgroundColor;
    }

    auto totalCount = std::accumulate(rowTotalCounts.begin(), rowTotalCounts.end(), std::size_t{0});

    for (auto column : IndexRange{0, columnCount-1})
    {
        auto value = columnSums[column];
        auto text = ToValueText(valueType, value, totalCount);

        auto &data = table[rowCount-1][column].Data;
        data[static_cast<int>(StatsTableDataRole::display)] = text;
        //data[static_cast<int>(StatsTableDataRole::foreground)] = foregroundColor;
        //data[static_cast<int>(StatsTableDataRole::background)] = backgroundColor;
    }

    auto &data = table[rowCount-1][columnCount-1].Data;
    data[static_cast<int>(StatsTableDataRole::display)] = QString::number(totalCount);
    //data[static_cast<int>(StatsTableDataRole::foreground)] = foregroundColor;
    //data[static_cast<int>(StatsTableDataRole::background)] = backgroundColor;

    mHorizontalHeaderModel.ResetModel(std::move(horizontalHeader));
    mVerticalHeaderModel.ResetModel(std::move(verticalHeader));
    mTableModel.ResetModel(std::move(table));

    mMusicListModel.ResetModel({});
}

void
StatisticsManager::
updateMusicList(const QString &iidxId,
                const QString &playStyleQStr,
                const QString &tableTypeQStr,
                const QString &difficultyVersionQStr,
                const QString &columnTypeQStr,
                const QString &activeVersionQStr,
                int tableRow,
                int tableColumn)
{
    if (iidxId.isEmpty()||playStyleQStr.isEmpty()||tableTypeQStr.isEmpty()
        ||difficultyVersionQStr.isEmpty()||columnTypeQStr.isEmpty()||activeVersionQStr.isEmpty())
    {
        return;
    }

    qDebug() << "StatisticsManager::updateMusicList row " << tableRow << "column" << tableColumn;
    auto* scoreAnalysisPtr = mCore.FindAnalysis(iidxId.toStdString());
    if (!scoreAnalysisPtr)
    {
        qDebug() << "Cannot find ScoreAnalysis for player " << iidxId;
        return;
    }

    //auto begin = s2Time::Now();

    auto &scoreAnalysis = *scoreAnalysisPtr;

    auto playStyle = score2dx::ToPlayStyle(playStyleQStr.toStdString());
    auto tableType = ToStatsTableType(tableTypeQStr.toStdString());
    auto difficultyVersionIndex = difficultyVersionQStr.toULongLong();
    auto statsColumnType = ToStatsColumnType(columnTypeQStr.toStdString());
    auto activeVersionIndex = activeVersionQStr.toULongLong();

    const score2dx::Statistics* statisticsPtr = nullptr;

    if (tableRow==mTableModel.rowCount()-1)
    {
        if (tableType!=StatsTableType::VersionDifficulty)
        {
            statisticsPtr = &scoreAnalysis.StatisticsByStyle.at(playStyle);
        }
        else
        {
            statisticsPtr = &scoreAnalysis.StatisticsByVersionStyle.at(difficultyVersionIndex).at(playStyle);
        }
    }
    else
    {
        switch (tableType)
        {
            case StatsTableType::Level:
            {
                statisticsPtr = &scoreAnalysis.StatisticsByStyleLevel.at(playStyle)[tableRow+1];
                break;
            }
            case StatsTableType::AllDifficulty:
            {
                auto styleDifficulty = score2dx::ConvertToStyleDifficulty(playStyle, static_cast<score2dx::Difficulty>(tableRow+1));
                statisticsPtr = &scoreAnalysis.StatisticsByStyleDifficulty.at(styleDifficulty);
                break;
            }
            case StatsTableType::VersionDifficulty:
            {
                auto styleDifficulty = score2dx::ConvertToStyleDifficulty(playStyle, static_cast<score2dx::Difficulty>(tableRow+1));
                statisticsPtr = &scoreAnalysis.StatisticsByVersionStyleDifficulty.at(difficultyVersionIndex).at(styleDifficulty);
                break;
            }
        }
    }

    if (!statisticsPtr)
    {
        throw std::runtime_error("statisticsPtr is nullptr.");
    }

    auto &statistics = *statisticsPtr;

    std::set<std::size_t> chartIdList;

    if (tableColumn==mTableModel.columnCount()-1)
    {
        chartIdList = statistics.ChartIdList;
    }
    else if (tableColumn==mTableModel.columnCount()-2)
    {
        switch (statsColumnType)
        {
            case StatsColumnType::Clear:
            {
                for (auto &[clearType, colChartIdList] : statistics.ChartIdListByClearType)
                {
                    for (auto chartId : colChartIdList)
                    {
                        chartIdList.emplace(chartId);
                    }
                }
                break;
            }
            case StatsColumnType::DjLevel:
            {
                for (auto &[djLevel, colChartIdList] : statistics.ChartIdListByDjLevel)
                {
                    for (auto chartId : colChartIdList)
                    {
                        chartIdList.emplace(chartId);
                    }
                }
                break;
            }
            case StatsColumnType::ScoreLevel:
            {
                for (auto &[scoreLevel, colChartIdList] : statistics.ChartIdListByScoreLevelRange)
                {
                    for (auto chartId : colChartIdList)
                    {
                        chartIdList.emplace(chartId);
                    }
                }
                break;
            }
        }
    }
    else
    {
        switch (statsColumnType)
        {
            case StatsColumnType::Clear:
            {
                chartIdList = statistics.ChartIdListByClearType.at(static_cast<score2dx::ClearType>(tableColumn));
                break;
            }
            case StatsColumnType::DjLevel:
            {
                chartIdList = statistics.ChartIdListByDjLevel.at(static_cast<score2dx::DjLevel>(tableColumn));
                break;
            }
            case StatsColumnType::ScoreLevel:
            {
                chartIdList = statistics.ChartIdListByScoreLevelRange.at(static_cast<score2dx::StatisticScoreLevelRange>(tableColumn));
                break;
            }
        }
    }

    std::vector<StatsMusicData> musicList;
    musicList.reserve(chartIdList.size());

    for (auto chartId : chartIdList)
    {
        musicList.emplace_back();
        auto &statsMusicData = musicList.back();

        auto &database = mCore.GetMusicDatabase();
        auto [musicId, playStyle, difficulty] = score2dx::ToMusicStyleDiffculty(chartId);
        auto styleDifficulty = score2dx::ConvertToStyleDifficulty(playStyle, difficulty);
        auto [versionIndex, musicIndex] = score2dx::ToIndexes(musicId);

        statsMusicData.Data[static_cast<int>(StatsMusicDataRole::version)] = score2dx::ToVersionString(versionIndex).c_str();

        auto findBestScoreData = icl_s2::Find(scoreAnalysis.MusicBestScoreData, musicId);
        if (!findBestScoreData)
        {
            qDebug() << "cannot find best score data of music id" << musicId;
            continue;
        }

        auto &bestScoreData = (findBestScoreData.value()->second).at(playStyle);
        auto findChartScore = bestScoreData.VersionBestMusicScore.FindChartScore(difficulty);
        if (!findChartScore)
        {
            qDebug() << "cannot find chart score music id" << musicId << "[" << ToString(styleDifficulty).c_str() << "].";
            continue;
        }

        auto &chartScore = *findChartScore;

        statsMusicData.Data[static_cast<int>(StatsMusicDataRole::clear)] = ToPrettyString(chartScore.ClearType).c_str();

        auto &title = database.GetLatestMusicInfo(musicId).GetField(score2dx::MusicInfoField::Title);
        if (title.empty())
        {
            qDebug() << "title is empty music id" << musicId << "[" << ToString(styleDifficulty).c_str() << "].";
        }
        auto findChartInfo = database.FindChartInfo(versionIndex, title, styleDifficulty, activeVersionIndex);
        if (!findChartInfo)
        {
            qDebug() << "cannot find chart info music id" << musicId << "[" << ToString(styleDifficulty).c_str() << "].";
            continue;
        }

        auto &chartInfo = findChartInfo.value();

        statsMusicData.Data[static_cast<int>(StatsMusicDataRole::level)] = QString::number(chartInfo.Level);
        statsMusicData.Data[static_cast<int>(StatsMusicDataRole::difficulty)] = ToString(difficulty)[0];
        statsMusicData.Data[static_cast<int>(StatsMusicDataRole::title)] = title.c_str();
        statsMusicData.Data[static_cast<int>(StatsMusicDataRole::djLevel)] = ToString(chartScore.DjLevel).c_str();
        statsMusicData.Data[static_cast<int>(StatsMusicDataRole::score)] = QString::number(chartScore.ExScore);

        auto findCareerBest = icl_s2::Find(bestScoreData.CareerBestChartScores, difficulty);
        if (!findCareerBest)
        {
            qDebug() << "cannot find career best score of music id" << musicId << "[" << ToString(styleDifficulty).c_str() << "].";
            continue;
        }
        auto &careerBestChartScore = findCareerBest.value()->second;
        auto scoreDiff = careerBestChartScore.BestChartScore.ExScore-chartScore.ExScore;
        if (scoreDiff<0)
        {
            qDebug() << "career best score is not better, music id" << musicId
                     << "[" << ToString(styleDifficulty).c_str()
                     << "], PB = " << careerBestChartScore.BestChartScore.ExScore
                     << ", current = " << chartScore.ExScore;
            continue;
        }

        auto scoreDiffStr = "-"+QString::number(scoreDiff);
        if (scoreDiff==0)
        {
            scoreDiffStr = "PB";
        }

        statsMusicData.Data[static_cast<int>(StatsMusicDataRole::bestScoreDiff)] = scoreDiffStr;
        statsMusicData.Data[static_cast<int>(StatsMusicDataRole::careerBestVersion)] = score2dx::ToVersionString(careerBestChartScore.VersionIndex).c_str();
        statsMusicData.Data[static_cast<int>(StatsMusicDataRole::careerBestScore)] = QString::number(careerBestChartScore.BestChartScore.ExScore);
    }

    mMusicListModel.ResetModel(std::move(musicList));

    //s2Time::Print<std::chrono::milliseconds>(s2Time::CountNs(begin), "StatisticsManager::updateMusicList");
    //std::cout << std::flush;
}

StatsTableModel &
StatisticsManager::
GetHorizontalHeaderModel()
{
    return mHorizontalHeaderModel;
}

StatsTableModel &
StatisticsManager::
GetVerticalHeaderModel()
{
    return mVerticalHeaderModel;
}

StatsTableModel &
StatisticsManager::
GetTableModel()
{
    return mTableModel;
}

StatsMusicListModel &
StatisticsManager::
GetMusicListHeaderModel()
{
    return mMusicListHeaderModel;
}

StatsMusicListModel &
StatisticsManager::
GetMusicListModel()
{
    return mMusicListModel;
}

}
