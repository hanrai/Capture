#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScxmlStateMachine>
#include <QFont>
#include <QtDebug>
#include "ocrengine.h"
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
    OCREngine *ocr = new OCREngine(machine, &app);

    qmlRegisterUncreatableType<QScxmlStateMachine>("StateMachine",
                                                   1, 0,
                                                   "StateMachine",
                                                   QLatin1String("StateMachine is not creatable."));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("ocr", ocr);
    engine.rootContext()->setContextProperty("mousePosition", ocr->getMouseRanger());
    engine.rootContext()->setContextProperty("machine", machine);
    engine.addImageProvider("snapshot", ocr);
    machine->start();

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
