#ifndef LICHESSOPENING_H
#define LICHESSOPENING_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QString>
#include <QMap>

#include "move.h"
#include "movedata.h"

class LichessOpening : public QObject
{
    Q_OBJECT

public:
    LichessOpening();
    ~LichessOpening();

    QByteArray queryPosition(const QString& fen);
    void setDb(const QString &db);
    void setVariant(const QString &v);
    void setIntervals(const QStringList &intervals);

private:
    QByteArray sync_request( QNetworkRequest& networkRequest );

private:
    QString m_requested;
    QByteArray reply;
    QString m_db;
    QString m_variant;
    QStringList m_intervals;
};

#endif // LICHESSOPENING_H
