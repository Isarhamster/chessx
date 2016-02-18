/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef __ECOTHREAD_H__
#define __ECOTHREAD_H__

#include <QThread>
#include "ecopositions.h"
#include "game.h"

/** @ingroup GUI
A small little class to handle loading of the ECO file for the
mainwindow */

class EcoThread : public QThread
{
    Q_OBJECT

public:
    EcoThread() {};
    void run()
    {
        bool ok = true;
        ok |= EcoPositions::loadEcoFile(":chessx.eco");
        ok |= Board::loadEcoFile(":chessx.gtm");
        EcoPositions::m_ecoReady = true;
        emit loaded(this, ok);
    };

signals:
    void loaded(QObject*, bool);
};

#endif // __ECOTHREAD_H__
