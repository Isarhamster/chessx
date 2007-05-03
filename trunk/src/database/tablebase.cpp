
#include <QObject>
#include <QUrl>
#include <QHttp>
#include "tablebase.h"

Shredder::Shredder()
	:m_http(new QHttp)
{
	QObject::connect(m_http, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
}

Shredder::~Shredder()
{
	m_http->abort();
	delete m_http;
}

void Shredder::getBestMove(QString fen)
{
	QString prep(fen.simplified());
	QString count(prep.left(prep.indexOf(" ")));
	int white = count.count(QRegExp("[A-Z]"));
	int black = count.count(QRegExp("[a-z]"));
	if (white + black > 6 || black > 4 || white > 4 || black < 1 || white < 1)
		return;
	QChar toMove=(prep[prep.indexOf(QString(" "))+1].toLower());
	QUrl url(QString("/online/playshredder/fetch.php?action=egtb&hook=%1&fen=%2")
		.arg(toMove).arg(fen));
	m_http->abort();
	m_http->setHost("www.shredderchess.com");
	m_http->get(url.toEncoded());
}

void Shredder::httpDone(bool error) {

	if (error)
		return;

	QString ret(m_http->readAll());
	if (ret.indexOf("Not found") >= 0)
		return;

	if (ret[5] == 'w')
		ret.remove(QRegExp("NEXTCOLOR.*\\n"));
	else	ret.remove(QRegExp(".*NEXTCOLOR\\n"));
	ret.remove(0,ret.indexOf("\n")+1);
	ret.remove(":");
	ret.remove("Win in ");
	ret.replace(QRegExp("[-x]"), " ");
	ret.replace("Draw", "0");
	ret.replace("Lose in ", "-");

	QStringList fld(ret.left(ret.indexOf('\n')).split(' '));
	if (fld.size() < 3)
		return;

	Move move(fld[0].toInt(), fld[1].toInt());
	int score = fld[2].toInt();
	if (fld.size() > 3) {
		switch(score) {
		case 8: move.setPromotionPiece(Queen); break;
		case 9: move.setPromotionPiece(Rook); break;
		case 10: move.setPromotionPiece(Bishop); break;
		case 11: move.setPromotionPiece(Knight); break;
		default: return;
		}
		score = fld[3].toInt();
	}
	emit bestMove(move, score);
}

