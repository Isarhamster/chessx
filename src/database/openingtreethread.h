/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef OPENINGTREETHREAD_H
#define OPENINGTREETHREAD_H

#include "filter.h"
#include "gamex.h"
#include "movedata.h"

#include <QPointer>

class OpeningTreeThread : public QThread
{
    Q_OBJECT
public:
    OpeningTreeThread();
    void run();
    void cancel();
    bool updateFilter(FilterX& f, const BoardX& b, unsigned int&, bool updateFilter, bool sourceIsDatabase, bool bEnd);

signals:
    void requestGameFilterUpdate(int index, int value);
    void MoveUpdate(BoardX*, QList<MoveData>);
    void UpdateFinished(BoardX*);
    void UpdateTerminated(BoardX*);
    void progress(int);

protected:
    void ProgressUpdate(QMap<Move, MoveData>& moves, unsigned int games, int i, int n);
private:
    unsigned int* m_games;

    bool    m_break;
    BoardX   m_board;
    QPointer<FilterX> m_filter;
    bool m_updateFilter;
    bool m_sourceIsDatabase;
    bool m_bEnd;
};

#endif // OPENINGTREETHREAD_H
