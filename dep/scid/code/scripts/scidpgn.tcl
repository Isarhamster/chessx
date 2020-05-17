#!/bin/sh

# scidpgn: prints the PGN representation of all games in a
#   Scid database, with all comments and variations.

# The "\" at the end of the comment line below is necessary! It means
#   that the "exec" line is a comment to Tcl but not to /bin/sh.
# The next line restarts using tkscid, the Scid Tcl interpreter: \
exec tkscid "$0" "$@"

if {$argc != 1} { 
    puts "Usage: scidpgn database"
    puts "Example: scidpgn  mybase"
    exit 1
}

set baseName [file rootname [ lindex $argv 0 ]]
if {[catch { sc_base open $baseName } baseId]} {
    puts "Error: could not open database"
    exit 1
}

set nGames [sc_base numGames $baseId]
for {set i 1} {$i <= $nGames} {incr i} {
    if {[catch { sc_game load $i }]} {
        puts "Error: could not load game number $i"
        exit 1
    }
    puts [sc_game pgn -tags 1 -comments 1 -var 1 -width 75]
}
