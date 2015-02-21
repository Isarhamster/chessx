/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "settings.h"
#include "telnetclient.h"

#include <QFileInfo>
#include <QHostAddress>
#include <QProcess>
#include <QRegExp>
#include <QStringList>
#include <QTcpSocket>

TelnetClient::TelnetClient(QObject *parent)
    : QObject(parent)
{
    m_state = 0;
    m_bInternalTelnet = true;

    m_socket = new QTcpSocket(this);
    m_socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(SlotReadData()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(SlotDisconnected()));

    m_extToolProcess = new QProcess(this);
    connect(m_extToolProcess, SIGNAL(readyRead()), SLOT(SlotReadTimesealData()));
    connect(m_extToolProcess, SIGNAL(readyReadStandardOutput()), SLOT(SlotReadTimesealData()));
    connect(m_extToolProcess, SIGNAL(error(QProcess::ProcessError)), SLOT(SlotTimesealError()));
}

TelnetClient::~TelnetClient()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
        m_socket->close();
    }
    delete m_socket;
}

void TelnetClient::DispatchReadData(QByteArray bytes)
{
    QString data = m_remainder;
    m_remainder.clear();
    data.append(QString::fromLatin1(bytes));

    bool sendXON = data.contains(0x13);
    data.remove(0x13);
    data.remove(0x07);

    bool hasNetData = false;

    switch (m_state)
    {
        case 0:
        {
            QRegExp reLogin("[Ll]ogin:");
            if (data.contains(reLogin))
            {
                ++m_state;
                send(m_name);
            }
            break;
        }
        case 1:
        {
            QRegExp rePasswd("[Pp]assword:");
            if (data.contains(rePasswd))
            {
                guestName = m_name;
                ++m_state;
                send(m_passwd);
            }
            else
            {
                QRegExp reEnter("Press return");
                if (data.contains(reEnter))
                {
                    QRegExp reGuestName("\"([^\"]*)\"");
                    int pos = reGuestName.indexIn(data);
                    if(pos >= 0)
                    {
                        guestName = reGuestName.cap(1);
                    }
                    ++m_state;
                    send("\n");
                }
            }
            break;
        }
        case 2:
        {
            QRegExp reLoggedIn("[Ss]ession");
            if (data.contains(reLoggedIn))
            {
                ++m_state;
                OnSessionStarted(guestName);
            }
            break;
        }
        default:
        {
            bool endsWithCR = data.endsWith("\n");
            QStringList lines = data.split("\n", QString::SkipEmptyParts);
            if (lines.count() && !endsWithCR)
            {
                hasNetData = true;
                m_remainder = lines.back();
                lines.pop_back();
            }
            foreach (QString line, lines)
            {
                line.remove("\r");
                line.remove("\n");
                line = line.trimmed();
                if (!line.isEmpty())
                {
                    OnReceiveTelnetMessage(line);
                }
            }
        }
        break;
    }

    if (sendXON)
    {
        send(QString("%1").arg(QChar(0x11)));
    }
}

bool TelnetClient::InternalTelnet() const
{
    return m_bInternalTelnet;
}

void TelnetClient::SlotReadData()
{
    while(m_socket->bytesAvailable())
    {
        QByteArray bytes = m_socket->readAll();
        DispatchReadData(bytes);
    }
}

void TelnetClient::SlotDisconnected()
{
    m_socket->close();
    emit disconnected();
}

void TelnetClient::SlotReadTimesealData()
{
    if(m_extToolProcess->isOpen())
    {
        QByteArray bytes = m_extToolProcess->readAllStandardOutput();
        DispatchReadData(bytes);
    }
}

void TelnetClient::SlotTimesealError()
{
    emit disconnected();
}

bool TelnetClient::StartTimeseal(const QString &host, int port, QString name, QString passwd)
{
    if (!AppSettings->getValue("/FICS/useTimeseal").toBool())
        return false;

    QFileInfo fi(AppSettings->timesealFilePath());
    if (fi.exists())
    {
        m_bInternalTelnet = false;
        m_state = 0;
        m_name = name;
        m_passwd = passwd;

        m_extToolProcess->setReadChannel(QProcess::StandardOutput);

        QStringList options;
        options << host;
        options << QString::number(port);
        QString command = options.join(" ");

        m_extToolProcess->start(fi.absoluteFilePath(), options, QIODevice::ReadWrite | QIODevice::Unbuffered);
        if (m_extToolProcess->waitForStarted(3000))
        {
            return true;
        }
    }

    return false;
}

void TelnetClient::StartSocket(const QString &host, int port, QString name, QString passwd)
{
    if (m_socket->state() == QAbstractSocket::UnconnectedState)
    {
        m_bInternalTelnet = true;
        m_state = 0;
        m_name = name;
        m_passwd = passwd;
        m_socket->connectToHost(host, port);
    }
}

QString TelnetClient::getGuestName() const
{
    return guestName;
}

void TelnetClient::send(QString s)
{
    if (m_bInternalTelnet)
    {
        if (m_socket->state() == QAbstractSocket::ConnectedState)
        {
            s.append("\n");
            if (m_socket->write(s.toLatin1()) < 0)
            {
                exitSession();
            }
            else
            {
                m_socket->flush();
            }
        }
    }
    else
    {
        if (m_extToolProcess->isOpen())
        {
            s.append("\n");
            QByteArray b = s.toLatin1();
            if (m_extToolProcess->write(b) < 0)
            {
                exitSession();
            }
            else
            {
                m_extToolProcess->waitForBytesWritten(1000);
            }
        }
        else
        {
            emit disconnected();
        }
    }
}

void TelnetClient::connectHost(const QString &host, int port, QString name, QString passwd)
{
    if (!StartTimeseal(host, port, name, passwd))
    {
        StartSocket(host, port, name, passwd);
    }
}

bool TelnetClient::sessionStarted() const
{
    return (m_state > 0);
}

void TelnetClient::exitSession()
{
    if (m_socket->state() != QAbstractSocket::UnconnectedState)
    {
        m_socket->close();
    }

    if (m_extToolProcess->isOpen())
    {
        m_extToolProcess->close();
    }

    m_state = 0;
}
