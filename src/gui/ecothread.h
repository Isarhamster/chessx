/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ECOTHREAD_H_INCLUDED
#define ECOTHREAD_H_INCLUDED

#include <QThread>
#include "ecopositions.h"
#include "game.h"
#include "settings.h"

/** @ingroup GUI
A small little class to handle loading of the ECO file for the
mainwindow */

class EcoThread : public QThread
{
    Q_OBJECT

public:
    EcoThread() {}
    void run()
    {
        bool ok = true;
        QString eco = AppSettings->ecoPath();
        if (!EcoPositions::loadEcoFile(eco))
        {
            ok = false;
        }
        QString gtm = AppSettings->gtmPath();
        if (!Board::loadEcoFile(gtm))
        {
            ok = false;
        }
        EcoPositions::m_ecoReady = true;
        emit loaded(this, ok);
    }

signals:
    void loaded(QObject*, bool);
};

#endif // ECOTHREAD_H_INCLUDED
