#pragma once

#include <memory>
#include <vector>

#include <QAbstractListModel>

#include "icl_s2/Common/SmartEnum.hxx"

#include "gui/Activity/MusicActivityListModel.hpp"

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

class ActivityListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int rowItemCount READ getRowItemCount NOTIFY rowItemCountChanged)

public:
        ~ActivityListModel();

        int getRowItemCount() const { return rowCount(); }

        Q_INVOKABLE
        MusicActivityListModel*
        getMusicActivityListModel(int row);

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
                   std::vector<std::vector<MusicActivityData>> &&musicActivityList);

signals:
        void rowItemCountChanged();

private:
    //! @brief Vector of {Index=rowIndex, ActivityData}.
    std::vector<ActivityData> mActivityList;
    //! @brief Vector of {Index=rowIndex, MusicActivityData}.
    std::vector<std::vector<MusicActivityData>> mMusicActivityList;
};

}
