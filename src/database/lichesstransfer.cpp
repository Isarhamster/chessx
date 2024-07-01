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

#include "lichesstransfer.h"

QByteArray LichessTransfer::sync_request( QNetworkRequest& request )
{
    QNetworkAccessManager manager;
    QNetworkReply* reply;
    QEventLoop connection_loop;
    QObject::connect(&manager, SIGNAL( finished(QNetworkReply*) ), &connection_loop, SLOT( quit() ) );
    reply = manager.get( request );
    connection_loop.exec();
    reply->deleteLater();
    return reply->readAll();
}

QByteArray LichessTransfer::queryData(const QString& api_call)
{
    QUrl url;
    url = api_call;
    url.setHost("lichess.org");
    url.setScheme("https");

    QNetworkRequest request = NetworkHelper::Request(url);
    return sync_request( request );
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

QByteArray LichessTransfer::queryResults(enum LichessTournamentType t, const QString& tournamentId)
{
    QString stype = lichessTournamentString(t);
    return tournamentId.isEmpty() ? QByteArray() : queryData(QString("/api/%1/%2/results").arg(stype).arg(tournamentId));
}
