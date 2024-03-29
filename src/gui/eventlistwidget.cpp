/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "eventlistwidget.h"
#include "ui_tagdetailwidget.h"
#include "database.h"
#include "databaseinfo.h"
#include "tags.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

EventListWidget::EventListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagDetailWidget)
{
    setObjectName("EventListWidget");
    ui->setupUi(this);
    m_filterModel = new QStringListModel(ui->tagList);
    ui->tagList->setModel(m_filterModel);

    setObjectName("EventListWidget");
    connect(ui->filterEdit, SIGNAL(textChanged(QString)), SLOT(findEvent(QString)));
    connect(ui->filterDatabase, SIGNAL(clicked()), SLOT(filterSelectedEvent()));
    connect(ui->addFilter, SIGNAL(clicked()), SLOT(filterSelectedEventAdd()));
    connect(ui->renameItem, SIGNAL(clicked()), SLOT(renameSelectedEvent()));
    connect(ui->tagList, SIGNAL(doubleClicked(QModelIndex)), SLOT(filterSelectedEvent()));

    selectEvent(QString());
    QItemSelectionModel* selectionModel = ui->tagList->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(selectionChangedSlot()));

    ui->detailText->setOpenLinks(false);
    connect(ui->detailText, SIGNAL(anchorClicked(QUrl)), SLOT(slotLinkClicked(QUrl)));

    slotReconfigure();
}

EventListWidget::~EventListWidget()
{
    delete ui;
}

void EventListWidget::slotReconfigure()
{
    ui->tagList->slotReconfigure();
}

void EventListWidget::selectionChangedSlot()
{
    const QModelIndexList& selection = ui->tagList->selectionModel()->selectedRows();
    if(selection.count())
    {
        QString ts = selection[0].data().toString();
        eventSelected(ts);
    }
    else
    {
        eventSelected(QString());
    }
}

void EventListWidget::findEvent(const QString& s)
{
    if(s.isEmpty())
    {
        m_filterModel->setStringList(m_list);
    }
    else
    {
        QStringList newList = m_list.filter(s, Qt::CaseInsensitive);
        m_filterModel->setStringList(newList);
        if (newList.count()==1)
        {
            selectEvent(newList.at(0));
        }
    }
}

void EventListWidget::eventSelected(const QString& event)
{
    if(!event.isEmpty())
    {
        m_event.setName(event);
        ui->filterDatabase->setEnabled(true);
        ui->addFilter->setEnabled(true);
        ui->renameItem->setEnabled(true);
        QString head = QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><head><title>%1</title><meta name='qrichtext' content='1'><meta http-equiv=\"Content-type\" content=\"text/html;charset=UTF-8\"></head>").arg(event);
        QString text = QString("%1<body><h1><a href='event:%2'>%3</a></h1>%4%5%6%7%8</body></html>")
                       .arg(head,
                            m_event.name(),
                            m_event.name(),
                            m_event.formattedGameCount(),
                            m_event.formattedRange(),
                            m_event.formattedRating(),
                            m_event.formattedScore(),
                            m_event.listOfPlayers());
        ui->detailText->setHtml(text);
    }
    else
    {
        ui->filterDatabase->setEnabled(false);
        ui->addFilter->setEnabled(false);
        ui->renameItem->setEnabled(false);
        ui->detailText->setText(tr("<html><i>No event chosen.</i></html>"));
    }
}

void EventListWidget::slotSelectEvent(const QString& event)
{
    m_filterModel->setStringList(m_list);
    ui->filterEdit->clear();
    selectEvent(event);
}

void EventListWidget::selectEvent(const QString& event)
{
    eventSelected(event);
    if(!event.isEmpty())
    {
        const QStringList& list = m_filterModel->stringList();
        int row = list.indexOf(event);
        if(row >= 0)
        {
            QModelIndex index = m_filterModel->index(row, 0);
            if(index.isValid())
            {
                ui->tagList->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
                ui->tagList->scrollTo(index);
            }
        }
    }
}

void EventListWidget::filterSelectedEvent()
{
    const QModelIndexList& selection = ui->tagList->selectionModel()->selectedRows();
    if(selection.count())
    {
        QString ts = selection[0].data().toString();
        emit filterRequest(ts);
    }
}

void EventListWidget::filterSelectedEventAdd()
{
    const QModelIndexList& selection = ui->tagList->selectionModel()->selectedRows();
    if(selection.count())
    {
        QString ts = selection[0].data().toString();
        ts.prepend("+");
        emit filterRequest(ts);
    }
}

void EventListWidget::renameSelectedEvent()
{
    const QModelIndexList& selection = ui->tagList->selectionModel()->selectedRows();
    if(selection.count())
    {
        QString ts = selection[0].data().toString();
        emit renameRequest(ts);
    }
}

void EventListWidget::setDatabase(DatabaseInfo* dbInfo)
{
    Database* db = dbInfo->database();
    ui->detailText->setText(tr("<html><i>No event chosen.</i></html>"));
    m_event.setDatabase(db);
    m_list.clear();
    if(db && db->index())
    {
        m_list = db->index()->tagValues(TagNameEvent);
    }
    m_list.sort();
    findEvent(ui->filterEdit->text().simplified());
    m_filterModel->sort(0);
}

void EventListWidget::slotLinkClicked(const QUrl& url)
{
    if(url.scheme() == "player")
    {
        emit filterEventPlayerRequest(url.path(), ui->detailText->documentTitle());
    }
    else if (url.scheme() == "event")
    {
        // QString event = ui->tagList->currentIndex().data().toString();
        emit filterRequest(url.path());
    }
}
