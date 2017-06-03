#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QDebug>

#include <QStringListModel>
#include <QQmlContext>

#include "solution.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QObject *rootObject;
    QWindow *rootWindow;

    QQmlContext * rootContext;
    QStringListModel strModel;
    Solution * sln = new Solution(&strModel);

    // set the root context
    rootContext = engine.rootContext();
    rootContext->setContextProperty("slnModel", &strModel);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    // set the root object
    rootObject = engine.rootObjects().value(0);
    rootWindow = qobject_cast<QQuickWindow *>(rootObject);
    QObject::connect(rootWindow, SIGNAL(loadClicked()), sln, SLOT(load()));

    return app.exec();
}
