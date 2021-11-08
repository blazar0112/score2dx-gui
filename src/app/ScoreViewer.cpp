#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlDebuggingEnabler>

#include "gui/Core/Core.hpp"
#include "gui/Core/MusicListModel.hpp"
#include "gui/Graph/GraphManager.hpp"
#include "gui/Statistics/StatisticsManager.hpp"

int main(int argc, char *argv[])
{
    QApplication app{argc, argv};
    app.setOrganizationName("somename");
    app.setOrganizationDomain("somename");

    //QQmlDebuggingEnabler enabler;

    gui::Core core;
    gui::MusicListModel musicListModel{core.GetScore2dxCore()};
    gui::GraphManager graphManager{core.GetScore2dxCore()};
    gui::StatisticsManager statisticsManager{core.GetScore2dxCore()};

    QQmlApplicationEngine engine;
    //'' Non-copyable model cannot use property method.
    engine.rootContext()->setContextProperty("core", &core);
    engine.rootContext()->setContextProperty("difficultyListModel", &core.GetDifficultyListModel());

    engine.rootContext()->setContextProperty("musicListModel", &musicListModel);

    engine.rootContext()->setContextProperty("graphManager", &graphManager);
    engine.rootContext()->setContextProperty("graphAnalysisListModel", &graphManager.GetGraphAnalysisListModel());
    engine.rootContext()->setContextProperty("scoreLevelListModel", &graphManager.GetScoreLevelListModel());

    engine.rootContext()->setContextProperty("statisticsManager", &statisticsManager);
    engine.rootContext()->setContextProperty("statsTableModel", &statisticsManager.GetStatsTableModel());

    engine.load(QUrl("qrc:/qml/ScoreViewer.qml"));

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
