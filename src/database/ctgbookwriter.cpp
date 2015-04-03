/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "ctgbookwriter.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

CtgBookWriter::CtgBookWriter(QObject *parent) :
    QThread(parent),
    m_source(0)
{
}

CtgBookWriter::~CtgBookWriter()
{
    delete m_destination;
}

void CtgBookWriter::run()
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

void CtgBookWriter::writeBookForDatabase(Database *src, const QString &out, int maxPly, int minGame, bool uniform)
{
    m_break = false;
    m_out = out;
    m_source = src;
    m_destination = new CtgDatabase();
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

void CtgBookWriter::cancel()
{
    m_break = true;
}



