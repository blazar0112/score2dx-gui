#include "gui/Activity/ActivityListModel.hpp"

#include <QDebug>

#include "ies/Common/IntegralRangeUsing.hpp"

namespace gui
{

ChartActivityListModel*
ActivityListModel::
getChartActivityListModel(int row)
{
    auto chartActivityListModel = new ChartActivityListModel();
    std::vector<ChartActivityData> chartActivityList;
    if (row>=0)
    {
        chartActivityList = mMusicChartActivityList.at(row);
    }
    chartActivityListModel->ResetModel(std::move(chartActivityList));
    return chartActivityListModel;
}

int
ActivityListModel::
getTotalIncreasedPlayCount()
const
{
    int totalPlayCount = 0;
    for (auto &activityData : mActivityList)
    {
        auto previousPlayCount = activityData.Data[static_cast<int>(ActivityDataRole::previousPlayCount)].toInt();
        auto playCount = activityData.Data[static_cast<int>(ActivityDataRole::playCount)].toInt();
        auto diff = playCount-previousPlayCount;
        if (diff<0)
        {
            qDebug() << "previousPlayCount" << previousPlayCount << "> playCount" << playCount;
            continue;
        }
        totalPlayCount += diff;
    }
    return totalPlayCount;
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
           std::vector<std::vector<ChartActivityData>> &&musicChartActivityList)
{
    try
    {
        mActivityList = std::move(activityList);
        mMusicChartActivityList = std::move(musicChartActivityList);

        beginResetModel();

        for (auto row : IntRange{0, static_cast<int>(mActivityList.size()), ies::EmptyPolicy::Allow})
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
