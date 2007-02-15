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

#include <QMap>
#include <QString>
#include <QStringList>
#include <QTextStream>

/** @ingroup Database
	 The Spellchecker class provides spellchecker functionality.
	 
	 The Spellchecker can import spellings from specially formatted text files, or
	 load them from a native binary format. The class caters for a number of
	 spelling types and rules for correcting spelling.
 */

class Spellchecker
{
 public:
 	
  /**
	 	 The rule type indicates what kind of substitution a spelling rule performs
	 
	   Spellings are corrected with a number of different substitution rules. Each
		 rule type indicates where in the string it will look for a substring. When
		 a match is found, it is replaced with the corrected substring. 
		 
		 The literal rule is preferred as it is often the most suitable and is the
		 most efficient. It works as a straight match and replace on the whole
		 string. If many similar spelling mistakes are made at the beginning or end
		 of a string, the prefix and suffix rules may be more appropriate. This
		 results in more corrections with less rules. Finally, if a common spelling
		 error maybe found in any part of the string, the infix rule should be used.
		 
		 During correction relevant rules will be applied the following order:
		 prefix, infix, suffix, then literal. You will need to	 be aware of this when
		 more than one rule may be relevant to a particular spelling.
	 */
	enum RuleType {	Literal, /**< Matches the whole string */
									Prefix, /**< Matches at the beginning of the string */
									Infix, /**< Matches anywhere in the string */
									Suffix, /**< Matches at the end of the string */
									RuleTypeCount };
	
	/**	The spelling type indicates what kind of spelling is to be corrected */
 	enum SpellingType {	Player, /**< Player name */
											Site, /**< Site name */
											Event, /**< Event name */
											Round, /**< Round */
											SpellingTypeCount };
 
	/** Default constructor, creates a Spellchecker with no spellings */
	Spellchecker();
	
	/** Loads spellings from a native format */
	bool load(const QString& filename);
	
	/** Saves spellings to a native format */
	bool save(const QString& filename);
	
	/** Imports spellings from a specically formatted text file */
	bool import(const QString& filename);
 
	/**
		 Corrects a spelling 
	
		 The string parameter has the various spelling rules applied to it, and the
		 resultant corrected spelling is returned.
	*/
	QString correct(const QString& string, SpellingType spellingType) const;
	
	/** Finds the incorrect spellings associated with a given correct spelling */
	QStringList findSpellings(const QString& correct, RuleType ruleType,
															SpellingType spellingType) const;
	
	/** Adds a spelling rule to the Spellchecker */
	void addRule(const QString incorrect, const QString& correct,
								RuleType ruleType, SpellingType spellingType);
										
	/** Removes a spelling rule */
	bool removeRule(const QString& incorrect, RuleType ruleType,
										SpellingType spellingType);

	/** Returns the number of spelling rules, of all types, in the Spellchecker */
	int count() const;
												
	/** Removes all spelling rules from the Spellchecker */
	void clear();

 private:
 	/** Imports a section from a specially formatted text file */
 	bool importSection(QTextStream& stream, const QString& section,
											SpellingType spellingType);
											
	/** Standardises the format of a string for matching purposes */
	QString standardise(const QString& string, SpellingType spellingType) const;
 	
	QMap<QString,QString> m_maps[RuleTypeCount][SpellingTypeCount];
};
