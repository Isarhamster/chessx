/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "ecopositions.h"
#include <QDataStream>
#include <QFile>

QMap<quint64, QString>* EcoPositions::m_ecoPositions = 0;

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
  delete m_ecoPositions;
}

bool EcoPositions::isEcoPosition(const Board& b, QString& eco)
{
    quint64 key = b.getHashValue();
    if(m_ecoPositions->contains(key))
    {
        eco = m_ecoPositions->value(key);
        return true;
    }
    return false;
}
