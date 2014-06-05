#include "boardsearchdialog.h"
#include "ui_boardsearchdialog.h"
#include "search.h"
#include "settings.h"

BoardSearchDialog::BoardSearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoardSearchDialog)
{
    ui->setupUi(this);

    ui->boardView->configure();
    ui->boardView->setFlags(BoardView::IgnoreSideToMove | BoardView::SuppressGuessMove | BoardView::AllowCopyPiece);
    ui->boardView->showMoveIndicator(false);

    connect(ui->okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
    ui->modeCombo->addItem(tr("Find in current filter"), Search::And);
    ui->modeCombo->addItem(tr("Search whole database"), Search::NullOperator);
    ui->modeCombo->addItem(tr("Add to current filter"), Search::Or);

    QTimer::singleShot(0, this, SLOT(restoreLayout()));
}

void BoardSearchDialog::restoreLayout()
{
    AppSettings->layout(this);
}

BoardSearchDialog::~BoardSearchDialog()
{
    delete ui;
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

void BoardSearchDialog::setBoard(const Board& b)
{
    ui->boardView->setBoard(b);
}
