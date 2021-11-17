#include "gui/Statistics/StatsChartListModel.hpp"

#include <cctype>

#include "icl_s2/Common/IntegralRangeUsing.hpp"

#include "score2dx/Iidx/Version.hpp"

namespace gui
{

int
StatsChartListModel::
rowCount(const QModelIndex &parent)
const
{
    Q_UNUSED(parent);
    return static_cast<int>(mChartList.size());
}

QVariant
StatsChartListModel::
data(const QModelIndex &index, int role)
const
{
    if (index.row()<0||index.row()>=rowCount())
    {
        return {};
    }

    int dataRole = role-Qt::UserRole;
    if (dataRole<StatsChartDataRoleSmartEnum::Min()||dataRole>StatsChartDataRoleSmartEnum::Max())
    {
        return {};
    }

    return mChartList[index.row()].Data[dataRole];
}

QHash<int, QByteArray>
StatsChartListModel::
roleNames()
const
{
    QHash<int, QByteArray> roles;
    for (auto role : StatsChartDataRoleSmartEnum::ToRange())
    {
        auto index = Qt::UserRole+static_cast<int>(role);
        roles[index] = ToString(role).c_str();
    }
    return roles;
}

void
StatsChartListModel::
ResetModel(std::vector<StatsChartData> &&chartList)
{
    try
    {
        mChartList = std::move(chartList);

        beginResetModel();

        for (auto row : IntRange{0, static_cast<int>(mChartList.size()), icl_s2::EmptyPolicy::Allow})
        {
            auto modelIndex = createIndex(row, 0);
            for (auto role : IndexRange{0, StatsChartDataRoleSmartEnum::Size()})
            {
                setData(modelIndex, mChartList[row].Data[role], Qt::UserRole+role);
            }
        }

        endResetModel();

        emit rowItemCountChanged();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("StatsChartListModel::ResetModel(): exception\n    "
                                 +std::string{e.what()});
    }
}

}
