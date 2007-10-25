/***************************************************************************
                          cxdindex.cpp  - handles loading and saving of 
					the index of a chessxdatabase
                             -------------------
   begin                : 18 October 2007
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

#include "cxdindex.h" 
#include "common.h"

const Tag CxdIndex::tags[12] = {TagEvent,TagSite,TagDate,TagRound,TagWhite,TagBlack,
                        TagResult,TagFEN,TagWhiteElo,TagBlackElo,TagPlyCount,TagECO};


CxdIndex::CxdIndex(Index& index)
{
  m_index=&index;

  for(int i=0,k=0; i<m_nbIndexTags; ++i)
  {
    m_toTagFileIndex[i]=k; 
    m_mappedTags[k]=BasicTagMap[tags[i]];
    if(tags[i]!=TagWhite && tags[i]!=TagWhiteElo) ++k;
  }

  for(int i=0; i<m_nbTagFiles; ++i)
  {
    m_tagDataStreams[i].setDevice(&m_tagFiles[i]);
  }

  m_indexDataStream.setDevice(&m_indexFile);

  m_saxhandler=0;
  m_isOpen=false;
}

CxdIndex::~CxdIndex()
{
  if(m_isOpen) close();
}

bool CxdIndex::open(SaxHandler& saxhandler)
{
  m_saxhandler=&saxhandler;
  setFilenamesFromSaxHandler();
  openFiles();

  m_index->clear();
  // loading tagvalues into memory
  for(int i=0; i<m_nbTagFiles; ++i)
  {
    StringTagValues* tv = new StringTagValues;
    m_tagFiles[i].seek(0);
    tv->read(m_tagDataStreams[i]);
    m_index->m_tagList.addTagValues(m_mappedTags[i],tv);
  } 

  // loading index into memory
  m_indexFile.peek(0);
  while(!m_indexFile.atEnd())
  {
    IndexItem* i = new IndexItem;
    i->cxdRead(m_indexFile);
    m_index->add(i);
  }

  m_isOpen=true;
  return 1;
}

void CxdIndex::close()
{
  if(!m_isOpen) return;
  m_indexFile.close();
  for(int i=0; i<m_nbTagFiles; ++i)
  {m_tagFiles[i].close();}
  m_isOpen=false;
}

bool CxdIndex::create(SaxHandler& saxhandler)
{
  m_saxhandler=&saxhandler;
  setFilenamesFromSaxHandler();

  // Creating tagvalues
  m_index->clear();
  for(int i=0; i<m_nbTagFiles; ++i)
  {
    StringTagValues* tv = new StringTagValues;
    m_index->m_tagList.addTagValues(m_mappedTags[i],tv);
  } 

  // Here we could warn if the files already exist. 
  m_indexFile.resize(0);
  for(int i=0; i<m_nbTagFiles; ++i)
  {m_tagFiles[i].resize(0);}
  openFiles();

  m_isOpen=true;
  return 1;
}



GameId CxdIndex::appendGame(Game& game)
{
  // special tags are set explicitly.
  //  The use of setTag with TagNames is not very efficient. On
  //  this could be worked later by introducing an appropriate
  //  function in game allowing to set tags directly by giving
  //  the tagindex. 
  game.setTag(TagNames[TagPlyCount],QString().setNum(game.plyCount()));
  game.setTag(TagNames[TagECO],game.ecoClassify());

  // updating indextag-files
  QString s;
  TagValues* tv;
  for(int i=0; i<m_nbIndexTags; ++i)
  {
    s=game.tag(TagNames[tags[i]]);
    tv=m_index->m_tagList[tags[i]];
    if(!tv->contains(s))
    {
      tv->appendToStream(s,m_tagDataStreams[m_toTagFileIndex[i]]);
      m_tagFiles[m_toTagFileIndex[i]].flush();
    }
  }

  // updating index in memory
  int gameId=m_index->cxdAdd(game);

  // updating index on disk
  m_indexFile.seek(m_indexFile.size());
  m_index->m_indexItems[gameId]->cxdWrite(m_indexFile);  
  m_indexFile.flush();

  return gameId;
}

int CxdIndex::getTagFileIndex(const Tag& tag)
{
  Tag t(tag);
  if(t==TagPlayerName) t=TagWhite;
  else if(t==TagPlayerElo) t=TagWhiteElo;
  int i=0;
  while(i<m_nbIndexTags)
  { 
    if(tags[i]==t) break;
    ++i;
  }
  if(i==m_nbIndexTags) return -1;
  return m_toTagFileIndex[i];
}

void CxdIndex::setFilenamesFromSaxHandler()
{
  m_indexFile.setFileName(m_saxhandler->m_indexFilename);

  m_tagFiles[getTagFileIndex(TagEvent)].setFileName(m_saxhandler->m_indexEventFilename);
  m_tagFiles[getTagFileIndex(TagSite)].setFileName(m_saxhandler->m_indexSiteFilename);
  m_tagFiles[getTagFileIndex(TagDate)].setFileName(m_saxhandler->m_indexDateFilename);
  m_tagFiles[getTagFileIndex(TagRound)].setFileName(m_saxhandler->m_indexRoundFilename);
  m_tagFiles[getTagFileIndex(TagPlayerName)].setFileName(m_saxhandler->m_indexPlayerNameFilename);
  m_tagFiles[getTagFileIndex(TagResult)].setFileName(m_saxhandler->m_indexResultFilename);
  m_tagFiles[getTagFileIndex(TagPlyCount)].setFileName(m_saxhandler->m_indexPlyCountFilename);
  m_tagFiles[getTagFileIndex(TagFEN)].setFileName(m_saxhandler->m_indexFENFilename);
  m_tagFiles[getTagFileIndex(TagPlayerElo)].setFileName(m_saxhandler->m_indexPlayerEloFilename);
  m_tagFiles[getTagFileIndex(TagECO)].setFileName(m_saxhandler->m_indexECOFilename);
}

void CxdIndex::openFiles()
{
  m_indexFile.open(QIODevice::ReadWrite);
  for(int i=0;i<m_nbTagFiles; ++i)
  {m_tagFiles[i].open(QIODevice::ReadWrite);}
}

