###
### sergame.tcl: part of Scid.
### Copyright (C) 2007  Pascal Georges
###
################################################################################
# The number used for the engine playing a serious game is 3
################################################################################

namespace eval sergame {
  
  # DEBUG
  set ::uci::uciInfo(log_stdout3) 0
  
  # if true, follow a specific opening
  set openingMovesList {}
  set openingMovesHash {}
  set openingMoves ""
  set outOfOpening 0
  array set engineListBox {}
  set engineName ""
  set bookSlot 2
  set storeEval 0

  # list of fen positions played to detect 3 fold repetition
  set lFen {}
  set lastPlayerMoveUci ""
  
  ################################################################################
  #
  ################################################################################
  proc config {} {
    global ::sergame::configWin ::sergame::chosenOpening
    
    set w ".configSerGameWin"
    if {[winfo exists $w]} {
      focus $w
      return
    }
    
    win::createDialog $w
    wm title $w "$::tr(configuregame)"
    
    bind $w <F1> { helpWindow SeriousGame }
    setWinLocation $w
    
    ttk::frame $w.fconfig -padding 10
    ttk::frame $w.fbuttons
    ttk::labelframe $w.fengines -text $::tr(Engine)
    ttk::labelframe $w.ftime -text $::tr(TimeMode)
    ttk::labelframe $w.fopening -text $::tr(Opening)
    
    grid $w.fengines -row 0 -column 0 -pady { 0 10 } -sticky we -padx { 0 10 }
    grid $w.fopening -row 0 -column 1 -pady { 0 10 } -sticky nswe -padx { 10 0 }
    grid $w.ftime -row 1 -column 0 -pady { 10 0 } -sticky nswe -padx { 0 10 }
    grid $w.fconfig -row 1 -column 1 -pady { 10 0 } -sticky we -padx { 10 0 }
    grid $w.fbuttons -row 2 -column 1 -sticky we
    
    # builds the list of UCI engines
    ttk::frame $w.fengines.fEnginesList
    ttk::treeview $w.fengines.fEnginesList.lbEngines -columns {0} -show {} -selectmode browse \
        -yscrollcommand "$w.fengines.fEnginesList.ybar set"
    $w.fengines.fEnginesList.lbEngines column 0 -width 100
    $w.fengines.fEnginesList.lbEngines configure -height 5
    ttk::scrollbar $w.fengines.fEnginesList.ybar -command "$w.fengines.fEnginesList.lbEngines yview"
    pack $w.fengines.fEnginesList.ybar -side right -fill y
    pack $w.fengines.fEnginesList.lbEngines -side left -fill x -expand 1
    pack $w.fengines.fEnginesList -expand yes -fill x -side top


    set i 0
    set idx 0
    foreach e $::engines(list) {
      if { [lindex $e 7] != 1} { incr idx ; continue }
      set ::sergame::engineListBox($i) $idx
      set name [lindex $e 0]
      $w.fengines.fEnginesList.lbEngines insert {} end -id $idx -values [list $name]
      incr i
      incr idx
    }
    
    # Engine configuration (limit strength for example)
    ttk::button $w.fengines.bEngineConfig -text $::tr(ConfigureUCIengine) -command {
      set sel [.configSerGameWin.fengines.fEnginesList.lbEngines selection]
      set index $::sergame::engineListBox($sel)
      set engineData [lindex $::engines(list) $index]
      set name [lindex $engineData 0]
      set cmd [ toAbsPath [lindex $engineData 1] ]
      set args [lindex $engineData 2]
      set dir [ toAbsPath [lindex $engineData 3] ]
      set options [lindex $engineData 8]
      set ::uci::autoSaveOptionsIndex $index
      set ::uci::autoSaveOptions 1
      ::uci::uciConfig 3 [ toAbsPath $cmd ] $args [ toAbsPath $dir ] $options
    }
    pack $w.fengines.bEngineConfig -side top -pady 5 -anchor e -padx 4
    
    # if no engines defined, bail out
    if {$i == 0} {
      tk_messageBox -type ok -message "No UCI engine defined" -icon error
      destroy $w
      return
    }
    
    $w.fengines.fEnginesList.lbEngines selection set $::sergame::chosenEngine
    $w.fengines.fEnginesList.lbEngines see $::sergame::chosenEngine
    
    # load book names
    ttk::checkbutton $w.fconfig.cbUseBook -text $::tr(UseBook) -variable ::sergame::useBook
    set bookPath $::scidBooksDir
    set bookList [ lsort -dictionary [ glob -nocomplain -directory $bookPath *.bin ] ]
    if { [llength $bookList] == 0 } {
      $w.fconfig.cbUseBook configure -state disabled
      set ::sergame::useBook 0
    }
    set i 0
    set idx 0
    set tmp {}
    foreach file  $bookList {
      lappend tmp [ file tail $file ]
      if { $::sergame::bookToUse == [ file tail $file ]} {
        set idx $i
      }
      incr i
    }
    
    ttk::combobox $w.fconfig.combo -width 12 -values $tmp
    catch { ch$w.fconfig.combo current $idx }
    
    set row 0
    
    # Time bonus frame
    ttk::frame $w.ftime.timebonus
    pack  $w.ftime.timebonus -side top -fill x
    
    ttk::radiobutton $w.ftime.timebonus.rb1 -text $::tr(TimeBonus) -value "timebonus" -variable ::sergame::timeMode
    grid $w.ftime.timebonus.rb1 -row $row -column 0 -sticky w -rowspan 2
    
    ttk::label $w.ftime.timebonus.whitelabel -text $::tr(White)
    grid $w.ftime.timebonus.whitelabel -row $row -column 1
    ttk::spinbox $w.ftime.timebonus.whitespminutes -background white -width 2 -from 1 -to 120 -increment 1 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    grid $w.ftime.timebonus.whitespminutes -row $row -column 2
    ttk::label $w.ftime.timebonus.whitelminutes -text $::tr(TimeMin)
    grid $w.ftime.timebonus.whitelminutes -row $row -column 3
    ttk::spinbox $w.ftime.timebonus.whitespseconds -background white -width 2 -from 0 -to 60 -increment 1 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    grid $w.ftime.timebonus.whitespseconds -row $row -column 4
    ttk::label $w.ftime.timebonus.whitelseconds -text $::tr(TimeSec)
    grid $w.ftime.timebonus.whitelseconds -row $row -column 5
    
    incr row
    ttk::label $w.ftime.timebonus.blacklabel -text $::tr(Black)
    grid $w.ftime.timebonus.blacklabel -row $row -column 1
    ttk::spinbox $w.ftime.timebonus.blackspminutes -background white -width 2 -from 1 -to 120 -increment 1 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    grid $w.ftime.timebonus.blackspminutes -row $row -column 2
    ttk::label $w.ftime.timebonus.blacklminutes -text $::tr(TimeMin)
    grid $w.ftime.timebonus.blacklminutes -row $row -column 3
    ttk::spinbox $w.ftime.timebonus.blackspseconds -background white -width 2 -from 0 -to 60 -increment 1 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    grid $w.ftime.timebonus.blackspseconds -row $row -column 4
    ttk::label $w.ftime.timebonus.blacklseconds -text $::tr(TimeSec)
    grid $w.ftime.timebonus.blacklseconds -row $row -column 5
    
    $w.ftime.timebonus.whitespminutes set [expr $::uci::uciInfo(wtime3) / (60 * 1000)]
    $w.ftime.timebonus.whitespseconds set [expr $::uci::uciInfo(winc3) /  1000]
    $w.ftime.timebonus.blackspminutes set [expr $::uci::uciInfo(btime3) / (60 * 1000)]
    $w.ftime.timebonus.blackspseconds set [expr $::uci::uciInfo(binc3) /  1000 ]
    
    # Fixed depth
    ttk::frame $w.ftime.depth
    ttk::radiobutton $w.ftime.depth.button -text $::tr(FixedDepth) -value "depth" -variable ::sergame::timeMode -width 16
    ttk::spinbox $w.ftime.depth.value -background white -width 3 -from 1 -to 20 -increment 1 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    $w.ftime.depth.value set $::sergame::depth
    
    pack $w.ftime.depth -side top -fill x
    pack $w.ftime.depth.button -side left
    pack $w.ftime.depth.value -side left
    
    ttk::frame $w.ftime.nodes
    ttk::radiobutton $w.ftime.nodes.button -text "$::tr(Nodes) (x1000)" -value "nodes" -variable ::sergame::timeMode  -width 16
    ttk::spinbox $w.ftime.nodes.value -background white -width 3 -from 5 -to 10000 -increment 5 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    $w.ftime.nodes.value set [ expr $::sergame::nodes /1000]
    
    pack $w.ftime.nodes -side top -fill x
    pack $w.ftime.nodes.button -side left
    pack $w.ftime.nodes.value -side left
    
    ttk::frame $w.ftime.movetime
    ttk::radiobutton $w.ftime.movetime.button -text $::tr(SecondsPerMove) -value "movetime" -variable ::sergame::timeMode -width 16
    ttk::spinbox $w.ftime.movetime.value -background white -width 3 -from 1 -to 120 -increment 1 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    $w.ftime.movetime.value set [ expr $::sergame::movetime /1000]
    
    pack $w.ftime.movetime -side top -fill x
    pack $w.ftime.movetime.button -side left
    pack $w.ftime.movetime.value -side left

    pack $w.fconfig.cbUseBook -side top -anchor w
    pack $w.fconfig.combo -side top -anchor w -padx 20 -fill x
    
    # New game or use current position ?
    ttk::checkbutton $w.fconfig.cbPosition -text $::tr(StartFromCurrentPosition) -variable ::sergame::startFromCurrent
    pack $w.fconfig.cbPosition  -side top -anchor w
    
    # ponder
    ttk::checkbutton $w.fconfig.cbPonder -text $::tr(Ponder) -variable ::sergame::ponder
    pack $w.fconfig.cbPonder  -side top -anchor w
    
    # Warn if the user makes weak/bad moves
    ttk::checkbutton $w.fconfig.cbCoach -text $::tr(CoachIsWatching) -variable ::sergame::coachIsWatching
    pack $w.fconfig.cbCoach -side top -anchor w
    #Should the evaluation of the position stored in the comment?
    ttk::checkbutton $w.fconfig.storeEval -text $::tr(AddScoreToShortAnnotations) -variable ::sergame::storeEval
    pack $w.fconfig.storeEval -side top -anchor w
    
    # choose a specific opening
    ttk::checkbutton $w.fopening.cbOpening -text $::tr(SpecificOpening) -variable ::sergame::isOpening
    ttk::frame $w.fopening.fOpeningList
    ttk::treeview $w.fopening.fOpeningList.lbOpening -columns {0} -show {} -selectmode browse \
        -yscrollcommand "$w.fopening.fOpeningList.ybar set"
    $w.fopening.fOpeningList.lbOpening column 0 -width 250
    $w.fopening.fOpeningList.lbOpening configure -height 5
    set idx 0
    foreach o $::tacgame::openingList {
        $w.fopening.fOpeningList.lbOpening insert {} end -id $idx -values [list $o]
        incr idx
    }

    $w.fopening.fOpeningList.lbOpening selection set $::sergame::chosenOpening
    $w.fopening.fOpeningList.lbOpening see $::sergame::chosenOpening
    
    ttk::scrollbar $w.fopening.fOpeningList.ybar -command "$w.fopening.fOpeningList.lbOpening yview"
    pack $w.fopening.fOpeningList.ybar -side right -fill y
    pack $w.fopening.fOpeningList.lbOpening -side left -fill both -expand 1
    pack $w.fopening.fOpeningList -fill both -side top
    pack $w.fopening.cbOpening -fill x -side top
    
    ttk::button $w.fbuttons.close -text $::tr(Play) -command {
      focus .
      set ::sergame::chosenEngine [.configSerGameWin.fengines.fEnginesList.lbEngines selection]
      set ::sergame::engineName [.configSerGameWin.fengines.fEnginesList.lbEngines set $::sergame::chosenEngine 0]
      set ::sergame::chosenOpening [.configSerGameWin.fopening.fOpeningList.lbOpening selection]
      if {$::sergame::useBook} {
        set ::sergame::bookToUse [.configSerGameWin.fconfig.combo get]
        if {$::sergame::bookToUse == "" } {
          set ::sergame::useBook 0
        }
      }
      set ::uci::uciInfo(wtime3) [expr [.configSerGameWin.ftime.timebonus.whitespminutes get]*1000*60]
      set ::uci::uciInfo(btime3) [expr [.configSerGameWin.ftime.timebonus.blackspminutes get]*1000*60]
      set ::uci::uciInfo(winc3) [expr [.configSerGameWin.ftime.timebonus.whitespseconds get]*1000]
      set ::uci::uciInfo(binc3) [expr [.configSerGameWin.ftime.timebonus.blackspseconds get]*1000]
      set ::uci::uciInfo(fixeddepth3) [.configSerGameWin.ftime.depth.value get]
      set ::uci::uciInfo(fixednodes3) [expr [.configSerGameWin.ftime.nodes.value get]*1000]
      set ::uci::uciInfo(movetime3) [expr [.configSerGameWin.ftime.movetime.value get]*1000]

      destroy .configSerGameWin
      ::sergame::play $::sergame::chosenEngine
    }
    ttk::button $w.fbuttons.cancel -textvar ::tr(Cancel) -command "focus .; destroy $w"
    
    packbuttons right $w.fbuttons.cancel $w.fbuttons.close
    bind $w <Escape> { .configSerGameWin.fbuttons.cancel invoke }
    bind $w <Return> { .configSerGameWin.fbuttons.close invoke }
    bind $w <F1> { helpWindow SeriousGame }
    bind $w <Destroy> ""
    bind $w <Configure> "recordWinSize $w"
    wm resizable $w 0 0
  }
  
  ################################################################################
  #
  ################################################################################
  proc play { engine {n 3} } {
    global ::sergame::chosenOpening ::sergame::isOpening ::tacgame::openingList ::sergame::openingMovesList \
        ::sergame::openingMovesHash ::sergame::openingMoves ::sergame::outOfOpening
    
    if {$::sergame::isOpening || !$::sergame::startFromCurrent} {
      if {[::game::Clear] eq "cancel"} { return }
    }

    set ::sergame::lFen {}
    
    ::uci::startEngine $::sergame::engineListBox($engine) $n
    set engineData [lindex $::engines(list) $::sergame::engineListBox($engine)]
    foreach {option} [lindex $engineData 8] {
      array set ::uciOptions$n $option
    }
    ::uci::sendUCIoptions $n
    
    set ::uci::uciInfo(prevscore$n) 0.0
    set ::uci::uciInfo(score$n) 0.0
    set ::uci::uciInfo(ponder$n) ""
    
    if {$::sergame::startFromCurrent} {
      set isOpening 0
    }
    
    # ponder
    if {$::sergame::ponder} {
      ::sergame::sendToEngine $n "setoption name Ponder value true"
    } else {
      ::sergame::sendToEngine $n "setoption name Ponder value false"
    }
    
    # if will follow a specific opening line
    if {$isOpening} {
      set fields [split [lindex $openingList $chosenOpening] ":"]
      set openingName [lindex $fields 0]
      set openingMoves [string trim [lindex $fields 1]]
      set openingMovesList ""
      set openingMovesHash ""
      set outOfOpening 0
      foreach m [split $openingMoves] {
        # in case of multiple adjacent spaces in opening line
        if {$m =={}} {
          continue
        }
        set p [string trim $m]
        lappend openingMovesList [string trim [regsub {^[1-9]+\.} $p ""] ]
      }
      
      lappend openingMovesHash [sc_pos hash]
      foreach m  $openingMovesList {
        if {[catch {sc_move addSan $m}]} { }
        lappend openingMovesHash [sc_pos hash]
      }
    }

    # Engine plays for the upper side
    if {[::board::isFlipped .main.board]} {
      set ::sergame::playerColor "black"
      set ::sergame::engineColor "white"
    } else {
      set ::sergame::playerColor "white"
      set ::sergame::engineColor "black"
    }

    if {!$::sergame::startFromCurrent} {
      # create a new game if a DB is opened
      sc_game tags set -event "Serious game"
      sc_game tags set -$::sergame::playerColor "Player"
      sc_game tags set -$::sergame::engineColor "$::sergame::engineName"
      sc_game tags set -date [::utils::date::today]
    }

    set ::sergame::waitPlayerMove 0
    set ::sergame::wentOutOfBook 0
    set ::playMode "::sergame::callback"
    ::notify::GameChanged

    clocks init $n
    clocks start

    ::sergame::engineGo $n
  }

  proc callback {cmd} {
    switch $cmd {
        stop { ::sergame::abortGame }
    }
    return 0
  }

  proc abortGame { { n 3 } } {
    unset ::playMode
    after cancel ::sergame::engineGo $n
    clocks stop
    set ::sergame::lFen {}
    if { $::uci::uciInfo(pipe$n) != ""} {
      ::uci::closeUCIengine $n
      set ::uci::uciInfo(bestmove$n) "abort"
    }
    ::notify::GameChanged
  }

  proc clocks {cmd {n 3}} {
    if {$::sergame::timeMode != "timebonus"} { return }

    switch $cmd {
      init {
          ::gameclock::new "" 1
          ::gameclock::new "" 2
          ::gameclock::setSec 1 [expr 0 - $::uci::uciInfo(wtime$n)/1000]
          ::gameclock::setSec 2 [expr 0 - $::uci::uciInfo(btime$n)/1000]
      }
      start {
          if { [sc_pos side] == "white" } {
            ::gameclock::start 1
          } else {
            ::gameclock::start 2
          }
      }
      stop {
          ::gameclock::stop 1
          ::gameclock::stop 2
      }
      toggle {
          if {[::gameclock::stop 1]} {
            ::gameclock::add 1 [expr $::uci::uciInfo(winc$n)/1000]
            ::gameclock::storeTimeComment 1
            ::gameclock::start 2
          } elseif {[::gameclock::stop 2]} {
            ::gameclock::add 2 [expr $::uci::uciInfo(binc$n)/1000]
            ::gameclock::storeTimeComment 2
            ::gameclock::start 1
          }
          ::notify::PosChanged -pgn
      }
    }
  }

  proc takeBack {takebackClockW takebackClockB} {
    sc_move back 1
    if {$takebackClockW != ""} {
      ::gameclock::setSec 1 [expr 0 - $takebackClockW]
      ::gameclock::setSec 2 [expr 0 - $takebackClockB]
      clocks start
    }
    ::notify::PosChanged -pgn
  }
  
  ################################################################################
  #
  ################################################################################
  proc sendToEngine {n text} {
    ::sergame::logEngine $n "Scid  : $text"
    catch {puts $::uci::uciInfo(pipe$n) $text}
  }
  ################################################################################
  # returns true if last move is a mate and stops clocks
  ################################################################################
  proc endOfGame {} {
    set move_done [sc_game info previousMove]
    if { [string index [sc_game info previousMove] end ] == "#"} {
      clocks stop
      return 1
    }
    return 0
  }
  ################################################################################
  #
  ################################################################################
  proc engineGo { n } {
    global ::sergame::isOpening ::sergame::openingMovesList ::sergame::openingMovesHash ::sergame::openingMoves \
        ::sergame::timeMode ::sergame::outOfOpening
    
    after cancel ::sergame::engineGo $n
    
    if { [::sergame::endOfGame] } { return }
    
    if { [sc_pos side] != $::sergame::engineColor } {
      set ::sergame::waitPlayerMove 1
      after 1000 ::sergame::engineGo $n
      return
    }
    
    set takebackClockW ""
    set takebackClockB ""
    if {$::sergame::waitPlayerMove} {
      # The player moved
      set ::sergame::waitPlayerMove 0
      if {$::sergame::timeMode == "timebonus"} {
        set takebackClockW [::gameclock::getSec 1]
        set takebackClockB [::gameclock::getSec 2]
        clocks toggle $n
      }
      repetition
    }
    
    # make a move corresponding to a specific opening, (it is engine's turn)
    if {$isOpening && !$outOfOpening} {
      set index 0
      # Warn if the user went out of the opening line chosen
      if { !$outOfOpening } {
        set ply [ expr [sc_pos moveNumber] * 2 - 1]
        if { [sc_pos side] == "white" } {
          set ply [expr $ply - 1]
        }
        
        if { [lsearch $openingMovesHash [sc_pos hash]] == -1 && [llength $openingMovesList] >= $ply} {
          clocks stop
          set answer [tk_messageBox -icon question -parent .main -title $::tr(OutOfOpening) -type yesno \
              -message "$::tr(NotFollowedLine) $openingMoves\n $::tr(DoYouWantContinue)" ]
          if {$answer == no} {
            takeBack $takebackClockW $takebackClockB
            after 1000 ::sergame::engineGo $n
            return
          }  else  {
            set outOfOpening 1
          }
          clocks start
        }
      }
      
      set hpos [sc_pos hash]
      # Find a corresponding position in the opening line
      set length [llength $openingMovesHash]
      for {set i 0}   { $i < [expr $length-1] } { incr i } {
        set h [lindex $openingMovesHash $i]
        if {$h == $hpos} {
          set index [lsearch $openingMovesHash $h]
          set move [lindex $openingMovesList $index]
          # play the move
          set action "replace"
          if {![sc_pos isAt vend]} { set action [confirmReplaceMove] }
          if {$action == "replace"} {
            if {[catch {sc_move addSan $move}]} {}
          } elseif {$action == "var"} {
            sc_var create
            if {[catch {sc_move addSan $move}]} {}
          } elseif {$action == "mainline"} {
            sc_var create
            if {[catch {sc_move addSan $move}]} {}
            sc_var promote
            sc_move forward 1
          }
          
          clocks toggle $n
          updateBoard -pgn -animate
          repetition
          after 1000 ::sergame::engineGo $n
          return
        }
      }
    }
    # -------------------------------------------------------------
    # use a book
    if {$::sergame::useBook && ! $::sergame::wentOutOfBook} {
      set move [ ::book::getMove $::sergame::bookToUse [sc_pos fen] $::sergame::bookSlot]
      if {$move == ""} {
        set ::sergame::wentOutOfBook 1
      } else  {
        sc_move addSan $move
        ::utils::sound::AnnounceNewMove $move
        # we made a book move so assume a score = 0
        set ::uci::uciInfo(prevscore$n) 0.0
        clocks toggle $n
        updateBoard -pgn -animate
        repetition
        after 1000 ::sergame::engineGo $n
        return
      }
    }
    # -------------------------------------------------------------
    # check if the engine pondered on the right move
    
    if { $::sergame::ponder && $::uci::uciInfo(ponder$n) == $::sergame::lastPlayerMoveUci} {
      ::sergame::sendToEngine $n "ponderhit"
    } else {
      
      if { $::sergame::ponder } {
        ::sergame::sendToEngine $n "stop"
      }
      set ::analysis(waitForReadyOk$n) 1
      ::sergame::sendToEngine $n "isready"
      vwait ::analysis(waitForReadyOk$n)
      ::sergame::sendToEngine $n "position fen [sc_pos fen]"
      set wtime [expr [::gameclock::getSec 1] * 1000 ]
      set btime [expr [::gameclock::getSec 2] * 1000 ]
      if {$timeMode == "timebonus"} {
        ::sergame::sendToEngine $n "go wtime $wtime btime $btime winc $::uci::uciInfo(winc$n) binc $::uci::uciInfo(binc$n)"
      } elseif {$timeMode == "depth"} {
        ::sergame::sendToEngine $n "go depth $::uci::uciInfo(fixeddepth$n)"
      } elseif {$timeMode == "movetime"} {
        ::sergame::sendToEngine $n "go movetime $::uci::uciInfo(movetime$n)"
      } elseif {$timeMode == "nodes"} {
        ::sergame::sendToEngine $n "go nodes $::uci::uciInfo(fixednodes$n)"
      }
    }
    
    set ::uci::uciInfo(bestmove$n) ""
    vwait ::uci::uciInfo(bestmove$n)
    
    # -------------------------------------------------------------
    # if weak move detected, propose the user to tack back
    if { $::sergame::coachIsWatching && $::uci::uciInfo(prevscore$n) != "" } {
      set blunder 0
      set delta [expr $::uci::uciInfo(score$n) - $::uci::uciInfo(prevscore$n)]
      if {$delta > $::informant("?!") && $::sergame::engineColor == "white" ||
        $delta < [expr 0.0 - $::informant("?!")] && $::sergame::engineColor == "black" } {
        set blunder 1
      }
      
      if {$delta > $::informant("?") && $::sergame::engineColor == "white" ||
        $delta < [expr 0.0 - $::informant("?")] && $::sergame::engineColor == "black" } {
        set blunder 2
      }
      
      if {$delta > $::informant("??") && $::sergame::engineColor == "white" ||
        $delta < [expr 0.0 - $::informant("??")] && $::sergame::engineColor == "black" } {
        set blunder 3
      }
      
      if {$blunder == 1} {
        set tBlunder "DubiousMovePlayedTakeBack"
      } elseif {$blunder == 2} {
        set tBlunder "WeakMovePlayedTakeBack"
      } elseif {$blunder == 3} {
        set tBlunder "BadMovePlayedTakeBack"
      }
      
      if {$blunder != 0} {
        clocks stop
        set answer [tk_messageBox -icon question -parent .main -title "Scid" -type yesno -message $::tr($tBlunder) ]
        if {$answer == yes} {
          takeBack $takebackClockW $takebackClockB
          after 1000 ::sergame::engineGo $n
          return
        }
        clocks start
      }
    }
    
    # -------------------------------------------------------------
    if { $::uci::uciInfo(bestmove$n) == "abort" } {
      return
    }
    
    ::uci::sc_move_add $::uci::uciInfo(bestmove$n)
    ::utils::sound::AnnounceNewMove $::uci::uciInfo(bestmove$n)
    set ::uci::uciInfo(prevscore$n) $::uci::uciInfo(score$n)
    if { $::sergame::storeEval == 1 } {
      storeEvalComment $::uci::uciInfo(score$n)
    }
    updateBoard -pgn -animate
    repetition
    
    clocks toggle $n

    # ponder mode (the engine just played its move)
    if {$::sergame::ponder && $::uci::uciInfo(ponder$n) != ""} {
      ::sergame::sendToEngine $n "position fen [sc_pos fen] moves $::uci::uciInfo(ponder$n)"
      set wtime [expr [::gameclock::getSec 1] * 1000 ]
      set btime [expr [::gameclock::getSec 2] * 1000 ]
      if {$timeMode == "timebonus"} {
        ::sergame::sendToEngine $n "go ponder wtime $wtime btime $btime winc $::uci::uciInfo(winc$n) binc $::uci::uciInfo(binc$n)"
      } elseif {$timeMode == "depth"} {
        ::sergame::sendToEngine $n "go ponder depth $::uci::uciInfo(fixeddepth$n)"
      } elseif {$timeMode == "movetime"} {
        ::sergame::sendToEngine $n "go ponder movetime $::uci::uciInfo(movetime$n)"
      } elseif {$timeMode == "nodes"} {
        ::sergame::sendToEngine $n "go ponder nodes $::uci::uciInfo(fixednodes$n)"
      }
    }
    
    after 1000 ::sergame::engineGo $n
  }
  ################################################################################
  #   add current position for 3fold repetition detection and returns 1 if
  # the position is a repetition
  ################################################################################
  proc repetition {} {
    set elt [lrange [split [sc_pos fen]] 0 2]
    # append the position only if different from the last element
    if { $elt != [ lindex $::sergame::lFen end ] } {
      lappend ::sergame::lFen $elt
    }
    
    if { [llength [lsearch -all $::sergame::lFen $elt] ] >=3 } {
      tk_messageBox -type ok -message $::tr(Draw) -parent .main -icon info
      puts $::sergame::lFen
      return 1
    }
    return 0
  }
  ################################################################################
  #
  ################################################################################
  proc logEngine {n text} {
    if {$::uci::uciInfo(log_stdout$n)} {
      puts stdout "$n $text"
    }
  }
  
}
###
### End of file: sergame.tcl
###
