/***************************************************************************
                          cxdsaxhandler.cpp  - handler used for parsing cxd files
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

#include <QtDebug>
#include <QMessageBox>

#include "cxdsaxhandler.h"
#include "chessxdatabase.h"

// ------------------------------------------------
// ------------------------------------------------
// |  Parsing xml database file                   |
// ------------------------------------------------
// ------------------------------------------------



SaxHandler::SaxHandler()
{}

bool SaxHandler::startElement(const QString& ,
		              const QString& ,
			      const QString& ,
			      const QXmlAttributes& )
{
  m_currentText.clear();
  return 1;
}

bool SaxHandler::endElement(const QString& ,
		            const QString& ,
			    const QString& qName)
{
  if(qName == "version") m_version=m_currentText;
  else if(qName == "gameFilename") m_gameFilename=m_currentText;
  else if(qName == "gameAccessFilename") m_gameAccessFilename=m_currentText;
  else if(qName == "indexFilename") m_indexFilename=m_currentText;
  else if(qName == "indexEventFilename") m_indexEventFilename=m_currentText;
  else if(qName == "indexSiteFilename") m_indexSiteFilename=m_currentText;
  else if(qName == "indexDateFilename") m_indexDateFilename=m_currentText;
  else if(qName == "indexRoundFilename") m_indexRoundFilename=m_currentText;
  else if(qName == "indexPlayerNameFilename") m_indexPlayerNameFilename=m_currentText;
  else if(qName == "indexResultFilename") m_indexResultFilename=m_currentText;
  else if(qName == "indexPlyCountFilename") m_indexPlyCountFilename=m_currentText;
  else if(qName == "indexFENFilename") m_indexFENFilename=m_currentText;
  else if(qName == "indexPlayerEloFilename") m_indexPlayerEloFilename=m_currentText;
  else if(qName == "indexECOFilename") m_indexECOFilename=m_currentText; 
  return 1;
}

bool SaxHandler::characters(const QString& str)
{
  m_currentText += str;
  return 1;
}

bool SaxHandler::fatalError(const QXmlParseException& exception)
{
  QMessageBox::warning(0, QObject::tr("SAX Handler"),
		       QObject::tr("Parse error at line %1, column "
			           "%2:\n%3.")
		       .arg(exception.lineNumber())
		       .arg(exception.columnNumber())
		       .arg(exception.message()));
  return false;
}

bool SaxHandler::writeCxdFile(const QString& filename) const
{
 QFile file(filename);
 file.resize(0);
 file.open(QIODevice::WriteOnly);

 file.write("<?xml version=\"1.0\"?>\n\n");
 file.write("<ChessXDatabase>\n");
 writeSimpleXmlTag("version",ChessXDatabase::m_currentVersion,file);
 writeSimpleXmlTag("gameFilename",m_gameFilename,file);
 writeSimpleXmlTag("gameAccessFilename",m_gameAccessFilename,file);

 writeSimpleXmlTag("indexFilename",m_indexFilename,file);
 writeSimpleXmlTag("indexEventFilename",m_indexEventFilename,file);
 writeSimpleXmlTag("indexSiteFilename",m_indexSiteFilename,file);
 writeSimpleXmlTag("indexDateFilename",m_indexDateFilename,file);
 writeSimpleXmlTag("indexRoundFilename",m_indexRoundFilename,file);
 writeSimpleXmlTag("indexPlayerNameFilename",m_indexPlayerNameFilename,file);
 writeSimpleXmlTag("indexResultFilename",m_indexResultFilename,file);
 writeSimpleXmlTag("indexPlyCountFilename",m_indexPlyCountFilename,file);
 writeSimpleXmlTag("indexFENFilename",m_indexFENFilename,file);
 writeSimpleXmlTag("indexPlayerEloFilename",m_indexPlayerEloFilename,file);
 writeSimpleXmlTag("indexECOFilename",m_indexECOFilename,file);

 file.write("</ChessXDatabase>");
 file.close();
 return 1;
}

void SaxHandler::writeSimpleXmlTag(const QString& xmltagname, const QString& xmltagvalue,
                       QIODevice& qiod)
{
  qiod.write((QString("<")+xmltagname+QString(">")+xmltagvalue+QString("</")
    +xmltagname+QString(">\n")).toStdString().c_str());
}
