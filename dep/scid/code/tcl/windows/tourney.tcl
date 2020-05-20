
####################
# Tournament window

namespace eval ::tourney {}

foreach {n v} {start 0000.00.00 end 2047.12.31 minPlayers 2 maxPlayers 999 \
                 minGames 1 maxGames 9999 minElo 0 maxElo 4000 sort Date \
                 country "" site "" event "" player "" size 50} {
  set ::tourney::$n $v
}

trace variable ::tourney::start w ::utils::validate::Date
trace variable ::tourney::end w ::utils::validate::Date
foreach {n v} {minPlayers 999 maxPlayers 999 minGames 9999 maxGames 9999 \
                 minElo [sc_info limit elo] maxElo [sc_info limit elo]} {
  trace variable ::tourney::$n w [list ::utils::validate::Integer $v 0]
}

set tourneyWin 0

proc ::tourney::toggle {} {
  set w .tourney
  if {[winfo exists $w]} {
    destroy $w
  } else {
    ::tourney::Open
  }
}

proc ::tourney::Open {} {
  global tourneyWin
  set w .tourney
  if {[winfo exists $w]} { return }
  set tourneyWin 1

  if {! [info exists ::tourney::_defaults]} { ::tourney::defaults }

  ::createToplevel $w
#  $w configure -background [ttk::style lookup . -background]
  ::setTitle $w "Scid: [tr WindowsTmt]"
  wm title $w "Scid: [tr WindowsTmt]"

  bind $w <F1> {helpWindow Tmt}
  bind $w <Return> ::tourney::refresh
  bind $w <Destroy> { set tourneyWin 0 }
  bind $w <Up> "$w.t.text yview scroll -1 units"
  bind $w <Down> "$w.t.text yview scroll 1 units"
  bind $w <Prior> "$w.t.text yview scroll -1 pages"
  bind $w <Next> "$w.t.text yview scroll 1 pages"
  bind $w <Key-Home> "$w.t.text yview moveto 0"
  bind $w <Key-End> "$w.t.text yview moveto 0.99"

  foreach i {o1 o2 o3 b} {ttk::frame $w.$i}
  autoscrollText both $w.t $w.t.text Treeview
  $w.t.text configure -width 75 -height 22 -font font_Small -wrap none  -state normal
  set xwidth [font measure [$w.t.text cget -font] "0"]
  set tablist {}
  foreach {tab justify} {3 r 4 l 18 r 23 r 30 r 32 l 55 l} {
    set tabwidth [expr {$xwidth * $tab} ]
    lappend tablist $tabwidth $justify
  }
  $w.t.text configure -tabs $tablist
  $w.t.text tag configure date -foreground firebrick3
  $w.t.text tag configure np -foreground DodgerBlue3
  $w.t.text tag configure elo -foreground Green
  $w.t.text tag configure best -foreground steelBlue
  $w.t.text tag configure event -foreground firebrick3
  $w.t.text tag configure title -font font_SmallBold -background lightSteelBlue

  set font font_Small
  set fbold font_SmallBold
  set f $w.o1
  ttk::label $f.from -text "[tr TmtSortDate]:" -font $fbold
  ttk::entry $f.efrom -textvariable ::tourney::start -width 10 -font $font
  bindFocusColors $f.efrom
  ttk::label $f.to -text "-" -font $font
  ttk::entry $f.eto -textvariable ::tourney::end -width 10 -font $font
  bindFocusColors $f.eto
  pack $f.from $f.efrom $f.to $f.eto -side left

  ttk::label $f.cn -text "  $::tr(Country):" -font $fbold
  ttk::combobox $f.ecn -width 4 -textvar ::tourney::country -values {{} AUT CZE DEN ENG ESP FRA GER GRE HUN ITA NED POL RUS \
        SCG SUI SWE USA YUG}
  
  bindFocusColors $f.ecn
  pack $f.cn $f.ecn -side left

  ttk::label $f.size -text $::tr(TmtLimit:) -font $fbold
  ttk::combobox $f.esize -width 4 -justify right -textvar ::tourney::size -values {10 20 50 100 200 500 1000}
  trace variable ::tourney::size w {::utils::validate::Integer 1000 0}
  bindFocusColors $f.esize

  pack $f.esize $f.size -side right -pady "2 0"

  set f $w.o2
  ttk::label $f.players -text "[tr TmtSortPlayers]:" -font $fbold
  ttk::entry $f.pmin -textvariable ::tourney::minPlayers \
    -width 3 -justify right -font $font
  bindFocusColors $f.pmin
  ttk::label $f.pto -text "-"
  ttk::entry $f.pmax -textvariable ::tourney::maxPlayers \
    -width 3 -justify right -font $font
  bindFocusColors $f.pmax
  pack $f.players $f.pmin $f.pto $f.pmax -side left -pady "2 0"

  ttk::label $f.games -text "   [tr TmtSortGames]:" -font $fbold
  ttk::entry $f.gmin -textvariable ::tourney::minGames \
    -width 4 -justify right -font $font
  bindFocusColors $f.gmin
  ttk::label $f.gto -text "-" -font $font
  ttk::entry $f.gmax -textvariable ::tourney::maxGames \
    -width 4 -justify right -font $font
  bindFocusColors $f.gmax
  pack $f.games $f.gmin $f.gto $f.gmax -side left -pady "2 0"
  ttk::label $f.elolab -text "$::tr(TmtMeanElo):" -font $fbold
  ttk::entry $f.elomin -textvariable ::tourney::minElo \
    -width 5 -justify right -font $font
  bindFocusColors $f.elomin
  ttk::label $f.eto -text "-" -font $font
  ttk::entry $f.elomax -textvariable ::tourney::maxElo \
    -width 5 -justify right -font $font
  bindFocusColors $f.elomax
  pack $f.elomax $f.eto $f.elomin $f.elolab -side right

  set f $w.o3
  ttk::label $f.sitelab -text "$::tr(Site):" -font $fbold
  ttk::combobox $f.site -textvariable ::tourney::site -width 12
  ::utils::history::SetCombobox ::tourney::site $f.site
  bindFocusColors $f.site
  pack $f.sitelab $f.site -side left

  ttk::label $f.eventlab -text "   $::tr(Event):" -font $fbold
  ttk::combobox $f.event -textvariable ::tourney::event -width 12
  ::utils::history::SetCombobox ::tourney::event $f.event
  bindFocusColors $f.event
  pack $f.eventlab $f.event -side left -pady "2 0"

  ttk::label $f.playerlab -text "$::tr(Player):" -font $fbold
  ttk::combobox $f.player -textvariable ::tourney::player -width 12
  ::utils::history::SetCombobox ::tourney::player $f.player
  bindFocusColors $f.player
  pack $f.player $f.playerlab -side right
  focus $f.site

  dialogbutton $w.b.defaults -textvar ::tr(Defaults) -command ::tourney::defaults
  dialogbutton $w.b.help -textvar ::tr(Help) -command {helpWindow Tmt}
  dialogbutton $w.b.update -textvar ::tr(Search) -command ::tourney::refresh
  packbuttons right $w.b.update $w.b.help
  packbuttons left $w.b.defaults
  grid $w.o1 -sticky news
  grid $w.o2 -sticky news
  grid $w.o3 -sticky news
  grid $w.b  -sticky news
  grid $w.t  -sticky news
  grid rowconfigure $w 4 -weight 1
  grid columnconfigure $w 0 -weight 1

  ::createToplevelFinalize $w
  ::tourney::refresh
}

proc ::tourney::defaults {} {
  set ::tourney::_defaults 1
  set ::tourney::start ""
  set ::tourney::end ""
  set ::tourney::size 50
  set ::tourney::minPlayers ""
  set ::tourney::maxPlayers ""
  set ::tourney::minGames ""
  set ::tourney::maxGames ""
  set ::tourney::minElo ""
  set ::tourney::maxElo ""
  set ::tourney::country ""
  set ::tourney::site ""
  set ::tourney::event ""
  set ::tourney::player ""
}

proc ::tourney::refresh {{option ""}} {
  set w .tourney
  if {! [winfo exists $w]} { return }

  busyCursor $w
  ::utils::history::AddEntry ::tourney::site $::tourney::site
  ::utils::history::AddEntry ::tourney::event $::tourney::event
  ::utils::history::AddEntry ::tourney::player $::tourney::player

  set t $w.t.text
  $t configure -state normal
  $t delete 1.0 end
  ::tourney::check
  update

  set ::curr_db [sc_base current]
  set filter [sc_filter new $::curr_db]
  set start $::tourney::start
  set end $::tourney::end
  if { $start eq "" } { set start "1800.01.01" }
  if { $end eq "" } { set year [::utils::date::today year]; set start "$year.12.31" }
  sc_filter search $::curr_db $filter header \
      -filter RESET \
      -date [list $start $end] \
      -site $::tourney::site \
      -sitecountry [string toupper $::tourney::country] \
      -event $::tourney::event
  set err [catch {sc_base tournaments $::curr_db $filter {*}[::tourney::getSearchOptions]  } tlist]
  sc_filter release $::curr_db $filter
  unbusyCursor .

  if {$err} {
      ERROR::MessageBox
      return
  }

  if {[llength $tlist] > 0} {
    $t insert end "\t" title
    foreach i {Date Players Games Elo Site Event} {
      $t tag configure s$i -font font_SmallBold
      $t tag bind s$i <1> "set ::tourney::sort $i; ::tourney::refresh"
      $t tag bind s$i <Any-Enter> "$t tag config s$i -foreground red"
      $t tag bind s$i <Any-Leave> "$t tag config s$i -foreground {}"
      if { $i == "Event" } { set tab ":" } else { set tab "\t" }
      $t insert end $tab title
      $t insert end [tr TmtSort$i] [list s$i title]
    }
    $t insert end "\t" title
    $t tag configure sWinner -font font_SmallBold
    $t insert end [tr TmtSortWinner] [list sWinner title]
    $t insert end "\n"
  } else {
    $t insert end $::tr(TmtNone)
  }

  set hc yellow
  set count 0
  foreach tmt $tlist {
    incr count
    if {$count > $::tourney::size} { break }
    set date [lindex $tmt 0]
    set site [lindex $tmt 1]
    set event [lindex $tmt 2]
    set np [lindex $tmt 3]
    set ng [lindex $tmt 4]
    set elo [lindex $tmt 5]
    set g [lindex $tmt 6]
    set white [::utils::string::Surname [lindex $tmt 7]]
    set welo [lindex $tmt 8]
    set wscore [lindex $tmt 9]
    set black [::utils::string::Surname [lindex $tmt 10]]
    set belo [lindex $tmt 11]
    set bscore [lindex $tmt 12]
    if {$welo > 0} { append white "($welo)" }
    if {$belo > 0} { append black "($belo)" }
    append white " $wscore"
    append black " $bscore"
    set one "1."
    set two "2."
    if {$wscore == $bscore} {
      set one "1="; set two "1="
    }
    set best "$one $white, $two $black, ..."
    if {$np == 2} { set best "$one $white, $two $black" }

    $t tag bind g$count <ButtonPress-1> [list ::tourney::select $g]
    $t tag bind g$count <ButtonPress-$::MB3> [list ::tourney::select $g]
    $t tag bind g$count <Any-Enter> \
      "$t tag configure g$count -background $hc"
    $t tag bind g$count <Any-Leave> \
      "$t tag configure g$count -background {}"
    $t insert end "\n"
    $t insert end "\t$count\t" g$count
    $t insert end $date [list date g$count]
    $t insert end "\t" g$count
    $t insert end $np [list np g$count]
    $t insert end "\t" g$count
    $t insert end $ng [list ng g$count]
    $t insert end "\t" g$count
    $t insert end $elo [list elo g$count]
    $t insert end "\t" g$count
    $t insert end "$site: " [list site g$count]
    $t insert end "$event" [list event g$count]
    $t insert end "\t$best" [list best g$count]
  }
  $t insert end "\n"
  $t configure -state disabled
}

proc ::tourney::check {} {
  set start $::tourney::start
  set end $::tourney::end
  if {[string length $start] == 4} { append start ".??.??" }
  if {[string length $end] == 4} { append end ".12.31" }
  if {[string length $start] == 7} { append start ".??" }
  if {[string length $end] == 7} { append end ".31" }
  set ::tourney::start $start
  set ::tourney::end $end
  set s $::tourney::country
  set s [string toupper [string trim $s]]
  if {[string length $s] > 3} { set s [string range $s 0 2] }
  set ::tourney::country $s
  if {$::tourney::country == "---"} {
    set ::tourney::country ""
  }
  if { $::tourney::maxGames ne "" && $::tourney::minGames > $::tourney::maxGames} {
    set help $::tourney::maxGames
    set ::tourney::maxGames $::tourney::minGames
    set ::tourney::minGames $help
  }
  if {$::tourney::maxElo ne "" && $::tourney::minElo > $::tourney::maxElo} {
    set help $::tourney::maxElo
    set ::tourney::maxElo $::tourney::minElo
    set ::tourney::minElo $help
  }
  if {$::tourney::maxPlayers ne "" && $::tourney::minPlayers > $::tourney::maxPlayers} {
    set help $::tourney::maxPlayers
    set ::tourney::maxPlayers $::tourney::minPlayers
    set ::tourney::minPlayers $help
  }
}

proc ::tourney::select {gnum} {
  if {[catch {::game::Load $gnum} result]} {
    tk_messageBox -type ok -icon info -title "Scid" -message $result
    return
  }
  updateBoard -pgn
  updateTitle
  ::crosstab::Open
}

proc ::tourney::getSearchOptions {} {
    set options [list $::tourney::size]
    if {$::tourney::player ne ""} {
        lappend options "-player" $::tourney::player
    }
    if {$::tourney::sort ne ""} {
        lappend options "-sort" $::tourney::sort
    }
    if {$::tourney::minPlayers ne "" || $::tourney::maxPlayers ne ""} {
        set minp 2
        set maxp 999
        if {$::tourney::minPlayers ne ""} {
            set minp $::tourney::minPlayers
        }
        if {$::tourney::maxPlayers ne ""} {
            set maxp $::tourney::maxPlayers
        }
        lappend options "-n_players" [list $minp $maxp]
    }
    if {$::tourney::minGames ne "" || $::tourney::maxGames ne ""} {
        set ming 1
        set maxg 9999
        if {$::tourney::minGames ne ""} {
            set ming $::tourney::minGames
        }
        if {$::tourney::maxGames ne ""} {
            set maxg $::tourney::maxGames
        }
        lappend options "-n_games" [list $ming $maxg]
    }
    if {$::tourney::minElo ne "" || $::tourney::maxElo ne ""} {
        set mine 0
        set maxe 4000
        if {$::tourney::minElo ne ""} {
            set mine $::tourney::minElo
        }
        if {$::tourney::maxElo ne ""} {
            set maxe $::tourney::maxElo
        }
        lappend options "-avgelo" [list $mine $maxe]
    }
    return $options
}
