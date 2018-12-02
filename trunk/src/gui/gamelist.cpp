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

    sortModel = new GameListSortModel(0);
    sortModel->setSourceModel(m_model);
    sortModel->setDynamicSortFilter(false);
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

    updateFilter();
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
    emit signalFirstGameLoaded(m_model->filter()->count()>0);
    emit signalLastGameLoaded(m_model->filter()->count()>0);
}

void GameList::slotItemSelected(const QModelIndex& index)
{
    scrollTo(index, EnsureVisible);
}

void GameList::slotResetFilter()
{
    emit requestResetFilter();
}

void GameList::slotReverseFilter()
{
    emit requestRevertFilter();
}

QModelIndex GameList::NewSortIndex(int row) const
{
    if (sortModel)
    {
        QModelIndex m = sortModel->index(row,0);
        return m;
    }
    return m_model->index(row,0);
}

QModelIndex GameList::GetSourceIndex(const QModelIndex& index) const
{
    if (sortModel)
    {
        QModelIndex m = sortModel->mapToSource(index);
        return m;
    }
    return index;
}

QModelIndex GameList::GetSortModelIndex(const QModelIndex& index) const
{
    if (sortModel)
    {
        QModelIndex m = sortModel->mapFromSource(index);
        return m;
    }
    return index;
}

void GameList::itemSelected(const QModelIndex& index)
{
    if (selectionModel()->selectedRows().size() == 1)
    {
        QModelIndex m = GetSourceIndex(index);
        emit selected(m_model->filter()->indexToGame(m.row()));
    }
}

void GameList::selectPreviousGame()
{
    QModelIndex sortIndex = currentIndex();
    int row = std::max(0, sortIndex.row()-1);
    QModelIndex sourceIndex = GetSourceIndex(NewSortIndex(row));
    emit selected(m_model->filter()->indexToGame(sourceIndex.row()));
}

bool GameList::selectNextGame()
{
    QModelIndex sortIndex = currentIndex();
    int row = sortIndex.row();
    if ((row>=0) && (row + 1 < m_model->filter()->count()))
    {
        QModelIndex sourceIndex = GetSourceIndex(NewSortIndex(row+1));
        emit selected(m_model->filter()->indexToGame(sourceIndex.row()));
        return true;
    }
    return false;
}

void GameList::setFilter(Filter* filter)
{
    setSortingEnabled(false);
    horizontalHeader()->setSortIndicatorShown(false);

    QByteArray state = horizontalHeader()->saveState();

    setModel(0);

    delete m_model;
    delete sortModel;

    if (filter)
    {
        m_model = new FilterModel(filter);
        if (filter->count() > 32768)
        {
            setModel(m_model);
        }
        else
        {
            sortModel = new GameListSortModel(0);
            sortModel->setSourceModel(m_model);
            sortModel->setDynamicSortFilter(false);
            setModel(sortModel);
        }
    }

    horizontalHeader()->restoreState(state);

    if (filter)
    {
        horizontalHeader()->setSortIndicatorShown(!!sortModel);
        setSortingEnabled(!!sortModel);
    }

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
            int n = m_model->filter()->indexToGame(source.row());
            if (m_model->filter()->database()->deleted(n))
            {
               ++deleted;
            }
            else
            {
                ++activated;
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

        menu.exec(mapToGlobal(pos));
    }
    else
    {
        // Right click occured on empty space
        menu.addAction(tr("Reset filter"), this, SLOT(slotResetFilter()));
        menu.addAction(tr("Reverse filter"), this, SLOT(slotReverseFilter()));
    }
    menu.exec(mapToGlobal(pos));
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
    else if(dlg.tag() == 0)
    {
        // filter by number
        Search* ns = new NumberSearch(m_model->filter()->database(), value);
        m_model->filter()->executeSearch(ns, Filter::Operator(dlg.mode()));
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
                m_model->filter()->executeSearch(ts, Filter::Operator(dlg.mode()));
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
            m_model->filter()->executeSearch(ts, Filter::Operator(dlg.mode()));
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
        m_model->filter()->executeSearch(ts);
        m_model->filter()->executeSearch(ts2, Filter::Or);
    }
    else
    {
        Search* ts = new TagSearch(m_model->filter()->database(),  fragment, url.path());
        m_model->filter()->executeSearch(ts);
    }
    emit raiseRequest();
}

void GameList::slotFilterListByEcoPlayer(QString tag, QString eco, QString player)
{
    m_model->filter()->setAll(1);
    Search* ts = new TagSearch(m_model->filter()->database(),  tag, player);
    Search* ts3 = new TagSearch(m_model->filter()->database(), TagNameECO, eco);
    m_model->filter()->executeSearch(ts);
    m_model->filter()->executeSearch(ts3, Filter::And);
    emit raiseRequest();
}

void GameList::slotFilterListByEvent(QString s)
{
    m_model->filter()->setAll(1);
    Search* ts = new TagSearch(m_model->filter()->database(), TagNameEvent, s);
    m_model->filter()->executeSearch(ts);
    emit raiseRequest();
}

void GameList::slotFilterListByEventPlayer(QString player, QString event)
{
    m_model->filter()->setAll(1);
    Search* ts = new TagSearch(m_model->filter()->database(),  TagNameWhite, player);
    Search* ts2 = new TagSearch(m_model->filter()->database(), TagNameBlack, player);
    Search* ts3 = new TagSearch(m_model->filter()->database(), TagNameEvent, event);
    m_model->filter()->executeSearch(ts);
    m_model->filter()->executeSearch(ts2, Filter::Or);
    m_model->filter()->executeSearch(ts3, Filter::And);
    emit raiseRequest();
}

void GameList::slotFilterListByEco(QString s)
{
    m_model->filter()->setAll(1);
    Search* ts = new TagSearch(m_model->filter()->database(), TagNameECO, s);
    m_model->filter()->executeSearch(ts);
    emit raiseRequest();
}

void GameList::selectGame(int index)
{
    if (index >= 0)
    {
        int i = m_model->filter()->gameToIndex(index);
        if(i != -1)
        {
            QModelIndex filterModelIndex = m_model->index(i, 0);
            QModelIndex sortModelIndex = GetSortModelIndex(filterModelIndex);
            setCurrentIndex(sortModelIndex);
            selectRow(sortModelIndex.row());
            emit signalFirstGameLoaded(sortModelIndex.row()==0);
            emit signalLastGameLoaded(sortModelIndex.row()+1 == m_model->filter()->count());
        }
    }
    else
    {
        emit signalFirstGameLoaded(true);
        emit signalLastGameLoaded(true);
    }
}

void GameList::updateFilter()
{
    if (m_model->filter()->database())
    {
        m_model->setFilter(m_model->filter());
        emit raiseRequest();
    }
}

QList<int> GameList::selectedGames()
{
    QList<int> gameIndexList;
    foreach(QModelIndex index, selectionModel()->selectedRows())
    {
        QModelIndex m = GetSourceIndex(index);
        gameIndexList.append(m_model->filter()->indexToGame(m.row()));
    }
    return gameIndexList;
}

void GameList::slotCopyGame()
{
    QList<int> gameIndexList = selectedGames();
    emit requestCopyGame(gameIndexList);
}

void GameList::slotFindDuplicate()
{
    QList<int> gameIndexList = selectedGames();
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
    QList<int> gameIndexList = selectedGames();
    emit requestMergeGame(gameIndexList);
}

void GameList::slotDeleteGame()
{
    QList<int> gameIndexList = selectedGames();
    emit requestDeleteGame(gameIndexList);
}

void GameList::slotHideGame()
{
    QList<int> gameIndexList = selectedGames();
    foreach( int game, gameIndexList )
    {
        m_model->filter()->set(game, 0);
    }

    updateFilter();
}

void GameList::startDrag(Qt::DropActions /*supportedActions*/)
{
    GameMimeData *mimeData = new GameMimeData;
    foreach(QModelIndex index, selectionModel()->selectedRows())
    {
        QModelIndex m = GetSourceIndex(index);
        int row = m.row();
        int gameIndex = m_model->filter()->indexToGame(row);
        mimeData->m_indexList.append(gameIndex);
    }

    if (mimeData->m_indexList.count() < 100) // Avoid excessive size of clipboard
    {
        QString text;

        foreach(int gameIndex, mimeData->m_indexList)
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
        emit signalDropEvent(event);
    }
    else
    {
        event->ignore();
    }
}
