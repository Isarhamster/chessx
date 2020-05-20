###
### opening.tcl: part of Scid.
### Copyright (C) 2007  Pascal Georges
###
namespace eval opening {
  set repBase -1
  # list of elements of type fenMovesEvalList (fen move1 nag1 .... movei nagi)
  set allLinesFenList {}
  set fenMovesEvalList {}
  # list of hash lists, one list per game (a game = a line)
  set allLinesHashList {}
  set hashList {}
  # options for repertoire training
  set playerBestMove 1
  set opBestMove 1
  set onlyFlaggedLines 0
  set repColor "w"
  set resetStats 0
  
  set movesLoaded 0
  set fenLastUpdate 0
  set fenLastStatsUpdate 0
  set cancelLoadRepertoire 0
  set lastMainLoopFen 0
  set lastMainLoopFlipped [sc_pos side]
  
  # parameters for opening trainer window
  set displayCM 0
  set DisplayCMValue 0
  set tCM ""
  set lastCMFen ""
  set lastCM "-1"
  
  set displayOpeningStats 1
  set listStats {} ;# list of {fen x y z t} where x:good move played, y:dubious move, z:move out of rep, t:position played
  
  ################################################################################
  # Configuration
  ################################################################################
  proc config {} {
    global ::opening::playerBestMove ::opening::opBestMove ::opening::repColor
    
    set w .openingConfig
    if { [winfo exists $w] } { focus $w ; return  }
    if { [winfo exists ".openingWin"] } { focus ".openingWin" ; return }
    
    win::createDialog $w
    wm title $w $::tr(Repertoiretrainingconfiguration)
    setWinLocation $w
    ttk::frame $w.f0 -relief groove
    
    ttk::radiobutton $w.f0.rbRepColorW -value "w" -variable ::opening::repColor -text $::tr(white)
    ttk::radiobutton $w.f0.rbRepColorB -value "b" -variable ::opening::repColor -text $::tr(black)
    ttk::radiobutton $w.f0.rbRepColorWB -value "wb" -variable ::opening::repColor -text $::tr(both)
    pack $w.f0.rbRepColorW $w.f0.rbRepColorB $w.f0.rbRepColorWB -side left  -expand yes -fill both
    
    ttk::frame $w.f1
    ttk::checkbutton $w.f1.cbPlayerBestMove -text $::tr(PlayerBestMove) -variable ::opening::playerBestMove
    ttk::checkbutton $w.f1.cbOpBestMove -text $::tr(OpponentBestMove) -variable ::opening::opBestMove
    ttk::checkbutton $w.f1.cbOnlyFlaggedLines -text $::tr(OnlyFlaggedLines) -variable ::opening::onlyFlaggedLines
    ttk::checkbutton $w.f1.cbResetStats -text $::tr(resetStats) -variable ::opening::resetStats
    pack $w.f1.cbPlayerBestMove $w.f1.cbOpBestMove $w.f1.cbOnlyFlaggedLines $w.f1.cbResetStats -anchor w -side top
    
    ttk::frame $w.f2
    ttk::button $w.f2.ok -text $::tr(Continue) -command " destroy $w ; ::opening::openRep"
    ttk::button $w.f2.cancel -text $::tr(Cancel) -command "focus .; destroy $w"
    packdlgbuttons $w.f2.cancel $w.f2.ok
    
    pack $w.f0 $w.f1 $w.f2 -side top -fill both
    
    bind $w <F1> { helpWindow OpeningTrainer }
    bind $w <Escape> "destroy $w"
    bind $w <Destroy> ""
    bind $w <Configure> "recordWinSize $w"
  }
  ################################################################################
  # Open a repertoire
  ################################################################################
  proc openRep {} {
    global ::windows::switcher::base_types ::opening::repColor ::opening::repBase
    
    if {$::opening::resetStats} {
      set ::opening::listStats {}
    } else  {
      loadStats
    }
    
    set repBase -1
    set typeW [lsearch $base_types {Openings for White} ]
    set typeB [lsearch $base_types {Openings for Black} ]
    set typeWB [lsearch $base_types {Openings for either color} ]
    
    foreach x [sc_base list] {
      set type [getBaseType $x]
      if {$type == $typeW && $repColor == "w" || $type == $typeB && $repColor == "b" || $type == $typeWB && $repColor == "wb"} {
        set repBase  $x
        break
      }
    }
    
    if {$repBase == -1} {
      tk_messageBox -title $::tr(Repertoirenotfound) -type ok -icon warning \
          -message $::tr(Openfirstrepertoirewithtype)
      return
    }
    
    set prevBase [sc_base current]
    if {$prevBase != $repBase} { sc_base switch $repBase }
    loadRep "$repBase - [sc_base filename $repBase]"
    if {$prevBase != $repBase} {
      sc_base switch $prevBase
    } else  {
      sc_base switch $::clipbase_db
    }
    
    if { $::opening::movesLoaded == 0 } {
      tk_messageBox -title $::tr(Repertoirenotfound) -type ok -icon error -message $::tr(ZeroMovesLoaded)
    }
    
    if { ! [sc_base inUse $prevBase] } {
      # switch to clipboard base if the current base is empty
      sc_base switch $::clipbase_db
    }
    
    # add a blank game for training in current base, if the current base is opened
    sc_game new
    sc_game tags set -event $::tr(Openingtrainer)
    sc_game save 0
    updateBoard -pgn -animate
    ::windows::gamelist::Refresh
    updateTitle
    
    ::opening::openingWin
    ::opening::mainLoop
  }
  ################################################################################
  # Loads a repertoire
  # Go through all games and variations and build a tree of positions encountered
  #
  ################################################################################
  proc loadRep { name } {
    global ::opening::repBase ::opening::fenMovesEvalList ::opening::allLinesFenList \
        ::opening::allLinesHashList ::opening::hashList ::opening::onlyFlaggedLines \
        ::opening::movesLoaded ::opening::cancelLoadRepertoire
    
    set movesLoaded 0
    set cancelLoadRepertoire 0
    set allLinesFenList {}
    set allLinesHashList {}
    
    set ::curr_db [sc_base current]
    progressWindow "Scid" "$::tr(Loadingrepertoire)..." $::tr(Cancel) "::opening::sc_progressBar"
    for {set g 1} { $g <= [sc_base numGames $::curr_db]} { incr g} {
      if {$cancelLoadRepertoire} { break  }
      if {$onlyFlaggedLines && ![sc_base gameflag $::curr_db $g get W] && ![sc_base gameflag $::curr_db $g get B]} {
        continue
      }
      set fenMovesEvalList {}
      set hashList  {}
      sc_game load $g
      changeProgressWindow "$::tr(Loadingrepertoire)...\n$name\n$::tr(Movesloaded) $movesLoaded"
      updateProgressWindow $g [sc_base numGames $::curr_db]
      parseGame
      lappend allLinesFenList $fenMovesEvalList
      set hashList [lsort -unique $hashList]
      lappend allLinesHashList $hashList
    }
    
    closeProgressWindow
  }
  ################################################################################
  # cancel repertoire loading
  ################################################################################
  proc sc_progressBar {} {
    set ::opening::cancelLoadRepertoire 1
  }
  ################################################################################
  # parse one game and fill the list
  ################################################################################
  proc parseGame {} {
    while {![sc_pos isAt vend]} {
      fillFen
      # Go through all variants
      for {set v 0} {$v<[sc_var count]} {incr v} {
        # enter each var (beware the first move is played)
        sc_var enter $v
        parseVar
      }
      # now treat the main line
      sc_move forward
    }
  }
  ################################################################################
  # parse recursively variants.
  ################################################################################
  proc parseVar {} {
    while {![sc_pos isAt vend]} {
      fillFen
      # Go through all variants
      for {set v 0} {$v<[sc_var count]} {incr v} {
        sc_var enter $v
        fillFen
        # we are at the start of a var, before the first move : start recursive calls
        parseVar
      }
      sc_move forward
    }
    # at the end of a var : exit it
    sc_var exit
  }
  ################################################################################
  # fill the tree with repertoire information
  # we are at a given position :
  # - fill hash list in order to speed up searches
  # - fill fenMovesEvalList with {fen {move eval} {move eval} .... }
  ################################################################################
  proc fillFen {} {
    global ::opening::fenMovesEvalList ::opening::hashList ::opening::movesLoaded
    
    if {[sc_pos isAt vend] && [sc_var count] == 0 } {
      return
    }
    
    set s [split [sc_pos fen]]
    set fen "[lindex $s 0] [lindex $s 1] [lindex $s 2] [lindex $s 3]"
    
    set newFen {}
    set moves {}
    set newIndex -1
    incr movesLoaded
    
    lappend hashList [sc_pos hash]
    
    # check if the fen already exists in the list
    for {set i 0} { $i < [llength $fenMovesEvalList]} {incr i} {
      set f [lindex $fenMovesEvalList $i]
      if {[lindex $f 0] == $fen} { set newFen $fen ; set moves [lindex $f 1] ; set newIndex $i ; break }
    }
    set newFen $fen
    
    # the main move
    if {! [sc_pos isAt vend] } {
      set m [sc_game info nextMove]
      sc_move forward
      set nag [sc_pos getNags]
      sc_move back
      if {[lsearch $moves $m] == -1 } {
        lappend moves $m $nag
      } else  {
        # the move already exists : check if NAG values are coherent
        set lmoves [lsearch -all $moves $m]
        foreach i $lmoves {
          if {[lindex $moves [expr $i +1]] != $nag} {
            puts "redundancy and incoherence $m $nag for $newFen"
          }
        }
      }
    }
    # Go through all variants
    for {set v 0} {$v<[sc_var count]} {incr v} {
      sc_var enter $v
      set nag [sc_pos getNags]
      set m [sc_game info previousMove]
      if {[lsearch $moves $m] == -1 } {
        lappend moves $m $nag
      } else  {
        # the move already exists : check if NAG values are coherent
        set lmoves [lsearch -all $moves $m]
        foreach i $lmoves {
          if {[lindex $moves [expr $i +1]] != $nag} {
            puts "var redundancy and incoherence $m $nag for $newFen"
          }
        }
      }
      sc_var exit
    }
    
    # put the newFen in the list
    if {$newIndex == -1} {
      lappend fenMovesEvalList [list $fen $moves ]
    } else  {
      lset fenMovesEvalList $newIndex [list $fen $moves ]
    }
  }
  ################################################################################
  # main loop called every second to trigger playing
  ################################################################################
  proc mainLoop {} {
    global ::opening::allLinesHashList ::opening::allLinesFenList tCM
    
    after cancel ::opening::mainLoop
    
    # Handle case of player's turn (which always plays from bottom of the board)
    if { [sc_pos side] == "white" &&  ![::board::isFlipped .main.board] || [sc_pos side] == "black" &&  [::board::isFlipped .main.board] } {
      # it is player's turn : update UI
      ::opening::update_tCM
      ::opening::updateStats
      after 1000  ::opening::mainLoop
      return
    }
    
    # check the position has not been treated already
    if {[sc_pos fen] == $::opening::lastMainLoopFen && [::board::isFlipped .main.board] == $::opening::lastMainLoopFlipped} {
      after 1000 ::opening::mainLoop
      return
    }
    
    # the player moved : check if his move was in the repertoire and as good as expected
    set move_done [sc_game info previousMove]
    if { $move_done != "" } {
      sc_move back
      set cm [ getCm ]
      sc_move forward
      # No move available : reached the end of a line
      if { [llength $cm] == 0 } {
        ::opening::update_tCM
        ::opening::updateStats
        after 1000 ::opening::mainLoop
        return
      }
      
      # we know there are some CM
      set l [lsearch -all $cm $move_done]
      # move not in repertoire
      if {[llength $l] == 0} {
        tk_messageBox -type ok -message $::tr(Movenotinrepertoire) -icon info
        sc_move back
        addStats -good 0 -dubious 0 -absent 1 -total 1
        ::opening::update_tCM
        ::opening::updateStats
        updateBoard -pgn -animate
        after 1000  ::opening::mainLoop
        return
      }
      
      # The move played is in repertoire !
      set moveOK 1
      
      if {$::opening::playerBestMove} {
        foreach i $l {
          if {! [ ::opening::isGoodMove [ lindex $cm [expr $i+1] ] ] } {
            addStatsPrev -good 0 -dubious 1 -absent 0 -total 1
            set moveOK 0
            set nag [ lindex $cm [expr $i+1] ]
            break
          }
        }
        
        # The move is not good : offer to take back
        if { ! $moveOK } {
          # addStatsPrev -good 0 -dubious 0 -absent 1 -total 0
          set answer [tk_messageBox -icon question -title $::tr(OutOfOpening) -type yesno \
              -message "$::tr(yourmoveisnotgood) ($nag) \n $::tr(DoYouWantContinue)" ]
          if {$answer == no} {
            sc_move back
            updateBoard -pgn
            after 1000  ::opening::mainLoop
            return
          }
        } else  { ;# the move is a good one
          addStatsPrev -good 1 -dubious 0 -absent 0 -total 1
        }
      } else  { ;# player is allowed to play bad moves
        foreach i $l {
          set goodMove 1
          if {! [ ::opening::isGoodMove [ lindex $cm [expr $i+1] ] ] } {
            set goodMove 0
            break
          }
        }
        if {$goodMove} {
          addStatsPrev -good 1 -dubious 0 -absent 0 -total 1
        } else  {
          addStatsPrev -good 1 -dubious 1 -absent 0 -total 1
        }
      }
      
    }
    # end of player's move check
    # now it is computer's turn
    set cm [ getCm ]
    
    if {[llength $cm] != 0} {
      ::opening::play $cm
    }
    set ::opening::lastMainLoopFen [sc_pos fen]
    set ::opening::lastMainLoopFlipped [::board::isFlipped .main.board]
    
    ::opening::update_tCM
    ::opening::updateStats
    after 1000  ::opening::mainLoop
  }
  ################################################################################
  # isGoodMove : returns true if the nag list in parameter is empty or contains !? ! !!
  ################################################################################
  proc isGoodMove { n } {
    if { [lsearch -exact $n "?"] != -1 || [lsearch -exact $n "?!"] != -1 || [lsearch -exact $n "??"] != -1} {
      return 0
    }
    return 1
  }
  ################################################################################
  # get all candidate moves in the repertoire from current position
  # the list returned is of the form {move1 nag1 move2 nag2 ....}
  # the moves are not unique
  ################################################################################
  proc getCm {  } {
    global ::opening::allLinesHashList ::opening::allLinesFenList ::opening::lastCMFen
    
    set fen [ sc_pos fen ]
    # avoids calculation
    if {$fen == $lastCMFen } { return $lastCM }
    
    set cm {}
    # First find the position in hash lists to spare time
    set linesFound {}
    for {set i 0} {$i<[llength $allLinesHashList]} {incr i} {
      set res [lsearch -sorted [lindex $allLinesHashList $i] [sc_pos hash]]
      if {$res != -1} { lappend linesFound $i }
    }
    
    set s [split $fen]
    set fen "[lindex $s 0] [lindex $s 1] [lindex $s 2] [lindex $s 3]"
    foreach i $linesFound {
      set line [lindex $allLinesFenList $i]
      foreach f $line {
        if {[lindex $f 0] == $fen} { set cm [concat $cm [lindex $f 1]] }
      }
    }
    
    set lastCM $cm
    set lastCMFen $fen
    
    return $cm
  }
  ################################################################################
  # play one of the candidate moves
  ################################################################################
  proc play { cm } {
    # addStatsPrev -good 0 -dubious 0 -absent 0 -total 1
    set r [expr int(rand()*[llength $cm]/2) ]
    set m [ lindex $cm [ expr $r * 2 ] ]
    
    if {[sc_pos moveNumber] == 1 && [sc_pos side] == "white"} {
      ::game::Clear
    }
    
    if {![catch {sc_move addSan [::untrans $m] }]} {
    }
    updateBoard -pgn -animate
  }
  ################################################################################
  # The window displayed when in opening trainer mode
  ################################################################################
  proc openingWin {} {
    global ::opening::displayCM ::opening::displayCMValue ::opening::tCM ::opening::fenLastUpdate
    
    set w ".openingWin"
    if {[winfo exists $w]} { focus $w ; return }
    
    win::createDialog $w
    wm title $w $::tr(Openingtrainer)
    setWinLocation $w
    ttk::frame $w.f1
    ttk::frame $w.f2 -relief raised
    ttk::frame $w.f3
    
    ttk::checkbutton $w.f1.cbDisplayCM  -text $::tr(DisplayCM) -variable ::opening::displayCM \
        -command "set fenLastUpdate 0 ; ::opening::update_tCM 1"
    ttk::checkbutton $w.f1.cbDisplayCMValue  -text $::tr(DisplayCMValue) -variable ::opening::displayCMValue \
        -command "set fenLastUpdate 0 ; ::opening::update_tCM 1"
    ttk::label $w.f1.lCM -textvariable ::opening::tCM
    pack $w.f1.cbDisplayCM $w.f1.cbDisplayCMValue -anchor w -side top
    pack $w.f1.lCM -side top -anchor w -pady "0 3" -padx 20
    
    ttk::checkbutton $w.f2.cbDisplayStats  -text $::tr(DisplayOpeningStats) -variable ::opening::displayOpeningStats \
        -command "::opening::updateStats 1"
    ttk::label $w.f2.lStats1 -textvariable ::opening::lStats1 -width 4 -anchor center -background green
    ttk::label $w.f2.lStats2 -textvariable ::opening::lStats2 -width 4 -anchor center -background yellow
    ttk::label $w.f2.lStats3 -textvariable ::opening::lStats3 -width 4 -anchor center -background red
    ttk::label $w.f2.lStats4 -textvariable ::opening::lStats4 -width 4 -anchor center -background white
    
    ttk::label $w.f2.lStats1exp -text $::tr(NumberOfGoodMovesPlayed)
    ttk::label $w.f2.lStats2exp -text $::tr(NumberOfDubiousMovesPlayed)
    ttk::label $w.f2.lStats3exp -text $::tr(NumberOfMovesPlayedNotInRepertoire)
    ttk::label $w.f2.lStats4exp -text $::tr(NumberOfTimesPositionEncountered)
    
    grid $w.f2.cbDisplayStats -row 0 -column 0 -columnspan 2 -pady "3 0" -sticky w -padx 10
    grid $w.f2.lStats4 -row 1 -column 0 -sticky w -padx 5
    grid $w.f2.lStats1 -row 2 -column 0 -sticky w -padx 5
    grid $w.f2.lStats2 -row 3 -column 0 -sticky w -padx 5
    grid $w.f2.lStats3 -row 4 -column 0 -sticky w -padx 5
    
    
    grid $w.f2.lStats4exp -row 1 -column 1 -sticky w -padx 5
    grid $w.f2.lStats1exp -row 2 -column 1 -sticky w -padx 5
    grid $w.f2.lStats2exp -row 3 -column 1 -sticky w -padx 5
    grid $w.f2.lStats3exp -row 4 -column 1 -sticky w -padx 5 -pady "0 5"
    
    ttk::button $w.f3.report -textvar ::tr(ShowReport) -command ::opening::report
    ttk::button $w.f3.close -textvar ::tr(Abort) -command ::opening::endTraining
    
    pack $w.f3.report $w.f3.close -side top -anchor center -fill x -pady "5 0"
    pack $w.f1 $w.f2 $w.f3 -fill x
    
    bind $w <F1> { helpWindow OpeningTrainer }
    bind $w <Escape> "destroy $w"
    bind $w <Destroy> ""
    bind $w <Configure> "recordWinSize $w"
    wm minsize $w 45 0
  }
  ################################################################################
  #
  ################################################################################
  proc endTraining {} {
    after cancel ::opening::mainLoop
    saveStats
    focus .
    destroy ".openingWin"
  }
  ################################################################################
  # display the candidate moves list (with NAG values)
  ################################################################################
  proc  update_tCM { { forceUpdate 0 } } {
    global ::opening::displayCM ::opening::displayCMValue ::opening::tCM ::opening::fenLastUpdate
    
    # If current fen is the same as the one used during latest update call, do nothing
    if {$fenLastUpdate == [sc_pos fen] && ! $forceUpdate} { return }
    
    set cm [ getCm ]
    
    if { [llength $cm] == 0 } {
      .openingWin.f1.lCM configure -background LightCoral
      set tCM $::tr(EndOfVar)
      set fenLastUpdate [sc_pos fen]
      return
    }
    
    if { !$displayCM } { set tCM "" ; set fenLastUpdate 0 ; return }
    
    .openingWin.f1.lCM configure -background linen
    
    set tmp ""
    
    for {set x 0} {$x<[llength $cm]} { set x [expr $x+2]} {
      set m [lindex $cm $x]
      # if the move already found, skip it, even if it has other nags : to be corrected ?
      if {[string first $m $tmp] != -1} { continue }
      append tmp  $m " "
      set nlist [lindex $cm [expr $x+1] ]
      if {$nlist == 0} { continue }
      if {$displayCMValue} {
        foreach n $nlist {
          append tmp $n " "
        }
      }
      # go to new line every 3 (moves,nags)
      if {[expr $x % 3] == 2} { append tmp "\n" }
    }
    
    set fenLastUpdate [sc_pos fen]
    set tCM $tmp
  }
  ################################################################################
  #
  ################################################################################
  proc loadStats {} {
    set optionsFile [scidConfigFile optrainer]
    if {[catch {source $optionsFile} ]} {
      ::splash::add "Unable to find the options file: [file tail $optionsFile]"
    } else {
      ::splash::add "Your options file \"[file tail $optionsFile]\" was found and loaded."
    }
  }
  ################################################################################
  #
  ################################################################################
  proc saveStats {} {
    set optrainerFile [scidConfigFile optrainer]
    if {[catch {open $optrainerFile w} f]} {
      return 0
    }
    puts $f "set ::opening::listStats { $::opening::listStats }"
    close $f
    return 1
  }
  ################################################################################
  # getStats
  # returns a list containing the 4 stats values for current pos
  # or an empty list if the stats are not available for current position
  ################################################################################
  proc getStats {} {
    set s [split [sc_pos fen]]
    set fen "[lindex $s 0] [lindex $s 1] [lindex $s 2] [lindex $s 3]"
    set found 0
    set idx 0
    foreach l $::opening::listStats {
      if {[lindex $l 0] == $fen} {
        set found 1
        break
      }
      incr idx
    }
    if {$found} {
      return [lindex $l 1]
    }
    return {}
  }
  
  ################################################################################
  # addStats
  # x = success best moves only, y = success all moves z = failures t = coverage by computer
  ################################################################################
  proc addStats { args } {
    set dx 0
    set dy 0
    set dz 0
    set dt 0
    
    for {set i 0 } {$i < [llength $args]} {incr i 2} {
      if {[lindex $args $i] == "-good"} { set dx [lindex $args [expr $i + 1] ] ; continue }
      if {[lindex $args $i] == "-dubious"} { set dy [lindex $args [expr $i + 1] ] ; continue }
      if {[lindex $args $i] == "-absent"} { set dz [lindex $args [expr $i + 1] ] ; continue }
      if {[lindex $args $i] == "-total"} { set dt [lindex $args [expr $i + 1] ] ; continue }
    }
    
    set s [split [sc_pos fen]]
    set fen "[lindex $s 0] [lindex $s 1] [lindex $s 2] [lindex $s 3]"
    set found 0
    set idx 0
    foreach l $::opening::listStats {
      if {[lindex $l 0] == $fen} {
        set found 1
        break
      }
      incr idx
    }
    
    if {$found} {
      set lval [lindex $l 1]
      set ::opening::listStats [ lreplace $::opening::listStats $idx $idx [list $fen [list \
          [expr [lindex $lval 0]+$dx] \
          [expr [lindex $lval 1]+$dy] \
          [expr [lindex $lval 2]+$dz] \
          [expr [lindex $lval 3]+$dt] \
          ] ] ]
    } else  {
      lappend ::opening::listStats [list $fen [list $dx $dy $dz $dt] ]
    }
    updateStats 1
  }
  ################################################################################
  #
  ################################################################################
  proc addStatsPrev { args } {
    if {[sc_pos isAt vstart] } { return }
    if { ![catch {sc_move back} ]} {
      eval addStats $args
      sc_move forward
    }
  }
  ################################################################################
  #
  ################################################################################
  proc updateStats { {force 0} } {
    global ::opening::fenLastStatsUpdate
    
    # If current fen is the same as the one used during latest update call, do nothing
    if {$fenLastStatsUpdate == [sc_pos fen] && !$force} { return }
    
    set fenLastStatsUpdate [sc_pos fen]
    
    if { $::opening::displayOpeningStats } {
      set gs [getStats]
      set ::opening::lStats1 [lindex $gs 0]
      set ::opening::lStats2 [lindex $gs 1]
      set ::opening::lStats3 [lindex $gs 2]
      set ::opening::lStats4 [lindex $gs 3]
    } else  {
      set ::opening::lStats1 " "
      set ::opening::lStats2 " "
      set ::opening::lStats3 " "
      set ::opening::lStats4 " "
    }
  }
  ################################################################################
  # shows a repertoire report (how much of the rep was trained)
  ################################################################################
  proc report {} {
    global ::opening::listStats ::opening::allLinesFenList
    set w ".openingWin.optrainerreport"
    if {[winfo exists $w]} { focus $w ; return }
    
    toplevel $w
    wm title $w $::tr(Openingtrainer)
    setWinLocation $w
    
    ttk::frame $w.ft
    text $w.ft.text -height 10 -width 40 -wrap word -background white
    pack $w.ft.text
    pack $w.ft
    
    ttk::frame $w.fclose
    ttk::button $w.fclose.close -textvar ::tr(Close) -command "destroy $w"
    pack $w.fclose.close
    
    # builds stats report
    set posNotPlayed 0
    set posTotalPlayed 0
    set success 0
    set dubMoves 0
    set outOfRep 0
    set totalPos 0
    foreach line $allLinesFenList {
      incr totalPos [llength $line]
      foreach pos $line {
        set fenLine [lindex $pos 0]
        set idx 0
        set found 0
        foreach l $listStats {
          if {$fenLine == [lindex $l 0]} {
            set found 1
            break
          }
          incr idx
        }
        if { $found } {
          set stats [lindex [ lindex $listStats $idx ] 1]
          if { $stats != "" } {
            incr success [lindex $stats 0]
            incr dubMoves [lindex $stats 1]
            incr outOfRep [lindex $stats 2]
            incr posTotalPlayed [lindex $stats 3]
          }
        } else {
          incr posNotPlayed
        }
      }
    }
    $w.ft.text insert end "$::tr(PositionsInRepertoire) $totalPos\n"
    $w.ft.text insert end "$::tr(PositionsNotPlayed) $posNotPlayed\n"
    $w.ft.text insert end "$::tr(PositionsPlayed) [expr $totalPos - $posNotPlayed]\n"
    $w.ft.text insert end "$::tr(Success) $success\n"
    $w.ft.text insert end "$::tr(DubiousMoves) $dubMoves\n"
    $w.ft.text insert end "$::tr(OutOfRepertoire) $outOfRep\n"
    
    $w.ft.text configure -state disabled
    
    bind $w <F1> { helpWindow OpeningTrainer }
    bind $w <Escape> "destroy $w"
    bind $w <Destroy> ""
    bind $w <Configure> "recordWinSize $w"
    # wm minsize $w 45 0
  }
  ################################################################################
  #
  ################################################################################
}
###
### End of file: opening.tcl
###
