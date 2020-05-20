
####################
# Game Browser window

namespace eval ::gbrowser {}

proc ::gbrowser::new {base gnum {ply -1}} {
  if {! [info exists ::gbrowser::size] } { set ::gbrowser::size 65 }
  options.save ::gbrowser::size

  set n 0
  while {[winfo exists .gb$n]} { incr n }
  set w .gb$n
  toplevel $w
  if {$base < 1} { set base [sc_base current] }
  if {$gnum < 1} { set game [sc_game number] }
  set filename [file tail [sc_base filename $base]]
  wm title $w "Scid: $::tr(BrowseGame) ($filename: $gnum)"
  lassign [sc_base gamesummary $base $gnum] header ::gbrowser::boards($n) moves
  
  pack [ttk::frame $w.b] -side bottom -fill x
  ::board::new $w.bd $::gbrowser::size
  pack $w.bd -side left -padx 4 -pady 4
  
  autoscrollText y $w.t $w.t.text Treeview
  $w.t.text configure -wrap word -state normal \
      -width 45 -height 12 -font font_Small -setgrid 1
  pack $w.t -side right -fill both -expand yes
  
  set t $w.t.text
  event generate $t <ButtonRelease-1>
  $t tag configure next -foreground yellow -background darkBlue
  $t insert end "$header" header
  $t insert end "\n\n"
  set m 0
  
  foreach i $moves {
    set moveTag m$m
    $t insert end [::trans $i] $moveTag
    $t insert end " "
    $t tag bind $moveTag <ButtonRelease-1> "::gbrowser::update $n $m"
    $t tag bind $moveTag <Any-Enter> \
        "$t tag configure $moveTag -foreground red
    $t configure -cursor hand2"
    $t tag bind $moveTag <Any-Leave> \
        "$t tag configure $moveTag -foreground {}
    $t configure -cursor {}"
    incr m
  }
  bind $w <F1> {helpWindow GameList Browsing}
  bind $w <Escape> "destroy $w"
  bind $w <Home> "::gbrowser::update $n start"
  bind $w <End> "::gbrowser::update $n end"
  bind $w <Left> "::gbrowser::update $n -1"
  bind $w <Right> "::gbrowser::update $n +1"
  bind $w <Up> "::gbrowser::update $n -10"
  bind $w <Down> "::gbrowser::update $n +10"
  bind $w <minus> {set ::gbrowser::size [::board::resize %W.bd -1]}
  bind $w <Control-Shift-Left> {set ::gbrowser::size [::board::resize %W.bd -1]}
  bind $w <plus> {set ::gbrowser::size [::board::resize %W.bd +1]}
  bind $w <Control-Shift-Right> {set ::gbrowser::size [::board::resize %W.bd +1]}
  bindMouseWheel $w "::gbrowser::mousewheelHandler $n"
  
  ttk::button $w.b.start -image tb_start -command "::gbrowser::update $n start"
  ttk::button $w.b.back -image tb_prev -command "::gbrowser::update $n -1"
  ttk::button $w.b.forward -image tb_next -command "::gbrowser::update $n +1"
  ttk::button $w.b.end -image tb_end -command "::gbrowser::update $n end"
  ttk::frame $w.b.gap -width 3
  ttk::button $w.b.autoplay -image tb_play -command "::gbrowser::autoplay $n"
  ttk::frame $w.b.gap2 -width 3
  ttk::button $w.b.flip -image tb_flip -command "::gbrowser::flip $n"
  
  pack $w.b.start $w.b.back $w.b.forward $w.b.end $w.b.gap \
      $w.b.autoplay $w.b.gap2 $w.b.flip -side left -padx 3 -pady 1
  
  set ::gbrowser::autoplay($n) 0
  
  if {$gnum > 0} {
    ttk::button $w.b.load -textvar ::tr(LoadGame) -command "sc_base switch $base; ::game::Load $gnum"
    ttk::button $w.b.merge -textvar ::tr(MergeGame) -command "mergeGame $base $gnum"
  }
  ttk::button $w.b.close -textvar ::tr(Close) -command "destroy $w"
  pack $w.b.close -side right -padx 1 -pady 1
  if {$gnum > 0} {
    pack $w.b.merge $w.b.load -side right -padx 1 -pady 1
  }
  
  wm resizable $w 1 0
  if {$ply < 0} {
    set ply 0
    #TODO: search if game gnum match the current board pos
  }
  ::gbrowser::update $n $ply
  
  if { [::board::isFlipped .main.board] } {
    ::gbrowser::flip $n
  }
}

proc gbrowser::mousewheelHandler {n direction} {
  if {$direction < 0} {
    ::gbrowser::update $n -1
  } else {
    ::gbrowser::update $n +1
  }
}

proc ::gbrowser::flip {n} {
  ::board::flip .gb$n.bd
}

proc ::gbrowser::update {n ply} {
  set w .gb$n
  if {! [winfo exists $w]} { return }
  set oldply 0
  if {[info exists ::gbrowser::ply($n)]} { set oldply $::gbrowser::ply($n) }
  if {$ply == "forward"} { set ply [expr {$oldply + 1} ] }
  if {$ply == "back"} { set ply [expr {$oldply - 1} ] }
  if {$ply == "start"} { set ply 0 }
  if {$ply == "end"} { set ply 9999 }
  if {[string index $ply 0] == "-"  ||  [string index $ply 0] == "+"} {
    set ply [expr {$oldply + $ply} ]
  }
  if {$ply < 0} { set ply 0 }
  set max [expr {[llength $::gbrowser::boards($n)] - 1} ]
  if {$ply > $max} { set ply $max }
  set ::gbrowser::ply($n) $ply
  ::board::update $w.bd [lindex $::gbrowser::boards($n) $ply] 1
  
  set t $w.t.text
  $t configure -state normal
  set moveRange [$t tag nextrange m$ply 1.0]
  $t tag remove next 1.0 end
  set moveRange [$t tag nextrange m$ply 1.0]
  if {[llength $moveRange] == 2} {
    $t tag add next [lindex $moveRange 0] [lindex $moveRange 1]
    $t see [lindex $moveRange 0]
  }
  $t configure -state disabled
  
  if {$::gbrowser::autoplay($n)} {
    if {$ply >= $max} {
      ::gbrowser::autoplay $n
    } else {
      after cancel "::gbrowser::update $n +1"
      after $::autoplayDelay "::gbrowser::update $n +1"
    }
  }
}

proc ::gbrowser::autoplay {n} {
  if {$::gbrowser::autoplay($n)} {
    set ::gbrowser::autoplay($n) 0
    .gb$n.b.autoplay configure -image tb_play
    return
  } else {
    set ::gbrowser::autoplay($n) 1
    .gb$n.b.autoplay configure -image tb_stop
    ::gbrowser::update $n +1
  }
}
