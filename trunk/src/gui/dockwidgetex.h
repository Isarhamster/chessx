/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef DOCKWIDGETEX_H
#define DOCKWIDGETEX_H

#include <QDockWidget>

class DockWidgetEx : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockWidgetEx(const QString &title, QWidget *parent = nullptr, Qt::WindowFlags flags = nullptr);

signals:

public slots:

protected:
    void showEvent(QShowEvent * event);
};

#endif // DOCKWIDGETEX_H
