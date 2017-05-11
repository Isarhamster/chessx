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
    virtual void loadGameMoves(GameId index, Game& game);
    virtual int findPosition(GameId index, const Board& position);
    quint64 getHashFromBoard(Board b) const;

    /** Open a book data File */
    bool openFile(const QString& filename, bool readOnly=false);
    /** Closes the database */
    void close();
    /** Find Information to a given key */
    bool findMove(quint64 key, MoveData &move);
    /** Start a search for a new key */
    void reset();
    void book_make(Database& db, volatile bool& breakFlag);
signals:

public slots:

protected:

private:
    QString m_filename;
    QIODevice* m_file;
    quint64 m_count;
};

#endif // CTGDATABASE_H
