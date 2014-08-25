#include "ctgdatabase.h"

// ---------------------------------------------------------
// construction
// ---------------------------------------------------------

CtgDatabase::CtgDatabase() :
    Database(),
    m_file(0),
    m_count(0)
{
}

CtgDatabase::~CtgDatabase()
{
    close();
}

// ---------------------------------------------------------
// Database Interface implementation
// ---------------------------------------------------------

bool CtgDatabase::open(const QString &filename, bool)
{
    if(m_file)
    {
        return false;
    }
    m_break = false;
    m_filename = filename;
    if(openFile(filename, true))
    {
        m_utf8 = false;
        QFileInfo fi(m_filename);
        m_count = fi.size() / 16; // Polyglot entry size is 16
        return true;
    }
    return false;
}

bool CtgDatabase::parseFile()
{
    return false;
}

QString CtgDatabase::filename() const
{
    return m_filename;
}

quint64 CtgDatabase::count() const
{
    return 0;
}

quint64 CtgDatabase::positionCount() const
{
    return m_count;
}

void CtgDatabase::loadGameMoves(GameId, Game &)
{

}

bool CtgDatabase::openFile(const QString &filename, bool readOnly)
{
    //open file
    QFile* file = new QFile(filename);
    if(readOnly && !file->exists())
    {
        delete file;
        return false;
    }
    if (file->open(readOnly ? QIODevice::ReadOnly : QIODevice::WriteOnly))
    {
        m_file = file;
        return true;
    }

    delete file;
    return false;
}

void CtgDatabase::close()
{
    //close the file, and delete objects
    if(m_file)
    {
        m_file->close();
    }
    delete m_file;
    m_file = 0;
}

// ---------------------------------------------------------
// CTG implementation
// ---------------------------------------------------------

// ---------------------------------------------------------
// Book parser - public interface
// ---------------------------------------------------------

bool CtgDatabase::findMove(quint64 key, MoveData& m)
{
    // todo
    return false;
}

// ---------------------------------------------------------
// Book building - public interface
// ---------------------------------------------------------

bool CtgDatabase::openForWriting(const QString &filename, int maxPly, int minGame, bool uniform)
{
    if(m_file)
    {
        return false;
    }

    // todo

    return openFile(filename, false);
}

void CtgDatabase::book_make(Database &db, volatile bool& breakFlag)
{
    // todo
}
