/****************************************************************************
*   Copyright (C) 2016 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef DIGITALCLOCK_H
#define DIGITALCLOCK_H

#include <QLCDNumber>

class QSound;
class QTimer;

class DigitalClock : public QLCDNumber
{
    Q_OBJECT

public:
    DigitalClock(QWidget *parent = 0);
	~DigitalClock();

    QString time() const;
    void setTime(const QString &time);
    void ToggleCountDown();

public slots:
    void StartCountDown(bool start = true);
    void StopCountDown(bool stop = true);
    void ResetTock(bool useTock, bool countDown);

protected:
    bool TestColor(QString s, int seconds) const;
    QString DecrementTime(QString s) const;

private slots:
    void updateTime();

private:
    QTimer* timer;
    QSound* tockSound;
    bool m_useTock;
    int m_tockToDo;
    bool m_bFirstTestForTock;
    void TestTocks(QString s);
    QString m_time;
    bool m_countDown;
};

#endif
