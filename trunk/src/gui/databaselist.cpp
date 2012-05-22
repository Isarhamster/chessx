#include "databaselist.h"
#include "databaselistmodel.h"
#include <QHeaderView>
#include <QSortFilterProxyModel>

DatabaseList::DatabaseList(QWidget *parent) :
    TableView(parent)
{
    setObjectName("DatabaseList");
    setWindowTitle(tr("Databases"));

    m_model = new DatabaseListModel(this);

    m_filterModel = new QSortFilterProxyModel(this);
    m_filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_filterModel->setSourceModel(m_model);
    setModel(m_filterModel);

    connect(this, SIGNAL(clicked(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
    connect(this, SIGNAL(activated(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));

    setSortingEnabled(true);


}

DatabaseList::~DatabaseList()
{
}

void DatabaseList::itemSelected(const QModelIndex& index)
{
    if (index.column()==0)
    {
        QString ts = m_filterModel->data(m_filterModel->index(index.row(),3)).toString();
        emit requestOpenDatabase(ts);
    }
}

void DatabaseList::addRecentFile(const QString& s)
{
    m_model->addRecentFile(s);
}
