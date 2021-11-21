#include "gui/Activity/MusicActivityListModel.hpp"

#include "icl_s2/Common/IntegralRangeUsing.hpp"

namespace gui
{

MusicActivityListModel::
MusicActivityListModel()
{
}

int
MusicActivityListModel::
rowCount(const QModelIndex &parent)
const
{
    Q_UNUSED(parent);
    return static_cast<int>(mMusicActivityList.size());
}

QVariant
MusicActivityListModel::
data(const QModelIndex &index, int role)
const
{
    if (index.row()<0||index.row()>=rowCount())
    {
        return {};
    }

    int dataRole = role-Qt::UserRole;
    if (dataRole<MusicActivityDataRoleSmartEnum::Min()||dataRole>MusicActivityDataRoleSmartEnum::Max())
    {
        return {};
    }

    return mMusicActivityList[index.row()].Data[dataRole];
}

QHash<int, QByteArray>
MusicActivityListModel::
roleNames()
const
{
    QHash<int, QByteArray> roles;
    for (auto role : MusicActivityDataRoleSmartEnum::ToRange())
    {
        auto index = Qt::UserRole+static_cast<int>(role);
        roles[index] = ToString(role).c_str();
    }
    return roles;
}

void
MusicActivityListModel::
ResetModel(std::vector<MusicActivityData> &&musicActivityList)
{
    try
    {
        mMusicActivityList = std::move(musicActivityList);

        beginResetModel();

        for (auto row : IntRange{0, static_cast<int>(mMusicActivityList.size()), icl_s2::EmptyPolicy::Allow})
        {
            auto modelIndex = createIndex(row, 0);
            for (auto role : IndexRange{0, MusicActivityDataRoleSmartEnum::Size()})
            {
                setData(modelIndex, mMusicActivityList[row].Data[role], Qt::UserRole+role);
            }
        }

        endResetModel();

        emit rowItemCountChanged();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("MusicActivityListModel::ResetModel(): exception\n    "
                                 +std::string{e.what()});
    }
}

}
