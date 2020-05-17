###
### tacgame.tcl: part of Scid.
### Copyright (C) 2006  Pascal Georges
###

namespace eval tacgame {
  ######################################################################
  ### Tacgame window: uses a chess engine (Phalanx) in easy mode and
  ### another engine (for example Toga) to track blunders
  
  set resignCount 0
  
  # if true, follow a specific opening
  set openingMovesList {}
  set openingMovesHash {}
  set openingMoves ""
  set outOfOpening 0
  
  # list of fen positions played to detect 3 fold repetition
  set lFen {}
  
  set index1 0
  set index2 0
  
  set lastblundervalue 0.0
  set prev_lastblundervalue 0.0
  set blundermissed false
  set blunderwarning false
  set blunderwarningvalue 0.0
  set blundermissedvalue 0.0
  
  set blunderWarningLabel $::tr(Noblunder)
  set scoreLabel 0.0
  
  set blunderpending 0
  set prev_blunderpending 0
  set currentPosHash 0
  set lscore {}
  
  set analysisCoach(automove1) 0
  
  # ======================================================================
  # resetValues
  #   Resets all blunders data.
  # ======================================================================
  proc resetValues {} {
    # see tcl/start.tcl
    set ::tacgame::blundermissed false
    set ::tacgame::lastblundervalue 0.0
    set ::tacgame::prev_lastblundervalue 0.0
    set ::tacgame::prev_blunderpending 0
    set ::tacgame::currentPosHash [sc_pos hash]
    set ::tacgame::lscore {}
    set ::tacgame::resignCount 0
  }
  
  # ======================================================================
  # resetEngine:
  #   Resets all engine-specific data.
  # ======================================================================
  proc resetEngine {n} {
    
    global ::tacgame::analysisCoach
    set analysisCoach(pipe$n) ""             ;# Communication pipe file channel
    set analysisCoach(seen$n) 0              ;# Seen any output from engine yet?
    set analysisCoach(seenEval$n) 0          ;# Seen evaluation line yet?
    set analysisCoach(score$n) 0             ;# Current score in centipawns
    set analysisCoach(moves$n) ""            ;# PV (best line) output from engine
    set analysisCoach(movelist$n) {}         ;# Moves to reach current position
    set analysisCoach(nonStdStart$n) 0       ;# Game has non-standard start
    set analysisCoach(has_analyze$n) 0       ;# Engine has analyze command
    set analysisCoach(has_setboard$n) 0      ;# Engine has setboard command
    set analysisCoach(send_sigint$n) 0       ;# Engine wants INT signal
    set analysisCoach(wants_usermove$n) 0    ;# Engine wants "usermove" before moves
    set analysisCoach(wholeSeconds$n) 0      ;# Engine times in seconds not centisec
    set analysisCoach(analyzeMode$n) 0       ;# Scid has started analyze mode
    set analysisCoach(invertScore$n) 1       ;# Score is for side to move, not white
    set analysisCoach(automove$n) 0
    set analysisCoach(automoveThinking$n) 0
    set analysisCoach(automoveTime$n) 2000
    set analysisCoach(lastClicks$n) 0
    set analysisCoach(after$n) ""
    set analysisCoach(log$n) ""              ;# Log file channel
    set analysisCoach(logCount$n) 0          ;# Number of lines sent to log file
    set analysisCoach(wbEngineDetected$n) 0  ;# Is this a special Winboard engine?
  }
  
  # ======================================================================
  #		::tacgame::config
  #   Configure coach games :
  #			- Phalanx engine (because it has an 'easy' option)
  #			- Coach engine (Toga is the best)
  #			- level of difficulty
  # ======================================================================
  proc config {} {
    
    global ::tacgame::configWin ::tacgame::analysisCoachCommand ::tacgame::analysisCoach \
        engineCoach1 engineCoach2 ::tacgame::level ::tacgame::levelFixed \
        ::tacgame::isLimitedAnalysisTime ::tacgame::analysisTime ::tacgame::index1 ::tacgame::index2 ::tacgame::chosenOpening
    
    # check if game window is already opened. If yes abort previous game
    if {[winfo exists .coachWin]} {
      focus .
      destroy .coachWin
      ::tacgame::closeEngine 1
      ::tacgame::closeEngine 2
    }
    
    # find Phalanx and a UCI engine
    set i 0
    set index1 -1
    set index2 -1
    foreach e $::engines(list) {
      if { $index1 != -1 && $index2 != -1 } { break }
      set name [lindex $e 0]
      if { [ string match -nocase "*phalanx*" $name ]  } {
        set engineCoach1 $name
        set index1 $i
      }
      
      if {[lindex $e 7] != 0} {
        set engineCoach2 $name
        set index2 $i
      }
      incr i
    }
    
    # could not find engines
    if { $index1 == -1 || $index2 == -1 } {
      tk_messageBox -title "Scid" -icon warning -type ok -message $::tr(PhalanxOrTogaMissing)
      return
    }
    
    set w ".configWin"
    if {[winfo exists $w]} {
      focus $w
      # wm attributes $w -topmost
      return
    }
    
    win::createDialog $w
    wm title $w "$::tr(configurecoachgame)"
    
    bind $w <F1> { helpWindow TacticalGame }
    setWinLocation $w
    
    ttk::labelframe $w.flevel -text [string toupper $::tr(difficulty) 0 0 ]
    ttk::frame $w.flevel.diff_fixed
    ttk::frame $w.flevel.diff_random
    ttk::labelframe $w.fopening -text $::tr(Opening)
    ttk::labelframe $w.flimit -text $::tr(Time)
    ttk::frame $w.fbuttons
    
    pack $w.flevel -side top -fill x
    pack $w.flevel.diff_fixed -side top -anchor w
    pack $w.flevel.diff_random -side top -anchor w
    pack $w.fopening  -side top -fill both -expand 1 -pady 10
    pack $w.flimit $w.fbuttons -side top -fill x
    
    ttk::radiobutton $w.flevel.diff_random.cb -text $::tr(RandomLevel) -variable ::tacgame::randomLevel -value 1 -width 15
    ttk::scale $w.flevel.diff_random.lMin -orient horizontal -from 1200 -to 2200 -length 100 -variable ::tacgame::levelMin \
        -command { ::utils::validate::roundScale ::tacgame::levelMin 50 }
    ttk::label $w.flevel.diff_random.labelMin -textvariable ::tacgame::levelMin
    ttk::scale $w.flevel.diff_random.lMax -orient horizontal -from 1200 -to 2200 -length 100 -variable ::tacgame::levelMax \
        -command { ::utils::validate::roundScale ::tacgame::levelMax 50 }
    ttk::label $w.flevel.diff_random.labelMax -textvariable ::tacgame::levelMax
    ttk::radiobutton $w.flevel.diff_fixed.cb -text $::tr(FixedLevel) -variable ::tacgame::randomLevel -value 0 -width 15
    ttk::label $w.flevel.diff_fixed.labelFixed -textvariable ::tacgame::levelFixed
    ttk::scale $w.flevel.diff_fixed.scale -orient horizontal -from 1200 -to 2200 -length 200 \
        -variable ::tacgame::levelFixed -command { ::utils::validate::roundScale ::tacgame::levelFixed 50 }
    
    grid $w.flevel.diff_fixed.cb -row 0 -column 0 -rowspan 2
    grid $w.flevel.diff_fixed.labelFixed -row 0 -column 1
    grid $w.flevel.diff_fixed.scale -row 1 -column 1 -padx "10 0" -sticky e
    grid $w.flevel.diff_random.cb -row 0 -column 0 -rowspan 2
    grid $w.flevel.diff_random.labelMin -row 0 -column 1
    grid $w.flevel.diff_random.lMin -row 1 -column 1 -padx "10 0" -sticky e
    grid $w.flevel.diff_random.labelMax -row 0 -column 2
    grid $w.flevel.diff_random.lMax -row 1 -column 2 -sticky e
    
    # start new game
    ttk::radiobutton $w.fopening.cbNew -text $::tr(StartNewGame)  -variable ::tacgame::openingType -value new
    
    # start from current position
    ttk::radiobutton $w.fopening.cbPosition -text $::tr(StartFromCurrentPosition) -variable ::tacgame::openingType -value current
    
    # or choose a specific opening
    ttk::radiobutton $w.fopening.cbSpecific -text $::tr(SpecificOpening) -variable ::tacgame::openingType -value specific
    
    pack $w.fopening.cbNew -anchor w
    pack $w.fopening.cbPosition -anchor w
    pack $w.fopening.cbSpecific -anchor w
    
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
    $w.fopening.fOpeningList.lbOpening selection set $::tacgame::chosenOpening
    $w.fopening.fOpeningList.lbOpening see $::tacgame::chosenOpening
    
    ttk::scrollbar $w.fopening.fOpeningList.ybar -command "$w.fopening.fOpeningList.lbOpening yview"
    pack $w.fopening.fOpeningList.lbOpening -side left -fill both -expand 1
    pack $w.fopening.fOpeningList.ybar  -side right -fill y
    pack $w.fopening.fOpeningList -expand yes -fill both -side top -expand 1
    
    # in order to limit CPU usage, limit the time for analysis (this prevents noise on laptops)
    ttk::checkbutton $w.flimit.blimit -text $::tr(limitanalysis) -variable ::tacgame::isLimitedAnalysisTime
    ttk::label $w.flimit.labelsecval -textvariable ::tacgame::analysisTime
    ttk::label $w.flimit.labelsec -text $::tr(seconds)
    ttk::scale $w.flimit.analysisTime -orient horizontal -from 5 -to 60 -length 200 -variable ::tacgame::analysisTime \
        -command { ::utils::validate::roundScale ::tacgame::analysisTime 1 }
    grid $w.flimit.blimit -column 0 -row 0 -columnspan 2 -sticky we
    grid $w.flimit.labelsecval  -column 0 -row 1 -sticky e
    grid $w.flimit.labelsec -column 1 -row 1 -sticky w -padx 5
    grid $w.flimit.analysisTime -column 0 -row 2 -columnspan 2 -sticky we
    # pack $w.flimit.blimit $w.flimit.labelsec $w.flimit.analysisTime  $w.flimit.labelsecval -side left -expand yes -pady 5
    
    ttk::button $w.fbuttons.close -text $::tr(Play) -command {
      focus .
      set ::tacgame::chosenOpening [.configWin.fopening.fOpeningList.lbOpening selection]
      destroy .configWin
      ::tacgame::play
    }
    ttk::button $w.fbuttons.cancel -textvar ::tr(Cancel) -command "focus .; destroy $w"
    
    packdlgbuttons $w.fbuttons.cancel $w.fbuttons.close
    
    bind $w <Escape> { .configWin.fbuttons.cancel invoke }
    bind $w <Return> { .configWin.fbuttons.close invoke }
    bind $w <F1> { helpWindow TacticalGame }
    bind $w <Destroy> ""
    bind $w <Configure> "recordWinSize $w"
    wm minsize $w 45 0
  }
  # ======================================================================
  #
  #	::tacgame::play
  #
  # ======================================================================
  proc play { } {
    global ::tacgame::analysisCoach ::tacgame::threshold ::tacgame::showblunder ::tacgame::showblundervalue \
        ::tacgame::blunderfound ::tacgame::showmovevalue ::tacgame::level ::tacgame::levelFixed engineCoach1 \
        engineCoach2 ::tacgame::index1 ::tacgame::index2 ::tacgame::chosenOpening \
        ::tacgame::openingType ::tacgame::openingList ::tacgame::openingMovesList \
        ::tacgame::openingMovesHash ::tacgame::openingMoves ::tacgame::outOfOpening
    
    if {$::tacgame::openingType ne "current"} {
      if {[::game::Clear] eq "cancel"} { return }
    }

    resetEngine 1
    resetEngine 2
    catch { unset ::uci::uciInfo(score2) }
    
    set ::tacgame::lFen {}
    
    set w .coachWin
    if {[winfo exists $w]} {
      focus .
      destroy $w
      return
    }

    if {$::tacgame::randomLevel} {
      if {$::tacgame::levelMax < $::tacgame::levelMin} {
        set tmp $::tacgame::levelMax
        set ::tacgame::levelMax $::tacgame::levelMin
        set ::tacgame::levelMin $tmp
      }
      set level [expr int(rand()*($::tacgame::levelMax - $::tacgame::levelMin)) + $::tacgame::levelMin ]
    } else {
      set level $::tacgame::levelFixed
    }
    
    # if will follow a specific opening line
    if {$openingType == "specific"} {
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
        set n [string trim $m]
        lappend openingMovesList [string trim [regsub {^[1-9]+\.} $m ""] ]
      }
      
      lappend openingMovesHash [sc_pos hash]
      foreach m  $openingMovesList {
        if {[catch {sc_move addSan $m}]} {
        }
        lappend openingMovesHash [sc_pos hash]
      }
    }
    
    # create a new game if a DB is opened
    if {$::tacgame::openingType != "current"} {
      sc_game tags set -event "Tactical game"
      if { [::board::isFlipped .main.board] } {
        sc_game tags set -white "Phalanx - $level ELO"
      } else  {
        sc_game tags set -black "Phalanx - $level ELO"
      }
      sc_game tags set -date [::utils::date::today]
    }
    
    ::notify::GameChanged
    
    createToplevel $w
    applyThemeColor_background $w
    setTitle $w "$::tr(coachgame) (Elo $level)"
    
    ttk::frame $w.fdisplay
    ttk::frame $w.fthreshold
    ttk::frame $w.finformations
    ttk::labelframe $w.fclockw -text "$::tr(Time) $::tr(Player)"
    ttk::labelframe $w.fclockb -text "$::tr(Time) $::tr(Engine)"
    ttk::frame $w.fbuttons
    pack $w.fdisplay -side top -fill both -pady 5 -padx 10
    pack [ttk::separator $w.line1 -orient horizontal] -side top -fill x -padx 10 -pady 5
    pack $w.fthreshold -side top -fill both -pady 5 -padx 10
    pack [ttk::separator $w.line2 -orient horizontal] -side top -fill x -padx 10 -pady 5
    pack $w.finformations $w.fclockb $w.fclockw -side top -fill both -pady 5 -padx 10
    pack $w.fbuttons -side top -pady "10 15"
    
    ttk::checkbutton $w.fdisplay.b1 -text $::tr(showblunderexists) -variable ::tacgame::showblunder
    ttk::checkbutton $w.fdisplay.b2 -text $::tr(showblundervalue) -variable ::tacgame::showblundervalue
    ttk::checkbutton $w.fdisplay.b5 -text $::tr(showscore) -variable ::tacgame::showevaluation
    pack $w.fdisplay.b1 $w.fdisplay.b2 $w.fdisplay.b5 -anchor w
    
    ttk::label $w.fthreshold.l -text $::tr(moveblunderthreshold) -wraplength 300
    
    ttk::scale $w.fthreshold.t -orient horizontal -from 0.0 -to 10.0 -length 200 \
        -variable ::tacgame::threshold -command { ::utils::validate::floatScale ::tacgame::threshold 0.1 }
    ttk::label $w.fthreshold.labelt -textvariable ::tacgame::threshold
    pack $w.fthreshold.l $w.fthreshold.labelt $w.fthreshold.t -side top -anchor w

    ttk::label $w.finformations.l1 -textvariable ::tacgame::blunderWarningLabel -background linen
    ttk::label $w.finformations.l3 -textvariable ::tacgame::scoreLabel -foreground WhiteSmoke -background SlateGray
    pack $w.finformations.l1 $w.finformations.l3 -padx 5 -pady 5 -side top -fill x
    
    ::gameclock::new $w.fclockb 2 80
    ::gameclock::new $w.fclockw 1 80
    ::gameclock::reset 1
    ::gameclock::start 1
    
    ttk::button $w.fbuttons.close -textvar ::tr(Abort) -command "destroy .coachWin"
    pack $w.fbuttons.close -expand yes -fill both -padx 20 -pady 2
    
    ::tacgame::launchengine $index1 1
    ::uci::startEngine $index2 2
    set ::uci::uciInfo(multipv2) 1
    changePVSize 2
    
    ::tacgame::resetValues
    updateAnalysisText
    
    bind $w <F1> { helpWindow TacticalGame }
    bind $w <Destroy> "if {\[string equal $w %W\]} {::tacgame::abortGame}"
    bind $w <Escape> "destroy .coachWin"
    wm minsize $w 45 0
    createToplevelFinalize $w

    set ::playMode "::tacgame::callback"
    ::tacgame::phalanxGo
  }

  proc callback {cmd} {
    switch $cmd {
        stop { destroy .coachWin }
    }
    return 0
  }

  proc toggleClocks {} {
    if {[::gameclock::stop 1]} {
      ::gameclock::storeTimeComment 1
      ::gameclock::start 2
    } elseif {[::gameclock::stop 2]} {
      ::gameclock::storeTimeComment 2
      ::gameclock::start 1
    }
    ::notify::PosChanged -pgn -animate
  }


  ################################################################################
  #
  ################################################################################
  proc abortGame { { destroyWin 1 } } {
    catch { unset ::playMode }
    after cancel ::tacgame::phalanxGo
    stopAnalyze
    ::tacgame::closeEngine 1
    ::tacgame::closeEngine 2
    ::gameclock::stop 1
    ::gameclock::stop 2
    ::notify::GameChanged
  }
  # ======================================================================
  #   ::tacgame::launchengine
  #  - launches both engines
  #  - updates values for :
  #						blundermissed (boolean), blunderwarning (boolean)
  #						blunderwarningvalue (real), blundermissedvalue (real)
  #						totalblundersmissed (real), totalblunders (real)
  # ======================================================================
  
  proc launchengine {index n} {
    global ::tacgame::analysisCoach ::tacgame::level
    
    ::tacgame::resetEngine $n
    
    set engineData [lindex $::engines(list) $index]
    set analysisName [lindex $engineData 0]
    set analysisCommand [ ::toAbsPath [ lindex $engineData 1 ] ]
    set analysisArgs [lindex $engineData 2]
    set analysisDir [ ::toAbsPath [lindex $engineData 3] ]
    
    # turn phalanx book, ponder and learning off, easy on
    if {$n == 1} {
      # convert Elo = 1200 to level 100 up to Elo=2200 to level 0
      set easylevel [expr int(100-(100*($level-1200)/(2200-1200)))]
      append analysisArgs " -b+ -p- -l- -e $easylevel "
    }
    
    # If the analysis directory is not current dir, cd to it:
    set oldpwd ""
    if {$analysisDir != "."} {
      set oldpwd [pwd]
      catch {cd $analysisDir}
    }
    
    # Try to execute the analysis program:
    if {[catch {set analysisCoach(pipe$n) [open "| [list $analysisCommand] $analysisArgs" "r+"]} result]} {
      if {$oldpwd != ""} { catch {cd $oldpwd} }
      tk_messageBox -title "Scid: error starting analysis" \
          -icon warning -type ok \
          -message "Unable to start the program:\n$analysisCommand"
      ::tacgame::resetEngine $n
      return
    }
    
    # Return to original dir if necessary:
    if {$oldpwd != ""} { catch {cd $oldpwd} }
    
    # Configure pipe for line buffering and non-blocking mode:
    fconfigure $analysisCoach(pipe$n) -buffering line -blocking 0
    
    if {$n == 1} {
      fileevent $analysisCoach(pipe$n) readable "::tacgame::processInput"
      after 1000 "::tacgame::checkAnalysisStarted $n"
    }
    
  }
  
  # ======================================================================
  # ::tacgame::closeEngine
  #   Close an engine.
  # ======================================================================
  proc closeEngine {n} {
    global windowsOS ::tacgame::analysisCoach
    
    # Check the pipe is not already closed:
    if { $n == 1 } {
      if {$analysisCoach(pipe$n) == "" } {
        return
      }
    }
    if { $n == 2 } {
      ::uci::closeUCIengine $n
      return
    }
    
    # Send interrupt signal if the engine wants it:
    if {(!$windowsOS)  &&  $analysisCoach(send_sigint$n)} {
      catch {exec -- kill -s INT [pid $analysisCoach(pipe$n)]}
    }
    
    # Some engines in analyze mode may not react as expected to "quit"
    # so ensure the engine exits analyze mode first:
    sendToEngine $n "exit"
    sendToEngine $n "quit"
    catch { flush $analysisCoach(pipe$n) }
    
    # Uncomment the following line to turn on blocking mode before
    # closing the engine (but probably not a good idea!)
    #   fconfigure $analysisCoach(pipe$n) -blocking 1
    
    # Close the engine, ignoring any errors since nothing can really
    # be done about them anyway -- maybe should alert the user with
    # a message box?
    catch {close $analysisCoach(pipe$n)}
    
    set analysisCoach(pipe$n) ""
  }
  # ======================================================================
  # sendToEngine:
  #   Send a command to a running analysis engine.
  # ======================================================================
  proc sendToEngine {n text} {
    catch {puts $::tacgame::analysisCoach(pipe$n) $text}
  }
  
  # ======================================================================
  # checkAnalysisStarted
  #   Called a short time after an analysis engine was started
  #   to send it commands if Scid has not seen any output from
  #   it yet.
  # ======================================================================
  proc checkAnalysisStarted {n} {
    global ::tacgame::analysisCoach
    if {$analysisCoach(seen$n)} { return }
    
    # Some Winboard engines do not issue any output when
    # they start up, so the fileevent above is never triggered.
    # Most, but not all, of these engines will respond in some
    # way once they have received input of some type.  This
    # proc will issue the same initialization commands as
    # those in processAnalysisInput below, but without the need
    # for a triggering fileevent to occur.
    
    set analysisCoach(seen$n) 1
    ::tacgame::sendToEngine $n "xboard"
    ::tacgame::sendToEngine $n "protover 2"
    ::tacgame::sendToEngine $n "post"
    ::tacgame::sendToEngine $n "ponder off"
    
    # Prevent some engines from making an immediate "book"
    # reply move as black when position is sent later:
    ::tacgame::sendToEngine $n "force"
  }
  
  # ======================================================================
  #
  # processInput from the engine blundering (Phalanx)
  #
  # ======================================================================
  proc processInput {} {
    global ::tacgame::analysisCoach ::tacgame::analysis
    
    # Get one line from the engine:
    set line [gets $analysisCoach(pipe1)]
    
    # Check that the engine did not terminate unexpectedly:
    if {[eof $analysisCoach(pipe1)]} {
      fileevent $analysisCoach(pipe1) readable {}
      catch {close $analysisCoach(pipe1)}
      set analysisCoach(pipe1) ""
      tk_messageBox -type ok -icon info -parent .main -title "Scid" \
          -message "The analysis engine 1 terminated without warning; it probably crashed or had an internal error."
    }
    
    if {! $analysisCoach(seen1)} {
      # First line of output from the program, so send initial commands:
      set analysisCoach(seen1) 1
      ::tacgame::sendToEngine 1 "xboard"
      ::tacgame::sendToEngine 1 "post"
    }
    
    ::tacgame::makePhalanxMove $line
    
  }
  
  # ======================================================================
  # startAnalyzeMode:
  #   Put the engine in analyze mode
  # ======================================================================
  proc startAnalyze { } {
    global ::tacgame::analysisCoach ::tacgame::isLimitedAnalysisTime ::tacgame::analysisTime
    set n 2
    set ::analysis(waitForReadyOk$n) 1
    ::uci::sendToEngine $n "isready"
    vwait ::analysis(waitForReadyOk$n)
    ::uci::sendToEngine $n "position fen [sc_pos fen]"
    ::uci::sendToEngine $n "go infinite"
    
    if { $isLimitedAnalysisTime == 1 }  {
      after [expr 1000 * $analysisTime] ::tacgame::stopAnalyze
    }
    
  }
  # ======================================================================
  # stopAnalyzeMode:
  #   Stop the engine analyze mode
  # ======================================================================
  proc stopAnalyze { } {
    global ::tacgame::analysisCoach ::tacgame::isLimitedAnalysisTime ::tacgame::analysisTime
    
    after cancel ::tacgame::stopAnalyze
    ::uci::sendToEngine 2 "stop"
  }
  ################################################################################
  # returns true if last move is a mate and stops clocks
  ################################################################################
  proc endOfGame {} {
    if { [string index [sc_game info previousMove] end ] == "#"} {
      return 1
    }
    return 0
  }
  # ======================================================================
  # phalanxGo
  #		it is phalanx's turn to play
  # ======================================================================
  proc phalanxGo {} {
    global ::tacgame::analysisCoach ::tacgame::openingType ::tacgame::openingMovesList \
        ::tacgame::openingMovesHash ::tacgame::openingMoves ::tacgame::outOfOpening
    
    after cancel ::tacgame::phalanxGo
    
    if { [::tacgame::endOfGame] } {
      ::tacgame::abortGame
      return
    }
    
    # check if Phalanx is already thinking
    if { $analysisCoach(automoveThinking1) == 1 } {
      after 1000 ::tacgame::phalanxGo
      return
    }
    
    updateAnalysisText
    
    if { [sc_pos side] != [::tacgame::getPhalanxColor] } {
      after 1000 ::tacgame::phalanxGo
      return
    }
    
    toggleClocks
    repetition
    
    # make a move corresponding to a specific opening, (it is Phalanx's turn)
    if {$openingType == "specific" && !$outOfOpening} {
      set index 0
      # Warn if the user went out of the opening line chosen
      if { !$outOfOpening } {
        set ply [ expr [sc_pos moveNumber] * 2 - 1]
        if { [sc_pos side] == "white" } {
          set ply [expr $ply - 1]
        }
        
        if { [lsearch $openingMovesHash [sc_pos hash]] == -1 && [llength $openingMovesList] >= $ply} {
          set answer [tk_messageBox -icon question -parent .main -title $::tr(OutOfOpening) -type yesno \
              -message "$::tr(NotFollowedLine) $openingMoves\n $::tr(DoYouWantContinue)" ]
          if {$answer == no} {
            sc_move back 1
            toggleClocks
            after 1000 ::tacgame::phalanxGo
            return
          }  else  {
            set outOfOpening 1
          }
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
          
          ::utils::sound::AnnounceNewMove $move
          
          toggleClocks
          repetition
          after 1000 ::tacgame::phalanxGo
          return
        }
      }
      
    }
    
    # Pascal Georges : original Phalanx does not have 'setboard'
    set analysisCoach(automoveThinking1) 1
    sendToEngine 1 "setboard [sc_pos fen]"
    sendToEngine 1 "go"
    after 1000 ::tacgame::phalanxGo
  }
  ################################################################################
  #   add current position for 3fold repetition detection and returns 1 if
  # the position is a repetition
  ################################################################################
  proc repetition {} {
    set elt [lrange [split [sc_pos fen]] 0 2]
    # append the position only if different from the last element
    if { $elt != [ lindex $::tacgame::lFen end ] } {
      lappend ::tacgame::lFen $elt
    }
    if { [llength [lsearch -all $::tacgame::lFen $elt] ] >=3 } {
      tk_messageBox -type ok -message $::tr(Draw) -parent .main -icon info
      return 1
    }
    return 0
  }
  # ======================================================================
  # makePhalanxMove:
  #
  # ======================================================================
  proc makePhalanxMove { input } {
    global ::tacgame::lscore ::tacgame::analysisCoach ::tacgame::currentPosHash ::tacgame::resignCount
    
    # The input move is of the form "my move is MOVE"
    if {[scan $input "my move is %s" move] != 1} { return 0 }
    
    ::tacgame::stopAnalyze
    
    # Phalanx will move : update the score list to detect any blunder
    if {[info exists ::tacgame::sc1]} {
      lappend lscore $::tacgame::sc1
    }
    
    # if the resign value has been reached more than 3 times in a raw, resign
    if { ( [getPhalanxColor] == "black" && [lindex $lscore end] >  $::informant("++-") ) || \
          ( [getPhalanxColor] == "white" && [lindex $lscore end] < [expr 0.0 - $::informant("++-")] ) } {
      incr resignCount
    } else  {
      set resignCount 0
    }
    
    # check the sequence of moves
    # in case of any event (board setup, move back/forward), reset score list
    if { ![sc_pos isAt start] && ![sc_pos isAt vstart]} {
      sc_move back 1
      if { [sc_pos hash] != $currentPosHash} {
        set lscore {}
        updateAnalysisText
      }
      sc_move forward 1
    } else  {
      if { [sc_pos hash] != $currentPosHash} {
        set lscore {}
        updateAnalysisText
      }
    }
    
    # play the move
    set action "replace"
    if {![sc_pos isAt vend]} { set action [confirmReplaceMove] }
    if {$action == "replace"} {
      if {[catch {sc_move addSan $move}]} {
        # No move from Phalanx : remove the score (last element)
        set lscore [lreplace $lscore end end]
        return 0
      }
    } elseif {$action == "var"} {
      sc_var create
      if {[catch {sc_move addSan $move}]} {
        # No move from Phalanx : remove the score (last element)
        set lscore [lreplace $lscore end end]
        return 0
      }
    } elseif {$action == "mainline"} {
      sc_var create
      if {[catch {sc_move addSan $move}]} {
        # No move from Phalanx : remove the score (last element)
        set lscore [lreplace $lscore end end]
        return 0
      }
      sc_var promote
      sc_move forward 1
    }
    if { $::tacgame::showevaluation == 1 && [info exists ::uci::uciInfo(score2)] } {
      storeEvalComment $::uci::uciInfo(score2)
    }
    
    set analysisCoach(automoveThinking1) 0
    set currentPosHash [sc_pos hash]
    
    ::tacgame::startAnalyze
    ::utils::sound::AnnounceNewMove $move
    
    toggleClocks
    repetition
    
    if { $resignCount > 3 } {
      tk_messageBox -type ok -message $::tr(Iresign) -parent .main -icon info
      set resignCount 0
    }
    
    return 1
  }
  
  # ======================================================================
  # updateScore
  # ======================================================================
  proc updateScore { } {
    if { ! $::tacgame::showevaluation } { return }
    if {![info exists ::uci::uciInfo(score2)]} {
      set ::tacgame::scoreLabel ""
      return
    } else {
      set ::tacgame::scoreLabel "Score: $::uci::uciInfo(score2)"
    }
  }
  
  # ======================================================================
  # updateAnalysisText
  #   Update the text in an analysis window.
  #   Human blunders are not checked, only Phalanx'one
  # ======================================================================
  proc updateAnalysisText { } {
    global ::tacgame::analysisCoach ::tacgame::showblunder ::tacgame::blunderWarningLabel \
        ::tacgame::showblunder ::tacgame::showblundervalue ::tacgame::showblunderfound ::tacgame::showmovevalue \
        ::tacgame::showevaluation ::tacgame::lscore ::tacgame::threshold \
        ::tacgame::lastblundervalue ::tacgame::prev_lastblundervalue ::tacgame::scoreLabel \
        ::tacgame::blunderpending ::tacgame::prev_blunderpending ::tacgame::sc1
    
    # only update when it is human turn
    if { [getPhalanxColor] == [sc_pos side] } { return }
    
    catch {
      set sc1 $::uci::uciInfo(score2)
      set sc2 [lindex $lscore end]
    }
    
    # There are less than 2 scores in the list
    if {[llength $lscore] < 2} {
      set blunderWarningLabel $::tr(Noinfo)
      set scoreLabel ""
      if {[llength $lscore] == 1 && $showevaluation } {
        set scoreLabel "Score : [lindex $lscore end]"
      }
      return
    }
    
    # set sc1 [lindex $lscore end]
    # set sc2 [lindex $lscore end-1]
    
    if { $analysisCoach(automoveThinking1) } {
      set blunderWarningLabel $::tr(Noinfo)
    }
    
    # Check if a blunder was made by Phalanx at last move.
    # The check is done during player's turn
    if { $showblunder && [::tacgame::getPhalanxColor] != [sc_pos side] } {
      if {[llength $lscore] >=2} {
        if { ($sc1 - $sc2 > $threshold && [::tacgame::getPhalanxColor] == "black") || \
              ($sc1 - $sc2 < [expr 0.0 - $threshold] && [::tacgame::getPhalanxColor] == "white") } {
          set lastblundervalue [expr $sc1-$sc2]
          # append a ?!, ? or ?? to the move if there is none yet and if the game was not dead yet
          # (that is if the score was -6, if it goes down to -10, this is a normal evolution
          if { [expr abs($sc2)] < $::informant("++-") } {
            sc_pos clearNags
            set b [expr abs($lastblundervalue)]
            if { $b >= $::informant("?!") && $b < $::informant("?") } {
              sc_pos addNag "?!"
            } elseif { $b >= $::informant("?") && $b < $::informant("??") }  {
              sc_pos addNag "?"
            } elseif  { $b >= $::informant("??") } {
              sc_pos addNag "??"
            }
          }
          
          .coachWin.finformations.l1 configure -background LightCoral
          if { $showblundervalue } {
            set tmp $::tr(blunder)
            append tmp [format " %+8.2f" [expr abs($sc1-$sc2)]]
            set blunderWarningLabel $tmp
            set blunderpending 1
          } else {
            set blunderWarningLabel "$::tr(blunder) !"
          }
        } else {
          sc_pos clearNags
          .coachWin.finformations.l1 configure -background linen
          set blunderWarningLabel $::tr(Noblunder)
          set blunderpending 0
        }
      }
    } else {
      set blunderWarningLabel "---"
    }
    
    if { !$showblunder || $analysisCoach(automoveThinking1) } {
      set blunderWarningLabel "---"
    }
    
    # displays current score sent by the "good" engine (Toga)
    if { $showevaluation } {
      set scoreLabel "Score : $sc1"
    } else {
      set scoreLabel ""
    }
  }
  
  # ======================================================================
  # getPhalanxColor
  #   Returns "white" or "black" (Phalanx always plays at top)
  # ======================================================================
  proc getPhalanxColor {} {
    # Phalanx always plays for the upper side
    if { [::board::isFlipped .main.board] == 0 } {
      return "black"
    } else  {
      return "white"
    }
  }
  
  ################################################################################
  #
  ################################################################################
  set openingList [ list \
      "$::tr(Reti): 1.Nf3" \
      "$::tr(English): 1.c4" \
      "$::tr(d4Nf6Miscellaneous): 1.d4 Nf6" \
      "$::tr(Trompowsky): 1.d4 Nf6 2.Bg5" \
      "$::tr(Budapest): 1.d4 Nf6 2.c4 e5" \
      "$::tr(OldIndian): 1.d4 Nf6 2.c4 d6" \
      "$::tr(BenkoGambit): 1.d4 Nf6 2.c4 c5 3.d5 b5" \
      "$::tr(ModernBenoni): 1.d4 Nf6 2.c4 c5 3.d5 e6" \
      "$::tr(DutchDefence): 1.d4 f5" \
      "1.e4" \
      "$::tr(Scandinavian): 1.e4 d5" \
      "$::tr(AlekhineDefence): 1.e4 Nf6" \
      "$::tr(Pirc): 1.e4 d6" \
      "$::tr(CaroKann): 1.e4 c6" \
      "$::tr(CaroKannAdvance): 1.e4 c6 2.d4 d5 3.e5" \
      "$::tr(Sicilian): 1.e4 c5" \
      "$::tr(SicilianAlapin): 1.e4 c5 2.c3" \
      "$::tr(SicilianClosed): 1.e4 c5 2.Nc3" \
      "$::tr(Sicilian): 1.e4 c5 2.Nf3 Nc6" \
      "$::tr(Sicilian): 1.e4 c5 2.Nf3 e6" \
      "$::tr(SicilianRauzer): 1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 Nc6" \
      "$::tr(SicilianDragon): 1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 g6 " \
      "$::tr(SicilianScheveningen): 1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 e6" \
      "$::tr(SicilianNajdorf): 1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 a6" \
      "$::tr(OpenGame): 1.e4 e5" \
      "$::tr(Vienna): 1.e4 e5 2.Nc3" \
      "$::tr(KingsGambit): 1.e4 e5 2.f4" \
      "$::tr(RussianGame): 1.e4 e5 2.Nf3 Nf6" \
      "$::tr(OpenGame): 1.e4 e5 2.Nf3 Nc6" \
      "$::tr(ItalianTwoKnights): 1.e4 e5 2.Nf3 Nc6 3.Bc4" \
      "$::tr(Spanish): 1.e4 e5 2.Nf3 Nc6 3.Bb5" \
      "$::tr(SpanishExchange): 1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Bxc6" \
      "$::tr(SpanishOpen): 1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6 5.O-O Nxe4" \
      "$::tr(SpanishClosed): 1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6 5.O-O Be7" \
      "$::tr(FrenchDefence): 1.e4 e6" \
      "$::tr(FrenchAdvance): 1.e4 e6 2.d4 d5 3.e5" \
      "$::tr(FrenchTarrasch): 1.e4 e6 2.d4 d5 3.Nd2" \
      "$::tr(FrenchWinawer): 1.e4 e6 2.d4 d5 3.Nc3 Bb4" \
      "$::tr(FrenchExchange): 1.e4 e6 2.d4 d5 3.exd5 exd5" \
      "$::tr(QueensPawn): 1.d4 d5" \
      "$::tr(Slav): 1.d4 d5 2.c4 c6" \
      "$::tr(QGA): 1.d4 d5 2.c4 dxc4" \
      "$::tr(QGD): 1.d4 d5 2.c4 e6" \
      "$::tr(QGDExchange): 1.d4 d5 2.c4 e6 3.cxd5 exd5" \
      "$::tr(SemiSlav): 1.d4 d5 2.c4 e6 3.Nc3 Nf6 4.Nf3 c6" \
      "$::tr(QGDwithBg5): 1.d4 d5 2.c4 e6 3.Nc3 Nf6 4.Bg5" \
      "$::tr(QGDOrthodox): 1.d4 d5 2.c4 e6 3.Nc3 Nf6 4.Bg5 Be7 5.e3 O-O 6.Nf3 Nbd7" \
      "$::tr(Grunfeld): 1.d4 Nf6 2.c4 g6 3.Nc3 d5" \
      "$::tr(GrunfeldExchange): 1.d4 Nf6 2.c4 g6 3.Nc3 d5 4.cxd5" \
      "$::tr(GrunfeldRussian): 1.d4 Nf6 2.c4 g6 3.Nc3 d5 4.Nf3 Bg7 5.Qb3" \
      "$::tr(Catalan): 1.d4 Nf6 2.c4 e6 3.g3 " \
      "$::tr(CatalanOpen): 1.d4 Nf6 2.c4 e6 3.g3 d5 4.Bg2 dxc4" \
      "$::tr(CatalanClosed): 1.d4 Nf6 2.c4 e6 3.g3 d5 4.Bg2 Be7" \
      "$::tr(QueensIndian): 1.d4 Nf6 2.c4 e6 3.Nf3 b6" \
      "$::tr(NimzoIndian): 1.d4 Nf6 2.c4 e6 3.Nc3 Bb4" \
      "$::tr(NimzoIndianClassical): 1.d4 Nf6 2.c4 e6 3.Nc3 Bb4 4.Qc2" \
      "$::tr(NimzoIndianRubinstein): 1.d4 Nf6 2.c4 e6 3.Nc3 Bb4 4.e3" \
      "$::tr(KingsIndian): 1.d4 Nf6 2.c4 g6" \
      "$::tr(KingsIndianSamisch): 1.d4 Nf6 2.c4 g6 4.e4 d6 5.f3" \
      "$::tr(KingsIndianMainLine): 1.d4 Nf6 2.c4 g6 4.e4 d6 5.Nf3" \
      ]
}
###
### End of file: tacgame.tcl
###
