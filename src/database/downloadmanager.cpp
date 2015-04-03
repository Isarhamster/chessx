/****************************************************************************
 **
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include "downloadmanager.h"
#include "settings.h"
#include "version.h"

#include <QApplication>
#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QUrl>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

DownloadManager::DownloadManager(QObject *parent) :
    QObject(parent)
{
}

void DownloadManager::doDownload(const QUrl &url)
{
    QString filename = saveFileName(url);
    if(QFile::exists(filename))
    {
        emit onDownloadFinished(url, filename);
        return;
    }

    doDownloadToPath(url, filename);
}

void DownloadManager::doDownloadToPath(const QUrl &url, const QString& filename)
{
    if(url.isEmpty() || !url.isValid())
    {
        qDebug() << "Error: Invalid/Empty Url";
        emit downloadError(url);
        return;
    }

    QNetworkRequest request(url);
    QByteArray userAgent = QString(QCoreApplication::applicationName() + "/" + STR_VERSION_NET).toLatin1();
    request.setRawHeader("User-Agent",userAgent);
    QNetworkReply *reply = manager.get(request);

    connect(reply, SIGNAL(finished()),
            SLOT(downloadFinished()));

    destinationPaths.insert(url, filename);
    currentDownloads.append(reply);
}

QString DownloadManager::saveFileName(const QUrl &url)
{
    QString dir = AppSettings->commonDataPath();
    QDir().mkpath(dir);

    QString path = url.path();

    QString basename = QFileInfo(path).fileName();

    if(basename.isEmpty())
    {
        basename = "download.pgn";
    }

    return dir + QDir::separator() + basename;
}

bool DownloadManager::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

void DownloadManager::execute(QStringList args)
{
    if(args.isEmpty())
    {
        return;
    }

    foreach(QString arg, args)
    {
        QUrl url = QUrl::fromEncoded(arg.toLocal8Bit());
        doDownload(url);
    }
}

void DownloadManager::downloadFinished()
{
    QNetworkReply* reply = (QNetworkReply*) sender();

    if(reply)
    {
        QUrl url = reply->url();
        if(reply->error())
        {
            qDebug() << "Network Error " << reply->errorString();
            emit downloadError(url);
        }
        else
        {
            QString filename = destinationPaths.value(url);
            if(saveToDisk(filename, reply))
            {
                emit onDownloadFinished(url, filename);
            }
            else
            {
                qDebug() << "Could not save file " << url.toString() << " to " << filename;
                emit downloadError(url);
            }
        }
        currentDownloads.removeAll(reply);
        destinationPaths.remove(url);
        reply->deleteLater();
    }
    else
    {
        qDebug() << "Sender is not a QNetworkReply derived implementation";
    }

    if(currentDownloads.isEmpty())
    {
        emit downloadManagerIdle();
    }
}
