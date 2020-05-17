###
### wbdetect.tcl: part of Scid.
### Copyright (C) 1999-2002  Shane Hudson.
#      Copyright (C) 2006-2007  Pascal Georges.
###

######################################################################
#
# Code to detect various Winboard engines being used as analysis
# engines in Scid.
#
# Thanks to Allen Lake for testing many WinBoard engines
# with Scid in Windows and providing this code.
#
# Most cases below are for engines that have analyze mode but
# do not let Scid know about it by sending a "feature" line
# with "analyze=1" in response to the "protover 2" command.
# Some cases also cover engines that report times in seconds
# instead of centiseconds.

proc detectWBEngine { {n 1} engineOutput } {
  
  global analysis
  
  # Check for a line containing "Amy version" to detect use of
  # the Amy 0.7 Winboard engine, which doesn't support the
  # "setboard" command, but does support the "analyze" command.
  if {[string match "*Amy version*" $engineOutput] } {
    logEngineNote $n {Seen "Amy"; assuming analyze command.}
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "Baron" to detect use of the
  # Baron 0.26, 0.26a, 0.27, or 0.28a Winboard engines.  These
  # engines display analysis time in whole seconds, rather than
  # in centiseconds, so I have added code to detect this.
  if {[string match "*Baron*" $engineOutput] } {
    logEngineNote $n {Seen "Baron"; assuming analyze, setboard, times in seconds.}
    set analysis(has_setboard$n) 1
    set analysis(has_analyze$n) 1
    set analysis(wholeSeconds$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "D U K E" to detect use of
  # the Duke 1.0 or 1.1 Winboard engines, which don't support the
  # "setboard" command, but do support the "analyze" command.
  if {[string match "*D U K E*" $engineOutput] } {
    logEngineNote $n {Seen "Duke"; assuming analyze command.}
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "ESCbook.bin" to detect use of
  # the Esc 1.09 Winboard engine, which doesn't support the
  # "setboard" command, but does support the "analyze" command.
  if {[string match "*ESCbook.bin*" $engineOutput] } {
    logEngineNote $n {Seen "ESC"; assuming analyze command.}
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "FORTRESS" to detect use of
  # the Fortress 1.62 Winboard engine, which doesn't support the
  # "setboard" command, but does support the "analyze" command.
  if {[string match "*FORTRESS*" $engineOutput] } {
    logEngineNote $n {Seen "Fortress"; assuming analyze command.}
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing only "Chess", to detect the use of
  # GNU Chess 4, which issues time in whole seconds rather than in
  # centiseconds.
  #if {! [string compare $engineOutput "Chess"]} {
  #  logEngineNote $n {Seen "GNU Chess 4"; assuming times in seconds.}
  #  set analysis(wholeSeconds$n) 1
  #  set analysis(wbEngineDetected$n) 1
  #  return
  #}
  
  # Check for a line containing "GNU Chess v5" to detect use of
  # the GNU Chess 5.02 or 5.03 Winboard engine, which don't support the
  # "analyze" command, but do support the "setboard" command.
  if {[string match "*GNU Chess v5*" $engineOutput] } {
    logEngineNote $n {Seen "GNU Chess 5"; assuming setboard command.}
    set analysis(has_setboard$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "Gromit3" to detect use of the
  # Gromit 3.00 or Gromit 3.8.2 Winboard engine, which don't
  # support the "setboard" command, but do support the
  # "analyze" command.
  if {[string match "*Gromit3*" $engineOutput]  ||  [string match "GROMIT" $engineOutput]} {
    logEngineNote $n {Seen "Gromit"; assuming analyze command.}
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "Jester" to detect use of the
  # Jester 0.82 Winboard engine.  This engine supports "analyze"
  # but does not support "setboard" or "protover".
  if {[string match "*Jester*" $engineOutput] } {
    logEngineNote $n {Seen "Jester"; assuming analyze command.}
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "Calzerano" to detect use of the
  # Leila 0.36 or Leila 0.41i Winboard engine, which don't
  # support the "setboard" command, but do support the
  # "analyze" command.
  if {[string match "*Calzerano*" $engineOutput] } {
    logEngineNote $n {Seen "Calzerano" (Leila); assuming analyze command.}
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "LordKing" to detect use of the
  # LordKing 3.0, 3.1, or 3.2 Winboard engines.  These engines
  # have "analyze", but do not support "setboard" or "protover".
  if {[string match "*LordKing*" $engineOutput] } {
    logEngineNote $n {Seen "LordKing"; assuming analyze command.}
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "NEJMET" to detect use of the
  # Nejmet 2.6.0 Winboard engine, which supports "setboard"
  # and "analyze", but not "protover".
  if {[string match "*NEJMET*" $engineOutput] } {
    logEngineNote $n {Seen "Nejmet"; assuming analyze and setboard commands.}
    set analysis(has_setboard$n) 1
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "Nejmet" to detect use of the
  # Nejmet 3.0.1 and 3.0.2 Winboard engines, which send
  # "feature analyse=1" instead of "feature analyze=1".
  if {[string match "*Nejmet*" $engineOutput] } {
    logEngineNote $n {Seen "Nejmet"; assuming analyze and setboard commands.}
    set analysis(has_setboard$n) 1
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "Pharaon" to detect use of the
  # Pharaon 2.50 or Pharaon 2.61 Winboard engines.  These
  # engines display analysis time in whole seconds, rather than
  # in centiseconds, so I have added code to detect this.
  # Performance of these engines has been somewhat uneven, with
  # occasional crashes of the engine, but more stable and
  # predictable with this code in place.
  if {[string match "*Pharaon*" $engineOutput] } {
    logEngineNote $n {Seen "Pharaon"; assuming analyze, setboard, times in seconds.}
    set analysis(has_setboard$n) 1
    set analysis(has_analyze$n) 1
    set analysis(wholeSeconds$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "Skaki" to detect use of the
  # Skaki 1.19 Winboard engine.  This engine has "analyze",
  # but does not support "setboard" or "protover".
  if {[string match "*Skaki*" $engineOutput] } {
    logEngineNote $n {Seen "Skaki"; assuming analyze command.}
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "EngineControl-TCB" to detect use of the
  # TCB 0045 Winboard engine.  This engine has "analyze",
  # but does not support "setboard" or "protover".
  if {[string match "*EngineControl-TCB*" $engineOutput] } {
    logEngineNote $n {Seen "TCB"; assuming analyze and setboard commands.}
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "ZChess" to detect use of the
  # ZChess 2.22 Winboard engine.  ZChess is the predecessor
  # of the Pharaon series of Winboard engines and, as such,
  # displays analysis time in whole seconds, rather than
  # in centiseconds.
  if {[string match "*ZChess*" $engineOutput] } {
    logEngineNote $n {Seen "ZChess"; assuming analyze, setboard, times in seconds.}
    set analysis(has_analyze$n) 1
    set analysis(wholeSeconds$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "King of Kings" to detect use
  # of the King of Kings 2.02 Winboard engine.  KofK uses the
  # "protover" command, but seems to confuse previous code on
  # Win98SE. Setting analysis(has_setboard$n) and
  # analysis(has_analyze$n) explicitly seems to help.
  if {[string match "*King of Kings*" $engineOutput] } {
    logEngineNote $n {Seen "King of Kings"; assuming analyze and setboard commands.}
    set analysis(has_setboard$n) 1
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "EXchess" to detect use of the
  # EXchess 4.02 or 4.03 Winboard engine, which supports "setboard"
  # and "analyze", but not "protover".
  if {[string match "*EXchess*" $engineOutput] } {
    logEngineNote $n {Seen "EXchess"; assuming analyze and setboard commands.}
    set analysis(has_setboard$n) 1
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  # Check for a line containing "WildCat version 2.61" to detect use of the
  # WildCat 2.61 Winboard engine, which supports "analyze"
  # but not "setboard" or "protover".
  if {[string match "*WildCat version 2.61*" $engineOutput] } {
    logEngineNote $n {Seen "WildCat 2.61"; assuming analyze and setboard commands.}
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
  
  # Check for a line containing "Phalanx" to detect use of the
  # Phalanx Winboard engine, which supports "analyze"
  # but not "setboard" or "protover".
  if {[string match "*Phalanx*" $engineOutput] } {
    logEngineNote $n {Seen "Phalanx"; assuming analyze command.}
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    set analysis(has_setboard$n) 1
    return
  }
  
  # Check for a line containing "Scorpio" , which supports "analyze"
  # and "setboard".
  if {[string match -nocase "*Scorpio*" $engineOutput] } {
    logEngineNote $n {Seen "Scorpio"; assuming analyze command.}
    set analysis(has_analyze$n) 1
    set analysis(wbEngineDetected$n) 1
    return
  }
  
}

###
### End of file: wbdetect.tcl
###
