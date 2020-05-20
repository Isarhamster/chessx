### reviewgame.tcl: part of Scid.
### Copyright (C) 2009  Pascal Georges
###
######################################################################
### Try to guess the moves of a game
#

# TODO :
# - permettre tourner l'échiquier après le démarrage

namespace eval reviewgame {
  
  set prevScore 0
  set prevLine ""
  set nextEngineMove ""
  set prevFen ""
  set engineSlot 6
  set window ".reviewgame"
  set timeShort 5
  set timeExtended 1
  set margin 0.3
  set moveOfGameIsBest 0
  # The score of the move really played
  set scoreGame 0.0
  # The score of the user's move
  set scorePlayed 0.0
  # Score of the engine
  set scoreEngine 0.0
  
  set sequence 0
  
  array set analysisEngine {}
  
  set progressBarStep 1
  set progressBarTimer 0
  set autoProceed 0
}

################################################################################
#
################################################################################
proc ::reviewgame::start {} {
  if { ! [::reviewgame::launchengine] } {
    tk_messageBox -type ok -icon warning -title "Scid" -message "This feature require at least one UCI engine"
    return
  }

  set w $::reviewgame::window
  createToplevel $w
  setTitle $w [::tr "GameReview" ]
  wm minsize $w 200 200
  
  ttk::frame $w.fgameinfo
  set welo [sc_game tags get WhiteElo]
  set belo [sc_game tags get BlackElo]
  if { $welo == "0"} { set welo "-" }
  if { $belo == "0"} { set belo "-" }
  ttk::label $w.fgameinfo.l1 -text "[sc_game tags get White] ($welo) - [sc_game tags get Black] ($belo)"
  set result [sc_game tags get Result]
  if { $result == "1" } { set result "1-0" }
  if { $result == "0" } { set result "0-1" }
  if { $result == "=" } { set result "1/2 - 1/2" }
  ttk::label $w.fgameinfo.l2 -text "$result"
  pack $w.fgameinfo.l1 $w.fgameinfo.l2
  pack $w.fgameinfo -expand 1 -fill both
  
  ttk::frame $w.fparam
  ttk::label $w.fparam.ltime1 -text "[::tr Time] ([::tr sec])"
  ttk::spinbox $w.fparam.time1 -values { 5 10 15 30 45 60 90 120 } -command { set ::reviewgame::timeShort [$::reviewgame::window.fparam.time1 get] } -width 7
  $w.fparam.time1 set $::reviewgame::timeShort
  ttk::label $w.fparam.ltime2 -text "[::tr GameReviewTimeExtended] ([ ::tr min])"
  ttk::spinbox $w.fparam.time2 -values { 1 2 3 4 5} -command { set ::reviewgame::timeExtended [$::reviewgame::window.fparam.time1 get] } -width 7
  $w.fparam.time2 set $::reviewgame::timeExtended
  ttk::label $w.fparam.lmargin -text "[::tr GameReviewMargin]"
  ttk::spinbox $w.fparam.margin -from 0.1 -to 1.0 -increment 0.1 -command { set ::reviewgame::margin [$::reviewgame::window.fparam.time1 get] } -width 7
  $w.fparam.margin set $::reviewgame::margin
  
  set row 0
  grid $w.fparam.ltime1 -column 0 -row $row -sticky nw
  grid $w.fparam.time1 -column 1 -row $row -sticky nw
  incr row
  grid $w.fparam.ltime2 -column 0 -row $row -sticky nw
  grid $w.fparam.time2 -column 1 -row $row -sticky nw
  incr row
  grid $w.fparam.lmargin -column 0 -row $row -sticky nw
  grid $w.fparam.margin -column 1 -row $row -sticky nw
  incr row
  
  ttk::checkbutton $w.fparam.cbproceed -text "[::tr GameReviewAutoContinue]" -variable ::reviewgame::autoProceed
  grid $w.fparam.cbproceed -column 0 -row $row  -columnspan 2 -sticky nw
  
  pack $w.fparam -expand 1 -fill both
  
  ttk::frame $w.finfo
  pack $w.finfo -expand 1 -fill both
  ttk::progressbar $w.finfo.pb -orient horizontal -length 100 -value 0 -mode determinate
  ttk::label $w.finfo.pblabel -image tb_stop -compound left
  ttk::label $w.finfo.sc1 -text "-"
  ttk::label $w.finfo.sc2 -text "-"
  ttk::label $w.finfo.sc3 -foreground blue -text "-"
  ttk::button $w.finfo.proceed -textvar ::tr(Continue) -command ::reviewgame::proceed
  ttk::button $w.finfo.extended -text "[::tr GameReviewReCalculate]" -command ::reviewgame::extendedTime
  
  set row 0
  grid $w.finfo.pb -column 0 -row $row -sticky nw
  incr row
  grid $w.finfo.pblabel -column 0 -row $row -sticky nw
  incr row
  grid $w.finfo.proceed -column 0 -row $row -sticky nw
  grid $w.finfo.extended -column 1 -row $row -sticky nw
  incr row
  grid $w.finfo.sc1 -column 0 -row $row  -columnspan 2 -sticky nw
  incr row
  grid $w.finfo.sc2 -column 0 -row $row  -columnspan 2 -sticky nw
  incr row
  grid $w.finfo.sc3 -column 0 -row $row  -columnspan 2 -sticky nw
  incr row
  
  ttk::button $w.finfo.sol -text [::tr ShowSolution ] -command ::reviewgame::showSolution
  grid $w.finfo.sol -column 0 -row $row  -sticky nw
  incr row
  
  # Display statistics
  ttk::label $w.finfo.stats -text ""
  grid $w.finfo.stats -column 0 -row $row -sticky nw -columnspan 3

  ttk::frame $w.fbuttons
  pack $w.fbuttons -fill x
  ttk::button $w.fbuttons.close -textvar ::tr(Abort) -command ::reviewgame::endTraining
  pack $w.fbuttons.close -expand 1 -fill x
  
  set ::reviewgame::boardFlipped [::board::isFlipped .main.board]
  
  bind $w <Destroy> "if {\[string equal $w %W\]} {::reviewgame::endTraining}"
  bind $w <F1> { helpWindow ReviewGame }
  ::createToplevelFinalize $w
  set ::reviewgame::prevFen [sc_pos fen]
  set ::reviewgame::movesLikePlayer 0
  set ::reviewgame::movesLikeEngine 0
  set ::reviewgame::numberMovesPlayed 0
  ::setPlayMode "::reviewgame::callback"
  ::reviewgame::resetValues
  ::reviewgame::mainLoop
  
}

proc ::reviewgame::callback {cmd} {
  switch $cmd {
      stop { destroy $::reviewgame::window }
  }
  return 0
}

################################################################################
#
################################################################################
proc ::reviewgame::showSolution {} {
  set w $::reviewgame::window
  $w.finfo.sol configure -text "[ sc_game info nextMove ]"
  set ::reviewgame::solutionDisplayed 1
}
################################################################################
#
################################################################################
proc ::reviewgame::endTraining {} {
  set w $::reviewgame::window
  
  after cancel ::reviewgame::mainLoop
  set ::reviewgame::bailout 1
  set ::reviewgame::sequence 0
  after cancel ::reviewgame::stopAnalyze
  ::reviewgame::stopAnalyze
  focus .
  bind $w <Destroy> {}
  ::win::closeWindow $w
  ::setPlayMode ""
  
  catch { ::uci::closeUCIengine $::reviewgame::engineSlot }
}
################################################################################
#
################################################################################
proc ::reviewgame::isPlayerTurn {} {
  if { [sc_pos side] == "white" &&  ![::board::isFlipped .main.board] || [sc_pos side] == "black" &&  [::board::isFlipped .main.board] } {
    return 1
  }
  return 0
}
################################################################################
# Handle the case where position was changed not during normal play but certainly with
# move back / forward / rewind commands
################################################################################
proc ::reviewgame::abnormalContinuation {} {
  ::reviewgame::stopAnalyze
  ::reviewgame::resetValues
  ::windows::stats::Refresh
  updateBoard -pgn
  if { [sc_pos side] == "white" && [::board::isFlipped .main.board] || [sc_pos side] == "black" &&  ![::board::isFlipped .main.board] } {
    ::board::flip .main.board
  }
  set ::reviewgame::prevFen [sc_pos fen]
  ::reviewgame::startAnalyze
  ::reviewgame::mainLoop
}
################################################################################
# waits for the user to play and check the move played
################################################################################
proc ::reviewgame::mainLoop {} {
  global ::reviewgame::prevScore ::reviewgame::prevLine ::reviewgame::analysisEngine ::reviewgame::nextEngineMove
  global ::reviewgame::sequence ::reviewgame::useExtendedTime
  set w $::reviewgame::window
  
  after cancel ::reviewgame::mainLoop
  
  if { ! [ checkConsistency ] } { puts "ERROR checkConsistency returns false" ; return }
  
  if { $useExtendedTime } {
    set ::reviewgame::thinkingTime [expr $::reviewgame::timeExtended * 60 ]
  } else {
    set ::reviewgame::thinkingTime $::reviewgame::timeShort
  }
  
  # in start position, it must be user's turn
  if { ! [::reviewgame::isPlayerTurn] && $sequence == 0} {
    if { [ sc_game info nextMoveNT ] != ""} {
      ::move::Forward
    }
  }
  
  $w.finfo.proceed configure -state disabled
  $w.finfo.sol configure -state disabled
  
  # Phase 1 : analyze the move really played during the game
  if {$sequence == 0} {
    $w.finfo.sc1 configure -text ""
    $w.finfo.sc2 configure -text ""
    set ::reviewgame::movePlayed [ sc_game info nextMoveNT ]
    if {$::reviewgame::movePlayed == ""} {
      return
    }
    $w.finfo.pblabel configure -image tb_stop -text "[::tr GameReviewAnalyzingMovePlayedDuringTheGame]"
    ::reviewgame::startAnalyze $::reviewgame::thinkingTime $::reviewgame::movePlayed
    vwait ::reviewgame::sequence
    if { $::reviewgame::bailout } { return }
  }
  
  # Phase 2 : find the best engine move in current position
  if { $sequence == 1 } {
    $w.finfo.pblabel configure -image tb_stop -text "[::tr GameReviewAnalyzingThePosition]"
    ::reviewgame::startAnalyze $::reviewgame::thinkingTime
    vwait ::reviewgame::sequence
    if { $::reviewgame::bailout } { return }
  }
  
  $w.finfo.pblabel configure -image tb_play -text "[::tr GameReviewEnterYourMove]"
  $w.finfo.sol configure -state normal
  
  # is this player's turn (which always plays from bottom of the board) ?
  if { [::reviewgame::isPlayerTurn] } {
    after 1000  ::reviewgame::mainLoop
    return
  }
  
  $w.finfo.sol configure -text "[::tr ShowSolution]"
  
  checkPlayerMove
  
  $w.finfo.proceed configure -state normal
  $w.finfo.extended configure -state normal
  updateStats
  set ::reviewgame::useExtendedTime 0
  after 1000  ::reviewgame::mainLoop
}
################################################################################
#
################################################################################
proc ::reviewgame::checkPlayerMove {} {
  global ::reviewgame::prevScore ::reviewgame::prevLine ::reviewgame::analysisEngine ::reviewgame::nextEngineMove
  global ::reviewgame::sequence ::reviewgame::useExtendedTime
  set w $::reviewgame::window
  
  incr ::reviewgame::numberMovesPlayed
  
  # Phase 3 : ponder on user's move if different of best engine move and move played
  # We know user has played
  set user_move [sc_game info previousMoveNT]
  
  # ponder on user's move if he did not play the same move as in match
  if {$user_move != $::reviewgame::movePlayed} {
    $w.finfo.pblabel configure -image tb_stop -text "[::tr GameReviewCheckingYourMove]"
    $w.finfo.sc3 configure -text ""
    ::reviewgame::startAnalyze $::reviewgame::thinkingTime ;#$user_move
    vwait ::reviewgame::sequence
    if { $::reviewgame::bailout } { return }
    $w.finfo.pblabel configure -image tb_stop -text "[::tr GameReviewYourMoveWasAnalyzed]"
    # display user's score
    $w.finfo.sc3 configure -text "[::tr GameReviewScoreOfYourMove] : $analysisEngine(score,2)"
  }
  
  # User guessed the correct move played in game
  if {$user_move == $::reviewgame::movePlayed } {
    
    set  ::reviewgame::sequence 0
    
    $w.finfo.sc3 configure -text "[::tr GameReviewYouPlayedSameMove]" -foreground "sea green"
    if {  ! $::reviewgame::solutionDisplayed } {
      incr ::reviewgame::movesLikePlayer
    }
    # Starting with Scid 4.1, when the move entered already exists it is not added but we simply move forward, so the code below is useless
    # set var [sc_var number]
    # sc_var exit
    # sc_var delete $var
    # sc_move forward
    # updateBoard -animate -pgn
    
    # display played move score
    $w.finfo.sc2 configure -text "[::tr GameReviewGameMoveScore] : $analysisEngine(score,1) [::trans $::reviewgame::movePlayed]"
    # display engine's score
    $w.finfo.sc1 configure -text "[::tr GameReviewEngineScore] : $analysisEngine(score,2) [::trans [lindex $analysisEngine(moves,2) 0]]"
    if { $::reviewgame::autoProceed } {
      # guessed the correct move played during the game, so continue directly
      proceed
    }
    $w.finfo.pblabel configure -image tb_play -text ""
    set sequence 0
  } elseif { $user_move == [ lindex $analysisEngine(moves,2) 0] || [ isGoodScore $analysisEngine(score,2) $analysisEngine(score,3)  ] } {
    
    set  ::reviewgame::sequence 0
    
    # User guessed engine's move
    if {$user_move == [ lindex $analysisEngine(moves,2) 0]} {
      $w.finfo.sc3 configure -text "[::tr GameReviewYouPlayedLikeTheEngine]" -foreground "sea green"
      incr ::reviewgame::movesLikeEngine
      
    } else  {
      $w.finfo.sc3 configure -text "[::tr GameReviewNotEngineMoveButGoodMove] : [::trans $user_move] ($analysisEngine(score,3))" -foreground blue
    }
    sc_var exit
    sc_move forward
    updateBoard -animate -pgn
    # display played move score
    $w.finfo.sc2 configure -text "[::tr GameReviewGameMoveScore] : $analysisEngine(score,1) [::trans $::reviewgame::movePlayed]"
    # display engine's score
    $w.finfo.sc1 configure -text "[::tr GameReviewEngineScore] $analysisEngine(score,2) [::trans [lindex $analysisEngine(moves,2) 0]]"
  } else  {
    
    # user played a bad move : comment it and restart the process
    
    set  ::reviewgame::sequence 2
    
    $w.finfo.sc3 configure -text "[::tr GameReviewMoveNotGood] $analysisEngine(score,3) \n([::trans $analysisEngine(moves,3)])" -foreground red
    sc_pos addNag "?"
    
    # Instead of adding info in comments, add real variations
    # sc_pos setComment "($analysisEngine(score,3)) $analysisEngine(moves,3) Engine : ($analysisEngine(score,2)) \n[::trans $analysisEngine(moves,2)]"
    sc_pos setComment "($analysisEngine(score,3))"
    sc_move addSan $analysisEngine(moves,3)
    sc_var exit
    sc_var create
    sc_pos setComment "Engine : ($analysisEngine(score,2))"
    sc_move addSan $analysisEngine(moves,2)
    sc_var exit
    updateBoard -pgn
    
    # allows a re-calculation
    $w.finfo.extended configure -state normal
    
    # display played move score
    $w.finfo.sc2 configure -text "[::tr GameReviewGameMoveScore] : $analysisEngine(score,1)"
    # display engine's score
    $w.finfo.sc1 configure -text "[::tr GameReviewEngineScore] $analysisEngine(score,2)"
    set  ::reviewgame::sequence 2
    # after 1000 ::reviewgame::mainLoop
    # return
  }
  
}
################################################################################
#
################################################################################
proc ::reviewgame::updateStats {} {
  
  set l $::reviewgame::window.finfo.stats
  if { ![::board::isFlipped .main.board] } {
    set player [sc_game info white]
  } else  {
    set player [sc_game info black]
  }
  
  $l configure -text "[::tr GameReviewMovesPlayedLike] $player : $::reviewgame::movesLikePlayer / $::reviewgame::numberMovesPlayed\n[::tr GameReviewMovesPlayedEngine] : $::reviewgame::movesLikeEngine / $::reviewgame::numberMovesPlayed"
}
################################################################################
#
################################################################################
proc ::reviewgame::isGoodScore {engine player} {
  global ::reviewgame::margin
  if { ![::board::isFlipped .main.board] } {
    # if player plays white
    if {$player >= [expr $engine - $margin]} {
      return 1
    }
  } else  {
    if {$player <= [expr $engine + $margin]} {
      return 1
    }
  }
  return 0
}
################################################################################
## resetValues
#   Resets global data.
################################################################################
proc ::reviewgame::resetValues {} {
  set ::reviewgame::prevScore 0
  set ::reviewgame::prevLine ""
  set ::reviewgame::nextEngineMove ""
  set ::reviewgame::prevFen ""
  set ::reviewgame::sequence 0
  set ::reviewgame::analysisEngine(analyzeMode) 0
  set ::reviewgame::bailout 0
  set ::reviewgame::useExtendedTime 0
  set ::reviewgame::solutionDisplayed 0
}

################################################################################
#  Will start engine
# in case of an error, return 0, or 1 if the engine is ok
################################################################################
proc ::reviewgame::launchengine {} {
  global ::reviewgame::analysisEngine
  
  ::uci::resetUciInfo $::reviewgame::engineSlot
  
  set analysisEngine(analyzeMode) 0
  
  # find engine
  set engineFound 0
  set index 0
  foreach e $::engines(list) {
    if {[lindex $e 7] != 0} {
      set engineFound 1
      break
    }
    incr index
  }
  if { ! $engineFound } {
    return 0
  }
  
  ::uci::startEngine $index $::reviewgame::engineSlot ;# start engine in analysis mode
  return 1
}

# ======================================================================
# sendToEngine:
#   Send a command to a running analysis engine.
# ======================================================================
proc ::reviewgame::sendToEngine {text} {
  ::uci::sendToEngine $::reviewgame::engineSlot $text
}

# ======================================================================
# startAnalyzeMode:
#   Put the engine in analyze mode, from current position after move played (in UCI format), time is in seconds
# ======================================================================
proc ::reviewgame::startAnalyze { analysisTime { move "" } } {
  global ::reviewgame::analysisEngine ::reviewgame::engineSlot
  
  set pb $::reviewgame::window.finfo.pb
  set length [$pb cget -maximum]
  set ::reviewgame::progressBarTimer  [expr ( $analysisTime * 1000 * $::reviewgame::progressBarStep ) / $length ]
  after $::reviewgame::progressBarTimer ::reviewgame::updateProgressBar
  
  # Check that the engine has not already had analyze mode started:
  if {$analysisEngine(analyzeMode)} {
    ::reviewgame::sendToEngine  "exit"
  }
  set analysisEngine(analyzeMode) 1
  after cancel ::reviewgame::stopAnalyze
  
  # we want to ponder on a particular move, hence we need to switch to a temporary position so
  # UCI code can correctly format the variations
  if {$move != ""} {
    sc_game push copyfast
    sc_move addSan $move
    set ::analysis(fen$engineSlot) [sc_pos fen]
    sc_game pop
  } else  {
    set ::analysis(fen$engineSlot) [sc_pos fen]
  }
  
  ::reviewgame::sendToEngine "position fen $::analysis(fen$engineSlot) $move"
  ::reviewgame::sendToEngine "go infinite"
  after [expr 1000 * $analysisTime] "::reviewgame::stopAnalyze $move"
}
# ======================================================================
# stopAnalyzeMode:
#   Stop the engine analyze mode
# ======================================================================
proc ::reviewgame::stopAnalyze { { move "" } } {
  global ::reviewgame::analysisEngine ::reviewgame::sequence
  
  # Check that the engine has already had analyze mode started:
  if { ! $analysisEngine(analyzeMode) } { return }
  
  after cancel ::reviewgame::updateProgressBar
  if { [winfo exists $::reviewgame::window.finfo.pb]} {
    $::reviewgame::window.finfo.pb configure -value 0
  }

  incr ::reviewgame::sequence
  set pv [lindex $::analysis(multiPV$::reviewgame::engineSlot) 0]
  set analysisEngine(score,$sequence) [lindex $pv 1]
  set analysisEngine(moves,$sequence) [lindex $pv 2]
  
  set analysisEngine(analyzeMode) 0
  ::reviewgame::sendToEngine  "stop"
}
################################################################################
#
################################################################################
proc ::reviewgame::proceed {} {
  # next cycle
  ::move::Forward
  ::reviewgame::resetValues
  set ::reviewgame::prevFen [sc_pos fen]
  after 1000 ::reviewgame::mainLoop
}
################################################################################
# Rethink on the position with extended time
################################################################################
proc ::reviewgame::extendedTime {} {
  
  # if already calculating, do nothing
  if { $::reviewgame::analysisEngine(analyzeMode)} {
    return
  }
  
  if { ![::reviewgame::isPlayerTurn] } {
    ::move::Back
  }
  
  set ::reviewgame::useExtendedTime 1
  set ::reviewgame::sequence 0
  ::reviewgame::mainLoop
  
}
################################################################################
#
################################################################################
proc ::reviewgame::updateProgressBar {} {
  $::reviewgame::window.finfo.pb step $::reviewgame::progressBarStep
  after $::reviewgame::progressBarTimer ::reviewgame::updateProgressBar
}
################################################################################
#
################################################################################
proc ::reviewgame::checkConsistency {} {
  if { $::reviewgame::boardFlipped != [::board::isFlipped .main.board] } {
    tk_messageBox -type ok -icon warning -title "Scid" -message "Choose the side BEFORE starting the exercise"
    return 0
  }
  return 1
}

################################################################################
#   returns 1 if the player is allowed to enter a move (pondering is done)
################################################################################
proc ::reviewgame::playerCanMove {} {
  if { ! [winfo exists $::reviewgame::window] } { return 1 }
  
  if { !  [::reviewgame::isPlayerTurn]  } {
    return 0
  } elseif { $::reviewgame::sequence == 2 } {
    return 1
  }
  
  return 0
}

###
### End of file: reviewgame.tcl
###
