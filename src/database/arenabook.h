/****************************************************************************
*   Copyright (C) 2017 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ARENABOOK_H
#define ARENABOOK_H

#include "board.h"
#include "database.h"
#include "gamex.h"
#include "movedata.h"

#include <QPointer>

struct ABK_MOVE;

class ArenaBook : public Database
{
    Q_OBJECT
public:
    ~ArenaBook();

    /** Opens the given database */
    virtual bool open(const QString& filename, bool);
    /** Parse the database */
    virtual bool parseFile();
    /** File-based database name */
    virtual QString filename() const;

    /** Get the number of stored games*/
    virtual quint64 count() const;
    /** Get the number of stored positions*/
    quint64 positionCount() const;

    /** Loads a game at @p index, returns true if successful */
    virtual bool loadGame(GameId gameId, GameX& game);
    /** Loads only moves into a game from the given position */
    virtual void loadGameMoves(GameId id, GameX& game);
    /** Loads game moves and try to find a position */
    virtual int findPosition(GameId, const BoardX &position);

    virtual bool hasIndexFile() const { return true; }
    /** Open a book data File */
    bool openFile(const QString& filename, bool readOnly=false);
    /** Closes the database */
    void close();

private :
    void add_move(GameX *game, const ABK_MOVE* move);
    void tag_game(GameX* game, int ply, GameId index);

private: // BOOK Parser
    QString m_filename;
    QPointer<QIODevice> m_file;
    quint64 m_posCount {0};
    quint64 m_count {0};

private: // Game list
    QVector<GameX*> m_games;
};

#endif // ARENABOOK_H
