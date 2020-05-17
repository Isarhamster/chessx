# bookmark.tcl:
# Bookmarks list and Recently-used files list in Scid.

set bookmarks(data) {}
set bookmarks(subMenus) 0

# Read the bookmarks file if it exists:
catch {source [scidConfigFile bookmarks]}


namespace eval ::bookmarks {}

# ::bookmarks::PostMenu:
#   Posts the bookmarks toolbar menu.
#
proc ::bookmarks::PostMenu {} {
  .main.tb.bkm.menu post [winfo pointerx .] [winfo pointery .]
  if {[::bookmarks::CanAdd]} {
    .main.tb.bkm.menu activate 0
  } else {
    .main.tb.bkm.menu activate 2
  }
}

# ::bookmarks::Refresh:
#   Updates all bookmarks submenus.
#
proc ::bookmarks::Refresh {} {
  foreach menu {.menu.file.bookmarks .main.tb.bkm.menu} {
    ::bookmarks::RefreshMenu $menu
  }
}

proc ::bookmarks::RefreshMenu {menu} {
  global bookmarks helpMessage

  ::bookmarks::DeleteChildren $menu
  $menu delete 0 end
  # $menu configure -disabledforeground [$menu cget -foreground]
  set numBookmarkEntries [llength $bookmarks(data)]
  $menu add command -label FileBookmarksAdd -command ::bookmarks::AddCurrent
  set helpMessage($menu,0) FileBookmarksAdd
  $menu add cascade -label FileBookmarksFile -menu $menu.file
  menu $menu.file
  set helpMessage($menu,1) FileBookmarksFile
  if {! [::bookmarks::CanAdd]} {
    $menu entryconfigure 0 -state disabled
    $menu entryconfigure 1 -state disabled
  }
  $menu add command -label FileBookmarksEdit -command ::bookmarks::Edit
  set helpMessage($menu,2) FileBookmarksEdit
  if {$bookmarks(subMenus)} {
    set display List
    set newval 0
  } else {
    set display Sub
    set newval 1
  }
  $menu add command -label FileBookmarks$display \
    -command "set bookmarks(subMenus) $newval; ::bookmarks::Refresh"
  set helpMessage($menu,3) FileBookmarks$display
  foreach tag [list Add File Edit $display] {
    configMenuText $menu FileBookmarks$tag FileBookmarks$tag $::language
  }
  if {$numBookmarkEntries == 0} { return }
  $menu add separator

  # Add each bookmark entry:
  set current $menu
  set inSubMenu 0
  set nfolders 0
  foreach entry $bookmarks(data) {
    if {$entry == ""} { continue }
    set isfolder [::bookmarks::isfolder $entry]

    if {$isfolder} {
      incr nfolders
      $menu.file add command -label [::bookmarks::Text $entry] \
        -command "::bookmarks::AddCurrent $nfolders"
    }

    if {! $bookmarks(subMenus)} {
      if {$isfolder} {
        $current add command -label [::bookmarks::IndexText $entry]
      } elseif {!$isfolder} {
        $current add command -label [::bookmarks::IndexText $entry] \
          -command [list ::bookmarks::Go $entry]
      }
      continue
    }

    # Move out of submenu where necessary:
    if {$isfolder  &&  $inSubMenu} {
      set current [winfo parent $current]
    }

    if {$isfolder} {
      # Menu (folder) entry:
      set current [::bookmarks::NewSubMenu $current $entry]
      set inSubMenu 1
    } else {
      # Bookmark entry:
      $current add command -label [::bookmarks::Text $entry] \
        -command [list ::bookmarks::Go $entry]
    }
  }
}

# ::bookmarks::CanAdd:
#   Returns 1 if the current game can be added as a bookmark.
#   It must be in an open database, not a PGN file, and not game number 0.
#
proc ::bookmarks::CanAdd {} {
  if {[sc_game number] == 0} { return 0 }
  if {$::curr_db == $::clipbase_db} { return 0 }
  set fname [sc_base filename $::curr_db]
  foreach suffix {.pgn .PGN} {
    if {[string match "*$suffix" "$fname"]} { return 0 }
  }
  return 1
}

# ::bookmarks::AddCurrent:
#   Adds the current game to the bookmarks list.
#
proc ::bookmarks::AddCurrent {{folder 0}} {
  global bookmarks
  if {! [sc_base inUse]} {
    return
  }
  set text [::bookmarks::New game]
  set len [llength $bookmarks(data)]
  set fcount 0
  for {set i 0} {$i < $len} {incr i} {
    if {[::bookmarks::isfolder [lindex $bookmarks(data) $i]]} {
      if {$fcount == $folder} { break }
      incr fcount
    }
  }
  set bookmarks(data) [linsert $bookmarks(data) $i $text]
  ::bookmarks::Save
  ::bookmarks::Refresh
}

# ::bookmarks::New:
#   Returns a bookmarks list entry for the current game or a new folder.
#
proc ::bookmarks::New {type} {
  if {$type == "folder"} { return [list "f" ""] }
  set text "[file tail [sc_base filename $::curr_db]]: [sc_game info result], "
  append text "[::utils::string::Surname [sc_game info white]] - "
  append text "[::utils::string::Surname [sc_game info black]], "
  append text "[::utils::string::CityName [sc_game info site]] "
  set round [sc_game info round]
  if {$round != ""  &&  $round != "?"} { append text "($round) " }
  append text "[sc_game info year]"
  set list [list "g" $text]
  sc_game pgn
  lappend list [sc_base filename $::curr_db] [sc_game number] [sc_pos pgnOffset]
  lappend list [sc_game info white] [sc_game info black]
  lappend list [sc_game info year] [sc_game info site]
  lappend list [sc_game info round] [sc_game info result]
  return $list
}

# ::bookmarks::Go
#
#   Jumps to a selected bookmark.
#
proc ::bookmarks::Go {entry} {
  if {[::bookmarks::isfolder $entry]} { return }
  set fname [lindex $entry 2]
  set gnum [lindex $entry 3]
  set ply [lindex $entry 4]
  set slot [sc_base slot $fname]
  if {$slot != 0} {
    sc_base switch $slot
  } else {
    busyCursor .
    if {[catch { ::file::Open $fname} result]} {
      unbusyCursor .
      tk_messageBox -icon warning -type ok -parent . \
        -title "Scid" -message "Unable to load the database:\n$fname\n\n$result"
      return
    }
    unbusyCursor .
    set ::glist 1
    ::recentFiles::add "[file rootname $fname].si4"
  }
  # Find and load the best database game matching the bookmark:
  set white [lindex $entry 5]
  set black [lindex $entry 6]
  set year  [lindex $entry 7]
  set site  [lindex $entry 8]
  set round [lindex $entry 9]
  set result [lindex $entry 10]

  set best [sc_game find $gnum $white $black $site $round $year $result]
  if {[catch {::game::Load $best}]} {
    tk_messageBox -icon warning -type ok -parent . \
      -title "Scid" -message "Unable to load game number: $best"
  } else {
    sc_move pgn $ply
  }
  ::notify::GameChanged
  ::notify::DatabaseChanged
}

# ::bookmarks::DeleteChildren
#
#   Deletes all submenus of a bookmark menu.
#
proc ::bookmarks::DeleteChildren {w} {
  foreach child [winfo children $w] {
    ::bookmarks::DeleteChildren $child
    destroy $child
  }
}

# ::bookmarks::NewSubMenu
#
#   Creates a new bookmark submenu.
#
proc ::bookmarks::NewSubMenu {w entry} {
  set i 1
  while {[winfo exists $w.m$i]} { incr i }
  $w add cascade -label [::bookmarks::Text $entry] -menu $w.m$i
  menu $w.m$i -tearoff 0
  return $w.m$i
}

# Globals used for bookmark editing:
#
set bookmarks(edit) ""
set bookmarks(ismenu) 0


# ::bookmarks::Edit
#
#   Creates the bookmark editing window.
#
proc ::bookmarks::Edit {} {
  global bookmarks
  set w .bmedit
  if {[winfo exists $w]} { return }
  set bookmarks(old) $bookmarks(data)
  win::createDialog $w
  wm title $w "Scid: [tr FileBookmarksEdit]"
  # wm transient $w .
  bind $w <F1> {helpWindow Bookmarks}
  ttk::entry $w.e -width 40 \
    -textvariable bookmarks(edit) -font font_Small
  $w.e configure -validate key -validatecommand "after 200 ::bookmarks::EditRefresh;  return true"

  pack $w.e -side top -fill x
  pack [ttk::frame $w.b2] -side bottom -fill x
  pack [ttk::frame $w.b1] -side bottom -fill x
  pack [ttk::frame $w.f] -side top -fill both -expand 1
  ttk::treeview $w.f.list -columns {0} -show {} -selectmode browse \
             -yscrollcommand "$w.f.ybar set"
  $w.f.list configure -height 10
  $w.f.list column 0 -width 50
  ttk::scrollbar $w.f.ybar -takefocus 0 -command "$w.f.list yview"
  bind $w.f.list <<TreeviewSelect>>  ::bookmarks::EditSelect
  pack $w.f.ybar -side right -fill y
  pack $w.f.list -side left -fill both -expand 1
  set i 0
  foreach entry $bookmarks(data) {
      $w.f.list insert {} end -id $i -values [list [::bookmarks::IndexText $entry]]
      incr i
  }
  dialogbutton $w.b1.newFolder -text $::tr(NewSubmenu) \
    -command {::bookmarks::EditNew folder}
  dialogbutton $w.b1.newGame -text [tr FileBookmarksAdd] \
    -command {::bookmarks::EditNew game}
  if {! [::bookmarks::CanAdd]} { $w.b1.newGame configure -state disabled }
  dialogbutton $w.b1.delete -text $::tr(Delete)  -command ::bookmarks::EditDelete
  ttk::button $w.b2.up -image tb_up -command {::bookmarks::EditMove up}
  ttk::button $w.b2.down -image tb_down -command {::bookmarks::EditMove down}
  dialogbutton $w.b2.ok -text "OK" -command ::bookmarks::EditDone
  dialogbutton $w.b2.cancel -text $::tr(Cancel) -command {
    set bookmarks(data) $bookmarks(old)
    catch {grab release .bmedit}
    destroy .bmedit
  }
  pack $w.b1.newFolder $w.b1.newGame $w.b1.delete -side left -padx 2 -pady 2
  pack $w.b2.up $w.b2.down -side left -padx 2 -pady 2
  packdlgbuttons $w.b2.cancel $w.b2.ok
  set bookmarks(edit) ""

  update
  catch {grab .bmedit}
}

# ::bookmarks::EditDone
#
#    Updates the bookmarks and closes the bookmark editing window.
#
proc ::bookmarks::EditDone {} {
  catch {grab release .bmedit}
  destroy .bmedit
  ::bookmarks::Save
  ::bookmarks::Refresh
}

# ::bookmarks::EditRefresh
#
#   Updates the bookmarks whenever the contents of the bookmark
#   editing entry box are changed.
#
proc ::bookmarks::EditRefresh {} {
  global bookmarks
  set list .bmedit.f.list
  set sel [lindex [$list selection] 0]
  if {$sel == ""} { return }
  set text $bookmarks(edit)
  set e [lindex $bookmarks(data) $sel]
  set e [::bookmarks::SetText $e $text]
  set text [::bookmarks::IndexText $e]
  set bookmarks(data) [lreplace $bookmarks(data) $sel $sel $e]
  $list item $sel -values [list $text]
  $list selection set $sel
}

# ::bookmarks::EditSelect
#
#   Sets the bookmark editing entry box when a bookmark is selected.
#
proc ::bookmarks::EditSelect {{sel ""}} {
  global bookmarks
  set list .bmedit.f.list
  set sel [lindex [$list selection] 0]
  if {$sel == ""} {
    .bmedit.e delete 0 end
    return
  }
  if {$sel >= [llength $bookmarks(data)]} {
    set bookmarks(edit) ""
    return
  }
  set e [lindex $bookmarks(data) $sel]
  set bookmarks(ismenu) [::bookmarks::isfolder $e]
  set bookmarks(edit) [::bookmarks::Text $e]
}

# ::bookmarks::isfolder:
#   Returns 1 if this bookmark entry is a folder (submenu).
#
proc ::bookmarks::isfolder {entry} {
  if {[lindex $entry 0] == "f"} { return 1 }
  return 0
}

# ::bookmarks::Text:
#   Returns the entry text of a bookmark.
#
proc ::bookmarks::Text {entry} {
  return [lindex $entry 1]
}

proc ::bookmarks::IndexText {entry} {
  set text ""
  if {[lindex $entry 0] == "f"} {
    append text "\[[lindex $entry 1]\]"
  } else {
    append text "    [lindex $entry 1]"
  }
  return $text
}

proc ::bookmarks::SetText {entry text} {
  return [lreplace $entry 1 1 $text]
}

# ::bookmarks::EditMove
#
#   Moves the selected bookmark "up" or "down" one place.
#
proc ::bookmarks::EditMove {{dir "up"}} {
  global bookmarks
  set w .bmedit
  set list $w.f.list
  set sel [lindex [$list selection] 0]
  if {$sel == ""} { return }
  set e [lindex $bookmarks(data) $sel]
  set text [::bookmarks::IndexText $e]
  set newsel $sel
  if {$dir == "up"} {
    incr newsel -1
    if {$newsel < 0} { return }
  } else {
    incr newsel
    if { [$list next $sel] == {} } { return }
  }
  set bookmarks(data) [lreplace $bookmarks(data) $sel $sel]
  set bookmarks(data) [linsert $bookmarks(data) $newsel $e]
  set i 0
  foreach entry $bookmarks(data) {
      $w.f.list item $i -values [list [::bookmarks::IndexText $entry]]
      incr i
  }
  $list selection set $newsel
}

# ::bookmarks::EditDelete
#
#   Deletes the selected bookmark.
#
proc ::bookmarks::EditDelete {} {
  global bookmarks
  set w .bmedit
  set list $w.f.list
  set sel [lindex [$list selection] 0]
  if {$sel == ""} { return }
  set bookmarks(data) [lreplace $bookmarks(data) $sel $sel]
  $list delete $sel
  set bookmarks(edit) ""
}

# ::bookmarks::EditNew
#
#   Inserts a new entry ("folder" for a submenu or "game" for the
#   current game) after the selected bookmark.
#
proc ::bookmarks::EditNew {{type "folder"}} {
  global bookmarks
  set w .bmedit
  set list $w.f.list
  set folder 0
  if {[string index $type 0] == "f"} {
    set folder 1
    set entry [::bookmarks::New folder]
  } else {
    set entry [::bookmarks::New game]
  }
  set sel [lindex [$list selection] 0]
  set id [llength $bookmarks(data)]
  if {$sel == ""} {
    lappend bookmarks(data) $entry
    set sel [$list insert {} end -id $id -values [list [::bookmarks::IndexText $entry]]]
    $list selection set $sel
    $list see $sel
    ::bookmarks::EditSelect
    return
  }
  incr sel
  set bookmarks(data) [linsert $bookmarks(data) $sel $entry]
  set sel [$list insert {} end  -id $id -values [list [::bookmarks::IndexText $entry]]]
  $list selection set $sel
  $list see $sel
  ::bookmarks::EditSelect
}

# ::bookmarks::Save
#
#   Saves the bookmarks file, reporting any error in a message box if
#   reportError is true.
#
proc ::bookmarks::Save {{reportError 0}} {
  global bookmarks
  set f {}
  set filename [scidConfigFile bookmarks]
  if  {[catch {open $filename w} f]} {
    if {$reportError} {
      tk_messageBox -title "Scid" -type ok -icon warning \
        -message "Unable to write bookmarks file: $filename\n$f"
    }
    return
  }
  puts $f "# Scid $::scidVersion bookmarks file\n"
  foreach i {subMenus data} {
    puts $f "set bookmarks($i) [list [set bookmarks($i)]]"
    puts $f ""
  }
  close $f
}


# End of file: bookmark.tcl
