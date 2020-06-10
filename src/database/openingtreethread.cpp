/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include <QMutex>
#include <QMutexLocker>

#include "ctgdatabase.h"
#include "database.h"
#include "openingtreethread.h"
#include "polyglotdatabase.h"

using namespace chessx;

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

OpeningTreeThread::OpeningTreeThread()
{
    m_games = nullptr;
    m_break = false;
    m_updateFilter = false;
    m_sourceIsDatabase = false;
    m_bEnd = false;
}

void OpeningTreeThread::run()
{
    QMap<Move, MoveData> moves;
    unsigned int games = 0;
    emit progress(0);
    QList<MoveData> emptyMoveList;
    emit MoveUpdate(&m_board, emptyMoveList);

    if (PolyglotDatabase* pgdb = qobject_cast<PolyglotDatabase*>(m_filter ? m_filter->database() : nullptr))
    {
        games = pgdb->getMoveMapForBoard(m_board, moves);
        ProgressUpdate(moves, games, 100, 100);
    }
    else if (CtgDatabase* pgdb = qobject_cast<CtgDatabase*>(m_filter ? m_filter->database() : nullptr))
    {
        games = pgdb->getMoveMapForBoard(m_board, moves);
        ProgressUpdate(moves, games, 100, 100);
    }
    else if (m_filter)
    {
        const auto batchSize = 100;

        // setup buffers for batch processing
        QList<GameId> rqBuffer;
        QList<MoveId> rsBuffer;
        rqBuffer.reserve(batchSize);
        rsBuffer.reserve(batchSize);

        // determine options
        Database::PositionSearchOptions opts = Database::PositionSearch_Default;
        if (m_bEnd)
            opts = Database::PositionSearch_GameEnd;

        // scan games
        int total = m_filter->size();
        int processed = 0;
        while (processed < total)
        {
            // prepare requests
            rqBuffer.clear();
            rsBuffer.clear();
            if (m_sourceIsDatabase)
            {
                auto size = std::min(total - processed, batchSize);
                for (auto i = 0; i < size; ++i)
                {
                    auto gameId = processed + i;
                    rqBuffer.append(gameId);
                }
                processed += size;
            }
            else
            {
                for (; processed < total && rqBuffer.size() < batchSize; ++processed)
                {
                    auto gameId = processed;
                    if (m_filter->contains(gameId))
                        rqBuffer.append(gameId);
                }
            }

            // perform search
            m_filter->database()->findPosition(m_board, opts, rqBuffer, rsBuffer, moves);

            // update progress
            for (auto rs: rsBuffer)
            {
                if (rs != NO_MOVE)
                    games += 1;
            }
            ProgressUpdate(moves, games, processed, total);

            // update filter if necessary
            if (m_updateFilter)
            {
                for (auto i = 0; i < rqBuffer.size(); ++i)
                {
                    auto rq = rqBuffer.at(i);
                    auto rs = rsBuffer.at(i);
                    emit requestGameFilterUpdate(rq, rs + 1);
                }
            }

            // interrupt if requested
            if (m_break)
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

bool OpeningTreeThread::updateFilter(FilterX& f, const BoardX& b, unsigned int& g, bool updateFilter, bool sourceIsDatabase, bool bEnd)
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

void OpeningTreeThread::ProgressUpdate(QMap<Move, MoveData> &moves, unsigned int games, int i, int n)
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
