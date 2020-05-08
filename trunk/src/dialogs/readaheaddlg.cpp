#include "readaheaddlg.h"
#include "ui_readaheaddlg.h"
#include "settings.h"

ReadAheadDlg::ReadAheadDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadAheadDlg)
{
    ui->setupUi(this);
}

ReadAheadDlg::~ReadAheadDlg()
{
    delete ui;
}

unsigned int ReadAheadDlg::ply() const
{
    return m_ply;
}

void ReadAheadDlg::setPly(unsigned int ply)
{
    m_ply = ply;
    ui->ply->setValue(ply);
}

static unsigned int getPly(QWidget *parent)
{
    unsigned int readAheadPly = AppSettings->getValue("/Board/ReadAhead").toInt();
    ReadAheadDlg dlg(parent);
    dlg.setPly(readAheadPly);
    if (dlg.exec() == QDialog::Accepted)
    {
        readAheadPly = dlg.ply();
        AppSettings->setValue("/Board/ReadAhead", readAheadPly);
        return readAheadPly; // 0 or a positive number from 0..99
    }
    return 0; // Either cancel or 0
}
