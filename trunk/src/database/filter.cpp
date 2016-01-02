/***************************************************************************
 *   (C) 2005 Ejner Borgbjerg <ejner@users.sourceforge.net>                *
 *   (C) 2006 William Hoggarth <whoggarth@users.sourceforge.net>           *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "database.h"
#include "filter.h"
#include "filtersearch.h"
#include "query.h"
#include "tristatetree.h"
#include <QtDebug>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

Filter::Filter(Database* database) : QThread()
{
    m_database = database;
    m_count = m_database->count();
    m_vector = new QVector<int>(m_count, 1);
    m_cache.first = m_cache.second = -1;
    m_gamesSearched = 0;
    m_searchTime = 0;
    currentSearch = 0;
}

Filter::~Filter()
{
    cancel();
    delete currentSearch;
    delete m_vector;
}

Database* Filter::database()
{
    return m_database;
}

void Filter::set(int game, int value)
{
    if((game >= size()) || (gamePosition(game) == value))
    {
        return;
    }
    if(value && !contains(game))
    {
        m_count++;
    }
    else if(!value && contains(game))
    {
        m_count--;
    }
    (*m_vector)[game] = value;
}

void Filter::setAll(int value)
{
    cancel();
    m_vector->fill(value);
    m_count = value ? size() : 0;
    m_cache.first = m_cache.second = -1;
}

bool Filter::contains(int game) const
{
    if(game < m_vector->count())
    {
        return (m_vector->at(game) != 0);
    }
    return false;
}

int Filter::gamePosition(int game) const
{
    return m_vector->at(game);
}

int Filter::size() const
{
    return m_vector->size();
}

int Filter::gameToIndex(int index)
{
    if(index > size() || index < 0 || !contains(index))
    {
        return -1;
    }
    if(index < size() / 2)
    {
        int count = 0;
        for(int i = 0; i < index; ++i)
        {
            if(contains(i))
            {
                ++count;
            }
        }
        return count;
    }
    else
    {
        int count = m_count - 1;
        for(int i = size() - 1 ; i > index; i--)
        {
            if(contains(i))
            {
                count--;
            }
        }
        return count;
    }
}

int Filter::indexToGame(int index)
{
    if(m_cache.first == index)
    {
        return m_cache.second;
    }
    m_cache.first = index;
    if(index < m_count)
    {
        if(index < m_count / 2)
        {
            for(int i = 0; i < size(); ++i)
            {
                index -= contains(i);
                if(index < 0)
                {
                    m_cache.second = i;
                    return i;
                }
            }
        }
        else
        {
            for(int i = size() - 1 ; i >= 0; i--)
            {
                index += contains(i);
                if(index >= m_count)
                {
                    m_cache.second = i;
                    return i;
                }
            }
        }
    }
    m_cache.second = -1;
    return -1;
}

int Filter::previousGame(int current) const
{
    if(!m_count)
    {
        return -1;
    }
    for(int i = qBound(-1, current, size()) - 1; i >= 0; i--)
    {
        if(contains(i))
        {
            return i;
        }
    }
    return -1;
}

int Filter::nextGame(int current) const
{
    if(!m_count)
    {
        return -1;
    }
    for(int i = qBound(-1, current, size()) + 1; i < size(); ++i)
    {
        if(contains(i))
        {
            return i;
        }
    }
    return -1;
}

void Filter::resize(int newsize, bool includeNew)
{
    for(int i = newsize; i < size(); ++i)   // Decrease count by number of removed games
    {
        if(contains(i))
        {
            m_count--;
        }
    }
    int oldsize = size();
    m_vector->resize(newsize);
    // Set new (uninitialized games) to 'includeNew' value.
    for(int i = oldsize; i < newsize; ++i)
    {
        (*m_vector)[i] = includeNew;
    }
    if(includeNew)
    {
        m_count += newsize - oldsize;
    }
}

void Filter::reverse()
{
    m_count = size() - m_count;
    for(int i = 0; i < size(); ++i)
    {
        if(m_vector->at(i))
        {
            (*m_vector)[i] = 0;
        }
        else
        {
            (*m_vector)[i] = 1;
        }
    }

}

QVector<int> Filter::intVector() const
{
    return *m_vector;
}

void Filter::run()
{
    connect(currentSearch, SIGNAL(prepareUpdate(int)), this, SIGNAL(searchProgress(int)));
    currentSearch->Prepare(m_break);
    switch (currentSearchOperator)
    {
    case Filter::NullOperator:
        for(int searchIndex = 0; searchIndex < size(); ++searchIndex)
        {
            if (m_break) break;
            set(searchIndex, currentSearch->matches(searchIndex));
            if (searchIndex % 1024 == 0) emit searchProgress(searchIndex*100/size());
        }
        break;
    case Filter::And:
        for(int searchIndex = 0; searchIndex < size(); ++searchIndex)
        {
            if (m_break) break;
            if (contains(searchIndex))
            {
                set(searchIndex, currentSearch->matches(searchIndex));
            }
            if (searchIndex % 1024 == 0) emit searchProgress(searchIndex*100/size());
        }
        break;
    case Filter::Or:
        for(int searchIndex = 0; searchIndex < size(); ++searchIndex)
        {
            if (m_break) break;
            if (!contains(searchIndex))
            {
                set(searchIndex, currentSearch->matches(searchIndex));
            }
            if (searchIndex % 1024 == 0) emit searchProgress(searchIndex*100/size());
        }
        break;
    case Filter::Remove:
        for(int searchIndex = 0; searchIndex < size(); ++searchIndex)
        {
            if (m_break) break;
            if (contains(searchIndex))
            {
                set(searchIndex, !currentSearch->matches(searchIndex));
            }
            if (searchIndex % 1024 == 0) emit searchProgress(searchIndex*100/size());
        }
        break;
    default:
        break;
    }
    delete currentSearch;
    currentSearch = 0;
    emit searchProgress(100);
    emit searchFinished();
}

void Filter::cancel()
{
    if (isRunning())
    {
        m_break = true;
        wait();
    }
}

void Filter::executeSearch(Search* search)
{
    cancel();
    setAll(0);
    m_break = false;
    currentSearch = search;
    currentSearchOperator = Filter::NullOperator;
    start();
}

void Filter::executeSearch(Search* search, Filter::Operator searchOperator)
{
    if (searchOperator==Filter::NullOperator)
    {
        executeSearch(search);
        return;
    }

    wait();
    m_break = false;
    currentSearch = search;
    currentSearchOperator = searchOperator;
    start();
}

void Filter::executeQuery(Query* query)
{
    QVector <QPair <FilterSearch*, int> > filterSearches;
    QVector <QPair <Search*, int> > searches;
    int filterSearchCount = 0;
    int searchCount = 0;
    QTime t;

    t.start();
    m_gamesSearched = 0;

    TriStateTree triStateTree = TriStateTree(*query);

    /* Make a list of all searches, filter searches separately */
    int leafNode = 0;
    for(int element = 0; element < query->count(); ++element)
    {
        if(query->isElementSearch(element))
        {
            Search* el = query->search(element);
            if(qobject_cast<FilterSearch*>(el))
            {
                filterSearches.append(QPair<FilterSearch*, int> (qobject_cast<FilterSearch*>(el), leafNode));
            }
            else
            {
                searches.append(QPair<Search*, int>(el, leafNode));
            }

            ++leafNode;
        }
    }
    filterSearchCount = (int) filterSearches.size();
    searchCount = (int) searches.size();

    /* Iterates through all games in the database.
     * Don't worry, a search won't be performed unless necessary */
    for(int searchIndex = 0; searchIndex < size(); ++searchIndex)
    {
        triStateTree.clear();

        /* Add filter searches to tree. This could solve the tree in certain cases
         * making it unecessary to call searchGame() */
        if(filterSearchCount)
        {
            for(int search = 0; search < filterSearchCount; ++search)
            {
                if(triStateTree.setState(filterSearches.at(search).second,
                                           filterSearches.at(search).first->contains(searchIndex)))
                {
                    /* This means the tree evaluated to true */
                    break;
                }
            }
            /* So if the filter(s) wasn't enough to solve the tree,
             * lets see what we can find in the game */
            if(triStateTree.state() == TriStateTree::Unknown)
            {
                for(int search = 0; search < searchCount; ++search)
                {
                    if(triStateTree.setState(searches.at(search).second,
                                               searches.at(search).first->matches(searchIndex)))
                    {
                        break;
                    }
                }
                ++m_gamesSearched;
            }
        }
        else
        {
            /* If the query is not combined with this or any other filter
             * there is no way the tree could have been solved, so just check the game */
            for(int search = 0; search < searchCount; ++search)
            {
                if(triStateTree.setState(searches.at(search).second,
                                           searches.at(search).first->matches(searchIndex)))
                {
                    break;
                }
            }
            ++m_gamesSearched;
        }

        /* Update the filter with the result of the tree */
        set(searchIndex, triStateTree.state() == TriStateTree::True);
    }
    m_searchTime = t.elapsed();

}

int Filter::gamesSearched() const
{
    return m_gamesSearched;
}

int Filter::searchTime() const
{
    return m_searchTime;
}
