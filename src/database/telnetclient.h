/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TELNETCLIENT_H
#define TELNETCLIENT_H

#include <QObject>
#include <QString>
#include <QByteArray>

class QProcess;
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

    QString getGuestName() const;

    bool InternalTelnet() const;

    bool sessionStarted() const;
    bool loggedInAsGuest() const;

signals:
    void disconnected();

protected slots:
    void SlotReadData();
    void SlotSocketDisconnected();
    void SlotReadTimesealData();
    void SlotTimesealError();

protected:
    void closeSession();
    bool StartTimeseal(const QString &host, int port, QString name, QString passwd);
    void StartSocket(const QString &host, int port, QString name, QString passwd);
    void DispatchReadData(QByteArray bytes);

protected: //methods
    virtual void startSession() = 0;
    virtual void sendCommand(QString s) = 0;

protected: //callback
    virtual void OnSessionStarted(QString) = 0;
    virtual void OnReceiveTelnetMessage(QString) = 0;

private:
    QTcpSocket* m_socket;
    QString m_name;
    QString m_passwd;
    int     m_state;
    bool    m_loggedInAsGuest;
    QString m_remainder;
    QString guestName;

    QProcess* m_extToolProcess;
    
    bool m_bInternalTelnet;

};

#endif // TELNETCLIENT_H
