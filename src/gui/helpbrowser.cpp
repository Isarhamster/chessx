/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "helpbrowser.h"
#include <QUrl>
#include <QFile>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

HelpBrowser::HelpBrowser(QWidget *parent) :
    QTextBrowser(parent)
{
    setOpenLinks(true);
    setOpenExternalLinks(true);
    loadResource(QTextDocument::StyleSheetResource, QUrl(":/help/about.css"));
    connect(this, SIGNAL(sourceChanged(QUrl)), SLOT(slotSourceChanged(QUrl)));
}

void HelpBrowser::SetStartPage()
{
    setSource(QUrl("qrc:/help/about0.html"));
}

void HelpBrowser::slotSourceChanged(const QUrl& url)
{
    if (url.toString().contains("#"))
        return;

    QString path = url.path().prepend(":");
    QFile f(path);
    QStringList texts;
    QStringList links;
    if( f.open( QIODevice::ReadOnly ) )
    {
        QString str;
        while( !f.atEnd() )
        {
            str = f.readLine();
            if (str.contains("<h3>"))
            {
                QRegExp regCmd("<a name=\"([^\"]*)\"></a>([^<]*)</h3>");
                if (regCmd.indexIn(str) >= 0)
                {
                    links.append(regCmd.cap(1).prepend(path+"/#"));
                    texts.append(regCmd.cap(2));
                }
            }
        }

        f.close();
    }
    emit linkTargets(texts, links);
}

