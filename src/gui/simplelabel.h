/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef SIMPLELABEL_H
#define SIMPLELABEL_H

#include <QLabel>

class SimpleLabel : public QLabel
{
public:
    explicit SimpleLabel(QWidget *parent=nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    explicit SimpleLabel(const QString &text, QWidget *parent=nullptr, Qt::WindowFlags f = Qt::WindowFlags());
public:
    void setBackgroundColor(QColor c);
    void resetBackgroundColor();
private:
    QColor m_bgColor;
};

#endif // SIMPLELABEL_H
