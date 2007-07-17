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
	new analysis will be triggered. */
	void setPosition(const Board& board);
	/** Displays given analysis received from an engine. */
	void showAnalysis(const Engine::Analysis& analysis) const;
	/** Called when configuration was changed (probably by accepting changes in Preferences dialog. */
	void slotReconfigure();
	/** Use a different engine for analysis. */
	void changeEngine(int index);
	/** Toggle analysis. */
	void analyze(bool run);
	/** Stops analysis and destroys current engine. Used when changing engine or deleting whole widget. */
	void removeEngine();
	/** The engine is now ready, as requested */
	void engineActivated();
private:
	Ui::AnalysisWidget ui;
	Engine* m_engine;
	bool m_active;
	Board m_board;
	int m_closeTimer;
	void timerEvent(QTimerEvent*);
	void stopTimer();
private slots:
	void setShown(bool show);
};

#endif // __ANALYSIS_H__

