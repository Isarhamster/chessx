/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "simplelabel.h"

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

