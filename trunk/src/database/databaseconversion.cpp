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
#include <QStringList>
#include <QTextStream>
#include "databaseconversion.h"

bool DatabaseConversion::playerDatabaseFromScidRatings(const QString& inFileName, const QString& outFileName, const QString& pictureDir)
{
// was tested and works with ratings.ssp from january 2004
// and the latest from http://members.aon.at/schachverein.steyr/ratings.ssp.zip
	PlayerDatabase pdb;
	QDir dir = QDir(pictureDir);
	QStringList pictures = dir.entryList();

	if (pdb.removeDatabase(outFileName)) {
		std::cout << "removed " << (char*)outFileName.toLatin1().constData() << "\n";
	} else {
		std::cout << "failure removing " << outFileName.toLatin1().constData() << "\n";
	}
	if (pdb.create(outFileName)) {
		std::cout << "created " << outFileName.toLatin1().constData() << "\n";
	} else {
		std::cout << "failure creating " << outFileName.toLatin1().constData() << "\n";
		return false;
	}

// now read from ratings.ssp
	QFile inFile(inFileName);
	if (!(inFile.open(QIODevice::ReadOnly))) {
		std::cout << "failure opening output file \n";
		return false;
	}

	QTextStream stream(&inFile);
	QString line;
	QString lineStripped;
	while (!line.startsWith("@PLAYER") && !stream.atEnd()) {
		line = stream.readLine();
	}

	QString name;
	int numberOfPlayers = 0;
	int numberOfPictures = 0;

	while (!line.startsWith("### END OF PLAYER SECTION") && !stream.atEnd()) {
		line = stream.readLine();
		lineStripped = line.trimmed();
		//std::cout << line << "\n";
		if (lineStripped.startsWith("#")) {//comment line
		} else if (lineStripped.length() == 0) {//empty line
		}
//      else if(lineStripped.startsWith("=") ) {//name correction line
		//    }
		else if (lineStripped.startsWith("%Bio")) {//Biography note
			pdb.appendToBiography(lineStripped.mid(5, 9999) + "<br>");
		} else if (lineStripped.startsWith("%Title")) {//title award note - add to biography
			pdb.appendToBiography(lineStripped.mid(7, 9999) + "<br>");
		} else if (lineStripped.startsWith("%Render")) {//name with special characters fe. Huebner
		} else if (lineStripped.startsWith("%Elo")) {//elo data
			QStringList sl = lineStripped.split(" ");
			QStringList::Iterator it = sl.begin();
			it++;
			for (; it != sl.end(); it++) {
				QString s = *it;
				//std::cout << s << ":";
				QStringList sl2 = s.split(":");
				QString year = sl2[0];
				QString ratings = sl2[1];//ratings for one year, separated by commas
				QStringList sl3 = ratings.split(",");
				for (uint i = 0;i < (uint)sl3.size();i++) {
					if (sl3[i] != "?") {
//              std::cout << "rating for " << name << ": " << year << ": " << i+1 << ": " << sl3[i] << "\n";
						pdb.setElo(year.toInt(), i + 1, sl3[i].toInt());
					}
				}
			}
		} else if (lineStripped.startsWith("%")) {//unknown code
			//std::cout << "unknown code in ratings.ssp \n";
			//std::cout << line << "\n";
//ignore, as all kinds of tournament names are there now.
//return false;
		} else if (lineStripped.startsWith("= ")) {//name alias
			//std::cout << "name alias line: " << lineStripped << "\n";
			QString nameAlias = lineStripped.mid(2);
			if (!pdb.hasPhoto()) {
//look for players picture
				//std::cout << "nameAlias= " << nameAlias << "\n";
				for (int i = 0; i < pictures.size(); i++) {
					QStringList sl4 = pictures.at(i).split(".");
					if (sl4[0].compare(nameAlias) == 0) {
						QImage* img = new QImage(dir.path() + "/" + (pictures.at(i)));
						pdb.setPhoto(*img);
						numberOfPictures++;
						pictures.removeAt(i);
						break;
					}
				}
			}
		} else {//name line

			if (numberOfPlayers % 1000 == 0 && numberOfPlayers > 0) {
				std::cout << "processed: " << numberOfPlayers << "\n";
				pdb.commit();//seems binary content changed when doing this ?!
			}

			QStringList sl0 = line.split("#");
			name = sl0[0].trimmed();
			int index = sl0[1].indexOf(QChar(' '));
			QString title = sl0[1].left(index);
			QString rest = sl0[1].mid(index, 9999);

//        std::cout << "name= " << name << "\n";
//        std::cout << "title= " << title << "\n";
//        std::cout << "rest= " << rest << "\n";

			if (!pdb.add(name))
				std::cout << "failed adding player: " << name.toLatin1().constData() << "\n";

//look for players picture
			for (int i = 0; i < pictures.size(); i++) {
				QStringList sl4 = pictures.at(i).split(".");
				if (sl4[0].compare(name) == 0) {
					QImage* img = new QImage(dir.path() + "/" + pictures.at(i));
					pdb.setPhoto(*img);
					numberOfPictures++;
					pictures.removeAt(i);
					break;
				}
			}

			numberOfPlayers++;
			pdb.setTitle(title);

			QStringList sl = rest.split(" ");
			for (QStringList::Iterator it = sl.begin(); it != sl.end(); ++it) {
				QString s = *it;
				//std::cout << s << ":";
				if (s.startsWith("[")) {//rating, peak or estimated
					s = s.mid(1, s.length() - 2);
					//std::cout << "rating= " << s;
					if (s.endsWith("*"))
						pdb.setEstimatedElo(s.left(s.length() - 1).toInt());
					// peak elo is dealt with by setElo()
				} else if (s.contains(".")) {//birth date and/or death date
					if (s.startsWith("--")) {//only death date(!) Baay is an example
						pdb.setDateOfDeath(s.mid(2, 10));
					} else {
						pdb.setDateOfBirth(s.left(10));
						if (s.length() >= 22) {
							pdb.setDateOfDeath(s.mid(12, 10));
						}
					}
				} else {//country code(s)
					pdb.setCountry(s);
				}
			}
//        std::cout << "\n";
		}//name line
	}

	inFile.close();

	std::cout << numberOfPlayers << " players read in from ratings.ssp\n";
	std::cout << numberOfPictures << " players had a picture\n";
	for (QStringList::Iterator it = pictures.begin(); it != pictures.end(); ++it) {
		std::cout << "player picture that was not converted: " << (*it).toLatin1().constData() << "\n";
	}

	pdb.commit();
	pdb.close();

	return true;

}
