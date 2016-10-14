/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef BOARDVIEWEX_H
#define BOARDVIEWEX_H

#include <QWidget>

namespace Ui {
class BoardViewEx;
}

class BoardView;

class BoardViewEx : public QWidget
{
    Q_OBJECT

public:
    explicit BoardViewEx(QWidget *parent = 0);
    ~BoardViewEx();

    BoardView* boardView();
    QObject *dbIndex();

    void showTime(bool show);
    void setTime(bool white, QString t);

private:
    Ui::BoardViewEx *ui;
};

#endif // BOARDVIEWEX_H
