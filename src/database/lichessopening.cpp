#include "lichessopening.h"
#include "settings.h"
#include "version.h"

#include <QApplication>
#include <QByteArray>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QRegExp>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QJsonDocument>

LichessOpening::LichessOpening()
{
}

LichessOpening::~LichessOpening()
{
}

QByteArray LichessOpening::sync_request( QNetworkRequest& request )
{
    QNetworkAccessManager manager;
    QNetworkReply* reply;
    QEventLoop connection_loop;
    connect(&manager, SIGNAL( finished( QNetworkReply* ) ), &connection_loop, SLOT( quit() ) );
    reply = manager.get( request );
    connection_loop.exec();
    reply->deleteLater();
    return reply->readAll();
}

QByteArray LichessOpening::queryPosition(const QString& fen)
{
    QUrl url;
    QString prep(fen.simplified());
    QString count(prep.left(prep.indexOf(" ")));

    if (AppSettings->getValue("/General/onlineTablebases").toBool())
    {
        QString requested = QString("/%1?fen=%2").arg(m_db).arg(fen);
        if (!m_variant.isEmpty()) requested += QString("&variant=%1").arg(m_variant);
        foreach(QString interval, m_intervals)
        {
           requested += QString("&speeds[]=%1").arg(interval);
        }
        if (m_intervals.count())
        {
            for (int i=1600;i<=2200;i+=200)
            {
                requested += QString("&ratings[]=%1").arg(i);
            }
        }

        if (m_requested == requested)
        {
            return reply;
        }

        url = requested;
        url.setHost("explorer.lichess.ovh");
        url.setScheme("http");

        QNetworkRequest request(url);
        QByteArray userAgent = QString(QCoreApplication::applicationName() + "/" + STR_VERSION_NET).toLatin1();
        request.setRawHeader("User-Agent",userAgent);
        request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
        reply = sync_request( request );
        m_requested = requested;
        return reply;
    }
    return QByteArray();
}

void LichessOpening::setDb(const QString &db)
{
    m_db = db;
}

void LichessOpening::setVariant(const QString &v)
{
    m_variant = v;
}

void LichessOpening::setIntervals(const QStringList &intervals)
{
    m_intervals = intervals;
}
