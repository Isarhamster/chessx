/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
*   Copyright (C) 2006 by Tobias Koenig                                     *
*   Copyright (C)      by pro                                               *
****************************************************************************/

#include "document.h"
#include "kzip.h"

#include <QByteArray>
#include <QMap>
#include <QString>
#include <QImage>
#include <QDebug>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

using namespace OOO;

Document::Document(const QString &fileName)
: mFileName(fileName) {
    /// password protected file search ManifestEntry::ManifestEntry( const QString &fileName ) : on okular
}

bool Document::open() {
    mContent.clear();
    mStyles.clear();
    KZipStream *Kzip = new KZipStream(mFileName);
    if (Kzip->canread()) {
        const QStringList entries = Kzip->filelist();

        if (!entries.contains("META-INF")) {
            setError("Invalid document structure (META-INF directory is missing)");
            return false;
        }

        if (!entries.contains("content.xml")) {
            setError("Invalid document structure (content.xml is missing)");
            return false;
        }

        if (!entries.contains("styles.xml")) {
            /// nokia qt odt export dont write
            setError("Invalid document structure (styles.xml is missing)");
            ////  return false;
        }

        if (!entries.contains("meta.xml")) {
            /// nokia qt odt export dont write
            setError("Invalid document structure (meta.xml is missing)");
            //// return false;
        }

        /// fill files
        //// QMap<QString,QByteArray> listData()
        QMap<QString, QByteArray> allfiles = Kzip->listData();
        QMapIterator<QString, QByteArray> i(allfiles);
        while (i.hasNext()) {
            i.next();
            QByteArray xdata(i.value());
            QImage pic; /// i.value() /// i.key(); 
            const QString namex = QString(i.key());
            pic.loadFromData(xdata);
            if (!pic.isNull())
            {
                mImages.insert(namex, xdata);
            }
            else
            {
                if (namex == "styles.xml") {
                    mStyles = xdata;
                }
                if (namex == "content.xml") {
                    mContent = xdata;
                }
                if (namex == "META-INF/manifest.xml") {
                    mMeta = xdata;
                }
                if (namex == "mimetype") {
                    mMimetype = xdata;
                }
                if (namex == "settings.xml") {
                    mSetting = xdata;
                }
            }

        }
        if (QByteArray("application/vnd.oasis.opendocument.text") != mMimetype)
        {
            setError(QString("Invalid MimeType found: %1!").arg(QString::fromUtf8(mMimetype)));
            return false;
        }
        
        return true;
    }
    return false;
}

Document::~Document() {
    /// delete mManifest;
}

QString Document::lastErrorString() const {
    return mErrorString;
}

QByteArray Document::content() const {
    return mContent;
}

QByteArray Document::meta() const {
    return mMeta;
}

QByteArray Document::styles() const {
    return mStyles;
}

QMap<QString, QByteArray> Document::images() const {
    return mImages;
}

void Document::setError(const QString &error)
{
    qDebug() << error;
    mErrorString = error;
}

