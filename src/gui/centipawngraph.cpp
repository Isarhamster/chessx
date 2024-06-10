#include "centipawngraph.h"

#include <QLCDNumber>
#include <QGridLayout>
#include <QPainter>
#include <QPushButton>
#include <QComboBox>

#include "chartwidget.h"
#include "gameevaluation.h"

CentipawnGraph::CentipawnGraph(QWidget* parent)
    : QWidget(parent)
    , evaluation{nullptr}
{
    setObjectName("CentipawnGraph");
    QGridLayout * layout = new QGridLayout(this);
    m_startAnalysis = new QPushButton{"Start Analysis"};
    m_startAnalysis->setDefault(true);
    connect(m_startAnalysis, &QPushButton::clicked, this, &CentipawnGraph::analysisRequested);
    m_engineList = new QComboBox;
    setupEngineList();
    layout->addWidget(m_startAnalysis, /*row*/0, 0);
    layout->addWidget(m_engineList, 0, 1);
    m_chart = new ChartWidget();
    m_chart->setObjectName("ChartWidget");
    m_chart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(m_chart, 1, 0, 1, 2);
    m_chart->show();
    connect(m_chart, &ChartWidget::halfMoveRequested, this, &CentipawnGraph::requestPly);
    adjustSize();
}

void CentipawnGraph::setupEngineList() noexcept
{
    m_engineList->clear();
    EngineList enginesList;
    enginesList.restore();
    QStringList names = enginesList.names();
    m_engineList->setEditable(false);
    if (!names.size())
    {
        m_engineList->addItem("No Engines Configured");
        m_engineList->setEnabled(false);
        m_startAnalysis->setEnabled(false);
    }
    else
    {
        m_engineList->addItems(names);
        m_engineList->setEnabled(true);
        m_startAnalysis->setEnabled(true);
        m_engineList->setToolTip("Select a pre-configured engine");
    }
}

void CentipawnGraph::slotDisplayCurrentPly(int ply)
{
    updateGeometry();
    m_chart->setPly(ply);
    m_chart->show();
    m_chart->show();
}

void CentipawnGraph::slotDisplayMaterial(const QList<double>& material)
{
    m_chart->setValues(0, material);
}

void CentipawnGraph::slotDisplayEvaluations(const QList<double>& evaluations)
{
    // If we don't have a computed evaluation, use this one retrieved from annotations.
    m_chart->setValues(1, evaluations);
}

void CentipawnGraph::analysisRequested(bool /*checked*/) noexcept
{
    // In order to start analysis, we need the current game which we don't have.
    // We need to emit a signal that the main window can handle so that the
    // main window can call back with the current game to get analysis started.
    emit startAnalysisRequested();
}

void CentipawnGraph::startAnalysis(GameX const & game) noexcept
{
    try
    {
        if (evaluation)
        {
            return;
        }
        m_startAnalysis->setEnabled(false);
        m_engineList->setEnabled(false);
        evaluation = std::make_unique<GameEvaluation>(m_engineList->currentIndex(), 1000, game);
        connect(evaluation.get(), &GameEvaluation::evaluationChanged, this, &CentipawnGraph::evaluationChanged);
        connect(evaluation.get(), &GameEvaluation::evaluationComplete, this, &CentipawnGraph::evaluationComplete);
        currentGame = game;
        scores.clear();
        scores.reserve(currentGame.cursor().countMoves()+1);
        for (int i{0}; i < currentGame.cursor().countMoves()+1; ++i)
            scores << 0;
        evaluation->start();
    }
    catch (...)
    {
        // Failed to start evaluation; swallowing error
    }
}

void CentipawnGraph::evaluationComplete() noexcept
{
    evaluation = nullptr;
    m_startAnalysis->setEnabled(true);
    m_engineList->setEnabled(true);
}

void CentipawnGraph::evaluationChanged(std::unordered_map<int, double> const & scoreUpdates) noexcept
{
    GameX tempGame{currentGame};
    tempGame.moveToStart();
    for (std::pair<int, double> const & score : scoreUpdates)
    {
        int moveNumber = score.first;
        if (scores.size() <= moveNumber)
        {
            continue;
        }
        double newScore {std::clamp(score.second, -10.0, 10.0)};
        scores.replace(moveNumber, newScore);
    }
    m_chart->setValues(1, scores);
}
