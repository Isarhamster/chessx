#include "sciddatabase.h"

ScidDatabase::ScidDatabase()
{

}

bool ScidDatabase::open(const QString& filename, bool utf8)
{
    (void)utf8;
    m_filename = filename;
    return true;
}

bool ScidDatabase::parseFile()
{
    return true;
}

QString ScidDatabase::filename() const
{
    return m_filename;
}

void ScidDatabase::loadGameMoves(GameId index, GameX& game)
{
    (void)index;
    (void)game;
}

int ScidDatabase::findPosition(GameId index, const BoardX& position)
{
    (void)index;
    (void)position;
    return NO_MOVE;
}
