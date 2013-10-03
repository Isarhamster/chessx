/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "databaselist.h"
#include "databaselistmodel.h"
#include "GameMimeData.h"
#include "settings.h"

#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QtGui>

DatabaseList::DatabaseList(QWidget *parent) :
    TableView(parent)
{
    setObjectName("DatabaseList");
    setWindowTitle(tr("Databases"));

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
    connect(m_filterModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)), SLOT(rowsChanged(const QModelIndex &, int, int)));
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
}

void DatabaseList::slotContextMenu(const QPoint& pos)
{
    m_cell = indexAt(pos);
    // Make sure the right click occured on a cell!
    if(m_cell.isValid())
    {
        QMenu menu(this);
        bool bIsFavorite = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_FAVORITE), Qt::UserRole).toString() == "Favorite";
        bool bIsNotFavorite = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_FAVORITE), Qt::UserRole).toString().isEmpty();
        bool bHasPath = !m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_PATH), Qt::UserRole).toString().isEmpty();
        bool bIsOpen = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_OPEN), Qt::UserRole).toString() == "Open";
        menu.addAction(bIsOpen ? tr("Activate") : tr("Open"), this, SLOT(dbOpen()));

        menu.addAction(tr("Close"), this, SLOT(dbClose()))->setEnabled(bIsOpen && bHasPath);
        menu.addSeparator();
        menu.addAction(tr("Add to favorites"), this, SLOT(dbAddToFavorites()))->setEnabled(bIsNotFavorite);
        menu.addAction(tr("Remove from Favorites"), this, SLOT(dbRemoveFromFavorites()))->setEnabled(bIsFavorite);
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
        menu.addSeparator();
        menu.addAction(tr("Show in Finder"), this, SLOT(slotShowInFinder()))->setEnabled(bHasPath);
#endif
        menu.addSeparator();
        QAction* action = menu.addAction("UTF8", this, SLOT(dbToggleUTF8()));
        action->setCheckable(true);
        QString utf8 = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_UTF8)).toString();
        bool bUtf8 = (utf8.compare("UTF8") == 0);
        action->setChecked(bUtf8);

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
    if(index.column() == 0)
    {
        slotDoubleClicked(index);
    }
}

void DatabaseList::dbOpen()
{
    Q_ASSERT(m_cell.isValid());
    QString ts = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_PATH)).toString();
    QString utf8 = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_UTF8)).toString();
    bool bUtf8 = (utf8.compare("UTF8") == 0);
    emit requestOpenDatabase(ts, bUtf8);
}

void DatabaseList::dbToggleUTF8()
{
    QString ts = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_PATH)).toString();
    QString utf8 = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_UTF8)).toString();
    bool bUtf8 = (utf8.compare("UTF8") == 0);
    setFileUtf8(ts, !bUtf8);
}

void DatabaseList::dbClose()
{
    Q_ASSERT(m_cell.isValid());
    QString ts = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_PATH)).toString();
    emit requestCloseDatabase(ts);
}

void DatabaseList::dbAddToFavorites()
{
    Q_ASSERT(m_cell.isValid());
    QString ts = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_PATH)).toString();
    setFileFavorite(ts, true, 0);
}

void DatabaseList::dbRemoveFromFavorites()
{
    Q_ASSERT(m_cell.isValid());
    QString ts = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_PATH)).toString();
    setFileFavorite(ts, false, 0);
}

void DatabaseList::rowsChanged(const QModelIndex &, int start, int end)
{
    for(int i = start; i <= end; ++i)
    {
        setRowHeight(i, 24);
    }
}

void DatabaseList::slotShowInFinder()
{
    Q_ASSERT(m_cell.isValid());
    QString pathIn = m_filterModel->data(m_filterModel->index(m_cell.row(), DBLV_PATH)).toString();
    // Mac, Windows support folder or file.
#if defined(Q_OS_WIN)
    QString param;
    if(!QFileInfo(pathIn).isDir())
    {
        param = QLatin1String("/select,");
    }
    param += QDir::toNativeSeparators(pathIn);
    QProcess::startDetached("explorer.exe", QStringList(param));
#elif defined(Q_OS_MAC)
    QStringList scriptArgs;
    scriptArgs << QLatin1String("-e")
               << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
               .arg(pathIn);
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    scriptArgs.clear();
    scriptArgs << QLatin1String("-e")
               << QLatin1String("tell application \"Finder\" to activate");
    QProcess::execute("/usr/bin/osascript", scriptArgs);
#endif
}

int DatabaseList::getLastIndex(const QString& s) const
{
    return m_model->getLastIndex(s);
}

void DatabaseList::addFileOpen(const QString& s, bool utf8)
{
    m_model->addFileOpen(s, utf8);
}

void DatabaseList::setFileFavorite(const QString& s, bool bFavorite, int index)
{
    m_model->addFavoriteFile(s, bFavorite, index);
}

void DatabaseList::setFileUtf8(const QString& s, bool utf8)
{
    m_model->setFileUtf8(s, utf8);
}

void DatabaseList::setFileClose(const QString& s, int lastIndex)
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
    m_lastModifier = event->keyboardModifiers();

    const QMimeData *mimeData = event->mimeData();
    const GameMimeData* gameMimeData = qobject_cast<const GameMimeData*>(mimeData);
    const DbMimeData* dbMimeData = qobject_cast<const DbMimeData*>(mimeData);

    if(gameMimeData || dbMimeData || event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void DatabaseList::dragMoveEvent(QDragMoveEvent *event)
{
    m_lastModifier = event->keyboardModifiers();

    event->acceptProposedAction();
}

void DatabaseList::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void DatabaseList::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    const GameMimeData* gameMimeData = qobject_cast<const GameMimeData*>(mimeData);
    const DbMimeData* dbMimeData = qobject_cast<const DbMimeData*>(mimeData);
    if(gameMimeData)
    {
        QModelIndex index = indexAt(event->pos());
        appendGameToDataBase(index, gameMimeData->m_index);
    }
    else if(dbMimeData)
    {
        appendDataBaseToDataBase(event->pos(), dbMimeData->m_path);
    }
    else if(mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        foreach(QUrl url, urlList)
        {
            QString ts = url.toString();

            if(m_lastModifier == Qt::AltModifier)
            {
                emit requestLinkDatabase(ts);
            }
            else
            {
                emit requestOpenDatabase(ts, false);
            }
        }
    }

    event->acceptProposedAction();
}

void DatabaseList::appendGameToDataBase(QModelIndex index, int gameIndex)
{
    // Make sure the drop occured on a cell!
    if(index.isValid())
    {
        QString path = m_filterModel->data(m_filterModel->index(index.row(), DBLV_PATH)).toString();
        emit requestAppendGame(path, gameIndex);
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

void DatabaseList::startToDrag(const QModelIndex& index)
{
    DbMimeData *mimeData = new DbMimeData;
    mimeData->m_path = m_filterModel->data(m_filterModel->index(index.row(), DBLV_PATH)).toString();

    QPixmap pixmap = style()->standardPixmap(QStyle::SP_FileIcon);

    QDrag* pDrag = new QDrag(this);
    pDrag->setMimeData(mimeData);
    pDrag->setPixmap(pixmap);

    pDrag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
}

void DatabaseList::startDrag(Qt::DropActions supportedActions)
{
    startToDrag(currentIndex());
}
