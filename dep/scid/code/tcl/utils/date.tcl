
namespace eval ::utils::date {}

# ::utils::date::today:
#   Returns todays date, in "yyyy.mm.dd" format.
#   The optional parameter "year", "month" or "day" can be used to
#   limit the returned value to just the year, month or day.
#
proc ::utils::date::today {{type all}} {
  set timeNow [clock seconds]
  set year [clock format $timeNow -format "%Y"]
  set month [clock format $timeNow -format "%m"]
  set day [clock format $timeNow -format "%d"]
  switch -- $type {
    "all"   { return [format "%s.%s.%s" $year $month $day] }
    "year"  { return $year }
    "month" { return $month }
    "day"   { return $day }
    default { error "Unrecognised parameter: $type" }
  }
}

# ::utils::date::chooser
#
#   Produce a date-selection dialog box.
#   Originally based on code from Effective Tcl/Tk Programming by
#   Mark Harrison, but with lots of changes and improvements.
#
proc ::utils::date::chooser {{date "now"}} {
  set time [clock seconds]
  if {$date != "now"} {
    catch {set time [clock scan $date]}
  }
  set ::utils::date::_time $time
  set ::utils::date::_selected [clock format $time -format "%Y-%m-%d"]

  set win .dateChooser
  toplevel $win
  canvas $win.cal -width 300 -height 220
  ::applyThemeColor_background $win.cal
  pack [ttk::frame $win.b] -side bottom -fill x
  ttk::button $win.b.ok -text "OK" -command "destroy $win"
  ttk::button $win.b.cancel -text $::tr(Cancel) -command "
    set ::utils::date::_selected {}
    destroy $win"
  pack $win.b.cancel $win.b.ok -side right -padx 5 -pady 5
  pack $win.cal -side top -expand yes -fill both

  ttk::button $win.cal.prevY -image tb_start -command "::utils::date::_month $win -12"
  ttk::button $win.cal.prev -image tb_prev -command "::utils::date::_month $win -1"
  ttk::button $win.cal.next -image tb_next -command "::utils::date::_month $win +1"
  ttk::button $win.cal.nextY -image tb_end -command "::utils::date::_month $win +12"
  bind $win.cal <Configure> "::utils::date::_redraw $win"
  bind $win.cal <Double-Button-1> "destroy $win"
  bind $win <Escape> "$win.b.cancel invoke"
  bind $win <Return> "$win.b.ok invoke"
  bind $win <Prior> "$win.cal.prev invoke"
  bind $win <Next> "$win.cal.next invoke"
  bind $win <Shift-Prior> "$win.cal.prevY invoke"
  bind $win <Shift-Next> "$win.cal.nextY invoke"
  bind $win <Up> "::utils::date::_day $win -7"
  bind $win <Down> "::utils::date::_day $win +7"
  bind $win <Left> "::utils::date::_day $win -1"
  bind $win <Right> "::utils::date::_day $win +1"

  wm minsize $win 250 200
  wm title $win "Scid: Choose Date"
  focus $win
  grab $win
  tkwait window $win
  if {$::utils::date::_selected == ""} { return {} }
  set time [clock scan $::utils::date::_selected]
  return [list \
          [clock format $time -format "%Y"] \
          [clock format $time -format "%m"] \
          [clock format $time -format "%d"] \
         ]
}

proc ::utils::date::_day {win delta} {
  set unit "day"
  if {$delta < 0} {set unit "day ago"}
  set time [clock scan "[expr abs($delta)] $unit" -base $::utils::date::_time]
  set day [string trimleft [clock format $time -format "%d"] 0]
  set month [string trimleft [clock format $time -format "%m"] 0]
  set year [clock format $time -format "%Y"]
  ::utils::date::_select $win "$year-$month-$day"
}

proc ::utils::date::_month {win delta} {
  set dir [expr {($delta > 0) ? 1 : -1} ]
  set day [string trimleft [clock format $::utils::date::_time -format "%d"] 0]
  set month [string trimleft [clock format $::utils::date::_time -format "%m"] 0]
  set year [clock format $::utils::date::_time -format "%Y"]

  for {set i 0} {$i < abs($delta)} {incr i} {
    incr month $dir
    if {$month < 1} {
      set month 12
      incr year -1
    } elseif {$month > 12} {
      set month 1
      incr year 1
    }
  }
  if {[catch {::date::_select $win "$year-$month-$day"}]} {
    ::utils::date::_select $win "$year-$month-28"
  }
}

proc ::utils::date::_redraw {win} {
  $win.cal delete all
  set time $::utils::date::_time
  set wmax [winfo width $win.cal]
  set hmax [winfo height $win.cal]

  $win.cal create window 3 3 -anchor nw -window $win.cal.prevY
  $win.cal create window 40 3 -anchor nw -window $win.cal.prev
  $win.cal create window [expr {$wmax-43} ] 3 -anchor ne -window $win.cal.next
  $win.cal create window [expr {$wmax-3} ] 3 -anchor ne -window $win.cal.nextY
  set bottom [lindex [$win.cal bbox all] 3]

  set month [string trimleft [clock format $time -format "%m"] 0]
  set year [clock format $time -format "%Y"]
  $win.cal create text [expr {$wmax/2} ] $bottom -anchor s -font font_Bold \
    -text "[lindex $::tr(Months) [expr $month - 1]] $year"

  incr bottom 3
  $win.cal create line 0 $bottom $wmax $bottom -width 2
  incr bottom 25

  set current ""

  set layout [::utils::date::_layout $time]
  set weeks [expr {[lindex $layout end]+1} ]

  for {set day 0} {$day < 7} {incr day} {
    set x0 [expr {$day*($wmax-7)/7+3} ]
    set x1 [expr {($day+1)*($wmax-7)/7+3} ]
    $win.cal create text [expr {($x1+$x0)/2} ] $bottom -anchor s \
      -text [lindex $::tr(Days) $day] -font font_Small
  }
  incr bottom 3

  foreach {day date dcol wrow} $layout {
    set x0 [expr {$dcol*($wmax-7)/7+3} ]
    set y0 [expr {$wrow*($hmax-$bottom-4)/$weeks+$bottom} ]
    set x1 [expr {($dcol+1)*($wmax-7)/7+3} ]
    set y1 [expr {($wrow+1)*($hmax-$bottom-4)/$weeks+$bottom} ]

    if {$date == $::utils::date::_selected} {set current $date}

    $win.cal create rectangle $x0 $y0 $x1 $y1 -outline black -fill white

    $win.cal create text [expr {$x0+4} ] [expr {$y0+2} ] -anchor nw -text "$day" \
      -fill black -font font_Small -tags [list $date-text all-text]

    $win.cal create rectangle $x0 $y0 $x1 $y1 \
      -outline "" -fill "" -tags [list $date-sensor all-sensor]

    $win.cal bind $date-sensor <ButtonPress-1> "::utils::date::_select $win $date"
  }

  if {$current != ""} {
    $win.cal itemconfigure $current-sensor -outline red -width 3
    $win.cal raise $current-sensor
  } elseif {$::utils::date::_selected == ""} {
    set date [clock format $time -format "%Y-%m-%d"]
    ::utils::date::_select $win $date
  }
}

proc ::utils::date::_layout {time} {
  set month [string trimleft [clock format $time -format "%m"] 0]
  set year  [clock format $time -format "%Y"]

  foreach lastday {31 30 29 28} {
    if {[catch {clock scan "$year-$month-$lastday"}] == 0} { break }
  }
  set seconds [clock scan "$year-$month-1"]
  set firstday [clock format $seconds -format %w]
  set weeks [expr {ceil(double($lastday+$firstday)/7)} ]

  set rlist ""
  for {set day 1} {$day <= $lastday} {incr day} {
    set seconds [clock scan "$year-$month-$day"]
    set date [clock format $seconds -format "%Y-%m-%d"]
    set daycol [clock format $seconds -format %w]
    set weekrow [expr {($firstday+$day-1)/7} ]
    lappend rlist $day $date $daycol $weekrow
  }
  return $rlist
}

proc ::utils::date::_select {win date} {
  set time [clock scan $date]
  set date [clock format $time -format "%Y-%m-%d"]

  set currentMonth [clock format $::utils::date::_time -format "%m %Y"]
  set selectedMonth [clock format $time -format "%m %Y"]
  set ::utils::date::_time $time
  set ::utils::date::_selected $date

  if {$currentMonth == $selectedMonth} {
    $win.cal itemconfigure all-sensor -outline "" -width 1
    $win.cal itemconfigure $date-sensor -outline red -width 3
    $win.cal raise $date-sensor
  } else {
    ::utils::date::_redraw $win
  }
}
