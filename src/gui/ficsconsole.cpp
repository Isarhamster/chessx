/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "ficsconsole.h"
#include "ficsclient.h"
#include "settings.h"

#include "ui_ficsconsole.h"

#include <QString>

#undef FICS_DEBUG

FicsConsole::FicsConsole(QWidget *parent, FicsClient* ficsClient) :
    QWidget(parent),
    ui(new Ui::FicsConsole),
    m_ficsClient(ficsClient)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);

    connect(ui->tabWidget, SIGNAL(tabBarClicked(int)), SLOT(SlotTabChanged(int)));

    connect(ui->textOut, SIGNAL(editingFinished()), SLOT(SendCommand()));
    connect(m_ficsClient, SIGNAL(receivedMessage(int,QString)), SLOT(PrintMessage(int,QString)), Qt::QueuedConnection);
    connect(m_ficsClient, SIGNAL(receivedBoard(int, QString)), SLOT(HandleBoard(int, QString)), Qt::QueuedConnection);
    connect(m_ficsClient, SIGNAL(commandStarted(int)), SLOT(CommandStarted(int)), Qt::QueuedConnection);
    connect(m_ficsClient, SIGNAL(commandDone(int)), SLOT(CommandDone(int)), Qt::QueuedConnection);
    connect(m_ficsClient, SIGNAL(disconnected()), SLOT(Disconnected()), Qt::QueuedConnection);

    connect(ui->listGames, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleObserveRequest(QListWidgetItem*)));
    connect(ui->listHistory, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleExamineRequest(QListWidgetItem*)));
    connect(ui->listRelay, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleRelayRequest(QListWidgetItem*)));
    connect(ui->listPuzzle, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleTacticsRequest(QListWidgetItem*)));
    connect(ui->listPlayers, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(HandleHistoryRequest(QListWidgetItem*)));

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
        disconnect(m_ficsClient, SIGNAL(commandDone(int)));
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
#ifdef FICS_DEBUG
    ui->textIn->appendPlainText(s);
#endif
    ui->tabWidget->setCurrentIndex(0);
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
    QString s = item->text();
    QString cmd = s.remove(' ').toLower();
    QString request = "tell puzzlebot " + cmd;
    m_ficsClient->sendCommand(request);
}

void FicsConsole::HandleHistoryRequest(QListWidgetItem* item)
{
    if (!m_ficsClient) return;

    QString s = item->text();
    QString player = s.section(' ',1,1);
    player.remove(QRegExp("\\([^\\)]*\\)"));
    if (!player.isEmpty())
    {
        m_lastHistoryPlayer = player;
        QString request = "history " + player;
        m_ficsClient->sendCommand(request);
    }
}

void FicsConsole::SendMove(QString m)
{
    m_ficsClient->sendCommand(m);
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
#ifdef FICS_DEBUG
    ui->textIn->appendPlainText(QString("Issued command %1").arg(cmd));
#endif
    switch (cmd)
    {
    case FicsClient::BLKCMD_HISTORY:
        ui->listHistory->clear();
        break;
    case FicsClient::BLKCMD_GAMES:
        ui->listGames->clear();
        break;
    case FicsClient::BLKCMD_WHO:
        m_transportList.clear();
        break;
    }
}

void FicsConsole::CommandDone(int cmd)
{
#ifdef FICS_DEBUG
    ui->textIn->appendPlainText(QString("Done command %1").arg(cmd));
#endif
    switch (cmd)
    {
    case FicsClient::BLKCMD_HISTORY:
        ui->tabWidget->setCurrentIndex(1);
        break;
    case FicsClient::BLKCMD_GAMES:
        break;
    case FicsClient::BLKCMD_WHO:
        {
            ui->listPlayers->clear();

            QRegExp sep("(\\s+|\\^|~|:|#|\\.)");
            QStringList list;
            QStringList ulist;
            foreach(QString s, m_transportList)
            {
                QStringList l = s.split(sep);
                for (int i=0; i<l.count()-1; i+=2)
                {
                    QString rating = l[i];
                    if (!rating.startsWith("-"))
                    {
                        if (rating.length()==3)
                        {
                            rating.prepend(" ");
                            list.append(rating+" "+l[i+1]);
                        }
                        else if (rating.startsWith("+"))
                        {
                            ulist.append(rating+" "+l[i+1]);
                        }
                        else
                        {
                            list.append(rating+" "+l[i+1]);
                        }
                    }
                }
            }
            list.sort();
            for(int k = 0; k < (list.size()/2); ++k) list.swap(k,list.size()-(1+k));
            ulist.sort();
            for(int k = 0; k < (ulist.size()/2); ++k) ulist.swap(k,ulist.size()-(1+k));
            ui->listPlayers->addItems(list);
            ui->listPlayers->addItems(ulist);
            m_transportList.clear();
            break;
        }
    }
}

void FicsConsole::Disconnected()
{
    ui->textIn->appendPlainText(tr("Disconnected"));
    ui->tabWidget->setCurrentIndex(0);
}

void FicsConsole::SlotTabChanged(int tab)
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
        ui->listPlayers->clear();
        ui->listPlayers->addItem(tr("Reading list of players..."));
        m_ficsClient->sendCommand("who");
        break;
    }
}

void FicsConsole::PrintMessage(int blockCmd,QString s)
{
    s = s.trimmed();
    if (!s.isEmpty())
    {
        switch (blockCmd)
        {
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
                    }
                }
            }
            break;
        case FicsClient::BLKCMD_GAMES:
            ui->listGames->addItem(s);
            break;
        case FicsClient::BLKCMD_XTELL:
            ui->listRelay->addItem(s);
            break;
        case FicsClient::BLKCMD_WHO:
            {
                if (!s.contains("players displayed"))
                {
                    m_transportList.append(s);
                }
                break;
            }
        default:
            ui->textIn->appendPlainText(s);
            ui->tabWidget->setCurrentIndex(0);
            break;
        }
    }
}
