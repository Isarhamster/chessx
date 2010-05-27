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
			 const QString& directory,
			 QTextStream* logStream) : QObject()
{
	m_name = name;
	m_command = command;
	m_logStream = logStream;
	m_process = 0;
	m_active = false;
	m_analyzing = false;
	m_directory = directory;
}

Engine* Engine::newEngine(int index)
{
	Engine *engine = NULL;

	QStringList engines;
	AppSettings->beginGroup("/Engines/");
	QString key(QString::number(index));
	QString name = AppSettings->value(key + "/Name").toString();
	QString command = AppSettings->value(key + "/Command").toString();
	QString options = AppSettings->value(key + "/Options").toString();
	QString directory = AppSettings->value(key + "/Directory").toString();
	QString protocol = AppSettings->value(key + "/Protocol").toString();
	AppSettings->endGroup();

	if (command.contains(' '))
		command = QString("\"%1\"").arg(command);
	QString exe = QString("%1 %2").arg(command).arg(options);

	if (protocol == "WinBoard")
		engine = new WBEngine(name, exe, directory);
	else	engine = new UCIEngine(name, exe, directory);

	return engine;
}

Engine::~Engine()
{
	if (m_process) {
		deactivate();
		m_process->disconnect();
		m_process->kill();
		m_process->waitForFinished(1000);
		delete m_process;
	}
}


void Engine::setLogStream(QTextStream* logStream)
{
	m_logStream = logStream;
}

void Engine::activate()
{
	if (m_active) {
		return;
	}

	m_process = new QProcess(this);
	m_process->setReadChannel(QProcess::StandardOutput);
	if (!m_directory.isEmpty())
		m_process->setWorkingDirectory(m_directory);
	connect(m_process, SIGNAL(started()), SLOT(protocolStart()));
	connect(m_process, SIGNAL(error(QProcess::ProcessError)), SLOT(processError()));
	connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(pollProcess()));
	connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(processExited()));
	m_process->start(m_command);
}

void Engine::deactivate()
{
	if (m_active)
		protocolEnd();
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
	if (m_logStream) {
		*m_logStream << "<-- " << message << endl;
		emit logUpdated();
	}

	QString out(message);
	out += "\n";
	if (m_process && message != "")
		m_process->write(out.toLocal8Bit());
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
	while (m_process->canReadLine()) {
		message = m_process->readLine();
		message = message.trimmed();
		if (m_logStream) {
			*m_logStream << "--> " << message << endl;
			emit logUpdated();
		}
		processMessage(message);
	}
}

void Engine::processError()
{
	emit error();
}

void Engine::processExited()
{
	setActive(false);
	delete m_process;
	m_process = 0;
}
