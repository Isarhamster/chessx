############################################################
### Comment Editor window
# Copyright (C) 2016 Fulvio Benini
#
# This file is part of Scid (Shane's Chess Information Database).
# Scid is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation.

namespace eval ::windows::commenteditor {
	variable isOpen 0
	variable w_ .commentWin
	variable needNotify_ 0
	variable undoComment_ 1
	variable undoNAGs_ 1

	proc clearComment_ {} {
		if {[sc_pos getComment] != ""} {
			undoFeature save
			sc_pos setComment ""
			set ::windows::commenteditor::needNotify_ 1
		}
		notify_ idle
	}

	proc clearNAGs_ {} {
		if {[sc_pos getNags] != 0} {
			undoFeature save
			sc_pos clearNags
			set ::windows::commenteditor::needNotify_ 1
		}
		notify_ idle
	}

	proc notify_ {wait} {
		if {$::windows::commenteditor::needNotify_} {
			after cancel "::notify::PosChanged -pgn"
			after $wait "::notify::PosChanged -pgn"
		}
	}

	proc notifyCancel_ {} {
		after cancel "::notify::PosChanged -pgn"
	}

	proc storeComment_ {} {
		variable w_
		# The "end-1c" below is because Tk adds a newline to text contents:
		set oldComment [sc_pos getComment]
		set newComment [$w_.cf.txtframe.text get 1.0 end-1c]
		if {"$oldComment" ne "$newComment"} {
			variable undoComment_
			if { $undoComment_ } {
				set undoComment_ 0
				undoFeature save
			}
			sc_pos setComment $newComment
			set ::windows::commenteditor::needNotify_ 1
		}
		notify_ 1500
	}

	proc storeNAGs_ {} {
		variable w_
		set nag_stored [sc_pos getNags]
		set nag_text [$w_.nf.text get]
		#sc_pos getNags returns 0 when empty
		if {$nag_text == ""} { set nag_text 0 }
		if {"$nag_text" ne "$nag_stored"} {
			variable undoNAGs_
			if { $undoNAGs_ } {
				set undoNAGs_ 0
				undoFeature save
			}
			sc_pos clearNags
			foreach {nag} [split "$nag_text" " "] {
				sc_pos addNag $nag
			}
			set ::windows::commenteditor::needNotify_ 1
		}
		notify_ 1500
	}

	proc updateMarkersCol_ {varname args} {
		variable w_
		if {![winfo exists $w_.mf]} { return }

		foreach b [winfo children $w_.mf.markers] {
			$b configure -foreground $::markColor
		}
	}
	trace add variable ::markColor write "::windows::commenteditor::updateMarkersCol_"

}

proc ::windows::commenteditor::createWin { {focus_if_exists 1} } {
	variable w_

	if {! [::win::createWindow $w_ [tr {WindowsComment}] 530x220]} {
		if { $focus_if_exists } {
			::win::makeVisible $w_
			focus $w_.cf.txtframe.text
		} else {
			::win::closeWindow $w_
		}
		return
	}

	# NAGs frame:
	ttk::frame $w_.nf
	ttk::label $w_.nf.label -font font_Bold -text [tr AnnotationSymbols]
	ttk::button $w_.nf.clear -text [tr Clear] -command "::windows::commenteditor::clearNAGs_"
	ttk::entry $w_.nf.text
	ttk::frame $w_.nf.b
	set i 0
	foreach {nag description} {
		! GoodMove
		? PoorMove
		!! ExcellentMove
		?? Blunder
		!? InterestingMove
		?! DubiousMove
		+-- WhiteCrushing
		--+ BlackCrushing
		+- WhiteDecisiveAdvantage
		-+ BlackDecisiveAdvantage
		+/- WhiteClearAdvantage
		-/+ BlackClearAdvantage
		+= WhiteSlightAdvantage
		=+ BlackSlightAdvantage
		= Equality
		~ Unclear
		N Novelty
		D Diagram
	} {
		ttk::button $w_.nf.b.b$i -text "$nag" -width 3 -command "::addNag $nag"
		::utils::tooltip::Set $w_.nf.b.b$i $description
		grid $w_.nf.b.b$i -row [expr {$i % 2}] -column [expr {int($i / 2)}] -padx 2 -pady 2
		incr i
	}
	grid columnconfig $w_.nf 0 -weight 1
	grid $w_.nf.label $w_.nf.clear -sticky nsew
	grid $w_.nf.text -sticky nsew -columnspan 2
	grid $w_.nf.b -sticky nsew -columnspan 2

	# Comment frame:
	ttk::frame $w_.cf
	ttk::label $w_.cf.label -font font_Bold -text [tr Comment]
	ttk::button $w_.cf.clear -text [tr Clear] -command "::windows::commenteditor::clearComment_"
	autoscrollText y $w_.cf.txtframe $w_.cf.txtframe.text Treeview
	$w_.cf.txtframe.text configure -wrap word -state normal
	grid rowconfig $w_.cf 1 -weight 1
	grid columnconfig $w_.cf 0 -weight 1
	grid $w_.cf.label $w_.cf.clear -sticky nsew
	grid $w_.cf.txtframe -sticky nsew -columnspan 2

	### Markers frame
	ttk::frame $w_.mf
	ttk::label $w_.mf.header -font font_Bold -text $::tr(InsertMark:)
	set usage "Usage (on the main board):\n"
	append usage "ctrl+click --> mark square\n"
	append usage "ctrl+drag --> draw arrow"
	ttk::label $w_.mf.usage -text "$usage"
	ttk::frame $w_.mf.colors
	set i 0
	foreach color {
		green
		red
		orange
		yellow
		blue
		darkBlue
		purple
		white
		black
		gray
	} {
		radiobutton $w_.mf.colors.col_$color \
			-indicatoron "false" \
			-background "$color" -selectcolor "$color" \
			-text " " -width 2 \
			-variable "::markColor" -value "$color"
		grid $w_.mf.colors.col_$color -row [expr {$i / 2}] -column [expr {int($i % 2)}]
		incr i
	}
	ttk::frame $w_.mf.markers
	set i 0
	foreach {marker lbl} {
		full █
		circle ◯
		disk ⬤
		+ +
		- -
		X X
		! !
		? ?
		= =
		A A
		B B
		C C
		D D
		E E
		F F
		0 0
		1 1
		2 2
		3 3
		4 4
		5 5
		6 6
		7 7
		8 8
		9 9
	} {
		radiobutton $w_.mf.markers.mark_$marker \
			-indicatoron "false" \
			-foreground "$::markColor" -background "light gray" -selectcolor "dark gray" \
			-text "$lbl" -width 2 \
			-variable "::markType" -value "$marker"
		grid $w_.mf.markers.mark_$marker -row [expr {$i % 5}] -column [expr {int($i / 5)}]
		incr i
	}
	grid $w_.mf.header -columnspan 2 -sticky nsew -padx 8
	grid $w_.mf.usage -columnspan 2 -sticky nsew -padx 8
	grid $w_.mf.colors -row 2 -column 0 -sticky nsew -padx 8 -pady 8
	grid $w_.mf.markers -row 2 -column 1 -sticky nsew -pady 8 -padx 8

	# Arrange frames:
	grid $w_.cf -row 0 -column 0 -columnspan 2 -sticky nsew
	grid $w_.mf -row 0 -rowspan 2 -column 2 -sticky nsew
	grid $w_.nf -row 1 -column 0 -columnspan 2 -sticky nsew
	grid rowconfig $w_ 0 -weight 1
	grid columnconfig $w_ 0 -weight 1

	# Load current NAGs and comment
	Refresh

	# Add bindings at the end
	bind $w_ <Destroy> "if {\[string equal $w_ %W\]} { set ::windows::commenteditor::isOpen 0; ::windows::commenteditor::notify_ 1 }"
	bind $w_.nf.text <KeyPress>   "::windows::commenteditor::notifyCancel_"
	bind $w_.nf.text <KeyRelease> "::windows::commenteditor::storeNAGs_"
	bind $w_.cf.txtframe.text <KeyPress>   "::windows::commenteditor::notifyCancel_"
	bind $w_.cf.txtframe.text <KeyRelease> "::windows::commenteditor::notify_ 1000"
	bind $w_.cf.txtframe.text <<Modified>> "
		::windows::commenteditor::storeComment_;
		$w_.cf.txtframe.text edit modified false
	"

	set ::windows::commenteditor::isOpen 1
	$w_.cf.txtframe.text edit modified false
	after idle focus $w_.cf.txtframe.text
}

# Gets the comment and NAGs for the current position and updates the
# corresponding widgets.
# Disables NAGs widgets if at vstart (NAGs cannot be inserted before moves).
proc ::windows::commenteditor::Refresh {} {
	variable w_
	if {![winfo exists $w_]} { return }

	variable needNotify_ 0
	variable undoNAGs_ 1
	variable undoComment_ 1

	set comment [sc_pos getComment]
	if {$comment != [$w_.cf.txtframe.text get 1.0 end-1c]} {
		$w_.cf.txtframe.text delete 1.0 end
		$w_.cf.txtframe.text insert end $comment
	}

	set nag [sc_pos getNags]
	$w_.nf.text configure -state normal
	$w_.nf.text delete 0 end
	if {$nag != "0"} {
		$w_.nf.text insert end $nag
	}
	# if at vstart, disable NAG codes
	if {[sc_pos isAt vstart]} {
		set state "disabled"
	} else	{
		set state "normal"
	}
	$w_.nf.clear configure -state $state
	$w_.nf.text configure -state $state
	foreach c [winfo children $w_.nf.b] {
		$c configure -state $state
	}
}

proc makeCommentWin {{toggle ""}} {
	::windows::commenteditor::createWin [string compare "$toggle" "toggle"]
}
