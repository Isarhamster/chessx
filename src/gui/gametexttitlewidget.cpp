#include "gametexttitlewidget.h"
#include <QDockWidget>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPainter>
#include <QLayout>

GameTextTitleWidget::GameTextTitleWidget(QWidget* parent)
{
    m_parent = parent;
    floatStyle = false;
    QFont f = font();
    f.setPointSize(f.pointSize()-2);
    setFont(f);
}

void GameTextTitleWidget::HandleStyle(QMouseEvent *event)
{
    QDockWidget* dock = qobject_cast<QDockWidget*>(m_parent);
    if (dock)
    {
        if (dock->isFloating() || event->button()==Qt::LeftButton)
        {
            if (!floatStyle)
            {
                floatStyle = true;
                // Set further style like mask for dock or label
              }
        }
        else
        {
            if (floatStyle)
            {
                floatStyle = false;
                // Revert setting styles
            }
        }
    }
}

void GameTextTitleWidget::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    event->ignore();
    HandleStyle(event);
}

void GameTextTitleWidget::mouseMoveEvent(QMouseEvent *event)
{
    QLabel::mouseMoveEvent(event);
    event->ignore();
    HandleStyle(event);
}

void GameTextTitleWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
    event->ignore();
    HandleStyle(event);
}

