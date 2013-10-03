/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TOOLMAINWINDOW_H
#define TOOLMAINWINDOW_H

#include <QMainWindow>

class ToolMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ToolMainWindow(QWidget *parent = 0);

signals:

public slots:
    void saveConfig();
    void slotReconfigure();
};

#endif // TOOLMAINWINDOW_H
