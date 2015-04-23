/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef THREADEDGUESS_H
#define THREADEDGUESS_H

#include "board.h"
#include "guess.h"

#include <QThread>

class ThreadedGuess : public QThread
{
    Q_OBJECT

public:
    ThreadedGuess();
    ~ThreadedGuess();

    void cancel();
    bool guessMove(Board b);

protected:
    virtual void run();

signals:
    void guessFoundForBoard(Guess::Result, Board);

private:
    bool m_dontGuess;
    Board m_board;
};

#endif // THREADEDGUESS_H
