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

void HistoryLabel::showHistory()
{
    if (!m_view)
    {
        initView();
    }
    if (m_view->isVisible()) return;
    m_model->setStringList(m_history);
    QModelIndex lastRow(m_model->index(m_history.size()-1));
    int bottomY = m_view->visualRect(lastRow).bottom();
    int widthHint = m_view->sizeHint().width();
    if (bottomY < m_view->sizeHint().height())
    {
        m_view->resize(widthHint, bottomY + 2);
        m_view->move(mapToGlobal(QPoint(0, height()-bottomY)));
    }
    m_view->show();
}

void HistoryLabel::setText(const QString &text, bool dontStore)
{
    if (text == this->text()) return;
    if (!text.isEmpty() && !dontStore)
    {
        m_history.prepend(text);
        if (m_history.size()>20)
        {
            m_history.removeLast();
        }
        if (m_view && m_view->isVisible())
        {
            m_model->setStringList(m_history);
        }
    }
    QLabel::setText(text);
}
