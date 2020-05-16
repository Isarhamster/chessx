/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "settings.h"
#include "logstream.h"
#include <QDebug>
#include <QTime>
#include <QDir>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

QTextStream LogStream::s_logStream;

void
#ifdef MSC_VER
_cdecl
#endif
#if QT_VERSION < 0x050000
SimpleLoggingHandler(QtMsgType type, const char *txt)
#else
SimpleLoggingHandler(QtMsgType type, const QMessageLogContext &, const QString &txt)
#endif
{
    QString msg = txt;

    LogStream::s_logStream << QTime::currentTime().toString().toLatin1().data();
    switch(type)
    {
    case QtDebugMsg:
        LogStream::s_logStream << " : ";
        break;
    case QtCriticalMsg:
        LogStream::s_logStream << " Critical: ";
        break;
    case QtWarningMsg:
        LogStream::s_logStream << " Warning: ";
        break;
#if QT_VERSION > 0x050500
    case QtInfoMsg:
        LogStream::s_logStream << " Information: ";
        break;
#endif
    case QtFatalMsg:
         LogStream::s_logStream <<  " Fatal: ";
        break;
    }
    LogStream::s_logStream << msg.toLatin1().data() << endl;
}

LogStream::LogStream()
{
#ifdef _DEBUG
    startFileLog();
#endif
}

LogStream::~LogStream()
{
#ifdef _DEBUG
   stopFileLog();
#endif
}

void LogStream::startFileLog()
{
    QString currentPath = AppSettings->logPath() + "chessx.log";
    m_logFile.setFileName(currentPath);
    if (m_logFile.open(QIODevice::WriteOnly))
    {
        s_logStream.setDevice(&m_logFile);
#if QT_VERSION < 0x050000
        qInstallMsgHandler(SimpleLoggingHandler);
#else
        qInstallMessageHandler(SimpleLoggingHandler);
#endif
    }
}

void LogStream::stopFileLog()
{
    m_logFile.close();
}

