#ifndef COMMANAGER_H
#define COMMANAGER_H

#include <QObject>
#include <QStringListModel>

class ComManager : public QObject
{
    Q_OBJECT
    QStringListModel * m_model;
public:
    explicit ComManager(QObject *parent = 0);
    ComManager(QStringListModel * model);
    void loadInterfaces();

signals:

public slots:
};

#endif // COMMANAGER_H
