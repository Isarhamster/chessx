###
### windows/stats.tcl: Filter Statistics window for Scid
###

set ::windows::stats::isOpen 0
set ::windows::stats::like_graphyear 1
set ::windows::stats::like_graphelo 1
set ::windows::stats::old_year 0
set ::windows::stats::old_elo 0
set ::windows::stats::stat_year 0
set ::windows::stats::statelo 0

proc ::windows::stats::Open {} {
  set w .statsWin
  if {[winfo exists $w]} {
    focus .
    destroy $w
    set ::windows::stats::isOpen 0
    return
  }
  win::createDialog $w
#  toplevel $w
  keyboardShortcuts $w
  wm title $w "Scid: $::tr(FilterStatistic)"
  setWinLocation $w
  bind $w <Configure> "recordWinSize $w"

#  use autoscrollframe to display large infos
  autoscrollText y $w.statsasb $w.stats Treeview
  $w.stats configure -width 64 -height 40 -state normal -font font_Fixed -wrap none
#  autoscrollframe $w.statsasb text $w.stats -width 64 -height 40\
#      -foreground black -background white -font font_Fixed\
#       -wrap none

  ttk::frame $w.fbuttons
  ttk::button $w.fbuttons.print -image tb_save -command {
    set ftype {
      { "Text files" {".txt"} }
      { "All files"  {"*"}    }
    }
    set fname [tk_getSaveFile -initialdir [pwd] -filetypes $ftype -title "Save text file"]
    if {$fname != ""} {
      if {[catch {set tempfile [open $fname w]}]} {
        tk_messageBox -title "Scid: Error saving file" -type ok -icon warning \
          -message "Unable to save the file: $fname\n\n"
      } else {
        puts $tempfile [.statsWin.stats get 1.0 end-1c]
        close $tempfile
      }
    }
  }
  ttk::checkbutton $w.fbuttons.graphyear -text $::tr(Year) \
      -variable ::windows::stats::like_graphyear -command ::windows::stats::refresh_wnd
  ttk::checkbutton $w.fbuttons.graphelo  -text $::tr(Rating) \
      -variable ::windows::stats::like_graphelo -command ::windows::stats::refresh_wnd
  ttk::checkbutton $w.fbuttons.statelo -text "StatElo" \
      -variable ::windows::stats::statelo -command ::windows::stats::refresh_wnd
  ttk::checkbutton $w.fbuttons.statyear -text "StatYear" \
      -variable ::windows::stats::stat_year -command ::windows::stats::refresh_wnd
  ttk::checkbutton $w.fbuttons.old_elo -text "OldElo" \
      -variable ::windows::stats::old_elo -command ::windows::stats::refresh_wnd
  ttk::checkbutton $w.fbuttons.oldyear -text "OldYear" \
      -variable ::windows::stats::old_year -command ::windows::stats::refresh_wnd

  ttk::button $w.fbuttons.setup -image tb_config -command configureFilterGraph
  pack $w.fbuttons.graphyear $w.fbuttons.graphelo $w.fbuttons.statyear $w.fbuttons.statelo $w.fbuttons.oldyear $w.fbuttons.old_elo -side left
  pack $w.fbuttons.setup $w.fbuttons.print -side right -padx "5 0" -pady 2
  pack $w.fbuttons -side top -fill x
  pack $w.statsasb -side top -fill both -expand yes
  set ::windows::stats::isOpen 1
  bind $w <Control-q> "destroy $w"
  bind $w <Escape> "destroy $w"
  bind $w <F1> { helpWindow Index }
  bind $w <Destroy> {
    set ::windows::stats::isOpen 0
  }

  wm resizable $w 0 1

  ::windows::stats::refresh_wnd
}

# TODO: remove this function
proc ::windows::stats::Refresh {} {
  ::notify::DatabaseModified $::curr_db dbfilter
}

proc ::windows::stats::refresh_wnd {} {
  global FilterMaxMoves FilterMinMoves FilterStepMoves FilterMaxElo FilterMinElo FilterStepElo FilterMaxYear FilterMinYear FilterStepYear FilterGuessELO
  variable display

  if {! [winfo exists .statsWin]} { return }

  # Set up variables for translated phrases:
  set all [::utils::string::Capital $::tr(allGames)]
  set both $::tr(OprepStatBoth)
  set since $::tr(OprepStatSince)
  set games [::utils::string::Capital $::tr(games)]
  set score [::utils::string::Capital $::tr(score)]

  # Find length of longest left-hand column:
  set alen [expr {[string length $all] + 1}]
  set blen [expr {[string length $both] + 7}]
  set slen [expr {[string length $since] + 12}]
  set len $alen
  if {$len < $blen} { set len $blen }
  if {$len < $slen} { set len $slen }

  set height 4
  set ratings 0
  set years 0
  set rlist [lsort -decreasing [array names display r*]]
  set ylist [lsort [array names display y*]]

  foreach i $rlist {
    if {$display($i)} { set ratings 1 }
  }
  foreach i $ylist {
    if {$display($i)} { set years 1 }
  }

  if {$ratings} { incr height }
  if {$years} { incr height }

  set s ""
  set sm "\n"  
  set stat ""
  append s " [::utils::string::Pad $stat [expr $len - 4]] [::utils::string::PadRight $games 10]"
  append s "     1-0     =-=     0-1 [::utils::string::PadRight $score 8]\n"
  append s "--------------------------------------------------------------"
  append s "\n [::utils::string::Pad $all $len]" [sc_filter stats all]
# New Statistic: Count the games in intervalls "start elo  - end elo"
#         if elo is deselected in option menu, then enlarge the intervall to next selectet elo.
# New Statistic: Count the games in intervalls
    if { $::windows::stats::like_graphelo } {
	set endElo $FilterMaxElo
	set startElo $FilterMinElo
	for {set i $startElo} {$i <= $endElo} {set i [expr {$i + $FilterStepElo}]} {
	    incr height
	    incr height
	    set nelo [expr {$i+$FilterStepElo}]
	    # shorten gap between 0 and "useful" ratings 1800
	    set j $i
	    if { $i < 100 } { set i [expr { 1800 - $FilterStepElo}] }
	    set stat "min. Elo $i-$nelo"
	    append s "\n [::utils::string::Pad $stat $len]"   [sc_filter stats elo $i $nelo]
	    set stat "max. Elo $i-$nelo"
	    # +10000 workaround to trigger max elo in filter function
	    append sm "\n [::utils::string::Pad $stat $len]"   [sc_filter stats elo $i [expr {$nelo + 10000}]]
	}
    }
    if {$::windows::stats::statelo && $ratings} {
	append s "\n"
	set j 0
	set k [lindex $rlist $j]
	while { $k!= "" && ! $display($k) } {
	    incr j
	    set k [lindex $ylist $j]
	    if { $k == "" } { break }
	}
	set nelo [string range [lindex $rlist $j] 1 end]
	incr height
	#first line searches all games greater 2600 Elo
	set stat "min. Elo $nelo-3500"
	append s "\n [::utils::string::Pad $stat $len]"   [sc_filter stats elo $nelo 9999]
	set stat "max. Elo $nelo-3500"
	append sm "\n [::utils::string::Pad $stat $len]"   [sc_filter stats elo $nelo -9999]
	set j 0
	foreach i $rlist {
	    incr j
	    if {$display($i)} {
		incr height
		set elo [string range $i 1 end]
		set l $j
		set k [lindex $rlist $l]
		while { $k!= "" && ! $display($k) } {
		    incr l
		    set k [lindex $rlist $l]
		    if { $k == "" } { break }
		}
		set nelo [string range [lindex $rlist $l] 1 end]
		if { $nelo == "" } { set nelo 0 }
		#count all games where player whith lowest Elo is in the specific range
		set stat "min. Elo $nelo-$elo"
		append s "\n [::utils::string::Pad $stat $len]"   [sc_filter stats elo $nelo $elo]
		set stat "max. Elo $nelo-$elo"
		#count all games where player whith highest Elo is in the specific range
		# +10000 workaround to trigger max elo in filter function
		append sm "\n [::utils::string::Pad $stat $len]"   [sc_filter stats elo $nelo [expr {$elo + 10000}]]
	    }
	}
    }
    append s $sm
    append s "\n"
# New Statistic: Count the games in intervalls "from year - to year"
# if year is deselected in option menu, then enlarge the intervall to next selectet year.
    if { $::windows::stats::like_graphyear } {
	set endYear $FilterMaxYear
	set startYear $FilterMinYear
	for {set i $startYear} {$i <= $endYear} {set i [expr {$i + $FilterStepYear}]} {
	    incr height
	    set ie [expr {$i+$FilterStepYear}]
	    set stat "$i - $ie"
	    append s "\n [::utils::string::Pad $stat $len]"   [sc_filter stats year $i $ie]
	}
    }
    if {$::windows::stats::stat_year && $years} {
# this statistic ignores the intervall and uses 1 Year as intervall
	set j 0
	foreach i $ylist {
	    incr j
	    if {$display($i)} {
		incr height
		set year [string range $i 1 end]
		set l $j
		set k [lindex $ylist $l]
		while { $k!= "" && ! $display($k) } {
		    incr l
		    set k [lindex $ylist $l]
		    if { $k == "" } { break }
		}
		set nyear [string range $k 1 end]
		if { $nyear == "" } { set nyear 2099 }
		set stat "$year - $nyear"
		append s "\n [::utils::string::Pad $stat $len]"   [sc_filter stats year $year $nyear]
	    }
	}
    }
#Old statistic: count the games from specific value to maximum value 
  set stat ""
  if { $::windows::stats::old_elo || $::windows::stats::old_year} {
      append s "\n\n"
      append s "OldYear/OldElo[::utils::string::Pad $stat [expr $len - 17]] [::utils::string::PadRight $games 10]"
      append s "     1-0     =-=     0-1 [::utils::string::PadRight $score 8]\n"
      append s "--------------------------------------------------------------"
      append s "\n [::utils::string::Pad $all $len]" [sc_filter stats all]
  }
  if {$ratings && $::windows::stats::old_elo} {
    append s "\n"
    foreach i $rlist {
      if {$display($i)} {
        incr height
        set elo [string range $i 1 end]
        set stat "$both $elo+"
        append s "\n [::utils::string::Pad $stat $len]"   [sc_filter stats elo $elo 0]
      }
    }
  }

  if {$years && $::windows::stats::old_year} {
    append s "\n"
    foreach i $ylist {
      if {$display($i)} {
        incr height
        set year [string range $i 1 end]
        set stat "$since $year.01.01"
        append s "\n [::utils::string::Pad $stat $len]"   [sc_filter stats year $year 0]
      }
    }
  }
  append s "\n"

  set w .statsWin.stats
  if { $height > 50  } { set height 50 }  
  $w configure -state normal
  $w delete 1.0 end
  $w insert end $s
  $w tag configure bgGray -background gray95
  # Shade every second line to help readability:
  set lastLineNum [expr {int([$w index end])}]
#TODO: active code when second color for textbox available
#  for {set i 2} {$i <= $lastLineNum} {incr i 2} {
#    $w tag add bgGray $i.0 "$i.0 lineend +1c"
#  }
  $w tag configure blue -foreground darkBlue
  $w tag configure red -foreground red
  $w tag add blue 1.0 2.0
  $w tag add red 2.0 3.0
  $w configure -height $height
  $w configure -state disabled
}
