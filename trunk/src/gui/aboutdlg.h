/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>

namespace Ui
{
class AboutDlg;
}

class AboutDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDlg(QWidget *parent = 0);
    ~AboutDlg();

protected slots:
    void restoreLayout();
    void accept();
    void reject();

private:
    Ui::AboutDlg *ui;
};

#endif // ABOUTDLG_H
