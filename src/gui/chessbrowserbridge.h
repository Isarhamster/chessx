#ifndef CHESSBROWSERBRIDGE_H
#define CHESSBROWSERBRIDGE_H

#include <QObject>

class ChessBrowserBridge : public QObject
{
    Q_OBJECT
public:
    explicit ChessBrowserBridge(QObject *parent = nullptr);

public slots:
    void moveClicked(int id) {
        emit moveSelected(id);
    }
signals:
    void moveSelected(int id);
};

#endif // CHESSBROWSERBRIDGE_H
