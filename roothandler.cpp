#include "roothandler.h"
#include <QDebug>

RootHandler::RootHandler(QObject *parent) : QObject(parent)
{

}

void RootHandler::handleButtonSlot()
{
    qDebug("Ahoj");
}
