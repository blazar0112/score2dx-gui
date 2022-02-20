#pragma once

#include <QObject>
#include <QStandardItemModel>
#include <QStringList>

#include "ies/Common/SmartEnum.hxx"

#include "score2dx/Core/Core.hpp"

#include "gui/Statistics/StatsTableModel.hpp"
#include "gui/Statistics/StatsChartListModel.hpp"

namespace gui
{

IES_SMART_ENUM(StatsTableType,
    Level,
    AllDifficulty,
    VersionDifficulty
);

IES_SMART_ENUM(StatsColumnType,
    Clear,
    DjLevel,
    ScoreLevelCategory
);

IES_SMART_ENUM(StatsValueType,
    Count,
    Percentage
);

class StatisticsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList activeVersionList READ getActiveVersionList CONSTANT)
    Q_PROPERTY(QStringList difficultyVersionList READ getDifficultyVersionList NOTIFY difficultyVersionListChanged)
    Q_PROPERTY(QStringList chartListFilterList READ getChartListFilterList NOTIFY chartListFilterListChanged)

public:
        explicit StatisticsManager(const score2dx::Core &core, QObject* parent=nullptr);

        Q_INVOKABLE
        void
        updateDifficultyVersionList();

    //! @brief Update stats table from iidxId's previous analyzed ScoreAnalysis.
        Q_INVOKABLE
        void
        updateStatsTable(const QString &iidxId,
                         const QString &playStyleQStr,
                         const QString &tableTypeQStr,
                         const QString &difficultyVersionQStr,
                         const QString &columnTypeQStr,
                         const QString &valueTypeQStr);

        Q_INVOKABLE
        void
        updateChartList(const QString &iidxId,
                        const QString &playStyleQStr,
                        const QString &tableTypeQStr,
                        const QString &difficultyVersionQStr,
                        const QString &columnTypeQStr,
                        const QString &activeVersionQStr,
                        int tableRow,
                        int tableColumn);

        const QStringList & getActiveVersionList() const { return mActiveVersionList; }
        const QStringList & getDifficultyVersionList() const { return mDifficultyVersionList; }
        const QStringList & getChartListFilterList() const { return mChartListFilterList; }

        StatsTableModel &
        GetHorizontalHeaderModel();

        StatsTableModel &
        GetVerticalHeaderModel();

        StatsTableModel &
        GetTableModel();

        StatsChartListModel &
        GetChartListHeaderModel();

        StatsChartListModel &
        GetChartListModel();

signals:
        void difficultyVersionListChanged();
        void chartListFilterListChanged();

private:
    const score2dx::Core &mCore;
    //! @brief Selectable active version list, [22, 29].
    QStringList mActiveVersionList;
    //! @brief Selectable versions of stats by difficulty, [0, activeVersion].
    QStringList mDifficultyVersionList;
    //! @brief Filter applied to generate chart list.
    QStringList mChartListFilterList;

    //'' Because default Horizontal/VerticalHeaderView cannot customize roles, use 3 table models.
    //! @brief 1 x ColumnCount size TableModel use as horizontal header.
    StatsTableModel mHorizontalHeaderModel;
    //! @brief RowCount x 1 size TableModel use as vertical header.
    StatsTableModel mVerticalHeaderModel;
    StatsTableModel mTableModel;

    StatsChartListModel mChartListHeaderModel;
    StatsChartListModel mChartListModel;
};

}
