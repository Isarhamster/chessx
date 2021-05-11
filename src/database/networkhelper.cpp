/****************************************************************************
*   Copyright (C) 2021 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include <QCoreApplication>
#include <QString>

#include "networkhelper.h"
#include "version.h"

QNetworkRequest NetworkHelper::Request(const QUrl& url)
{
    QNetworkRequest request(url);
    QByteArray userAgent = QString(QCoreApplication::applicationName() + "/" + STR_VERSION_NET).toLatin1();
    request.setRawHeader("User-Agent",userAgent);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    return request;
}
