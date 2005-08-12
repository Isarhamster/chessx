/***************************************************************************
                          common  -  description
                             -------------------
    begin                : 28/05/2005
    copyright            : (C) 2005 Ejner Borgbjerg 
                           <ejner@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <iostream>
#include <qstringlist.h>
#include <qtextstream.h>
#include "databaseconversion.h"

bool DatabaseConversion::playerDatabaseFromScidRatings(const QString& inFileName, const QString& outFileName, const QString& pictureDir){
// was tested and works with ratings.ssp from january 2004
// and the latest from http://members.aon.at/schachverein.steyr/ratings.ssp.zip
  PlayerDatabase pdb;
  QDir dir = QDir(pictureDir);
  QStringList pictures = dir.entryList();

  if (pdb.removeDatabase(outFileName)){
    std::cout << "removed " << outFileName.latin1() << "\n";
  }
  else{
    std::cout << "failure removing " << outFileName.latin1() << "\n";
  }
  if (pdb.create(outFileName)){
    std::cout << "created " << outFileName.latin1() << "\n";
  }
  else{
    std::cout << "failure creating " << outFileName.latin1() << "\n";
    return false;
  }

// now read from ratings.ssp
  QFile inFile(inFileName);
  if ( !(inFile.open( IO_ReadOnly ) )) {
    std::cout << "failure opening output file \n";
    return false;
  }

  QTextStream stream( &inFile );
  QString line;
  QString linesStripped;
  while ( !line.startsWith("@PLAYER") && !stream.atEnd() ) {
      line = stream.readLine();
  }

  QString name;
  int numberOfPlayers = 0;
  int numberOfPictures = 0;

  while ( !line.startsWith("### END OF PLAYER SECTION") && !stream.atEnd() ) {
      line = stream.readLine();
      linesStripped = line.stripWhiteSpace();
        //std::cout << line << "\n";
      if (linesStripped.startsWith("#")){//comment line
      }
      else if(linesStripped.length() == 0) {//empty line
      }
      else if(linesStripped.startsWith("=") ) {//name correction line
      }
      else if(linesStripped.startsWith("%Bio")) {//Biography note
        pdb.appendToBiography(linesStripped.mid(5,9999)+"<br>");
      }
      else if(linesStripped.startsWith("%Title")) {//title award note - add to biography
        pdb.appendToBiography(linesStripped.mid(7,9999)+"<br>");
      }
      else if(linesStripped.startsWith("%Render")) {//name with special characters fe. Huebner
      }
      else if(linesStripped.startsWith("%Elo")) {//elo data
        QStringList sl = QStringList::split(" ",linesStripped);
	QStringList::Iterator it = sl.begin();
	it++;
        for (; it != sl.end(); it++ ) {
          QString s = *it;
          //std::cout << s << ":";
          QStringList sl2 = QStringList::split(":",s);
          QString year = sl2[0];
          QString ratings = sl2[1];//ratings for one year, separated by commas
          QStringList sl3 = QStringList::split(",",ratings);
          for (uint i=0;i < (uint)sl3.size();i++ ) {
            if(sl3[i] != "?"){
//              std::cout << "rating for " << name << ": " << year << ": " << i+1 << ": " << sl3[i] << "\n";
              pdb.setElo(year.toInt(),i+1,sl3[i].toInt());
            }
          }
        }
      }
      else if(linesStripped.startsWith("%")) {//unknown code
        //std::cout << "unknown code in ratings.ssp \n";
        //std::cout << line << "\n";
//ignore!        return false;
      }
      else {//name line

        if (numberOfPlayers%1000==0 && numberOfPlayers>0){
            std::cout << "processed: " << numberOfPlayers << "\n";
            pdb.commit();//seems binary content changed when doing this ?!
        }

        QStringList sl0 = QStringList::split("#",line);
        name = sl0[0].stripWhiteSpace();
        int index = sl0[1].find(QChar(' '));
        QString title = sl0[1].left(index);
        QString rest = sl0[1].mid(index,9999);

//        std::cout << "name= " << name << "\n";
//        std::cout << "title= " << title << "\n";
//        std::cout << "rest= " << rest << "\n";

        if (!pdb.add(name))
           std::cout << "failed adding player: " << name.latin1() << "\n";

//look for players picture
        for ( QStringList::Iterator it = pictures.begin(); it != pictures.end(); ++it ) {
          QStringList sl4 = QStringList::split(".",*it);
          if (sl4[0].compare(name)==0){
//          if ((*it).startsWith(name)){
            QImage* img = new QImage(dir.path()+"/"+(*it));
            pdb.setPhoto(*img);
            numberOfPictures++;
            pictures.remove(it);
            break;
          }
        }

        numberOfPlayers++;
        pdb.setTitle(title);

        QStringList sl = QStringList::split(" ",rest);
        for ( QStringList::Iterator it = sl.begin(); it != sl.end(); ++it ) {
          QString s = *it;
          //std::cout << s << ":";
          if (s.startsWith("[")){//rating, peak or estimated
            s = s.mid(1,s.length()-2);
            //std::cout << "rating= " << s;
            if (s.endsWith("*"))
              pdb.setEstimatedElo(s.left(s.length()-1).toInt());
            else
              pdb.setPeakElo(s.toInt());
          }
          else if(s.contains(".")){//birth date and/or death date
            if (s.startsWith("--")){//only death date(!) Baay is an example
              pdb.setDateOfDeath(s.mid(2,10));
            }
            else{
              pdb.setDateOfBirth(s.left(10));
              if (s.length()>=22){
                pdb.setDateOfDeath(s.mid(12,10));
              }
            }
          }
          else {//country code(s)
            pdb.setCountry(s);
          }
        }
//        std::cout << "\n";
      }
  }
  
  inFile.close();

  std::cout << numberOfPlayers << " players read in from ratings.ssp\n"; 
  std::cout << numberOfPictures << " players had a picture\n"; 
  for ( QStringList::Iterator it = pictures.begin(); it != pictures.end(); ++it ) {
     std::cout << "player picture that was not converted: " << *it << "\n";
  }

  pdb.commit();
  pdb.close();

  return true;

}
