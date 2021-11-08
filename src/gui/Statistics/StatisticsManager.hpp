#pragma once

#include <QObject>
#include <QStandardItemModel>
#include <QStringList>

#include "icl_s2/Common/SmartEnum.hxx"

#include "score2dx/Core/Core.hpp"

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
    //Q_PROPERTY(QStandardItemModel statsTableModel READ getStatsTableModel NOTIFY statsTableModelChanged)

public:
        explicit StatisticsManager(const score2dx::Core &core, QObject* parent=nullptr);

    //! @brief Update stats table from iidxId's previous analyzed ScoreAnalysis.
        Q_INVOKABLE
        void
        updateStatsTable(const QString &iidxId,
                         const QString &playStyleQStr,
                         const QString &tableType,
                         const QString &version,
                         const QString &columnTypeQStr,
                         const QString &valueType);

        const QStringList & getActiveVersionList() const { return mActiveVersionList; }

        QStandardItemModel &
        GetStatsTableModel();

signals:
        void statsTableModelChanged();

private:
    const score2dx::Core &mCore;
    QStringList mActiveVersionList;
    QStandardItemModel mStatsTableModel;
};

}
