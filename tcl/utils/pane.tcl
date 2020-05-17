
# The following paned window code is loosely based on code from the book:
#
#  Effective Tcl/Tk Programming
#     Mark Harrison, DSC Communications Corp.
#     Michael McLennan, Bell Labs Innovations for Lucent Technologies
#     Addison-Wesley Professional Computing Series
#  Copyright (c) 1996-1997  Lucent Technologies Inc. and Mark Harrison
#
# Many modifications and improvements for use in Scid have been made,
# including namespacing the code.

namespace eval ::utils::pane {}

array set ::utils::pane::_data {}

# Create
#
#   Create a paned window.
#
proc ::utils::pane::Create {win pane1 pane2 width height {ratio 0.5} {orient vert}} {
  variable _data
  set _data($win,1) $pane1
  set _data($win,2) $pane2
  set _data($win,drag) 1
  set vertical 1
  if {[string index $orient 0] == "h"} { set vertical 0 }
  set _data($win,vertical) $vertical
  # Default minimum size of each frame is 10%:
  set _data($win,min) 0.1
  set _data($win,max) 0.9

  ttk::frame $win -width $width -height $height
  ttk::frame $win.$pane1
  ttk::frame $win.$pane2
  if {$vertical} {
    place $win.$pane1 -relx 0.5 -rely 0 -anchor n -relwidth 1.0 -relheight 0.5
    place $win.$pane2 -relx 0.5 -rely 1 -anchor s -relwidth 1.0 -relheight 0.5

    ttk::frame $win.pane_sash -height 2 -borderwidth 2 -relief groove \
      -cursor sb_v_double_arrow ;# -background black
    place $win.pane_sash -relx 0.5 -rely 0.5 -relwidth 1.0 -anchor c

    ttk::frame $win.pane_grip -width 20 -height 7 -borderwidth 1 -relief solid \
      -cursor sb_v_double_arrow
    place $win.pane_grip -relx 0.95 -rely 0.5 -anchor c
  } else {
    place $win.$pane1 -relx 0 -rely 0.5 -anchor w -relwidth 0.5 -relheight 1.0
    place $win.$pane2 -relx 1 -rely 0.5 -anchor e -relwidth 0.5 -relheight 1.0

    ttk::frame $win.pane_sash -width 1 -borderwidth 1 -relief flat \
      -cursor sb_h_double_arrow
    place $win.pane_sash -relx 0.5 -rely 0.5 -relheight 1.0 -anchor c

    ttk::frame $win.pane_grip -height 20 -width 7 -borderwidth 1 -relief solid \
      -cursor sb_h_double_arrow
    place $win.pane_grip -relx 0.5 -rely 0.95 -anchor c
  }

  #bind $win.pane_grip <Enter>           "::utils::pane::Enter $win"
  #bind $win.pane_grip <Leave>           "::utils::pane::Leave $win"
  #bind $win.pane_sash <Enter>           "::utils::pane::Enter $win"
  #bind $win.pane_sash <Leave>           "::utils::pane::Leave $win"

  if {$vertical} { set c "%Y" } else { set c "%X" }
  bind $win.pane_grip <ButtonPress-1>   "::utils::pane::Grab $win"
  bind $win.pane_grip <B1-Motion>       "::utils::pane::Drag $win $c"
  bind $win.pane_grip <ButtonRelease-1> "::utils::pane::Drop $win $c"
  bind $win.pane_sash <ButtonPress-1>   "::utils::pane::Grab $win"
  bind $win.pane_sash <B1-Motion>       "::utils::pane::Drag $win $c"
  bind $win.pane_sash <ButtonRelease-1> "::utils::pane::Drop $win $c"

  ::utils::pane::Divide $win $ratio
  return $win
}

proc ::utils::pane::SetDrag {win bool} {
  set ::utils::pane::_data($win,drag) $bool
}

proc ::utils::pane::SetRange {win min max} {
  set ::utils::pane::_data($win,min) $min
  set ::utils::pane::_data($win,max) $max
}

proc ::utils::pane::Enter {win} {
#  $win.pane_sash configure -background yellow
#  $win.pane_grip configure -background yellow
}

proc ::utils::pane::Leave {win} {
#  $win.pane_sash configure -background black
#  $win.pane_grip configure -background black
}

proc ::utils::pane::Grab {win} {
#  $win.pane_sash configure -background red
#  $win.pane_grip configure -background red
}

proc ::utils::pane::Drag {win y} {
  variable _data
  set vertical $_data($win,vertical)
  if {$vertical} {
    set realY [expr {$y-[winfo rooty $win]}]
    set Ymax  [winfo height $win]
  } else {
    set realY [expr {$y-[winfo rootx $win]}]
    set Ymax  [winfo width $win]
  }
  set frac [expr {double($realY) / $Ymax}]
  if {$frac < $_data($win,min)} {set frac $_data($win,min)}
  if {$frac > $_data($win,max)} {set frac $_data($win,max)}

  if {$_data($win,drag)} {
    ::utils::pane::Divide $win $frac
  } else {
    if {$vertical} {
      place $win.pane_sash -rely $frac
      place $win.pane_grip -rely $frac
    } else {
      place $win.pane_sash -relx $frac
      place $win.pane_grip -relx $frac
    }
  }
  return $frac
}

proc ::utils::pane::Drop {win y} {
  set frac [::utils::pane::Drag $win $y]
  ::utils::pane::Divide $win $frac
#  $win.pane_sash configure -background black
#  $win.pane_grip configure -background gray
}

proc ::utils::pane::Divide {win frac} {
  if {$::utils::pane::_data($win,vertical)} {
    place $win.pane_sash -rely $frac
    place $win.pane_grip -rely $frac
    place $win.$::utils::pane::_data($win,1) -relheight $frac
    place $win.$::utils::pane::_data($win,2) -relheight [expr {1.0 - $frac}]
  } else {
    place $win.pane_sash -relx $frac
    place $win.pane_grip -relx $frac
    place $win.$::utils::pane::_data($win,1) -relwidth $frac
    place $win.$::utils::pane::_data($win,2) -relwidth [expr {1.0 - $frac}]
  }
}
