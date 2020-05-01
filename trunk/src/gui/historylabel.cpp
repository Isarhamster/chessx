/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "historylabel.h"

#include <QStringList>
#include <QListView>
#include <QStringListModel>
#include <QAction>
#include <QMouseEvent>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

HistoryLabel::HistoryLabel(QWidget *parent, Qt::WindowFlags f) :
    QLabel(parent,f), m_model(nullptr), m_view(nullptr)
{
    init();
}

HistoryLabel::HistoryLabel(const QString &text, QWidget *parent, Qt::WindowFlags f) :
    QLabel(text, parent, f), m_model(nullptr), m_view(nullptr)
{
    init();
}

void HistoryLabel::init()
{
    QAction * showHistory = new QAction("History", this);
    connect(showHistory, SIGNAL(triggered()), SLOT(showHistory()));
    addAction(showHistory);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    maxSize = 64;
    itemsRemoved = false;
}

void HistoryLabel::initView()
{
    m_model = new QStringListModel(this);
    m_view = new QHistoryListView(this);
    m_view->setModel(m_model);
    m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_view->setSelectionMode(QAbstractItemView::NoSelection);
    m_view->setWordWrap(true);
    m_view->setAlternatingRowColors(true);
}

void HistoryLabel::mouseDoubleClickEvent( QMouseEvent * e )
{
    if ( e->button() == Qt::LeftButton )
    {
        showHistory();
        e->accept();
    }
}

int HistoryLabel::getMaxSize() const
{
    return maxSize;
}

void HistoryLabel::setMaxSize(int value)
{
    maxSize = value;
    while (m_history.size()>maxSize)
    {
        m_history.removeLast();
        itemsRemoved = true;
    }
}

void HistoryLabel::showHistory()
{
    if (!m_view)
    {
        initView();
    }
    if (m_view->isVisible()) return;
    QStringList l = m_history;
    if (itemsRemoved)
    {
        l.append("...");
    }
    m_model->setStringList(l);

    QPoint pos = QCursor::pos();
    pos.setX(pos.x() - m_view->sizeHint().width());
    pos.setY(pos.y() - m_view->sizeHint().height());
    if (pos.x()<0) pos.setX(0);
    if (pos.y()<0) pos.setY(0);

    m_view->move(pos);
    m_view->show();
}

void HistoryLabel::setText(const QString &text, bool dontStore)
{
    if (text == this->text()) return;
    if (!text.isEmpty() && !dontStore)
    {
        m_history.prepend(text);

        if (m_history.size()>maxSize)
        {
            m_history.removeLast();
            itemsRemoved = true;
        }
        QStringList l = m_history;
        if (itemsRemoved)
        {
            l.append("...");
        }
        if (m_view && m_view->isVisible())
        {
            m_model->setStringList(l);
        }
    }
    QLabel::setText(text);
}
