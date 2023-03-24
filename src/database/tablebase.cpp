/***************************************************************************
 *   (C) 2007 Sean Estabrooks                                              *
 *   (C) 2008-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "networkhelper.h"
#include "settings.h"
#include "tablebase.h"
#include "version.h"

#include <QApplication>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QRegularExpression>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QJsonDocument>

using namespace chessx;

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

bool Tablebase::s_allowEngineOutput = true;

void Tablebase::setAllowEngineOutput(bool allow)
{
    s_allowEngineOutput = allow;
}

OnlineTablebase::OnlineTablebase()
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(httpDone(QNetworkReply*)));
}

OnlineTablebase::~OnlineTablebase()
{
}

void OnlineTablebase::abortLookup()
{
}

void OnlineTablebase::getBestMove(QString fen)
{
    abortLookup();
    if(m_fen == fen)
    {
        return;
    }
    m_fen = fen;
    QTimer::singleShot(100, this, SLOT(sendIt()));
}

void OnlineTablebase::sendIt()
{
    QUrl url;
    QString prep(m_fen.simplified());
    QString count(prep.left(prep.indexOf(" ")));
    int white = count.count(QRegularExpression("[A-Z]"));
    int black = count.count(QRegularExpression("[a-z]"));

    if(white + black > 7 || black < 1 || white < 1)
    {
        return;
    }

    if (AppSettings->getValue("/General/tablebaseSource").toInt() || ((white + black) > 6) || black > 4 || white > 4)
    {
        QString requested = QString("/standard?fen=%1").arg(m_fen);
        url = requested;
        url.setHost("tablebase.lichess.ovh");
    }
    else
    {
        QChar toMove = (prep[prep.indexOf(QString(" ")) + 1].toLower());
        QString requested = QString("/online/playshredder/fetch.php?action=egtb&hook=%1&fen=%2")
                            .arg(toMove).arg(m_fen);
        url = requested;
        url.setHost("www.shredderchess.com");
    }

    url.setScheme("http");
    m_requested = url.toString();

    QNetworkRequest request = NetworkHelper::Request(url);
    manager.get(request);
}

void OnlineTablebase::httpDone(QNetworkReply *reply)
{
    QUrl url = reply->request().url();

    if(!reply->error())
    {
        if(url.toString() == m_requested)
        {
            m_requested.clear();

            QString ret(reply->readAll());
            if (url.host().contains("lichess"))
            {
                if(ret.indexOf("invalid fen") >= 0)
                {
                    return;
                }

                QJsonDocument doc = QJsonDocument::fromJson(ret.toLatin1());
                if (!doc.isNull())
                {
                    QList<Move> bestMoves;

                    QJsonValue jdtm = doc.object().value("dtm");
                    QJsonValue jdtz = doc.object().value("dtz");
                    int dtm = jdtm.toInt();
                    int dtz = jdtz.toInt();
                    int bestScore = 0;
                    if (jdtm.isNull())
                    {
                        bestScore = jdtz.isNull() ? 0 : dtz;
                    }
                    else
                    {
                        bestScore = dtm;
                    }
                    int result = bestScore ? (bestScore > 0) ? 1:-1:0;

                    QJsonArray moves = doc.object().value("moves").toArray();
                    for (QJsonArray::const_iterator it = moves.constBegin(); it != moves.constEnd(); ++it)
                    {
                        QJsonValue xjdtm = (*it).toObject().value("dtm");
                        QJsonValue xjdtz = (*it).toObject().value("dtz");
                        int xdtm = xjdtm.toInt();
                        int xdtz = xjdtz.toInt();

                        int xbestScore = 0;
                        if (xjdtm.isNull())
                        {
                            xbestScore = xjdtz.isNull() ? 0 : xdtz;
                        }
                        else
                        {
                            xbestScore = xdtm;
                        }

                        int nxresult = xbestScore ? (xbestScore > 0) ? -1:1:0;
                        if (result != nxresult) break;

                        QJsonValue m = (*it).toObject().value("uci");
                        Move move = Move::fromUCI(m.toString().toLatin1());
                        bestMoves.append(move);
                    }
                    if (s_allowEngineOutput && (bestMoves.count() || bestScore))
                    {
                        if (dtm)
                        {
                            bestScore += result * 0x800;
                        }
                        emit bestMove(bestMoves, bestScore);
                    }
                }
            }
            else
            {
                if(ret.indexOf("Not found") >= 0)
                {
                    return;
                }

                qDebug() << ret << Qt::endl << Qt::flush;

                if(ret[5] == 'w')
                {
                    ret.remove(QRegularExpression("NEXTCOLOR.*\\n", QRegularExpression::DotMatchesEverythingOption));
                }
                else
                {
                    ret.remove(QRegularExpression(".*NEXTCOLOR\\n", QRegularExpression::DotMatchesEverythingOption));
                }

                qDebug() << ret << Qt::endl << Qt::flush;

                ret.remove(0, ret.indexOf("\n") + 1);
                ret.remove(":");
                ret.remove("Win in ");
                ret.replace(QRegularExpression("[-x]"), " ");
                ret.replace("Draw", "0");
                ret.replace("Lose in ", "-");

                QStringList moveList = ret.split('\n',Qt::SkipEmptyParts);
                if (moveList.size() >= 1)
                {
                    QList<Move> bestMoves;
                    bool first = true;
                    int bestScore = 0;
                    foreach(QString tbMove, moveList)
                    {
                        QStringList fld = tbMove.split(' ',Qt::SkipEmptyParts);
                        if(fld.size() < 3)
                        {
                            break;
                        }
                        Move move(Square(fld[0].toInt()), Square(fld[1].toInt()));
                        int score = fld[2].toInt();
                        if(fld.size() > 3)
                        {
                            switch(score)
                            {
                            case 8:
                                move.setPromoted(Queen);
                                break;
                            case 9:
                                move.setPromoted(Rook);
                                break;
                            case 10:
                                move.setPromoted(Bishop);
                                break;
                            case 11:
                                move.setPromoted(Knight);
                                break;
                            default:
                                return;
                            }
                            score = fld[3].toInt();
                        }
                        if (first) {
                            bestScore = score;
                            first = false;
                        }
                        if (score == bestScore)
                        {
                            bestMoves.append(move);
                        }
                        else
                        {
                            break;
                        }
                    }
                    if (s_allowEngineOutput && !first)
                    {
                        bestScore = bestScore*2; // Convert to half moves
                        bestScore += ((bestScore>0)?1:-1) * 0x800;
                        emit bestMove(bestMoves, bestScore);
                    }
                }
            }
        }
    }
    reply->deleteLater();
}

