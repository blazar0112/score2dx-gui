#pragma once

#include <vector>

#include <QAbstractListModel>
#include <QPointF>

namespace gui
{

class ScoreLevelListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ getCount CONSTANT)

public:
    enum DataRole {
        XRole = Qt::UserRole+1,
        YRole
    };

        explicit ScoreLevelListModel(QObject* parent=nullptr);

        int
        rowCount(const QModelIndex &parent=QModelIndex{})
        const
        override;

        QVariant
        data(const QModelIndex &index, int role=Qt::DisplayRole)
        const
        override;

        void
        ResetList(const std::vector<QPointF> &dataList);

        int
        getCount()
        const;

    //! @note QML ListModel compatible interface
        Q_INVOKABLE
        QVariantMap
        get(int rowIndex)
        const;

protected:
        QHash<int, QByteArray>
        roleNames()
        const
        override;

private:
    //! @brief Vector of QPointF, index is rowIndex.
    std::vector<QPointF> mDataList;
};

}
