#ifndef STUDYSELECTIONDIALOG_H
#define STUDYSELECTIONDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class StudySelectionDialog;
}

class StudySelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StudySelectionDialog(QWidget *parent = nullptr);
    ~StudySelectionDialog();
    void run();

    QList<QPair<QString, QString>> getStudies() const;

protected:
    virtual void closeEvent(QCloseEvent *);
    virtual void accept();

private slots:
    void on_studies_itemDoubleClicked(QTableWidgetItem *item);

private:
    void fill();
    QList<QPair<QString, QString>> studies;
    Ui::StudySelectionDialog *ui;
};

#endif // STUDYSELECTIONDIALOG_H
