/***************************************************************************
                          query.h  -  Holds query definitions.
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

#ifndef __QUERY_H__
#define __QUERY_H__

#include "search.h"
#include <qt34/qptrlist.h>
#include <qt34/qvaluelist.h>


class Query //represents a combination of basic searches using postfix notation
{
   public :
      enum ElementType {SearchElement, OperatorElement};
      typedef QValueList<Search::Operator> OperatorList;
      typedef QPtrList<Search> SearchList;
      typedef QValueList<int> IntList;
      typedef QValueList<ElementType> ElementTypeList;
      Query();
      ~Query();
      //query interrogation methods
      Search::Operator searchOperator(int index);
      Search* search(int index);
      int count();
      bool isValid();

      //query modification methods
      void append(Search::Operator op);
      void append(const Search* search);
      /*bool set(int index, Search::Operator* op);
      bool set(int index, const Search* search);*/
      bool remove(int index);
      void clear();
      /**
       * My own stuff for debugging
       */
      void printArrays();
      QString operatorText(OperatorList::iterator op);
      QString operatorText(Search::Operator op);
      QString searchText(Search* search);
   private :
      OperatorList m_operator;
      SearchList m_search;
      IntList m_operatorMap;
      IntList m_searchMap;
      ElementTypeList m_elementType;
      int m_operatorCount;
      int m_searchCount;
      int m_totalCount;

};

#endif // __QUERY_H__
