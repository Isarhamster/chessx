#ifndef ADVANCEDDOCKWIDGET_H
#define ADVANCEDDOCKWIDGET_H

#pragma once

#include <QDockWidget>

class DockTitleBar;
class IDockTitleBarExtension;

class AdvancedDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit AdvancedDockWidget(const QString& title,
                                QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    void addTitleBarExtension(IDockTitleBarExtension *ext);

private:
    DockTitleBar *mTitleBar;
};

#endif // ADVANCEDDOCKWIDGET_H
