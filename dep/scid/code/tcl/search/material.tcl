###
### search/material.tcl: Material Search routine for Scid.
###

namespace eval ::search::material {}

##########
# Size 20 is only used in Material search window, not for boards.
# It has two special extra images, wm20 and bm20, which contain a
# bishop and knight, for indicating a minor piece.
#

image create photo wm20 -data {
  R0lGODlhFAAUAMIAAH9/f7+/vz8/PwAAAP///////////////yH5BAEKAAcALAAAAAAUABQA
  AANweLp8cCG02cSwNB8RSACctgBAR3iDqJDd5wlidBLCHGb1CQzzx+yPDYAWyJ1gixpSwOLM
  CClFESSRup7RImF42zxP0Vpg0EE6SGjSCqxTKYxHN4RJ6sYETHxVNa3vM2gDQyBoGkNbhIdq
  FHRBZyAaCQA7}

image create photo bm20 -data {
  R0lGODlhFAAUAMIAAH9/f7+/vwAAAD8/P////////////////yH5BAEKAAcALAAAAAAUABQA
  AANneLp8cCG02YQYgupj+5CbEgyYAAQCuJHlNYZo1wHDo7VyOjSAebQxS69R25UCvVlmMXIp
  TrmhSGgB4S5LzoVQegK+YJtWwLWEFjnzGVL7ftYQMoGQggerZ3CrLealoxomXxJIX1kNCQA7
}

image create photo p20 -data {
  R0lGODlh8AAUAMIAAH9/fz8/P7+/vwAAAP///////////////yH5BAEKAAcALAAAAADwABQA
  AAP+eLrc/jDKSau9OOvNu/8VAIBkJAhl2ohqe5xuCgTBGL/oQaMwJew30KwWhAkGA0Vv8gvk
  LALRUxJ9BTSAk40qFXWzkKZWCmQit5Uo2GdDW4ZuiFQ9koZ33mxZAjhjBidJFDNIRBgBhRQD
  Q4t9NH0NP3o1BEgEYjNTDix/SIITfQOIcROIooOFpouekV6PlQMEQ2qaK6QSsZUholGit5GA
  BJeAuMIixgDCnwrJAbKLsMPNsiY0VxeeyxGhnoZm2cTb4OMrP88C0X3NVWF+2CLaELnCUTRm
  6CfDtQuUwv7G1xb8iHUkmSV1lZy0GpErSZR9DbJVUOULCUQl3VRdPDL+rtsKRM8MxuqDjlcr
  FBIflkomK+CdLP8I2Ivg5NIOmxIe5RnygOSzhDKlLGqg01LCGjMhUHQpj1AhmfEYmHIy8JSJ
  jlZXAHIUDWRBojWcFnK1zZk/bw9oBLt09lcuMcpA7eS0CU8WVyIeMTBHD9ARdMjkjPt14BhF
  QEkddDuhSm7MqIUrrgJ0ZhSDvJIr+7o14x9dB3z9zTtCE3A+nHxiUpNXsFKgXj+mHPN3pKa/
  z5cH48LqJJwDVWoT8enYDis4W9k4cjzjliWk0p5ZBn5QcKdvOardXqqXN1nJVjFpxMTNgJw4
  4zypj3V6kRlxecYl7g0+mZtewcLQ/vYMjTb+U6lh5fXfJtmVNcpOj/xnGzL/kHaeO/AZ1xtN
  AaY3nHk9dZOHKnH0th43M1D4T2KXzebEbKKVFcoMhDEz1y8cvUjIPo3AU2MmNI0zYGEU2eiJ
  a3JUqF9PFT6nnnd5GHMdRrScQMeSC3Q23oCdxXaEapAdMI+Sisy1I0YyQslMgOi48iU34AzY
  yxlQJTfUA1hRoJMXYmJkHESOLIXIl1v+A5mAMgE2IkS9qLUGdDH9gIt0fprAaHQRxHeHeIfV
  eEc2CuV0Z6TrNVYcVrdEodp0ZY36WVVsPrPYb/HxmVFykfrYyJfLddTeCx15MZ8ovJlEVHx1
  zoNillrWICgh2zxpeluLfbZVnllK9pefNiiaSopPWLrVD0BZoqnbboOhBexxEQF7bXxuGfdg
  rlTEJxt9YDh1n0Dj7rOmjhtVmmmG6E2ArlRpapGmYsDa6+2qlwYcxAWHyrHwwxAX1h47EVds
  8cUYZ6xxBwkAADs=
}

image create photo e20 -height 20 -width 20
set x 0
foreach p {wp wn wb wr wq wk bp bn bb br bq bk} {
  image create photo ${p}20 -width 20 -height 20
  ${p}20 copy p20 -from $x 0 [expr $x + 19] 19
  incr x 20
}

set ignoreColors 0
set minMoveNum 1
set maxMoveNum 999
set minHalfMoves 1
set oppBishops 0
set sameBishops 0
set minMatDiff -40
set maxMatDiff +40

trace variable minMoveNum w {::utils::validate::Integer 999 0}
trace variable maxMoveNum w {::utils::validate::Integer 999 0}
trace variable minHalfMoves w {::utils::validate::Integer 99 0}
trace variable minMatDiff w {::utils::validate::Integer -99 0}
trace variable maxMatDiff w {::utils::validate::Integer -99 0}

set nPatterns 3
set nMaxPatterns 10

array set pMin [list wq 0 bq 0 wr 0 br 0 wb 0 bb 0 wn 0 bn 0 wm 0 bm 0 wp 0 bp 0]
array set pMax [list wq 2 bq 2 wr 2 br 2 wb 2 bb 2 wn 2 bn 2 wm 4 bm 4 wp 8 bp 8]
for { set i 1 } { $i <= $nMaxPatterns } { incr i } {
  set pattPiece($i) "?";  set pattFyle($i) "?";  set pattRank($i) "?"
}

proc checkPieceCounts {name el op} {
  global pMin pMax
  ::utils::validate::Integer 9 0 $name $el $op
  # Now make sure minor piece counts fit with bishop/knight counts:
  set wmMin [expr {$pMin(wn) + $pMin(wb)} ]
  set wmMax [expr {$pMax(wn) + $pMax(wb)} ]
  set bmMin [expr {$pMin(bn) + $pMin(bb)} ]
  set bmMax [expr {$pMax(bn) + $pMax(bb)} ]
  if {$pMin(wm) < $wmMin} { set pMin(wm) $wmMin }
  if {$pMax(wm) > $wmMax} { set pMax(wm) $wmMax }
  if {$pMin(bm) < $bmMin} { set pMin(bm) $bmMin }
  if {$pMax(bm) > $bmMax} { set pMax(bm) $bmMax }
  foreach p {wq wr wb wn wm wp bq br bb bn bm bp} {
    if {$pMax($p) != ""  &&  $pMax($p) < $pMin($p)} { set pMax($p) $pMin($p) }
  }
}

trace variable pMin w checkPieceCounts
trace variable pMax w checkPieceCounts


proc makeBoolMenu {w varName} {
  upvar #0 $varName var
  if {![info exists var]} { set var "Yes" }
  ttk::menubutton $w -menu $w.menu -style Pad0.Small.TButton
  
  menu $w.menu -tearoff 0
  $w.menu add radiobutton -label Yes -image tb_tick -variable $varName -value Yes \
      -command "$w configure -image tb_tick"  ;# -hidemargin 1
  $w.menu add radiobutton -label No -image tb_cross -variable $varName -value No \
      -command "$w configure -image tb_cross" ;# -hidemargin 1
  return $w.menu
}

proc makePieceMenu {w varName} {
  global dark
  upvar #0 $varName var
  if {![info exists var]} { set var "?" }
  ttk::menubutton $w -menu $w.menu -style Pad0.Small.TButton
  menu $w.menu -tearoff 0
  $w.menu add radiobutton -label " ? " -variable $varName -value "?" \
      -command "$w configure -image e20"  -hidemargin 1
  foreach i {wk wq wr wb wn wp bk bq br bb bn bp} {
    $w.menu add radiobutton -label $i -image ${i}20 -value $i \
        -variable $varName \
        -command "$w configure -image ${i}20"  -hidemargin 1
  }
  foreach i {" ? " wk bk} {
    $w.menu entryconfigure $i -columnbreak 1
  }
  return $w.menu
}

proc updatePatternImages {} {
  global pattPiece nPatterns pattBool
  if {! [winfo exists .sm]} { return }
  for {set i 1} {$i <= $nPatterns} {incr i} {
    if {$pattBool($i) == "Yes"} {
      .sm.mp.patt.grid.b$i configure -image tb_tick
    } else {
      .sm.mp.patt.grid.b$i configure -image tb_cross
    }
    if {$pattPiece($i) == "?"} {
      .sm.mp.patt.grid.p$i configure -image e20
    } else {
      .sm.mp.patt.grid.p$i configure -image "$pattPiece($i)20"
    }
  }
}

# ::search::material::zero
#
#   Called to clear all material minimum/maximum values to zero.
#
proc ::search::material::zero {} {
  global pMin pMax
  array set pMin {wq 0 bq 0 wr 0 br 0 wb 0 bb 0 wn 0 bn 0 wm 0 bm 0 wp 0 bp 0}
  array set pMax {wq 0 bq 0 wr 0 br 0 wb 0 bb 0 wn 0 bn 0 wm 0 bm 0 wp 0 bp 0}
}

proc ::search::material::any {} {
  global pMin pMax
  array set pMin {wq 0 bq 0 wr 0 br 0 wb 0 bb 0 wn 0 bn 0 wm 0 bm 0 wp 0 bp 0}
  array set pMax {wq 2 bq 2 wr 2 br 2 wb 2 bb 2 wn 2 bn 2 wm 4 bm 4 wp 8 bp 8}
  set ::minMatDiff -40
  set maxMatDiff +40
}

proc clearPatterns {} {
  global pattPiece pattFyle pattRank pattBool nPatterns nMaxPatterns

  set nPatterns 3
  for { set i 1 } { $i <= $nMaxPatterns } { incr i } {
    set pattPiece($i) "?";  set pattFyle($i) "?";  set pattRank($i) "?"
    set pattBool($i) "Yes"
    if { $i > $nPatterns } {
	foreach j { b p f r } {
	    grid forget .sm.mp.patt.grid.$j$i
	}
    }
  }
 .sm.mp.patt.b2.pattAdd configure -state enable
  updatePatternImages
}

proc showPattern { count } {
  global pattPiece pattFyle pattRank pattBool nPatterns nMaxPatterns
	set column [expr {7 * (($count - 1) / 7)} ]
	set row [expr {($count - 1) % 7} ]
#    set column 0
#    set row $count
	set f .sm.mp.patt
	grid $f.grid.b$count -row $row -column $column -padx 0 -pady 0 ; incr column
	grid $f.grid.p$count -row $row -column $column -padx 0 -pady 0 ; incr column
	grid $f.grid.f$count -row $row -column $column -padx 0 -pady 0 ; incr column
	grid $f.grid.r$count -row $row -column $column -padx "0 10" -pady 0; incr column
}

proc setPatterns {pattlist} {
  global pattPiece pattFyle pattRank pattBool nPatterns nMaxPatterns
  
  clearPatterns
  set count 1
  foreach patt $pattlist {
    if {$count > $nPatterns} {
	showPattern $count
	set nPatterns $count
    }
    if {$count <= $nPatterns  &&  [llength $patt] == 4} {
      set pattPiece($count) [lindex $patt 0]
      set pattFyle($count) [lindex $patt 1]
      set pattRank($count) [lindex $patt 2]
      set pattBool($count) [lindex $patt 3]
      incr count
    }
  }
  updatePatternImages
}

set smDisplayed(Material) 1
set smDisplayed(Patterns) 0


proc set1Bishops { } {
  global pMin pMax oppBishops sameBishops
    if { $sameBishops || $oppBishops } {
	set pMin(wb) 1
	set pMax(wb) 1
	set pMin(bb) 1
	set pMax(bb) 1
    }
}
# ::search::material
#
#   Opens the window for searching by material or patterns.
#
proc ::search::material {{ref_base ""}} {
  global dark pMin pMax ignoreColors minMoveNum maxMoveNum
  global pattPiece pattFyle pattRank pattBool oppBishops sameBishops nPatterns nMaxPatterns
  global minHalfMoves smDisplayed
  
  set w .sm
  if {[winfo exists $w]} {
    wm deiconify $w
    raiseWin $w
    return
  }
  set small font_Small
  
  win::createDialog $w
  wm title $w "Scid: $::tr(MaterialSearch)"
  #  button $w.piecelabel -font font_Bold -textvar ::tr(Material:) -command {
  #    if {$smDisplayed(Material)} {
  #      set smDisplayed(Material) 0
  #      pack forget .sm.q .sm.r .sm.b .sm.n .sm.m .sm.p .sm.b1 .sm.mdiff
  #    } else {
  #      set smDisplayed(Material) 1
  #      pack .sm.q .sm.r .sm.b .sm.n .sm.m .sm.p .sm.b1 .sm.mdiff \
  #        -after .sm.piecelabel
  #    }
  #  }
  
  bind $w <F1> { helpWindow Searches Material }
  bind $w <Escape> "$w.b3.cancel invoke"
  bind $w <Return> "$w.b3.search invoke"

  pack [ttk::frame $w.refdb] -side top -fill x
  CreateSelectDBWidget "$w.refdb" "refDatabaseM" "$ref_base"

  ttk::frame $w.mp
  ttk::labelframe $w.mp.material -text $::tr(Material)
  pack $w.mp -side top
  grid $w.mp.material -row 0 -column 0 -rowspan 2 -columnspan 2 -sticky nswe -padx "0 10" -pady 5
  
  foreach piece {q r b n m p} {
    ttk::frame $w.mp.material.$piece
    pack $w.mp.material.$piece -side top
  }
  
  foreach i {q r b n m p} {
    set f $w.mp.material.$i
    ttk::button $f.w0 -text "0" -command "set pMin(w$i) 0; set pMax(w$i) 0"
    ttk::button $f.w1 -text "1" -command "set pMin(w$i) 1; set pMax(w$i) 1"
    ttk::button $f.w2 -text "2" -command "set pMin(w$i) 2; set pMax(w$i) 2"
    ttk::button $f.wa -text "0+" -command "set pMin(w$i) 0; set pMax(w$i) 2"
    ttk::button $f.w1p -text "1+" -command "set pMin(w$i) 1; set pMax(w$i) 2"
    ttk::label $f.wi -image w${i}20 -font font_Small
    ttk::label $f.wto -text "-" -font font_Small -padding 0
    ttk::entry $f.wmin -width 2 -textvar pMin(w$i) -font font_Small -justify right ;#-relief sunken
    ttk::entry $f.wmax -width 2 -textvar pMax(w$i) -font font_Small -justify right ;#-relief sunken
    pack $f.w0 $f.w1 $f.w2 $f.wa $f.w1p $f.wi $f.wmin $f.wto $f.wmax -side left -pady 1
    
    pack [ttk::frame $f.space -width 20] -side left
    ttk::button $f.b0 -text "0" -command "set pMin(b$i) 0; set pMax(b$i) 0"
    ttk::button $f.b1 -text "1" -command "set pMin(b$i) 1; set pMax(b$i) 1"
    ttk::button $f.b2 -text "2" -command "set pMin(b$i) 2; set pMax(b$i) 2"
    ttk::button $f.ba -text "0+" -command "set pMin(b$i) 0; set pMax(b$i) 2"
    ttk::button $f.b1p -text "1+" -command "set pMin(b$i) 1; set pMax(b$i) 2"
    ttk::label $f.bi -image b${i}20 -font font_Small
    ttk::label $f.bto -text "-" -font font_Small
    ttk::entry $f.bmin -width 2 -textvar pMin(b$i) -font font_Small -justify right ;#-relief sunken
    ttk::entry $f.bmax -width 2 -textvar pMax(b$i) -font font_Small -justify right ;#-relief sunken
    pack $f.b0 $f.b1 $f.b2 $f.ba $f.b1p $f.bi $f.bmin $f.bto $f.bmax -side left -pady 1
    
    foreach b {0 1 2 a 1p} {
      $f.w$b configure -width 2 -takefocus 0 -style Pad0.Small.TButton ;# -font $small -pady 0 -padx 1
      $f.b$b configure -width 2 -takefocus 0 -style Pad0.Small.TButton ;# -font $small -pady 0 -padx 1
    }
    foreach widget {wmin wmax bmin bmax} {
      bindFocusColors $f.$widget
    }
    if {$i == "p"} {
      $f.w1p configure -command "set pMin(wp) 1; set pMax(wp) 8"
      $f.wa configure -command "set pMin(wp) 0; set pMax(wp) 8"
      $f.b1p configure -command "set pMin(bp) 1; set pMax(bp) 8"
      $f.ba configure -command "set pMin(bp) 0; set pMax(bp) 8"
    }
    if {$i == "m"} {
      $f.w1p configure -command "set pMin(wm) 1; set pMax(wm) 4"
      $f.wa configure -command "set pMin(wm) 0; set pMax(wm) 4"
      $f.b1p configure -command "set pMin(bm) 1; set pMax(bm) 4"
      $f.ba configure -command "set pMin(bm) 0; set pMax(bm) 4"
    }
  }
  
  # Buttons that manipulate material settings:
  set f $w.mp.material.b1
  pack [ttk::frame $f] -side top
  
  dialogbuttonsmall $f.zero [ list -textvar ::tr(Zero) -command ::search::material::zero] Pad0.Small.TButton
  dialogbuttonsmall $f.reset [ list -textvar ::tr(Any) -command ::search::material::any ] Pad0.Small.TButton
  dialogbuttonsmall $f.current [ list -textvar ::tr(CurrentBoard) -command {
    ::search::material::zero
    set bd [sc_pos board]
    for {set i 0} {$i < 64} {incr i} {
      set piece $::board::letterToPiece([ string index $bd $i ])
      if {$piece != "e"  &&  $piece != "wk"  &&  $piece != "bk"} {
        incr pMin($piece); set pMax($piece) $pMin($piece)
      }
    }
  } ] Pad0.Small.TButton
  
  ttk::menubutton $f.common -textvar ::tr(CommonEndings...) -menu $f.common.m -style pad0.TMenubutton ;# -font $small -relief raised
  menu $f.common.m -font $small
  set m $f.common.m
  $m add command -label [tr EndingPawns] -command {
    ::search::material::zero
    array set pMin {wp 1 bp 1}
    array set pMax {wp 8 bp 8}
  }
  $m add command -label [tr EndingRookVsPawns] -command {
    ::search::material::zero
    array set pMin {wr 1 bp 1}
    array set pMax {wr 1 bp 8}
  }
  $m add command -label [tr EndingRookPawnVsRook] -command {
    ::search::material::zero
    array set pMin {wr 1 br 1 wp 1}
    array set pMax {wr 1 br 1 wp 1}
  }
  $m add command -label [tr EndingRookPawnsVsRook] -command {
    ::search::material::zero
    array set pMin {wr 1 br 1 wp 1}
    array set pMax {wr 1 br 1 wp 8}
  }
  $m add command -label [tr EndingRooks] -command {
    ::search::material::zero
    array set pMin {wr 1 br 1}
    array set pMax {wr 1 br 1 wp 8 bp 8}
    set pMin(wr) 1; set pMax(wr) 1; set pMin(wp) 0; set mPax(wp) 8
    set pMin(br) 1; set pMax(br) 1; set pMin(bp) 0; set mPax(bp) 8
  }
  $m add command -label [tr EndingRooksPassedA] -command {
    ::search::material::zero
    array set pMin {wr 1 br 1 wp 1}
    array set pMax {wr 1 br 1 wp 8 bp 8}
    setPatterns {{wp a ? Yes} {bp a ? No} {bp b ? No}}
    set ignoreColors 1
  }
  $m add command -label [tr EndingRooksDouble] -command {
    ::search::material::zero
    array set pMin {wr 2 br 2}
    array set pMax {wr 2 br 2 wp 8 bp 8}
  }
  $m add command -label [tr EndingBishops] -command {
    ::search::material::zero
    array set pMin {wb 1 bb 1 wm 1 bm 1}
    array set pMax {wb 1 bb 1 wm 1 bm 1 wp 8 bp 8}
  }
  $m add command -label [tr EndingBishopVsKnight] -command {
    ::search::material::zero
    array set pMin {wb 1 bn 1 wm 1 bm 1}
    array set pMax {wb 1 bn 1 wm 1 bm 1 wp 8 bp 8}
  }
  $m add command -label [tr EndingKnights] -command {
    ::search::material::zero
    array set pMin {wn 1 bn 1 wm 1 bm 1}
    array set pMax {wn 1 bn 1 wm 1 bm 1 wp 8 bp 8}
  }
  $m add command -label [tr EndingQueens] -command {
    ::search::material::zero
    array set pMin {wq 1 bq 1}
    array set pMax {wq 1 bq 1 wp 8 bp 8}
  }
  $m add command -label [tr EndingQueenPawnVsQueen] -command {
    ::search::material::zero
    array set pMin {wq 1 bq 1 wp 1}
    array set pMax {wq 1 bq 1 wp 1}
  }
  $m add command -label [tr BishopPairVsKnightPair] -command {
    ::search::material::zero
    array set pMin {wb 2 bn 2 wm 2 bm 2}
    array set pMax {wq 1 bq 1 wr 2 br 2 wb 2 bn 2 wm 2 bm 2 wp 8 bp 8}
  }
  
  pack $f.zero -side left -pady 5 -padx "0 10"
  pack $f.reset $f.current -side left -pady 5 -padx 10
  pack $f.common -side left -padx "10 0" -pady 5
  #if {! $smDisplayed(Material)} {
  #  pack forget .sm.q .sm.r .sm.b .sm.n .sm.m .sm.p .sm.b1 .sm.mdiff
  #}
  
  ttk::frame $w.mp.material.mdiff
  set f $w.mp.material.mdiff
  pack $f -side left -anchor n -pady 5
  ttk::label $f.label -text $::tr(MaterialDiff)
  ttk::entry $f.min -width 3 -textvar minMatDiff -font $small -justify right ;#-relief sunken
  bindFocusColors $f.min
  ttk::label $f.sep -text "-" -font $small
  ttk::entry $f.max -width 3 -textvar maxMatDiff -font $small -justify right ;#-relief sunken
  bindFocusColors $f.max
  ttk::label $f.sep2 -text " " -font $small
  ttk::button $f.any -textvar ::tr(Any) -style Pad0.Small.TButton -command {set minMatDiff -40; set maxMatDiff +40} -width 0
  ttk::button $f.w1 -text " + " -style Pad0.Small.TButton -command {set minMatDiff +1; set maxMatDiff +40}  -width 0
  ttk::button $f.equal -text " = " -style Pad0.Small.TButton -command {set minMatDiff 0; set maxMatDiff 0}  -width 0
  ttk::button $f.b1 -text " - " -style Pad0.Small.TButton -command {set minMatDiff -40; set maxMatDiff -1}  -width 0
  ttk::label $f.explain -font $small -text "$::tr(White) - $::tr(Black) Q=9 R=5 B=3 N=3 P=1"

  pack $f.label -side top -anchor w
  pack $f.explain -side bottom
  pack $f.min $f.sep $f.max -side left
  pack $f.sep2 $f.any $f.w1 $f.equal $f.b1 -side left -padx 1 -pady 1
  
  ttk::labelframe $w.mp.patt -text $::tr(Patterns)
  set f $w.mp.patt
  grid $w.mp.patt -row 0 -column 2 -sticky nwe -pady 5
  
  #dialogbutton $w.pattl -font font_Bold -textvar ::tr(Patterns:) -command {
  #  if {$smDisplayed(Patterns)} {
  #    set smDisplayed(Patterns) 0
  #    pack forget .sm.patt .sm.b2
  #  } else {
  #    set smDisplayed(Patterns) 1
  #    pack .sm.patt .sm.b2 -after .sm.pattl
  #  }
  #}

  ttk::frame $f.grid
  for { set i 1 } { $i <= $nMaxPatterns } { incr i } {
    makeBoolMenu $f.grid.b$i pattBool($i)
    set menuPiece1 [ makePieceMenu $f.grid.p$i pattPiece($i) ]
    tk_optionMenu $f.grid.f$i pattFyle($i) "?" a b c d e f g h
    tk_optionMenu $f.grid.r$i pattRank($i) "?" 1 2 3 4 5 6 7 8
    $f.grid.f$i configure -width 1 -indicatoron 0 -pady 0 -background [ttk::style lookup . -background]
    $f.grid.r$i configure -width 1 -indicatoron 0 -pady 0 -background [ttk::style lookup . -background]
    if { $i <= $nPatterns } {
    set column [expr {7 * (($i - 1) / 7)} ]
    set row [expr {($i - 1) % 7} ]
#    set column 0
#    set row $i
    grid $f.grid.b$i -row $row -column $column -padx 0 -pady 0 ; incr column
    grid $f.grid.p$i -row $row -column $column -padx 0 -pady 0 ; incr column
    grid $f.grid.f$i -row $row -column $column -padx 0 -pady 0 ; incr column
    grid $f.grid.r$i -row $row -column $column -padx "0 10" -pady 0; incr column
    }
  }
  updatePatternImages
  
  ### Buttons that manipulate patterns:
  set f $w.mp.patt.b2
  ttk::frame $f
  dialogbutton $f.clearPat -textvar ::tr(Clear) -command clearPatterns
  ttk::menubutton $f.common -textvar ::tr(CommonPatterns...) -menu $f.common.m
  menu $f.common.m -font $small
  $f.common.m add command -label [tr PatternWhiteIQP] -command {
    if {$pMin(wp) < 1} { set pMin(wp) 1 }
    setPatterns {{wp d ? Yes} {wp c ? No} {wp e ? No}}
  }
  $f.common.m add command -label [tr PatternWhiteIQPBreakE6] -command {
    if {$pMin(wp) < 1} { set pMin(wp) 1 }
    if {$pMin(bp) < 1} { set pMin(bp) 1 }
    setPatterns {{wp d 5 Yes} {wp c ? No} {wp e ? No} {wp d 4 No} \
          {bp e 6 Yes} {bp c ? No} {bp d ? No}}
  }
  $f.common.m add command -label [tr PatternWhiteIQPBreakC6] -command {
    if {$pMin(wp) < 1} { set pMin(wp) 1 }
    if {$pMin(bp) < 1} { set pMin(bp) 1 }
    setPatterns {{wp d 5 Yes} {wp c ? No} {wp e ? No} {wp d 4 No} \
          {bp c 6 Yes} {bp e ? No} {bp d ? No}}
  }
  $f.common.m add command -label [tr PatternBlackIQP] -command {
    if {$pMin(bp) < 1} { set pMin(bp) 1 }
    setPatterns {{bp d ? Yes} {bp c ? No} {bp e ? No}}
  }
  $f.common.m add command -label [tr PatternWhiteBlackIQP] -command {
    if {$pMin(wp) < 1} { set pMin(wp) 1 }
    if {$pMin(bp) < 1} { set pMin(bp) 1 }
    setPatterns {{wp d ? Yes} {wp c ? No} {wp e ? No} \
          {bp d ? Yes} {bp c ? No} {bp e ? No}}
  }
  $f.common.m add command -label [tr PatternCoupleC3D4] -command {
    set pMin(wp) 4; set pMax(wp) 6
    set pMin(bp) 4; set pMax(bp) 6
    setPatterns {{wp c 3 Yes} {wp d 4 Yes} {wp b ? No} {wp e ? No}
      {bp c ? No} {bp d ? No}}
  }
  $f.common.m add command -label [tr PatternHangingC5D5] -command {
    set pMin(bp) 4; set pMax(bp) 6
    set pMin(wp) 4; set pMax(wp) 6
    setPatterns {{bp c 5 Yes} {bp d 5 Yes} {bp b ? No} {bp e ? No}
      {wp c ? No} {wp d ? No}}
  }
  $f.common.m add command -label [tr PatternMaroczy] -command {
    if {$pMin(bp) < 1} { set pMin(bp) 1 }
    if {$pMax(bp) > 7} { set pMax(bp) 7 }
    if {$pMin(wp) < 2} { set pMin(wp) 2 }
    if {$pMax(wp) > 7} { set pMax(wp) 7 }
    setPatterns {{wp c 4 Yes} {wp e 4 Yes} {bp d ? Yes} {wp d ? No}
      {bp c ? No} {bp d 5 No}}
  }
  $f.common.m add command -label [tr PatternRookSacC3] -command {
    set pMin(br) 2; set pMax(br) 2
    set pMin(wr) 2; set pMax(wr) 2
    setPatterns { {br c 3 Yes} {wp b 2 Yes} }
  }
  $f.common.m add command -label [tr PatternKc1Kg8] -command {
    setPatterns { {wk c 1 Yes} {bk g 8 Yes} }
  }
  $f.common.m add command -label [tr PatternKg1Kc8] -command {
    setPatterns { {wk g 1 Yes} {bk c 8 Yes} }
  }
  $f.common.m add command -label [tr PatternLightFian] -command {
    set pMin(wb) 1; set pMin(bb) 1
    setPatterns { {wb g 2 Yes} {bb b 7 Yes} }
  }
  $f.common.m add command -label [tr PatternDarkFian] -command {
    set pMin(wb) 1; set pMin(bb) 1
    setPatterns { {wb b 2 Yes} {bb g 7 Yes} }
  }
  $f.common.m add command -label [tr PatternFourFian] -command {
    set pMin(wb) 2; set pMin(bb) 2
    setPatterns { {wb b 2 Yes} {wb g 2 Yes} {bb b 7 Yes} {bb g 7 Yes} }
  }
  
#  ttk::button $f.pattAdd -text $::tr(GlistAddField) -command {}
  ttk::button $f.pattAdd -text " + " -command {
      if { $::nPatterns < $::nMaxPatterns } {
	  incr ::nPatterns
	  showPattern $::nPatterns
	  updatePatternImages
	  if { $::nPatterns == $::nMaxPatterns } { .sm.mp.patt.b2.pattAdd configure -state disabled }
      }
  }
  pack $f -side top -anchor w
  pack $f.common -side top -pady "0 5" -anchor w
  pack $f.pattAdd -side left -pady "0 5" -padx "0 10" -anchor w
  pack $f.clearPat -side left -anchor w -pady "0 5"
  pack $w.mp.patt.grid -side top -anchor w ;# -fill both -expand 1
  #if {! $smDisplayed(Patterns)} {
  #  pack forget $w.patt $w.b2
  #}
  updatePatternImages
  
  ttk::frame $w.mp.material.bishops
  set f $w.mp.material.bishops
  pack $f -side right -pady 5
  ttk::label $f.wb -image wb20
  ttk::label $f.bb -image bb20
  ttk::checkbutton $f.same -text $::tr(SameColor) -variable sameBishops -command set1Bishops
  ttk::checkbutton $f.opp -text $::tr(OppColor) -variable oppBishops -command set1Bishops
  pack $f.wb $f.bb -side left -anchor w
  foreach i { same opp } { pack $f.$i -side top -anchor w }

  addHorizontalRule $w
  
  ### Now the move counter:
  pack [ ttk::frame $w.fmc ] -side top -expand 1 -fill both
  
  set f $w.fmc.move
  pack [ttk::frame $f] -side top -ipady 5 ;# -expand 1 -fill both
  ttk::label $f.fromlab -textvar ::tr(MoveNumberRange:) -font font_Small
  ttk::entry $f.from -width 4 -textvar minMoveNum -justify right -font font_Small
  ttk::label $f.tolab -text "-" -font font_Small
  ttk::entry $f.to -width 4 -textvar maxMoveNum -justify right -font font_Small
  ttk::label $f.space -text "  "
  ttk::label $f.label1 -textvar ::tr(MatchForAtLeast) -font font_Small
  ttk::entry $f.hmoves -width 3 -textvar minHalfMoves -justify right -font font_Small
  ttk::label $f.label2 -textvar ::tr(HalfMoves) -font font_Small
  bindFocusColors $f.from
  bindFocusColors $f.to
  bindFocusColors $f.hmoves
  pack $f.fromlab $f.from $f.tolab $f.to $f.space $f.label1 $f.hmoves $f.label2 -side left
  
  ::search::addFilterOpFrame $w 1
  
  ### Progress bar:
  ttk::frame $w.fprogress
  canvas $w.fprogress.progress -height 20 -width 500 -bg white -relief solid -border 1
  $w.fprogress.progress create rectangle 0 0 0 0 -outline blue -fill blue -tags bar
  $w.fprogress.progress create text 495 10 -anchor e -font font_Regular -tags time -fill black -text "0:00 / 0:00"
  
  ### Last of all, the buttons frame:
  
  set f $w.b3
  pack [ttk::frame $f] -side top -ipady 5 -fill x
  ttk::checkbutton $f.ignorecol -textvar ::tr(IgnoreColors) -variable ignoreColors ;# -padx 4
  
  dialogbutton $f.save -textvar ::tr(Save...) -command ::search::material::save
  
  dialogbutton $f.stop -textvar ::tr(Stop) -command progressBarCancel
  $f.stop configure -state disabled
  
  dialogbutton $f.search -textvar ::tr(Search) -command {
    busyCursor .
    set curr_base [sc_base current]
    sc_base switch [lindex $refDatabaseM 0]
    .sm.b3.stop configure -state normal
    grab .sm.b3.stop
    set bishops "Either"
    if { $sameBishops } { set bishops "Same" }
    if { $oppBishops } { set bishops "Opposite" }
    if { $sameBishops && $oppBishops } { set bishops "Either" }
    progressBarSet .sm.fprogress.progress 501 21
    set str [sc_search material \
        -wq [list $pMin(wq) $pMax(wq)] -bq [list $pMin(bq) $pMax(bq)] \
        -wr [list $pMin(wr) $pMax(wr)] -br [list $pMin(br) $pMax(br)] \
        -wb [list $pMin(wb) $pMax(wb)] -bb [list $pMin(bb) $pMax(bb)] \
        -wn [list $pMin(wn) $pMax(wn)] -bn [list $pMin(bn) $pMax(bn)] \
        -wm [list $pMin(wm) $pMax(wm)] -bm [list $pMin(bm) $pMax(bm)] \
        -wp [list $pMin(wp) $pMax(wp)] -bp [list $pMin(bp) $pMax(bp)] \
        -flip $ignoreColors -filter $::search::filter::operation \
        -range [list $minMoveNum $maxMoveNum] \
        -length $minHalfMoves -bishops $bishops \
        -diff [list $minMatDiff $maxMatDiff] \
        -patt "$pattBool(1) $pattPiece(1) $pattFyle(1) $pattRank(1)" \
        -patt "$pattBool(2) $pattPiece(2) $pattFyle(2) $pattRank(2)" \
        -patt "$pattBool(3) $pattPiece(3) $pattFyle(3) $pattRank(3)" \
        -patt "$pattBool(4) $pattPiece(4) $pattFyle(4) $pattRank(4)" \
        -patt "$pattBool(5) $pattPiece(5) $pattFyle(5) $pattRank(5)" \
        -patt "$pattBool(6) $pattPiece(6) $pattFyle(6) $pattRank(6)" \
        -patt "$pattBool(7) $pattPiece(7) $pattFyle(7) $pattRank(7)" \
        -patt "$pattBool(8) $pattPiece(8) $pattFyle(8) $pattRank(8)" \
        -patt "$pattBool(9) $pattPiece(9) $pattFyle(9) $pattRank(9)" \
        -patt "$pattBool(10) $pattPiece(10) $pattFyle(10) $pattRank(10)" ]
    grab release .sm.b3.stop
    .sm.b3.stop configure -state disabled
    sc_base switch $curr_base
    .sm.fprogress.progress itemconfigure time -text $str
    .sm.fprogress.progress coords bar 0 0 0 0
    unbusyCursor .
    ::notify::DatabaseModified [sc_base current] dbfilter
    foreach {filterSz gameSz mainSz} [sc_filter sizes $curr_base dbfilter] {}
    set str [::windows::gamelist::formatFilterText $filterSz $gameSz]
    .sm.filterop configure -text "$::tr(FilterOperation) ($str)"
  }
  
  dialogbutton $f.cancel -textvar ::tr(Close) \
      -command { focus .; destroy .sm }
  
  pack $f.ignorecol $w.b3.save -side left -pady 5 -padx 5
  pack $w.b3.cancel $w.b3.search $w.b3.stop -side right -pady 5 -padx 5
  pack $w.fprogress.progress -side top -pady 2
  pack $w.fprogress -expand 1 -fill both
  
  # update
  wm resizable $w 0 0
  ::search::Config
  focus $f.search
}

proc ::search::material::save {} {
  global pMin pMax ignoreColors minMoveNum maxMoveNum minHalfMoves
  global pattPiece pattFyle pattRank pattBool sameBishops oppBishops nPatterns
  
  set ftype { { "Scid SearchOptions files" {".sso"} } }
  set fName [tk_getSaveFile -initialdir [pwd] -filetypes $ftype -title "Create a SearchOptions file"]
  if {$fName == ""} { return }
  
  if {[string compare [file extension $fName] ".sso"] != 0} {
    append fName ".sso"
  }
  
  if {[catch {set searchF [open $fName w]}]} {
    tk_messageBox -title "Error: Unable to open file" -type ok -icon error \
        -message "Unable to create SearchOptions file: $fName"
    return
  }
  puts $searchF "\# SearchOptions File created by Scid $::scidVersion"
  puts $searchF "set searchType Material"
  # First write the material counts:
  foreach i {wq bq wr br wb bb wn bn wp bp} {
    puts $searchF "set pMin($i) $pMin($i)"
    puts $searchF "set pMax($i) $pMax($i)"
  }
  # Now write other numeric values:
  foreach i {
    ignoreColors minMoveNum maxMoveNum minHalfMoves sameBishops oppBishops nPatterns
    ::search::filter::operation
  } {
    puts $searchF "set $i [set $i]"
  }
  # Last, write the patterns:
  for {set i 1} {$i <= $nPatterns} {incr i} {
    puts $searchF "set pattPiece($i) $pattPiece($i)"
    puts $searchF "set pattFyle($i) $pattFyle($i)"
    puts $searchF "set pattRank($i) $pattRank($i)"
    puts $searchF "set pattBool($i) $pattBool($i)"
  }
  tk_messageBox -type ok -icon info -title "Search Options saved" \
      -message "Material/pattern search options saved to: $fName"
  close $searchF
}

