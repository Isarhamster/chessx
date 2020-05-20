###
### import.tcl: part of Scid.
### Copyright (C) 2000  Shane Hudson.
###

### Import game window

proc importPgnGame {} {
  if {[winfo exists .importWin]} { return }
  set w .importWin
  win::createDialog $w
  wm title $w "Scid: $::tr(ImportPGN)"
  wm minsize $w 50 5
  ttk::frame $w.b
  pack $w.b -side bottom -fill x
  set pane [::utils::pane::Create $w.pane edit err 580 300 0.8]
  pack $pane -side top -expand true -fill both
  set edit $w.pane.edit
  autoscrollText both $edit.f $edit.text Treeview
  $edit.text configure -height 12 -width 80 -wrap none -setgrid 1 -state normal
  # Override tab-binding for this widget:
  bind $edit.text <Key-Tab> "[bind all <Key-Tab>]; break"
  grid $edit.f -row 0 -column 0 -sticky nesw
  grid rowconfig $edit 0 -weight 1 -minsize 0
  grid columnconfig $edit 0 -weight 1 -minsize 0
  
  # Right-mouse button cut/copy/paste menu:
  menu $edit.text.rmenu -tearoff 0
  $edit.text.rmenu add command -label "Cut" -command "tk_textCut $edit.text"
  $edit.text.rmenu add command -label "Copy" -command "tk_textCopy $edit.text"
  $edit.text.rmenu add command -label "Paste" -command "tk_textPaste $edit.text"
  $edit.text.rmenu add command -label "Select all" -command "$edit.text tag add sel 1.0 end"
  bind $edit.text <ButtonPress-$::MB3> "tk_popup $edit.text.rmenu %X %Y"
  
  autoscrollText y $pane.err.f $pane.err.text Treeview
  $pane.err.text configure -height 4 -width 75 -wrap word -setgrid 1 -state normal
  $pane.err.text insert end $::tr(ImportHelp1)
  $pane.err.text insert end "\n"
  $pane.err.text insert end $::tr(ImportHelp2)
  $pane.err.text configure -state disabled
  pack $pane.err.f -side left -expand true -fill both
  
  ttk::button $w.b.paste -text "$::tr(PasteCurrentGame) (Alt-P)" -command {
    .importWin.pane.edit.text delete 1.0 end
    .importWin.pane.edit.text insert end [sc_game pgn -width 70]
    .importWin.pane.err.text configure -state normal
    .importWin.pane.err.text delete 1.0 end
    .importWin.pane.err.text configure -state disabled
  }
  ttk::button $w.b.clear -text "$::tr(Clear) (Alt-C)" -command {
    .importWin.pane.edit.text delete 1.0 end
    .importWin.pane.err.text configure -state normal
    .importWin.pane.err.text delete 1.0 end
    .importWin.pane.err.text configure -state disabled
  }
  ttk::button $w.b.ok -text "$::tr(Import) (Alt-I)" -command {
    set err [catch {sc_game import \
          [.importWin.pane.edit.text get 1.0 end]} result]
    .importWin.pane.err.text configure -state normal
    .importWin.pane.err.text delete 1.0 end
    .importWin.pane.err.text insert end $result
    .importWin.pane.err.text configure -state disabled
    if {! $err} {
      ::notify::GameChanged
    }
  }
  ttk::button $w.b.cancel -textvar ::tr(Close) -command {
    destroy .importWin; focus .
  }
  pack $w.b.paste $w.b.clear -side left -padx 5 -pady "15 5"
  packdlgbuttons $w.b.cancel $w.b.ok
  # Paste the current selected text automatically:
  # if {[catch {$w.pane.edit.text insert end [selection get]}]} {
  # ?
  # }
  # Select all of the pasted text:
  $w.pane.edit.text tag add sel 1.0 end
  
  bind $w <F1> { helpWindow Import }
  bind $w <Alt-i> { .importWin.b.ok invoke }
  bind $w <Alt-p> { .importWin.b.paste invoke }
  bind $w <Alt-c> { .importWin.b.clear invoke }
  bind $w <Escape> { .importWin.b.cancel invoke }
  # bind $w.pane.edit.text <Any-KeyRelease> { .importWin.b.ok invoke }
  focus $w.pane.edit.text
}


proc importClipboardGame {} {
  importPgnGame
  catch {event generate .importWin.pane.edit.text <<Paste>>}
  # Paste the current selected text automatically if no data was pasted from clipboard:
  if { [ .importWin.pane.edit.text get 1.0 end ] == "\n" } {
    catch { .importWin.pane.edit.text insert end [selection get] }
  }
}

proc importPgnLine {line} {
  importPgnGame
  set w .importWin.pane.edit.text
  $w delete 1.0 end
  $w insert end $line
  $w tag add sel 1.0 end
  focus $w
}

################################################################################
#
################################################################################
proc importMoveList {line} {
  sc_move start
  sc_move addSan $line
  updateBoard -pgn
}
################################################################################
#
################################################################################
proc importMoveListTrans {line} {
  set doImport 0
    if {[llength [sc_game firstMoves 1]] == 0} {
      set doImport 1
    } elseif {[tk_messageBox -message [::tr "OverwriteExistingMoves"] -type yesno -icon question ] == yes} {
      set doImport 1
    }
  if {$doImport} {
    set line [untrans $line]
    sc_move start
    sc_move addSan $line
    updateBoard -pgn
  }
}


### Import file of Pgn games:
proc importPgnFile {{base} {fnames ""}} {
  if {$fnames == ""} {
      set ftypes { { "Portable Game Notation files" {".pgn" ".PGN"} } }
    lappend ftypes { "Text files" {".txt" ".TXT"} }
    lappend ftypes { "All files" {"*"} }

    set fnames [tk_getOpenFile -multiple 1 -initialdir $::initialDir(pgn) -filetypes $ftypes -title "$::tr(ImportingIn) [file tail [sc_base filename $base] ]" ]
    if {$fnames == ""} { return }
    set ::initialDir(pgn) [file dirname [lindex $fnames 0]]
    set autoclose 0
  } else {
    set autoclose 1
  }
  
  set w .ipgnWin
  if {[winfo exists $w]} { destroy $w }
  toplevel $w
  wm title $w "Scid: $::tr(ImportingFiles) [file tail [sc_base filename $base] ]"
  canvas $w.progress -width 400 -height 20 -bg white -relief solid -border 1
  $w.progress create rectangle 0 0 0 0 -fill blue -outline blue -tags bar
  $w.progress create text 395 10 -anchor e -font font_Regular -tags time \
    -fill black -text "0:00 / 0:00"

  ttk::frame $w.buttons
  ttk::button $w.buttons.stop -textvar ::tr(Stop) -command { progressBarCancel}
  ttk::button $w.buttons.close -textvar ::tr(Close) -command "focus .; destroy $w"
  pack $w.buttons.close $w.buttons.stop -side right -ipadx 5 -padx 5 -pady 2
    
  pack [ttk::frame $w.tf] -side top -expand yes -fill both
  text $w.text -height 8 -width 60 -background gray90 \
      -wrap none -setgrid 1 -yscrollcommand "$w.ybar set"
  ttk::scrollbar $w.ybar -command "$w.text yview"
  pack $w.ybar -in $w.tf -side right -fill y
  pack $w.text -in $w.tf -side left -fill both -expand yes
  pack $w.buttons $w.progress -side right

  catch {grab $w.buttons.stop}
  bind $w <Escape> "$w.buttons.stop invoke"
  $w.buttons.close configure -state disabled

  busyCursor .
  foreach fname $fnames {
    $w.text insert end "$::tr(ImportingFrom) [file tail $fname]...\n"
    $w.text configure -state disabled
    progressBarSet $w.progress 401 21
    set err [catch {sc_base import $base $fname} result]
    $w.text configure -state normal
    if {$err == 1} {
      set autoclose 0
      $w.text insert end "[ERROR::getErrorMsg]\n$result\n\n"
    } else {
      set nImported [lindex $result 0]
      set warnings [lindex $result 1]
      set str "Imported $nImported "
      if {$nImported == 1} { append str "game" } else { append str "games" }
      if {$warnings == ""} {
        append str " with no PGN errors or warnings."
      } else {
        set autoclose 0
        append str ".\nPGN errors/warnings:\n$warnings"
      }
      $w.text insert end "$str\n\n"
      if {$err == 3} {
        $w.text insert end ".\nINTERRUPTED\n"
        set autoclose 0
        break
      }
    }
  }
  unbusyCursor .

  $w.text configure -state disabled
  $w.buttons.close configure -state normal
  $w.buttons.stop configure -state disabled
  catch {grab release $w.buttons.stop}
  bind $w <Escape> "$w.buttons.close invoke; break"

  after idle "::notify::DatabaseModified $base"
  if { $autoclose } { destroy $w }
}

###
### End of file: import.tcl
###

