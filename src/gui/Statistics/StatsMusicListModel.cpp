#include "gui/Statistics/StatsMusicListModel.hpp"

#include <cctype>

#include "icl_s2/Common/IntegralRangeUsing.hpp"

#include "score2dx/Iidx/Version.hpp"

namespace gui
{

int
StatsMusicListModel::
rowCount(const QModelIndex &parent)
const
{
    Q_UNUSED(parent);
    return static_cast<int>(mMusicList.size());
}

QVariant
StatsMusicListModel::
data(const QModelIndex &index, int role)
const
{
    if (index.row()<0||index.row()>=rowCount())
    {
        return {};
    }

    int dataRole = role-Qt::UserRole;
    if (dataRole<StatsMusicDataRoleSmartEnum::Min()||dataRole>StatsMusicDataRoleSmartEnum::Max())
    {
        return {};
    }

    return mMusicList[index.row()].Data[dataRole];
}

QHash<int, QByteArray>
StatsMusicListModel::
roleNames()
const
{
    QHash<int, QByteArray> roles;
    for (auto role : StatsMusicDataRoleSmartEnum::ToRange())
    {
        auto index = Qt::UserRole+static_cast<int>(role);
        roles[index] = ToString(role).c_str();
    }
    return roles;
}

void
StatsMusicListModel::
ResetModel(std::vector<StatsMusicData> &&musicList)
{
    try
    {
        mMusicList = std::move(musicList);

        beginResetModel();

        for (auto row : IntRange{0, static_cast<int>(mMusicList.size()), icl_s2::EmptyPolicy::Allow})
        {
            auto modelIndex = createIndex(row, 0);
            for (auto role : IndexRange{0, StatsMusicDataRoleSmartEnum::Size()})
            {
                setData(modelIndex, mMusicList[row].Data[role], Qt::UserRole+role);
            }
        }

        endResetModel();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("StatsMusicListModel::ResetModel(): exception\n    "
                                 +std::string{e.what()});
    }
}

}
