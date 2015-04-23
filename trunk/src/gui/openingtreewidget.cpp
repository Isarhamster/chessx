/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "openingtreewidget.h"
#include "ui_openingtreewidget.h"

#include <QModelIndex>
#include <QToolButton>
#include <QUndoGroup>
#include <QUndoStack>

#include "boardview.h"
#include "databaseinfo.h"
#include "openingtree.h"
#include "settings.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

OpeningTreeWidget::OpeningTreeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpeningTreeWidget)
{
    ui->setupUi(this);

    qRegisterMetaType<QList<MoveData> >("QList<MoveData>");

    m_openingTree = new OpeningTree(ui->OpeningTreeView);

    QUndoGroup* undoGroup = new QUndoGroup(this);
    m_UndoStack = new QUndoStack(undoGroup);
    m_UndoStack->setUndoLimit(10);
    undoGroup->addStack(m_UndoStack);
    undoGroup->setActiveStack(m_UndoStack);

    connect(ui->btUndo, SIGNAL(clicked()), undoGroup, SLOT(undo()));
    connect(undoGroup, SIGNAL(canUndoChanged(bool)), ui->btUndo, SLOT(setEnabled(bool)));
    ui->btUndo->setEnabled(false);

    ui->OpeningTreeView->setObjectName("OpeningTree");
    ui->OpeningTreeView->setSortingEnabled(true);
    ui->OpeningTreeView->setModel(m_openingTree);
    ui->OpeningTreeView->sortByColumn(1, Qt::DescendingOrder);

    connect(ui->OpeningTreeView, SIGNAL(clicked(const QModelIndex&)), parent, SLOT(slotSearchTreeMove(const QModelIndex&)));
    connect(m_openingTree, SIGNAL(progress(int)), this, SLOT(slotOperationProgress(int)));
    connect(m_openingTree, SIGNAL(openingTreeUpdated()), this, SLOT(slotTreeUpdate()));
    connect(m_openingTree, SIGNAL(openingTreeUpdateStarted()), this, SLOT(slotTreeUpdateStarted()));
    connect(ui->sourceSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSourceChanged()));

    m_openingBoardView = new BoardView(this, BoardView::IgnoreSideToMove | BoardView::SuppressGuessMove);
    m_openingBoardView->setObjectName("OpeningBoardView");
    m_openingBoardView->setMinimumSize(200, 200);
    ui->OpeningBoardWidget->addWidget(m_openingBoardView, 1);
    m_openingBoardView->setVAlignTop(true);
    m_openingBoardView->configure();
    m_openingBoardView->setEnabled(false);
}

OpeningTreeWidget::~OpeningTreeWidget()
{
    delete ui;
}

void OpeningTreeWidget::cancel()
{
    m_openingTree->cancel();
}

QString OpeningTreeWidget::move(QModelIndex index) const
{
    return m_openingTree->move(index);
}

Board OpeningTreeWidget::board() const
{
    return m_openingTree->board();
}

void OpeningTreeWidget::updateFilter(Filter& f, const Board& b, bool bEnd)
{
    if (ui->btPin->isChecked()) return;

    if (m_openingTree->board() != b)
    {
        m_UndoStack->push(new BoardUndoCommand(this,&f,m_openingTree->board(),m_openingTree->bEnd(),""));
    }
    doSetBoard(f,b,bEnd);
}

void OpeningTreeWidget::doSetBoard(Filter& f, const Board& b, bool bEnd)
{
    m_openingBoardView->setBoard(b);
    m_openingTree->updateFilter(f, b, ui->filterGames->isChecked(), ui->sourceSelector->currentIndex()==1, bEnd);
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
        bool dbIsFilterSource = (ui->sourceSelector->currentIndex()<=1);
        emit signalTreeUpdated(dbIsFilterSource);
    }
}

void OpeningTreeWidget::slotTreeUpdateStarted()
{
    ui->progress->setValue(0);
}

int OpeningTreeWidget::getFilterIndex(QString& name) const
{
    int index = ui->sourceSelector->currentIndex();
    if (index>1)
        name = m_filePaths[ui->sourceSelector->currentIndex()-2];
    else
        name = ui->sourceSelector->currentText();
    return index;
}

void OpeningTreeWidget::updateFilterIndex(QStringList files)
{
    m_filePaths.clear();
    m_filePaths = files;
    disconnect(ui->sourceSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSourceChanged()));
    QString current = ui->sourceSelector->currentText();
    ui->sourceSelector->clear();
    QStringList baseNames;
    foreach(QString filename, files)
    {
        QFileInfo fi(filename);
        QString baseName = fi.baseName();
        if (DatabaseInfo::IsBook(filename))
        {
            baseName += tr(" (Book)");
        }
        baseNames.append(baseName);
    }

    QStringList allFiles;
    allFiles << tr("Database") << tr("Filter") << baseNames;
    ui->sourceSelector->insertItems(0, allFiles);
    if (allFiles.contains(current))
    {
        int index = ui->sourceSelector->findText(current);
        ui->sourceSelector->setCurrentIndex(index);
    }
    else
    {
        ui->sourceSelector->setCurrentIndex(0);
    }
    connect(ui->sourceSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(slotSourceChanged()));
}

bool OpeningTreeWidget::shouldAddMove() const
{
    return ui->makeMove->isChecked();
}

void OpeningTreeWidget::slotSourceChanged()
{
    m_UndoStack->clear();
    m_openingTree->cancel();
    emit signalSourceChanged();
}
