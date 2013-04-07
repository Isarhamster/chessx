/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "ecolistwidget.h"
#include "ui_tagdetailwidget.h"
#include "database.h"
#include "databaseinfo.h"

ECOListWidget::ECOListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagDetailWidget)
{
    setObjectName("ECOListWidget");
    ui->setupUi(this);
    m_filterModel = new QStringListModel(this);
    ui->tagList->setModel(m_filterModel);

    setObjectName("ECOListWidget");
    connect(ui->filterEdit, SIGNAL(textChanged(const QString&)), SLOT(findEvent(const QString&)));
    connect(ui->tagList, SIGNAL(clicked(const QModelIndex&)), SLOT(showSelectedEvent()));
    connect(ui->filterDatabase, SIGNAL(clicked()), SLOT(filterSelectedEvent()));
    connect(ui->renameItem, SIGNAL(clicked()), SLOT(renameSelectedEvent()));
    connect(ui->tagList, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(filterSelectedEvent()));

    selectECO(QString());
    QItemSelectionModel* selectionModel = ui->tagList->selectionModel();
    connect(selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(selectionChangedSlot()));

    ui->detailText->setOpenLinks(false);
    connect(ui->detailText, SIGNAL(anchorClicked(QUrl)), SLOT(slotLinkClicked(QUrl)));

    slotReconfigure();
}

ECOListWidget::~ECOListWidget()
{
    delete ui;
}

void ECOListWidget::slotReconfigure()
{
}

void ECOListWidget::selectionChangedSlot()
{
    const QModelIndexList& selection = ui->tagList->selectionModel()->selectedIndexes();
    if (selection.count())
    {
        QString ts = selection[0].data().toString();
        selectECO(ts);
    }
    else
    {
        selectECO(QString());
    }
}

void ECOListWidget::findECO(const QString& s)
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

void ECOListWidget::slotSelectECO(const QString& eco)
{
    m_filterModel->setStringList(m_list);
    ui->filterEdit->clear();
    selectECO(eco);
}

void ECOListWidget::selectECO(const QString& eco)
{
    if (!eco.isEmpty())
    {
        // TODO m_event.setName(event);
        ui->filterDatabase->setEnabled(true);
        ui->renameItem->setEnabled(true);
        /*ui->detailText->setText(QString("<h1>%1</h1><p>%2%3%4%5%6")
                .arg(m_event.name()).arg(m_event.formattedGameCount())
                .arg(m_event.formattedRange())
                .arg(m_event.formattedRating())
                .arg(m_event.formattedScore())
                .arg(m_event.listOfPlayers()));
                */
        const QStringList& list = m_filterModel->stringList();
        int row = list.indexOf(eco);
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
    else
    {
        ui->filterDatabase->setEnabled(false);
        ui->renameItem->setEnabled(false);
        ui->detailText->setText(tr("<html><i>No event chosen.</i></html>"));
    }
}

void ECOListWidget::showSelectedECO()
{
    if (ui->tagList->currentIndex().isValid())
    {
        QString ts = ui->tagList->currentIndex().data().toString();
        selectECO(ts);
    }
}

void ECOListWidget::filterSelectedECO()
{
    const QModelIndexList& selection = ui->tagList->selectionModel()->selectedIndexes();
    if (selection.count())
    {
        QString ts = selection[0].data().toString();
        emit filterRequest(ts);
    }
}

void ECOListWidget::setDatabase(DatabaseInfo* dbInfo)
{
    Database* db = dbInfo->database();
    ui->detailText->setText(tr("<html><i>No event chosen.</i></html>"));
    // TODO m_event.setDatabase(db);
    m_list.clear();
    if (db && db->index())
    {
        m_list = db->index()->tagValues(TagNameEvent);
    }
    m_list.sort();
    m_filterModel->setStringList(m_list);
    m_filterModel->sort(0);
}

void ECOListWidget::slotLinkClicked(const QUrl& url)
{
    if (url.scheme() == "eco")
    {
        QString eco = ui->tagList->currentIndex().data().toString();
        emit filterEcoRequest(url.path(), eco);
    }
}
