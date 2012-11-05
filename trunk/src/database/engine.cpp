/***************************************************************************
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net>      *
 *   (C) 2008-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "settings.h"
#include "engine.h"
#include "wbengine.h"
#include "uciengine.h"

/*** Engine ***/

Engine::Engine(const QString& name,
			 const QString& command,
             bool bTestMode,
			 const QString& directory,
			 QTextStream* logStream)
{
	m_name = name;
	m_command = command;
    m_bTestMode = bTestMode;
	m_logStream = logStream;
	m_process = 0;
	m_active = false;
	m_analyzing = false;
	m_directory = directory;
}

Engine* Engine::newEngine(int index)
{
    return newEngine(index,false);
}

Engine* Engine::newEngine(EngineList& engineList, int index, bool bTestMode)
{
    Engine *engine = 0;

    QString name      = engineList[index].name;
    QString command   = engineList[index].command;
    QString options   = engineList[index].options;
    QString directory = engineList[index].directory;
    EngineData::EngineProtocol protocol = engineList[index].protocol;

    if (command.contains(' '))
        command = QString("\"%1\"").arg(command);
    QString exe = QString("%1 %2").arg(command).arg(options);

    if (protocol == EngineData::WinBoard)
        engine = new WBEngine(name, exe, bTestMode, directory);
    else
        engine = new UCIEngine(name, exe, bTestMode, directory);

    engine->m_mapOptionValues = engineList[index].m_optionValues;
    return engine;
}

Engine* Engine::newEngine(int index, bool bTestMode)
{
	Engine *engine = 0;

	AppSettings->beginGroup("/Engines/");
	QString key(QString::number(index));
	QString name = AppSettings->value(key + "/Name").toString();
	QString command = AppSettings->value(key + "/Command").toString();
	QString options = AppSettings->value(key + "/Options").toString();
	QString directory = AppSettings->value(key + "/Directory").toString();
	QString protocol = AppSettings->value(key + "/Protocol").toString();

	if (command.contains(' '))
		command = QString("\"%1\"").arg(command);
	QString exe = QString("%1 %2").arg(command).arg(options);

	if (protocol == "WinBoard")
        engine = new WBEngine(name, exe, bTestMode, directory);
    else
        engine = new UCIEngine(name, exe, bTestMode, directory);

    AppSettings->getMap(key + "/OptionValues", engine->m_mapOptionValues);
    AppSettings->endGroup();

	return engine;
}

Engine::~Engine()
{
    if (m_process) {
        m_process = 0;
    }
}


void Engine::setLogStream(QTextStream* logStream)
{
	m_logStream = logStream;
}

void Engine::activate()
{
    if (m_process) {
		return;
	}

	m_process = new QProcess(this);
    if (m_process)
    {
        m_process->setReadChannel(QProcess::StandardOutput);
        if (!m_directory.isEmpty())
            m_process->setWorkingDirectory(m_directory);
        connect(m_process, SIGNAL(started()), SLOT(protocolStart()));
        connect(m_process, SIGNAL(error(QProcess::ProcessError)), SLOT(processError(QProcess::ProcessError)));
        connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(pollProcess()));
        connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(processExited()));
        m_process->start(m_command);
    }
}

void Engine::deactivate()
{
    if (m_active)
    {
        protocolEnd();
        m_process->waitForFinished(200);
    }
}

bool Engine::isActive()
{
	return m_active;
}

bool Engine::isAnalyzing()
{
	return m_analyzing;
}

void Engine::send(const QString& message)
{
    qDebug() << "<-- " << message << endl;

	QString out(message);
	out.append('\n');
	if (m_process && !message.isEmpty())
		m_process->write(out.toLatin1());
}

void Engine::setActive(bool active)
{
	if (active && !m_active) {
		m_active = true;
		emit activated();
	} else {
		if (!active && m_active) {
			setAnalyzing(false);
			m_active = false;
			emit deactivated();
		}
	}
}

void Engine::setAnalyzing(bool analyzing)
{
	if (analyzing) {
		m_analyzing = true;
		emit analysisStarted();
	} else {
		if (!analyzing && m_analyzing) {
			m_analyzing = false;
			emit analysisStopped();
		}
	}
}

void Engine::sendAnalysis(const Analysis& analysis)
{
	emit analysisUpdated(analysis);
}

void Engine::setMpv(int mpv)
{
	m_mpv = mpv;
}

void Engine::pollProcess()
{
	QString message;

    while (m_process && m_process->canReadLine()) {
        message = m_process->readLine().simplified();
        qDebug() << "--> " << message << endl;
        processMessage(message);
	}
}

void Engine::processError(QProcess::ProcessError errMsg)
{
    setActive(false);
    emit error(errMsg);
}

void Engine::processExited()
{
	setActive(false);
	m_process = 0;
}
