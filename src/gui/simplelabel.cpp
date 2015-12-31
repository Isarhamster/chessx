/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "simplelabel.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

SimpleLabel::SimpleLabel(QWidget *parent, Qt::WindowFlags f)
    :QLabel(parent,f)
{
    setAutoFillBackground(true);
    QPalette pal = palette();
    m_bgColor = pal.color(backgroundRole());
}

SimpleLabel::SimpleLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    :QLabel(text,parent,f)
{
    setAutoFillBackground(true);
    QPalette pal = palette();
    m_bgColor = pal.color(backgroundRole());
}

void SimpleLabel::setBackgroundColor(QColor c)
{
    QPalette pal = palette();
    pal.setColor(backgroundRole(), c);
    setPalette(pal);
}

void SimpleLabel::resetBackgroundColor()
{
   setBackgroundColor(m_bgColor);
}

