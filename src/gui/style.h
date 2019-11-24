/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef STYLE_H
#define STYLE_H

#include <QObject>

class Style : public QObject
{
    Q_OBJECT
public:
    explicit Style(QObject *parent = nullptr);

    static void setStyle(QWidget* w);
signals:

public slots:

};

#endif // STYLE_H
