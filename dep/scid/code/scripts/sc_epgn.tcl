#!/bin/sh

# sc_epgn: prints the PGN representation of selected games in a
#   Scid database, with no comments, variations or nonstandard tags,
#   making it useful for email correspondence chess since you don't
#   want to send your opponent your notes and analysis.

# The "\" at the end of the comment line below is necessary! It means
#   that the "exec" line is a comment to Tcl but not to /bin/sh.
# The next line restarts using tkscid, the Scid Tcl interpreter: \
exec tkscid "$0" "$@"

if {$argc < 2} { 
    puts "Usage: sc_epgn database game-number(s)"
    puts "Example: sc_epgn  mybase  1 2 3"
    exit 1
}

set baseName [ lindex $argv 0 ]
if [catch { sc_base open $baseName }] {
    puts "Error: could not open database"
    exit 1
}

foreach i [lrange $argv 1 end] {
    if [catch { sc_game load $i }] {
        puts -nonewline "Error: could not load game number "
        puts $i
        exit 1
    }
    puts [sc_game pgn -tags 0 -comments 0 -var 0 -width 70]
}
