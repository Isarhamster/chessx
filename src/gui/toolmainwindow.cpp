/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "settings.h"
#include "toolmainwindow.h"
#include <QtCore>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

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
