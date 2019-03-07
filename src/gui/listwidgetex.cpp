/****************************************************************************
*   Copyright (C) 2019 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include <QFont>
#include <QObject>
#include <QWheelEvent>
#include "listwidgetex.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

ListWidgetEx::ListWidgetEx(QWidget*)
{
}

bool ListWidgetEx::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::Wheel)
    {
        QWheelEvent *wEvent = dynamic_cast<QWheelEvent *>(event);
        if (wEvent)
        {
            myWheelEvent(wEvent);
        }
    }
    return false;
}

void ListWidgetEx::setFontSize(int fontSize)
{
    fontSize = std::max(fontSize, 8);
    QFont f = font();
    f.setPointSize(fontSize);
    setFont(f);
}

void ListWidgetEx::myWheelEvent(QWheelEvent* e)
{
    unsigned int button = e->modifiers() & Qt::KeyboardModifierMask;
    if (button == (Qt::ControlModifier|Qt::AltModifier))
    {
        QFont f = font();
        int n = e->delta()>0 ? 1 : -1;
        int fontSize = f.pointSize() + n;
        setFontSize(fontSize);
    }
}
