#!/bin/sh

# sc_eco:
#     Reclassify the ECO code of all games in a Scid database.
#
# Usage:  sc_spell <scid-database> <eco-file>

# The next line restarts using tkscid: \
exec tkscid "$0" "$@"

set args [llength $argv]
if {$args != 2} {
    puts stderr "Usage: sc_eco <scid-database> <eco-file>"
    exit 1
}

# Open the ECO file:
set ecofile [lindex $argv 1]
if {[catch {sc_eco read $ecofile} result]} {
    puts stderr "Error reading ECO file: $result"
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

puts "Classifying games..."
puts [sc_eco base 1 1]
sc_base close $curr_db
