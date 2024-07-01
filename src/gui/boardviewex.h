/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef BOARDVIEWEX_H
#define BOARDVIEWEX_H

#include <QWidget>

#include "gamex.h"

namespace Ui {
class BoardViewEx;
}

class BoardView;

class BoardViewEx : public QWidget
{
    Q_OBJECT
public:
    explicit BoardViewEx(QWidget *parent = nullptr);
    ~BoardViewEx();

    BoardView* boardView();
    QObject *dbIndex();

public slots:
    void slotReconfigure();
    void saveConfig();
    void showTime(bool show);
    void setTime(bool white, QString t);
    void startTime(bool white);
    void configureTime(bool white, bool countDown);
    void stopTimes();

protected slots:
    void boardIsFlipped(bool, bool);

private:
    void paintEvent(QPaintEvent *pe);

    Ui::BoardViewEx *ui;
};

#endif // BOARDVIEWEX_H
