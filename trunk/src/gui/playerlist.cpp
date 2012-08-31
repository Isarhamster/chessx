/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include <QHeaderView>
#include <QSortFilterProxyModel>

#include "playerlist.h"
#include "playerlistmodel.h"
#include "database.h"

PlayerList::PlayerList(QWidget *parent) :
    TableView(parent)
{
    setObjectName("PlayerList");
    setWindowTitle(tr("Player list"));

    m_model = new PlayerListModel(this);

    m_filterModel = new QSortFilterProxyModel(this);
    m_filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    setModel(m_filterModel);

    connect(this, SIGNAL(clicked(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
    connect(this, SIGNAL(activated(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));

    setSortingEnabled(false); // Todo: Sorting is extremely slow, reactive if this can be done faster
    configure();
}

PlayerList::~PlayerList()
{
}

void PlayerList::setDatabase(Database* db)
{
    m_model->setDatabase(db);
    m_filterModel->setSourceModel(m_model);
    m_filterModel->sort(0);
}

void PlayerList::itemSelected(const QModelIndex& index)
{
    if (index.column()==0)
    {
        QString ts = m_filterModel->data(index).toString();
        emit filterRequest(ts);
    }
}

void PlayerList::selectPlayer(const QString& name)
{
    QModelIndexList m = m_filterModel->match(m_filterModel->index(0,0),
                                Qt::DisplayRole,
                                name, -1,
                                Qt::MatchContains | Qt::MatchRecursive );
    if (m.count()>0)
    {
        setCurrentIndex(m[0]);
        selectRow(m[0].row());
    }
    emit raiseRequest();
}

void PlayerList::configure()
{
}

