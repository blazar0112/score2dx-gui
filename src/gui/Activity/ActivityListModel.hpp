#pragma once

#include <memory>
#include <vector>

#include <QAbstractListModel>

#include "icl_s2/Common/SmartEnum.hxx"

#include "gui/Activity/ChartActivityListModel.hpp"

namespace gui
{

ICL_S2_SMART_ENUM(ActivityDataRole,
    time,
    title,
    version,
    previousPlayCount,
    playCount
);

struct ActivityData
{
    std::array<QVariant, ActivityDataRoleSmartEnum::Size()> Data;
};

//! @brief List of activity of music at time.
class ActivityListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int rowItemCount READ getRowItemCount NOTIFY rowItemCountChanged)

public:
        int getRowItemCount() const { return rowCount(); }

        Q_INVOKABLE
        ChartActivityListModel*
        getChartActivityListModel(int row);

        Q_INVOKABLE
        int
        getTotalIncreasedPlayCount()
        const;

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
        ResetModel(std::vector<ActivityData> &&activityList,
                   std::vector<std::vector<ChartActivityData>> &&musicChartActivityList);

signals:
        void rowItemCountChanged();

private:
    //! @brief Each row represents music activity. Vector of {Index=rowIndex, ActivityData}.
    std::vector<ActivityData> mActivityList;
    //! @brief Each row represents music's chart activity.
    //! Vector of {Index=rowIndex, Vector of {Index=ChartActivityModel row, ChartActivityData}}.
    std::vector<std::vector<ChartActivityData>> mMusicChartActivityList;
};

}
