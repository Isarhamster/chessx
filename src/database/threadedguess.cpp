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

ThreadedGuess::ThreadedGuess()
{
    m_dontGuess = false;

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
    b.swapToMove();
    b.clearEnPassantSquare();
    Guess::MoveList moveList;
    Guess::Result sm = Guess::guessMove(qPrintable(b.toFen()),
                                        b.chess960(), b.castlingRooks(),
                                        Guess::NULL_SQUARE,
                                        moveList, 1500);
    if (!m_dontGuess)
    {
        if (Guess::guessAllowed())
        {
            emit guessFoundForBoard(sm, m_board);
        }
    }
}

