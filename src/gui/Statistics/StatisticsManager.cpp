#include "gui/Statistics/StatisticsManager.hpp"

#include <ranges>

#include <QDebug>

#include "icl_s2/Common/IntegralRangeUsing.hpp"

namespace gui
{

StatisticsManager::
StatisticsManager(const score2dx::Core &core, QObject *parent)
:   QObject(parent),
    mCore(core)
{
    auto &activeVersions = mCore.GetMusicDatabase().GetActiveVersions();
    for (auto &[activeVersionIndex, activeVersion] : activeVersions | std::views::reverse)
    {
        mActiveVersionList << QString::number(activeVersionIndex);
    }
}

void
StatisticsManager::
updateStatsTable(const QString &iidxId,
                 const QString &playStyleQStr,
                 const QString &tableType,
                 const QString &version,
                 const QString &columnTypeQStr,
                 const QString &valueType)
{
    if (iidxId.isEmpty()||playStyleQStr.isEmpty()||tableType.isEmpty()
        ||version.isEmpty()||columnTypeQStr.isEmpty()||valueType.isEmpty())
    {
        return;
    }

    auto* scoreAnalysisPtr = mCore.FindAnalysis(iidxId.toStdString());
    if (!scoreAnalysisPtr)
    {
        qDebug() << "Cannot find ScoreAnalysis for player " << iidxId;
        return;
    }

    //auto &scoreAnalysis = *scoreAnalysisPtr;

    mStatsTableModel.clear();
    auto playStyle = score2dx::ToPlayStyle(playStyleQStr.toStdString());

    if (tableType==ToString(StatsTableType::Level).c_str())
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
                mStatsTableModel.setHorizontalHeaderItem(static_cast<int>(clear), new QStandardItem(ToString(clear).c_str()));
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

    //'' placeholder: fill N/A
    //'' ToDo: fill data from score analysis.
    for (auto row : IntRange{0, mStatsTableModel.rowCount()})
    {
        for (auto column : IntRange{0, mStatsTableModel.columnCount()})
        {
            auto item = new QStandardItem("N/A");
            mStatsTableModel.setItem(row, column, item);
        }
    }
}

QStandardItemModel &
StatisticsManager::
GetStatsTableModel()
{
    return mStatsTableModel;
}

}
