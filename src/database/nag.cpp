/***************************************************************************
 *   (C) 2006 William Hoggarth <whoggarth@users.sourceforge.net>           *
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "nag.h"

void NagSet::addNag(Nag nag)
{
	if (contains(nag) || nag == NullNag || nag >= NagCount)
		return;
	if (nag >= MoveNagStart && nag <= MoveNagEnd && nag != ForcedMove &&
		 nag != SingularMove)
		removeNagRange(MoveNagStart, MoveNagEnd);
	else if (nag >= EvaluationNagStart && nag <= EvaluationNagEnd)
		removeNagRange(EvaluationNagStart, EvaluationNagEnd);
	append(nag);
}

void NagSet::removeNag(Nag nag)
{
	if (nag == NullNag) return;

	int index = indexOf(nag);
	if (index != -1) remove(index);
}

QString NagSet::toString(unsigned format) const
{
	QString moveNag;
	QString evaluationNag;
	QString otherNags;

	for (int i = 0; i < count(); i++) {
		if (at(i) >= MoveNagStart && at(i) <= MoveNagEnd)
			moveNag.append(format == PGN ? " $" + QString::number(at(i)) : nagToString(at(i)));
		else if (at(i) >= EvaluationNagStart && at(i) <= EvaluationNagEnd)
			evaluationNag.append(format == PGN ? " $" + QString::number(at(i)) : nagToString(at(i)));
		else
			otherNags.append(format == PGN ? " $" + QString::number(at(i)) : nagToString(at(i)));
	}
	return moveNag + evaluationNag + otherNags;
}

static const QString g_nagStringList[NagCount] = {
	"",
	"!",
	"?",
	"!!",
	"??",
	"!?",
	"?!",
	QString::fromUtf8("□"),
	QString::fromUtf8("□"),
	"??",
	"=",
	"=",
	"=",
	QString::fromUtf8("∞"),
	"+=",
	"=+",
	QString::fromUtf8("±"),
	QString::fromUtf8("∓"),
	"+-",
	"-+",
	"+-",
	"-+",
	QString::fromUtf8("⨀"),
	QString::fromUtf8("⨀"),
	"with slight space advantage",
	"with slight space advantage",
	"with moderate space advantage",
	"with moderate space advantage",
	"with decisive space advantage",
	"with decisive space advantage",
	"with slight development advantage",
	"with slight development advantage",
	"with moderate development advantage",
	"with moderate development advantage",
	"with decisive development advantage",
	"with decisive development advantage",
	"with initiative",
	"with initiative",
	"with lasting initiative",
	"with lasting initiative",
	"with attack",
	"with attack",
	"with insufficient compensation for material deficit",
	"with insufficient compensation for material deficit",
	QString::fromUtf8("∞="),
	QString::fromUtf8("∞="),
	"with more than adequate compensation for material deficit",
	"with more than adequate compensation for material deficit",
	"with slight center control advantage",
	"with slight center control advantage",
	"with moderate center control advantage",
	"with moderate center control advantage",
	"with decisive center control advantage",
	"with decisive center control advantage",
	"with slight kingside control advantage",
	"with slight kingside control advantage",
	"with moderate kingside control advantage",
	"with moderate kingside control advantage",
	"with decisive kingside control advantage",
	"with decisive kingside control advantage",
	"with slight queenside control advantage",
	"with slight queenside control advantage",
	"with moderate queenside control advantage",
	"with moderate queenside control advantage",
	"with decisive queenside control advantage",
	"with decisive queenside control advantage",
	"with vulnerable first rank",
	"with vulnerable first rank",
	"with well protected first rank",
	"with well protected first rank",
	"with poorly protected king",
	"with poorly protected king",
	"with well protected king",
	"with well protected king",
	"with poorly placed king",
	"with poorly placed king",
	"with well placed king",
	"with well placed king",
	"with very weak pawn structure",
	"with very weak pawn structure",
	"with moderately weak pawn structure",
	"with moderately weak pawn structure",
	"with moderately strong pawn structure",
	"with moderately strong pawn structure",
	"with very strong pawn structure",
	"with very strong pawn structure",
	"with poor knight placement",
	"with poor knight placement",
	"with good knight placement",
	"with good knight placement",
	"with poor bishop placement",
	"with poor bishop placement",
	"with good bishop placement",
	"with good bishop placement",
	"with poor rook placement",
	"with poor rook placement",
	"with good rook placement",
	"with good rook placement",
	"with poor queen placement",
	"with poor queen placement",
	"with good queen placement",
	"with good queen placement",
	"with poor piece coordination",
	"with poor piece coordination",
	"with good piece coordination",
	"with good piece coordination",
	"White played the opening very poorly",
	"Black played the opening very poorly",
	"White played the opening poorly",
	"Black played the opening poorly",
	"White played the opening well",
	"Black played the opening well",
	"White played the opening very well",
	"Black played the opening very well",
	"White played the middlegame very poorly",
	"Black played the middlegame very poorly",
	"White played the middlegame poorly",
	"Black played the middlegame poorly",
	"White played the middlegame well",
	"Black played the middlegame well",
	"White played the middlegame very well",
	"Black played the middlegame very well",
	"White played the ending very poorly",
	"Black played the ending very poorly",
	"White played the ending poorly",
	"Black played the ending poorly",
	"White played the ending well",
	"Black played the ending well",
	"White played the ending very well",
	"Black played the ending very well",
	QString::fromUtf8("⇄"),
	QString::fromUtf8("⇄"),
	QString::fromUtf8("⇄"),
	QString::fromUtf8("⇄"),
	QString::fromUtf8("⇄"),
	QString::fromUtf8("⇄"),
	"Moderate time control pressure",
	"Moderate time control pressure",
	"Severe time control pressure",
	"Severe time control pressure",
	QString::fromUtf8("△"),
	"aimed against",
	"Better was",
	"Worse was",
	"Equivalent was",
	"RR",
	"N",
	"weak point",
	"endgame",
	"line",
	"diagonal",
	"pair of bishops",
	"pair of bishops",
	"bishops of opposite color",
	"bishops of the same color"
};

const QString& NagSet::nagToString(Nag nag)
{
	return g_nagStringList[nag];
}

Nag NagSet::fromString(const QString &nag)
{
	for (int i = 1; i < NagCount; i++)
		if (g_nagStringList[i] == nag)
			return Nag(i);
	return NullNag;
}

int NagSet::prefixCount(const QString &nag)
{
	QSet<QString> matches;
	for (int i = 1; i < NagCount; i++)
		if (g_nagStringList[i].startsWith(nag))
			matches.insert(g_nagStringList[i]);
	return matches.count();

}

void NagSet::removeNagRange(Nag from, Nag to)
{
	for (int i = count() - 1; i >= 0; i--)
		if (at(i) >= from && at(i) <= to)
			remove(i);
}

