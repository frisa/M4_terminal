#ifndef SOLUTIONREPO_H
#define SOLUTIONREPO_H

#include <QObject>
#include <QStringListModel>

class Solution : public QObject
{
    Q_OBJECT
    QStringListModel * m_model;
public:
    explicit Solution(QObject *parent = 0);
    Solution(QStringListModel * model);
    Q_INVOKABLE void updateSolutions(const QString text);
signals:

public slots:
    void load();
};

#endif // SOLUTIONREPO_H
