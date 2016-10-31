/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include <QtWidgets>
#include <QTimer>
#include <QSound>

#include "digitalclock.h"
#include "settings.h"

DigitalClock::DigitalClock(QWidget *parent)
    : QLCDNumber(parent)
{
    setSegmentStyle(Filled);
    setDigitCount(7);

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));

    tockSound = new QSound(":/sounds/woodthunk.wav");
    m_useTock = false;
    m_tockToDo = 0;
    m_bFirstTestForTock = false;
    m_countDown = true;
}

void DigitalClock::StartCountDown(bool start)
{
    if (start)
    {
        timer->start(1000);
    }
    else
    {
        StopCountDown();
    }
}

void DigitalClock::StopCountDown(bool stop)
{
    if (stop)
    {
        timer->stop();
    }
    else
    {
        StartCountDown();
    }
}

void DigitalClock::ResetTock(bool useTock, bool countDown)
{
    m_useTock = useTock;
    m_countDown = countDown;
    m_bFirstTestForTock = true;
    m_tockToDo = AppSettings->getValue("/Sound/Move").toBool() ? 3 : 0;
}

void DigitalClock::updateTime()
{
    QString strTime = time();
    strTime = DecrementTime(strTime);
    setTime(strTime);

    if (m_useTock)
    {
        TestTocks(strTime);
    }

    if (TestColor(strTime,10))
    {
        //setBackgroundColor(Qt::yellow);
    }
    else
    {
        //resetBackgroundColor();
    }

    StartCountDown();
}

void DigitalClock::TestTocks(QString s)
{
#ifdef USE_SOUND
    if (m_tockToDo && m_countDown)
    {
        QTime tm = QTime::fromString(s,"h:m:ss");
        if (tm.msecsSinceStartOfDay()<=m_tockToDo*10000)
        {
            if (!m_bFirstTestForTock)
            {
                tockSound->play();
                --m_tockToDo;
            }
        }
        else
        {
            m_bFirstTestForTock = false;
        }
    }
#endif
}

QString DigitalClock::time() const
{
    return m_time;
}

void DigitalClock::setTime(const QString &time)
{
    m_time = time;
    setDigitCount(m_time.length());
    display(m_time);
}

bool DigitalClock::TestColor(QString s, int seconds) const
{
    QTime t = QTime::fromString(s,"h:m:ss");
    if (t.msecsSinceStartOfDay()<seconds*1000)
    {
        return true;
    }
    return false;
}

QString DigitalClock::DecrementTime(QString s) const
{
    QString result;
    QTime t = QTime::fromString(s,"h:m:ss");

    if (t.isValid())
    {
        t = t.addSecs(m_countDown ? -1 : +1);
        result = t.toString("h:mm:ss");
    }
    else
    {
        t = QTime::fromString(s,"m:ss");
        if (t.isValid())
        {
            if (!m_countDown || abs(t.secsTo(QTime(0,0,0,0)))>1)
            {
                t = t.addSecs(m_countDown ? -1 : +1);
            }
            else
            {
                t = QTime(0,0,0,0);
            }
            result = t.toString("m:ss");
        }
    }
    return result;
}
