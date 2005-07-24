/***************************************************************************
                          spellchecker.h  -  Spellchecker for names
                             -------------------
    begin                : 4 July 2005
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

#include <qfile.h>
#include <qmap.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtextstream.h>

/**
	 The Spellchecker class provides spellchecker functionality. Spellings
	 can be imported from specially formatted text files, or loaded from a
	 native binary format. There is support for literal matching and
	 substitution rules.
 */

class Spellchecker
{
 public:
 	
 	/** Different types of rules for spelling correction */
	enum RuleType { LITERAL, PREFIX, INFIX, SUFFIX };
	
	/** Different types of spellings */
 	enum SpellingType { PLAYER, SITE, EVENT, ROUND };
 
	/** Default constructor, creates a Spellchecker with no spellings */
	Spellchecker();
	
	/** Loads spellings from a native format */
	bool load(QFile& file);
	
	/** Saves spellings to a native format */
	bool save(QFile& file);
	
	/** Imports spellings from a specically formatted text file */
	bool import(QFile& file);
 
	/** Corrects a spelling */
	void correct(QString& string, SpellingType spellingType) const;
	
	/** Finds the incorrect spellings associated with a given correct spelling */
	void findSpellings(QStringList& spellingList, const QString& correct,
											RuleType ruleType, SpellingType spellingType) const;
	
	/** Adds a spelling rule to the Spellchecker */
	void addRule(const QString incorrect, const QString& correct,
								RuleType ruleType, SpellingType spellingType);
										
	/** Removes a spelling rule */
	bool removeRule(const QString& incorrect, RuleType ruleType,
										SpellingType spellingType);

	/** Returns the number of spelling rules in Spellchecker */
	int countAllRules() const;
												
	/** Removes all spelling rules from the Spellchecker */
	void removeAllRules();

 private:
 	/** Imports a section from a specially formatted text file */
 	bool importSection(QTextStream& stream, const QString& section,
											SpellingType spellingType);
											
	/** Standardises the format of a string for matching purposes */
	void standardise(QString& string, SpellingType spellingType) const;
	
	static const int noRuleTypes = SUFFIX + 1;
 	static const int noSpellingTypes = ROUND + 1;
 	
	QMap<QString,QString> m_maps[noRuleTypes][noSpellingTypes];
};
