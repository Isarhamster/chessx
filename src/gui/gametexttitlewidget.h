#ifndef GAMETEXTTITLEWIDGET_H
#define GAMETEXTTITLEWIDGET_H

#include <QLabel>

class GameTextTitleWidget : public QLabel
{
public:
    GameTextTitleWidget(QWidget* parent);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QWidget* m_parent;
    void HandleStyle(QMouseEvent *event);
    bool floatStyle;
};

#endif // GAMETEXTTITLEWIDGET_H
