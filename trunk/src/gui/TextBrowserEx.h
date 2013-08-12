/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TEXTBROWSEREX_H
#define TEXTBROWSEREX_H

#include <QTextBrowser>
#include <QtCore>

class TextBrowserEx : public QTextBrowser
{
    Q_OBJECT

public:
    explicit TextBrowserEx(QWidget* parent = 0) : QTextBrowser(parent) {}

    void setSource(const QUrl& name) { if (name.scheme().isEmpty()) QTextBrowser::setSource(name); }
};

#endif // TEXTBROWSEREX_H
