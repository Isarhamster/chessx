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
    bool guestLogin = AppSettings->getValue("/FICS/guestLogin").toBool();
    if (guestLogin)
    {
        account = "guest";
        passwd = "";
    }
    emit receivedMessage(BLKCMD_NULL,"Requesting Session...");
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

void FicsClient::OnSessionStarted(QString guestName)
{
    send("set seek 0");
    send("set style 12");
    send("set shout 0");
    send("set cshout 0");
    send("set gin 0");
    send("set pin 0");
    send("set chanoff 1");
    send("set height 240");
    send("set width 240");
    send("set autoflag 1");
    send("set kibitz 1");
    send("iset block 1");
    QString connection = InternalTelnet() ? "Telnet" : "Timeseal";
    emit receivedMessage(0,tr("Connected to FICS as %1 using %2")
                         .arg(guestName)
                         .arg(connection));
    emit connected();
}

void FicsClient::OnReceiveTelnetMessage(QString s)
{
    int start = -1;
    int end = -1;

    if (InternalTelnet())
    {
        start = s.indexOf(StartReply);
        end = s.indexOf(EndReply);

        if (end>=0)
        {
            s.remove(EndReply);
        }

        if (start>=0)
        {
            s.remove(StartReply);
            QStringList l = s.split(Separator);
            m_cmd = l[1].toInt();

            qDebug() << "Command started " << m_cmd;

            emit commandStarted(m_cmd);
            s = l[2];
        }

        if (s.startsWith("fics%"))
        {
            s.remove("fics%");
            s = s.trimmed();
        }
    }
    else
    {
        QRegExp regCmd("fics%[^9]*99([\\d]*)(.*)");

        if (regCmd.indexIn(s) >= 0)
        {
           QString d = regCmd.cap(1);
           m_cmd = d.toInt();
           qDebug() << "Command started " << m_cmd;
           s = regCmd.cap(2);
        }
        else if (s.startsWith("fics%"))
        {
            s.remove(0,5);
            s = s.trimmed();
            m_cmd = BLKCMD_NULL;
        }

        if (s.startsWith("Challenge:"))
        {
            m_cmd = BLKCMD_INTERNAL_OTHER;
        }
        else if (s.contains("offers you a draw"))
        {
            m_cmd = BLKCMD_INTERNAL_OTHER;
        }
        else if (s.contains("would like to abort the game"))
        {
            m_cmd = BLKCMD_INTERNAL_OTHER;
        }
    }

    ProcessUnblockedMessage(s.trimmed());

    if (end >= 0)
    {
        m_cmd = BLKCMD_NULL;
    }
}

void FicsClient::ProcessUnblockedMessage(QString s)
{
    if (!s.isEmpty())
    {
        if (s.startsWith("<12>"))
        {
            s.remove("<12>");
            emit receivedBoard(m_cmd, s.trimmed());
        }
        else if (s.startsWith('{'))
        {
            // Filter out stuff like abort / resign / draw / play
            if (s.contains("Creating"))
            {
                emit receivedMessage(BLKCMD_INTERNAL_MATCH_START,s);
            }
            else
            {
                emit receivedMessage(BLKCMD_INTERNAL_MATCH_END,s);
            }
        }
        else if (s.startsWith(":"))
        {
            s.remove(0,1);
            emit receivedMessage(BLKCMD_XTELL,s);
        }
        else if (s.contains("puzzlebot"))
        {
            emit receivedMessage(BLKCMD_INTERNAL_PUZZLEBOT, s);
        }
        else if (s.contains("seeking"))
        {
            emit receivedMessage(BLKCMD_SOUGHT,s);
        }
        else if (s.startsWith('{'))
        {   // Handle start of game where opponent is accepting
            if (s.contains("Creating"))
            {
                emit receivedMessage(BLKCMD_INTERNAL_MATCH_START,s);
            }
            else
            {
                emit receivedMessage(BLKCMD_INTERNAL_MATCH_END,s);
            }
        }
        else if ((s.contains("tells you") || s.contains("says:")) && !s.contains("ROBOadmin"))
        {
            emit receivedMessage(BLKCMD_SAY,s);
        }
        else if (m_cmd != BLKCMD_NULL)
        {
            emit receivedMessage(m_cmd,s);
        }
        else
        {
            qDebug() << "Cmd " << m_cmd << ": ignoring message: " << s;
        }
    }
}
