#pragma once

#include <QObject>
#include <QStandardItemModel>
#include <QStringList>

#include "icl_s2/Common/SmartEnum.hxx"

#include "score2dx/Core/Core.hpp"

#include "gui/Statistics/StatsTableModel.hpp"

namespace gui
{

ICL_S2_SMART_ENUM(StatsTableType,
    Level,
    AllDifficulty,
    VersionDifficulty
);

ICL_S2_SMART_ENUM(StatsColumnType,
    Clear,
    DjLevel,
    ScoreLevel
);

ICL_S2_SMART_ENUM(StatsValueType,
    Count,
    Percentage
);

class StatisticsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList activeVersionList READ getActiveVersionList CONSTANT)
    Q_PROPERTY(QStringList difficultyVersionList READ getDifficultyVersionList NOTIFY difficultyVersionListChanged)

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
                         const QString &versionQStr,
                         const QString &columnTypeQStr,
                         const QString &valueTypeQStr);

        const QStringList & getActiveVersionList() const { return mActiveVersionList; }
        const QStringList & getDifficultyVersionList() const { return mDifficultyVersionList; }

        StatsTableModel &
        GetTableModel();

        StatsTableModel &
        GetHorizontalHeaderModel();

        StatsTableModel &
        GetVerticalHeaderModel();

signals:
        void difficultyVersionListChanged();

private:
    const score2dx::Core &mCore;
    //'' Selectable active version list, [22, 29].
    QStringList mActiveVersionList;
    //'' Selectable versions of stats by difficulty, [0, activeVersion].
    QStringList mDifficultyVersionList;

    //'' Because default Horizontal/VerticalHeaderView cannot customize roles, use 3 table models.
    //! @brief 1 x ColumnCount size TableModel use as horizontal header.
    StatsTableModel mHorizontalHeaderModel;
    //! @brief RowCount x 1 size TableModel use as vertical header.
    StatsTableModel mVerticalHeaderModel;
    StatsTableModel mTableModel;
};

}