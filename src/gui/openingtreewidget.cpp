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

    m_openingTree = new OpeningTree;
    ui->OpeningTreeView->setObjectName("OpeningTree");
    ui->OpeningTreeView->setSortingEnabled(true);
    ui->OpeningTreeView->setModel(m_openingTree);
    ui->OpeningTreeView->sortByColumn(1, Qt::DescendingOrder);
    connect(ui->OpeningTreeView, SIGNAL(clicked(const QModelIndex&)), parent, SLOT(slotSearchTreeMove(const QModelIndex&)));
    connect(m_openingTree, SIGNAL(progress(int)), parent, SLOT(slotOperationProgress(int)));
    connect(m_openingTree, SIGNAL(openingTreeUpdated()), parent, SLOT(slotTreeUpdate()));
    connect(m_openingTree, SIGNAL(openingTreeUpdateStarted()), parent, SLOT(slotTreeUpdateStarted()));
    connect(parent, SIGNAL(reconfigure()), SLOT(slotReconfigure()));

    m_boardView = new BoardView(this, BoardView::IgnoreSideToMove | BoardView::SuppressGuessMove);
    m_boardView->setObjectName("OpeningBoardView");
    m_boardView->setMinimumSize(200, 200);
    ui->OpeningBoardWidget->addWidget(m_boardView,1);
    m_boardView->configure();
    m_boardView->setEnabled(false);
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

bool OpeningTreeWidget::update(Filter& f, const Board& b, bool updateFilter)
{
    m_boardView->setBoard(b);
    return m_openingTree->update(f, b, updateFilter);
}

void OpeningTreeWidget::saveConfig()
{
    AppSettings->setLayout(this);
    ui->OpeningTreeView->saveConfig();
}

void OpeningTreeWidget::slotReconfigure()
{
    AppSettings->layout(this);
    ui->OpeningTreeView->slotReconfigure();
    m_boardView->configure();
}



