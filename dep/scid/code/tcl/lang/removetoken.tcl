#!/usr/bin/tclsh
###
### removetoken.tcl
###

# (C) Pascal Georges 2007, Uwe Klimmek 2020
#
# Will remove entries containing a token from the argumentlist

array set encodings {
  czech iso8859-2
  deutsch iso8859-1
  francais utf-8
  hungary iso8859-2
  italian iso8859-1
  nederlan iso8859-1
  norsk iso8859-1
  polish iso8859-2
  portbr iso8859-1
  russian utf-8
  serbian iso8859-2
  spanish iso8859-1
  swedish iso8859-1
  catalan iso8859-1
  suomi iso8859-1
  greek utf-8
}

set languages {czech deutsch francais hungary italian nederlan norsk polish
  portbr spanish swedish serbian russian catalan suomi greek
}

################################################################################
proc remove {langfile enc tokens} {
  # Read the language file

  set f [open $langfile.tcl r]
  fconfigure $f -encoding $enc
  set data [read $f]
  close $f
  set langData [split $data "\n"]
  set tokenList [split $tokens " "]
  #split generates an additional "\n", delete this
  set langData [lreplace $langData end end]

  set fnew [open $langfile.tcl.new w]
  fconfigure $fnew -encoding $enc
  set removenext 0
  set removebrace 0
  foreach line $langData {
    set fields [split $line]
    set command [lindex $fields 0]
    set lang [lindex $fields 1]
    set name [lindex $fields 2]
    if { $removebrace } {
        set removebrace 0
        if { [string first "\}" $line] < 0 } {
          set removebrace 1
        }
    } elseif { $removenext } {
        set removenext 0
        if { [string index $line end] == "\\" } {
          set removenext 1
        }
    } elseif { $command == "translate" || $command == "menuText" } {
        foreach token $tokenList {
            set found 0
            if { $name == $token } {
                set found 1
                break
            }
        }
        if { ! $found } {
            puts $fnew $line
        } else {
            if { [string index $line end] == "\\" } {
                set removenext 1
            } elseif { [string first "\{" $line] >= 0 &&  [string first "\}" $line] < 0} {
                set removebrace 1
            }
        }
    } else {
        puts $fnew $line
    }
  }
  close $fnew
}
################################################################################

set token $argv

foreach language $languages {
  remove $language $encodings($language) $token
}

# end of file
