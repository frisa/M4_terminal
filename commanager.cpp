#include "commanager.h"

ComManager::ComManager(QObject *parent) : QObject(parent)
{

}

ComManager::ComManager(QStringListModel * model) : m_model(model)
{
    QStringList list;
    for (int i = 1; i <= 3; ++i) {
        list << QString("NULL%1").arg(i);
    }
    m_model->setStringList(list);
}

void ComManager::loadInterfaces()
{
    QStringList list;
    for (int i = 1; i <= 3; ++i) {
        list << QString("COM%1").arg(i);
    }
    m_model->setStringList(list);
}
