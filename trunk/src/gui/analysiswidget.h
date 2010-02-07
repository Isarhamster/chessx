#ifndef __ANALYSIS_WIDGET_H__
#define __ANALYSIS_WIDGET_H__

#include "board.h"
#include "engine.h"
#include "wbengine.h"
#include "uciengine.h"
#include "ui_analysiswidget.h"

/** @ingroup GUI
   The Analysis widget which shows engine output
*/

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
	/** Toggle analysis. */
	void analyze(bool run);
	/** Displays given analysis received from an engine. */
	void showAnalysis(const Engine::Analysis& analysis) const;
	/** The engine is now ready, as requested */
	void engineActivated();
private:

	Ui::AnalysisWidget ui;
	Engine* m_engine;
	bool m_active;
	Board m_board;
};

#endif // __ANALYSIS_H__

