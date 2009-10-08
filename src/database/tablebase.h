/***************************************************************************
 *   (C) 2007 Sean Estabrooks                                              *
 *   (C) 2008-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __TABLEBASE_H__
#define __TABLEBASE_H__

#include <QTimer>
#include "common.h"
#include "move.h"

class QHttp;

/** @ingroup Feature
 * Abstract base class for different types of tablebase access
 *
 * @todo
 * - Subclass to access lokasoft tablebases
 * - Method to return all tablebase info rather than just best move.
 * - Provide a configuration screen to enable and disable etc.
 * - Subclass to support local tablebases
 * - Add caching and/or prefetching of online queries to reduce lag
 */
class Tablebase : public QObject
{
	Q_OBJECT
signals:
	/** Emitted when best move has been gathered from tablebase */
	virtual void bestMove(Move move, int score) = 0;
public slots:
	/** Request best move from table base given FEN position */
	virtual void getBestMove(QString fen) = 0;
	/** Cancel the previous lookup */
	virtual void abortLookup() = 0;
};


/** @ingroup Feature
 * Implement Tablebase access to ShredderChess.com 6 piece tablebases.
 *
 */
class Shredder : public Tablebase
{
	Q_OBJECT
public:
	Shredder();
	~Shredder();
signals:
	virtual void bestMove(Move move, int score);
public slots:
	void getBestMove(QString fen);
	void abortLookup();
private slots:
	void httpDone(int id, bool error);
	void sendIt();
private:
	QHttp* m_http;
	QTimer* m_timer;
	QString m_fen;
	int m_id;
};

#endif // __TABLEBASE_H__
