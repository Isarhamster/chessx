/****************************************************************************
*   Copyright (C) 2019 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef GAMELISTSORTMODEL_H
#define GAMELISTSORTMODEL_H

#include <QSortFilterProxyModel>

class FilterX;

class GameListSortModel : public QSortFilterProxyModel
{
public:
    explicit GameListSortModel(QObject *parent = nullptr) :
        QSortFilterProxyModel(parent),
        m_filter(nullptr)
    {}
    void setFilter(FilterX* filter);
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
    FilterX* m_filter;
};

#endif // GAMELISTSORTMODEL_H
