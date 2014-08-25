#ifndef CTGDATABASE_H
#define CTGDATABASE_H

#include "database.h"
#include "movedata.h"

class CtgDatabase : public Database
{
    Q_OBJECT
public:
    CtgDatabase();
    ~CtgDatabase();

    /** Opens the given database */
    virtual bool open(const QString& filename, bool);
    virtual bool openForWriting(const QString& filename, int maxPly, int minGame, bool uniform);
    /** Parse the database */
    virtual bool parseFile();
    /** File-based database name */
    virtual QString filename() const;

    /** Get the number of stored games*/
    virtual quint64 count() const;
    /** Get the number of stored positions*/
    quint64 positionCount() const;

    /** Loads only moves into a game from the given position */
    virtual void loadGameMoves(int index, Game& game);
signals:

public slots:

};

#endif // CTGDATABASE_H
