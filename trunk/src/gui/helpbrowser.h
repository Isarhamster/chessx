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
    explicit HelpBrowser(QWidget *parent = nullptr);

    void SetStartPage();
signals:
    void linkTargets(const QStringList& texts, const QStringList& links);

public slots:

private slots:
    void slotSourceChanged(const QUrl &url);

protected:
    virtual QVariant loadResource(int type, const QUrl &name) override;
};

#endif // HELPBROWSER_H
