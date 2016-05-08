#include "helpbrowsershell.h"
#include "ui_helpbrowsershell.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

HelpBrowserShell::HelpBrowserShell(QWidget *parent) :
    QMainWindow(parent),
    m_bInUpdate(0),
    ui(new Ui::HelpBrowserShell)
{
    ui->setupUi(this);
    connect(Browser(), SIGNAL(linkTargets(QStringList,QStringList)), SLOT(SlotHelpLinkTargets(QStringList,QStringList)));
    connect(Index(), SIGNAL(currentIndexChanged(int)), SLOT(SlotIndexItemSelected(int)));
    Browser()->SetStartPage();
}

HelpBrowserShell::~HelpBrowserShell()
{
    delete ui;
}

QComboBox *HelpBrowserShell::Index()
{
    return ui->comboIndex;
}

HelpBrowser *HelpBrowserShell::Browser()
{
    return ui->textBrowser;
}

void HelpBrowserShell::SlotHelpLinkTargets(const QStringList& texts, const QStringList& links)
{
    if (!m_bInUpdate)
    {
        ++m_bInUpdate;
        QComboBox* cb = Index();
        cb->clear();
        for (int i=0; i<texts.count(); ++i)
        {
            cb->addItem(texts.at(i), links.at(i));
        }
        --m_bInUpdate;
    }
}

void HelpBrowserShell::SlotIndexItemSelected(int index)
{
    if (!m_bInUpdate && index >= 0)
    {
        ++m_bInUpdate;
        Browser()->setSource(QUrl(Index()->currentData().toString()));
        --m_bInUpdate;
    }
}

void HelpBrowserShell::on_btSearchText_clicked()
{
    searchText(ui->searchText->text());
}

void HelpBrowserShell::on_searchText_textChanged(const QString &arg1)
{
    Browser()->moveCursor(QTextCursor::Start);
    searchText(arg1);
}

void HelpBrowserShell::searchText(QString s)
{
    Qt::KeyboardModifiers modifiers = QApplication::keyboardModifiers() & Qt::KeyboardModifierMask;
    bool backward = ((modifiers & Qt::ShiftModifier) == Qt::ShiftModifier);

    QTextDocument::FindFlags ff;
    if (backward) ff |= QTextDocument::FindBackward;
    bool found = Browser()->find(s, ff);
    if (!found)
    {
        Browser()->moveCursor(backward ? QTextCursor::End : QTextCursor::Start);
        Browser()->find(s, ff);
    }
}
