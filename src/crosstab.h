//////////////////////////////////////////////////////////////////////
//
//  FILE:       crosstab.h
//              Crosstable class
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.6
//
//  Notice:     Copyright (c) 2000-2004 Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#ifndef WINCE

#ifndef SCID_CROSSTAB_H
#define SCID_CROSSTAB_H

#include "common.h"
#include "date.h"
#include "spellchk.h"

class DString;

const uint CROSSTABLE_MaxPlayers = 500;  // Max. number of players.
const uint CROSSTABLE_MaxRounds =   60;  // Max. number of Swiss event rounds.

struct clashT
{
    resultT  result;
    uint     gameNum;
    uint     opponent;  // For swiss crosstables.
    colorT   color;
    uint     round;
    clashT * next;
};

enum crosstableSortT
{
    CROSSTABLE_SortName, CROSSTABLE_SortElo, CROSSTABLE_SortScore, CROSSTABLE_SortCountry
};

enum crosstableOutputT
{
    CROSSTABLE_Plain, CROSSTABLE_Hypertext, CROSSTABLE_Html, CROSSTABLE_LaTeX
};

enum crosstableModeT
{
    CROSSTABLE_AllPlayAll, CROSSTABLE_Swiss, CROSSTABLE_Knockout,
    CROSSTABLE_Auto
};

struct playerDataT
{
    idNumberT   id;
    char *      name;
    eloT        elo;
    uint        score;   // Score, stored as 2 pts per win, 1 pt per draw.
    uint        gameCount;
    uint        tiebreak; // Sonneborn-Berger tiebreak for all-play-all,
                          // or Bucholz tiebreak for Swiss.
    uint        oppEloCount;
    uint        oppEloTotal;
    uint        oppEloScore;  // score against Elo opponents
    clashT *    firstClash [CROSSTABLE_MaxPlayers];
    clashT *    lastClash [CROSSTABLE_MaxPlayers];
    uint        clashCount[CROSSTABLE_MaxPlayers];
    clashT *    roundClash [CROSSTABLE_MaxRounds];
    char        title [8];
    char        country [8];
    dateT       birthdate;
    int         ageInYears;
    bool        printed;
};


class Crosstable
{
  private:

    uint         PlayerCount;
    uint         GameCount;
    uint         MaxClashes;  // Maximum games between any two players
    uint         MaxRound;
    uint         ResultCount [NUM_RESULT_TYPES];
    dateT        FirstDate;

    bool         ShowTitles;
    bool         ShowElos;
    bool         ShowCountries;
    bool         ShowFlags;
    bool         ShowTallies;
    bool         ShowAges;
    bool         ShowTiebreaks;
    bool         SwissColors;  // If true, show colors in Swiss tables.
    bool         SeparateScoreGroups;
    char         DecimalPointChar;
    bool         APAColumnNums;  // If true, print numbers instead of partial
                                 // names above all-play-all table columns.

    crosstableOutputT OutputFormat;
    crosstableSortT SortOption;
    bool         ThreeWin;

    playerDataT * PlayerData [CROSSTABLE_MaxPlayers];

    // The following fields are set in PrintTable() and then used in
    // each of the private Print* methods:
    bool         PrintTitles;
    bool         PrintRatings;
    bool         PrintCountries;
    bool         PrintFlags;
    bool         PrintTallies;
    bool         PrintAges;
    bool         PrintTiebreaks;
    const char * StartTable;
    const char * EndTable;
    const char * StartRow;
    const char * EndRow;
    const char * NewLine;
    const char * BlankRowLine;
    const char * StartCol;
    const char * EndCol;
    const char * StartRightCol;
    const char * EndRightCol;
    const char * StartBoldCol;
    const char * EndBoldCol;
    uint         LongestNameLen;
    uint         LineWidth;
    uint         PlayerNumWidth;
    uint         SortedIndex [CROSSTABLE_MaxPlayers];
    uint         InvertedIndex [CROSSTABLE_MaxPlayers];
    uint         CurrentGame;

    void   Tiebreaks (crosstableModeT mode);

    void   PrintDashesLine (DString * dstr);
    void   PrintPlayer (DString * dstr, playerDataT * pdata);
    void   PrintPerformance (DString * dstr, playerDataT * pdata);
    void   PrintScorePercentage (DString * dstr, playerDataT * pdata);
    void   PrintAllPlayAll (DString * dstr, uint playerLimit);
    void   PrintKnockout (DString * dstr, uint playerLimit);
    void   PrintSwiss (DString * dstr, uint playerLimit);

    void   Init();
    void   Destroy();

  public:
    Crosstable() { Init(); }
    ~Crosstable() { Destroy(); }

    void   SetOutputFormat (crosstableOutputT opt) { OutputFormat = opt; }
    void   SetPlainOutput()     { OutputFormat = CROSSTABLE_Plain; }
    void   SetHtmlOutput()      { OutputFormat = CROSSTABLE_Html; }
    void   SetHypertextOutput() { OutputFormat = CROSSTABLE_Hypertext; }
    void   SetLaTeXOutput()     { OutputFormat = CROSSTABLE_LaTeX; }

    void   SetSortOption (crosstableSortT option) { SortOption = option; }
    void   SetThreeWin   (bool threewin) { ThreeWin = threewin; }

    void   SortByName()  { SortOption = CROSSTABLE_SortName; }
    void   SortByElo()   { SortOption = CROSSTABLE_SortElo; }
    void   SortByScore() { SortOption = CROSSTABLE_SortScore; }
    void   SortByCountry() { SortOption = CROSSTABLE_SortCountry; }

    void   SetAges (bool b) {ShowAges = b; }
    void   SetTitles (bool b) { ShowTitles = b; }
    void   SetElos (bool b) { ShowElos = b; }
    void   SetCountries (bool b) { ShowCountries = b; }
    void   SetFlags (bool b) { ShowFlags = b; }
    void   SetTallies (bool b) { ShowTallies = b; }
    void   SetTiebreaks (bool b) { ShowTiebreaks = b; }
    void   SetSwissColors (bool b) { SwissColors = b; }
    void   SetSeparateScoreGroups (bool b) { SeparateScoreGroups = b; }
    void   SetDecimalPointChar (char ch) { DecimalPointChar = ch; }
    void   SetNumberedColumns (bool b) { APAColumnNums = b; }

    uint   NumPlayers() { return PlayerCount; }
    errorT AddPlayer (idNumberT id, const char * name, eloT elo, const SpellChecker*);
    errorT AddResult (uint gameNumber, idNumberT white, idNumberT black,
                      resultT result, uint round, dateT date);

    crosstableModeT BestMode (void);
    eloT   AvgRating();
    void   PrintTable (DString * dstr, crosstableModeT mode, uint playerLimit, int currentGame);

    static uint Performance (uint oppAvg, uint percentage);
    static uint FideCategory (eloT rating);
    static eloT OpponentElo (eloT player, eloT opponent);
    static int RatingChange (eloT player, uint oppAvg, uint percentage, 
                             uint count);
};

#endif  // #ifndef SCID_CROSSTAB_H

#endif // WINCE
