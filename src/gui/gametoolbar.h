#ifndef GAMETOOLBAR_H
#define GAMETOOLBAR_H

#include <QToolBar>

#include "piece.h"

class QLCDNumber;
class ChartWidget;

class GameToolBar : public QToolBar
{
    Q_OBJECT

public:
    GameToolBar(const QString& title, QWidget* parent = nullptr);

signals:
    void requestPly(int);

public slots:
    void slotDisplayCurrentPly(int ply);
    void slotDisplayMaterial(const QList<double>& material);
    void slotDisplayEvaluations(const QList<double>& evaluations);
    void slotDisplayTime(const QString& timeWhite, const QString& timeBlack);
    void slotDisplayTime(Color color, const QString& time);

private:
    QLCDNumber* m_clock1;
    QLCDNumber* m_clock2;
    ChartWidget* m_chart;
};

#endif
