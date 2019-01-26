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
    QLabel(parent), m_piece(Empty)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setMinimumSize(QSize(10, 10));
    setAlignment(Qt::AlignCenter);
    setScaledContents(false);
    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Raised);
}

Piece BoardSetupToolButton::piece() const
{
    return m_piece;
}

void BoardSetupToolButton::setPiece(const Piece &piece)
{
    m_piece = piece;
}

QPixmap BoardSetupToolButton::BasePixmap() const
{
    return m_pixmap;
}

void BoardSetupToolButton::setBasePixmap(const QPixmap& pm)
{
    if (pm.isNull())
    {
        m_pixmap = style()->standardIcon(QStyle::SP_TrashIcon).pixmap(128, 128);
    }
    else
    {
        m_pixmap = pm;
    }
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
    int w = e->size().width()-4;
    int h = e->size().height()-4;

    qreal r = m_piece!=Empty ? m_pixmap.devicePixelRatio() : 1.0;
    QPixmap p1 = m_pixmap.scaled(r*w, r*h, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if (!p1.isNull())
    {
        setPixmap(p1);
    }
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

int BoardSetupToolButton::heightForWidth(int w) const
{
    return w;
}

bool BoardSetupToolButton::hasHeightForWidth() const
{
    return true;
}
