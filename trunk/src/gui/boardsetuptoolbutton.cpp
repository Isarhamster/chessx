/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "boardsetuptoolbutton.h"

#include <QtGui>
#include <QPixmap>
#include <QStyle>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

BoardSetupToolButton::BoardSetupToolButton(QWidget *parent) :
    QLabel(parent)
{
}

void BoardSetupToolButton::mousePressEvent(QMouseEvent * e)
{
    QLabel::mousePressEvent(e);
    emit signalClicked(m_piece);
    emit signalDragStarted(this, e);
    slotSetSelected();
}

void BoardSetupToolButton::mouseReleaseEvent(QMouseEvent * e)
{
    QLabel::mouseReleaseEvent(e);
}

void BoardSetupToolButton::resizeEvent(QResizeEvent * e)
{
    int w = e->size().width() - 4;
    int h = e->size().height() - 4;
    QPixmap p1;
    if(m_pixmap.isNull())
    {
        p1 = (style()->standardIcon(QStyle::SP_TrashIcon)).pixmap(w, h);
    }
    else
    {
        p1 = m_pixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    setPixmap(p1);
}

void BoardSetupToolButton::slotClearBackground(Piece p)
{
    if(p == m_piece)
    {
        slotSetSelected();
    }
    else
    {
        setStyleSheet("QLabel { background-color: lightgray }");
    }
}

void BoardSetupToolButton::slotSetSelected()
{
    setStyleSheet("QLabel { background-color: yellow }");
}
