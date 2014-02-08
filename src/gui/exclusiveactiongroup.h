/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef EXCLUSIVEACTIONGROUP_H
#define EXCLUSIVEACTIONGROUP_H

class QAction;

#include <QList>
#include <QObject>

class ExclusiveActionGroup : public QObject
{
    Q_OBJECT
public:
    explicit ExclusiveActionGroup(QObject *parent = 0);

    QAction * addAction ( QAction * action );

signals:

protected slots:
    void slotActionToggled(bool);

private:
    QList<QAction*> actions;
};

#endif // EXCLUSIVEACTIONGROUP_H
