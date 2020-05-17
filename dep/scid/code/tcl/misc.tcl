###
### misc.tcl: part of Scid.
### Copyright (C) 2001  Shane Hudson.
### Copyright (C) 2007  Pascal Georges
### Copyright (C) 2015  Fulvio Benini
###
### Miscellaneous routines called by other Tcl functions

################################################################################
# vwait but will timeout after a delay. Var must be fully qualified (::)
################################################################################
proc vwaitTimed { var {delay 0} {warn "warnuser"} } {
  
  proc trigger {var warn} {
    if {$warn == "warnuser"} {
      tk_messageBox -type ok -icon error -parent . -title "Protocol error" -message "vwait timeout for $var"
    }
    set $var 1
  }
  
  if { $delay != 0 } {
    set timerId [after $delay "trigger $var $warn"]
  }
  
  vwait $var
  
  if [info exists timerId] { after cancel $timerId }
  
}

################################################################################
# bindFocusColors:
#   Configures a text or entry widget so it turns lightYellow when it
#   gets the focus, and turns white again when it loses focus.
#
# THIS IS CURRENTLY DISABLED since it works fine with regular entry widgets
# but causes problems with our combobox widgets, not sure why!
#
proc bindFocusColors {w {inColor lightYellow} {outColor white}} {
  $w configure -background $outColor
  #bind $w <FocusIn> "+$w configure -background $inColor"
  #bind $w <FocusOut> "+$w configure -background $outColor"
}


## FROM TK 8.5.9
## ttk::bindMouseWheel $bindtag $command...
#	Adds basic mousewheel support to $bindtag.
#	$command will be passed one additional argument
#	specifying the mousewheel direction (-1: up, +1: down).
#

proc bindMouseWheel {bindtag callback} {
    switch -- [tk windowingsystem] {
	x11 {
	    bind $bindtag <ButtonPress-4> "$callback -1; break"
	    bind $bindtag <ButtonPress-5> "$callback +1; break"
	}
	win32 {
	    bind $bindtag <<MWheel>> "[append callback { [expr {-(%d/120)}]}]; break"
	}
	aqua {
	    bind $bindtag <MouseWheel> "[append callback { [expr {-(%D)}]} ]; break"
	}
    }
}

# dialogbuttonframe:
#   Creates a frame that will be shown at the bottom of a
#   dialog window. It takes two parameters: the frame widget
#   name to create, and a list of button args. Each element
#   should contain a widget name, and button arguments.
#
proc dialogbuttonframe {frame buttonlist} {
  ttk::frame $frame
  set bnames {}
  set maxlength 0
  foreach buttonargs $buttonlist {
    set bname $frame.[lindex $buttonargs 0]
    set bargs [lrange $buttonargs 1 end]
    eval ttk::button $bname $bargs
    set bnames [linsert $bnames 0 $bname]
    set length [string length [$bname cget -text]]
    if {$length > $maxlength} { set length $maxlength}
  }
  if {$maxlength < 7} { set maxlength 7 }
  foreach b $bnames {
    $b configure -width $maxlength -padx 4
    pack $b -side right -padx 4 -pady 4
  }
}

# packbuttons
#   Packs a row of dialog buttons to the left/right of their frame
#   with a standard amount of padding.
#
proc packbuttons {side args} {
  eval pack $args -side $side -padx 5 -pady 3
}
proc packdlgbuttons {args} {
  pack {*}$args -side right -padx 5 -pady "15 5"
}
# dialogbutton:
#   Creates a button that will be shown in a dialog box, so it
#   is given a minimum width.
#
proc dialogbutton {w args} {
  set retval [eval ttk::button $w $args] ;# -style TButton
  set length [string length [$w cget -text]]
  if {$length < 7} { set length 7 }
  $w configure -width $length
  return retval
}

proc dialogbuttonsmall {w args {style "Small.TButton"} } {
  set retval [eval ttk::button $w -style $style $args]
  set length [string length [$w cget -text]]
  if {$length < 7} { set length 7 }
  $w configure -width $length
  return retval
}

# autoscrollframe
#   Creates and returns a frame containing a widget which is gridded
#   with scrollbars that automatically hide themselves when they are
#   not needed.
#   The frame and widget may already exist; they are created if needed.
#   FBF 2011.03.05:
#     $frame and $w aspects are not changed if they already exists
#     scrollbars are created on time 0, otherwise they are not hidden
#
#   Usage:
#      autoscrollframe [-bars none|x|y|both] frame type w args
#
proc autoscrollframe {args} {
  set bars both
  if {[lindex $args 0] == "-bars"} {
    set bars [lindex $args 1]
    if {$bars != "x" && $bars != "y" && $bars != "none" && $bars != "both"} {
      return -code error "Invalid parameter: -bars $bars"
    }
    set args [lrange $args 2 end]
  }
  if {[llength $args] < 3} {
    return -code error "Insufficient number of parameters"
  }
  set frame [lindex $args 0]
  set type [lindex $args 1]
  set w [lindex $args 2]
  set args [lrange $args 3 end]
  
  set retval $frame
  if {! [winfo exists $frame]} {
    ttk::frame $frame
    $frame configure -relief sunken -borderwidth 2
  }
  if {! [winfo exists $w]} {
    $type $w
    if {[llength $args] > 0} {
      eval $w configure $args
    }
    $w configure -relief flat -borderwidth 0
  }

  autoscrollBars $bars $frame $w
  return $retval
}

proc autoscrollBars {bars frame w} {
  global _autoscroll

  grid $w -in $frame -row 0 -column 0 -sticky news

  if {$bars == "y"  ||  $bars == "both"} {
    ttk::scrollbar $frame.ybar -command [list $w yview] -takefocus 0
    $w configure -yscrollcommand [list _autoscroll $frame.ybar]
    grid $frame.ybar -row 0 -column 1 -sticky ns
    set _autoscroll($frame.ybar) 1
    set _autoscroll(time:$frame.ybar) 0
    bindMouseWheel $w "_autoscrollMouseWheel $w $frame.ybar"
  }
  if {$bars == "x"  ||  $bars == "both"} {
    ttk::scrollbar $frame.xbar -command [list $w xview] -takefocus 0 -orient horizontal
    $w configure -xscrollcommand [list _autoscroll $frame.xbar]
    grid $frame.xbar -row 1 -column 0 -sticky we
    set _autoscroll($frame.xbar) 1
    set _autoscroll(time:$frame.xbar) 0
  }
  grid rowconfigure $frame 0 -weight 1
  grid columnconfigure $frame 0 -weight 1
  grid rowconfigure $frame 1 -weight 0
  grid columnconfigure $frame 1 -weight 0
}

proc _autoscrollMouseWheel {{w} {bar} {direction}} {
  if {$::_autoscroll($bar) == 0} return
  $w yview scroll $direction units
}

array set _autoscroll {}

# _autoscroll
#   This is the "set" command called for auto-scrollbars.
#   If the bar is shown but should not be, it is hidden.
#   If the bar is hidden but should be shown, it is redrawn.
#   Note that once a bar is shown, it will not be removed again for
#   at least a few milliseconds; this is to overcome problematic
#   interactions between the x and y scrollbars where hiding one
#   causes the other to be shown etc. This usually happens because
#   the stupid Tcl/Tk text widget doesn't handle scrollbars well.
#
proc _autoscroll {bar args} {
  global _autoscroll
  if {[llength $args] == 2} {
    set min [lindex $args 0]
    set max [lindex $args 1]
    if {$min > 0.0  ||  $max < 1.0} {
      if {! $_autoscroll($bar)} {
        grid configure $bar
        set _autoscroll($bar) 1
        set _autoscroll(time:$bar) [clock clicks -milli]
      }
    } else {
      if {[clock clicks -milli] > [expr {$_autoscroll(time:$bar) + 100}]} {
        grid remove $bar
        set _autoscroll($bar) 0
      }
    }
  }
  eval $bar set $args
}

proc _autoscrollMap {frame} {
  # wm geometry [winfo toplevel $frame] [wm geometry [winfo toplevel $frame]]
}


# busyCursor, unbusyCursor:
#   Sets all cursors to watch (indicating busy) or back to their normal
#   setting again.

array set scid_busycursor {}
set scid_busycursorState 0

proc doBusyCursor {w flag} {
  global scid_busycursor
  if {! [winfo exists $w]} { return }
  if {[winfo class $w] == "Menu"} { return }
  
  if {$flag} {
    if { [ catch { set scid_busycursor($w) [$w cget -cursor] } ] } {
      return
    }
    catch {$w configure -cursor watch}
  } else {
    catch {$w configure -cursor $scid_busycursor($w)} err
  }
  foreach i [winfo children $w] { doBusyCursor $i $flag }
}

proc busyCursor {w {flag 1}} {
  global scid_busycursor scid_busycursorState
  if {$scid_busycursorState == $flag} { return }
  set scid_busycursorState $flag
  doBusyCursor $w $flag
}

proc unbusyCursor {w} {busyCursor $w 0}


# addHorizontalRule, addVerticalRule
#   Add a horizontal/vertical rule frame to a window.
#   The optional parameters [x/y]padding and sunken allow the spacing and
#   appearance of the rule to be specified.
#
set horizRuleCounter 0
set vertRuleCounter 0

proc addHorizontalRule {w {ypadding 5} {relief sunken} {height 2} } {
  global horizRuleCounter
  
  ttk::separator $w.line$horizRuleCounter -orient horizontal
  pack $w.line$horizRuleCounter -fill x ;# -pady $ypadding
  
  # set f [ ttk::frame $w.line$horizRuleCounter -height $height -borderwidth 2 -relief $relief ]
  # pack $f -fill x -pady $ypadding
  incr horizRuleCounter
}

proc addVerticalRule {w {xpadding 5} {relief sunken}} {
  global vertRuleCounter
  
  ttk::separator $w.line$vertRuleCounter -orient vertical
  pack $w.line$vertRuleCounter -fill y -side left ;# -padx $xpadding
  
  # set f [ ttk::frame $w.line$vertRuleCounter -width 2 -borderwidth 2 -relief $relief ]
  # pack $f -fill y -padx $xpadding -side left
  incr vertRuleCounter
}

# progressWindow:
#   Creates a window with a label, progress bar, and (if specified),
#   a cancel button and cancellation command.
#
proc progressWindow { title text {button ""} {command "progressBarCancel"} } {
  set w .progressWin
  if {[winfo exists $w]} { return }

  set ::progressWin_focus [focus]

  win::createDialog $w 6
  wm resizable $w 0 0
  wm title $w $title

  ttk::frame $w.f
  ttk::label $w.f.t -text $text
  autoscrollframe -bars y $w.f.cmsg text $w.f.cmsg.text -width 70 -height 14 -wrap word -font font_Regular
  canvas $w.f.c -width 400 -height 20 -bg white -relief solid -border 1 -highlightthickness 0
  $w.f.c create rectangle 0 0 0 0 -fill DodgerBlue3 -outline DodgerBlue3 -tags bar
  $w.f.c create text 395 10 -anchor e -font font_Regular -tags time -fill black -text "0:00 / 0:00"
  ttk::button $w.f.cancel -text $button -command "$command"

  grid $w.f.t -row 0 -columnspan 2 -pady 4 -sticky news
  grid $w.f.cmsg -row 1 -columnspan 2 -pady 4 -sticky news
  grid $w.f.c -row 2 -column 0 -pady 4 -stick w
  grid $w.f.cancel -row 2 -column 1 -padx "10 0"
  grid $w.f -sticky news
  grid rowconfigure $w.f 1 -weight 1
  grid columnconfigure $w.f 0 -weight 1
  grid remove $w.f.cmsg
  if {$button == ""} { grid remove $w.f.cancel }

  # Set up geometry for middle of screen:
  set x [expr ([winfo screenwidth $w] - 400) / 2]
  set y [expr ([winfo screenheight $w] - 40) / 2]
  wm geometry $w +$x+$y
  grab $w
  wm withdraw $w

  progressBarSet $w.f.c 401 21

  set ::progressCanvas(show) "catch {wm deiconify $w}"
}

proc progressBarSet { canvasname width height } {
  update idletasks
  set ::progressCanvas(name) $canvasname
  set ::progressCanvas(w) $width
  set ::progressCanvas(h) $height
  set ::progressCanvas(cancel) 0
  set ::progressCanvas(init) 1
  set ::progressCanvas(show) {}
  set ::progressCanvas(time) [clock milliseconds]
  after idle { unset ::progressCanvas(init) }
}

proc progressBarCancel { } {
  set ::progressCanvas(cancel) 1
}


proc progressCallBack {done {msg ""}} {
  if {$done == "init"} {
    if {[info exists ::progressCanvas(init)]} {
      return $::progressCanvas(init)
    }
    # No progress bar
    return -code break
  }

  if {! [winfo exists $::progressCanvas(name)] || $::progressCanvas(cancel)} {
    #Interrupted
    return -code break
  }

  set elapsed [expr { [clock milliseconds] - $::progressCanvas(time) }]
  if {$done != 0} {
    set estimated [expr { int($elapsed / double($done) / 1000) }]
    set elapsed [expr { $elapsed / 1000 }]
  } else {
    set elapsed [expr { $elapsed / 1000 }]
    set estimated $elapsed
  }

  if {$::progressCanvas(show) != ""} {
    if {$elapsed == 0 && $estimated < 2 && $msg == ""} { return }
    eval $::progressCanvas(show)
    set ::progressCanvas(show) {}
  }

  set width [expr { int(double($::progressCanvas(w)) * double($done)) }]
  $::progressCanvas(name) coords bar 0 0 $width $::progressCanvas(h)

  set t [format "%d:%02d / %d:%02d" \
      [expr {$elapsed / 60}] [expr {$elapsed % 60}] \
      [expr {$estimated / 60}] [expr {$estimated % 60}]]
  $::progressCanvas(name) itemconfigure time -text $t

  if {$msg != ""} {
    catch {
      set widget "$::progressCanvas(name)msg"
      grid $widget
      append widget ".text"
      $widget insert end "$msg\n"
    }
  }

  update

  if {! [winfo exists $::progressCanvas(name)] || $::progressCanvas(cancel)} {
    #Interrupted
    return -code break
  }
}

proc changeProgressWindow {newtext} {
  set w .progressWin
  if {[winfo exists $w]} {
    $w.f.t configure -text $newtext
    update idletasks
  }
}

proc updateProgressWindow {done total} {
  set w .progressWin
  if {! [winfo exists $w]} { return }
  if {$total != 0} {
    set done [expr { double($done) / double($total) }]
  } else {
    set done 1
  }
  ::progressCallBack $done
}

proc closeProgressWindow {{force false}} {
  set w .progressWin
  if {! [winfo exists $w]} { return }

  if {!$force && [$w.f.cmsg.text index end] != "2.0" } {
    $w.f.cancel configure -text "$::tr(Close)"
    $w.f.cancel configure -command "closeProgressWindow true"
    grid forget $w.f.c
    grid $w.f.cancel
    $w.f.cmsg.text configure -state disabled
    return
  }
  grab release $w
  destroy $w
  update idletasks
  catch {focus $::progressWin_focus}
}

proc CreateSelectDBWidget {{w} {varname} {ref_base ""} {readOnly 1}} {
  set listbases {}
  if {$ref_base == ""} { set ref_base [sc_base current] }
  set tr_database [tr Database]
  set tr_prefix_len [expr {[string length $tr_database] + 1}]
  set selected 0
  foreach i [sc_base list] {
      if {$readOnly || ![sc_base isReadOnly $i]} {
        set fname [file tail [sc_base filename $i]]
        if {$i == $ref_base} { set selected [llength $listbases] }
        lappend listbases "$tr_database $i: $fname"
      }
  }

  ttk::combobox $w.lb -values $listbases -state readonly
  grid $w.lb -sticky news
  grid columnconfigure $w 0 -weight 1

  bind $w.lb <<ComboboxSelected>> "
    set $varname \[ string index \[$w.lb get\] $tr_prefix_len \]
  "
  $w.lb current $selected
  event generate $w.lb <<ComboboxSelected>>
}
proc storeEmtComment { h m s } {
    set time "[format "%d" $h]:[format "%02d" $m]:[format "%02d" $s]"

    #Replace %emt if present, otherwise prepend it
    if {[regsub {\[%emt\s*.*?\]} [sc_pos getComment] "\[%emt $time\]" comment]} {
      sc_pos setComment "$comment"
    } else {
      sc_pos setComment "\[%emt $time\]$comment"
    }
  }
proc storeEvalComment { value } {
    #Replace %eval if present, otherwise prepend it
    if {[regsub {\[%eval\s*.*?\]} [sc_pos getComment] "\[%eval $value\]" comment]} {
      sc_pos setComment "$comment"
    } else {
      sc_pos setComment "\[%eval $value\]$comment"
    }
  }

################################################################################
# clock widget
################################################################################
namespace eval gameclock {
  array set data {}
  ################################################################################
  proc new { parent n { size 100 } {showfall 0} } {
    global ::gameclock::data
    set data(showfallen$n) $showfall
    set data(id$n) ""
    if {$parent != ""} {
      set data(id$n) $parent.clock$n
      canvas $data(id$n) -height $size -width $size
      pack $data(id$n) -side top -anchor center
      for {set i 1} {$i<13} {incr i} {
        set a [expr {$i/6.*acos(-1)}]
        set x [expr { ($size/2 + (($size-15)/2)*sin($a) ) }]
        set y [expr { ($size/2 - (($size-15)/2)*cos($a) ) }]
        $data(id$n) create text $x $y -text $i -tag clock$n
      }
      bind $data(id$n) <Button-1> "::gameclock::toggleClock $n"
    }
    set data(fg$n) "black"
    set data(running$n) 0
    set data(digital$n) 1
    ::gameclock::reset $n
    ::gameclock::draw $n
  }
  ################################################################################
  proc draw { n } {
    global ::gameclock::data

    #TODO: Hack. For the moment we assume that:
    # -clock 1 is the white clock on the main board
    # -clock 2 is the black clock on the main board
    set sec $data(counter$n)
    set h [format "%d" [expr abs($sec) / 60 / 60] ]
    set m [format "%02d" [expr (abs($sec) / 60) % 60] ]
    set s [format "%02d" [expr abs($sec) % 60] ]
    if {$n == 1} { set ::gamePlayers(clockW) "$h:$m:$s" }
    if {$n == 2} { set ::gamePlayers(clockB) "$h:$m:$s" }

    if {! [winfo exists $data(id$n)]} { return }
    $data(id$n) delete aig$n
    
    set w [$data(id$n) cget -width ]
    set h [$data(id$n) cget -height ]
    set cx [ expr $w / 2 ]
    set cy [ expr $h / 2 ]
    if {$w < $h} {
      set size [ expr $w - 15 ]
    } else  {
      set size [ expr $h - 15 ]
    }

    if { $sec > 0 && $data(showfallen$n) } {
      set color "red"
    } else  {
      set color $::gameclock::data(fg$n)
    }
    
    if {$color == "white"} {set fg "black"} else {set fg "white"}
    
    foreach divisor {30 1800 21600} length "[expr $size/2 * 0.8] [expr $size/2 * 0.7] [expr $size/2 * 0.4]" \
        width {1 2 3} {
          set angle [expr {$sec * acos(-1) / $divisor}]
          set x [expr {$cx + $length * sin($angle)}]
          set y [expr {$cy - $length * cos($angle)}]
          $data(id$n) create line $cx $cy $x $y -width $width -tags aig$n -fill $color
        }
    # draw a digital clock
    if {$data(digital$n)} {
      set m [format "%02d" [expr abs($sec) / 60] ]
      set s [format "%02d" [expr abs($sec) % 60] ]
      $data(id$n) create text $cx [expr $cy + $size/4 ] -text "$m:$s" -anchor center -fill $color -tag aig$n
    }
  }
  ################################################################################
  proc every {ms body n} {
    incr ::gameclock::data(counter$n)
    eval $body
    if {$::gameclock::data(id$n) == "" ||
        [winfo exists $::gameclock::data(id$n)]} {
      after $ms [info level 0]
    }
  }
  ################################################################################
  proc getSec { n } {
    return [expr 0 - $::gameclock::data(counter$n)]
  }
  ################################################################################
  proc setSec { n value } {
    set ::gameclock::data(counter$n) $value
    ::gameclock::draw $n
  }
  ################################################################################
  proc add { n value } {
    set ::gameclock::data(counter$n) [ expr $::gameclock::data(counter$n) - $value ]
    ::gameclock::draw $n
  }
  
  ################################################################################
  proc reset { n } {
    ::gameclock::stop $n
    set ::gameclock::data(counter$n) 0
  }
  ################################################################################
  proc start { n } {
    if {$::gameclock::data(running$n)} { return }
    set ::gameclock::data(running$n) 1
    ::gameclock::every 1000 "draw $n" $n
  }
  ################################################################################
  proc stop { n } {
    if {! $::gameclock::data(running$n)} { return 0 }
    set ::gameclock::data(running$n) 0
    after cancel "::gameclock::every 1000 \{draw $n\} $n"
    return 1
  }
  ################################################################################
  proc storeTimeComment { color } {
    set sec [::gameclock::getSec $color]
    set h [format "%d" [expr abs($sec) / 60 / 60] ]
    set m [format "%02d" [expr (abs($sec) / 60) % 60] ]
    set s [format "%02d" [expr abs($sec) % 60] ]
    set time "$h:$m:$s"

    #Replace %clk if present, otherwise prepend it
    if {[regsub {\[%clk\s*.*?\]} [sc_pos getComment] "\[%clk $time\]" comment]} {
      sc_pos setComment "$comment"
    } else {
      sc_pos setComment "\[%clk $time\]$comment"
    }
  }
  ################################################################################
  proc toggleClock { n } {
    if { $::gameclock::data(running$n) } {
      stop $n
    } else  {
      start $n
    }
  }
  ################################################################################
  proc setColor { n color } {
    if {$color == "white"} {
      set fg "black"
      set bg "white"
    } else {
      set fg "white"
      set bg "black"
    }
    set ::gameclock::data(fg$n) $fg
    $::gameclock::data(id$n) configure -background $bg
    $::gameclock::data(id$n) itemconfigure clock$n -fill $fg
    $::gameclock::data(id$n) itemconfigure aig$n -fill $fg
  }
  ################################################################################
  proc isRunning { } {
    global ::gameclock::data
    catch {
      if {$data(running1) || $data(running2)} { return 1 }
    }
    return 0
  }
}
################################################################################
# html generation
################################################################################
namespace eval html {
  set data {}
  set idx 0
  
  ################################################################################
  proc exportCurrentFilter {} {
    # Check that we have some games to export:
    if {[sc_filter count] == 0} {
      tk_messageBox -title "Scid: Filter empty" -type ok -icon info \
          -message "The filter contains no games."
      return
    }
    set ftype {
      { "HTML files" {".html" ".htm"} }
      { "All files" {"*"} }
    }
    set idir $::initialDir(html)
    set fName [tk_getSaveFile -initialdir $idir -filetypes $ftype -defaultextension ".html" -title "Create an HTML file"]
    if {$fName == ""} { return }
    if {[file extension $fName] != ".html" } {
      append fName ".html"
    }
    set prefix [file rootname [file tail $fName] ]
    set dirtarget [file dirname $fName]
    set sourcedir [file join $::scidExeDir html]
    catch {file copy -force [file join $sourcedir bitmaps] $dirtarget}
    catch {file copy -force [file join $sourcedir scid.js] $dirtarget}
    catch {file copy -force [file join $sourcedir scid.css] $dirtarget}
    # writeIndex "[file join $dirtarget $prefix].html" $prefix
    progressWindow "Scid" "Exporting games..."
    set savedGameNum [sc_game number]
    set gn [sc_filter first]
    set players {}
    set ::html::cancelHTML 0
    set total [sc_filter count]
    
    # build the list of matches
    set idx 1
    while {$gn != 0 && ! $::html::cancelHTML} {
      updateProgressWindow $idx $total
      sc_game load $gn
      set pl "[sc_game tags get White] - [sc_game tags get Black]"
      lappend players $pl
      set gn [sc_filter next]
      incr idx
    }
    
    set idx 1
    set gn [sc_filter first]
    while {$gn != 0 && ! $::html::cancelHTML} {
      updateProgressWindow $idx $total
      sc_game load $gn
      fillData
      set pl "[sc_game tags get White] - [sc_game tags get Black]"
      toHtml $::html::data $idx $dirtarget $prefix $players $pl [sc_game tags get "Event"] [sc_game tags get "ECO"] [sc_game info result] [sc_game tags get "Date"]
      set gn [sc_filter next]
      incr idx
    }
    
    closeProgressWindow
    exportPGN "[file join $dirtarget $prefix].pgn" "filter"
    sc_game load $savedGameNum
  }
  ################################################################################
  proc sc_progressBar {} {
    set ::html::cancelHTML 1
  }
  ################################################################################
  proc exportCurrentGame {} {
    
    set ftype {
      { "HTML files" {".html" ".htm"} }
      { "All files" {"*"} }
    }
    set idir $::initialDir(html)
    set fName [tk_getSaveFile -initialdir $idir -filetypes $ftype -defaultextension ".html" -title "Create an HTML file"]
    if {[file extension $fName] != ".html" && [file extension $fName] != ".htm" } {
      append fName ".html"
    }
    if {$fName == ""} { return }
    set prefix [file rootname [file tail $fName] ]
    set dirtarget [file dirname $fName]
    set sourcedir [file join $::scidExeDir html]
    catch { file copy -force [file join $sourcedir bitmaps] $dirtarget }
    catch { file copy -force [file join $sourcedir scid.js] $dirtarget }
    catch { file copy -force [file join $sourcedir scid.css] $dirtarget }
    
    fillData
    set players [list "[sc_game tags get White] - [sc_game tags get Black]"]
    toHtml $::html::data -1 $dirtarget $prefix $players [lindex $players 0] \
        [sc_game tags get "Event"] [sc_game tags get "ECO"] \
        [sc_game info result] [sc_game tags get "Date"]
    exportPGN "[file join $dirtarget $prefix].pgn" "current"
  }
  ################################################################################
  # Dictionary mapping from special characters to their entities. (from tcllib)
  variable entities {
    \xa0 &nbsp; \xa1 &iexcl; \xa2 &cent; \xa3 &pound; \xa4 &curren;
    \xa5 &yen; \xa6 &brvbar; \xa7 &sect; \xa8 &uml; \xa9 &copy;
    \xaa &ordf; \xab &laquo; \xac &not; \xad &shy; \xae &reg;
    \xaf &macr; \xb0 &deg; \xb1 &plusmn; \xb2 &sup2; \xb3 &sup3;
    \xb4 &acute; \xb5 &micro; \xb6 &para; \xb7 &middot; \xb8 &cedil;
    \xb9 &sup1; \xba &ordm; \xbb &raquo; \xbc &frac14; \xbd &frac12;
    \xbe &frac34; \xbf &iquest; \xc0 &Agrave; \xc1 &Aacute; \xc2 &Acirc;
    \xc3 &Atilde; \xc4 &Auml; \xc5 &Aring; \xc6 &AElig; \xc7 &Ccedil;
    \xc8 &Egrave; \xc9 &Eacute; \xca &Ecirc; \xcb &Euml; \xcc &Igrave;
    \xcd &Iacute; \xce &Icirc; \xcf &Iuml; \xd0 &ETH; \xd1 &Ntilde;
    \xd2 &Ograve; \xd3 &Oacute; \xd4 &Ocirc; \xd5 &Otilde; \xd6 &Ouml;
    \xd7 &times; \xd8 &Oslash; \xd9 &Ugrave; \xda &Uacute; \xdb &Ucirc;
    \xdc &Uuml; \xdd &Yacute; \xde &THORN; \xdf &szlig; \xe0 &agrave;
    \xe1 &aacute; \xe2 &acirc; \xe3 &atilde; \xe4 &auml; \xe5 &aring;
    \xe6 &aelig; \xe7 &ccedil; \xe8 &egrave; \xe9 &eacute; \xea &ecirc;
    \xeb &euml; \xec &igrave; \xed &iacute; \xee &icirc; \xef &iuml;
    \xf0 &eth; \xf1 &ntilde; \xf2 &ograve; \xf3 &oacute; \xf4 &ocirc;
    \xf5 &otilde; \xf6 &ouml; \xf7 &divide; \xf8 &oslash; \xf9 &ugrave;
    \xfa &uacute; \xfb &ucirc; \xfc &uuml; \xfd &yacute; \xfe &thorn;
    \xff &yuml; \u192 &fnof; \u391 &Alpha; \u392 &Beta; \u393 &Gamma;
    \u394 &Delta; \u395 &Epsilon; \u396 &Zeta; \u397 &Eta; \u398 &Theta;
    \u399 &Iota; \u39A &Kappa; \u39B &Lambda; \u39C &Mu; \u39D &Nu;
    \u39E &Xi; \u39F &Omicron; \u3A0 &Pi; \u3A1 &Rho; \u3A3 &Sigma;
    \u3A4 &Tau; \u3A5 &Upsilon; \u3A6 &Phi; \u3A7 &Chi; \u3A8 &Psi;
    \u3A9 &Omega; \u3B1 &alpha; \u3B2 &beta; \u3B3 &gamma; \u3B4 &delta;
    \u3B5 &epsilon; \u3B6 &zeta; \u3B7 &eta; \u3B8 &theta; \u3B9 &iota;
    \u3BA &kappa; \u3BB &lambda; \u3BC &mu; \u3BD &nu; \u3BE &xi;
    \u3BF &omicron; \u3C0 &pi; \u3C1 &rho; \u3C2 &sigmaf; \u3C3 &sigma;
    \u3C4 &tau; \u3C5 &upsilon; \u3C6 &phi; \u3C7 &chi; \u3C8 &psi;
    \u3C9 &omega; \u3D1 &thetasym; \u3D2 &upsih; \u3D6 &piv;
    \u2022 &bull; \u2026 &hellip; \u2032 &prime; \u2033 &Prime;
    \u203E &oline; \u2044 &frasl; \u2118 &weierp; \u2111 &image;
    \u211C &real; \u2122 &trade; \u2135 &alefsym; \u2190 &larr;
    \u2191 &uarr; \u2192 &rarr; \u2193 &darr; \u2194 &harr; \u21B5 &crarr;
    \u21D0 &lArr; \u21D1 &uArr; \u21D2 &rArr; \u21D3 &dArr; \u21D4 &hArr;
    \u2200 &forall; \u2202 &part; \u2203 &exist; \u2205 &empty;
    \u2207 &nabla; \u2208 &isin; \u2209 &notin; \u220B &ni; \u220F &prod;
    \u2211 &sum; \u2212 &minus; \u2217 &lowast; \u221A &radic;
    \u221D &prop; \u221E &infin; \u2220 &ang; \u2227 &and; \u2228 &or;
    \u2229 &cap; \u222A &cup; \u222B &int; \u2234 &there4; \u223C &sim;
    \u2245 &cong; \u2248 &asymp; \u2260 &ne; \u2261 &equiv; \u2264 &le;
    \u2265 &ge; \u2282 &sub; \u2283 &sup; \u2284 &nsub; \u2286 &sube;
    \u2287 &supe; \u2295 &oplus; \u2297 &otimes; \u22A5 &perp;
    \u22C5 &sdot; \u2308 &lceil; \u2309 &rceil; \u230A &lfloor;
    \u230B &rfloor; \u2329 &lang; \u232A &rang; \u25CA &loz;
    \u2660 &spades; \u2663 &clubs; \u2665 &hearts; \u2666 &diams;
    \x22 &quot; \x26 &amp; \x3C &lt; \x3E &gt; \u152 &OElig;
    \u153 &oelig; \u160 &Scaron; \u161 &scaron; \u178 &Yuml;
    \u2C6 &circ; \u2DC &tilde; \u2002 &ensp; \u2003 &emsp; \u2009 &thinsp;
    \u200C &zwnj; \u200D &zwj; \u200E &lrm; \u200F &rlm; \u2013 &ndash;
    \u2014 &mdash; \u2018 &lsquo; \u2019 &rsquo; \u201A &sbquo;
    \u201C &ldquo; \u201D &rdquo; \u201E &bdquo; \u2020 &dagger;
    \u2021 &Dagger; \u2030 &permil; \u2039 &lsaquo; \u203A &rsaquo;
    \u20AC &euro;
  }
  proc html_entities {s} {
    variable entities
    return [string map $entities $s]
  }
  ################################################################################
  proc toHtml { dt game dirtarget prefix {players ""} {this_players ""} {event ""} {eco "ECO"} {result "*"} {date ""} } {
    
    if { $game != -1 } {
      set f [open "[file join $dirtarget $prefix]_${game}.html" w]
    } else  {
      set f [open "[file join $dirtarget $prefix].html" w]
    }
    # header
    puts $f "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">"
    puts $f "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">"
    puts $f "<head>"
    puts $f "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />"
    puts $f "<link rel=\"stylesheet\" type=\"text/css\" href=\"scid.css\" />"
    puts $f "<script src=\"scid.js\" type=\"text/javascript\"></script>"
    puts $f "<script type=\"text/javascript\">"
    puts $f "// <!\[CDATA\["
    puts $f "movesArray = new Array("
    for {set i 0} {$i<[llength $dt]} {incr i} {
      array set elt [lindex $dt $i]
      puts -nonewline $f "\"$elt(fen) $elt(prev) $elt(next)\""
      if {$i < [expr [llength $dt] -1]} { puts $f "," }
    }
    puts $f ");"
    puts $f "var current = 0;"
    puts $f "var prefix = \"$prefix\";"
    puts $f "// \]\]>"
    puts $f "</script>"
    puts $f "<title>Scid</title>"
    puts $f "<meta content=\"Scid\" name=\"author\" />"
    puts $f "</head>"
    puts $f "<body onload=\"doinit()\" onkeydown=\"handlekey(event)\">"
    puts $f "<div id=\"framecontent\">"
    puts $f "<div class=\"innertube\">"
    # diagram
    puts $f "<div id=\"diagram\"><!-- diagram goes here --></div>"
    # navigation
    puts $f "<div id=\"nav\" style=\"text-align: center\"><!-- navigation goes here -->"
    puts $f "<form action=\"#\">"
    puts $f "<p>"
    puts $f "<input type='button' value=' &darr;&uarr; ' onclick='rotate()' /> <input type='button' value=' |&lt; ' onclick='jump(0)' /> <input type='button' value=' &lt; ' onclick='moveForward(0)' /> <input type='button' value=' &gt; ' onclick='moveForward(1)' /> <input type='button' value=' &gt;| ' onclick='jump(1)' /> "
    puts $f "</p><p>"
    # other games navigation
    puts $f "<select name=\"gameselect\" id=\"gameselect\" size=\"1\" onchange=\"gotogame()\">"
    set i 1
    foreach l $players {
      if { $game == $i } {
        puts $f "<option  selected=\"selected\">$i. [html_entities $l]</option>"
      } else  {
        puts $f "<option>$i. [html_entities $l]</option>"
      }
      incr i
    }
    puts $f "</select>"
    puts $f "</p><p>"
    puts $f "<input type=\"button\" value=\"&lt;--\" onclick=\"gotoprevgame()\" /> &nbsp; <input type=\"button\" value=\"--&gt;\" onclick=\"gotonextgame()\" />"
    puts $f "</p><p>"
    puts $f "<a href=\"${prefix}.pgn\">${prefix}.pgn</a>"
    puts $f "</p>"
    puts $f "</form>"
    puts $f "</div>"
    puts $f "</div>"
    puts $f "</div>"
    puts $f "<div id=\"maincontent\">"
    puts $f "<div class=\"innertube\">"
    puts $f "<div id=\"moves\"><!-- moves go here -->"
    # game header
    puts $f "<span class=\"hPlayers\"> [html_entities $this_players]</span>"
    puts $f "<span class=\"hEvent\"><br /> [html_entities $event]</span>"
    puts $f "<span class=\"hEvent\"><br />\[$date\]</span>"
    puts $f "<span class=\"hAnnot\"><br />\[$eco\]</span>"
    puts $f "<p>"
    # link moves
    set prevdepth 0
    set prevvarnumber 0
    for {set i 0} {$i<[llength $dt]} {incr i} {
      array set elt [lindex $dt $i]
      if {$elt(depth) == 0} {
        set class "V0"
      } elseif {$elt(depth) == 1} {
        set class "V1"
      } else {
        set class "V2"
      }
      if { $prevdepth == $elt(depth) && $prevvarnumber != $elt(var) } {
        puts $f "<span class=\"VC\">\]</span></div>"
        puts $f "<div class=\"var\"><span class=\"VC\">\[</span>"
      } else {
        while { $prevdepth > $elt(depth) } {
            puts $f "<span class=\"VC\">\]</span></div>"
            set prevdepth [expr $prevdepth - 1]
        }
        while { $prevdepth < $elt(depth) } {
            puts $f "<div class=\"var\"><span class=\"VC\">\[</span>"
            set prevdepth [expr $prevdepth + 1]
        }
      }
      set prevvarnumber $elt(var)
      # id = "mv1" not "id=1" now
      set nag [html_entities $elt(nag)]
      set comment [html_entities $elt(comment)]
      puts $f "<a href=\"javascript:gotoMove($elt(idx))\" id=\"mv$elt(idx)\" class=\"$class\">$elt(move)$nag</a>"
      if {$elt(diag)} {
        insertMiniDiag $elt(fen) $f
      }
      if {$comment != ""} {
        puts $f "<span class=\"VC\">$comment</span>"
      }
    }
    while { $prevdepth > 0 } {
        puts $f "<span class=\"VC\">\]</span></div>"
        set prevdepth [expr $prevdepth - 1]
    }

    puts $f "<br /><span class=\"VH\">$result</span>"
    puts $f "<p>"
    puts $f "<a href=\"http://scid.sourceforge.net/\" style=\"font-size: 0.8em\">Created with Scid</a>"
    puts $f "</div>"
    puts $f "</div>"
    puts $f "</div>"
    puts $f "</body>"
    puts $f "</html>"
    close $f
  }
  ################################################################################
  proc colorSq {sq} {
    if { [expr $sq % 2] == 1 && [expr int($sq / 8) %2 ] == 0 || [expr $sq % 2] == 0 && [expr int($sq / 8) %2 ] == 1 } {
      return "bs"
    } else {
      return "ws"
    }
  }
  ################################################################################
  proc piece2gif {piece} {
    if {$piece == "K"} { return "wk" }
    if {$piece == "k"} { return "bk" }
    if {$piece == "Q"} { return "wq" }
    if {$piece == "q"} { return "bq" }
    if {$piece == "R"} { return "wr" }
    if {$piece == "r"} { return "br" }
    if {$piece == "B"} { return "wb" }
    if {$piece == "b"} { return "bb" }
    if {$piece == "N"} { return "wn" }
    if {$piece == "n"} { return "bn" }
    if {$piece == "P"} { return "wp" }
    if {$piece == "p"} { return "bp" }
    if {$piece == " "} { return "sq" }
  }
  ################################################################################
  proc insertMiniDiag {fen f} {
    
    set square 0
    set space " "
    puts $f "<table Border=0 CellSpacing=0 CellPadding=0><tr>"
    
    for {set i 0} {$i < [string length $fen]} {incr i} {
      set l [string range $fen $i $i ]
      set res [scan $l "%d" c]
      if {$res == 1} {
        if  { $c >= 1 && $c <= 8 } {
          for { set j 0} {$j < $c} {incr j} {
            puts $f "<td class=\"[colorSq $square]\"><img border=0 align=\"left\" src=\"bitmaps/mini/[piece2gif $space].gif\"></td>"
            incr square
          }
        }
      } elseif {$l == "/"}  {
        puts $f "</tr><tr>"
      } else  {
        puts $f "<td class=\"[colorSq $square]\"><img border=0 align=\"left\" src=\"bitmaps/mini/[piece2gif $l].gif\"></td>"
        incr square
      }
    }
    
    puts $f "</tr></table>"
  }
  
  ################################################################################
  # fill data with { idx FEN prev next move nag comment depth }
  proc fillData {} {
    set ::html::data {}
    set ::html::idx -1
    sc_move start
    parseGame
  }
  
  ################################################################################
  proc parseGame { {prev -2} } {
    global ::html::data ::html::idx
    
    set already_written 0
    
    set dots 0
    
    while {1} {
      if { ! $already_written } {
        recordElt $dots $prev
        set dots 0
        set prev -2
      } else {
        set dots 1
      }
      set already_written 0
      
      # handle variants
      if {[sc_var count]>0} {
        # First write the move in the current line for which variations exist
        #
        if { ![sc_pos isAt vend]} {
          sc_move forward
          recordElt $dots $prev
          sc_move back
          set lastIdx $idx
          set already_written 1
        }
        for {set v 0} {$v<[sc_var count]} {incr v} {
          sc_var enter $v
          # in order to get the comment before first move
          sc_move back
          parseGame -1
          sc_var exit
        }
        #update the "next" token
        array set elt [lindex $data $lastIdx]
        set elt(next) [expr $idx + 1]
        lset data $lastIdx [array get elt]
        #update the "previous" token
        set prev $lastIdx
      }
      
      if {[sc_pos isAt vend]} { break }
      sc_move forward
    }
  }
  ################################################################################
  proc recordElt { dots {prev -2} } {
    global ::html::data ::html::idx
    
    array set elt {}
    
    incr idx
    set elt(idx) $idx
    set elt(fen) [lindex [split [sc_pos fen]] 0]
    if {$prev != -2} {
      set elt(prev) $prev
    } else  {
      set elt(prev) [expr $idx-1]
    }
    
    set nag [sc_pos getNags]
    if {$nag == "0"} { set nag "" }
    if {[string match "*D *" $nag] || [string match "*# *" $nag]} {
      set elt(diag) 1
    } else  {
      set elt(diag) 0
    }
    set nag [regsub -all "D " $nag "" ]
    set nag [regsub -all "# " $nag "" ]
    set elt(nag) $nag
    set comment [sc_pos getComment]
    set comment [regsub -all "\[\x5B\]%draw (.)+\[\x5D\]" $comment ""]
    set elt(comment) $comment
    set elt(depth) [sc_var level]
    set elt(var) [sc_var number]
    if {![sc_pos isAt vend]} {
      set elt(next) [expr $idx +1 ]
    } else  {
      set elt(next) -1
    }
    
    set m [sc_game info previousMove]
    set mn [sc_pos moveNumber]
    
    set elt(move) ""
    if {[sc_pos side] == "black" && $m != ""} {
      set elt(move) "$mn.$m"
    } else {
      
      if {! [sc_pos isAt vstart] } {
        sc_move back
        set pnag [sc_pos getNags]
        if {$pnag == "0"} { set pnag "" }
        if {[string match "*D *" $pnag] || [string match "*# *" $pnag]} {
          set pdiag 1
        } else  {
          set pdiag 0
        }
        if {  [sc_pos isAt vstart] ||  [sc_pos getComment] != "" || $pdiag > 0 } {
          set dots 1
        }
        sc_move forward
      }
      
      if {$dots && $m != ""} {
        set elt(move) "[expr $mn -1]. ... $m"
      } else  {
        set elt(move) $m
      }
      
    }
    
    lappend ::html::data [array get elt]
    
  }
  
  ################################################################################
  # proc writeIndex {fn prefix} {
  # set f [open $fn w]
  # puts $f "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">"
  # puts $f "<html>"
  # puts $f "<head>"
  # puts $f "<meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">"
  # puts $f "<title>Scid</title>"
  # puts $f "<meta content=\"Scid\" name=\"author\">"
  # puts $f "</head>"
  # puts $f "<frameset BORDER=\"0\" FRAMEBORDER=\"0\" FRAMESPACING=\"0\" COLS=\"380,*\">"
  # puts $f "<frameset BORDER=\"0\" FRAMEBORDER=\"0\" FRAMESPACING=\"0\" ROWS=\"380,*\">"
  # puts $f "<frame NAME=\"diagram\" SCROLLING=\"Auto\">"
  # puts $f "<frame NAME=\"nav\" SRC=\"${prefix}_nav.html\" SCROLLING=\"Auto\">"
  # puts $f "</frameset>"
  # puts $f "<frame NAME=\"moves\" SRC=\"${prefix}_1.html\" SCROLLING=\"Auto\">"
  # puts $f "</frameset>"
  # puts $f "</html>"
  # close $f
  # }
  ################################################################################
  proc exportPGN { fName selection } {
    if {$selection == "filter"} {
      progressWindow "Scid" "Exporting games..." $::tr(Cancel)
    }
    sc_base export $selection "PGN" $fName -append 0 -starttext "" -endtext "" -comments 1 -variations 1 \
        -space 1 -symbols 1 -indentC 0 -indentV 0 -column 0 -noMarkCodes 1 -convertNullMoves 1
    if {$selection == "filter"} {
      closeProgressWindow
    }
  }
  
}
################################################################################
#
################################################################################

# end of misc.tcl
