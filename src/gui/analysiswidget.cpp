#include "settings.h"
#include "board.h"
#include "analysiswidget.h"
#include "enginelist.h"

#include <QShowEvent>

AnalysisWidget::AnalysisWidget()
		: m_engine(0)
{
	ui.setupUi(this);
	connect(ui.engineList, SIGNAL(activated(int)), SLOT(startEngine()));
	connect(ui.analyzeButton, SIGNAL(clicked(bool)), SLOT(analyze(bool)));
	ui.analyzeButton->setFixedHeight(ui.engineList->sizeHint().height());
}

AnalysisWidget::~AnalysisWidget()
{
	stopEngine();
}

void AnalysisWidget::stopEngine()
{
	if (m_engine) {
		analyze(false);
		delete m_engine;
		m_engine = NULL;
	}
}

void AnalysisWidget::startEngine()
{
	bool analysing = ui.analyzeButton->isChecked();
	int index = ui.engineList->currentIndex();
	stopEngine();
	if (index != -1) {
		m_engine = Engine::newEngine(index);
		connect(m_engine, SIGNAL(activated()), SLOT(engineActivated()));
		connect(m_engine, SIGNAL(analysisUpdated(const Engine::Analysis&)),
				  SLOT(showAnalysis(const Engine::Analysis&)));
		analyze(analysing);
		AppSettings->setValue("/Analysis/Engine", ui.engineList->itemText(index));
	}
}

void AnalysisWidget::engineActivated()
{
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
			m_engine->deactivate();
			ui.analyzeButton->setChecked(false);
			ui.analyzeButton->setText(tr("Analyze"));
		}
	}
}

void AnalysisWidget::visibilityChanged()
{
	if (!parentWidget() || !parentWidget()->isVisible())
		analyze(false);
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
	else
		ui.engineList->setCurrentIndex(0);
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
	if (m_engine && m_engine->isActive()) {
		ui.variationText->clear();
		m_engine->startAnalysis(m_board);
	}
}

