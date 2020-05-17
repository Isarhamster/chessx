###
#
# Tooltips
#

set useLocalTklib [catch {package require tooltip 1.4.6}]
if {$useLocalTklib} {
  source [file nativename [file join $::scidTclDir "utils/tklib_tooltip.tcl"]]
}

namespace eval ::utils::tooltip {
  global ::utils::tooltip::font

  if {![info exists font]} {
    set font TkDefaultFont
  }

  # Undocumented tooltip variable that allows users to extend / override
  # label creation options.
  set tooltip::labelOpts [list -highlightthickness 0 -relief solid -bd 1 \
      -background lightyellow -fg black -font $font]

  proc Set {button msg} {
    tooltip::tooltip $button $msg
  }

  proc SetTag {text_widget msg tag} {
    tooltip::tooltip $text_widget -tag $tag $msg
  }
}
