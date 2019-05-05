/***************************************************************************
 *   (C) 2007 Sean Estabrooks                                              *
 *   (C) 2008-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "settings.h"
#include "tablebase.h"
#include "version.h"

#include <QApplication>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QRegExp>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QJsonDocument>

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
    int white = count.count(QRegExp("[A-Z]"));
    int black = count.count(QRegExp("[a-z]"));

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

    QNetworkRequest request(url);
    QByteArray userAgent = QString(QCoreApplication::applicationName() + "/" + STR_VERSION_NET).toLatin1();
    request.setRawHeader("User-Agent",userAgent);
    manager.get(request);
}

void OnlineTablebase::httpDone(QNetworkReply *reply)
{
    QUrl url = reply->url();

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

                    QJsonValue wdl = doc.object().value("wdl");
                    if (wdl.isNull())
                    {
                        return;
                    }

                    int result = wdl.toInt();

                    QJsonValue jdtm = doc.object().value("dtm");
                    int dtm = jdtm.toInt();
                    QJsonValue jdtz = doc.object().value("dtz");
                    int bestScore;
                    if (result)
                    {
                        bestScore = jdtm.isNull() ? 0 : dtm - ((dtm>0) ? 1:(-1));
                    }
                    else
                    {
                        bestScore = jdtm.isNull() ? 0 : dtm;
                    }

                    QJsonArray moves = doc.object().value("moves").toArray();
                    for (QJsonArray::const_iterator it = moves.constBegin(); it != moves.constEnd(); ++it)
                    {
                        QJsonValue wdl = (*it).toObject().value("wdl");
                        if (-wdl.toInt()!=result) break; // We are done with all moves leading to the best result

                        QJsonValue xjdtm = (*it).toObject().value("dtm");
                        int xdtm = xjdtm.toInt();

                        if (bestScore)
                        {
                            if (xdtm != -bestScore) break;
                        }
                        QJsonValue m = (*it).toObject().value("uci");
                        Move move = Move().fromUCI(m.toString().toLatin1());
                        bestMoves.append(move);
                    }
                    if (s_allowEngineOutput && bestMoves.count())
                    {
                        if (jdtm.isNull())
                        {
                            dtm = 0;
                            if (result > 0) dtm = 0x800;
                            if (result < 0) dtm = -0x800;
                        }
                        emit bestMove(bestMoves, dtm);
                    }
                }
            }
            else
            {
                if(ret.indexOf("Not found") >= 0)
                {
                    return;
                }

                if(ret[5] == 'w')
                {
                    ret.remove(QRegExp("NEXTCOLOR.*\\n"));
                }
                else
                {
                    ret.remove(QRegExp(".*NEXTCOLOR\\n"));
                }
                ret.remove(0, ret.indexOf("\n") + 1);
                ret.remove(":");
                ret.remove("Win in ");
                ret.replace(QRegExp("[-x]"), " ");
                ret.replace("Draw", "0");
                ret.replace("Lose in ", "-");

                QStringList moveList = ret.split('\n',QString::SkipEmptyParts);
                if (moveList.size() >= 1)
                {
                    QList<Move> bestMoves;
                    bool first = true;
                    int bestScore;
                    foreach(QString tbMove, moveList)
                    {
                        QStringList fld = tbMove.split(' ',QString::SkipEmptyParts);
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
                        emit bestMove(bestMoves, bestScore);
                    }
                }
            }
        }
    }
    reply->deleteLater();
}

