#ifndef GAME_EVALUATION_H_INCLUDED
#define GAME_EVALUATION_H_INCLUDED
#include <QPromise>
#include "board.h"
#include "gamex.h"
#include "enginex.h"

/** @ingroup Core
   The GameEvaluation class represents an algorith for evaluating the centipawn score at
   every move in a game.
*/

class GameEvaluationWorker final : public QObject
{
    Q_OBJECT
public:
    GameEvaluationWorker(int engineIndex, BoardX const & startPosition, BoardX const & currentPosition,
            int msPerMove, int moveNumber);
    // QObjects can't be copied or moved
    GameEvaluationWorker & operator=(GameEvaluationWorker &&) = delete;
    GameEvaluationWorker(GameEvaluationWorker&&) = delete;
    ~GameEvaluationWorker() noexcept;

    double getLastScore() const noexcept;
    bool isRunning() const noexcept;
    void update() noexcept;

    int const moveNumber;

private:
    BoardX currentPosition;
    int msPerMove;
    EngineX * engine;
    double lastScore{0};
    bool running{false};
    std::optional<QDateTime> startTimestamp;

private slots:
    void engineActivated();
    void engineAnalysisStarted();
    void engineAnalysisUpdated(Analysis const &);
};

class GameEvaluation final : public QObject
{
    Q_OBJECT
public :
    GameEvaluation(int engineIndex, int msPerMove, GameX) noexcept;
    ~GameEvaluation() noexcept;

    // This function does not block, but uses a QT Timer to call the
    // evaluationChanged signal periodically.
    void start();
    void stop() noexcept;
signals:
    void evaluationChanged(std::unordered_map<int, double> const & blah);
    void evaluationComplete();

private:
    int const engineIndex;
    int const msPerMove;
    GameX game;
    QTimer timer;
    int targetThreadCount;
    QString line;
    int moveNumbers;

    bool running{false};
    std::list<GameEvaluationWorker> workers;
    void update() noexcept;

};

#endif	// GAME_EVALUATION_H_INCLUDED

