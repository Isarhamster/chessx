/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "plaintextedit.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

PlainTextEdit::PlainTextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
}

void PlainTextEdit::keyPressEvent(QKeyEvent* event)
{
    // Pressing Return is ignored
    if ((event->key() == Qt::Key_Return) && ((event->modifiers() & Qt::KeyboardModifierMask) == Qt::NoModifier))
    {
        event->ignore();
        return;
    }

    // Pressing Control+Return sends a return directly to the underlying QPLainTextEdit thus inserting a Return
    if(event->key() == Qt::Key_Return && (event->modifiers() & Qt::ControlModifier))
    {
        event->ignore();
        QKeyEvent event2(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
        QPlainTextEdit::keyPressEvent(&event2);
    }

    // ZoomIn / Out with Control++ or Control+-
    if(event->key() == Qt::Key_Minus && (event->modifiers() & Qt::ControlModifier))
    {
        zoomOut();
    }
    else if(event->key() == Qt::Key_Plus && (event->modifiers() & Qt::ControlModifier))
    {
        zoomIn();
    }

    QPlainTextEdit::keyPressEvent(event);
}

void PlainTextEdit::wheelEvent(QWheelEvent *e)
{
    Qt::KeyboardModifiers m = e->modifiers();

    // ZoomIn / Out with Control+Alt+Wheel
    if ((m & (Qt::ControlModifier | Qt::AltModifier)) == (Qt::ControlModifier | Qt::AltModifier))
    {
        const int delta = e->delta();
        if (delta < 0)
            zoomOut();
        else if (delta > 0)
            zoomIn();
    }
    QPlainTextEdit::wheelEvent(e);
}
