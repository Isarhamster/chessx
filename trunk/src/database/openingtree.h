/***************************************************************************
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __OPENINGTREE_H__
#define __OPENINGTREE_H__

#include "movedata.h"

#include <QAbstractTableModel>
#include <QPointer>

class Filter;
class OpeningTreeThread;

/** @ingroup Search
The OpeningTree class is a class to calculate opening tree for given position. */

class OpeningTree : public QAbstractTableModel
{
    Q_OBJECT
public:
    /** Empty constructor. */
    OpeningTree(QObject* parent);
    ~OpeningTree();
    /** Calculate opening tree from given position, using given filter. It sets
    the filter to contain only game matching position @p b .
    @return true if the update was not cancelled.*/
    bool updateFilter(Filter& f, const Board& b, bool updateFilter, bool sourceIsFilter, bool bEnd);
    /** Returns the number of moves in the Opening Tree */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    /** Returns the number of columns of the Opening Tree */
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    /** Returns an item of data given the item 'index' */
    virtual QVariant data(const QModelIndex &index, int role) const;
    /** Returns the header information for header 'section' */
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    /** Support sorting. */
    virtual void sort(int column, Qt::SortOrder order);
    /** Support sorting. */
    virtual void sort();
    /** Do the sorting under a lock */
    void doSort(int column, Qt::SortOrder order);
    /** Move leading to given entry. */
    QString move(const QModelIndex& index) const;
    /** Current Board */
    Board board() const;

    Filter *filter() const;
    bool bEnd() const;

public slots:
    /** Cancel a running update */
    void cancel();
protected slots:
    void updateFinished(Board*);
    void updateTerminated(Board*);
    void moveUpdated(Board* b, QList<MoveData> moveList);
signals:
    void progress(int);
    void openingTreeUpdated();
    void openingTreeUpdateStarted();
protected:
    QPixmap paintPercentage(int percentage) const;
private:
    bool m_bRequestPending;
    QList<MoveData> m_moves;
    unsigned int m_games;
    QStringList m_names;
    int m_sortcolumn;
    Qt::SortOrder m_order;
    Board m_board;
    QPointer<Filter> m_filter;
    bool m_updateFilter;
    bool m_sourceIsDatabase;
    bool m_bEnd;

    OpeningTreeThread& oupd;
};

#endif

