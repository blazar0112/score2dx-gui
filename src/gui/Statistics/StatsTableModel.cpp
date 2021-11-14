#include "gui/Statistics/StatsTableModel.hpp"

#include <QDebug>

#include "icl_s2/Common/IntegralRangeUsing.hpp"

namespace gui
{

int
StatsTableModel::
rowCount(const QModelIndex &parent)
const
{
    Q_UNUSED(parent);
    return static_cast<int>(mTable.size());
}

int
StatsTableModel::
columnCount(const QModelIndex &parent)
const
{
    Q_UNUSED(parent);
    if (mTable.empty()) { return 0; }
    return static_cast<int>(mTable[0].size());
}

QVariant
StatsTableModel::
data(const QModelIndex &index, int role)
const
{
    if (index.row()<0||index.row()>=rowCount())
    {
        return {};
    }

    if (index.column()<0||index.column()>=columnCount())
    {
        return {};
    }

    int dataRole = role-Qt::UserRole;
    if (dataRole<StatsTableDataRoleSmartEnum::Min()||dataRole>StatsTableDataRoleSmartEnum::Max())
    {
        return {};
    }

    return mTable[index.row()][index.column()].Data[dataRole];
}

QHash<int, QByteArray>
StatsTableModel::
roleNames()
const
{
    QHash<int, QByteArray> roles;
    for (auto role : StatsTableDataRoleSmartEnum::ToRange())
    {
        auto index = Qt::UserRole+static_cast<int>(role);
        roles[index] = ToString(role).c_str();
    }
    return roles;
}

void
StatsTableModel::
ResetModel(std::vector<std::vector<StatsTableData>> &&table)
{
    try
    {
        mTable = std::move(table);

        int rowCount = mTable.size();
        if (rowCount==0)
        {
            throw std::runtime_error("table has empty rows.");
        }

        int columnCount = mTable[0].size();
        if (columnCount==0)
        {
            throw std::runtime_error("table has empty columns.");
        }

        //'' check column count
        for (auto &rowData : mTable)
        {
            if (columnCount!=static_cast<int>(rowData.size()))
            {
                throw std::runtime_error("column count not match table rows.");
            }
        }

        beginResetModel();

        IntRange roleRange{StatsTableDataRoleSmartEnum::Min(), StatsTableDataRoleSmartEnum::Max()+1};

        for (auto row : IntRange{0, rowCount})
        {
            for (auto column : IntRange{0, columnCount})
            {
                auto modelIndex = createIndex(row, column);

                for (auto role : roleRange)
                {
                    setData(modelIndex, mTable[row][column].Data[role], Qt::UserRole+role);
                }
            }
        }

        endResetModel();

        emit rowItemCountChanged();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("StatsTableModel::ResetModel(): exception\n    "
                                 +std::string{e.what()});
    }
}

}
