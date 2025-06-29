/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "board.h"
#include "ficsconsole.h"
#include "ficsclient.h"
#include "partialdate.h"
#include "qt6compat.h"
#include "settings.h"
#include "tags.h"

#include "ui_ficsconsole.h"

#include <QButtonGroup>
#include <QCompleter>
#include <QMenu>
#include <QToolButton>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QRegularExpression>

using namespace chessx;

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

FicsConsole::FicsConsole(QWidget *parent, FicsClient* ficsClient) :
    QWidget(parent),
    ui(new Ui::FicsConsole),
    gameMode(false),
    puzzleMode(false),
    m_bWhiteToMove(true),
    m_prevTab(TabMessage)
{
    setObjectName("FicsConsole");
    m_ficsClient = ficsClient;

    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(TabMessage);

    ui->tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tabWidget, SIGNAL(customContextMenuRequested(QPoint)), SLOT(SlotContextMenu(QPoint)));
    connect(ui->tabWidget, SIGNAL(tabBarClicked(int)), SLOT(SlotTabClicked(int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(SlotTabChanged(int)));

    btgSeek = new QButtonGroup(this);
    btgSeek->addButton(ui->btLightning);
    btgSeek->addButton(ui->btBlitz);
    btgSeek->addButton(ui->btStandard);
    connect(btgSeek, SIGNAL(idClicked(int)), SLOT(SlotSeekTimeChanged(int)));

    connect(ui->textOut, SIGNAL(editingFinished()), SLOT(SendCommand()));
    connect(m_ficsClient, SIGNAL(receivedMessage(int,QString)), SLOT(HandleMessage(int,QString)), Qt::QueuedConnection);
    connect(m_ficsClient, SIGNAL(receivedBoard(int,QString)), SLOT(HandleBoard(int,QString)), Qt::QueuedConnection);
    connect(m_ficsClient, SIGNAL(commandStarted(int)), SLOT(CommandStarted(int)), Qt::QueuedConnection);
    connect(m_ficsClient, SIGNAL(disconnected()), SLOT(Disconnected()), Qt::QueuedConnection);

    connect(ui->listGames, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleObserveRequest(QListWidgetItem*)));
    connect(ui->listHistory, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleExamineRequest(QListWidgetItem*)));
    connect(ui->listRelay, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleRelayRequest(QListWidgetItem*)));
    connect(ui->listPuzzle, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleTacticsRequest(QListWidgetItem*)));
    connect(ui->listPlayers, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(HandleHistoryRequest(QTableWidgetItem*)));
    connect(ui->listSeeks, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleSeekRequest(QListWidgetItem*)));

    connect(ui->listGames, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(CMHandleObserveRequest(QPoint)));
    connect(ui->listHistory, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(CMHandleExamineRequest(QPoint)));
    connect(ui->listRelay, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(CMHandleRelayRequest(QPoint)));
    connect(ui->listPuzzle, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(CMHandleTacticsRequest(QPoint)));
    connect(ui->listPlayers, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(CMHandleHistoryRequest(QPoint)));
    connect(ui->listSeeks, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(CMHandleSeekRequest(QPoint)));

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
    button->setText(tr("Rematch"));
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->btBoxMessage->addButton(button, QDialogButtonBox::ActionRole);
    connect(button, SIGNAL(clicked()), SLOT(SlotSendRematch()));

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
    connect(ui->editNoPlay, SIGNAL(textChanged(QString)), SLOT(SlotNoPlayChanged(QString)));

    ui->btAddNoPlay->setDisabled(ui->editNoPlay->text().isEmpty());

    QStringList words;
    words << "Thanks for the game";
    QCompleter* completer = new QCompleter(words, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->sayMessage->setCompleter(completer);

    ui->seekTime->setValue(AppSettings->getValue("/FICS/minutes").toInt());
    ui->seekIncrement->setValue(AppSettings->getValue("/FICS/increment").toInt());
    ui->eloMin->setValue(AppSettings->getValue("/FICS/eloLow").toInt());
    ui->eloMax->setValue(AppSettings->getValue("/FICS/eloHigh").toInt());
}

FicsConsole::~FicsConsole()
{
    delete ui;
}

bool FicsConsole::canUsePremove() const
{
    return (gameMode && !puzzleMode);
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
    QStringList l = s.split(' ');
    m_bWhiteToMove = (l[C64_COLOR_TO_MOVE]=="W");

    bool handleBoard = true;
    if (gameMode && !puzzleMode)
    {
        Char64Relation relation = static_cast<Char64Relation>(l[C64_GAME_RELATION].toInt());
        if (m_lastRelation == relation)
        {
            handleBoard = false;
        }
        else
        {
            m_lastRelation = relation;
        }
    }

    if (handleBoard)
    {
        emit ReceivedBoard(cmd, s);
    }

    if (cmd == FicsClient::BLKCMD_EXAMINE || cmd == FicsClient::BLKCMD_FORWARD)
    {
        if (m_ficsClient) m_ficsClient->sendCommand("forward");
    }
    if (handleBoard && gameMode)
    {
        emit RequestStoredMove();
    }

    if (gameMode && !puzzleMode)
    {
        QString timeWhite = l[C64_REMAINDER_TIME_WHITE];
        QString timeBlack = l[C64_REMAINDER_TIME_BLACK];
        QTime tw(0,0,0,0);
        tw = tw.addSecs(timeWhite.toInt());
        QTime tb(0,0,0,0);
        tb = tb.addSecs(timeBlack.toInt());
        emit FicsShowTime(White, tw.toString("h:mm:ss"));
        emit FicsShowTime(Black, tb.toString("h:mm:ss"));
        emit SignalStartTime(m_bWhiteToMove);
    }
}

void FicsConsole::HandleExamineRequest(QListWidgetItem* item)
{
    if (!m_ficsClient) return;

    SlotSendUnexamine();
    m_ficsClient->sendCommand("unobserve");
    puzzleMode = false;

    QString s = item->text();

    int n = s.section(':',0,0).toInt();
    if (n)
    {
        emit RequestNewGame();
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
    SlotSendUnexamine();
    m_ficsClient->sendCommand("unobserve");
    puzzleMode = false;

    QString s = item->text();
    int n = s.section(' ',0,0).toInt();
    if (n)
    {
        emit RequestNewGame();
        QString request = "observe " + QString::number(n);
        m_ficsClient->sendCommand(request);
    }
}

void FicsConsole::HandleTacticsRequest(QListWidgetItem* item)
{
    if (!m_ficsClient) return;
    SlotSendUnexamine();
    m_ficsClient->sendCommand("unobserve");

    QString cmd = item->data(Qt::UserRole).toString();

    QString request = "tell puzzlebot " + cmd;
    ui->listPuzzlebotMessages->clear();
    ui->listPuzzlebotMessages->addItem("Requesting puzzle...");
    emit FicsShowTimer(true);
    puzzleMode = true;
    emit RequestNewGame();
    emit RequestAddTag(TagNameDate, PartialDate::today().asString());
    if (AppSettings->getValue("/Board/noHints").toBool())
    {
        emit RequestGameMode(true);
    }
    m_ficsClient->sendCommand(request);
}

void FicsConsole::HandleHistoryRequest(QTableWidgetItem* item)
{
    if (!m_ficsClient) return;
    if (!item->column()) return;

    QString player = item->text();
    player.remove(QRegularExpression("\\([^\\)]*\\)"));
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
    QRegularExpression play("\\\"(play [^\\\"]*)");
    QRegularExpressionMatch match;
    if (s.indexOf(play, 0, &match) >= 0)
    {
        QString seek = match.captured(1);
        SlotSendUnexamine();
        m_ficsClient->sendCommand("unobserve");
        m_ficsClient->sendCommand(seek);
    }
}

void FicsConsole::SendMove(QString m)
{
    if (gameMode || puzzleMode)
    {
        m_ficsClient->sendCommand(m);
    }
}

void FicsConsole::SlotSeekTimeChanged(int)
{
    ui->listSeeks->clear();
    if (m_ficsClient) m_ficsClient->sendCommand("sought");
}

QString FicsConsole::DecrementTime(QString s) const
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

void FicsConsole::SlotSendAccept()
{
   if (m_ficsClient) m_ficsClient->sendCommand("accept");
}

void FicsConsole::SlotSendDraw()
{
   if (m_ficsClient) m_ficsClient->sendCommand("draw");
}

void FicsConsole::SlotSendDecline()
{
    if (m_ficsClient) m_ficsClient->sendCommand("decline");
}

void FicsConsole::SlotSendAbort()
{
    if (m_ficsClient) m_ficsClient->sendCommand("abort");
}

void FicsConsole::SlotSendResign()
{
    if (m_ficsClient) m_ficsClient->sendCommand("resign");
}

void FicsConsole::SlotSendHint()
{
    if (m_ficsClient) m_ficsClient->sendCommand("tell puzzlebot hint");
}

void FicsConsole::SlotSendUnexamine()
{
    if (gameMode)
    {
        emit RequestGameMode(false);
    }
    if (m_ficsClient) m_ficsClient->sendCommand("unexamine");
}

void FicsConsole::SlotSendRematch()
{
    if (!gameMode)
    {
        if (m_ficsClient) m_ficsClient->sendCommand("rematch");
    }
}

void FicsConsole::SlotSendSeek()
{
    int t = ui->seekTime->value();
    int inc = ui->seekIncrement->value();
    int from = ui->eloMin->value();
    int to = ui->eloMax->value();
    QString color;
    if (ui->cbColor->currentIndex() == 0)
    {
        color = " w";
    }
    else if (ui->cbColor->currentIndex() == 1)
    {
        color = " b";
    }
    if ((t || inc) && (from<=to))
    {
        QString seek = QString("seek %1 %2 %3%4 %5-%6 ")
                .arg(t)
                .arg(inc)
                .arg(ui->cbRated->currentIndex() ? "unrated" : "rated", color)
                .arg(from)
                .arg(to);
        QListWidgetItem* item = new QListWidgetItem(seek);
        item->setForeground(QBrush(Qt::gray));
        ui->listSeeks->addItem(item);
        if (m_ficsClient) m_ficsClient->sendCommand(seek);
    }
}

void FicsConsole::SlotSayMessage()
{
    QString msg = ui->sayMessage->text();
    if (m_ficsClient) m_ficsClient->sendCommand(QString("say %1").arg(msg));
    ui->sayMessage->clear();
    ui->textIn->appendHtml(QString("<i>%1</i>").arg(msg));
    UpdateSayCompleter(msg);
}

void FicsConsole::UpdateSayCompleter(QString msg)
{
    QCompleter* completer = ui->sayMessage->completer();
    QStringListModel* model = qobject_cast<QStringListModel*>(completer->model());
    QStringList words = model->stringList();
    if (!words.contains(msg))
    {
        words.append(msg);
        QCompleter* nc = new QCompleter(words);
        ui->sayMessage->setCompleter(nc);
    }
}

Color FicsConsole::playerColor() const
{
    if (gameMode)
    {
        return m_bPlayerIsBlack ? Black : White;
    }
    return NoColor;
}

void FicsConsole::SlotAddNoPlay()
{
    QString msg = ui->editNoPlay->text();
    if (m_ficsClient) m_ficsClient->sendCommand(QString("+noplay %1").arg(msg));
    ui->editNoPlay->clear();
    ui->listNoPlay->clear();
    if (m_ficsClient) m_ficsClient->sendCommand("=noplay");
}

void FicsConsole::SlotNoPlayChanged(const QString& s)
{
    ui->btAddNoPlay->setDisabled(s.isEmpty());
}

void FicsConsole::SlotContextMenu(const QPoint &pos)
{
    QWidget* w = ui->tabWidget->childAt(pos);
    QListView* v = nullptr;
    if (w)
    {
        v = qobject_cast<QListView*>(w->parentWidget());
    }

    if (!v)
    {
        QMenu headerMenu;
        QAction* closeFics = headerMenu.addAction(tr("Disconnect"));
        closeFics->setDisabled(gameMode);
        QAction* selectedItem = headerMenu.exec(mapToGlobal(pos));
        if (!gameMode && (selectedItem == closeFics))
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
    SlotSendUnexamine();
    m_ficsClient->sendCommand("unobserve");
    puzzleMode = false;

    QString s = item->text();
    int n = s.section(' ',0,0).toInt(); // The observer id is right at the start of the string
    if (n)
    {
        emit RequestNewGame();
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

    QRegularExpression sep("(\\s+|\\^|~|:|&|#|\\.)");
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
    ui->textIn->ensureCursorVisible();
    ui->tabWidget->setCurrentIndex(TabMessage);
    m_lastHistoryPlayer.clear();
    ui->listNoPlay->clear();
    ui->listSeeks->clear();
    ui->listGames->clear();
    ui->listPlayers->clearContents();
    ui->listPuzzlebotMessages->clear();
    if (gameMode)
    {
        emit RequestGameMode(false);
    }
    emit FicsShowTimer(false);
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
    if (!s.isEmpty())
    {
        // qDebug() << "Cmd " << blockCmd << ": " << s;
        switch (blockCmd)
        {
        case FicsClient::BLKCMD_TIME:
            {
                if (s.startsWith("Game"))
                {
                    QStringList l = s.split(" ",SkipEmptyParts);
                    if (l.size()>=6)
                    {
                        QString nameWhite = l[2];
                        QString nameBlack = l[4];
                        emit RequestAddTag(TagNameWhite, nameWhite);
                        emit RequestAddTag(TagNameBlack, nameBlack);
                        emit RequestAddTag(TagNameDate, PartialDate::today().asString());
                        int ratingWhite = l[3].remove("(").remove(")").toInt();
                        int ratingBlack = l[5].remove("(").remove(")").toInt();
                        if (ratingWhite) emit RequestAddTag(TagNameWhiteElo, QString::number(ratingWhite));
                        if (ratingBlack) emit RequestAddTag(TagNameBlackElo, QString::number(ratingBlack));
                    }
                }
                else
                {
                    QRegularExpression wReg("White Clock :([^\\.]*)");
                    QRegularExpressionMatch match;
                    if (s.indexOf(wReg, 0, &match) >= 0)
                    {
                        QString w = match.captured(1).trimmed();
                        emit FicsShowTime(White, w);
                    }
                    QRegularExpression bReg("Black Clock :([^\\.]*)");
                    if (s.indexOf(bReg, 0, &match) >= 0)
                    {
                        QString b = match.captured(1).trimmed();
                        emit FicsShowTime(Black, b);
                    }
                }
                break;
            }
        case FicsClient::BLKCMD_HISTORY:
            {
                QRegularExpression typeOfGame("\\[([^\\]]+)\\]");
                QRegularExpressionMatch match;
                int pos = s.indexOf(typeOfGame, 0, &match);
                if(pos >= 0)
                {
                    QString segt = match.captured(1).trimmed();
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
                QStringList l = s.split(" ", SkipEmptyParts);
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
                    if (m_ficsClient) m_ficsClient->sendCommand("xtell relay next");
                }
		else
		{
		    ui->textIn->appendPlainText(s);
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
                    if (ui->disableComputer->isChecked())
                    {
                        if (s.contains("(C)")) break;
                    }
                    if (ui->ratedOnly->isChecked())
                    {
                        if (s.contains("unrated")) break;
                    }
                    ui->listSeeks->addItem(s);
                    ui->listSeeks->scrollToBottom();
                }
            }
            break;
        case FicsClient::BLKCMD_SOUGHT:
            {
                QStringList l = s.split(" ",SkipEmptyParts);
                if (l.size() >= 8)
                {
                    QString sought = btgSeek->checkedButton()->objectName().remove(0,2).toLower();
                    if (l[6].contains(sought))
                    {
                        if (ui->disableComputer->isChecked())
                        {
                            if (l[2].contains("(C)")) break;
                        }
                        if (ui->ratedOnly->isChecked())
                        {
                            if (l[5].contains("unrated")) break;
                        }
                        QString spec;
                        for (int i=5;i<l.size();++i)
                        {
                            spec.append(l[i]);
                            spec.append(" ");
                        }
                        QString seek = QString("%1 (%2) sought %3 %4 %5(\"play %6\" to respond)").
                                     arg(l[2], l[1], l[3], l[4], spec, l[0]);
                        ui->listSeeks->addItem(seek);
                        ui->listSeeks->scrollToBottom();
                    }
                }
            }
            break;
        case FicsClient::BLKCMD_INTERNAL_MATCH_START:
            {
                gameMode = true;
                puzzleMode = false; // Better safe than sorry -> puzzleMode breaks a lot of things
                if (m_ficsClient) m_ficsClient->sendCommand("time");
                m_lastRelation = C64_REL_ISOLATED; // Anything invalid in this context
                emit FicsShowTimer(true);
                m_bWhiteToMove = true;
                emit RequestNewGame();
                emit RequestGameMode(gameMode);
                ui->textIn->appendPlainText(s);
                ui->textIn->ensureCursorVisible();
                ui->tabWidget->setCurrentIndex(TabMessage);
                QStringList l = s.split(' ');
                if (l.length() > 2)
                {
                    QString w = l[2].remove('(');
                    if (m_ficsClient) m_bPlayerIsBlack = w != m_ficsClient->getGuestName();
                    emit SignalPlayerIsBlack(m_bPlayerIsBlack, true);
                }
            }
            break;
        case FicsClient::BLKCMD_INTERNAL_MATCH_END:
            {
                gameMode = false;
                ui->textIn->appendPlainText(s);
                ui->textIn->ensureCursorVisible();
                emit SignalGameResult(s);
                emit RequestSaveGame();
                emit RequestGameMode(gameMode);
                emit FicsShowTimer(false);
            }
            break;
        case FicsClient::BLKCMD_INTERNAL_GAME_END:
            {
                ui->textIn->appendPlainText(s);
                ui->textIn->ensureCursorVisible();
                emit SignalGameResult(s);
                if (!puzzleMode)
                {
                    // Puzzlemode is saved a little later
                    emit RequestSaveGame();
                }
                break;
            }
        case FicsClient::BLKCMD_SHOWLIST:
            if (!s.contains("--"))
            {
                QStringList l = s.split(" ", SkipEmptyParts);
                foreach(QString name, l)
                {
                    ui->listNoPlay->addItem(name);
                }
            }
            break;
        case FicsClient::BLKCMD_SAY:
            s.remove(QRegularExpression("[^:]*:"));
            ui->textIn->appendHtml(QString("<b>%1</b>").arg(s));
            ui->tabWidget->setCurrentIndex(TabMessage);
            break;
        case FicsClient::BLKCMD_INTERNAL_SESSION_STARTED:
            emit raiseRequest();
            ui->textIn->appendPlainText(s);
            ui->textIn->ensureCursorVisible();
            if (m_ficsClient && m_ficsClient->loggedInAsGuest())
            {
                ui->cbRated->setCurrentIndex(1);
                ui->cbRated->setEnabled(false);
            }
            else
            {
                ui->cbRated->setEnabled(true);
            }
            break;
        case FicsClient::BLKCMD_NULL:
            ui->textIn->appendPlainText(s);
            ui->textIn->ensureCursorVisible();
            break;
        case FicsClient::BLKCMD_INTERNAL_PUZZLEBOT:
            if (s.contains("kibitzes"))
            {
                s.remove(QRegularExpression("puzzlebot[^:]*kibitzes:"));
                s = s.trimmed();
                if (!s.contains("tell puzzlebot"))
                {
                    ui->listPuzzlebotMessages->addItem(s.trimmed());
                    ui->listPuzzlebotMessages->scrollToBottom();
                    ui->tabWidget->setCurrentIndex(TabPuzzle);
                }
                if (s.contains("Timeout reached"))
                {
                    emit SignalGameResult("*");
                    emit RequestSaveGame();
                    if (gameMode)
                    {
                        emit RequestGameMode(false);
                    }
                    emit FicsShowTimer(false);
                    puzzleMode = false;
                    SlotSendUnexamine();
                }
                else if (s.contains("solved") && !s.contains("almost"))
                {
                    emit RequestSaveGame();
                    if (gameMode)
                    {
                        emit RequestGameMode(false);
                    }
                    emit FicsShowTimer(false);
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
                    else if (s.startsWith("Black moves"))
                    {
                        m_bPlayerIsBlack = true;
                        emit FicsShowTime(Black, "0:00");
                        emit FicsShowTime(White, "0:00");
                        emit SignalStartTime(false);
                        emit SignalPlayerIsBlack(true,false);
                    }
                    else if (s.startsWith("White moves"))
                    {
                        m_bPlayerIsBlack = false;
                        emit FicsShowTime(Black, "0:00");
                        emit FicsShowTime(White, "0:00");
                        emit SignalStartTime(true);
                        emit SignalPlayerIsBlack(false,false);
                    }
                }
            }
            else if (s.contains("puzzlebot backs up 1 move"))
            {
                emit RequestRemoveLastMove();
            }
            break;
        case FicsClient::BLKCMD_FORWARD:
            if (!s.contains("goes"))
            {
                ui->textIn->appendPlainText(s);
                ui->textIn->ensureCursorVisible();
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
            ui->textIn->ensureCursorVisible();
            ui->tabWidget->setCurrentIndex(TabMessage);
            break;
        }
    }
}

void FicsConsole::CMHandleObserveRequest(const QPoint& pos)
{
    QMenu menu;
    menu.addAction(tr("Observe"));
    if (menu.exec(ui->listGames->mapToGlobal(pos)))
    {
        QListWidgetItem* item = ui->listGames->itemAt(pos);
        if (item)
        {
            HandleObserveRequest(item);
        }
    }
}
void FicsConsole::CMHandleExamineRequest(const QPoint& pos)
{
    QMenu menu;
    menu.addAction(tr("Examine"));
    if (menu.exec(ui->listHistory->mapToGlobal(pos)))
    {
        QListWidgetItem* item = ui->listHistory->itemAt(pos);
        if (item)
        {
            HandleExamineRequest(item);
        }
    }
}
void FicsConsole::CMHandleRelayRequest(const QPoint& pos)
{
    QMenu menu;
    menu.addAction(tr("Relay"));
    if (menu.exec(ui->listRelay->mapToGlobal(pos)))
    {
        QListWidgetItem* item = ui->listRelay->itemAt(pos);
        if (item)
        {
            HandleRelayRequest(item);
        }
    }
}
void FicsConsole::CMHandleTacticsRequest(const QPoint& pos)
{
    QMenu menu;
    menu.addAction(tr("Puzzle"));
    if (menu.exec(ui->listPuzzle->mapToGlobal(pos)))
    {
        QListWidgetItem* item = ui->listPuzzle->itemAt(pos);
        if (item)
        {
            HandleTacticsRequest(item);
        }
    }
}
void FicsConsole::CMHandleHistoryRequest(const QPoint& pos)
{
    QMenu menu;
    menu.addAction(tr("History"));
    if (menu.exec(ui->listPlayers->mapToGlobal(pos)))
    {
        QTableWidgetItem* item = ui->listPlayers->itemAt(pos);
        if (item)
        {
            HandleHistoryRequest(item);
        }
    }
}
void FicsConsole::CMHandleSeekRequest(const QPoint& pos)
{
    QMenu menu;
    menu.addAction(tr("Accept"));
    if (menu.exec(ui->listSeeks->mapToGlobal(pos)))
    {
        QListWidgetItem* item = ui->listSeeks->itemAt(pos);
        if (item)
        {
            HandleSeekRequest(item);
        }
    }
}

void FicsConsole::saveConfig()
{
    AppSettings->setValue("/FicsConsole/NoPlaySplit", ui->noPlaySplitter->saveState());
    AppSettings->setValue("/FicsConsole/SeekSplit", ui->seekSplitter->saveState());
    AppSettings->setValue("/FICS/minutes", ui->seekTime->value());
    AppSettings->setValue("/FICS/increment", ui->seekIncrement->value());
    AppSettings->setValue("/FICS/eloLow", ui->eloMin->value());
    AppSettings->setValue("/FICS/eloHigh", ui->eloMax->value());
}

void FicsConsole::slotReconfigure()
{
    ui->noPlaySplitter->restoreState(AppSettings->value("/FicsConsole/NoPlaySplit").toByteArray());
    ui->seekSplitter->restoreState(AppSettings->value("/FicsConsole/SeekSplit").toByteArray());

    bool on = AppSettings->getValue("/FICS/commandline").toBool();
    ui->line->setVisible(on);
    ui->textOut->setEnabled(on);
    ui->textOut->setVisible(on);
}
