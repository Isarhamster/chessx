/***************************************************************************
								  gamelist.cpp  -  Game List window
									  -------------------
	 begin                : Sun 23 Jul 2006
	 copyright            : (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "database.h"
#include "filtermodel.h"
#include "game.h"
#include "gamelist.h"
#include "GameMimeData.h"
#include "quicksearch.h"
#include "search.h"
#include "settings.h"

#include <QDrag>
#include <QHeaderView>
#include <QMenu>
#include <QPixmap>

GameList::GameList(Filter* filter, QWidget* parent) : TableView(parent)
{
    setObjectName("GameList");
    setWindowTitle(tr("Game list"));
    //QSortFilterProxyModel* sortModel = new QSortFilterProxyModel(this);
    m_model = new FilterModel(filter, this);
    //sortModel->setSourceModel(m_model);
    //setModel(sortModel);
    setModel(m_model);
    connect(this, SIGNAL(clicked(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
    connect(this, SIGNAL(activated(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(slotContextMenu(const QPoint&)));
    connect(selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            this, SLOT(slotItemSelected(QModelIndex)));
#if QT_VERSION < 0x050000
    horizontalHeader()->setClickable(true);
#else
    horizontalHeader()->setSectionsClickable(true);
#endif
    connect(horizontalHeader(), SIGNAL(sectionClicked(int)), SLOT(simpleSearch(int)));

    slotReconfigure();

    setSortingEnabled(false);
    setDragEnabled(true);
}


GameList::~GameList()
{
    delete m_model;
}

void GameList::slotItemSelected(const QModelIndex& index)
{
    m_index = index;
    scrollTo(index, EnsureVisible);
}

void GameList::itemSelected(const QModelIndex& index)
{
    emit selected(m_model->filter()->indexToGame(index.row()));
}

void GameList::setFilter(Filter* filter)
{
    m_model->setFilter(filter);
    emit raiseRequest();
}

void GameList::slotContextMenu(const QPoint& pos)
{
    QModelIndex cell = indexAt(pos);
    m_index = cell;
    QModelIndexList selection = selectedIndexes();
    // Make sure the right click occured on a cell!
    if(cell.isValid() && selection.contains(cell))
    {
        QMenu menu(tr("Game list"), this);
        menu.addAction(tr("Copy games..."), this, SLOT(slotCopyGame()));
        QMenu* mergeMenu = menu.addMenu(tr("Merge into current game"));
        mergeMenu->addAction(tr("All Games"), this, SLOT(slotMergeAllGames()));
        mergeMenu->addAction(tr("Filter"), this, SLOT(slotMergeFilter()));
        mergeMenu->addAction(tr("Selected game"), this, SLOT(slotMergeGame()));
        menu.addSeparator();
        QAction* deleteAction = menu.addAction(tr("Delete game"), this, SLOT(slotDeleteGame()));
        deleteAction->setCheckable(true);
        deleteAction->setEnabled(!m_model->filter()->database()->isReadOnly());
        int n = m_model->filter()->indexToGame(cell.row());
        deleteAction->setChecked(m_model->filter()->database()->deleted(n));
        menu.exec(mapToGlobal(pos));
    }
}

void GameList::simpleSearch(int tagid)
{
    //if (QApplication::queryKeyboardModifiers() & Qt::ShiftModifier)
    //{
    //    sortByColumn(tagid);
    //    return;
    //}
    QuickSearchDialog dlg(this);

    dlg.setTag(tagid);
    if(m_model->filter()->count() <= 1)
    {
        dlg.setMode(1);
    }
    if(dlg.exec() != QDialog::Accepted)
    {
        return;
    }

    QString tag = m_model->GetColumnTags().at(dlg.tag());
    QString value = dlg.value();
    if(value.isEmpty())
    {
        m_model->filter()->setAll(1);
    }
    else if((dlg.tag() == 0) || (dlg.tag() == 7) || (dlg.tag() == 11))
    {
        // filter by number
        Search* ns = new NumberSearch(m_model->filter()->database(), value);
        if(dlg.mode())
        {
            m_model->filter()->executeSearch(ns, Search::Operator(dlg.mode()));
        }
        else
        {
            m_model->filter()->executeSearch(ns);
        }
    }
    else
    {
        QStringList list = value.split("-", QString::SkipEmptyParts);
        if ((list.size() > 1) && (dlg.tag() != 9))
        {
            // Filter a range
            Search* ts = new TagSearch(m_model->filter()->database(), tag, list.at(0), list.at(1));
            if(dlg.mode())
            {
                m_model->filter()->executeSearch(ts, Search::Operator(dlg.mode()));
            }
            else
            {
                m_model->filter()->executeSearch(ts);
            }
        }
        else
        {
            // Filter tag using partial values
            Search* ts = new TagSearch(m_model->filter()->database(), tag, value);
            if(dlg.mode())
            {
                m_model->filter()->executeSearch(ts, Search::Operator(dlg.mode()));
            }
            else
            {
                m_model->filter()->executeSearch(ts);
            }
        }
    }
}

void GameList::slotFilterListByPlayer(QString s)
{
    m_model->filter()->setAll(1);
    Search* ts = new TagSearch(m_model->filter()->database(),  TagNameWhite, s);
    Search* ts2 = new TagSearch(m_model->filter()->database(), TagNameBlack, s);
    m_model->filter()->executeSearch(ts);
    m_model->filter()->executeSearch(ts2, Search::Or);
}

void GameList::slotFilterListByEcoPlayer(QString tag, QString eco, QString player)
{
    m_model->filter()->setAll(1);
    Search* ts = new TagSearch(m_model->filter()->database(),  tag, player);
    Search* ts3 = new TagSearch(m_model->filter()->database(), TagNameECO,   eco);
    m_model->filter()->executeSearch(ts);
    m_model->filter()->executeSearch(ts3, Search::And);
}

void GameList::slotFilterListByEvent(QString s)
{
    m_model->filter()->setAll(1);
    Search* ts = new TagSearch(m_model->filter()->database(), TagNameEvent, s);
    m_model->filter()->executeSearch(ts);
}

void GameList::slotFilterListByEventPlayer(QString player, QString event)
{
    m_model->filter()->setAll(1);
    Search* ts = new TagSearch(m_model->filter()->database(),  TagNameWhite, player);
    Search* ts2 = new TagSearch(m_model->filter()->database(), TagNameBlack, player);
    Search* ts3 = new TagSearch(m_model->filter()->database(), TagNameEvent, event);
    m_model->filter()->executeSearch(ts);
    m_model->filter()->executeSearch(ts2, Search::Or);
    m_model->filter()->executeSearch(ts3, Search::And);
}

void GameList::slotFilterListByEco(QString s)
{
    m_model->filter()->setAll(1);
    Search* ts = new TagSearch(m_model->filter()->database(), TagNameECO, s);
    m_model->filter()->executeSearch(ts);
}

void GameList::selectGame(int index)
{
    int i = m_model->filter()->gameToIndex(index);
    if(i != -1)
    {
        setCurrentIndex(m_model->index(i, 0));
        selectRow(i);
    }
}

void GameList::updateFilter()
{
    m_model->setFilter(m_model->filter());
    emit raiseRequest();
}

void GameList::slotCopyGame()
{
    emit requestCopyGame(m_model->filter()->indexToGame(m_index.row()));
}

void GameList::slotMergeAllGames()
{
    emit requestMergeAllGames();
}

void GameList::slotMergeFilter()
{
    emit requestMergeFilter();
}

void GameList::slotMergeGame()
{
    emit requestMergeGame(m_model->filter()->indexToGame(m_index.row()));
}

void GameList::slotDeleteGame()
{
    emit requestDeleteGame(m_model->filter()->indexToGame(m_index.row()));
}

void GameList::startToDrag(const QModelIndex& index)
{
    GameMimeData *mimeData = new GameMimeData;
    mimeData->m_index = m_model->filter()->indexToGame(index.row());
    QPixmap pixmap = style()->standardPixmap(QStyle::SP_FileIcon);

    QDrag* pDrag = new QDrag(this);
    pDrag->setMimeData(mimeData);
    pDrag->setPixmap(pixmap);
    // pDrag->setHotSpot(hotSpot);

    pDrag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
}

void GameList::startDrag(Qt::DropActions /*supportedActions*/)
{
    startToDrag(m_index);
}
