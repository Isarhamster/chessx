#include <QFile>
#include "enginex.h"
#include "gamex.h"
#include "gameevaluation.h"

GameEvaluation::GameEvaluation(int engineIndex, int msPerMove, GameX game) noexcept
    : engineIndex{engineIndex}
    , msPerMove{msPerMove}
    , game{game}
    , targetThreadCount{std::max(1, QThread::idealThreadCount())}
    , moveNumbers{0}
{
    if (targetThreadCount > 4)
        // We'll leave one logical core idle if we can afford it.
        --targetThreadCount;
    connect(&timer, &QTimer::timeout, this, &GameEvaluation::update);
}

void GameEvaluation::start()
{
    if (running)
    {
        throw std::logic_error{"Game evaluation already running"};
    }
    running = true;
    game.moveToStart();
    workers.clear();
    timer.stop();
    timer.start(std::chrono::milliseconds{100});
    // We place the first worker for the starting position here.
    workers.emplace_back(engineIndex, game.startingBoard(), game.board(), msPerMove, moveNumbers++);
}

void GameEvaluation::update() noexcept
{
    std::unordered_map<int, double> evaluations;
    for (std::list<GameEvaluationWorker>::iterator i{workers.begin()}; i != workers.end(); ++i)
    {
        i->update();
        evaluations.emplace(i->moveNumber, i->getLastScore());
        if (!i->isRunning())
        {
            // Erase *after* getting the value because erasing will destroy the worker
            i = workers.erase(i);
            --i;
        }
    }
    emit evaluationChanged(evaluations);
    while (static_cast<int>(workers.size()) < targetThreadCount)
    {
        if (!game.forward())
        {
            break;
        }
        try
        {
            int tempNum {moveNumbers++};
            workers.emplace_back(engineIndex, game.startingBoard(), game.board(), msPerMove, tempNum);
        }
        catch (...)
        {
            // Destroy any workers that have started.
            workers.clear();
            break;
        }
    }
    if (!workers.size())
    {
        timer.stop();
        emit evaluationComplete();
        running = false;
    }
}

void GameEvaluation::stop() noexcept
{
    workers.clear();
    timer.stop();
    emit evaluationComplete();
}

GameEvaluation::~GameEvaluation() noexcept
{
    workers.clear();
    timer.stop();
}

GameEvaluationWorker::GameEvaluationWorker(int engineIndex, BoardX const & startPosition, BoardX const & currentPosition,
        int msPerMove, int moveNumber)
    : moveNumber{moveNumber}
    , currentPosition{currentPosition}
    , msPerMove{msPerMove}
    , engine{EngineX::newEngine(engineIndex)}
{
    if (!engine)
    {
        throw std::runtime_error{"Failed to instantiate engine"};
    }
    if (!engine->m_mapOptionValues.contains("Threads"))
    {
        throw std::runtime_error{"Could not set engine threads to 1."};
    }
    engine->m_mapOptionValues["Threads"] = 1;
    engine->setStartPos(startPosition);
    connect(engine, &EngineX::activated, this, &GameEvaluationWorker::engineActivated);
    connect(engine, &EngineX::analysisStarted, this, &GameEvaluationWorker::engineAnalysisStarted);
    connect(engine, &EngineX::analysisUpdated, this, &GameEvaluationWorker::engineAnalysisUpdated);
    engine->activate();
    running = true;
}

GameEvaluationWorker::~GameEvaluationWorker() noexcept
{
    if (!engine)
    {
        return;
    }
    try
    {
        engine->deactivate();
        delete engine;
    }
    catch (...)
    {
        // I don't think we can do anything about this exception.
    }
}

void GameEvaluationWorker::engineActivated()
{
    EngineParameter parameters {msPerMove};
    engine->startAnalysis(currentPosition, 1, parameters, false, "");
}

void GameEvaluationWorker::engineAnalysisStarted()
{
    startTimestamp = QDateTime::currentDateTimeUtc();
}

void GameEvaluationWorker::engineAnalysisUpdated(Analysis const & analysis)
{
    if (analysis.bestMove())
    {
        // When the engine reports a best move, no score is reported, so we skip it
        return;
    }
    if (analysis.isMate())
    {
        lastScore = static_cast<double>(10);
        if (std::signbit(analysis.score()))
            lastScore *= -1;
        // If it's black's turn and black is winning, analysis.score() returns a
        // positive number in a "mating" condition. We need a score from white's
        // perspective.
        bool blacksTurn {static_cast<bool>(analysis.variation().size() % 2)};
        if (!blacksTurn)
            lastScore *= -1;
    }
    else
    {
        lastScore = analysis.fscore();
    }
}

double GameEvaluationWorker::getLastScore() const noexcept
{
    return lastScore;
}

bool GameEvaluationWorker::isRunning() const noexcept
{
    return running;
}

void GameEvaluationWorker::update() noexcept
{
    if (!startTimestamp)
    {
        // Maybe have a timeout and kill the engine if analysis still hasn't started?
        return;
    }
    if (startTimestamp->msecsTo(QDateTime::currentDateTimeUtc()) < msPerMove)
        return;
    engine->deactivate();
    running = false;
    // We'll let the destructor delete the engine since this is called during computation when
    // processing time is more premium than later.
}
