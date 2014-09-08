/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "ficsclient.h"
#include "settings.h"

FicsClient::FicsClient(QObject *parent):
    TelnetClient(parent)
{
}

void FicsClient::startSession()
{
    QString account = AppSettings->getValue("/FICS/userName").toString();
    QString passwd = AppSettings->getValue("/FICS/passWord").toString();
    connectHost("freechess.org", 5000, account, passwd);
}

void FicsClient::sendAccept()
{
    sendFicsCommand("accept");
}

void FicsClient::sendHistory()
{
    sendFicsCommand("history");
}

void FicsClient::sendPlayRequest(int gameId)
{
    QString cmd = QString("play %1").arg(gameId);
    sendFicsCommand(cmd);
}

void FicsClient::sendCommand(QString s)
{
    sendFicsCommand(s);
}

void FicsClient::sendFicsCommand(QString s)
{
    send(s);
}

void FicsClient::OnSessionStarted()
{
    sendFicsCommand("set seek 0");
    sendFicsCommand("set style 12");
    sendFicsCommand("set shout 0");
    sendFicsCommand("set cshout 0");
    sendFicsCommand("set gin 0");
    sendFicsCommand("set pin 0");
    sendFicsCommand("set mailmess 1");
    sendFicsCommand("- channel 1");
    sendFicsCommand("- channel 2");
    sendFicsCommand("- channel 50");
    sendFicsCommand("iset block 1");
}

void FicsClient::OnReceiveTelnetMessage(QString s)
{

}
