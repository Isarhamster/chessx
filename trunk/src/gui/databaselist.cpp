/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "databaselist.h"
#include "databaselistmodel.h"
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QtGui>
#include "settings.h"
#include "GameMimeData.h"

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
    setModel(m_filterModel);

    connect(this, SIGNAL(clicked(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
    connect(this, SIGNAL(activated(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(slotContextMenu(const QPoint&)));
    connect(m_filterModel, SIGNAL(rowsInserted(const QModelIndex &,int,int)), SLOT(rowsChanged (const QModelIndex &,int,int)));
    connect(m_model, SIGNAL(OnSelectIndex(const QModelIndex &)), SLOT(slotCurrentIndexChanged (const QModelIndex &)));

    setSortingEnabled(false);
    setAcceptDrops(true);

    setColumnWidth(DBLV_FAVORITE, 50);
    setColumnWidth(DBLV_NAME, 200);
    setColumnWidth(DBLV_SIZE, 75);
    setColumnWidth(DBLV_OPEN, 50);
    setColumnWidth(DBLV_PATH, 200);
    setColumnWidth(DBLV_OPEN, 50);
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
    if (m_cell.isValid())
    {
        QMenu menu(this);
        bool bIsFavorite = m_filterModel->data(m_filterModel->index(m_cell.row(),DBLV_FAVORITE), Qt::ToolTipRole).toString() == tr("Favorite");
        bool bIsNotFavorite = m_filterModel->data(m_filterModel->index(m_cell.row(),DBLV_FAVORITE), Qt::ToolTipRole).toString().isEmpty();
        bool bHasPath = !m_filterModel->data(m_filterModel->index(m_cell.row(),DBLV_PATH), Qt::ToolTipRole).toString().isEmpty();
        menu.addAction(tr("Open"), this, SLOT(dbOpen()));
        menu.addSeparator();
        menu.addAction(tr("Add to favorites"), this, SLOT(dbAddToFavorites()))->setEnabled(bIsNotFavorite);
        menu.addAction(tr("Remove from Favorites"), this, SLOT(dbRemoveFromFavorites()))->setEnabled(bIsFavorite);
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
        menu.addSeparator();
        menu.addAction(tr("Show in Finder"), this, SLOT(slotShowInFinder()))->setEnabled(bHasPath);
#endif
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
}

void DatabaseList::dbOpen()
{
    Q_ASSERT(m_cell.isValid());
    QString ts = m_filterModel->data(m_filterModel->index(m_cell.row(),DBLV_PATH)).toString();
    QString utf8 = m_filterModel->data(m_filterModel->index(m_cell.row(),DBLV_UTF8)).toString();
    bool bUtf8 = (utf8.compare("UTF8")==0);
    emit requestOpenDatabase(ts,bUtf8);
}

void DatabaseList::dbAddToFavorites()
{
    Q_ASSERT(m_cell.isValid());
    QString ts = m_filterModel->data(m_filterModel->index(m_cell.row(),DBLV_PATH)).toString();
    setFileFavorite(ts,true);
}

void DatabaseList::dbRemoveFromFavorites()
{
    Q_ASSERT(m_cell.isValid());
    QString ts = m_filterModel->data(m_filterModel->index(m_cell.row(),DBLV_PATH)).toString();
    setFileFavorite(ts,false);
}

void DatabaseList::rowsChanged(const QModelIndex &,int start,int end)
{
    for (int i= start; i<= end; ++i)
         setRowHeight(i, 24);
}

void DatabaseList::slotShowInFinder()
{
    Q_ASSERT(m_cell.isValid());
    QString pathIn = m_filterModel->data(m_filterModel->index(m_cell.row(),DBLV_PATH)).toString();
    // Mac, Windows support folder or file.
#if defined(Q_OS_WIN)
    QString param;
    if (!QFileInfo(pathIn).isDir())
        param = QLatin1String("/select,");
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

void DatabaseList::itemSelected(const QModelIndex& index)
{
    if (index.column()==0)
    {
        QString ts = m_filterModel->data(m_filterModel->index(index.row(),DBLV_PATH)).toString();
        QString utf8 = m_filterModel->data(m_filterModel->index(index.row(),DBLV_UTF8)).toString();
        bool bUtf8 = (utf8.compare("UTF8")==0);
        emit requestOpenDatabase(ts,bUtf8);
    }
}

void DatabaseList::addFileOpen(const QString& s, bool utf8)
{
    m_model->addFileOpen(s,utf8);
}

void DatabaseList::setFileFavorite(const QString& s, bool bFavorite)
{
    m_model->addFavoriteFile(s, bFavorite);
}

void DatabaseList::setFileUtf8(const QString& s, bool utf8)
{
    m_model->setFileUtf8(s, utf8);
}

void DatabaseList::setFileClose(const QString& s)
{
    m_model->setFileClose(s);
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

    if (gameMimeData || event->mimeData()->hasUrls())
        event->acceptProposedAction();
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
    if (gameMimeData)
    {
        appendGameToDataBase(event->pos(), gameMimeData->m_game);
    }
    else if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        foreach(QUrl url, urlList)
        {
            QString ts = url.toString();
            emit requestOpenDatabase(ts, false);

            if (m_lastModifier == Qt::AltModifier)
            {
                emit requestLinkDatabase(ts);
            }
        }
    }

    event->acceptProposedAction();
}

void DatabaseList::appendGameToDataBase(QPoint pos, const Game& g)
{
    m_cell = indexAt(pos);
    // Make sure the drop occured on a cell!
    if (m_cell.isValid())
    {
        QString path = m_filterModel->data(m_filterModel->index(m_cell.row(),DBLV_PATH)).toString();
        emit requestAppendGame(path, g);
    }
}
