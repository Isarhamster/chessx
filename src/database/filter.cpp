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
#include <QtDebug>

Filter::Filter(Database* database)
{
	m_database = database;
	m_count = m_database->count();
	m_vector = new QVector<int>(m_count, 1);
    m_cache.first = m_cache.second = -1;
	m_gamesSearched = 0;
	m_searchTime = 0;
}

Filter::Filter(const Filter& filter)
{
	m_vector = new QVector<int>(filter.intVector());
	m_count = filter.m_count;
	m_cache.first = m_cache.second = 0;
	m_gamesSearched = 0;
	m_searchTime = 0;
}

Filter& Filter::operator=(const Filter & filter)
{
	if (this != &filter)
	{
		delete m_vector;
		m_vector = new QVector<int>(filter.intVector());
		m_count = filter.count();
		m_cache = filter.m_cache;
		m_gamesSearched = 0;
		m_searchTime = 0;
	}
	return *this;
}

Filter::~Filter()
{
	delete m_vector;
}

Database* Filter::database()
{
	return m_database;
}

void Filter::set(int game, int value)
{
	if ((game >= size()) || (gamePosition(game) == value)) {
		return;
	}
	if (value && !contains(game)) {
		m_count++;
	} else if (!value && contains(game)) {
		m_count--;
	}
	(*m_vector)[game] = value;
}

void Filter::setAll(int value)
{
	m_vector->fill(value);
	m_count = value ? size() : 0;
    m_cache.first = m_cache.second = -1;
}

bool Filter::contains(int game) const
{
    if (game < m_vector->count())
    {
        return (m_vector->at(game) != 0);
    }
    return false;
}

int Filter::gamePosition(int game) const
{
    return m_vector->at(game);
}

int Filter::count() const
{
	return m_count;
}

int Filter::size() const
{
	return m_vector->size();
}

int Filter::gameToIndex(int index)
{
	if (index > size() || index < 0 || !contains(index)) return -1;
	if (index < size() / 2) {
		int count = 0;
        for (int i = 0; i < index; ++i)
            if (contains(i)) ++count;
		return count;
	} else {
		int count = m_count - 1;
		for (int i = size() - 1 ; i > index; i--)
			if (contains(i)) count--;
		return count;
	}
}

int Filter::indexToGame(int index)
{
    if (m_cache.first == index) return m_cache.second;
    m_cache.first = index;
    if (index < m_count)
    {
        if (index < m_count / 2)
            for (int i = 0; i < size(); ++i) {
                index -= contains(i);
                if (index < 0)
                {
                    m_cache.second = i;
                    return i;
                }
            }
        else
            for (int i = size() - 1 ; i >= 0; i--) {
                index += contains(i);
                if (index >= m_count)
                {
                    m_cache.second = i;
                    return i;
                }
            }
    }
    m_cache.second = -1;
	return -1;
}

int Filter::previousGame(int current) const
{
	if (!m_count) return -1;
	for (int i = qBound(-1, current, size()) - 1; i >= 0; i--)
		if (contains(i)) return i;
	return -1;
}

int Filter::nextGame(int current) const
{
	if (!m_count) return -1;
    for (int i = qBound(-1, current, size()) + 1; i < size(); ++i)
		if (contains(i)) return i;
	return -1;
}

void Filter::resize(int newsize, bool includeNew)
{
    for (int i = newsize; i < size(); ++i)  // Decrease count by number of removed games
		if (contains(i))
			m_count--;
	int oldsize = size();
	m_vector->resize(newsize);
	// Set new (uninitialized games) to 'includeNew' value.
	for (int i = oldsize; i < newsize; ++i)
		(*m_vector)[i] = includeNew;
	if (includeNew)
		m_count += newsize - oldsize;
}

void Filter::reverse()
{
	m_count = size() - m_count;
    for (int i = 0; i < size(); ++i) {
		if (m_vector->at(i)) {
			(*m_vector)[i] = 0;
		} else {
			(*m_vector)[i] = 1;
		}
	}

}

const bool ops[4][2][2] = { { {0, 0}, {0, 1}} /* And */, {{0, 1}, {1, 1}} /* Or */,
	{{0, 1}, {1, 0}} /* Xor */, {{0, 0}, {1, 0}} /* Minus */
};


void Filter::join(const Filter& filter, Operator op)
{
	if (filter.size() != size())
		return;
	m_count = 0;
    for (int i = 0; i < size(); ++i) {
		(*m_vector)[i] = ops[op][contains(i)][filter.contains(i)];
		m_count += contains(i);
	}
}

QVector<int> Filter::intVector() const
{
	return *m_vector;
}

void Filter::executeSearch(Search& search)
{
    for (int searchIndex = 0; searchIndex < size(); ++searchIndex) {
		set(searchIndex, search.matches(searchIndex));
	}
}
void Filter::executeSearch(Search& search, Search::Operator searchOperator)
{
    for (int searchIndex = 0; searchIndex < size(); ++searchIndex) {
		if ((searchOperator == Search::And) && contains(searchIndex)) {
			set(searchIndex, search.matches(searchIndex));
		}
		if ((searchOperator == Search::Or) && !contains(searchIndex)) {
			set(searchIndex, search.matches(searchIndex));
		}
	}
}

void Filter::executeQuery(Query& query)
{
	QVector <QPair <FilterSearch, int> > filterSearches;
	QVector <QPair <Search*, int> > searches;
	int filterSearchCount = 0;
	int searchCount = 0;
	QTime t;

	t.start();
	m_gamesSearched = 0;

	m_triStateTree = TriStateTree(query);

	/* Make a list of all searches, filter searches separately */
	int leafNode = 0;
    for (int element = 0; element < query.count(); ++element) {
		if (query.isElementSearch(element)) {
			if (query.search(element)->type() == Search::FilterSearch) {
				filterSearches.append(QPair < FilterSearch,
								int > (*static_cast < const FilterSearch * >(query.search(element)), leafNode));
			} else {
				searches.append(QPair < Search*, int >(query.search(element), leafNode));
			}

            ++leafNode;
		}
	}
	filterSearchCount = (int) filterSearches.size();
	searchCount = (int) searches.size();

	/* Iterates through all games in the database.
	 * Don't worry, a search won't be performed unless necessary */
    for (int searchIndex = 0; searchIndex < size(); ++searchIndex) {
		m_triStateTree.clear();

		/* Add filter searches to tree. This could solve the tree in certain cases
		 * making it unecessary to call searchGame() */
		if (filterSearchCount) {
            for (int search = 0; search < filterSearchCount; ++search) {
				if (m_triStateTree.setState(filterSearches.at(search).second,
								 filterSearches.at(search).first.contains(searchIndex))) {
					/* This means the tree evaluated to true */
					break;
				}
			}
			/* So if the filter(s) wasn't enough to solve the tree,
			 * lets see what we can find in the game */
			if (m_triStateTree.state() == TriStateTree::Unknown) {
                for (int search = 0; search < searchCount; ++search) {
					if (m_triStateTree.setState(searches.at(search).second,
									 searches.at(search).first->matches(searchIndex))) {
						break;
					}
				}
                ++m_gamesSearched;
			}
		} else {
			/* If the query is not combined with this or any other filter
			 * there is no way the tree could have been solved, so just check the game */
            for (int search = 0; search < searchCount; ++search) {
				if (m_triStateTree.setState(searches.at(search).second,
								 searches.at(search).first->matches(searchIndex))) {
					break;
				}
			}
            ++m_gamesSearched;
		}

		/* Update the filter with the result of the tree */
		set(searchIndex, m_triStateTree.state() == TriStateTree::True);
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
