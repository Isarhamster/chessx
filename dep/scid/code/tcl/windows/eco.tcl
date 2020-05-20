
####################
# ECO Browser window

set ::windows::eco::code ""
set ::windows::eco::count 0
set ::windows::eco::isOpen 0

proc ::windows::eco::OpenClose {} {
  if {[winfo exists .ecograph]} {
    destroy .ecograph
  } else {
    ::windows::eco::Refresh
  }
}

# ::windows::eco::Refresh
#
#    Updates the ECO Browser window, opening it if necessary.
#    If the ECO code "code" is "x", then the value of the
#    variable ::windows::eco::code is used instead.
#
proc ::windows::eco::Refresh {{code "x"}} {
  set w .ecograph
  set graph $w.pane.graph
  set text $w.pane.text
  if {$code != "x"} { set ::windows::eco::code $code }
  if {! [winfo exists $w]} {
    set ::windows::eco::isOpen 1
    ::createToplevel $w
    wm minsize $w 200 100
    bind $w <Escape> "destroy $w"
    bind $w <F1> {helpWindow ECO}
    bind $w <Destroy> {set ::windows::eco::isOpen 0}
    ttk::entry $w.title -width 15 -font font_Bold
    pack $w.title -side top -fill x
    ttk::frame $w.b
    pack $w.b -side bottom -fill x
    ttk::button $w.b.classify -textvar ::tr(ReclassifyGames) -command classifyAllGames
    dialogbutton $w.b.help -textvar ::tr(Help) -command {helpWindow ECO}
    dialogbutton $w.b.close -textvar ::tr(Close) -command "destroy $w"
    pack $w.b.classify -side left -padx 5 -pady 5
    packbuttons right $w.b.close $w.b.help
    set pane [::utils::pane::Create $w.pane graph text 500 400 0.5]
    ::utils::pane::SetRange $w.pane 0.3 0.7
    ::utils::pane::SetDrag $w.pane 0
    pack $pane -side top -expand true -fill both

    canvas $graph.c -width 500 -height 250 -selectforeground [ttk::style lookup . -foreground]
    applyThemeColor_background $graph.c

    pack $graph.c -side top -fill both -expand yes
    autoscrollText y $text.f $text.text Treeview
    $text.text tag configure bold -font font_Bold
    $text.text tag configure indent -lmargin2 20
    $text.text configure -height 12 -width 75 -wrap word -state normal
    ::htext::init $text.text
    grid $text.f -row 0 -column 0 -sticky nesw
    grid rowconfig $text 0 -weight 1 -minsize 0
    grid columnconfig $text 0 -weight 1 -minsize 0

    foreach i {0 1 2 3 4 5 6 7 8 9 A B C D E a b c d e f g h i j k l m n o p
               q r s t u v w x y z} {
      bind $w <KeyPress-$i> "::windows::eco::KeyPress $i"
    }

    foreach i {Left Delete less BackSpace} {
      bind $w <KeyPress-$i> {::windows::eco::KeyPress "<"}
    }

    bind $w <Home>  {.ecograph.pane.text.text yview moveto 0}
    bind $w <End>   {.ecograph.pane.text.text yview moveto 1.0}
    bind $w <Up>    {.ecograph.pane.text.text yview scroll -1 units}
    bind $w <Down>  {.ecograph.pane.text.text yview scroll 1 units}
    bind $w <Prior> {.ecograph.pane.text.text yview scroll -1 pages}
    bind $w <Next>  {.ecograph.pane.text.text yview scroll 1 pages}

    bind $graph.c <1> { ::windows::eco::Select %x }
    bind $graph.c <$::MB3> { ::windows::eco::KeyPress "<" }

    bind $graph <Configure> {
      ::utils::graph::configure eco -height [expr {[winfo height .ecograph.pane.graph.c] - 50} ]
      ::utils::graph::configure eco -width [expr {[winfo width .ecograph.pane.graph.c] - 60} ]
      ::utils::graph::redraw eco
    }
    bind $w <Configure> {
      ::utils::graph::configure eco -height [expr {[winfo height .ecograph.pane.graph.c] - 50} ]
      ::utils::graph::configure eco -width [expr {[winfo width .ecograph.pane.graph.c] - 60} ]
      ::utils::graph::redraw eco
    }
    ::setTitle $w "Scid: [tr WindowsECO]"
    ::utils::graph::create eco -width 1 -height 1 -xtop 50 -ytop 20 \
      -xmin 0.5 -xtick 1 -ytick 5 -font font_Small -canvas $graph.c

    ::createToplevelFinalize $w
    update
  }

  ::windows::eco::update
}

proc ::windows::eco::update {} {
  set w .ecograph
  set graph $w.pane.graph
  set text $w.pane.text

  set height [expr {[winfo height $graph.c] - 50} ]
  set width [expr {[winfo width $graph.c] - 60} ]

  set code $::windows::eco::code
  # Collect data:
  set len [string length $code]
  set subcodes {}
  if {$len == 0} {
    set subcodes {A B C D E}
  } elseif {$len == 1  ||  $len == 2} {
    set subcodes {0 1 2 3 4 5 6 7 8 9}
  } elseif {$len == 3} {
    set subcodes {a b c d e f g h i j k l m n o p q r s t u v w x y z}
  }

  set xlabels {}
  set count 0
  set data {}
  set maxfreq 1
  set wins {}
  set draws {}

  set ::curr_db [sc_base current]
  foreach i $subcodes {
    set subcode "$code$i"
    set stats [sc_base stats $::curr_db eco $subcode]
    set freq [lindex $stats 0]
    incr count
    lappend data $count
    lappend data $freq
    lappend wins $count
    lappend wins [lindex $stats 1]
    lappend draws $count
    lappend draws [expr {[lindex $stats 1] + [lindex $stats 2] + [lindex $stats 4]} ]
    if {$freq > $maxfreq} {set maxfreq $freq}
    if {$len == 3} {
      set subcode $i
    }
    lappend xlabels [list $count $subcode]
  }
  set hline 5
  if {$maxfreq >    20} { set hline    10 }
  if {$maxfreq >    50} { set hline    25 }
  if {$maxfreq >   100} { set hline    50 }
  if {$maxfreq >   200} { set hline   100 }
  if {$maxfreq >   500} { set hline   250 }
  if {$maxfreq >  1000} { set hline   500 }
  if {$maxfreq >  2000} { set hline  1000 }
  if {$maxfreq >  5000} { set hline  2500 }
  if {$maxfreq > 10000} { set hline  5000 }
  if {$maxfreq > 20000} { set hline 10000 }
  if {$maxfreq > 50000} { set hline 25000 }
  if {$maxfreq > 100000} { set hline 50000 }

  ::utils::graph::create eco -width $width -height $height -xtop 50 -ytop 20 \
    -xmin 0.5 -xtick 1 -ytick $hline -font font_Small -canvas $graph.c
  ::utils::graph::data eco data -color SteelBlue4 -points 0 -lines 0 -bars 1 \
    -barwidth 0.8 -outline black -coords $data
  ::utils::graph::data eco draws -color SteelBlue3 -points 0 -lines 0 -bars 1 \
    -barwidth 0.8 -outline black -coords $draws
  ::utils::graph::data eco wins -color SteelBlue1 -points 0 -lines 0 -bars 1 \
    -barwidth 0.8 -outline black -coords $wins
  ::utils::graph::data eco bounds -points 0 -lines 0 -bars 0 -coords {1 0 1 1}
  ::utils::graph::configure eco -ymin 0 -xmin 0.4 -xmax [expr {$count + 0.6} ] \
    -xlabels $xlabels -hline [list [list gray80 1 each $hline]]
  ::utils::graph::redraw eco
  $text.text configure -state normal
  $text.text delete 1.0 end
  set stats [sc_base stats $::curr_db eco $code]
  if {$len == 0} {
    set section $::tr(ECOAllSections)
  } elseif {$len < 3} {
    set section "$::tr(ECOSection) \"$code\""
  } else {
    set section "$::tr(ECOCode) \"$code\""
  }
  set header "<center><b>$::tr(ECOSummary) $section</b><br>"
  append header "[lindex $stats 0] $::tr(games): +[lindex $stats 1] =[lindex $stats 2] -[lindex $stats 3]  ([lindex $stats 5]%)</center>\n\n"
  ::htext::display $text.text "$header[sc_eco summary $code 1]"
  $text.text configure -state disabled
  $w.title configure -state normal -justify center
  $w.title delete 0 end
  $w.title insert end "$::tr(ECOFrequency) $section"
  $w.title configure -state disabled
  set ::windows::eco::count $count
}

proc ::windows::eco::Select {xc} {
  variable count
  variable code

  set x [::utils::graph::xunmap eco $xc]
  set selection 0
  for {set i 1} {$i <= $count} {incr i} {
    if {$x >= [expr {$i - 0.4} ]  &&  $x <= [expr {$i + 0.4} ]} {
      set selection $i
    }
  }
  if {$selection == 0} { return }
  incr selection -1
  set len [string length $code]
  if {$len == 0} {
    set code [lindex {A B C D E} $selection]
  } elseif {$len == 1  ||  $len == 2} {
    append code $selection
  } elseif {$len == 3} {
    append code [lindex {a b c d e f g h i j k l m n o p q r s t u v w x y z} $selection]
  } else {
    return
  }
  ::windows::eco::Refresh
}

# ::windows::eco::KeyPress
#
#    Handles keyboard events in ECO browser window
#
proc ::windows::eco::KeyPress {key} {
  set code $::windows::eco::code
  set len [string length $code]
  if {$key == "<"} {
    set ::windows::eco::code [string range $code 0 [expr {$len - 2} ]]
    ::windows::eco::Refresh
    return
  }
  if {$key == "top"} {
    set ::windows::eco::code ""
    ::windows::eco::Refresh
    return
  }

  if {$len == 0} {
    set key [string toupper $key]
    switch $key {
      A - B - C - D - E {
        # nothing
      }
      default { set key "" }
    }
  } elseif {$len == 1 || $len == 2} {
    switch $key {
      0 - 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 {
        # nothing
      }
      default { set key "" }
    }
  } elseif {$len == 3} {
    set key [string tolower $key]
    switch $key {
      a - b - c - d - e - f - g - h - i - j - k - l - m - n - o - p - q - r -
      s - t - u - v - w - x - y - z {
        # nothing
      }
      default { set key "" }
    }
  }

  if {$key != ""} {
    set ::windows::eco::code "$code$key"
    ::windows::eco::Refresh
  }
}

