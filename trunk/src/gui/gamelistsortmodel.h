/****************************************************************************
*   Copyright (C) 2019 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef GAMELISTSORTMODEL_H
#define GAMELISTSORTMODEL_H

#include <QSortFilterProxyModel>

class Filter;

class GameListSortModel : public QSortFilterProxyModel
{
public:
    explicit GameListSortModel(QObject *parent = 0) :
        QSortFilterProxyModel(parent),
        m_filter(0)
    {}
    void setFilter(Filter* filter);
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
    Filter* m_filter;
};

#endif // GAMELISTSORTMODEL_H
