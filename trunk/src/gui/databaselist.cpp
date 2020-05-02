/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "databaseinfo.h"
#include "databaselist.h"
#include "databaselistmodel.h"
#include "exttool.h"
#include "GameMimeData.h"
#include "settings.h"
#include "shellhelper.h"

#include <QFileInfo>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QtGui>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

#ifdef Q_OS_WIN32
#define LINK_MODIFIER Qt::AltModifier
#else // MAC OS
#define LINK_MODIFIER Qt::MetaModifier
#endif

DatabaseList::DatabaseList(QWidget *parent) :
    TableView(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setObjectName("DatabaseList");
    setWindowTitle(tr("Databases"));
    setPosDecoration(QStyleOptionViewItem::Top);
    m_model = new DatabaseListModel(this);

    m_filterModel = new QSortFilterProxyModel(this);
    m_filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_filterModel->setSourceModel(m_model);
    m_filterModel->setDynamicSortFilter(true);
    m_filterModel->sort(1);
    m_filterModel->setSortRole(Qt::UserRole);

    setModel(m_filterModel);

    connect(this, SIGNAL(clicked(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
    connect(this, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(slotDoubleClicked(const QModelIndex&)));
    connect(this, SIGNAL(activated(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(slotContextMenu(const QPoint&)));
    connect(m_model, SIGNAL(OnSelectIndex(const QModelIndex &)), SLOT(slotCurrentIndexChanged(const QModelIndex &)));

    setAlternatingRowColors(true);
    setDragEnabled(true);
    setAcceptDrops(true);

    setColumnWidth(DBLV_FAVORITE, 50);
    setColumnWidth(DBLV_NAME, 200);
    setColumnWidth(DBLV_SIZE, 75);
    setColumnWidth(DBLV_OPEN, 50);
    setColumnWidth(DBLV_PATH, 200);
    setColumnWidth(DBLV_OPEN, 50);
    setColumnWidth(DBLV_DATE, 75);
    setColumnWidth(DBLV_DATE_READ, 75);
    setSortingEnabled(true);

    Q_ASSERT(horizontalHeader()->isSortIndicatorShown());
}

DatabaseList::~DatabaseList()
{
}

void DatabaseList::slotCurrentIndexChanged(const QModelIndex& source)
{
    QModelIndex i = m_filterModel->mapFromSource(source);
    selectRow(i.row());
    showRow(i.row());
}

void DatabaseList::slotContextMenu(const QPoint& pos)
{
    m_cell = indexAt(pos);
    // Make sure the right click occured on a cell!
    if(m_cell.isValid())
    {
        QMenu menu("Database List",this);
        int stars = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_FAVORITE), Qt::UserRole).toString().toInt();
        bool bIsFavorite = stars > 0;

        QString filename = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_PATH), Qt::UserRole).toString();
        bool bHasSuffix = filename.contains(".");
        bool bIsOpen = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_OPEN), Qt::UserRole).toString() == "Open";
        bool bIsBook = DatabaseInfo::IsBook(filename);

        menu.addAction(bIsOpen ? tr("Activate") : tr("Open"), this, SLOT(dbOpen()))->setEnabled(!bIsOpen || !bIsBook);
        menu.addAction(tr("Close"), this, SLOT(dbClose()))->setEnabled(bIsOpen && bHasSuffix);
        menu.addSeparator();
        menu.addAction(tr("Keep file"), this, SLOT(dbKeepFile()))->setEnabled(bHasSuffix && stars != 1);
        menu.addAction(tr("Two star favorite"), this, SLOT(dbAddToFavorites2()))->setEnabled(bHasSuffix && stars != 2);
        menu.addAction(tr("Three star favorite"), this, SLOT(dbAddToFavorites3()))->setEnabled(bHasSuffix && stars != 3);
        menu.addAction(tr("Open at startup"), this, SLOT(dbAddToOpenAtStartup()))->setEnabled(bHasSuffix && stars != 4);
        menu.addAction(tr("Set active at startup"), this, SLOT(dbSetActiveAtStartup()))->setEnabled(bHasSuffix && stars != 5);
        menu.addAction(tr("Remove from Favorites"), this, SLOT(dbRemoveFromFavorites()))->setEnabled(bHasSuffix && bIsFavorite);
        menu.addSeparator();
        menu.addAction(tr("Show in Finder"), this, SLOT(slotShowInFinder()))->setEnabled(bHasSuffix);

        bool enablePolyglot = bIsOpen && !bIsBook;
        menu.addAction(tr("Make a Polyglot book..."), this, SLOT(slotMakeBook()))->setEnabled(enablePolyglot);

        QFileInfo fi(AppSettings->getValue("Tools/Path1").toString());
        QString extTool1 = fi.baseName();
        if (!extTool1.isEmpty())
        {
            menu.addAction(extTool1, this, SLOT(slotExtTool1()))->setEnabled(bHasSuffix && fi.exists());
        }
        menu.addSeparator();
        QAction* action = menu.addAction("UTF8", this, SLOT(dbToggleUTF8()));
        action->setCheckable(true);
        QString utf8 = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_UTF8)).toString();
        bool bUtf8 = (utf8.compare("UTF8") == 0);
        action->setChecked(bUtf8);
        action->setEnabled(bHasSuffix);

        menu.exec(mapToGlobal(pos));
    }
}

void DatabaseList::save() const
{
    AppSettings->beginGroup("Favorites");

    QStringList list;
    m_model->toStringList(list);
    AppSettings->setValue("Files", list);

    QStringList attrList;
    m_model->toAttrStringList(attrList);
    AppSettings->setValue("Attributes", attrList);

    AppSettings->endGroup();

    QList<QVariant> indexList;
    m_model->toIndexList(indexList);
    AppSettings->setValue("Favorites/LastGameIndex", indexList);
}

void DatabaseList::slotDoubleClicked(const QModelIndex& index)
{
    QString ts = m_filterModel->data(m_filterModel->index(index.row(), DBLV_PATH)).toString();
    QString utf8 = m_filterModel->data(m_filterModel->index(index.row(), DBLV_UTF8)).toString();
    bool bUtf8 = (utf8.compare("UTF8") == 0);
    emit requestOpenDatabase(ts, bUtf8);
}

void DatabaseList::itemSelected(const QModelIndex& index)
{
    if(index.column() == 0 || index.column() == DBLV_OPEN)
    {
        slotDoubleClicked(index);
    }
}

void DatabaseList::dbOpen()
{
    QModelIndexList list = selectionModel()->selectedRows();
    foreach(QModelIndex index, list)
    {
        QString s = m_filterModel->data(m_filterModel->index(index.row(), DBLV_PATH)).toString();
        QString utf8 = m_filterModel->data(m_filterModel->index(index.row(), DBLV_UTF8)).toString();
        bool bUtf8 = (utf8.compare("UTF8") == 0);
        emit requestOpenDatabase(s, bUtf8);
    }
}

void DatabaseList::dbToggleUTF8()
{
    QModelIndexList list = selectionModel()->selectedRows();
    foreach(QModelIndex index, list)
    {
        QString s = m_filterModel->data(m_filterModel->index(index.row(), DBLV_PATH)).toString();
        QString utf8 = m_filterModel->data(m_filterModel->index(index.row(), DBLV_UTF8)).toString();
        bool bUtf8 = (utf8.compare("UTF8") == 0);
        setFileUtf8(s, !bUtf8);
    }
}

void DatabaseList::dbClose()
{
    QModelIndexList list = selectionModel()->selectedRows();
    foreach(QModelIndex index, list)
    {
        QString s = m_filterModel->data(m_filterModel->index(index.row(), DBLV_PATH)).toString();
        emit requestCloseDatabase(s);
    }
}

void DatabaseList::dbSetStarsForSelection(int stars)
{
    QModelIndexList list = selectionModel()->selectedRows();
    foreach(QModelIndex index, list)
    {
        QString s = m_filterModel->data(m_filterModel->index(index.row(), DBLV_PATH)).toString();
        setFileFavorite(s, true, 0);
        setStars(s,stars);
    }
}

void DatabaseList::dbRemoveFromFavorites()
{
    dbSetStarsForSelection(0);
}

void DatabaseList::dbKeepFile()
{
    dbSetStarsForSelection(1);
}

void DatabaseList::dbAddToFavorites2()
{
    dbSetStarsForSelection(2);
}

void DatabaseList::dbAddToFavorites3()
{
    dbSetStarsForSelection(3);
}

void DatabaseList::dbAddToOpenAtStartup()
{
    dbSetStarsForSelection(4);
}

void DatabaseList::dbSetActiveAtStartup()
{
    // There can only be one such file, the previous file loses the active flag
    Q_ASSERT(m_cell.isValid());
    QString ts = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_PATH)).toString();
    limitStars(4);
    setFileFavorite(ts, true, 0);
    setStars(ts,5);
}

void DatabaseList::slotMakeBook()
{
    Q_ASSERT(m_cell.isValid());
    QString pathIn = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_PATH)).toString();
    emit requestMakeBook(pathIn);
}

void DatabaseList::slotExtTool1()
{
    Q_ASSERT(m_cell.isValid());
    QString pathIn = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_PATH)).toString();

    ExtTool t;
    t.RunExtTool1(pathIn);
}

void DatabaseList::slotShowInFinder()
{
    Q_ASSERT(m_cell.isValid());
    QString pathIn = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_PATH)).toString();
    ShellHelper::showInFinder(pathIn);
}

GameId DatabaseList::getLastIndex(const QString& s) const
{
    return m_model->getLastIndex(s);
}

void DatabaseList::limitStars(int limit)
{
    m_model->limitStars(limit);
}

int DatabaseList::stars(const QString &s) const
{
    return m_model->stars(s);
}

void DatabaseList::addFileOpen(const QString& s, bool utf8)
{
    m_model->addFileOpen(s, utf8);
}

void DatabaseList::setFileFavorite(const QString& s, bool bFavorite, int index)
{
    m_model->addFavoriteFile(s, bFavorite, index);
    if (bFavorite)
    {
        emit raiseRequest();
    }
}

void DatabaseList::setStars(const QString &s, int stars)
{
    m_model->setStars(s, stars);
}

void DatabaseList::setFileUtf8(const QString& s, bool utf8)
{
    m_model->setFileUtf8(s, utf8);
}

void DatabaseList::setFileClose(const QString& s, GameId lastIndex)
{
    m_model->setFileClose(s, lastIndex);
}

void DatabaseList::setFileCurrent(const QString& s)
{
    m_model->setFileCurrent(s);
}

void DatabaseList::update(const QString& s)
{
    m_model->update(s);
}

void DatabaseList::dragEnterEvent(QDragEnterEvent *event)
{
    TableView::dragEnterEvent(event);
    m_lastModifier = qApp->keyboardModifiers();

    const QMimeData *mimeData = event->mimeData();
    const GameMimeData* gameMimeData = qobject_cast<const GameMimeData*>(mimeData);
    const DbMimeData* dbMimeData = qobject_cast<const DbMimeData*>(mimeData);

    if(gameMimeData)
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
    else if(dbMimeData && dbMimeData->hasUrls())
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
    else if(mimeData && mimeData->hasUrls())
    {
        if (m_lastModifier & LINK_MODIFIER)
        {
            event->setDropAction(Qt::LinkAction);
        }
        else
        {
            event->setDropAction(Qt::MoveAction);
        }
        event->accept();
    }
}

void DatabaseList::dragMoveEvent(QDragMoveEvent *event)
{
    TableView::dragMoveEvent(event);
    m_lastModifier = qApp->keyboardModifiers();
    const QMimeData *mimeData = event->mimeData();
    const GameMimeData* gameMimeData = qobject_cast<const GameMimeData*>(mimeData);
    const DbMimeData* dbMimeData = qobject_cast<const DbMimeData*>(mimeData);
    bool accept = true;
    if(gameMimeData)
    {
        event->setDropAction(Qt::CopyAction);
        QModelIndex index = indexAt(event->pos());
        if(index.isValid())
        {
            QString path = m_filterModel->data(m_filterModel->index(index.row(), DBLV_PATH)).toString();
            if (path == gameMimeData->source)
            {
                accept = false;
            }
        }
    }
    else if(dbMimeData && dbMimeData->hasUrls())
    {
        event->setDropAction(Qt::CopyAction);
        QModelIndex index = indexAt(event->pos());
        if(index.isValid())
        {
            QString path = m_filterModel->data(m_filterModel->index(index.row(), DBLV_PATH)).toString();
            QList<QUrl> urlList = dbMimeData->urls();
            foreach(QUrl url, urlList)
            {
                QString s = url.toString();
                if (path == s)
                {
                    accept = false;
                    break;
                }
            }
        }
    }
    else if(mimeData && mimeData->hasUrls())
    {
        if(m_lastModifier & LINK_MODIFIER)
        {
            event->setDropAction(Qt::LinkAction);
        }
        else
        {
            event->setDropAction(Qt::MoveAction);
        }
    }

    if (accept)
    {
        event->accept();
    }
}

void DatabaseList::dragLeaveEvent(QDragLeaveEvent *event)
{
    TableView::dragLeaveEvent(event);
    event->accept();
}

void DatabaseList::dropEvent(QDropEvent *event)
{
    TableView::dropEvent(event);
    if (event->dropAction())
    {
        // TODO - test where the files come from in case of gameMimeData !!!!!!!!! -> crash
        const QMimeData *mimeData = event->mimeData();
        const GameMimeData* gameMimeData = qobject_cast<const GameMimeData*>(mimeData);
        const DbMimeData* dbMimeData = qobject_cast<const DbMimeData*>(mimeData);
        if(gameMimeData)
        {
            QModelIndex index = indexAt(event->pos());
            appendGameToDataBase(index, gameMimeData->m_indexList, gameMimeData->source);
        }
        else if(dbMimeData && dbMimeData->hasUrls())
        {
            QList<QUrl> urlList = dbMimeData->urls();
            foreach(QUrl url, urlList)
            {
                QString s = url.toString();
                appendDataBaseToDataBase(event->pos(), s);
            }
        }
        else if(mimeData && mimeData->hasUrls())
        {
            QList<QUrl> urlList = mimeData->urls();
            foreach(QUrl url, urlList)
            {
                QString ts = url.toString();

                if(m_lastModifier & LINK_MODIFIER)
                {
                    emit requestLinkDatabase(ts);
                }
                else
                {
                    emit requestOpenDatabase(ts, false);
                }
            }
        }
    }

    event->acceptProposedAction();
}

void DatabaseList::appendGameToDataBase(QModelIndex index, QList<GameId> gameIndexList, QString source)
{
    // Make sure the drop occured on a cell!
    if(index.isValid())
    {
        QString path = m_filterModel->data(m_filterModel->index(index.row(), DBLV_PATH)).toString();
        emit requestAppendGames(path, gameIndexList, source);
    }
}

void DatabaseList::appendDataBaseToDataBase(QPoint pos, QString src)
{
    QModelIndex index = indexAt(pos);
    // Make sure the drop occured on a cell!
    if(index.isValid())
    {
        QString path = m_filterModel->data(m_filterModel->index(index.row(), DBLV_PATH)).toString();
        emit requestAppendDatabase(path, src);
    }
}

void DatabaseList::startToDrag()
{
    DbMimeData *mimeData = new DbMimeData;
    QList<QUrl> urlList;
    QModelIndexList list = selectionModel()->selectedRows();
    foreach(QModelIndex index, list)
    {
        QString s = m_filterModel->data(m_filterModel->index(index.row(), DBLV_PATH)).toString();
        urlList << QUrl(s);
    }
    mimeData->setUrls(urlList);

    QPixmap pixmap = style()->standardPixmap(QStyle::SP_FileIcon);

    QDrag* pDrag = new QDrag(this);
    pDrag->setMimeData(mimeData);
    pDrag->setPixmap(pixmap);

    pDrag->exec(Qt::CopyAction, Qt::LinkAction);
}

void DatabaseList::startDrag(Qt::DropActions supportedActions)
{
    TableView::startDrag(supportedActions);
    startToDrag();
}
