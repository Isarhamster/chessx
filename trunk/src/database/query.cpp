/***************************************************************************
                          query.cpp  -  Holds query definitions
                             -------------------
    begin                : 06/12/2005
    copyright            : (C) 2005 Marius Roets
                           <saidinwielder@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "query.h"

Query::Query()
{
   m_search.setAutoDelete(true);
   clear();
}
Query::~Query()
{
   clear();
}
Search::Operator Query::searchOperator(int index)
{
   int i = m_operatorMap.findIndex(index);
   if (i >= 0) {
      return *(m_operator.at(i));
   } else {
      return Search::NullOperator;
   }
}
Search* Query::search(int index)
{
   int i = m_searchMap.findIndex(index);
   if (i >= 0) {
      return m_search.at(i);
   } else {
      return NULL;
   }
}
int Query::count()
{
   return m_totalCount;
}

bool Query::isValid()
{
   return true;
}
void Query::append(Search::Operator op)
{
   m_operator.append(op);
   m_operatorMap.append(m_totalCount);
   m_elementType.append(OperatorElement);
   m_totalCount++;
   m_operatorCount++;
}
void Query::append(const Search* search)
{
   m_search.append(search);
   m_searchMap.append(m_totalCount);
   m_elementType.append(SearchElement);
   m_totalCount++;
   m_searchCount++;
}
bool Query::remove(int index)
{
   int indexToRemove;
   IntList::iterator intIt;

   m_elementType.remove(m_elementType.at(index));
   indexToRemove = m_operatorMap.findIndex(index);
   if (indexToRemove >= 0) {
      m_operatorMap.remove(m_operatorMap.at(indexToRemove));
      m_operator.remove(m_operator.at(indexToRemove));
      m_operatorCount--;
      m_totalCount--;
   }
   indexToRemove = m_searchMap.findIndex(index);
   if (indexToRemove >= 0) {
      m_searchMap.remove(m_searchMap.at(indexToRemove));
      m_search.remove(indexToRemove);
      m_searchCount--;
      m_totalCount--;
   }
   for( intIt = m_operatorMap.begin(); intIt != m_operatorMap.end(); ++intIt ) {
      if ((*intIt) > index) {
         (*intIt)--;
      }
   }
   for( intIt = m_searchMap.begin(); intIt != m_searchMap.end(); ++intIt ) {
      if ((*intIt) > index) {
         (*intIt)--;
      }
   }
   return true;
}
void Query::clear()
{
   m_searchCount = 0;
   m_operatorCount = 0;
   m_totalCount = 0;
   m_search.clear();
   m_operator.clear();
   m_searchMap.clear();
   m_operatorMap.clear();
   m_elementType.clear();
}

