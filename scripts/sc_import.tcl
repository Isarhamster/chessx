#!/bin/sh

# sc_import:
#     Import PGN files of games into an existing Scid database.
#
# Usage:  sc_import <scid-database> <pgn-files....>

# The next line restarts using tkscid: \
exec tkscid "$0" "$@"

set args [llength $argv]
if {$args < 1} {
    puts stderr "Usage: sc_import <scid-database> <pgn-files...>"
    exit 1
}

# Open the database:
set basename [lindex $argv 0]
if {[catch {sc_base open $basename} result]} {
    puts stderr "Error opening database \"$basename\": $result"
    exit 1
}
set curr_db [sc_base current]
if {[sc_base isReadOnly $curr_db]} {
    puts stderr "Error: database \"$basename\" is read-only."
    exit 1
}

for {set i 1} {$i < $args} {incr i} {
    set pgnfile [lindex $argv $i]
    if {[catch {sc_base import $curr_db $pgnfile} result]} {
        puts stderr "Error importing \"$pgnfile\": $result"
        exit 1
    }
    set numImported [lindex $result 0]
    set warnings [lindex $result 1]
        puts "Imported $numImported games from $pgnfile"
    if {$warnings == ""} {
        puts "There were no PGN errors or warnings."
    } else {
        puts "PGN errors/warnings:"
        puts $warnings
    }
}

sc_base close $curr_db
