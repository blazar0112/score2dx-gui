#include "gui/Graph/GraphAnalysisListModel.hpp"

#include <cctype>

#include "icl_s2/Common/IntegralRangeUsing.hpp"

#include "score2dx/Iidx/Version.hpp"

namespace gui
{

QVariant
GraphAnalysisRecord::
GetField(GraphAnalysisRecordField field)
const
{
    switch (field)
    {
        case GraphAnalysisRecordField::Record:
            return Record;
        case GraphAnalysisRecordField::PreviousRecord:
            return PreviousRecord;
        case GraphAnalysisRecordField::NewRecord:
            return NewRecord;
    }

    return {};
}

GraphAnalysisRecord &
GraphAnalysisData::
GetRecord(GraphAnalysisType analysisType)
{
    return const_cast<GraphAnalysisRecord &>(std::as_const(*this).GetRecord(analysisType));
}

const GraphAnalysisRecord &
GraphAnalysisData::
GetRecord(GraphAnalysisType analysisType)
const
{
    return Records[static_cast<std::size_t>(analysisType)];
}

GraphAnalysisListModel::
GraphAnalysisListModel(QObject* parent)
:   QAbstractListModel(parent)
{
}

int
GraphAnalysisListModel::
rowCount(const QModelIndex &parent)
const
{
    Q_UNUSED(parent);
    return static_cast<int>(mDataList.size());
}

QVariant
GraphAnalysisListModel::
data(const QModelIndex &index, int role)
const
{
    if (index.row()<0||index.row()>=rowCount())
    {
        return {};
    }

    auto rowIndex = static_cast<std::size_t>(index.row());
    auto &analysisData = mDataList[rowIndex];

    auto beginRoleIndex = Qt::UserRole+1;
    IntRange roleRange{beginRoleIndex, beginRoleIndex+(4*3+1)+1};
    if (!roleRange.IsInRange(role))
    {
        return {};
    }

    auto roleIndex = role-beginRoleIndex;
    if (roleIndex==4*3)
    {
        return analysisData.ScoreLevelRangeDiff;
    }

    auto analysisType = static_cast<GraphAnalysisType>(roleIndex/GraphAnalysisRecordFieldSmartEnum::Size());
    auto recordField = static_cast<GraphAnalysisRecordField>(roleIndex%GraphAnalysisRecordFieldSmartEnum::Size());

    return analysisData.GetRecord(analysisType).GetField(recordField);
}

void
GraphAnalysisListModel::
ResetList(const std::vector<GraphAnalysisData> &dataList)
{
    mDataList = dataList;

    beginResetModel();

    for (auto rowIndex : IndexRange{0, mDataList.size(), icl_s2::EmptyPolicy::Allow})
    {
        auto modelIndex = createIndex(rowIndex, 0);
        auto &data = mDataList[rowIndex];

        auto roleIndex = Qt::UserRole+1;

        for (auto analysisType : GraphAnalysisTypeSmartEnum::ToRange())
        {
            auto i = static_cast<std::size_t>(analysisType);
            auto &record = data.Records[i];

            for (auto recordField : GraphAnalysisRecordFieldSmartEnum::ToRange())
            {
                setData(modelIndex, record.GetField(recordField), roleIndex);
                roleIndex++;
            }
        }
    }

    endResetModel();
}

int
GraphAnalysisListModel::
getCount()
const
{
    return rowCount();
}

QVariantMap
GraphAnalysisListModel::
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
GraphAnalysisListModel::
roleNames()
const
{
    QHash<int, QByteArray> roles;
    auto roleIndex = Qt::UserRole+1;

    for (auto analysisType : GraphAnalysisTypeSmartEnum::ToRange())
    {
        auto rolePrefix = ToString(analysisType);
        rolePrefix[0] = tolower(rolePrefix[0]);

        for (auto recordField : GraphAnalysisRecordFieldSmartEnum::ToRange())
        {
            auto roleRecord = rolePrefix+ToString(recordField);
            roles[roleIndex] = roleRecord.c_str();
            ++roleIndex;
        }
    }

    roles[roleIndex] = "scoreLevelRangeDiff";
    return roles;
}

}
