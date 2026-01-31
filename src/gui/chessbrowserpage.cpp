#include "chessbrowserpage.h"
#include <QDesktopServices>

ChessBrowserPage::ChessBrowserPage(QObject *parent)
    : QWebEnginePage(parent)
{
}

bool ChessBrowserPage::acceptNavigationRequest(
    const QUrl &url,
    NavigationType type,
    bool /* isMainFrame */
    )
{
    QStringList schemes = { "move", "cmt", "precmt", "egtb", "tag", "eco", "event" };
    QStringList files = { "http", "https", "file" };

    if (schemes.contains(url.scheme()))
    {
        emit chessLinkActivated(url);
        return false; // 🚫 stop navigation
    }

    if (files.contains(url.scheme()) && type == QWebEnginePage::NavigationTypeLinkClicked)
    {
        QDesktopServices::openUrl(url);
        return false; // 🚫 stop navigation
    }

    return true; // enable local links
}
