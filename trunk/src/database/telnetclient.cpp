/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "TelnetClient.h"

#include <QTcpSocket>
#include <QHostAddress>

#define DEBUG_TELNET
//#undef DEBUG_TELNET

TelnetClient::TelnetClient(QObject *parent)
    : QObject(parent)
{
    m_state = 0;
    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(SlotReadData()));
    connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
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
    QByteArray bytes = m_socket->readAll();
    QString data = QString::fromLatin1(bytes);
#ifdef DEBUG_TELNET
    qDebug() << "<--" << data;
#endif

    data.remove(QChar(0x07)); // bell

    switch (m_state)
    {
        case 0:
        {
            QRegExp reLogin  = QRegExp("[Ll]ogin:");
            if (data.contains(reLogin))
            {
                ++m_state;
                send(m_name);
            }
            break;
        }
        case 1:
        {
            QRegExp rePasswd = QRegExp("[Pp]assword:");
            if (data.contains(rePasswd))
            {
                ++m_state;
                send(m_passwd);
            }
            else
            {
                QRegExp reEnter = QRegExp("Press return");
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
            QRegExp reLoggedIn = QRegExp("[Ss]ession");
            if (data.contains(reLoggedIn))
            {
                ++m_state;
                OnSessionStarted();
            }
            break;
        }
        default:
        OnReceiveTelnetMessage(data);
        break;
    }
}

void TelnetClient::send(QString s)
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
#ifdef DEBUG_TELNET
    qDebug() << "-->" << s;
#endif
        s.append("\n");
        m_socket->write(s.toLatin1());
        m_socket->flush();
    }
}

void TelnetClient::connectHost(const QString &host, int port, QString name, QString passwd)
{
    if (m_socket->state() == QAbstractSocket::UnconnectedState)
    {
#ifdef DEBUG_TELNET
    qDebug() << "connecting...";
#endif
        m_name = name;
        m_passwd = passwd;
        m_socket->connectToHost(host, port);
    }
}

void TelnetClient::exitSession()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
#ifdef DEBUG_TELNET
    qDebug() << "closing...";
#endif
        m_socket->close();
    }
}
