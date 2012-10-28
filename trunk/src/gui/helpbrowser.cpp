#include "helpbrowser.h"

HelpBrowser::HelpBrowser(QWidget *parent) :
    QTextBrowser(parent)
{
    setOpenExternalLinks(true);
    setSource(QUrl("qrc:/help/about0.html"));
}
