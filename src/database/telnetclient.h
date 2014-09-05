/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TELNETCLIENT_H
#define TELNETCLIENT_H

#include <QObject>

class QTcpSocket;

class TelnetClient : public QObject
{
    Q_OBJECT
public:
    explicit TelnetClient(QObject *parent = 0);
    virtual ~TelnetClient();

    void connectHost(const QString& host, int port, QString name, QString passwd);
    void send(const QString& s);

signals:
    void messageReceived(const QString&);
    void disconnected();

public slots:
    void SlotReadData();

protected:
    QTcpSocket* m_socket;

private:
    QString m_name;
    QString m_passwd;
    int     m_state;

};

#endif // TELNETCLIENT_H
