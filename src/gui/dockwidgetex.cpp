#include "dockwidgetex.h"

DockWidgetEx::DockWidgetEx(const QString &title, QWidget *parent, Qt::WindowFlags flags) :
    QDockWidget(title, parent, flags)
{
}

DockWidgetEx::DockWidgetEx(QWidget *parent, Qt::WindowFlags flags) :
    QDockWidget(parent, flags)
{
    connect(this, SIGNAL(visibilityChanged(bool)),
            this, SLOT(onVisibilityChanged(bool)));
}

void DockWidgetEx::showEvent ( QShowEvent * event )
{
    QDockWidget::showEvent(event);
    if (isVisible())
    {
        raise();
    }
}
