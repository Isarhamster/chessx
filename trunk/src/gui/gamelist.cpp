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
#include "filter.h"
#include "filtermodel.h"
#include "gamelist.h"
#include "gamelistsortmodel.h"
#include "GameMimeData.h"
#include "numbersearch.h"
#include "quicksearch.h"
#include "settings.h"
#include "tags.h"
#include "tagsearch.h"

#include "game.h"
#include "output.h"

#include <qevent.h>
#include <QDrag>
#include <QHeaderView>
#include <QMenu>
#include <QPixmap>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

GameList::GameList(Filter* filter, QWidget* parent) : TableView(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setObjectName("GameList");
    setWindowTitle(tr("Game list"));
    m_model = new FilterModel(filter);
    connect(m_model, SIGNAL(searchProgress(int)), SIGNAL(searchProgress(int)));
    connect(m_model, SIGNAL(searchFinished()), SIGNAL(searchFinished()));

    sortModel = new GameListSortModel(0);
    sortModel->setFilter(filter);
    sortModel->setSourceModel(m_model);
    sortModel->setDynamicSortFilter(true);
    setModel(sortModel);

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

    slotReconfigure();

    horizontalHeader()->setSortIndicatorShown(true);
    setSortingEnabled(true);

    setDragEnabled(true);
    setAcceptDrops(true);
}


GameList::~GameList()
{
    setModel(0);
    delete sortModel;
    delete m_model;
}

void GameList::startUpdate()
{
    if (m_model)
    {
        m_model->startUpdate();
    }
}

void GameList::endUpdate()
{
    if (m_model)
    {
        m_model->endUpdate();
    }
}

void GameList::slotReconfigure()
{
    if (m_model)
    {
        m_model->updateColumns();
    }
    TableView::slotReconfigure();
}

void GameList::ShowContextMenu(const QPoint& pos)
{
    QMenu headerMenu;
    QAction* filterTag = headerMenu.addAction(tr("Find tag..."));
    headerMenu.addSeparator();
    QAction* hide = headerMenu.addAction(tr("Hide Column"));
    headerMenu.addSeparator();
    QAction* resizeAll = headerMenu.addAction(tr("Resize visible Columns"));
    QAction* showAll = headerMenu.addAction(tr("Show all Columns"));

    QAction* selectedItem = headerMenu.exec(mapToGlobal(pos));
    int column = columnAt(pos.x());
    if (selectedItem == filterTag)
    {
        simpleSearch(column);
    }
    else if(selectedItem == hide)
    {
        if(column > 0)
        {
            hideColumn(column);
        }
    }
    else if(selectedItem == showAll)
    {
        for(int i = 0; i < model()->columnCount(); ++i)
        {
            showColumn(i);
        }
    }
    else if (selectedItem == resizeAll)
    {
        resizeColumnsToContents();
    }
}

void GameList::removeSelection()
{
    clearSelection();
    emit signalFirstGameLoaded(true);
    emit signalLastGameLoaded(true);
}

void GameList::slotItemSelected(const QModelIndex& index)
{
    scrollTo(index, EnsureVisible);
}

void GameList::filterInvert()
{
    m_model->invert();
}

void GameList::filterSetAll(int value)
{
    m_model->setAll(value);
}

QModelIndex GameList::NewSortIndex(int row) const
{
    QModelIndex m = sortModel->index(row,0);
    return m;
}

QModelIndex GameList::GetSourceIndex(const QModelIndex& index) const
{
    QModelIndex m = sortModel->mapToSource(index);
    return m;
}

QModelIndex GameList::GetSortModelIndex(const QModelIndex& index) const
{
    QModelIndex m = sortModel->mapFromSource(index);
    return m;
}

void GameList::itemSelected(const QModelIndex& index)
{
    if (selectionModel()->selectedRows().size() == 1)
    {
        QModelIndex m = GetSourceIndex(index);
        GameId n = m.row();
        if (VALID_INDEX(n))
        {
            emit gameSelected(n);
        }
    }
}

bool GameList::triggerGameSelection(int sortRow)
{
    QModelIndex sortIndex = NewSortIndex(sortRow);
    QModelIndex sourceIndex = GetSourceIndex(sortIndex);
    GameId game = sourceIndex.row();
    if (VALID_INDEX(game))
    {
        emit gameSelected(sourceIndex.row());
        return true;
    }
    return false;
}

void GameList::selectPreviousGame()
{
    QModelIndex sortIndex = currentIndex();
    int oldRow = sortIndex.row();
    int row = std::max(0, sortIndex.row()-1);
    if (row != oldRow)
    {
        triggerGameSelection(row);
    }
}

bool GameList::selectNextGame()
{
    QModelIndex sortIndex = currentIndex();
    int oldRow = sortIndex.row();
    int row = std::min(m_model->filter()->count()-1, oldRow+1);
    if (row != oldRow)
    {
        return triggerGameSelection(row);
    }
    return false;
}

void GameList::selectRandomGame()
{
    if(m_model->filter()->count()>1)
    {
        QModelIndex sortIndex = currentIndex();
        int oldRow = sortIndex.row();
        int randomSortRow = rand() % (m_model->filter()->count()-1); // The last game is represented by current game
        if (oldRow == randomSortRow)
        {
            randomSortRow = m_model->filter()->count()-1;
        }
        QModelIndex sourceIndex = GetSourceIndex(NewSortIndex(randomSortRow));
        GameId game = sourceIndex.row();
        if (VALID_INDEX(game))
        {
            emit gameSelected(game);
        }
    }
}

void GameList::setFilter(Filter* filter)
{
    QByteArray state = horizontalHeader()->saveState();

    if (filter)
    {
        sortModel->setFilter(filter);
        m_model->setFilter(filter);
    }

    horizontalHeader()->restoreState(state);
    emit raiseRequest();
}

void GameList::slotContextMenu(const QPoint& pos)
{
    QModelIndex cell = indexAt(pos);
    QModelIndexList selection = selectedIndexes();
    QMenu menu(tr("Game list"), this);
    if(cell.isValid() && selection.contains(cell))
    {
        // Right click occured on a cell!
        menu.addAction(tr("Copy games..."), this, SLOT(slotCopyGame()));
        menu.addAction(tr("Filter twins"), this, SLOT(slotFindDuplicate()));
        QMenu* mergeMenu = menu.addMenu(tr("Merge into current game"));
        mergeMenu->addAction(tr("All Games"), this, SLOT(slotMergeAllGames()));
        mergeMenu->addAction(tr("Filter"), this, SLOT(slotMergeFilter()));
        mergeMenu->addAction(tr("Selected games"), this, SLOT(slotMergeGame()));
        menu.addSeparator();

        int deleted = 0;
        int activated = 0;
        QModelIndexList list = selectionModel()->selectedRows();
        foreach(QModelIndex index, list)
        {
            QModelIndex source = GetSourceIndex(index);
            GameId n = source.row();
            if (VALID_INDEX(n))
            {
                if (m_model->filter()->database()->deleted(n))
                {
                   ++deleted;
                }
                else
                {
                    ++activated;
                }
            }
            if (activated && deleted)
            {
                break;
            }
        }

        QAction* deleteAction;
        if (activated && deleted)
        {
            deleteAction = menu.addAction(tr("Toggle deletions"), this, SLOT(slotDeleteGame()));
        }
        else
        {
            QString deleteText;
            if (deleted)
            {
                deleteText = selection.count()>1 ? tr("Undelete games") : tr("Undelete game");
            }
            else
            {
                deleteText = selection.count()>1 ? tr("Delete games") : tr("Delete game");
            }
            deleteAction = menu.addAction(deleteText, this, SLOT(slotDeleteGame()));
        }
        deleteAction->setEnabled(!m_model->filter()->database()->isReadOnly());

        menu.addSeparator();
        QString hideText = selection.count()>1 ? tr("Hide games") : tr("Hide game");
        menu.addAction(hideText, this, SLOT(slotHideGame()));
        menu.addAction(tr("Hide deleted games"), this, SLOT(slotHideDeletedGames()));
    }
    menu.addAction(tr("Reset filter"), this, SLOT(filterSetAll()));
    menu.addAction(tr("Reverse filter"), this, SLOT(filterInvert()));
    menu.exec(mapToGlobal(pos));
}

void GameList::executeSearch(Search* search, FilterOperator searchOperator)
{
    m_model->executeSearch(search, searchOperator, 0);
}

void GameList::simpleSearch(int tagid)
{
    QuickSearchDialog dlg(this);
    for (int section = 0; section < m_model->columnCount(); ++section)
    {
        QString tag = m_model->headerData(section, Qt::Horizontal).toString();
        dlg.addTag(tag);
    }

    dlg.setTag(tagid);
    dlg.setMode(1);

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
    else if(dlg.tag() == 0)
    {
        // filter by number
        Search* ns = new NumberSearch(m_model->filter()->database(), value);
        m_model->executeSearch(ns, FilterOperator(dlg.mode()));
    }
    else
    {
        QStringList list = value.split("-", QString::SkipEmptyParts);
        if ((list.size() > 1) && (dlg.tag() != 9)) // Tag 9 is the Result
        {
            // Filter a range
            Search* ts = (dlg.tag() == 11) ? // Tag 11 is number of moves
                    new TagSearch(m_model->filter()->database(), tag, list.at(0).toInt(), list.at(1).toInt()) :
                    new TagSearch(m_model->filter()->database(), tag, list.at(0), list.at(1));
            if(dlg.mode())
            {
                m_model->executeSearch(ts, FilterOperator(dlg.mode()));
            }
            else
            {
                m_model->executeSearch(ts);
            }
        }
        else
        {
            // Filter tag using partial values
            Search* ts = new TagSearch(m_model->filter()->database(), tag, value);
            m_model->executeSearch(ts, FilterOperator(dlg.mode()));
        }
    }
    emit raiseRequest();
}

void GameList::slotFilterListByPlayer(QString s)
{
    QUrl url(s);
    QString fragment = url.fragment();
    m_model->filter()->setAll(1);

    if (fragment.isEmpty())
    {
        Search* ts = new TagSearch(m_model->filter()->database(),  TagNameWhite, url.path());
        Search* ts2 = new TagSearch(m_model->filter()->database(), TagNameBlack, url.path());
        ts->AddSearch(ts2, FilterOperator::Or);
        m_model->executeSearch(ts);
    }
    else
    {
        Search* ts = new TagSearch(m_model->filter()->database(),  fragment, url.path());
        m_model->executeSearch(ts);
    }
    emit raiseRequest();
}

void GameList::slotFilterListByEcoPlayer(QString tag, QString eco, QString player)
{
    m_model->filter()->setAll(1); // ??
    Search* ts = new TagSearch(m_model->filter()->database(),  tag, player);
    Search* ts3 = new TagSearch(m_model->filter()->database(), TagNameECO, eco);
    ts->AddSearch(ts3, FilterOperator::And);
    m_model->executeSearch(ts);
    emit raiseRequest();
}

void GameList::slotFilterListByEvent(QString s)
{
    m_model->filter()->setAll(1);
    Search* ts = new TagSearch(m_model->filter()->database(), TagNameEvent, s);
    m_model->executeSearch(ts);
    emit raiseRequest();
}

void GameList::slotFilterListByEventPlayer(QString player, QString event)
{
    m_model->filter()->setAll(1);
    Search* ts = new TagSearch(m_model->filter()->database(),  TagNameWhite, player);
    Search* ts2 = new TagSearch(m_model->filter()->database(), TagNameBlack, player);
    Search* ts3 = new TagSearch(m_model->filter()->database(), TagNameEvent, event);
    ts->AddSearch(ts2, FilterOperator::Or);
    ts2->AddSearch(ts3, FilterOperator::And);
    m_model->executeSearch(ts);
    emit raiseRequest();
}

void GameList::slotFilterListByEco(QString s)
{
    Search* ts = new TagSearch(m_model->filter()->database(), TagNameECO, s);
    m_model->executeSearch(ts, FilterOperator::NullOperator);
}

void GameList::endSearch()
{
    emit raiseRequest();
}

void GameList::selectGame(GameId index)
{
    if (VALID_INDEX(index))
    {
        QModelIndex filterModelIndex = m_model->index(index, 0);
        QModelIndex sortModelIndex = GetSortModelIndex(filterModelIndex);
        setCurrentIndex(sortModelIndex);
        emit signalFirstGameLoaded(sortModelIndex.row()==0);
        emit signalLastGameLoaded(sortModelIndex.row()+1 >= m_model->filter()->count());
    }
    else
    {
        emit signalFirstGameLoaded(true);
        emit signalLastGameLoaded(true);
    }
}

void GameList::updateFilter(GameId index, int value)
{
    if (m_model->filter()->database()) // ?
    {
        if (VALID_INDEX(index))
        {
            m_model->set(index, value);
        }
    }
}

QList<GameId> GameList::selectedGames()
{
    QList<GameId> gameIndexList;
    foreach(QModelIndex index, selectionModel()->selectedRows())
    {
        QModelIndex m = GetSourceIndex(index);
        GameId n = m.row();
        if (VALID_INDEX(n))
        {
            gameIndexList.append(n);
        }
    }
    return gameIndexList;
}

void GameList::slotCopyGame()
{
    QList<GameId> gameIndexList = selectedGames();
    emit requestCopyGame(gameIndexList);
}

void GameList::slotFindDuplicate()
{
    QList<GameId> gameIndexList = selectedGames();
    emit requestFindDuplicates(gameIndexList);
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
    QList<GameId> gameIndexList = selectedGames();
    emit requestMergeGame(gameIndexList);
}

void GameList::slotDeleteGame()
{
    QList<GameId> gameIndexList = selectedGames();
    emit requestDeleteGame(gameIndexList);
}

void GameList::slotHideGame()
{
    QList<GameId> gameIndexList = selectedGames();
    foreach(GameId game, gameIndexList)
    {
        m_model->set(game, 0);
    }
}

void GameList::slotHideDeletedGames()
{
    QList<GameId> gameIndexList = selectedGames();
    foreach(GameId game, gameIndexList)
    {
        if (m_model->filter()->database()->deleted(game))
        {
            m_model->set(game,0);
        }
    }
}

void GameList::startDrag(Qt::DropActions /*supportedActions*/)
{
    GameMimeData *mimeData = new GameMimeData;
    mimeData->source = m_model->filter()->database()->filename();
    foreach(QModelIndex index, selectionModel()->selectedRows())
    {
        QModelIndex m = GetSourceIndex(index);
        int row = m.row();
        GameId gameIndex = row;
        if (VALID_INDEX(gameIndex))
        {
            mimeData->m_indexList.append(gameIndex);
        }
    }

    if (mimeData->m_indexList.count() < 1000) // Avoid excessive size of clipboard
    {
        QString text;
        foreach(GameId gameIndex, mimeData->m_indexList)
        {
            Game g;
            if(m_model->filter()->database()->loadGame(gameIndex, g))
            {
                Output textWriter(Output::Pgn);
                QString pgn = textWriter.output(&g);
                if (!text.isEmpty())
                {
                    text.append("\n");
                }
                text.append(pgn);
                text.append("\n");
            }
        }
        mimeData->setText(text);
    }

    QPixmap pixmap = style()->standardPixmap(QStyle::SP_FileIcon);

    QDrag* pDrag = new QDrag(this);
    pDrag->setMimeData(mimeData);
    pDrag->setPixmap(pixmap);

    pDrag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
}

void GameList::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    const DbMimeData* dbMimeData = qobject_cast<const DbMimeData*>(mimeData);

    if(dbMimeData || (mimeData && mimeData->hasUrls()))
    {
        event->acceptProposedAction();
    }
}

void GameList::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void GameList::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void GameList::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    const DbMimeData* dbMimeData = qobject_cast<const DbMimeData*>(mimeData);

    if(dbMimeData || (mimeData && mimeData->hasUrls()))
    {
        m_model->startUpdate();
        emit signalDropEvent(event);
        m_model->endUpdate();
    }
    else
    {
        event->ignore();
    }
}
