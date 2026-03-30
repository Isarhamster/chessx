#ifndef DOCKTITLEBAR_H
#define DOCKTITLEBAR_H

#pragma once

#include <QWidget>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QFrame>

class IDockTitleBarExtension;

class DockTitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit DockTitleBar(QDockWidget *dock);

    void addExtension(IDockTitleBarExtension *ext);

protected:
    void enterEvent(QEnterEvent *) override;
    void leaveEvent(QEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;

private:
    void addDefaultButtons();

    QDockWidget *mDock;
    QLabel *mTitle;
    QHBoxLayout *mLayout;

    QList<QToolButton*> mHoverButtons;
private:
    void setHoverVisible(bool visible);
};

#endif // DOCKTITLEBAR_H
