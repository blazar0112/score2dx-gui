#pragma once

#include <QAbstractTableModel>

#include "ies/Common/SmartEnum.hxx"

namespace gui
{

IES_SMART_ENUM(StatsTableDataRole,
    display,
    foreground,
    background
);

struct StatsTableData
{
    std::array<QString, StatsTableDataRoleSmartEnum::Size()> Data
    {
        "",
        "black",
        "white"
    };
};

//! @brief TableModel with headerless, customizable table cells.
//! @note Default header for item model cannot have customed user roles.
class StatsTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int rowItemCount READ getRowItemCount NOTIFY rowItemCountChanged)

public:
        int getRowItemCount() const { return rowCount(); }

        int
        rowCount(const QModelIndex &parent = QModelIndex())
        const
        override;

        int
        columnCount(const QModelIndex &parent = QModelIndex())
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
        ResetModel(std::vector<std::vector<StatsTableData>> &&table);

signals:
        void rowItemCountChanged();

private:
    //! @brief Vector of {Index=RowIndex, Vector of {Index=ColumnIndex, Data}}.
    std::vector<std::vector<StatsTableData>> mTable;
};

}
