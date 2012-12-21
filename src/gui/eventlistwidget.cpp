/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "eventlistwidget.h"
#include "ui_tagdetailwidget.h"
#include "database.h"

EventListWidget::EventListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagDetailWidget)
{
    setObjectName("EventListWidget");
    ui->setupUi(this);
    m_filterModel = new QStringListModel(this);
    ui->tagList->setModel(m_filterModel);

    setObjectName("EventListWidget");
    connect(ui->filterEdit, SIGNAL(textChanged(const QString&)), SLOT(findEvent(const QString&)));
    connect(ui->tagList, SIGNAL(clicked(const QModelIndex&)), SLOT(showSelectedEvent()));
    connect(ui->filterDatabase, SIGNAL(clicked()), SLOT(filterSelectedEvent()));
    connect(ui->tagList, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(filterSelectedEvent()));

    selectEvent(QString());
    QItemSelectionModel* selectionModel = ui->tagList->selectionModel();
    connect(selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
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
}

void EventListWidget::selectionChangedSlot()
{
    if (ui->tagList->currentIndex().isValid())
    {
        QString ts = ui->tagList->currentIndex().data().toString();
        selectEvent(ts);
    }
    else
    {
        selectEvent(QString());
    }
}

void EventListWidget::findEvent(const QString& s)
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

void EventListWidget::selectEvent(const QString& event)
{
    if (!event.isEmpty())
    {
        m_event.setName(event);
        ui->filterDatabase->setEnabled(true);
        ui->detailText->setText(QString("<h1>%1</h1><p>%2%3%4%5%6")
                .arg(m_event.name()).arg(m_event.formattedGameCount())
                .arg(m_event.formattedRange())
                .arg(m_event.formattedRating())
                .arg(m_event.formattedScore())
                .arg(m_event.listOfPlayers()));
    }
    else
    {
        ui->filterDatabase->setEnabled(false);
        ui->detailText->setText(tr("<html><i>No event chosen.</i></html>"));
    }
}

void EventListWidget::showSelectedEvent()
{
    if (ui->tagList->currentIndex().isValid())
    {
        QString ts = ui->tagList->currentIndex().data().toString();
        selectEvent(ts);
    }
}

void EventListWidget::filterSelectedEvent()
{
    if (ui->tagList->currentIndex().isValid())
    {
        QString ts = ui->tagList->currentIndex().data().toString();
        emit filterRequest(ts);
    }
}

void EventListWidget::setDatabase(Database* db)
{
    ui->detailText->setText(tr("<html><i>No event chosen.</i></html>"));
    m_event.setDatabase(db);
    m_list.clear();
    if (db && db->index())
    {
        m_list = db->index()->tagValues(TagNameEvent);
    }
    m_filterModel->setStringList(m_list);
    m_filterModel->sort(0);
}

void EventListWidget::slotLinkClicked(const QUrl& url)
{
    if (url.scheme() == "player")
    {
        QString event = ui->tagList->currentIndex().data().toString();
        emit filterEventPlayerRequest(event, url.path());
    }
}
