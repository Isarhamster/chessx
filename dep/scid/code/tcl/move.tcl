# Copyright (C) 2009-2015 Fulvio Benini
#
# This file is part of Scid (Shane's Chess Information Database).
#
# Scid is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation.
#
# Scid is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Scid.  If not, see <http://www.gnu.org/licenses/>.

### move.tcl
### Functions for moving within a game.

namespace eval ::move {}

proc ::move::drawVarArrows {} {
	if {! $::showVarArrows || $::autoplayMode ||
		([info exists ::playMode] && [eval "$::playMode drawVarArrows"] == 0)} {
		return 0
	}

	set bDrawArrow 0
	set varList [sc_var list UCI]

	if {$varList != ""} {
		set move [sc_game info nextMoveUCI]
		if {$move != ""} { set varList [linsert $varList 0 $move] }
		foreach { move } $varList {
			set bDrawn 0
			set sq_start [ ::board::sq [ string range $move 0 1 ] ]
			set sq_end [ ::board::sq [ string range $move 2 3 ] ]
			foreach mark $::board::_mark(.main.board) {
				if { [lindex $mark 0] == "arrow" } {
					if {[lindex $mark 1] == $sq_start && [lindex $mark 2] == $sq_end} {
						set bDrawn 1
						break
					}
				}
			}
			if {! $bDrawn } { set bDrawArrow 1; break }
		}
	}

	return $bDrawArrow
}

proc ::move::showVarArrows {} {
	set move [sc_game info nextMoveUCI]
	if {$move != ""} {
		set sq_start [ ::board::sq [ string range $move 0 1 ] ]
		set sq_end [ ::board::sq [ string range $move 2 3 ] ]
		::board::mark::add ".main.board" "arrow" $sq_start $sq_end "green"
	}
	set varList [sc_var list UCI]
	foreach { move } $varList {
		set sq_start [ ::board::sq [ string range $move 0 1 ] ]
		set sq_end [ ::board::sq [ string range $move 2 3 ] ]
		::board::mark::add ".main.board" "arrow" $sq_start $sq_end "blue"
	}
}

proc ::move::Start {} {
	if {[info exists ::playMode] && [eval "$::playMode moveStart"] == 0} {
		return
	}
	sc_move start
	updateBoard
	if {[::move::drawVarArrows]} { ::move::showVarArrows }
}

proc ::move::End {} {
	if {[info exists ::playMode] && [eval "$::playMode moveEnd"] == 0} {
		return
	}
	sc_move end
	updateBoard
	if {[::move::drawVarArrows]} { ::move::showVarArrows }
}

proc ::move::EndVar {} {
	if {[info exists ::playMode] && [eval "$::playMode moveEnd"] == 0} {
		return
	}
	sc_move endVar
	updateBoard
	if {[::move::drawVarArrows]} { ::move::showVarArrows }
}

proc ::move::ExitVar {} {
	if {[sc_var level] == 0 } { return 0; }
	if {[info exists ::playMode] && [eval "$::playMode moveExitVar"] == 0} {
		return
	}
	sc_var exit;
	updateBoard
	if {[::move::drawVarArrows]} { ::move::showVarArrows }
}

proc ::move::ExitVarOrStart {} {
	if {[::move::ExitVar] eq 0} {
		::move::Start
	}
}

proc ::move::Back {{count 1}} {
	if {[sc_pos isAt start]} { return }
	if {[sc_pos isAt vstart]} { ::move::ExitVar; return }
	if {[info exists ::playMode] && [eval "$::playMode moveBack"] == 0} {
		return
	}

	sc_move back $count

	if {[sc_pos isAt vstart]} { sc_var exit }

	if {$count == 1} {
		updateBoard -animate
		::utils::sound::AnnounceBack
	} else {
		updateBoard
	}

	if {[::move::drawVarArrows]} { ::move::showVarArrows }
}

proc ::move::Forward {{count 1}} {
	if {[sc_pos isAt end] || [sc_pos isAt vend]} { return }
	if {[info exists ::playMode] && [eval "$::playMode moveForward"] == 0} {
		return
	}

	set bArrows [::move::drawVarArrows]

	set move [sc_game info next]
	if {$count == 1} {
		if {[sc_var count] != 0 && ! $::autoplayMode && $::showVarPopup} {
			showVars
			set bArrows $::showVarArrows
		} else {
			if {! $bArrows} { sc_move forward }
		}

		# Animate and speak this move:
		updateBoard -animate
		::utils::sound::AnnounceForward $move
	} else {
		if {! $bArrows} { sc_move forward $count }
		updateBoard
	}
	if {$bArrows} { ::move::showVarArrows }
}

#Follow the main line or enter a variation
#Return 0 for main line, 1 for 1st variation, etc...
proc ::move::Follow {{moveUCI}} {
	if {$moveUCI != "null"} {
		set moveUCI2 "[string range $moveUCI 2 3][string range $moveUCI 0 1][string range $moveUCI 4 end]"
	} else {
		set moveUCI2 "0000"
	}
	set varList [sc_var list UCI]
	set varList [linsert $varList 0 "[sc_game info nextMoveUCI]" ]
	set i -1
	foreach {move} $varList {
		if { [ string compare -nocase $moveUCI $move] == 0 || \
			 [ string compare -nocase $moveUCI2 $move] == 0 } {
			if {$i == -1} {
				sc_move forward
			} else {
				sc_var moveInto $i
			}
			return 1
		}
		incr i
	}
	return 0
}

proc ::move::PGNOffset { location } {
	sc_move pgn $location
	updateBoard
	if {[::move::drawVarArrows]} { ::move::showVarArrows }
}
