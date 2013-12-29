/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ACTIONSDIALOG_H
#define ACTIONSDIALOG_H

#include <QDialog>
#include <QList>

class QAction;
class QTableWidget;
class QTableItem;
class QWidget;

namespace Ui
{
class ActionDialog;
}

class ActionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ActionDialog(QWidget *parent);
    ~ActionDialog();

signals:
    void signalResetKey();

protected slots:
    void accept();

private slots:
    void saveOldActionText(int row, int column);
    void validateAction(int row, int column);
    void resetKeys();

protected:
    QString normalizedText(const QAction *action) const;
    void resetList();

private:
    QString oldAccelText;

private:
    Ui::ActionDialog *ui;
};

#endif
