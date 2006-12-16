/***************************************************************************
                          spellchecker.cpp  -  Spellchecker for names
                             -------------------
    begin                : 5 July 2005
    copyright            : (C) 2005 William Hoggarth
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

#include <qdatastream.h>
#include <qdir.h>
#include <qfile.h>
#include <qglobal.h>
#include <qregexp.h>

#include "spellchecker.h"

Spellchecker::Spellchecker()
{
}

bool Spellchecker::load(const QString& filename)
{
	//initialise data stream
	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly)) {
		return false;
	}
	QDataStream stream(&file); 
	quint32 magicNumber;
	
	stream >> magicNumber;
	if(magicNumber != 0xCD5CBD01U) {
		if(magicNumber & 0xFFFFFF != 0xCD5CBD00) {
			qWarning("Incorrect magic number for Spellchecker file.");
		} else {
			qWarning("Spellchecker file format newer than this program version");
		}
		return false;
	}
	
	stream.setVersion(5); //QDataStream::Qt_3_1

	//remove any existing rules
	clear();
	
	//read in data
	for(int ruleType = 0; ruleType < RuleTypeCount; ruleType++) {
		for(int spellingType = 0; spellingType < SpellingTypeCount; spellingType++) {
			stream >> m_maps[ruleType][spellingType];
		}
	}
	
	//check for errors
	file.close();
	if(file.error() == QFile::NoError) {
		return true;
	} else {
		clear();
		return false;
	}
}

bool Spellchecker::save(const QString& filename)
{
	//rename the file as a back up
	QFile file(filename);
	bool newFile = !file.exists();
	if(!newFile) {
		QDir dir;
		dir.rename(filename, filename + "~");
	}
	
	//open new file and initialise stream
	file.open(QIODevice::WriteOnly);
	QDataStream stream(&file);
	stream.setVersion(5); //QDataStream::Qt_3_1

	//write out data
	stream << (quint32)0xCD5CBD01U;
	
	for(int ruleType = 0; ruleType < RuleTypeCount; ruleType++) {
		for(int spellingType = 0; spellingType < SpellingTypeCount; spellingType++) {
			stream << m_maps[ruleType][spellingType];
		}
	}
	
	//if successful remove backup, otherwise restore it
	file.close();
	if(file.error() == QFile::NoError) {
		if(!newFile) {
			QDir dir;
			dir.remove(filename + "~");
		}
		return true;
	} else {
		QDir dir;
		dir.remove(filename);
		if(!newFile) {
			dir.rename(filename + "~", filename);
		}
		return false;
	}
}


bool Spellchecker::import(const QString& filename)
{
	//initialise stream
	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly)) {
		return false;
	}
	QTextStream stream(&file);

	//import rules
	bool imported = true;
	imported &= importSection(stream, "PLAYER", Player);
	imported &= importSection(stream, "SITE", Site);
	imported &= importSection(stream, "EVENT", Event);
	imported &= importSection(stream, "ROUND", Round);

	file.close();
	return imported;
}     

QString Spellchecker::correct(const QString& string,
																SpellingType spellingType) const
{
	QString corrected = string;
	
	//apply substitution rules first
	QMap<QString,QString>::const_iterator iterator;
	
	//prefixes
	for(iterator = m_maps[Prefix][spellingType].constBegin();
				iterator != m_maps[Prefix][spellingType].constEnd(); iterator++) {
		if(corrected.startsWith(iterator.key())) {
			corrected.replace(0, iterator.key().length(), iterator.value());
			break;
		}
	}

	//infixes
	for(iterator = m_maps[Infix][spellingType].constBegin();
				iterator != m_maps[Infix][spellingType].constEnd(); iterator++) {
		corrected.replace(iterator.key(), iterator.value());
	}

	//suffixes
	for(iterator = m_maps[Suffix][spellingType].constBegin();
				iterator != m_maps[Suffix][spellingType].constEnd(); iterator++) {
		if(corrected.endsWith(iterator.key())) {
			corrected.replace(corrected.lastIndexOf(iterator.key()),
											iterator.key().length(), iterator.value());
			break;
		}
	}
	
	//look for literal match
	QString standardised = standardise(corrected, spellingType);
	QString literalMatch = m_maps[Literal][spellingType][standardised];

	if(literalMatch != "") {
		//found, return
		return literalMatch;
	} else {
		//if player name, try rearranging the name
		if(spellingType == Player) {
			standardised = standardise(corrected.section(' ', -1) +
																		corrected.section(' ', 0, -2),
																		spellingType);	
			literalMatch = m_maps[Literal][spellingType][standardised];
			if(literalMatch != "") {
				//found, return
				return literalMatch;
			}
		}
	}
	
	return corrected;
}

QStringList Spellchecker::findSpellings(const QString& correct,
																					RuleType ruleType,
																					SpellingType spellingType) const
{
	//iterate through map looking for matches
	QStringList spellingList;
	QMap<QString,QString>::const_iterator iterator;
	
	for(iterator = m_maps[ruleType][spellingType].constBegin();
				iterator != m_maps[ruleType][spellingType].constEnd(); iterator++) {
		if(iterator.value() == correct) {
			spellingList << iterator.key();
		}
	}
	
	return spellingList;
}

void Spellchecker::addRule(const QString incorrect, const QString& correct,
															RuleType ruleType, SpellingType spellingType)
{
	QString standardised = incorrect;
	if(ruleType == Literal) {
		standardised = standardise(standardised, spellingType);
	}
	m_maps[ruleType][spellingType].insert(standardised, correct);
}

bool Spellchecker::removeRule(const QString& incorrect, RuleType ruleType,
																SpellingType spellingType)
{
	QString standardised = standardise(incorrect, spellingType);
	
	bool removed = m_maps[ruleType][spellingType].contains(standardised);
	m_maps[ruleType][spellingType].remove(standardised);
	return removed;
}

int Spellchecker::count() const
{
	int count = 0;
	
	for(int ruleType = 0; ruleType < RuleTypeCount; ruleType++) {
		for(int spellingType = 0; spellingType < SpellingTypeCount; spellingType++) {
			count += m_maps[ruleType][spellingType].count();
		}
	}
	
	return count;
}
				
void Spellchecker::clear()
{
	for(int ruleType = 0; ruleType < RuleTypeCount; ruleType++) {
		for(int spellingType = 0; spellingType < SpellingTypeCount; spellingType++) {
			m_maps[ruleType][spellingType].clear();
		}
	}
}

bool Spellchecker::importSection(QTextStream& stream, const QString& section,
																		SpellingType spellingType)
{
	//locate section
	QString line;
	
	while(true) {
		if(stream.atEnd()) {
			return false;
		}

		line = stream.readLine();

		if(line.startsWith("@" + section)) {
			//section found, exit loop
			break;
		}
	}
	
	//add rules in section
	int lineNo = 0;
	QString incorrect = "";
	QString correct = "";

	while(!stream.atEnd()) {

		line = stream.readLine();
		line = line.trimmed();
		lineNo++;

		
		if(line.length() == 0) {
			//empty line, ignore
			continue;
		}
		
		if(line == "### END OF " + section + " SECTION") {
			//end of section, finish
			break;
		}
		
		if(line.at(0) == '#') {
			//comment, ignore line
			continue;
		}

		if(line.at(0) == '%') {
			if(line.startsWith("%Prefix")) {
				//prefix rule
				incorrect = line.section("\"", 1, 1);
				correct = line.section("\"", 3, 3);
				addRule(incorrect, correct, Prefix, spellingType);
				incorrect = "";
				continue;
			}

			if(line.startsWith("%Infix")) {
				//infix rule
				incorrect = line.section("\"", 1, 1);
				correct = line.section("\"", 3, 3);
				addRule(incorrect, correct, Infix, spellingType);
				incorrect = "";
				continue;
			}

			if(line.startsWith("%Suffix")) {
				//suffix rule
				incorrect = line.section("\"", 1, 1);
				correct = line.section("\"", 3, 3);
				addRule(incorrect, correct, Suffix, spellingType);
				incorrect = "";
				continue;
			}

			//must be bio information, ignore line
			continue;
		}

		if(line.at(0) == '=') {
			//alternate spelling, add to map
			if(correct == "") {
				qWarning("Error at line %d whilst importing Spellcheck file", lineNo);
				return false;
			}
			
			line.remove(0, 1); // remove =
			line = line.trimmed();
			addRule(line, correct, Literal, spellingType);
			continue;
		}
		
		//must be a correctly spelled name, remove comments and store
		int hashIndex = line.indexOf('#');
		if(hashIndex != -1) {
			line.truncate(hashIndex - 1);
		}
		correct = line.trimmed();
		addRule(correct, correct, Literal, spellingType);
	}

	return true;
}

QString Spellchecker::standardise(const QString& string,
																		SpellingType spellingType) const
{
	//remove exterraneous characters
	QString standardised = string;
	standardised.remove(QRegExp("[.,\\s-_()]"));
	
	if(spellingType == Player) {
		//capitalise first letter
		standardised.replace(0, 1, string.at(0).toUpper());
	
		//standardise captilisation of names beginning with "Van de"
		standardised.replace("van de", "Van de", Qt::CaseInsensitive);
	}
	
	return standardised;
}
