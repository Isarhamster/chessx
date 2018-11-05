/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "editaction.h"
#include "nag.h"
#include "settings.h"

#include <QAction>
#include <QToolButton>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

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

void GameWindow::saveConfig()
{
    AppSettings->setLayout(this);
    AppSettings->setLayout(ui->browserSplitter);
}

void GameWindow::slotReconfigure()
{
    AppSettings->layout(this);
    AppSettings->layout(ui->browserSplitter);
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
            Nag nag = static_cast<Nag>(e.data().toInt());
            tbActions[nag] = action;
            items.push_back(nag);
        }
    }

    qSort(items);

    foreach(Nag nag, items)
    {
        setupNagInToolBox(nag, tbActions[nag]);
    }

    setupSpacers();

    ui->browserSplitter->setStretchFactor(0,3);
    ui->browserSplitter->setStretchFactor(1,1);
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
