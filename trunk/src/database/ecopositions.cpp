/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "ecopositions.h"
#include <QDataStream>
#include <QFile>
#include <QThread>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

QMap<quint64, QString>* EcoPositions::m_ecoPositions = 0;
volatile bool EcoPositions::m_ecoReady = false;

bool EcoPositions::loadEcoFile(const QString& ecoFile)
{
    m_ecoPositions = new QMap<quint64, QString>();

    QFile file(ecoFile);
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream sin(&file);
        quint32 id;
        sin >> id;
        if(id == COMPILED_ECO_FILE_ID)
        {
            sin >> *m_ecoPositions;
            return true;
        }
        return false;
    }
    return false;
}

QString EcoPositions::findEcoNameDetailed(QString eco)
{
    foreach(QString actualEco, m_ecoPositions->values())
    {
        if (actualEco.startsWith(eco))
        {
            QString opName = actualEco.section(" ",1);
            return opName;
        }
    }
    return QString();
}

QString EcoPositions::findEcoName(QString eco)
{
    foreach(QString actualEco, m_ecoPositions->values())
    {
        if (actualEco.startsWith(eco))
        {
            QString opName = actualEco.section(" ",1);
            if (opName.contains(':'))
            {
                opName = opName.section(":",0,0);
            }
            return opName;
        }
    }
    return QString();
}

void EcoPositions::terminateEco()
{
    QMap<quint64, QString>* p = m_ecoPositions;
    m_ecoPositions = 0;
    delete p;
}

bool EcoPositions::isEcoPosition(const Board& b, QString& eco)
{
    while (!m_ecoReady) QThread::sleep(1);
    if (!m_ecoPositions) return false;

    quint64 key = b.getHashValue();
    if(m_ecoPositions->contains(key))
    {
        eco = m_ecoPositions->value(key);
        return true;
    }
    return false;
}
