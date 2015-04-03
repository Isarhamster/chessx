/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "dockwidgetex.h"
#include <QAction>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

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
