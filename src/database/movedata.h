/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef MOVEDATA_H
#define MOVEDATA_H

#include <array>
#include <cstdint>
#include <numeric>

#include "gamex.h"
#include "move.h"

/**
 * Count number of games per result
 */
class ResultsCounter
{
public:
    ResultsCounter() = default;
    ResultsCounter(std::initializer_list<Result> rs)
        : ResultsCounter()
    {
        for (auto r: rs)
            update(r);
    }

    /** Update corresponding counter */
    void update(Result r) { m_counts[r] += 1; }
    /** Get total number of games */
    size_t count() const { return std::accumulate(m_counts.cbegin(), m_counts.cend(), 0); }
    /** Get number of games having result @p r */
    size_t count(Result r) const { return m_counts[r]; }

    double scorePercentage() const;
    double whiteWinPercentage() const;
    double blackWinPercentage() const;

    ResultsCounter& operator+=(const ResultsCounter& rhs)
    {
        for (size_t i = 0; i < m_counts.size(); ++i)
            m_counts[i] += rhs.m_counts[i];
        return *this;
    }

    bool operator==(const ResultsCounter& rhs) const { return m_counts == rhs.m_counts; }
    bool operator!=(const ResultsCounter& rhs) const { return !(*this == rhs); }

    explicit operator bool() const { return count() != 0; }

private:
    std::array<size_t, 4> m_counts = {0, 0, 0, 0};
};

inline ResultsCounter operator+(const ResultsCounter& lhs, const ResultsCounter& rhs)
{
    ResultsCounter sum(lhs);
    sum += rhs;
    return sum;
}

template <int MinValue, int MaxValue>
class IntegralMetrics
{
public:
    IntegralMetrics() = default;
    IntegralMetrics(std::initializer_list<int> vs)
        : IntegralMetrics()
    {
        for (auto v: vs)
            update(v);
    }

    void update(int v)
    {
        if (Min <= v && v <= Max)
        {
            m_count += 1;
            m_sum += v;
        }
    }

    /** Total number of collected valid samples */
    size_t count() const { return m_count; }

    /** Average over valid samples */
    int average() const
    {
        return m_count? static_cast<int>(m_sum / static_cast<decltype(m_sum)>(m_count)): 0;
    }

    IntegralMetrics& operator+=(const IntegralMetrics& rhs)
    {
        m_count += rhs.m_count;
        m_sum += rhs.m_sum;
        return *this;
    }

    bool operator==(const IntegralMetrics& rhs) const { return m_count == rhs.m_count && m_sum == rhs.m_sum; }
    bool operator!=(const IntegralMetrics& rhs) const { return !(*this == rhs); }

    explicit operator bool() const { return m_count != 0; }

private:
    size_t m_count = 0;
    std::int64_t m_sum = 0;

    static constexpr int Min = MinValue;
    static constexpr int Max = MaxValue;
};

class MoveData
{
public:
    enum MoveType {StandardMove, GameEnd};
    MoveData();
    void addGame(GameX& g, Color c, MoveType movetype = StandardMove);
    double percentage() const;
    double percentageWhite() const;
    double percentageBlack() const;
    int averageRating() const;
    int averageYear() const;
    QString san;
    QString localsan;
    unsigned int count;
    unsigned int result[4];
    unsigned long long rating;
    unsigned int rated;
    long year;
    unsigned int dated;
    Move move;
    friend bool operator<(const MoveData& m1, const MoveData& m2);
    friend bool operator>(const MoveData& m1, const MoveData& m2);
    friend bool compareMove(const MoveData& m1, const MoveData& m2);
    friend bool compareScore(const MoveData& m1, const MoveData& m2);
    friend bool compareRating(const MoveData& m1, const MoveData& m2);
    friend bool compareYear(const MoveData& m1, const MoveData& m2);
    bool hasPercent() const;
};

bool operator<(const MoveData& m1, const MoveData& m2);
bool operator>(const MoveData& m1, const MoveData& m2);
bool compareMove(const MoveData& m1, const MoveData& m2);
bool compareScore(const MoveData& m1, const MoveData& m2);
bool compareRating(const MoveData& m1, const MoveData& m2);
bool compareYear(const MoveData& m1, const MoveData& m2);

#endif // MOVEDATA_H
