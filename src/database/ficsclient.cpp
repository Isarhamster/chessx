/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "ficsclient.h"
#include "settings.h"

FicsClient::FicsClient(QObject *parent):
    TelnetClient(parent),
    m_cmd(BLKCMD_NULL)
{
}

void FicsClient::startSession()
{
    QString account = AppSettings->getValue("/FICS/userName").toString();
    QString passwd = AppSettings->getValue("/FICS/passWord").toString();
    connectHost("freechess.org", 5000, account, passwd);
    m_cmd = BLKCMD_NULL;
}

void FicsClient::sendCommand(QString s)
{
    sendFicsCommand(s+"\r");
}

void FicsClient::sendFicsCommand(QString s)
{
    sendFicsCommandWithId(s,99);
}

void FicsClient::sendFicsCommandWithId(QString s, int id)
{
    s.prepend(QString("%1 ").arg(id));
    send(s);
}

void FicsClient::OnSessionStarted()
{
    send("set seek 0");
    send("set style 12");
    send("set shout 0");
    send("set cshout 0");
    send("set gin 0");
    send("set pin 0");
    send("set mailmess 1");
    send("set chanoff 1");
    send("iset block 1");
    emit receivedMessage(0,tr("Connected to FICS"));
}

void FicsClient::OnReceiveTelnetMessage(QString s)
{
    int start = s.indexOf(StartReply);
    int end = s.indexOf(EndReply);

    if (end>=0)
    {
        s.remove(EndReply);
    }

    if (start>=0)
    {
        s.remove(StartReply);
        QStringList l = s.split(Separator);
        m_cmd = l[1].toInt();
        emit commandStarted(m_cmd);
    }
    else
    {
        if (!s.isEmpty())
        {
            if (s.startsWith("<12>"))
            {
                s.remove("<12>");
                emit receivedBoard(m_cmd, s.trimmed());
            }
            else if(m_cmd != BLKCMD_NULL)
            {
                emit receivedMessage(m_cmd,s);
            }
            else
            {
                // TODO - open issue how to correctly route this stuff
                if (s.startsWith(":"))
                {
                    s.remove(0,1);
                    emit receivedMessage(BLKCMD_XTELL,s);
                }
            }
        }
    }

    if (end >= 0)
    {
        emit commandDone(m_cmd);
        m_cmd = BLKCMD_NULL;
    }
}
