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

QStringList NagSet::g_nagStringListLong;

void NagSet::addNag(Nag nag)
{
	if (contains(nag) || nag == NullNag || nag >= NagCount)
		return;
    if (nag >= MoveNagStart && nag <= MoveNagEnd && nag != ForcedMove && nag != SingularMove)
		removeNagRange(MoveNagStart, MoveNagEnd);

    if (conditionalRemoveNagRange(nag, EvaluationNagStart, EvaluationNagEnd))
    {
        removeNag(WhiteHasModerateCounterplay);
    }
    if (conditionalRemoveNagRange(nag, WhiteHasModerateCounterplay, BlackHasModerateCounterplay))
    {
        removeNagRange(EvaluationNagStart, EvaluationNagEnd);
    }
    conditionalRemoveNagRange(nag, BishopNagStart,     BishopNagEnd);
    conditionalRemoveNagRange(nag, OpeningNagStart,    OpeningNagEnd);
    conditionalRemoveNagRange(nag, MiddleGameNagStart, MiddleGameNagEnd);
    conditionalRemoveNagRange(nag, EndingNagStart,     EndingNagEnd);

    append(nag);
}

bool NagSet::conditionalRemoveNagRange(Nag nag, Nag start, Nag end)
{
    if (nag >= start && nag <= end)
    {
        removeNagRange(start, end);
        return true;
    }
    return false;
}

void NagSet::removeNag(Nag nag)
{
	if (nag == NullNag) return;

	int index = indexOf(nag);
	if (index != -1) remove(index);
}

bool NagSet::isMoveNag(Nag nag)
{
    return (nag >= MoveNagStart && nag <= MoveNagEnd);
}

bool NagSet::isEvalNag(Nag nag)
{
    return (nag >= EvaluationNagStart && nag <= EvaluationNagEnd);
}

bool NagSet::isOtherNag(Nag nag)
{
    return ((nag < NagCount) && !isEvalNag(nag) && !isMoveNag(nag));
}

QString NagSet::toString(unsigned format) const
{
    QString moveNags;
    QString evaluationNags;
	QString otherNags;

    for (int i = 0; i < count(); ++i)
    {
        QString strNag;
        if (at(i)<NagCount)
        {
            switch (format)
            {
            case Simple:
                strNag = nagToString(at(i));
                break;
            case PGN:
                strNag = " $" + QString::number(at(i));
                break;
            case HTML:
                strNag = nagToStringHTML(at(i));
                break;
            }
        }

        if (isMoveNag(at(i)))
            moveNags.append(strNag);
        else if (isEvalNag(at(i)))
            evaluationNags.append(strNag);
        else
            otherNags.append(strNag);
	}

    return moveNags + evaluationNags + otherNags;
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
    "++--",
    "--++",
    QString::fromUtf8("⨀"), // Zugzwang
    QString::fromUtf8("⨀"), // Zugzwang
    "With slight space advantage",
    "With slight space advantage",
    "With moderate space advantage",
    "With moderate space advantage",
    "With decisive space advantage",
    "With decisive space advantage",
    "With slight development advantage",
    "With slight development advantage",
    QString::fromUtf8("⟳"), // "With moderate development advantage"
    QString::fromUtf8("⟳"), // "With moderate development advantage"
    "With decisive development advantage",
    "With decisive development advantage",
    QString::fromUtf8("→"), // "With initiative"
    QString::fromUtf8("→"), // "With initiative"
    "With lasting initiative",
    "With lasting initiative",
    QString::fromUtf8("↑"), // "With attack"
    QString::fromUtf8("↑"), // "With attack"
    "With insufficient compensation for material deficit",
    "With insufficient compensation for material deficit",
    QString::fromUtf8("∞="),
	QString::fromUtf8("∞="),
    "With more than adequate compensation for material deficit",
    "With more than adequate compensation for material deficit",
    "With slight center control advantage",
    "With slight center control advantage",
    "With moderate center control advantage",
    "With moderate center control advantage",
    "With decisive center control advantage",
    "With decisive center control advantage",
    "With slight kingside control advantage",
    "With slight kingside control advantage",
    "With moderate kingside control advantage",
    "With moderate kingside control advantage",
    "With decisive kingside control advantage",
    "With decisive kingside control advantage",
    "With slight queenside control advantage",
    "With slight queenside control advantage",
    "With moderate queenside control advantage",
    "With moderate queenside control advantage",
    "With decisive queenside control advantage",
    "With decisive queenside control advantage",
    "With vulnerable first rank",
    "With vulnerable first rank",
    "With well protected first rank",
    "With well protected first rank",
    "With poorly protected king",
    "With poorly protected king",
    "With well protected king",
    "With well protected king",
    "With poorly placed king",
    "With poorly placed king",
    "With well placed king",
    "With well placed king",
    "With very weak pawn structure",
    "With very weak pawn structure",
    "With moderately weak pawn structure",
    "With moderately weak pawn structure",
    "With moderately strong pawn structure",
    "With moderately strong pawn structure",
    "With very strong pawn structure",
    "With very strong pawn structure",
    "With poor knight placement",
    "With poor knight placement",
    "With good knight placement",
    "With good knight placement",
    "With poor bishop placement",
    "With poor bishop placement",
    "With good bishop placement",
    "With good bishop placement",
    "With poor rook placement",
    "With poor rook placement",
    "With good rook placement",
    "With good rook placement",
    "With poor queen placement",
    "With poor queen placement",
    "With good queen placement",
    "With good queen placement",
    "With poor piece coordination",
    "With poor piece coordination",
    "With good piece coordination",
    "With good piece coordination",
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
    QString::fromUtf8("⊕"), // "Severe time control pressure"
    QString::fromUtf8("⊕"), // "Severe time control pressure"
    QString::fromUtf8("△"),
    "Aimed against",
    QString::fromUtf8("⌓"), // "Better was"
    "<", // "Worse was"
	"Equivalent was",
	"RR",
	"N",
    QString::fromUtf8("✕"), // "weak point"
    QString::fromUtf8("⊥"), // "endgame"
    QString::fromUtf8("⇔"), // "line"
    QString::fromUtf8("⇗"), // "diagonal",
    QString::fromUtf8("☍"), // "pair of bishops"
    QString::fromUtf8("☍"), // "pair of bishops"
    QString::fromUtf8("❐"), // "bishops of opposite color"
    QString::fromUtf8("▞"), // "bishops of the same color"
    0,0,0,0,0,0, // 160
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //180
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //200
    "#"
};

void NagSet::InitNagStringListLong()
{
    g_nagStringListLong <<
    "" <<
    tr("Good Move") <<
    tr("Poor Move") <<
    tr("Very good Move") <<
    tr("Blunder") <<
    tr("Speculative Move") <<
    tr("Dubious Move") <<
    tr("Only Move") <<
    tr("Only Move") <<
    tr("Blunder") <<
    tr("Equal") <<
    tr("Equal") <<
    tr("Equal") <<
    tr("Unclear") <<
    tr("White has slight advantage") <<
    tr("Black has slight advantage") <<
    tr("White has moderate advantage") <<
    tr("Black has moderate advantage") <<
    tr("White has decisive advantage") <<
    tr("Black has decisive advantage") <<
    tr("White has crushing advantage") <<
    tr("Black has crushing advantage") <<
    tr("Zugzwang") <<
    tr("Zugzwang") <<
    tr("With slight space advantage") <<
    tr("With slight space advantage") <<
    tr("With moderate space advantage") <<
    tr("With moderate space advantage") <<
    tr("With decisive space advantage") <<
    tr("With decisive space advantage") <<
    tr("With slight development advantage") <<
    tr("With slight development advantage") <<
    tr("With moderate development advantage") <<
    tr("With moderate development advantage") <<
    tr("With decisive development advantage") <<
    tr("With decisive development advantage") <<
    tr("With initiative") <<
    tr("With initiative") <<
    tr("With lasting initiative") <<
    tr("With lasting initiative") <<
    tr("With attack") <<
    tr("With attack") <<
    tr("With insufficient compensation for material deficit") <<
    tr("With insufficient compensation for material deficit") <<
    tr("With compensation") <<
    tr("With compensation") <<
    tr("With more than adequate compensation for material deficit") <<
    tr("With more than adequate compensation for material deficit") <<
    tr("With slight center control advantage") <<
    tr("With slight center control advantage") <<
    tr("With moderate center control advantage") <<
    tr("With moderate center control advantage") <<
    tr("With decisive center control advantage") <<
    tr("With decisive center control advantage") <<
    tr("With slight kingside control advantage") <<
    tr("With slight kingside control advantage") <<
    tr("With moderate kingside control advantage") <<
    tr("With moderate kingside control advantage") <<
    tr("With decisive kingside control advantage") <<
    tr("With decisive kingside control advantage") <<
    tr("With slight queenside control advantage") <<
    tr("With slight queenside control advantage") <<
    tr("With moderate queenside control advantage") <<
    tr("With moderate queenside control advantage") <<
    tr("With decisive queenside control advantage") <<
    tr("With decisive queenside control advantage") <<
    tr("With vulnerable first rank") <<
    tr("With vulnerable first rank") <<
    tr("With well protected first rank") <<
    tr("With well protected first rank") <<
    tr("With poorly protected king") <<
    tr("With poorly protected king") <<
    tr("With well protected king") <<
    tr("With well protected king") <<
    tr("With poorly placed king") <<
    tr("With poorly placed king") <<
    tr("With well placed king") <<
    tr("With well placed king") <<
    tr("With very weak pawn structure") <<
    tr("With very weak pawn structure") <<
    tr("With moderately weak pawn structure") <<
    tr("With moderately weak pawn structure") <<
    tr("With moderately strong pawn structure") <<
    tr("With moderately strong pawn structure") <<
    tr("With very strong pawn structure") <<
    tr("With very strong pawn structure") <<
    tr("With poor knight placement") <<
    tr("With poor knight placement") <<
    tr("With good knight placement") <<
    tr("With good knight placement") <<
    tr("With poor bishop placement") <<
    tr("With poor bishop placement") <<
    tr("With good bishop placement") <<
    tr("With good bishop placement") <<
    tr("With poor rook placement") <<
    tr("With poor rook placement") <<
    tr("With good rook placement") <<
    tr("With good rook placement") <<
    tr("With poor queen placement") <<
    tr("With poor queen placement") <<
    tr("With good queen placement") <<
    tr("With good queen placement") <<
    tr("With poor piece coordination") <<
    tr("With poor piece coordination") <<
    tr("With good piece coordination") <<
    tr("With good piece coordination") <<
    tr("White played the opening very poorly") <<
    tr("Black played the opening very poorly") <<
    tr("White played the opening poorly") <<
    tr("Black played the opening poorly") <<
    tr("White played the opening well") <<
    tr("Black played the opening well") <<
    tr("White played the opening very well") <<
    tr("Black played the opening very well") <<
    tr("White played the middlegame very poorly") <<
    tr("Black played the middlegame very poorly") <<
    tr("White played the middlegame poorly") <<
    tr("Black played the middlegame poorly") <<
    tr("White played the middlegame well") <<
    tr("Black played the middlegame well") <<
    tr("White played the middlegame very well") <<
    tr("Black played the middlegame very well") <<
    tr("White played the ending very poorly") <<
    tr("Black played the ending very poorly") <<
    tr("White played the ending poorly") <<
    tr("Black played the ending poorly") <<
    tr("White played the ending well") <<
    tr("Black played the ending well") <<
    tr("White played the ending very well") <<
    tr("Black played the ending very well") <<
    tr("With counterplay") <<
    tr("With counterplay") <<
    tr("With counterplay") <<
    tr("With counterplay") <<
    tr("With counterplay") <<
    tr("With counterplay") <<
    tr("Moderate time control pressure") <<
    tr("Moderate time control pressure") <<
    tr("Severe time control pressure") <<
    tr("Severe time control pressure") <<
    tr("With the idea") <<
    tr("Aimed against") <<
    tr("Better was") <<
    tr("Worse was") <<
    tr("Equivalent was") <<
    tr("RR") <<
    tr("N") <<
    tr("Weak point") <<
    tr("Endgame") <<
    tr("Line") <<
    tr("Diagonal") <<
    tr("Pair of bishops") <<
    tr("Pair of bishops") <<
    tr("Bishops of opposite color") <<
    tr("Bishops of the same color") <<
    "" <<"" <<"" <<"" <<"" <<"" << // 160
    "" <<"" <<"" <<"" <<"" <<"" <<"" <<"" <<"" <<"" <<
    "" <<"" <<"" <<"" <<"" <<"" <<"" <<"" <<"" <<"" << //180
    "" <<"" <<"" <<"" <<"" <<"" <<"" <<"" <<"" <<"" <<
    "" <<"" <<"" <<"" <<"" <<"" <<"" <<"" <<"" <<"" << //200
    tr("Diagram");
}

static const QString g_nagStringListHTML[NagCount] = {
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
    "&prop;",
    "+=",
    "=+",
    "&plusmn;",
    QString::fromUtf8("∓"),
    "+-",
    "-+",
    "++--",
    "--++",
    "&Theta;", // Zugzwang
    "&Theta;", // Zugzwang
    "With slight space advantage",
    "With slight space advantage",
    "With moderate space advantage",
    "With moderate space advantage",
    "With decisive space advantage",
    "With decisive space advantage",
    "With slight development advantage",
    "With slight development advantage",
    QString::fromUtf8("⟳"), // "With moderate development advantage"
    QString::fromUtf8("⟳"), // "With moderate development advantage"
    "With decisive development advantage",
    "With decisive development advantage",
    "&rarr;", // "With initiative"
    "&rarr;", // "With initiative"
    "With lasting initiative",
    "With lasting initiative",
    "&uarr;", // "With attack"
    "&uarr;", // "With attack"
    "With insufficient compensation for material deficit",
    "With insufficient compensation for material deficit",
    "&prop;=",
    "&prop;=",
    "With more than adequate compensation for material deficit",
    "With more than adequate compensation for material deficit",
    "With slight center control advantage",
    "With slight center control advantage",
    "With moderate center control advantage",
    "With moderate center control advantage",
    "With decisive center control advantage",
    "With decisive center control advantage",
    "With slight kingside control advantage",
    "With slight kingside control advantage",
    "With moderate kingside control advantage",
    "With moderate kingside control advantage",
    "With decisive kingside control advantage",
    "With decisive kingside control advantage",
    "With slight queenside control advantage",
    "With slight queenside control advantage",
    "With moderate queenside control advantage",
    "With moderate queenside control advantage",
    "With decisive queenside control advantage",
    "With decisive queenside control advantage",
    "With vulnerable first rank",
    "With vulnerable first rank",
    "With well protected first rank",
    "With well protected first rank",
    "With poorly protected king",
    "With poorly protected king",
    "With well protected king",
    "With well protected king",
    "With poorly placed king",
    "With poorly placed king",
    "With well placed king",
    "With well placed king",
    "With very weak pawn structure",
    "With very weak pawn structure",
    "With moderately weak pawn structure",
    "With moderately weak pawn structure",
    "With moderately strong pawn structure",
    "With moderately strong pawn structure",
    "With very strong pawn structure",
    "With very strong pawn structure",
    "With poor knight placement",
    "With poor knight placement",
    "With good knight placement",
    "With good knight placement",
    "With poor bishop placement",
    "With poor bishop placement",
    "With good bishop placement",
    "With good bishop placement",
    "With poor rook placement",
    "With poor rook placement",
    "With good rook placement",
    "With good rook placement",
    "With poor queen placement",
    "With poor queen placement",
    "With good queen placement",
    "With good queen placement",
    "With poor piece coordination",
    "With poor piece coordination",
    "With good piece coordination",
    "With good piece coordination",
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
    "&oplus;", // "Severe time control pressure"
    "&oplus;", // "Severe time control pressure"
    "&Delta;",
    "Aimed against",
    ">", // "Better was"
    "<", // "Worse was"
    "Equivalent was",
    "RR",
    "N",
    "x", // "weak point"
    QString::fromUtf8("⊥"), // "endgame"
    "&hArr;", // "line"
    QString::fromUtf8("⇗"), // "diagonal",
    QString::fromUtf8("☍"), // "pair of bishops"
    QString::fromUtf8("☍"), // "pair of bishops"
    QString::fromUtf8("❐"), // "bishops of opposite color"
    QString::fromUtf8("▞"), // "bishops of the same color"
    0,0,0,0,0,0, // 160
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //180
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //200
    "" // Diagram is displayed graphically in HTML
};

const QString& NagSet::nagToString(Nag nag)
{
	return g_nagStringList[nag];
}

const QString& NagSet::nagToStringHTML(Nag nag)
{
    return g_nagStringListHTML[nag];
}

QString NagSet::nagToMenuString(Nag nag)
{
    if (NagSet::g_nagStringListLong.count() == 0)
    {
        InitNagStringListLong();
    }
    if (g_nagStringList[nag].length() <= 2)
    {
        return QString("%1 (%2)").arg(g_nagStringList[nag]).arg(g_nagStringListLong[nag]);
    }
    else
    {
        return NagSet::nagToString(nag);
    }
}

Nag NagSet::fromString(const QString &nag)
{
    for (int i = 1; i < NagCount; ++i)
		if (g_nagStringList[i] == nag)
			return Nag(i);
	return NullNag;
}

int NagSet::prefixCount(const QString &nag)
{
	QSet<QString> matches;
    for (int i = 1; i < NagCount; ++i)
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

