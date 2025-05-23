/***************************************************************************
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net>      *
 *   (C) 2008-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QProcess>

#include "settings.h"
#include "enginex.h"
#include "wbengine.h"
#include "uciengine.h"

#define DEBUG_ENGINE
//#undef DEBUG_ENGINE

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

/*** Engine ***/

bool EngineX::s_allowEngineOutput = true;

EngineX::EngineX(const QString& name,
               const QString& command,
               bool bTestMode,
               const QString& directory,
               bool log, bool sendHistory)
{
    m_name = name;
    m_command = command;
    m_mpv = 0;
    m_bTestMode = bTestMode;
    m_sendHistory = sendHistory;
    QTextStream* logStream = nullptr;
    if (log)
    {
        m_logFile.setFileName(AppSettings->logPath()+name+".log");
        if (m_logFile.open(QIODevice::WriteOnly))
        {
            logStream = new QTextStream(&m_logFile);
        }
    }
    m_logStream = logStream;
    m_process = nullptr;
    m_active = false;
    m_analyzing = false;
    m_directory = directory;
}

EngineX* EngineX::newEngine(int index)
{
    return newEngine(index, false);
}

EngineX* EngineX::newEngine(EngineList& engineList, int index, bool bTestMode)
{
    EngineX *engine = nullptr;

    QString name      = engineList[index].name;
    QString command   = engineList[index].command;
    QString options   = engineList[index].options;
    QString directory = engineList[index].directory;
    bool logging      = engineList[index].logging;
    bool sendHistory  = engineList[index].sendHistory;
    EngineData::EngineProtocol protocol = engineList[index].protocol;

    if(command.contains(' '))
    {
        command = QString("\"%1\"").arg(command);
    }
    QString exe = QString("%1 %2").arg(command, options);

    if(protocol == EngineData::WinBoard)
    {
        engine = new WBEngine(name, exe, bTestMode, directory, logging, sendHistory);
    }
    else
    {
        engine = new UCIEngine(name, exe, bTestMode, directory, logging, sendHistory);
    }

    engine->m_mapOptionValues = engineList[index].m_optionValues;
    return engine;
}

EngineX* EngineX::newEngine(int index, bool bTestMode)
{
    EngineX *engine = nullptr;

    AppSettings->beginGroup("/Engines/");
    QString key(QString::number(index));
    QString name = AppSettings->value(key + "/Name").toString();
    QString command = AppSettings->value(key + "/Command").toString();
    QString options = AppSettings->value(key + "/Options").toString();
    QString directory = AppSettings->value(key + "/Directory").toString();
    QString protocol = AppSettings->value(key + "/Protocol").toString();
    bool log = AppSettings->value(key + "/Logging").toBool();
    bool sendHistory = AppSettings->value(key + "/SendHistory", true).toBool();

    if(command.contains(' '))
    {
        command = QString("\"%1\"").arg(command);
    }
    QString exe = QString("%1 %2").arg(command, options);

    if(protocol == "WinBoard")
    {
        engine = new WBEngine(name, exe, bTestMode, directory, log, sendHistory);
    }
    else
    {
        engine = new UCIEngine(name, exe, bTestMode, directory, log, sendHistory);
    }

    AppSettings->getMap(key + "/OptionValues", engine->m_mapOptionValues);
    AppSettings->endGroup();

    return engine;
}

EngineX::~EngineX()
{
    if(m_process)
    {
        m_process = nullptr;
    }
    delete m_logStream;
}

void EngineX::activate()
{
    if(m_process)
    {
        return;
    }

    m_process = new QProcess(this);
    if(m_process)
    {
        m_process->setReadChannel(QProcess::StandardOutput);
        if(!m_directory.isEmpty())
        {
            m_process->setWorkingDirectory(m_directory);
        }
        connect(m_process, SIGNAL(started()), SLOT(protocolStart()));
#if QT_VERSION < 0x060000
        connect(m_process, SIGNAL(error(QProcess::ProcessError)), SLOT(processError(QProcess::ProcessError)));
#else
        connect(m_process, SIGNAL(errorOccurred(QProcess::ProcessError)), SLOT(processError(QProcess::ProcessError)));
#endif
        connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(pollProcess()));
        connect(m_process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(processExited()));
        QStringList parameters = QProcess::splitCommand(m_command);
        QString cmd = parameters.at(0);
        parameters.pop_front();
        m_process->start(cmd, parameters);
    }
}

void EngineX::deactivate()
{
    if(m_active)
    {
        protocolEnd();
        if(m_process)
        {
            m_process->waitForFinished(500);
        }
    }
}

bool EngineX::isActive()
{
    return m_active;
}

bool EngineX::isAnalyzing()
{
    return m_analyzing;
}

void EngineX::send(const QString& message)
{
    if (s_allowEngineOutput && m_logStream)
    {
        *m_logStream << "<-- " << message << Qt::endl;
    }

    QString out(message);
    out.append('\n');
    if(m_process && !message.isEmpty())
    {
        m_process->write(out.toLatin1());
    }
}

void EngineX::setActive(bool active)
{
    if(active && !m_active)
    {
        m_active = true;
        emit activated();
    }
    else
    {
        if(!active && m_active)
        {
            setAnalyzing(false);
            m_active = false;
            emit deactivated();
        }
    }
}

void EngineX::setAnalyzing(bool analyzing)
{
    if(analyzing)
    {
        m_analyzing = true;
        emit analysisStarted();
    }
    else if(m_analyzing)
    {
        m_analyzing = false;
        emit analysisStopped();
    }
}

void EngineX::sendAnalysis(const Analysis& analysis)
{
    if (s_allowEngineOutput)
    {
        emit analysisUpdated(analysis);
    }
}

bool EngineX::getSendHistory() const
{
    return m_sendHistory;
}

int EngineX::defaultMpv() const
{
    return m_mapOptionValues.value("MultiPV", "1").toInt();
}

void EngineX::setMpv(int mpv)
{
    m_mpv = mpv;
}

void EngineX::setMoveTime(const EngineParameter& mt)
{
    m_moveTime = mt;
}

void EngineX::pollProcess()
{
    QString message;

    while(m_process && m_process->canReadLine())
    {
        message = m_process->readLine().simplified();
        if (s_allowEngineOutput && m_logStream)
        {
            *m_logStream << "--> " << message << Qt::endl;
        }
        processMessage(message);
    }
}

void EngineX::removeProcess()
{
    disconnect(m_process, SIGNAL(started()));
#if QT_VERSION < 0x060000
    disconnect(m_process, SIGNAL(error(QProcess::ProcessError)));
#else
    disconnect(m_process, SIGNAL(errorOccurred(QProcess::ProcessError)));
#endif
    disconnect(m_process, SIGNAL(readyReadStandardOutput()));
    disconnect(m_process, SIGNAL(finished(int,QProcess::ExitStatus)));
    m_process = nullptr;
}

void EngineX::processError(QProcess::ProcessError errMsg)
{
    setActive(false);
    removeProcess();
    emit error(errMsg);
}

bool EngineX::hasOption(const QString& name) const
{
    bool found = false;
    foreach (EngineOptionData eod, m_options)
    {
        if (eod.m_name == name)
        {
            found = true;
            break;
        }
    }
    return found;
}

bool EngineX::getOption(const QString& name, EngineOptionData& result)
{
    bool found = false;
    foreach (EngineOptionData eod, m_options)
    {
        if (eod.m_name == name)
        {
            found = true;
            result = eod;
            break;
        }
    }
    return found;
}

void EngineX::logError(const QString& errMsg)
{
    if (s_allowEngineOutput && m_logStream)
    {
        *m_logStream << "### " << errMsg << Qt::endl;
    }
    qDebug() << errMsg;
}

void EngineX::setAllowEngineOutput(bool allow)
{
    s_allowEngineOutput = allow;
}

bool EngineX::engineOutputAllowed()
{
    return s_allowEngineOutput;
}

void EngineX::processExited()
{
    setActive(false);
    removeProcess();
    emit deactivated();
}
