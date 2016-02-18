/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ECOPOSITIONS_H
#define ECOPOSITIONS_H

#include <QMap>
#include <QString>
#include "board.h"

#define COMPILED_ECO_FILE_ID ((quint32)0xCD5CBD02U)

struct EcoPositions
{
public:
    static QMap<quint64, QString>* m_ecoPositions;
    static volatile bool m_ecoReady;

    /** Method that loads a file containing ECO classifications for use by the ecoClassify method. Returns true if successful */
    static bool loadEcoFile(const QString& ecoFile);
    static QString findEcoNameDetailed(QString eco);
    static QString findEcoName(QString eco);
    static void terminateEco();

    static bool isEcoPosition(const Board &b, QString &eco);
};

#endif // ECOPOSITIONS_H
