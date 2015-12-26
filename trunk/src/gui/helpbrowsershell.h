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

protected slots:
    void SlotHelpLinkTargets(const QStringList& texts, const QStringList& links);
    void SlotIndexItemSelected(int index);
private:
    int m_bInUpdate;
    Ui::HelpBrowserShell *ui;
};

#endif // HELPBROWSERSHELL_H
