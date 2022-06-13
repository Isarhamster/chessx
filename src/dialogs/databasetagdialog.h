#ifndef DATABASETAGDIALOG_H
#define DATABASETAGDIALOG_H

#include <QDialog>

namespace Ui {
class DatabaseTagDialog;
}

class DatabaseTagDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseTagDialog(QWidget *parent = nullptr);
    ~DatabaseTagDialog();
    void setTagNames(QStringList tags);
    QString currentTag() const;
    QString current() const;
    QString replacement() const;
private:
    Ui::DatabaseTagDialog *ui;
};

#endif // DATABASETAGDIALOG_H
