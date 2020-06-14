#ifndef GAMETOOLBAR_H
#define GAMETOOLBAR_H

#include <QToolBar>

class QLCDNumber;
class ChartWidget;

class GameToolBar : public QToolBar
{
    Q_OBJECT

public:
    GameToolBar(const QString& title, QWidget* parent = nullptr);

public slots:
    void slotDisplayCurrentPly(int ply);

private:
    QLCDNumber* m_clock1;
    QLCDNumber* m_clock2;
public:
    ChartWidget* m_chart;
};

#endif
