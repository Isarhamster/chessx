#include "settings.h"
#include "board.h"
#include "analysiswidget.h"
#include "enginelist.h"

#include <QShowEvent>

AnalysisWidget::AnalysisWidget()
		: m_engine(0), m_active(false), m_closeTimer(0)
{
	ui.setupUi(this);
	connect(ui.engineList, SIGNAL(activated(int)), SLOT(changeEngine(int)));
	connect(ui.analyzeButton, SIGNAL(clicked(bool)), SLOT(analyze(bool)));
	ui.analyzeButton->setFixedHeight(ui.engineList->sizeHint().height());
}

AnalysisWidget::~AnalysisWidget()
{
	removeEngine();
}

void AnalysisWidget::removeEngine()
{
	if (m_engine) {
		analyze(false);
		delete m_engine;
		m_engine = NULL;
	}
}

void AnalysisWidget::changeEngine(int index)
{
	bool oldValue = m_active;
	removeEngine();
	if (index >= 0 && index < ui.engineList->count()) {
		ui.engineList->setCurrentIndex(index);
		m_engine = Engine::newEngine(index);
		connect(m_engine,
			SIGNAL(analysisUpdated(const Engine::Analysis&)),
			SLOT(showAnalysis(const Engine::Analysis&)));
		connect(m_engine, SIGNAL(activated()), SLOT(engineActivated()));
		analyze(oldValue);
		AppSettings->setValue("/Analysis/Engine", ui.engineList->itemText(index));
	}
}

void AnalysisWidget::engineActivated()
{
	m_active = true;
	m_engine->startAnalysis(m_board);
}

void AnalysisWidget::analyze(bool run)
{
	if (m_engine) {
		if (run) {
			m_engine->activate();
			ui.variationText->clear();
			ui.analyzeButton->setChecked(true);
			ui.analyzeButton->setText(tr("Stop"));
		} else {
			m_active = false;
			m_engine->deactivate();
			ui.analyzeButton->setChecked(false);
			ui.analyzeButton->setText(tr("Analyze"));
		}
	}
}

void AnalysisWidget::setShown(bool show)
{
	if (show)
		stopTimer();
	else	m_closeTimer = startTimer(250);
}

void AnalysisWidget::slotReconfigure()
{
	QString oldEngineName = ui.engineList->currentText();
	qDebug() << oldEngineName;

	EngineList enginesList;
	enginesList.restore();
	QStringList names = enginesList.names();
	ui.engineList->clear();
	ui.engineList->insertItems(0,	names);
	int index = names.indexOf(oldEngineName);
	if (index != -1)
		ui.engineList->setCurrentIndex(index);
	else changeEngine(0);
}

void AnalysisWidget::showAnalysis(const Engine::Analysis& analysis) const
{
	Board board = m_board;
	float score = analysis.score;
	QString out;
	if (analysis.mateIn) {
		int mateIn = int(score);
		QString color = mateIn >= 0 ? "000080" : "800000";
		QString text = tr("Mate in");
		out = QString("<font color=\"#%1\"><b>%2 %3</b></font> ")
		      .arg(color).arg(text).arg(abs(mateIn));
	} else if (score >= 0.0)
		out = QString("<font color=\"#000080\"><b>+%1</b></font> ").arg(score, 0, 'f', 2);
	else out = QString("<font color=\"#800000\"><b>%1</b></font> ").arg(score, 0, 'f', 2);
	int moveNo = m_board.moveNumber();
	bool white = m_board.toMove() == White;
	for (int i = 0; i < analysis.variation.size(); ++i) {
		if (white)
			out += QString::number(moveNo++) + ". ";
		else  if (i == 0)
			out += QString::number(moveNo++) + "... ";
		const Move& m = analysis.variation[i];
		out += board.moveToSan(m);
		out += " ";
		board.doMove(m);
		white = !white;
	}
	out += QString(" (depth %1)").arg(analysis.depth);
	ui.variationText->setText(out);
}

void AnalysisWidget::setPosition(const Board& board)
{
	m_board = board;
	if (m_engine && m_active) {
		ui.variationText->clear();
		m_engine->startAnalysis(m_board);
	}
}

void AnalysisWidget::stopTimer()
{
	if (m_closeTimer) {
		killTimer(m_closeTimer);
		m_closeTimer = 0;
	}
}

void AnalysisWidget::timerEvent(QTimerEvent*)
{
	/*
	 * If user closes dock that contains AnalysisWidget we want to
	 * stop the engine.  So we monitor the toggled(bool) signal to
	 * shut the engine off when we see toggled(false).
	 *
	 * However Qt sends a toggled(false) followed immediately by
	 * a toggled(true) when a dock is simply moved to a new location.
	 * This unfortunate Qt policy means the engine would be stopped
	 * if the user simply relocates the dock.
	 *
	 * To deal with this, we wait 1/4 second after receiving
	 * toggled(false) to see if we get a toggled(true).  If we
	 * don't receive the toggled(true) promptly, this method will run.
	 *
	 * If this method runs, the dock must really be hidden, not just
	 * being moved, so disable engine.
	 *
	 * Surely there is a better way to get a clean signal from Qt to
	 * tell us when Dock is hidden and avoid the need for this hack.
	 *
	 */
	stopTimer();
	analyze(false);
}

