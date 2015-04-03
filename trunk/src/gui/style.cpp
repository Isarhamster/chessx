/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "settings.h"
#include "style.h"

#include <QWidget>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER


Style::Style(QObject *parent) :
    QObject(parent)
{
}

void Style::setStyle(QWidget *w)
{
    QFile style(":/styles/orange.css");
    if (style.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&style);
        QString content = stream.readAll();
        w->setStyleSheet(content);
    }
}
