/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "dockwidgetex.h"
#include <QAction>

DockWidgetEx::DockWidgetEx(const QString &title, QWidget *parent, Qt::WindowFlags flags) :
    QDockWidget(title, parent, flags)
{
}

void DockWidgetEx::showEvent(QShowEvent * event)
{
    QDockWidget::showEvent(event);
    if(isVisible() && qobject_cast<QAction*>(sender()))
    {
        raise();
    }
}
