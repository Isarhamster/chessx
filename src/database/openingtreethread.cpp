/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "openingtreethread.h"
#include "database.h"
#include "polyglotdatabase.h"

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
    Game g;
    QMap<Move, MoveData> moves;
    int games = 0;
    QTime updateTime = QTime::currentTime().addSecs(1);
    emit progress(0);
    emit MoveUpdate(&m_board, new QList<MoveData>);
    int n = m_filter->size();
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
                Move move = b.parseMove(m.san);
                m.san = b.moveToSan(move);
                moves[move] = m; // Dummy entry, just to get a map key!
                games += m.count;
            }

            ProgressUpdate(moves, updateTime, games, i, n);

            if(m_break)
            {
                break;
            }
        }
    }
    else for(int i = 0; i < n; ++i)
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
                g.moveToId(id);
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
    *m_games = games;
    if(!m_break)
    {
        QList<MoveData>* moveList = new QList<MoveData>();
        for(QMap<Move, MoveData>::iterator it = moves.begin(); it != moves.end(); ++it)
        {
            moveList->append(it.value());
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
            QList<MoveData>* moveList = new QList<MoveData>();
            for(QMap<Move, MoveData>::iterator it = moves.begin(); it != moves.end(); ++it)
            {
                moveList->append(it.value());
            }
            emit MoveUpdate(&m_board, moveList);
        }
    }
}
