#ifndef BOARDSEARCHDIALOG_H
#define BOARDSEARCHDIALOG_H

#include <QDialog>
#include "board.h"

namespace Ui {
class BoardSearchDialog;
}

class BoardSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BoardSearchDialog(QWidget *parent = 0);
    ~BoardSearchDialog();

    int mode() const;
    void setBoard(const Board &b);
protected slots:
    void reject();
    void accept();
    void restoreLayout();
private:
    Ui::BoardSearchDialog *ui;
};

#endif // BOARDSEARCHDIALOG_H
