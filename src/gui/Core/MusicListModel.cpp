#include "gui/Core/MusicListModel.hpp"

#include "ies/Common/IntegralRangeUsing.hpp"

#include "score2dx/Iidx/Version.hpp"

namespace gui
{

MusicListModel::
MusicListModel(const score2dx::Core &core, QObject* parent)
:   QAbstractListModel(parent),
    mCore(core)
{
    std::size_t musicCount = 0;
    auto &allTimeMusics = mCore.GetMusicDatabase().GetAllTimeMusics();
    for (auto versionIndex : IndexRange{0, allTimeMusics.size()})
    {
        auto &versionMusics = allTimeMusics[versionIndex];
        musicCount += versionMusics.size();
    }

    mMusicList.resize(musicCount);

    beginInsertRows({}, 0, rowCount());

    std::size_t rowIndex = 0;
    for (auto versionIndex : ReverseIndexRange{0, score2dx::VersionNames.size()})
    {
        auto &versionMusics = allTimeMusics.at(versionIndex);
        for (auto musicIndex : IndexRange{0, versionMusics.size()})
        {
            auto musicId = score2dx::ToMusicId(versionIndex, musicIndex);
            auto &musicInfo = mCore.GetMusicDatabase().GetMusic(musicId).GetMusicInfo();
            auto &data = mMusicList[rowIndex];
            data.Id = musicId;
            data.Title = musicInfo.GetField(score2dx::MusicInfoField::Title).c_str();
            data.Artist = musicInfo.GetField(score2dx::MusicInfoField::Artist).c_str();
            data.Version = score2dx::VersionNames[versionIndex].c_str();

            auto modelIndex = createIndex(rowIndex, 0);
            setData(modelIndex, data.Id, IdRole);
            setData(modelIndex, data.Title, TitleRole);
            setData(modelIndex, data.Artist, ArtistRole);
            setData(modelIndex, data.Version, VersionRole);

            ++rowIndex;
        }
    }

    endInsertRows();
}

int
MusicListModel::
rowCount(const QModelIndex &parent)
const
{
    Q_UNUSED(parent);
    return static_cast<int>(mMusicList.size());
}

QVariant
MusicListModel::
data(const QModelIndex &index, int role)
const
{
    if (index.row()<0||index.row()>=rowCount())
    {
        return {};
    }

    auto rowIndex = static_cast<std::size_t>(index.row());
    auto &musicData = mMusicList[rowIndex];
    switch (role)
    {
        case IdRole:
            return musicData.Id;
        case TitleRole:
            return musicData.Title;
        case ArtistRole:
            return musicData.Artist;
        case VersionRole:
            return musicData.Version;
    }

    return {};
}

int
MusicListModel::
getCount()
const
{
    return rowCount();
}

QVariantMap
MusicListModel::
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
MusicListModel::
roleNames()
const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[TitleRole] = "title";
    roles[ArtistRole] = "artist";
    roles[VersionRole] = "version";
    return roles;
}

}
