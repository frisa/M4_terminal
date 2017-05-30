#ifndef ROOTHANDLER_H
#define ROOTHANDLER_H

#include <QObject>

class RootHandler : public QObject
{
    Q_OBJECT
public:
    explicit RootHandler(QObject *parent = 0);

signals:

public slots:
    void handleButtonSlot(void);
};

#endif // ROOTHANDLER_H
