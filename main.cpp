#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QDebug>

#include <QStringListModel>
#include <QQmlContext>

#include "solution.h"
#include "commanager.h"
#include "elfmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QObject *rootObject;
    QWindow *rootWindow;

    QQmlContext * rootContext;
    QStringListModel strModel;
    QStringListModel comModel;
    Solution * sln = new Solution(&strModel);
    ComManager * comMgr = new ComManager(&comModel);
    ElfListModel elfModel;
    ElfFile * elfFile = new ElfFile();

    comMgr->loadInterfaces();

    elfModel.addElfFile("/home/jan/Git/M4_bootloader/Debug/bootloader2.out", elfFile);
    // set the root context
    rootContext = engine.rootContext();
    rootContext->setContextProperty("slnModel", &strModel);
    rootContext->setContextProperty("comModel", &comModel);
    rootContext->setContextProperty("elfModel", &elfModel);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    // set the root object
    rootObject = engine.rootObjects().value(0);
    rootWindow = qobject_cast<QQuickWindow *>(rootObject);
    QObject::connect(rootWindow, SIGNAL(loadClicked()), sln, SLOT(load()));
    QObject::connect(rootWindow, SIGNAL(loadClicked()), sln, SLOT(load()));

    return app.exec();
}
