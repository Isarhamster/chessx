#ifndef LICHESSOPENINGDATABASE_H
#define LICHESSOPENINGDATABASE_H

#include "database.h"
#include "lichessopening.h"
#include <QObject>
#include <QString>

class LichessOpeningDatabase : public Database
{
    Q_OBJECT

public:
    LichessOpeningDatabase();
    /** Opens the given database */
    virtual bool open(const QString &filename, bool);
    /** Parse the database */
    virtual bool parseFile() { return true; };
    /** File-based database name */
    virtual QString filename() const { return m_filename; };

    virtual void loadGameMoves(GameId /*index*/, GameX& /*game*/) { };
    /** Loads game moves and try to find a position */
    virtual int findPosition(GameId /*index*/, const BoardX& /*position*/) { return 0; };
    /** Get a map of MoveData from a given board position */
    unsigned int getMoveMapForBoard(const BoardX &board, QMap<Move, MoveData>& moves);

    QString name() const { return m_filename; };

private:
    QString m_filename;
    LichessOpening m_client;
    QStringList parseTopGame(const QJsonArray& topGames);

};

#endif // LICHESSOPENINGDATABASE_H
