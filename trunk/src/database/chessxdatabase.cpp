/***************************************************************************
                          chessxdatabase.cpp  - chessx native database class 
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

#include <QDir>
#include <QStringList>
#include <QtDebug>
#include <QMessageBox>
#include <QIODevice>
#include <QDataStream>

#include "board.h"
#include "nag.h"

#include "chessxdatabase.h"

// temporal includes for testing and debugging
#include "pgndatabase.h"
#include "memorydatabase.h"

const QString ChessXDatabase::m_currentVersion="0.1";
const QString ChessXDatabase::m_xmlFilenameExt="cxd";
const QString ChessXDatabase::m_gameFilenameExt="cxg";
const QString ChessXDatabase::m_gameAFilenameExt="cxa";
const QString ChessXDatabase::m_indexFilenameExt="cxi";
const QString ChessXDatabase::m_tagValueFilenameExt="cxv";
const QString ChessXDatabase::m_otherTagsFilenameExt="cxt";
const QString ChessXDatabase::m_otherTagsAFilenameExt="cxu";


// ------------------------------------------------
// ------------------------------------------------
// |  Helpers for writing xml database file       |
// ------------------------------------------------
// ------------------------------------------------


// --------------------------------------------------
// --------------------------------------------------
// |  Begin of definitions for class ChessXDatabase |
// --------------------------------------------------
// --------------------------------------------------


ChessXDatabase::ChessXDatabase()
:m_cxdIndex(m_index)
{initialise();}
	
ChessXDatabase::~ChessXDatabase()
{if(m_isOpen) close();}

bool ChessXDatabase::create(const QString& filename)
{
  m_xmlFilename=filename;

  QString basef(m_xmlFilename); 
  if(m_xmlFilename.endsWith(".cxd")) basef.chop(4);
  
  m_saxhandler.m_version=m_currentVersion;
  m_saxhandler.m_gameFilename=basef+"."+m_gameFilenameExt;
  m_saxhandler.m_gameAccessFilename=basef+"."+m_gameAFilenameExt;
  m_saxhandler.m_indexFilename=basef+"."+m_indexFilenameExt; 

  m_saxhandler.m_indexEventFilename=basef+"_event."+m_tagValueFilenameExt;
  m_saxhandler.m_indexSiteFilename=basef+"_site."+m_tagValueFilenameExt;
  m_saxhandler.m_indexDateFilename=basef+"_date."+m_tagValueFilenameExt;
  m_saxhandler.m_indexRoundFilename=basef+"_round."+m_tagValueFilenameExt;
  m_saxhandler.m_indexPlayerNameFilename=basef+"_playername."+m_tagValueFilenameExt;
  m_saxhandler.m_indexResultFilename=basef+"_result."+m_tagValueFilenameExt;
  m_saxhandler.m_indexPlyCountFilename=basef+"_plycount."+m_tagValueFilenameExt;
  m_saxhandler.m_indexFENFilename=basef+"_fen."+m_tagValueFilenameExt;
  m_saxhandler.m_indexPlayerEloFilename=basef+"_playerelo."+m_tagValueFilenameExt;
  m_saxhandler.m_indexECOFilename=basef+"_eco."+m_tagValueFilenameExt;

  m_cxdMoves.create(m_saxhandler);
  m_cxdIndex.create(m_saxhandler);

  writeCxdFile(m_xmlFilename);

  m_isOpen=true;
  m_count=0;
  return 1;
}

bool ChessXDatabase::open(const QString& filename)
{
  if(m_isOpen) return 0;
  m_xmlFilename=filename;
  if(!parseCxdFile(m_xmlFilename))
  {
    initialise();
    return 0;
  }

  if(!m_cxdMoves.open(m_saxhandler) ||
     !m_cxdIndex.open(m_saxhandler))
  {
    initialise();
    return 0;
  }

  m_count=m_cxdMoves.nb_games();

  if(!readIndexData())
  {
    initialise();
    return 0;
  }

  m_isOpen=1;
  return 1;
}

QString ChessXDatabase::filename() const
{return m_xmlFilename;}

void ChessXDatabase::close()
{
  if(!m_isOpen) return;
  m_cxdMoves.close();
  m_cxdIndex.close();
  initialise();
}

bool ChessXDatabase::isReadOnly()
{return 0;}

bool ChessXDatabase::clear()
{
  Q_ASSERT_X(1,"clear","NOT IMPLEMENTED");
  return 0;
}

bool ChessXDatabase::loadGame(int index, Game& game)
{
  if(!m_isOpen || index >= m_count) return false;
  game.clear();
  loadGameHeaders(index,game);

  if(!m_cxdMoves.loadMoves(index,game)) return false;
  return true;
}

void ChessXDatabase::loadGameMoves(int index, Game& game)
{
  m_cxdMoves.loadMoves(index,game);
}

bool ChessXDatabase::replace(int index, Game& game)
{return false;}

bool ChessXDatabase::appendGame(Game& game)
{
  if(!m_cxdMoves.appendGame(game)) 
  {
    // todo: depending on what went wrong, some cleanup operations
    // have probably to be implemented.
    return false;
  }
  // adjust index:
  // at the moment this implementation only handles index-tags
  m_cxdIndex.appendGame(game);

  ++m_count;
  return 1;
}

bool ChessXDatabase::remove(int index)
{
  Q_ASSERT_X(1,"remove","NOT IMPLEMENTED");
  return 0;
}

void ChessXDatabase::compact()
{
  Q_ASSERT_X(1,"compact","NOT IMPLEMENTED");
}

bool ChessXDatabase::isModified()
{return m_isModified;}


void ChessXDatabase::initialise()
{
  m_isOpen=false;
  m_count=0;
  m_isModified=0;

  m_xmlFilename="";
}

bool ChessXDatabase::parseCxdFile(const QString& filename)
{
 QFile file(filename);
 if(!file.exists()) return 0;
 QXmlInputSource inputSource(&file);
 QXmlSimpleReader reader;
 reader.setContentHandler(&m_saxhandler);
 reader.setErrorHandler(&m_saxhandler);
 
 if(!reader.parse(inputSource)) return 0; 

// additional tests could be added here for example to see if the files
// indicated in the xml file exist.
 
 m_xmlFilename=filename;
 return 1;
}

bool ChessXDatabase::writeCxdFile(const QString& filename) const
{
  return m_saxhandler.writeCxdFile(filename);
}

bool ChessXDatabase::readIndexData()
{
//  for(int i=0; i<m_count; ++i)
//  {m_index.setTag(TagWhite, QString("Game ")+QString().setNum(i+1),i);}
  for(int i=0; i<m_count; ++i)
  {m_index.add();}
  return 1;
}

// ------------------------------------------------
// ------------------------------------------------
// |  Testing and debugging                       |
// ------------------------------------------------
// ------------------------------------------------

void dumpboard(const Board& b)
{
  for(int i=7; i>=0; --i)
  {
    for(int j=0; j<8; ++j)
    { std::cout << b.pieceAt(8*i+j) << " "; }
    std::cout << std::endl;
  }
}

void ricoTest()
{
  std::cout << "Begin of my test function" << std::endl;

/*
  MemoryDatabase memd;
  memd.open("small.pgn");
  std::cout << "m_count" << std::endl;
  std::cout << memd.count() << std::endl;
*/

  PgnDatabase pgnd;
//  pgnd.open("/home/ricoz/development/chessx/sandbox/bin/test.pgn");
  pgnd.open("/home/ricoz/development/chessx/sandbox/bin/test.pgn");
  std::cout << "m_count: " << pgnd.count() << std::endl;

  ChessXDatabase cxd;
  cxd.create("/home/ricoz/development/chessx/sandbox/bin/aaa.cxd");
//  cxd.open("/home/ricoz/development/chessx/sandbox/bin/aaa.cxd");

  Game g;

//  pgnd.loadGame(682,g);
//  cxd.appendGame(g);


  for(int i=0; i<pgnd.count(); ++i)
  {
    std::cout << "next game to load: " << i+1 << std::endl;
    pgnd.loadGame(i,g);
    cxd.appendGame(g);
  }
  
  cxd.close();
  pgnd.close();

  std::cout << "End of my test function" << std::endl;
}
