#!/bin/sh

# tbstats.tcl:
#   Scid script to show the number of times each material configuration
#   covered by an endgame tablebase occurs in a given Scid database.
#   Usage: tbstats database-name
#      or: tkscid tbstats.tcl database-name

# The "\" at the end of this line is necessary: \
exec tkscid "$0" "$@"

array set p {0 q  1 r  2 b  3 n  4 p}
set pieces {0 1 2 3 4}
set nopawns {0 1 2 3}

proc search {str} {
  array set w {Q 0 R 0 N 0 B 0 P 0}
  array set b {Q 0 R 0 N 0 B 0 P 0}
  set side w
  set arglist [split $str {}]

  foreach i $arglist {
    if {$i == "-"} {
      set side b
    } else {
      set i [string toupper $i]
      if {$side == "w"} { catch {incr w($i) }} else { catch {incr b($i)} }
    }
  }

  sc_search material -wq $w(Q) -bq $b(Q) -wr $w(R) -br $b(R) \
    -wb $w(B) -bb $b(B) -wn $w(N) -bn $b(N) -wp $w(P) -bp $b(P) -flip 1
  return [sc_filter count]
}

proc section {w b {type pawns}} {
  global mode
  set str ""
  if {$mode != "raw"} { append str "\n" }
  if {$mode == "html"} {
    append str {<p><b><font color="#000070">}
    append str "<a name=\"$w-$b\">"
  } else {
    append str "# "
  }
  if {$type == "pawns"} {
    append str "$w-$b"
  } else {
    append str "$w-$b (no pawns)"
  }
  if {$mode == "html"} { append str "</a></font></b></p>\n<pre>" }
  puts $str
}

proc endsection {} {
  global mode
  if {$mode == "html"} {
    puts "</pre>"
  }
}

proc startrow {} {
  global mode
}

proc endrow {} {
  global mode
  newline
}

proc newline {} {
  global mode
  if {$mode != "raw"} { puts "" }
}

proc indent {} {
  global mode
  if {$mode != "raw"} { puts -nonewline "               " }
}

proc tbstats {str} {
  global mode perMillion
  set str [string toupper $str]
  set count [search $str]
  if {$perMillion} {
    set count [expr double($count) * 1000000.0 / double([sc_base numGames [sc_base current]])]
    set count [expr round($count)]
  }
  puts -nonewline [format " %8s %5d" $str $count]
  if {$mode == "raw"} { puts "" }
}


proc tb11 {} {
  global p pieces
  section 1 1
  startrow
  tbstats "k-k"
  endrow
  endsection
}

proc tb21 {} {
  global p pieces
  section 2 1
  startrow
  foreach w $pieces {
    tbstats "k$p($w)-k"
  }
  endrow
  endsection
}

proc tb22 {} {
  global p pieces
  section 2 2
  foreach w $pieces {
    startrow
    foreach b $pieces {
      if {$b < $w} {
        indent
      } else {
        tbstats "k$p($w)-k$p($b)"
      }
    }
    endrow
  }
  endsection
}

proc tb31 {} {
  global p pieces
  section 3 1
  foreach w1 $pieces {
    startrow
    foreach w2 $pieces {
      if {$w2 < $w1} {
        indent
      } else {
        tbstats "k$p($w1)$p($w2)-k"
      }
    }
    endrow
  }
  endsection
}

proc tb32 {} {
  global p pieces
  section 3 2
  foreach w1 $pieces {
    foreach w2 $pieces {
      if {$w2 < $w1} { continue }
      startrow
      foreach b $pieces {
        tbstats "k$p($w1)$p($w2)-k$p($b)"
      }
      endrow
    }
  }
  endsection
}

proc tb41 {} {
  global p pieces
  section 4 1
  foreach w1 $pieces {
    foreach w2 $pieces {
      if {$w2 < $w1} { continue }
      startrow
      foreach w3 $pieces {
        if {$w3 < $w1  ||  $w3 < $w2} {
          indent
        } else {
          tbstats "k$p($w1)$p($w2)$p($w3)-k"
        }
      }
      endrow
    }
  }
  endsection
}

proc tb33 {{type pawns}} {
  global p
  if {$type == "pawns"} {
    set pieces $::pieces
  } else {
    set pieces $::nopawns
  }
  section 3 3 $type
  foreach w1 $pieces {
    foreach w2 $pieces {
      if {$w2 < $w1} { continue }
      foreach b1 $pieces {
        if {$b1 < $w1} { continue }
        startrow
        foreach b2 $pieces {
          if {$b2 < $b1  ||  $b2 < $w1  ||  $b2 < $w2} {
            indent
          } else {
            tbstats "k$p($w1)$p($w2)-k$p($b1)$p($b2)"
          }
        }
        endrow
      }
    }
  }
  endsection
}

proc tb42 {{type pawns}} {
  global p
  if {$type == "pawns"} {
    set pieces $::pieces
  } else {
    set pieces $::nopawns
  }
  section 4 2 $type
  foreach w1 $pieces {
    foreach w2 $pieces {
      if {$w2 < $w1} { continue }
      foreach w3 $pieces {
        if {$w3 < $w1  ||  $w3 < $w2} { continue }
        startrow
        foreach b $pieces {
          tbstats "k$p($w1)$p($w2)$p($w3)-k$p($b)"
        }
        endrow
      }
    }
  }
  endsection
}

proc tb51 {{type pawns}} {
  global p
  if {$type == "pawns"} {
    set pieces $::pieces
  } else {
    set pieces $::nopawns
  }
  section 5 1 $type
  foreach w1 $pieces {
    foreach w2 $pieces {
      if {$w2 < $w1} { continue }
      foreach w3 $pieces {
        if {$w3 < $w1  ||  $w3 < $w2} { continue }
        startrow
        foreach w4 $pieces {
          if {$w4 < $w1  ||  $w4 < $w2  ||  $w4 < $w3} { 
            indent 
          } else {
            tbstats "k$p($w1)$p($w2)$p($w3)$p($w4)-k"
          }
        }
        endrow
      }
    }
  }
  endsection
}

proc tb43 {{type pawns}} {
  global p
  if {$type == "pawns"} {
    set pieces $::pieces
  } else {
    set pieces $::nopawns
  }
  section 4 3 $type
  foreach w1 $pieces {
    foreach w2 $pieces {
      if {$w2 < $w1} { continue }
      foreach w3 $pieces {
        if {$w3 < $w1  ||  $w3 < $w2} { continue }
        foreach b1 $pieces {
          startrow
          foreach b2 $pieces {
            if {$b2 < $b1} {
              indent
            } else {
              tbstats "k$p($w1)$p($w2)$p($w3)-k$p($b1)$p($b2)"
            }
          }
          endrow
        }
      }
    }
  }
  endsection
}

proc tb52 {{type pawns}} {
  global p
  if {$type == "pawns"} {
    set pieces $::pieces
  } else {
    set pieces $::nopawns
  }
  section 5 2 $type
  foreach w1 $pieces {
    foreach w2 $pieces {
      if {$w2 < $w1} { continue }
      foreach w3 $pieces {
        if {$w3 < $w1  ||  $w3 < $w2} { continue }
        foreach w4 $pieces {
          if {$w4 < $w1  ||  $w4 < $w2  ||  $w4 < $w3} { continue }
          startrow
          foreach b1 $pieces {
            tbstats "k$p($w1)$p($w2)$p($w3)$p($w4)-k$p($b1)"
          }
          endrow
        }
      }
    }
  }
}

proc tb61 {{type pawns}} {
  global p
  if {$type == "pawns"} {
    set pieces $::pieces
  } else {
    set pieces $::nopawns
  }
  section 6 1 $type
  foreach w1 $pieces {
    foreach w2 $pieces {
      if {$w2 < $w1} { continue }
      foreach w3 $pieces {
        if {$w3 < $w1  ||  $w3 < $w2} { continue }
        foreach w4 $pieces {
          if {$w4 < $w1  ||  $w4 < $w2  ||  $w4 < $w3} { continue }
          startrow
          foreach w5 $pieces {
            if {$w5 < $w1  ||  $w4 < $w2  ||  $w4 < $w3  ||  $w5 < $w4} { 
              indent 
            } else {
              tbstats "k$p($w1)$p($w2)$p($w3)$p($w4)$p($w5)-k"
            }
          }
          endrow
        }
      }
    }
  }
  endsection
}


foreach i {33 42 51 43 52 61} {
  proc "tb${i}p" {} "tb$i pawns"
  proc "tb${i}n" {} "tb$i nopawns"
}


proc usage {} {
  global argv0
  puts stderr "Usage: $argv0 \[-t|-1|-h\] <database> \[material ...\]"
  puts stderr "Option \"-t\" (default): plain text output."
  puts stderr "Option \"-1\": raw output, with one statistic per line."
  puts stderr "Option \"-h\": HTML-format output."
  puts stderr "Example: $argv0 -1 <database> 21 22 31 32 41 33"
  puts stderr "Default ($argv0 <database>) == $argv0 <database> 21 22 31 32 41 33 42 51 43 52 61"
  exit 1
}

### Main program:

if {[llength $argv] < 1} { usage }
set arg 0

set mode text
if {[lindex $argv $arg] == "-1"} {
  incr arg
  set mode raw
}
if {[string range [lindex $argv $arg] 0 1] == "-h"} {
  incr arg
  set mode html
}
if {[string range [lindex $argv $arg] 0 1] == "-t"} {
  incr arg
  set mode text
}

set perMillion 0
if {[lindex $argv $arg] == "-m"} {
  incr arg
  set perMillion 1
}

set db [lindex $argv $arg]
if {[catch { sc_base open $db }]} {
    puts stderr "Error: could not open the Scid database: $db"
    exit 1
}

incr arg
set argv [lrange $argv $arg end]

set date [clock format [clock seconds] -format "%d %b %Y"]
set nGames [sc_base numGames [sc_base current]]

if {$mode == "html"} {
  puts "<html><title>Tablebase endgame frequency statistics</title>"
  puts {<body bgcolor="#FFFFFF">}
  puts {<h1><font color="#000070">Tablebase endgame frequency statistics</font></h1>}
  puts {}
  puts "<p>Tablebase endgame frequency statistics by Scid [sc_info version]"
  puts {(<a href="http://scid.sourceforge.net/">scid.sourceforge.net</a>)<br>}
  puts "Generated from the database \"$db\" ($nGames games) on $date"
  if {$perMillion} {
    puts "<br>"
    puts "Values are occurrences per million games."
  }
  puts "</p>"
} else {
  puts "# Tablebase endgame frequency statistics by Scid [sc_info version] (scid.sourceforge.net)"
  puts "# Generated from the database \"$db\" ($nGames games) on $date"
  if {$perMillion} {
    puts "# Values are occurrences per million games."
  }
}

if {[llength $argv] == 0} {
  set argv [list 21  22 31  32 41  33 42 51  43 52 61]
}

if {$mode == "html"} {
  puts {<p><b><font color="#000070">Contents</font></b><br>}
  puts <ul>
  set count 0
  set prev 0
  foreach i $argv {
    set w [string index $i 0]
    set b [string index $i 1]
    set sum [expr $w + $b]
    if {$prev != $sum} {
      puts -nonewline " <li> [expr $w+$b] men: "
    } else {
      puts -nonewline " / "
    }
    set prev $sum
    incr count
    puts "  <a href=\"\#$w-$b\">$w-$b</a>"
  }
  puts "</ul><hr>"
}

foreach i $argv {
  catch {tb$i}
}

if {$mode == "html"} {
  puts "\n<hr>\n</body>\n</html>"
}
