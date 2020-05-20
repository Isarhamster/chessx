//////////////////////////////////////////////////////////////////////
//
//  FILE:       crosstab.cpp
//              Crosstable class methods
//
//  Part of:    Scid (Shane's Chess Information Database)
//  Version:    3.3
//
//  Notice:     Copyright (c) 2001  Shane Hudson.  All rights reserved.
//
//  Author:     Shane Hudson (sgh@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#ifndef WINCE

#include "crosstab.h"
#include "dstring.h"

// Expected differences in rating according to performance
// from 50% to 100%:
const uint perf_elodiff [51] = {
    /* 50 - 59 */   0,   7,  14,  21,  29,  36,  43,  50,  57,  65,
    /* 60 - 69 */  72,  80,  87,  95, 102, 110, 117, 125, 133, 141,
    /* 70 - 79 */ 149, 158, 166, 175, 184, 193, 202, 211, 220, 230,
    /* 80 - 89 */ 240, 251, 262, 273, 284, 296, 309, 322, 336, 351,
    /* 90 - 99 */ 366, 383, 401, 422, 444, 470, 501, 538, 589, 677,
    /*   100   */ 999
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Crosstable::Performance():
//    Given an average of opponents ratings and a percentage score,
//    returns the performance rating.
uint
Crosstable::Performance (uint oppAvg, uint percentage)
{
    if (percentage > 100) { percentage = 100; }
    uint performance = oppAvg;
    if (percentage < 50) {
        performance -= perf_elodiff [50 - percentage];
    } else {
        performance += perf_elodiff [percentage - 50];
    }
    return performance;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Crosstable::RatingChange():
//    Calculates rating change, given current rating, average rating
//    and score
int 
Crosstable::RatingChange (eloT player, uint oppAvg, uint percentage, uint games)
{
   uint diff = (player > oppAvg) ? player - oppAvg : oppAvg - player;
   int i;
   for (i=0; i<50 ; i++)
       if (diff <= perf_elodiff[i])
           break;
   uint expected = i;
   if (player > oppAvg) 
       expected += 50;
   else 
       expected = 50 - expected;
   int cutoff = (percentage > expected) ? 5 : -5;
   return (((int)percentage - (int)expected) * (int)games + cutoff) / 10;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Crosstable::FideCategory()
//    Given an average Elo rating for an all-play-all tournament,
//    returns the FIDE Category of the tournament.
//    Ratings under 2251 have no category.
//    2251-2275 = Cat. 1, 2276-2300 = Cat. 2, etc in blocks of 25.
uint
Crosstable::FideCategory (eloT rating)
{
    if (rating <= 2250) { return 0; }
    return 1 + ((rating - 2251) / 25);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Crosstable::OpponentElo():
//      Strips ELO rating if difference is bigger than 350
eloT 
Crosstable::OpponentElo (eloT player, eloT opponent)
{
   const eloT Margin = 350;
   if (!player)
       return opponent;
   else if (player - opponent > Margin)
       return player - Margin;
   else if (opponent - player > Margin)
       return player + Margin;
   else 
       return opponent;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// comparePlayerData():
//      Compares two playerDataT structs based on their tournament score.
int
comparePlayerData (playerDataT * p1, playerDataT * p2, crosstableSortT option)
{
    int result = 0;
    switch (option) {
    case CROSSTABLE_SortScore:  // Sort by highest score, then fewest games:
        result = p2->score - p1->score;
        if (result == 0) { result = p1->gameCount - p2->gameCount; }
        if (result == 0) { result = p2->tiebreak - p1->tiebreak; }
        break;

    case CROSSTABLE_SortName:
        result = strCompare(p1->name, p2->name);
        break;

    case CROSSTABLE_SortElo:
        result = p2->elo - p1->elo;
        break;

    case CROSSTABLE_SortCountry:
        result = strCompare(p1->country, p2->country);
        break;
    }
    return result;
}


void
Crosstable::Init ()
{
    for (uint pcount=0; pcount < CROSSTABLE_MaxPlayers; pcount++) {
        PlayerData[pcount] = NULL;
    }
    GameCount = 0;
    PlayerCount = 0;
    MaxClashes = 0;
    MaxRound = 0;
    FirstDate = ZERO_DATE;
    for (resultT r = 0; r < NUM_RESULT_TYPES; r++) { ResultCount[r] = 0; }
    ShowTitles = ShowElos = ShowFlags = ShowCountries = ShowTallies = SwissColors = ShowAges = true;
    ShowTiebreaks = false;
    SortOption = CROSSTABLE_SortScore;
    OutputFormat = CROSSTABLE_Plain;
    DecimalPointChar = '.';
    APAColumnNums = false;
}

void
Crosstable::Destroy ()
{
    for (uint player=0; player < PlayerCount; player++) {
        playerDataT * pdata = PlayerData[player];
        ASSERT (pdata != NULL);
#ifdef WINCE
        my_Tcl_Free((char*)pdata->name);
#else
        delete[] pdata->name;
#endif
        for (uint opp = 0; opp < PlayerCount; opp++) {
            clashT * clash = pdata->firstClash[opp];
            while (clash != NULL) {
                clashT * temp = clash->next;
#ifdef WINCE
        my_Tcl_Free((char*)clash);
#else
                delete clash;
#endif
                clash = temp;
            }
        }

#ifdef WINCE
        my_Tcl_Free((char*)pdata);
#else
        delete pdata;
#endif
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Crosstable::AddPlayer()
//      Adds a player to the crosstable, if that player is not
//      already listed.
errorT
Crosstable::AddPlayer (idNumberT id, const char * name, eloT elo, const SpellChecker* SpellCheck)
{
    for (uint i = 0; i < PlayerCount; i++) {
        if (PlayerData[i]->id == id) {
            // The player already exists in the crosstable, but
            // check the elo rating and keep the largest value:
            if (elo > PlayerData[i]->elo) { PlayerData[i]->elo = elo; }
            return OK;
        }
    }
    if (PlayerCount == CROSSTABLE_MaxPlayers) { return ERROR_Full; }
    playerDataT * pdata = new playerDataT;

    PlayerData[PlayerCount] = pdata;
    pdata->id = id;
    pdata->name = strDuplicate (name);
    pdata->elo = elo;
    pdata->score = 0;
    pdata->gameCount = 0;
    pdata->tiebreak = 0;
    pdata->oppEloCount = 0;
    pdata->oppEloTotal = 0;
    pdata->oppEloScore = 0;
    pdata->title[0] = 0;
    pdata->country[0] = 0;
    pdata->birthdate = ZERO_DATE;
    pdata->ageInYears = 0;
    for (uint opp = 0; opp < CROSSTABLE_MaxPlayers; opp++) {
        pdata->firstClash[opp] = pdata->lastClash[opp] = NULL;
        pdata->clashCount[opp] = 0;
    }
    for (uint round = 1; round < CROSSTABLE_MaxRounds; round++) {
        pdata->roundClash[round] = NULL;
    }

    if (SpellCheck != NULL ) {
        const PlayerInfo* pInfo = SpellCheck->getPlayerInfo(name);
        if (pInfo != NULL) {
            strCopy (pdata->title, pInfo->getTitle());
            strCopy (pdata->country, pInfo->getLastCountry());
            pdata->birthdate = pInfo->getBirthdate();
            if (strEqual (pdata->title, "w")) { strCopy (pdata->title, "w  "); }
        }
    }
    PlayerCount++;
    return OK;
}


uint max(int a, int b) {return a<b ? b : a;}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Crosstable::AddResult()
//      Adds a game result to the crosstable.
errorT
Crosstable::AddResult (uint gameNumber, idNumberT white, idNumberT black,
                       resultT result, uint round, dateT date)
{
    // Find the two players in the player data:
    int whiteIdx = -1;
    int blackIdx = -1;
    uint i;
    for (i=0; i < PlayerCount; i++) {
        if (PlayerData[i]->id == white) { whiteIdx = i; break; }
    }
    for (i=0; i < PlayerCount; i++) {
        if (PlayerData[i]->id == black) { blackIdx = i; break; }
    }

    // Both players must exist in the crosstable:
    if (whiteIdx < 0  ||  blackIdx < 0) { return ERROR_NotFound; }

    // The two players must actually be different:
    if (whiteIdx == blackIdx) { return ERROR_Corrupt; }

    playerDataT * pwhite = PlayerData[whiteIdx];
    playerDataT * pblack = PlayerData[blackIdx];

    // The number of prior encounters must be consistent:
    ASSERT (pwhite->clashCount[blackIdx] == pblack->clashCount[whiteIdx]);
    clashT * whiteClash = new clashT;

    if (pwhite->firstClash[blackIdx] == NULL) {  // New head of list:
        pwhite->firstClash[blackIdx] = whiteClash;
    } else {
        pwhite->lastClash[blackIdx]->next = whiteClash;
    }
    whiteClash->next = NULL;
    pwhite->lastClash[blackIdx] = whiteClash;

    clashT * blackClash = new clashT;
    if (pblack->firstClash[whiteIdx] == NULL) { // New head of list:
        pblack->firstClash[whiteIdx] = blackClash;
    } else {
        pblack->lastClash[whiteIdx]->next = blackClash;
    }
    blackClash->next = NULL;
    pblack->lastClash[whiteIdx] = blackClash;

    whiteClash->result = result;
    blackClash->result = RESULT_OPPOSITE[result];
    whiteClash->gameNum = gameNumber;
    blackClash->gameNum = gameNumber;
    whiteClash->opponent = blackIdx;
    blackClash->opponent = whiteIdx;
    whiteClash->color = WHITE;
    blackClash->color = BLACK;
    whiteClash->round = round;
    blackClash->round = round;
    if (round > 0  &&  round < CROSSTABLE_MaxRounds) {
        pwhite->roundClash[round] = whiteClash;
        pblack->roundClash[round] = blackClash;
        if (round > MaxRound) { MaxRound = round; }
    }
    pwhite->clashCount[blackIdx]++;
    pblack->clashCount[whiteIdx]++;
    if (pwhite->clashCount[blackIdx] > MaxClashes) {
        MaxClashes = pwhite->clashCount[blackIdx];
    }
    pwhite->gameCount++;
    pblack->gameCount++;

    // Update averages of opponents ratings for performance stats:
    if (result && pblack->elo > 0) {
        pwhite->oppEloCount++;
        pwhite->oppEloTotal += OpponentElo(pwhite->elo, pblack->elo);
    }
    if (result && pwhite->elo > 0) {
        pblack->oppEloCount++;
        pblack->oppEloTotal += OpponentElo(pblack->elo, pwhite->elo);
    }

    if (FirstDate == ZERO_DATE) { FirstDate = date; }
    if (date != ZERO_DATE  &&  date < FirstDate) {
        FirstDate = date;
    }

    switch (result) {
    case RESULT_White:
        pwhite->score += (ThreeWin ? 6 : 2);
        if (pblack->elo > 0) {
            pwhite->oppEloScore += 2;
        }
        break;
    case RESULT_Black:
        pblack->score += (ThreeWin ? 6 : 2);
        if (pwhite->elo > 0) {
            pblack->oppEloScore += 2;
        }
        break;
    case RESULT_Draw:
        pwhite->score += (ThreeWin ? 2 : 1);
        pblack->score += (ThreeWin ? 2 : 1);
        if (pblack->elo > 0) {
            pwhite->oppEloScore ++;
        }
        if (pwhite->elo > 0) {
            pblack->oppEloScore ++;
        }
        break;
    default:
        break;  // Nothing.
    }
    ResultCount[result]++;
    GameCount++;
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Crosstable::Tiebreaks()
//      Computes the tiebreak score for all players.
void
Crosstable::Tiebreaks (crosstableModeT mode)
{
    uint player;
    for (player = 0; player < PlayerCount; player++) {
        playerDataT * pd = PlayerData[player];
        pd->tiebreak = 0;
        uint tb = 0;
        // Tiebreaks are meaningless for Knockout tables:
        if (mode == CROSSTABLE_Knockout) { continue; }

        for (uint opp = 0; opp < PlayerCount; opp++) {
            if (opp == player) { continue; }
            clashT * clash = pd->firstClash[opp];
            while (clash != NULL) {
                uint oppScore = PlayerData[opp]->score;
                if (mode == CROSSTABLE_Swiss) {
                    // For Swiss, just do sum of opponent scores:
                    tb += PlayerData[opp]->score;
                } else {
                    // AllPlayAll mode: do Sonneborn-Berger:
                    if (clash->result == RESULT_White) {
                        tb += oppScore + oppScore;
                    } else if (clash->result == RESULT_Draw) {
                        tb += oppScore;
                    }
                }
                clash = clash->next;
            }
        }
        pd->tiebreak = tb;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Crosstable::BestMode():
//    Returns the best mode of table to use for this tournament:
//    All-play-all, Swiss or Knockout.
//    XXX Currently never chooses Knockout, fix this sometime...
crosstableModeT
Crosstable::BestMode (void)
{
    // If 12 players of less, use all-play-all:
    if (PlayerCount <= 12) { return CROSSTABLE_AllPlayAll; }
    // If more than 30 players, use Swiss:
    if (PlayerCount > 30) { return CROSSTABLE_Swiss; }
    // If less than 5 games per player on average, use Swiss:
    if ((GameCount / PlayerCount) < 5) { return CROSSTABLE_Swiss; }
    // If less than half the number of games in a complete all-play-all
    // tournament, use Swiss:
    uint completeAllPlayAll = (PlayerCount * (PlayerCount - 1)) / 2;
    if (GameCount < completeAllPlayAll / 2) { return CROSSTABLE_Swiss; }
    // Otherwise, use all-play-all:
    return CROSSTABLE_AllPlayAll;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Crosstable::AvgRating()
//      Returns the average Elo rating of all players in the
//      tournament who have a rating. Players with no rating
//      are ignored.
eloT
Crosstable::AvgRating ()
{
    uint count = 0;
    uint total = 0;
    for (uint i=0; i < PlayerCount; i++) {
        if (PlayerData[i]->elo > 0) { total += PlayerData[i]->elo; count++; }
    }
    if (count == 0) { return 0; }
    return (eloT) (total / count);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Crosstable::PrintTable()
//      Prints the crosstable to a self-extending DString.
//      The format can be plain text or hypertext with player and game tags,
//      depending on the value of the OutputFormat member variable.
void
Crosstable::PrintTable (DString * dstr, crosstableModeT mode, uint playerLimit, int currentGame)
{
    CurrentGame = currentGame;
    if (playerLimit == 0  ||  playerLimit > PlayerCount) {
        playerLimit = PlayerCount;
    }
    if (playerLimit < 2) { return; }

    if (mode == CROSSTABLE_Auto) { mode = BestMode(); }

    // Sort the players by score, name or rating:
    Tiebreaks (mode);
    uint player;
    for (player=0; player < PlayerCount; player++) {
        SortedIndex[player] = player;
        InvertedIndex[player] = player;
    }
    for (uint first=0; first < PlayerCount-1; first++) {
        for (uint second = first+1; second < PlayerCount; second++) {
            if (comparePlayerData (PlayerData[SortedIndex[first]],
                                   PlayerData[SortedIndex[second]],
                                   SortOption) > 0) {
                uint temp = SortedIndex[first];
                SortedIndex[first] = SortedIndex[second];
                SortedIndex[second] = temp;
            }
        }
    }
    for (player=0; player < PlayerCount; player++) {
        InvertedIndex[SortedIndex[player]] = player;
    }

    PlayerNumWidth = 2;
    if (playerLimit > 99) { PlayerNumWidth = 3; }

    // Determine the longest player name:
    LongestNameLen = 0;
    for (player = 0; player < PlayerCount; player++) {
        uint len = strLength (PlayerData[player]->name);
        if (len > LongestNameLen) { LongestNameLen = len; }
    }

    // Determine if any players have elo rating, title, country:
    PrintRatings = false;
    PrintTitles = false;
    PrintCountries = false;
    PrintFlags = false;
    PrintAges = false;
    PrintTiebreaks = true;
    PrintTallies = true;
    for (player = 0; player < PlayerCount; player++) {
        playerDataT * pd = PlayerData[player];
        if (pd->elo > 0) { PrintRatings = true; }
        if (pd->title[0] != 0) { PrintTitles = true; }
        if (pd->country[0] != 0) { PrintCountries = true; PrintFlags = true; }
        if (pd->birthdate != ZERO_DATE) {
            PrintAges = true;
            int age = (int) date_GetYear(FirstDate);
            age -= (int) date_GetYear (pd->birthdate);
            if (date_GetMonth(pd->birthdate) > date_GetMonth(FirstDate)) {
                age--;
            }
            pd->ageInYears = age;
        }
    }
    if (! ShowElos) { PrintRatings = false; }
    if (! ShowTitles) { PrintTitles = false; }
    if (! ShowCountries) { PrintCountries = false; }
    if (! ShowFlags) { PrintFlags = false; }
    if (! ShowTallies) { PrintTallies = false; }
    if (! ShowAges) { PrintAges = false; }
    if (! ShowTiebreaks) { PrintTiebreaks = false; }
    if (mode == CROSSTABLE_Knockout) { PrintTiebreaks = false; }

    // Print the table header:
    StartTable = "";
    EndTable = "";
    StartRow = "";
    EndRow = "";
    NewLine = "\n";
    BlankRowLine = "\n";
    StartCol = "";
    EndCol = "";
    StartRightCol = "";
    EndRightCol = "";
    StartBoldCol = "";
    EndBoldCol = "";

    if (OutputFormat == CROSSTABLE_Hypertext) {
        NewLine = BlankRowLine = "<br>";
    }
    if (OutputFormat == CROSSTABLE_Html) {
        StartTable = "\n<p><table border=1 cellspacing=0 cellpadding=4>\n";
        EndTable = "</table></p>\n";
        StartRow = "<tr>";
        EndRow = "</tr>";
        NewLine = "\n";
        BlankRowLine = "<tr></tr>\n";
        StartCol = "<td>";
        EndCol = "</td>";
        StartRightCol = "<td align=right>";
        EndRightCol = "</td>";
        StartBoldCol = "<th>";
        EndBoldCol = "</th>";
    }
    if (OutputFormat == CROSSTABLE_LaTeX) {
        StartTable = "";
        EndTable = "\\end{tabular}\n\n";
        StartRow = "";
        EndRow = " \\\\";
        NewLine = "\n";
        BlankRowLine = "\\\\\n";
        StartCol = "";
        EndCol = " &";
        StartRightCol = "";
        EndRightCol = " &";
        StartBoldCol = "\\bf ";
        EndBoldCol = " &";
    }

    // LineWidth is used to determine length of line of dashes to print.
    LineWidth = LongestNameLen + 6;
    if (PrintRatings) { LineWidth += 16; }
    if (PrintTitles) { LineWidth += 4; }
    if (PrintCountries) { LineWidth += 4; }
    if (PrintFlags && OutputFormat == CROSSTABLE_Hypertext) { LineWidth += 3; }
    if (PrintAges) { LineWidth += 4; }

    if (mode == CROSSTABLE_Swiss) {
        LineWidth += 28 + PlayerNumWidth;
        LineWidth += (PlayerNumWidth + (SwissColors ? 3 : 2)) * MaxRound;
        if (PrintTiebreaks) { LineWidth += 5; }
    } else if (mode == CROSSTABLE_AllPlayAll) {
        LineWidth += 28 + PlayerNumWidth;
        if (playerLimit == 2) {
            LineWidth += (MaxClashes + 1);
        } else {
            LineWidth += (playerLimit * (MaxClashes + 1));
        }
        if (PrintTiebreaks) { LineWidth += 7; }
    } else { // Knockout
        LineWidth = (LongestNameLen * 2) + 17 + MaxClashes;
        if (PrintRatings) { LineWidth += 10; }
        if (PrintTitles) { LineWidth += 8; }
        if (PrintCountries) { LineWidth += 8; }
        if (PrintFlags) { LineWidth += 4; }
        if (PrintAges) { LineWidth += 8; }
    }

    switch (mode) {
    case CROSSTABLE_AllPlayAll:
        PrintAllPlayAll (dstr, playerLimit);
        break;
    case CROSSTABLE_Knockout:
        PrintKnockout (dstr, playerLimit);
        break;
    case CROSSTABLE_Swiss:
    case CROSSTABLE_Auto:
        PrintSwiss (dstr, playerLimit);
        break;
    }

    char stemp [100];
    if (GameCount > 1) {
        sprintf (stemp, "%u game%s: %s%u %s%u %s%u",
                 GameCount, strPlural (GameCount),
                 OutputFormat == CROSSTABLE_LaTeX ? "{\\tt +}" : "+",
                 ResultCount[RESULT_White],
                 OutputFormat == CROSSTABLE_LaTeX ? "{\\tt =}" : "=",
                 ResultCount[RESULT_Draw],
                 OutputFormat == CROSSTABLE_LaTeX ? "{\\tt -}" : "-",
                 ResultCount[RESULT_Black]);
        dstr->Append (stemp);
        if (ResultCount[RESULT_None] > 0) {
            sprintf (stemp, " %s%u", 
                     OutputFormat == CROSSTABLE_LaTeX ? "{\\tt *}" : "*",
                     ResultCount[RESULT_None]);
            dstr->Append (stemp);
        }
        dstr->Append (NewLine);
    }
    return;
}

void
Crosstable::PrintDashesLine (DString * dstr)
{
    // Print line of dashes, if not in HTML or LaTeX:
    if (OutputFormat == CROSSTABLE_LaTeX) {
        dstr->Append ("\\hline\n");
        return;
    }
    if (OutputFormat == CROSSTABLE_Html) {
        return;
    }
    for (uint i=0; i < LineWidth; i++) {
        dstr->AddChar ('-');
    }
    dstr->Append (NewLine);
}

void
Crosstable::PrintPlayer (DString * dstr, playerDataT * pdata)
{
    char stemp[1000];
    if (OutputFormat == CROSSTABLE_Hypertext) {
        sprintf (stemp, "<pi %s>", pdata->name);
        dstr->Append (stemp);
    }
    sprintf (stemp, "%-*s ", LongestNameLen, pdata->name);
    dstr->Append (StartCol, stemp, EndCol);

    if (PrintRatings) {
        if (pdata->elo) {
            sprintf (stemp, "%4u ", pdata->elo);
        } else {
            if (OutputFormat == CROSSTABLE_Html) {
                strcpy (stemp, "  -  ");
            } else {
                strcpy (stemp, "     ");
            }
        }
        dstr->Append (StartRightCol, stemp, EndRightCol);
    }

    // if exporting to html, don't print blank fields
    //   as firefox doesn't make a grid box for blanks S.A.

    if (PrintTitles) {
        if (OutputFormat == CROSSTABLE_Html && !strCompare(pdata->title,"")) {
            sprintf (stemp, " -  ");
        } else {
            sprintf (stemp, "%3s ", pdata->title);
        }
        dstr->Append (StartCol, stemp, EndCol);
    }
    if (PrintAges) {
        if (pdata->ageInYears == 0) {
            if (OutputFormat == CROSSTABLE_Html) {
                sprintf (stemp, " -  ");
            } else {
                strCopy (stemp, "    ");
            }
        } else {
            sprintf (stemp, "%3d ", pdata->ageInYears);
        }
        dstr->Append (StartCol, stemp, EndCol);
    }
    if (PrintCountries) {
        if (OutputFormat == CROSSTABLE_Html && !strCompare(pdata->country,"")) {
            sprintf (stemp, " -  ");
        } else {
            sprintf (stemp, "%-3s ", pdata->country);
        }
        dstr->Append (StartCol, stemp, EndCol);
    }
    if (PrintFlags && OutputFormat == CROSSTABLE_Hypertext) {
        sprintf (stemp, "<img flag_%s>",  pdata->country[0] ? pdata->country : "unkown");
        dstr->Append (StartCol, stemp, EndCol);
    }
    if (OutputFormat == CROSSTABLE_Hypertext) { dstr->Append ("</pi>"); }
}

void
Crosstable::PrintPerformance (DString * dstr, playerDataT * pdata)
{
    if (!PrintRatings) { return; }
    if (!pdata->oppEloCount) { return; }

    int oppAvgRating = pdata->oppEloTotal / pdata->oppEloCount;
    int percentage = pdata->oppEloScore * 50 + pdata->oppEloCount/2;
    percentage = percentage / pdata->oppEloCount;
    int performance = Performance(oppAvgRating, percentage);
    if (performance > 0  &&  performance < 5000) {
        char stemp [20];
        if (pdata->elo) {
            int change = RatingChange (pdata->elo, oppAvgRating, 
                                       percentage, pdata->oppEloCount);
            sprintf (stemp, "%4d %+3d", performance, change);
        } else {
            sprintf (stemp, "%4d    ", performance);
        }
        dstr->Append ("   ", StartRightCol, stemp, EndRightCol);
    }
}

// Calculate and format the percentage performance of the player 8 Points from 10 Games = 80%
void
Crosstable::PrintScorePercentage (DString * dstr, playerDataT * pdata)
{
    char stemp [20];
    uint per_score;

    per_score = ( pdata->gameCount > 0) ? pdata->score * 500 / pdata->gameCount : 0;
    sprintf (stemp, "%3d%c%1d%%", per_score / 10 , DecimalPointChar, per_score % 10);
    dstr->Append (" ", StartRightCol, stemp, EndRightCol);
}

void
Crosstable::PrintAllPlayAll (DString * dstr, uint playerLimit)
{
    char stemp [1000];
    uint player;

    dstr->Append (StartTable);
    if (OutputFormat == CROSSTABLE_LaTeX) {
        dstr->Append ("\\begin{tabular}{rl");
        if (PrintRatings) { dstr->Append ("r"); }
        if (PrintTitles) { dstr->Append ("r"); }
        if (PrintAges) { dstr->Append ("r"); }
        if (PrintCountries) { dstr->Append ("l"); }
        dstr->Append ("r@{ / }r");
        if (PrintTiebreaks) { dstr->Append ("r"); }
        for (uint i=0; i < playerLimit; i++) {
            dstr->Append ("c");
            if (i < playerLimit-1) { dstr->Append ("@{ }"); }
        }
        if (PrintRatings) { dstr->Append ("r"); }
        dstr->Append ("r}\n");
    }
    dstr->Append (StartRow);
    if (OutputFormat == CROSSTABLE_Html) {
        dstr->Append ("<th></th> <th>Player</th> ");
    } else if (OutputFormat == CROSSTABLE_LaTeX) {
        dstr->Append ("  & \\bf Player & ");
    } else {
        strPad (stemp, "", LongestNameLen + 2 + PlayerNumWidth, ' ');
        dstr->Append (stemp);
    }
    if (PrintRatings) {
        dstr->Append (StartBoldCol, " Rtng", EndBoldCol);
    }
    if (PrintTitles) {
        dstr->Append (StartBoldCol, "  Ti", EndBoldCol);
    }
    if (PrintAges) {
        dstr->Append (StartBoldCol, " Age", EndBoldCol);
    }
    if (PrintCountries) {
        dstr->Append (StartBoldCol, " Nat", EndBoldCol);
    }
    if (PrintFlags && OutputFormat == CROSSTABLE_Hypertext) {
        dstr->Append (StartBoldCol, " Nat ", EndBoldCol);
    }
    if (OutputFormat == CROSSTABLE_LaTeX) {
        // Todo : fix LateX Score column alignment with 3 points for win.
        dstr->Append (" \\multicolumn{2}{c}{\\bf Score} & ");
    } else {
        if (ThreeWin) 
            dstr->Append ("  ", StartBoldCol, "Score", EndBoldCol, " ");
        else if (PrintFlags && OutputFormat == CROSSTABLE_Hypertext)
            dstr->Append (" ", StartBoldCol, "Score  ", EndBoldCol, "   ");
        else
            dstr->Append ("  ", StartBoldCol, " Score  ", EndBoldCol, "   ");
    }
    if (PrintTiebreaks) {
        dstr->Append (StartBoldCol, " (Tie) ", EndBoldCol);
    }

    for (player = 0; player < playerLimit; player++) {
        strPad (stemp, PlayerData[SortedIndex[player]]->name, MaxClashes, ' ');
        if (APAColumnNums) {
            // Print numbers instead of names over columns:
            strPad (stemp, "", MaxClashes, ' ');
            uint pnum = player + 1;
            stemp[MaxClashes-1] = (pnum % 10) + '0';
            if (MaxClashes >= 2  &&  pnum >= 10) {
                stemp[MaxClashes-2] = ((pnum / 10) % 10) + '0';
            }
            if (MaxClashes >= 3  &&  pnum >= 100) {
                stemp[MaxClashes-3] = ((pnum / 10) % 10) + '0';
            }
        }
        if (playerLimit == 2) {
            // Make two-player crosstable look better:
            player = 1;
            strPad (stemp,
                    "12345678901234567890123456789012345678901234567890",
                    MaxClashes, ' ');
        }
        if (OutputFormat == CROSSTABLE_LaTeX) {
            dstr->Append (" \\tt ", stemp, " &");
        } else {
            dstr->Append (" ", StartBoldCol, stemp, EndBoldCol);
        }
    }
    if (PrintRatings) {
        if ( OutputFormat == CROSSTABLE_Html) {
            dstr->Append ("   ", StartBoldCol, "Perf Chg", EndBoldCol);
        } else {
            dstr->Append ("   ", StartBoldCol, "Perf Chg Percnt", EndBoldCol);
        }
    } else
        dstr->Append (" ", StartBoldCol, "Percnt", EndBoldCol);
    if (PrintTallies && OutputFormat == CROSSTABLE_Html) {
        dstr->Append ("   ", StartBoldCol, "Perc", EndBoldCol);
        dstr->Append ("   ", StartBoldCol, "+/-/=", EndBoldCol);
    }
    dstr->Append (EndRow, NewLine);

    PrintDashesLine (dstr);

    // Print the rows of players and results:
    uint previousScore = 0;
    for (player = 0; player < playerLimit; player++) {
        uint index = SortedIndex[player];
        playerDataT * pdata = PlayerData[index];

        // Print a blank line if we are at a new score group:
        if (SeparateScoreGroups  &&  SortOption == CROSSTABLE_SortScore) {
            if (player > 0  &&  pdata->score != previousScore) {
                dstr->Append (BlankRowLine);
            }
        }
        previousScore = pdata->score;

        sprintf (stemp, "%*u: ", PlayerNumWidth, player+1);
        dstr->Append (StartRow, StartRightCol, stemp, EndRightCol);

        PrintPlayer (dstr, pdata);

        if (OutputFormat == CROSSTABLE_LaTeX) {
            sprintf (stemp, " %2u%c%c ", pdata->score / 2, DecimalPointChar,
                     (pdata->score & 1 ? '5' : '0'));
            dstr->Append (StartRightCol, stemp, EndRightCol);
            sprintf (stemp, " %2u  ", pdata->gameCount);
            dstr->Append (StartRightCol, stemp, EndRightCol);
        } else {
            if (ThreeWin) {
	      sprintf (stemp, " %2u%c%c  ",
		       pdata->score / 2, DecimalPointChar,
		       (pdata->score & 1 ? '5' : '0'));
            } else {
	      sprintf (stemp, " %2u%c%c / %2u  ",
		       pdata->score / 2, DecimalPointChar,
		       (pdata->score & 1 ? '5' : '0'), pdata->gameCount);
            }
            dstr->Append (StartRightCol, stemp, EndRightCol);
        }
        if (PrintTiebreaks) {
            sprintf (stemp, "%3u%c%02u ", pdata->tiebreak / 4,
                     DecimalPointChar, (pdata->tiebreak % 4) * 25);
            dstr->Append (StartRightCol, stemp, EndRightCol);
        }

        uint r_won = 0, r_loss = 0, r_draw = 0;
        for (uint oppCount = 0; oppCount < playerLimit; oppCount++) {
            if (playerLimit == 2  &&  oppCount == player) { continue; }
            uint opp = SortedIndex[oppCount];
            dstr->AddChar (' ');
            dstr->Append (StartRightCol);
            clashT * clash = pdata->firstClash[opp];

            for (uint count = 0; count < MaxClashes; count++) {
                if (clash != NULL) {
                    if (OutputFormat == CROSSTABLE_Hypertext) {
                        if (CurrentGame == clash->gameNum)
                            sprintf(stemp, "<green><g_%u>%c</g></green>",
                                    clash->gameNum, RESULT_CHAR[clash->result]);
                        else
                            sprintf(stemp, "<blue><g_%u>%c</g></blue>",
                                    clash->gameNum, RESULT_CHAR[clash->result]);
                        dstr->Append (stemp);
                    } else {
                        dstr->AddChar (RESULT_CHAR[clash->result]);
                    }
                    switch (clash->result) {
                    case 1:
                        r_won++;
                        break;
                    case 2:
                        r_loss++;
                        break;
                    case 3:
                        r_draw++;
                        break;
                    }
                    clash = clash->next;
                } else {
                    dstr->AddChar (index == opp ? 'X' : '.');
                }
            }
            dstr->Append (EndCol);
        }

        PrintPerformance (dstr, pdata);
        PrintScorePercentage (dstr, pdata);

        if (PrintTallies) {
	  dstr->Append (StartCol);
	  sprintf (stemp, "  (+%u -%u =%u)", r_won, r_loss, r_draw);
	  dstr->Append (stemp);
	  dstr->Append (EndCol);
        }

        dstr->Append (EndRow, NewLine);
    }
    PrintDashesLine (dstr);
    dstr->Append (EndTable);
}

void
Crosstable::PrintSwiss (DString * dstr, uint playerLimit)
{
    char stemp [1000];
    uint player;

    const char * resultStr [NUM_RESULT_TYPES];
    if (OutputFormat == CROSSTABLE_LaTeX) {
        resultStr[RESULT_White] = "{\\tt +}";
        resultStr[RESULT_Draw]  = "{\\tt =}";
        resultStr[RESULT_Black] = "{\\tt -}";
        resultStr[RESULT_None] = "{\\tt *}";
    } else {
        resultStr[RESULT_White] = "+";
        resultStr[RESULT_Draw]  = "=";
        resultStr[RESULT_Black] = "-";
        resultStr[RESULT_None] = "*";
    }

    dstr->Append (StartTable);
    if (OutputFormat == CROSSTABLE_LaTeX) {
        dstr->Append ("\\begin{tabular}{rl");
        if (PrintRatings) { dstr->Append ("r"); }
        if (PrintTitles) { dstr->Append ("r"); }
        if (PrintAges) { dstr->Append ("r"); }
        if (PrintCountries) { dstr->Append ("l"); }
        dstr->Append ("r@{ / }r");
        if (PrintTiebreaks) { dstr->Append ("r"); }
        for (uint i=0; i < MaxRound; i++) {
            dstr->Append ("r");
            if (i < MaxRound-1) { dstr->Append ("@{ }"); }
        }
        if (PrintRatings) { dstr->Append ("r"); }
        dstr->Append ("r}\n");
    }
    dstr->Append (StartRow);
    if (OutputFormat == CROSSTABLE_Html) {
        dstr->Append ("<th></th> <th>Player</th> ");
    } else if (OutputFormat == CROSSTABLE_LaTeX) {
        dstr->Append ("  & \\bf Player & ");
    } else {
        strPad (stemp, "", LongestNameLen + 2 + PlayerNumWidth, ' ');
        dstr->Append (stemp);
    }
    if (PrintRatings) {
        dstr->Append (StartBoldCol, " Rtng", EndBoldCol);
    }
    if (PrintTitles) {
        dstr->Append (StartBoldCol, "  Ti", EndBoldCol);
    }
    if (PrintAges) {
        dstr->Append (StartBoldCol, " Age", EndBoldCol);
    }
    if (PrintCountries) {
        dstr->Append (StartBoldCol, " Nat", EndBoldCol);
    }
    if (PrintFlags && OutputFormat == CROSSTABLE_Hypertext) {
        dstr->Append (StartBoldCol, " Nat ", EndBoldCol);
    }
    if (OutputFormat == CROSSTABLE_LaTeX) {
        dstr->Append (" \\multicolumn{2}{c}{\\bf Score} & ");
    } else {
        if (ThreeWin) 
            dstr->Append ("  ", StartBoldCol, "Score", EndBoldCol, " ");
        else if (PrintFlags && OutputFormat == CROSSTABLE_Hypertext)
            dstr->Append (" ", StartBoldCol, "Score  ", EndBoldCol, "   ");
        else
            dstr->Append ("  ", StartBoldCol, " Score  ", EndBoldCol, "   ");
    }
    if (PrintTiebreaks) {
        dstr->Append (StartBoldCol, "(Tie)", EndBoldCol);
    }

    for (uint round = 1; round <= MaxRound; round++) {
        if (OutputFormat == CROSSTABLE_LaTeX) {
            dstr->Append (" \\multicolumn{1}{c}{\\bf ", round, "} & ");
        } else {
            sprintf (stemp, " %s%*d ", SwissColors ? " " : "",
                     PlayerNumWidth, round);
            dstr->Append (StartBoldCol, stemp, EndBoldCol);
        }
    }
    if (PrintRatings) {
        if ( OutputFormat == CROSSTABLE_Html) {
            dstr->Append ("   ", StartBoldCol, "Perf Chg", EndBoldCol);
        } else {
            dstr->Append ("   ", StartBoldCol, "Perf Chg Percnt", EndBoldCol);
        }
    } else
        dstr->Append (" ", StartBoldCol, "Percnt", EndBoldCol);
    if (PrintTallies && OutputFormat == CROSSTABLE_Html) {
        dstr->Append ("   ", StartBoldCol, "Perc", EndBoldCol);
        dstr->Append ("   ", StartBoldCol, "+/-/=", EndBoldCol);
    }
    dstr->Append (EndRow, NewLine);

    PrintDashesLine (dstr);

    // Print the rows of players and results:
    uint previousScore = 0;
    for (player = 0; player < playerLimit; player++) {
        uint index = SortedIndex[player];
        playerDataT * pdata = PlayerData[index];

        // Print a blank line if we are at a new score group:
        if (SeparateScoreGroups  &&  SortOption == CROSSTABLE_SortScore) {
            if (player > 0  &&  pdata->score != previousScore) {
                dstr->Append (BlankRowLine);
            }
        }
        previousScore = pdata->score;

        sprintf (stemp, "%*u: ", PlayerNumWidth, player+1);
        dstr->Append (StartRow, StartRightCol, stemp, EndRightCol);

        PrintPlayer (dstr, pdata);

        if (OutputFormat == CROSSTABLE_LaTeX) {
            sprintf (stemp, " %2u%c%c ", pdata->score / 2, DecimalPointChar,
                     (pdata->score & 1 ? '5' : '0'));
            dstr->Append (StartRightCol, stemp, EndRightCol);
            sprintf (stemp, " %2u  ", pdata->gameCount);
            dstr->Append (StartRightCol, stemp, EndRightCol);
        } else {
            if (ThreeWin) {
	      sprintf (stemp, " %2u%c%c  ",
		       pdata->score / 2, DecimalPointChar,
		       (pdata->score & 1 ? '5' : '0'));
            } else {
	      sprintf (stemp, " %2u%c%c / %2u  ",
		       pdata->score / 2, DecimalPointChar,
		       (pdata->score & 1 ? '5' : '0'), pdata->gameCount);
            }
            dstr->Append (StartRightCol, stemp, EndRightCol);
        }
        if (PrintTiebreaks) {
            sprintf (stemp, "%2u%c%c ", pdata->tiebreak / 2,
                     DecimalPointChar, (pdata->tiebreak & 1 ? '5' : '0'));
            dstr->Append (StartRightCol, stemp, EndRightCol);
        }

        uint r_won = 0, r_loss = 0, r_draw = 0;
        for (uint round = 1; round <= MaxRound; round++) {
            clashT * clash = pdata->roundClash[round];
            dstr->AddChar (' ');
            dstr->Append (StartRightCol);
            if (clash == NULL) {
                dstr->Append ("...");
                if (PlayerNumWidth == 3) { dstr->AddChar ('.'); }
                if (SwissColors) { dstr->AddChar ('.'); }
            } else {
                if (OutputFormat == CROSSTABLE_Hypertext) {
                    if (CurrentGame == clash->gameNum)
                      sprintf (stemp, "<green><g_%u>", clash->gameNum);
                    else
                      sprintf (stemp, "<blue><g_%u>", clash->gameNum);
                    dstr->Append (stemp);
                }
                if (SwissColors) {
                    sprintf (stemp, "%*d%c%s", PlayerNumWidth,
                             InvertedIndex[clash->opponent] + 1,
                             clash->color == WHITE ? 'w' : 'b',
                             resultStr[clash->result]);
                } else {
                    sprintf (stemp, "%*d%s", PlayerNumWidth,
                             InvertedIndex[clash->opponent] + 1,
                             resultStr[clash->result]);
                }
                dstr->Append (stemp);
                if (OutputFormat == CROSSTABLE_Hypertext) {
                    if (CurrentGame == clash->gameNum)
                      dstr->Append ("</g></green>");
                    else
                      dstr->Append ("</g></blue>");
                }
                switch (clash->result) {
		  case 1:
		      r_won++; break;
		  case 2:
		      r_loss++; break;
		  case 3:
		      r_draw++; break;
                }
            }
            dstr->Append (EndCol);
        }
        PrintPerformance (dstr, pdata);
        PrintScorePercentage (dstr, pdata);

        if (PrintTallies) {
	  dstr->Append (StartCol);
	  sprintf (stemp, "  (+%u -%u =%u)", r_won, r_loss, r_draw);
	  dstr->Append (stemp);
	  dstr->Append (EndCol);
        }

        dstr->Append (EndRow, NewLine);
    }

    PrintDashesLine (dstr);
    dstr->Append (EndTable);
}


void
Crosstable::PrintKnockout (DString * dstr, uint playerLimit)
{
    char stemp [1000];
    uint player;

    for (uint round = 1; round <= MaxRound; round++) {
        if (OutputFormat == CROSSTABLE_LaTeX) {
            dstr->Append ("\n\n", round, ":\n\n");
        } else {
            dstr->Append (round, ":", NewLine);
        }
        dstr->Append (StartTable);
        if (OutputFormat == CROSSTABLE_LaTeX) {
            dstr->Append ("\\begin{tabular}{l");
            if (PrintRatings) { dstr->Append ("r"); }
            if (PrintTitles) { dstr->Append ("r"); }
            if (PrintAges) { dstr->Append ("r"); }
            if (PrintCountries) { dstr->Append ("l"); }
            dstr->Append ("@{\\hspace{4ex}}lr@{ / }r@{\\hspace{4ex}}l");
            if (PrintRatings) { dstr->Append ("r"); }
            if (PrintTitles) { dstr->Append ("r"); }
            if (PrintAges) { dstr->Append ("r"); }
            if (PrintCountries) { dstr->Append ("l"); }
            dstr->Append ("r}\n");
        }
        PrintDashesLine (dstr);
        for (player = 0; player < playerLimit; player++) {
            PlayerData[player]->printed = false;
        }
        for (player = 0; player < playerLimit; player++) {
            uint index = SortedIndex[player];
            playerDataT * pdata = PlayerData[index];
            clashT * clash = pdata->roundClash[round];
            if (clash == NULL) { continue; }
            if (pdata->printed) { continue; }
            uint opponent = clash->opponent;
            // if black first, skip this game till white is first
            if (pdata->firstClash[opponent]->color == 1) { continue; }
            uint score = 0;
            uint nGames = 0;
            clash = pdata->firstClash[opponent];
            while (clash != 0) {
                if (clash->round == round) {
                    score += RESULT_SCORE[clash->result];
                    nGames++;
                }
                clash = clash->next;
            }
            // Skip this to allow white to be shown first
            // if (score < nGames) { continue; }
            dstr->Append (StartRow);
            PrintPlayer (dstr, pdata);

            dstr->Append (StartCol, "  ");
            if (OutputFormat == CROSSTABLE_LaTeX) { dstr->Append ("{\\tt "); }
            clash = pdata->firstClash[opponent];
            for (uint count = 0; count < MaxClashes; count++) {
                while (clash != NULL  &&  clash->round != round) {
                    clash = clash->next;
                }
                if (clash != NULL) {
                    if (OutputFormat == CROSSTABLE_Hypertext) {
                        if (CurrentGame == clash->gameNum)
                            sprintf (stemp, "<green><g_%u>%c</g></green>",
                                     clash->gameNum,
                                     RESULT_CHAR[clash->result]);
                        else
                            sprintf (stemp, "<blue><g_%u>%c</g></blue>",
                                     clash->gameNum,
                                     RESULT_CHAR[clash->result]);
                        dstr->Append (stemp);
                    } else {
                        dstr->AddChar (RESULT_CHAR[clash->result]);
                    }
                    clash = clash->next;
                } else {
                    dstr->AddChar (' ');
                }
            }
            if (OutputFormat == CROSSTABLE_LaTeX) { dstr->Append ("}"); }
            dstr->Append (" ", EndCol);

            if (OutputFormat == CROSSTABLE_LaTeX) {
                sprintf (stemp, " %2u%c%c ", score / 2, DecimalPointChar,
                         (score & 1 ? '5' : '0'));
                dstr->Append (StartRightCol, stemp, EndRightCol);
                sprintf (stemp, " %2u  ", nGames);
                dstr->Append (StartRightCol, stemp, EndRightCol);
            } else {
                sprintf (stemp, "%2u%c%c / %2u   ",
                         score / 2, DecimalPointChar,
                         (score & 1 ? '5' : '0'), nGames);
                dstr->Append (StartRightCol, stemp, EndRightCol);
            }
            PrintPlayer (dstr, PlayerData[opponent]);
            dstr->Append (EndRow, NewLine);
            pdata->printed = true;
            PlayerData[opponent]->printed = true;
        }
        PrintDashesLine (dstr);
        dstr->Append (EndTable, NewLine);
    }
}

#endif // WINCE
//////////////////////////////////////////////////////////////////////
//  EOF: crosstab.cpp
//////////////////////////////////////////////////////////////////////
