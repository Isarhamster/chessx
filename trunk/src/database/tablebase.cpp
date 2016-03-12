/***************************************************************************
 *   (C) 2007 Sean Estabrooks                                              *
 *   (C) 2008-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

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
    QString prep(m_fen.simplified());
    QString count(prep.left(prep.indexOf(" ")));
    int white = count.count(QRegExp("[A-Z]"));
    int black = count.count(QRegExp("[a-z]"));
    if(white + black > 6 || black > 4 || white > 4 || black < 1 || white < 1)
    {
        return;
    }
    QChar toMove = (prep[prep.indexOf(QString(" ")) + 1].toLower());
    QString requested = QString("/online/playshredder/fetch.php?action=egtb&hook=%1&fen=%2")
                        .arg(toMove).arg(m_fen);

    QUrl url(requested);
    url.setScheme("http");
    url.setHost("www.shredderchess.com");

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
                if (s_allowEngineOutput)
                {
                    emit bestMove(bestMoves, bestScore);
                }
            }
        }
    }
    reply->deleteLater();
}

