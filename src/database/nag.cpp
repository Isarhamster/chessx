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

void NagSet::addNag(Nag nag)
{
	if (nag != NullNag) push_back(nag);
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

	for (unsigned i = 0; i < count(); i++) {
		switch (this->at(i)) {
		case GoodMove:
		case PoorMove:
		case VeryGoodMove:
		case VeryPoorMove:
		case SpeculativeMove:
		case QuestionableMove:
		case ForcedMove:
		case SingularMove:
		case WorstMove:
			moveNag = format == PGN ? "$" + QString::number(at(i)) : nagToString(at(i));
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
			evaluationNag = format == PGN ? "$" + QString::number(at(i)) : nagToString(at(i));
			break;

		default:
			otherNags = format == PGN ? "$" + QString::number(at(i)) : nagToString(at(i));
			break;
		}
	}

	return moveNag + evaluationNag + otherNags.simplified();
}

static const QString g_nagStringList[NagCount] = {
	"",
	"!",
	"?",
	"!!",
	"??",
	"!?",
	"?!",
	"forced move",
	"singular move",
	"??",
	"=",
	"=, quiet position",
	"=, active position",
	"~",
	"+=",
	"=+",
	"+/-",
	"-/+",
	"+-",
	"-+",
	"+-",
	"-+",
	"White is in zugzwang",
	"Black is in zugzwang",
	"White has a slight space advantage",
	"Black has a slight space advantage",
	"White has a moderate space advantage",
	"Black has a moderate space advantage",
	"White has a decisive space advantage",
	"Black has a decisive space advantage",
	"White has a slight time (development) advantage",
	"Black has a slight time (development) advantage",
	"White has a moderate time (development) advantage",
	"Black has a moderate time (development) advantage",
	"White has a decisive time (development) advantage",
	"Black has a decisive time (development) advantage",
	"White has the initiative",
	"Black has the initiative",
	"White has a lasting initiative",
	"Black has a lasting initiative",
	"White has the attack",
	"Black has the attack",
	"White has insufficient compensation for material deficit",
	"Black has insufficient compensation for material deficit",
	"~=",
	"~=",
	"White has more than adequate compensation for material deficit",
	"Black has more than adequate compensation for material deficit",
	"White has a slight center control advantage",
	"Black has a slight center control advantage",
	"White has a moderate center control advantage",
	"Black has a moderate center control advantage",
	"White has a decisive center control advantage",
	"Black has a decisive center control advantage",
	"White has a slight kingside control advantage",
	"Black has a slight kingside control advantage",
	"White has a moderate kingside control advantage",
	"Black has a moderate kingside control advantage",
	"White has a decisive kingside control advantage",
	"Black has a decisive kingside control advantage",
	"White has a slight queenside control advantage",
	"Black has a slight queenside control advantage",
	"White has a moderate queenside control advantage",
	"Black has a moderate queenside control advantage",
	"White has a decisive queenside control advantage",
	"Black has a decisive queenside control advantage",
	"White has a vulnerable first rank",
	"Black has a vulnerable first rank",
	"White has a well protected first rank",
	"Black has a well protected first rank",
	"White has a poorly protected king",
	"Black has a poorly protected king",
	"White has a well protected king",
	"Black has a well protected king",
	"White has a poorly placed king",
	"Black has a poorly placed king",
	"White has a well placed king",
	"Black has a well placed king",
	"White has a very weak pawn structure",
	"Black has a very weak pawn structure",
	"White has a moderately weak pawn structure",
	"Black has a moderately weak pawn structure",
	"White has a moderately strong pawn structure",
	"Black has a moderately strong pawn structure",
	"White has a very strong pawn structure",
	"Black has a very strong pawn structure",
	"White has poor knight placement",
	"Black has poor knight placement",
	"White has good knight placement",
	"Black has good knight placement",
	"White has poor bishop placement",
	"Black has poor bishop placement",
	"White has good bishop placement",
	"Black has good bishop placement",
	"White has poor rook placement",
	"Black has poor rook placement",
	"White has good rook placement",
	"Black has good rook placement",
	"White has poor queen placement",
	"Black has poor queen placement",
	"White has good queen placement",
	"Black has good queen placement",
	"White has poor piece coordination",
	"Black has poor piece coordination",
	"White has good piece coordination",
	"Black has good piece coordination",
	"White has played the opening very poorly",
	"Black has played the opening very poorly",
	"White has played the opening poorly",
	"Black has played the opening poorly",
	"White has played the opening well",
	"Black has played the opening well",
	"White has played the opening very well",
	"Black has played the opening very well",
	"White has played the middlegame very poorly",
	"Black has played the middlegame very poorly",
	"White has played the middlegame poorly",
	"Black has played the middlegame poorly",
	"White has played the middlegame well",
	"Black has played the middlegame well",
	"White has played the middlegame very well",
	"Black has played the middlegame very well",
	"White has played the ending very poorly",
	"Black has played the ending very poorly",
	"White has played the ending poorly",
	"Black has played the ending poorly",
	"White has played the ending well",
	"Black has played the ending well",
	"White has played the ending very well",
	"Black has played the ending very well",
	"White has slight counterplay",
	"Black has slight counterplay",
	"White has moderate counterplay",
	"Black has moderate counterplay",
	"White has decisive counterplay",
	"Black has decisive counterplay",
	"White has moderate time control pressure",
	"Black has moderate time control pressure",
	"White has severe time control pressure",
	"Black has severe time control pressure",
	"with the idea",
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
	"White has a pair of bishops",
	"Black has a pair of bishops",
	"Bishops of opposite color",
	"Bishops of same color"
};

const QString& NagSet::nagToString(Nag nag)
{
	return g_nagStringList[nag];
}

