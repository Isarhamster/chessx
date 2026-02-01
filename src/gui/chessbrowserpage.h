#ifndef CHESSBROWSERPAGE_H
#define CHESSBROWSERPAGE_H

#include <QWebEnginePage>

class ChessBrowserPage : public QWebEnginePage
{
    Q_OBJECT
public:
    explicit ChessBrowserPage(QObject *parent = nullptr);

signals:

protected:

};

#endif
