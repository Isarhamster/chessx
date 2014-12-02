/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "telnetclient.h"

#include <QHostAddress>
#include <QRegExp>
#include <QStringList>
#include <QTcpSocket>

TelnetClient::TelnetClient(QObject *parent)
    : QObject(parent)
{
    m_state = 0;
    m_socket = new QTcpSocket(this);
    m_socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(SlotReadData()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(SlotDisconnected()));
}

TelnetClient::~TelnetClient()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
        m_socket->close();
    }
    delete m_socket;
}

void TelnetClient::SlotReadData()
{
    while(m_socket->bytesAvailable())
    {
        QByteArray bytes = m_socket->readAll();

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
                    ++m_state;
                    send(m_passwd);
                }
                else
                {
                    QRegExp reEnter("Press return");
                    if (data.contains(reEnter))
                    {
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
                    OnSessionStarted();
                }
                break;
            }
            default:
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
                    line = line.trimmed();
                    line.remove("\r");
                    line.remove("\n");
                    if (!line.isEmpty())
                    {
                        OnReceiveTelnetMessage(line);
                    }
                }
            break;
        }

        if (sendXON)
        {
            send(QString("%1").arg(QChar(0x11)));
        }
    }
}

void TelnetClient::SlotDisconnected()
{
    m_socket->close();
    emit disconnected();
}

void TelnetClient::send(QString s)
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
        s.append("\n");
        m_socket->write(s.toLatin1());
        m_socket->flush();
    }
}

void TelnetClient::connectHost(const QString &host, int port, QString name, QString passwd)
{
    if (m_socket->state() == QAbstractSocket::UnconnectedState)
    {
        m_state = 0;
        m_name = name;
        m_passwd = passwd;
        m_socket->connectToHost(host, port);
    }
}

void TelnetClient::exitSession()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
        send("exit");
    }

    if (m_socket->state() != QAbstractSocket::UnconnectedState)
    {
        m_socket->close();
    }

    m_state = 0;
}
