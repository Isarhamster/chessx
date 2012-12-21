/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "playerlistwidget.h"
#include "ui_tagdetailwidget.h"
#include "database.h"

PlayerListWidget::PlayerListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagDetailWidget)
{
    setObjectName("PlayerListWidget");
    ui->setupUi(this);
    m_filterModel = new QStringListModel(this);
    ui->tagList->setModel(m_filterModel);

    setObjectName("PlayerListWidget");
    connect(ui->filterEdit, SIGNAL(textChanged(const QString&)), SLOT(findPlayers(const QString&)));
    connect(ui->tagList, SIGNAL(clicked(const QModelIndex&)), SLOT(showSelectedPlayer()));
    connect(ui->filterDatabase, SIGNAL(clicked()), SLOT(filterSelectedPlayer()));
    connect(ui->tagList, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(filterSelectedPlayer()));

    selectPlayer(QString());
    QItemSelectionModel* selectionModel = ui->tagList->selectionModel();
    connect(selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(selectionChangedSlot()));
    ui->detailText->setOpenLinks(false);

    slotReconfigure();
}

PlayerListWidget::~PlayerListWidget()
{
    delete ui;
}

void PlayerListWidget::slotReconfigure()
{
}

void PlayerListWidget::selectionChangedSlot()
{
    if (ui->tagList->currentIndex().isValid())
    {
        QString ts = ui->tagList->currentIndex().data().toString();
        selectPlayer(ts);
    }
    else
    {
        selectPlayer(QString());
    }
}

void PlayerListWidget::findPlayers(const QString& s)
{
    if (s.isEmpty())
    {
        m_filterModel->setStringList(m_list);
    }
    else
    {
        QStringList newList = m_list.filter(s, Qt::CaseInsensitive);
        m_filterModel->setStringList(newList);
    }
}

void PlayerListWidget::selectPlayer(const QString& player)
{
    if (!player.isEmpty())
    {
        m_player.setName(player);
        ui->filterDatabase->setEnabled(true);
        ui->detailText->setText(QString("<h1>%1</h1><p>%2%3%4%5%6")
                .arg(m_player.name()).arg(m_player.formattedGameCount())
                .arg(m_player.formattedRange())
                .arg(m_player.formattedRating())
                .arg(m_player.formattedScore())
                .arg(m_player.listOfOpenings())
                );
    }
    else
    {
        ui->filterDatabase->setEnabled(false);
        ui->detailText->setText(tr("<html><i>No player chosen.</i></html>"));
    }
}

void PlayerListWidget::showSelectedPlayer()
{
    if (ui->tagList->currentIndex().isValid())
    {
        QString ts = ui->tagList->currentIndex().data().toString();
        selectPlayer(ts);
    }
}

void PlayerListWidget::filterSelectedPlayer()
{
    if (ui->tagList->currentIndex().isValid())
    {
        QString ts = ui->tagList->currentIndex().data().toString();
        emit filterRequest(ts);
    }
}

void PlayerListWidget::setDatabase(Database* db)
{
    ui->detailText->setText(tr("<html><i>No player chosen.</i></html>"));
    m_player.setDatabase(db);
    m_list.clear();
    if (db && db->index())
    {
        m_list = db->index()->playerNames();
    }
    m_filterModel->setStringList(m_list);
    m_filterModel->sort(0);
}
