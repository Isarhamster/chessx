
####################
# Recent files list:

set recentFiles(limit) 10   ;# Maximum number of recent files to remember.
set recentFiles(menu)   9   ;# Maximum number of files to show in File menu.
set recentFiles(extra)  9   ;# Maximum number of files to show in extra menu.
set recentFiles(data)  {}   ;# List of recently used files.

catch {source [scidConfigFile recentfiles]}

namespace eval ::recentFiles {}

# ::recentFiles::save
#   Saves the recent-file-list file, reporting any error in a message box
#   if reportError is true.
#
proc ::recentFiles::save {{reportError 0}} {
  global recentFiles
  set f {}
  set filename [scidConfigFile recentfiles]
  if  {[catch {open $filename w} f]} {
    if {$reportError} {
      tk_messageBox -title "Scid" -type ok -icon warning \
          -message "Unable to write file: $filename\n$f"
    }
    return
  }
  puts $f "# Scid $::scidVersion recent files list"
  puts $f ""
  foreach i {limit menu extra data} {
    puts $f "set recentFiles($i) [list [set recentFiles($i)]]"
    puts $f ""
  }
  if {[info exists ::recentSort]}  {
    puts $f "set ::recentSort [list $::recentSort]"
  }
  ::file::autoLoadBases.save $f
  close $f
}

# ::recentFiles::add
#   Adds a file to the recent files list, or moves it to the front
#   if that file is already in the list.
#
proc ::recentFiles::add {fname} {
  global recentFiles
  
  if {$fname == "" } { return }
  
  set rlist $recentFiles(data)
  
  # Remove file to be added from its current place in the
  # list, if it is there:
  while {1} {
    set idx [lsearch -exact $rlist $fname]
    if {$idx < 0} { break }
    set rlist [lreplace $rlist $idx $idx]
  }
  
  # Insert the current file at the start of the list:
  set rlist [linsert $rlist 0 $fname]
  
  # Trim the list if necessary:
  if {[llength $rlist] < $recentFiles(limit)} {
    set rlist [lrange $rlist 0 [expr {$recentFiles(limit) - 1} ]]
  }
  
  set recentFiles(data) $rlist
  # ::recentFiles::save
}

# ::recentFiles::load
#   Loads the selected recent file, or switches to its database slot
#   if it is already open.
#
proc ::recentFiles::load {fname} {
  set rname $fname
  if {[file extension $rname] == ".si4"} {
    set rname [file rootname $rname]
  }
  ::file::Open $fname
}

#################################################################################
proc ::recentFiles::treeshow {menu} {
  global recentFiles
  set rlist $recentFiles(data)
  $menu delete 0 end
  set nfiles [llength $rlist]
  if {$nfiles > $recentFiles(limit)} { set nfiles $recentFiles(limit) }
  
  for {set i 0} {$i<$nfiles} {incr i} {
    set name [lindex $rlist $i]
    $menu add command -label "$name" -command [list ::file::openBaseAsTree $name]
  }
}

#################################################################################
# ::recentFiles::show
#   Adds the recent files to the end of the specified menu.
#   Returns the number of menu entries added.
#
proc ::recentFiles::show {menu idx} {
  global recentFiles
  set rlist $recentFiles(data)
  set nfiles [llength $rlist]
  set nExtraFiles [expr {$nfiles - $recentFiles(menu)} ]
  if {$nfiles > $recentFiles(menu)} { set nfiles $recentFiles(menu) }
  if {$nExtraFiles > $recentFiles(extra)} {
    set nExtraFiles $recentFiles(extra)
  }
  if {$nExtraFiles < 0} { set nExtraFiles 0 }
  
  # Add menu commands for the most recent files:
  
  for {set i 0} {$i < $nfiles} {incr i} {
    set fname [lindex $rlist $i]
    set mname [::recentFiles::menuname $fname]
    set text [file tail $fname]
    set num [expr {$i + 1} ]
    set underline -1
    if {$num <= 9} { set underline 0 }
    if {$num == 10} { set underline 1 }
    $menu insert $idx command -label "$num: $mname" -underline $underline \
        -command [list ::recentFiles::load $fname]
    set ::helpMessage($menu,$idx) "  [file nativename $fname]"
    incr idx
  }
  
  # If no extra submenu of recent files is needed, return now:
  if {$nExtraFiles <= 0} { return $nfiles }
  
  # Now add the extra submenu of files:
  catch {destroy $menu.recentFiles}
  menu $menu.recentFiles
  $menu insert $idx cascade -label "..." -underline 0 -menu $menu.recentFiles
  set i $nfiles
  for {set extra 0} {$extra < $nExtraFiles} {incr extra} {
    set fname [lindex $rlist $i]
    incr i
    set mname [::recentFiles::menuname $fname]
    set text [file tail $fname]
    set num [expr {$extra + 1} ]
    set underline -1
    if {$num <= 9} { set underline 0 }
    if {$num == 10} { set underline 1 }
    $menu.recentFiles add command -label "$num: $mname" -underline $underline \
        -command [list ::recentFiles::load $fname]
    set ::helpMessage($menu.recentFiles,$extra) "  $fname"
  }
  return [expr {$nfiles + 1} ]
}

# ::recentFiles::menuname
#   Given a full-path filename, returns a possibly shortened
#   version suitable for displaying in a menu, such as
#   "..../my/files/abc.pgn" instead of "/long/path/to/my/files/abc.pgn"
#
proc ::recentFiles::menuname {fname} {
  set mname $fname
  set mname [file nativename $mname]
  if {[file extension $mname] == [sc_info suffix index]} {
    set mname [file rootname $mname]
  }
  if {[string length $mname] < 25} { return $mname }
  
  # Generate a menu name " ..../path/filename" for the file:
  set dir [file dirname $fname]
  while {1} {
    set tail [file join [file tail $dir] $mname]
    set dir [file dirname $dir]
    if {[string length $tail] > 20} { break }
    set mname $tail
  }
  set mname [file join .... $mname]
  set mname [file nativename $mname]
  return $mname
}

# ::recentFiles::configure
#   Produces a dialog box for configuring the number of recent files
#   to display in the File menu and in a submenu.
#
proc ::recentFiles::configure { w } {
  global recentFiles

  set tmpcombo {}
  for {set x 1} {$x <= 10} {incr x} {
      lappend tmpcombo $x
  }
  ttk::label $w.lmenu -text $::tr(RecentFilesMenu)
  ttk::label $w.lextra -text $::tr(RecentFilesExtra)
  ttk::combobox $w.menu -textvariable recentFiles(menu) -width 2 -values $tmpcombo -justify right -state readonly
  ttk::combobox $w.extra -textvariable recentFiles(extra) -width 2 -values $tmpcombo -justify right -state readonly
  grid $w.lmenu -row 0 -column 0 -sticky w
  grid $w.menu -row 0 -column 1 -sticky w -pady 5 -padx 5
  grid $w.lextra -row 1 -column 0 -sticky w
  grid $w.extra -row 1 -column 1 -sticky w -pady 5 -padx 5
}
