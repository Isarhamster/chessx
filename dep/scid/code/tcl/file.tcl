# ::file::Exit
#
#    Prompt for confirmation then exit.
#
proc ::file::Exit {}  {
  # Check for altered game in all bases except the clipbase:
  set unsavedCount 0
  set savedBase [sc_base current]
  set msg ""
  foreach i [sc_base list] {
    sc_base switch $i
    if {[sc_game altered] && ![sc_base isReadOnly $i]} {
      if {$unsavedCount == 0} {
        append msg $::tr(ExitUnsaved)
        append msg "\n\n"
      }
      incr unsavedCount
      set fname [file tail [sc_base filename $i]]
      set g [sc_game number]
      append msg "   Base $i: $fname "
      append msg "($::tr(game) $g)"
      append msg "\n"
    }
  }
  # Switch back to original database:
  sc_base switch $savedBase
  
  # Check if a mask is opened and dirty
  ::tree::mask::close
  
  if {$msg != ""} {
    append msg "\n"
  }
  append msg $::tr(ExitDialog)
  
  # Only ask before exiting if there are unsaved changes:
  if {$unsavedCount > 0} {
    set answer [tk_messageBox -title "Scid: [tr FileExit]" \
        -message $msg -type yesno -icon question]
    if {$answer != "yes"} { return }
  }
  if {$::optionsAutoSave} {
    options.write
  }
  ::recentFiles::save
  ::utils::history::Save
  destroy .
}


# ::file::New
#
#   Opens file-save dialog and creates a new database.
#
proc ::file::New {} {
  set ftype {
    { "Scid databases" {".si4"} }
    { "PGN files" {".pgn" ".PGN"} }
  }
  
  set fName [tk_getSaveFile \
             -initialdir $::initialDir(base) \
             -filetypes $ftype \
             -defaultextension ".si4" \
             -title "Create a Scid database"]
  
  if {$fName == ""} { return }
  set file_extension [string tolower [file extension $fName]]
  if {$file_extension == ".si4"} {
    set dbType "SCID4"
    set dbName [file rootname $fName]
  } elseif {$file_extension == ".pgn"} {
    set dbType "PGN"
    set dbName $fName
  }
  if {[catch {sc_base create $dbType $dbName} baseId]} {
    ERROR::MessageBox "$fName\n"
    return
  }
  set ::curr_db $baseId
  set ::initialDir(base) [file dirname $fName]
  ::recentFiles::add $fName
  ::notify::GameChanged
  ::notify::DatabaseChanged
  return $baseId
}

# ::file::Open
#
#    Opens file-open dialog and opens the selected Scid database.
#
proc ::file::Open {{fName ""}} {
  set err [::file::Open_ "$fName"]
  if {$err == 0} {
    set ::curr_db $::file::lastOpened
    ::windows::gamelist::Open $::curr_db
    ::notify::DatabaseChanged
    set gamenum 1
    foreach {tagname tagvalue} [sc_base extra $::curr_db] {
      if {$tagname eq "autoload" && $tagvalue != 0} {
        set gamenum $tagvalue
        break
      }
    }
    if {$gamenum <= [sc_base numGames $::curr_db]} {
      ::game::Load $gamenum 0
    } else {
      ::notify::GameChanged
    }
  }
  return $err
}

proc ::file::openBaseAsTree { { fName "" } } {
  set current [sc_base current]
  set err [::file::Open_ "$fName"]
  sc_base switch $current
  ::notify::DatabaseChanged
  if {$err == 0} { ::tree::make $::file::lastOpened 1 }
  return $err
}

proc ::file::Open_ {{fName ""} } {
  if {$fName == ""} {
      set ftype {
        { "All Scid files" {".si4" ".si3" ".pgn" ".epd"} }
        { "Scid databases, PGN files" {".si4" ".si3" ".pgn" ".PGN"} }
        { "Scid databases" {".si4" ".si3"} }
        { "PGN files" {".pgn" ".PGN"} }
        { "EPD files" {".epd" ".EPD"} }
      }
  
    set fName [tk_getOpenFile -initialdir $::initialDir(base) -filetypes $ftype -title "Open a Scid file"]
    if {$fName == ""} { return 2}
  }

  set ext [string tolower [file extension "$fName"] ]
  if {"$ext" == ".si4"} { set fName [file rootname "$fName"] }
  if {[sc_base slot $fName] != 0} {
    tk_messageBox -title "Scid: opening file" -message "The database you selected is already opened."
    return 1
  }

  set err 0
  if {"$ext" == ".si3"} {
    set err [::file::Upgrade [file rootname "$fName"] ]
  } elseif {"$ext" == ".pgn"} {
    # PGN file:
    progressWindow "Scid" "$::tr(OpeningTheDatabase): $fName..." $::tr(Cancel)
    set err [catch {sc_base open PGN "$fName"} ::file::lastOpened]
    closeProgressWindow
    if {$err} {
      ERROR::MessageBox "$fName\n"
    } else {
      catch { sc_base extra $::file::lastOpened type 3 }
      set ::initialDir(base) [file dirname "$fName"]
      ::recentFiles::add "$fName"
    }
  } elseif {"$ext" == ".epd"} {
    # EPD file:
    set err [catch {sc_base create MEMORY "$fName"} ::file::lastOpened]
    if {$err} {
      ERROR::MessageBox "$fName\n"
    } else {
      importPgnFile $::file::lastOpened [list "$fName"]
      sc_base extra $::file::lastOpened type 3
      set ::initialDir(base) [file dirname "$fName"]
      ::recentFiles::add "$fName"
    }
  } elseif {"$ext" eq ".si4" || "$ext" eq ""} {
    progressWindow "Scid" "$::tr(OpeningTheDatabase): [file tail "$fName"]..." $::tr(Cancel)
    set err [catch {sc_base open "$fName"} ::file::lastOpened]
    closeProgressWindow
    if {$err} {
      if { $::errorCode == $::ERROR::NameDataLoss } { set err 0 }
      ERROR::MessageBox "$fName.si4\n"
    } else {
      set ::initialDir(base) [file dirname "$fName"]
      ::recentFiles::add "$fName.si4"
    }
  } else {
    tk_messageBox -title "Scid: opening file" -message "Unsupported database format:  $ext"
    set err 1
  }
  
  return $err
}

# ::file::Upgrade
#
#   Upgrades an old (version 3) Scid database to version 4.
#
proc ::file::Upgrade {name} {
  if {[file readable "$name.si4"]} {
    set msg [string trim $::tr(ConfirmOpenNew)]
    set res [tk_messageBox -title "Scid" -type yesno -icon info -message $msg]
    if {$res == "no"} { return }
    return [::file::Open_ "$name.si4"]
  }
  
  set msg [string trim $::tr(ConfirmUpgrade)]
  set res [tk_messageBox -title "Scid" -type yesno -icon info -message $msg]
  if {$res == "no"} { return }

  set err [catch {
    file copy "$name.sg3"  "$name.sg4"
    file copy "$name.sn3"  "$name.sn4"
    file copy "$name.si3"  "$name.si4" }]
  if {$err} {
    ERROR::MessageBox "$name\n"
    return 1
  }

  progressWindow "Scid" "$::tr(Opening): [file tail $name]..." $::tr(Cancel)
  set err [catch {sc_base open $name} ::file::lastOpened]
  closeProgressWindow
  if {$::errorCode == $::ERROR::NameDataLoss} {
    ERROR::MessageBox "$name\n"
    set err 0
  }
  if {$err} {
    ERROR::MessageBox "$name\n"
    catch {
      file delete "$name.sg4"
      file delete "$name.sn4"
      file delete "$name.si4" }
  } else {
    progressWindow "Scid" [concat $::tr(CompactDatabase) "..."] $::tr(Cancel)
    set err_compact [catch {sc_base compact $::file::lastOpened}]
    closeProgressWindow
    if {$err_compact} { ERROR::MessageBox }
  }
  return $err
}

# ::file::Close:
#   Closes the active base.
#
proc ::file::Close {{base -1}} {
  # Remember the current base:
  set current [sc_base current]
  if {$base < 0} { set base $current }
  if {![sc_base inUse $base]} { return }
  # Switch to the base which will be closed, and check for changes:
  sc_base switch $base
  set confirm [::game::ConfirmDiscard]
  if {$confirm == 0} {
    sc_base switch $current
    return
  }
  # Close Tree window whenever a base is closed/switched:
  if {[winfo exists .treeWin$base]} { destroy .treeWin$base }

  ::search::CloseAll

  # If base to close was the current one, reset to clipbase
  if { $current == $base } { set current 9 }

  if {[catch {sc_base close $base}]} {
    ERROR::MessageBox
  }

  if {$confirm == 2} { ::notify::DatabaseModified $::clipbase_db }

  # Now switch back to the original base
  ::file::SwitchToBase $current 0
}

proc ::file::SwitchToBase {{b} {saveHistory 1}} {
  if {![catch {sc_base switch $b} res]} {
    set ::curr_db $res
    # Close email window when a base is switched:
    if {[winfo exists .emailWin]} { destroy .emailWin }
  }
  ::notify::GameChanged
  ::notify::DatabaseChanged
}

# Databases that will be automatically loaded ad startup
proc ::file::autoLoadBases.load {} {
  if {![info exists ::autoLoadBases]} { return }
  foreach base $::autoLoadBases {
    if {[::file::Open $base] != 0} {
      set idx [lsearch -exact $::autoLoadBases $base]
      if {$idx != -1} { set ::autoLoadBases [lreplace $::autoLoadBases $idx $idx] }
    }
  }
}

proc ::file::autoLoadBases.save { {channelId} } {
  if {![info exists ::autoLoadBases]} { return }
  puts $channelId "set ::autoLoadBases [list $::autoLoadBases]"
}
proc ::file::autoLoadBases.find { {baseIdx} } {
  if {![info exists ::autoLoadBases]} { return -1 }
  if {[ catch {set base [sc_base filename $baseIdx]} ]} { return -1}
  return [lsearch -exact $::autoLoadBases $base]
}
proc ::file::autoLoadBases.add { {baseIdx} } {
  if {[ catch {set base [sc_base filename $baseIdx]} ]} { return }
  lappend ::autoLoadBases $base
}
proc ::file::autoLoadBases.remove { {baseIdx} } {
  if {![info exists ::autoLoadBases]} { return }
  if {[ catch {set base [sc_base filename $baseIdx]} ]} { return }
  set idx [lsearch -exact $::autoLoadBases $base]
  if {$idx != -1} {
    set ::autoLoadBases [lreplace $::autoLoadBases $idx $idx]
  }
}
