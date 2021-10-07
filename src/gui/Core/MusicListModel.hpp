#pragma once

#include <vector>

#include <QAbstractListModel>

#include "score2dx/Core/Core.hpp"

namespace gui
{

struct MusicData
{
    std::size_t Id{0};
    QString Title;
    QString Artist;
    QString Version;
};

class MusicListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ getCount CONSTANT)

public:
    enum DataRole {
        IdRole = Qt::UserRole+1,
        TitleRole,
        ArtistRole,
        VersionRole
    };

        MusicListModel(const score2dx::Core &core, QObject* parent=nullptr);

        int
        rowCount(const QModelIndex &parent=QModelIndex{})
        const
        override;

        QVariant
        data(const QModelIndex &index, int role=Qt::DisplayRole)
        const
        override;

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
    const score2dx::Core &mCore;
    //! @brief Vector of MusicData, index is rowIndex.
    std::vector<MusicData> mMusicList;
};

}
