#!/bin/sh

# sc_spell:
#     Clean a Scid database by applying all player, event, site and round
#     name corrections, then flagging all duplicate games as deleted.
#
# Usage:  sc_spell <scid-database> <spellcheck-file>

# The next line restarts using tkscid: \
exec tkscid "$0" "$@"

set args [llength $argv]
if {$args != 2} {
    puts stderr "Usage: sc_spell <scid-database> <spellcheck-file>"
    exit 1
}

# Open the spellcheck file:
set spellfile [lindex $argv 1]
if {[catch {sc_name read $spellfile} result]} {
    puts stderr "Error reading spellcheck file: $result"
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

puts "Spellchecking names..."
foreach nameType {player event site round} {
   # Now do each name correction:
    if {![catch {sc_name spellcheck $nameType} corrections]} {
        if {[catch {sc_name correct $nameType $corrections} result]} {
            puts stderr "Error correcting $nameType names"
            exit 1
        }
        puts "  Number of games corrected: [lindex $result 2]\n"
    } else {
        puts "  0 $nameType names were corrected."
    }
}

puts "Checking for duplicate games..."
set result [sc_base duplicates [sc_base current] ]
puts "  Found $result duplicate games."
sc_base close $curr_db
