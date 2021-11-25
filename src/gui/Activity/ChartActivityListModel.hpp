#pragma once

#include <vector>

#include <QAbstractListModel>

#include "icl_s2/Common/SmartEnum.hxx"

namespace gui
{

//! @note Previous must exist, if has no new record then it's empty string.
ICL_S2_SMART_ENUM(ChartActivityDataRole,
    level,
    difficulty,
    previousClear,
    previousScore,
    previousDjLevel,
    previousScoreLevelDiff,
    previousMiss,
    newRecordClear,
    newRecordScore,
    newRecordDjLevel,
    newRecordScoreLevelDiff,
    newRecordMiss,
    careerDiffableBestScoreDiff,
    careerDiffableBestMissDiff
);

struct ChartActivityData
{
    std::array<QVariant, ChartActivityDataRoleSmartEnum::Size()> Data;
};

class ChartActivityListModel : public QAbstractListModel
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
        ResetModel(std::vector<ChartActivityData> &&chartActivityList);

signals:
        void rowItemCountChanged();

private:
    //! @brief Vector of {Index=rowIndex, ChartActivityData}.
    std::vector<ChartActivityData> mChartActivityList;
};

}
