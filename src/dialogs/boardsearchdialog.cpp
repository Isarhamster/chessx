/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "boardsearchdialog.h"
#include "ui_boardsearchdialog.h"
#include "filter.h"
#include "settings.h"

#include <QPushButton>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

BoardSearchDialog::BoardSearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoardSearchDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Find"));

    restoreLayout();

    ui->boardView->configure();
    ui->boardView->setFlags(BoardView::IgnoreSideToMove | BoardView::SuppressGuessMove);
    ui->boardView->showMoveIndicator(false);
    ui->boardView->setEnabled(false);

    ui->modeCombo->addItem(tr("Find in current filter"), FilterOperator::And);
    ui->modeCombo->addItem(tr("Search whole database"), FilterOperator::NullOperator);
    ui->modeCombo->addItem(tr("Add to current filter"), FilterOperator::Or);
    ui->modeCombo->addItem(tr("Remove from current filter"), FilterOperator::Remove);

    connect(ui->btLeft, SIGNAL(clicked()), SLOT(showPrevBoard()));
    connect(ui->btRight, SIGNAL(clicked()), SLOT(showNextBoard()));

    m_currentBoardIndex = 0;
}

BoardSearchDialog::~BoardSearchDialog()
{
    delete ui;
}
void BoardSearchDialog::restoreLayout()
{
    AppSettings->layout(this);
}

void BoardSearchDialog::showPrevBoard()
{
    m_currentBoardIndex--;
    if (m_currentBoardIndex < 0)
    {
        m_currentBoardIndex = m_boardList.size() - 1;
    }
    setCurrentBoard();
}

void BoardSearchDialog::showNextBoard()
{
    m_currentBoardIndex++;
    if (m_currentBoardIndex >= m_boardList.size())
    {
        m_currentBoardIndex = 0;
    }
    setCurrentBoard();
}

int BoardSearchDialog::mode() const
{
    if(ui->modeCombo->currentIndex() == -1)
    {
        return 0;
    }
    return ui->modeCombo->itemData(ui->modeCombo->currentIndex()).toInt();
}

void BoardSearchDialog::accept()
{
    AppSettings->setLayout(this);
    QDialog::accept();
}

void BoardSearchDialog::reject()
{
    AppSettings->setLayout(this);
    QDialog::reject();
}

void BoardSearchDialog::setCurrentBoard()
{
    const Board& b = m_boardList.at(m_currentBoardIndex);
    ui->boardView->setBoard(b);
}

void BoardSearchDialog::setBoardList(QList<Board> list)
{
    m_boardList = list;
    ui->boardView->setBoard(list.at(0));
    m_currentBoardIndex = 0;
    ui->btLeft->setEnabled(m_boardList.size()>1);
    ui->btRight->setEnabled(m_boardList.size()>1);
}

int BoardSearchDialog::boardIndex() const
{
    return m_currentBoardIndex;
}
