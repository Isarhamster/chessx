#include "openingtreethread.h"
#include "database.h"

MoveData::MoveData()
{
    count = 0;
    for(int  r = ResultUnknown; r <= BlackWin; ++r)
    {
        result[r] = 0;
    }
    year = rating = 0;
    dated = rated = 0;

}

void MoveData::addGame(Game& g, Color c, MoveType movetype)
{
    if(!count)
        move = (movetype == StandardMove) ? g.moveToSan(Game::MoveOnly, Game::PreviousMove)
               : qApp->translate("MoveData", "[end]");
    ++count;
    result[g.result()]++;
    unsigned elo = (c == White) ? g.tag("WhiteElo").toInt() : g.tag("BlackElo").toInt();
    if(elo >= 1000)
    {
        rating += elo;
        ++rated;
    }
    unsigned y = g.tag("Date").section(".", 0, 0).toInt();
    if(y > 1000)
    {
        year += y;
        ++dated;
    }
}

double MoveData::percentage() const
{
    unsigned c = result[ResultUnknown] + 2 * result[WhiteWin] + result[Draw];
    return c * 500 / count / 10.0;
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
    return m1.count < m2.count || (m1.count == m2.count && m1.move < m2.move);
}

void OpeningTreeThread::run()
{
    Game g;
    QMap<Move, MoveData> moves;
    int games = 0;
    QTime updateTime = QTime::currentTime().addSecs(1);
    emit progress(0);
    emit MoveUpdate(&m_board, new QList<MoveData>);
    for(int i = 0; i < m_filter->size(); ++i)
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

        if (updateTime<=QTime::currentTime())
        {
            updateTime = QTime::currentTime().addSecs(3);
            emit progress(i * 100 / m_filter->size());
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
