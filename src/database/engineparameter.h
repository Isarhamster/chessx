/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ENGINEPARAMETER_H
#define ENGINEPARAMETER_H

#include <QString>

class EngineParameter
{
public:
    EngineParameter(unsigned int mt, int moves, unsigned int white, unsigned int black)
    {
        tm           = TIME_SUDDEN_DEATH;
        ms_totalTime = mt;
        ms_increment = 0;
        ms_bonus     = 1000;
        ms_white     = white;
        ms_black     = black;
        movesToDo    = moves;
        annotateEgt  = true;
        allowBook    = false;
        bookMove     = 0;
        engineStarts = false;
    }

    EngineParameter(int mt=0)
    {
        tm           = TIME_GONG;
        ms_totalTime = mt;
        ms_increment = 0;
        ms_bonus     = 1000;
        ms_white     = mt;
        ms_black     = mt;
        movesToDo    = 999;
        annotateEgt  = true;
        allowBook    = false;
        bookMove     = 0;
        engineStarts = false;
    }

    QString timeAsString() const
    {
        if (tm == TIME_GONG)
            return QString("%1s").arg(ms_totalTime/1000);
        else
            return QString("%1s/%2:0:%3").arg(movesToDo).arg(ms_totalTime/1000).arg(ms_increment);
    }

    void reset()
    {
        ms_white     = ms_totalTime;
        ms_black     = ms_totalTime;
    }

    typedef enum
    {
        TIME_GONG, TIME_SUDDEN_DEATH
    } TimeModus;

    TimeModus tm;

    /// per move in GONG, total in SUDDEN_DEATH
    int ms_totalTime;
    unsigned int ms_white;  ///< SUDDEN_DEATH only
    unsigned int ms_black;  ///< SUDDEN_DEATH only
    int movesToDo;          ///< SUDDEN_DEATH only
    bool annotateEgt;
    unsigned int ms_bonus;
    unsigned int ms_increment;
    bool allowBook;
    int bookMove;
    bool engineStarts;
};

#endif // ENGINEPARAMETER_H

