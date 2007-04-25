//////////////////////////////////////////////////////////////////////
//
//  FILE:       timer.h
//              Millisecond resolution timer class
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    1.7
//
//  Notice:     Copyright (c) 1999  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#ifndef SCID_TIMER_H
#define SCID_TIMER_H

//////////////////////////////////////////////////////////////////////
// Timer::MilliSecs() returns the number of milliseconds since the
// timer was constructed or its Reset() method was last called.
// It uses gettimeofday() in Unix, or ftime() in Windows.


#ifdef WIN32
#  include <sys/timeb.h>
#else
#  ifndef NO_GETTIMEOFDAY
#    include <sys/time.h>
#  endif
#endif

namespace Guess
{

struct msecTimerT {
    long seconds;
    long milliseconds;
};


inline static void 
setTimer (msecTimerT *t)
{
#ifdef WIN32
    // Use ftime() call in Windows:
    struct timeb tb;
    ftime (&tb);
    t->seconds = tb.time;
    t->milliseconds = tb.millitm;
#else
#  ifdef NO_GETTIMEOFDAY
    // No gettimeofday() call, so timing is disabled.
    t->seconds = 0;
    t->milliseconds = 0;
#  else
    // Use gettimeofday() system call in Unix:
    struct timeval timeOfDay;
    gettimeofday (&timeOfDay, NULL);
    t->seconds = timeOfDay.tv_sec;
    t->milliseconds = timeOfDay.tv_usec / 1000;
#  endif  // NO_GETTIMEOFDAY
#endif  // WIN32
}


class Timer {

  private:

    msecTimerT StartTime;

  public:
  
    Timer() { Reset (); }
    ~Timer() {}
    void Reset() { setTimer (&StartTime); }

    int MilliSecs (void) {
        msecTimerT nowTime;
        setTimer (&nowTime);
        return 1000 * (nowTime.seconds - StartTime.seconds) +
                    (nowTime.milliseconds - StartTime.milliseconds);
    }

    int CentiSecs (void) {
        msecTimerT nowTime;
        setTimer (&nowTime);
        return 100 * (nowTime.seconds - StartTime.seconds) +
                    (nowTime.milliseconds - StartTime.milliseconds) / 10;
    }

};

} // End namespace Guess

#endif

//////////////////////////////////////////////////////////////////////
//  EOF: timer.h
//////////////////////////////////////////////////////////////////////
