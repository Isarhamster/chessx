#ifndef READAHEADDLG_H
#define READAHEADDLG_H

#include <QDialog>

namespace Ui {
class ReadAheadDlg;
}

class ReadAheadDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ReadAheadDlg(QWidget *parent = nullptr);
    ~ReadAheadDlg();
    static unsigned int getPly(QWidget *parent);

    unsigned int ply() const;
    void setPly(unsigned int ply);

private:
    Ui::ReadAheadDlg *ui;

    unsigned int m_ply;
};

#endif // READAHEADDLG_H
