#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScxmlStateMachine>
#include <QFont>
#include <QtDebug>
#include "engine.h"
#include "mouseposition.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    app.setFont(QFont("微软雅黑"));

    QCoreApplication::setOrganizationName("Sophia");
    QCoreApplication::setOrganizationDomain("sophia.com");
    QCoreApplication::setApplicationName("Capture");

    QScxmlStateMachine *machine = QScxmlStateMachine::fromFile(":statemachine.scxml");
    if (!machine->parseErrors().isEmpty()) {
        const auto errors = machine->parseErrors();
        for (const QScxmlError &error : errors) {
            qDebug() << error.toString();
        }
    };
    Engine *mainEngine = new Engine(machine, &app);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("engine", mainEngine);
    engine.rootContext()->setContextProperty("hotSpot", mainEngine->getHotSpot());
    engine.rootContext()->setContextProperty("contractSpot", mainEngine->getContractSpot());
    engine.rootContext()->setContextProperty("dateSpot", mainEngine->getDateSpot());
    engine.rootContext()->setContextProperty("timeSpot", mainEngine->getTimeSpot());
    engine.rootContext()->setContextProperty("mousePosition", mainEngine->getMouseRanger());
    engine.rootContext()->setContextProperty("machine", machine);
    engine.addImageProvider("snapshot", mainEngine);
    machine->start();

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
