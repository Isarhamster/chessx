#include "databaselist.h"
#include "databaselistmodel.h"
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QtGui>
#include "settings.h"

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

    setSortingEnabled(false);
    setAcceptDrops(true);

    horizontalHeader()->setStretchLastSection(true);
    setColumnWidth(DBLV_FAVORITE, 50);
    setColumnWidth(DBLV_NAME, 200);
    setColumnWidth(DBLV_SIZE, 75);
    setColumnWidth(DBLV_OPEN, 50);
}

DatabaseList::~DatabaseList()
{
}

void DatabaseList::slotContextMenu(const QPoint& pos)
{
    QMenu *menu = new QMenu;
    m_cell = indexAt(pos);
    // Make sure the right click occured on a cell!
    if (m_cell.isValid())
    {
        bool bIsFavorite = m_filterModel->data(m_filterModel->index(m_cell.row(),DBLV_FAVORITE), Qt::ToolTipRole).toString() == "Favorite";
        bool bIsNotFavorite = m_filterModel->data(m_filterModel->index(m_cell.row(),DBLV_FAVORITE), Qt::ToolTipRole).toString().isEmpty();
        bool bHasPath = !m_filterModel->data(m_filterModel->index(m_cell.row(),DBLV_PATH), Qt::ToolTipRole).toString().isEmpty();
        menu->addAction("Add to favorites", this, SLOT(dbAddToFavorites()))->setEnabled(bIsNotFavorite);
        menu->addAction("Remove from Favorites", this, SLOT(dbRemoveFromFavorites()))->setEnabled(bIsFavorite);
        menu->addSeparator();
        menu->addAction("Show in Finder", this, SLOT(slotShowInFinder()))->setEnabled(bHasPath);
        menu->exec(mapToGlobal(pos));
    }
}

void DatabaseList::save() const
{
    AppSettings->beginGroup("Favorites");
    QStringList list;
    m_model->toStringList(list);
    AppSettings->setValue("Files", list);
    AppSettings->endGroup();
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
    const QString explorer = Environment::systemEnvironment().searchInPath(QLatin1String("explorer.exe"));
    if (explorer.isEmpty()) {
        return;
    }
    QString param;
    if (!QFileInfo(pathIn).isDir())
        param = QLatin1String("/select,");
    param += QDir::toNativeSeparators(pathIn);
    QProcess::startDetached(explorer, QStringList(param));
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
#else
    // we cannot select a file here, because no file browser really supports it...
    const QFileInfo fileInfo(pathIn);
    const QString folder = fileInfo.absoluteFilePath();
    const QString app = Utils::UnixUtils::fileBrowser(Core::ICore::instance()->settings());
    QProcess browserProc;
    const QString browserArgs = Utils::UnixUtils::substituteFileBrowserParameters(app, folder);
    if (debug)
        qDebug() <<  browserArgs;
    bool success = browserProc.startDetached(browserArgs);
    const QString error = QString::fromLocal8Bit(browserProc.readAllStandardError());
    success = success && error.isEmpty();
#endif
}

void DatabaseList::itemSelected(const QModelIndex& index)
{
    if (index.column()==0)
    {
        QString ts = m_filterModel->data(m_filterModel->index(index.row(),DBLV_PATH)).toString();
        emit requestOpenDatabase(ts);
    }
}

void DatabaseList::addFileOpen(const QString& s)
{
    m_model->addFileOpen(s);
}

void DatabaseList::setFileFavorite(const QString& s, bool bFavorite)
{
    m_model->addFavoriteFile(s, bFavorite);
}

void DatabaseList::setFileClose(const QString& s)
{
    m_model->setFileClose(s);
}

void DatabaseList::setFileCurrent(const QString& s)
{
    m_model->setFileCurrent(s);
}

void DatabaseList::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void DatabaseList::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DatabaseList::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void DatabaseList::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        foreach(QUrl url, urlList)
        {
            QString ts = url.toLocalFile();
            emit requestOpenDatabase(ts);
        }
    }

    event->acceptProposedAction();
}
