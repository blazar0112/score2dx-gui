#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlDebuggingEnabler>

#include "gui/Core/Core.hpp"
#include "gui/Core/MusicListModel.hpp"
#include "gui/Score/ScoreAnalyzer.hpp"

int main(int argc, char *argv[])
{
    QApplication app{argc, argv};
    app.setOrganizationName("somename");
    app.setOrganizationDomain("somename");

    //QQmlDebuggingEnabler enabler;

    gui::Core core;
    gui::MusicListModel musicListModel{core.GetScore2dxCore()};
    gui::ScoreAnalyzer analyzer{core.GetScore2dxCore()};

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("core", &core);
    engine.rootContext()->setContextProperty("difficultyListModel", &core.GetDifficultyListModel());

    engine.rootContext()->setContextProperty("csvTableModel", &core.GetCsvTableModel());

    engine.rootContext()->setContextProperty("musicListModel", &musicListModel);

    engine.rootContext()->setContextProperty("analyzer", &analyzer);
    engine.rootContext()->setContextProperty("analysisListModel", &analyzer.GetAnalysisListModel());
    engine.rootContext()->setContextProperty("scoreLevelListModel", &analyzer.GetScoreLevelListModel());
    engine.load(QUrl("qrc:/qml/ScoreViewer.qml"));

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
