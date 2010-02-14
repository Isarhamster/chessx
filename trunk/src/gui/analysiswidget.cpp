/***************************************************************************
 *   (C) 2008-2010 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "settings.h"
#include "analysis.h"
#include "board.h"
#include "analysiswidget.h"
#include "enginelist.h"

#include <QShowEvent>
#include <QMessageBox>

AnalysisWidget::AnalysisWidget()
		: m_engine(0)
{
	ui.setupUi(this);
	connect(ui.engineList, SIGNAL(activated(int)), SLOT(toggleAnalysis()));
	connect(ui.analyzeButton, SIGNAL(clicked(bool)), SLOT(toggleAnalysis()));
	connect(ui.variationText, SIGNAL(anchorClicked(QUrl)),
			  SLOT(slotLinkClicked(QUrl)));
	ui.analyzeButton->setFixedHeight(ui.engineList->sizeHint().height());
}

AnalysisWidget::~AnalysisWidget()
{
	stopEngine();
}

void AnalysisWidget::startEngine()
{
	int index = ui.engineList->currentIndex();
	stopEngine();
	if (index != -1) {
		if (parentWidget() && !parentWidget()->isVisible())
			parentWidget()->show();
		ui.variationText->clear();
		m_engine = Engine::newEngine(index);
		connect(m_engine, SIGNAL(activated()), SLOT(engineActivated()));
		connect(m_engine, SIGNAL(error()), SLOT(engineError()));
		connect(m_engine, SIGNAL(analysisUpdated(const Analysis&)),
				  SLOT(showAnalysis(const Analysis&)));
		m_engine->activate();
		AppSettings->setValue("/Analysis/Engine", ui.engineList->itemText(index));
	}
}

void AnalysisWidget::stopEngine()
{
	ui.analyzeButton->setChecked(false);
	ui.analyzeButton->setText(tr("Analyze"));
	if (m_engine) {
		m_engine->deactivate();
		delete m_engine;
		m_engine = 0;
	}
}

bool AnalysisWidget::isEngineRunning() const
{
	return m_engine && ui.analyzeButton->isChecked();
}

void AnalysisWidget::engineActivated()
{
	ui.analyzeButton->setChecked(true);
	ui.analyzeButton->setText(tr("Stop"));
	m_engine->startAnalysis(m_board);
}

void AnalysisWidget::engineError()
{
	QMessageBox::warning(this, tr("Error"),
						 tr("There was an error running engine <b>%1</b>.")
						 .arg(ui.engineList->currentText()));
	delete m_engine;
	m_engine = 0;
	stopEngine();
}

void AnalysisWidget::toggleAnalysis()
{
	if (!isAnalysisEnabled())
		stopEngine();
	else startEngine();
}

void AnalysisWidget::slotReconfigure()
{
	QString oldEngineName = ui.engineList->currentText();
	if (oldEngineName.isEmpty())
		oldEngineName = AppSettings->value("/Analysis/Engine").toString();

	EngineList enginesList;
	enginesList.restore();
	QStringList names = enginesList.names();
	ui.engineList->clear();
	ui.engineList->insertItems(0,	names);
	int index = names.indexOf(oldEngineName);
	if (index != -1)
		ui.engineList->setCurrentIndex(index);
	else {
		ui.engineList->setCurrentIndex(0);
		stopEngine();
	}
}

void AnalysisWidget::showAnalysis(const Analysis& analysis)
{
	ui.variationText->setText(analysis.toString(m_board));
	m_analysis = analysis;
}

void AnalysisWidget::setPosition(const Board& board)
{
	m_board = board;
	if (m_engine && m_engine->isActive()) {
		ui.variationText->clear();
		m_engine->startAnalysis(m_board);
	}
}

void AnalysisWidget::slotLinkClicked(const QUrl&)
{
	emit addVariation(m_analysis);
}


bool AnalysisWidget::isAnalysisEnabled() const
{
	if (!parentWidget())
		return false;
	if (!parentWidget()->isVisible() || !ui.analyzeButton->isChecked())
		return false;
	return true;
}
