# preport.tcl: Player reports.
# Part of Scid. Copyright 2003 Shane Hudson

namespace eval ::preport {}
array set ::preport::_data {}

set preport(MaxGames) 400

set ::preport::_player ""
set ::preport::_color white
set ::preport::_pos start
set ::preport::_clipbase 0

# preportDlg
#   Present a dialog allowing the user to select the
#   player and color for which to generate a player report.
#
proc ::preport::preportDlg {args} {
  if {[sc_base numGames $::curr_db] == 0} {
    tk_messageBox -title "Scid" -type ok -icon warning -message "No games in current base"
    return
  }
  # Set default player and color if parameters are provided
  if {[llength $args] >= 1} {
    set ::preport::_player [lindex $args 0]
    if {$::preport::_player == [sc_game info white]} {
      set ::preport::_color white
    } elseif {$::preport::_player == [sc_game info black]} {
      set ::preport::_color black
    }
  }
  if {[llength $args] >= 2} {
    set ::preport::_color [lindex $args 1]
  }
  
  set w .preportDlg
  if {[winfo exists $w]} { return }
  win::createDialog $w
  wm title $w "Scid: [tr ToolsPlayerReport]"
  wm resizable $w 0 0
  pack [ttk::frame $w.g] -side top -fill x -expand yes
  ttk::label $w.g.where -text $::tr(Player)
  grid $w.g.where -row 0 -column 0 -sticky w
  ttk::combobox $w.g.player -width 40 -textvariable ::preport::_player
  ::utils::history::SetCombobox ::preport::_player $w.g.player
  grid $w.g.player -row 0 -column 1 -sticky we
  ttk::label $w.g.has -text $::tr(ColorMarker)
  grid $w.g.has -row 1 -column 0 -sticky w
  ttk::frame $w.g.color
  ttk::radiobutton $w.g.color.white -text $::tr(White) \
      -variable ::preport::_color -value white
  ttk::frame $w.g.color.gap -width 5
  ttk::radiobutton $w.g.color.black -text $::tr(Black) \
      -variable ::preport::_color -value black
  pack $w.g.color.white $w.g.color.gap $w.g.color.black -side left
  grid $w.g.color -row 1 -column 1 -sticky w
  ttk::label $w.g.pos -text $::tr(StartPos)
  grid $w.g.pos -row 2 -column 0
  ttk::frame $w.g.pselect
  ttk::radiobutton $w.g.pselect.start -text "$::tr(Defaults) $::tr(StartPos)" \
      -variable ::preport::_pos -value start
  ttk::radiobutton $w.g.pselect.current -text $::tr(CurrentBoard) \
      -variable ::preport::_pos -value current
  pack $w.g.pselect.start $w.g.pselect.current -side left
  grid $w.g.pselect -row 2 -column 1 -sticky w
  ttk::checkbutton $w.g.clipbase -text $::tr(PReportClipbase) \
      -variable ::preport::_clipbase
  grid $w.g.clipbase -row 3 -column 1 -sticky w
  addHorizontalRule $w
  pack [ttk::frame $w.b] -side bottom -fill x
  pack [ttk::frame $w.b2] -side bottom -fill x
  set whiteName [sc_game info white]
  set blackName [sc_game info black]
  dialogbutton $w.b2.white -text "$::tr(White) ($whiteName)" -command {
    set ::preport::_player [sc_game info white]
    set ::preport::_color white
  }
  dialogbutton $w.b2.black -text "$::tr(Black) ($blackName)" -command {
    set ::preport::_player [sc_game info black]
    set ::preport::_color black
  }
  if {$whiteName == ""  ||  $whiteName == "?"} {
    $w.b2.white configure -state disabled
  }
  if {$blackName == ""  ||  $blackName == "?"} {
    $w.b2.black configure -state disabled
  }
  
  dialogbutton $w.b.help -text $::tr(Help) \
      -command {helpWindow Reports Player}
  dialogbutton $w.b.ok -text OK \
      -command "catch {grab release $w}; destroy $w; ::preport::makeReportWin"
  dialogbutton $w.b.cancel -text $::tr(Cancel) \
      -command "catch {grab release $w}; destroy $w"
  # foreach button {help ok cancel} {
  # $w.b.$button configure -font font_Small
  # }
  if {$whiteName != ""  &&  $whiteName != "?"} {
    packbuttons left $w.b2.white
  }
  if {$blackName != ""  &&  $blackName != "?"} {
    packbuttons right $w.b2.black
  }
  packbuttons left $w.b.help -pady "15 0"
  packdlgbuttons $w.b.cancel $w.b.ok
  if {[sc_base current] == $::clipbase_db} {
    $w.g.clipbase configure -state disabled
  }
  bind $w <Return> [list $w.b.ok invoke]
  bind $w <Escape> [list $w.b.cancel invoke]
  grab $w
  focus $w.g.player
}

proc ::preport::ConfigMenus {{lang ""}} {
  if {! [winfo exists .preportWin]} { return }
  if {$lang == ""} { set lang $::language }
  set m .preportWin.menu
  foreach idx {0 1} tag {File Help} {
    configMenuText $m $idx Oprep$tag $lang
  }
  foreach idx {0 1 2 4 6} tag {Text Html LaTeX Options Close} {
    configMenuText $m.file $idx OprepFile$tag $lang
  }
  foreach idx {0 1} tag {Report Index} {
    configMenuText $m.helpmenu $idx OprepHelp$tag $lang
  }
  
}

proc ::preport::makeReportWin {args} {
  set showProgress 1
  set args [linsert $args 0 "args"]
  if {[lsearch -exact $args "-noprogress"] >= 0} { set showProgress 0 }
  if {$showProgress} {
    set w .progress
    toplevel $w
    wm withdraw $w
    wm title $w "Scid: [tr ToolsPlayerReport]"
    bind $w <Visibility> "raiseWin $w"
    
    pack [ttk::frame $w.b] -side bottom -fill x
    set ::preport::_interrupt 0
    ttk::button $w.b.cancel -text $::tr(Cancel) -command {
      set ::preport::_interrupt 1
      progressBarCancel
    }
    pack $w.b.cancel -side right -pady 5 -padx 2
    
    foreach i {1 2 3} name {"Searching database for report games"
      "Searching current board" "Generating report information"} {
      ttk::label $w.text$i -text "$i. $name"
      pack $w.text$i -side top
      canvas $w.c$i -width 400 -height 20 -bg white -relief solid -border 1
      $w.c$i create rectangle 0 0 0 0 -fill blue -outline blue -tags bar
      $w.c$i create text 395 10 -anchor e -font font_Regular -tags time \
          -fill black -text "0:00 / 0:00"
      pack $w.c$i -side top -pady 10
    }
    wm resizable $w 0 0
    # Set up geometry for middle of screen:
    set x [winfo screenwidth $w]; set x [expr $x - 400]; set x [expr $x / 2]
    set y [winfo screenheight $w]; set y [expr $y - 20]; set y [expr $y / 2]
    wm geometry $w +$x+$y
    wm deiconify $w
    grab $w.b.cancel
    progressBarSet $w.c1 401 21
  }

  set searchArgs {}
  lappend searchArgs -filter RESET
  lappend searchArgs "-$::preport::_color"
  lappend searchArgs "\"$::preport::_player\""
  eval sc_search header $searchArgs
  if {$showProgress} {
    if {$::preport::_interrupt} {
      catch {grab release $w.b.cancel}
      destroy $w
      return
    }
    progressBarSet $w.c2 401 21
  }
  
  ::utils::history::AddEntry ::preport::_player $::preport::_player
  
  if {$::preport::_pos == "start"} { sc_game push }
  sc_search board AND Exact false 0
  if {$showProgress} {
    if {$::preport::_interrupt} {
      unbusyCursor .
      catch {grab release $w.b.cancel}
      destroy $w
      return
    }
    progressBarSet $w.c3 401 21
  }
  sc_report player create $::preport(ExtraMoves) $::preport(MaxGames)
  if {$::preport::_pos == "start"} { sc_game pop }
  if {$::preport::_clipbase} {
    if {[sc_base current] != $::clipbase_db} {
      sc_clipbase clear
      ::windows::gamelist::CopyGames "" [sc_base current] $::clipbase_db
    }
  }
  if {$showProgress} {
    catch {grab release $w.b.cancel}
    destroy $w
    if {$::preport::_interrupt} { return }
  }
  set report [::preport::report ctext 1]
  
  if {[lsearch -exact $args "-nodisplay"] >= 0} { return }
  
  set w .preportWin
  if {![winfo exists $w]} {
    toplevel $w
    wm title $w "Scid: $::tr(PReportTitle)"
    menu $w.menu
    ::setMenu $w $w.menu
    $w.menu add cascade -label OprepFile -menu $w.menu.file
    $w.menu add cascade -label OprepHelp -menu $w.menu.helpmenu
    foreach i {file helpmenu} {
      menu $w.menu.$i -tearoff 0
    }
    $w.menu.file add command -label OprepFileText \
        -command {::preport::saveReport text}
    $w.menu.file add command -label OprepFileHtml \
        -command {::preport::saveReport html}
    $w.menu.file add command -label OprepFileLaTeX \
        -command {::preport::saveReport latex}
    $w.menu.file add separator
    $w.menu.file add command -label OprepFileOptions \
        -command ::preport::setOptions
    $w.menu.file add separator
    $w.menu.file add command -label Close \
        -command "$w.b.close invoke"
    $w.menu.helpmenu add command -label "Player Report Help" \
        -accelerator F1 -command {helpWindow Reports Player}
    $w.menu.helpmenu add command -label "Index" \
        -command {helpWindow Index}
    
    bind $w <F1> {helpWindow Reports Player}
    bind $w <Escape> "$w.b.close invoke"
    bind $w <Up> "$w.text yview scroll -1 units"
    bind $w <Down> "$w.text yview scroll 1 units"
    bind $w <Prior> "$w.text yview scroll -1 pages"
    bind $w <Next> "$w.text yview scroll 1 pages"
    bind $w <Key-Home> "$w.text yview moveto 0"
    bind $w <Key-End> "$w.text yview moveto 0.99"

    autoscrollText y $w.scroll $w.text Treeview
    $w.text configure -height 30 -width 85 -font font_Small -state normal -wrap word
    ::htext::init $w.text
    ttk::frame $w.b
    ttk::button $w.b.opts -text [tr OprepFileOptions] -command ::preport::setOptions
    ttk::button $w.b.help -textvar ::tr(Help) -command {helpWindow Reports Player}
    ttk::button $w.b.viewHTML -text $::tr(OprepViewHTML) \
        -command ::preport::previewHTML
    ttk::button $w.b.update -textvar ::tr(Update...) -command {
      ::preport::preportDlg
    }
    ttk::button $w.b.close -textvar ::tr(Close) -command "focus .; destroy $w"
    pack $w.b -side bottom -fill x
    pack $w.scroll -side top -fill both -expand yes
    pack $w.b.close $w.b.update -side right -padx 2 -pady 2
    if {$::windowsOS} {
      pack $w.b.viewHTML -side left -padx 2 -pady 2
    }
    pack $w.b.opts -side left -padx 2 -pady 2
    ::preport::ConfigMenus
  }
  
  busyCursor .
  $w.text configure -state normal
  $w.text delete 1.0 end
  regsub -all "\n" $report "<br>" report
  ::htext::display $w.text $report
  $w.text configure -state disabled
  unbusyCursor .

  ::notify::DatabaseModified $::curr_db dbfilter
}


proc ::preport::setOptions {} {
  set w .preportOptions
  if {[winfo exists $w]} { return }
  win::createDialog $w
  pack [ttk::frame $w.f] -side top -fill x
  set row 0
  foreach i {Stats AvgPerf Results MovesFrom Themes Endgames} {
    set yesno($i) 1
  }
  foreach i {Stats Oldest Newest MostFrequentOpponents Results sep \
        AvgPerf HighRating sep \
        MostFrequentEcoCodes Themes Endgames sep \
        MaxGames ExtraMoves} {
    set from 0; set to 10; set tick 1; set res 1
    if {$i == "MaxGames"} {
      set from 0; set to 500; set tick 100; set res 50
    }
    if {$i == "sep"} {
      ttk::separator $w.f.fsep$row
      grid $w.f.fsep$row -row $row -column 0 -sticky we -columnspan 2

    } elseif {[info exists yesno($i)]} {
      ttk::checkbutton $w.f.f$i -variable ::preport($i) -offvalue 0 -onvalue 1 -text $::tr(Oprep$i)
      grid $w.f.f$i -row $row -column 0 -sticky w -columnspan 2
    } else {
        set tmpcombo {}
        for {set x $from} {$x <= $to} {incr x $res} {
          lappend tmpcombo $x
        }
      ttk::combobox $w.f.s$i -textvariable ::preport($i) -width 3 -values $tmpcombo -justify right -state readonly
      ttk::label $w.f.t$i -textvar ::tr(Oprep$i) -font font_Small
      grid $w.f.s$i -row $row -column 0 -sticky we -padx "0 5"
      grid $w.f.t$i -row $row -column 1 -sticky w
    }
    grid rowconfigure $w.f $row -pad 3
    incr row
  }
  addHorizontalRule $w
  pack [ttk::frame $w.b] -side bottom -fill x
  ttk::button $w.b.defaults -textvar ::tr(Defaults) -command {
    array set ::preport [array get ::preportDefaults]
  }
  ttk::button $w.b.ok -text "OK" -command {
    destroy .preportOptions
    catch {set ::preport::_data(yview) [lindex [.preportWin.text yview] 0]}
    ::preport::makeReportWin
    catch {.preportWin.text yview moveto $::preport::_data(yview)}
  }
  ttk::button $w.b.cancel -textvar ::tr(Cancel) -command {
    array set ::preport [array get ::preport::backup]
    destroy .preportOptions
  }
  pack $w.b.defaults -side left -padx 5 -pady "15 5"
  packdlgbuttons $w.b.cancel $w.b.ok
  array set ::preport::backup [array get ::preport]
  wm resizable $w 0 0
  wm title $w  "Scid: [tr ToolsPlayerReport]: [tr Options]"
  bind $w <Escape> "$w.b.cancel invoke"
}


# previewHTML:
#   Saves the report to a temporary file, and invokes the user's web
#   browser to display it.
#
proc ::preport::previewHTML {} {
  busyCursor .
  set tmpdir $::scidLogDir
  set tmpfile "TempPlayerReport"
  set fname [file join $tmpdir $tmpfile]
  if {[catch {set tempfile [open $fname.html w]}]} {
    tk_messageBox -title "Scid: Error writing report" -type ok -icon warning \
        -message "Unable to write the file: $fname.html"
  }
  puts $tempfile [::preport::report html 1]
  close $tempfile
  if {[string match $::tcl_platform(os) "Windows NT"]} {
    catch {exec $::env(COMSPEC) /c start $fname.html &}
  } else {
    catch {exec start $fname.html &}
  }
  unbusyCursor .
}

proc ::preport::saveReport {fmt} {
  set default ".txt"
  set ftype {
    { "Text files" {".txt"} }
    { "All files"  {"*"}    }
  }
  if {$fmt == "latex"} {
    set default ".tex"
    set ftype {
      { "LaTeX files" {".tex" ".ltx"} }
      { "All files"  {"*"}    }
    }
  } elseif {$fmt == "html"} {
    set default ".html"
    set ftype {
      { "HTML files" {".html" ".htm"} }
      { "All files"  {"*"}    }
    }
  }
  
  set fname [tk_getSaveFile -initialdir [pwd] -filetypes $ftype \
      -defaultextension $default -title "Scid: Save opening report"]
  if {$fname == ""} { return }
  
  busyCursor .
  if {[catch {set tempfile [open $fname w]}]} {
    tk_messageBox -title "Scid: Error writing report" -type ok -icon warning \
        -message "Unable to write the file: $fname\n\n"
  } else {
    set report [::preport::report $fmt 1]
    if {$::hasEncoding  &&  $::langEncoding($::language) != ""} {
      catch {set report [encoding convertto $::langEncoding($::language) $report]}
    }
    puts $tempfile $report
    close $tempfile
  }
  unbusyCursor .
}

proc ::preport::_reset {} {
  set ::preport::_data(sec) 0
  set ::preport::_data(subsec) 0
}

proc ::preport::_title {} {
  set fmt $::preport::_data(fmt)
  set title $::tr(PReportTitle)
  if {$fmt == "latex"} {
    return "\\begin{center}{\\LARGE \\bf $title}\\end{center}\n\n"
  } elseif {$fmt == "html"} {
    return "<h1><center>$title</center></h1>\n\n"
  } elseif {$fmt == "ctext"} {
    return "<h1><center>$title</center></h1>\n\n"
  }
  set r    "--------------------------------------------------------------"
  append r "\n                        [string toupper $title]\n"
  append r "--------------------------------------------------------------"
  append r "\n\n"
  return $r
}

proc ::preport::_sec {text} {
  set fmt $::preport::_data(fmt)
  incr ::preport::_data(sec)
  set ::preport::_data(subsec) 0
  if {$fmt == "latex"} {
    return "\n\n\\section{$text}\n"
  } elseif {$fmt == "html"} {
    return "\n<h2>$::preport::_data(sec). $text</h2>\n"
  } elseif {$fmt == "ctext"} {
    return "<h4>$::preport::_data(sec). $text</h4>"
  }
  set line "$::preport::_data(sec). [string toupper $text]"
  set underline "-----------------------------------------------------"
  return "\n\n$line\n[string range $underline 1 [string length $line]]\n"
}

proc ::preport::_subsec {text} {
  set fmt $::preport::_data(fmt)
  incr ::preport::_data(subsec)
  if {$fmt == "latex"} {
    return "\n\\subsection{$text}\n\n"
  } elseif {$fmt == "html"} {
    return "\n<h3>$::preport::_data(sec).$::preport::_data(subsec) $text</h3>\n\n"
  } elseif {$fmt == "ctext"} {
    return "\n<maroon><b>$::preport::_data(sec).$::preport::_data(subsec) $text</b></maroon>\n\n"
  }
  return "\n$::preport::_data(sec).$::preport::_data(subsec)  $text\n\n"
}


proc ::preport::report {fmt {withTable 1}} {
  global tr preport
  sc_report player format $fmt
  set fmt [string tolower $fmt]
  set ::preport::_data(fmt) $fmt
  ::preport::_reset
  
  # numRows: the number of rows to show in the theory table.
  # If it is zero, the number of rows if decided according to the
  # number of games in the report.
  set numRows 0
  
  # Specify whether a theory table is to be printed, so note numbers
  # can be generated and displayed if necessary:
  sc_report player notes $withTable $numRows
  
  set n "\n"; set p "\n\n"; set preText ""; set postText ""
  set percent "%"; set bullet "  * "
  if {$fmt == "latex"} {
    set n "\\\\\n"; set p "\n\n"
    #set preText "{\\samepage\\begin{verbatim}\n"
    #set postText "\\end{verbatim}\n}\n"
    set percent "\\%"; set bullet "\\hspace{0.5cm}\$\\bullet\$"
  } elseif {$fmt == "html"} {
    set n "<br>\n"; set p "<p>\n\n"
    set preText "<pre>\n"; set postText "</pre>\n"
  } elseif {$fmt == "ctext"} {
    set preText "<tt>"; set postText "</tt>"
  }
  
  # Generate the report:
  set games $tr(games)
  set moves $tr(moves)
  set counts [sc_report player count]
  set rgames [lindex $counts 0]
  set tgames [lindex $counts 1]
  
  set r {}
  append r $::optable::_docStart($fmt)
  set r [string map [list "\[OprepTitle\]" $tr(PReportTitle)] $r]
  append r [::preport::_title]
  append r "$tr(Player): \"$::preport::_player\""
  if {$::preport::_color == "white"} {
    append r " $tr(PReportColorWhite)"
  } else {
    append r " $tr(PReportColorBlack)"
  }
  set eco ""
  if {$::preport::_pos == "current"  &&  ![sc_pos isAt start]} {
    append r " [format $tr(PReportMoves) [sc_report player line]]"
    set eco [sc_report player eco]
  }
  append r " ("
  if {$fmt == "ctext"} {
    append r "<darkblue><run sc_report player select all 0; ::notify::DatabaseModified $::curr_db dbfilter>"
  }
  append r "$rgames"
  if {$fmt == "ctext"} { append r "</run></darkblue>"; }
  append r " $games)$n"
  append r "$tr(Database): [file tail [sc_base filename $::curr_db]] "
  append r "([::utils::thousands [sc_base numGames $::curr_db]] $games)$n"
  if {$eco != ""} {
    append r "$tr(ECO): $eco$n"
  }
  append r "$::tr(OprepGenerated) Scid $::scidVersion, [::utils::date::today]$n"
  
  if {$preport(Stats)  ||  $preport(Oldest) > 0  ||  $preport(Newest) > 0  ||
    $preport(MostFrequentOpponents) > 0  ||  $preport(Results)} {
    append r [::preport::_sec $tr(OprepStatsHist)]
  }
  if {$preport(Stats)} {
    append r [::preport::_subsec $tr(OprepStats)]
    append r [::optable::stats $fmt]
  }
  if {$preport(Oldest) > 0} {
    append r [::preport::_subsec $tr(OprepOldest)]
    append r [sc_report player best o $preport(Oldest)]
  }
  if {$preport(Newest) > 0} {
    append r [::preport::_subsec $tr(OprepNewest)]
    append r [sc_report player best n $preport(Newest)]
  }
  if {$preport(MostFrequentOpponents) > 0} {
    append r [::preport::_subsec "$tr(OprepMostFrequentOpponents)"]
    if {$::preport::_color == "white"} {
      append r [sc_report player players black $preport(MostFrequentOpponents)]
    } else {
      append r [sc_report player players white $preport(MostFrequentOpponents)]
    }
  }
  if {$preport(Results)} {
    append r [::preport::_subsec $::tr(OprepResults)]
    append r [::optable::results player $fmt]
  }
  
  if {$preport(AvgPerf)  ||  $preport(HighRating)} {
    append r [::preport::_sec $tr(OprepRatingsPerf)]
  }
  if {$preport(AvgPerf)} {
    append r [::preport::_subsec $tr(OprepAvgPerf)]
    set e [sc_report player elo white]
    set welo [lindex $e 0]; set wng [lindex $e 1]
    set bpct [lindex $e 2]; set bperf [lindex $e 3]
    set e [sc_report player elo black]
    set belo [lindex $e 0]; set bng [lindex $e 1]
    set wpct [lindex $e 2]; set wperf [lindex $e 3]
    append r "$tr(OprepWRating): $welo ($wng $games);  "
    append r "$tr(OprepWPerf): $wperf ($wpct$percent vs $belo)$n"
    append r "$tr(OprepBRating): $belo ($bng $games);  "
    append r "$tr(OprepBPerf): $bperf ($bpct$percent vs $welo)$n"
  }
  if {$preport(HighRating) > 0} {
    append r [::preport::_subsec $tr(OprepHighRating)]
    append r [sc_report player best a $preport(HighRating)]
  }
  
  if {$preport(Themes)  ||  $preport(MostFrequentEcoCodes) > 0  ||
    $preport(Endgames)} {
    append r [::preport::_sec $tr(OprepMovesThemes)]
  }
  if {$preport(MostFrequentEcoCodes) > 0} {
    append r [::preport::_subsec $tr(PReportOpenings)]
    append r [sc_report player eco $preport(MostFrequentEcoCodes)]
  }
  if {$preport(Themes)} {
    append r [::preport::_subsec $tr(OprepThemes)]
    append r [sc_report player themes $tr(OprepThemeDescription:) \
        $tr(OprepThemeSameCastling:) $tr(OprepThemeOppCastling:) \
        $tr(OprepThemeKPawnStorm:) $tr(OprepThemeQueenswap:) \
        $tr(OprepTheme1BishopPair:) \
        $tr(OprepThemeWIQP:) $tr(OprepThemeBIQP:) \
        $tr(OprepThemeWP567:) $tr(OprepThemeBP234:) \
        $tr(OprepThemeOpenCDE:) ]
  }
  if {$preport(Endgames)} {
    append r [::preport::_subsec $tr(OprepEndgames)]
    append r "$tr(OprepEndClass:)$n"
    append r [sc_report player endmat]
  }
  
  if {$withTable  &&  $::preport(MaxGames) > 0} {
    set sec [::preport::_sec $tr(OprepTheoryTable)]
    set comment ""
    if {$tgames > $::preport(MaxGames)} {
      set comment [format $tr(OprepTableComment) $::preport(MaxGames)]
    }
    append r [sc_report player print $numRows $sec $comment]
  }
  append r $::optable::_docEnd($fmt)
  
  # Eszet (ss) characters seem to be mishandled by LaTeX, even with
  # the font encoding package, so convert them explicitly:
  if {$fmt == "latex"} { regsub -all ß $r {{\\ss}} r }
  
  return $r
}
