#include "networkhelper.h"
#include "version.h"

#include <QApplication>
#include <QByteArray>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QStringList>
#include <QUrlQuery>
#include <QtCore/qstringview.h>

#include "lichesstransfer.h"

QByteArray LichessTransfer::sync_request( QNetworkRequest& request, QByteArray queryData )
{
    QNetworkAccessManager manager;
    QNetworkReply* reply;
    QEventLoop connection_loop;
    QObject::connect(&manager, SIGNAL( finished(QNetworkReply*) ), &connection_loop, SLOT( quit() ) );
    reply = queryData.isEmpty() ? manager.get(request) : manager.post( request, queryData);
    connection_loop.exec();
    reply->deleteLater();
    return reply->readAll();
}

QByteArray LichessTransfer::queryData(const QString& api_call, QString token)
{
    QUrl url;
    url = api_call;
    url.setHost("lichess.org");
    url.setScheme("https");

    QNetworkRequest request = NetworkHelper::Request(url);
    if (!token.isEmpty())
    {
        QString temp = "Bearer " + token;
        request.setRawHeader("Authorization", temp.toLocal8Bit());
        request.setRawHeader("Version", "2.0");
    }
    return sync_request( request );
}

QByteArray LichessTransfer::postData(const QString& api_call, QUrlQuery queryData, QString token )
{
    QUrl url;
    url = api_call;
    url.setHost("lichess.org");
    url.setScheme("https");

    QNetworkRequest request = NetworkHelper::Request(url);
    if (!token.isEmpty())
    {
        QString temp = "Bearer " + token;
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setRawHeader("Authorization", temp.toLocal8Bit());
        request.setRawHeader("Version", "2.0");
    }
    return sync_request( request, queryData.toString(QUrl::FullyEncoded).toUtf8());
}

QString LichessTransfer::lichessTournamentString(enum LichessTournamentType t)
{
    QString stype;
    switch (t)
    {
    case LichessTournamentTypeArena: stype="tournament"; break;
    case LichessTournamentTypeSwiss: stype="swiss"; break;
    case LichessTournamentTypeBroadcast: stype="broadcast"; break;
    default: return QByteArray();
    }
    return stype;
}

QByteArray LichessTransfer::queryTournaments(enum LichessTournamentType t, QString teamId, int maxLoad)
{
    QString stype = lichessTournamentString(t);
    if (!teamId.isEmpty()) teamId.prepend("team/");
    QString nb;
    if (t==LichessTournamentTypeBroadcast) nb = "nb";
    else nb = "max";
    return queryData(QString("/api/%1%2?%3=%4").arg(teamId).arg(stype).arg(nb).arg(maxLoad));
}

QByteArray LichessTransfer::queryStudies(QString username, QString token)
{
    return queryData(QString("/api/study/by/%1").arg(username), token);
}

QByteArray LichessTransfer::writeGameToStudy(QString studyid, QString token, QString pgn, bool chess960)
{
    QUrlQuery queryData;
    queryData.addQueryItem("pgn", pgn);
    queryData.addQueryItem("variant", chess960 ? "chess960" : "standard");

    return postData(QString("/api/study/%1/import-pgn").arg(studyid), queryData, token);
}

QByteArray LichessTransfer::queryResults(enum LichessTournamentType t, const QString& tournamentId)
{
    QString stype = lichessTournamentString(t);
    return tournamentId.isEmpty() ? QByteArray() : queryData(QString("/api/%1/%2/results").arg(stype).arg(tournamentId));
}
