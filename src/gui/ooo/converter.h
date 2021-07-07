/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
*   Copyright (C) 2006 by Tobias Koenig                                     *
*   Copyright (C)      by pro                                               *
****************************************************************************/

#ifndef CONVERTER_H
#define	CONVERTER_H

#include "document.h"
#include "styleparser.h"
#include "styleinformation.h"
#include "converter.h"
#include "formatproperty.h"

#include <QTextCharFormat>

class QDomElement;
class QDomText;

namespace OOO {

    class Document;

    class Converter {
    public:
        Converter();
        virtual ~Converter();

        virtual QTextDocument *convert(const QString &fileName);

    private:
        void setError(QString msg, int vars = 0);

        bool convertBody(const QDomElement &element);
        bool convertText(const QDomElement &element);
        bool convertHeader(QTextCursor *cursor, const QDomElement &element);
        bool convertParagraph(QTextCursor *cursor, const QDomElement &element, const QTextBlockFormat &format = QTextBlockFormat(), bool merge = false);
        bool convertTextNode(QTextCursor *cursor, const QDomText &element, const QTextCharFormat &format);
        bool convertSpan(QTextCursor *cursor, const QDomElement &element, const QTextCharFormat &format);
        bool convertLink(QTextCursor *cursor, const QDomElement &element, const QTextCharFormat &format);
        bool convertList(QTextCursor *cursor, const QDomElement &element);
        bool convertTable(const QDomElement &element);
        bool convertFrame(const QDomElement &element);
        bool convertAnnotation(QTextCursor *cursor, const QDomElement &element);
       
        QTextDocument *m_TextDocument;
        QTextCursor *m_Cursor;

        StyleInformation *m_StyleInformation;
        bool firstTime;
    };
}






#endif	/* CONVERTER_H */

