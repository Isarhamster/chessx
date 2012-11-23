/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "settings.h"
#include "toolmainwindow.h"
#include <QtCore>

ToolMainWindow::ToolMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowFlags(Qt::Widget);
}

void ToolMainWindow::saveConfig()
{
    // saving subwindows does not work anymore as of Qt 4.4
}

void ToolMainWindow::slotReconfigure()
{
    // restoring subwindows does not work anymore as of Qt 4.4
}
