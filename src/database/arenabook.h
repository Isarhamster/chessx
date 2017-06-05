/****************************************************************************
*   Copyright (C) 2017 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ARENABOOK_H
#define ARENABOOK_H

#include "board.h"
#include "database.h"
#include "game.h"
#include "movedata.h"

struct ABK_MOVE;

class ArenaBook : public Database
{
    Q_OBJECT
public:
    ArenaBook();
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
    virtual bool loadGame(GameId gameId, Game& game);
    /** Loads only moves into a game from the given position */
    virtual void loadGameMoves(GameId id, Game& game);
    /** Loads game moves and try to find a position */
    virtual int findPosition(GameId, const Board &position);

    virtual bool hasIndexFile() const { return true; }
    /** Open a book data File */
    bool openFile(const QString& filename, bool readOnly=false);
    /** Closes the database */
    void close();

private: // BOOK Parser
    QString m_filename;
    QIODevice* m_file;
    quint64 m_posCount;
    quint64 m_count;
    void add_move(Game *game, const ABK_MOVE* move);
    void tag_game(Game* game, int ply, GameId index);

private: // Game list
    QVector<Game*> m_games;
};

#endif // ARENABOOK_H
