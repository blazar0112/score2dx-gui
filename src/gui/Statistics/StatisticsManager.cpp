#include "gui/Statistics/StatisticsManager.hpp"

#include <vector>

#include <QDebug>

#include "icl_s2/Common/IntegralRangeUsing.hpp"

#include "fmt/format.h"

#include "score2dx/Iidx/Version.hpp"

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
                 const QString &versionQStr,
                 const QString &columnTypeQStr,
                 const QString &valueTypeQStr)
{
    if (iidxId.isEmpty()||playStyleQStr.isEmpty()||tableTypeQStr.isEmpty()
        ||versionQStr.isEmpty()||columnTypeQStr.isEmpty()||valueTypeQStr.isEmpty())
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

    mStatsTableModel.clear();
    auto playStyle = score2dx::ToPlayStyle(playStyleQStr.toStdString());
    auto tableType = ToStatsTableType(tableTypeQStr.toStdString());

    if (tableType==StatsTableType::Level)
    {
        mStatsTableModel.setRowCount(score2dx::MaxLevel+1);
        for (auto row : IntRange{0, score2dx::MaxLevel})
        {
            mStatsTableModel.setVerticalHeaderItem(row, new QStandardItem(QString::number(row+1)));
        }
        mStatsTableModel.setVerticalHeaderItem(score2dx::MaxLevel, new QStandardItem("Total"));
    }
    else
    {
        //'' exclude Beginner, but add Total.
        mStatsTableModel.setRowCount(score2dx::DifficultySmartEnum::Size());
        int row = 0;
        for (auto difficulty : score2dx::DifficultySmartEnum::ToRange())
        {
            if (difficulty==score2dx::Difficulty::Beginner) { continue; }
            auto styleDifficulty = score2dx::ConvertToStyleDifficulty(playStyle, difficulty);
            mStatsTableModel.setVerticalHeaderItem(row, new QStandardItem(ToString(styleDifficulty).c_str()));
            ++row;
        }
        mStatsTableModel.setVerticalHeaderItem(row, new QStandardItem("Total"));
    }

    auto statsColumnType = ToStatsColumnType(columnTypeQStr.toStdString());
    switch (statsColumnType)
    {
        case StatsColumnType::Clear:
        {
            mStatsTableModel.setColumnCount(score2dx::ClearTypeSmartEnum::Size()+1);
            for (auto clear : score2dx::ClearTypeSmartEnum::ToRange())
            {
                mStatsTableModel.setHorizontalHeaderItem(static_cast<int>(clear), new QStandardItem(ToPrettyString(clear).c_str()));
            }
            mStatsTableModel.setHorizontalHeaderItem(score2dx::ClearTypeSmartEnum::Size(), new QStandardItem("Total"));
            break;
        }
        case StatsColumnType::DjLevel:
        {
            mStatsTableModel.setColumnCount(score2dx::DjLevelSmartEnum::Size()+1);
            for (auto djLevel : score2dx::DjLevelSmartEnum::ToRange())
            {
                mStatsTableModel.setHorizontalHeaderItem(static_cast<int>(djLevel), new QStandardItem(ToString(djLevel).c_str()));
            }
            mStatsTableModel.setHorizontalHeaderItem(score2dx::DjLevelSmartEnum::Size(), new QStandardItem("Total"));
            break;
        }
        case StatsColumnType::ScoreLevel:
        {
            mStatsTableModel.setColumnCount(score2dx::StatisticScoreLevelRangeSmartEnum::Size()+1);
            for (auto scoreLevel : score2dx::StatisticScoreLevelRangeSmartEnum::ToRange())
            {
                mStatsTableModel.setHorizontalHeaderItem(static_cast<int>(scoreLevel), new QStandardItem(ToPrettyString(scoreLevel).c_str()));
            }
            mStatsTableModel.setHorizontalHeaderItem(score2dx::StatisticScoreLevelRangeSmartEnum::Size(), new QStandardItem("Total"));
            break;
        }
    }

    qDebug() << "mStatsTableModel row " << mStatsTableModel.rowCount() << ", column " << mStatsTableModel.columnCount();

    //'' column of rowSums and total sum (the rightest column) alwasy display count.
    //'' value in each cell and rowSums can choose count or percentage.
    //! @brief Sum of each row, display at right side, size = rowCount-1 (exclude row of columnSums).
    std::vector<std::size_t> rowSums(mStatsTableModel.rowCount()-1, 0);
    //! @brief Sum of each column, display at bottom side, size = columnCount-1 (exclude column of rowSums).
    std::vector<std::size_t> columnSums(mStatsTableModel.columnCount()-1, 0);
    std::size_t totalSum = 0;

    auto versionIndex = versionQStr.toULongLong();
    auto valueType = ToStatsValueType(valueTypeQStr.toStdString());

    for (auto row : IntRange{0, mStatsTableModel.rowCount()-1})
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
                statisticsPtr = &scoreAnalysis.StatisticsByVersionStyleDifficulty.at(versionIndex).at(styleDifficulty);
                break;
            }
        }

        if (!statisticsPtr)
        {
            throw std::runtime_error("statisticsPtr is nullptr.");
        }

        auto &statistics = *statisticsPtr;
        rowSums[row] = statistics.ChartIdList.size();
        totalSum += rowSums[row];

        for (auto column : IntRange{0, mStatsTableModel.columnCount()-1})
        {
            std::size_t count = 0;

            switch (statsColumnType)
            {
                case StatsColumnType::Clear:
                {
                    count = statistics.ChartIdListByClearType.at(static_cast<score2dx::ClearType>(column)).size();
                    break;
                }
                case StatsColumnType::DjLevel:
                {
                    count = statistics.ChartIdListByDjLevel.at(static_cast<score2dx::DjLevel>(column)).size();
                    break;
                }
                case StatsColumnType::ScoreLevel:
                {
                    count = statistics.ChartIdListByScoreLevelRange.at(static_cast<score2dx::StatisticScoreLevelRange>(column)).size();
                    break;
                }
            }

            columnSums[column] += count;

            double percentage = 0.0;
            if (rowSums[row]!=0)
            {
                percentage = static_cast<double>(count)*100/rowSums[row];
            }
            auto text = QString::number(count);
            if (valueType==StatsValueType::Percentage)
            {
                text = fmt::format("{:.2f}%", percentage).c_str();
            }
            auto item = new QStandardItem(text);
            mStatsTableModel.setItem(row, column, item);
        }

        auto item = new QStandardItem(QString::number(rowSums[row]));
        mStatsTableModel.setItem(row, mStatsTableModel.columnCount()-1, item);
    }

    for (auto column : IntRange{0, mStatsTableModel.columnCount()-1})
    {
        auto count = columnSums[column];
        double percentage = 0.0;
        if (totalSum!=0)
        {
            percentage = static_cast<double>(count)*100/totalSum;
        }
        auto text = QString::number(count);
        if (valueType==StatsValueType::Percentage)
        {
            text = fmt::format("{:.2f}%", percentage).c_str();
        }
        auto item = new QStandardItem(text);
        mStatsTableModel.setItem(mStatsTableModel.rowCount()-1, column, item);
    }

    mStatsTableModel.setItem(
        mStatsTableModel.rowCount()-1,
        mStatsTableModel.columnCount()-1,
        new QStandardItem(QString::number(totalSum))
    );
}

QStandardItemModel &
StatisticsManager::
GetStatsTableModel()
{
    return mStatsTableModel;
}

}
