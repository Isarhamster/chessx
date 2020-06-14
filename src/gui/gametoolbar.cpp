#include "gametoolbar.h"

GameToolBar::GameToolBar(const QString& title, QWidget* parent)
    : QToolBar(title, parent)
{
    setObjectName("GameToolBar");
}
