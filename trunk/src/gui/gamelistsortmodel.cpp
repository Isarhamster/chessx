/****************************************************************************
*   Copyright (C) 2019 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "filter.h"
#include "gamelistsortmodel.h"

bool GameListSortModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (m_filter && m_filter->gamePosition(source_row))
    {
        return true;
    }
    return false;
}

void GameListSortModel::setFilter(Filter* filter)
{
    m_filter = filter;
}
