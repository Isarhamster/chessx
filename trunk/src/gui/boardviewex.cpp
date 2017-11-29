/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "boardviewex.h"
#include "ui_boardviewex.h"
#include "settings.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

BoardViewEx::BoardViewEx(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoardViewEx)
{
    ui->setupUi(this);
    connect(boardView(), SIGNAL(signalFlipped(bool,bool)), SLOT(boardIsFlipped(bool,bool)));
    setMouseTracking(true);
    showTime(false);

    connect(ui->listVariations, SIGNAL(clicked(QModelIndex)),
            this, SLOT(variationClicked(QModelIndex)));
}

BoardViewEx::~BoardViewEx()
{
    ui->timeTop->StopCountDown();
    ui->timeBottom->StopCountDown();
    delete ui;
}

void BoardViewEx::variationClicked(QModelIndex index)
{
    emit enterVariation(index.row());
}

BoardView* BoardViewEx::boardView()
{
    return ui->boardView;
}

QObject* BoardViewEx::dbIndex()
{
    return ui->boardView->dbIndex();
}

void BoardViewEx::showTime(bool show)
{
    if (!show)
    {
        ui->timeBottom->StopCountDown();
        ui->timeTop->StopCountDown();
    }
    ui->timeBottom->setVisible(show);
    ui->timeTop->setVisible(show);
}

void BoardViewEx::configureTime(bool white, bool countDown)
{
    boardView()->setFlipped(white);
    ui->timeBottom->ResetTock(countDown, countDown);
    ui->timeTop->ResetTock(false, countDown);
}

void BoardViewEx::setTime(bool white, QString t)
{
    bool flipped = ui->boardView->isFlipped();
    bool top = (white && flipped) || (!white && !flipped);
    DigitalClock* lcd = top ? ui->timeTop : ui->timeBottom;
    lcd->StopCountDown();
    lcd->setTime(t);
}

void BoardViewEx::startTime(bool white)
{
    bool flipped = ui->boardView->isFlipped();
    bool top = (white && flipped) || (!white && !flipped);
    DigitalClock* lcd = top ? ui->timeTop : ui->timeBottom;
    lcd->StartCountDown();
}

void BoardViewEx::stopTimes()
{
    ui->timeTop->StopCountDown();
    ui->timeBottom->StopCountDown();
}

void BoardViewEx::boardIsFlipped(bool oldState, bool newState)
{
    if (oldState != newState)
    {
        QString topTime = ui->timeTop->time();
        QString bottomTime = ui->timeBottom->time();
        ui->timeTop->setTime(bottomTime);
        ui->timeBottom->setTime(topTime);
        ui->timeTop->ToggleCountDown();
        ui->timeBottom->ToggleCountDown();
    }
}

QString BoardViewEx::getComment() const
{
    return ui->editComment->toPlainText();
}

void BoardViewEx::setComment(const QString &value)
{
    if (value != ui->editComment->toPlainText())
    {
        ui->editComment->setPlainText(value);
    }
}

void BoardViewEx::slotReconfigure()
{
    AppSettings->layout(this);
    AppSettings->layout(ui->annotationSplitter);
}

void BoardViewEx::saveConfig()
{
    AppSettings->setLayout(this);
    AppSettings->setLayout(ui->annotationSplitter);
}

void BoardViewEx::on_editComment_textChanged()
{
    emit signalNewAnnotation(ui->editComment->toPlainText().toLatin1());
}

void BoardViewEx::setAnnotationPlaceholder(bool enable)
{
    if (enable)
    {
        ui->editComment->setPlaceholderText(tr("Enter comments and annotations here."));
    }
    else
    {
        ui->editComment->setPlaceholderText("");
    }
}

void BoardViewEx::showVariations(QList<MoveId> /* listVariations */, QStringList textVariations)
{
    ui->listVariations->clear();
    ui->listVariations->addItems(textVariations);
}
