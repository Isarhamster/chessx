#include "advanceddockwidget.h"
#include "docktitlebar.h"

AdvancedDockWidget::AdvancedDockWidget(const QString& title,
                                       QWidget *parent, Qt::WindowFlags flags)
    : QDockWidget(title, parent)
{
    mTitleBar = new DockTitleBar(this);
    setTitleBarWidget(mTitleBar);
}

void AdvancedDockWidget::addTitleBarExtension(IDockTitleBarExtension *ext)
{
    mTitleBar->addExtension(ext);
}
