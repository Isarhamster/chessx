/***************************************************************************
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QFile>
#include <QUndoStack>

#include "arenabook.h"
#include "ctgdatabase.h"
#include "databaseinfo.h"
#include "ficsdatabase.h"
#include "filter.h"
#include "gamex.h"
#include "gameundocommand.h"
#include "memorydatabase.h"
#include "pgndatabase.h"
#include "polyglotdatabase.h"
#include "qt6compat.h"
#include "settings.h"
#include "tags.h"

#ifdef USE_SCID
#include "sciddatabase.h"
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

DatabaseInfo::DatabaseInfo(QUndoGroup* undoGroup, Database *db)
{
    m_database = db;
    m_filename = db->name();
    m_filter = new FilterX(m_database);
    m_bLoaded = true;
    m_utf8 = false;
    m_hadBOM = false;
    m_undoStack = new QUndoStack((QObject*)undoGroup);
    newGame();
    connect(m_undoStack, SIGNAL(cleanChanged(bool)), SLOT(dbCleanChanged(bool)));
    connect(&m_game, SIGNAL(signalGameModified(bool,GameX,QString)),SLOT(setGameModified(bool,GameX,QString)));
    connect(&m_game, SIGNAL(signalMoveChanged()), SIGNAL(signalMoveChanged()));
    connect(db, SIGNAL(dirtyChanged(bool)), SLOT(dbDirtyChanged(bool)));
}

DatabaseInfo::DatabaseInfo(QUndoGroup* undoGroup, const QString& fname): m_filter(nullptr), m_index(InvalidGameId)
{
    m_filename = fname;
    m_bLoaded = false;
    m_utf8 = false;
    m_hadBOM = false;
    m_undoStack = new QUndoStack((QObject*)undoGroup);
    connect(m_undoStack, SIGNAL(cleanChanged(bool)), SLOT(dbCleanChanged(bool)));
    connect(&m_game, SIGNAL(signalGameModified(bool,GameX,QString)),SLOT(setGameModified(bool,GameX,QString)));
    connect(&m_game, SIGNAL(signalMoveChanged()), SIGNAL(signalMoveChanged()));
    QFile file(fname);
    if (IsFicsDB())
    {
        m_database = new FicsDatabase;
    }
#ifdef USE_SCID
    else if (isScidDb())
    {
        m_database = new ScidDatabase();
    }
#endif
    else if (IsPolyglotBook())
    {
        m_database = new PolyglotDatabase;
    }
    else if (IsArenaBook())
    {
        m_database = new ArenaBook;
    }
    else if (IsChessbaseBook())
    {
        m_database = new CtgDatabase;
    }
    else if(file.size()/(1024 * 1024) < AppSettings->getValue("/General/EditLimit").toInt())
    {
        m_database = new MemoryDatabase;
    }
    else
    {
        m_database = new PgnDatabase;
        ((PgnDatabase*)m_database)->set64bit(file.size() > INT_MAX);
    }
    connect(m_database, SIGNAL(dirtyChanged(bool)), SLOT(dbDirtyChanged(bool)));
}

void DatabaseInfo::dbDirtyChanged(bool modified)
{
    emit dirtyChanged(modified);
}

void DatabaseInfo::doLoadFile(QString filename)
{
    if (m_filter) m_filter->cancel();
    if(!m_database->open(filename, m_utf8))
    {
        emit LoadFinished(this);
        return;
    }
    m_database->setHadBOM(m_hadBOM);
    m_database->parseFile();
    delete m_filter;
    m_filter = new FilterX(m_database);
    m_bLoaded = true;
    emit LoadFinished(this);
}

void DatabaseInfo::run()
{
    QString fname = resolvedPath(m_filename);
    if (fname.isEmpty()) fname = m_filename; // Support virtual databases
    if (!fname.isEmpty()) doLoadFile(fname);
}

bool DatabaseInfo::testBOM()
{   
    QString fname = resolvedPath(m_filename);
    QFile file(fname);
    if(file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray a = file.read(3);
            if ((a.length() == 3) &&
                ((unsigned char)a[0] == 0xEF) &&
                ((unsigned char)a[1] == 0xBB) &&
                ((unsigned char)a[2] == 0xBF))
            {
               return true;
            }
        }
    }
    return false;
}

bool DatabaseInfo::open(bool utf8)
{
    // Check for BOM and override utf8 in case of a UTF8-BOM is found
    // Note: Does not detect UNICODE and other nasty BOMs
    if (testBOM())
    {
        utf8 = true;
        m_hadBOM = true;
    }

    m_bLoaded = false;
    m_utf8 = utf8;
    start();
    return true;
}

void DatabaseInfo::close()
{
    if (m_filter)
    {
        m_filter->cancel();
    }
    m_bLoaded = false;
    m_database->m_break = true;
    if(isRunning())
    {
        bool bSuccess = wait(5000);
        if(!bSuccess)
        {
            terminate();
        }
    }

    delete m_database;
    delete m_filter;

    clearLastGames();
    m_database = nullptr;
    disconnect(m_undoStack, SIGNAL(cleanChanged(bool)), this, SLOT(dbCleanChanged(bool)));
    m_undoStack->clear();
}

DatabaseInfo::~DatabaseInfo()
{
}

FilterX *DatabaseInfo::filter()
{
    return m_filter;
}

bool DatabaseInfo::loadGame(GameId index)
{
    if(!m_bLoaded)
    {
        return false;
    }
    if(!isValid())
    {
        return false;
    }
    if(!m_database || index >= m_database->count())
    {
        return false;
    }
    if(!m_database->loadGame(index, m_game))
    {
        return false;
    }

    m_lastGames.push_back(index);
    m_index = index;
    int n = m_filter ? m_filter->gamePosition(index) - 1 : 0;
    if(n < 0)
    {
        n = 0;
    }
    m_game.moveToId(n);
    m_undoStack->clear();
    setGameModified(false, GameX(), "");

    return true;
}

void DatabaseInfo::updateMaterial()
{
    m_game.scoreMaterial(m_material);
    m_game.scoreEvaluations(m_evaluations);
    emit signalGameModified(!m_undoStack->isClean());
}

void DatabaseInfo::dbCleanChanged(bool bClean)
{
    emit signalGameModified(!bClean);
}

bool DatabaseInfo::modified() const
{
    return !m_undoStack->isClean();
}

bool DatabaseInfo::gameNeedsSaving() const
{
    return (isValid() && modified() && !m_database->isReadOnly() && !m_database->deleted(m_index));
}

void DatabaseInfo::setGameModified(bool modified, const GameX& g, QString action)
{
    if (modified)
    {
        Q_ASSERT(!action.isEmpty());
        m_undoStack->push(new GameUndoCommand(this, g, m_game, action));
    }
    else
    {
        m_undoStack->clear();
    }
    updateMaterial();
}

QUndoStack *DatabaseInfo::undoStack() const
{
    return m_undoStack;
}

const QList<double>& DatabaseInfo::material() const
{
    return m_material;
}

const QList<double>& DatabaseInfo::evaluations() const
{
    return m_evaluations;
}

void DatabaseInfo::restoreState(const GameX& game)
{
    emit signalRestoreState(game);
}

void DatabaseInfo::newGame()
{
    m_undoStack->clear();
    m_game.clearTags();
    m_game.clear();
    m_index = InvalidGameId;
    setGameModified(false, GameX(), "");
}

bool DatabaseInfo::saveGame()
{
    if(!m_bLoaded)
    {
        return false;
    }
    if(!isValid() || m_database->isReadOnly())
    {
        return false;
    }

    QString eco = m_game.tag(TagNameECO).left(3);
    if(eco == "?")
    {
        eco.clear();
    }

    if(AppSettings->getValue("/General/automaticECO").toBool())
    {
        if(eco.isEmpty() || !AppSettings->getValue("/General/preserveECO").toBool())
        {
            eco = m_game.ecoClassify().left(3);
            if(!eco.isEmpty())
            {
                m_game.setTag(TagNameECO, eco);
                if (VALID_INDEX(m_index))
                {
                    database()->index()->setTag(TagNameECO, eco, m_index);
                }
            }
        }
    }

    for(int i = 0; i < 7; ++i)
    {
        QString tag = StandardTags[i];
        if (!m_game.hasTag(tag))
        {
            m_game.setTag(tag,QString());
            if (VALID_INDEX(m_index))
            {
                database()->index()->setTag(tag, QString(), m_index);
            }
        }
    }

    if(m_index < m_database->count())
    {
        if(m_database->replace(m_index, m_game))
        {
            if (m_database->deleted(m_index))
            {
                // In case a deleted game is saved, assume that the use wants to keep content, so undelete it
                m_database->index()->setDeleted(m_index, false);
            }
            setGameModified(false, GameX(), "");
            m_undoStack->setClean();
            return true;
        }
    }
    else if(m_index == InvalidGameId && m_database->appendGame(m_game))
    {
        if (m_filter)
        {
            // TODO: Das Filtermodel muss vorher verstaendigt werden
            m_filter->cancel();
            m_filter->resize(m_database->count(), true);
        }
        m_index = m_database->count() - 1;
        m_lastGames.push_back(m_index);
        if(!eco.isEmpty())
        {
            database()->index()->setTag(TagNameECO, eco, m_index);
        }
        database()->index()->setTag(TagNameLength, QString::number((m_game.plyCount() + 1) / 2), m_index);

        setGameModified(false, GameX(), "");
        m_undoStack->setClean();
        return true;
    }
    return false;
}

void DatabaseInfo::replaceGame(const GameX &game)
{
    m_game = game;
    updateMaterial();
}

QString DatabaseInfo::dbPath() const
{
    return database()->filename();
}

QString DatabaseInfo::dbName() const
{
    return database() ? database()->name() : "";
}

void DatabaseInfo::clearLastGames()
{
    m_lastGames.clear();
}

bool DatabaseInfo::IsLoaded() const
{
    return m_bLoaded;
}

bool DatabaseInfo::IsUtf8() const
{
    return m_utf8;
}

QString DatabaseInfo::ficsPath()
{
    return AppSettings->commonDataFilePath("FICS.pgn");
}

bool DatabaseInfo::isNative() const
{
    return database() && (database()->inherits("PgnDatabase"));
}

bool DatabaseInfo::isClipboard() const
{
    return database() && database()->IsClipboard();
}

bool DatabaseInfo::isScidDb() const
{
    QFileInfo fi(m_filename);
    return (fi.suffix().toLower() == "si4");
}

bool DatabaseInfo::IsFicsDB() const
{
    QFileInfo fi(m_filename);
    QFileInfo fi2(ficsPath());
    return (fi.absoluteFilePath() == fi2.absoluteFilePath());
}

bool DatabaseInfo::IsPolyglotBook() const
{
    return IsPolyglotBook(m_filename);
}

bool DatabaseInfo::IsChessbaseBook() const
{
    return IsChessbaseBook(m_filename);
}

bool DatabaseInfo::IsArenaBook() const
{
    return IsArenaBook(m_filename);
}

bool DatabaseInfo::IsBook() const
{
    return IsBook(m_filename);
}

/* static */ bool DatabaseInfo::IsPolyglotBook(QString s)
{
    QFileInfo fi(s);
    return (fi.suffix().toLower() == "bin");
}

/* static */ bool DatabaseInfo::IsChessbaseBook(QString s)
{
    QFileInfo fi(s);
    return (fi.suffix().toLower() == "ctg");
}

/* static */ bool DatabaseInfo::IsArenaBook(QString s)
{
    QFileInfo fi(s);
    return (fi.suffix().toLower() == "abk");
}

/* static */ bool DatabaseInfo::IsOnlineBook(QString s)
{
    QFileInfo fi(s);
    return (s.contains("Lichess", Qt::CaseInsensitive) && fi.suffix().isEmpty());
}

/* static */ bool DatabaseInfo::IsBook(QString s)
{
    // Add here if more book formats come in
    return IsPolyglotBook(s) || IsChessbaseBook(s) || IsOnlineBook(s) ;
}

/* static */ QString DatabaseInfo::resolvedPath(const QString &fname) {
    QUrl url(fname);
    if (url.isValid() && !url.isRelative()) {
        if (url.scheme() == "file") {
            // Convert file:// URI to local file path
            return QFileInfo(url.toLocalFile()).canonicalFilePath();
        } else {
            // For non-file schemes, return empty
            if (QFileInfo::exists(fname)) return QFileInfo(fname).canonicalFilePath();
            qDebug() << "Rejected resolved path: " << fname << " : " << url;
            return "";
        }
    } else {
        // Treat as a plain local path
        return QFileInfo(fname).canonicalFilePath();
    }
}

/* static */ bool DatabaseInfo::IsLocalDatabase(QString s)
{
    QFileInfo fi = QFileInfo(s);
    QString suffix = fi.suffix().toLower();

    return ((suffix == "pgn") ||
            (suffix == "si4") ||
            (suffix == "bin") ||
            (suffix == "abk") ||
            (suffix == "ctg"));
}

/* static */ bool DatabaseInfo::IsLocalArchive(QString s)
{
    QFileInfo fi = QFileInfo(s);
    QString suffix = fi.suffix().toLower();

    return ((suffix == "zip") ||
            (suffix == "gz"));
}

qint64 DatabaseInfo::GetDatabaseSize(QString filename)
{
    QFileInfo fi(filename);
    QList<QFileInfo> files;
    files << fi;
    if (fi.suffix().toLower() == "si4")
    {
        // chop filename extension
        QString base(filename);
        base.chop(4);
        // account for name & game files
        files << QFileInfo(base + ".sn4")
              << QFileInfo(base + ".sg4");
    }
    qint64 size = 0;
    foreach(QFileInfo f, files)
    {
        size += f.size();
    }
    return size;
}
