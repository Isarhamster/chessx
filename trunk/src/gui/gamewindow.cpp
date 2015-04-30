/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "editaction.h"
#include "nag.h"

#include <QToolButton>

GameWindow::GameWindow(QWidget *parent) :
    ToolMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    setObjectName("GameWindow");

    setupToolBox();

    connect(ui->listVariations, SIGNAL(clicked(QModelIndex)),
            this, SLOT(variationClicked(QModelIndex)));
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::variationClicked(QModelIndex index)
{
    emit enterVariation(index.row());
}

void GameWindow::setupToolBox()
{
    QMap<Nag, QAction*> tbActions;
    QList<Nag> items;
    foreach (QAction* action, ui->chessBrowser->m_actions.keys())
    {
        EditAction& e = ui->chessBrowser->m_actions[action];
        if (e.type() == EditAction::AddNag)
        {
            Nag nag = (Nag)e.data().toInt();
            tbActions[nag] = action;
            items.push_back(nag);
        }
    }

    qSort(items);

    foreach(int nag, items)
    {
        setupNagInToolBox((Nag)nag, tbActions[(Nag)nag]);
    }

    setupSpacers();

    ui->browserSplitter->setStretchFactor(0,3);
    ui->browserSplitter->setStretchFactor(1,1);

    ui->toolBoxSplitter->setStretchFactor(0,1);
    ui->toolBoxSplitter->setStretchFactor(1,3);
}

void GameWindow::setupNagInToolBox(Nag nag, QAction* action)
{
    int page = -1;
    if (NagSet::isMoveNag(nag)) page = 0;
    else if (NagSet::isEvalNag(nag)) page = 1;
    else if (NagSet::isOtherNag(nag)) page = 2;
    if (page >= 0)
    {
        QString txt = NagSet::nagToString(nag);
        action->setIconText(txt);
        addActionAtPage(page,action);
    }
}

void GameWindow::setupSpacers()
{
    for (int i=0; i<ui->toolBox->count();++i)
    {
        QWidget* boxPage = ui->toolBox->widget(i);
        QGridLayout* layout = qobject_cast<QGridLayout*>(boxPage->layout());
        int rows = layout->rowCount();
        QSpacerItem* verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        layout->addItem(verticalSpacer,rows,0);
    }
}

ChessBrowser *GameWindow::browser()
{
    return ui->chessBrowser;
}

void GameWindow::showVariations(QList<MoveId> list)
{
    QStringList variationTexts = browser()->getAnchors(list);
    ui->listVariations->clear();
    foreach(QString s, variationTexts)
    {
        ui->listVariations->addItem(s);
    }
}

void GameWindow::addActionAtPage(int page, QAction* action)
{
    QWidget* boxPage = ui->toolBox->widget(page);
    QToolButton* toolButton = new QToolButton(boxPage);
    toolButton->setDefaultAction(action);
    toolButton->setFixedSize(QSize(32,32));
    toolButton->setStyleSheet("QToolButton { font-size: 12pt; }");
    QGridLayout* layout = qobject_cast<QGridLayout*>(boxPage->layout());

    int rows = layout->rowCount();
    int widgets = layout->count();
    if (widgets % 2)
    {
        layout->addWidget(toolButton,rows-1,1);
    }
    else
    {
        layout->addWidget(toolButton,rows,0);
    }
}
