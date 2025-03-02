#ifndef LICHESSTRANSFER_H
#define LICHESSTRANSFER_H

#include <QByteArray>
#include <QNetworkRequest>

class LichessTransfer
{
public:
    enum LichessTournamentType {
        LichessTournamentTypeArena,
        LichessTournamentTypeSwiss,
        LichessTournamentTypeBroadcast
    };

    static QString lichessTournamentString(enum LichessTournamentType t);
    static QByteArray sync_request( QNetworkRequest& request );
    static QByteArray queryData(const QString& api_call, QString token = "");
    static QByteArray queryTournaments(enum LichessTournamentType t, QString teamId="", int maxLoad=50);
    static QByteArray queryStudies(QString name, QString token);
    static QByteArray queryResults(enum LichessTournamentType t, const QString& tournamentId);
};

#endif // LICHESSTRANSFER_H
