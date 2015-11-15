/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef SIMPLELABEL_H
#define SIMPLELABEL_H

#include <QLabel>

class SimpleLabel : public QLabel
{
public:
    explicit SimpleLabel(QWidget *parent=0, Qt::WindowFlags f=0);
    explicit SimpleLabel(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);
public:
    void setBackgroundColor(QColor c);
    void resetBackgroundColor();
private:
    QColor m_bgColor;
};

#endif // SIMPLELABEL_H
