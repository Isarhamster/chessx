/****************************************************************************
*   Copyright (C) 2017 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "arenabook.h"
#include "abk.h"
#include "game.h"
#include "tags.h"

#include <QMutexLocker>

// ---------------------------------------------------------
// construction
// ---------------------------------------------------------

ArenaBook::ArenaBook() :
    Database(),
    m_file(0),
    m_posCount(0),
    m_count(0)
{
}

ArenaBook::~ArenaBook()
{
    for(int i = 0; i < m_games.count(); ++i)
    {
        delete m_games[i];
    }
    m_games.clear();
    m_index.clear();
    close();
}

// ---------------------------------------------------------
// Database Interface implementation
// ---------------------------------------------------------

bool ArenaBook::open(const QString &filename, bool)
{
    if(m_file)
    {
        return false;
    }
    m_break = false;
    m_filename = filename;
    if(openFile(filename, true))
    {
        m_utf8 = false;
        QFileInfo fi(m_filename);
        m_posCount = fi.size() / sizeof(ABK_ENTRY); // TODO
        return true;
    }
    return false;
}

void ArenaBook::add_move(Game* game, const ABK_MOVE* move)
{
    if ((move->from >> 3) + 1 == 32 && (move->to >> 3) + 1 == 32) return;

    Move m = game->board().prepareMove((Square)move->from, (Square)move->to);

    // Check the promotion piece and convert
    if (m.isPromotion() && move->promotion)
    {
        static const PieceType promotionPiece[] = { None, Rook, Knight, Bishop, Queen };
        m.setPromoted(promotionPiece[move->promotion]);
    }

    game->dbAddMove(m);
}

void ArenaBook::tag_game(Game* game, int ply, GameId index)
{
    // Determine tags
    QString nameWhite = QString("Game %1").arg(m_count+1);
    game->setTag(TagNameWhite, nameWhite);
    game->setTag(TagNamePlyCount, QString::number(ply));
    QString valLength = QString::number((game->plyCount() + 1) / 2);
    game->setTag(TagNameLength, valLength);
    QString eco = game->ecoClassify().left(3);
    if(!eco.isEmpty())
    {
        game->setTag(TagNameECO, eco);
    }
    // Copy them to the index as well
    setTagsToIndex(*game, index);
}

bool ArenaBook::parseFile()
{
    if (!m_file) return false;

    QMutexLocker m(&m_mutex);

    m_count = 0;

    QByteArray ba = m_file->readAll();
    const ABK_ENTRY* arena_book = (ABK_ENTRY*) ba.constData();

    int ply = 0;
    int node[1000] = { 0 };
    ABK_MOVE move_stack[1000];

    ply = 0;
    node[0] = 900;  // offset to first node in abk-file

    Game* game = new Game;

    while(1)
    {
        add_move(game, &arena_book[node[ply]].move);

        if (arena_book[node[ply]].first_child > 0)
        {  // current game

            move_stack[ply] = arena_book[node[ply]].move;
            node[ply + 1] = arena_book[node[ply]].first_child;
            ++ply;
        }
        else
        {
            m_count = m_index.add();

            tag_game(game, ply, m_count);
            game->unmountBoard();
            m_games.append(game);

            node[ply] = arena_book[node[ply]].next_sibling;
            while (node[ply] < 0)
            {
                --ply;
                if (ply < 0)
                {
                    return true; // We are done!
                }
                node[ply] = arena_book[node[ply]].next_sibling;
            }
            ++m_count;
            game = new Game;
            for (int i = 0; i < ply; i++)
            {
                add_move(game, &move_stack[i]);
            }
        }
        if (node[ply] >= (int) m_posCount)
        {
            delete game;
            return false; // Node out of range
        }
    }

    close(); // no need to keep it open, everything is in memory
}

QString ArenaBook::filename() const
{
    return m_filename;
}

quint64 ArenaBook::count() const
{
    return m_count;
}

quint64 ArenaBook::positionCount() const
{
    return m_posCount;
}

bool ArenaBook::loadGame(GameId gameId, Game& game)
{
    loadGameMoves(gameId, game);
    return true;
}

void ArenaBook::loadGameMoves(GameId gameId, Game & game)
{
    game = *m_games[gameId];
    loadGameHeaders(gameId, game);
}

int ArenaBook::findPosition(GameId index, const Board &position)
{
    Game g;
    loadGameMoves(index, g);
    return g.findPosition(position);
}

bool ArenaBook::openFile(const QString &filename, bool readOnly)
{
    //open file
    QFile* file = new QFile(filename);
    if(readOnly && !file->exists())
    {
        delete file;
        return false;
    }
    if (file->open(readOnly ? QIODevice::ReadOnly : QIODevice::WriteOnly))
    {
        m_file = file;
        return true;
    }

    delete file;
    return false;
}

void ArenaBook::close()
{
    //close the file, and delete objects
    if(m_file)
    {
        m_file->close();
    }
    delete m_file;
    m_file = 0;
}
