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
}

GameWindow::~GameWindow()
{
    delete ui;
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

ChessBrowser *GameWindow::browser()
{
    return ui->chessBrowser;
}

void GameWindow::addActionAtPage(int page, QAction* action)
{
    QWidget* boxPage = ui->toolBox->widget(page);
    QToolButton* toolButton = new QToolButton(boxPage);
    toolButton->setDefaultAction(action);
    toolButton->setFixedSize(QSize(32,32));
    toolButton->setStyleSheet("QToolButton { font-size: 12pt; }");
    boxPage->layout()->addWidget(toolButton);
}
