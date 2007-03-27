/***************************************************************************
                          database.cpp  -  abstract database class
                             -------------------
    begin                : 9 April 2006
    copyright            : (C) 2006 William Hoggarth 
														<whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QFile>
#include <QtDebug>
#include "database.h"

Database::~Database()
{
}

bool Database::loadGameHeaders(int index, Game& game)
{
   QList <QPair< QString, QString> > gameTags;
   gameTags = m_index.allGameTags(index);

   for ( int i = 0; i < gameTags.count(); ++i) {
      game.setTag(gameTags[i].first,gameTags[i].second);
   }

   return true;
}
void Database::saveToPGN(int index, Output& output, QTextStream& out)
{
   Game game;
   loadGame(index,game);
   out << output.output(&game);

}
void Database::exportToPGN(QString filename)
{
   QFile file(filename);
   if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return;

   Output output(Output::Pgn,"/home/data/chessx/chessx/data/templates/pgn-default.template");
   QTextStream out(&file);
   for (int i = 0; i < m_count; i++) {
      saveToPGN(i, output, out);
      out << "\n\n";
   }
   file.close();

}

