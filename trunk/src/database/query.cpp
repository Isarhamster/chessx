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
   clear();
}
Query::~Query()
{
   clear();
}
Search::Operator Query::searchOperator(int index) const
{
   if (!isValidIndex(index)) 
      return Search::NullOperator;
   int i = m_operatorMap.findIndex(index);
   if (i >= 0) {
      return *(m_operator.at(i));
   } else {
      return Search::NullOperator;
   }
}
Search* Query::search(int index)
{
   if (!isValidIndex(index)) 
      return NULL;
   int i = m_searchMap.findIndex(index);
   if (i >= 0) {
      return m_search.at(i);
   } else {
      return NULL;
   }
}
int Query::count() const
{
   return m_elementType.count();
}
int Query::countOperators() const
{
   return m_operatorMap.count();
}
int Query::countOperands() const
{
   return m_searchMap.count();
}
bool Query::isElementSearch(int index) const
{
   if (!isValidIndex(index)) 
      return false;
   if (m_elementType[index] == SearchElement)
      return true;
   else
      return false;
}
bool Query::isElementOperator(int index) const
{
   if (!isValidIndex(index)) 
      return false;
   if (m_elementType[index] == OperatorElement)
      return true;
   else
      return false;
}
bool Query::isValid()
{
   int i;
   if (!internalCheck())
      return false;
   // Check if there are enough operands for the operators
   int operandCount = 0;
   Search::Operator op;
   ElementTypeList::iterator elementIter;
   for ( elementIter = m_elementType.begin(), i=0; elementIter != m_elementType.end(); ++elementIter,++i ) {
      switch (*elementIter) {
         case SearchElement :
            ++operandCount;
            break;
         case OperatorElement :
            op = searchOperator(i);
            switch (op) {
               case Search::And :
               case Search::Or :
               case Search::Add :
               case Search::Remove :
                  // These operators need 2 operands, and leaves one answer on the stack
                  if (operandCount < 2) {
                     return false;
                  }
                  --operandCount;
                  break;
               case Search::Not :
                  //Not needs one operand, and it leaves one answer on the stack
                  if (operandCount < 1) {
                     return false;
                  }
                  break;
               case Search::NullOperator :
                  // Not sure if this is valid or not
                  break;
               default :
                  // Undefined operator
                  return false;
            }
            break;
         default :
            // Undefined element
            return false;
      }
   }
   if (operandCount != 1) {
      qDebug ("Warning: Not enough operators to evaluate all operands");
   }

   return true;
}
void Query::setAutoDelete(bool flag)
{
   m_search.setAutoDelete(flag);
}

void Query::append(Search::Operator op)
{
   m_operator.append(op);
   m_operatorMap.append(m_elementType.count());
   m_elementType.append(OperatorElement);
}
void Query::append(const Search* search)
{
   m_search.append(search);
   m_searchMap.append(m_elementType.count());
   m_elementType.append(SearchElement);
}
bool Query::set(int index, Search::Operator op)
{
   if (!isValidIndex(index)) 
      return false;
   int subindex = m_operatorMap.findIndex(index);
   if (subindex >= 0) {
      // In this case the given index is already an operator, so we just replace it
      (*m_operator.at(subindex)) = op;
      return true;
   } else {
      subindex = m_searchMap.findIndex(index);
      if (subindex >= 0) {
         // Here the given index is a search, so we have to remove the search,
         // and add a operator at that position. If autodelete is set, the memory
         // for the search will be deallocated.
         m_searchMap.remove(m_searchMap.at(subindex));
         m_search.remove(subindex);
         m_operator.append(op);
         m_operatorMap.append(index);
         m_elementType[index] = OperatorElement;
         return true;
      } else {
         // The index could not be found in the search or operator lists. Strange!?
         return false;
      }
   }

}
bool Query::set(int index, const Search* search)
{
   if (!isValidIndex(index)) 
      return false;
   int subindex = m_searchMap.findIndex(index);
   if (subindex >= 0) {
      // In this case the given index is already an search, so we just replace it
      // Not sure if AutoDelete affects this. Qt Docs doesn't say. Have to test it.
      m_search.replace(subindex,search);
      return true;
   } else {
      subindex = m_operatorMap.findIndex(index);
      if (subindex >= 0) {
         // Here the given index is a operator, so we have to remove the operator,
         // and add a search at that position
         m_operatorMap.remove(m_operatorMap.at(subindex));
         m_operator.remove(m_operator.at(subindex));
         m_search.append(search);
         m_searchMap.append(index);
         m_elementType[index] = SearchElement;
         return true;
      } else {
         // The index could not be found in the operator or search lists
         return false;
      }
   }

}
bool Query::remove(int index)
{
   int indexToRemove;
   IntList::iterator intIt;
   if (!isValidIndex(index)) {
      return false;
   }

   switch (m_elementType[index]) {
      case OperatorElement:
         indexToRemove = m_operatorMap.findIndex(index);
         if (indexToRemove >= 0) {
            m_operatorMap.remove(m_operatorMap.at(indexToRemove));
            m_operator.remove(m_operator.at(indexToRemove));
         } else {
            // Element type does not match actual element
            return false;
         }
         break;
      case SearchElement:
         indexToRemove = m_searchMap.findIndex(index);
         if (indexToRemove >= 0) {
            m_searchMap.remove(m_searchMap.at(indexToRemove));
            m_search.remove(indexToRemove);
         } else {
            // Element type does not match actual element
            return false;
         }
         break;
      default :
         // Undefined element
         return false;
   }
   m_elementType.remove(m_elementType.at(index));
   // Reduces index for elements after the element being remove, moving them
   // up in the virtual list.
   for( intIt = m_operatorMap.begin(); intIt != m_operatorMap.end(); ++intIt ) {
      if ((*intIt) > index) 
         (*intIt)--;
   }
   for( intIt = m_searchMap.begin(); intIt != m_searchMap.end(); ++intIt ) {
      if ((*intIt) > index)
         (*intIt)--;
   }
   return true;
}
void Query::clear()
{
   m_search.clear();
   m_operator.clear();
   m_searchMap.clear();
   m_operatorMap.clear();
   m_elementType.clear();
}

bool Query::isValidIndex(uint index) const
{
   if (index > (m_elementType.count()-1)) 
      return false;
   return true;
}
bool Query::internalCheck()
{  
   int i;
   Search::Operator op;
   Search *s = NULL;
   ElementTypeList::iterator elementIter;

   if (count() != (countOperands() + countOperators())) {
      return false;
   }

   for ( elementIter = m_elementType.begin(), i=0; elementIter != m_elementType.end(); ++elementIter,++i ) {
      switch (*elementIter) {
         case SearchElement :
            s = search(i);
            if (s == NULL) {
               return false;
            }
            switch (s->type()) {
               case Search::NullSearch :
               case Search::PositionSearch :
               case Search::EloSearch :
                  break;
               default :
                  // undefined search
                  return false;
            }
            break;
         case OperatorElement :
            op = searchOperator(i);
            switch (op) {
               case Search::And :
               case Search::Or :
               case Search::Add :
               case Search::Remove :
               case Search::Not :
               case Search::NullOperator :
                  break;
               default :
                  // Undefined operator
                  return false;
            }
            break;
         default :
            // Undefined element
            return false;
      }
   }
   return true;
}
