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
	EcoThread(QString path) : m_path(path) {};
	void run() { Game::loadEcoFile(m_path); emit loaded(); };
signals:
	void loaded();
private:
	QString m_path;
};

#endif // __ECOTHREAD_H__
