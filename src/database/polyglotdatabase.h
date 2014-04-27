/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef POLYGLOTDATABASE_H
#define POLYGLOTDATABASE_H

#include "database.h"
#include "movedata.h"

typedef struct {
    quint64 key;
    quint16 move;
    quint16 weight;
    quint32 learn;
} entry_t;

class PolyglotDatabase : public Database
{
    Q_OBJECT
public:
    PolyglotDatabase();
    ~PolyglotDatabase();

    /** Opens the given database */
    virtual bool open(const QString& filename, bool utf8);
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
    quint64 getHashFromBoard(Board b) const;

    /** Open a book data File */
    bool openFile(const QString& filename);
    /** Closes the database */
    void close();
    /** Find Information to a given key */
    bool findMove(quint64 key, MoveData &move);
    /** Start a search for a new key */
    void reset();
signals:

public slots:

protected:
    int find_key(quint64 key, entry_t *entry);
    int entry_from_file(entry_t *entry);
    int int_from_file(int l, quint64 &r);

    QString move_to_string(quint16 move) const;
private:
    QString m_filename;
    QIODevice* m_file;
    quint64 m_count;
};

#endif // POLYGLOTDATABASE_H
