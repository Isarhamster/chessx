/***************************************************************************
                          cxdsaxhandler.h  - handler used for parsing cxd files
                             -------------------
   begin                : 15 July 2007
   copyright            : (C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 3 as     *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License version 3 for more details.                 *
 *                                                                         *
 ***************************************************************************/

#ifndef __CXDSAXHANDLER_H__
#define __CXDSAXHANDLER_H__

#include <QFile>
#include <QByteArray>
#include <QXmlDefaultHandler>

#include "database.h"
#include "game.h"

/** @ingroup Database
   Class for parsing and writing xml files in cxd format. 

   @todo
   - perhaps change filenames to relative paths such that moving
     a database on disk is easy.
*/

class SaxHandler : public QXmlDefaultHandler
{
public:
  SaxHandler();

  bool startElement(const QString& namespaceURI,
		    const QString& localName,
		    const QString& qName,
		    const QXmlAttributes& attributes);
  bool endElement(const QString& namespaceURI,
		  const QString& localName,
		  const QString& qName);
  bool characters(const QString& str);
  bool fatalError(const QXmlParseException& exception);

  

  bool writeCxdFile(const QString& filename) const;


  QString m_version;
  QString m_gameFilename;
  QString m_gameAccessFilename;

  QString m_assignFilename;

  QString m_indexFilename;
  QString m_flagsFilename;

  QString m_indexEventFilename;
  QString m_indexSiteFilename;
  QString m_indexDateFilename;
  QString m_indexRoundFilename;
  QString m_indexPlayerNameFilename;
  QString m_indexResultFilename;
  QString m_indexPlyCountFilename;
  QString m_indexFENFilename;
  QString m_indexPlayerEloFilename;
  QString m_indexECOFilename;



  QString m_tagFilename;
private:
  QString m_currentText;

static void writeSimpleXmlTag(const QString& xmltagname,
                              const QString& xmltagvalue,
                              QIODevice& qiod);

};


#endif
