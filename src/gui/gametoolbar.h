#ifndef GAMETOOLBAR_H
#define GAMETOOLBAR_H

#include <QToolBar>

class GameToolBar : public QToolBar
{
    Q_OBJECT

public:
    GameToolBar(const QString& title, QWidget* parent = nullptr);
};

#endif
