/***************************************************************************
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef DATABASEINFO_H__
#define DATABASEINFO_H__

#include "circularbuffer.h"
#include "gamex.h"
#include "gameid.h"

#include <QString>
#include <QObject>
#include <QUndoCommand>
#include <QPointer>
#include <QMetaType>

class Database;
class FilterX;
class QUndoStack;
class QUndoGroup;

/** @ingroup Database
The DatabaseInfo class is a simple class to keep various database-related
information together. */


class DatabaseInfo: public QThread
{
    Q_OBJECT
public:
    void run();
    /** Create information for clipboard database */
    DatabaseInfo(QUndoGroup *undoGroup, Database* db);
    /** Create information for file database */
    DatabaseInfo(QUndoGroup *undoGroup, const QString& filename);
    /** Close database and free memory */
    ~DatabaseInfo();
    /** Open database. */
    bool open(bool utf8);
    /** Close database. */
    void close();
    /** @return @p true if database is valid */
    bool isValid() const
    {
        return m_bLoaded ? m_database != nullptr : false;
    }
    /** @return Database object */
    Database* database()
    {
        return m_database;
    }
    const Database* database() const
    {
        return m_database;
    }
    /** @return current filter */
    FilterX* filter();
    /** @return current game  */
    GameX& currentGame()
    {
        return m_game;
    }
    const GameX& currentGame() const
    {
        return m_game;
    }
    /** @return index of current game  */
    GameId currentIndex() const
    {
        return m_index;
    }
    /** Load game @p index */
    bool loadGame(GameId index);
    /** Starts new game, giving it @p index equal to the number of games in the database */
    void newGame();
    /** Saves game, replacing current one or adding new. If @ref currentIndex() is
    in the database, game will be replaced, else if will be appended at the end. */
    bool saveGame();

    void replaceGame(const GameX& game);

    void updateMaterial();

    QString displayName() const
    {
        return m_filename;
    }
    QString dbPath() const;
    QString dbName() const;

    bool IsLoaded() const;
    bool IsUtf8() const;

    bool isScidDb() const;
    bool IsFicsDB() const;
    bool IsChessbaseBook() const;
    bool IsPolyglotBook() const;
    bool IsArenaBook() const;
    bool IsBook() const;

    static bool IsPolyglotBook(QString name);
    static bool IsChessbaseBook(QString s);
    static bool IsArenaBook(QString name);
    static bool IsBook(QString name);

    bool modified() const;
    bool gameNeedsSaving() const;

    void restoreState(const GameX& game);

    QUndoStack *undoStack() const;

    QList<double> m_material;

    const QList<double> &material() const;

    static QString ficsPath();

    bool isClipboard() const;
    bool isNative() const;

    const CircularBuffer<GameId>& lastGames() const { return m_lastGames; }
    void clearLastGames();

    static qint64 GetDatabaseSize(QString filename);

protected:
    void doLoadFile(QString filename);

signals:
    void LoadFinished(DatabaseInfo*);
    void signalRestoreState(const GameX &game);
    void signalGameModified(bool gameNeedsSaving);
    void signalMoveChanged();


public slots:
    void dbCleanChanged(bool);
    void setModified(bool modified, const GameX &g, QString action);

private:
    QUndoStack* m_undoStack;
    Database* m_database;
    QPointer<FilterX> m_filter;
    GameX m_game;
    QString m_filename;
    GameId m_index;
    CircularBuffer<GameId> m_lastGames;
    bool m_bLoaded;
    bool m_utf8;
};

#endif


