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
    AppSettings->setLayout(this);
}

void ToolMainWindow::slotReconfigure()
{
    AppSettings->layout(this);
}
