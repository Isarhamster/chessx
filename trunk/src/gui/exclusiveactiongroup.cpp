/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "exclusiveactiongroup.h"
#include <QAction>

ExclusiveActionGroup::ExclusiveActionGroup(QObject *parent) :
    QObject(parent)
{
}

QAction * ExclusiveActionGroup::addAction ( QAction * action )
{
    actions.push_back(action);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(slotActionToggled(bool)));
    return action;
}

void ExclusiveActionGroup::slotActionToggled ( bool checked )
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action && checked)
    {
        foreach(QAction* uncheck, actions)
        {
            if (uncheck != action)
            {
                uncheck->setChecked(false); // triggers recursion, doesnt matter though
            }
        }
    }
}
