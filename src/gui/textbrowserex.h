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
    explicit TextBrowserEx(QWidget* parent = nullptr) : QTextBrowser(parent) {}

    virtual void doSetSource(const QUrl &name, QTextDocument::ResourceType type = QTextDocument::UnknownResource)
    {
        if(name.scheme().isEmpty())
        {
#if QT_VERSION >= 0x060000
            QTextBrowser::doSetSource(name, type);
#endif
        }
    }
    void setSource(const QUrl& name)
    {
        if(name.scheme().isEmpty())
        {
            QTextBrowser::setSource(name);
        }
    }
};

#endif // TEXTBROWSEREX_H
