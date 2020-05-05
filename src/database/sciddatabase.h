#ifndef SCIDDATABASE_H
#define SCIDDATABASE_H

#include "database.h"

/** @ingroup Database
   This class provides read-only access to SCID's binary database.
*/
class ScidDatabase : public Database
{
public:
    ScidDatabase();

    // Database overrides
    /** Opens the given database */
    bool open(const QString& filename, bool utf8) override;
    /** Parse the database */
    bool parseFile() override;
    /** File-based database name */
    QString filename() const override;
    /** Loads only moves into a game from the given position */
    void loadGameMoves(GameId index, GameX& game) override;
    /** Loads game moves and try to find a position */
    int findPosition(GameId index, const BoardX& position) override;

private:
    QString m_filename;
};

#endif // SCIDDATABASE_H
