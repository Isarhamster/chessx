#ifndef __TABLEBASE_H__
#define __TABLEBASE_H__

#include "common.h"
#include "move.h"

class QHttp;

/** @ingroup Database
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


/** @ingroup Database
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
	void httpDone(bool error);
private:
	QHttp* m_http;
};

#endif // __TABLEBASE_H__
