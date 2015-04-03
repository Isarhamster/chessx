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
    if ((event->key() == Qt::Key_Return) && ((event->modifiers() & Qt::KeyboardModifierMask) == Qt::NoModifier))
    {
        event->ignore();
        return;
    }

    if(event->key() == Qt::Key_Return && (event->modifiers() & Qt::ControlModifier))
    {
        event->ignore();
        QKeyEvent event2(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
        QPlainTextEdit::keyPressEvent(&event2);
    }

    QPlainTextEdit::keyPressEvent(event);
}
