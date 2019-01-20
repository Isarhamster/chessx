/****************************************************************************
*   Copyright (C) 2019 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "filter.h"
#include "gamelistsortmodel.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

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
