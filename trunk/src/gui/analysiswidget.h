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
	void setPosition(const Board& board);
	void showAnalysis(const Engine::Analysis& analysis) const;
	void slotReconfigure();
	/** user wants to use a different engine for analysis */
	void changeEngine(int index);
	/** user wants to begin or end analysis */
	void analyze(bool run);
	/** destroy existing engine */
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

