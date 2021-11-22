#include "gui/Activity/ActivityListModel.hpp"

#include <QDebug>

#include "icl_s2/Common/IntegralRangeUsing.hpp"

namespace gui
{

ActivityListModel::
~ActivityListModel()
{

}

MusicActivityListModel*
ActivityListModel::
getMusicActivityListModel(int row)
{
    auto musicActivityListModel = new MusicActivityListModel();
    std::vector<MusicActivityData> musicActivity;
    if (row>=0)
    {
        musicActivity = mMusicActivityList.at(row);
    }
    musicActivityListModel->ResetModel(std::move(musicActivity));
    return musicActivityListModel;
}

int
ActivityListModel::
rowCount(const QModelIndex &parent)
const
{
    Q_UNUSED(parent);
    return static_cast<int>(mActivityList.size());
}

QVariant
ActivityListModel::
data(const QModelIndex &index, int role)
const
{
    if (index.row()<0||index.row()>=rowCount())
    {
        return {};
    }

    int dataRole = role-Qt::UserRole;
    if (dataRole<ActivityDataRoleSmartEnum::Min()||dataRole>ActivityDataRoleSmartEnum::Max())
    {
        return {};
    }

    return mActivityList[index.row()].Data[dataRole];
}

QHash<int, QByteArray>
ActivityListModel::
roleNames()
const
{
    QHash<int, QByteArray> roles;
    for (auto role : ActivityDataRoleSmartEnum::ToRange())
    {
        auto index = Qt::UserRole+static_cast<int>(role);
        roles[index] = ToString(role).c_str();
    }
    return roles;
}

void
ActivityListModel::
ResetModel(std::vector<ActivityData> &&activityList,
           std::vector<std::vector<MusicActivityData>> &&musicActivityList)
{
    try
    {
        mActivityList = std::move(activityList);
        mMusicActivityList = std::move(musicActivityList);

        beginResetModel();

        for (auto row : IntRange{0, static_cast<int>(mActivityList.size()), icl_s2::EmptyPolicy::Allow})
        {
            auto modelIndex = createIndex(row, 0);
            for (auto role : IndexRange{0, ActivityDataRoleSmartEnum::Size()})
            {
                setData(modelIndex, mActivityList[row].Data[role], Qt::UserRole+role);
            }
        }

        endResetModel();

        emit rowItemCountChanged();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("ActivityListModel::ResetModel(): exception\n    "
                                 +std::string{e.what()});
    }
}

}
