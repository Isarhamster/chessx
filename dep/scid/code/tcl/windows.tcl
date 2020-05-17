###
### windows.tcl: part of Scid.
### Copyright (C) 1999-2003  Shane Hudson.
###


########################################################################
###  Optional windows: all off initially.

set treeWin 0
set pgnWin 0
set filterGraph 0

################################################################################
# Creates a toplevel window depending of the docking option
################################################################################
proc createToplevel { {w} {closeto ""} } {
  # Raise window if already exist
  if { [winfo exists $w] } {
    lassign [::win::isDocked $w] docked_nb w
    if {$docked_nb ne ""} {
        $docked_nb select $w
    } else {
        wm deiconify $w
    }
    return "already_exists"
  }

  set f ".fdock[string range $w 1 end]"
  frame $f  -container 1
  toplevel $w -use [ winfo id $f ]
  keyboardShortcuts $w

  # Set default width and height values, if they do not exists
  if {![info exists ::winGeometry($f)]} {
    set ::winGeometry($f) ""
  }

  ::win::manageWindow $f ""
}

################################################################################
# In the case of a window closed without the context menu in docked mode, arrange for the tabs to be cleaned up
# This function is necessary only if does exists a "destroy" command for the win created with createToplevel
################################################################################
proc createToplevelFinalize {w} {
    bind $w <Destroy> "+if {\[string equal $w %W\]} {
      cleanup_todo_remove $w
    }"
}
proc cleanup_todo_remove { w } {
    set dockw ".fdock[string range $w 1 end]"
    set tab [::docking::find_tbn $dockw]
    if {$tab != ""} {
      $tab forget $dockw
      ::docking::_cleanup_tabs $tab
    }
    after idle "if {[winfo exists $dockw]} { destroy $dockw }"
    catch { focus .main }
}

# recordWinSize:
#   Records window width and height, for saving in options file.
#
proc recordWinSize {win} {
  global winWidth winHeight winX winY
  if {![winfo exists $win]} { return }
  set temp [wm geometry $win]
  
  set suffix ""
  set n [scan $temp "%dx%d+%d+%d" width height x y]
  if {$n == 4} {
    set winWidth${suffix}($win) $width
    set winHeight${suffix}($win) $height
    set winX${suffix}($win) $x
    set winY${suffix}($win) $y
  }
}

proc setWinLocation {win} {
  global winX winY
  set suffix ""
  if {[info exists winX${suffix}($win)]  &&  [info exists winY${suffix}($win)]  && \
        [set winX${suffix}($win)] >= 0  &&  [set winY${suffix}($win)] >= 0} {
    catch [list wm geometry $win "+[set winX${suffix}($win)]+[set winY${suffix}($win)]"]
  }
}

proc setWinSize {win} {
  global winWidth winHeight
  set suffix ""
  if {[info exists winWidth${suffix}($win)]  &&  [info exists winHeight${suffix}($win)]  &&  \
        [set winWidth${suffix}($win) ] > 0  &&  [set winHeight${suffix}($win) ] > 0 } {
    catch [list wm geometry $win "[set winWidth${suffix}($win) ]x[set winHeight${suffix}($win) ]"]
  }
}

###
### End of file: windows.tcl
###
