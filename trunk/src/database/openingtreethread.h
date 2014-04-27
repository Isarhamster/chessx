/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef OPENINGTREETHREAD_H
#define OPENINGTREETHREAD_H

#include "filter.h"
#include "game.h"
#include "movedata.h"

class OpeningTreeThread : public QThread
{
    Q_OBJECT
public:
    void run();
    void cancel();
    bool updateFilter(Filter& f, const Board& b, int&, bool updateFilter, bool sourceIsDatabase, bool bEnd);

signals:
    void MoveUpdate(Board*, QList<MoveData>*);
    void UpdateFinished(Board*);
    void UpdateTerminated(Board*);
    void progress(int);

protected:
    void ProgressUpdate(QMap<Move, MoveData>& moves, QTime &updateTime, int games, int i, int n);
private:
    int* m_games;

    bool    m_break;
    Board   m_board;
    Filter* m_filter;
    bool m_updateFilter;
    bool m_sourceIsDatabase;
    bool m_bEnd;
};

#endif // OPENINGTREETHREAD_H
