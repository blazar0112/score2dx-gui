#pragma once

#include <vector>

#include <QAbstractListModel>

#include "icl_s2/Common/SmartEnum.hxx"

namespace gui
{

ICL_S2_SMART_ENUM(StatsMusicDataRole,
    version,
    clear,
    level,
    difficulty,
    title,
    djLevel,
    score,
    bestScoreDiff,
    careerBestVersion,
    careerBestScore
);

//! @brief String to let GUI control display behavior.
//! @note Role:
//!     ToString() if not noted
//!     clear: ToPrettyString(ClearType)
//!     difficulty: ToString(DifficultyAcronym)
//!     scoreLevel: no API, need to update library.
struct StatsMusicData
{
    std::array<QString, StatsMusicDataRoleSmartEnum::Size()> Data;
};

class StatsMusicListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int rowItemCount READ getRowItemCount NOTIFY rowItemCountChanged)

public:
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
        ResetModel(std::vector<StatsMusicData> &&musicList);

signals:
        void rowItemCountChanged();

private:
    //! @brief Vector of {Index=rowIndex, StatsMusicData}.
    std::vector<StatsMusicData> mMusicList;
};

}
