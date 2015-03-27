/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "common.h"
#include "ficsconsole.h"
#include "ficsclient.h"
#include "settings.h"

#include "ui_ficsconsole.h"

#include <QButtonGroup>
#include <QMenu>
#include <QToolButton>
#include <QString>

FicsConsole::FicsConsole(QWidget *parent, FicsClient* ficsClient) :
    QWidget(parent),
    ui(new Ui::FicsConsole),
    m_ficsClient(ficsClient),
    gameMode(false),
    puzzleMode(false),
    m_bWhiteToMove(true),
    m_prevTab(TabMessage)
{
    setObjectName("FicsConsole");

    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(TabMessage);

    m_countDownTimer = new QTimer(this);
    m_countDownTimer->setInterval(1000);
    m_countDownTimer->setSingleShot(true);
    connect(m_countDownTimer, SIGNAL(timeout()), SLOT(SlotCountDownGameTime()));

    ui->tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tabWidget, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(SlotContextMenu(const QPoint&)));
    connect(ui->tabWidget, SIGNAL(tabBarClicked(int)), SLOT(SlotTabClicked(int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(SlotTabChanged(int)));

    btgSeek = new QButtonGroup(this);
    btgSeek->addButton(ui->btLightning);
    btgSeek->addButton(ui->btBlitz);
    btgSeek->addButton(ui->btStandard);
    connect(btgSeek, SIGNAL(buttonClicked(int)), SLOT(SlotSeekTimeChanged(int)));

    connect(ui->textOut, SIGNAL(editingFinished()), SLOT(SendCommand()));
    connect(m_ficsClient, SIGNAL(receivedMessage(int,QString)), SLOT(HandleMessage(int,QString)), Qt::QueuedConnection);
    connect(m_ficsClient, SIGNAL(receivedBoard(int, QString)), SLOT(HandleBoard(int, QString)), Qt::QueuedConnection);
    connect(m_ficsClient, SIGNAL(commandStarted(int)), SLOT(CommandStarted(int)), Qt::QueuedConnection);
    connect(m_ficsClient, SIGNAL(disconnected()), SLOT(Disconnected()), Qt::QueuedConnection);

    connect(ui->listGames, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleObserveRequest(QListWidgetItem*)));
    connect(ui->listHistory, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleExamineRequest(QListWidgetItem*)));
    connect(ui->listRelay, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleRelayRequest(QListWidgetItem*)));
    connect(ui->listPuzzle, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleTacticsRequest(QListWidgetItem*)));
    connect(ui->listPlayers, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(HandleHistoryRequest(QTableWidgetItem*)));
    connect(ui->listSeeks, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleSeekRequest(QListWidgetItem*)));

    QListWidgetItem* item = new QListWidgetItem(tr("Get Mate"));
    item->setData(Qt::UserRole, "gm");
    ui->listPuzzle->addItem(item);
    item = new QListWidgetItem(tr("Get Study"));
    item->setData(Qt::UserRole, "gs");
    ui->listPuzzle->addItem(item);
    item = new QListWidgetItem(tr("Get Tactics"));
    item->setData(Qt::UserRole, "gt");
    ui->listPuzzle->addItem(item);

    QToolButton* button = new QToolButton(this);
    button->setText(tr("Accept"));
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->btBoxMessage->addButton(button, QDialogButtonBox::ActionRole);
    connect(button, SIGNAL(clicked()), SLOT(SlotSendAccept()));

    button = new QToolButton(this);
    button->setText(tr("Draw"));
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->btBoxMessage->addButton(button, QDialogButtonBox::ActionRole);
    connect(button, SIGNAL(clicked()), this, SLOT(SlotSendDraw()));

    button = new QToolButton(this);
    button->setText(tr("Decline"));
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->btBoxMessage->addButton(button, QDialogButtonBox::ActionRole);
    connect(button, SIGNAL(clicked()), this, SLOT(SlotSendDecline()));

    button = new QToolButton(this);
    button->setText(tr("Abort"));
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->btBoxMessage->addButton(button, QDialogButtonBox::ActionRole);
    connect(button, SIGNAL(clicked()), this, SLOT(SlotSendAbort()));

    button = new QToolButton(this);
    button->setText(tr("Resign"));
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->btBoxMessage->addButton(button, QDialogButtonBox::ActionRole);
    connect(button, SIGNAL(clicked()), this, SLOT(SlotSendResign()));

    button = new QToolButton(this);
    button->setText(tr("Hint"));
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->btBoxPuzzle->addButton(button, QDialogButtonBox::ActionRole);
    connect(button, SIGNAL(clicked()), this, SLOT(SlotSendHint()));

    button = new QToolButton(this);
    button->setText(tr("Cancel"));
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->btBoxPuzzle->addButton(button, QDialogButtonBox::ActionRole);
    connect(button, SIGNAL(clicked()), SLOT(SlotSendUnexamine()));

    connect(ui->btSeek, SIGNAL(clicked()), SLOT(SlotSendSeek()));

    connect(ui->sayMessage, SIGNAL(returnPressed()), SLOT(SlotSayMessage()));
    connect(ui->btSay, SIGNAL(clicked()), SLOT(SlotSayMessage()));

    connect(ui->btAddNoPlay, SIGNAL(clicked()), SLOT(SlotAddNoPlay()));
    connect(ui->editNoPlay, SIGNAL(returnPressed()), SLOT(SlotAddNoPlay()));

#ifndef FICS_DEBUG
    ui->line->setVisible(false);
    ui->textOut->setEnabled(false);
    ui->textOut->setVisible(false);
#endif
}

FicsConsole::~FicsConsole()
{
    delete ui;
}

void FicsConsole::Terminate()
{
    if (m_ficsClient)
    {
        disconnect(m_ficsClient, SIGNAL(receivedMessage(int,QString)));
        disconnect(m_ficsClient, SIGNAL(receivedBoard(int, QString)));
        disconnect(m_ficsClient, SIGNAL(commandStarted(int)));
        disconnect(m_ficsClient, SIGNAL(disconnected()));
        m_ficsClient->exitSession();
    }
}

void FicsConsole::SendCommand()
{
    if (!m_ficsClient) return;
    QString line = ui->textOut->text();
    if (m_ficsClient && !line.isEmpty())
    {
        m_ficsClient->sendCommand(line);
        ui->textOut->clear();
    }
}

void FicsConsole::HandleBoard(int cmd, QString s)
{
    m_bWhiteToMove = !m_bWhiteToMove;

    emit ReceivedBoard(cmd, s);
    if (cmd == FicsClient::BLKCMD_EXAMINE || cmd == FicsClient::BLKCMD_FORWARD)
    {
        m_ficsClient->sendCommand("forward");
    }
}

void FicsConsole::HandleExamineRequest(QListWidgetItem* item)
{
    if (!m_ficsClient) return;

    m_ficsClient->sendCommand("unexamine");
    m_ficsClient->sendCommand("unobserve");
    QString s = item->text();

    int n = s.section(':',0,0).toInt();
    if (n)
    {
        if (m_lastHistoryPlayer.isEmpty())
        {
            m_lastHistoryPlayer = AppSettings->getValue("/FICS/userName").toString();
        }
        QString request = "examine " + m_lastHistoryPlayer + " " + QString::number(n);
        m_ficsClient->sendCommand(request);
    }
}

void FicsConsole::HandleRelayRequest(QListWidgetItem* item)
{
    if (!m_ficsClient) return;

    m_ficsClient->sendCommand("unexamine");
    m_ficsClient->sendCommand("unobserve");
    QString s = item->text();
    int n = s.section(' ',0,0).toInt();
    if (n)
    {
        QString request = "observe " + QString::number(n);
        m_ficsClient->sendCommand(request);
    }
}

void FicsConsole::HandleTacticsRequest(QListWidgetItem* item)
{
    if (!m_ficsClient) return;

    m_ficsClient->sendCommand("unexamine");
    m_ficsClient->sendCommand("unobserve");

    QString cmd = item->data(Qt::UserRole).toString();

    QString request = "tell puzzlebot " + cmd;
    ui->listPuzzlebotMessages->clear();
    ui->listPuzzlebotMessages->addItem("Requesting puzzle...");
    puzzleMode = true;
    emit RequestNewGame();
    m_ficsClient->sendCommand(request);
}

void FicsConsole::HandleHistoryRequest(QTableWidgetItem* item)
{
    if (!m_ficsClient) return;
    if (!item->column()) return;

    QString player = item->text();
    player.remove(QRegExp("\\([^\\)]*\\)"));
    if (!player.isEmpty())
    {
        m_lastHistoryPlayer = player;
        QString request = "history " + player;
        m_ficsClient->sendCommand(request);

        QString request2 = "finger " + player;
        m_ficsClient->sendCommand(request2);
    }
}

void FicsConsole::HandleSeekRequest(QListWidgetItem* item)
{
    if (!m_ficsClient) return;

    QString s = item->text();
    QRegExp play("\\\"(play [^\\\"]*)");
    if (play.indexIn(s) >= 0)
    {
        QString seek = play.cap(1);
        m_ficsClient->sendCommand(seek);
    }
}

void FicsConsole::SendMove(QString m)
{
    if (gameMode || puzzleMode)
    {
        m_ficsClient->sendCommand(m);
        if (gameMode)
        {
            m_ficsClient->sendCommand("time");
        }
    }
}

void FicsConsole::SlotSeekTimeChanged(int)
{
    ui->listSeeks->clear();
    m_ficsClient->sendCommand("sought");
}

QString FicsConsole::FormatTime(QString s) const
{
    QString result;
    QTime t = QTime::fromString(s,"h:m:ss");

    if (t.isValid())
    {
        t = t.addSecs(-1);
        result = t.toString("h:mm:ss");
    }
    else
    {
        t = QTime::fromString(s,"m:ss");
        if (t.isValid())
        {
            if (abs(t.secsTo(QTime(0,0,0,0)))>1)
            {
                t = t.addSecs(-1);
            }
            else
            {
                t = QTime(0,0,0,0);
            }
            result = t.toString("m:ss");
        }
    }
    return result;
}

void FicsConsole::SlotCountDownGameTime()
{
    if (gameMode)
    {
        if (m_bWhiteToMove)
        {
            QString w = ui->timeWhite->text();
            ui->timeWhite->setText(FormatTime(w));
        }
        else
        {
            QString b = ui->timeBlack->text();
            ui->timeBlack->setText(FormatTime(b));
        }
        m_countDownTimer->start();
    }
}

void FicsConsole::SlotSendAccept()
{
   m_ficsClient->sendCommand("accept");
}

void FicsConsole::SlotSendDraw()
{
   m_ficsClient->sendCommand("draw");
}

void FicsConsole::SlotSendDecline()
{
    m_ficsClient->sendCommand("decline");
}

void FicsConsole::SlotSendAbort()
{
    m_ficsClient->sendCommand("abort");
}

void FicsConsole::SlotSendResign()
{
    m_ficsClient->sendCommand("resign");
}

void FicsConsole::SlotSendHint()
{
    m_ficsClient->sendCommand("tell puzzlebot hint");
}

void FicsConsole::SlotSendUnexamine()
{
    m_ficsClient->sendCommand("unexamine");
}

void FicsConsole::SlotSendSeek()
{
    int t = ui->seekTime->value();
    int inc = ui->seekIncrement->value();
    if (t || inc)
    {
        m_ficsClient->sendCommand(QString("seek %1 %2 %3")
            .arg(t)
            .arg(inc)
            .arg(ui->cbRated->currentIndex() ? "unrated" : "rated"));
    }
}

void FicsConsole::SlotSayMessage()
{
    QString msg = ui->sayMessage->text();
    m_ficsClient->sendCommand(QString("say %1").arg(msg));
    ui->sayMessage->clear();
    ui->textIn->appendHtml(QString("<i>%1</i>").arg(msg));
}

void FicsConsole::SlotAddNoPlay()
{
    QString msg = ui->editNoPlay->text();
    m_ficsClient->sendCommand(QString("+noplay %1").arg(msg));
    ui->editNoPlay->clear();
    ui->listNoPlay->clear();
    m_ficsClient->sendCommand("=noplay");
}

void FicsConsole::SlotContextMenu(const QPoint &pos)
{
    if (!gameMode)
    {
        QMenu headerMenu;
        QAction* closeFics = headerMenu.addAction(tr("Disconnect"));

        QAction* selectedItem = headerMenu.exec(mapToGlobal(pos));
        if (selectedItem == closeFics)
        {
            emit RequestCloseFICS();
        }
    }
}

void FicsConsole::SlotGameModeChanged(bool newMode)
{
    gameMode = newMode;
}

void FicsConsole::HandleObserveRequest(QListWidgetItem* item)
{
    if (!m_ficsClient) return;

    m_ficsClient->sendCommand("unexamine");
    m_ficsClient->sendCommand("unobserve");
    QString s = item->text();
    int n = s.section(' ',0,0).toInt(); // The observer id is right at the start of the string
    if (n)
    {
        QString request = "observe " + QString::number(n);
        m_ficsClient->sendCommand(request);
    }
}

void FicsConsole::CommandStarted(int cmd)
{
    switch (cmd)
    {
    case FicsClient::BLKCMD_HISTORY:
        ui->listHistory->clear();
        ui->tabWidget->setCurrentIndex(TabHistory);
        break;
    case FicsClient::BLKCMD_FINGER:
        ui->listFinger->clear();
        ui->tabWidget->setCurrentIndex(TabHistory);
        break;
    case FicsClient::BLKCMD_GAMES:
        ui->listGames->clear();
        break;
    case FicsClient::BLKCMD_XTELL:
        ui->listRelay->clear();
        break;
    case FicsClient::BLKCMD_WHO:
        break;
    }
}

void FicsConsole::SetPlayerListItemsFromLine(QString s)
{
    if (s.contains("players displayed"))
        return;

    QRegExp sep("(\\s+|\\^|~|:|&|#|\\.)");
    QStringList l = s.split(sep);
    ui->listPlayers->setSortingEnabled(false);
    for (int i=0; i<l.count()-1; i+=2)
    {
        QString rating = l[i];
        QString user = l[i+1];
        if (!rating.startsWith("-"))
        {
            if (rating.length()==3)
            {
                rating.prepend(" ");
            }
        }
        int n = ui->listPlayers->rowCount();
        ui->listPlayers->setRowCount(n+1);

        QTableWidgetItem* item = new QTableWidgetItem(rating);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->listPlayers->setItem(n,0,item);

        item = new QTableWidgetItem(user);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->listPlayers->setItem(n,1,item);

        ui->listPlayers->scrollToBottom();
    }
    ui->listPlayers->setSortingEnabled(true);
}

void FicsConsole::Disconnected()
{
    ui->textIn->appendPlainText(tr("Disconnected"));
    ui->tabWidget->setCurrentIndex(TabMessage);
    m_lastHistoryPlayer.clear();
    ui->listNoPlay->clear();
    ui->listSeeks->clear();
    ui->listGames->clear();
    ui->listPlayers->clearContents();
    ui->listPuzzlebotMessages->clear();
    gameMode = false;
    puzzleMode = false;
}

void FicsConsole::SlotTabChanged(int tab)
{
    if (!m_ficsClient) return;

    switch (m_prevTab)
    {
    case TabSeeks:
        m_ficsClient->sendCommand("set seek 0");
        break;
    default:
        break;
    }

    m_prevTab = tab;
}

void FicsConsole::SlotTabClicked(int tab)
{
    if (!m_ficsClient) return;

    switch (tab)
    {
    case TabMessage:
        break;
    case TabHistory:
        if (m_lastHistoryPlayer.isEmpty())
        {
            // Query own games unless something else is already displayed
            m_ficsClient->sendCommand("history");
        }
        break;
    case TabGames:
        m_ficsClient->sendCommand("games /blus");
        break;
    case TabRelay:
        ui->listRelay->clear();
        ui->listRelay->addItem(tr("Retrieving relayed games..."));
        m_ficsClient->sendCommand("xtell relay listgames");
        break;
    case TabPlayers:
        ui->listPlayers->clearContents();
        ui->listPlayers->setRowCount(0);
        m_ficsClient->sendCommand("who");
        break;
    case TabSeeks:
        ui->listSeeks->clear();
        m_ficsClient->sendCommand("sought");
        m_ficsClient->sendCommand("set seek 1");
        if (ui->listNoPlay->count()==0)
        {
            m_ficsClient->sendCommand("=noplay");
        }
        break;
    }
}

void FicsConsole::HandleMessage(int blockCmd,QString s)
{
    s = s.trimmed();
    if (!s.isEmpty())
    {
        qDebug() << "Cmd " << blockCmd << ": " << s;
        switch (blockCmd)
        {
        case FicsClient::BLKCMD_TIME:
            {
                m_countDownTimer->stop();
                QRegExp wReg("White Clock :([^\\.]*)");
                if (wReg.indexIn(s) >= 0)
                {
                    QString w = wReg.cap(1).trimmed();
                    ui->timeWhite->setText(w);
                }
                QRegExp bReg("Black Clock :([^\\.]*)");
                if (bReg.indexIn(s) >= 0)
                {
                    QString b = bReg.cap(1).trimmed();
                    ui->timeBlack->setText(b);
                }
                m_countDownTimer->start();
                break;
            }
        case FicsClient::BLKCMD_HISTORY:
            {
                QRegExp typeOfGame("\\[([^\\]]+)\\]");
                int pos = typeOfGame.indexIn(s);
                if(pos >= 0)
                {
                    QString segt = typeOfGame.cap(1).trimmed();
                    if (segt.startsWith('b') || segt.startsWith('l') || segt.startsWith('s') || segt.startsWith('u'))
                    {
                        ui->listHistory->addItem(s);
                        ui->listHistory->scrollToBottom();
                    }
                }
            }
            break;
        case FicsClient::BLKCMD_FINGER:
            {
                ui->listFinger->addItem(s);
            }
            break;
        case FicsClient::BLKCMD_GAMES:
            ui->listGames->addItem(s);
            ui->listGames->scrollToBottom();
            break;
        case FicsClient::BLKCMD_XTELL:
            if (!s.startsWith("(") && s.length() > 1)
            {
                QStringList l = s.split(" ", QString::SkipEmptyParts);
                if (l.length() == 5)
                {
                    // todo: Convert List to table widget
                    ui->listRelay->addItem(s);
                    ui->listRelay->scrollToBottom();
                }
                else if (s.startsWith("There are"))
                {
                    ui->listRelay->addItem(s);
                    ui->listRelay->scrollToBottom();
                }
                else if (s.contains("tell relay next"))
                {
                    m_ficsClient->sendCommand("xtell relay next");
                }
            }
            break;
        case FicsClient::BLKCMD_WHO:
            {
                SetPlayerListItemsFromLine(s);
                break;
            }
        case FicsClient::BLKCMD_SEEK:
            {
                QString seek = btgSeek->checkedButton()->objectName().remove(0,2).toLower();
                if (s.contains(seek))
                {
                    ui->listSeeks->addItem(s);
                    ui->listSeeks->scrollToBottom();
                }
            }
            break;
        case FicsClient::BLKCMD_SOUGHT:
            {
                QStringList l = s.split(" ",QString::SkipEmptyParts);
                if (l.size() >= 8)
                {
                    QString sought = btgSeek->checkedButton()->objectName().remove(0,2).toLower();
                    if (l[6].contains(sought))
                    {
                        QString spec;
                        for (int i=5;i<l.size();++i)
                        {
                            spec.append(l[i]);
                            spec.append(" ");
                        }
                        QString seek = QString("%1 (%2) sought %3 %4 %5(\"play %6\" to respond)").
                                     arg(l[2]).arg(l[1]).arg(l[3]).arg(l[4]).arg(spec).arg(l[0]);
                        ui->listSeeks->addItem(seek);
                        ui->listSeeks->scrollToBottom();
                    }
                }
            }
            break;
        case FicsClient::BLKCMD_INTERNAL_MATCH_START:
            {
                gameMode = true;
                ui->timeWhite->setText(QString());
                ui->timeBlack->setText(QString());
                m_bWhiteToMove = false;
                emit RequestNewGame();
                emit RequestGameMode(gameMode);
                ui->textIn->appendPlainText(s);
                ui->tabWidget->setCurrentIndex(TabMessage);
                QStringList l = s.split(' ');
                if (l.length() > 2)
                {
                    QString w = l[2].remove('(');
                    bool bPlayerIsBlack = w != m_ficsClient->getGuestName();
                    emit SignalPlayerIsBlack(bPlayerIsBlack);
                }
            }
            break;
        case FicsClient::BLKCMD_INTERNAL_MATCH_END:
            {
                gameMode = false;
                m_countDownTimer->stop();
                emit RequestGameMode(gameMode);
                ui->textIn->appendPlainText(s);
                emit SignalGameResult(s);
                emit RequestSaveGame();
            }
            break;
        case FicsClient::BLKCMD_SHOWLIST:
            if (!s.contains("--"))
            {
                QStringList l = s.split(" ", QString::SkipEmptyParts);
                foreach(QString name, l)
                {
                    ui->listNoPlay->addItem(name);
                }
            }
            break;
        case FicsClient::BLKCMD_SAY:
            s.remove(QRegExp("[^:]*:"));
            ui->textIn->appendHtml(QString("<b>%1<b>").arg(s));
            ui->tabWidget->setCurrentIndex(TabMessage);
            break;
        case FicsClient::BLKCMD_NULL:
            ui->textIn->appendPlainText(s);
            break;
        case FicsClient::BLKCMD_INTERNAL_PUZZLEBOT:
            if (s.contains("kibitzes"))
            {
                ui->timeWhite->setText(QString());
                ui->timeBlack->setText(QString());

                s.remove(QRegExp("puzzlebot[^:]*kibitzes:"));
                if (!s.contains("tell puzzlebot"))
                {
                    ui->listPuzzlebotMessages->addItem(s.trimmed());
                    ui->listPuzzlebotMessages->scrollToBottom();
                    ui->tabWidget->setCurrentIndex(TabPuzzle);
                }
                if (s.contains("solved"))
                {
                    emit RequestSaveGame();
                    puzzleMode = false;
                    SlotSendUnexamine();
                }
                else
                {
                    if (s.contains("problem number"))
                    {
                        QString event = s;
                        event.remove("This is ");
                        event.remove('[');
                        event.remove(']');
                        emit RequestAddTag(TagNameEvent, event);
                    }
                }
            }
            else if (s.contains("puzzlebot backs up 1 move"))
            {
                emit RequestRemoveLastMove();
            }
            if (!s.contains("tell puzzlebot"))
            {
                puzzleMode = true;
            }
            break;
        case FicsClient::BLKCMD_FORWARD:
            if (!s.contains("goes"))
            {
                ui->textIn->appendPlainText(s);
                ui->tabWidget->setCurrentIndex(TabMessage);
            }
            break;
        case FicsClient::BLKCMD_BACKWARD:
        case FicsClient::BLKCMD_MEXAMINE:
        case FicsClient::BLKCMD_GAME_MOVE:
        case FicsClient::BLKCMD_TELL:
        case FicsClient::BLKCMD_OBSERVE:
        case FicsClient::BLKCMD_EXAMINE:
        case FicsClient::BLKCMD_UNOBSERVE:
        case FicsClient::BLKCMD_UNEXAMINE:
        case FicsClient::BLKCMD_SET:
            break;
        default:
            ui->textIn->appendPlainText(s);
            ui->tabWidget->setCurrentIndex(TabMessage);
            break;
        }
    }
}
