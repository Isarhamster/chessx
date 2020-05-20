# Scid (Shane's Chess Information Database)
#
# Copyright (C) 1999-2004 Shane Hudson
# Copyright (C) 2006-2009 Pascal Georges
# Copyright (C) 2008-2011 Alexander Wagner
# Copyright (C) 2013-2015 Fulvio Benini
#
# Scid is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation.


####################################################
# Set default values

proc InitDefaultToolbar {} {
  foreach {tbicon status}  {
    newdb 0 open 0 save 0 closedb 0
    finder 0 bkm 0 gprev 0 gnext 0
    newgame 0 copy 0 paste 0
    boardsearch 0 headersearch 0 materialsearch 0
    switcher 0 glist 0 pgn 0 tmt 0 maint 0 eco 0 tree 0 crosstab 0 engine 0
  } {
    set ::toolbar_state($tbicon) $status
  }
}

proc InitWinsDefaultGeometry {} {
  global winWidth winHeight

  # Default window size:
  set winWidth(.) 1024
  set winHeight(.) 570

  # Default help window size:
  set winWidth(.helpWin)  50
  set winHeight(.helpWin) 32

  # Default stats window size:
  set winWidth(.statsWin) 60
  set winHeight(.statsWin) 13

  # Default spellcheck results window size:
  set winWidth(.spellcheckWin)  55
  set winHeight(.spellcheckWin) 25

  # Default Correspondence Chess window size:
  set winWidth(.ccWindow) 10
  set winHeight(.ccWindow) 20

  # List of saved layouts : 3 slots available
  set ::docking::layout_list(1) {}
  set ::docking::layout_list(2) {{MainWindowGeometry 540x650+0+0} {{.pw vertical {}} {TNotebook .nb .fdockmain}}}
  set ::docking::layout_list(3) {{MainWindowGeometry 1024x532+0+0} {{.pw vertical {}} {TPanedwindow {{.pw.pw0 horizontal 413} {TNotebook .nb .fdockmain} {TPanedwindow {{.pw.pw0.pw1 vertical 270} {TNotebook .tb2 {.fdockanalysisWin1 .fdockpgnWin}} {TNotebook .tb3 .fdockglistWin1}}}}}}}
  set ::docking::layout_list(auto) {{MainWindowGeometry 1024x532+0+0} {{.pw vertical {}} {TPanedwindow {{.pw.pw0 horizontal 413} {TNotebook .nb .fdockmain} {TPanedwindow {{.pw.pw0.pw1 vertical 270} {TNotebook .tb2 {.fdockanalysisWin1 .fdockpgnWin}} {TNotebook .tb3 .fdockglistWin1}}}}}}}
}

proc InitDefaultStats {} {
  # Default stats window lines:
  array set ::windows::stats::display {
    r2600 1
    r2500 1
    r2400 1
    r2300 1
    r2200 0
    r2100 0
    r2000 0
    y1900 0
    y1950 0
    y1960 0
    y1970 0
    y1980 0
    y1990 0
    y1995 0
    y2000 1
    y2002 1
    y2004 1
    y2006 1
    y2007 1
    y2008 1
  }

  # Enable stats for subsequent years
  for { set year [clock format [clock seconds] -format {%Y}] } \
    { $year>2008 && ![info exists ::windows::stats::display([subst {y$year}])] } \
    { incr year -1 } {
    set ::windows::stats::display([subst {y$year}]) 1
  }
}

proc InitDefaultFonts {} {
  global fontOptions
  if {$::windowsOS} {
    set fontOptions(Regular) [list Tahoma    10 normal roman]
    set fontOptions(Menu)    [list system     9 normal roman]
    set fontOptions(Small)   [list Tahoma     8 normal roman]
    set fontOptions(Tiny)    [list Arial      7 normal roman]
    set fontOptions(Fixed)   [list courier    9 normal roman]
  } elseif {$::macOS} {
    set fontOptions(Regular) [list system    11 normal roman]
    set fontOptions(Menu)    [list menu      14 normal roman]
    set fontOptions(Small)   [list system    10 normal roman]
    set fontOptions(Tiny)    [list system     9 normal roman]
    set fontOptions(Fixed)   [list Monaco    10 normal roman]
  } else {
    set fontOptions(Regular) [list {DejaVu Sans}       10 normal roman]
    set fontOptions(Menu)    [list {DejaVu Sans}       10 normal roman]
    set fontOptions(Small)   [list {DejaVu Sans}        9 normal roman]
    set fontOptions(Tiny)    [list {DejaVu Sans}        8 normal roman]
    set fontOptions(Fixed)   [list {DejaVu Sans Mono}  10 normal roman]
  }
}

proc InitDefaultGListLayouts {} {
  set ::glist_Layouts {RatingDate DateEvent Full}
  set ::glist_ColOrder(RatingDate) {{7} {1} {2} {3} {4} {5} {6} {23} {22} {8} {9} {10} {11} {12} {13} {14} {15} {16} {0}}
  set ::glist_ColWidth(RatingDate) {{50} {50} {50} {120} {40} {120} {40} {80} {200} {30}  {200} {30} {20} {20} {20} {20} {35} {50} {30} {100} {40} {40} {50} {140}}
  set ::glist_ColAnchor(RatingDate) {{e} {c} {c} {w} {c} {w} {c} {w} {w} {e}  {w} {c} {c} {c} {c} {c} {c} {c} {c} {c} {c} {c} {c} {w}}
  set ::glist_Sort(RatingDate) { {22} {-} {7} {-} }
  set ::glist_FindBar(RatingDate) 0
  set ::glist_ColOrder(DateEvent) {7 3 4 5 6 1 23 8 9 10 0}
  set ::glist_ColWidth(DateEvent) {50 50 50 120 40 120 40 80 145 30 112 30 20 20 20 20 35 50 30 100 40 40 50 153}
  set ::glist_ColAnchor(DateEvent) {{e} {c} {c} {w} {c} {w} {c} {w} {w} {e}  {w} {c} {c} {c} {c} {c} {c} {c} {c} {c} {c} {c} {c} {w}}
  set ::glist_Sort(DateEvent) {7 - 8 - 9 -}
  set ::glist_FindBar(DateEvent) 0
  set ::glist_ColOrder(Full) {20 7 3 4 5 6 1 2 23 8 19 10 9 16 17 22 21 18 11 12 13 14 15 0}
  set ::glist_ColWidth(Full) {50 50 39 120 40 120 40 70 200 30 200 30 20 20 20 20 35 119 30 78 40 40 50 155}
  set ::glist_ColAnchor(Full) {{e} {c} {c} {w} {c} {w} {c} {w} {w} {e}  {w} {c} {c} {c} {c} {c} {c} {c} {c} {c} {c} {c} {c} {w}}
  set ::glist_Sort(Full) {0 +}
  set ::glist_FindBar(Full) 0
}

proc InitDefaultAnnotate {} {
  set ::isBatchOpening 0
  set ::isBatchOpeningMoves 12
  set ::isBatch 0
  set ::markTacticalExercises 0
  set ::isAnnotateVar 0
  set ::isShortAnnotation 0
  set ::addScoreToShortAnnotations 0
  set ::addAnnotatorTag 0
  set ::annotateMoves all
  set ::annotateBlunders blundersonly
  set ::scoreAllMoves 0
  # Blunder Threshold
  set ::blunderThreshold 1.0
}

InitDefaultFonts
InitWinsDefaultGeometry
InitDefaultToolbar
InitDefaultStats
InitDefaultGListLayouts
InitDefaultAnnotate

#Default textures for lite and dark squares
set boardfile_dark "LightWood3-d"
set boardfile_lite "LightWood3-l"

# boardSize: Default board size.
set boardSize 40

# boardStyle: Default board piece set.
set boardStyle Merida1
if { $macOS } { set boardStyle Merida }

# Colors: dark and lite are square colors
#     whitecolor/blackcolor are piece colors
#     highcolor is the color when something is selected.
#     bestcolor is used to indicate a suggested move square.
# set dark        "\#70a070"
# set lite        "\#e0d070"
set lite "\#f3f3f3"
set dark "\#7389b6"
set whitecolor  "\#ffffff"
set blackcolor  "\#000000"
set whiteborder "\#000000"
set blackborder "\#ffffff"
set highcolor   "\#b0d0e0"
set bestcolor   "\#bebebe"
set buttoncolor "\#b0c0d0"
set borderwidth 0

# Square markers color and type
set ::markColor green
set ::markType full

# boardCoords: 1 to show board Coordinates, 0 to hide them.
set boardCoords 0

# autoResizeBoard:
# resize the board to fit the container
set autoResizeBoard 1

# showGameInfo:
# The game info panel below the main board
set showGameInfo 0

# language for help pages and messages:
set language E

# Default theme
set ::lookTheme "default"
set ::ThemePackageFile ""

# Auto-save options when exiting:
set optionsAutoSave 1

#  Numeric locale: first char is decimal, second is thousands.
#  Example: ".," for "1,234.5" format; ",." for "1.234,5" format.
set locale(numeric) ".,"

# Ask for piece translations (first letter)
set translatePieces 1

# Highlight the last move played
set arrowLastMove 0
set highlightLastMove 1
set highlightLastMoveWidth 2
set highlightLastMoveColor "grey"
set highlightLastMovePattern {} ; # this option is not saved

# Gloss Of Danger
set glossOfDanger 0

# Show suggested moves: on by default
set suggestMoves 1

# Show variations popup window
set showVarPopup 0
set showVarArrows 1

# Keyboard Move entry options:
set moveEntry(On) 1
set moveEntry(AutoExpand) 0
set moveEntry(Coord) 1

# windowsDock:
# if true, most of toplevel windows are dockable and embedded in a main window
# windows can be moves among tabs (drag and drop) or undocked (right-clicking on tab)
set windowsDock 1


set ::tactics::analysisTime 3

set ::tacgame::threshold 0.9
set ::tacgame::blunderwarning false
set ::tacgame::blunderwarningvalue 0.0
set ::tacgame::levelMin 1200
set ::tacgame::levelMax 2200
set ::tacgame::levelFixed 1500
set ::tacgame::randomLevel 0
set ::tacgame::isLimitedAnalysisTime 1
set ::tacgame::showblunder 1
set ::tacgame::showblundervalue 1
set ::tacgame::showblunderfound 1
set ::tacgame::showmovevalue 1
set ::tacgame::showevaluation 1
set ::tacgame::isLimitedAnalysisTime 1
set ::tacgame::analysisTime 10
set ::tacgame::openingType new
set ::tacgame::chosenOpening 0

# Analysis command: to start chess analysis engine.
set analysisCommand ""
if {$windowsOS} {
  set analysisChoices {wcrafty.exe}
} else {
  set analysisChoices {crafty}
}

# Default Tree sort method:
set tree(order) frequency

### Tree/mask options:
set ::tree::mask::recentMask {}
    
# Defaults for the PGN window:
# if ::pgn::showColor is 1, the PGN text will be colorized.
set ::pgn::showColor 1
set ::pgn::indentVars 1
set ::pgn::indentComments 1
set ::pgn::symbolicNags 1
set ::pgn::moveNumberSpaces 0
set ::pgn::shortHeader 1
set ::pgn::boldMainLine 1
set ::pgn::columnFormat 0
set ::pgn::stripMarks 0
set ::pgn::showPhoto 1
set pgnColor(Header) "\#00008b"
set pgnColor(Main) "\#000000"
set pgnColor(Var) "\#0000ee"
set pgnColor(Nag) "\#ee0000"
set pgnColor(Comment) "\#008b00"
set pgnColor(Current) lightSteelBlue
set pgnColor(Background) "\#ffffff"

# Defaults for FICS
set ::fics::use_timeseal 0
set ::fics::timeseal_exec "timeseal"
set ::fics::port_fics 5000
set ::fics::port_timeseal 5001
set ::fics::login ""
set ::fics::password ""
set ::fics::usedefaultvars 1
set ::fics::findopponent(initTime) 15
set ::fics::findopponent(incTime) 20
set ::fics::findopponent(rated) "rated"
set ::fics::findopponent(color) ""
set ::fics::findopponent(limitrating) 1
set ::fics::findopponent(rating1) 1500
set ::fics::findopponent(rating2) 3000
set ::fics::findopponent(manual) "auto"
set ::fics::findopponent(formula) ""
set ::fics::consolebg     black
set ::fics::consolefg     LimeGreen
set ::fics::consoleheight 10
set ::fics::consolewidth  40
set ::fics::colseeking     coral
set ::fics::colgame        grey70
set ::fics::colgameresult  SlateBlue1
set ::fics::colficspercent khaki1
set ::fics::colficshelpnext blue
set ::fics::server_ip "0.0.0.0"
set ::fics::premoveEnabled 1
set ::fics::playing 0

# default resolvers for player info
set ::pinfo::wikipAPI      "http://de.wikipedia.org/w/api.php?action=query&format=xml"
# Appers PND resolver
set ::pinfo::wikipurl      "http://toolserver.org/~apper/pd/person/pnd-redirect"
# SeeAlso resolver for PND -> WikiPedia
set ::pinfo::SeeAlsoPND2WP "http://ws.gbv.de/seealso/pnd2wikipedia/?format=seealso&id="
# Deutsche NationalBibliothek
set ::pinfo::dnburl        "http://d-nb.info/gnd"
# all other ID resolvers come from [scidConfigFile resolvers]

# Defaults for Novag Citrine
set ::novag::referee "OFF"

# Defaults for serious game training
set ::sergame::isOpening 0
set ::sergame::chosenOpening 0
set ::sergame::chosenEngine 0
set ::sergame::useBook 1
set ::sergame::bookToUse ""
set ::sergame::startFromCurrent 0
set ::sergame::coachIsWatching 0
set ::sergame::timeMode "timebonus"
set ::sergame::depth 3
set ::sergame::movetime 0
set ::sergame::nodes 10000
set ::sergame::ponder 0
set ::uci::uciInfo(wtime3) [expr 5 * 60 * 1000 ]
set ::uci::uciInfo(winc3) [expr 10 * 1000 ]
set ::uci::uciInfo(btime3) [expr 5 * 60 * 1000 ]
set ::uci::uciInfo(binc3) [expr 10 * 1000 ]

# Defaults for initial directories:
set initialDir(base) "."
set initialDir(pgn) "."
set initialDir(book) "."
set initialDir(html) "."
set initialDir(tex)  "."
set initialDir(stm)  "."
set initialDir(report) "."
set initialDir(tablebase1) ""
set initialDir(tablebase2) ""
set initialDir(tablebase3) ""
set initialDir(tablebase4) ""

# Default PGN display options:
set pgnStyle(Tags) 1
set pgnStyle(Comments) 1
set pgnStyle(Vars) 1

# Autoplay and animation delays in milliseconds:
set autoplayDelay 5000
set animateDelay 200
set autoplayMode 0

# Geometry of windows:
array set geometry {}

# startup:
#   Stores which windows should be opened on startup.
set startup(tip) 1


# Game information area options:
set gameInfo(photos) 1
set gameInfo(hideNextMove) 0
set gameInfo(showMaterial) 0
set gameInfo(showFEN) 0
set gameInfo(showMarks) 1
set gameInfo(wrap) 0
set gameInfo(fullComment) 0
set gameInfo(showTB) 0
if {[sc_info tb]} { set gameInfo(showTB) 2 }

# Twin deletion options:

array set twinSettings {
  players No
  colors  No
  event   No
  site    Yes
  round   Yes
  year    Yes
  month   Yes
  day     No
  result  No
  eco     No
  moves   Yes
  skipshort  Yes
  setfilter  Yes
  undelete   Yes
  comments   Yes
  variations Yes
  usefilter  No
  delete     Shorter
}
array set twinSettingsDefaults [array get twinSettings]

# Opening report options:
array set optable {
  Stats 1
  Oldest 5
  Newest 5
  Popular 1
  MostFrequent 6
  MostFrequentWhite 1
  MostFrequentBlack 1
  AvgPerf 1
  HighRating 8
  Results 1
  Shortest 5
  ShortestWhite 1
  ShortestBlack 1
  MoveOrders 8
  MovesFrom 1
  Themes 1
  Endgames 1
  MaxGames 500
  ExtraMoves 1
}
array set optableDefaults [array get optable]

# Player report options
array set preport {
  Stats 1
  Oldest 5
  Newest 5
  MostFrequentOpponents 6
  AvgPerf 1
  HighRating 8
  Results 1
  MostFrequentEcoCodes 6
  Themes 1
  Endgames 1
  MaxGames 500
  ExtraMoves 1
}
array set preportDefaults [array get preport]

# Analysis options (Informant values)
# The different threshold values for !? ?? += etc
array set informant {}
set informant("!?") 0.5
set informant("?") 1.5
set informant("??") 3.0
set informant("?!") 0.5
set informant("+=") 0.5
set informant("+/-") 1.5
set informant("+-") 3.0
set informant("++-") 5.5

# Export file options:
set exportFlags(comments) 1
set exportFlags(indentc) 0
set exportFlags(vars) 1
set exportFlags(indentv) 0
set exportFlags(column) 0
set exportFlags(append) 0
set exportFlags(symbols) 0
set exportFlags(htmldiag) 0
set exportFlags(stripMarks) 0
set exportFlags(convertNullMoves) 0
set default_exportStartFile(PGN) {}
set default_exportEndFile(PGN) {}

set default_exportStartFile(LaTeX) {\documentclass[10pt,twocolumn]{article}
  % This is a LaTeX file generated by Scid.
  % You must have the "chess12" package installed to typeset this file.
  
  \usepackage{times}
  \usepackage{a4wide}
  \usepackage{chess}
  \usepackage[T1]{fontenc}
  
  \setlength{\columnsep}{7mm}
  \setlength{\parindent}{0pt}
  
  % Macros for variations and diagrams:
  \newenvironment{variation}{\begin{quote}}{\end{quote}}
  \newenvironment{diagram}{\begin{nochess}}{$$\showboard$$\end{nochess}}
  
  \begin{document}
}
set default_exportEndFile(LaTeX) {\end{document}
}


set default_exportStartFile(HTML) {<html>
  <head>
    <title>Scid export</title>
    <style>
      @media print {
        table { page-break-inside: avoid; }
      }
    </style>
  </head>
  <body bgcolor="#ffffff">

}
set default_exportEndFile(HTML) {</body>
  </html>
}

foreach type {PGN HTML LaTeX} {
  set exportStartFile($type) $default_exportStartFile($type)
  set exportEndFile($type) $default_exportEndFile($type)
}

# autoRaise: defines whether the "raise" command should be used to raise
# certain windows (like progress bars) when they become obscured.
# Some Unix window managers (e.g. some versions of Enlightenment and sawfish,
# so I have heard) have a bug where the Tcl/Tk "raise" command times out
# and takes a few seconds. Setting autoRaise to 0 will help avoid this.

set autoRaise 1

proc raiseWin {w} {
  global autoRaise
  if {$autoRaise} { raise $w }
  return
}

# Email configuration:
set email(logfile) [file join $scidLogDir "scidmail.log"]
set email(oldlogfile) [file join $scidUserDir "scidmail.log"]
set email(smtp) 1
set email(smproc) "/usr/lib/sendmail"
set email(server) localhost
set email(from) ""
set email(bcc) ""
# Rename old email log file if necessary:
if {[file readable $email(oldlogfile)]  &&  ![file readable $email(logfile)]} {
  catch {file rename $email(oldlogfile) $email(logfile)}
}

### Audio move announcement options:
set ::utils::sound::soundFolder [file nativename [file join $::scidShareDir sounds]]
set ::utils::sound::announceNew 0
set ::utils::sound::announceForward 0
set ::utils::sound::announceBack 0

# Spell-checking file: default is "spelling.ssp".
set spellCheckFile [file join $scidShareDir "spelling.ssp"]

# book configuration
set ::book::lastBook "" ; # book name without extension (.bin)

# Engines list file: -- OLD NAMES, NO LONGER USED
#set engines(file) [file join $scidUserDir "engines.lis"]
#set engines(backup) [file join $scidUserDir "engines.bak"]

# Engines data:
set engines(list) {}
set engines(sort) Time
set engineCoach1 {}
set engineCoach2 {}



# scidConfigFile:
#   Returns the full path and name of a Scid configuration file,
#   given its configuration type.
#
proc scidConfigFile {type} {
  global scidConfigDir

  foreach {cfgtype fname} {
    options "options.dat"
    engines "engines.dat"
    engines.bak "engines.dat"
    recentfiles "recent.dat"
    history "history.dat"
    bookmarks "bookmarks.dat"
    reports "reports.dat"
    optrainer "optrainer.dat"
    resolvers "resolvers.dat"
    xfccstate "xfccstate.dat"
    correspondence "correspondence.dat"
    ExtHardware "hardware.dat"
    treecache "treecache.dat"
  } {
    if { $type == $cfgtype } {
      return [file nativename [file join $scidConfigDir $fname]]
    }
  }

  return -code error "No such config file type: $type"
}

set optionsFile [scidConfigFile options]

################################################################################
#  Load options file. All default values should be set before this point or new saved values will be overwritten by default ones
################################################################################
if {[file exists $optionsFile] && [catch {source $optionsFile}]} {
  tk_messageBox -message $::errorInfo
}

# Now, if the options file was written by Scid 3.5 or older, it has a lot of
# yucky variable names in the global namespace. So convert them to the new
# namespace variables:
#
proc ConvertOldOptionVariables {} {
  set oldNewNames {
    doColorPgn ::pgn::showColor
    pgnIndentVars ::pgn::indentVars
    pgnIndentComments ::pgn::indentComments
    pgnShortHeader ::pgn::shortHeader
    pgnMoveFont ::pgn::boldMainLine
    pgnMoveNumSpace ::pgn::moveNumberSpaces
    pgnStripMarks ::pgn::stripMarks
    pgnSymbolicNags ::pgn::symbolicNags
    pgnColumn ::pgn::columnFormat
  }
  
  foreach {old new} $oldNewNames {
    if {[info exists ::$old]} {
      set $new [set ::$old]
    }
  }
}
ConvertOldOptionVariables

proc options.write {} {
 uplevel #0 {
  set optionF ""
  if {[catch {open [scidConfigFile options] w} optionF]} {
    tk_messageBox -title "Scid: Unable to write file" -type ok -icon warning \
        -message "Unable to write options file: [scidConfigFile options]\n$optionF"
  } else {
    puts $optionF "# Scid options file"
    puts $optionF "# Version: $scidVersion"
    puts $optionF "# This file contains commands in the Tcl language format."
    puts $optionF "# If you edit this file, you must preserve valid its Tcl"
    puts $optionF "# format or it will not set your Scid options properly."
    puts $optionF ""
    foreach i {boardSize boardStyle language ::pgn::showColor \
          ::pgn::indentVars ::pgn::indentComments ::pgn::showPhoto \
          ::pgn::shortHeader ::pgn::boldMainLine ::pgn::stripMarks \
          ::pgn::symbolicNags ::pgn::moveNumberSpaces ::pgn::columnFormat \
          tree(order) optionsAutoSave ::tree::mask::recentMask \
          ecoFile suggestMoves showVarPopup showVarArrows \
          blunderThreshold autoplayDelay animateDelay boardCoords \
          moveEntry(AutoExpand) moveEntry(Coord) \
          translatePieces arrowLastMove highlightLastMove highlightLastMoveWidth highlightLastMoveColor \
          glossOfDanger locale(numeric) \
          spellCheckFile autoRaise windowsDock showGameInfo \
          exportFlags(comments) exportFlags(vars) \
          exportFlags(indentc) exportFlags(indentv) \
          exportFlags(column) exportFlags(symbols) \
          exportFlags(htmldiag) exportFlags(convertNullMoves) \
          email(smtp) email(smproc) email(server) \
          email(from) email(bcc) \
          gameInfo(photos) gameInfo(hideNextMove) gameInfo(wrap) \
          gameInfo(fullComment) gameInfo(showMarks) \
          gameInfo(showMaterial) gameInfo(showFEN) gameInfo(showTB) \
          engineCoach1 engineCoach2 scidBooksDir scidBasesDir ::book::lastBook \
          ::utils::sound::soundFolder ::utils::sound::announceNew \
          ::utils::sound::announceForward ::utils::sound::announceBack \
          ::tacgame::threshold ::tacgame::blunderwarning ::tacgame::blunderwarningvalue \
          ::tacgame::levelMin  ::tacgame::levelMax  ::tacgame::levelFixed ::tacgame::randomLevel \
          ::tacgame::isLimitedAnalysisTime ::tacgame::showblunder ::tacgame::showblundervalue \
          ::tacgame::showblunderfound ::tacgame::showmovevalue ::tacgame::showevaluation \
          ::tacgame::isLimitedAnalysisTime ::tacgame::analysisTime ::tacgame::openingType ::tacgame::chosenOpening \
          ::sergame::chosenOpening ::sergame::chosenEngine ::sergame::useBook ::sergame::bookToUse \
          ::sergame::startFromCurrent ::sergame::coachIsWatching ::sergame::timeMode \
          ::sergame::depth ::sergame::movetime ::sergame::nodes ::sergame::ponder ::sergame::isOpening \
          ::uci::uciInfo(wtime3) ::uci::uciInfo(winc3) ::uci::uciInfo(btime3) ::uci::uciInfo(binc3) \
          boardfile_lite boardfile_dark \
          FilterMaxMoves FilterMinMoves FilterStepMoves FilterMaxElo FilterMinElo FilterStepElo \
          FilterMaxYear FilterMinYear FilterStepYear FilterGuessELO lookTheme ThemePackageFile autoResizeBoard \
          isBatchOpening isBatchOpeningMoves isBatch \
          markTacticalExercises scoreAllMoves \
          isAnnotateVar isShortAnnotation addScoreToShortAnnotations annotateBlunders\
          addAnnotatorTag annotateMoves } {
      puts $optionF "set $i [list [set $i]]"
    }
    
    foreach i [lsort [array names menuColor]] {
      puts $optionF "set menuColor($i) [list $menuColor($i)]"
    }
    foreach i [lsort [array names menuBarColor]] {
      puts $optionF "set menuBarColor($i) [list $menuBarColor($i)]"
    }
    puts $optionF ""
    foreach i [lsort [array names winWidth]] {
      puts $optionF "set winWidth($i)  [expr $winWidth($i)]"
      puts $optionF "set winHeight($i) [expr $winHeight($i)]"
    }
    puts $optionF ""
    foreach i [lsort [array names winX]] {
      puts $optionF "set winX($i)  [expr $winX($i)]"
      puts $optionF "set winY($i)  [expr $winY($i)]"
    }
    puts $optionF ""
    puts $optionF "set analysisCommand [list $analysisCommand]"
    puts $optionF "set analysisChoices [list $analysisChoices]"
    puts $optionF ""
    foreach i {lite dark whitecolor blackcolor highcolor bestcolor \
          whiteborder blackborder borderwidth \
          pgnColor(Header) pgnColor(Main) pgnColor(Var) \
          pgnColor(Nag) pgnColor(Comment) pgnColor(Background) \
          pgnColor(Current) } {
      puts $optionF "set $i [list [set $i]]"
    }
    puts $optionF ""
    foreach i [lsort [array names optable]] {
      puts $optionF "set optable($i) [list $optable($i)]"
    }
    foreach i [lsort [array names ::windows::stats::display]] {
      puts $optionF "set ::windows::stats::display($i) [list $::windows::stats::display($i)]"
    }
    foreach i [lsort [array names startup]] {
      puts $optionF "set startup($i) [list $startup($i)]"
    }
    foreach i [lsort [array names toolbar_state]] {
      puts $optionF "set toolbar_state($i) [list $toolbar_state($i)]"
    }
    foreach i [lsort [array names twinSettings]] {
      puts $optionF "set twinSettings($i) [list $twinSettings($i)]"
    }
    puts $optionF ""
    foreach i {Regular Menu Small Tiny Fixed} {
      puts $optionF "set fontOptions($i) [list $fontOptions($i)]"
    }
    puts $optionF ""
    foreach type [array names initialDir] {
      puts $optionF "set initialDir($type) [list $initialDir($type)]"
    }
    puts $optionF ""
    foreach type {PGN HTML LaTeX} {
      puts $optionF "set exportStartFile($type) [list $exportStartFile($type)]"
      puts $optionF "set exportEndFile($type) [list $exportEndFile($type)]"
    }
    puts $optionF ""
    foreach i [lsort [array names informant]] {
      puts $optionF "set informant($i) [list $informant($i)]"
    }
    puts $optionF ""
    
    # save FICS config
    foreach i { use_timeseal timeseal_exec port_fics port_timeseal login password usedefaultvars premoveEnabled \
          consolebg consolefg consoleheight consolewidth colseeking colgame colgameresult colficspercent server_ip } {
      puts $optionF "set ::fics::$i [list [set ::fics::$i]]"
    }
    foreach i [lsort [array names ::fics::profileVars]] {
      puts $optionF "set ::fics::profileVars($i) [list $::fics::profileVars($i)]"
    }

    # save pinfo config
    foreach i { wikipurl dnburl SeeAlsoPND2WP wikipAPI} {
      puts $optionF "set ::pinfo::$i [list [set ::pinfo::$i]]"
    }
    
    # save NOVAG config
    foreach i { referee } {
      puts $optionF "set ::novag::$i [list [set ::novag::$i]]"
    }
    
    # Save layouts
    ::docking::layout_save "auto"
    foreach slot {1 2 3 auto} {
      puts $optionF "set ::docking::layout_list($slot) [list $::docking::layout_list($slot)]"
    }
    foreach wnd [array names ::winGeometry] {
      if {[winfo exists $wnd]} {
        ::win::saveWinGeometry $wnd
      }
      if {$::winGeometry($wnd) ne ""} {
        puts $optionF "set ::winGeometry($wnd) $::winGeometry($wnd)"
      }
    }

    
    # Save var that was added with options.save()
    if {[info exists ::autosave_opt]} {
      puts $optionF ""
      puts $optionF "set ::autosave_opt [list $::autosave_opt]"
      foreach {var} $::autosave_opt {
        if {[array exists $var]} {
            puts $optionF "array set $var [list [array get $var]]"
        } else {
            puts $optionF "set $var [list [set $var]]"
        }
      }
    }
    
    close $optionF
    set ::statusBar "Options were saved to: [scidConfigFile options]"
  }
 }
}

proc options.autoSaveHack {} {
  catch {
    set optionF [open [scidConfigFile options] "a"]
	puts $optionF "set ::optionsAutoSave $::optionsAutoSave"
    close $optionF
  }
}

# Sets an option whose value is restored when the program starts
proc options.persistent {varname default_value} {
  if {![info exists $varname]} {
    set $varname $default_value
  }
  options.save $varname
}

proc options.save {varname} {
  if {![info exists ::autosave_opt] || [lsearch -exact $::autosave_opt $varname] == -1} {
    lappend ::autosave_opt $varname
  }
}

###
### End of file: options.tcl
