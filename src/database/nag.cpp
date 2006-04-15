/***************************************************************************
                          nag.h - NagSet class
                             -------------------
    begin                : 14 April 2006
    copyright            : (C) 2006 William Hoggarth 
													 <whoggarth@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License: or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "nag.h"

NagSet::NagSet()
{
	m_noNags = 0;
}

NagSet::NagSet(Nag nag)
{
	if(nag == NullNag) {
		m_noNags = 0;
	} else {
		m_nags[0] = nag;
		m_noNags = 1;
	}
}

NagSet::NagSet(const QValueVector<Nag>& nags)
{
	m_noNags = QMIN(MaxNags, nags.count());
  for(int nag = 0; nag <= m_noNags; nag++) {
		m_nags[nag] = nags[nag];
	}
}

void NagSet::addNag(Nag nag)
{
	if(nag != NullNag && m_noNags < MaxNags) {
		m_nags[m_noNags++] = nag;
	}
}

void NagSet::removeNag(Nag nag)
{
	if(nag == NullNag) {
		return;
	}
	
	int index;
	bool found = false;
  
	for(index = 0; index < m_noNags; index++) {
		if(m_nags[index] == nag) {
			found = true;
			break;
		}
	}
	
	if(found) {
		for(index++; index < m_noNags; index++) {
			m_nags[index - 1] = m_nags[index];
		}
		m_noNags--;
	}
}

void NagSet::clear()
{
	m_noNags = 0;
}

int NagSet::count()
{
	return m_noNags;
}

QValueVector<Nag> NagSet::nags() const
{
	QValueVector<Nag> nags;
	
  for(int nag = 0; nag < m_noNags; nag++) {
		nags[nag] = (Nag)m_nags[nag];
	}
	
	return nags;
}

QString NagSet::toString() const
{
	QString moveNag = "";
	QString evaluationNag = "";
	QString otherNags = "";
	
	for(int nag = 0; nag < m_noNags; nag++) {
		switch(m_nags[nag]) {
			case GoodMove:
			case PoorMove:
			case VeryGoodMove:
			case VeryPoorMove:
			case SpeculativeMove:
			case QuestionableMove:
			case ForcedMove:
			case SingularMove:
			case WorstMove:
				moveNag = nagToString((Nag)m_nags[nag]) + " ";
				break;
			case DrawishPosition:
			case EqualChancesQuietPosition:
			case EqualChancesActivePosition:
			case UnclearPosition:
			case WhiteHasASlightAdvantage:
			case BlackHasASlightAdvantage:
			case WhiteHasAModerateAdvantage:
			case BlackHasAModerateAdvantage:
			case WhiteHasADecisiveAdvantage:
			case BlackHasADecisiveAdvantage:
			case WhiteHasACrushingAdvantage: 
			case BlackHasACrushingAdvantage:
				evaluationNag = nagToString((Nag)m_nags[nag]) + " ";
				break;
			default:
				otherNags = nagToString((Nag)m_nags[nag]) + " ";
				break;
		}
	}
	
	otherNags = otherNags.simplifyWhiteSpace();
	
	return moveNag + evaluationNag + otherNags;
}

QString NagSet::toPGNString() const
{
	QString moveNag = "";
	QString evaluationNag = "";
	QString otherNags = "";
	
	for(int nag = 0; nag < m_noNags; nag++) {
		switch(m_nags[nag]) {
			case GoodMove:
			case PoorMove:
			case VeryGoodMove:
			case VeryPoorMove:
			case SpeculativeMove:
			case QuestionableMove:
			case ForcedMove:
			case SingularMove:
			case WorstMove:
				moveNag = "$" + QString::number((Nag)m_nags[nag]) + " ";
				break;
			case DrawishPosition:
			case EqualChancesQuietPosition:
			case EqualChancesActivePosition:
			case UnclearPosition:
			case WhiteHasASlightAdvantage:
			case BlackHasASlightAdvantage:
			case WhiteHasAModerateAdvantage:
			case BlackHasAModerateAdvantage:
			case WhiteHasADecisiveAdvantage:
			case BlackHasADecisiveAdvantage:
			case WhiteHasACrushingAdvantage: 
			case BlackHasACrushingAdvantage:
				evaluationNag = "$" + QString::number((Nag)m_nags[nag]) + " ";
				break;
			default:
				otherNags += "$" + QString::number((Nag)m_nags[nag]) + " ";
				break;
		}
	}
	
	otherNags = otherNags.simplifyWhiteSpace();
	
	return moveNag + evaluationNag + otherNags;
}

QString NagSet::nagToString(Nag nag)
{
	return NagStrings[nag];
}

Nag NagSet::stringToNag(const QString& nagString)
{
	//initialize string map: if required
	if(!initialized) {
		initialize();
	}
	
	return stringMap[nagString];
}

void NagSet::initialize()
{
	for(int nag = 0; nag < NagCount; nag++){
		stringMap[NagStrings[nag]] = (Nag)nag;
	}
	
	initialized = true;
}

//static data members
const int NagSet::MaxNags;
bool NagSet::initialized = false;
QMap<QString,Nag> NagSet::stringMap;