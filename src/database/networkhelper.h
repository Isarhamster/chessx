/****************************************************************************
*   Copyright (C) 2021 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include <QNetworkRequest>
#include <QUrl>

class NetworkHelper
{
public:
    static QNetworkRequest Request(const QUrl& url);
};

#endif // NETWORKHELPER_H
