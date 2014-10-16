/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TELNETCLIENT_H
#define TELNETCLIENT_H

#include <QObject>
#include <QString>

class QTcpSocket;

class TelnetClient : public QObject
{
    Q_OBJECT
public:
    explicit TelnetClient(QObject *parent = 0);
    virtual ~TelnetClient();

    void connectHost(const QString& host, int port, QString name, QString passwd);
    void send(QString s);

    void exitSession();

signals:
    void disconnected();

public slots:
    void SlotReadData();

protected: //methods
    virtual void startSession() = 0;
    virtual void sendAccept() = 0;
    virtual void sendHistory() = 0;
    virtual void sendPlayRequest(int gameId) = 0;
    virtual void sendCommand(QString s) = 0;
    virtual void sendObserve(int gameId) = 0;
    virtual void sendUnobserve(int gameId) = 0;

protected: //callback
    virtual void OnSessionStarted()=0;
    virtual void OnReceiveTelnetMessage(QString)=0;

private:
    QTcpSocket* m_socket;
    QString m_name;
    QString m_passwd;
    int     m_state;
    QString m_remainder;

};

#endif // TELNETCLIENT_H
