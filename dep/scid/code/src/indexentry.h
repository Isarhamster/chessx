/*
* Copyright (c) 1999-2002  Shane Hudson
* Copyright (c) 2006-2009  Pascal Georges
* Copyright (C) 2014-2017  Fulvio Benini

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

#ifndef SCID_INDEXENTRY_H
#define SCID_INDEXENTRY_H

#include "common.h"
#include "date.h"
#include "matsig.h"
#include "namebase.h"

// HPSIG_SIZE = size of HomePawnData array in an IndexEntry.
// It is nine bytes: the first byte contains the number of valid entries
// in the array, and the next 8 bytes contain up to 16 half-byte entries.
const uint HPSIG_SIZE = 9;

const eloT MAX_ELO = 4000; // Since we store Elo Ratings in 12 bits

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
//    It takes 56 bytes.
class IndexEntry {
    uint64_t offset_         : 46; // Start of gamefile record for this game.
    uint64_t gameDataSize_   : 18; // Length of gamefile record for this game.

    uint64_t storedLineCode_ :  8;
    uint64_t whiteID_        : 28;
    uint64_t blackID_        : 28;

    uint32_t eventID_        : 28;
    uint32_t whiteEloType_   :  4;

    uint32_t siteID_         : 28;
    uint32_t blackEloType_   :  4;

    uint32_t roundID_        : 28;
    uint32_t result_         :  4;

    uint32_t whiteElo_       : 12;
    uint32_t date_           : 20;

    uint32_t blackElo_       : 12;
    uint32_t eventDate_      : 20;

    uint32_t numHalfMoves_   : 10;
    uint32_t flags_          : 22;

    uint32_t finalMatSig_    : 24; // material of the final position in the game
    uint32_t nVariations_    :  4;
    uint32_t nComments_      :  4;

    uint16_t ECOcode_;

    uint8_t  nNags_          :  4;

    byte     HomePawnData [HPSIG_SIZE];  // homePawnSig data.

public:
    template <class T> errorT Write (T* file, versionT version) const;

    // get functions
    uint64_t  GetOffset() const { return offset_; }
    uint32_t  GetLength() const { return gameDataSize_; }
    idNumberT GetWhite() const { return whiteID_; }
    eloT      GetWhiteElo() const { return whiteElo_; }
    byte      GetWhiteRatingType() const { return whiteEloType_; }
    idNumberT GetBlack() const { return blackID_; }
    eloT      GetBlackElo() const { return blackElo_; }
    byte      GetBlackRatingType() const { return blackEloType_; }
    idNumberT GetEvent() const { return eventID_; }
    idNumberT GetSite() const { return siteID_; }
    idNumberT GetRound() const { return roundID_; }
    dateT     GetDate() const { return date_; }
    dateT     GetEventDate() const { return eventDate_; }
    resultT   GetResult() const { return result_; }
    uint      GetVariationCount() const { return DecodeCount(nVariations_); }
    uint      GetCommentCount() const { return DecodeCount(nComments_); }
    uint      GetNagCount() const { return DecodeCount(nNags_); }
    uint16_t  GetNumHalfMoves() const { return numHalfMoves_; }
    matSigT   GetFinalMatSig() const { return finalMatSig_; }
    byte      GetStoredLineCode() const { return storedLineCode_; }
    ecoT      GetEcoCode() const { return ECOcode_; }
    bool      GetFlag(uint32_t mask) const { return (flags_ & mask) == mask; }
    const byte* GetHomePawnData() const { return HomePawnData; }
    void SetHomePawnData(byte hpCount, const byte hpVal[8]) {
        HomePawnData[0] = hpCount; // First byte stores the count
        std::copy_n(hpVal, 8, HomePawnData + 1);
    }

    // set functions, assert that the value is not truncated.
    void SetOffset(uint64_t offset) {
        offset_ = offset;
        ASSERT(GetOffset() == offset);
    }
    void SetLength(size_t length) {
        gameDataSize_ = length;
        ASSERT(GetLength() == length);
    }
    void SetWhite(idNumberT id) {
        whiteID_ = id;
        ASSERT(GetWhite() == id);
    }
    void SetWhiteElo(eloT elo) {
        whiteElo_ = elo;
        ASSERT(GetWhiteElo() == elo);
    }
    void SetWhiteRatingType(byte b) {
        whiteEloType_ = b;
        ASSERT(GetWhiteRatingType() == b);
    }
    void SetBlack(idNumberT id) {
        blackID_ = id;
        ASSERT(GetBlack() == id);
    }
    void SetBlackElo(eloT elo) {
        blackElo_ = elo;
        ASSERT(GetBlackElo() == elo);
    }
    void SetBlackRatingType(byte b) {
        blackEloType_ = b;
        ASSERT(GetBlackRatingType() == b);
    }
    void SetEvent(idNumberT id) {
        eventID_ = id;
        ASSERT(GetEvent() == id);
    }
    void SetSite(idNumberT id) {
        siteID_ = id;
        ASSERT(GetSite() == id);
    }
    void SetRound(idNumberT id) {
        roundID_ = id;
        ASSERT(GetRound() == id);
    }
    void SetDate(dateT date) {
        date_ = date;
        ASSERT(GetDate() == date);
    }
    void SetEventDate(dateT edate) {
        eventDate_ = edate;
        ASSERT(GetEventDate() == edate);
    }
    void SetResult(resultT res) {
        result_ = res;
        ASSERT(GetResult() == res);
    }
    void SetVariationCount(unsigned x) { nVariations_ = EncodeCount(x); }
    void SetCommentCount(unsigned x) { nComments_ = EncodeCount(x); }
    void SetNagCount(unsigned x) { nNags_ = EncodeCount(x); }
    void SetRawVariationCount(unsigned x) {
        nVariations_ = x;
        ASSERT(x == nVariations_);
    }
    void SetRawCommentCount(unsigned x) {
        nComments_ = x;
        ASSERT(x == nComments_);
    }
    void SetRawNagCount(unsigned x) {
        nNags_ = x;
        ASSERT(x == nNags_);
    }
    void SetNumHalfMoves(ushort b) {
        numHalfMoves_ = b;
        ASSERT(GetNumHalfMoves() == b);
    }
    void SetFinalMatSig(matSigT ms) {
        finalMatSig_ = ms;
        ASSERT(GetFinalMatSig() == ms);
    }
    void SetStoredLineCode(byte b) {
        storedLineCode_ = b;
        ASSERT(GetStoredLineCode() == b);
    }
    void SetEcoCode(ecoT eco) {
        ECOcode_ = eco;
        ASSERT(GetEcoCode() == eco);
    }
    void SetFlag(uint32_t flagMask, bool set) {
        if (set)
            flags_ |= flagMask;
        else
            flags_ &= ~flagMask;
    }

    // Handy functions that do not directly access member vars.
    uint  GetYear () const { return date_GetYear (GetDate()); }
    uint  GetMonth() const { return date_GetMonth (GetDate()); }
    uint  GetDay ()  const { return date_GetDay (GetDate()); }

    void SetPlayer(colorT col, idNumberT id) {
        return (col == BLACK) ? SetBlack(id) : SetWhite(id);
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
    eloT GetWhiteElo (const NameBase* nb)  const {
        eloT r = GetWhiteElo();
        if (r == 0 && nb != 0) return nb->GetElo (GetWhite());
        return r;
    }
    eloT GetBlackElo (const NameBase* nb) const {
        eloT r = GetBlackElo();
        if (r == 0 && nb != 0) return nb->GetElo (GetBlack());
        return r;
    }
    byte   GetRating(const NameBase* nb) const;

    bool GetStartFlag () const      { return GetFlag(1 << IDX_FLAG_START); }
    bool GetPromotionsFlag () const { return GetFlag(1 << IDX_FLAG_PROMO); }
    bool GetUnderPromoFlag() const  { return GetFlag(1 << IDX_FLAG_UPROMO); }
    bool GetCommentsFlag () const   { return (GetCommentCount() > 0); }
    bool GetVariationsFlag () const { return (GetVariationCount() > 0); }
    bool GetNagsFlag () const       { return (GetNagCount() > 0); }
    bool GetDeleteFlag () const     { return GetFlag(1 << IDX_FLAG_DELETE); }

    static uint CharToFlag (char ch);
    static uint32_t CharToFlagMask (char flag);
    static uint32_t StrToFlagMask (const char* flags);
    uint GetFlagStr(char* dest, const char* flags) const;

    void SetStartFlag (bool b)      { SetFlag(1 << IDX_FLAG_START, b); }
    void SetPromotionsFlag (bool b) { SetFlag(1 << IDX_FLAG_PROMO, b); }
    void SetUnderPromoFlag (bool b) { SetFlag(1 << IDX_FLAG_UPROMO, b); }
    void SetDeleteFlag (bool b)     { SetFlag(1 << IDX_FLAG_DELETE, b); }
    void clearFlags() { return SetFlag(IDX_MASK_ALLFLAGS, false); }

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
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IndexEntry::Write():
//      Writes a single index entry to an open index file.
//      INDEX_ENTRY_SIZE must be updated
template <class T> errorT IndexEntry::Write(T* file, versionT version) const {
	if (version < 400) // Cannot write old-version index files:
		return ERROR_FileVersion;

	char buf[INDEX_ENTRY_SIZE];
	char* buf_it = buf;
	auto WriteOneByte = [&buf_it](uint8_t v) { *buf_it++ = v; };
	auto WriteTwoBytes = [&WriteOneByte](uint16_t v) {
		WriteOneByte(static_cast<uint8_t>(v >> 8));
		WriteOneByte(static_cast<uint8_t>(v));
	};
	auto WriteFourBytes = [&WriteTwoBytes](uint32_t v) {
		WriteTwoBytes(static_cast<uint16_t>(v >> 16));
		WriteTwoBytes(static_cast<uint16_t>(v));
	};

	const IndexEntry* ie = this;

	ASSERT(ie->GetOffset() < (1ULL << 32));
	WriteFourBytes(static_cast<uint32_t>(ie->GetOffset()));

	ASSERT(ie->GetLength() < (1ULL << 17));
	WriteTwoBytes(static_cast<uint16_t>(ie->GetLength()));
	uint8_t len_flags = static_cast<uint8_t>(ie->GetLength() >> 9) & 0x80;
	len_flags |= static_cast<uint8_t>(ie->flags_ >> 16) & 0x3F;
	WriteOneByte(len_flags);
	WriteTwoBytes(static_cast<uint16_t>(ie->flags_));

	// WhiteID and BlackID are 20-bit values, EventID and SiteID are
	// 19-bit values, and RoundID is an 18-bit value.
	// WhiteID high 4 bits = bits 4-7 of WhiteBlack_High.
	// BlackID high 4 bits = bits 0-3 of WhiteBlack_High.
	// EventID high 3 bits = bits 5-7 of EventSiteRnd_high.
	// SiteID  high 3 bits = bits 2-4 of EventSiteRnd_high.
	// RoundID high 2 bits = bits 0-1 of EventSiteRnd_high.
	ASSERT(std::max(ie->GetWhite(), ie->GetBlack()) < (1ULL << 20));
	uint32_t WhiteID_Low = ie->GetWhite();
	uint32_t BlackID_Low = ie->GetBlack();
	uint32_t WhiteBlack_High = (WhiteID_Low & 0x0F0000) >> 12;
	WhiteBlack_High |= (BlackID_Low & 0x0F0000) >> 16;
	WriteOneByte(static_cast<uint8_t>(WhiteBlack_High));
	WriteTwoBytes(static_cast<uint16_t>(WhiteID_Low));
	WriteTwoBytes(static_cast<uint16_t>(BlackID_Low));

	ASSERT(std::max(ie->GetEvent(), ie->GetSite()) < (1ULL << 19));
	ASSERT(ie->GetRound() < (1ULL << 18));
	uint32_t EventID_Low = ie->GetEvent();
	uint32_t SiteID_Low = ie->GetSite();
	uint32_t RoundID_Low = ie->GetRound();
	uint32_t EventSiteRnd_High = (EventID_Low & 0x070000) >> 11;
	EventSiteRnd_High |= (SiteID_Low & 0x070000) >> 14;
	EventSiteRnd_High |= (RoundID_Low & 0x030000) >> 16;
	WriteOneByte(static_cast<uint8_t>(EventSiteRnd_High));
	WriteTwoBytes(static_cast<uint16_t>(EventID_Low));
	WriteTwoBytes(static_cast<uint16_t>(SiteID_Low));
	WriteTwoBytes(static_cast<uint16_t>(RoundID_Low));

	uint16_t varCounts = ie->nVariations_ & 0x0F;
	varCounts |= static_cast<uint16_t>(ie->nComments_ & 0x0F) << 4;
	varCounts |= static_cast<uint16_t>(ie->nNags_ & 0x0F) << 8;
	varCounts |= static_cast<uint16_t>(ie->GetResult() & 0x0F) << 12;
	WriteTwoBytes(varCounts);

	WriteTwoBytes(ie->GetEcoCode());

	// Due to a compact encoding format, the EventDate
	// must be within a few years of the Date.
	uint32_t date = ie->GetDate() & 0xFFFFF;
	uint32_t edate = ie->GetEventDate();
	uint32_t eyear = date_GetYear(edate);
	uint32_t dyear = date_GetYear(date);
	if ((eyear + 3) < dyear || eyear > (dyear + 3)) {
		edate = ZERO_DATE;
	} else {
		eyear = (eyear + 4 - dyear) & 7;
		edate = (eyear << 9) | (date_GetMonth(edate) << 5) | date_GetDay(edate);
	}
	WriteFourBytes((edate << 20) | date);

	// Elo ratings and rating types: 2 bytes each.
	uint16_t wElo = std::min(MAX_ELO, ie->GetWhiteElo());
	wElo |= static_cast<uint16_t>(ie->GetWhiteRatingType()) << 12;
	uint16_t bElo = std::min(MAX_ELO, ie->GetBlackElo());
	bElo |= static_cast<uint16_t>(ie->GetBlackRatingType()) << 12;
	WriteTwoBytes(wElo);
	WriteTwoBytes(bElo);

	ASSERT(ie->GetFinalMatSig() < (1ULL << 24));
	ASSERT(ie->GetStoredLineCode() < (1ULL << 8));
	uint32_t FinalMatSig = ie->GetFinalMatSig();
	FinalMatSig |= static_cast<uint32_t>(ie->GetStoredLineCode()) << 24;
	WriteFourBytes(FinalMatSig);

	// The first byte of HomePawnData has high bits of the NumHalfMoves
	// counter in its top two bits:
	uint16_t nMoves = ie->GetNumHalfMoves();
	ASSERT(nMoves < (1ULL << 10));
	WriteOneByte(static_cast<uint8_t>(nMoves));
	uint8_t pawnData0 = static_cast<uint8_t>(nMoves >> 8) << 6;

	// Write the 9-byte homePawnData array:
	const byte* pb = ie->GetHomePawnData();
	pawnData0 |= *pb & 0x3F;
	WriteOneByte(pawnData0);
	std::copy(pb + 1, pb + HPSIG_SIZE, buf_it);

	return file->sputn(buf, INDEX_ENTRY_SIZE) == INDEX_ENTRY_SIZE
	           ? OK
	           : ERROR_FileWrite;
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
