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

#include <qlineedit.h>
#include <q3listview.h>
#include <q3textbrowser.h>

#include "playerdialog.h"
#include "playerdatabase.h"
#include "settings.h"

PlayerDialog::PlayerDialog(PlayerDatabase* db, QWidget* parent) : QDialog(parent)
{
  ui.setupUi(this);

  setName("PlayerDialog");
  m_database = db;
  connect(ui.playerEdit, SIGNAL(textChanged (const QString&)), SLOT(findPlayers(const QString&)));
  connect(ui.playerList, SIGNAL(currentChanged(Q3ListViewItem*)), SLOT(showPlayer(Q3ListViewItem*)));
  if (parent)
    connect(parent, SIGNAL(reconfigure()), SLOT(configure()));
  configure();

  for (int i = 1; i < 4; i++)
    ui.playerList->adjustColumn(i);
  ui.playerView->setMinimumWidth(width() / 2);
  findPlayers(QString());
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

void PlayerDialog::findPlayers(const QString& s)
{
  // Capitalize first letter
  QString name = s;
  if (!name.isEmpty())
     name[0] = name[0].upper();
  ui.playerEdit->setText(s);
  QStringList players = m_database->findPlayers(name, m_showLimit);
  ui.playerList->clear();
  for (QStringList::ConstIterator it = players.begin(); it != players.end(); ++it)
  {
    m_database->setCurrent(*it);
    int birth = m_database->dateOfBirth().year();
    int death = m_database->dateOfDeath().year();
    new Q3ListViewItem(ui.playerList, *it, birth ? QString::number(birth) : "",
       death ? QString::number(death) : "", m_database->title(),  m_database->country());
  }
  ui.playerList->setCurrentItem(ui.playerList->firstChild());
  showPlayer(ui.playerList->firstChild());
  show();
}

void PlayerDialog::showPlayer(Q3ListViewItem* i)
{
  if (i)
    showPlayer(i->text(0));
}


void PlayerDialog::showPlayer(const QString& s)
{
  m_database->setCurrent(s);
  QString birth = m_database->dateOfBirth().asShortString();
  QString death = m_database->dateOfDeath().asShortString();
  QString live = death.isEmpty() ? tr("Born %1.").arg(birth) :
    tr("Born: %1, died %2.").arg(birth).arg(death);
  QString country = m_database->country();
  QString image;
  if (m_database->hasPhoto())
  {
    ui.playerView->mimeSourceFactory()->setImage("image.png", m_database->photo());
    image = "<img align=\"right\" src=\"image.png\">";
  }
  QString bio;
  if (m_database->hasBiography())
    bio = tr("<h2>Biography</h2>%1\n").arg(m_database->biography());
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

  // Final text
  ui.playerView->setText(tr("<h1>%1</h1>%2%3<br>Country: %4<br>Title: %5\n%6%7")
    .arg(s).arg(image).arg(live).arg(country).arg(title).arg(bio).arg(rating));
}

