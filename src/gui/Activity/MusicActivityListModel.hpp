#pragma once

#include <vector>

#include <QAbstractListModel>

#include "icl_s2/Common/SmartEnum.hxx"

namespace gui
{

//! @note Previous must exist, if has no new record then it's empty string.
ICL_S2_SMART_ENUM(MusicActivityDataRole,
    styleDifficulty,
    level,
    difficulty,
    previousClear,
    previousScore,
    previousMiss,
    newRecordClear,
    newRecordScore,
    newRecordMiss
);

struct MusicActivityData
{
    std::array<QVariant, MusicActivityDataRoleSmartEnum::Size()> Data;
};

class MusicActivityListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int rowItemCount READ getRowItemCount NOTIFY rowItemCountChanged)

public:
        MusicActivityListModel();

        int getRowItemCount() const { return rowCount(); }

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
        ResetModel(std::vector<MusicActivityData> &&musicActivityList);

signals:
        void rowItemCountChanged();

private:
    //! @brief Vector of {Index=rowIndex, MusicActivityData}.
    std::vector<MusicActivityData> mMusicActivityList;
};

}

//Q_DECLARE_METATYPE(gui::MusicActivityListModel*)
