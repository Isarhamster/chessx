/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "exclusiveactiongroup.h"
#include <QAction>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

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

const QAction* ExclusiveActionGroup::checkedAction() const
{
    foreach(const QAction* a, actions)
    {
        if (a->isChecked())
        {
            return a;
        }
    }
    return 0;
}

void ExclusiveActionGroup::untrigger()
{
    foreach(QAction* a, actions)
    {
        if (a->isChecked())
        {
            a->trigger();
            break;
        }
    }
}
