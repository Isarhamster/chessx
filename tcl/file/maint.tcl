# maint.tcl:   Maintenance-related functions
# Part of Scid.
# Copyright (C) 2000-2004 Shane Hudson.

namespace eval ::maint {}

### TODO: Move procedures and variables into the maint namespace.
### TODO: Make sub-namespaces (sort, compact, cleaner, etc)

################################################################################
# ::maint::SetGameFlags
#
#   Updates a flag for the current game, all filtered games, or all games.
#   <type> should be "current", "filter" or "all".
#   <flag> should be "delete", "user", "endgame", etc.
#   <value> should be 0 or 1
#
proc ::maint::SetGameFlags {flag type value} {
  if {$flag == "mark"} { set flag $::maintFlag }
  if {$value} {
    set op "set"
  } else {
    set op "unset"
  }
  set base [sc_base current]
  switch -- $type {
    "current" {
      sc_base gameflag $base [sc_game number] $op $flag
    }
    "filter" {
      busyCursor .
      update idletasks
      sc_base gameflag $base "dbfilter" $op $flag
      unbusyCursor .
    }
    "all" {
      busyCursor .
      update idletasks
      sc_base gameflag $base "all" $op $flag
      unbusyCursor .
    }
    default { return }
  }
  updateBoard
  ::notify::DatabaseModified [sc_base current]
}

set maintFlag W
set maintFlaglist {W B M E N P T Q K ! ? U 1 2 3 4 5 6}
array set maintFlags {
  W WhiteOpFlag
  B BlackOpFlag
  M MiddlegameFlag
  E EndgameFlag
  N NoveltyFlag
  P PawnFlag
  T TacticsFlag
  Q QsideFlag
  K KsideFlag
  ! BrilliancyFlag
  ? BlunderFlag
  U UserFlag
  1 CustomFlag1
  2 CustomFlag2
  3 CustomFlag3
  4 CustomFlag4
  5 CustomFlag5
  6 CustomFlag6
}


set maintWin 0

# ::maint::OpenClose
#
#   Creates the database maintenance window.
#
proc ::maint::OpenClose {} {
  global maintWin maintFlag maintFlags maintFlaglist
  set w .maintWin
  if {[winfo exists $w]} {
    destroy $w
    set maintWin 0
    return
  }
  set maintWin 1
  set font font_Small
  set bold font_SmallBold
  win::createDialog $w
  wm title $w "Scid: [tr FileMaint]"
  wm resizable $w 0 0
  bind $w <F1> {helpWindow Maintenance}
  bind $w <Escape> "destroy $w; break"
  bind $w <Destroy> {set maintWin 0}
  
  ttk::frame $w.title
  ttk::label $w.title.name -text "[tr Database]:"
  ttk::label $w.title.vname -text "0" -font font_Bold
  ttk::label $w.title.icon
  ttk::button $w.title.vicon -command {changeBaseType [sc_base current]}
  grid $w.title.vicon -rowspan 2 -padx "0 10"
  grid $w.title.name -row 0 -column 1 -sticky w
  grid $w.title.vname -row 1 -column 1 -sticky nw
  grid columnconfigure $w.title 1 -weight 1

  ttk::frame $w.stats
  ttk::label $w.stats.games -textvar ::tr(NumOfGames) -font font_SmallBold
  ttk::label $w.stats.vgames -text "0" -font font_SmallBold
  ttk::label $w.stats.mark -font $font
  ttk::label $w.stats.vmark -text "0" -font $font
  ttk::label $w.stats.filter -textvar ::tr(NumFilterGames) -font $font
  ttk::label $w.stats.vfilter -text "0" -font $font
  ttk::label $w.stats.dates -textvar ::tr(YearRange) -font $font
  ttk::label $w.stats.vdates -text "0" -font $font
  ttk::label $w.stats.ratings -textvar ::tr(RatingRange) -font $font
  ttk::label $w.stats.vratings -text "0" -font $font
  grid $w.stats.games x x $w.stats.dates x x $w.stats.ratings -row 0 -sticky w
  grid x $w.stats.vgames x x $w.stats.vdates x x $w.stats.vratings -row 0 -sticky e
  grid x x x $w.stats.mark x x $w.stats.filter -row 1 -sticky w
  grid x x x x $w.stats.vmark x x $w.stats.vfilter -row 1 -sticky e
  grid columnconfigure $w.stats 2 -weight 1
  grid columnconfigure $w.stats 5 -weight 1
  
  ttk::frame $w.dbdesc
  ttk::label $w.dbdesc.lab -text $::tr(Description:) -font font_SmallBold
  ttk::entry $w.dbdesc.text -textvariable ::maint::dbdesc -validate key -validatecommand "
    $w.dbdesc.edit configure -state normal
    return true
  "
  ttk::button $w.dbdesc.edit -text "[tr Save]" -style Small.TButton -command {
    if { [catch {sc_base extra $::curr_db description $::maint::dbdesc}] } {
      ERROR::MessageBox
    }
    ::maint::Refresh
  }
  grid $w.dbdesc.lab $w.dbdesc.text -padx "0 5" -sticky we
  grid $w.dbdesc.edit -row 0 -column 2 -sticky e
  grid columnconfigure $w.dbdesc 1 -weight 1

  ttk::frame $w.customFlags
  ttk::label $w.customFlags.lab -text "[::tr CustomFlags]:" -font font_SmallBold
  ttk::entry $w.customFlags.text1 -width 8 -validate key -validatecommand "::maint::validateCustomFlag $w %P"
  ttk::entry $w.customFlags.text2 -width 8 -validate key -validatecommand "::maint::validateCustomFlag $w %P"
  ttk::entry $w.customFlags.text3 -width 8 -validate key -validatecommand "::maint::validateCustomFlag $w %P"
  ttk::entry $w.customFlags.text4 -width 8 -validate key -validatecommand "::maint::validateCustomFlag $w %P"
  ttk::entry $w.customFlags.text5 -width 8 -validate key -validatecommand "::maint::validateCustomFlag $w %P"
  ttk::entry $w.customFlags.text6 -width 8 -validate key -validatecommand "::maint::validateCustomFlag $w %P"
  ttk::button $w.customFlags.edit -text "[tr Save]" -style Small.TButton -command "::maint::saveCustomFlags $w"
  grid $w.customFlags.lab $w.customFlags.text1 $w.customFlags.text2 $w.customFlags.text3 \
       $w.customFlags.text4 $w.customFlags.text5 $w.customFlags.text6 -padx "0 5"
  grid $w.customFlags.edit -row 0 -column 7 -sticky e
  grid columnconfigure $w.customFlags 7 -weight 1

  ttk::frame $w.autog
  ttk::label $w.autog.lab -text $::tr(AutoloadGame:) -font font_SmallBold
  ttk::entry $w.autog.text -width 10 -justify right -textvariable autoloadGame -validate key -validatecommand {
    if {![string is integer %P]} { return false }
    .maintWin.autog.edit configure -state normal
    return true
  }
  ttk::button $w.autog.current -text $::tr(Current) -style Small.TButton -command {
    set ::autoloadGame [sc_game number]
    .maintWin.autog.edit configure -state normal
  }
  ttk::button $w.autog.edit -text "[tr Save]" -style Small.TButton -command {
    sc_base extra $::curr_db autoload $::autoloadGame
    ::maint::Refresh
  }
  grid $w.autog.lab $w.autog.text $w.autog.current -padx "0 5"
  grid $w.autog.edit -row 0 -column 3 -sticky e
  grid columnconfigure $w.autog 3 -weight 1

  ttk::frame $w.dm
  ttk::labelframe $w.dm.delete -text [tr DeleteFlag]
  ttk::labelframe $w.dm.mark -text [tr Flag]
  grid $w.dm.delete -row 0 -column 0 -sticky snwe -padx "0 10" -pady "0 10"
  grid $w.dm.mark -row 0 -column 1 -sticky snwe -pady "0 10"
  grid columnconfigure $w.dm.mark 0 -weight 1
  grid columnconfigure $w.dm.mark 1 -weight 1
  grid columnconfigure $w.dm 0 -weight 1
  grid columnconfigure $w.dm 1 -weight 1

  ttk::label $w.dm.delete.vdelete
  ttk::menubutton $w.dm.mark.title -menu $w.dm.mark.title.m
  menu $w.dm.mark.title.m -font $font
  
  foreach flag $maintFlaglist  {
      $w.dm.mark.title.m add command -label $flag -command "set maintFlag $flag; ::maint::Refresh"
  }
  
  foreach flag {delete mark} on {Delete Mark} off {Undelete Unmark} {
    set row 0
    foreach b {Current Filter All} {
      ttk::button $w.dm.$flag.on$b -textvar "::tr($on$b)" -style Small.TButton -command "::maint::SetGameFlags $flag [string tolower $b] 1"
      ttk::button $w.dm.$flag.off$b -textvar "::tr($off$b)" -style Small.TButton -command "::maint::SetGameFlags $flag [string tolower $b] 0"
    }

    if { $flag eq "mark" } {
      grid $w.dm.$flag.title -columnspan 2 -row 0 -column 0 -sticky we -padx 30 -pady "0 5"
    } else {
      grid $w.dm.$flag.vdelete -columnspan 2 -sticky w
    }
    incr row
    grid $w.dm.$flag.onCurrent -row $row -column 0 -sticky we -padx "0 5" -pady "0 5"
    grid $w.dm.$flag.offCurrent -row $row -column 1 -sticky we -pady "0 5"
    incr row
    grid $w.dm.$flag.onFilter -row $row -column 0 -sticky we -padx "0 5" -pady "0 5"
    grid $w.dm.$flag.offFilter -row $row -column 1 -sticky we -pady "0 5"
    incr row
    grid $w.dm.$flag.onAll -row $row -column 0 -sticky we -padx "0 5" -pady "0 5"
    grid $w.dm.$flag.offAll -row $row -column 1 -sticky we -pady "0 5"
  }
  grid rowconfigure $w.dm.delete 0 -weight 1
  grid columnconfigure $w.dm.delete 0 -weight 1
  grid columnconfigure $w.dm.delete 1 -weight 1

  grid $w.title -sticky news
  grid $w.stats -pady 5 -sticky news
  grid $w.dbdesc -sticky news
  grid $w.autog -pady 5 -sticky news
  grid $w.customFlags -sticky news
  grid $w.dm -pady "10 0" -sticky news

  ::maint::Refresh
}

proc ::maint::validateCustomFlag {w val} {
  if {[string length $val] > 8} { return false }
  $w.customFlags.edit configure -state normal
  return true
}
proc ::maint::saveCustomFlags {w} {
    for {set i 1} {$i <7} {incr i} {
      set desc [$w.customFlags.text$i get]
      sc_base extra $::curr_db flag$i $desc
    }
    ::maint::Refresh
}

proc ::maint::Refresh {} {
  global maintFlag maintFlags
  updateClassifyWin
  
  set w .maintWin
  if {![winfo exists $w]} { return }
  set ::curr_db [sc_base current]
  set ng [sc_base numGames $::curr_db]
  set dates [sc_base stats $::curr_db dates]
  set deleted [sc_base stats $::curr_db flag D]
  set marked [sc_base stats $::curr_db flag $maintFlag]
  set flags [sc_base stats $::curr_db flags]
  set ratings [sc_base stats $::curr_db ratings]
  $w.title.vicon configure -image dbt0
  $w.title.vname configure -text [file tail [sc_base filename $::curr_db]]
  $w.stats.vgames configure -text [::utils::thousands $ng]
  $w.dm.delete.vdelete configure -text "[tr NumDeletedGames]: [::utils::percentFormat $deleted $ng]"
  $w.stats.vmark configure -text [::utils::percentFormat $marked $ng]
  $w.stats.vfilter configure -text [::utils::percentFormat [sc_filter count] $ng]
  $w.stats.vdates configure \
      -text "[lindex $dates 0]-[lindex $dates 1] ([lindex $dates 2])"
  $w.stats.vratings configure \
      -text "[lindex $ratings 0]-[lindex $ratings 1] ([lindex $ratings 2])"
  
  set i 0
  foreach flag $::maintFlaglist  {
      $w.dm.mark.title.m entryconfigure $i -label "[tr $maintFlags($flag)] ($flag)"
      incr i
  }

  grid remove $w.dbdesc
  grid remove $w.autog
  grid remove $w.customFlags
  foreach {tagname tagvalue} [sc_base extra $::curr_db] {
    if {$tagname eq "type"} {
      catch { $w.title.vicon configure -image dbt$tagvalue }
    } elseif {$tagname eq "description"} {
      set ::maint::dbdesc $tagvalue
      grid $w.dbdesc
    } elseif {$tagname eq "autoload" } {
      set ::autoloadGame $tagvalue
      grid $w.autog
    } elseif { [regexp {flag([1-6])} $tagname -> i] } {
      $w.customFlags.text$i configure -state normal
      $w.customFlags.text$i delete 0 end
      $w.customFlags.text$i insert end $tagvalue
      grid $w.customFlags
      if {$tagvalue ne ""} {
        $w.dm.mark.title.m entryconfigure [expr $i + 11] -label "$tagvalue ($i)"
      }
    }
  }

  set idx [lsearch -exact $::maintFlaglist $::maintFlag ]
  if { $idx != -1 } {
    set flagname [$w.dm.mark.title.m entrycget $idx -label]
    $w.stats.mark configure -text $flagname
    $w.dm.mark.title configure -text $flagname
  }

  # Set widget's states
  set state [expr {[sc_base isReadOnly $::curr_db] ? "disabled" : "normal"}]
  foreach frame {title dbdesc autog customFlags} {
    foreach widget [winfo children $w.$frame] {
	  if {[winfo class $widget] eq "TLabel"} { continue }
	  $widget configure -state $state
	}
  }
  $w.dbdesc.edit configure -state disabled
  $w.autog.edit configure -state disabled
  $w.customFlags.edit configure -state disabled

  set state [expr {$state eq "disabled" || $ng == 0 ? "disabled" : "normal"}]
  foreach frame {dm.delete dm.mark} {
    foreach widget [winfo children $w.$frame] {
	  if {[winfo class $widget] eq "TLabel"} { continue }
	  $widget configure -state $state
	}
  }
  if {$state eq "normal" && ![sc_game number]} {
     $w.dm.delete.onCurrent configure -state disabled
     $w.dm.delete.offCurrent configure -state disabled
     $w.dm.mark.onCurrent configure -state disabled
     $w.dm.mark.offCurrent configure -state disabled
  }
}

# markTwins:
#   Finds twin games and marks them for deletion.
#   Takes parent window as parameter since it can be the main window,
#   or the maintenance window.
#
proc markTwins {{parent .}} {
  global twinSettings
  if {[sc_base numGames $::curr_db] == 0} {
    tk_messageBox -type ok -icon info -title [concat "Scid: " $::tr(noGames)] \
        -message $::tr(TwinCheckNoDelete)
    return
  }
  
  set w .twinSettings
  if {! [winfo exists $w]} {
    win::createDialog $w
    wm resizable $w 0 0
    wm title $w "Scid: $::tr(DeleteTwins)"
    pack [ttk::frame $w.f]
    set small font_Small
    
    ttk::label $w.f.note -text $::tr(TwinsNote) -justify left -wraplength 300 -font $small
    pack $w.f.note -side top -anchor w -ipady 0 -pady 0
    ttk::labelframe $w.f.g -text $::tr(TwinsCriteria)
    pack $w.f.g -side top -anchor w -fill x -pady 10
    set row 0
    set col 0
    foreach name {Colors Event Day Result Round Month ECO Site Year Moves} {
      set n [string tolower $name]
      ttk::checkbutton $w.f.g.b$n -text $::tr(Twins$name) \
          -variable twinSettings($n) -onvalue Yes -offvalue No
      grid $w.f.g.b$n -row $row -column $col -sticky w
      incr col
      if {$col >= 4} {
        incr row; set col 0
      } else {
        grid [ttk::label $w.f.g.space$n -text "   "] -row $row -column $col
        incr col
      }
    }
    ttk::frame $w.f.pg
    pack $w.f.pg -side top -anchor w -pady 10 -fill x
    ttk::labelframe $w.f.pg.players -text $::tr(TwinsPlayers)
    ttk::radiobutton $w.f.pg.players.yes -variable twinSettings(players) -value Yes \
        -text $::tr(TwinsPlayersExact) -style Small.TRadiobutton
    ttk::radiobutton $w.f.pg.players.no -variable twinSettings(players) -value No \
        -text $::tr(TwinsPlayersPrefix) -style Small.TRadiobutton
    
    pack $w.f.pg.players.yes $w.f.pg.players.no -side top -anchor w
  }
  
  ttk::labelframe $w.f.pg.g2 -text $::tr(TwinsWhich)
  pack $w.f.pg.players $w.f.pg.g2 -side left -anchor w -fill x -padx "0 15"
  ttk::radiobutton $w.f.pg.g2.exall -text $::tr(SelectAllGames) -style Small.TRadiobutton \
      -variable twinSettings(usefilter) -value No
  ttk::radiobutton $w.f.pg.g2.exfil -text $::tr(SelectFilterGames) -style Small.TRadiobutton \
      -variable twinSettings(usefilter) -value Yes
  pack $w.f.pg.g2.exall $w.f.pg.g2.exfil -side top -fill x -anchor w

  ttk::labelframe $w.f.g3 -text $::tr(TwinsWhen)
  pack $w.f.g3 -side top -fill x
  set row 0
  foreach n {skipshort undelete setfilter comments variations} \
      name {SkipShort Undelete SetFilter Comments Vars} {
        ttk::checkbutton $w.f.g3.b$n -text $::tr(Twins$name) -variable twinSettings($n) -onvalue Yes -offvalue No
        grid $w.f.g3.b$n -row $row -column 0 -sticky w
        incr row
      }
  ttk::label $w.f.g3.ldelete -text $::tr(TwinsDeleteWhich) -font font_Bold
  grid $w.f.g3.ldelete -row 0 -column 1 -sticky w -padx "15 0"

  set row 1
  foreach v {Shorter Older Newer} {
    ttk::radiobutton $w.f.g3.v$v -text $::tr(TwinsDelete$v) \
        -variable twinSettings(delete) -value $v -style Small.TRadiobutton
     grid $w.f.g3.v$v -row $row -column 1 -padx "25 0" -sticky w
     incr row
  }

  ttk::frame $w.f.b
  dialogbutton $w.f.b.defaults -textvar ::tr(Defaults) -command {
    array set twinSettings [array get twinSettingsDefaults]
  }
  dialogbuttonsmall $w.f.b.help [ list -text $::tr(Help) -command "helpWindow Maintenance Twins; focus $w" ]
  dialogbuttonsmall $w.f.b.go [ list -text $::tr(TwinsDelete) -command {
    if {[twinCriteriaOK .twinSettings]} {
      grab release .twinSettings
      .twinSettings.f.b.cancel configure -command "progressBarCancel"
      set result [doMarkDups .twinSettings]
      focus .
      destroy .twinSettings
      if {$result > 0} {
        ::notify::DatabaseChanged
        set gn [sc_filter first]
        ::game::Load $gn
        updateTwinChecker
      }
    }
  } ]
  
  dialogbuttonsmall $w.f.b.cancel [ list -text $::tr(Cancel) -command "grab release $w; focus .; destroy $w" ]
  
  canvas $w.f.progress -width 300 -height 20 -bg white -relief solid -border 1
  $w.f.progress create rectangle 0 0 0 0 -fill blue -outline blue -tags bar
  $w.f.progress create text 295 10 -anchor e -font font_Regular -tags time \
      -fill black -text "0:00 / 0:00"
  
  pack $w.f.progress -side bottom -padx 2 -pady 2
  pack $w.f.b -side bottom -fill x
  packdlgbuttons  $w.f.b.cancel $w.f.b.go
  pack $w.f.b.defaults $w.f.b.help -side left -padx 5 -pady "15 5"
  bind $w <F1> "$w.f.b.help invoke"
  bind $w <Escape> "$w.f.b.cancel invoke"
  bind $w <Return> "$w.f.b.go invoke"
  grab $w
  update idletasks
  $w.f.note configure -wraplength [winfo width $w]
  return
}

# twinCriteriaOK:
#   Check that the user specified at least three of the the same site,
#   same round, and same year settings, since otherwise it is quite
#   likely that actual games with similar moves will be marked as twins:
#
proc twinCriteriaOK {{parent .}} {
  global twinSettings
  
  set msg $::tr(TwinCriteria1)
  
  # First, check that if same moves is off, then the same colors, event,
  # site, round, year and month flags should all be set:
  if {$twinSettings(moves) == "No"} {
    if {$twinSettings(colors) == "No"  ||  $twinSettings(event) == "No"  || \
          $twinSettings(site) == "No"  ||  $twinSettings(year) == "No"  || \
          $twinSettings(month) == "No"} {
      append msg $::tr(TwinCriteria2)
      set result [tk_messageBox -type yesno -parent $parent -icon warning \
          -title $::tr(TwinCriteriaConfirm) \
          -message $msg]
      if {$result == "no"} { return 0 } else { return 1 }
    }
  }
  
  # Now check that at least two of site, round, and year are set:
  set count 0
  if {$twinSettings(site) == "Yes"} { incr count }
  if {$twinSettings(round) == "Yes"} { incr count }
  if {$twinSettings(year) == "Yes"} { incr count }
  if {$count < 2} {
    append msg $::tr(TwinCriteria3)
    set result [tk_messageBox -type yesno -parent $parent -icon warning \
        -title $::tr(TwinCriteriaConfirm) \
        -message $msg]
    if {$result == "no"} { return 0 } else { return 1 }
  }
  return 1
}


proc doMarkDups {{parent .}} {
  global twinSettings
  
  if {$twinSettings(undelete) == "Yes"} {
    if {[catch {sc_base gameflag [sc_base current] all unset del}]} {
      unbusyCursor .
      ERROR::MessageBox
      return 0
    }
  }

  progressBarSet $parent.f.progress 301 21
  if {[catch {sc_base duplicates [sc_base current] \
          -colors $twinSettings(colors) \
          -event $twinSettings(event) -site $twinSettings(site) \
          -round $twinSettings(round) -year $twinSettings(year) \
          -month $twinSettings(month) -day $twinSettings(day) \
          -result $twinSettings(result) -eco $twinSettings(eco) \
          -moves $twinSettings(moves) -players $twinSettings(players) \
          -skipshort $twinSettings(skipshort) \
          -setfilter $twinSettings(setfilter) \
          -usefilter $twinSettings(usefilter) \
          -comments $twinSettings(comments) \
          -variations $twinSettings(variations) \
          -delete $twinSettings(delete)} result]} {
    ERROR::MessageBox
    set result 0
  } else {
    set message [subst $::tr(TwinCheckFound1)]
    if {$result > 0} {append message $::tr(TwinCheckFound2)}
    append message "."
    tk_messageBox -type ok -parent $parent -icon info -title [concat "Scid: " $::tr(Result)] \
        -message $message
  }
  ::maint::Refresh
  return $result
}


set classifyOption(AllGames) all
set classifyOption(ExtendedCodes) 1

# ClassifyAllGames:
#   Reclassifies all games (recomputes the ECO code of each game).
#   User can choose to reclassify all games, or only those games that
#   currently have no ECO code assigned.
#
proc classifyAllGames {} {
  makeClassifyWin
}

proc makeClassifyWin {} {
  global classifyOption
  set w .classify
  if {[winfo exists $w]} {
    raiseWin $w
    return
  }
  win::createDialog $w
  wm title $w "Scid: [tr FileMaintClass]"
  
  pack [ttk::frame $w.f] -expand 1
  ttk::labelframe  $w.f.g -text $::tr(ClassifyWhich)
  ttk::radiobutton $w.f.g.all -textvar ::tr(ClassifyAll) -variable classifyOption(AllGames) -value all
  ttk::radiobutton $w.f.g.filter -textvar ::tr(SelectFilterGames) -variable classifyOption(AllGames) -value filter
  set year [::utils::date::today year]
  set month [::utils::date::today month]
  set day [::utils::date::today day]
  ttk::radiobutton $w.f.g.year -textvar ::tr(ClassifyYear) -variable classifyOption(AllGames) \
      -value "date:[expr $year - 1].$month.$day"
  if {$month == "01"} {
    incr year -1
    set month 12
  } else {
    scan $month "%02u" month
    incr month -1
    set month [format "%02u" $month]
  }
  ttk::radiobutton $w.f.g.month -textvar ::tr(ClassifyMonth) -variable classifyOption(AllGames) \
      -value "date:$year.$month.$day"
  ttk::radiobutton $w.f.g.new -textvar ::tr(ClassifyNew) -variable classifyOption(AllGames) -value nocode
  set row 0
  foreach f {all filter year month new} {
    grid $w.f.g.$f -row $row -column 0 -sticky w
    incr row
  }
  ttk::labelframe $w.f.codes -text $::tr(ClassifyCodes)
  ttk::radiobutton $w.f.codes.extended -textvar ::tr(ClassifyBasic) -variable classifyOption(ExtendedCodes) -value 0
  ttk::radiobutton $w.f.codes.basic -textvar ::tr(ClassifyExtended) -variable classifyOption(ExtendedCodes) -value 1
  
  ttk::frame $w.f.b
  ttk::button $w.f.b.go -textvar ::tr(Classify) -command {
    .classify.f.b.cancel configure -command "progressBarCancel"
    .classify.f.b.cancel configure -textvar ::tr(Stop)
    progressBarSet .classify.f.progress 301 21
    grab .classify.f.b.cancel
    if {[catch  {sc_eco base $classifyOption(AllGames) $classifyOption(ExtendedCodes)} result]} {
      grab release .classify.f.b.cancel
      ERROR::MessageBox
    } else {
      grab release .classify.f.b.cancel
    }
    .classify.f.b.cancel configure -command {focus .; destroy .classify}
    .classify.f.b.cancel configure -textvar ::tr(Close)
    ::windows::gamelist::Refresh
  }
  ttk::button $w.f.b.cancel -textvar ::tr(Close) -command "focus .; destroy $w"
  canvas $w.f.progress -width 300 -height 20 -bg white -relief solid -border 1
  $w.f.progress create rectangle 0 0 0 0 -fill blue -outline blue -tags bar
  $w.f.progress create text 295 10 -anchor e -font font_Regular -tags time \
      -fill black -text "0:00 / 0:00"
  
  pack $w.f.g -anchor w -fill x -side top -pady "0 10"

  pack $w.f.codes -side top -pady 10 -anchor w -fill x
  pack $w.f.codes.extended $w.f.codes.basic -side top -anchor w -fill x
  pack $w.f.b -side top -fill x
  packdlgbuttons $w.f.b.cancel $w.f.b.go
  pack $w.f.progress -side bottom -padx 2 -pady 2
  wm resizable $w 0 0
  bind $w <F1> {helpWindow ECO}
  bind $w <Escape> "$w.b.cancel invoke"
  updateClassifyWin
}

proc updateClassifyWin {} {
  set w .classify
  if {! [winfo exists $w]} { return }
  set state disabled
  if {[sc_base inUse]} { set state normal }
  $w.f.b.go configure -state $state
}

# Twin checker window:
# Shows PGN of current game, and its twin.

set twincheck(left) 0
set twincheck(right) 0

proc updateTwinChecker {} {
  global twincheck
  set w .twinchecker
  if {![winfo exists $w]} {
    win::createDialog $w
    pack [ttk::frame $w.b] -side bottom -fill x
    pack [ttk::frame $w.f] -side top -fill both -expand yes
    ttk::frame $w.f.left
    pack $w.f.left -side left -fill y -expand yes
    ttk::frame $w.f.right
    pack $w.f.right -side left -fill y -expand yes -padx "10 0"
    foreach i {left right} {
      set f $w.f.$i
      pack [ttk::frame $f.title] -side top -fill x
      ttk::label $f.title.label -font font_Bold -text [concat $::tr(game) " 0: "]
      ttk::checkbutton $f.title.d -text $::tr(Deleted) -variable twincheck($i) -style Small.TCheckbutton
      ttk::label $f.title.note -font font_Small
      pack $f.title.label $f.title.d $f.title.note -side left
      ttk::label $f.tmt -font font_Small -text "" -anchor w
      pack $f.tmt -side bottom -fill x
      autoscrollframe $f.t text $f.t.text \
          -height 16 -width 40 -background white \
          -takefocus 0 -wrap word
      $f.t.text tag configure h -background lightSteelBlue
      pack $f.t -side top -fill both -expand yes
    }
    $w.f.left.title.note configure -text [concat "(\"1\"" $::tr(TwinCheckUndelete)]
    $w.f.right.title.note configure -text  [concat "(\"2\"" $::tr(TwinCheckUndelete)]
    ttk::button $w.b.prev -text $::tr(TwinCheckprevPair) \
        -command {::game::LoadNextPrev previous}
    ttk::button $w.b.next -text $::tr(TwinChecknextPair) -underline 0 \
        -command {::game::LoadNextPrev next}
    ttk::button $w.b.share -text $::tr(TwinCheckTag) -underline 0
    ttk::button $w.b.delete -text $::tr(DeleteTwins) -underline 0 \
        -command "markTwins $w"
    ttk::button $w.b.help -text $::tr(Help) -command {helpWindow Maintenance Twins}
    ttk::button $w.b.close -text $::tr(Close) -command "focus .; destroy $w"
    packdlgbuttons $w.b.close $w.b.delete $w.b.help
    pack $w.b.prev $w.b.next $w.b.share -side left -padx 5 -pady "15 5"
    bind $w <F1> "$w.b.help invoke"
    bind $w <Escape> "focus .; destroy $w"
    bind $w <Alt-p> {::game::LoadNextPrev previous}
    bind $w <KeyPress-p> {::game::LoadNextPrev previous}
    bind $w <Alt-n> {::game::LoadNextPrev next}
    bind $w <KeyPress-n> {::game::LoadNextPrev next}
    bind $w <Alt-d> "markTwins $w"
    bind $w <KeyPress-d> "markTwins $w"
    bind $w <KeyPress-1> "$w.f.left.title.d invoke"
    bind $w <KeyPress-$::MB2> "$w.f.right.title.d invoke"
    bind $w <KeyPress-s> "$w.b.share invoke"
    bind $w <KeyPress-u> {
      if {$twincheck(left)} {.twinchecker.f.left.title.d invoke}
      if {$twincheck(right)} {.twinchecker.f.right.title.d invoke}
    }
    bind $w <Alt-u> {
      if {$twincheck(left)} {.twinchecker.f.left.title.d invoke}
      if {$twincheck(right)} {.twinchecker.f.right.title.d invoke}
    }
    wm resizable $w 0 1
    wm title $w $::tr(TwinChecker)
  }
  
  set gn [sc_game number]
  set dup 0
  if {$gn > 0} {
    set dup [sc_game info duplicate]
  }
  set twincheck(left) 0
  set twincheck(right) 0
  
  $w.f.left.title.label configure -text [concat $::tr(game) " $gn:  "]
  
  if {$gn > 0} {
    set twincheck(left) [sc_base gameflag [sc_base current] $gn get del]
    $w.f.left.title.d configure -command "sc_base gameflag \[sc_base current\] $gn invert del; ::notify::GameChanged"
    $w.f.left.title.d configure -state normal
    set tmt [sc_game crosstable count +deleted]
    $w.f.left.tmt configure -text [concat $::tr(TwinCheckTournament) $tmt]
  } else {
    $w.f.left.title.d configure -state disabled
    $w.f.left.tmt configure -text ""
  }
  if {$dup > 0} {
    set twincheck(right) [sc_base gameflag [sc_base current] $dup get del]
    $w.f.right.title.label configure -text [concat $::tr(game) " $dup:  "]
    $w.f.right.title.d configure -command "sc_base gameflag \[sc_base current\] $dup invert del; ::notify::GameChanged"
    $w.f.right.title.d configure -state normal
    set tmt [sc_game crosstable count -game $dup +deleted]
    $w.f.right.tmt configure -text [concat $::tr(TwinCheckTournament) $tmt]
  } else {
    $w.f.right.title.label configure -text $::tr(TwinCheckNoTwin)
    $w.f.right.title.d configure -state disabled
    $w.f.right.tmt configure -text ""
  }
  
  $w.b.share configure -state disabled -command {}
  if {$gn > 0  &&  $dup > 0} {
    if {[llength [sc_game tags share check $gn $dup]] > 0} {
      $w.b.share configure -state normal -command "shareTwinTags $gn $dup $w"
    }
  }
  set t $w.f.left.t.text
  $t configure -state normal
  $t delete 1.0 end
  $t insert end [sc_game pgn]
  
  set t $w.f.right.t.text
  $t configure -state normal
  $t delete 1.0 end
  if {$dup > 0} {
    $t insert end [sc_game pgn -gameNumber $dup]
  } else {
    $t insert end $::tr(TwinCheckNoTwinfound)
  }
  
  # Now color the differences if appropriate:
  if {$dup > 0} {
    set rlen [$w.f.right.t.text index end-1c]
    set llen [$w.f.left.t.text index end-1c]
    
    for {set i 0} {$i < $rlen} {incr i} {
      set line [$w.f.right.t.text get $i.0 "$i.0 lineend"]
      set length [string length $line]
      set max 0
      for {set j 0} {$j < $llen} {incr j} {
        set otherLine [$w.f.left.t.text get $j.0 "$j.0 lineend"]
        set plen [strPrefixLen $line $otherLine]
        if {$plen > $max} { set max $plen }
      }
      if {$max < $length} {
        if {![string compare [string index $line 0] "\["]} { set max 0 }
        $w.f.right.t.text tag add h $i.$max "$i.0 lineend"
      }
    }
    
    for {set i 0} {$i < $llen} {incr i} {
      set line [$w.f.left.t.text get $i.0 "$i.0 lineend"]
      set length [string length $line]
      set max 0
      for {set j 0} {$j < $rlen} {incr j} {
        set otherLine [$w.f.right.t.text get $j.0 "$j.0 lineend"]
        set plen [strPrefixLen $line $otherLine]
        if {$plen > $max} { set max $plen }
      }
      if {$max < $length} {
        if {![string compare [string index $line 0] "\["]} { set max 0 }
        $w.f.left.t.text tag add h $i.$max "$i.0 lineend"
      }
    }
  }
  
  if {[sc_base inUse]} {
    $w.b.delete configure -state normal
  } else {
    $w.b.delete configure -state disabled
  }
  
  foreach side {left right} {
    $w.f.$side.t.text configure -state disabled
  }
  
}

# shareTwinTags:
#   Updates the tags of two twin games by sharing information,
#   filling in the date, round or ratings of each game based on
#   the other where possible.
#
proc shareTwinTags {g1 g2 {parent .}} {
  set sharelist [sc_game tags share check $g1 $g2]
  if {[llength $sharelist] == 0} { return }
  
  set msg $::tr(TwinChangeTag)
  foreach {gn tag old new} $sharelist {
    append msg [concat $::tr(game) " $gn: $tag: \"$old\" -> \"$new\""]
    append msg "\n"
  }
  set answer [tk_messageBox -parent $parent -title "Scid" \
      -type okcancel -default ok -icon question -message $msg]
  if {$answer != "ok"} { return }
  sc_game tags share update $g1 $g2
  sc_game tags reload
  updateBoard -pgn
  ::windows::gamelist::Refresh
}

# baseIsCompactable:
#   Returns true only if the current base is compactable.
#
proc baseIsCompactable {} {
  # Only a database that is in use, not read-only, and not the
  # clipbase, can be compacted:
  set curr_base [sc_base current]
  if {[sc_base isReadOnly $curr_base]} { return 0 }
  if {$curr_base == $::clipbase_db} { return 0 }
  return 1
}

proc compactDB {{base -1}} {
  if {$base < 0} { set base [sc_base current] }
  if {[::game::Clear] == "cancel"} { return }
  if {[catch {sc_base compact $base stats} stats]} {
    return [ERROR::MessageBox "$::tr(CompactDatabase)\n"]
  }
  set msg "[sc_base filename $base]\n\n"
  append msg "Deleted games: [lindex $stats 0]\n"
  append msg "Unused names: [lindex $stats 1]\n"
  append msg "Sparse games: [lindex $stats 2]\n"
  append msg "Missing names (bad idx): [lindex $stats 3]"
  append msg "\n\nProceed?"
  set confirm [tk_messageBox -type okcancel -icon info -parent . \
               -title [concat "Scid: " $::tr(CompactDatabase)] \
               -message "$msg"]
  if {$confirm != "ok"} { return }

  #Ugly, but the safest approach, because:
  #  On windows child process inherits the file handles (if the child process was created
  #    after opening the database std::remove in scidBaseT::compact will fail)
  #  A child process may cause a racing condition (indirectly calling sc_base functions)
  #    i.e. in "annotate mode" an engine can save a game during the compaction
  #    and it is difficult and too risky to try to predict all cases
  # TODO: avoid file handle inheritance
  #       close the database before the compaction
  #
  destroy .analysisWin1
  destroy .analysisWin2
  destroy .coachWin
  destroy .tacticsWin
  destroy .reviewgame
  if {[winfo exists .calvarWin]} { ::calvar::stop }
  destroy .inputengineconsole

  progressWindow "Scid" [concat $::tr(CompactDatabase) "..."] $::tr(Cancel)
  set err [catch {sc_base compact $base} result]
  closeProgressWindow
  if {$err} {
    set extra "$::tr(CompactDatabase)\n"
    if {$::errorCode == $::ERROR::FileOpen} {
      append extra "\n$::ERROR::msg(CompactCreate)"
    }
    ERROR::MessageBox "$extra"
    ::file::SwitchToBase $::clipbase_db 0
    ::notify::DatabaseChanged
  } else {
    set msg "[sc_base filename $base]\n\n"
    append msg [tr GameFileCompacted]
    tk_messageBox -type ok -icon info -parent . \
        -title [concat "Scid: " $::tr(CompactDatabase)] \
        -message "$msg"
    ::notify::DatabaseModified $base
    ::game::Clear
  }
}

# allocateRatings:
#   Allocate player ratings to games based on the spellcheck file.
#
set addRatings(overwrite) 0
set addRatings(filter) 0

proc allocateRatings {} {
  if {[catch {sc_name ratings -test 1} result]} {
    tk_messageBox -type ok -icon info -parent . -title "Scid" -message $result
    return
  }
  set w .ardialog
  win::createDialog $w
  wm title $w "Scid"
  ttk::label $w.lab -wraplength 3i -justify left -text $::tr(AllocRatingDescription)
  pack $w.lab -side top -expand 1 -fill x -anchor w
  ttk::labelframe $w.g -text $::tr(AddRatings)
  pack $w.g -side top -fill x -anchor w -pady 10
  ttk::radiobutton $w.g.all -variable addRatings(filter) \
      -value 0 -text $::tr(SelectAllGames)
  ttk::radiobutton $w.g.filter -variable addRatings(filter) \
      -value 1 -text $::tr(SelectFilterGames)
  pack $w.g $w.g.all $w.g.filter -side top -anchor w
  ttk::checkbutton $w.r -text $::tr(RatingOverride) \
          -variable addRatings(overwrite) -onvalue 1 -offvalue 0
  pack $w.r -side top -anchor w -fill x -pady "5 0"
  pack [ttk::frame $w.b] -side top -fill x
  ttk::button $w.b.ok -text "OK" \
      -command "catch {grab release $w}; destroy $w; doAllocateRatings"
  ttk::button $w.b.cancel -text $::tr(Cancel) \
      -command "catch {grab release $w}; destroy $w"
  packdlgbuttons $w.b.cancel $w.b.ok
  catch {grab $w}
  focus $w.b.ok
}

proc doAllocateRatings {} {
  global addRatings
  if {[catch {sc_name ratings -test 1} result]} {
    tk_messageBox -type ok -icon info -parent . -title "Scid" -message $result
    return
  }
  progressWindow "Scid" "Adding Elo ratings..." $::tr(Cancel)
  set err [catch {sc_name ratings -change $addRatings(overwrite) -filter $addRatings(filter)} result]
  closeProgressWindow
  if {$err} {
    ERROR::MessageBox
  } else {
    set r [::utils::thousands [lindex $result 0]]
    set g [::utils::thousands [lindex $result 1]]
    tk_messageBox -type ok -icon info -parent . \
        -title "Scid" -message [subst $::tr(AddedRatings)]
  }
  ::notify::DatabaseModified $::curr_db
}


# stripTags:
#   Strip unwanted PGN tags from the current database.

array set stripTagCount {}

proc stripTags {} {
  global stripTagChoice stripTagCount
  set w .striptags
  if {[winfo exists $w]} {
    raise $w
    return
  }
  set stripTagList {}
  
  # Find extra PGN tags:
  progressWindow "Scid" "Searching for extra PGN tags..." $::tr(Cancel)
  set err [catch {sc_base taglist $::curr_db} result]
  closeProgressWindow
  if {$err} {
    ERROR::MessageBox
    return
  }
  
  # Make list of extra tags and their frequency:
  array unset stripTagCount
  set nTags 0
  foreach {tag count} $result {
    set stripTagCount($tag) $count
    incr nTags
  }
  
  if {$nTags == 0} {
    tk_messageBox -title "Scid" -icon info -type ok \
        -message "No extra tags were found."
    return
  }
  
  win::createDialog $w
  wm title $w "Scid: $::tr(StripTags)"
  ttk::label $w.title -text "Extra PGN tags:" -font font_Bold -anchor w
  pack $w.title -side top -fill x -anchor center
  pack [ttk::frame $w.f] -side top -fill x
  pack [ttk::frame $w.b] -side bottom -fill x
  
  set row 0
  unset -nocomplain ::stripTagChoice
  foreach tag [lsort [array names stripTagCount]] {
    set ::stripTagChoice($tag) 0
    ttk::checkbutton $w.f.t$tag -text "$tag" -variable stripTagChoice($tag) -command "
      foreach {tag selected} \[array get ::stripTagChoice\] {
        if {\$selected} {
          $w.b.strip configure -state normal
          return
        }
      }
      $w.b.strip configure -state disabled
    "
    ttk::label $w.f.c$tag -text "  [::utils::thousands $stripTagCount($tag)]"
    grid $w.f.t$tag -row $row -column 0 -sticky w
    grid $w.f.c$tag -row $row -column 1 -sticky e
    incr row
  }
  ttk::button $w.b.strip -text $::tr(StripTag...) -state disabled -command "doStripTags $w"
  ttk::button $w.b.cancel -text $::tr(Cancel) \
      -command "catch {grab release $w}; destroy $w"
  packdlgbuttons $w.b.cancel $w.b.strip
  wm resizable $w 0 0
  update
  catch {grab $w}
}

proc doStripTags {topwin} {
  set msg "Do you really want to remove all occurrences of the PGN tags:\n"
  set tags {}
  foreach {tag selected} [array get ::stripTagChoice] {
    if {$selected} {
      append msg "  $tag\n"
      lappend tags $tag
    }
  }
  append msg "from this database?"
  set result [tk_messageBox -title "Scid" -parent $topwin \
      -icon question -type yesno -message $msg]
  if {$result == "no"} {
    return
  }
  destroy $topwin
  progressWindow "Scid" "Removing PGN tags..." $::tr(Stop)
  set err [catch {sc_base strip $::curr_db {*}$tags} result]
  closeProgressWindow
  if {$err && $::errorCode != $::ERROR::UserCancel} {
    ERROR::MessageBox
    if {$result == 0} {
      return
    }
  }
  set result "Modified $result games."
  append result "\n\n"
  append result "To save space and maintain database efficiency, it is a "
  append result "good idea to compact the game file after removing tags."
  tk_messageBox -title "Scid" -type ok -icon info -message $result
  ::notify::GameChanged
  ::notify::DatabaseModified $::curr_db
}

# cleanerWin:
#   Open a dialog so the user can choose several maintenance tasks
#   in one action.

set cleaner(players) 1
set cleaner(events) 1
set cleaner(sites) 1
set cleaner(rounds) 1
set cleaner(eco) 1
set cleaner(elo) 1
set cleaner(twins) 1
set cleaner(cgames) 0

proc cleanerWin {} {
  set w .mtoolWin
  if {[winfo exists $w]} { return }
  
  win::createDialog $w
  wm title $w "Scid: $::tr(Cleaner)"
  bind $w <F1> {helpWindow Maintenance Cleaner}
  pack [ttk::frame $w.f]
  
  pack [ttk::frame $w.f.help] -side top -fill x
  ttk::label $w.f.help.text -wraplength 3i -text [string trim $::tr(CleanerHelp)]
  pack $w.f.help.text -side left -fill x -expand yes
  
  pack [ttk::frame $w.f.f] -side top -pady "10 0"

  set spelltext $::tr(Spellchecking)
  foreach i {players events sites rounds eco elo twins cgames} \
      j {Players Events Sites Rounds ReclassifyGames AddEloRatings DeleteTwins CompactDatabase } {
    # only use pretext spellcheck for the first four values
    if { $i eq "eco" } { set spelltext "" }
    ttk::checkbutton $w.f.f.$i -variable cleaner($i) -offvalue 0 -onvalue 1 -text "$spelltext $::tr($j)"
  }
  set row 0
  foreach i {players events sites rounds eco elo twins cgames} {
    grid $w.f.f.$i -row $row -column 0 -sticky w
    incr row
  }
  
  pack [ttk::frame $w.f.b] -side bottom -fill x
  ttk::button $w.f.b.ok -text "OK" -command "catch {grab release $w}; destroy $w; doCleaner"
  ttk::button $w.f.b.cancel -text $::tr(Cancel) -command "catch {grab release $w}; destroy $w"
  packdlgbuttons $w.f.b.cancel $w.f.b.ok
  wm resizable $w 0 0
  update
  catch {grab $w}
}

# Maximum nr of corrections to be scanned
# Set to zero to find them all
# Set to some positive number to limit
#
set cleaner_maxSpellCorrections 0


proc doCleaner {} {
  global cleaner twinSettings
  global cleaner_maxSpellCorrections
  
  set dbase [sc_base current]

  set answer [tk_messageBox -type yesno -icon question \
      -title $::tr(TwinCriteriaConfirm) \
      -message [string trim $::tr(CleanerConfirm)]]
  if {$answer == "no"} { return }
  
  set w .mtoolStatus
  if {! [winfo exists $w]} {
    win::createDialog $w
    wm title $w "Scid: $::tr(Cleaner)"
    pack [ttk::frame $w.f]
    pack [ttk::frame $w.f.b] -side bottom -fill x -expand yes
    pack [ttk::frame $w.f.t] -side top -fill both -expand yes
    text $w.f.t.text -width 60 -height 10 -wrap none -setgrid 1 \
        -cursor top_left_arrow -yscrollcommand "$w.f.t.ybar set"
    ttk::scrollbar $w.f.t.ybar -orient vertical -command "$w.f.t.text yview" -takefocus 0
    pack $w.f.t.ybar -side right -fill y
    pack $w.f.t.text -side left -fill both -expand yes
    ttk::button $w.f.b.close -text $::tr(Close) -command "catch {grab release $w}; destroy $w"
    packdlgbuttons $w.f.b.close
    wm minsize $w 20 5
  }
  
  busyCursor .
  catch {grab $w}
  set t $w.f.t.text
  $t delete 1.0 end
  $t insert end "$::tr(Cleaner)."
  $t insert end "  $::tr(Database): [file tail [sc_base filename $dbase]]\n"
  
  $w.f.b.close configure -state disabled
  
  set count 1
  
  foreach nameType {Player Event Site Round} {
    set names $nameType
    append names "s"
    set tag [string tolower $names]
    if {$cleaner($tag)} {
      mtoolAdd $t "$count: $::tr(Spellchecking): $::tr($names)..."
      incr count
      set err [catch {sc_name spellcheck -max $cleaner_maxSpellCorrections $nameType} corrections]
      if {! $err} {
        update
        set err [catch {sc_name correct $nameType $corrections} result]
      }
      if {$err} {
        set msg [ERROR::getErrorMsg]
      } else {
        set msg "Number of games corrected: [lindex $result 2]\n"
      }
      $t insert end "   $msg\n"
      $t see end
    }
  }
  
  if {$cleaner(eco)} {
    mtoolAdd $t "$count: $::tr(ReclassifyGames)..."
    incr count
    catch {sc_eco base $::classifyOption(AllGames) \
          $::classifyOption(ExtendedCodes)} result
    $t insert end "   ECO classified $result games\n"
    $t see end
  }
  
  if {$cleaner(elo)} {
    mtoolAdd $t "$count: $::tr(AddEloRatings)..."
    incr count
    if {[catch {sc_name ratings} result]} {
      $t insert end "   $result\n"
    } else {
      set r [::utils::thousands [lindex $result 0]]
      set g [::utils::thousands [lindex $result 1]]
      $t insert end "   Scid added $r Elo ratings in $g games.\n"
    }
  }
  
  if {$cleaner(twins)} {
    mtoolAdd $t "$count: $::tr(DeleteTwins)..."
    incr count
    if {$twinSettings(undelete) == "Yes"} {
      if {[catch {sc_base gameflag [sc_base current] all unset del}]} {
        $t insert end "   [ERROR::getErrorMsg]\n"
      }
    }
    if {[catch {sc_base duplicates [sc_base current] \
            -colors $twinSettings(colors) \
            -event $twinSettings(event) -site $twinSettings(site) \
            -round $twinSettings(round) -year $twinSettings(year) \
            -month $twinSettings(month) -day $twinSettings(day) \
            -result $twinSettings(result) -eco $twinSettings(eco) \
            -moves $twinSettings(moves) -players $twinSettings(players) \
            -setfilter $twinSettings(setfilter) \
            -usefilter $twinSettings(usefilter) \
            -comments $twinSettings(comments) \
            -variations $twinSettings(variations) \
            -delete $twinSettings(delete)} result]} {
      set message $result
    } else {
      set message "Scid found $result twin games"
      if {$result > 0} {append message " and set their delete flags"}
    }
    $t insert end "   $message.\n"
  }
  
  if {$cleaner(cgames)} {
    mtoolAdd $t "$count: $::tr(CompactDatabase)..."
    incr count
    if {[catch {sc_base compact $dbase stats} stats]} {
      $t insert end "   Error: unable to compacwt the database.\n"
    } else {
      if {[lindex $stats 0] == 0 && [lindex $stats 1] == 0 && \
          [lindex $stats 2] == 0 && [lindex $stats 3] == 0} {
        $t insert end "   Database already compacted.\n"
      } else {
        if {[catch {sc_base compact $dbase} result]} {
          $t insert end "   Error: unable to compact the database.\n"
        } else {
          $t insert end "   Done.\n"
        }
      }
    }
    $t see end
  }
  
  mtoolAdd $t "Done."
  ::notify::GameChanged
  ::notify::DatabaseModified $::curr_db
  $w.f.b.close configure -state normal
  catch {grab release $w}
  unbusyCursor .
}

proc mtoolAdd {tw title} {
  set time [clock format [clock seconds] -format "%H:%M:%S"]
  $tw insert end "\n\[$time\]\n"
  if {$title != ""} { $tw insert end "$title\n" }
  $tw see end
  update
}

