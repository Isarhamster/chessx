############################################################
### Board setup window:

# Global variables for entry of the start position:
set epFile {}          ;# legal values are empty, or "a"-"h".
set moveNum 1          ;# legal values are 1-999.
set setupStatus {}     ;# stores the FEN string.
set castling KQkq      ;# will be empty or some combination of KQkq letters.
set toMove White       ;# side to move, "White" or "Black".
set pastePiece K       ;# Piece being pasted, "K", "k", "Q", "q", etc.

# Traces to keep entry values sensible:
trace variable moveNum  w {::utils::validate::Integer 999 0}
trace variable epFile   w {::utils::validate::Regexp {^(-|[a-h])?$}}
trace variable castling w {::utils::validate::Regexp {^(-|[KQkq]*)$}}

set setupBd {}
set setupFen {}

# setupBoard:
#   The main procedure for creating the dialog for setting the start board.
#   Calls switchPastePiece and makeSetupFen.
#   On "Setup" button press, calls sc_pos startBoard to try to set the
#   starting board.
#
proc setupBoard {} {
  global boardSizes boardSize lite dark setupBd pastePiece toMove epFile moveNum
  global origFen
  global setupStatus castling setupFen highcolor
  if {[winfo exists .setup]} { return }
  set setupBd [lindex [sc_pos board] 0]
  set origFen [sc_pos fen]

  set w ".setup"
  win::createDialog $w
  wm title $w "Scid: $::tr(SetupBoard)"
  wm minsize $w 640 460

  #Frames
  ttk::frame $w.l
  ttk::frame $w.r
  ttk::frame $w.statusbar
  ttk::frame $w.buttons
  grid $w.l -row 1 -column 0 -sticky news
  grid rowconfigure $w 1 -weight 1
  grid columnconfigure $w 0 -weight 1
  grid $w.r -row 1 -column 4 -sticky news
  grid $w.statusbar -row 2 -column 0 -columnspan 5 -sticky news
  grid $w.buttons -row 3 -column 0 -columnspan 5 -sticky news

  #Board
  ::board::new $w.l.bd
  ::board::coords $w.l.bd
  for {set i 0} { $i < 64 } { incr i } {
    ::board::bind $w.l.bd $i <B1-Motion>       "dragBoardPiece  $w.l.bd %X %Y $i"
    ::board::bind $w.l.bd $i <ButtonRelease-1> "setupBoardPiece $w.l.bd %X %Y"
  }
  grid $w.l.bd -sticky news
  grid rowconfigure $w.l.bd 0 -weight 1
  grid columnconfigure $w.l.bd 0 -weight 1
  
  ### Piece Buttons
  foreach psize $::boardSizes {
      if {$psize >= 40} { break }
  }
  grid [ttk::frame $w.spacePiecesLeft -width 10] -row 1 -column 1 -sticky news
  grid [ttk::frame $w.pieces] -row 1 -column 2 -sticky news
  grid [ttk::frame $w.pieces.spaceTop -height 2] -row 0 -columnspan 2
  grid [ttk::frame $w.pieces.w] -row 1 -column 0 -sticky news
  grid [ttk::frame $w.pieces.b] -row 1 -column 1 -sticky news
  foreach i {p n b r q k} {
    foreach color {w b} value "[string toupper $i] $i" {
      ttk::radiobutton $w.pieces.$color.$i -image $color$i$psize -variable pastePiece -value $value -style Toolbutton
      grid $w.pieces.$color.$i -column 0 -pady 2 -padx 2
    }
  }
  set ::setupBoardFlipped [::board::isFlipped .main.board]
  ::board::flip .setup.l.bd $::setupBoardFlipped
  ttk::checkbutton $w.pieces.rotate -text $::tr(Rotate) -image tb_BD_Flip -compound left \
      -variable ::setupBoardFlipped -command {
    set ::setupBd  [string reverse $::setupBd]
    set ::setupFen [makeSetupFen]
    ::board::update .setup.l.bd $::setupBd
    ::board::flip .setup.l.bd
    set ::setupBoardFlipped [::board::isFlipped .setup.l.bd]
  }
  grid $w.pieces.rotate -row 2 -columnspan 2 -sticky news -padx 2 -pady 2


  ### Side to move frame.
  set toMove [lindex {White Black} [string equal [lindex $origFen 1] b]]
  ttk::frame $w.r.tomove
  ttk::label $w.r.tomove.label -textvar ::tr(SideToMove:)
  ttk::frame $w.r.tomove.buttons
  ttk::radiobutton $w.r.tomove.buttons.w -text $::tr(White) -variable toMove -value White \
      -command {set setupFen [makeSetupFen]}
  ttk::radiobutton $w.r.tomove.buttons.b -text $::tr(Black) -variable toMove -value Black \
      -command {set setupFen [makeSetupFen]}
  
  pack $w.r.tomove -pady 7
  pack $w.r.tomove.label -side top -pady 2
  pack $w.r.tomove.buttons -side top
  pack $w.r.tomove.buttons.w $w.r.tomove.buttons.b -side left
  
  ### Entry boxes: Move number, Castling and En Passant file.
  pack [ttk::frame $w.r.mid] -padx 5 -pady 5
  
  set moveNum [lindex $origFen 5]
  ttk::frame $w.r.mid.movenum
  ttk::label $w.r.mid.movenum.label -textvar ::tr(MoveNumber:)
  ttk::entry $w.r.mid.movenum.e -width 3 -background white -textvariable moveNum
  
  pack $w.r.mid.movenum -pady 10 -expand yes -fill x
  pack $w.r.mid.movenum.label $w.r.mid.movenum.e -side left -anchor w -expand yes -fill x
  
  set castling [lindex $origFen 2]
  ttk::frame $w.r.mid.castle
  ttk::label $w.r.mid.castle.label -textvar ::tr(Castling:)
  ttk::combobox $w.r.mid.castle.e -width 5 -textvariable castling -values {KQkq K Q k q - KQ kq Kk Kq Kkq Qk Qq Qkq KQk KQq}

  pack $w.r.mid.castle -pady 10 -expand yes -fill x
  pack $w.r.mid.castle.label $w.r.mid.castle.e -side left -anchor w -expand yes -fill x
  
  set epFile [string index [lindex $origFen 3] 0]
  ttk::frame $w.r.mid.ep
  ttk::label $w.r.mid.ep.label -textvar ::tr(EnPassantFile:)
  ttk::combobox $w.r.mid.ep.e -width 2 -textvariable epFile -values {- a b c d e f g h}
  
  pack $w.r.mid.ep -pady 10 -expand yes -fill x
  pack $w.r.mid.ep.label $w.r.mid.ep.e -side left -anchor w -expand yes -fill x
  
  # Set bindings so the Fen string is updated at any change. The "after idle"
  # is needed to ensure any keypress which causes a text edit is processed
  # before we regenerate the FEN text.
  
  foreach i "$w.r.mid.ep.e $w.r.mid.castle.e $w.r.mid.movenum.e" {
    bind $i <Any-KeyPress> {after idle {set setupFen [makeSetupFen]}}
    bind $i <FocusOut> {
      after idle {set setupFen [makeSetupFen]}}
  }
  
  ### Buttons: Clear Board and Initial Board.
  ttk::frame $w.r.b
  ttk::button $w.r.b.clear -textvar ::tr(EmptyBoard) -command {
    set setupBd "................................................................"
    ::board::update .setup.l.bd $setupBd
    set castling {}
    set setupFen [makeSetupFen]
  }
  ttk::button $w.r.b.initial -textvar ::tr(InitialBoard) -command {
    setSetupBoardToFen %W "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
  }
  ttk::button $w.r.b.switchcolor -text $::tr(SwitchColors) -command {
    regsub -all {(?:([A-Z])|([a-z]))} $::setupBd {[string tolower "\1"][string toupper "\2"]} invertCase
    set ::setupBd [subst $invertCase]
    set ::toMove [expr {$::toMove == "White" ? "Black" : "White"}]
    regsub -all {(?:([A-Z])|([a-z]))} $::castling {[string tolower "\1"][string toupper "\2"]} invertCase
    set ::castling [subst $invertCase]
    set epFile {-}
    ::board::update .setup.l.bd $setupBd
    set ::setupFen [makeSetupFen]
  }
  ttk::button $w.r.b.flip -text $::tr(FlipBoard) -command {
    ::board::flip .setup.l.bd
    set ::setupBoardFlipped [::board::isFlipped .setup.l.bd]
  }

  pack $w.r.b -side top -pady 15
  pack $w.r.b.clear -side top -padx 5 -pady 5 -fill x
  pack $w.r.b.initial -side top -padx 5 -pady 5 -fill x
  pack $w.r.b.switchcolor -side top -padx 5 -pady 5 -fill x
  pack $w.r.b.flip -side top -padx 5 -pady 5 -fill x
  
  ### Buttons: Setup and Cancel.
  ttk::button $w.buttons.ok -text "OK" -width 7 -command exitSetupBoard
  ttk::button $w.buttons.cancel -textvar ::tr(Cancel) -command {destroy .setup}
  pack [ttk::frame $w.buttons.spaceTop -height 4] -side top
  pack $w.buttons.cancel -side right -padx 5
  pack $w.buttons.ok -side right -padx 5
  
  ttk::button .setup.paste -textvar ::tr(PasteFen) -command {
    if {[catch {set setupFen [selection get -selection CLIPBOARD]} ]} {
      # PRIMARY is the X selection, unsure about CLIPBOARD
      if {[catch {set setupFen [selection get -selection PRIMARY]}]} { return }
    }
    setSetupBoardToFen %W $setupFen
  }
  ttk::button .setup.clear -textvar ::tr(ClearFen) -command {set setupFen ""}
  
  ttk::combobox .setup.status -textvariable setupFen -height 10
  bind .setup.status <<ComboboxSelected>> {setSetupBoardToFen %W $setupFen}
  ::utils::history::SetCombobox setupFen .setup.status
  ::utils::history::SetLimit setupFen 20
  
  pack .setup.paste .setup.clear -in .setup.statusbar -side left
  pack .setup.status -in .setup.statusbar -side right -expand yes -fill x -anchor w

  bind $w.l <Configure> "::board::resizeAuto $w.l.bd \[grid bbox $w 0 1\]"
  bind $w <Destroy> "if {\[string equal $w %W\]} { ::win::saveWinGeometry $w }"
  bind $w <Escape> {destroy .setup}
  ::win::restoreWinGeometry $w

  set setupFen [makeSetupFen]
}

proc setSetupBoardToFen {w setupFen} {
  global setupBd toMove castling epFile moveNum

  sc_game push
  if {[catch {sc_game startBoard $setupFen} err]} {
    fenErrorDialog $err
  } else {
    set ::setupFen [sc_pos fen]
    set setupBd [lindex [sc_pos board] 0]
    set toMove [lindex {White Black} [string equal [lindex $setupFen 1] b]]
    set castling [lindex $setupFen 2]
    set epFile [string index [lindex $setupFen 3] 0]
    set moveNum [lindex $setupFen 5]
    ::board::update .setup.l.bd $setupBd
  }
  sc_game pop
}


# makeSetupFen:
#    Reconstructs the FEN string from the current settings in the
#    setupBoard dialog. Check to see if the position is
#    acceptable (a position can be unacceptable by not having exactly
#    one King per side, or by having more than 16 pieces per side).
#
proc makeSetupFen {} {
  global setupFen setupBd moveNum toMove castling epFile
  set fenStr ""
  set errorStr [validateSetup]
  if {$errorStr != ""} {
    set fenStr "Invalid board: "
    append fenStr $errorStr
    return $fenStr
  }
  for {set bRow 56} {$bRow >= 0} {incr bRow -8} {
    if {$bRow < 56} { append fenStr "/" }
    set emptyRun 0
    for {set bCol 0} {$bCol < 8} {incr bCol} {
      set sq [expr {$bRow + $bCol} ]
      set piece [string index $setupBd $sq]
      if {$piece == "."} {
        incr emptyRun
      } else {
        if {$emptyRun > 0} {
          append fenStr $emptyRun
          set emptyRun 0
        }
        append fenStr $piece
      }
    }
    if {$emptyRun > 0} { append fenStr $emptyRun }
  }
  append fenStr " " [string tolower [string index $toMove 0]] " "
  if {$castling == ""} {
    append fenStr "- "
  } else {
    append fenStr $castling " "
  }
  if {$epFile == ""  ||  $epFile == "-"} {
    append fenStr "-"
  } else {
    append fenStr $epFile
    if {$toMove == "White"} {
      append fenStr "6"
    } else {
      append fenStr "3"
    }
  }
  # We assume a halfmove clock of zero:
  append fenStr " 0 " $moveNum
  set setupFen $fenStr
  return $fenStr
}

# validateSetup:
#   Called by makeSetupFen to check that the board is sensible: that is,
#   that there is one king per side and there are at most 16 pieces per
#   side and there are no pawn in the 1st or 8th row
#
proc validateSetup {} {
  global setupBd
  set wkCount 0; set bkCount 0; set wCount 0; set bCount 0
  set wpCount 0; set bpCount 0
  for {set i 0} {$i < 64} {incr i} {
    set p [string index $setupBd $i]
    if {$p == "."} {
    } elseif {$p == "P"} { incr wCount; incr wpCount
    } elseif {$p == "p"} { incr bCount; incr bpCount
    } elseif {$p == "N" || $p == "B" || $p == "R" || $p == "Q"} {
      incr wCount
    } elseif {$p == "n" || $p == "b" || $p == "r" || $p == "q"} {
      incr bCount
    } elseif {$p == "K"} { incr wCount; incr wkCount
    } elseif {$p == "k"} { incr bCount; incr bkCount
    } else { return "Invalid piece: $p" }
    if {$p == "P"  ||  $p == "p"} {
      if {$i < 8}   { return "There must be no pawn in the 1st row" }
      if {$i >= 56} { return "There must be no pawn in the 8th row" }
    }
  }
  if {$wkCount != 1} { return "There must be one white king"
  } elseif {$bkCount != 1} { return "There must be one black king"
  } elseif {$wCount > 16} { return "Too many white pieces"
  } elseif {$bCount > 16} { return "Too many black pieces"
  } elseif {$wpCount > 8} { return "Too many white pawns"
  } elseif {$bpCount > 8} { return "Too many black pawns" }
  return ""
}

proc dragBoardPiece {w x y startSq} {
  set square [::board::getSquare $w $x $y]
  if {$square != $startSq && [::board::getDragSquare $w] == -1} {
    set tmp [string index $::setupBd $startSq]
    if {$tmp == "."} { return }
    set ::pastePiece $tmp
    ::board::setDragSquare .setup.l.bd $startSq
  }

  ::board::dragPiece .setup.l.bd $x $y
}

proc setupBoardPiece {w x y} {
  global setupBd pastePiece setupFen

  set square [::board::getSquare $w $x $y]
  set newPiece $pastePiece
  set oldPiece [string index $setupBd $square]
  set delSq [::board::setDragSquare .setup.l.bd -1]
  if {$delSq != -1} {
    #Dragged
    if {$delSq == $square} { return }
    set setupBd [string replace $setupBd $delSq $delSq "."]
  } else {
    #Left click
    if {$oldPiece == $newPiece} {
        set newPiece "."
    }
  }

  set setupBd [string replace $setupBd $square $square $newPiece]
  ::board::update .setup.l.bd $setupBd
  set setupFen [makeSetupFen]
}

proc exitSetupBoard {} {
  global setupFen

  undoFeature save
  if {[catch {sc_game startBoard $setupFen} err]} {
    undoFeature undo
    fenErrorDialog $err
  } else {
    ::utils::history::AddEntry setupFen $setupFen
    destroy .setup
    ::board::flipAuto .main.board $::setupBoardFlipped
    ::notify::PosChanged -pgn
  }
}

### End of Board setup window
############################################################


proc fenErrorDialog {{msg {}}} {
  if {[winfo exists .setup]} {
    tk_messageBox -icon info -type ok -title "Scid: Invalid FEN" -message $msg -parent .setup
  } else {
    tk_messageBox -icon info -type ok -title "Scid: Invalid FEN" -message $msg
  }
}

# copyFEN
#
#   Copies the FEN of the current position to the text clipboard.
#
proc copyFEN {} {
  set fen [sc_pos fen]
  # Create a text widget to hold the fen so it can be the owner
  # of the current text selection:
  set w .tempFEN
  if {! [winfo exists $w]} { text $w }
  $w delete 1.0 end
  $w insert end $fen sel
  clipboard clear
  clipboard append $fen
  selection own $w
  selection get
}

# pasteFEN
#
#   Bypasses the board setup window and tries to paste the current
#   text selection as the setup position, producing a message box
#   if the selection does not appear to be a valid FEN string.
#
proc pasteFEN {} {
  set fenStr ""
  if {[catch {set fenStr [selection get -selection CLIPBOARD]} ]} {
    catch {set fenStr [selection get -selection PRIMARY]}
  }
  set fenStr [string trim $fenStr]

  set fenExplanation {FEN is the standard text representation of a chess position. As an example, the FEN representation of the standard starting position is:
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"}

  if {$fenStr == ""} {
    set msg "The current text selection is empty. To paste the start board, select some text that contains a position in FEN notation.\n\n$fenExplanation"
    fenErrorDialog $msg
    return
  }
  if {[catch {sc_game startBoard $fenStr}]} {
    if {[string length $fenStr] > 80} {
      set fenStr [string range $fenStr 0 80]
      append fenStr "..."
    }
    set msg "\"$fenStr\" is not a valid chess position in FEN notation.\n\n $fenExplanation"

    fenErrorDialog $msg
    return
  }
  updateBoard -pgn
}

