#ifndef OPENINGTREETHREAD_H
#define OPENINGTREETHREAD_H

#include "filter.h"
#include "game.h"

class MoveData
{
public:
    enum MoveType {StandardMove, GameEnd};
    MoveData();
    void addGame(Game& g, Color c, MoveType movetype = StandardMove);
    double percentage() const;
    int averageRating() const;
    int averageYear() const;
    QString move;
    unsigned count;
    unsigned result[4];
    long rating;
    unsigned rated;
    long year;
    unsigned dated;
    friend bool operator<(const MoveData& m1, const MoveData& m2);
    friend bool compareMove(const MoveData& m1, const MoveData& m2);
    friend bool compareScore(const MoveData& m1, const MoveData& m2);
    friend bool compareRating(const MoveData& m1, const MoveData& m2);
    friend bool compareYear(const MoveData& m1, const MoveData& m2);
};

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
