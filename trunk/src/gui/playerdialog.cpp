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
#include <qlistview.h>
#include <qtextbrowser.h>

#include "playerdialog.h"
#include "playerdatabase.h"


PlayerDialog::PlayerDialog(PlayerDatabase* db, QWidget* parent) : PlayerDialogBase(parent)
{
  m_database = db;
  connect(playerEdit, SIGNAL(textChanged (const QString&)), SLOT(findPlayers(const QString&)));
  connect(playerList, SIGNAL(currentChanged(QListViewItem*)), SLOT(showPlayer(QListViewItem*)));
  playerList->setColumnWidth(0, playerList->width() * 8 / 10);
  findPlayers("");
}

PlayerDialog::~PlayerDialog()
{
}

void PlayerDialog::findPlayers(const QString& s)
{
  QStringList players = m_database->findPlayers(s, 100);
  playerList->clear();
  for (QStringList::ConstIterator it = players.begin(); it != players.end(); ++it)
  {
    m_database->setCurrent(*it);
    int birth = m_database->dateOfBirth().year();
    int death = m_database->dateOfDeath().year();
    new QListViewItem(playerList, *it, birth ? QString::number(birth) : "",
       death ? QString::number(death) : "", QString::number(m_database->estimatedElo()));
  }
}

void PlayerDialog::showPlayer(QListViewItem* i)
{
  showPlayer(i->text(0));
}


void PlayerDialog::showPlayer(const QString& s)
{
  if (!m_database->setCurrent(s))
  {
    playerView->setText(QString("<h1>%1</h1>\n").arg(s) + 
      "<i>No information about player found.</i>");
    return;
  }
  QString birth = m_database->dateOfBirth().asShortString();
  QString death = m_database->dateOfDeath().asShortString();
  QString live = death.isEmpty() ? QString("Born %1.").arg(birth) :
    QString("Born: %1, died %2.").arg(birth).arg(death);
  QString bio = m_database->biography();
  QString country = m_database->country();
  QString image;
  QImage photo = m_database->photo();
  if (!photo.isNull())
  {
    playerView->mimeSourceFactory()->setImage("image.png", photo);
    image = "<img hspace=\"10\" align=\"right\" src=\"image.png\">";
  }
  if (!bio.isEmpty())
    bio = QString("<h2>Biography</h2>%1\n").arg(bio);
  QString title = m_database->title();
  playerView->setText(QString("<h1>%1</h1>%2%3<br>Country: %4<br>Title: %5\n%6")
    .arg(s).arg(image).arg(live).arg(country).arg(title).arg(bio));

}
