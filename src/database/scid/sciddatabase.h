#ifndef SCIDDATABASE_H
#define SCIDDATABASE_H

#include <memory>

#include "database.h"

class ScidStorage;

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
    /** Database size on disk */
    qint64 diskSize() const override;
    /** Loads a game at @p index, returns true if successful */
    bool loadGame(GameId gameId, GameX& game) override;
    /** Loads only moves into a game from the given position */
    void loadGameMoves(GameId index, GameX& game) override;
    /** Loads game moves and try to find a position */
    int findPosition(GameId index, const BoardX& position) override;
    /** Returns the number of games in the database */
    quint64 count() const override;

private:
    QString m_filename;
    QStringList m_files;
    std::unique_ptr<ScidStorage> m_storage;
};

/** Base class for implementing \p Progress::Impl adapter
 */
class ProgressBase: public QObject
{
    Q_OBJECT

public:
    ProgressBase(volatile bool *interrupt): m_interrupt(interrupt)
    {}

signals:
    void progressValueChanged(int percents);
    void progressMessage(QString message);

protected:
    volatile bool *m_interrupt;
};


#endif // SCIDDATABASE_H
