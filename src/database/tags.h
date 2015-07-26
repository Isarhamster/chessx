/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef TAGS_H
#define TAGS_H

#include <QString>

const char* const TagNameFEN         = "FEN";
const char* const TagNameECO         = "ECO";
const char* const TagNameSetUp       = "Setup";
const char* const TagNameSource      = "Source";
const char* const TagNameResult      = "Result";
const char* const TagNameWhite       = "White";
const char* const TagNameBlack       = "Black";
const char* const TagNameEvent       = "Event";
const char* const TagNameSite        = "Site";
const char* const TagNameRound       = "Round";
const char* const TagNameDate        = "Date";
const char* const TagNameEventDate   = "EventDate";
const char* const TagNamePlyCount    = "PlyCount";
const char* const TagNameWhiteClock  = "WhiteClock";
const char* const TagNameBlackClock  = "BlackClock";
const char* const TagNameWhiteElo    = "WhiteElo";
const char* const TagNameBlackElo    = "BlackElo";
const char* const TagNameWhiteTeam   = "WhiteTeam";
const char* const TagNameBlackTeam   = "BlackTeam";
const char* const TagNameTimeControl = "TimeControl";
const char* const TagNameLength      = "Length";
const char* const TagNameVariant     = "Variant";

const char* const StandardTags[7] = {"Event", "Site", "Date", "Round", "White", "Black", "Result"};

bool isStandardTag(QString tag);

#endif // TAGS_H

