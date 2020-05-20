# Copyright (C) 2018  Uwe Klimmek
#
# This file is part of SCID (Shane's Chess Information Database).
#
# SCID is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation.
#
# SCID is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with SCID. If not, see <http://www.gnu.org/licenses/>.

namespace eval ::appearance {}

proc ::appearance::applyMenuBarColor { menu docked_nb } {
  if { ! [info exists ::menuColor(background)] && \
       ! [info exists ::menuBarColor(background)] } { return }

  if { $docked_nb eq ""} {
    set varname ::menuBarColor(background)
  } else {
    set varname ::menuColor(background)
  }
  if { [info exists $varname] } {
    $menu configure -background [set $varname]
  } else {
    lassign [lrange [$menu configure -background] 3 end] default currCol
    if { $default ne $currCol } {
      $menu configure -background $default
    }
  }
}

proc ::appearance::chooseMenuColor {col_var w} {
  set col [ tk_chooseColor -initialcolor [set $col_var] -title "Scid"]
  if { $col != "" } {
      set $col_var $col
      ::appearance::Refresh $w
  }
}

proc ::appearance::setNewMenuColors { m configure_cmd } {
  $m configure {*}$configure_cmd
  foreach child [winfo children $m] {
    setNewMenuColors $child $configure_cmd
  }
}

proc ::appearance::updateMenuColors {} {
  foreach {col value} [array get ::menuDialog_] {
    lappend configure_cmd "-[string tolower $col]"
    lappend configure_cmd $value
  }
  set toplevel_wins [::win::getWindows]
  lappend toplevel_wins "."
  foreach wnd $toplevel_wins {
    lassign [::win::getMenu $wnd] menu
    if {$menu ne ""} {
      setNewMenuColors $menu $configure_cmd

      lassign [::win::isDocked $wnd] docked_nb
      applyMenuBarColor $menu $docked_nb
    }
  }
}

proc ::appearance::setMenuColors {} {
  foreach {col value} [array get ::menuDialog_] {
    if { $value ne $::menuDialogDefault_($col) } {
      set ::menuColor($col) $value
    } else {
      unset -nocomplain ::menuColor($col)
    }
    option add *Menu.$col $value
  }
  foreach {col value} [array get ::menuBarDialog_] {
    if { $value ne $::menuDialogDefault_($col) } {
      set ::menuBarColor($col) $value
    } else {
      unset -nocomplain ::menuBarColor($col)
    }
  }
  updateMenuColors
}

proc ::appearance::defaultColors {} {
  foreach col { background foreground activeBackground activeForeground selectColor disabledForeground } {
    set config [.menu configure -[string tolower $col]]
    set ::menuDialogDefault_($col) [lindex $config 3]
    set ::menuDialog_($col) $::menuDialogDefault_($col)
  }
  set ::menuBarDialog_(background) $::menuDialogDefault_(background)
}

# Menu Color Config
#
#   Dialog window for configuring Menu colors
proc ::appearance::menuConfigDialog { w } {
  ::appearance::defaultColors
  foreach {col value} [array get ::menuColor] {
    set ::menuDialog_($col) $value
  }
  foreach {col value} [array get ::menuBarColor] {
    set ::menuBarDialog_($col) $value
  }

  pack [ttk::frame $w.f] -side top
  set f $w.f
  set r 0

  ttk::label $f.mainmenu -text "Menu bar Menu"
  if { ! $::windowsOS &&  ! $::macOS} {
    ttk::button $f.mbg -text $::tr(MenuColorBackground) -command "::appearance::chooseMenuColor ::menuBarDialog_(background) $w"
    grid $f.mainmenu -row $r -column 0 -columnspan 3 -padx 4 -sticky e
    grid $f.mbg -row $r -column 4 -padx 4
    incr r
  }
  ttk::label $f.menuline0 -text "Disabled"
  grid $f.menuline0 -row $r -column 2 -padx 4
  ttk::button $f.dfg -text $::tr(MenuColorForeground) -command "::appearance::chooseMenuColor ::menuDialog_(disabledForeground) $w"
  grid $f.dfg -row $r -column 3 -padx 4
  incr r
  ttk::button $f.sel -text $::tr(MenuColorSelect) -command "::appearance::chooseMenuColor ::menuDialog_(selectColor) $w"
  grid $f.sel -row $r -column 0 -padx 4
  ttk::label $f.select -text "x"
  grid $f.select -row $r -column 1
  ttk::label $f.menuline1 -text "Menu1"
  grid $f.menuline1 -row $r -column 2 -padx 4
  ttk::button $f.fg -text $::tr(MenuColorForeground) -command "::appearance::chooseMenuColor ::menuDialog_(foreground) $w"
  grid $f.fg -row $r -column 3 -padx 4 -pady 5
  ttk::button $f.bg -text $::tr(MenuColorBackground) -command "::appearance::chooseMenuColor ::menuDialog_(background) $w"
  grid $f.bg -row $r -column 4 -padx 4
  incr r
  ttk::label $f.menuline2 -text "Menu2"
  grid $f.menuline2 -row $r -column 2 -padx 4
  ttk::button $f.afg -text $::tr(MenuColorForeground) -command "::appearance::chooseMenuColor ::menuDialog_(activeForeground) $w"
  grid $f.afg -row $r -column 3 -padx 4
  ttk::button $f.abg -text $::tr(MenuColorBackground) -command "::appearance::chooseMenuColor ::menuDialog_(activeBackground) $w"
  grid $f.abg -row $r -column 4 -padx 4 -pady 5
  ::appearance::Refresh $w
}

proc ::appearance::Refresh { w } {
  global menuDialog_

  $w.f.menuline0 configure -background $menuDialog_(background) -foreground $menuDialog_(disabledForeground)
  $w.f.menuline1 configure -background $menuDialog_(background) -foreground $menuDialog_(foreground)
  $w.f.menuline2 configure -background $menuDialog_(activeBackground) -foreground $menuDialog_(activeForeground)
  $w.f.select configure -foreground $menuDialog_(selectColor)
  $w.f.mainmenu configure -background $::menuBarDialog_(background) -foreground $menuDialog_(foreground)
  ::appearance::setMenuColors
}
