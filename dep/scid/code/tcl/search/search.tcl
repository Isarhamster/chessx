###
### search.tcl: Search routines for Scid.
###

namespace eval ::search {}

# searchType: set to Header or Material in a SearchOptions file
set searchType 0

set ::search::filter::operation 2


# search::filter::reset
# TODO: remove this function
proc ::search::filter::reset {} {
  ::windows::gamelist::FilterReset "" [sc_base current]
}

# ::search::addFilterOpFrame
#
#   Adds a search filter operation selection frame to the window.
#   Adds a frame of radiobuttons allowing the filter operation
#   (AND with current filter, OR with current filter, or RESET filter)
#   to be chosen.
#   The default value for the first search is RESET:
proc ::search::addFilterOpFrame {w {small 0}} {
  ttk::labelframe $w.filterop -text $::tr(FilterOperation)
  set f $w.filterop
  pack $f -side top -fill x
  
  set regular TRadiobutton
  set bold Bold.TRadiobutton
  if {$small} {
    set regular Small.TRadiobutton
    set bold SmallBold.TRadiobutton
  }
  
  ttk::frame $f.b
  ttk::radiobutton $f.b.and -textvar ::tr(FilterAnd) -variable ::search::filter::operation -value 0 -style $regular 
  ttk::radiobutton $f.b.or -textvar ::tr(FilterOr) -variable ::search::filter::operation -value 1 -style $regular
  ttk::radiobutton $f.b.ignore -textvar ::tr(FilterIgnore) -variable ::search::filter::operation -value 2 -style $regular
  pack $f.b -anchor w -side top
  pack $f.b.and $f.b.or $f.b.ignore -side left -padx 5
}


# ::search::Config
#
#   Sets state of Search button in Header, Board and Material windows
#
proc ::search::Config {{state ""}} {
  if {$state == ""} {
    set state disabled
    if {[sc_base inUse]} { set state normal }
  }
  catch {.sh.b.search configure -state $state }
  catch {.sb.b.search configure -state $state }
  catch {.sm.b3.search configure -state $state }
}


proc ::search::usefile {} {
  set ftype { { "Scid SearchOption files" {".sso"} } }
  set ::fName [tk_getOpenFile -initialdir $::initialDir(base) \
      -filetypes $ftype -title "Select a SearchOptions file"]
  if {$::fName == ""} { return }
  
  if {[catch {uplevel "#0" {source $::fName} } ]} {
    tk_messageBox -title "Scid: Error reading file" -type ok -icon warning \
        -message "Unable to open or read SearchOptions file: $fName"
  } else {
    switch -- $::searchType {
      "Material" { ::search::material }
      "Header"   { ::search::header }
      default    { return }
    }
  }
}
