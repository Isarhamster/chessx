/***************************************************************************
                          nag.h - NAG definitions and NagSet class
                             -------------------
    begin                : 14 April 2006
    copyright            : (C) 2006 William Hoggarth
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

#ifndef __NAG_H__
#define __NAG_H__

#include <QString>
#include <QVector>

/* Don't forget to add string for each 'nag' in source file. */
typedef enum
{
	NullNag,
	GoodMove,
	PoorMove,
	VeryGoodMove,
	VeryPoorMove,
	SpeculativeMove,
	QuestionableMove,
	ForcedMove,
	SingularMove,
	WorstMove,
	DrawishPosition,
	EqualChancesQuietPosition,
	EqualChancesActivePosition,
	UnclearPosition,
	WhiteHasASlightAdvantage,
	BlackHasASlightAdvantage,
	WhiteHasAModerateAdvantage,
	BlackHasAModerateAdvantage,
	WhiteHasADecisiveAdvantage,
	BlackHasADecisiveAdvantage,
	WhiteHasACrushingAdvantage,
	BlackHasACrushingAdvantage,
	WhiteIsInZugzwang,
	BlackIsInZugzwang,
	WhiteHasASlightSpaceAdvantage,
	BlackHasASlightSpaceAdvantage,
	WhiteHasAModerateSpaceAdvantage,
	BlackHasAModerateSpaceAdvantage,
	WhiteHasADecisiveSpaceAdvantage,
	BlackHasADecisiveSpaceAdvantage,
	WhiteHasASlightTimeAdvantage,
	BlackHasASlightTimeAdvantage,
	WhiteHasAModerateTimeAdvantage,
	BlackHasAModerateTimeAdvantage,
	WhiteHasADecisiveTimeAdvantage,
	BlackHasADecisiveTimeAdvantage,
	WhiteHasTheInitiative,
	BlackHasTheInitiative,
	WhiteHasALastingInitiative,
	BlackHasALastingInitiative,
	WhiteHasTheAttack,
	BlackHasTheAttack,
	WhiteHasInsufficientCompensationForMaterialDeficit,
	BlackHasInsufficientCompensationForMaterialDeficit,
	WhiteHasSufficientCompensationForMaterialDeficit,
	BlackHasSufficientCompensationForMaterialDeficit,
	WhiteHasMoreThanAdequateCompensationForMaterialDeficit,
	BlackHasMoreThanAdequateCompensationForMaterialDeficit,
	WhiteHasASlightCenterControlAdvantage,
	BlackHasASlightCenterControlAdvantage,
	WhiteHasAModerateCenterControlAdvantage,
	BlackHasAModerateCenterControlAdvantage,
	WhiteHasADecisiveCenterControlAdvantage,
	BlackHasADecisiveCenterControlAdvantage,
	WhiteHasASlightKingsideControlAdvantage,
	BlackHasASlightKingsideControlAdvantage,
	WhiteHasAModerateKingsideControlAdvantage,
	BlackHasAModerateKingsideControlAdvantage,
	WhiteHasADecisiveKingsideControlAdvantage,
	BlackHasADecisiveKingsideControlAdvantage,
	WhiteHasASlightQueensideControlAdvantage,
	BlackHasASlightQueensideControlAdvantage,
	WhiteHasAModerateQueensideControlAdvantage,
	BlackHasAModerateQueensideControlAdvantage,
	WhiteHasADecisiveQueensideControlAdvantage,
	BlackHasADecisiveQueensideControlAdvantage,
	WhiteHasAVulnerableFirstRank,
	BlackHasAVulnerableFirstRank,
	WhiteHasAWellProtectedFirstRank,
	BlackHasAWellProtectedFirstRank,
	WhiteHasAPoorlyProtectedKing,
	BlackHasAPoorlyProtectedKing,
	WhiteHasAWellProtectedKing,
	BlackHasAWellProtectedKing,
	WhiteHasAPoorlyPlacedKing,
	BlackHasAPoorlyPlacedKing,
	WhiteHasAWellPlacedKing,
	BlackHasAWellPlacedKing,
	WhiteHasAVeryWeakPawnStructure,
	BlackHasAVeryWeakPawnStructure,
	WhiteHasAModeratelyWeakPawnStructure,
	BlackHasAModeratelyWeakPawnStructure,
	WhiteHasAModeratelyStrongPawnStructure,
	BlackHasAModeratelyStrongPawnStructure,
	WhiteHasAVeryStrongPawnStructure,
	BlackHasAVeryStrongPawnStructure,
	WhiteHasPoorKnightPlacement,
	BlackHasPoorKnightPlacement,
	WhiteHasGoodKnightPlacement,
	BlackHasGoodKnightPlacement,
	WhiteHasPoorBishopPlacement,
	BlackHasPoorBishopPlacement,
	WhiteHasGoodBishopPlacement,
	BlackHasGoodBishopPlacement,
	WhiteHasPoorRookPlacement,
	BlackHasPoorRookPlacement,
	WhiteHasGoodRookPlacement,
	BlackHasGoodRookPlacement,
	WhiteHasPoorQueenPlacement,
	BlackHasPoorQueenPlacement,
	WhiteHasGoodQueenPlacement,
	BlackHasGoodQueenPlacement,
	WhiteHasPoorPieceCoordination,
	BlackHasPoorPieceCoordination,
	WhiteHasGoodPieceCoordination,
	BlackHasGoodPieceCoordination,
	WhiteHasPlayedTheOpeningVeryPoorly,
	BlackHasPlayedTheOpeningVeryPoorly,
	WhiteHasPlayedTheOpeningPoorly,
	BlackHasPlayedTheOpeningPoorly,
	WhiteHasPlayedTheOpeningWell,
	BlackHasPlayedTheOpeningWell,
	WhiteHasPlayedTheOpeningVeryWell,
	BlackHasPlayedTheOpeningVeryWell,
	WhiteHasPlayedTheMiddlegameVeryPoorly,
	BlackHasPlayedTheMiddlegameVeryPoorly,
	WhiteHasPlayedTheMiddlegamePoorly,
	BlackHasPlayedTheMiddlegamePoorly,
	WhiteHasPlayedTheMiddlegameWell,
	BlackHasPlayedTheMiddlegameWell,
	WhiteHasPlayedTheMiddlegameVeryWell,
	BlackHasPlayedTheMiddlegameVeryWell,
	WhiteHasPlayedTheEndingVeryPoorly,
	BlackHasPlayedTheEndingVeryPoorly,
	WhiteHasPlayedTheEndingPoorly,
	BlackHasPlayedTheEndingPoorly,
	WhiteHasPlayedTheEndingWell,
	BlackHasPlayedTheEndingWell,
	WhiteHasPlayedTheEndingVeryWell,
	BlackHasPlayedTheEndingVeryWell,
	WhiteHasSlightCounterplay,
	BlackHasSlightCounterplay,
	WhiteHasModerateCounterplay,
	BlackHasModerateCounterplay,
	WhiteHasDecisiveCounterplay,
	BlackHasDecisiveCounterplay,
	WhiteHasModerateTimeControlPressure,
	BlackHasModerateTimeControlPressure,
	WhiteHasSevereTimeControlPressure,
	BlackHasSevereTimeControlPressure,
	WithTheIdea,
	AimedAgainst,
	BetterMove,
	WorseMove,
	EquivalentMove,
	EditorsRemark,
	Novelty,
	WeakPoint,
	EndGame,
	Line,
	Diagonal,
	WhiteHasAPairOfBishops,
	BlackHasAPairOfBishops,
	BishopsOfOppositeColor,
	BishopsOfSameColor,
	NagCount
} Nag;

/** @ingroup Database
The NagSet class stores set of PGN NAGs.
*/
class NagSet : public QVector<Nag>
{
public:
    enum { Simple, PGN }; /**< format of a string */

    NagSet() {}
    NagSet(Nag nag) { addNag(nag); }

    void addNag(Nag nag);
    void removeNag(Nag nag);
	QString toString(unsigned format = Simple) const;
	static const QString& nagToString(Nag nag);
};

#endif	// __NAG_H__

