#ifndef HELPBROWSERSHELL_H
#define HELPBROWSERSHELL_H

#include <QMainWindow>

namespace Ui {
class HelpBrowserShell;
}

class HelpBrowser;
class QComboBox;

class HelpBrowserShell : public QMainWindow
{
    Q_OBJECT

public:
    explicit HelpBrowserShell(QWidget *parent = 0);
    ~HelpBrowserShell();

    QComboBox* Index();
    HelpBrowser* Browser();

protected:
    void searchText(QString s);
protected slots:
    void SlotHelpLinkTargets(const QStringList& texts, const QStringList& links);
    void SlotIndexItemSelected(int index);
private slots:
    void on_btSearchText_clicked();

    void on_searchText_textChanged(const QString &arg1);

private:
    int m_bInUpdate;
    Ui::HelpBrowserShell *ui;
};

#endif // HELPBROWSERSHELL_H
