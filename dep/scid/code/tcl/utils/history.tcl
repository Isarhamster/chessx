
### history.tcl
### Text entry history functions for Scid.
### Copyright (C) 2004 Shane Hudson.

namespace eval ::utils::history {}


set ::utils::history::defaultListLength 10
array set ::utils::history::listLength {}
array set ::utils::history::comboboxWidget {}

if {! [info exists ::utils::history::listData]} {
  array set ::utils::history::listData {}
}

# Load any history lists that were saved in the last session:
catch {source [scidConfigFile history]}


proc ::utils::history::SetList {key list} {
  set ::utils::history::listData($key) $list
}


proc ::utils::history::GetList {key} {
  variable listData
  if {[info exists listData($key)]} {
    return $listData($key)
  }
  return {}
}


#  ::utils::history::AddEntry
#
#   Inserts an entry at the top of the list of the specified key and
#   ensures that only one copy of the entry exists in the list.
#   The list is then pruned to the limit size is necessary.
#
proc ::utils::history::AddEntry {key entry} {
  variable listData
  # We do not add the empty string to a history list:
  if {$entry == "" } {
    return
  }
  
  if {[info exists listData($key)]} {
    # Take out this entry if it exists, so it will not appear twice:
    set index [lsearch -exact $listData($key) $entry]
    if {$index == 0} {
      # The entry is already at the start of the list; nothing to do
      return
    } elseif {$index > 0} {
      set listData($key) [lreplace $listData($key) $index $index]
    }
    set listData($key) [linsert $listData($key) 0 $entry]
    ::utils::history::PruneList $key
  } else {
    set listData($key) [list $entry]
  }
  RefillCombobox $key
  
  if { [llength [GetList $key]] > 0 } {
    set cb [ GetCombobox $key ]
    if { $cb != "" && [winfo exists $cb]} {
      $cb current 0
    }
  }
  
}


proc ::utils::history::SetLimit {key length} {
  set ::utils::history::listLength($key) $length
  ::utils::history::PruneList $key
}


proc ::utils::history::GetLimit {key} {
  variable listLength
  variable defaultListLength
  if {[info exists ::utils::history::listLength($key)]} {
    return $::utils::history::listLength($key)
  }
  return $defaultListLength
}


proc ::utils::history::PruneList {key {length -1}} {
  variable listData
  if {! [info exists listData($key)]} { return }
  if {$length < 0} {
    set length [::utils::history::GetLimit $key]
  }
  set listData($key) [lrange $listData($key) 0 [expr {$length - 1}]]
}



# ::utils::history::SetCombobox
#
#   Returns the combobox widget associated with this history key.
#
proc ::utils::history::GetCombobox {key} {
  variable comboboxWidget
  if {[info exists comboboxWidget($key)]} {
    return $comboboxWidget($key)
  }
  return ""
}


# ::utils::history::SetCombobox
#
#   Associates the specified widget (which must be a megawidget created
#   with ::combobox::combobox, see contrib/combobox.tcl) with the specific
#   history key. Whenever the list for this history key is modified, the
#   combobox widget will be updated.
#
proc ::utils::history::SetCombobox {key cbWidget} {
  set ::utils::history::comboboxWidget($key) $cbWidget
  RefillCombobox $key
}


# ::utils::history::SetCombobox
#
#   Refills the history list of the combobox associated with the specified
#   history key, if there is one.
#
proc ::utils::history::RefillCombobox {key} {
  variable comboboxWidget
  
  set cbWidget [GetCombobox $key]
  if {$cbWidget == ""} { return }
  
  # If the combobox widget is part of a dialog which is generated as needed,
  # it may not exist right now:
  if {! [winfo exists $cbWidget]} { return }
  
  $cbWidget delete 0 end
  set entries [GetList $key]
  $cbWidget configure -values $entries
}


# ::utils::history::Save
#   Saves the combobox history file, reporting any error in a message box
#   if reportError is true.
#
proc ::utils::history::Save {{reportError 0}} {
  variable listData
  
  set f {}
  set filename [scidConfigFile history]
  
  if  {[catch {open $filename w} f]} {
    if {$reportError} {
      tk_messageBox -title "Scid" -type ok -icon warning \
          -message "Unable to write file: $filename\n$f"
    }
    return
  }
  
  puts $f "# Scid $::scidVersion combobox history lists"
  puts $f ""
  foreach i [lsort [array names listData]] {
    puts $f "set ::utils::history::listData($i) [list $listData($i)]"
  }
  close $f
}
