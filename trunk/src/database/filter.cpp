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
    resetCache();
    m_gamesSearched = 0;
    m_searchTime = 0;
    currentSearchOperator = NullOperator;
    m_break = false;
    m_lock = 0;
}

Filter::~Filter()
{
    cancel();
    delete currentSearch;
    delete m_vector;
}

Filter::Filter(Filter const& rhs) : QThread()
{
    *this = rhs;
}

void Filter::resetCache()
{
    m_cache.first = -1;
    m_cache.second = InvalidGameId;
}

Filter& Filter::operator= (Filter const& rhs)
{
    m_database = rhs.m_database;
    m_count = rhs.m_count;
    m_vector = new QVector<int>(*rhs.m_vector);
    resetCache();
    m_gamesSearched = 0;
    m_searchTime = 0;
    currentSearch = 0;
    currentSearchOperator = NullOperator;
    m_break = false;
    m_lock = 0;
    return *this;
}

const Database* Filter::database() const
{
    return m_database;
}

void Filter::lock(Filter* locked)
{
   m_lock = locked;
}

Database* Filter::database()
{
    return m_database;
}

void Filter::set(GameId game, int value)
{
    if((game >= size()) || (gamePosition(game) == value))
    {
        return;
    }
    if(value && !contains(game))
    {
        resetCache();
        m_count++;
    }
    else if(!value && contains(game))
    {
        resetCache();
        m_count--;
    }

    (*m_vector)[game] = value;
}

void Filter::setAll(int value)
{
    cancel();
    m_vector->fill(value);
    m_count = value ? size() : 0;
    resetCache();
}

bool Filter::contains(GameId game) const
{
    if(game < m_vector->count())
    {
        return (m_vector->at(game) != 0);
    }
    return false;
}

int Filter::gamePosition(GameId game) const
{
    return m_vector->at(static_cast<int>(game));
}

unsigned int Filter::size() const
{
    return static_cast<unsigned int>(m_vector->size());
}

int Filter::gameToIndex(GameId index)
{
    if(!contains(index))
    {
        return -1;
    }
    if (m_count == size())
    {
        return static_cast<int>(index);
    }
    if(index < size() / 2)
    {
        int count = 0;
        for(GameId i = 0; i < index; ++i)
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
        for(GameId i = size() - 1 ; i > index; --i)
        {
            if(contains(i))
            {
                --count;
            }
        }
        return count;
    }
}

GameId Filter::indexToGame(int index)
{
    if(m_cache.first == index)
    {
        return m_cache.second;
    }
    if (m_count == size())
    {
        return static_cast<GameId>(index);
    }
    m_cache.first = index;
    if(index < m_count)
    {
        if(index < m_count / 2)
        {
            GameId start = 0;
            if (VALID_INDEX(m_cache.second))
            {
                if (index>m_cache.first)
                {
                    start = m_cache.second;
                    index -= m_cache.first;
                }
            }
            for(GameId i = start; i < static_cast<GameId>(size()); ++i)
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
            GameId start = static_cast<GameId>(size()) - 1;
            if (VALID_INDEX(m_cache.second))
            {
                if (index>m_cache.first)
                {
                    start = m_cache.second;
                    index += m_cache.first;
                }
            }
            for(GameId i = start ; VALID_INDEX(i); --i)
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
    resetCache();
    return InvalidGameId;
}

GameId Filter::previousGame(GameId current) const
{
    if(m_count)
    {
        for(GameId i = qBound(InvalidGameId, current, size()) - 1; VALID_INDEX(i); --i)
        {
            if(contains(i))
            {
                return i;
            }
        }
    }
    return InvalidGameId;
}

GameId Filter::nextGame(GameId current) const
{
    if(m_count)
    {
        for(GameId i = qBound(InvalidGameId, current, size()) + 1; i < size(); ++i)
        {
            if(contains(i))
            {
                return i;
            }
        }
    }
    return InvalidGameId;
}

void Filter::resize(int newsize, bool includeNew)
{
    for(GameId i = static_cast<GameId>(newsize); i < size(); ++i)   // Decrease count by number of removed games
    {
        if(contains(i))
        {
            --m_count;
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
    resetCache();
}

void Filter::invert()
{
    cancel();
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
    resetCache();
}

void Filter::runSingleSearch(Search* s, FilterOperator op)
{
    connect(s, SIGNAL(prepareUpdate(int)), this, SIGNAL(searchProgress(int)));
    s->Prepare(m_break);
    switch (op)
    {
    case FilterOperator::NullOperator:
        for(int searchIndex = 0; searchIndex < size(); ++searchIndex)
        {
            if (m_break) break;
            set(searchIndex, s->matches(searchIndex));
            if (searchIndex % 1024 == 0) emit searchProgress(searchIndex*100/size());
        }
        break;
    case FilterOperator::And:
        for(int searchIndex = 0; searchIndex < size(); ++searchIndex)
        {
            if (m_break) break;
            if (contains(searchIndex))
            {
                set(searchIndex, s->matches(searchIndex));
            }
            if (searchIndex % 1024 == 0) emit searchProgress(searchIndex*100/size());
        }
        break;
    case FilterOperator::Or:
        for(int searchIndex = 0; searchIndex < size(); ++searchIndex)
        {
            if (m_break) break;
            if (!contains(searchIndex))
            {
                set(searchIndex, s->matches(searchIndex));
            }
            if (searchIndex % 1024 == 0) emit searchProgress(searchIndex*100/size());
        }
        break;
    case FilterOperator::Remove:
        for(int searchIndex = 0; searchIndex < size(); ++searchIndex)
        {
            if (m_break) break;
            if (contains(searchIndex))
            {
                set(searchIndex, !s->matches(searchIndex));
            }
            if (searchIndex % 1024 == 0) emit searchProgress(searchIndex*100/size());
        }
        break;
    default:
        break;
    }
}

void Filter::run()
{
    Search* s = currentSearch;
    FilterOperator op = currentSearchOperator;
    while(s)
    {
        runSingleSearch(s, op);
        op = s->searchOperator();
        s = s->nextSearch();
    }
    delete currentSearch;
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
    if (m_lock && m_lock->isRunning())
    {
        m_lock->wait();
    }
}

void Filter::executeSearch(Search* search, FilterOperator searchOperator)
{
    if (searchOperator == FilterOperator::NullOperator)
    {
        setAll(0);
    }
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
