#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QDebug>

#include "roothandler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QObject *rootObject;
    QWindow *rootWindow;
    RootHandler rootHdl;

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    rootObject = engine.rootObjects().value(0);
    rootWindow = qobject_cast<QQuickWindow *>(rootObject);
    QObject::connect(rootWindow, SIGNAL(solutionClicked()), &rootHdl, SLOT(handleButtonSlot()));

    return app.exec();
}
