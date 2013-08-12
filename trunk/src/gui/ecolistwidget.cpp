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
    ui->renameItem->setVisible(false);

    setObjectName("ECOListWidget");
    connect(ui->filterEdit, SIGNAL(textChanged(const QString&)), SLOT(findECO(const QString&)));
    connect(ui->tagList, SIGNAL(clicked(const QModelIndex&)), SLOT(showSelectedECO()));
    connect(ui->filterDatabase, SIGNAL(clicked()), SLOT(filterSelectedECO()));
    connect(ui->tagList, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(filterSelectedECO()));

    selectECO(QString());
    QItemSelectionModel* selectionModel = ui->tagList->selectionModel();
    connect(selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(selectionChangedSlot()));

    ui->detailText->setOpenLinks(true);
    ui->detailText->setOpenExternalLinks(false);

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
        ecoSelected(ts);
    }
    else
    {
        ecoSelected(QString());
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

void ECOListWidget::ecoSelected(const QString& eco)
{
    if (!eco.isEmpty())
    {
        m_eco.setCode(eco);
        ui->filterDatabase->setEnabled(true);
        ui->renameItem->setEnabled(true);
        ui->detailText->setHtml(QString("<html><body><h1>%1</h1><p>%2%3%4%5</body></html>")
                .arg(m_eco.name()).arg(m_eco.formattedGameCount())
                .arg(m_eco.formattedRating())
                .arg(m_eco.formattedScore())
                .arg(m_eco.listOfPlayers()));
    }
    else
    {
        ui->filterDatabase->setEnabled(false);
        ui->renameItem->setEnabled(false);
        ui->detailText->setText(tr("<html><i>No ECO code chosen.</i></html>"));
    }

}

void ECOListWidget::selectECO(const QString& eco)
{
    ecoSelected(eco);
    if (!eco.isEmpty())
    {
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
    ui->detailText->setText(tr("<html><i>No ECO code chosen.</i></html>"));
    m_eco.setDatabase(db);
    m_list.clear();
    if (db && db->index())
    {
        m_list = db->index()->tagValues(TagNameECO);
    }
    m_list.sort();
    m_filterModel->setStringList(m_list);
    m_filterModel->sort(0);
}

void ECOListWidget::slotLinkClicked(const QUrl& url)
{
    if (url.scheme().startsWith("player"))
    {
        QString eco = ui->tagList->currentIndex().data().toString();
        emit filterEcoPlayerRequest(url.scheme().contains("white") ? TagNameWhite:TagNameBlack,
                                    eco, url.path());
    }
}
