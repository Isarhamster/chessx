/***************************************************************************
 *   (C) 2005 William Hoggarth <whoggarth@users.sourceforge.net>           *
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "query.h"
#include "search.h"
#include "filteroperator.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

Query::Query()
{
    clear();
}

Query::~Query()
{
    while(!m_search.isEmpty())
    {
        delete m_search.takeFirst();
    }
    clear();
}

FilterOperator Query::searchOperator(int index) const
{
    if(!isValidIndex(index))
    {
        return FilterOperator::NullOperator;
    }
    int i = m_operatorMap.indexOf(index);
    if(i >= 0)
    {
        return m_operator.at(i);
    }
    else
    {
        return FilterOperator::NullOperator;
    }
}

Search* Query::search(int index)
{
    if(!isValidIndex(index))
    {
        return NULL;
    }
    int i = m_searchMap.indexOf(index);
    if(i >= 0)
    {
        return m_search.at(i);
    }
    else
    {
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
    if(!isValidIndex(index))
    {
        return false;
    }
    if(m_elementType[index] == SearchElement)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Query::isElementOperator(int index) const
{
    if(!isValidIndex(index))
    {
        return false;
    }
    if(m_elementType[index] == OperatorElement)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Query::isValid()
{
    int i;
    if(!internalCheck())
    {
        return false;
    }
    // Check if there are enough operands for the operators
    int operandCount = 0;
    FilterOperator op;
    ElementTypeList::const_iterator elementIter;
    for(elementIter = m_elementType.cbegin(), i = 0; elementIter != m_elementType.cend(); ++elementIter, ++i)
    {
        switch(*elementIter)
        {
        case SearchElement :
            ++operandCount;
            break;
        case OperatorElement :
            op = searchOperator(i);
            switch(op)
            {
            case FilterOperator::And :
            case FilterOperator::Or :
            case FilterOperator::Remove :
                // These operators need 2 operands, and leaves one answer on the stack
                if(operandCount < 2)
                {
                    return false;
                }
                --operandCount;
                break;
            case FilterOperator::Not :
                //Not needs one operand, and it leaves one answer on the stack
                if(operandCount < 1)
                {
                    return false;
                }
                break;
            case FilterOperator::NullOperator :
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
    if(operandCount != 1)
    {
        qDebug("Warning: Not enough operators to evaluate all operands");
    }

    return true;
}

void Query::append(FilterOperator op)
{
    m_operator.append(op);
    m_operatorMap.append(m_elementType.count());
    m_elementType.append(OperatorElement);
}

void Query::append(Search* search)
{
    m_search.append(search);
    m_searchMap.append(m_elementType.count());
    m_elementType.append(SearchElement);
}

bool Query::set(int index, FilterOperator op)
{
    if(!isValidIndex(index))
    {
        return false;
    }
    int subindex = m_operatorMap.indexOf(index);
    if(subindex >= 0)
    {
        // In this case the given index is already an operator, so we just replace it
        m_operator[subindex] = op;
        return true;
    }
    else
    {
        subindex = m_searchMap.indexOf(index);
        if(subindex >= 0)
        {
            // Here the given index is a search, so we have to remove the search,
            // and add a operator at that position. If autodelete is set, the memory
            // for the search will be deallocated.
            m_searchMap.removeAt(m_searchMap.at(subindex));
            delete m_search.at(subindex);
            m_search.removeAt(subindex);
            m_operator.append(op);
            m_operatorMap.append(index);
            m_elementType[index] = OperatorElement;
            return true;
        }
        else
        {
            // The index could not be found in the search or operator lists. Strange!?
            return false;
        }
    }
}

bool Query::set(int index, Search* search)
{
    if(!isValidIndex(index))
    {
        return false;
    }
    int subindex = m_searchMap.indexOf(index);
    if(subindex >= 0)
    {
        delete m_search.at(subindex);
        m_search.replace(subindex, search);
        return true;
    }
    else
    {
        subindex = m_operatorMap.indexOf(index);
        if(subindex >= 0)
        {
            // Here the given index is a operator, so we have to remove the operator,
            // and add a search at that position
            m_operatorMap.removeAt(m_operatorMap.at(subindex));
            m_operator.removeAt(m_operator.at(subindex));
            m_search.append(search);
            m_searchMap.append(index);
            m_elementType[index] = SearchElement;
            return true;
        }
        else
        {
            // The index could not be found in the operator or search lists
            return false;
        }
    }
}

bool Query::remove(int index)
{
    int indexToRemove;
    IntList::iterator intIt;
    if(!isValidIndex(index))
    {
        return false;
    }

    switch(m_elementType[index])
    {
    case OperatorElement:
        indexToRemove = m_operatorMap.indexOf(index);
        if(indexToRemove >= 0)
        {
            m_operatorMap.removeAt(m_operatorMap.at(indexToRemove));
            m_operator.removeAt(m_operator.at(indexToRemove));
        }
        else
        {
            // Element type does not match actual element
            return false;
        }
        break;
    case SearchElement:
        indexToRemove = m_searchMap.indexOf(index);
        if(indexToRemove >= 0)
        {
            m_searchMap.removeAt(m_searchMap.at(indexToRemove));
            delete m_search.at(indexToRemove);
            m_search.removeAt(indexToRemove);
        }
        else
        {
            // Element type does not match actual element
            return false;
        }
        break;
    default :
        // Undefined element
        return false;
    }
    m_elementType.removeAt(m_elementType.at(index));
    // Reduces index for elements after the element being remove, moving them
    // up in the virtual list.
    for(intIt = m_operatorMap.begin(); intIt != m_operatorMap.end(); ++intIt)
    {
        if((*intIt) > index)
        {
            (*intIt)--;
        }
    }
    for(intIt = m_searchMap.begin(); intIt != m_searchMap.end(); ++intIt)
    {
        if((*intIt) > index)
        {
            (*intIt)--;
        }
    }
    return true;
}

void Query::clear()
{
    while(!m_search.isEmpty())
    {
        delete m_search.takeFirst();
    }
    m_search.clear();
    m_operator.clear();
    m_searchMap.clear();
    m_operatorMap.clear();
    m_elementType.clear();
}

bool Query::isValidIndex(unsigned int index) const
{
    if((int)index > (int)(m_elementType.count() - 1))
    {
        return false;
    }
    return true;
}

bool Query::internalCheck()
{
    return (count() == (countOperands() + countOperators()));
}
