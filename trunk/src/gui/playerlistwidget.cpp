/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "playerlistwidget.h"
#include "ui_tagdetailwidget.h"
#include "database.h"
#include "databaseinfo.h"

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
    connect(ui->filterDatabase, SIGNAL(clicked()), SLOT(filterSelectedPlayer()));
    connect(ui->renameItem, SIGNAL(clicked()), SLOT(renameSelectedPlayer()));
    connect(ui->tagList, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(filterSelectedPlayer()));

    selectPlayer(QString());
    QItemSelectionModel* selectionModel = ui->tagList->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(selectionChangedSlot()));

    ui->detailText->setOpenLinks(false);
    connect(ui->detailText, SIGNAL(anchorClicked(QUrl)), SLOT(slotLinkClicked(QUrl)));

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
    const QModelIndexList& selection = ui->tagList->selectionModel()->selectedIndexes();
    if (selection.count())
    {
        QString ts = selection[0].data().toString();
        playerSelected(ts);
    }
    else
    {
        playerSelected(QString());
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

void PlayerListWidget::slotSelectPlayer(const QString& player)
{
    m_filterModel->setStringList(m_list);
    ui->filterEdit->clear();
    selectPlayer(player);
}

void PlayerListWidget::playerSelected(const QString& player)
{
    if (!player.isEmpty())
    {
        m_player.setName(player);
        ui->filterDatabase->setEnabled(true);
        ui->renameItem->setEnabled(true);
        QString head = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><head><title>Player List</title><meta name='qrichtext' content='1'><meta http-equiv=\"Content-type\" content=\"text/html;charset=UTF-8\"></head>";
        QString text = QString("%1<body><h1>%2</h1>%3%4%5%6%7</body></html>")
                .arg(head)
                .arg(m_player.name()).arg(m_player.formattedGameCount())
                .arg(m_player.formattedRange())
                .arg(m_player.formattedRating())
                .arg(m_player.formattedScore())
                .arg(m_player.listOfOpenings());
        ui->detailText->setHtml(text);
    }
    else
    {
        ui->filterDatabase->setEnabled(false);
        ui->renameItem->setEnabled(false);
        ui->detailText->setText(tr("<html><i>No player chosen.</i></html>"));
    }
}

void PlayerListWidget::selectPlayer(const QString& player)
{
    playerSelected(player);
    if (!player.isEmpty())
    {
        const QStringList& list = m_filterModel->stringList();
        int row = list.indexOf(player);
        if (row>=0)
        {
            QModelIndex index = m_filterModel->index( row, 0 );
            if ( index.isValid() )
            {
                ui->tagList->selectionModel()->select( index, QItemSelectionModel::ClearAndSelect );
                ui->tagList->scrollTo(index);
            }
        }
    }
}

void PlayerListWidget::filterSelectedPlayer()
{
    const QModelIndexList& selection = ui->tagList->selectionModel()->selectedIndexes();
    if (selection.count())
    {
        QString ts = selection[0].data().toString();
        emit filterRequest(ts);
    }
}

void PlayerListWidget::renameSelectedPlayer()
{
    const QModelIndexList& selection = ui->tagList->selectionModel()->selectedIndexes();
    if (selection.count())
    {
        QString ts = selection[0].data().toString();
        emit renameRequest(ts);
    }
}

void PlayerListWidget::setDatabase(DatabaseInfo* dbInfo)
{
    Database* db = dbInfo->database();
    ui->detailText->setText(tr("<html><i>No player chosen.</i></html>"));
    m_player.setDatabase(db);
    m_list.clear();
    if (db && db->index())
    {
        m_list = db->index()->playerNames();
    }
    m_list.sort();
    m_filterModel->setStringList(m_list);
    m_filterModel->sort(0);
}

void PlayerListWidget::slotLinkClicked(const QUrl& url)
{
    if (url.scheme().startsWith("eco"))
    {
        QString player = ui->tagList->currentIndex().data().toString();
        QString tag = (url.scheme().contains("white")) ? TagNameWhite : TagNameBlack;
        emit filterEcoPlayerRequest(tag, url.path(), player);
    }
}
