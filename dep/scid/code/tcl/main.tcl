# Copyright (C) 1999-2004 Shane Hudson
# Copyright (C) 2006-2009 Pascal Georges
# Copyright (C) 2008-2011 Alexander Wagner
# Copyright (C) 2013-2016 Fulvio Benini
#
# This file is part of Scid (Shane's Chess Information Database).
#
# Scid is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation.
#
# Scid is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Scid.  If not, see <http://www.gnu.org/licenses/>.

###
### main.tcl: Routines for creating and updating the main window.
###

############################################################
# Keyboard move entry:
#   Handles letters, digits and BackSpace/Delete keys.
#   Note that king- and queen-side castling moves are denoted
#   "OK" and "OQ" respectively.
#   The letters n, r, q, k, o and l are promoted to uppercase
#   automatically. A "b" can match to a b-pawn or Bishop move,
#   so in some rare cases, a capital B may be needed for the
#   Bishop move to distinguish it from the pawn move.

set moveEntry(Text) ""
set moveEntry(List) {}

proc moveEntry_Clear {} {
    global moveEntry
    set moveEntry(Text) ""
    set moveEntry(List) {}
}

proc moveEntry_Complete {} {
    global moveEntry
    set len [llength $moveEntry(List)]
    if {$len > 0} {
        set move [lindex $moveEntry(List) 0]
        if {$move == "OK"} { set move "O-O" }
        if {$move == "OQ"} { set move "O-O-O" }
        moveEntry_Clear
        addSanMove $move
    }
}

proc moveEntry_Backspace {} {
    global moveEntry
    set moveEntry(Text) [string range $moveEntry(Text) 0 \
            [expr {[string length $moveEntry(Text)] - 2}]]
    set moveEntry(List) [sc_pos matchMoves $moveEntry(Text) $moveEntry(Coord)]
    updateStatusBar
}

proc moveEntry_Char {ch} {
    global moveEntry
    set oldMoveText $moveEntry(Text)
    set oldMoveList $moveEntry(List)
    append moveEntry(Text) $ch
    set moveEntry(List) [sc_pos matchMoves $moveEntry(Text) $moveEntry(Coord)]
    set len [llength $moveEntry(List)]
    if {$len == 0} {
        # No matching moves, so do not accept this character as input:
        set moveEntry(Text) $oldMoveText
        set moveEntry(List) $oldMoveList
    } elseif {$len == 1} {
        # Exactly one matching move, so make it if AutoExpand is on,
        # or if it equals the move entered. Note the comparison is
        # case insensitive to allow for 'b' to match both pawn and
        # Bishop moves.
        set move [string tolower [lindex $moveEntry(List) 0]]
        
        if {$moveEntry(AutoExpand) > 0  ||
            ![string compare [string tolower $moveEntry(Text)] $move]} {
            return [moveEntry_Complete]
        }
    } elseif {$len == 2} {
        # Check for the special case where the user has entered a b-pawn
        # capture that clashes with a Bishop move (e.g. bxc4 and Bxc4):
        set first [string tolower [lindex $moveEntry(List) 0]]
        set second [string tolower [lindex $moveEntry(List) 1]]
        if {[string equal $first $second]} {
            set moveEntry(List) [list $moveEntry(Text)]
            return [moveEntry_Complete]
        }
    }
    updateStatusBar
}

# updateMainGame:
#   Updates the main board with games's info
#
proc updateMainGame {} {
    global gamePlayers
    set gamePlayers(nameW)  [sc_game info white]
    set gamePlayers(nameB)  [sc_game info black]
    set eloW                [sc_game info welo]
    set gamePlayers(eloW)   [expr {$eloW == 0 ? "" : "($eloW)"}]
    set eloB                [sc_game info belo]
    set gamePlayers(eloB)   [expr {$eloB == 0 ? "" : "($eloB)"}]
    set gamePlayers(clockW) ""
    set gamePlayers(clockB) ""
}

# updateTitle:
#   Updates the main Scid window title.
#
proc updateTitle {} {
    set title "Scid - "
    set fname [sc_base filename $::curr_db]
    set fname [file tail $fname]
    append title "$fname ($::tr(game) "
    append title "[::utils::thousands [sc_game number]] / "
    append title "[::utils::thousands [sc_base numGames $::curr_db]])"
    ::setTitle . $title
    set white [sc_game info white]
    set black [sc_game info black]
    if {[string length $white] > 2 &&  [string length $black] > 2} {
        if {$fname == {[clipbase]} } { set fname clipbase }
        set altered ""
        if {[sc_game altered]} { set altered "*" }
        ::setTitle .main "($fname$altered): $white -- $black"
    } else {
        ::setTitle .main $title
    }
}

# updateStatusBar:
#   Updates the main Scid window status bar.
#
proc updateStatusBar {} {
    if {! [winfo exists .main]} { return }

    if {$::menuHelpMessage != ""} {
        ::board::setInfoAlert .main.board "[tr Help]:" "$::menuHelpMessage" "black" ""
        return
    }

    if {$::autoplayMode == 1} {
        ::board::setInfoAlert .main.board "Autoplay:" [tr Stop] "red" "cancelAutoplay"
        return
    }

    if {[info exists ::playMode]} {
        set pInfo [eval "$::playMode info"]
        if {[llength $pInfo] != 4} {
            ::board::setInfoAlert .main.board "Playing..." [tr Stop] "red" {eval "$::playMode stop"}
        } else {
            ::board::setInfoAlert .main.board {*}pInfo
        }
        return
    }

    # show [%clk] command (if we are not playing)
    set toMove  [sc_pos side]
    set comment [sc_pos getComment]
    if { ![gameclock::isRunning] } {
        set ::gamePlayers(clockW) ""
        set ::gamePlayers(clockB) ""
        set clkExp {.*?\[%clk\s*(.*?)\s*\].*}
        set prevCom [sc_pos getPrevComment]
        if {$toMove == "white"} {
            regexp $clkExp $comment -> ::gamePlayers(clockB)
            regexp $clkExp $prevCom -> ::gamePlayers(clockW)
        } else {
            regexp $clkExp $comment -> ::gamePlayers(clockW)
            regexp $clkExp $prevCom -> ::gamePlayers(clockB)
        }
    }

    if {[info exists ::guessedAddMove]} {
        set ::gameLastMove [lindex $::guessedAddMove 1]
        ::board::setInfoAlert .main.board [lindex $::guessedAddMove 0] "\[click to change\]" "DodgerBlue3" ".main.menuaddchoice"
        unset ::guessedAddMove
        return
    }

    global moveEntry
    if {$moveEntry(Text) != ""} {
        set msg "\[ $moveEntry(Text) \]  "
        foreach thisMove $moveEntry(List) {
            append msg "$thisMove "
        }
        ::board::setInfoAlert .main.board "Enter Move:" "$msg" "DodgerBlue3" ""
        return
    }

    # remove technical comments, notify only human readable ones
    regsub -all {\[%.*\]} $comment {} comment

    if {$comment != ""} {
        ::board::setInfoAlert .main.board "Comment:" "$comment" "green" "::makeCommentWin"
        ::board::addInfo .main.board [sc_game info ECO]
        return
    }
    

    set statusBar ""
    set move [sc_game info previousMoveNT]
    if {$move != ""} {
      regsub {K} $move "\u2654" move
      regsub {Q} $move "\u2655" move
      regsub {R} $move "\u2656" move
      regsub {B} $move "\u2657" move
      regsub {N} $move "\u2658" move
      set number "[sc_pos moveNumber]"
      if {$toMove == "white"} {
        incr number -1
        append number ".."
      }
      append statusBar "Last move"
      if {[sc_var level] != 0} { append statusBar " (var)" }
      append statusBar ": $number.$move"
      ::board::setInfo .main.board "$statusBar"
    } else {
      set msg "[sc_game info date] - [sc_game info event]"
      ::board::setInfoAlert .main.board "[tr Event]:" $msg "DodgerBlue3" "::crosstab::Open"
    }
    ::board::addInfo .main.board [sc_game info ECO]
}

proc updateMainToolbar {} {
  if {[sc_pos isAt start]} {
    ::board::setButtonCmd .main.board leavevar ""
    ::board::setButtonCmd .main.board back ""
    catch { unset ::gameInfoBar(tb_BD_Start) }
  } else {
    ::board::setButtonCmd .main.board leavevar "::move::ExitVarOrStart"
    ::board::setButtonCmd .main.board back "::move::Back"
    set ::gameInfoBar(tb_BD_Start) "::move::Start"
  }
  if {[sc_pos isAt end] || [sc_pos isAt vend]} {
    ::board::setButtonCmd .main.board forward ""
    ::board::setButtonCmd .main.board endvar ""
    catch { unset ::gameInfoBar(tb_BD_End) }
    catch { unset ::gameInfoBar(tb_BD_Autoplay) }
  } else {
    ::board::setButtonCmd .main.board forward "::move::Forward"
    ::board::setButtonCmd .main.board endvar "::move::EndVar"
    set ::gameInfoBar(tb_BD_End) "::move::End"
    set ::gameInfoBar(tb_BD_Autoplay) "startAutoplay"
  }

  if {[sc_var level] == 0} {
    catch { unset ::gameInfoBar(tb_BD_VarDelete) }
    catch { unset ::gameInfoBar(tb_BD_VarPromote) }
    catch { unset ::gameInfoBar(tb_BD_VarLeave) }
    catch { unset ::gameInfoBar(tb_BD_BackToMainline) }
  } else {
    set ::gameInfoBar(tb_BD_VarDelete) { ::pgn::deleteVar }
    set ::gameInfoBar(tb_BD_VarPromote) { ::pgn::mainVar }
    set ::gameInfoBar(tb_BD_VarLeave) { ::move::ExitVar }
    set ::gameInfoBar(tb_BD_BackToMainline) { while {[sc_var level] != 0} {::move::ExitVar} }
  }

  set ::gameInfoBar(tb_BD_SetupBoard) "setupBoard"
}

# Update the "tree" filter of databases that have a view (gamelist or tree windows)
# which depends on the current position.
proc ::updateTreeFilter {{base ""}} {
    if { [info exists ::treeFilterUpdating_] } {
        set ::treeFilterUpdating_ {}
        ::progressBarCancel
        return
    }

    set ::treeFilterUpdating_ {}
    set ::treeFilterUpdatingBases_ [::windows::gamelist::listTreeBases $base]
    lappend ::treeFilterUpdatingBases_ {*}[::tree::listTreeBases $base]
    foreach elem [lsort -unique -index 0 $::treeFilterUpdatingBases_] {
        lassign $elem base filter progressbar

        set ::treeFilterUpdating_ [lsearch -all -inline -exact -index 0 $::treeFilterUpdatingBases_ $base]
        if { [llength $::treeFilterUpdating_] == 0 } {
            # canceled while updating another base
            continue
        }

        #TODO: don't do a full database search if there is only one filter.
        #set n_filters [llength [lsort -unique -index 1 $::treeFilterUpdating_]]

        eval progressBarSet $progressbar
        set err [catch {sc_filter search $base "tree" board}]
        if {$err && $::errorCode != $::ERROR::UserCancel} {
            ERROR::MessageBox
        }
        if { [llength $::treeFilterUpdating_] == 0 } {
            # Restart if the position changed before the update finished.
            after idle {
                unset ::treeFilterUpdating_
                ::updateTreeFilter
            }
            return
        }
        ::notify::DatabaseModified $base tree
    }
    unset ::treeFilterUpdating_
}

proc ::cancelUpdateTreeFilter {progressbar} {
    if {![info exists ::treeFilterUpdating_]} {
        return
    }
    set idx [lsearch -exact -index 2 $::treeFilterUpdating_ $progressbar]
    if {$idx != -1} {
        if {[llength $::treeFilterUpdating_] == 1} {
            ::progressBarCancel
        } else {
            set ::treeFilterUpdating_ [lreplace $::treeFilterUpdating_ $idx $idx]
        }
    } else {
        set idx [lsearch -exact -index 2 $::treeFilterUpdatingBases_ $progressbar]
        if {$idx != -1} {
            set ::treeFilterUpdatingBases_ [lreplace $::treeFilterUpdatingBases_ $idx $idx]
        }
    }
}

proc toggleRotateBoard {} {
    ::board::flip .main.board
}




############################################################
### The board:

proc toggleShowMaterial {} {
    board::toggleMaterial .main.board
}

# MouseWheel in main window:
proc main_mousewheelHandler {direction} {
    if {$direction < 0} {
        ::move::Back
    } else {
        ::move::Forward
    }
}

################################################################################
# added by Pascal Georges
# returns a list of num moves from main line following current position
################################################################################
proc getNextMoves { {num 4} } {
    set tmp ""
    set count 0
    while { [sc_game info nextMove] != "" && $count < $num} {
        append tmp " [sc_game info nextMove]"
        sc_move forward
        incr count
    }
    sc_move back $count
    return $tmp
}
################################################################################
# displays a box with main line and variations for easy selection with keyboard
################################################################################
proc showVars {} {
    if {$::autoplayMode == 1} { return }
    
    # No need to display an empty menu
    if {[sc_var count] == 0} {
        return
    }
    
    if {[sc_var count] == 1 &&  [sc_game info nextMove] == ""} {
        # There is only one variation and no main line, so enter it
        sc_var moveInto 0
        updateBoard
        return
    }

    set w .variations
    if {[winfo exists $w]} { return }
    
    set varList [sc_var list]
    set numVars [sc_var count]
    
    # Present a menu of the possible variations
    toplevel $w
    ::setTitle $w $::tr(Variations)
    setWinLocation $w
    set h [expr $numVars + 1]
    if { $h> 19} { set h 19 }
    ttk::treeview $w.lbVar -columns {0} -show {} -selectmode browse
    $w.lbVar configure -height $h
    $w.lbVar column 0 -width 250
    pack $w.lbVar -side left -fill both -expand 1
    
    #insert main line
    set move [sc_game info nextMove]
    set j 0
    if {$move == ""} {
        set move "($::tr(empty))"
    } else  {
        $w.lbVar insert {} end -id $j -values [list "0: [getNextMoves 5]"]
        incr j
    }
    
    # insert variations
    for {set i 0} {$i < $numVars} {incr i} {
        set move [::trans [lindex $varList $i]]
        if {$move == ""} {
            set move "($::tr(empty))"
        } else  {
            sc_var moveInto $i
            append move [getNextMoves 5]
            sc_var exit
        }
        set str "[expr {$i + 1}]: $move"
        $w.lbVar insert {} end -id $j -values [list "$str"]
        incr j
    }
    $w.lbVar selection set 0
    # bindings
    bind $w <Configure> "recordWinSize $w"
    bind .variations <Return> {catch { event generate .variations <Right> } }
    bind .variations <ButtonRelease-1> {catch { event generate .variations <Right> } }
    bind .variations <Right> {
        set cur [.variations.lbVar selection]
        destroy .variations
        if {$cur == 0} {
            sc_move forward; updateBoard -animate
        } else  {
            sc_var moveInto [expr $cur - 1]; updateBoard -animate
        }
    }
    bind .variations <Up> { set cur [.variations.lbVar selection]
        set sel [expr $cur - 1]
        if {$sel < 0} { set sel 0 }
        .variations.lbVar selection set $sel ; .variations.lbVar see $sel}
    bind .variations <Down> { set cur [.variations.lbVar selection]
        if {[.variations.lbVar next $cur] == {} } { return }
        set sel [expr $cur + 1]
        .variations.lbVar selection set $sel ; .variations.lbVar see $sel}
    bind .variations <Left> { destroy .variations }
    bind .variations <Escape> { catch { event generate .variations <Destroy> } }
    # in order to have the window always on top : this does not really work ...
    bind .variations <Visibility> {
        if { "%s" != "VisibilityUnobscured" } {
            focus .variations
            raise .variations
        }
    }
    bind .variations <FocusOut> {
        focus .variations
        raise .variations
    }
    
    # Needed or the main window loses the focus
    bind .variations <Destroy> { focus -force .main }

    catch { focus .variations }
    catch { grab $w }
    update
}
################################################################################
#
################################################################################

# updateBoard:
#    Updates the main board.
#    If a parameter "-pgn" is specified, the PGN text is also regenerated.
#    If a parameter "-animate" is specified, board changes are animated.
#
proc updateBoard {args} {
    ::notify::PosChanged {*}$args
}


# updateGameInfo:
#    Update the game status window .main.gameInfo
#
proc updateGameInfo {} {
    global gameInfo

    .main.gameInfo.text configure -state normal
    .main.gameInfo.text delete 0.0 end
    ::htext::display .main.gameInfo.text [sc_game info -hide $gameInfo(hideNextMove) \
            -material $gameInfo(showMaterial) \
            -cfull $gameInfo(fullComment) \
            -fen $gameInfo(showFEN) -tb $gameInfo(showTB)]
    if {$gameInfo(wrap)} {
        .main.gameInfo.text configure -wrap word
        .main.gameInfo.text tag configure wrap -lmargin2 10
        .main.gameInfo.text tag add wrap 1.0 end
    } else {
        .main.gameInfo.text configure -wrap none
    }
    .main.gameInfo.text configure -state disabled
    togglePhotosSize 0
}

set photosMinimized 0
proc togglePhotosSize {{toggle 1}} {
    place forget .main.photoW
    place forget .main.photoB
    if {! $::gameInfo(photos)} { return }

    updatePlayerPhotos
    if {$toggle} { set ::photosMinimized [expr !$::photosMinimized] }

    set distance [expr {[image width photoB] + 2}]
    if { $distance < 10 } { set distance 82 }

    if {$::photosMinimized} {
        place .main.photoW -in .main.gameInfo.text -x -17 -relx 1.0 -relheight 0.15 -width 15 -anchor ne
        place .main.photoB -in .main.gameInfo.text -x -1 -relx 1.0  -relheight 0.15 -width 15 -anchor ne
    } else  {
        place .main.photoW -in .main.gameInfo.text -x -$distance -relx 1.0 -relheight 1 -width [image width photoW] -anchor ne
        place .main.photoB -in .main.gameInfo.text -x -1 -relx 1.0 -relheight 1 -width [image width photoB] -anchor ne
    }
}


# readPhotoFile executed once at startup for each SPF file. Loads SPI file if it exists.
# Otherwise it generates index information and tries to write SPI file to disk (if it can be done)
proc readPhotoFile {fname} {
    set count 0
    set writespi 0
    
    if {! [regsub {\.spf$} $fname {.spi} spi]} {
        # How does it happend?
        return
    }
    
    # If SPI file was found then just source it and exit
    if { [file readable $spi]} {
        set count [array size ::unsafe::spffile]
        safeSource $spi fname $fname
        set newcount [array size ::unsafe::spffile]
        if {[expr $newcount - $count] > 0} {
            ::splash::add "Found [expr $newcount - $count] player photos in [file tail $fname]"
            ::splash::add "Loading information from index file [file tail $spi]"
            return [expr $newcount - $count]
        } else {
            set count 0
        }
    }
    
    # Check for the absence of the SPI file and check for the write permissions
    if { ![file exists $spi] && ![catch {open $spi w} fd_spi]} {
        # SPI file will be written to disk by scid
        set writespi 1
    }
    
    if {! [file readable $fname]} { return }
    
    set fd [open $fname]
    while {[gets $fd line] >= 0} {
        # search for the string      photo "Player Name"
        if { [regexp {^photo \"(.*)\" \{$} $line -> name] } {
            set count [expr $count + 1 ]
            set begin [tell $fd]
            # skip data block
            while {1} {
                set end [tell $fd]
                gets $fd line
                if {[regexp {.*\}.*} $line ]} {break}
            }
            set trimname [trimString $name]
            set size [expr $end - $begin ]
            set ::unsafe::photobegin($trimname) $begin
            set ::unsafe::photosize($trimname) $size
            set ::unsafe::spffile($trimname) $fname
            if { $writespi } {
                # writing SPI file to disk
                puts $fd_spi "set \"photobegin($trimname)\" $begin"
                puts $fd_spi "set \"photosize($trimname)\" $size"
                puts $fd_spi "set \"spffile($trimname)\" \"\$fname\""
            }
        }
    }
    if {$count > 0 && $writespi} {
        ::splash::add "Found $count player photos in [file tail $fname]"
        ::splash::add "Index file [file tail $spi] was generated succesfully"
    }
    if {$count > 0 && !$writespi} {
        ::splash::add "Found $count player photos in [file tail $fname]"
        ::splash::add "Could not generate index file [file tail $spi]"
        ::splash::add "Use spf2spi script to generate [file tail $spi] file "
    }
    
    if { $writespi } { close $fd_spi }
    close $fd
    return $count
}


#convert $data string tolower case and strip the first two blanks.
proc trimString {data} {
    set data [string tolower $data]
    set strindex [string first "\ " $data]
    set data [string replace $data $strindex $strindex]
    set strindex [string first "\ " $data]
    set data [string replace $data $strindex $strindex]
    return $data
}


# retrieve photo from the SPF file using index information
proc getphoto {name} {
    set data ""
    if {[info exists ::unsafe::spffile($name)]} {
        set fd [open $::unsafe::spffile($name)]
        seek $fd $::unsafe::photobegin($name) start
        set data [read $fd $::unsafe::photosize($name) ]
        close $fd
    }
    return $data
}


proc loadPlayersPhoto {} {
  set ::gamePlayers(photoW) {}
  set ::gamePlayers(photoB) {}
  image create photo photoW
  image create photo photoB

  # Directories where Scid searches for the photo files
  set photodirs [list $::scidDataDir $::scidUserDir $::scidConfigDir [file join $::scidShareDir "photos"]]
  if {[info exists ::scidPhotoDir]} { lappend photodirs $::scidPhotoDir }

  # Read all Scid photo (*.spf) files in the Scid data/user/config directories:
  set nImg 0
  set nFiles 0
  foreach dir $photodirs {
      foreach photofile [glob -nocomplain -directory $dir "*.spf"] {
          set n [readPhotoFile $photofile]
          if {$n > 0} {
              incr nFiles
              incr nImg $n
          }
      }
  }

  return [list $nImg $nFiles]
}
loadPlayersPhoto

# Try to change the engine name: ignore version number, try to ignore blanks
# TODO: rename this function (spellcheck playernames, converts to lower case and remove spaces)
proc trimEngineName { engine } {
    catch {
        set spell_name [sc_name retrievename $engine]
        if {$spell_name != ""} { set engine $spell_name }
    }
    set engine [string tolower $engine]
    
    if { [string first "deep " $engine] == 0 } {
        # strip "deep "
        set engine [string range $engine 5 end]
    }
    # delete two first blank to make "The King" same as "TheKing"
    # or "Green Light Chess" as "Greenlightchess"
    set strindex [string first "\ " $engine]
    set engine [string replace $engine $strindex $strindex]
    set strindex [string first "\ " $engine]
    set engine [string replace $engine $strindex $strindex]
    set strindex [string first "," $engine]
    set slen [string len $engine]
    if { $strindex == -1 && $slen > 2 } {
        #seems to be a engine name:
        # search until longest name matches an engine name
        set slen [string len $engine]
        for { set strindex $slen} {![info exists ::unsafe::spffile([string range $engine 0 $strindex])]\
                    && $strindex > 2 } {set strindex [expr {$strindex - 1}] } { }
        set engine [string range $engine 0 $strindex]
    }
    return $engine
}


# updatePlayerPhotos
#   Updates the images photoW and photoB for the two players of the current game.
#
proc updatePlayerPhotos {{force ""}} {
    foreach {name img} {nameW photoW nameB photoB} {
        set spellname $::gamePlayers($name)
        if {$::gamePlayers($img) != $spellname} {
            set ::gamePlayers($img) $spellname
            catch { set spellname [trimEngineName $spellname] }
            image create photo $img -data [getphoto $spellname]
        }
    }
}

#########################################################
### Chess move input

# Globals for mouse-based move input:

set selectedSq -1
set bestSq -1

set EMPTY 0
set KING 1
set QUEEN 2
set ROOK 3
set BISHOP 4
set KNIGHT 5
set PAWN 6

################################################################################
#
################################################################################
proc getPromoPiece {} {
    set w .promoWin
    set ::result 2
    toplevel $w
    # wm transient $w .main
    ::setTitle $w "Scid"
    wm resizable $w 0 0
    set col "w"
    if { [sc_pos side] == "black" } { set col "b" }
    ttk::button $w.bq -image ${col}q45 -command "set ::result 2 ; destroy $w"
    ttk::button $w.br -image ${col}r45 -command "set ::result 3 ; destroy $w"
    ttk::button $w.bb -image ${col}b45 -command "set ::result 4 ; destroy $w"
    ttk::button $w.bn -image ${col}n45 -command "set ::result 5 ; destroy $w"
    pack $w.bq $w.br $w.bb $w.bn -side left
    bind $w <Escape> "set ::result 2 ; destroy $w"
    bind $w <Return> "set ::result 2 ; destroy $w"
    update
    catch { grab $w }
    tkwait window $w
    return $::result
}

# TODO: remove this
# confirmReplaceMove:
#   Asks the user what to do when adding a move when a move already
#   exists.
#   Returns a string value:
#      "replace" to replace the move, truncating the game.
#      "var" to add the move as a new variation.
#      "cancel" to do nothing.
#

proc confirmReplaceMove {} {
    if {[winfo exists $::reviewgame::window]} {
        return "var"
    }

    option add *Dialog.msg.wrapLength 4i interactive
    catch {tk_dialog .dialog "Scid: $::tr(ReplaceMove)?" \
                $::tr(ReplaceMoveMessage) "" 0 \
                $::tr(ReplaceMove) $::tr(NewMainLine) \
                $::tr(AddNewVar) $::tr(Cancel)} answer
    option add *Dialog.msg.wrapLength 3i interactive
    if {$answer == 0} { return "replace" }
    if {$answer == 1} { return "mainline" }
    if {$answer == 2} { return "var" }
    return "cancel"
}

proc addNullMove {} {
    addMove null null
}

proc addMove { sq1 sq2 {animate "-animate"}} {
    global EMPTY
    set nullmove 0
    if {$sq1 == "null"  &&  $sq2 == "null"} { set nullmove 1 }
    if {!$nullmove  &&  [sc_pos isLegal $sq1 $sq2] == 0} {
        # Illegal move, but if it is King takes king then treat it as
        # entering a null move:
        set board [sc_pos board]
        set k1 [string tolower [string index $board $sq1]]
        set k2 [string tolower [string index $board $sq2]]
        if {$k1 == "k"  &&  $k2 == "k"} { set nullmove 1 } else { return }
    }
    if {$nullmove} {
        if {[sc_pos isCheck]} { return }
        set moveUCI "0000"
    } else {
        set moveUCI [::board::san $sq2][::board::san $sq1]
    }
    addMoveUCI $moveUCI "" $animate
}

proc addSanMove { {san} } {
    set err [catch { sc_game SANtoUCI $san } moveUCI ]
    if {! $err} { addMoveUCI $moveUCI }
    return $err
}

# addMoveUCI:
#   Adds the move indicated if it is legal.
#   If the move is a promotion, getPromoPiece will be called
#   to get the promotion piece from the user.
#
proc addMoveUCI {{moveUCI} {action ""} {animate "-animate"}} {
    set sq1 [::board::sq [string range $moveUCI 0 1] ]
    set sq2 [::board::sq [string range $moveUCI 2 3] ]
    if { [::fics::setPremove $sq1 $sq2] || ! [::fics::playerCanMove] || ! [::reviewgame::playerCanMove]} { return } ;# not player's turn

    if { [string length $moveUCI] == 4 && $sq1 != $sq2 && [sc_pos isPromotion $sq1 $sq2] } {
        switch -- [getPromoPiece] {
            2 { set promoLetter "q"}
            3 { set promoLetter "r"}
            4 { set promoLetter "b"}
            5 { set promoLetter "n"}
            default {set promoLetter ""}
        }
        append moveUCI $promoLetter
    }

    if {! $::annotateMode} {
        if {[::move::Follow $moveUCI]} { return [updateBoard $animate] }
    }


    if {![sc_pos isAt vend]} {
        if {$action == ""} {
            set replacedmove ""
            set n [sc_var count]
            if {$n == 0} {
                sc_move forward
                if {[sc_pos isAt vend]} {
                    set replacedmove [sc_game info previousMoveNT]
                }
                sc_move back
            }
            if {$replacedmove != ""} {
                set ::guessedAddMove [list "Replaced Move $replacedmove"]
            } else {
                set action "var"
            }
        }

        switch -- $action {
            mainline { set ::guessedAddMove [list "New Main Line"]}
            var      { set ::guessedAddMove [list "New Variation"]}
            replace  { set ::guessedAddMove [list "Replaced Main Line"]}
        }
        lappend ::guessedAddMove $moveUCI
    }

    undoFeature save
    if {($action == "mainline" || $action == "var") && ![sc_pos isAt vend]} {
        sc_var create
    }

    if {$moveUCI == "0000"} {
        sc_move addSan null
    } else {
        sc_move addUCI $moveUCI
    }
    if {$action == "mainline"} {
        sc_var promote
        sc_move forward 1
    }

    set ::sergame::lastPlayerMoveUci "$moveUCI"

    if {[winfo exists .fics]} {
        if { [::fics::playerCanMove] } {
            if { [string length $moveUCI] == 5 } {
                set promoletter [ string tolower [ string index $moveUCI end ] ]
                ::fics::writechan "promote $promoletter"
            }
            ::fics::writechan [ string range [sc_game info previousMoveUCI] 0 3 ]
            ::fics::storeTime
        }
    }

    if {$::novag::connected} {
        ::novag::addMove "$moveUCI"
    }

    set san [sc_game info previous]
    after idle [list ::utils::sound::AnnounceNewMove $san]

    ::notify::PosChanged -pgn $animate
}

proc suggestMove {} {
    if {! $::suggestMoves} { return 0}
    if {[info exists ::playMode]} {
        return [eval "$::playMode suggestMove"]
    }
    if {$::fics::playing != 0} { return 0 }
    return 1
}

# enterSquare:
#   Called when the mouse pointer enters a board square.
#   Finds the best matching square for a move (if there is a
#   legal move to or from this square), and colors the squares
#   to indicate the suggested move.
#
proc enterSquare { square } {	
    global bestSq bestcolor selectedSq
    if {$selectedSq == -1} {
        set bestSq -1
        if {[::suggestMove]} {
            set bestSq [sc_pos bestSquare $square]
            if {$bestSq != -1} {
                ::board::colorSquare .main.board $square $bestcolor
                ::board::colorSquare .main.board $bestSq $bestcolor        
            }
        }
    }
}

# leaveSquare:
#    Called when the mouse pointer leaves a board square.
#    Recolors squares to normal (lite/dark) color.
#
proc leaveSquare { square } {
    global selectedSq bestSq
    if {$selectedSq == -1} {
        ::board::colorSquare .main.board $bestSq
        ::board::colorSquare .main.board $square  
    }
}

# pressSquare:
#    Called when the left mouse button is pressed on a square. Sets
#    that square to be the selected square.
#
proc pressSquare { square } {
    global selectedSq highcolor
    
    if { ![::fics::playerCanMove] || ![::reviewgame::playerCanMove] } { return } ;# not player's turn
    
    # if training with calculations of var is on, just log the event
    if { [winfo exists .calvarWin] } {
        ::calvar::pressSquare $square
        return
    }
    
    if {$selectedSq == -1} {
        set selectedSq $square
        ::board::colorSquare .main.board $square $highcolor
        # Drag this piece if it is the same color as the side to move:
        set c [string index [sc_pos side] 0]  ;# will be "w" or "b"
        set p [string index [::board::piece .main.board $square] 0] ;# "w", "b" or "e"
        if {$c == $p} {
            ::board::setDragSquare .main.board $square
        }
    } else {
        ::board::setDragSquare .main.board -1
        ::board::colorSquare .main.board $selectedSq
        ::board::colorSquare .main.board $square
        set tmp $selectedSq
        set selectedSq -1
        if {$square != $tmp} {
            addMove $square $tmp
        }
        enterSquare $square
    }
}

# releaseSquare:
#   Called when the left mouse button is released over a square.
#   If the square is different to that the button was pressed on, it
#   is a dragged move; otherwise it is just selecting this square as
#   part of a move.
#
proc releaseSquare { w x y } {
    if { [winfo exists .calvarWin] } { return }
    
    global selectedSq bestSq
    
    ::board::setDragSquare $w -1
    set square [::board::getSquare $w $x $y]
    if {$square < 0} {
        set selectedSq -1
        return
    }

    if {$square == $selectedSq} {
        if {[::suggestMove]} {
            # User pressed and released on same square, so make the
            # suggested move if there is one:
            set selectedSq -1
            ::board::colorSquare $w $bestSq
            ::board::colorSquare $w $square
            addMove $square $bestSq
            enterSquare $square
        } else {
            # Current square is the square user pressed the button on,
            # so we do nothing.
        }
    } elseif {$selectedSq != -1} {
        # User has dragged to another square, so try to add this as a move:
        set tmp $selectedSq
        set selectedSq -1
        addMove $square $tmp ""
        ::board::colorSquare $w $square
        ::board::colorSquare $w $tmp
    }
}


# backSquare:
#    Handles the retracting of a move (when the right mouse button is
#    clicked on a square). Recolors squares to normal color also.
#    If the move is the last in the game or variation, is is removed
#    by truncating the game after retracting the move.
#
proc backSquare {} {
    global selectedSq bestSq
    set lastMoveInLine 0
    if {[sc_pos isAt vend]} {
        set lastMoveInLine 1
    }
    sc_move back
    
    # RMB used to delete the move if it was the last in a line. Removed it as there is no undo.
    # if {[sc_pos isAt vstart] && [sc_var level] != 0} {
    # ::pgn::deleteVar [sc_var number]
    # } elseif {$lastMoveInLine} {
    # sc_game truncate
    # }
    
    set selectedSq -1
    set bestSq -1
    # update the board without -pgn option because of poor performance with long games
    updateBoard -animate
    ::utils::sound::AnnounceBack
}

# addMarker:
#   add/delete square markers and arrows to the current position
#
proc addMarker {w x y} {
    set sq [::board::getSquare $w $x $y]
    if {! [info exists ::markStartSq]} {
        set ::markStartSq [::board::san $sq]
        return
    }

    set from $::markStartSq
    unset ::markStartSq
    set to [::board::san $sq]
    if {$from == "" || $to == ""} { return }

    if {$from == $to } {
        set cmd "$::markType,$to,$::markColor"
        set cmd_erase "\[a-z\]*,$to,\[a-z\]*"
    } else {
        set cmd "arrow,$from,$to,$::markColor"
        set cmd_erase "arrow,$from,$to,\[a-z\]*"
    }
    set oldComment [sc_pos getComment]
    regsub -all " *\\\[%draw $cmd\\\]" $oldComment "" newComment
    if {$newComment == $oldComment} {
        regsub -all " *\\\[%draw $cmd_erase\\\]" $oldComment "" newComment
        append newComment " \[%draw $cmd\]"
    }

    sc_pos setComment $newComment
    ::notify::PosChanged -pgn
}

# addNag:
#   add a Nag to the current position
#
proc addNag {nag} {
    undoFeature save
    sc_pos addNag "$nag"
    ::notify::PosChanged -pgn
}

################################################################################
#
################################################################################
proc undoFeature {action} {
    if {$action == "save"} {
        sc_game undoPoint
    } elseif {$action == "undo"} {
        sc_game undo
        notify::GameChanged
    } elseif {$action == "redo"} {
        sc_game redo
        notify::GameChanged
    } elseif {$action == "undoAll"} {
        sc_game undoAll
        notify::GameChanged
    }
}

proc setPlayMode { callback } {
    set ::playMode "$callback"
    if {$::playMode == ""} { unset ::playMode }
    ::notify::PosChanged
}

################################################################################
# In docked mode, resize board automatically
################################################################################
proc resizeMainBoard {} {
  if { $::autoResizeBoard } {
    update idletasks
    set availw [winfo width .fdockmain]
    set availh [winfo height .fdockmain]
    if {$::showGameInfo} {
      set availh [expr $availh - [winfo height .main.gameInfo] ]
    }
    if { [llength [pack slaves .main.tb]] != 0 } {
      set availh [expr $availh - [winfo height .main.tb] ]
    }
    set ::boardSize [::board::resizeAuto .main.board "0 0 $availw $availh"]
  }
}
################################################################################
# sets visibility of gameInfo panel at the bottom of main board
proc toggleGameInfo {} {
  if {$::showGameInfo} {
    grid .main.gameInfo -row 3 -column 0 -sticky news
  } else  {
    grid forget .main.gameInfo
  }
  updateGameInfo
}
################################################################################

proc CreateMainBoard { {w} } {
  createToplevel $w
  setTitle $w [ ::tr "Board" ]

  CreateGameInfo

  ::board::new $w.board $::boardSize
  ::board::showMarks $w.board 1
  for {set i 0} {$i < $::boardCoords} {incr i} { ::board::coords $w.board }
  if {$::gameInfo(showMaterial)} { ::board::toggleMaterial $w.board }

  ::board::addNamesBar $w.board gamePlayers
  ::board::addInfoBar $w.board gameInfoBar

  set ::gameInfoBar(tb_BD_Coords) "set ::boardCoords \[::board::coords $w.board\]"
  set ::gameInfoBar(tb_BD_Material) "set ::gameInfo(showMaterial) \[::board::toggleMaterial $w.board\]"

  menu .main.menuaddchoice -bg white -font font_Regular
  .main.menuaddchoice add command -label " Undo" -image tb_BD_Undo -compound left \
      -command {undoFeature undo}
  .main.menuaddchoice add command -label " $::tr(ReplaceMove)" -image tb_BD_Replace -compound left \
      -command {sc_game undo; addMoveUCI $::gameLastMove replace}
  .main.menuaddchoice add command -label " $::tr(NewMainLine)" -image tb_BD_NewMainline -compound left \
      -command {sc_game undo; addMoveUCI $::gameLastMove mainline}
  .main.menuaddchoice add command -label " $::tr(AddNewVar)" -image tb_BD_NewVar -compound left \
      -command {sc_game undo; addMoveUCI $::gameLastMove var}

  InitToolbar .main.tb

  for {set i 0} { $i < 64 } { incr i } {
    ::board::bind $w.board $i <Enter> "enterSquare $i"
    ::board::bind $w.board $i <Leave> "leaveSquare $i"
    ::board::bind $w.board $i <ButtonPress-1> "pressSquare $i"
    ::board::bind $w.board $i <Control-ButtonPress-1> "addMarker $w.board %X %Y"
    ::board::bind $w.board $i <Control-ButtonRelease-1> "addMarker $w.board %X %Y"
    ::board::bind $w.board $i <B1-Motion> "::board::dragPiece $w.board %X %Y"
    ::board::bind $w.board $i <ButtonRelease-1> "releaseSquare $w.board %X %Y"
    ::board::bind $w.board $i <ButtonPress-$::MB3> backSquare
  }

  foreach i {o q r n k O Q R B N K} {
    bind $w <$i> "moveEntry_Char [string toupper $i]"
    bind $w <Alt-$i> { continue }

  }
  foreach i {a b c d e f g h 1 2 3 4 5 6 7 8} {
    bind $w <Key-$i> "moveEntry_Char $i"
    bind $w <Alt-$i> { continue }
  }

  bind $w <Control-BackSpace> backSquare
  bind $w <Control-Delete> backSquare
  bind $w <BackSpace> moveEntry_Backspace
  bind $w <Delete> moveEntry_Backspace
  bind $w <space> moveEntry_Complete
  bind $w <ButtonRelease> "focus $w"
  bind $w <Configure> {+::resizeMainBoard }
  bind $w <Return> { #TODO: improve this
    if {[winfo exists .analysisWin1] && $analysis(analyzeMode1)} {
        .analysisWin1.b1.move invoke
    }
  }

  bindMouseWheel $w "main_mousewheelHandler"

  ttk::frame $w.space
  grid $w.space -row 4 -column 0 -columnspan 3 -sticky nsew
  grid rowconfigure $w 3 -weight 0
  grid rowconfigure $w 4 -weight 1

  grid columnconfigure $w 0 -weight 1
  grid $w.board -row 2 -column 0 -sticky we ;# -padx 5 -pady 5

  toggleGameInfo
  updateStatusBar
  updateMainToolbar
  updateTitle
}

proc CreateGameInfo {} {
  # .gameInfo is the game information widget:
  #
  autoscrollText y .main.gameInfo .main.gameInfo.text Treeview
  .main.gameInfo.text configure -width 20 -height 6 -wrap none -state disabled -cursor top_left_arrow -setgrid 1
  ::htext::init .main.gameInfo.text

  # Set up player photos:
  ttk::label .main.photoW -image photoW -anchor ne
  ttk::label .main.photoB -image photoB -anchor ne
  bind .main.photoW <ButtonPress-1> "togglePhotosSize"
  bind .main.photoB <ButtonPress-1> "togglePhotosSize"

  # Right-mouse button menu for gameInfo frame:
  menu .main.gameInfo.menu -tearoff 0

  .main.gameInfo.menu add checkbutton -label GInfoHideNext \
          -variable gameInfo(hideNextMove) -offvalue 0 -onvalue 1 -command updateBoard

  .main.gameInfo.menu add checkbutton -label GInfoMaterial -variable gameInfo(showMaterial) -offvalue 0 -onvalue 1 \
          -command { toggleShowMaterial }

  .main.gameInfo.menu add checkbutton -label GInfoFEN \
          -variable gameInfo(showFEN) -offvalue 0 -onvalue 1 -command updateBoard

  .main.gameInfo.menu add checkbutton -label GInfoMarks \
          -variable gameInfo(showMarks) -offvalue 0 -onvalue 1 -command updateBoard

  .main.gameInfo.menu add checkbutton -label GInfoWrap \
          -variable gameInfo(wrap) -offvalue 0 -onvalue 1 -command updateBoard

  .main.gameInfo.menu add checkbutton -label GInfoFullComment \
          -variable gameInfo(fullComment) -offvalue 0 -onvalue 1 -command updateBoard

  .main.gameInfo.menu add checkbutton -label GInfoPhotos \
          -variable gameInfo(photos) -offvalue 0 -onvalue 1 \
          -command {togglePhotosSize 0}

  .main.gameInfo.menu add separator

  .main.gameInfo.menu add radiobutton -label GInfoTBNothing \
          -variable gameInfo(showTB) -value 0 -command updateBoard

  .main.gameInfo.menu add radiobutton -label GInfoTBResult \
          -variable gameInfo(showTB) -value 1 -command updateBoard

  .main.gameInfo.menu add radiobutton -label GInfoTBAll \
          -variable gameInfo(showTB) -value 2 -command updateBoard

  .main.gameInfo.menu add separator

  .main.gameInfo.menu add command -label GInfoDelete -command {
      sc_base gameflag [sc_base current] [sc_game number] invert del
      ::notify::DatabaseModified [sc_base current]
  }

  bind .main.gameInfo.text <ButtonPress-$::MB3> {
    tk_popup .main.gameInfo.menu %X %Y
  }

  storeMenuLabels .main.gameInfo.menu
}

# Set toolbar help status messages:
proc setToolbarTooltips { tb } {
    foreach {b m} {
	newdb FileNew open FileOpen finder FileFinder
	save GameReplace closedb FileClose bkm FileBookmarks
	gprev GamePrev gnext GameNext
	newgame GameNew copy EditCopy paste EditPaste
	boardsearch SearchCurrent
	headersearch SearchHeader materialsearch SearchMaterial
	switcher WindowsSwitcher glist WindowsGList pgn WindowsPGN tmt WindowsTmt
	maint WindowsMaint eco WindowsECO tree WindowsTree crosstab ToolsCross
	engine ToolsAnalysis } {
	::utils::tooltip::Set $tb.$b $::helpMessage($::language,$m)
    }
}

proc InitToolbar {{tb}} {
	ttk::frame $tb -relief raised -border 1
	ttk::button $tb.newdb -image tb_newdb -command ::file::New -padding {2 0}
	ttk::button .main.tb.open -image tb_open -command ::file::Open -padding {2 0}
	ttk::button .main.tb.save -image tb_save  -padding {2 0} -command {
	  if {[sc_game number] != 0} {
		#busyCursor .
		gameReplace
		# catch {.save.buttons.save invoke}
		#unbusyCursor .
	  } else {
		gameAdd
	  }
	}
	ttk::button .main.tb.closedb -image tb_closedb -command ::file::Close -padding {2 0}
	ttk::button .main.tb.finder -image tb_finder -command ::file::finder::Open -padding {2 0}
	ttk::menubutton .main.tb.bkm -image tb_bkm -menu .main.tb.bkm.menu -padding {2 0}
	menu .main.tb.bkm.menu
	::bookmarks::RefreshMenu .main.tb.bkm.menu

	ttk::frame .main.tb.space1 -width 4
	ttk::button .main.tb.newgame -image tb_newgame -command ::game::Clear -padding {2 0}
	ttk::button .main.tb.copy -image tb_copy -command ::gameAddToClipbase -padding {2 0}
	ttk::button .main.tb.paste -image tb_paste \
		-command {catch {sc_clipbase paste}; updateBoard -pgn} -padding {2 0}
	ttk::frame .main.tb.space2 -width 4
	ttk::button .main.tb.gprev -image tb_gprev -command {::game::LoadNextPrev previous} -padding {2 0}
	ttk::button .main.tb.gnext -image tb_gnext -command {::game::LoadNextPrev next} -padding {2 0}
	ttk::frame .main.tb.space3 -width 4
	ttk::button .main.tb.boardsearch -image tb_boardsearch -command ::search::board -padding {2 0}
	ttk::button .main.tb.headersearch -image tb_headersearch -command ::search::header -padding {2 0}
	ttk::button .main.tb.materialsearch -image tb_materialsearch -command ::search::material -padding {2 0}
	ttk::frame .main.tb.space4 -width 4
	ttk::button .main.tb.switcher -image tb_switcher -command ::windows::switcher::Open -padding {2 0}
	ttk::button .main.tb.glist -image tb_glist -command ::windows::gamelist::Open -padding {2 0}
	ttk::button .main.tb.pgn -image tb_pgn -command ::pgn::OpenClose -padding {2 0}
	ttk::button .main.tb.tmt -image tb_tmt -command ::tourney::toggle -padding {2 0}
	ttk::button .main.tb.maint -image tb_maint -command ::maint::OpenClose -padding {2 0}
	ttk::button .main.tb.eco -image tb_eco -command ::windows::eco::OpenClose -padding {2 0}
	ttk::button .main.tb.tree -image tb_tree -command ::tree::make -padding {2 0}
	ttk::button .main.tb.crosstab -image tb_crosstab -command ::crosstab::OpenClose -padding {2 0}
	ttk::button .main.tb.engine -image tb_engine -command makeAnalysisWin -padding {2 0}
	ttk::button .main.tb.help -image tb_help -command {helpWindow Index} -padding {2 0}

	foreach i {newdb open save closedb finder bkm newgame copy paste gprev gnext \
		  boardsearch headersearch materialsearch \
		  switcher glist pgn tmt maint eco tree crosstab engine help} {
	  .main.tb.$i configure -takefocus 0
	}

    setToolbarTooltips $tb
	redrawToolbar
}

proc toggleToolbarButton { b i } {
    if { $::toolbar_temp($i) } {
	set ::toolbar_temp($i) 0
	$b.$i state !pressed
    } else {
	set ::toolbar_temp($i) 1
	$b.$i state pressed
    }
    array set ::toolbar_state [array get ::toolbar_temp]
    redrawToolbar
}

proc toggleAllToolbarButtons { b state } {
    foreach i [array names ::toolbar_temp] {
	set ::toolbar_temp($i) $state
	if { $state } { $b.$i state pressed } else { $b.$i state !pressed }
    }
    array set ::toolbar_state [array get ::toolbar_temp]
    redrawToolbar
}

proc ConfigToolbar { w } {
  array set ::toolbar_temp [array get ::toolbar_state]
  pack [ttk::frame $w.f] -side top -fill x
  set col 0
  set row 0
  foreach i {newdb open closedb finder save bkm row gprev gnext row newgame copy paste row boardsearch headersearch \
		 materialsearch row switcher glist pgn tmt maint eco tree crosstab engine } {
      if { $i eq "row" } { incr row; set col 0 } else {
	  ttk::button $w.f.$i -image tb_$i -command "toggleToolbarButton $w.f $i"
	  if { $::toolbar_temp($i) } { $w.f.$i state pressed }
	  grid $w.f.$i -row $row -column $col -sticky news -padx 4 -pady "0 8"
	  incr col
      }
  }
  setToolbarTooltips $w.f
  addHorizontalRule $w
  pack [ttk::frame $w.b] -side bottom -fill x
  ttk::button $w.on -text "+ [::utils::string::Capital $::tr(all)]" -command "toggleAllToolbarButtons $w.f 1"
  ttk::button $w.off -text "- [::utils::string::Capital $::tr(all)]" -command "toggleAllToolbarButtons $w.f 0"

  pack $w.on $w.off -side left -padx 2 -pady "5 0"
}

proc redrawToolbar {} {
  foreach i [winfo children .main.tb] { pack forget $i }
  set seenAny 0
  set seen 0
  foreach i {newdb open closedb finder save bkm} {
    if {$::toolbar_state($i)} {
      set seen 1; set seenAny 1
      pack .main.tb.$i -side left -pady 1 -padx 0 -ipadx 0 -pady 0 -ipady 0
    }
  }
  if {$seen} { pack .main.tb.space1 -side left }
  set seen 0
  foreach i {gprev gnext} {
    if {$::toolbar_state($i)} {
      set seen 1; set seenAny 1
      pack .main.tb.$i -side left -pady 1 -padx 0 -ipadx 0 -pady 0 -ipady 0
    }
  }
  if {$seen} { pack .main.tb.space2 -side left }
  set seen 0
  foreach i {newgame copy paste} {
    if {$::toolbar_state($i)} {
      set seen 1; set seenAny 1
      pack .main.tb.$i -side left -pady 1 -padx 0 -ipadx 0 -pady 0 -ipady 0
    }
  }
  if {$seen} { pack .main.tb.space3 -side left }
  set seen 0
  foreach i {boardsearch headersearch materialsearch} {
    if {$::toolbar_state($i)} {
      set seen 1; set seenAny 1
      pack .main.tb.$i -side left -pady 1 -padx 0 -ipadx 0 -pady 0 -ipady 0
    }
  }
  if {$seen} { pack .main.tb.space4 -side left }
  set seen 0
  foreach i {switcher glist pgn tmt maint eco tree crosstab engine} {
    if {$::toolbar_state($i)} {
      set seen 1; set seenAny 1
      pack .main.tb.$i -side left -pady 1 -padx 0 -ipadx 0 -pady 0 -ipady 0
    }
  }
  if {$seenAny} {
    grid .main.tb -row 0 -column 0 -columnspan 3 -sticky we
  } else {
    grid forget .main.tb
  }
}

##############################
