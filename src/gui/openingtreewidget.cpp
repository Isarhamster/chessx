/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "openingtreewidget.h"
#include "ui_openingtreewidget.h"

#include <QModelIndex>

#include "boardview.h"
#include "openingtree.h"
#include "settings.h"

OpeningTreeWidget::OpeningTreeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpeningTreeWidget)
{
    ui->setupUi(this);

    m_openingTree = new OpeningTree(this);
    ui->OpeningTreeView->setObjectName("OpeningTree");
    ui->OpeningTreeView->setSortingEnabled(true);
    ui->OpeningTreeView->setModel(m_openingTree);
    ui->OpeningTreeView->sortByColumn(1, Qt::DescendingOrder);
    connect(ui->OpeningTreeView, SIGNAL(clicked(const QModelIndex&)), parent, SLOT(slotSearchTreeMove(const QModelIndex&)));
    connect(m_openingTree, SIGNAL(progress(int)), this, SLOT(slotOperationProgress(int)));
    connect(m_openingTree, SIGNAL(openingTreeUpdated()), this, SLOT(slotTreeUpdate()));
    connect(m_openingTree, SIGNAL(openingTreeUpdateStarted()), this, SLOT(slotTreeUpdateStarted()));


    m_openingBoardView = new BoardView(this, BoardView::IgnoreSideToMove | BoardView::SuppressGuessMove);
    m_openingBoardView->setObjectName("OpeningBoardView");
    m_openingBoardView->setMinimumSize(200, 200);
    ui->OpeningBoardWidget->addWidget(m_openingBoardView, 1);
    m_openingBoardView->configure();
    m_openingBoardView->setEnabled(false);
}

OpeningTreeWidget::~OpeningTreeWidget()
{
    delete ui;
}

void OpeningTreeWidget::cancel(bool bVisible)
{
    m_openingTree->cancel(bVisible);
}

QString OpeningTreeWidget::move(QModelIndex index) const
{
    return m_openingTree->move(index);
}

Board OpeningTreeWidget::board() const
{
    return m_openingTree->board();
}

bool OpeningTreeWidget::updateFilter(Filter& f, const Board& b, bool bEnd)
{
    m_openingBoardView->setBoard(b);
    return m_openingTree->updateFilter(f, b, ui->filterGames->isChecked(), ui->sourceSelector->currentIndex()==0, bEnd);
}

void OpeningTreeWidget::saveConfig()
{
    AppSettings->setLayout(this);
    ui->OpeningTreeView->saveConfig();
}

void OpeningTreeWidget::slotReconfigure()
{
    m_openingBoardView->configure();
    AppSettings->layout(this);
    ui->OpeningTreeView->slotReconfigure();
}

void OpeningTreeWidget::slotOperationProgress(int value)
{
    ui->progress->setValue(value);
}

void OpeningTreeWidget::slotTreeUpdate()
{
    ui->progress->setValue(100);
    if (ui->filterGames->isChecked())
    {
        emit signalTreeUpdated();
    }
}

void OpeningTreeWidget::slotTreeUpdateStarted()
{
    ui->progress->setValue(0);
}
