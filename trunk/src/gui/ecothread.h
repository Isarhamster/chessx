#ifndef __ECOTHREAD_H__
#define __ECOTHREAD_H__

#include <QThread>
#include "game.h"

/** @ingroup GUI
A small little class to handle loading of the ECO file for the
mainwindow */

class EcoThread : public QThread
{
	Q_OBJECT

public:
    EcoThread() {};
    void run() {
        bool ok = true;
        ok |= Game::loadEcoFile(":chessx.eco");
        ok |= Board::loadEcoFile(":chessx.gtm");
        emit loaded(this, ok);
    };

signals:
    void loaded(QObject*, bool);
};

#endif // __ECOTHREAD_H__
