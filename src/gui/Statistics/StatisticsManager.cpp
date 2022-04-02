#include "gui/Statistics/StatisticsManager.hpp"

#include <vector>

#include <QDebug>

#include "ies/Common/IntegralRangeUsing.hpp"
#include "ies/StdUtil/Find.hxx"
#include "ies/Time/TimeUtilFormat.hxx"

#include "fmt/format.h"

#include "score2dx/Iidx/Version.hpp"
#include "score2dx/Analysis/Analyzer.hpp"

namespace s2Time = ies::Time;

namespace
{

//! @todo Move GetColors to QML side.

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
GetColors(score2dx::ScoreLevelCategory scoreLevelCategory)
{
    switch (scoreLevelCategory)
    {
        case score2dx::ScoreLevelCategory::AMinus:
            return {"black", "white"};
        case score2dx::ScoreLevelCategory::AEqPlus:
            return {"white", "#16A085"};
        case score2dx::ScoreLevelCategory::AAMinus:
            return {"white", "#57E1C6"};
        case score2dx::ScoreLevelCategory::AAEqPlus:
            return {"black", "#E6E6E6"};
        case score2dx::ScoreLevelCategory::AAAMinus:
            return {"white", "#CDC17B"};
        case score2dx::ScoreLevelCategory::AAAEqPlus:
            return {"white", "#F5B041"};
        case score2dx::ScoreLevelCategory::MaxMinus:
            return {"white", "red"};
        case score2dx::ScoreLevelCategory::Max:
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

    //! String display in header for role, some role data are combined to one header cell
    //! e.g. 'Lv' Header Cell display 'Lv' from level role, and color use difficulty role.
    static const std::array<std::string, StatsChartDataRoleSmartEnum::Size()> headerStrings
    {
        "Ver",
        "C",
        "Lv",
        "",
        "Title",
        "DJ Level",
        "Score",
        "SL Diff",
        "Miss",
        "PDBS Diff",
        "PDBS Ver",
        "PDB Score",
        "PDBM Diff",
        "PDBM Ver",
        "PDB Miss"
    };

    std::vector<StatsChartData> chartHeader(1);
    auto &statsChartData = chartHeader.front();

    for (auto roleIndex : IndexRange{0, StatsChartDataRoleSmartEnum::Size()})
    {
        statsChartData.Data[roleIndex] = headerStrings[roleIndex].c_str();
    }

    mChartListHeaderModel.ResetModel(std::move(chartHeader));
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
        case StatsColumnType::ScoreLevelCategory:
        {
            columnCount = score2dx::ScoreLevelCategorySmartEnum::Size()+2;
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
        case StatsColumnType::ScoreLevelCategory:
        {
            for (auto category : score2dx::ScoreLevelCategorySmartEnum::ToRange())
            {
                auto column = static_cast<int>(category);
                auto [foregroundColor, backgroundColor] = GetColors(category);

                auto &data = horizontalHeader[0][column].Data;
                data[static_cast<int>(StatsTableDataRole::display)] = ToPrettyString(category).c_str();
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
                case StatsColumnType::ScoreLevelCategory:
                {
                    value = statistics.ChartIdListByScoreLevelCategory.at(static_cast<score2dx::ScoreLevelCategory>(column)).size();
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

    mChartListModel.ResetModel({});
    mChartListFilterList.clear();
    emit chartListFilterListChanged();
}

void
StatisticsManager::
updateChartList(const QString &iidxId,
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

    auto* scoreAnalysisPtr = mCore.FindAnalysis(iidxId.toStdString());
    if (!scoreAnalysisPtr)
    {
        qDebug() << "Cannot find ScoreAnalysis for player " << iidxId;
        return;
    }

    //auto begin = s2Time::Now();
    mChartListFilterList.clear();

    auto &scoreAnalysis = *scoreAnalysisPtr;

    auto playStyle = score2dx::ToPlayStyle(playStyleQStr.toStdString());
    auto tableType = ToStatsTableType(tableTypeQStr.toStdString());
    auto difficultyVersionIndex = difficultyVersionQStr.toULongLong();
    auto statsColumnType = ToStatsColumnType(columnTypeQStr.toStdString());
    auto activeVersionIndex = activeVersionQStr.toULongLong();

    mChartListFilterList << "Active: "+activeVersionQStr;
    mChartListFilterList << QString{"Style: "}+ToString(static_cast<score2dx::PlayStyleAcronym>(playStyle)).c_str();

    const score2dx::Statistics* statisticsPtr = nullptr;

    if (tableRow==mTableModel.rowCount()-1)
    {
        if (tableType!=StatsTableType::VersionDifficulty)
        {
            statisticsPtr = &scoreAnalysis.StatisticsByStyle.at(playStyle);
            mChartListFilterList << "Version: All";
        }
        else
        {
            statisticsPtr = &scoreAnalysis.StatisticsByVersionStyle.at(difficultyVersionIndex).at(playStyle);
            mChartListFilterList << "Version: "+difficultyVersionQStr;
        }
    }
    else
    {
        switch (tableType)
        {
            case StatsTableType::Level:
            {
                statisticsPtr = &scoreAnalysis.StatisticsByStyleLevel.at(playStyle)[tableRow+1];
                mChartListFilterList << "Version: All";
                mChartListFilterList << "Level: "+QString::number(tableRow+1);
                break;
            }
            case StatsTableType::AllDifficulty:
            {
                auto difficulty = static_cast<score2dx::Difficulty>(tableRow+1);
                auto styleDifficulty = score2dx::ConvertToStyleDifficulty(playStyle, difficulty);
                statisticsPtr = &scoreAnalysis.StatisticsByStyleDifficulty.at(styleDifficulty);
                mChartListFilterList << "Version: All";
                mChartListFilterList << QString{"Difficulty: "}+ToString(difficulty).c_str();
                break;
            }
            case StatsTableType::VersionDifficulty:
            {
                auto difficulty = static_cast<score2dx::Difficulty>(tableRow+1);
                auto styleDifficulty = score2dx::ConvertToStyleDifficulty(playStyle, difficulty);
                statisticsPtr = &scoreAnalysis.StatisticsByVersionStyleDifficulty.at(difficultyVersionIndex).at(styleDifficulty);
                mChartListFilterList << "Version: "+difficultyVersionQStr;
                mChartListFilterList << QString{"Difficulty: "}+ToString(difficulty).c_str();
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
        mChartListFilterList << "Chart: All";
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
                mChartListFilterList << "Chart: All";
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
                mChartListFilterList << "Chart: All DJ Level";
                break;
            }
            case StatsColumnType::ScoreLevelCategory:
            {
                for (auto &[category, colChartIdList] : statistics.ChartIdListByScoreLevelCategory)
                {
                    for (auto chartId : colChartIdList)
                    {
                        chartIdList.emplace(chartId);
                    }
                }
                mChartListFilterList << "Chart: All Score Level Category";
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
                auto clear = static_cast<score2dx::ClearType>(tableColumn);
                chartIdList = statistics.ChartIdListByClearType.at(clear);
                mChartListFilterList << QString{"Chart: Clear="}+ToPrettyString(clear).c_str();
                break;
            }
            case StatsColumnType::DjLevel:
            {
                auto djLevel = static_cast<score2dx::DjLevel>(tableColumn);
                chartIdList = statistics.ChartIdListByDjLevel.at(djLevel);
                mChartListFilterList << QString{"Chart: DJ Level="}+ToString(djLevel).c_str();
                break;
            }
            case StatsColumnType::ScoreLevelCategory:
            {
                auto category = static_cast<score2dx::ScoreLevelCategory>(tableColumn);
                chartIdList = statistics.ChartIdListByScoreLevelCategory.at(category);
                mChartListFilterList << QString{"Chart: Score Level Category="}+ToPrettyString(category).c_str();
                break;
            }
        }
    }

    std::vector<StatsChartData> chartList;
    chartList.reserve(chartIdList.size());

    for (auto chartId : chartIdList)
    {
        chartList.emplace_back();
        auto &statsChartData = chartList.back();

        auto &database = mCore.GetMusicDatabase();
        auto [musicId, playStyle, difficulty] = score2dx::ToMusicStyleDiffculty(chartId);
        auto styleDifficulty = score2dx::ConvertToStyleDifficulty(playStyle, difficulty);
        auto [versionIndex, musicIndex] = score2dx::ToIndexes(musicId);

        statsChartData.Data[static_cast<int>(StatsChartDataRole::version)] = score2dx::ToVersionString(versionIndex).c_str();

        auto findBestScoreData = ies::Find(scoreAnalysis.MusicBestScoreData, musicId);
        if (!findBestScoreData)
        {
            qDebug() << "cannot find best score data of music id" << musicId << "[" << ToString(styleDifficulty).c_str() << "].";
            continue;
        }

        auto &bestScoreData = (findBestScoreData.value()->second).at(playStyle);
        auto* chartScorePtr = bestScoreData.GetVersionBestMusicScore().GetChartScore(difficulty);
        if (!chartScorePtr)
        {
            qDebug() << "cannot find chart score music id" << musicId << "[" << ToString(styleDifficulty).c_str() << "].";
            continue;
        }

        auto &chartScore = *chartScorePtr;

        statsChartData.Data[static_cast<int>(StatsChartDataRole::clear)] = ToPrettyString(chartScore.ClearType).c_str();

        //'' intended copy since QString construct below will have dangling problem.
        auto &title = database.GetTitle(musicId);
        if (title.empty())
        {
            qDebug() << "title is empty music id" << musicId << "[" << ToString(styleDifficulty).c_str() << "].";
        }
        auto findChartInfo = database.FindChartInfo(musicId, styleDifficulty, activeVersionIndex);
        if (!findChartInfo)
        {
            qDebug() << "cannot find chart info music id" << musicId << "[" << ToString(styleDifficulty).c_str() << "].";
            continue;
        }

        auto &chartInfo = *findChartInfo;

        auto rangeDiff = score2dx::ToScoreLevelDiffString(chartInfo.Note, chartScore.ExScore);

        statsChartData.Data[static_cast<int>(StatsChartDataRole::level)] = QString::number(chartInfo.Level);
        statsChartData.Data[static_cast<int>(StatsChartDataRole::difficulty)] = ToString(difficulty)[0];
        statsChartData.Data[static_cast<int>(StatsChartDataRole::title)] = title.c_str();

        statsChartData.Data[static_cast<int>(StatsChartDataRole::score)] = QString::number(chartScore.ExScore);
        statsChartData.Data[static_cast<int>(StatsChartDataRole::djLevel)] = ToString(chartScore.DjLevel).c_str();
        if (chartScore.ExScore!=0)
        {
            statsChartData.Data[static_cast<int>(StatsChartDataRole::scoreLevelDiff)] = rangeDiff.c_str();
        }
        else
        {
            statsChartData.Data[static_cast<int>(StatsChartDataRole::scoreLevelDiff)] = "NP";
        }

        //'' intentioned failed with score 0 ?
        statsChartData.Data[static_cast<int>(StatsChartDataRole::miss)] = "N/A";
        if (chartScore.MissCount.has_value())
        {
            statsChartData.Data[static_cast<int>(StatsChartDataRole::miss)] = QString::number(chartScore.MissCount.value());
        }

        statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestScoreDiff)] = "PB";
        statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestScoreVersion)] = "N/A";
        statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestScore)] = "N/A";
        statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestMissDiff)] = "PB";
        statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestMissVersion)] = "N/A";
        statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestMiss)] = "N/A";
        if (!chartScore.MissCount.has_value())
        {
            statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestMissDiff)] = "N/A";
        }

        if (auto* findCareerDiffableBestScore =
                bestScoreData.FindDiffableChartScoreRecord(score2dx::DiffableBestScoreType::ExScore, difficulty))
        {
            auto &careerBestDiffableScoreRecord = *findCareerDiffableBestScore;
            auto scoreDiff = chartScore.ExScore-careerBestDiffableScoreRecord.ChartScoreProp.ExScore;
            auto scoreDiffStr = fmt::format("{:+d}", scoreDiff);

            statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestScoreDiff)] =
                scoreDiffStr.c_str();
            statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestScoreVersion)] =
                score2dx::ToVersionString(careerBestDiffableScoreRecord.VersionIndex).c_str();
            statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestScore)] =
                QString::number(careerBestDiffableScoreRecord.ChartScoreProp.ExScore);
        }

        //'' intented non-hard/exhard failed? is it possible with retire mechanism ?
        if (chartScore.ExScore==0 && !chartScore.MissCount.has_value())
        {
            statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestScoreDiff)] = "NP";
        }

        if (auto* findCareerDiffableBestMiss =
                bestScoreData.FindDiffableChartScoreRecord(score2dx::DiffableBestScoreType::Miss, difficulty))
        {
            auto &careerBestDiffableMissRecord = *findCareerDiffableBestMiss;
            if (!chartScore.MissCount.has_value() || !careerBestDiffableMissRecord.ChartScoreProp.MissCount.has_value())
            {
                qDebug() << "Music id" << musicId << "[" << ToString(styleDifficulty).c_str() << "] find PB differ miss but not both miss available.";
            }

            auto missDiff = chartScore.MissCount.value()-careerBestDiffableMissRecord.ChartScoreProp.MissCount.value();
            auto missDiffStr = fmt::format("{:+d}", missDiff);

            statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestMissDiff)] =
                missDiffStr.c_str();
            statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestMissVersion)] =
                score2dx::ToVersionString(careerBestDiffableMissRecord.VersionIndex).c_str();
            statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestMiss)] =
                QString::number(careerBestDiffableMissRecord.ChartScoreProp.MissCount.value());
        }
        else
        {
            if (auto* findCareerBestMiss = bestScoreData.FindBestChartScoreRecord(score2dx::BestScoreType::BestMiss, difficulty))
            {
                auto &careerBestMiss = *findCareerBestMiss;
                if (!chartScore.MissCount.has_value() && careerBestMiss.ChartScoreProp.MissCount.has_value())
                {
                    statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestMissVersion)] =
                        score2dx::ToVersionString(careerBestMiss.VersionIndex).c_str();
                    statsChartData.Data[static_cast<int>(StatsChartDataRole::careerDiffableBestMiss)] =
                        QString::number(careerBestMiss.ChartScoreProp.MissCount.value());
                }
            }
        }
    }

    mChartListModel.ResetModel(std::move(chartList));
    emit chartListFilterListChanged();

    //s2Time::Print<std::chrono::milliseconds>(s2Time::CountNs(begin), "StatisticsManager::updateChartList");
    std::cout << std::flush;
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

StatsChartListModel &
StatisticsManager::
GetChartListHeaderModel()
{
    return mChartListHeaderModel;
}

StatsChartListModel &
StatisticsManager::
GetChartListModel()
{
    return mChartListModel;
}

}
