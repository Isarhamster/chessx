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
	"with slight space advantage",
	"with slight space advantage",
	"with moderate space advantage",
	"with moderate space advantage",
	"with decisive space advantage",
	"with decisive space advantage",
	"with slight development advantage",
	"with slight development advantage",
    QString::fromUtf8("⟳"), // "with moderate development advantage"
    QString::fromUtf8("⟳"), // "with moderate development advantage"
	"with decisive development advantage",
	"with decisive development advantage",
    QString::fromUtf8("→"), // "with initiative"
    QString::fromUtf8("→"), // "with initiative"
    "with lasting initiative",
    "with lasting initiative",
    QString::fromUtf8("↑"), // "with attack"
    QString::fromUtf8("↑"), // "with attack"
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
    QString::fromUtf8("⊕"), // "Severe time control pressure"
    QString::fromUtf8("⊕"), // "Severe time control pressure"
    QString::fromUtf8("△"),
    "aimed against",
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

static const QString g_nagStringListLong[NagCount] = {
    "",
    "Good Move",
    "Poor Move",
    "Very good Move",
    "Blunder",
    "Speculative Move",
    "Dubious Move",
    "Only Move",
    "Only Move",
    "Blunder",
    "Equal",
    "Equal",
    "Equal",
    "Unclear",
    "White has slight Advantage",
    "Black has slight Advantage",
    "White has moderate Advantage",
    "Black has moderate Advantage",
    "White has decisive Advantage",
    "Black has decisive Advantage",
    "White has crushing Advantage",
    "Black has crushing Advantage",
    "Zugzwang",
    "Zugzwang",
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
    "with compensation",
    "with compensation",
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
    "with counterplay",
    "with counterplay",
    "with counterplay",
    "with counterplay",
    "with counterplay",
    "with counterplay",
    "Moderate time control pressure",
    "Moderate time control pressure",
    "Severe time control pressure",
    "Severe time control pressure",
    "with the Idea",
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
    "bishops of the same color",
    0,0,0,0,0,0, // 160
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //180
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //200
    "Diagram"
};

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
    "with slight space advantage",
    "with slight space advantage",
    "with moderate space advantage",
    "with moderate space advantage",
    "with decisive space advantage",
    "with decisive space advantage",
    "with slight development advantage",
    "with slight development advantage",
    QString::fromUtf8("⟳"), // "with moderate development advantage"
    QString::fromUtf8("⟳"), // "with moderate development advantage"
    "with decisive development advantage",
    "with decisive development advantage",
    "&rarr;", // "with initiative"
    "&rarr;", // "with initiative"
    "with lasting initiative",
    "with lasting initiative",
    "&uarr;", // "with attack"
    "&uarr;", // "with attack"
    "with insufficient compensation for material deficit",
    "with insufficient compensation for material deficit",
    "&prop;=",
    "&prop;=",
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
    "&oplus;", // "Severe time control pressure"
    "&oplus;", // "Severe time control pressure"
    "&Delta;",
    "aimed against",
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
    "#"
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

