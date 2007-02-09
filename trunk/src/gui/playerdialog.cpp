/***************************************************************************
                          PlayerDialog - browser of player's database
                             -------------------
    begin                : sob maj 7 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QComboBox>
#include <QTextBrowser>

#include "playerdialog.h"
#include "playerdatabase.h"
#include "settings.h"

PlayerDialog::PlayerDialog(PlayerDatabase* db, QWidget* parent) : QDialog(parent)
{
  ui.setupUi(this);

  setName("PlayerDialog");
  m_database = db;
  connect(ui.findButton, SIGNAL(clicked()), SLOT(showPlayer()));
  connect(ui.playerCombo, SIGNAL(activated(const QString&)), SLOT(showPlayer(const QString&)));
  if (parent)
    connect(parent, SIGNAL(reconfigure()), SLOT(configure()));
  configure();
}

PlayerDialog::~PlayerDialog()
{
}

void PlayerDialog::configure()
{
  AppSettings->beginGroup("/Players/");
  m_showRating = AppSettings->readBoolEntry("rating", true);
  m_showLimit = AppSettings->readNumEntry("count", 100);
  AppSettings->endGroup();
}

void PlayerDialog::showPlayer(const QString& s)
{
  ui.playerCombo->setCurrentText(s);
  // Capitalize first letter
  QString name = s;
  if (!name.isEmpty())
     name[0] = name[0].upper();
  QStringList players = m_database->findPlayers(name, m_showLimit);
  ui.playerCombo->clear();
  if (players.count())
  {
    QString player;
    foreach(player, players)
      ui.playerCombo->insertItem(player);
    ui.playerView->setText(formatPlayer(ui.playerCombo->text(0)));
  }
  else
    ui.playerView->setText(tr("<html><i>No player found</i></html>"));
  show();
}

void PlayerDialog::showPlayer()
{
  showPlayer(ui.playerCombo->currentText());
}

QString PlayerDialog::formatPlayer(const QString& player)
{
  m_database->setCurrent(player);
  QString birth = m_database->dateOfBirth().asShortString();
  QString death = m_database->dateOfDeath().asShortString();
  QString live = death.isEmpty() ? tr("Born %1.").arg(birth) :
    tr("Born: %1, died %2.").arg(birth).arg(death);
  QString country = m_database->country();
  QString image;
  if (m_database->hasPhoto())
  {
    ui.playerView->document()->addResource(QTextDocument::ImageResource, QUrl("image.png"),
                            m_database->photo());
    image = "<br><img src=\"image.png\"><br>";
  }
  QString bio;
  if (m_database->hasBiography())
    bio = tr("<h3>Biography</h3>%1\n").arg(m_database->biography());
  QString title = m_database->title();

  // Rating
  QString rating;
  int elo = m_database->highestElo();
  if (elo) 
      rating = tr("Highest rating: %1<br>").arg(elo);
  int eloindex = m_database->lastEloListIndex();
  elo = m_database->elo(eloindex);
  if (elo)
    rating.append(tr("Last rating: %1 (%2)").arg(elo)
         .arg(m_database->eloListToDate(eloindex).asShortString()));
  if (m_showRating)
  {
    rating.append(tr("<br>Rating history:<br><blockquote>"));
    int start = m_database->firstEloListIndex();
    int end = m_database->lastEloListIndex();
    for (int i = start; i<=end; i++)
    {
      int elo = m_database->elo(i);
      if (!elo)
        continue;
      if (i != start)
        rating.append(", ");
      rating.append(QString("%1:&nbsp;%2").arg(m_database->eloListToDate(i).asShortString()).arg(elo));
    }
    rating.append("</blockquote>\n");
  }
  if (!rating.isEmpty())
    rating.prepend(tr("<h2>Rating</h2>"));

  return tr("<h2>%1</h2>%2%3<br>Country: %4<br>Title: %5\n%6%7")
    .arg(player).arg(image).arg(live).arg(country).arg(title).arg(bio).arg(rating);
}

