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
#include <qglobal.h>
#include <qregexp.h>

#include "spellchecker.h"

Spellchecker::Spellchecker()
{
}

bool Spellchecker::load(QFile& file)
{
	//initialise data stream
	file.at(0);
	QDataStream stream(&file); 
	Q_UINT32 magicNumber;
	
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
	removeAllRules();
	
	//read in data
	for(int ruleType = 0; ruleType < noRuleTypes; ruleType++) {
		for(int spellingType = 0; spellingType < noSpellingTypes; spellingType++) {
			stream >> m_maps[ruleType][spellingType];
		}
	}
	
	//check for errors
	if(file.status() == (unsigned)IO_Ok) {
		return true;
	} else {
		removeAllRules();
		return false;
	}
}

bool Spellchecker::save(QFile& file)
{
	//rename the file as a back up
	if(file.isOpen()) {
			file.close();
	}
	QString name = file.name();
	QString backupName = name + "~";
	QDir dir;
	dir.rename(name, backupName);
	
	//open new file and initialise stream
	QFile newFile(name);
	newFile.open(IO_WriteOnly);
	QDataStream stream(&newFile);
	stream.setVersion(5); //QDataStream::Qt_3_1

	//write out data
	stream << (Q_UINT32)0xCD5CBD01U;
	
	for(int ruleType = 0; ruleType < noRuleTypes; ruleType++) {
		for(int spellingType = 0; spellingType < noSpellingTypes; spellingType++) {
			stream << m_maps[ruleType][spellingType];
		}
	}
	
	//if successful remove backup, otherwise restore it
	newFile.close();
	if(newFile.status() == (unsigned)IO_Ok) {
		dir.remove(backupName);
		file.open(IO_ReadWrite);
		return true;
	} else {
		dir.remove(name);
		dir.rename(backupName, name);
		file.open(IO_ReadWrite);
		return false;
	}
}


bool Spellchecker::import(QFile& file)
{
	//initialise stream
	file.at(0);
	QTextStream stream(&file);

	//import rules
	bool imported = true;
	imported &= importSection(stream, "PLAYER", PLAYER);
	imported &= importSection(stream, "SITE", SITE);
	imported &= importSection(stream, "EVENT", EVENT);
	imported &= importSection(stream, "ROUND", ROUND);

	return imported;
}     

void Spellchecker::correct(QString& string, SpellingType spellingType) const
{
	//apply substitution rules first
	QMap<QString,QString>::const_iterator iterator;
	
	//prefixes
	for(iterator = m_maps[PREFIX][spellingType].constBegin();
				iterator != m_maps[PREFIX][spellingType].constEnd(); iterator++) {
		if(string.startsWith(iterator.key())) {
			string.replace(0, iterator.key().length(), iterator.data());
			break;
		}
	}

	//infixes
	for(iterator = m_maps[INFIX][spellingType].constBegin();
				iterator != m_maps[INFIX][spellingType].constEnd(); iterator++) {
		string.replace(iterator.key(), iterator.data());
	}

	//suffixes
	for(iterator = m_maps[SUFFIX][spellingType].constBegin();
				iterator != m_maps[SUFFIX][spellingType].constEnd(); iterator++) {
		if(string.endsWith(iterator.key())) {
			string.replace(string.findRev(iterator.key()),
											iterator.key().length(), iterator.data());
			break;
		}
	}
	
	//look for literal match
	QString standardised = string;
	standardise(standardised, spellingType);
	QString corrected = m_maps[LITERAL][spellingType][standardised];

	if(corrected != "") {
		//found, return
		string = corrected;
		return;
	} else {
		//if player name, try rearranging the name
		if(spellingType == PLAYER) {
			standardised = string.section(' ', -1) +
											string.section(' ', 0, -2);
			standardise(standardised, spellingType);
			
			corrected = m_maps[LITERAL][spellingType][standardised];
			if(corrected != "") {
				//found, return
				string = corrected;
				return;
			}
		}
	}
}

void Spellchecker::findSpellings(QStringList& spellingList,
																		const QString& correct, RuleType ruleType,
																		SpellingType spellingType) const
{
	//iterate through map looking for matches
	QMap<QString,QString>::const_iterator iterator;
	
	for(iterator = m_maps[ruleType][spellingType].constBegin();
				iterator != m_maps[ruleType][spellingType].constEnd(); iterator++) {
		if(iterator.data() == correct) {
			spellingList << iterator.key();
		}
	}
}

void Spellchecker::addRule(const QString incorrect, const QString& correct,
															RuleType ruleType, SpellingType spellingType)
{
	QString standardised = incorrect;
	if(ruleType == LITERAL) {
		standardise(standardised, spellingType);
	}
	m_maps[ruleType][spellingType].insert(standardised, correct);
}

bool Spellchecker::removeRule(const QString& incorrect, RuleType ruleType,
																SpellingType spellingType)
{
	QString standardised = incorrect;
	standardise(standardised, spellingType);
	
	bool removed = m_maps[ruleType][spellingType].contains(incorrect);
	m_maps[ruleType][spellingType].remove(incorrect);
	return removed;
}


int Spellchecker::countAllRules() const
{
	int count = 0;
	
	for(int ruleType = 0; ruleType < noRuleTypes; ruleType++) {
		for(int spellingType = 0; spellingType < noSpellingTypes; spellingType++) {
			count += m_maps[ruleType][spellingType].count();
		}
	}
	
	return count;
}
				
void Spellchecker::removeAllRules()       
{
	for(int ruleType = 0; ruleType < noRuleTypes; ruleType++) {
		for(int spellingType = 0; spellingType < noSpellingTypes; spellingType++) {
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
			//section not found
			qWarning("Unable to find " + section +
								" section in Spellcheck import file.");
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
		line = line.stripWhiteSpace();
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
				addRule(incorrect, correct, PREFIX, spellingType);
				incorrect = "";
				continue;
			}

			if(line.startsWith("%Infix")) {
				//infix rule
				incorrect = line.section("\"", 1, 1);
				correct = line.section("\"", 3, 3);
				addRule(incorrect, correct, INFIX, spellingType);
				incorrect = "";
				continue;
			}

			if(line.startsWith("%Suffix")) {
				//suffix rule
				incorrect = line.section("\"", 1, 1);
				correct = line.section("\"", 3, 3);
				addRule(incorrect, correct, SUFFIX, spellingType);
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
			line = line.stripWhiteSpace();
			addRule(line, correct, LITERAL, spellingType);
			continue;
		}
		
		//must be a correctly spelled name, remove comments and store
		int hashIndex = line.find('#');
		if(hashIndex != -1) {
			line.truncate(hashIndex - 1);
		}
		correct = line.stripWhiteSpace();
		addRule(correct, correct, LITERAL, spellingType);
	}

	return true;
}

void Spellchecker::standardise(QString& string, SpellingType spellingType) const
{
	//remove exterraneous characters
	string.remove(QRegExp("[.,\\s-_()]"));
	
	if(spellingType == PLAYER) {
		//capitalise first letter
		string.replace(0, 1, string.at(0).upper());
	
		//standardise captilisation of names beginning with "Van de"
		string.replace("van de", "Van de", false);
	}
}
