#include "helpbrowser.h"
#include <QUrl>

HelpBrowser::HelpBrowser(QWidget *parent) :
    QTextBrowser(parent)
{
    setOpenExternalLinks(true);
    loadResource ( QTextDocument::StyleSheetResource, QUrl(":/help/about.css"));
    setSource(QUrl("qrc:/help/about0.html"));
}
