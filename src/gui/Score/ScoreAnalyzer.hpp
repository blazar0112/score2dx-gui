#pragma once

#include <QObject>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLegend>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QXYSeries>
#include <QStringList>
#include <QStandardItemModel>

#include "score2dx/Core/Core.hpp"

#include "gui/Score/ScoreAnalysisListModel.hpp"
#include "gui/Score/ScoreLevelListModel.hpp"

namespace gui
{

class ScoreAnalyzer : public QObject
{
    Q_OBJECT

public:
        explicit ScoreAnalyzer(const score2dx::Core &core, QObject* parent=nullptr);

        Q_INVOKABLE
        void
        setup(QtCharts::QLegend* legend,
              QtCharts::QAbstractSeries* scoreSeries,
              QtCharts::QAbstractAxis* dateTimeAxis,
              QtCharts::QAbstractAxis* scoreAxis,
              QtCharts::QAbstractAxis* versionCategoryAxis,
              QtCharts::QAbstractSeries* scatterSeriesScoreLevel,
              QtCharts::QAbstractAxis* scoreLevelAxis);

        Q_INVOKABLE
        void
        updatePlayerScore(const QString &iidxIdQStr,
                          const QString &playStyleQStr,
                          int musicId,
                          const QString &difficultyQStr);

        ScoreAnalysisListModel &
        GetAnalysisListModel();

        ScoreLevelListModel &
        GetScoreLevelListModel();

private:
    const score2dx::Core &mCore;

    ScoreAnalysisListModel mAnalysisListModel;
    ScoreLevelListModel mScoreLevelListModel;

    QtCharts::QLegend* mLegend{nullptr};

    QtCharts::QXYSeries* mScoreSeries{nullptr};
    QtCharts::QDateTimeAxis* mDateTimeAxis{nullptr};
    QtCharts::QValueAxis* mScoreAxis{nullptr};

    QtCharts::QCategoryAxis* mVersionCategoryAxis{nullptr};

    QtCharts::QScatterSeries* mScatterSeriesScoreLevel{nullptr};
    QtCharts::QValueAxis* mScoreLevelAxis{nullptr};

        void
        InitializeChart();
};

}
