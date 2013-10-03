/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <QTextBrowser>

class HelpBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit HelpBrowser(QWidget *parent = 0);

signals:

public slots:

};

#endif // HELPBROWSER_H
