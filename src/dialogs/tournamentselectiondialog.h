#ifndef TOURNAMENTSELECTIONDIALOG_H
#define TOURNAMENTSELECTIONDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class TournamentSelectionDialog;
}

class TournamentSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TournamentSelectionDialog(QWidget *parent = nullptr);
    void run();
    ~TournamentSelectionDialog();
    QStringList getTournaments() const;

protected:
    virtual void closeEvent(QCloseEvent *);
    virtual void accept();

private slots:
    void on_tournements_itemDoubleClicked(QTableWidgetItem *item);

private:
    void fill();
    QStringList tournaments;

private:
    Ui::TournamentSelectionDialog *ui;
};

#endif // TOURNAMENTSELECTIONDIALOG_H
