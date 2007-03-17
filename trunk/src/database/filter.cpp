/***************************************************************************
                          filter.cpp  -  holds search results in memory
                             -------------------
    begin                : 27/11/2005
    copyright            : (C) 2005 Ejner Borgbjerg <ejner@users.sourceforge.net>
                           (C) 2006 William Hoggarth <whoggarth@users.sourceforge.net>
                           (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "database.h"
#include "filter.h"
#include <QtDebug>

Filter::Filter(Database* database)
{
   m_database = database;
   qDebug() << m_database->count();
   m_count = m_database->count();
   m_bitArray = new QBitArray(m_count);
   m_bitArray->fill(true);
   m_cache.first = m_cache.second = 0;
   m_gamesSearched = 0;
   m_searchTime = 0;
}

Filter::Filter(const Filter& filter)
{
  m_bitArray = new QBitArray(filter.bitArray());
  m_count = filter.m_count;
  m_cache.first = m_cache.second = 0;
  m_gamesSearched = 0;
  m_searchTime = 0;
}

Filter Filter::operator=(const Filter& filter)
{
  delete m_bitArray;
  m_bitArray = new QBitArray(filter.bitArray());
  m_count = filter.count();
  m_cache = filter.m_cache;
  m_gamesSearched = 0;
  m_searchTime = 0;
  return *this;
}

Filter::~Filter()
{
  delete m_bitArray;
}

Database* Filter::database()
{
   return m_database;
}

void Filter::set(int game, bool value)
{
  if (game >= size() || contains(game) == value)
    return;
  m_bitArray->setBit(game, value);
  if (value) {
      m_count++;
  }
  else {
      m_count--;
  }
}

void Filter::setAll(bool value)
{
  m_bitArray->fill(value);
  m_count = value ? size() : 0;
  if (value)
    m_cache.first = m_cache.second = m_count / 2;
  else
    m_cache.first = m_cache.second = 0;
}

bool Filter::contains(const int game) const
{
  return m_bitArray->at(game);
}

int Filter::count() const
{
  return m_count;
}

int Filter::size() const
{
  return (int)m_bitArray->size();
}

int Filter::gameToIndex(int index)
{
  if (index > size() || !contains(index)) return -1;
  if (index < size() / 2)
  {
    int count = 0;
    for (int i = 0; i < index; i++)
      if (contains(i)) count++;
    return count;
  }
  else
  {
    int count = m_count - 1;
    for (int i = size() - 1 ; i > index; i--)
      if (contains(i)) count--;
    return count;
  }
}

int Filter::indexToGame(int index)
{
  if (index > m_count) return -1;
  if (index < m_count / 2)
    for (int i = 0; i < size(); i++)
    {
      index -= contains(i);
      if (index < 0) return i;
    }
  else
    for (int i = size() -1 ; i >= 0; i--)
    {
      index += contains(i);
      if (index >= m_count) return i;
    }
  return -1;
}

int Filter::previousGame(int current) const
{
  if (!m_count) return -1;
  for (int i = current - 1; i >= 0; i--)
    if (contains(i)) return i;
  return -1;
}

int Filter::nextGame(int current) const
{
  if (!m_count) return -1;
  for (int i = current + 1; i < size(); i++)
    if (contains(i)) return i;
  return -1;
}

void Filter::resize(int newsize)
{
  for (int i = newsize; i < size(); i++)  // Decrease count by number of removed games
    if (contains(i)) m_count--;
  m_bitArray->resize(newsize);
}

void Filter::reverse()
{
  m_count = size() - m_count;
  for (int i = 0; i < size(); i++)
    m_bitArray->toggleBit(i);
}

 const bool ops[4][2][2] = {{{0, 0}, {0, 1}} /* And */, {{0, 1}, {1, 1}} /* Or */,
           {{0, 1}, {1, 0}} /* Xor */, {{0, 0}, {1, 0}} /* Minus */};


void Filter::join(const Filter& filter, Operator op)
{
  if (filter.size() != size())
    return;
  m_count = 0;
  for(int i = 0; i < size(); i++)
  {
    m_bitArray->setBit(i, ops[op][contains(i)][filter.contains(i)]);
    m_count += contains(i);
  }
}

QBitArray Filter::bitArray() const
{
  return *m_bitArray;
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
  FilterSearch filterSearch(this);
  query.append(search);
  query.append(filterSearch);
  query.append(searchOperator);
  executeQuery(query);
}
void Filter::executeQuery(Query& query)
{
  QVector <QPair <FilterSearch,int> > filterSearches;
  QVector <QPair <Search*,int> > searches;
  int filterSearchCount = 0;
  int searchCount = 0;
  QTime t;

  t.start();
  m_gamesSearched = 0;

  m_triStateTree = TriStateTree(query);

  /* Make a list of all filter searches, filter searches separately */
  int leafNode = 0;
  for (int element = 0; element < query.count(); element++)
  {
    if (query.isElementSearch(element))
    {
      if (query.search(element)->type() == Search::FilterSearch)
      {
        filterSearches.append(QPair < FilterSearch,
            int >(*static_cast < const FilterSearch * >(query.search(element)), leafNode));
      } else {
        searches.append(QPair < Search*, int >(query.search(element), leafNode));
      }

      leafNode++;
    }
  }
  filterSearchCount = (int) filterSearches.size();
  searchCount = (int) searches.size();

  /* Iterates through all games in the database.
   * Don't worry, a search won't be performed unless necessary */
  for (int searchIndex = 0; searchIndex < size(); searchIndex++)
  { 
    m_triStateTree.clear();

    /* Add filter searches to tree. This could solve the tree in certain cases
     * making it unecessary to call searchGame() */
    if (filterSearchCount)
    {
      for (int search = 0; search < filterSearchCount; search++)
      {
        if (m_triStateTree.setState(filterSearches.at(search).second,
                filterSearches.at(search).first.contains(searchIndex)))
        {
          /* This means the tree evaluated to true */
          break;
        }
      }
      /* So if the filter(s) wasn't enough to solve the tree,
       * lets see what we can find in the game */
      if (m_triStateTree.state() == TriStateTree::Unknown)
      {
        for (int search = 0; search < searchCount; search++) {
           if (m_triStateTree.setState(searches.at(search).second,
                searches.at(search).first->matches(searchIndex)))
           {
              break;
           }
        }
        m_gamesSearched++;
      }
    } else
    {
      /* If the query is not combined with this or any other filter
       * there is no way the tree could have been solved, so just check the game */
       for (int search = 0; search < searchCount; search++) {
          if (m_triStateTree.setState(searches.at(search).second,
                   searches.at(search).first->matches(searchIndex)))
          {
             break;
          }
       }
       m_gamesSearched++;
    }

    /* Update the filter with the result of the tree */
    set(searchIndex, m_triStateTree.state() == TriStateTree::True);
  }
  m_searchTime = t.elapsed();

}
const int Filter::gamesSearched()
{
   return m_gamesSearched;
}
const int Filter::searchTime()
{
   return m_searchTime;
}
