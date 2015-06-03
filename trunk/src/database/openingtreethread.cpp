/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "openingtreethread.h"
#include "database.h"
#include "polyglotdatabase.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

double MoveData::percentage() const
{
    unsigned c = result[ResultUnknown] + 2 * result[WhiteWin] + result[Draw];
    return c * 500 / count / 10.0;
}

bool MoveData::hasPercent() const
{
    int n = 0;
    for (int i=0; i<4; ++i)
    {
        n += result[i];
    }
    return (n>0);
}
int MoveData::averageRating() const
{
    return rated ? rating / rated : 0;
}

int MoveData::averageYear() const
{
    return dated ? year / dated : 0;
}

bool operator<(const MoveData& m1, const MoveData& m2)
{
    return m1.count < m2.count || (m1.count == m2.count && m1.san < m2.san);
}

void OpeningTreeThread::run()
{
    QMap<Move, MoveData> moves;
    int games = 0;
    QTime updateTime = QTime::currentTime().addSecs(1);
    emit progress(0);
    QList<MoveData> moveList;
    emit MoveUpdate(&m_board, moveList);
    int n = m_filter ? m_filter->size() : 0;
    if (PolyglotDatabase* pgdb = qobject_cast<PolyglotDatabase*>(m_filter->database()))
    {
        n = pgdb->positionCount();
        quint64 key = pgdb->getHashFromBoard(m_board);
        pgdb->reset();

        for (int i=0; i<n; ++i)
        {
            MoveData m;
            if (pgdb->findMove(key,m))
            {
                Board b = m_board;
                if (b.pieceAt(e1)==WhiteKing)
                {
                    if (m.san=="e1a1") m.san = "e1c1";
                    else if (m.san=="e1h1") m.san = "e1g1";
                }
                if (b.pieceAt(e8)==BlackKing)
                {
                    if (m.san=="e8a8") m.san = "e8c8";
                    else if (m.san=="e8h8") m.san = "e8g8";
                }

                Move move = b.parseMove(m.san);
                m.san = b.moveToSan(move);
                moves[move] = m;
                games += m.count;
            }

            ProgressUpdate(moves, updateTime, games, i, n);

            if(m_break)
            {
                break;
            }
        }
    }
    else
    {
        Game g;
        MountBoard m(g);
        for(int i = 0; i < n; ++i)
        {
            if (m_sourceIsDatabase || m_filter->contains(i))
            {
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

            ProgressUpdate(moves, updateTime, games, i, n);

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

void OpeningTreeThread::ProgressUpdate(QMap<Move, MoveData> &moves, QTime& updateTime, int games, int i, int n)
{
    if (updateTime<=QTime::currentTime())
    {
        updateTime = QTime::currentTime().addSecs(3);
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
}
