/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "threadedguess.h"
#include "guess.h"

#include <QMetaType>

using namespace chessx;

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

ThreadedGuess::ThreadedGuess(bool threat)
{
    m_dontGuess = false;
    m_bSwap = threat;
    thinkTime = 1000;
    clear();

    qRegisterMetaType<Guess::Result>("Guess::Result");
    qRegisterMetaType<BoardX>("BoardX");
}

ThreadedGuess::~ThreadedGuess()
{

}

void ThreadedGuess::cancel()
{
    m_dontGuess = true;
    if (isRunning())
    {
        wait();
    }
}

bool ThreadedGuess::guessMove(BoardX b)
{
    clear();

    if (isRunning()) return false;
    if (!Guess::guessAllowed()) return false;

    m_dontGuess = false;
    m_board = b;
    start();
    return true;
}

void ThreadedGuess::run()
{
    BoardX b = m_board;
    if (m_bSwap)
    {
        b.swapToMove();
        b.clearEnPassantSquare();
    }
    Guess::MoveList moveList;
    Guess::Result sm = Guess::guessMove(qPrintable(b.toFen()),
                                        b.chess960(), b.castlingRooks(),
                                        Guess::NULL_SQUARE,
                                        moveList, thinkTime);
    if (!m_dontGuess)
    {
        if (Guess::guessAllowed() && !sm.error)
        {
            setFrom(Square(sm.from));
            setTo(Square(sm.to));
            emit guessFoundForBoard(sm, m_board);
        }
        else
        {
            clear();
        }
    }
}

void ThreadedGuess::setThinkTime(unsigned int value)
{
    thinkTime = value;
}

chessx::Square ThreadedGuess::getFrom() const
{
    return from;
}

void ThreadedGuess::setFrom(const chessx::Square &value)
{
    from = value;
}

void ThreadedGuess::clear()
{
    from = to = InvalidSquare;
}

chessx::Square ThreadedGuess::getTo() const
{
    return to;
}

void ThreadedGuess::setTo(const chessx::Square &value)
{
    to = value;
}

void ThreadedGuess::setSwap(bool bSwap)
{
    m_bSwap = bSwap;
}

