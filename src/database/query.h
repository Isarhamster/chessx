/***************************************************************************
                          query.h  -  Holds query definitions.
                             -------------------
    begin                : 06/12/2005
    copyright            : (C) 2005 Marius Roets
                           <saidinwielder@users.sourceforge.net>
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

#ifndef __QUERY_H__
#define __QUERY_H__
class Search;

#include "search.h"
#include <QList>

/** The Query class is used to store and combine simple and complicated queries.
 * This is done by using reverse polish (postfix) notation. The Query class is a conceptual
 * list of operands (different kinds of searches) and operators (logical AND, OR, NOT), 
 * and methods to manipulate this list.
 * Example (A && B) || (C && D) 
 * where A,B,C,D are different searches and &&=And, ||=Or
 * would be stored as a list:
 * A B && C D && ||
 */

class Query 
{
   public :
      Query();
      ~Query();
      /** @return the operator at index, or NullOperator otherwise */
      Search::Operator searchOperator(int index) const;
      /** @return the search at index, or NULL pointer otherwise. Do not delete the result. */
      Search* search(int index);
      /** @return the number of elements in the list */
      int count() const;
      /** @return the number of operators in the list */
      int countOperators() const;
      /** @return the number of operands (search definitions) in the list */
      int countOperands() const;
      /** @returns true if the element at index is a operand (search definition), false otherwise */
      bool isElementSearch(int index) const;
      /** @returns true if the element at index is a operator , false otherwise */
      bool isElementOperator(int index) const;
      /** @return true if the current list is a valid expression, false otherwise
       * Question: What to do if there are too many operands */
      bool isValid();

      /** query modification methods */
      /** Add a new operator to the list */
      void append(Search::Operator op);
      /** Add a new operand (search definition) to the list */
      void append(const Search& search);
      /** Change element at index to operator op, return true if successful, false otherwise */
      bool set(int index, Search::Operator op);
      /** Change element at index to operand search, return true if successful, false otherwise */
      bool set(int index, const Search& search);
      /** Remove element at index from list, return true if successful, false otherwise */
      bool remove(int index);
      /** Clear the list of all elements */
      void clear();
   private :
      enum ElementType {SearchElement, OperatorElement};
      typedef QList<Search::Operator> OperatorList;
      typedef QList<Search*> SearchList;
      typedef QList<int> IntList;
      typedef QList<ElementType> ElementTypeList;

      /** List of the operators */
      OperatorList m_operator;
      /** List of the operands (search items) */
      SearchList m_search;
      /** Holds the conceptual list index for each operator in m_operator */
      IntList m_operatorMap;
      /** Holds the conceptual list index for each operand in m_search */
      IntList m_searchMap;
      /** Defines whether each item in the conceptual list is a operator (OperatorElement) 
       * or operand (SearchElement) */
      ElementTypeList m_elementType;

      /** Test if index is valid */
      bool isValidIndex(uint index) const;
      bool internalCheck();

};

#endif // __QUERY_H__
