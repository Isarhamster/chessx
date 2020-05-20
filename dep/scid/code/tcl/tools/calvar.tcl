###
### calvar.tcl: part of Scid.
### Copyright (C) 2007  Pascal Georges
###
################################################################################
# The number used for the engine playing a serious game is 4
################################################################################

namespace eval calvar {
  # DEBUG
  set ::uci::uciInfo(log_stdout4) 0
  
  array set engineListBox {}
  set thinkingTimePerLine 10
  set thinkingTimePosition 30
  set currentLine 1
  set currentListMoves {}
  # each line begins with a list of moves, a nag code and ends with FEN
  set lines {}
  set analysisQueue {}
  
  # contains multipv analysis of the position, to see if the user considered all important lines
  set initPosAnalysis {}
  
  set working 0
  set midmove ""
  
  set afterIdPosition 0
  set afterIdLine 0
  
  trace add variable ::calvar::working write { ::calvar::traceWorking }
  ################################################################################
  #
  ################################################################################
  proc traceWorking {a b c} {
    set widget .calvarWin.fCommand.bDone
    if {$::calvar::working} {
      $widget configure -state disabled
    } else {
      $widget configure -state normal
    }
  }
  ################################################################################
  #
  ################################################################################
  proc reset {} {
    set currentLine 1
    set currentListMoves {}
    set lines {}
    set working 0
    set analysisQueue {}
    if {[winfo exists .calvarWin]} {
      .calvarWin.fText.t delete 1.0 end
    }
  }
  ################################################################################
  #
  ################################################################################
  proc config {} {
    
    # check if game window is already opened. If yes abort previous game
    set w ".calvarWin"
    if {[winfo exists $w]} {
      focus .calvarWin
      return
    }
    
    set w ".configCalvarWin"
    if {[winfo exists $w]} {
      focus $w
      return
    }
    
    win::createDialog $w
    wm title $w [::tr "ConfigureCalvar"]
    
    bind $w <F1> { helpWindow CalVar }
    setWinLocation $w
    
    # builds the list of UCI engines
    ttk::frame $w.fengines
    ttk::label $w.fengines.eng -text $::tr(Engine)
    ttk::treeview $w.fengines.lbEngines -columns {0} -show {} -selectmode browse \
        -yscrollcommand "$w.fengines.ybar set"
    $w.fengines.lbEngines column 0 -width 100
    $w.fengines.lbEngines configure -height 5
    ttk::scrollbar $w.fengines.ybar -command "$w.fengines.lbEngines yview"
    pack $w.fengines.eng -side top -anchor w
    pack $w.fengines.ybar -side right -fill y
    pack $w.fengines.lbEngines -side left -fill both -expand yes
    pack $w.fengines -expand yes -fill both -side top
    set i 0
    set idx 0
    foreach e $::engines(list) {
      if { [lindex $e 7] != 1} { incr idx ; continue }
      set ::calvar::engineListBox($i) $idx
      set name [lindex $e 0]
      $w.fengines.lbEngines insert {} end -id $idx -values [list $name]
      incr i
      incr idx
    }
    $w.fengines.lbEngines selection set 0
    
    # if no engines defined, bail out
    if {$i == 0} {
      tk_messageBox -type ok -message "No UCI engine defined" -icon error
      destroy $w
      return
    }
    
    # parameters setting
    set f $w.parameters
    ttk::frame $w.parameters
    pack $f -side top -anchor w -pady 10
    # label $f.lThreshold -text "Threshold"
    # spinbox $f.sbThreshold -background white -width 3 -textvariable ::calvar::blunderThreshold -from 0.1 -to 1.5 -increment 0.1
    # pack $f.lThreshold $f.sbThreshold -side left
    ttk::label $f.lTime -text $::tr(SecondsPerMove)
    ttk::spinbox $f.sbTime -background white -width 3 -textvariable ::calvar::thinkingTimePerLine -from 5 -to 120 -increment 5 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    ttk::label $f.lTime2 -text "Position thinking time"
    ttk::spinbox $f.sbTime2 -background white -width 3 -textvariable ::calvar::thinkingTimePosition -from 5 -to 300 -increment 5 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    grid $f.lTime -column 0 -row 0 -sticky w
    grid $f.sbTime -column 1 -row 0 -padx 10 -pady 5
    grid $f.lTime2 -column 0 -row 1 -sticky w
    grid $f.sbTime2 -column 1 -row 1 -padx 10
    
    ttk::frame $w.fbuttons
    pack $w.fbuttons -expand yes -fill both
    ttk::button $w.fbuttons.start -text Start -command {
      focus .
      set chosenEngine [.configCalvarWin.fengines.lbEngines selection]
      set ::calvar::engineName [.configCalvarWin.fengines.lbEngines set $chosenEngine 0]
      destroy .configCalvarWin
      ::calvar::start $chosenEngine
    }
    ttk::button $w.fbuttons.cancel -textvar ::tr(Cancel) -command "focus .; destroy $w"
    
    packdlgbuttons $w.fbuttons.cancel $w.fbuttons.start
    
    bind $w <Escape> { .configCalvarWin.fbuttons.cancel invoke }
    bind $w <Return> { .configCalvarWin.fbuttons.start invoke }
    bind $w <Destroy> ""
    bind $w <Configure> "recordWinSize $w"
    wm minsize $w 45 0
  }
  ################################################################################
  #
  ################################################################################
  proc start { engine { n 4 } } {
    
    ::calvar::reset
    
    set w ".calvarWin"
    if {[winfo exists $w]} {
      focus .calvarWin
      return
    }
    createToplevel $w
    ::setTitle $w [::tr "Calvar"]
    bind $w <F1> { helpWindow CalVar }
    
    set f $w.fNag
    ttk::frame $f
    set i 0
    foreach nag { "=" "+=" "+/-" "+-" "=+" "-/+" "-+" } {
      ttk::button $f.nag$i -text $nag -command "::calvar::nag $nag" -width 3
      pack $f.nag$i -side left
      incr i
    }
    pack $f -expand 1 -fill both
    
    set f $w.fText
    ttk::frame $f
    text $f.t -height 12 -width 50
    applyThemeStyle Treeview $f.t
    pack $f.t -expand 1 -fill both
    pack $f -expand 1 -fill both
    
    set f $w.fPieces
    ttk::frame $f
    ttk::label $f.lPromo -text "Promotion"
    pack $f.lPromo -side left
    foreach piece { "q" "r" "b" "n" } {
      ttk::button $f.p$piece -image w${piece}20 -command "::calvar::promo $piece"
      pack $f.p$piece -side left
    }
    pack $f -expand 1 -fill both
    
    set f $w.fCommand
    ttk::frame $f
    ttk::button $f.bDone -text [::tr "DoneWithPosition"] -command ::calvar::positionDone
    pack $f.bDone
    pack $f -expand 1 -fill both
    
    set f $w.fbuttons
    ttk::frame $f
    pack $f -expand 1 -fill both
    ttk::button $w.fbuttons.stop -textvar ::tr(Stop) -command "::calvar::stop"
    pack $w.fbuttons.stop -expand yes -side left -padx 20 -pady 2
    
    bind $w <Escape> { .calvarWin.fbuttons.stop invoke }
    bind $w <Destroy> ""
    bind $w <Configure> "recordWinSize $w"
    wm minsize $w 45 0
    
    # start engine and set MultiPV to 10
    ::uci::startEngine $::calvar::engineListBox($engine) $n
    
    set ::analysis(multiPVCount$n) 10
    ::uci::sendToEngine $n "setoption name MultiPV value $::analysis(multiPVCount$n)"
    set ::calvar::suggestMoves_old $::suggestMoves
    set ::calvar::hideNextMove_old $::gameInfo(hideNextMove)
    
    set ::suggestMoves 0
    set ::gameInfo(hideNextMove) 1
    updateBoard
    
    # fill initPosAnalysis for the current position
    set ::calvar::working 1
    ::calvar::startAnalyze "" "" [sc_pos fen]
    
    set ::calvar::afterIdPosition [after [expr $::calvar::thinkingTimePosition * 1000] { ::calvar::stopAnalyze "" "" "" ; ::calvar::addLineToCompute "" }]
    ::createToplevelFinalize $w
  }
  ################################################################################
  #
  ################################################################################
  proc stop { {n  4 } } {
    after cancel $::calvar::afterIdPosition
    after cancel $::calvar::afterIdLine
    ::uci::closeUCIengine $n
    focus .
    destroy .calvarWin
    set ::suggestMoves $::calvar::suggestMoves_old
    set ::gameInfo(hideNextMove) $::calvar::hideNextMove_old
    updateBoard
  }
  
  ################################################################################
  #
  ################################################################################
  proc pressSquare { sq } {
    global ::calvar::midmove
    
    set sansq [::board::san $sq]
    if {$midmove == ""} {
      set midmove $sansq
    } else {
      lappend ::calvar::currentListMoves "$midmove$sansq"
      set midmove ""
    }
    set tmp " "
    if {$midmove == ""} {
      set tmp "-"
    }
    .calvarWin.fText.t insert "$::calvar::currentLine.end" "$tmp$sansq"
  }
  ################################################################################
  #
  ################################################################################
  proc promo { piece } {
    if { [llength $::calvar::currentListMoves] == 0 } { return }
    
    set tmp [lindex $::calvar::currentListMoves end]
    set tmp "$tmp$piece"
    lset ::calvar::currentListMoves end $tmp
    .calvarWin.fText.t insert end "$piece"
  }
  ################################################################################
  # This will end a line, and start engine computation
  ################################################################################
  proc nag { n } {
    .calvarWin.fText.t insert "$::calvar::currentLine.end" " $n\n"
    set newline [list $::calvar::currentListMoves $n [sc_pos fen]]
    lappend ::calvar::lines $newline
    incr ::calvar::currentLine
    addLineToCompute $newline
    set ::calvar::currentListMoves {}
  }
  ################################################################################
  #
  ################################################################################
  proc addLineToCompute {line {n 4} } {
    global ::calvar::analysisQueue
    if {$line != ""} {
      lappend analysisQueue $line
    }
    if { $::calvar::working } { return }
    
    while { [llength $analysisQueue] != 0 } {
      set line [lindex $analysisQueue 0]
      set analysisQueue [lreplace analysisQueue 0 0]
      computeLine $line
    }
  }
  ################################################################################
  #
  ################################################################################
  proc computeLine {line {n 4} } {
    set ::calvar::working 1
    set moves [ lindex $line 0 ]
    set nag [ lindex $line 1 ]
    set fen [ lindex $line 2 ]
    startAnalyze $moves $nag $fen
    set ::calvar::afterIdLine [after [expr $::calvar::thinkingTimePerLine * 1000] "::calvar::stopAnalyze [list $moves $nag $fen]"]
  }
  ################################################################################
  # we suppose FEN has not changed !
  ################################################################################
  proc handleResult {moves nag fen {n 4} } {
    set comment ""
    
    set usermoves [::uci::formatPv $moves $fen]
    set firstmove [lindex $usermoves 0]
    
    # format engine's output
    # append first move to the variations
    set ::analysis(multiPV$n) {}
    for {set i 0 } {$i < [llength $::analysis(multiPVraw$n)]} {incr i} {
      set elt [lindex $::analysis(multiPVraw$n) $i ]
      set line [::uci::formatPvAfterMoves $firstmove [lindex $elt 2] ]
      set line "$firstmove $line"
      lappend ::analysis(multiPV$n) [list [lindex $elt 0] [lindex $elt 1] $line [lindex $elt 3]]
    }
    
    if { [llength $moves] != [llength $usermoves]} {
      set comment " error in user moves [lrange $moves [llength $usermoves] end ]"
      puts $comment
    }
    
    set pv [ lindex $::analysis(multiPV$n) 0 ]
    if { [ llength $pv ] == 4 } {
      set engmoves [lindex $pv 2]
      # score is computed for the opposite side, so invert it
      set engscore [expr - 1.0 * [lindex $pv 1] ]
      set engdepth [lindex $pv 0]
      addVar $usermoves $engmoves $nag $comment $engscore
    } else  {
      puts "Error pv = $pv"
    }
  }
  ################################################################################
  # will add a variation at current position.
  # Try to merge the variation with an existing one.
  ################################################################################
  proc addVar {usermoves engmoves nag comment engscore} {
    # Cannot add a variation to an empty variation:
    if {[sc_pos isAt vstart]  &&  [sc_pos isAt vend]} {
      # enter the first move as dummy variation
      sc_move addSan [lindex $engmoves 0]
      sc_move back
    }
    
    set repeat_move ""
    # If at the end of the game or a variation, repeat previous move
    if {[sc_pos isAt vend] && ![sc_pos isAt vstart]} {
      set repeat_move [sc_game info previousMoveNT]
      sc_move back
    }
    
    # first enter the user moves
    sc_var create
    if {$repeat_move != ""} {sc_move addSan $repeat_move}
    sc_move addSan $usermoves
    if {$comment != ""} {
      sc_pos setComment $comment
    }
    
    sc_pos addNag $nag
    
    # now enter the engine moves
    while {![sc_pos isAt vstart] } {sc_move back}
    if {$repeat_move != ""} {sc_move forward}
    sc_var create
    sc_pos setComment  "$::calvar::engineName : $engscore"
    sc_move addSan $engmoves
    sc_var exit
    sc_var exit
    
    if {$repeat_move != ""} {sc_move forward}

    updateBoard -pgn
  }
  ################################################################################
  # will add a variation at current position.
  # Try to merge the variation with an existing one.
  ################################################################################
  proc addMissedLine {moves score depth} {
    # Cannot add a variation to an empty variation:
    if {[sc_pos isAt vstart]  &&  [sc_pos isAt vend]} {
      # enter the first move as dummy variation
      sc_move addSan [lindex $moves 0]
      sc_move back
    }
    
    set repeat_move ""
    # If at the end of the game or a variation, repeat previous move
    if {[sc_pos isAt vend] && ![sc_pos isAt vstart]} {
      set repeat_move [sc_game info previousMoveNT]
      sc_move back
    }
    
    sc_var create
    if {$repeat_move != ""} {sc_move addSan $repeat_move}
    sc_pos setComment "Missed line ($depth) $score"
    sc_move addSan $moves
    sc_var exit
    if {$repeat_move != ""} { sc_move forward }

    updateBoard -pgn
  }
  ################################################################################
  # The user stops entering var, check he founds all important ones.
  # All the moves that the user did not consider with a score better than the first best
  # move entered by the user should be pointed out.
  ################################################################################
  proc positionDone {} {
    global ::calvar::initPosAnalysis ::calvar::lines
    
    ################################################################################
    proc isPresent { engmoves } {
      global ::calvar::lines
      set res 0
      set firsteng [lindex $engmoves 0]
      foreach userLine $::calvar::lines {
        set usermoves [::uci::formatPv [lindex $userLine 0]]
        set firstuser [lindex $usermoves 0]
        if {$firstuser == $firsteng} { return 1 }
      }
      return 0
    }
    
    ################################################################################
    foreach pv $::calvar::initPosAnalysis {
      set engmoves [lindex $pv 2]
      set engscore [lindex $pv 1]
      set engdepth [lindex $pv 0]
      if { ! [isPresent $engmoves] } {
        addMissedLine $engmoves $engscore $engdepth
      } else {
        # the user considered at least one line (skip those that are below)
        break
      }
    }
    ::calvar::reset
  }
  ################################################################################
  # startAnalyze:
  #   Put the engine in analyze mode and ponder on the first move entered by the user to see
  # if the line's evaluation is coherent
  ################################################################################
  proc startAnalyze {moves nag fen {n 4}} {
    global analysis
    
    # Check that the engine has not already had analyze mode started:
    if {$analysis(analyzeMode$n)} { return }
    set analysis(analyzeMode$n) 1
    set analysis(waitForReadyOk$n) 1
    ::uci::sendToEngine $n "isready"
    vwait analysis(waitForReadyOk$n)
    set analysis(fen$n) $fen
    if { [llength $moves] > 0 } {
      ::uci::sendToEngine $n "position fen $fen moves [lindex $moves 0]"
    } else {
      ::uci::sendToEngine $n "position fen $fen"
    }
    ::uci::sendToEngine $n "go infinite"
  }
  ################################################################################
  # stopAnalyzeMode
  ################################################################################
  proc stopAnalyze { moves nag fen {n 4} } {
    if {! $::analysis(analyzeMode$n)} { return }
    set ::analysis(analyzeMode$n) 0
    ::uci::sendToEngine $n "stop"
    
    if { [llength $moves] > 0 } {
      handleResult $moves $nag $fen
    } else {
      set ::calvar::initPosAnalysis $::analysis(multiPV$n)
    }
    set ::calvar::working 0
    addLineToCompute ""
  }
  
}
###
### End of file: calvar.tcl
###
