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
    explicit AboutDlg(QWidget *parent = nullptr);
    ~AboutDlg();

protected slots:
    void restoreLayout();
    void accept();
    void reject();

private slots:
    void on_labelTempPath_linkActivated(const QString &link);

    void on_labelDataPath_linkActivated(const QString &link);

    void on_labelSettingsPath_linkActivated(const QString &link);

    void on_labelDocPath_linkActivated(const QString &link);

private:
    Ui::AboutDlg *ui;
};

#endif // ABOUTDLG_H
