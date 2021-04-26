#include "lichessopeningdatabase.h"
#include <QJsonDocument>

LichessOpeningDatabase::LichessOpeningDatabase()
{
}

bool LichessOpeningDatabase::open(const QString& filename, bool)
{
    m_filename = filename;
    QStringList internal = filename.toLower().split(" ");
    if (internal.count()>1)
    {
        internal.removeAt(0);
        m_client.setDb(internal.at(0));
        if (internal.count()>1)
        {
            internal.removeAt(0);
            m_client.setVariant(internal.at(0));
            internal.removeAt(0);
            m_client.setIntervals(internal);
        }
    }
    return true;
}

/** Get a map of MoveData from a given board position */
unsigned int LichessOpeningDatabase::getMoveMapForBoard(const BoardX &board, QMap<Move, MoveData>& moves)
{
    unsigned int total = 0;
    QString fen = board.toFen();
    QByteArray reply = m_client.queryPosition(fen);

    QJsonDocument doc = QJsonDocument::fromJson(reply);

    QJsonArray jMoves = doc.object().value("moves").toArray();

    if (jMoves.count())
    {
        for (QJsonArray::const_iterator it = jMoves.constBegin(); it != jMoves.constEnd(); ++it)
        {
            int a = (*it).toObject().value("averageRating").toInt();
            int w = (*it).toObject().value("white").toInt();
            int b = (*it).toObject().value("black").toInt();
            int d = (*it).toObject().value("draw").toInt();
            int n = w+d+b;
            total += n;
            QString u = (*it).toObject().value("uci").toString();
            Move m = board.parseMove(u.toLatin1());
            MoveData md;
            md.results.update(WhiteWin, w);
            md.results.update(Draw, d);
            md.results.update(BlackWin, b);
            md.rating.update(a, n);
            md.move = m;
            md.san = board.moveToSan(m);
            md.localsan = board.moveToSan(m, true);
            moves.insert(m, md);
        }
    }
    return total;

}
