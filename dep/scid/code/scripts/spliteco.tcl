#!/bin/sh
# \
exec `dirname $0`/tkscid "$0" "$@"

# spliteco:
#   Takes a Scid-format database and produces a number of smaller
#   databases containing the games grouped according to ECO code.
#   It can produce 5 (A-E), 50 (A0-E9) or 500 (A00-E99) databases.
#   Games with no ECO code are not copied.
#
# Usage:
#   spliteco [-v] <scid-database> 5|50|500
#
# Author: Shane Hudson
# Last updated: May 2002
# Scid website: http://scid.sourceforge.net

set verbose 0

proc main {} {
  global argv verbose
  # Look for optional "-v" argument:
  if {[string range [lindex $argv 0] 0 1] == "-v"} {
    set argv [lrange $argv 1 end]
    set verbose 1
  }
  # Check for two required arguments:
  if {[llength $argv] != 2} { usage }
  set basename [lindex $argv 0]
  set ngroups [lindex $argv 1]
  if {$ngroups != 5  &&  $ngroups != 50  &&  $ngroups != 500} { usage }

  # Strip suffix if necessary:
  set basename [file rootname $basename]

  # Try to open the database:
  if {[catch {sc_base open $basename} frombase]} {
    err "Error opening database \"$basename\": $frombase"
    exit 1
  }
  switch $ngroups {
    5   {split5   $basename $frombase}
    50  {split50  $basename $frombase}
    500 {split500 $basename $frombase}
  }
  sc_base close $frombase
}

proc err {s} {
  puts stderr $s
}

proc log {s} {
  global verbose
  if {$verbose} {
    puts -nonewline stdout $s
    flush stdout
  }
}

proc usage {} {
  err "spliteco:"
  err "  Splits a Scid-format database into 5, 50 or 500 smaller databases"
  err "  grouped by ECO code. Games with no ECO code are not copied."
  err "  For progress feedback, add the \"-v\" (verbose) option."
  err "Usage: spliteco \[-v\] <Scid-database> 5|50|500"
  exit 1
}

proc split5 {basename frombase} {
  foreach major {A B C D E} {
    log "ECO range $major: "
    set ngames [split $frombase ${basename}_$major ${major}00 ${major}99]
    log " copied [format %6d $ngames] games to 1 database\n"
  }
}

proc split50 {basename frombase} {
  foreach major {A B C D E} {
    log "ECO range $major: "
    set ngames 0
    foreach minor {0 1 2 3 4 5 6 7 8 9} {
      set code $major$minor
      incr ngames [split $frombase ${basename}_$code ${code}0 ${code}9]
      log "."
    }
    log " copied [format %6d $ngames] games to 10 databases\n"
  }
}

proc split500 {basename frombase} {
  foreach major {A B C D E} {
    log "ECO range $major: "
    set ngames 0
    foreach minor {0 1 2 3 4 5 6 7 8 9} {
      foreach subcode {0 1 2 3 4 5 6 7 8 9} {
        set code $major$minor$subcode
        incr ngames [split $frombase ${basename}_$code $code $code]
      }
      log "."
    }
    log " copied [format %6d $ngames] games to 100 databases\n"
  }
}

proc split {frombase newbasename startECO endECO} {
  if {[catch {sc_base create $newbasename} tobase]} {
    err "Error creating database \"$newbasename\": $tobase"
    exit 1
  }
  sc_base switch $frombase
  sc_filter reset $frombase dbfilter full
  sc_search header -eco [list $startECO $endECO]
  set ngames [sc_filter count]
  if {[catch {sc_base copygames $frombase dbfilter $tobase} err]} {
    err "Error copying games to \"$newbasename\": $err"
    exit 1
  }
  sc_base close $tobase
  return $ngames
}

main
