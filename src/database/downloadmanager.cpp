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

#include "databaseinfo.h"
#include "downloadmanager.h"
#include "networkhelper.h"
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
#include <QRegularExpression>
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

void DownloadManager::doDownloadToPath(QUrl url, QString filename)
{
    QString token = url.password();
    url = url.adjusted(QUrl::RemoveUserInfo);

    if(url.isEmpty() || !url.isValid())
    {
        qDebug() << "Error: Invalid/Empty Url";
        emit downloadError(url);
        return;
    }

    QNetworkRequest request = NetworkHelper::Request(url);

    if (!token.isEmpty())
    {
        QString temp = "Bearer " + token;
        request.setRawHeader("Authorization", temp.toLocal8Bit());
        request.setRawHeader("Version", "2.0");
    }

    request.setAttribute(QNetworkRequest::User, filename);

    QNetworkReply *reply = manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downloadFinished()));

    currentDownloads.append(reply);
}

QString DownloadManager::saveFileName(const QUrl &url)
{
    if (url.isLocalFile())
    {
        QString path = url.path();
        if (DatabaseInfo::IsLocalDatabase(path))
        {
            QFileInfo fi = QFileInfo(path);
            return AppSettings->commonDataFilePath(fi.fileName());
        }
        else if (DatabaseInfo::IsLocalArchive(path))
        {
            QFileInfo fi = QFileInfo(path);
            return AppSettings->commonDataFilePath(fi.baseName());
        }
    }
    return AppSettings->commonDataPath(); // Name will be determined after download
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

void DownloadManager::downloadFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if(reply)
    {
        QUrl url = reply->request().url();
        if(reply->error())
        {
            qDebug() << "Network Error " << reply->errorString();
            emit downloadError(url);
        }
        else
        {
            QVariant v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            if (v.isValid() && v.toInt() >= 300)
            {
                qDebug() << "HTTP Error " << v.toInt();
                emit downloadError(url);
            }
            else
            {
                QString filename = reply->request().attribute(QNetworkRequest::User).toString();
                if (filename.isEmpty() || QFileInfo(filename).isDir())
                {
                    // Create a temporary file
                    QString s = reply->rawHeader("Content-Disposition").constData();
                    QRegularExpression re("filename=(.*)");
                    QRegularExpressionMatch match;
                    if (s.contains(re, &match))
                    {
                        s = match.captured(1);
                        s.remove("'");
                        s.remove("\"");
                        s = s.trimmed();
                    }
                    if (s.isEmpty()) s = url.fileName();

                    if (filename.isEmpty())
                    {
                        filename = QDir::tempPath()+QDir::separator()+s;
                    }
                    else
                    {
                        filename = filename+QDir::separator()+s;
                    }
                    // Delete existing file // TODO but what if append is requested??
                    if (QFile::exists(filename) && QFileInfo(filename).isFile())
                    {
                        QFile::remove(filename);
                    }
                }

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
        }
        currentDownloads.removeAll(reply);
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
