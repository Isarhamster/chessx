/***************************************************************************
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "bitboard.h"
#include "chessxsettings.h"

#include <QCoreApplication>
#include <QtCore>
#include <QColor>
#include <QDialog>
#include <QDockWidget>
#include <QtGui>
#include <QWidget>
#include <QMainWindow>
#include <QSplitter>
#include <QLayout>

using namespace chessx;

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

ChessXSettings::ChessXSettings() : Settings()
{

}

ChessXSettings::ChessXSettings(const QString &fileName) : Settings(fileName)
{

}


bool ChessXSettings::layout(QWidget* w)
{
    if(!w || w->objectName().isEmpty())
    {
        return false;
    }
    if (w->objectName() != "MainWindow")
    {
        beginGroup(w->objectName());
    }
    beginGroup("Geometry");
    QList<int> values;
    bool valid = list(w->objectName(), values, 5);
    if(valid)    // Enough values
    {
        int x = values[0];
        int y = values[1];
        int wx;
        if (values.count()==5)
        {
            wx = x;
            x &= ~0xC0000000;
        }
        else
        {
            wx = values[5];
        }
        w->resize(values[2], values[3]);
        QDialog* dlg = qobject_cast<QDialog*>(w);
        if ((w->objectName() == "MainWindow") || dlg)
        {
            int wy = values[3];
            // Check that x/y are inside the available geometry
            QList<QScreen*> screen_list = qApp->screens();
            bool found = false;
            foreach(const QScreen* screen, screen_list)
            {
                QRect rect = screen->geometry();
                if (rect.contains(QPoint(x,y)))
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                const QScreen* screen = qApp->primaryScreen();
                QRect rect = screen->geometry();
                if (rect.x()+rect.width() < x)
                {
                    if (wx > rect.width())
                        x = rect.x();
                    else
                        x = rect.x() + (rect.width()-wx)/2;
                }
                if (rect.y()+rect.height() < y)
                {
                    if (wy > rect.height())
                        y = rect.y();
                    else
                        y = rect.y() + (rect.height()-wy)/2;
                }
            }
        }
        w->move(x, y);

        if (wx & 0x80000000)      w->setWindowState(Qt::WindowMaximized);
        else if (wx & 0x40000000) w->setWindowState(Qt::WindowFullScreen);

        QSplitter* s = qobject_cast<QSplitter*>(w);
        if (s)
        {
            QByteArray splits = value("Splitter", QByteArray()).toByteArray();
            if (splits.size())
            {
                s->restoreState(splits);
            }
        }
        else
        {
            QMainWindow* m = qobject_cast<QMainWindow*>(w);
            if(m)
            {
                QByteArray docks = value("Docks", QByteArray()).toByteArray();
                if(docks.count())
                {
                    m->restoreState(docks, 0);
                }

                QStringList floatingDocks = value("FloatingDocks").toStringList();
                foreach(QString name, floatingDocks)
                {
                    QDockWidget* dw = m->findChild<QDockWidget*>(name);
                    if (dw)
                    {
                        dw->setFloating(true);
                        QRect rect = value(QString("FloatingDock/%1/geometry").arg(name)).toRect();
                        dw->setGeometry(rect);
                        dw->setVisible(value(QString("FloatingDock/%1/visible").arg(name)).toBool());
                    }
                }
            }
            else
            {
                if(values[4])
                {
                    QDialog* d = qobject_cast<QDialog*>(w);
                    if (d) d->setModal(true);
                    w->show();
                }
            }
        }
    }
    endGroup();
    if (w->objectName() != "MainWindow")
    {
        endGroup();
    }
    return valid;
}

void ChessXSettings::setLayout(const QWidget* w)
{
    if(!w || w->objectName().isEmpty())
    {
        return;
    }
    if (w->objectName() != "MainWindow")
    {
        beginGroup(w->objectName());
    }
    beginGroup("Geometry");
    QList<int> values;
    int wx = 0;
    int x = w->x();
    int y = w->y();
    int wn = w->width();
    int hn = w->height();
    if (w->isFullScreen())     wx |= 0x40000000;
    else if (w->isMaximized()) wx |= 0x80000000;

    if (wx)
    {
        QRect ng = w->normalGeometry();
        QRect frame = w->frameGeometry();
        ng.adjust(-frame.x(), -frame.y(), -frame.x(), -frame.y());
        x = ng.x();
        y = ng.y();
        wn = ng.width();
        hn = ng.height();
    }
    values << x << y << wn << hn << w->isVisible() << wx;
    setList(w->objectName(), values);
    const QMainWindow* m = qobject_cast<const QMainWindow*>(w);
    if (m)
    {
        QByteArray docks = m->saveState(0);
        setValue("Docks", docks);

        QStringList floatingDocks;
        QList<QDockWidget*> fdocks = m->findChildren<QDockWidget*>();
        for(int i = 0; i < fdocks.size(); i++)
        {
            QDockWidget* dw = fdocks.at(i);
            if (dw->isFloating())
            {
                QString name = dw->objectName();
                floatingDocks << name;
                QRect rect = dw->geometry();
                setValue(QString("FloatingDock/%1/geometry").arg(name), rect);
                setValue(QString("FloatingDock/%1/visible").arg(name), !dw->isHidden());
            }
        }

        setValue("FloatingDocks", floatingDocks);
    }
    const QSplitter* s = qobject_cast<const QSplitter*>(w);
    if (s)
    {
        setValue("Splitter", s->saveState());
    }

    endGroup();
    if (w->objectName() != "MainWindow")
    {
        endGroup();
    }
}

void ChessXSettings::initWidgetValues(QMap<QString, QVariant>& map) const
{
    map.insert("/Board/lightColor", QColor(Qt::lightGray));
    map.insert("/Board/darkColor", QColor(Qt::darkGray));
    map.insert("/Board/highlightColor", QColor(Qt::yellow));
    map.insert("/Board/frameColor", QColor(Qt::black));
    map.insert("/Board/currentMoveColor", QColor(Qt::blue));
    map.insert("/Board/storedMoveColor", QColor(Qt::magenta));
    map.insert("/Board/variationMoveColor", QColor(Qt::darkMagenta));
    map.insert("/Board/threatColor", QColor(0xe0,0xe0,0xff,0x80));
    map.insert("/Board/targetColor", QColor(Qt::darkGreen));
    map.insert("/Board/checkColor", QColor(Qt::magenta));
    map.insert("/Board/wallColor", QColor(Qt::darkGray));
    map.insert("/Board/underprotectedColor", QColor(Qt::red));
    map.insert("/Board/engineColor", QColor("orange"));
}

//////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////

