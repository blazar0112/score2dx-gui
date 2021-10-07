#pragma once

#include <vector>

#include <QAbstractListModel>

#include "icl_s2/Common/SmartEnum.hxx"

namespace gui
{

//! @brief AnalysisType for record-update data type in ScoreAnalysisData.
//! Record data is stored in QString, can be empty, user need to know how to interpret each data type.
//!
//! Clear: space separated string of score2dx::ClearType.
//! Score: int, EX Score
//! DjLevel: string format <AAA|AA|A|B|...|F>.
//! MissCount: int, can be empty.
ICL_S2_SMART_ENUM(AnalysisType,
    Clear,
    Score,
    DjLevel,
    MissCount
);

ICL_S2_SMART_ENUM(AnalysisRecordField,
    Record,
    PreviousRecord,
    NewRecord
);

struct AnalysisRecord
{
    QString Record;
    QString PreviousRecord;
    bool NewRecord{false};

        QVariant
        GetField(AnalysisRecordField field)
        const;
};

struct ScoreAnalysisData
{
    //'' role: 0~11
    std::array<AnalysisRecord, AnalysisTypeSmartEnum::Size()> Records;

    //'' role: 12
    //! @brief ScoreLevelRangeDiff string in form of 'MAX+0', 'MAX-20', 'AAA+20', 'AAA+0', 'AAA-1', etc.
    QString ScoreLevelRangeDiff;

        AnalysisRecord &
        GetRecord(AnalysisType analysisType);

        const AnalysisRecord &
        GetRecord(AnalysisType analysisType)
        const;
};

//! @brief ScoreAnalysisListModel: additional data provided through ListModel.
//! ScoreAnalyezer can set Chart points directly, but to generate other gui need those extra infomation.
//! @todo Code is almost same as MusicListModel, but need to abstract out data() to generalize.
class ScoreAnalysisListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ getCount CONSTANT)

public:
        explicit ScoreAnalysisListModel(QObject* parent=nullptr);

        int
        rowCount(const QModelIndex &parent=QModelIndex{})
        const
        override;

        QVariant
        data(const QModelIndex &index, int role=Qt::DisplayRole)
        const
        override;

        void
        ResetList(const std::vector<ScoreAnalysisData> &dataList);

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
    //! @brief Vector of ScoreAnalysisData, index is rowIndex.
    std::vector<ScoreAnalysisData> mDataList;
};

}
