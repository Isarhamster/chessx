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

AnalysisWidget::AnalysisWidget()
		: m_engine(0)
{
	ui.setupUi(this);
	connect(ui.engineList, SIGNAL(activated(int)), SLOT(startEngine()));
	ui.analyzeButton->hide();
	//connect(ui.analyzeButton, SIGNAL(clicked(bool)), SLOT(analyze(bool)));
	//ui.analyzeButton->setFixedHeight(ui.engineList->sizeHint().height());
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
		ui.variationText->clear();
		m_engine = Engine::newEngine(index);
		connect(m_engine, SIGNAL(activated()), SLOT(engineActivated()));
		connect(m_engine, SIGNAL(analysisUpdated(const Analysis&)),
				  SLOT(showAnalysis(const Analysis&)));
		m_engine->activate();
		AppSettings->setValue("/Analysis/Engine", ui.engineList->itemText(index));
	}
}

void AnalysisWidget::stopEngine()
{
	if (m_engine) {
		m_engine->deactivate();
		delete m_engine;
		m_engine = NULL;
	}
}

void AnalysisWidget::engineActivated()
{
	m_engine->startAnalysis(m_board);
}

void AnalysisWidget::visibilityChanged()
{
	if (!parentWidget() || !parentWidget()->isVisible())
		stopEngine();
	else if (parentWidget() && parentWidget()->isVisible() && !m_engine)
		startEngine();
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
		startEngine();
	}
}

void AnalysisWidget::showAnalysis(const Analysis& analysis) const
{
	ui.variationText->setText(analysis.toString(m_board));
}

void AnalysisWidget::setPosition(const Board& board)
{
	m_board = board;
	if (m_engine && m_engine->isActive()) {
		ui.variationText->clear();
		m_engine->startAnalysis(m_board);
	}
}

