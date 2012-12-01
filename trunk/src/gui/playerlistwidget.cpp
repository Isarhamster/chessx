#include "playerlistwidget.h"
#include "ui_playerlistwidget.h"
#include "database.h"

PlayerListWidget::PlayerListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerListWidget)
{
    ui->setupUi(this);
    m_filterModel = new QStringListModel(this);
    ui->playersView->setModel(m_filterModel);

    setObjectName("PlayerListWidget");
    connect(ui->filterEdit, SIGNAL(textChanged(const QString&)), SLOT(findPlayers(const QString&)));
    connect(ui->playersView, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(showSelectedPlayer()));
    slotReconfigure();
}

PlayerListWidget::~PlayerListWidget()
{
    delete ui;
}

void PlayerListWidget::slotReconfigure()
{
}

void PlayerListWidget::findPlayers(const QString& s)
{
    if (s.isEmpty())
    {
        m_filterModel->setStringList(m_list);
    }
    else
    {
        QStringList newList = m_list.filter(s, Qt::CaseInsensitive);
        m_filterModel->setStringList(newList);
    }
}

void PlayerListWidget::selectPlayer(const QString& player)
{
    if (!player.isEmpty())
    {
        m_player.setName(player);
        m_player.update();
        ui->playerView->setText(QString("<h1>%1</h1><p>%2%3%4%5")
                .arg(m_player.name()).arg(m_player.formattedGameCount())
                .arg(m_player.formattedRange())
                .arg(m_player.formattedRating()).arg(m_player.formattedScore()));
    }
    else
    {
        ui->playerView->setText(tr("<html><i>No player chosen.</i></html>"));
    }
}

void PlayerListWidget::showSelectedPlayer()
{
    if (ui->playersView->currentIndex().isValid())
    {
        QString ts = ui->playersView->currentIndex().data().toString();
        selectPlayer(ts);
    }
}

void PlayerListWidget::setDatabase(Database* db)
{
    ui->playerView->setText(tr("<html><i>No player chosen.</i></html>"));
    m_player.setDatabase(db);
    m_list = QStringList::fromSet(db->index()->playerNames());
    m_filterModel->setStringList(m_list);
    m_filterModel->sort(0);

}
