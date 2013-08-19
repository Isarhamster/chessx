/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "logstream.h"
#include <QDebug>
#include <fstream>
#include <QTime>
#include <QDir>

using namespace std;
ofstream logfile;

static bool bDoLog = false;
void
#ifdef MSC_VER
_cdecl
#endif
#if QT_VERSION < 0x050000
SimpleLoggingHandler(QtMsgType type, const char *msg)
#else
SimpleLoggingHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
#endif
{
    if (!bDoLog) return;

    switch (type)
    {
        case QtDebugMsg:
        logfile << QTime::currentTime().toString().toLatin1().data() << " : "         << msg.toLatin1().data() << endl;
            break;
        case QtCriticalMsg:
            logfile << QTime::currentTime().toString().toLatin1().data() << " Critical: " << msg.toLatin1().data() << endl;
            break;
        case QtWarningMsg:
            logfile << QTime::currentTime().toString().toLatin1().data() << " Warning: "  << msg.toLatin1().data() << endl;
            break;
        case QtFatalMsg:
            logfile << QTime::currentTime().toString().toLatin1().data() <<  " Fatal: "   << msg.toLatin1().data() << endl;
            break;
     }
}


void startFileLog()
{
#ifdef _DEBUG
    QString currentPath = QDir::currentPath()+ "/chessx.log";
    logfile.open(currentPath.toLatin1().data(),ios::app);
    bDoLog = true;
#if QT_VERSION < 0x050000
    qInstallMsgHandler(SimpleLoggingHandler);
#else
    qInstallMessageHandler(SimpleLoggingHandler);
#endif
#endif
}

void stopFileLog()
{
    if (bDoLog)
    {
        bDoLog = false;
        logfile.close();
    }
}

