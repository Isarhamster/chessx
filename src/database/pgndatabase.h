/***************************************************************************
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net>      *
 *   (C) 2006 Ejner Borgbjerg <ejner@users.sourceforge.net>                *
 *   (C) 2007 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __PGNDATABASE_H__
#define __PGNDATABASE_H__

#include <QFile>
#include <QByteArray>

#include "database.h"

/** @ingroup Database
   The PgnDatabase class provides database access to PGN files.
   The class is derived from the Database class, providing methods for the
   loading and saving of games, and for performing searches and queries.
*/

typedef qint64 IndexBaseType;

class PgnDatabase : public Database
{
    Q_OBJECT
public:
    /** Default constructor */
    PgnDatabase(bool b64Bit);
    /** Destructor */
    ~PgnDatabase();
    //database operations
    /** Opens the given database */
    virtual bool open(const QString& filename, bool utf8);
    /** File-based database name */
    virtual QString filename() const;
    /** Closes the database */
    void close();

    //game retrieval & storage
    /** Loads a game from the given position, returns true if successful */
    bool loadGame(GameId gameId, Game& game);
    /** Loads only moves into a game from the given position */
    void loadGameMoves(GameId gameId, Game& game);
    virtual int findPosition(GameId index, const Board& position);
    /** Open a PGN Data File from a string */
    bool openString(const QString& content);

    /** Get the number of games from a database */
    virtual quint64 count() const;

protected:
    //parsing methods
    /** Reads moves from the file and adds them to the game. Performs position searches if any are active */
    bool parseMoves(Game* game);
    /** Parses a line from the file */
    void parseLine(Game* game);
    /** Parses a move token from the file */
    void parseDefaultToken(Game* game, QString token);
    /** Parses a token from the file */
    void parseToken(Game* game, const QStringRef &token);
    /** Parses a comment from the file */
    void parseComment(Game* game);
    /** Skips past any data which is not valid tag or move data */
    IndexBaseType skipJunk();
    /** Skips past any tag data */
    void skipTags();
    /** Skips past any move data */
    void skipMoves();
    /** Parses the tags, and adds the supported types to the index 'm_index' */
    void parseTagsIntoIndex();

    virtual bool parseFile();
    bool parseFileIntern();
    virtual void parseGame();

    bool readIndexFile(QDataStream& in, volatile  bool *breakFlag, short version);
    bool writeIndexFile(QDataStream& out) const;
    QString offsetFilename(const QString& filename) const;
    bool readOffsetFile(const QString&, volatile bool *breakFlag, bool &bUpdate);
    bool writeOffsetFile(const QString&) const;

    // Open a PGN data File
    bool openFile(const QString& filename);

    IndexBaseType m_count; // Should actually be a GameId - but cannot be changed due to serialization issues

    /** Adds the current file position as a new offset */
    void addOffset();

    QIODevice* m_file;
    QString m_currentLine;

    bool hasIndexFile() const;

    /** Resets/initialises important member variables. Called by constructor and close methods */
    void initialise();

    //file methods
    /** Reads the next line of text from the PGN file */
    void readLine();
    /** Skips the next line of text from the PGN file */
    void skipLine();
    /** Moves the file position to the start of the given game */
    void seekGame(GameId gameId);

    void prepareNextLineForMoveParser();
    void prepareNextLine();
private:

    //file variables
    QString m_filename;
    QString m_gameText;

    //parsing variables

    bool m_gameOver;
    bool m_inComment;
    bool m_inPreComment;
    QString m_comment;
    QString m_precomment;
    bool m_newVariation;
    int m_variation;

    //game index
    static const int AllocationSize = 16384;
    IndexBaseType m_allocated;
    qint32* m_gameOffsets32;
    qint64* m_gameOffsets64;
    QByteArray m_lineBuffer;

    bool bUse64bit;

    //offset methods
    /** Returns the file offset for the given game */
    inline IndexBaseType offset(GameId gameId)
    {
        if(bUse64bit)
        {
            return m_gameOffsets64[gameId];
        }
        else
        {
            return m_gameOffsets32[gameId];
        }
    }

    /** Adds a new file offset */
    inline void addOffset(IndexBaseType offset)
    {
        if(m_count == m_allocated)
        {
            //out of space reallocate memory
            if(bUse64bit)
            {
                qint64* newAllocation = new qint64[m_allocated += AllocationSize];
                memcpy(newAllocation, m_gameOffsets64, m_count * sizeof(qint64));
                delete[] m_gameOffsets64;
                m_gameOffsets64 = newAllocation;
            }
            else
            {
                qint32* newAllocation = new qint32[m_allocated += AllocationSize];
                memcpy(newAllocation, m_gameOffsets32, m_count * sizeof(qint32));
                delete[] m_gameOffsets32;
                m_gameOffsets32 = newAllocation;
            }
        }

        if(bUse64bit)
        {
            m_gameOffsets64[m_count++] = offset;
        }
        else
        {
            m_gameOffsets32[m_count++] = offset;
        }
    }
};

#endif
