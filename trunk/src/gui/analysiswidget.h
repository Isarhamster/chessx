/***************************************************************************
 *   (C) 2008-2010 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __ANALYSIS_WIDGET_H__
#define __ANALYSIS_WIDGET_H__

#include "engine.h"
#include "wbengine.h"
#include "uciengine.h"
#include "ui_analysiswidget.h"

/** @ingroup GUI
   The Analysis widget which shows engine output
*/

class Analysis;
class Board;

class AnalysisWidget : public QWidget
{
	Q_OBJECT
public:
	AnalysisWidget();
	~AnalysisWidget();
public slots:
	/** Sets new position. If analysis is active, the current content will be cleared and
	new analysis will be performed. */
	void setPosition(const Board& board);
	/** Called when configuration was changed (either on startup or from Preferences dialog. */
	void slotReconfigure();
	/** Stop if analysis is no longer visible. */
	void visibilityChanged();
	/** Start currently selected engine. */
	void startEngine();
	/** Stop any running  engine. */
	void stopEngine();
private slots:
	/** Displays given analysis received from an engine. */
	void showAnalysis(const Analysis& analysis) const;
	/** The engine is now ready, as requested */
	void engineActivated();
private:

	Ui::AnalysisWidget ui;
	Engine* m_engine;
	bool m_active;
	Board m_board;
};

#endif // __ANALYSIS_H__

