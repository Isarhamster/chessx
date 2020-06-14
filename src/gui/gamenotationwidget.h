#ifndef GAMENOTATIONWIDGET_H
#define GAMENOTATIONWIDGET_H

#include <QWidget>

class ChessBrowser;


class GameNotationWidget : public QWidget
{
    Q_OBJECT

public:
    GameNotationWidget(QWidget* parent = nullptr);

public:
    ChessBrowser *m_browser;
};

#endif
