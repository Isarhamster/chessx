#!/usr/bin/tclsh
###
### checklangs.tcl
###

# This script is not part of the Scid source code; it is a standalone
# program which verifies that every language file has the same
# translation commands in the same order as english.tcl.

array set codes {
  czech C
  deutsch D
  francais F
  greek G
  hungary H
  italian I
  nederlan N
  norsk O
  polish P
  portbr B
  russian R
  serbian Y
  spanish S
  swedish W
  catalan K
  suomi U
  greek G
}

set languages {czech deutsch francais hungary italian nederlan norsk polish
  portbr spanish swedish serbian russian catalan suomi greek
}

################################################################################
# handle multiple lines statements and keep only lines with
# menuText translate helpMsg
proc multiLines {tmp} {
  set data {}
  for {set i 0} {$i < [llength $tmp]} {incr i} {
    set line [lindex $tmp $i]
    if {[string first "\{" $line] != -1 && [string first "\}" $line] == -1 } {
      incr i
      while { [string first "\}" [lindex $tmp $i] ] == -1  && $i < [llength $tmp] } {
        append line "\n[lindex $tmp $i]"
        incr i
      }
      append line "\n[lindex $tmp $i]"
    }
    lappend data $line
  }
  set strippedData {}
  
  foreach line $data {
    if { [catch {set command [lindex $line 0]} ] } {
      continue
    } else {
      if { $command == "menuText" || $command == "translate" || $command == "helpMsg" } {
        lappend strippedData $line
      }
    }
  }
  
  return $strippedData
}
################################################################################
proc checkfile {code langfile} {
  # Read this language file and the english file:
  
  set f [open english.tcl r]
  set data [read $f]
  close $f
  set tmp [split $data "\n"]
  set englishData [multiLines $tmp]
  
  set f [open $langfile.tcl r]
  set data [read $f]
  close $f
  set tmp [split $data "\n"]
  set langData [multiLines $tmp]
  
  set langNames {}
  
  foreach line $langData {
    if { [catch {set command [lindex $line 0]} ] } {
      # puts "problem->$line"
      continue
    }
    set lang [lindex $line 1]
    set name [lindex $line 2]
    if {$lang == $code  &&  ($command == "menuText" || $command == "translate" || $command == "helpMsg")} {
      lappend langNames $command:$name
    }
  }
  
  set lastMatch -1
  foreach line $englishData {
    if { [catch {set command [lindex $line 0]} ] } { continue }
    set lang [lindex $line 1]
    set name [lindex $line 2]
    if {$lang == "E"  &&  ($command == "menuText" || $command == "translate" || $command == "helpMsg")} {
      set thisMatch [lsearch -exact $langNames $command:$name]
      if {$thisMatch < 0} {
        puts "$langfile - MISSING: $name"
      } else {
        if {$thisMatch != $lastMatch + 1} {
          puts "$langfile - OUT OF ORDER: $command $lang $name"
        }
        set lastMatch $thisMatch
      }
    }
  }
}
################################################################################

if {[llength $argv] == 0} { set argv $languages }

foreach language $argv {
  if {[info exists codes($language)]} {
    checkfile $codes($language) $language
  } else {
    puts "No such language file: $language"
  }
}

