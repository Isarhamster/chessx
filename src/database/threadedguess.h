/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef THREADEDGUESS_H
#define THREADEDGUESS_H

#include "board.h"
#include "guess.h"
#include "square.h"

#include <QThread>

class ThreadedGuess : public QThread
{
    Q_OBJECT

public:
    ThreadedGuess(bool threat=true);
    ~ThreadedGuess();

    void cancel();
    bool guessMove(BoardX b);

    void setSwap(bool bSwap=true);

    chessx::Square getTo() const;
    void setTo(const chessx::Square &value);

    chessx::Square getFrom() const;
    void setFrom(const chessx::Square &value);

    void clear();

    void setThinkTime(unsigned int value);

protected:
    virtual void run();

signals:
    void guessFoundForBoard(Guess::Result, BoardX);

private:
    bool m_dontGuess;
    bool m_bSwap;
    unsigned int thinkTime;
    chessx::Square from;
    chessx::Square to;
    BoardX m_board;
};

#endif // THREADEDGUESS_H
