/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "polyglotwriter.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

PolyglotWriter::PolyglotWriter(QObject *parent) :
    QThread(parent),
    m_source(0)
{
}

PolyglotWriter::~PolyglotWriter()
{
    delete m_destination;
}

void PolyglotWriter::run()
{
    m_destination->book_make(*m_source, m_break);
    if (!m_break)
    {
        emit bookBuildFinished(m_out);
    }
    deleteLater();
}

// ---------------------------------------------------------
// Mainthread Interface
// ---------------------------------------------------------

void PolyglotWriter::writeBookForDatabase(Database *src, const QString &out, int maxPly, int minGame, bool uniform)
{
    m_break = false;
    m_out = out;
    m_source = src;
    m_destination = new PolyglotDatabase();
    if (m_destination->openForWriting(out, maxPly, minGame, uniform))
    {
        start();
    }
    else
    {
        emit bookBuildError(out);
        deleteLater();
    }
}

void PolyglotWriter::cancel()
{
    m_break = true;
}



