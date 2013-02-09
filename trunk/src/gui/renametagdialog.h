#ifndef RENAMETAGDIALOG_H
#define RENAMETAGDIALOG_H

#include <QDialog>

namespace Ui {
class RenameTagDialog;
}

class RenameTagDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RenameTagDialog(QWidget *parent, QString ts, QString tagName);
    ~RenameTagDialog();
    
signals:
    void renameRequest(QString tag, QString newName, QString oldName);

protected slots:
    /** Start renaming a tag */
    void slotRename();

private:
    Ui::RenameTagDialog *ui;
    QString m_ts;
    QString m_tagName;
};

#endif // RENAMETAGDIALOG_H
