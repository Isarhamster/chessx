#!/bin/sh
#
# spf2spi:
#     Generate SPI (Scid Photo Index) file from the SPF file.
#     SPI file should be placed in the same directory as SPF file.
#
# Usage:  spf2spi filename.spf > filename.spi
# Example: spf2spi historic.spf > historic.spi
# \
exec tclsh "$0" "$@"

proc trimString {data} {
  #convert names tolower case and strip the first two blanks.
  set data [string tolower $data]
  set strindex [string first "\ " $data]
  set data [string replace $data $strindex $strindex]
  set strindex [string first "\ " $data]
  set data [string replace $data $strindex $strindex]
  return $data
}

if {$argc  != 1} {
  puts stderr "ERROR! spf2spi called with wrong number of arguments!"
  puts stderr ""
  puts stderr " spf2spi:"
  puts stderr "     Generate SPI (Scid Photo Index) file from the SPF file."
  puts stderr "     SPI file should be placed in the same directory as SPF file."
  puts stderr ""
  puts stderr " Usage:  spf2spi filename.spf > filename.spi"
  puts stderr " Example: spf2spi historic.spf > historic.spi"
  exit 1
}

set fd [open "$argv"]

while {[gets $fd line] >= 0} {
  if { [regexp {^photo \"(.*)\" \{$} $line -> name] } {
    set begin [tell $fd]
    while {1} {
      set end [tell $fd]
      gets $fd line
      if {[regexp {.*\}.*} $line ]} {break}
    }
    set photobegin($name) $begin
    set photosize($name) [expr $end - $begin ]
  }
}

foreach { name } [array names photobegin] {
  puts "set \"photobegin([trimString $name ])\" $photobegin($name)"
  puts "set \"photosize([trimString $name ])\" $photosize($name)"
  puts "set \"spffile([trimString $name ])\" \"\$fname\""
}

 	  	 
