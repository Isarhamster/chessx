/*
* Copyright (C) 2015-2019 Fulvio Benini

* This file is part of Scid (Shane's Chess Information Database).
*
* Scid is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation.
*
* Scid is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Scid. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SEARCHTOURNAMENTS_H
#define SEARCHTOURNAMENTS_H

#include "common.h"
#include "misc.h"
#include <algorithm>
#include <numeric>
#include <vector>

/**
 * class TourneyGame - Private class used by Tourney and SearchTournaments
 */
struct TourneyGame {
	idNumberT siteID_;
	idNumberT eventID_;
	dateT     eventDate_;
	idNumberT whiteID_;
	idNumberT blackID_;
	eloT      wElo_;
	eloT      bElo_;
	dateT     date_;
	gamenumT  gnum_;
	resultT   result_;

	TourneyGame(const IndexEntry* ie, gamenumT gnum) {
		siteID_ = ie->GetSite();
		eventID_ = ie->GetEvent();
		eventDate_ = ie->GetEventDate();
		whiteID_ = ie->GetWhite();
		blackID_ = ie->GetBlack();
		wElo_ = ie->GetWhiteElo();
		bElo_ = ie->GetBlackElo();
		date_ = ie->GetDate();
		gnum_ = gnum;
		result_ = ie->GetResult();
	}
};

/**
 * class Tourney - Calculate information about a tournament
 *
 * This class takes a range of TourneyGame references and calculate some
 * basic information (number of Players, average elo, scores, etc.).
 * Invariants:
 *     the range of TourneyGame refs should not be empty
 */
class Tourney {
public:
	template <typename Iter>
	Tourney(Iter begin, Iter end)
	    : begin_(begin), minDateGame_(begin) {
		ASSERT(begin != end);

		n_games_ = static_cast<gamenumT>(std::distance(begin, end));

		for (auto it = begin; it != end; it++) {
			auto& white = add_player(it->whiteID_, it->wElo_);
			white.score += RESULT_SCORE[it->result_];
			auto& black = add_player(it->blackID_, it->bElo_);
			black.score += RESULT_SCORE[RESULT_OPPOSITE[it->result_]];

			if (it->date_ < minDateGame_->date_) minDateGame_ = it;
		}
		std::sort(players_.begin(), players_.end(), // SortScoreDesc
		          [](auto& a, auto& b) { return a.score > b.score; });

		const auto eloSum = std::accumulate(players_.begin(), players_.end(),
		                                    std::make_pair(0ull, gamenumT{0}),
		                                    [](auto res, const auto& player) {
			                                    if (player.elo != 0) {
				                                    res.first += player.elo;
				                                    ++res.second;
			                                    }
			                                    return res;
		                                    });
		avgElo_ = (eloSum.second == 0)
		              ? 0
		              : static_cast<unsigned>(eloSum.first / eloSum.second);
	}

	idNumberT getEventId() const { return begin_->eventID_; }
	idNumberT getSiteId() const { return begin_->siteID_; }
	dateT getStartDate() const { return minDateGame_->date_; }
	gamenumT getStartGameNum() const { return minDateGame_->gnum_; }
	unsigned getAvgElo() const { return avgElo_; }
	gamenumT nGames() const { return n_games_; }
	unsigned nPlayers() const { return static_cast<unsigned>(players_.size()); }

	struct Player {
		idNumberT nameId;
		uint16_t score;
		eloT elo;

		bool operator==(idNumberT id) const { return nameId == id; }
	};
	const Player& getPlayer(size_t position) const {
		ASSERT(position < players_.size());
		return players_[position];
	}

private:
	std::vector<TourneyGame>::const_iterator begin_;
	std::vector<TourneyGame>::const_iterator minDateGame_;
	std::vector<Player> players_;
	gamenumT n_games_;
	unsigned avgElo_;

	Player& add_player(idNumberT nameID, eloT elo) {
		auto it = std::find(players_.begin(), players_.end(), nameID);
		if (it != players_.end()) {
			if (elo > it->elo)
				it->elo = elo;
			return *it;
		}
		players_.push_back({nameID, 0, elo});
		return players_.back();
	};
};


/**
 * class SearchTournamens - Search tournaments in a database
 *
 * This class group games in tournaments.
 * Games with the same EventId, SiteId and EventDate are considered
 * a tournament. A game with EventDate == 0 is also considered part of
 * a tournament if its date is greater than or equal to the EventDate
 * of the other games.
 * It's also possible to filter the results further by:
 *     - average elo
 *     - number of partecipants
 *     - number of games
 *     - name of a participant
 * Results can be sorted by
 *     - event name
 *     - event date
 *     - site name
 *     - number of partecipants
 *     - number of games
 *     - average elo
 *
 * Dependencies:
 * Indexes of NameBase are used for the names of the event, site and player
 * and they must be valid and unchanged throughout all the life of the
 * SearchTournament object.
 */
class SearchTournaments {
	const scidBaseT* dbase_;
	std::vector<TourneyGame> games_;
	std::vector<Tourney> tourney_;

public:
	SearchTournaments(const scidBaseT* dbase, const HFilter& filter)
	    : dbase_(dbase) {
		ASSERT(dbase != 0);
		ASSERT(filter != 0);

		games_.reserve(filter->size());
		for (auto gnum : filter) {
			games_.emplace_back(dbase->getIndexEntry(gnum), gnum);
		}

		std::sort(games_.begin(), games_.end(),
		          [](const TourneyGame& a, const TourneyGame& b) {
			          if (a.eventID_ != b.eventID_)
				          return a.eventID_ < b.eventID_;
			          if (a.siteID_ != b.siteID_)
				          return a.siteID_ < b.siteID_;
			          dateT d1 = a.eventDate_ != 0 ? a.eventDate_ : a.date_;
			          dateT d2 = b.eventDate_ != 0 ? b.eventDate_ : b.date_;
			          return d1 < d2;
		          });

		auto it = games_.begin();
		const auto it_end = games_.end();
		while (it != it_end) {
			const auto start = it;
			it = std::find_if(it, it_end, [start](const TourneyGame& g) {
				if (start->eventID_ != g.eventID_ ||
				    start->siteID_ != g.siteID_)
					return true;

				if (start->eventDate_ != 0 && g.eventDate_ == 0)
					return start->eventDate_ > g.date_;

				if (start->eventDate_ == 0 && g.eventDate_ != 0)
					return g.eventDate_ > start->date_;

				return start->eventDate_ != g.eventDate_;
			});
			tourney_.emplace_back(start, it);
		}
	}

	typedef std::vector<Tourney>::const_iterator Iter;
	Iter begin() const { return tourney_.begin(); }
	Iter end() const { return tourney_.end(); }

	void filterByAvgElo(const StrRange& range) {
		tourney_.erase(
			std::remove_if(tourney_.begin(), tourney_.end(),
				Filter<& Tourney::getAvgElo>(range)),
			tourney_.end());
	}

	void filterByNPlayers(const StrRange& range) {
		tourney_.erase(
			std::remove_if(tourney_.begin(), tourney_.end(),
				Filter<& Tourney::nPlayers>(range)),
			tourney_.end());
	}

	void filterByNGames(const StrRange& range) {
		tourney_.erase(
			std::remove_if(tourney_.begin(), tourney_.end(),
				Filter<& Tourney::nGames>(range)),
			tourney_.end());
	}

	void filterByPlayer(const char* name) {
		tourney_.erase(
			std::remove_if(tourney_.begin(), tourney_.end(),
				FilterByPlayer(name, dbase_->getNameBase())),
			tourney_.end());
	}


	bool sort(const char* criteria, size_t max);

private:
	template <uint (Tourney::* f)() const>
	class Filter {
		const StrRange& range_;

	public:
		Filter(const StrRange& range) : range_(range) {}

		bool operator()(const Tourney& t) {
			return ! range_.inRange((t.*f)());
		}
	};

	class FilterByPlayer {
		const char* name_;
		const NameBase* nb_;

	public:
		FilterByPlayer(const char* name, const NameBase* nb)
		: name_(name), nb_(nb) {}

		bool operator()(const Tourney& t) {
			for (size_t i = 0, n = t.nPlayers(); i < n; i++) {
				const char* name = nb_->GetName(NAME_PLAYER, t.getPlayer(i).nameId);
				if (strAlphaContains(name, name_)) return false;
			}
			return true;
		}
	};


	struct SortDate {
		bool operator()(const Tourney& a, const Tourney& b) {
			return a.getStartDate() > b.getStartDate();
		}
	};

	template <uint (Tourney::* f)() const>
	struct SortDesc {
		bool operator()(const Tourney& a, const Tourney& b) {
			return (a.*f)() > (b.*f)();
		}
	};

	template <nameT nt, idNumberT (Tourney::* f)() const>
	class SortId {
		const NameBase* nb_;
	public:
		SortId(const NameBase* nb) : nb_(nb) {}
		bool operator()(const Tourney& a, const Tourney& b) {
			const char* nameA = nb_->GetName(nt, (a.*f)());
			const char* nameB = nb_->GetName(nt, (b.*f)());
			return strCaseCompare(nameA, nameB) < 0;
		}
	};
};

inline bool SearchTournaments::sort(const char* criteria, size_t nOrdered) {
	static const char* criterions [] = {
		 "Date", "Elo", "Event", "Games", "Players", "Site", NULL
	};
	enum { DATE, ELO, EVENT, GAMES, PLAYERS, SITE };

	std::vector<Tourney>::iterator begin = tourney_.begin();
	std::vector<Tourney>::iterator it = (nOrdered < tourney_.size()) ?
		tourney_.begin() + nOrdered : tourney_.end();
	std::vector<Tourney>::iterator end = tourney_.end();

	switch (strUniqueMatch(criteria, criterions)) {
	case DATE:
		std::partial_sort(begin, it, end, SortDate());
		break;
	case ELO:
		std::partial_sort(begin, it, end, SortDesc<& Tourney::getAvgElo>());
		break;
	case EVENT:
		std::partial_sort(begin, it, end,
			SortId<NAME_EVENT, & Tourney::getEventId>(dbase_->getNameBase()));
		break;
	case GAMES:
		std::partial_sort(begin, it, end, SortDesc<& Tourney::nGames>());
		break;
	case PLAYERS:
		std::partial_sort(begin, it, end, SortDesc<& Tourney::nPlayers>());
		break;
	case SITE:
		std::partial_sort(begin, it, end,
			SortId<NAME_SITE, & Tourney::getSiteId>(dbase_->getNameBase()));
		break;
	default:
		return false;
	}

	return true;
}

#endif
