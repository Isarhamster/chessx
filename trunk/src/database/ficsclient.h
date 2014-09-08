/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef FICSCLIENT_H
#define FICSCLIENT_H

#include "telnetclient.h"

class FicsClient : public TelnetClient
{
public:
    FicsClient(QObject* parent);

    virtual void startSession();
    virtual void sendAccept();
    virtual void sendHistory();
    virtual void sendPlayRequest(int gameId);
    virtual void sendCommand(QString s);

protected:
    void sendFicsCommand(QString s);
    virtual void OnSessionStarted();
    virtual void OnReceiveTelnetMessage(QString);

};

#endif // FICSCLIENT_H
