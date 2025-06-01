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
    ui->boardView->setFlags(ui->boardView->flags() | BoardView::AllowCustomBackground);

    connect(boardView(), SIGNAL(signalFlipped(bool,bool)), SLOT(boardIsFlipped(bool,bool)));
    setMouseTracking(true);
    showTime(false);
}

BoardViewEx::~BoardViewEx()
{
    ui->timeTop->StopCountDown();
    ui->timeBottom->StopCountDown();
    delete ui;
}

void BoardViewEx::resizeEvent(QResizeEvent* e)
{
    QWidget::resizeEvent(e);
    updateBackground();
}

void BoardViewEx::updateBackground()
{
    bool backgroundEnabled = AppSettings->getValue("/Board/Background").toBool();

    if (backgroundEnabled) {
        QPixmap originalBackground = AppSettings->getPixmap("background.jpg");

        // Pre-scale background to widget size, preserving aspect ratio
        QSize winSize = size();
        if (!winSize.isEmpty()) {
            auto pixmapRatio = (float)originalBackground.width() / originalBackground.height();
            auto windowRatio = (float)winSize.width() / winSize.height();

            int drawWidth = winSize.width(), drawHeight = winSize.height();
            if (pixmapRatio > windowRatio) {
                drawWidth = (int)(winSize.height() * pixmapRatio);
            } else {
                drawHeight = (int)(winSize.width() / pixmapRatio);
            }

            scaledBackground = originalBackground.scaled(drawWidth, drawHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
    } else {
        scaledBackground = QPixmap();
    }

    update();
}

void BoardViewEx::paintEvent(QPaintEvent *pe)
{
    if (AppSettings->getValue("/Board/Background").toBool())
    {
        QPainter painter(this);

        auto winSize = size();
        auto pixmapSize = scaledBackground.size();

        int x = (winSize.width() - pixmapSize.width()) / 2;
        int y = (winSize.height() - pixmapSize.height()) / 2;

        painter.drawPixmap(x, y, scaledBackground);
    }

    QWidget::paintEvent(pe);
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
    lcd->repaint();
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

void BoardViewEx::slotReconfigure()
{
    AppSettings->layout(this);
    AppSettings->layout(ui->annotationSplitter);
    updateBackground();
}

void BoardViewEx::saveConfig()
{
    AppSettings->setLayout(this);
    AppSettings->setLayout(ui->annotationSplitter);
}

