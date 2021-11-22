#include "gui/Activity/ChartActivityListModel.hpp"

#include "icl_s2/Common/IntegralRangeUsing.hpp"

namespace gui
{

int
ChartActivityListModel::
rowCount(const QModelIndex &parent)
const
{
    Q_UNUSED(parent);
    return static_cast<int>(mChartActivityList.size());
}

QVariant
ChartActivityListModel::
data(const QModelIndex &index, int role)
const
{
    if (index.row()<0||index.row()>=rowCount())
    {
        return {};
    }

    int dataRole = role-Qt::UserRole;
    if (dataRole<ChartActivityDataRoleSmartEnum::Min()||dataRole>ChartActivityDataRoleSmartEnum::Max())
    {
        return {};
    }

    return mChartActivityList[index.row()].Data[dataRole];
}

QHash<int, QByteArray>
ChartActivityListModel::
roleNames()
const
{
    QHash<int, QByteArray> roles;
    for (auto role : ChartActivityDataRoleSmartEnum::ToRange())
    {
        auto index = Qt::UserRole+static_cast<int>(role);
        roles[index] = ToString(role).c_str();
    }
    return roles;
}

void
ChartActivityListModel::
ResetModel(std::vector<ChartActivityData> &&chartActivityList)
{
    try
    {
        mChartActivityList = std::move(chartActivityList);

        beginResetModel();

        for (auto row : IntRange{0, static_cast<int>(mChartActivityList.size()), icl_s2::EmptyPolicy::Allow})
        {
            auto modelIndex = createIndex(row, 0);
            for (auto role : IndexRange{0, ChartActivityDataRoleSmartEnum::Size()})
            {
                setData(modelIndex, mChartActivityList[row].Data[role], Qt::UserRole+role);
            }
        }

        endResetModel();

        emit rowItemCountChanged();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("ChartActivityListModel::ResetModel(): exception\n    "
                                 +std::string{e.what()});
    }
}

}
