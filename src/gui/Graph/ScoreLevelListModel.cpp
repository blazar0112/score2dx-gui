#include "gui/Graph/ScoreLevelListModel.hpp"

#include "icl_s2/Common/IntegralRangeUsing.hpp"

#include "score2dx/Iidx/Version.hpp"

namespace gui
{

ScoreLevelListModel::
ScoreLevelListModel(QObject* parent)
:   QAbstractListModel(parent)
{
}

int
ScoreLevelListModel::
rowCount(const QModelIndex &parent)
const
{
    Q_UNUSED(parent);
    return static_cast<int>(mDataList.size());
}

QVariant
ScoreLevelListModel::
data(const QModelIndex &index, int role)
const
{
    if (index.row()<0||index.row()>=rowCount())
    {
        return {};
    }

    auto rowIndex = static_cast<std::size_t>(index.row());
    auto &data = mDataList[rowIndex];
    switch (role)
    {
        case XRole:
            return data.x();
        case YRole:
            return data.y();
    }

    return {};
}

void
ScoreLevelListModel::
ResetList(const std::vector<QPointF> &dataList)
{
    mDataList = dataList;

    beginResetModel();

    for (auto rowIndex : IndexRange{0, mDataList.size()})
    {
        auto modelIndex = createIndex(rowIndex, 0);
        auto &data = mDataList[rowIndex];
        setData(modelIndex, data.x(), XRole);
        setData(modelIndex, data.y(), YRole);
    }

    endResetModel();
}

int
ScoreLevelListModel::
getCount()
const
{
    return rowCount();
}

QVariantMap
ScoreLevelListModel::
get(int rowIndex)
const
{
    QVariantMap data;
    auto modelIndex = index(rowIndex);
    if (!modelIndex.isValid())
    {
        return data;
    }

    QHashIterator<int, QByteArray> it{roleNames()};
    while (it.hasNext())
    {
        it.next();
        data[it.value()] = modelIndex.data(it.key());
    }

    return data;
}

QHash<int, QByteArray>
ScoreLevelListModel::
roleNames()
const
{
    QHash<int, QByteArray> roles;
    roles[XRole] = "x";
    roles[YRole] = "y";
    return roles;
}

}
