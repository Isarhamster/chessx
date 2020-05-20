/*
* Copyright (c) 1999-2002  Shane Hudson
* Copyright (c) 2006-2009  Pascal Georges
* Copyright (C) 2014  Fulvio Benini

* This file is part of Scid (Shane's Chess Information Database).
*
* Scid is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation.
*
* Scid is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Scid.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SCID_INDEXENTRY_V4_6_H
#define SCID_INDEXENTRY_V4_6_H

#include "common.h"
#include "date.h"
#include "matsig.h"
#include "namebase.h"

// HPSIG_SIZE = size of HomePawnData array in an IndexEntry.
// It is nine bytes: the first byte contains the number of valid entries
// in the array, and the next 8 bytes contain up to 16 half-byte entries.
const uint HPSIG_SIZE = 9;

const uint MAX_ELO = 4000; // Since we store Elo Ratings in 12 bits

const byte CUSTOM_FLAG_MASK[] = { 1, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5 };

// Total on-disk size per index entry: currently 47 bytes.
const uint  INDEX_ENTRY_SIZE = 47;
const uint  OLD_INDEX_ENTRY_SIZE = 46;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Class IndexEntry: one of these per game in the index file.
//
//    It contains more than just the location of the game data in the main
//    data file.  For fast searching, it also store some other important
//    values: players, event, site, date, result, eco, gamelength.
//
//    It takes 48 bytes, assuming sizeof(uint) == 4 and sizeof(ushort) == 2.

class IndexEntry
{
    uint32_t  Offset;            // Start of gamefile record for this game.
    uint16_t  Length_Low;        // Length of gamefile record for this game. 17 bits are used so the max
                                 // length is 128 ko (131071). So 7 bits are usable for custom flags or other.
    byte      Length_High;       // LxFFFFFF ( L = length for long games, x = spare, F = custom flags)
    // Name ID values are packed into 12 bytes, saving 8 bytes over the
    // simpler method of just storing each as a 4-byte idNumberT.
    byte      WhiteBlack_High;   // High bits of White, Black.
    uint16_t  WhiteID_Low;       // Lower 16 bits of White ID.
    uint16_t  BlackID_Low;       // Lower 16 bits of Black ID.
    uint16_t  EventID_Low;       // Lower 16 bits of Site.
    uint16_t  SiteID_Low;        // Lower 16 bits of Site ID.
    uint16_t  RoundID_Low;       // Lower 16 bits of Round ID.
    uint16_t  Flags;
    uint16_t  VarCounts;         // Counters for comments, variations, etc.
                                 // VarCounts also stores the result.
    ecoT      EcoCode;           // ECO code
    dateT     Dates;             // Date and EventDate fields.
    eloT      WhiteElo;
    eloT      BlackElo;
    matSigT   FinalMatSig;       // material of the final position in the game,
                                 // and the StoredLineCode in the top 8 bits.
    uint16_t  NumHalfMoves;
    byte      HomePawnData [HPSIG_SIZE];  // homePawnSig data.
    byte      EventSiteRnd_High; // High bits of Event, Site, Round.
    
public:
    void Init();
    template <class T> errorT Read (T* file, versionT version);
    template <class T> errorT Write (T* file, versionT version) const;


    uint32_t GetOffset () const { return Offset; }
    void SetOffset (uint64_t offset) { Offset = static_cast<uint32_t>(offset); }
    uint32_t GetLength() const {
        return Length_Low + (uint32_t(Length_High & 0x80) << 9);
    }
    void SetLength (size_t length) {
        Length_Low = static_cast<uint16_t>(length & 0xFFFF);
        // preserve the last 7 bits
        Length_High = ( Length_High & 0x7F ) | static_cast<byte>( (length >> 16) << 7 );
    }


    // Name Get and Set routines:
    //   WhiteID and BlackID are 20-bit values, EventID and SiteID are
    //   19-bit values, and RoundID is an 18-bit value.
    //
    //   WhiteID high 4 bits = bits 4-7 of WhiteBlack_High.
    //   BlackID high 4 bits = bits 0-3 of WhiteBlack_High.
    //   EventID high 3 bits = bits 5-7 of EventSiteRnd_high.
    //   SiteID  high 3 bits = bits 2-4 of EventSiteRnd_high.
    //   RoundID high 2 bits = bits 0-1 of EventSiteRnd_high.
    idNumberT GetWhite () const {
        idNumberT id = (idNumberT) WhiteBlack_High;
        id = id >> 4;  // High 4 bits = bits 4-7 of WhiteBlack_High.
        id <<= 16;
        id |= (idNumberT) WhiteID_Low;
        return id;
    }
    idNumberT GetBlack () const {
        idNumberT id = (idNumberT) WhiteBlack_High;
        id = id & 0xF;   // High 4 bits = bits 0-3 of WhiteBlack_High.
        id <<= 16;
        id |= (idNumberT) BlackID_Low;
        return id;
    }
    idNumberT GetPlayer(colorT col) const {
        if (col == BLACK) return GetBlack();
        return GetWhite();
    }
    idNumberT GetEvent () const {
        uint id = (idNumberT) EventSiteRnd_High;
        id >>= 5;  // High 3 bits = bits 5-7 of EventSiteRnd_High.
        id <<= 16;
        id |= (idNumberT) EventID_Low;
        return id;
    }
    idNumberT GetSite () const {
        uint id = (idNumberT) EventSiteRnd_High;
        id = (id >> 2) & 7;  // High 3 bits = bits 2-5 of EventSiteRnd_High.
        id <<= 16;
        id |= (idNumberT) SiteID_Low;
        return id;
    }
    idNumberT GetRound () const {
        uint id = (idNumberT) EventSiteRnd_High;
        id &= 3;   // High 2 bits = bits 0-1 of EventSiteRnd_High.
        id <<= 16;
        id |= (idNumberT) RoundID_Low;
        return id;
    }

    void SetWhite (idNumberT id) {
        WhiteID_Low = id & 0xFFFF;
        WhiteBlack_High = WhiteBlack_High & 0x0F;   // Clear bits 4-7.
        WhiteBlack_High |= ((id >> 16) << 4);       // Set bits 4-7.
    }
    void SetBlack (idNumberT id) {
        BlackID_Low = id & 0xFFFF;
        WhiteBlack_High = WhiteBlack_High & 0xF0;   // Clear bits 0-3.
        WhiteBlack_High |= (id >> 16);              // Set bits 0-3.
    }
    void SetPlayer (colorT col, idNumberT id) {
        if (col == BLACK) return SetBlack(id);
        return SetWhite(id);
    }
    void SetEvent (idNumberT id) {
        EventID_Low = id & 0xFFFF;
        // Clear bits 2-4 of EventSiteRnd_high: 31 = 00011111 binary.
        EventSiteRnd_High = EventSiteRnd_High & 31;
        EventSiteRnd_High |= ((id >> 16) << 5);
    }
    void SetSite (idNumberT id) {
        SiteID_Low = id & 0xFFFF;
        // Clear bits 2-4 of EventSiteRnd_high: 227 = 11100011 binary.
        EventSiteRnd_High = EventSiteRnd_High & 227;
        EventSiteRnd_High |= ((id >> 16) << 2);
    }
    void SetRound (idNumberT id) {
        RoundID_Low = id & 0xFFFF;
        // Clear bits 0-1 of EventSiteRnd_high: 252 = 11111100 binary.
        EventSiteRnd_High = EventSiteRnd_High & 252;
        EventSiteRnd_High |= (id >> 16);
    }


    const char* GetWhiteName (const NameBase* nb) const {
        return nb->GetName (NAME_PLAYER, GetWhite()); 
    }
    const char* GetBlackName (const NameBase* nb) const {
        return nb->GetName (NAME_PLAYER, GetBlack());
    }
    const char* GetEventName (const NameBase* nb) const {
        return nb->GetName (NAME_EVENT, GetEvent());
    }
    const char* GetSiteName (const NameBase* nb) const {
        return nb->GetName (NAME_SITE, GetSite());
    }
    const char* GetRoundName (const NameBase* nb) const {
        return nb->GetName (NAME_ROUND, GetRound());
    }

    dateT GetDate () const { return u32_low_20(Dates); }
    uint  GetYear () const { return date_GetYear (GetDate()); }
    uint  GetMonth() const { return date_GetMonth (GetDate()); }
    uint  GetDay ()  const { return date_GetDay (GetDate()); }
    dateT GetEventDate () const {
        uint dyear = date_GetYear (GetDate());
        dateT edate = u32_high_12 (Dates);
        uint month = date_GetMonth (edate);
        uint day = date_GetDay (edate);
        uint year = date_GetYear(edate) & 7;
        if (year == 0) { return ZERO_DATE; }
        year = dyear + year - 4;
        return DATE_MAKE (year, month, day);
    }
    resultT GetResult () const { return (VarCounts >> 12); }
    eloT GetWhiteElo () const { return u16_low_12(WhiteElo); }
    eloT GetWhiteElo (const NameBase* nb)  const {
        eloT r = GetWhiteElo();
        if (r == 0 && nb != 0) return nb->GetElo (GetWhite());
        return r;
    }
    eloT GetBlackElo () const { return u16_low_12(BlackElo); }
    eloT GetBlackElo (const NameBase* nb) const {
        eloT r = GetBlackElo();
        if (r == 0 && nb != 0) return nb->GetElo (GetBlack());
        return r;
    }
    eloT GetElo(colorT col) const {
        if (col == BLACK) return GetBlackElo();
        return GetWhiteElo();
    }
    byte   GetWhiteRatingType () const { return u16_high_4 (WhiteElo); }
    byte   GetBlackRatingType () const { return u16_high_4 (BlackElo); }
    ecoT   GetEcoCode () const { return EcoCode; }
    ushort GetNumHalfMoves () const { return NumHalfMoves; }
    byte   GetRating(const NameBase* nb) const;

    void SetDate  (dateT date)   {
        Dates = u32_set_low_20 (Dates, date);
    }
    void SetEventDate (dateT edate) {
        uint codedDate = date_GetMonth(edate) << 5;
        codedDate |= date_GetDay (edate);
        uint eyear = date_GetYear (edate);
        uint dyear = date_GetYear (GetDate());
        // Due to a compact encoding format, the EventDate
        // must be within a few years of the Date.
        if ((eyear + 3) < dyear  ||  eyear > (dyear + 3)) {
            codedDate = 0; 
        } else {
            codedDate |= (((eyear + 4 - dyear) & 7) << 9);
        }
        Dates = u32_set_high_12 (Dates, codedDate);
    }
    void SetResult (resultT res) {
        VarCounts = (VarCounts & 0x0FFF) | (((ushort)res) << 12);
    }
    void SetWhiteElo (eloT elo)  {
        WhiteElo = u16_set_low_12(WhiteElo, elo);
    }
    void SetBlackElo (eloT elo)  {
        BlackElo = u16_set_low_12 (BlackElo, elo);
    }
    void SetWhiteRatingType (byte b) {
        WhiteElo = u16_set_high_4 (WhiteElo, b);
    }
    void SetBlackRatingType (byte b) {
        BlackElo = u16_set_high_4 (BlackElo, b);
    }
    void SetEcoCode (ecoT eco)   { EcoCode = eco; }
    void SetNumHalfMoves (ushort b)  { NumHalfMoves = b; }


    bool GetFlag (uint32_t mask) const {
        uint32_t tmp = Flags;
        if ((mask & 0xFFFF0000) != 0) {
            // The if is not necessary but should be faster
            tmp |= (Length_High & 0x3F) << 16;
        }
        return (tmp & mask) == mask;
    }
    bool GetStartFlag () const      { return (Flags & (1 << IDX_FLAG_START)) != 0; }
    bool GetPromotionsFlag () const { return (Flags & (1 << IDX_FLAG_PROMO)) != 0; }
    bool GetUnderPromoFlag() const  { return (Flags & (1 << IDX_FLAG_UPROMO)) != 0; }
    bool GetCommentsFlag () const   { return (GetCommentCount() > 0); }
    bool GetVariationsFlag () const { return (GetVariationCount() > 0); }
    bool GetNagsFlag () const       { return (GetNagCount() > 0); }
    bool GetDeleteFlag () const     { return (Flags & (1 << IDX_FLAG_DELETE)) != 0; }

    static uint CharToFlag (char ch);
    static uint32_t CharToFlagMask (char flag);
    static uint32_t StrToFlagMask (const char* flags);
    uint GetFlagStr(char* dest, const char* flags) const;

    uint GetVariationCount () const { return DecodeCount(VarCounts & 15); }
    uint GetCommentCount () const   { return DecodeCount((VarCounts >> 4) & 15); }
    uint GetNagCount () const       { return DecodeCount((VarCounts >> 8) & 15); }

    matSigT GetFinalMatSig () const { return u32_low_24 (FinalMatSig); }
    byte GetStoredLineCode () const { return u32_high_8 (FinalMatSig); }
    const byte* GetHomePawnData () const { return HomePawnData; }
    byte* GetHomePawnData () { return HomePawnData; }

    void SetFlag (uint32_t flagMask, bool b) {
        uint16_t flagLow = flagMask & 0xFFFF;
        if (flagLow != 0) {
            if (b) { 
                Flags |= flagLow;
            } else {
                Flags &= ~flagLow;
            }
        }

        byte flagHigh = (flagMask >> 16) & 0x3F;
        if (flagHigh != 0) {
            if (b) {
                Length_High |= flagHigh;
            } else {
                Length_High &= ~flagHigh;
            }
        }
    }
    void SetStartFlag (bool b)      { SetFlag(1 << IDX_FLAG_START, b); }
    void SetPromotionsFlag (bool b) { SetFlag(1 << IDX_FLAG_PROMO, b); }
    void SetUnderPromoFlag (bool b) { SetFlag(1 << IDX_FLAG_UPROMO, b); }
    void SetDeleteFlag (bool b)     { SetFlag(1 << IDX_FLAG_DELETE, b); }
    void clearFlags() { return SetFlag(IDX_MASK_ALLFLAGS, false); }

    void SetVariationCount (uint x) {
        VarCounts = (VarCounts & 0xFFF0U) | EncodeCount(x);
    }
    void SetCommentCount (uint x) {
        VarCounts = (VarCounts & 0xFF0FU) | (EncodeCount(x) << 4);
    }
    void SetNagCount (uint x) {
        VarCounts = (VarCounts & 0xF0FFU) | (EncodeCount(x) << 8);
    }

    void SetFinalMatSig (matSigT ms) {
        FinalMatSig = u32_set_low_24 (FinalMatSig, ms);
    }
    void SetStoredLineCode (byte b)    {
        FinalMatSig = u32_set_high_8 (FinalMatSig, b);
    }

    enum {
        // IndexEntry Flag types:
        IDX_FLAG_START      =  0,   // Game has own start position.
        IDX_FLAG_PROMO      =  1,   // Game contains promotion(s).
        IDX_FLAG_UPROMO     =  2,   // Game contains promotion(s).
        IDX_FLAG_DELETE     =  3,   // Game marked for deletion.
        IDX_FLAG_WHITE_OP   =  4,   // White openings flag.
        IDX_FLAG_BLACK_OP   =  5,   // Black openings flag.
        IDX_FLAG_MIDDLEGAME =  6,   // Middlegames flag.
        IDX_FLAG_ENDGAME    =  7,   // Endgames flag.
        IDX_FLAG_NOVELTY    =  8,   // Novelty flag.
        IDX_FLAG_PAWN       =  9,   // Pawn structure flag.
        IDX_FLAG_TACTICS    = 10,   // Tactics flag.
        IDX_FLAG_KSIDE      = 11,   // Kingside play flag.
        IDX_FLAG_QSIDE      = 12,   // Queenside play flag.
        IDX_FLAG_BRILLIANCY = 13,   // Brilliancy or good play.
        IDX_FLAG_BLUNDER    = 14,   // Blunder or bad play.
        IDX_FLAG_USER       = 15,   // User-defined flag.
        IDX_FLAG_CUSTOM1    = 16,   // Custom flag.
        IDX_FLAG_CUSTOM2    = 17,   // Custom flag.
        IDX_FLAG_CUSTOM3    = 18,   // Custom flag.
        IDX_FLAG_CUSTOM4    = 19,   // Custom flag.
        IDX_FLAG_CUSTOM5    = 20,   // Custom flag.
        IDX_FLAG_CUSTOM6    = 21,   // Custom flag.
        IDX_NUM_FLAGS       = 22,
    };
    static const uint32_t IDX_MASK_ALLFLAGS = 0xFFFFFFFF;

private:
    static uint EncodeCount (uint x) {
        if (x <= 10) { return x; }
        if (x <= 12) { return 10; }
        if (x <= 17) { return 11; }  // 11 indicates 15 (13-17)
        if (x <= 24) { return 12; }  // 12 indicates 20 (18-24)
        if (x <= 34) { return 13; }  // 13 indicates 30 (25-34)
        if (x <= 44) { return 14; }  // 14 indicates 40 (35-44)
        return 15;                   // 15 indicates 50 or more
    }
    static uint DecodeCount (uint x) {
        static uint countCodes[16] = {0,1,2,3,4,5,6,7,8,9,10,15,20,30,40,50};
        return countCodes[x & 15];
    }

// Bitmask functions for index entry decoding:
    static byte u32_high_8( uint x )
    {
        return (byte)(x >> 24);
    }

    static uint u32_low_24( uint x )
    {
        return x & 0x00FFFFFF;
    }

    static uint u32_high_12( uint x )
    {
        return x >> 20;
    }

    static uint u32_low_20( uint x )
    {
        return x & 0x000FFFFF;
    }

    static byte u16_high_4( ushort x )
    {
        return (byte)(x >> 12);
    }

    static ushort u16_low_12( ushort x )
    {
        return x & 0x0FFF;
    }

    static uint u32_set_high_8( uint u, byte x )
    {
        return u32_low_24(u) | ((uint)x << 24);
    }

    static uint u32_set_low_24( uint u, uint x )
    {
        return (u & 0xFF000000) | (x & 0x00FFFFFF);
    }

    static uint u32_set_high_12( uint u, uint x )
    {
        return u32_low_20(u) | (x << 20);
    }

    static uint u32_set_low_20( uint u, uint x )
    {
        return (u & 0xFFF00000) | (x & 0x000FFFFF);
    }

    static ushort u16_set_high_4( ushort u, byte x )
    {
        return u16_low_12(u) | ((ushort)x << 12);
    }

    static ushort u16_set_low_12( ushort u, ushort x )
    {
        return (u & 0xF000) | (x & 0x0FFF);
    }
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IndexEntry::Init():
//        Initialise a single index entry.
//
inline void
IndexEntry::Init ()
{
    NumHalfMoves = 0;
    WhiteID_Low = 0;
    BlackID_Low = 0;
    EventID_Low = 0;
    SiteID_Low = 0;
    RoundID_Low = 0;
    WhiteBlack_High = 0;
    EventSiteRnd_High = 0;
    EcoCode = 0;
    Dates = 0;
    WhiteElo = 0;
    BlackElo = 0;
    FinalMatSig = 0;
    Flags = 0;
    VarCounts = 0;
    Offset = 0;
    Length_Low = 0;
    Length_High = 0;
    SetDate (ZERO_DATE);
    SetEventDate (ZERO_DATE);
    SetResult (RESULT_None);
    SetEcoCode (ECO_None);
    SetFinalMatSig (MATSIG_Empty);
    for (uint i=0; i < HPSIG_SIZE; i++) {
        HomePawnData[i] = 0;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IndexEntry::Read():
//      Reads a single entry's values from an open index file.
//
template <class T> errorT
IndexEntry::Read (T* file, versionT version)
{
    // Length of each gamefile record and its offset.
    Offset = file->ReadFourBytes ();
    Length_Low = file->ReadTwoBytes ();
    Length_High = (version < 400) ? 0 : file->ReadOneByte();
    Flags = file->ReadTwoBytes (); 

    // White and Black player names:
    WhiteBlack_High = file->ReadOneByte ();
    WhiteID_Low = file->ReadTwoBytes ();
    BlackID_Low = file->ReadTwoBytes ();

    // Event, Site and Round names:
    EventSiteRnd_High = file->ReadOneByte ();
    EventID_Low = file->ReadTwoBytes ();
    SiteID_Low = file->ReadTwoBytes ();
    RoundID_Low = file->ReadTwoBytes ();

    VarCounts = file->ReadTwoBytes();
    EcoCode = file->ReadTwoBytes ();

    // Date and EventDate are stored in four bytes.
    Dates = file->ReadFourBytes();

    // The two ELO ratings and rating types take 2 bytes each.
    WhiteElo = file->ReadTwoBytes ();
    BlackElo = file->ReadTwoBytes ();
    if (GetWhiteElo() > MAX_ELO) { SetWhiteElo(MAX_ELO); }
    if (GetBlackElo() > MAX_ELO) { SetBlackElo(MAX_ELO); }

    FinalMatSig = file->ReadFourBytes ();
    NumHalfMoves = file->ReadOneByte ();

    // Read the 9-byte homePawnData array:
    byte * pb = HomePawnData;
    // The first byte of HomePawnData has high bits of the NumHalfMoves
    // counter in its top two bits:
    uint pb0 = file->ReadOneByte();
    *pb = (pb0 & 63);
    pb++;
    NumHalfMoves = NumHalfMoves | ((pb0 >> 6) << 8);
    for (uint i2 = 1; i2 < HPSIG_SIZE; i2++) {
        *pb = file->ReadOneByte ();
        pb++;
    }

    // Top 2 bits of HomePawnData[0] are for NumHalfMoves:
    uint numMoves_High = HomePawnData[0];
    HomePawnData[0] = HomePawnData[0] & 63;
    numMoves_High >>= 6;
    numMoves_High <<= 8;
    NumHalfMoves = NumHalfMoves | numMoves_High;

    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IndexEntry::Write():
//      Writes a single index entry to an open index file.
//      INDEX_ENTRY_SIZE must be updated
template <class T> errorT
IndexEntry::Write (T* file, versionT version) const
{
    // Cannot write old-version index files:
    if (version < 400) { return ERROR_FileVersion; }

    version = 0;  // We don't have any version-specific code.
    
    file->WriteFourBytes (Offset);
    
    file->WriteTwoBytes (Length_Low);
    file->WriteOneByte (Length_High);
    file->WriteTwoBytes (Flags);

    file->WriteOneByte (WhiteBlack_High);
    file->WriteTwoBytes (WhiteID_Low);
    file->WriteTwoBytes (BlackID_Low);

    file->WriteOneByte (EventSiteRnd_High);
    file->WriteTwoBytes (EventID_Low);
    file->WriteTwoBytes (SiteID_Low);
    file->WriteTwoBytes (RoundID_Low);

    file->WriteTwoBytes (VarCounts);
    file->WriteTwoBytes (EcoCode);
    file->WriteFourBytes (Dates);

    // Elo ratings and rating types: 2 bytes each.
    file->WriteTwoBytes (WhiteElo);
    file->WriteTwoBytes (BlackElo);

    file->WriteFourBytes (FinalMatSig);
    file->WriteOneByte (NumHalfMoves & 255); 

    // Write the 9-byte homePawnData array:
    const byte* pb = HomePawnData;
    // The first byte of HomePawnData has high bits of the NumHalfMoves
    // counter in its top two bits:
    byte pb0 = *pb;
    pb0 = pb0 | ((NumHalfMoves >> 8) << 6);
    file->WriteOneByte (pb0);
    pb++;
    // write 8 bytes
    for (uint i2 = 1; i2 < HPSIG_SIZE; i2++) {
        file->WriteOneByte (*pb);
        pb++;
    }

    return OK;
}

inline byte IndexEntry::GetRating(const NameBase* nb) const {
    eloT welo = GetWhiteElo();
    eloT belo = GetBlackElo();
    if (welo == 0) { welo = nb->GetElo (GetWhite()); }
    if (belo == 0) { belo = nb->GetElo (GetBlack()); }
    int rating = static_cast<int>(welo + belo) / 140;

    // Bonus for comments or Nags
    if (GetCommentCount() > 2 || GetNagCount() > 2) {
        if (rating < 21) { // Missing elo
            rating = 40;
        } else {
            rating += 6;
        }
    }

    // Early draw penalty
    if (GetResult() == RESULT_Draw) {
        uint moves = GetNumHalfMoves();
        if (moves < 80) {
            rating -= 3;
            if (moves < 60) {
                rating -= 2;
                if (moves < 40) rating -= 2;
            }
        }
    }

    if (rating < 0) return 0;
    else return static_cast<byte> (rating);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IndexEntry::CharToFlag():
//    Returns the flag number corresponding to the given character.
inline uint
IndexEntry::CharToFlag (char ch)
{
    uint flag = 0;
    switch (toupper(ch)) {
        case 'D': flag = IDX_FLAG_DELETE;     break;
        case 'W': flag = IDX_FLAG_WHITE_OP;   break;
        case 'B': flag = IDX_FLAG_BLACK_OP;   break;
        case 'M': flag = IDX_FLAG_MIDDLEGAME; break;
        case 'E': flag = IDX_FLAG_ENDGAME;    break;
        case 'N': flag = IDX_FLAG_NOVELTY;    break;
        case 'P': flag = IDX_FLAG_PAWN;       break;
        case 'T': flag = IDX_FLAG_TACTICS;    break;
        case 'K': flag = IDX_FLAG_KSIDE;      break;
        case 'Q': flag = IDX_FLAG_QSIDE;      break;
        case '!': flag = IDX_FLAG_BRILLIANCY; break;
        case '?': flag = IDX_FLAG_BLUNDER;    break;
        case 'U': flag = IDX_FLAG_USER;       break;
        case '1': flag = IDX_FLAG_CUSTOM1;    break;
        case '2': flag = IDX_FLAG_CUSTOM2;    break;
        case '3': flag = IDX_FLAG_CUSTOM3;    break;
        case '4': flag = IDX_FLAG_CUSTOM4;    break;
        case '5': flag = IDX_FLAG_CUSTOM5;    break;
        case '6': flag = IDX_FLAG_CUSTOM6;    break;
    }
    return flag;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IndexEntry::CharToFlagMask():
//    Transform a char in a mask that can be used with GetFlag() and SetFlag()
inline uint32_t IndexEntry::CharToFlagMask(char flag)
{
    switch (toupper(flag)) {
        case 'S': return 1 << IDX_FLAG_START;
        case 'X': return 1 << IDX_FLAG_PROMO;
        case 'Y': return 1 << IDX_FLAG_UPROMO;
        case 'D': return 1 << IDX_FLAG_DELETE;
        case 'W': return 1 << IDX_FLAG_WHITE_OP;
        case 'B': return 1 << IDX_FLAG_BLACK_OP;
        case 'M': return 1 << IDX_FLAG_MIDDLEGAME;
        case 'E': return 1 << IDX_FLAG_ENDGAME;
        case 'N': return 1 << IDX_FLAG_NOVELTY;
        case 'P': return 1 << IDX_FLAG_PAWN;
        case 'T': return 1 << IDX_FLAG_TACTICS;
        case 'K': return 1 << IDX_FLAG_KSIDE;
        case 'Q': return 1 << IDX_FLAG_QSIDE;
        case '!': return 1 << IDX_FLAG_BRILLIANCY;
        case '?': return 1 << IDX_FLAG_BLUNDER;
        case 'U': return 1 << IDX_FLAG_USER;
        case '1': return 1 << IDX_FLAG_CUSTOM1;
        case '2': return 1 << IDX_FLAG_CUSTOM2;
        case '3': return 1 << IDX_FLAG_CUSTOM3;
        case '4': return 1 << IDX_FLAG_CUSTOM4;
        case '5': return 1 << IDX_FLAG_CUSTOM5;
        case '6': return 1 << IDX_FLAG_CUSTOM6;
    }

    ASSERT(0);
    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IndexEntry::StrToFlagMask():
//    Transform a string in a mask that can be used with GetFlag() and SetFlag()
inline uint32_t IndexEntry::StrToFlagMask(const char* flags)
{
    if (flags == 0) return 0;

    uint32_t res = 0;
    while (*flags != 0) {
        res |= IndexEntry::CharToFlagMask(*(flags++));
    }
    return res;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IndexEntry::GetFlagStr():
//    Fills in the provided flag string with information on the
//    user-settable flags set for this game.
//    Returns the number of specified flags that are turned on.
inline uint
IndexEntry::GetFlagStr(char* dest, const char* flags) const
{
    if (flags == NULL) { flags = "DWBMENPTKQ!?U123456"; }
    uint count = 0;
    while (*flags != 0) {
        uint32_t mask = CharToFlagMask(*flags);
        ASSERT(mask != 0);
        if (GetFlag(mask)) {
            *dest++ = *flags;
            count++;
        }
        flags++;
    }
    *dest = 0;
    return count;
}

#endif
