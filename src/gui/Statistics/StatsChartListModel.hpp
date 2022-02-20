#pragma once

#include <vector>

#include <QAbstractListModel>

#include "ies/Common/SmartEnum.hxx"

namespace gui
{

IES_SMART_ENUM(StatsChartDataRole,
    version,
    clear,
    level,
    difficulty,
    title,
    djLevel,
    score,
    scoreLevelDiff,
    miss,
    careerDiffableBestScoreDiff,
    careerDiffableBestScoreVersion,
    careerDiffableBestScore,
    careerDiffableBestMissDiff,
    careerDiffableBestMissVersion,
    careerDiffableBestMiss
);

//! @brief String to let GUI control display behavior.
//! @note Role:
//!     ToString() if not noted
//!     clear: ToPrettyString(ClearType)
//!     difficulty: ToString(DifficultyAcronym)
struct StatsChartData
{
    std::array<QString, StatsChartDataRoleSmartEnum::Size()> Data;
};

class StatsChartListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int rowItemCount READ getRowItemCount NOTIFY rowItemCountChanged)

public:
        int getRowItemCount() const { return rowCount(); }

        int
        rowCount(const QModelIndex &parent=QModelIndex{})
        const
        override;

        QVariant
        data(const QModelIndex &index, int role = Qt::UserRole)
        const
        override;

        QHash<int, QByteArray>
        roleNames()
        const
        override;

        void
        ResetModel(std::vector<StatsChartData> &&chartList);

signals:
        void rowItemCountChanged();

private:
    //! @brief Vector of {Index=rowIndex, StatsChartData}.
    std::vector<StatsChartData> mChartList;
};

}
