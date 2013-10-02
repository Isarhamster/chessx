#include "aboutdlg.h"
#include "settings.h"
#include "version.h"

#include "ui_aboutdlg.h"

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);
    QString refPath = AppSettings->dataPath();
    ui->labelDataPath->setText(refPath);
    refPath = AppSettings->getTempPath();
    ui->labelTempPath->setText(refPath);
    ui->labelCopyRightDate->setText(COPYRIGHT_DATE);
    QString version = QString(STR_REVISION).replace(',','.');
    ui->labelVersion->setText(version);
}

AboutDlg::~AboutDlg()
{
    delete ui;
}
