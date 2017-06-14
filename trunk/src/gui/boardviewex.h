/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef BOARDVIEWEX_H
#define BOARDVIEWEX_H

#include <QWidget>

#include "game.h"

namespace Ui {
class BoardViewEx;
}

class BoardView;

class BoardViewEx : public QWidget
{
    Q_OBJECT
public:
    explicit BoardViewEx(QWidget *parent = 0);
    ~BoardViewEx();

    BoardView* boardView();
    QObject *dbIndex();


    QString getComment() const;
    void setComment(const QString &value);

public slots:
    void slotReconfigure();
    void saveConfig();
    void showTime(bool show);
    void setTime(bool white, QString t);
    void startTime(bool white);
    void configureTime(bool white, bool countDown);
    void stopTimes();
    void setAnnotationPlaceholder(bool);
    void showVariations(QList<MoveId>, QStringList);

protected slots:
    void boardIsFlipped(bool, bool);
private slots:
    void on_editComment_textChanged();
    void variationClicked(QModelIndex index);
signals:
    void signalNewAnnotation(QString);
    void enterVariation(int index);
private:
    Ui::BoardViewEx *ui;
};

#endif // BOARDVIEWEX_H
