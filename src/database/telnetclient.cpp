/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "TelnetClient.h"

#include <QTcpSocket>
#include <QHostAddress>

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
    QString data;
    QByteArray bytes = m_socket->readAll();
    data = QString::fromLatin1(bytes);
    switch (m_state)
    {
        case 0:
        {
            QRegExp reLogin  = QRegExp("[Ll]ogin:");
            if (data.contains(reLogin))
            {
                ++m_state;
                if (m_passwd.isEmpty()) ++m_state;
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
            break;
        }
        case 2:
        {
            QRegExp reLoggedIn = QRegExp("Starting FICS session");
            if (data.contains(reLoggedIn))
            {
                ++m_state;
                emit messageReceived(data);
            }
            break;
        }
        default:
        emit messageReceived(data);
        break;
    }
}

void TelnetClient::send(const QString& s)
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
        m_socket->write(s.toLatin1());
        m_socket->flush();
    }
}

void TelnetClient::connectHost(const QString &host, int port, QString name, QString passwd)
{
    if (m_socket->state() == QAbstractSocket::UnconnectedState)
    {
        m_name = name;
        m_passwd = passwd;
        m_socket->connectToHost(host, port);
    }
}
