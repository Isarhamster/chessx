/****************************************************************************
*   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "helpbrowser.h"
#include "settings.h"
#include <QUrl>
#include <QFile>
#include <QRegularExpression>

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

HelpBrowser::HelpBrowser(QWidget *parent) :
    QTextBrowser(parent)
{
    setOpenLinks(true);
    setOpenExternalLinks(true);
    connect(this, SIGNAL(sourceChanged(QUrl)), SLOT(slotSourceChanged(QUrl)));
}

QVariant HelpBrowser::loadResource(int type, const QUrl &name)
{
    QString path = name.toString();
    if (AppSettings->getValue("/MainWindow/DarkTheme").toBool())
    {
        path.replace("about.css","about-dark.css");
    }
    return QTextBrowser::loadResource(type,QUrl(path));
}

void HelpBrowser::SetStartPage()
{
  QString lang = AppSettings->getValue("/General/language").toString();
  setSource(QUrl("qrc:/help/about0.html"));
  //hardcodes Spanish help Pages if language is set to Spanish
  if (lang=="es"){
    setSource(QUrl("qrc:/help/ES/about0.html"));
  }
  reload(); // Workaround bug in Qt
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
                QRegularExpression regCmd("<a name=\"([^\"]*)\"></a>([^<]*)</h3>");
                QRegularExpressionMatch match;
                if (str.indexOf(regCmd, 0, &match) >= 0)
                {
                    links.append(match.captured(1).prepend(path+"/#"));
                    texts.append(match.captured(2));
                }
            }
        }

        f.close();
    }
    emit linkTargets(texts, links);
}

