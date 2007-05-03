#ifndef __TABLEBASE_H__
#define __TABLEBASE_H__

#include "common.h"
#include "move.h"

class QHttp;

class Tablebase : public QObject
{
	Q_OBJECT
signals:
	virtual void bestMove(Move move, int score) = 0;
public slots:
	virtual void getBestMove(QString fen) = 0;
};


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
private slots:
	void httpDone(bool error);
private:
	QHttp* m_http;
};

#endif // __TABLEBASE_H__
