/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
*   Copyright (C) 2006 by Tobias Koenig                                     *
*   Copyright (C)      by pro                                               *
****************************************************************************/

#ifndef DOCUMENT_H
#define	DOCUMENT_H

#include <QString>
#include <QMap>
#include <QByteArray>

namespace OOO {

    class Document {
    public:
        Document(const QString &fileName);
        ~Document();

        bool open();

        QString lastErrorString() const;

        QByteArray content() const;
        QByteArray meta() const;
        QByteArray styles() const;
        QMap<QString, QByteArray> images() const;

    private:
        void setError(const QString&);

        QString mFileName;
        QByteArray mContent;
        QByteArray mMimetype;
        QByteArray mSetting;
        QByteArray mMeta;
        QByteArray mStyles;
        QMap<QString,QByteArray> mImages;
        //// Manifest *mManifest; mimetype
        QString mErrorString;
    };

}

#endif	/* DOCUMENT_H */

