/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "ficsclient.h"
#include "settings.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

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
    send("set prompt");
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
    send("set notakeback 1");
    send("set bell off");
    send("iset block 1");
    QString connection = InternalTelnet() ? "Telnet" : "Timeseal";
    emit receivedMessage(BLKCMD_INTERNAL_SESSION_STARTED,tr("Connected to FICS as %1 using %2")
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

        s.remove(PoseStart);
        s.remove(PoseEnd);

        if (start>=0)
        {
            s.remove(StartReply);
            QStringList l = s.split(Separator);
            m_cmd = l[1].toInt();

            emit commandStarted(m_cmd);
            s = l[2];
        }

        if (s.startsWith("fics%"))
        {
            s.remove("fics%");
        }
    }
    else
    {
        s.remove(EndReply);
        QRegExp regCmd("fics%[^9]*99([\\d]*)(.*)");

        if (regCmd.indexIn(s) >= 0)
        {
           QString d = regCmd.cap(1);
           m_cmd = d.toInt();

           emit commandStarted(m_cmd);
           s = regCmd.cap(2);
        }
        else if (s.startsWith("fics%"))
        {
            s.remove(0,5);
            m_cmd = BLKCMD_NULL;
        }
    }

    ProcessUnblockedMessage(s.normalized(QString::NormalizationForm_D).trimmed());

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
            // qDebug() << s;
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
            emit receivedMessage(BLKCMD_SEEK,s);
        }
        else if ((s.contains("tells you") || s.contains("says:")) && !s.contains("ROBOadmin"))
        {
            emit receivedMessage(BLKCMD_SAY,s);
        }
        else if (s.startsWith("Challenge:") || s.startsWith("Challenge from") || s.contains("offers you a draw") || s.contains("would like to abort the game"))
        {
            emit receivedMessage(BLKCMD_INTERNAL_OTHER,s);
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
