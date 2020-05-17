### graph.tcl: part of Scid.

# ::tools::graphs::Save
#
#   Saves a graph (e.g. tree graph, filter graph, rating graph) to a
#   color or greyscale Postscript file.
#
#   The mode should be "color" or "gray".
#
proc ::tools::graphs::Save {mode w} {
  if {! [winfo exists $w]} { return }
  set ftypes {{"PostScript files" {.eps .ps}} {"All files" *}}
  set fname [tk_getSaveFile -filetypes $ftypes -parent $w -defaultextension ".eps" -title "Scid: Save Graph"]
  if {$fname == ""} { return }
  
  if {[file extension $fname] != ".eps" && [file extension $fname] != ".ps" } {
    append fname ".eps"
  }
  
  if {[catch {$w postscript -file $fname -colormode $mode} result]} {
    tk_messageBox -icon info -parent $w -title "Scid" -message $result
  }
}

########################################
# Configure Filter graph window

if { ! [info exists FilterMinElo] } {
  set FilterMinElo 2100
  set FilterMaxElo 2800
  set FilterStepElo 100
  set FilterGuessELO 1
  set FilterMaxYear [clock format [clock seconds] -format {%Y}]
  set FilterMinYear [expr {$FilterMaxYear - 12}]
  set FilterStepYear 1
  set FilterMinMoves 5
  set FilterMaxMoves 80
  set FilterStepMoves 1
}

#Check for illegal Values and set to default values
proc checkConfigFilterGraph {} {
  global FilterMaxMoves FilterMinMoves FilterStepMoves FilterMaxElo FilterMinElo FilterStepElo FilterMaxYear FilterMinYear FilterStepYear
  if { $FilterStepMoves < 1 } { set FilterStepMoves 1 }
  if { $FilterStepElo < 1 } { set FilterStepElo 100 }
  if { $FilterStepYear < 1 } { set FilterStepYear 1 }
  if { $FilterMinMoves < 1 } { set FilterMinMoves 5 }
  if { $FilterMinElo < 0 } { set FilterMinElo 2100 }
  if { $FilterMinYear < 1 } { set FilterMinYear [expr {$FilterMaxYear - 12}] }
  if { $FilterMaxMoves < 1 } { set FilterMaxMoves 80 }
  if { $FilterMaxElo < 1 } { set FilterMaxElo 2800 }
  if { $FilterMaxYear < 1 } { set FilterMaxYear [clock format [clock seconds] -format {%Y}] }
}

proc configureFilterGraph {} {
  global FilterMaxMoves FilterMinMoves FilterStepMoves FilterMaxElo FilterMinElo FilterStepElo FilterMaxYear FilterMinYear FilterStepYear FilterGuessELO
  
  set w .configFilterGraph
  if {[winfo exists $w]} {
    destroy $w
  }
  
  win::createDialog $w
  wm title $w $::tr(ConfigureFilter)
  setWinLocation $w
  bind $w <F1> {helpWindow Graphs Filter}
  ttk::labelframe $w.filternew -text "Config $::tr(Year) $::tr(Rating)"
  set col 0
  set row 0
  #Create input for each configurationvalue
  foreach { i n } { Year Year Elo Rating Moves moves} {
    ttk::label $w.filternew.label$i -text $::tr($n): -font font_Bold
    grid $w.filternew.label$i -row $row -column $col -sticky w
    incr col
    foreach {j k} { FilterMin "  " FilterMax " - " FilterStep "  Interval:"} {
      ttk::label $w.filternew.label$j$i -text $k
      ttk::entry $w.filternew.i$j$i -textvariable $j$i -justify right -width 5 -validate all -validatecommand { regexp {^[0-9]{0,4}$} %P }
      grid $w.filternew.label$j$i -row $row -column $col -sticky w
      incr col
      grid $w.filternew.i$j$i -row $row -column $col -sticky w
      incr col
    }
    if { $i == "Elo" } {
      ttk::checkbutton $w.filternew.iEloGuess -text $::tr(FilterEstimate) -onvalue 1 -offvalue 0 -variable FilterGuessELO
      grid $w.filternew.iEloGuess -row $row -column $col -sticky w -padx "5 0"
    }
    incr row
    set col 0
  }
  if { [winfo exists .statsWin]} {
      ttk::labelframe $w.filterold -text "Config StatYear StatElo OldYear OldElo"
      foreach { i h v s } { Elo OprepStatBoth r* "+" Year OprepStatSince y* ".01.01" } {
         ttk::frame $w.filterold.old$i
         ttk::label $w.filterold.old$i.label -textvariable ::tr($h) -font font_Bold
         ttk::treeview $w.filterold.old$i.list -columns {0} -show {} -selectmode extended \
             -yscrollcommand "$w.filterold.old$i.ybar set"
         $w.filterold.old$i.list column 0 -width 100
         $w.filterold.old$i.list configure -height 7
         ttk::scrollbar $w.filterold.old$i.ybar -command "$w.filterold.old$i.list yview"
         pack $w.filterold.old$i.label -side top -anchor w
         pack $w.filterold.old$i.ybar -side right -fill y
         pack $w.filterold.old$i.list -side left -fill both -expand 1
         set j 0
         set il {}
         foreach y [lsort -decreasing [array names ::windows::stats::display $v]] {
             set value [string range $y 1 end]
             $w.filterold.old$i.list insert {} end -id $j -values "$value$s"
             if { $::windows::stats::display($y) } {
                 lappend il $j
             }
             incr j
         }
         $w.filterold.old$i.list selection set $il
      }
      pack $w.filterold.oldElo -side left -expand 1 -in $w.filterold -fill both -padx "0 10"
      pack $w.filterold.oldYear -side left -expand 1 -fill both -in $w.filterold
  }

  ttk::frame $w.buttons
  ttk::button $w.buttons.close -textvar ::tr(Close) -command {
    checkConfigFilterGraph; ::tools::graphs::filter::Refresh
    ::tools::graphs::absfilter::Refresh; destroy .configFilterGraph  }
  ttk::button $w.buttons.standard -textvar ::tr(Defaults) -command {
    set FilterMinElo 2100
    set FilterMaxElo 2800
    set FilterStepElo 100
    set FilterMaxYear [clock format [clock seconds] -format {%Y}]
    set FilterMinYear [expr {$FilterMaxYear - 12}]
    set FilterStepYear 1
    set FilterMinMoves 5
    set FilterMaxMoves 80
    set FilterStepMoves 1
    set FilterGuessELO 1
  }
  if { [winfo exists .statsWin]} {
     ttk::button $w.buttons.update -textvar ::tr(Update) -command { checkConfigFilterGraph
       set j 0
       set sel [.configFilterGraph.filterold.oldYear.list selection]
       foreach i [lsort -decreasing [array names ::windows::stats::display y*]] {
           set ::windows::stats::display($i) 0
           incr j
       }
       foreach i $sel {
           set ::windows::stats::display(y[string range [.configFilterGraph.filterold.oldYear.list set $i 0] 0 3]) 1
       }
       set j 0
       set sel [.configFilterGraph.filterold.oldElo.list selection]
       foreach i [lsort -decreasing [array names ::windows::stats::display r*]] {
           set ::windows::stats::display($i) 0
           incr j
       }
       foreach i $sel {
           set ::windows::stats::display(r[string range [.configFilterGraph.filterold.oldElo.list set $i 0] 0 3]) 1
       }
       ::tools::graphs::absfilter::Refresh;
       ::tools::graphs::filter::Refresh
       ::windows::stats::refresh_wnd
     }
  } else {
     ttk::button $w.buttons.update -textvar ::tr(Update) -command { checkConfigFilterGraph
        ::tools::graphs::absfilter::Refresh;
        ::tools::graphs::filter::Refresh
     }
  }
  
  pack $w.filternew -side top -fill x
  if { [winfo exists .statsWin]} {
      pack $w.filterold -side top -fill both -expand 1 -pady "10 0"
  }
  pack $w.buttons -anchor e
  packdlgbuttons $w.buttons.close $w.buttons.update $w.buttons.standard
  focus $w.filternew.iFilterMinYear
  bind $w <Configure> "recordWinSize $w"
}

#####################
# Filter graph window

# ::tools::graphs::filter::type
#   can be "decade", "year" or "elo" , "move"
#
set ::tools::graphs::filter::type year

proc tools::graphs::filter::Open {} {
  global filterGraph
  set w .fgraph
  if {[winfo exists $w]} {
    focus .
    destroy $w
    set filterGraph 0
    return
  }
  win::createDialog $w 0
  menu $w.menu
  ::setMenu $w $w.menu
  $w.menu add cascade -label GraphFile -menu $w.menu.file
  configMenuText $w.menu 0 GraphFile $::language
  menu $w.menu.file
  $w.menu.file add command -label GraphFileColor -command "::tools::graphs::Save color $w.c"
  configMenuText $w.menu.file 0 GraphFileColor $::language
  $w.menu.file add command -label GraphFileGrey -command "::tools::graphs::Save gray $w.c"
  configMenuText $w.menu.file 1 GraphFileGrey $::language
  $w.menu.file add separator
  $w.menu.file add command -label GraphFileClose -command "destroy $w"
  configMenuText $w.menu.file 3 GraphFileClose $::language
  wm title $w $::tr(TitleFilterGraph)
  set filterGraph 1
  bind $w <Destroy> {set filterGraph 0}
  
  ttk::frame $w.b
  pack $w.b -side top -fill x
  ttk::label $w.b.status -width 1 -font font_Small -anchor w
  
  canvas $w.c -width 600 -height 400 -selectforeground [ttk::style lookup . -foreground] -background [ttk::style lookup . -background]
  $w.c create text 25 5 -tag title -justify center -width 1 \
      -font font_Small -anchor n
  $w.c create text 250 295 -tag type -justify center -width 1 \
      -font font_Small -anchor s
  pack $w.c -side top -expand yes -fill both
  ::utils::graph::create filter
  
  bind $w <F1> {helpWindow Graphs Filter}
  bind $w <Configure> {
    .fgraph.c itemconfigure title -width [expr {[winfo width .fgraph.c] - 50}]
    .fgraph.c coords title [expr {[winfo width .fgraph.c] / 2}] 10
    .fgraph.c itemconfigure type -width [expr {[winfo width .fgraph.c] - 50}]
    .fgraph.c coords type [expr {[winfo width .fgraph.c] / 2}] \
        [expr {[winfo height .fgraph.c] - 10}]
    ::utils::graph::configure filter -height [expr {[winfo height .fgraph.c] - 80}]
    ::utils::graph::configure filter -width [expr {[winfo width .fgraph.c] - 60}]
    ::utils::graph::redraw filter
  }
  bind $w.c <1> tools::graphs::filter::Switch
  bind $w.c <$::MB3> ::tools::graphs::filter::Refresh
  
  foreach {name text} {decade Decade year Year elo Rating move moves} {
    ttk::radiobutton $w.b.$name -text $::tr($text) \
        -variable ::tools::graphs::filter::type -value $name \
        -command ::tools::graphs::filter::Refresh
    pack $w.b.$name -side left -padx 1 -pady 2
  }
  ttk::button $w.b.setup -image tb_config -command configureFilterGraph
  pack $w.b.decade $w.b.elo -side left -padx 1 -pady 2
  pack $w.b.setup -side right -pady 2
  pack $w.b.status -side left -padx 2 -pady 2 -fill x -expand yes
  
  ::tools::graphs::filter::Refresh
}

proc tools::graphs::filter::Switch {} {
  variable type
  switch $type {
    "decade" { set type "year" }
    "year" { set type "elo" }
    "elo" { set type "move" }
    "move" { set type "decade" }
  }
  ::tools::graphs::filter::Refresh
}

proc ::tools::graphs::filter::Refresh {} {
  global FilterMaxMoves FilterMinMoves FilterStepMoves FilterMaxElo FilterMinElo FilterStepElo FilterMaxYear FilterMinYear FilterStepYear FilterGuessELO
  
  set w .fgraph
  if {! [winfo exists $w]} { return }
  
  $w.c itemconfigure title -width [expr {[winfo width $w.c] - 50}]
  $w.c coords title [expr {[winfo width $w.c] / 2}] 10
  $w.c itemconfigure type -width [expr {[winfo width $w.c] - 50}]
  $w.c coords type [expr {[winfo width $w.c] / 2}] \
      [expr {[winfo height $w.c] - 10}]
  set height [expr {[winfo height $w.c] - 80}]
  set width [expr {[winfo width $w.c] - 60}]
  set vlines {}
  if {$::tools::graphs::filter::type == "elo"} {
    # Vertical lines for Elo-range graph:
    for {set i 1} {$i <=  $FilterMaxElo- $FilterMinElo} { incr i } {
      lappend vlines [list gray80 1 at $i.5]
    }
  } elseif {$::tools::graphs::filter::type == "year"} {
    # Vertical lines for Year-range graph:
    for {set i 1} {$i <= $FilterMaxYear- $FilterMinYear} {incr i } {
      lappend vlines [list gray80 1 at $i.5]
    }
  } elseif {$::tools::graphs::filter::type == "decade"} {
    # Vertical lines for Decade graph: most are gray, but those
    # just before 1950s and 2000s are blue to make them stand out.
    for {set i 1} {$i < 10} {incr i} {
      set vlineColor gray80
      if {$i == 4  ||  $i == 9} { set vlineColor steelBlue }
      lappend vlines [list $vlineColor 1 at $i.5]
    }
  }
  
  ::utils::graph::create filter -width $width -height $height -xtop 40 -ytop 35 \
      -ytick 1 -xtick 1 -font font_Small -canvas $w.c -textcolor black \
      -vline $vlines -background lightYellow -tickcolor black -xmin 0 -xmax 1
  ::utils::graph::redraw filter
  busyCursor .
  update
  
  set count 0
  set dlist {}
  set xlabels {}
  set max 0.0
  
  # Generate plot values and labels:
  if {$::tools::graphs::filter::type == "decade"} {
    set ftype date
    set typeName $::tr(Decade)
    set rlist [list 0000 1919 -1919  1920 1929 20-29 \
        1930 1939 30-39  1940 1949 40-49  1950 1959 50-59 \
        1960 1969 60-69  1970 1979 70-79  1980 1989 80-89 \
        1990 1999 90-99  2000 2009 2000+]
  } elseif {$::tools::graphs::filter::type == "year"} {
    set ftype date
    set typeName $::tr(Year)
    set endYear $FilterMaxYear
    set startYear $FilterMinYear
    set rlist {}
    for {set i $startYear} {$i <= $endYear} {set i [expr {$i + $FilterStepYear}]} {
      lappend rlist $i
      lappend rlist [expr {$i+$FilterStepYear-1}]
      lappend rlist [expr {$i+$FilterStepYear/2}]
    }
  } elseif {$::tools::graphs::filter::type == "elo"} {
    set ftype elo
    set typeName $::tr(Rating)
    #    set rlist [list 0 1999 0-1999  2000 2099 20xx  2100 2199 21xx  \
    2200 2299 22xx  2300 2399 23xx  2400 2499 24xx  \
        2500 2599 25xx  2600 2699 26xx  2700 3999 2700+]
    set endElo $FilterMaxElo
    set startElo $FilterMinElo
    set rlist {}
    for {set i $startElo} {$i <= $endElo} {set i [expr {$i + $FilterStepElo}]} {
      lappend rlist $i
      lappend rlist [expr {$i+$FilterStepElo-1}]
      # shorten gap between 0 and "useful" ratings 1800
      set j $i
      if { $i < 100 } { set i [expr { 1800 - $FilterStepElo}] }
      lappend rlist [concat $j-[expr {$i+$FilterStepElo-1}]]
    }
  } else {
    set ftype move
    set typeName $::tr(moves)
    set startMove $FilterMinMoves
    set endMove  $FilterMaxMoves
    set rlist {}
    for {set i $startMove} {$i <= $endMove} {set i [expr {$i + $FilterStepMoves}]} {
      lappend rlist [expr {2*$i}]
      lappend rlist [expr {2*($i+$FilterStepMoves)-1}]
      if { $i % 5 == 0 } {
        lappend rlist $i
      } else {
        lappend rlist ""
      }
    }
  }
  
  foreach {start end label} $rlist {
    if {$ftype == "date"} { append end ".12.31" }
    set r [sc_filter freq [sc_base current] dbfilter $ftype $start $end $FilterGuessELO]
    set filter [lindex $r 0]
    set all [lindex $r 1]
    if {$all == 0} {
      set freq 0.0
    } else {
      set freq [expr {double($filter) * 1000.0 / double($all)}]
    }
    if {$freq >= 1000.0} { set freq 999.9 }
    incr count
    lappend dlist $count
    lappend dlist $freq
    if {$freq > $max} { set max $freq }
    lappend xlabels [list $count $label]
  }
  
  # Find a suitable spacing of y-axis labels:
  set ytick 0.1
  if {$max > 1.0} { set ytick 0.2 }
  if {$max > 2.5} { set ytick 0.5 }
  if {$max >   5} { set ytick   1 }
  if {$max >  10} { set ytick   2 }
  if {$max >  25} { set ytick   5 }
  if {$max >  50} { set ytick  10 }
  if {$max > 100} { set ytick  20 }
  if {$max > 250} { set ytick  50 }
  if {$max > 500} { set ytick 100 }
  set hlines [list [list gray80 1 each $ytick]]
  # Add mean horizontal line:
  set filter [sc_filter count]
  set all [sc_base numGames $::curr_db]
  if {$all > 0} {
    set mean [expr {double($filter) * 1000.0 / double($all)}]
    if {$mean >= 1000.0} { set mean 999.9 }
    lappend hlines [list red 1 at $mean]
  }
  
  # Create fake dataset with bounds so we see 0.0::
  #::utils::graph::data decade bounds -points 0 -lines 0 -bars 0 -coords {1 0.0 1 0.0}
  
  ::utils::graph::data filter data -color darkBlue -points 1 -lines 1 -bars 0 \
      -linewidth 2 -radius 4 -outline darkBlue -coords $dlist
  ::utils::graph::configure filter -xlabels $xlabels -ytick $ytick \
      -hline $hlines -ymin 0 -xmin 0.5 -xmax [expr {$count + 0.5}]
  ::utils::graph::redraw filter
  $w.c itemconfigure title -text $::tr(GraphFilterTitle)
  $w.c itemconfigure type -text $typeName
  $w.b.status configure -text "  $::tr(Filter): [::windows::gamelist::filterText]"
  unbusyCursor .
  update
}

#Invert white/black Score in Score graph, switches for display move time and adding time
set ::tools::graphs::score::White 0
set ::tools::graphs::score::Black 0
set ::tools::graphs::score::Scores 1
set ::tools::graphs::score::Times 1
set ::tools::graphs::score::TimeSum 0

###########################
# Game score and time graph

# MoveTimeList
#    Returns a Tcl list of the numeric times of each move, as found
#    in the commment for each move.
#    A time is a number with the format
#        "%clk 00:00:00" or
#        "%clkms 12345" (used by BabaChess) or
#        "%emt 00:00:00" (used by pychess)
#        "%emt 1.23" (used by Raptor)
#    found somewhere in the comment of the move.
proc MoveTimeList {color add} {
    set movetimes   { }
    set mainline { }
    set base [sc_base current]
    set gnum [sc_game number]
    set game [sc_base getGame $base $gnum live]
    set n [llength $game]
    set movenr 0
    for {set i 0} { $i < $n} { incr i } {
	set RAVd [lindex [lindex $game $i] 0]
	set RAVn [lindex [lindex $game $i] 1]
	# only search in the mainline
	if { $RAVd == 0 && $RAVn == 0} {
	    # append comments for white
	    if {  $color == "w" && [expr $movenr % 2] == 1 }  {
		lappend mainline [lindex [lindex $game $i] 4] }
	    # append comments for black
	    if {  $color == "b" && [expr $movenr % 2] == 0 }  {
		lappend mainline [lindex [lindex $game $i] 4] }
	    incr movenr
	}
    }
    set movenr 0
    set offset 0.0
    if {  $color == "w" } { set offset 0.5 }
    set sum 0.0
    for {set i 0} { $i < $n} { incr i } {
	# only look for the first match, because normaly only one of these types should used in game
	set comment [lindex $mainline $i]
	set clkmsExp {.*?\[%clkms\s*(.*?)\s*\].*}
	set clkms ""
	regexp $clkmsExp $comment -> clkms
	if { $clkms != "" } {
	    scan $clkms "%f" sec
	    if { [scan $clkms "%f" sec ] == 1 } {
		# scale millisec to minutes
		lappend movetimes [expr $movenr+$offset] [expr { $sec / 60000.0 }] }
	} else {
	    set clkExp {.*?\[%clk\s*(.*?)\s*\].*}
	    set clock ""
	    regexp $clkExp $comment -> clock
	    if { $clock != "" } {
		if { [scan $clock "%f:%f:%f" ho mi sec ] == 3 } {
		    lappend movetimes [expr $movenr+$offset] [expr { $ho*60.0 + $mi + $sec/60}] }
	    } else {
		set emtExp {.*?\[%emt\s*(.*?)\s*\].*}
		set emt ""
		regexp $emtExp $comment -> emt
		if { $emt != "" } {
		    # emt could have 2 formats: 00:12:34 or 1.23
		    set ok 0
		    if { [regexp ":" $emt] } {
			if { [scan $emt "%f:%f:%f" ho mi sec ] == 3 } { incr ok }
		    } else {
			set ho 0.0
			set mi 0.0
			if { [scan $emt "%f" sec ] == 1 } { incr ok }
		    }
		    if { $ok == 1 } {
			set f [expr { $ho*3600.0 + $mi*60 + $sec}]
			if { $add } {
			    # add move times and scale to minutes
			    set f [expr { $f/60.0 + $sum }]
			    set sum $f
			}
			lappend movetimes [expr $movenr+$offset] $f
		    }
		}
	    }
	}
	incr movenr
    }
    return $movetimes
}

# MoveScoreList
#    Returns a Tcl list of the numeric move scores, as found
#    in the commment for each move.
#    A score is a number with the format
#        "+0.23" or
#        "-2.3" or
#        ":M4", ":M-4", "+M4", "-M4" for announce Mate
#    found somewhere in the comment of the move.
#    The list returned should be read in pairs of values: the first is the
#    move (0.0 = start, 0.5 after White's first move, 1.0 after Black's
#    first move, etc) and the second is the value found.
proc MoveScoreList { invw invb } {
    set moveScores { }
    set mainline { }
    set base [sc_base current]
    set gnum [sc_game number]
    set game [sc_base getGame $base $gnum live]
    set n [llength $game]
    set movenr 0
    for {set i 0} { $i < $n} { incr i } {
	set RAVd [lindex [lindex $game $i] 0]
	set RAVn [lindex [lindex $game $i] 1]
	# only search in the mainline
	if { $RAVd == 0 && $RAVn == 0} {
	    lappend mainline [lindex [lindex $game $i] 4]
	}
    }
    set movenr 0.0
    set side 0
    for {set i 1} { $i < $n} { incr i } {
	set comment [lindex $mainline $i]
	set evalExp {.*?\[%eval\s*(.*?)\s*\].*}
	set eval ""
	#little trick to check for valid score
	set score -20.0
	regexp $evalExp $comment -> eval
	if { $eval != "" } { # check for [%eval 1.23]
	    scan $eval "%f" score
	    if { [scan $eval "%f" score ] == 1 } {
		if { $score > 10 } { set score 9.9 }
		if { $score < -10 } { set score -9.9 }
	    } else {
		# Mate found in %eval #N oder #-N
		if { [string first "#" $eval] != -1 } {
		    set score 9.9
		    if { [string first "-" $eval] > 0 } { set score -9.9 }
		}
	    }
	}
	set foundIndex [string first ":M" $comment]
	# check for Mate :M5 or :M-3
	if { $foundIndex >= 0 } {
	    if { [scan [string range $comment [expr $foundIndex+2] end] "%f" score] == 1 } {
		# change Mate in x to +/-9.9
		if { $score >= 1 } { set score 9.9 }
		if { $score <= -1 } { set score -9.9 }
	    }
	} else {
	    set f 1
	    set foundIndex [string first "+M" $comment]
	    if { $foundIndex < 0 } { set foundIndex [string first "-M" $comment]; set f -1 }
	    # check for Mate +M5 or -M3 (Annotation from Arena GUI)
	    if { $foundIndex >= 0 } {
		if { [scan [string range $comment [expr $foundIndex+2] end] "%f" score] == 1 } {
		    # change Mate in x to +/-9.9
		    set score [expr $f * $score]
		    if { $score >= 1 } { set score 9.9 }
		    if { $score <= -1 } { set score -9.9 }
		}
	    } else {
		# check for scores +1.23 or -1.23, find the first apperance in the comment
		set foundIndex [string first "+" $comment]
		if { $foundIndex < 0 } { set foundIndex [string first "-" $comment] }
		if { $foundIndex >= 0 } {
		    if { [scan [string range $comment $foundIndex end] "%f" score] == 1 } {
			if { $score > 10 } { set score 9.9 }
			if { $score < -10 } { set score -9.9 }
		    }
		}
	    }
	}
	if { $score != -20.0 } {
	    # we have found something valid, check if the score should be inverted
	    if { $invw == 1 &&  $side == 1 } { set score [expr 0.0 - $score] }
	    if { $invb == 1 &&  $side == 0 } { set score [expr 0.0 - $score] }
	    lappend moveScores $movenr $score
	}
	set movenr [expr $movenr + 0.5]
	if { $side == 0 } { set side 1 } else { set side 0 }
    }
    return $moveScores
}

proc ::tools::graphs::score::Refresh { {docreate 1 }} {
  set linecolor red
  set firstColor darkgreen
  set secondColor blue
  set linewidth 2
  set psize 2
  
  set w .sgraph
  # Game has changed, but window is not open: do nothing
  if {! [winfo exists $w] && $docreate == 0 } { return }
  
  if {! [winfo exists $w] } {
    ::createToplevel $w
    menu $w.menu
    ::setMenu $w $w.menu
    $w.menu add cascade -label GraphFile -menu $w.menu.file
    menu $w.menu.file
    $w.menu.file add command -label GraphFileColor \
        -command "::tools::graphs::Save color $w.c"
    $w.menu.file add command -label GraphFileGrey \
        -command "::tools::graphs::Save gray $w.c"
    $w.menu.file add separator
    $w.menu.file add command -label GraphFileClose -command "destroy $w"
    $w.menu add cascade -label GraphOptions -menu $w.menu.options
    $w.menu add command -label Help -accelerator F1 -command {helpWindow Graphs Score}
    #Checkbuttons for Invert white/black Score in Score graph
    menu $w.menu.options
    foreach i {White Black} {
      $w.menu.options add checkbutton -label GraphOptions$i \
          -variable ::tools::graphs::score::$i -offvalue "0" -onvalue "1" \
          -command "::tools::graphs::score::Refresh"
    }
    canvas $w.c -width 500 -height 300 -selectforeground [ttk::style lookup . -foreground] -background [ttk::style lookup . -background]

    $w.c create text 25 5 -tag text -justify center -width 1 \
        -font font_Regular -anchor n
    ttk::frame $w.fbuttons
    ttk::checkbutton $w.fbuttons.score -text [tr ToolsScore] -variable ::tools::graphs::score::Scores \
        -command "::tools::graphs::score::Refresh"
    ttk::checkbutton $w.fbuttons.time -text [tr Time] -variable ::tools::graphs::score::Times \
        -command "::tools::graphs::score::Refresh"
    ttk::checkbutton $w.fbuttons.timesum -text $::tr(AnnotateTime) -variable ::tools::graphs::score::TimeSum \
        -command "::tools::graphs::score::Refresh" -offvalue "1" -onvalue "0"
    pack $w.fbuttons.score $w.fbuttons.time $w.fbuttons.timesum -side left -padx 6 -pady 0
    pack $w.fbuttons -side bottom -anchor e
    pack $w.c -side top -expand yes -fill both
    bind $w <F1> {helpWindow Graphs Score}
    bind $w <Configure> {
      .sgraph.c itemconfigure text -width [expr {[winfo width .sgraph.c] - 20}]
      .sgraph.c coords text [expr {[winfo width .sgraph.c] / 2}] 10
      ::utils::graph::configure score -height [expr {[winfo height .sgraph.c] - 50}]
      ::utils::graph::configure score -width [expr {[winfo width .sgraph.c] - 40}]
      ::utils::graph::redraw score
    }
    bind $w.c <1> {::tools::graphs::score::Move %x}
    wm title $w "Scid: [tr ToolsScore]"
    ::createToplevelFinalize $w
    ::tools::graphs::score::ConfigMenus
  }
  ::setTitle $w "Scid: [tr ToolsScore]"

  $w.c itemconfigure text -width [expr {[winfo width $w.c] - 20}]
  $w.c coords text [expr {[winfo width $w.c] / 2}] 10
  set height [expr {[winfo height $w.c] - 50} ]
  set width [expr {[winfo width $w.c] - 40} ]
  set yticks 1
  if { $::tools::graphs::score::Times } {
      set max 0
      # Find max Value of time, then set the tick value vor horizontal lines
      foreach j { "w" "b"} {
	  set coords [MoveTimeList $j $::tools::graphs::score::TimeSum]
	  set coords$j $coords
	  set ncoords [expr {[llength $coords] - 1}]
	  for {set i 0} {$i < $ncoords} {incr i 2} {
	      set y [lindex $coords [expr {$i + 1}]]
	      if { $y > $max } { set max $y }
	  }
      }
      if {$max > 20} { set yticks 5 }
      if {$max > 50} { set yticks 10 }
      if {$max > 100} { set yticks 20 }
  }

  ::utils::graph::create score -width $width -height $height -xtop 25 -ytop 25 \
      -ytick $yticks -xtick 5 -font font_Small -canvas $w.c -textcolor black \
      -hline [list [list gray80 1 each $yticks ]] \
      -vline {{gray80 1 each 1} {steelBlue 1 each 5}}

  # Create fake dataset with bounds so we see at least -1.0 to 1.0:
  ::utils::graph::data score bounds -points 0 -lines 0 -bars 0 -coords {1 0 1 0.9}
  
  # Update the graph:
  set whiteelo [sc_game tag get WhiteElo]
  set blackelo [sc_game tag get BlackElo]
  if {$whiteelo == 0} {set whiteelo ""} else {set whiteelo "($whiteelo)"}
  if {$blackelo == 0} {set blackelo ""} else {set blackelo "($blackelo)"}
  $w.c itemconfigure text -text "[sc_game info white]$whiteelo - [sc_game info black]$blackelo  [sc_game info site]  [sc_game info date]"
  busyCursor $w
  update

  if { $::tools::graphs::score::Times } {
      # draw move time
      catch {::utils::graph::data score data1 -color $firstColor -points 0 -lines 1\
		  -key [sc_game info white] -linewidth $linewidth -radius $psize -outline $firstColor -coords $coordsw }
      catch {::utils::graph::data score data2 -color $secondColor -points 0 -lines 1 \
		 -linewidth $linewidth -radius $psize -outline $secondColor -coords $coordsb}
  }
  if { $::tools::graphs::score::Scores } {
      # draw score bars
      catch {::utils::graph::data score data -color $linecolor -points 0 -lines 0 -bars 2 \
		 -linewidth $linewidth -radius $psize -outline $linecolor \
		 -coords [MoveScoreList $::tools::graphs::score::White $::tools::graphs::score::Black]}
  }

  ::utils::graph::redraw score
  unbusyCursor $w
  update
}

proc ::tools::graphs::score::ConfigMenus {{lang ""}} {
  if {! [winfo exists .sgraph]} { return }
  if {$lang == ""} { set lang $::language }
  set m .sgraph.menu
  foreach idx {0 1} tag {File Options} {
    configMenuText $m $idx Graph$tag $lang
  }
  foreach idx {0 1 3} tag {Color Grey Close} {
    configMenuText $m.file $idx GraphFile$tag $lang
  }
  foreach idx {0 1} tag {GraphOptionsWhite GraphOptionsBlack} {
    configMenuText $m.options $idx $tag $lang
  }
  $m entryconfig 2 -label $::tr(Help)
}

proc ::tools::graphs::score::Move {xc} {
  set x [expr {round([::utils::graph::xunmap score $xc] * 2 + 0.5)} ]
  sc_move start
  sc_move forward $x
  updateBoard
}


####################
# Rating graph

set ::tools::graphs::rating::year 1900
set ::tools::graphs::rating::type both
set ::tools::graphs::rating::elo info
set ::tools::graphs::rating::player ""

proc ::tools::graphs::rating::GetElo { player } {
  if {$::tools::graphs::rating::elo == "elo"} {
    set eloList [sc_name elo  $::tools::graphs::rating::year $player]
  } else {
    set filter [sc_filter new $::curr_db]
    sc_filter search $::curr_db $filter header -date "$::tools::graphs::rating::year 2047"
    set eloList [sc_base player_elo $::curr_db $filter $player]
    sc_filter release $::curr_db $filter
  }
  return $eloList
}

proc ::tools::graphs::rating::Refresh {{type ""} {player ""}} {
  set white [sc_game info white]
  set black [sc_game info black]
  set whiteColor red
  set blackColor blue
  set lwidth 2
  set psize 2
  
  if {$type == ""} { set type $::tools::graphs::rating::type }
  if {$player == ""} { set player $::tools::graphs::rating::player }
  set ::tools::graphs::rating::type $type
  set ::tools::graphs::rating::player $player
  
  set w .rgraph
  
  if {! [winfo exists $w]} {
    toplevel $w
    menu $w.menu
    ::setMenu $w $w.menu
    $w.menu add cascade -label GraphFile -menu $w.menu.file
    menu $w.menu.file
    $w.menu.file add command -label GraphFileColor \
        -command "::tools::graphs::Save color $w.c"
    $w.menu.file add command -label GraphFileGrey \
        -command "::tools::graphs::Save gray $w.c"
    $w.menu.file add separator
    $w.menu.file add command -label GraphFileClose -command "destroy $w"
    $w.menu add cascade -label GraphOptions -menu $w.menu.options
    menu $w.menu.options
    foreach i {White Black Both PInfo} j {white black both player} {
      $w.menu.options add radiobutton -label GraphOptions$i \
          -variable ::tools::graphs::rating::type -value $j \
          -command "::tools::graphs::rating::Refresh"
    }
    $w.menu.options add separator
    foreach i { EloDB EloFile } j {info elo} {
      $w.menu.options add radiobutton -label GraphOptions$i \
        -variable ::tools::graphs::rating::elo -value $j \
        -command "::tools::graphs::rating::Refresh"
    }
    $w.menu.options add separator
    foreach i {1900 1980 1985 1990 1995 2000 2005 2010 2015 } {
      $w.menu.options add radiobutton -label "Since $i" \
          -variable ::tools::graphs::rating::year -value $i \
          -command "::tools::graphs::rating::Refresh"
    }
    
    canvas $w.c -width 500 -height 300 -selectforeground [ttk::style lookup . -foreground] -background [ttk::style lookup . -background]
    $w.c create text 25 10 -tag text -justify center -width 1 \
        -font font_Regular -anchor n
    pack $w.c -side top -expand yes -fill both
    bind $w <F1> {helpWindow Graphs Rating}
    bind $w <Configure> {
      .rgraph.c itemconfigure text -width [expr {[winfo width .rgraph.c] - 50} ]
      .rgraph.c coords text [expr {[winfo width .rgraph.c] / 2} ] 10
      ::utils::graph::configure ratings -height [expr {[winfo height .rgraph.c] - 70} ]
      ::utils::graph::configure ratings -width [expr {[winfo width .rgraph.c] - 100} ]
      ::utils::graph::configure ratings -logy 10
      ::utils::graph::redraw ratings
    }
    bind $w.c <Button-1> "::tools::graphs::rating::Refresh"
    bind $w.c <Button-$::MB3> "::tools::graphs::rating::Refresh"
    wm title $w "Scid: [tr ToolsRating]"
    ::tools::graphs::rating::ConfigMenus
  }
  
  $w.c itemconfigure text -width [expr {[winfo width $w.c] - 50} ]
  $w.c coords text [expr {[winfo width $w.c] / 2} ] 10
  set height [expr {[winfo height $w.c] - 70} ]
  set width [expr {[winfo width $w.c] - 100} ]
  ::utils::graph::create ratings -width $width -height $height -xtop 50 -ytop 35 \
      -ytick 50 -xtick 1 -font font_Small -canvas $w.c -textcolor black \
      -hline {{gray80 1 each 25} {steelBlue 1 each 100}} \
      -vline {{gray80 1 each 1} {steelBlue 1 each 5}}
  ::utils::graph::redraw ratings
  busyCursor $w
  update
  
  set title "[tr ToolsRating]: "
  if {$type == "player"} {
    append title $player
    catch {::utils::graph::data ratings d -color $whiteColor -points 1 -lines 1 \
          -linewidth $lwidth -radius $psize -outline $whiteColor \
          -coords [GetElo $player] }
  }
  if {$type == "white"  ||  $type == "both"} {
    set key ""
    if {$type == "both"} { set key [::utils::string::Surname $white] }
    append title $white
    catch {::utils::graph::data ratings d -color $whiteColor -points 1 -lines 1 \
          -linewidth $lwidth -radius $psize -outline $whiteColor \
          -key $key -coords [GetElo $white] }
  }
  if {$type == "both"} { append title " - " }
  if {$type == "black"  ||  $type == "both"} {
    set key ""
    if {$type == "both"} { set key [::utils::string::Surname $black] }
    append title $black
    catch {::utils::graph::data ratings d2 -color $blackColor -points 1 -lines 1 \
          -linewidth $lwidth -radius $psize -outline $blackColor \
          -key $key -coords [GetElo $black]}
  }
  set minYear [expr {int([::utils::graph::cget ratings axmin])} ]
  set maxYear [expr {int([::utils::graph::cget ratings axmax])} ]
  ::utils::graph::configure ratings -xtick 1
  if {[expr {$maxYear - $minYear} ] > 10} {::utils::graph::configure ratings -xtick 5}
  ::utils::graph::redraw ratings
  $w.c itemconfigure text -text $title
  unbusyCursor $w
}

proc ::tools::graphs::rating::ConfigMenus {{lang ""}} {
  if {! [winfo exists .rgraph]} { return }
  if {$lang == ""} { set lang $::language }
  set m .rgraph.menu
  foreach idx {0 1} tag {File Options} {
    configMenuText $m $idx Graph$tag $lang
  }
  foreach idx {0 1 3} tag {Color Grey Close} {
    configMenuText $m.file $idx GraphFile$tag $lang
  }
  foreach idx {0 1 2 3 5 6} tag {White Black Both PInfo EloDB EloFile} {
    configMenuText $m.options $idx GraphOptions$tag $lang
  }
}


########################################
# Filter graph window for absolut values

# ::tools::graphs::absfilter::type
#   can be "decade", "year" or "elo", "move"
#
set ::tools::graphs::absfilter::type year

proc tools::graphs::absfilter::Open {} {
  global absfilterGraph
  set w .afgraph
  if {[winfo exists $w]} {
    focus .
    destroy $w
    set absfilterGraph 0
    return
  }
  win::createDialog $w 0
  menu $w.menu
  ::setMenu $w $w.menu
  $w.menu add cascade -label GraphFile -menu $w.menu.file
  configMenuText $w.menu 0 GraphFile $::language
  menu $w.menu.file
  $w.menu.file add command -label GraphFileColor -command "::tools::graphs::Save color $w.c"
  configMenuText $w.menu.file 0 GraphFileColor $::language
  $w.menu.file add command -label GraphFileGrey -command "::tools::graphs::Save gray $w.c"
  configMenuText $w.menu.file 1 GraphFileGrey $::language
  $w.menu.file add separator
  $w.menu.file add command -label GraphFileClose -command "destroy $w"
  configMenuText $w.menu.file 3 GraphFileClose $::language  
  wm title $w $::tr(TitleFilterGraph)
  set absfilterGraph 1
  bind $w <Destroy> {set absfilterGraph 0}
  
  ttk::frame $w.b
  pack $w.b -side top -fill x
  ttk::label $w.b.status -width 1 -font font_Small -anchor w
  
  canvas $w.c -width 600 -height 400 -selectforeground [ttk::style lookup . -foreground] -background [ttk::style lookup . -background]
  $w.c create text 25 5 -tag title -justify center -width 1 \
      -font font_Small -anchor n
  $w.c create text 250 295 -tag type -justify center -width 1 \
      -font font_Small -anchor s
  pack $w.c -side top -expand yes -fill both
  ::utils::graph::create absfilter
  
  bind $w <F1> {helpWindow Graphs Filter}
  bind $w <Configure> {
    .afgraph.c itemconfigure title -width [expr {[winfo width .afgraph.c] - 50}]
    .afgraph.c coords title [expr {[winfo width .afgraph.c] / 2}] 10
    .afgraph.c itemconfigure type -width [expr {[winfo width .afgraph.c] - 50}]
    .afgraph.c coords type [expr {[winfo width .afgraph.c] / 2}] \
        [expr {[winfo height .afgraph.c] - 10}]
    ::utils::graph::configure absfilter -height [expr {[winfo height .afgraph.c] - 80}]
    ::utils::graph::configure absfilter -width [expr {[winfo width .afgraph.c] - 60}]
    ::utils::graph::redraw absfilter
  }
  bind $w.c <1> tools::graphs::absfilter::Switch
  bind $w.c <$::MB3> ::tools::graphs::absfilter::Refresh
  foreach {name text} {decade Decade year Year elo Rating move moves} {
    ttk::radiobutton $w.b.$name -text $::tr($text) \
        -variable ::tools::graphs::absfilter::type -value $name \
        -command ::tools::graphs::absfilter::Refresh
    pack $w.b.$name -side left -padx 1 -pady 2
  }
  ttk::button $w.b.setup -image tb_config -command configureFilterGraph
  pack $w.b.decade $w.b.elo -side left -padx 1 -pady 2
  pack $w.b.setup -side right -pady 2
  pack $w.b.status -side left -padx 2 -pady 2 -fill x -expand yes
  
  ::tools::graphs::absfilter::Refresh
}

proc tools::graphs::absfilter::Switch {} {
  variable type
  switch $type {
    "decade" { set type "year" }
    "year" { set type "elo" }
    "elo" { set type "move" }
    "move" { set type "decade" }
  }
  ::tools::graphs::absfilter::Refresh
}

proc ::tools::graphs::absfilter::Refresh {} {
  global FilterMaxMoves FilterMinMoves FilterStepMoves FilterMaxElo FilterMinElo FilterStepElo FilterMaxYear FilterMinYear FilterStepYear FilterGuessELO
  
  set w .afgraph
  if {! [winfo exists $w]} { return }
  
  $w.c itemconfigure title -width [expr {[winfo width $w.c] - 50}]
  $w.c coords title [expr {[winfo width $w.c] / 2}] 10
  $w.c itemconfigure type -width [expr {[winfo width $w.c] - 50}]
  $w.c coords type [expr {[winfo width $w.c] / 2}] \
      [expr {[winfo height $w.c] - 10}]
  set height [expr {[winfo height $w.c] - 80}]
  set width [expr {[winfo width $w.c] - 60}]
  set vlines {}
  if {$::tools::graphs::absfilter::type == "elo"} {
    # Vertical lines for Elo-range graph:
    for {set i 1} {$i <=  $FilterMaxElo- $FilterMinElo} { incr i } {
      lappend vlines [list gray80 1 at $i.5]
    }
  } elseif {$::tools::graphs::absfilter::type == "year"} {
    # Vertical lines for Year-range graph:
    for {set i 1} {$i <= $FilterMaxYear- $FilterMinYear} {incr i } {
      lappend vlines [list gray80 1 at $i.5]
    }
  } elseif {$::tools::graphs::absfilter::type == "decade"} {
    # Vertical lines for Decade graph: most are gray, but those
    # just before 1950s and 2000s are blue to make them stand out.
    for {set i 1} {$i < 10} {incr i} {
      set vlineColor gray80
      if {$i == 4  ||  $i == 9} { set vlineColor steelBlue }
      lappend vlines [list $vlineColor 1 at $i.5]
    }
  }
  
  ::utils::graph::create absfilter -width $width -height $height -xtop 40 -ytop 35 \
      -ytick 1 -xtick 1 -font font_Small -canvas $w.c -textcolor black \
      -vline $vlines -background lightYellow -tickcolor black -xmin 0 -xmax 1
  ::utils::graph::redraw absfilter
  busyCursor .
  update
  
  set count 0
  set dlist {}
  set xlabels {}
  set max 0.0
  
  # Generate plot values and labels:
  if {$::tools::graphs::absfilter::type == "decade"} {
    set ftype date
    set typeName $::tr(Decade)
    set rlist [list 0000 1919 -1919  1920 1929 20-29 \
        1930 1939 30-39  1940 1949 40-49  1950 1959 50-59 \
        1960 1969 60-69  1970 1979 70-79  1980 1989 80-89 \
        1990 1999 90-99  2000 2009 2000+]
  } elseif {$::tools::graphs::absfilter::type == "year"} {
    set ftype date
    set typeName $::tr(Year)
    set endYear $FilterMaxYear
    set startYear $FilterMinYear
    set rlist {}
    for {set i $startYear} {$i <= $endYear} {set i [expr {$i + $FilterStepYear}]} {
      lappend rlist $i
      lappend rlist [expr {$i+$FilterStepYear-1}]
      lappend rlist [expr {$i+$FilterStepYear/2}]
    }
  } elseif {$::tools::graphs::absfilter::type == "elo"} {
    set ftype elo
    set typeName $::tr(Rating)
    #    set rlist [list 0 1999 0-1999  2000 2099 20xx  2100 2199 21xx  \
    2200 2299 22xx  2300 2399 23xx  2400 2499 24xx  \
        2500 2599 25xx  2600 2699 26xx  2700 3999 2700+]
    set endElo $FilterMaxElo
    set startElo $FilterMinElo
    set rlist {}
    for {set i $startElo} {$i <= $endElo} {set i [expr {$i + $FilterStepElo}]} {
      lappend rlist $i
      lappend rlist [expr {$i+$FilterStepElo-1}]
      # shorten gap between 0 and "useful" ratings 1800
      set j $i
      if { $i < 100 } { set i [expr { 1800 - $FilterStepElo}] }
      lappend rlist [concat $j-[expr {$i+$FilterStepElo-1}]]
    }
  } else {
    set ftype move
    set typeName $::tr(moves)
    set startMove $FilterMinMoves
    set endMove  $FilterMaxMoves
    set rlist {}
    for {set i $startMove} {$i <= $endMove} {set i [expr {$i + $FilterStepMoves}]} {
      lappend rlist [expr {2*$i}]
      lappend rlist [expr {2*($i+$FilterStepMoves)-1}]
      if { $i % 5 == 0 } {
        lappend rlist $i
      } else {
        lappend rlist ""
      }
    }
  }
  
  set mean 0
  foreach {start end label} $rlist {
    if {$ftype == "date"} { append end ".12.31" }
    set r [sc_filter freq [sc_base current] dbfilter $ftype $start $end $FilterGuessELO]
    set absfilter [lindex $r 0]
    set all [lindex $r 1]
    set freq $absfilter
    incr count
    set mean [expr { $mean + $absfilter }]
    lappend dlist $count
    lappend dlist $freq
    if {$freq > $max} { set max $freq }
    lappend xlabels [list $count $label]
  }
  
  # Find a suitable spacing of y-axis labels:
  set ytick 1
  if {$max >=  10} { set ytick   1 }
  if {$max >=  25} { set ytick   5 }
  #  if {$max >=  50} { set ytick   5 }
  if {$max >= 100} { set ytick  10 }
  if {$max >= 250} { set ytick  25 }
  if {$max >= 500} { set ytick  50 }
  if {$max >= 1000} { set ytick 100 }
  if {$max >= 2500} { set ytick 250 }
  if {$max >= 5000} { set ytick 500 }
  if {$max >= 10000} { set ytick  1000 }
  if {$max >= 25000} { set ytick  2500 }
  if {$max >= 50000} { set ytick  5000 }
  if {$max >= 100000} { set ytick 10000 }
  if {$max >= 250000} { set ytick 25000 }
  if {$max >= 500000} { set ytick 50000 }
  if {$max >= 1000000} { set ytick 100000 }
  set hlines [list [list gray80 1 each $ytick]]
  # Add mean horizontal line:
  set absfilter [sc_filter count]
  set all [sc_base numGames $::curr_db]
  if { $count != 0 } {set mean [expr { $mean / $count }] }
  if {$all > 0} {
    if {$mean > $max} { set max $mean }
    lappend hlines [list red 1 at $mean]
  }
  
  # Create fake dataset with bounds so we see 0.0::
  #::utils::graph::data decade bounds -points 0 -lines 0 -bars 0 -coords {1 0.0 1 0.0}
  
  ::utils::graph::data absfilter data -color darkBlue -points 1 -lines 1 -bars 0 \
      -linewidth 2 -radius 4 -outline darkBlue -coords $dlist
  ::utils::graph::configure absfilter -xlabels $xlabels -ytick $ytick \
      -hline $hlines -ymin 0 -xmin 0.5 -xmax [expr {$count + 0.5}]
  ::utils::graph::redraw absfilter
  $w.c itemconfigure title -text $::tr(GraphAbsFilterTitle)
  $w.c itemconfigure type -text $typeName
  $w.b.status configure -text "  $::tr(Filter): [::windows::gamelist::filterText]"
  unbusyCursor .
  update
}
### End of file: graphs.tcl
