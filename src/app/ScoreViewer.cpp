#include <QApplication>
#include <QDebug>
#include <QImageReader>
#include <QMessageBox>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlDebuggingEnabler>

#include "gui/Activity/ActivityManager.hpp"
#include "gui/Core/Core.hpp"
#include "gui/Core/MusicListModel.hpp"
#include "gui/Graph/GraphManager.hpp"
#include "gui/Statistics/StatisticsManager.hpp"

#include "ies/Time/TimeUtilFormat.hxx"

namespace s2Time = ies::Time;

int main(int argc, char *argv[])
{
    try
    {
        QApplication app{argc, argv};
        QCoreApplication::setOrganizationName("ScoreViewer");
        QCoreApplication::setOrganizationDomain("ScoreViewer.app");
        QGuiApplication::setWindowIcon(QIcon(":/qml/image/icon.png"));

        //QQmlDebuggingEnabler enabler;

        qRegisterMetaType<gui::ChartActivityListModel*>("ChartActivityListModel*");
        //qmlRegisterAnonymousType<gui::MusicActivityListModel*>("Score2dx.Gui", 1);

        gui::Core core;
        gui::MusicListModel musicListModel{core.GetScore2dxCore()};
        gui::GraphManager graphManager{core.GetScore2dxCore()};
        gui::StatisticsManager statisticsManager{core.GetScore2dxCore()};
        gui::ActivityManager activityManager{core};

        auto begin = s2Time::Now();
        QQmlApplicationEngine engine;
        qmlRegisterSingletonInstance<gui::Core>("Score2dx.Gui", 1, 0, "Core", &core);
        //'' Non-copyable model cannot use property method.
        qmlRegisterSingletonInstance<gui::MusicListModel>("Score2dx.Gui", 1, 0, "MusicListModel", &musicListModel);

        qmlRegisterSingletonInstance<gui::GraphManager>("Score2dx.Gui", 1, 0, "GraphManager", &graphManager);
        qmlRegisterSingletonInstance<gui::GraphAnalysisListModel>("Score2dx.Gui", 1, 0, "GraphAnalysisListModel", &graphManager.GetGraphAnalysisListModel());
        qmlRegisterSingletonInstance<gui::ScoreLevelListModel>("Score2dx.Gui", 1, 0, "ScoreLevelListModel", &graphManager.GetScoreLevelListModel());

        qmlRegisterSingletonInstance<gui::StatisticsManager>("Score2dx.Gui", 1, 0, "StatisticsManager", &statisticsManager);
        qmlRegisterSingletonInstance<gui::StatsTableModel>("Score2dx.Gui", 1, 0, "StatsHorizontalHeaderModel", &statisticsManager.GetHorizontalHeaderModel());
        qmlRegisterSingletonInstance<gui::StatsTableModel>("Score2dx.Gui", 1, 0, "StatsVerticalHeaderModel", &statisticsManager.GetVerticalHeaderModel());
        qmlRegisterSingletonInstance<gui::StatsTableModel>("Score2dx.Gui", 1, 0, "StatsTableModel", &statisticsManager.GetTableModel());
        qmlRegisterSingletonInstance<gui::StatsChartListModel>("Score2dx.Gui", 1, 0, "StatsChartListHeaderModel", &statisticsManager.GetChartListHeaderModel());
        qmlRegisterSingletonInstance<gui::StatsChartListModel>("Score2dx.Gui", 1, 0, "StatsChartListModel", &statisticsManager.GetChartListModel());

        qmlRegisterSingletonInstance<gui::ActivityManager>("Score2dx.Gui", 1, 0, "ActivityManager", &activityManager);
        qmlRegisterSingletonInstance<gui::ActivityListModel>("Score2dx.Gui", 1, 0, "ActivityListModel", &activityManager.GetActivityListModel());

        engine.load(QUrl("qrc:/qml/ScoreViewer.qml"));

        if (engine.rootObjects().isEmpty())
        {
            return -1;
        }

        s2Time::Print<std::chrono::milliseconds>(s2Time::CountNs(begin), "QML engine loading");
        std::cout << std::flush;

        return app.exec();
    }
    catch (const std::exception &e)
    {
        QApplication app{argc, argv};
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "Score Viewer exception:\n"+QString{e.what()});
        return 1;
    }
}
