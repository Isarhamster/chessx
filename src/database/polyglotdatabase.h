/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef POLYGLOTDATABASE_H
#define POLYGLOTDATABASE_H

#include <QMutex>

#include "database.h"
#include "movedata.h"

typedef struct _entry_t
{
    quint64 key;
    quint16 move;
    quint16 weight;
    quint32 learn;
} entry_t;

typedef struct _book_key
{
    inline bool operator<(const _book_key& k2)const
    {
        Q_ASSERT(this!=&k2);
        if (key > k2.key)
        {
            return false;
        }
        else if (key < k2.key)
        {
            return true;
        }
        else if (k2.move > move)
        {
            return false;
        }
        else if (k2.move < move)
        {
            return true;
        }
        return false;
    }
    quint64 key;
    quint16 move;
} book_key;

typedef struct _book_value
{
    _book_value() : n(0),sum(0) {}
    quint32 n;
    quint32 sum;
} book_value;

typedef struct _book_entry
{
   quint64 key;
   quint16 move;
   quint32 n;
   quint32 sum;
   _book_entry()
   {
       key=0;
       move=0;
       n=0;
       sum=0;
   }
   _book_entry(book_key k, book_value v)
   {
       key=k.key;
       move=k.move;
       n=v.n;
       sum=v.sum;
   }
   inline bool operator<(const _book_entry& k2)const
   {
       Q_ASSERT(this!=&k2);
       if (key > k2.key)
       {
           return false;
       }
       else if (key < k2.key)
       {
           return true;
       }
       else if (k2.sum > sum) // highest score first - do not respect uniform probs here
       {
           return false;
       }
       else if (k2.sum < sum)
       {
           return true;
       }
       return &k2 > this; // Make sorting deterministic
   }
} book_entry;

typedef QList<book_entry> Book;
typedef QMap<book_key,book_value> BookMap;

class PolyglotDatabase : public Database
{
    Q_OBJECT
public:
    PolyglotDatabase();
    ~PolyglotDatabase();

    /** Opens the given database */
    virtual bool open(const QString& filename, bool);
    virtual bool openForWriting(const QString& filename, int maxPly, int minGame, bool uniform, int result, int filterResult);
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
    int find_key(quint64 key, entry_t *entry);
    int entry_from_file(entry_t *entry);
    int int_from_file(int l, quint64 &r);

    QString move_to_string(quint16 move) const;
    void book_save();
    void write_integer(int size, quint64 n);
    int entry_score(const book_entry& entry);
    bool keep_entry(const book_entry &entry);
    void halve_stats(quint64 key);
    void book_sort();
    void spool_map();
    void overflow_correction();
    void update_entry(book_entry &entry, int result);
    book_entry *find_entry(const book_entry &entry);
    void book_filter();
    void add_database(Database &db);
    void add_game(Game &g, int result);
    bool get_move_entry(Move m, book_entry &entry) const;
    int get_promotion(Move m) const;
    int make_castling_move(Move m) const;
    int make_move(int from, int to) const;
private:
    QString m_filename;
    QIODevice* m_file;
    quint64 m_count;
    Book m_book;
    BookMap m_bookDictionary;
    bool m_uniform;
    int m_overwriteResult;
    int m_filterResult;
    quint32 m_minGame;
    int m_maxPly;
};

#endif // POLYGLOTDATABASE_H
