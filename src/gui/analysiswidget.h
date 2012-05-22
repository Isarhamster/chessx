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
#include "tablebase.h"
#include <QtGui>

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
	/** Start currently selected engine. */
	void startEngine();
	/** Stop any running  engine. */
	void stopEngine();
    /** Stop game analysis when analysis dock is hidden. */
    void slotVisibilityChanged(bool);
	/** Is any engine running. */
	bool isEngineRunning() const;
private slots:
	/** Stop if analysis is no longer visible. */
	void toggleAnalysis();
	/** Displays given analysis received from an engine. */
	void showAnalysis(const Analysis& analysis);
	/** The engine is now ready, as requested */
	void engineActivated();
	/** There was an error while running engine. */
	void engineError();
	/** Add variation. */
	void slotLinkClicked(const QUrl& link);
	/** Number of visible lines was changed. */
	void slotMpvChanged(int mpv);
	/** Show tablebase move information. */
	void showTablebaseMove(Move move, int score);
signals:
	void addVariation(const Analysis& analysis);
private:
	/** Should analysis be running. */
	bool isAnalysisEnabled() const;
	/** Update analysis. */
	void updateAnalysis();

	QList<Analysis> m_analyses;
	Ui::AnalysisWidget ui;
	Engine* m_engine;
	bool m_active;
	Board m_board;
	QString m_tablebaseEvaluation;
	Tablebase* m_tablebase;
};

#endif // __ANALYSIS_WIDGET_H__

