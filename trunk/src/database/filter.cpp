/***************************************************************************
                          filter.cpp  -  holds search results in memory
                             -------------------
    begin                : 27/11/2005
    copyright            : (C) 2005 Ejner Borgbjerg
                           <ejner@users.sourceforge.net>
                           (C) 2006 William Hoggarth
                           <whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QtDebug>

#include "filter.h"
#include "filtersearch.h"
#include "qpair.h"

Filter::Filter(const int size, const Filter::WhichGames whichGames)
{
	if(whichGames == AllGames) {
		m_bitArray = new QBitArray(size);
		m_bitArray->fill(true);
		m_count = size;
	} else {
		m_bitArray = new QBitArray(size);
		m_bitArray->fill(false);
		m_count = 0;
	}
	
	m_lastNth = 0;
	m_lastIndex = 0;
}

Filter::Filter(Database* database, WhichGames whichGames)
{
   m_database = database;
   int size = m_database->count();
	if(whichGames == AllGames) {
		m_bitArray = new QBitArray(size);
		m_bitArray->fill(true);
		m_count = size;
	} else {
		m_bitArray = new QBitArray(size);
		m_bitArray->fill(false);
		m_count = 0;
	}
	
	m_lastNth = 0;
	m_lastIndex = 0;
}

Filter::Filter(const Filter& filter)
{
	m_bitArray = new QBitArray(filter.bitArray());
	m_count = filter.m_count;
	m_lastNth = filter.m_lastNth;
	m_lastIndex = filter.m_lastIndex;
}

Filter::Filter(const QBitArray& bitArray)
{
	m_count = 0;
	m_bitArray = new QBitArray(bitArray);

	for(int i = 0; i < (int)m_bitArray->size(); i++) {
		m_count += m_bitArray->at(i);
	}
	
	m_lastNth = 0;
	m_lastIndex = 0;
}

Filter Filter::operator=(const Filter& filter)
{
	QBitArray* bitArray = new QBitArray(filter.bitArray());
	delete m_bitArray;
	m_bitArray = bitArray;
	
	m_count = filter.m_count;
	m_lastNth = filter.m_lastNth;
	m_lastIndex = filter.m_lastIndex;
	
	return *this;
}

Filter::~Filter()
{
	delete m_bitArray;
}

void Filter::add(int game)
{
	if(game >= (int)m_bitArray->size()) {
		return;
	}

	if(!m_bitArray->at(game)) {
		//bit arrays are no longer explictly shared in Qt4
		#if QT_VERSION < 0x040000
		m_bitArray->detach();
		#endif
		
		m_bitArray->setBit(game, true);
		m_count++;
		
		if(game < m_lastNth) {
			m_lastNth++;
		}
	}
}

void Filter::remove(int game)
{
	if(game >= (int)m_bitArray->size()) {
		return;
	}
	
  if(m_bitArray->at(game)) {
		//bit arrays are no longer explictly shared in Qt4
		#if QT_VERSION < 0x040000
		m_bitArray->detach();
		#endif
		
		m_bitArray->setBit(game, false);
		m_count--;
		
		if(game < m_lastNth) {
			m_lastNth--;
		}	else if(game == m_lastNth) {
			m_lastNth = 0;
			m_lastIndex = 0;
		}
	}
}

void Filter::setDatabase(Database* database)
{
   m_database = database;
}

Database* Filter::database()
{
   return m_database;
}

void Filter::set(int game, bool value)
{
	if(game >= (int)m_bitArray->size()) {
		return;
	}

	if(m_bitArray->at(game) != value) {
		//bit arrays are no longer explictly shared in Qt4
		#if QT_VERSION < 0x040000
		m_bitArray->detach();
		#endif
		
		m_bitArray->setBit(game, value);
		if(value) {
			m_count++;
			
			if(game < m_lastNth) {
				m_lastNth++;
			}
		} else {
			m_count--;
			
			if(game < m_lastNth) {
				m_lastNth--;
			}	else if(game == m_lastNth) {
				m_lastNth = 0;
				m_lastIndex = 0;
			}
		}
	}
}

void Filter::setAll(bool value)
{
	//bit arrays are no longer explictly shared in Qt4
	#if QT_VERSION < 0x040000
	m_bitArray->detach();
	#endif
	
	m_bitArray->fill(value);
	m_count = value * (int)m_bitArray->size();
	
	m_lastNth = 0;
	m_lastIndex = 0;
}

bool Filter::contains(const int game) const
{
	return m_bitArray->at(game);
}

int Filter::gameIndex(int nth) const
{
	if(nth >= m_count) {
		return -1;
	}
	
	int i;
	int count;
	
	if(nth > m_lastNth) {
		i = m_lastIndex + 1;
		count = m_lastNth;
	} else {
		i = 0;
		count = -1;
   }
	
	while (count < nth) {
		count += m_bitArray->at(i);
		i++;
	}

	m_lastNth = nth;
	m_lastIndex = i - 1;
	
	return i - 1;
}

int Filter::count() const
{
	return m_count;
}

int Filter::size() const{
	return (int)m_bitArray->size();
}

void Filter::resize(int size, Filter::WhichGames whichGames)
{
	delete m_bitArray;
	
	if(whichGames == AllGames) {
		m_bitArray = new QBitArray(size);
		m_bitArray->fill(true);
		m_count = size;
	} else {
		m_bitArray = new QBitArray(size);
		m_bitArray->fill(false);
		m_count = 0;
	}
	
	m_lastNth = 0;
	m_lastIndex = 0;
}

void Filter::reverse()
{
	//bit arrays are no longer explictly shared in Qt4
	#if QT_VERSION < 0x040000
	m_bitArray->detach();
	#endif

	m_count = m_bitArray->size() - m_count;

	for(int i = 0; i < (int)m_bitArray->size(); i++) {
		m_bitArray->toggleBit(i);
	}
	
	m_lastNth = 0;
	m_lastIndex = 0;
}

void Filter::intersect(const Filter& filter)
{
	//bit arrays are no longer explictly shared in Qt4
	#if QT_VERSION < 0x040000
	m_bitArray->detach();
	#endif

	m_count = 0;

	for(int i = 0; i < (int)m_bitArray->size(); i++) {
		m_bitArray->setBit(i, m_bitArray->at(i) && filter.m_bitArray->at(i));
		m_count += m_bitArray->at(i);
	}
	
	m_lastNth = 0;
	m_lastIndex = 0;
}

void Filter::add(const Filter& filter){
	//bit arrays are no longer explictly shared in Qt4
	#if QT_VERSION < 0x040000
	m_bitArray->detach();
	#endif
	
	m_count = 0;

	for(int i = 0; i < (int)m_bitArray->size(); i++) {
		m_bitArray->setBit(i, m_bitArray->at(i) || filter.m_bitArray->at(i));
		m_count += m_bitArray->at(i);
	}
	
	m_lastNth = 0;
	m_lastIndex = 0;
}

void Filter::remove(const Filter& filter)
{
	//bit arrays are no longer explictly shared in Qt4
	#if QT_VERSION < 0x040000
	m_bitArray->detach();
	#endif
	
	m_count = 0;

	for(int i = 0; i < (int)m_bitArray->size(); i++) {
		m_bitArray->setBit(i, m_bitArray->at(i) && ~filter.m_bitArray->at(i));
		m_count += m_bitArray->at(i);
	}
	
	m_lastNth = 0;
	m_lastIndex = 0;
}

QBitArray Filter::asBitArray() const
{
	QBitArray copy = *m_bitArray;
	
	//bit arrays are no longer explictly shared in Qt4
	#if QT_VERSION < 0x040000
	copy.detach();
	#endif
	
	return copy;
}

QBitArray Filter::bitArray() const
{
	return *m_bitArray;
}
TriStateTree::State Filter::setState(int leaf, TriStateTree::State state)
{
   return m_triStateTree.setState(leaf,state);
}
TriStateTree::State Filter::state() const
{
   return m_triStateTree.state();
}
TriStateTree::State Filter::state(int leaf) const
{
   return m_triStateTree.state(leaf);
}
TriStateTree::State Filter::setState(int leaf, bool state)
{
   return m_triStateTree.setState(leaf,state);
}

void Filter::executeSearch(const Search& search)
{
	//Turn search into a query and execute it
	Query query;
	query.append(search);
	executeQuery(query);
}
void Filter::executeSearch(const Search& search, Search::Operator searchOperator)
{
	//Turn search into a query and execute it
	Query query;
   FilterSearch filterSearch(*this);
	query.append(search);
   query.append(filterSearch);
   query.append(searchOperator);

	executeQuery(query);
}
void Filter::executeQuery(Query& query)
{
   QVector<QPair<FilterSearch, int> > filterSearches;
   int filterSearchCount = 0;
   int gamesSearched = 0; //For debugging

   m_database->initSearch(query, this);
	m_triStateTree = TriStateTree(query);

   /* Make a list of all filter searches */
   int leafNode = 0;
	for(int element = 0; element < query.count(); element++) {
		if(query.isElementSearch(element)) {
			if (query.search(element)->type() == Search::FilterSearch) {
            filterSearches.append(QPair<FilterSearch, int>(*static_cast<const FilterSearch*>(query.search(element)), leafNode));
         }		
         leafNode++;
      }
   }
   filterSearchCount = (int)filterSearches.size();
	
   /* Iterates through all games in the database.
    * Don't worry, a search won't be performed unless necessary */
   for(int searchIndex = 0; searchIndex < size(); searchIndex++) {
		m_triStateTree.clear();

      /* Add filter searches to tree. This could solve the tree in certain cases
       * making it unecessary to call searchGame() */
      if (filterSearchCount) {
         for(int search = 0; search < filterSearchCount; search++) {
            if(m_triStateTree.setState(filterSearches.at(search).second,filterSearches.at(search).first.contains(searchIndex))) {
               /* This means the tree evaluated to true */
               break;
            }
         } 
         /* So if the filter(s) wasn't enough to solve the tree,
          * lets see what we can find in the game */
         if (m_triStateTree.state() == TriStateTree::Unknown) {
            m_database->searchGame(searchIndex);
            gamesSearched++;
         }
      } else {
         /* If the query is not combined with this or any other filter
          * there is no way the tree could have been solved, so just check
          * the game */
         m_database->searchGame(searchIndex);
         gamesSearched++;
      }
		
      /* Update the filter with the result of the tree */
		if(m_triStateTree.state() == TriStateTree::True) {
			set(searchIndex, true);
		} else {
         set(searchIndex, false);
      }
	}
   
   m_database->finalizeSearch();
   //qDebug ("%d Games searched\n%d Games skipped\n%d Total",gamesSearched,size()-gamesSearched,size());
	
}
