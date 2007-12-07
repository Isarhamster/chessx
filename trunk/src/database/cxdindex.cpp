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

#include <QtDebug>

#include "cxdindex.h"
#include "common.h"
#include "cxdcompact.h"

const Tag CxdIndex::tags[m_nbIndexTags] = {TagEvent, TagSite, TagDate, TagRound, TagWhite, TagBlack,
				TagResult, TagWhiteElo, TagBlackElo, TagPlyCount, TagECO,
				TagFEN
			       };


CxdIndex::CxdIndex(Index& index)
:m_indexCFile(Index::defaultIndexItemSize)
{
	m_indexFile=m_indexCFile.qf();
	m_index = &index;

	for (int i = 0, k = 0; i < m_nbIndexTags; ++i) {
		m_toTagFileIndex[i] = k;
		m_mappedTags[k] = BasicTagMap[tags[i]];
		if (tags[i] != TagWhite && tags[i] != TagWhiteElo) ++k;
	}

	for (int i = 0; i < m_nbTagFiles; ++i) {
		m_tagDataStreams[i].setDevice(&m_tagFiles[i]);
	}

	m_saxhandler = 0;
	m_isOpen = false;
}

CxdIndex::~CxdIndex()
{
	if (m_isOpen) close();
}

bool CxdIndex::open(SaxHandler& saxhandler)
{
	m_saxhandler = &saxhandler;

	m_indexCFile.open(saxhandler.m_indexFilename);
	setTagFilenamesFromSaxHandler();
	openTagFiles();

	m_index->clear();
	// loading tagvalues into memory
	for (int i = 0; i < m_nbTagFiles; ++i) {
		StringTagValues* tv = new StringTagValues;
		m_tagFiles[i].seek(0);
		tv->read(m_tagDataStreams[i]);
		m_index->m_tagList.addTagValues(m_mappedTags[i], tv);
	}

	// loading index into memory
	m_indexCFile.qf()->seek(0);
	while (!m_indexFile->atEnd()) {
		IndexItem* i = new IndexItem;
		i->cxdRead(*m_indexFile);
		m_index->add(i);
	}

	m_isOpen = true;
	return 1;
}

void CxdIndex::close()
{
	if (!m_isOpen) return;
	m_indexCFile.close();
	for (int i = 0; i < m_nbTagFiles; ++i)
		{m_tagFiles[i].close();}
	m_isOpen = false;
}

bool CxdIndex::create(SaxHandler& saxhandler)
{
	m_saxhandler = &saxhandler;
	m_indexCFile.create(saxhandler.m_indexFilename);

	setTagFilenamesFromSaxHandler();

	// Creating tagvalues
	m_index->clear();
	for (int i = 0; i < m_nbTagFiles; ++i) {
		StringTagValues* tv = new StringTagValues;
		m_index->m_tagList.addTagValues(m_mappedTags[i], tv);
	}

	for (int i = 0; i < m_nbTagFiles; ++i)
		{m_tagFiles[i].resize(0);}
	openTagFiles();
	// Because of the possibility that tagvalues contain by construction
	// default values we write their current status to disk.
	for (int i = 0; i < m_nbTagFiles; ++i)
	        {m_index->m_tagList[tags[i]]->write(m_tagDataStreams[i]);}

	m_isOpen = true;
	return 1;
}

void CxdIndex::compact(const QList<bool>& ql)
{
	for(int i=0; i<m_nbTagFiles; ++i)
	{
	  Tag t=m_mappedTags[i];
	  TagValues* tv=m_index->tagValues(t);
	  QVector<bool> qv(tv->count(),0);
	  QList<Tag> tagTList;
	  for(int j=0; j<m_nbIndexTags ; ++j)
	  {
	    if(BasicTagMap[tags[j]]==t) tagTList.append(tags[j]);
	  }

	  // Fill qv
	  for(int k=0; k<tagTList.size(); ++k)
	  {
	    Tag ct(tagTList[k]);
	    int indexoffset=m_index->m_tagIndexPosition[ct].first;
	    int indexsize=m_index->m_tagIndexPosition[ct].second;
	    for(int j=0; j<ql.size(); ++j)
	    {
	      if(ql[j]) qv[m_index->m_indexItems[j]->index(indexoffset,indexsize)]=1;
	    }
	  }

	  // The default tagvalue in stringtagvalue should never be removed.
	  // Todo: eliminate default tag value or clean up this part such that
	  // it works for all tagvalues and not only stringtagvalues.
	  qv[0]=1;

	  // Building index map
	  QVector<qint32> indexMap(tv->count());
	  int newindex=0;
	  for(int j=0; j<tv->count(); ++j)
	  {
	    if(qv[j]) indexMap[j]=newindex++; 
	  }

	  // Changing index in memory
	  for(int k=0; k<tagTList.size(); ++k)
	  { 
	    Tag ct(tagTList[k]);
	    int indexoffset=m_index->m_tagIndexPosition[ct].first;
	    int indexsize=m_index->m_tagIndexPosition[ct].second;
	    for(int j=0; j<ql.size(); ++j)
	    {
	      if(ql[j]) // The index will only be adapted for games not to be deleted.
	      {
	      m_index->m_indexItems[j]->set(indexoffset,indexsize,
		  indexMap[m_index->m_indexItems[j]->index(indexoffset,indexsize)]);
	      }
	    }
	  }

	  // Changing tagvalues in memory.
	  tv->compact(qv);


	  // Writing tagvalues to disk
	  m_tagFiles[i].resize(0);	  
	  tv->write(m_tagDataStreams[i]);
	  m_tagFiles[i].flush();
	}

	// Compacting index and flags on memory.
	CxdCompact::compactList(m_index->m_indexItems,ql);
	CxdCompact::compactList(m_index->m_deleteFlags,ql);
	m_index->m_nbUsedIndexItems=m_index->m_deleteFlags.size();

	// Writing new index to disk
	m_indexFile->seek(0);
	for(int i=0; i<m_index->m_nbUsedIndexItems; ++i)
	{
	  m_index->m_indexItems[i]->cxdWrite(*m_indexFile);
	}
	m_indexFile->resize(m_index->m_nbUsedIndexItems*m_indexCFile.recordsize());
}


GameId CxdIndex::appendGame(Game& game)
{
	//  Special tags are set explicitly.
	prepareGameTags(game);	

	// updating indextag-files
	addToIndexTagFiles(game);

	// updating index in memory
	int gameId = m_index->cxdAdd(game);

	// updating index on disk
	m_indexFile->seek(m_indexFile->size());
	m_index->m_indexItems[gameId]->cxdWrite(*m_indexFile);
	m_indexFile->flush();

	return gameId;
}

void CxdIndex::replaceGame(Game& game, const int& gameId)
{
	//  Special tags are set explicitly.
	prepareGameTags(game);	

	// updating indextag-files
	addToIndexTagFiles(game);

	// updating index in memory
	m_index->cxdReplace(game,gameId);

	// updating index on disk
	m_indexCFile.seek(gameId);
	m_index->m_indexItems[gameId]->cxdWrite(*m_indexFile);
	m_indexFile->flush();
}

int CxdIndex::getTagFileIndex(const Tag& tag)
{
	Tag t(tag);
	if (t == TagPlayerName) t = TagWhite;
	else if (t == TagPlayerElo) t = TagWhiteElo;
	int i = 0;
	while (i < m_nbIndexTags) {
		if (tags[i] == t) break;
		++i;
	}
	if (i == m_nbIndexTags) return -1;
	return m_toTagFileIndex[i];
}

void CxdIndex::prepareGameTags(Game& game)
{
	//  The use of setTag with TagNames is not very efficient. On
	//  this could be worked later by introducing an appropriate
	//  function in game allowing to set tags directly by giving
	//  the tagindex.
	game.setTag(TagNames[TagPlyCount], QString().setNum(game.plyCount()));
	game.setTag(TagNames[TagECO], game.ecoClassify());
	Board startb(game.startBoard());
        if(startb!=standardStartBoard)
	{
	  game.setTag(TagNames[TagFEN],startb.toFen());
	}
	else
	{
	  game.setTag(TagNames[TagFEN],QString());
	}
}

void CxdIndex::addToIndexTagFiles(Game& game)
{
	QString s;
	TagValues* tv;
	for (int i = 0; i < m_nbIndexTags; ++i) {
		s = game.tag(TagNames[tags[i]]);
		tv = m_index->m_tagList[tags[i]];
		if (!tv->contains(s)) {
			tv->appendToStream(s, m_tagDataStreams[m_toTagFileIndex[i]]);
			m_tagFiles[m_toTagFileIndex[i]].flush();
		}
	}
}

void CxdIndex::setTagFilenamesFromSaxHandler()
{
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

void CxdIndex::openTagFiles()
{
	for (int i = 0;i < m_nbTagFiles; ++i)
		{m_tagFiles[i].open(QIODevice::ReadWrite);}
}

