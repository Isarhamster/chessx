/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include <QMutex>
#include <QMutexLocker>

#include "ctgdatabase.h"
#include "database.h"
#include "openingtreethread.h"
#include "polyglotdatabase.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

OpeningTreeThread::OpeningTreeThread()
{
    m_games = 0;
    m_break = false;
    m_updateFilter = false;
    m_sourceIsDatabase = false;
    m_bEnd = false;
}

void OpeningTreeThread::run()
{
    QMap<Move, MoveData> moves;
    int games = 0;
    emit progress(0);
    QList<MoveData> emptyMoveList;
    emit MoveUpdate(&m_board, emptyMoveList);

    if (PolyglotDatabase* pgdb = qobject_cast<PolyglotDatabase*>(m_filter ? m_filter->database() : 0))
    {
        games = pgdb->getMoveMapForBoard(m_board, moves);
        ProgressUpdate(moves, games, 100, 100);
    }
    else if (CtgDatabase* pgdb = qobject_cast<CtgDatabase*>(m_filter ? m_filter->database() : 0))
    {
        games = pgdb->getMoveMapForBoard(m_board, moves);
        ProgressUpdate(moves, games, 100, 100);
    }
    else
    {
        int n = m_filter ? m_filter->size() : 0;
        int progressCounter = 10+n/100;
        for(int i = 0; i < n; ++i)
        {
            if (m_sourceIsDatabase || m_filter->contains(i))
            {
                Game g;
                m_filter->database()->loadGameMoves(i, g);
                int id = g.findPosition(m_board);
                if((id != NO_MOVE) && (m_bEnd ? g.atGameEnd(id) : true))
                {
                    if(m_updateFilter)
                    {
                        m_filter->set(i, id + 1); // not zero means success, but id could be 0.
                    }
                    m_filter->database()->loadGameHeaders(i, g);
                    g.dbMoveToId(id);
                    if(g.atGameEnd())
                    {
                        moves[Move()].addGame(g, m_board.toMove(), MoveData::GameEnd);
                    }
                    else
                    {
                        g.forward();
                        moves[g.move()].addGame(g, m_board.toMove());
                    }
                    ++games;
                }
                else
                {
                    if(m_updateFilter)
                    {
                        m_filter->set(i, 0);
                    }
                }
            }

            if ((i==n) || (i%progressCounter) == 0)
            {
                ProgressUpdate(moves, games, i, n);
            }

            if(m_break)
            {
                break;
            }
        }
    }
    *m_games = games;
    if(!m_break)
    {
        QList<MoveData> moveList;
        for(QMap<Move, MoveData>::iterator it = moves.begin(); it != moves.end(); ++it)
        {
            moveList.append(it.value());
        }
        emit MoveUpdate(&m_board, moveList);
        emit UpdateFinished(&m_board);
    }
    else
    {
        emit UpdateTerminated(&m_board);
    }
}

void OpeningTreeThread::cancel()
{
    m_break = true;
}

bool OpeningTreeThread::updateFilter(Filter& f, const Board& b, int& g, bool updateFilter, bool sourceIsDatabase, bool bEnd)
{
    m_break = false;
    m_filter = &f;
    m_board = b;
    m_games = &g;
    m_bEnd  = bEnd;
    m_updateFilter = updateFilter;
    m_sourceIsDatabase = sourceIsDatabase;
    // todo: if running wait for stop
    start();
    return true;
}

void OpeningTreeThread::ProgressUpdate(QMap<Move, MoveData> &moves, int games, int i, int n)
{
    emit progress(i * 100 / n);
    if(!m_break)
    {
        *m_games = games;
        QList<MoveData> moveList;
        for(QMap<Move, MoveData>::iterator it = moves.begin(); it != moves.end(); ++it)
        {
            moveList.append(it.value());
        }
        emit MoveUpdate(&m_board, moveList);
    }
}
