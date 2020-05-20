# end.tcl: part of Scid.
# Copyright (C) 2000-2003 Shane Hudson.

if { ! [file isdirectory $::scidBooksDir]} {
  tk_messageBox -title "Scid" -type ok -icon warning -message "Books directory not found"
}


# findNovelty:
#   Searches the for first position in the current game not
#   found in the selected database.

set noveltyOlder 0

proc findNovelty {} {
  global noveltyBase noveltyOlder
  set noveltyBase [sc_base current]
  set w .noveltyWin
  if {[winfo exists $w]} {
    updateNoveltyWin
    return
  }
  win::createDialog $w
  wm title $w "Scid: $::tr(FindNovelty)"
  
  pack [ttk::frame $w.help] -side top -fill x
  ttk::label $w.help.text -wraplength 3i -justify left -text $::tr(NoveltyHelp)
  pack $w.help.text -side left -fill x -expand yes
  ttk::labelframe $w.bases -text $::tr(Database)
  pack $w.bases -side top -fill x -pady 5

  pack [ttk::frame $w.bases.del]
  
  ttk::labelframe $w.which -text $::tr(TwinsWhich)
  pack $w.which -side top -fill x -pady 5
  ttk::radiobutton $w.which.all -text $::tr(SelectAllGames) \
      -variable noveltyOlder -value 0
  ttk::radiobutton $w.which.older -text $::tr(SelectOlderGames) \
      -variable noveltyOlder -value 1
  pack $w.which.all $w.which.older -side top -anchor w -fill x
  
  pack [ttk::frame $w.b] -side top -fill x
  dialogbutton $w.b.go -text $::tr(FindNovelty) -command {
    destroy .noveltyWin
    progressWindow "Scid" "$::tr(FindNovelty)" $::tr(Cancel)
    if {$noveltyOlder} {
      set err [catch {sc_game novelty -older $noveltyBase} result]
    } else {
      set err [catch {sc_game novelty $noveltyBase} result]
    }
    closeProgressWindow
    if {$err} {
      ERROR::MessageBox
    } else {
      if {$result < 0} {
        tk_messageBox -message "[tr NoveltyNone]"
	  }
    }
    updateBoard -pgn
  }
  dialogbutton $w.b.close -textvar ::tr(Close) -command {
    catch {destroy .noveltyWin}
  }
  packdlgbuttons $w.b.close $w.b.go
  wm resizable $w 0 0
  focus $w.b.go
  bind $w <KeyPress-1> "$w.b1 invoke"
  bind $w <KeyPress-2> "$w.b2 invoke"
  bind $w <KeyPress-3> "$w.b3 invoke"
  bind $w <KeyPress-4> "$w.b4 invoke"
  updateNoveltyWin
}

proc updateNoveltyWin {} {
  set w .noveltyWin
  if {! [winfo exists $w]} { return }
  $w.which.older configure -text "$::tr(SelectOlderGames) (< [sc_game info date])"
  catch { destroy $w.bases.del }
  pack [frame $w.bases.del] -anchor w
  foreach i [sc_base list] {
    set name [file tail [sc_base filename $i]]
    set ng [::utils::thousands [sc_base numGames $i]]
    set txt "Base $i: $name ($ng $::tr(games))"
    ttk::radiobutton $w.bases.del.b$i -text "$txt" -variable noveltyBase -value $i -underline 5
    if {$ng == 0} { $w.bases.del.b$i configure -state disabled }
    pack $w.bases.del.b$i -side top -anchor w -fill x
  }
}

set merge(ply) 40

# mergeGame:
#   Produces a dialog for the user to merge a selected game into
#   the current game.
#
proc mergeGame {base gnum} {
  global merge

  sc_game push copy
  set err [catch {sc_game merge $base $gnum} result]
  sc_game pop
  if {$err} {
    tk_messageBox -title "Scid" -type ok -icon info \
        -message "Unable to merge the selected game:\n$result"
    return
  }
  set merge(base) $base
  set merge(gnum) $gnum
  set w .mergeDialog
  win::createDialog $w
  wm title $w "Scid: $::tr(MergeGame)"
  bind $w <Escape> "$w.b.cancel invoke"
  bind $w <F1> {helpWindow GameList Browsing}
  ttk::label $w.title -text $::tr(Preview:) -font font_Bold -anchor center
  pack $w.title -side top -fill x
  pack [ttk::frame $w.b] -side bottom -fill x

  autoscrollText y $w.f $w.f.text Treeview
  $w.f.text configure -wrap word -width 60 -height 20 \
      -font font_Small -state normal
  event generate $w.f.text <ButtonRelease-1>
  pack $w.f -fill both -expand yes
  set small font_Small
  ttk::label $w.b.label -text "Up to move:" -font $small
  pack $w.b.label -side left
  foreach i {5 10 15 20 25 30 35 40 500} {
    if {$i != 500} {
      set label $i
      set label_width 3
    } else {
      set label [::utils::string::Capital $::tr(all)]
      set label_width [string length $label]
    }
    ttk::button $w.b.m$i -text $label -width $label_width -padding 0 \
      -command "updateMergeGame $w $i"
    pack $w.b.m$i -side left
  }
  dialogbutton $w.b.ok -text "OK" -command {
    undoFeature save
    sc_game merge $merge(base) $merge(gnum) $merge(ply)
    catch {grab release .mergeDialog}
    destroy .mergeDialog
    updateBoard -pgn
  }
  dialogbutton $w.b.cancel -text $::tr(Cancel) \
      -command "catch {grab release $w}; destroy $w"
  packbuttons right $w.b.cancel $w.b.ok
  grab $w
  updateMergeGame $w [expr $merge(ply) / 2]
}

proc updateMergeGame {{w} {n_moves}} {
  global merge
  if {! [winfo exists $w]} { return }

  # Update widgets
  set widget [expr $merge(ply) / 2]
  $w.b.m$widget state !pressed
  $w.b.m$n_moves state pressed
  set merge(ply) [expr {$n_moves * 2}];

  sc_game push copy
  sc_game merge $merge(base) $merge(gnum) $merge(ply)
  set pgn [sc_game pgn -indentV 1 -short 1 -width 60]
  sc_game pop
  $w.f.text configure -state normal
  $w.f.text tag configure red -foreground Red
  $w.f.text tag configure blue -foreground Blue
  $w.f.text delete 1.0 end
  foreach line [split $pgn "\n"] {
    if {[string index $line 0] == " "} {
      $w.f.text insert end $line blue
    } else {
      $w.f.text insert end $line
    }
    $w.f.text insert end "\n"
  }
  $w.f.text tag add red 1.0 4.0
  #$w.f.text insert end $pgn
  $w.f.text configure -state disabled
}

# setExportText:
#   Allows the user to modify the text printed at the start and end of a
#   file for a particular export format
#
proc setExportText {exportType} {
  global exportStartFile exportEndFile
  
  switch -- $exportType {
    "PGN" {
      set title "Set PGN file export text"
    }
    "HTML" {
      set title "Set HTML file export text"
    }
    "LaTeX" {
      set title "Set LaTeX file export text"
    }
    default {
      return
    }
  }
  
  set w .setExportText$exportType
  if {[winfo exists $w]} { return }
  win::createDialog $w
  wm title $w "Scid: $title"
  
  ttk::frame $w.buttons
  pack $w.buttons -side bottom -fill x -anchor e
  
  set pane [::utils::pane::Create $w.pane start end 500 400]
  ::utils::pane::SetRange $w.pane 0.3 0.7
  pack $pane -side top -expand true -fill both
  foreach f [list $pane.start $pane.end] type {start end} {
    ttk::label $f.title -font font_Bold -text "Text at $type of $exportType file:"
    autoscrollText both $f.f $f.text Treeview
    $f.text configure -state normal -wrap none
    grid $f.title -row 0 -column 0 -sticky w
    grid $f.f -row 1 -column 0 -sticky nesw
    grid rowconfig $f 1 -weight 1 -minsize 0
    grid columnconfig $f 0 -weight 1 -minsize 0
  }
  
  $pane.start.text insert end $exportStartFile($exportType)
  $pane.end.text insert end $exportEndFile($exportType)
  
  ttk::button $w.buttons.default -text "Reset to Default" -command "
  $pane.start.text delete 1.0 end
  $pane.start.text insert end \$default_exportStartFile($exportType)
  $pane.end.text delete 1.0 end
  $pane.end.text insert end \$default_exportEndFile($exportType)
  "
  dialogbutton $w.buttons.ok -text "OK" -command "
  set exportStartFile($exportType) \[$pane.start.text get 1.0 end-1c\]
  set exportEndFile($exportType) \[$pane.end.text get 1.0 end-1c\]
  focus .
  destroy $w
  "
  dialogbutton $w.buttons.cancel -text $::tr(Cancel) -command "focus .; destroy $w"
  pack $w.buttons.default -side left -padx 5 -pady 2
  packdlgbuttons $w.buttons.cancel $w.buttons.ok
  focus $pane.start.text
}

image create photo htmldiag0 -data {
  R0lGODdhbgBkAIAAAAAAAP///ywAAAAAbgBkAAAC/oyPqcvtD6OctNqLs968+w+G4kiW5omm
  6moAgQu/ckzPdo3fes7vfv5wsYQtBFF2OCqNzCPrs4xEi0mQEyWcApxbqhfpvYa2ZCNZeQ4y
  k122YgqGNs7oMtu+aH9f6XT1vydm5ddCyIenlkB3KAgnePFIJzm5yECkRVmpuPbokflpaLl2
  eKeHCNcRCQo6F3e52qY3Gve04QhbJkvrGYQb+jbrB8sHaJPi25mnGItLvDmRnLwnCpH1luUb
  WGwpLdUsIdaFHLtdHe09bM45Lkw9p4uRXfHKbseFntibnk9fT/4XDR6kb+BKsfrkrFuhc+b2
  OYhh0J+1Z+8U6ltVMGIm/kaTpnnL95DgQzPpMC6RJtCCPArMOmqsNDFjq4YYq5lZGKokxZEc
  Vtok5pIkwl2p0NXsZZDUsmH3fmpIuWxeUKEHy828yo0dT6p5sk3sZrGrP6dWx3kMCRKYykhE
  xcpD1fWpxK1tOX4LK9OtVneuliKjAnEEIqkMA9UrgjctTokCY+4a29fvL6OM2ZabW3co1peH
  rwVb3DmM5lpSRlV2DHryrGPFEidqShrS59azz2Zw/TTyF0e168aG1ps3bRG4bz8pvqmH8h/M
  lztvDj0wVuG7g/sW3Bv48Orbr7Purky3eOpgkMsenxcuX/XHs3NzzzG8NsJQ38EnG2Uq+rWa
  /s7bVrvfRtwBxhIlLHWnEHUCklegfumtpgx5AloHjYHAMTjdahbeB46D+EG4Hoj68YaXh3Sx
  B9IVrADo3TUTHmjVTHA5pFuCR70G4oeSaYZiRTemyGJcw72lIWWj2TckjKLRKN5FKypZHj/B
  iBQZWFS6g2GIVI3Wo15HApljFTMSlKWLP4oyo45L8himkxuq2eJ+nKV0UoUvgvlfhmb2x2FI
  JsL505hPDmjjoDx1qeWWEyK655d6tunMNGbt5N2kaF0Wlop7DejnX9qRJGWj++TnJpMUhVPf
  bJhBOqehWqompmmQgbejT8Bgkup4s8aZ2pU71VGYK4xVp8qqLAJriREXw1LqpaV0xXeoqJ4C
  Uuyuz2g62KvL5tnqmb8uhhS128Imra03iZvtsz2xikU8CFLInXkqvVsavZwyekKZTcArZ5Pt
  6vutvf3GBjC7VrRX1HMKR8fwwg4bo26+/Eq4729FCUyxHEPcO7FpFtvSscC8DJExx9vI+3G/
  JfNK1ncqh4zybyuvLEEBADs=
}

image create photo htmldiag1 -data {
  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
}

# exportOptions:
#   Prompts the user to select exporting options.
#
proc exportOptions {exportType} {
  global exportFlags
  
  set w .exportFlagsWin
  set exportFlags(ok) -1
  win::createDialog $w
  wm title $w "Scid: [tr OptionsExport]"
  # wm transient $w .
  wm protocol $w WM_DELETE_WINDOW { }
  bind $w <Escape> "$w.b.cancel invoke"
  bind $w <Return> "$w.b.ok invoke"
  bind $w <F1> {helpWindow Export}
  
  pack [ttk::frame $w.o] -side top -fill x
  ttk::checkbutton $w.o.append -variable exportFlags(append) -offvalue 0 -onvalue 1 -textvar ::tr(AddToExistingFile)
  ttk::checkbutton $w.o.comments -variable exportFlags(comments) -offvalue 0 -onvalue 1 -textvar ::tr(ExportComments)
  ttk::checkbutton $w.o.stripMarks -variable exportFlags(stripMarks) -offvalue 0 -onvalue 1 -textvar ::tr(ExportStripMarks)
  ttk::checkbutton $w.o.indentc -variable exportFlags(indentc) -offvalue 0 -onvalue 1 -textvar ::tr(IndentComments)
  ttk::checkbutton $w.o.vars -variable exportFlags(vars) -offvalue 0 -onvalue 1 -textvar ::tr(ExportVariations)
  ttk::checkbutton $w.o.indentv -variable exportFlags(indentv) -offvalue 0 -onvalue 1 -textvar ::tr(IndentVariations)
  ttk::checkbutton $w.o.column -variable exportFlags(column) -offvalue 0 -onvalue 1 -textvar ::tr(ExportColumnStyle)
  ttk::checkbutton $w.o.symbols -variable exportFlags(symbols) -offvalue 0 -onvalue 1 -text "$::tr(ExportSymbolStyle) ! +="
  grid $w.o.append -row 0 -column 0 -sticky w
  grid $w.o.comments -row 1 -column 0 -sticky w
  grid $w.o.stripMarks -row 2 -column 0 -sticky w
  grid $w.o.indentc -row 3 -column 0 -sticky w
  grid $w.o.vars -row 4 -column 0 -sticky w
  grid $w.o.indentv -row 5 -column 0 -sticky w
  grid $w.o.column -row 6 -column 0 -sticky w
  grid $w.o.symbols -row 7 -column 0 -sticky w
  
  # Extra option for PGN format: handling of null moves
  if {$exportType == "PGN"} {
    ttk::checkbutton $w.o.nullMoves -variable exportFlags(convertNullMoves) -offvalue 0 -onvalue 1 -textvar ::tr(ConvertNullMove)
    grid $w.o.nullMoves -row 8 -column 0 -sticky w
  }
  
  # Extra option for HTML format: diagram image set
  if {$exportType == "HTML"} {
    ttk::label $w.o.hdiag -text "Diagram"
    ttk::radiobutton $w.o.hb0 -text "bitmaps" \
        -variable exportFlags(htmldiag) -value 0
    ttk::radiobutton $w.o.hb1 -text "bitmaps2" \
        -variable exportFlags(htmldiag) -value 1
    ttk::label $w.o.hl0 -image htmldiag0
    ttk::label $w.o.hl1 -image htmldiag1
    grid $w.o.hdiag -row 8 -column 0 -sticky w
    grid $w.o.hb0 -row 9 -column 0 -sticky w
    grid $w.o.hb1 -row 9 -column 1 -sticky w
    grid $w.o.hl0 -row 10 -column 0 -sticky w
    grid $w.o.hl1 -row 10 -column 1 -sticky w
  }
  
  pack [ttk::frame $w.b] -side top -fill x
  dialogbutton $w.b.ok -text "OK" -command {
    set exportFlags(ok) 1
  }
  dialogbutton $w.b.cancel -text $::tr(Cancel) -command {
    set exportFlags(ok) 0
  }
  packbuttons right $w.b.cancel $w.b.ok
  
  grab $w
  tkwait variable exportFlags(ok)
  grab release $w
  destroy $w
  return $exportFlags(ok)
}

# exportGames:
#   exports current game or all filtered games to a new PGN, LaTeX or Html file.
#
proc exportGames {selection exportType} {
  global ::pgn::moveNumberSpaces exportStartFile exportEndFile exportFlags
  if {$selection == "filter" && [sc_filter count] == 0} {
      tk_messageBox -title "Scid: Filter empty" -type ok -icon info \
          -message "The filter contains no games."
      return
  }
  
  if {[exportOptions $exportType] == 0} { return }
  sc_info html $exportFlags(htmldiag)
  
  switch -- $exportType {
    "PGN" {
      set ftype {
        { "PGN files" {".pgn"} }
        { "All files" {"*"} }
      }
      set title "a PGN file"
      set idir $::initialDir(base)
      set default ".pgn"
    }
    "HTML" {
      set ftype {
        { "HTML files" {".html" ".htm"} }
        { "All files" {"*"} }
      }
      set title "an HTML file"
      set idir $::initialDir(html)
      set default ".html"
    }
    "LaTeX" {
      set ftype {
        { "LaTeX files" {".tex" ".ltx"} }
        { "All files" {"*"} }
      }
      set title "a LaTeX file"
      set idir $::initialDir(tex)
      set default ".tex"
    }
    default { return }
  }
  
  if {$exportFlags(append)} {
    set getfile tk_getOpenFile
    set title "Add games to $title"
  } else {
    set getfile tk_getSaveFile
    set title "Create $title"
  }
  set fName [$getfile -initialdir $idir -filetypes $ftype -defaultextension $default -title $title]
  if {$fName == ""} { return }

  progressWindow "Scid" "Exporting games..." $::tr(Cancel)
  sc_base export $selection $exportType $fName -append $exportFlags(append) \
      -starttext $exportStartFile($exportType) \
      -endtext $exportEndFile($exportType) \
      -comments $exportFlags(comments) -variations $exportFlags(vars) \
      -space $::pgn::moveNumberSpaces -symbols $exportFlags(symbols) \
      -indentC $exportFlags(indentc) -indentV $exportFlags(indentv) \
      -column $exportFlags(column) -noMarkCodes $exportFlags(stripMarks) \
      -convertNullMoves $exportFlags(convertNullMoves)
  closeProgressWindow
}

###########################################################################
### Global variables used in gameSave:
set date 0; set year 0; set month 0; set day 0; set white 0; set black 0
set resultVal 0; set event 0; set site 0; set round 0
set whiteElo 0; set blackElo 0; set eco 0; set extraTags ""
set whiteRType "Elo"; set blackRType "Elo"
set edate 0; set eyear 0; set emonth 0; set eday 0

# Traces on game-save dialog variables to ensure sane values:

trace variable resultVal w  ::utils::validate::Result
trace variable whiteElo w {::utils::validate::Integer [sc_info limit elo] 0}
trace variable blackElo w {::utils::validate::Integer [sc_info limit elo] 0}
trace variable year w {::utils::validate::Integer [sc_info limit year] 1}
trace variable month w {::utils::validate::Integer 12 1}
trace variable day w {::utils::validate::Integer 31 1}
trace variable eyear w {::utils::validate::Integer [sc_info limit year] 1}
trace variable emonth w {::utils::validate::Integer 12 1}
trace variable eday w {::utils::validate::Integer 31 1}

set gsaveNum 0
set i 0; set j 0
set temp 0

array set nameMatches {}
set nameMatchCount 0

# updateMatchList:
#    Called from gameSave to update the matching name list as the user
#    types a player/site/event/round name.
#
proc updateMatchList { tw nametype maxMatches name el op } {
  global nameMatches nameMatchCount
  global $name editNameType
  if {![winfo exists $tw]} return
  
  if {$nametype == ""} { set nametype $editNameType }
  if {$nametype == "rating"} { set nametype "player" }
  set val [set $name]
  $tw configure -state normal
  $tw delete 0.0 end
  set matches {}
  catch {set matches [sc_name match $nametype $val $maxMatches]}
  set count [llength $matches]
  set nameMatchCount [expr {$count / 2}]
  for {set i 0} { $i < $count } {incr i 2} {
    set nameMatchCount [expr {($i / 2) + 1}]
    set nameMatches($nameMatchCount) [lindex $matches [expr {$i + 1}]]
    set str "$nameMatchCount:\t[lindex $matches $i]\t$nameMatches($nameMatchCount)\n"
    $tw insert end $str
  }
  $tw configure -state disabled
}

proc clearMatchList { tw } {
  global nameMatches nameMatchCount
  set nameMatchCount 0
  $tw configure -state normal
  $tw delete 0.0 end
  $tw configure -state disabled
}

# Traces to update the match list as names are typed in:

trace variable event w { updateMatchList .save.g.list e 9 }
trace variable site  w { updateMatchList .save.g.list s 9 }
trace variable white w { updateMatchList .save.g.list p 9 }
trace variable black w { updateMatchList .save.g.list p 9 }
trace variable round w { updateMatchList .save.g.list r 9 }

set editName ""
set editNameNew ""
set editNameType "player"
set editNameSelect "all"
set editNameRating ""
set editNameRType "Elo"
set editDate ""
set editDateNew ""

trace variable editNameRating w {::utils::validate::Integer [sc_info limit elo] 0}
trace variable editName w { updateMatchList .nedit.g.list "" 9 }
trace variable editDate w ::utils::validate::Date
trace variable editDateNew w ::utils::validate::Date

proc editNameNewProc { tw nametype maxMatches name el op } {
  global editNameNew
  if {! [winfo exists .nedit]} { return }
  if {[string compare $editNameNew ""]} {
    .nedit.buttons.replace configure -state normal
  } else {
    .nedit.buttons.replace configure -state disabled
  }
  catch {updateMatchList $tw $nametype $maxMatches $name $el $op}
}

trace variable editNameNew w { editNameNewProc .nedit.g.list "" 9 }


set nameEditorWin 0

proc makeNameEditor {} {
  if {! [winfo exists .nedit]} { nameEditor }
}

proc setNameEditorType {type} {
  if {! [winfo exists .nedit]} { return }
  catch {.nedit.typeButtons.$type invoke}
}

proc nameEditor {} {
  global editName editNameType editNameNew nameEditorWin editNameSelect
  global editNameRating editDate editDateNew
  
  set w .nedit
  if {[winfo exists $w]} {
    destroy $w
    return
  }
  win::createDialog $w
  wm title $w "Scid: [tr FileMaintNameEditor]"
  set nameEditorWin 1
  setWinLocation $w
  bind $w <Configure> "recordWinSize $w"

  ttk::labelframe $w.typeButtons -text $::tr(NameEditType)
  pack $w.typeButtons -side top -fill x -anchor w
  foreach i { "Player" "Event" "Site" "Round"} col {0 1 2 3} {
    set j [string tolower $i]
    ttk::radiobutton $w.typeButtons.$j -textvar ::tr($i) -variable editNameType \
        -value $j -command {
          grid remove .nedit.g.ratingE
          grid remove .nedit.g.rtype
          grid remove .nedit.g.fromD
          grid remove .nedit.g.toD
          grid .nedit.g.toL -row 1 -column 1 -sticky e
          grid .nedit.g.fromE -row 0 -column 2 -sticky w
          grid .nedit.g.toE -row 1 -column 2 -sticky w
        }
    grid $w.typeButtons.$j -row 1 -column $col -sticky w -padx "0 5"
  }
  ttk::radiobutton $w.typeButtons.rating -textvar ::tr(Rating) -variable editNameType \
      -value rating -command {
        grid remove .nedit.g.toE
        grid remove .nedit.g.toL
        grid remove .nedit.g.fromD
        grid remove .nedit.g.toD
        grid .nedit.g.fromE -row 0 -column 2 -sticky w
        grid .nedit.g.rtype -row 1 -column 0 -columnspan 2 -sticky e
        grid .nedit.g.ratingE -row 1 -column 2 -sticky w
      }
  grid $w.typeButtons.rating -row 2 -column 0  -columnspan 2 -sticky w
  ttk::radiobutton $w.typeButtons.date -textvar ::tr(Date) -variable editNameType \
      -value date -command {
        grid remove .nedit.g.toE
        grid remove .nedit.g.fromE
        grid remove .nedit.g.ratingE
        grid remove .nedit.g.rtype
        grid .nedit.g.fromD -row 0 -column 2 -sticky w
        grid .nedit.g.toL -row 1 -column 1 -sticky e
        grid .nedit.g.toD -row 1 -column 2 -sticky w
      }
  grid $w.typeButtons.date -row 2 -column 1 -sticky w -padx "0 5"
  ttk::radiobutton $w.typeButtons.edate -textvar ::tr(EventDate) \
      -variable editNameType -value edate \
      -command {
        grid remove .nedit.g.toE
        grid remove .nedit.g.fromE
        grid remove .nedit.g.ratingE
        grid remove .nedit.g.rtype
        grid .nedit.g.fromD -row 0 -column 2 -sticky w
        grid .nedit.g.toL -row 1 -column 1 -sticky e
        grid .nedit.g.toD -row 1 -column 2 -sticky w
      }
  grid $w.typeButtons.edate -row 2 -column 2 -sticky w -columnspan 2

  ttk::labelframe $w.selectButtons -text $::tr(NameEditSelect)
  pack $w.selectButtons -side top -fill x -pady 10
  foreach i {all filter crosstable} row {0 1 2} text {
    SelectAllGames
    SelectFilterGames
    SelectTournamentGames
  } {
    ttk::radiobutton $w.selectButtons.$i -textvar ::tr($text) \
        -variable editNameSelect -value $i
    grid $w.selectButtons.$i -row $row -column 0 -sticky w
  }
  
  autoscrollText y $w.g $w.g.list Treeview
  $w.g.list configure -height 9 -width 40 -relief sunken \
      -tabs {2c right 2.5c left} -wrap none
  pack $w.g -side top -fill x
  ttk::label $w.g.fromL -textvar ::tr(NameEditReplace:) -font font_Bold -anchor e
  ttk::entry $w.g.fromE -width 40 -textvariable editName
  ttk::entry $w.g.fromD -width 15 -textvariable editDate
  grid $w.g.fromL -row 0 -column 1 -sticky e
  grid $w.g.fromE -row 0 -column 2 -sticky we
  
  ttk::label $w.g.toL -textvar ::tr(NameEditWith:) -font font_Bold -anchor e
  ttk::entry $w.g.toE -width 40 -textvariable editNameNew
  ttk::entry $w.g.toD -width 15 -textvariable editDateNew
  grid $w.g.toL -row 1 -column 1 -sticky e
  grid $w.g.toE -row 1 -column 2 -sticky we
  
  ttk::entry $w.g.ratingE -width 5 -textvariable editNameRating -justify right
  set mlist [split [sc_info ratings] " "]
  ttk::menubutton $w.g.rtype -textvariable editNameRType -menu $w.g.rtype.menu
  menu $w.g.rtype.menu
  foreach m $mlist {
      $w.g.rtype.menu add radiobutton -variable editNameRType -label $m
  }
  
  ttk::label $w.g.title -textvar ::tr(NameEditMatches) \
      -font font_Bold
  
  grid $w.g.title -row 2 -column 1 -columnspan 2 -sticky n
  grid $w.g.list -row 3 -column 1 -rowspan 9 -columnspan 2 -sticky e
  
  updateMatchList $w.g.list "" 9 editName "" w
  
  foreach {i j} {.nedit.g.fromE "editName"  .nedit.g.toE "editNameNew" } {
    for {set z 1} {$z <= 9} {incr z} {
      bind $i [format "<Control-Key-%d>" $z] \
          [format "eval {if {\$nameMatchCount >= %d} { \
              set %s \$nameMatches(%d)}}; break" $z $j $z ]
    }
  }
  
  ttk::frame $w.buttons
  ttk::button $w.buttons.replace -textvar ::tr(NameEditReplace) -command {
    if {$editNameType == "rating"} {
      set err [catch {sc_name edit $editNameType $editNameSelect $editName $editNameRating $editNameRType} result]
    } elseif {$editNameType == "date"  ||  $editNameType == "edate"} {
      set err [catch {sc_name edit $editNameType $editNameSelect $editDate $editDateNew} result]
    } else {
      set err [catch {sc_name edit $editNameType $editNameSelect $editName $editNameNew} result]
    }
    if {$err} {
      ERROR::MessageBox
    } else {
      .nedit.status configure -text "Modified $result games."
    }
    sc_game tags reload
    updateBoard -pgn
    ::windows::gamelist::Refresh
  }
  
  dialogbutton $w.buttons.cancel -textvar ::tr(Close) -command {focus .; destroy .nedit}
  pack $w.buttons -fill x
  packdlgbuttons $w.buttons.cancel $w.buttons.replace
  
  ttk::label $w.status -text "" -width 1 -font font_Small -relief sunken -anchor w
  pack $w.status -side bottom -fill x
  
  wm resizable $w 0 0
  bind $w <Escape> { focus .; destroy .nedit }
  bind $w <Return> {.nedit.buttons.replace invoke}
  bind $w <Destroy> {set nameEditorWin 0}
  bind $w <F1> {helpWindow Maintenance Editing}
  focus $w
  $w.typeButtons.$editNameType invoke
}


# addGameSaveEntry:
#   used in gameSave for setting up the simpler labels and entry boxes.
#
proc addGameSaveEntry { name row textname } {
  ttk::label .save.g.label$name -textvar $textname
  ttk::entry .save.g.entry$name -width 30 -textvariable $name
  grid .save.g.label$name -row $row -column 0 -sticky w
  grid .save.g.entry$name -row $row -column 1 -columnspan 7 -sticky w
}

# gameSave:
#   The game save dialog. Used for adding and replacing games. If the
#   value gnum is zero, it is to add a new game; otherwise it is to
#   replace game number gnum.
#
proc gameSave { gnum } {
  global date year month day white black resultVal event site round
  global whiteElo blackElo whiteRType blackRType eco extraTags gsaveNum
  global edate eyear emonth eday
  
  # Make a new toplevel that contains the game save dialog:
  set w .save
  if {[winfo exists $w]} { return }
  win::createDialog $w
  if {$gnum == 0} {
    wm title $w "Scid: [tr GameAdd]"
    pack [ttk::frame $w.refdb] -side top -fill x -pady {0 10}
    CreateSelectDBWidget "$w.refdb" "gameSave_toBase" [sc_base current] 0
  } else {
    wm title $w "Scid: [tr GameReplace]"
  }
  set gsaveNum $gnum
  catch {grab $w}
  
  set f $w.g
  autoscrollText y $f $f.list Treeview
  $f.list configure -height 9 -width 40 -state disabled \
      -tabs {2c right 2.5c left} -wrap none
  ttk::label $f.title -textvar ::tr(NameEditMatches)
  pack $f -side top -anchor w
  
  # Get current values of tags:
  set year [sc_game tag get Year];    set eyear [sc_game tag get EYear]
  set month [sc_game tag get Month];  set emonth [sc_game tag get EMonth]
  set day [sc_game tag get Day];      set eday [sc_game tag get EDay]
  set white [sc_game tag get White];  set black [sc_game tag get Black]
  set event [sc_game tag get Event];  set site [sc_game tag get Site]
  set resultVal [sc_game tag get Result];  set round [sc_game tag get Round]
  set whiteElo [sc_game tag get WhiteElo]
  set blackElo [sc_game tag get BlackElo]
  set whiteRType [sc_game tag get WhiteRType]
  set blackRType [sc_game tag get BlackRType]
  set eco [sc_game tag get ECO]
  set extraTags [sc_game tag get Extra]
  if {[::board::isFlipped .main.board]} {
    regsub -all {FlipB "[01]"\n} $extraTags {} extraTags
    append extraTags "FlipB \"1\"\n"
  } else {
    regexp {FlipB "([01])"\n} $extraTags -> flipB
    if {[info exists flipB] && $flipB} {
      regsub -all {FlipB "[01]"\n} $extraTags {} extraTags
    }
  }
  clearMatchList $f.list
  
  # Use question marks instead of zero values in date:
  if {$year == 0} { set year "????" }
  if {$month == 0} { set month "??" }
  if {$day == 0} { set day "??" }
  if {$eyear == 0} { set eyear "????" }
  if {$emonth == 0} { set emonth "??" }
  if {$eday == 0} { set eday "??" }
  
  addGameSaveEntry event 0 ::tr(Event:)
  addGameSaveEntry site 1 ::tr(Site:)
  
  ttk::frame $f.dateframe
  ttk::label $f.datelabel -textvar ::tr(Date:)
  ttk::entry $f.dateyear -width 6 -textvariable year -justify right ;# -background white -relief sunken
  ttk::label $f.datedot1 -text "."
  ttk::entry $f.datemonth -width 3 -textvariable month -justify right
  ttk::label $f.datedot2 -text "."
  ttk::entry $f.dateday -width 3 -textvariable day -justify right
  grid $f.datelabel -row 2 -column 0 -sticky w
  grid $f.dateframe -row 2 -column 1 -columnspan 5 -sticky w
  ttk::button $f.datechoose -image tb_calendar -style Pad0.Small.TButton -command {
    set newdate [::utils::date::chooser "$year-$month-$day"]
    if {[llength $newdate] == 3} {
      set year [lindex $newdate 0]
      set month [lindex $newdate 1]
      set day [lindex $newdate 2]
    }
  }
  ttk::button $f.today -textvar ::tr(Today) -command {
    set year [::utils::date::today year]
    set month [::utils::date::today month]
    set day [::utils::date::today day]
  }
  pack $f.dateyear $f.datedot1 $f.datemonth $f.datedot2 $f.dateday \
      -in $f.dateframe -side left
  if {$::tcl_version >= 8.3} {
    pack $f.datechoose -in $f.dateframe -side left
  }
  pack $f.today -in $f.dateframe -side left
  
  ttk::frame $f.edateframe
  ttk::label $f.edatelabel -textvar ::tr(EventDate:)
  ttk::entry $f.edateyear -width 6 -textvariable eyear -justify right
  ttk::label $f.edatedot1 -text "."
  ttk::entry $f.edatemonth -width 3 -textvariable emonth -justify right
  ttk::label $f.edatedot2 -text "."
  ttk::entry $f.edateday -width 3 -textvariable eday -justify right
  grid $f.edatelabel -row 3 -column 0 -sticky w
  grid $f.edateframe -row 3 -column 1 -columnspan 5 -sticky w
  ttk::button $f.edatechoose -image tb_calendar -style Pad0.Small.TButton -command {
    set newdate [::utils::date::chooser "$eyear-$emonth-$eday"]
    if {[llength $newdate] == 3} {
      set eyear [lindex $newdate 0]
      set emonth [lindex $newdate 1]
      set eday [lindex $newdate 2]
    }
  }
  ttk::button $f.esame -text "=$::tr(Date)" -command {
    set eyear $year
    set emonth $month
    set eday $day
  }
  pack $f.edateyear $f.edatedot1 $f.edatemonth $f.edatedot2 $f.edateday \
      -in $f.edateframe -side left
  if {$::tcl_version >= 8.3} {
    pack $f.edatechoose -in $f.edateframe -side left
  }
  pack $f.esame -in $f.edateframe -side left
  
  addGameSaveEntry round 4 ::tr(Round:)
  addGameSaveEntry white 5 ::tr(White:)
  addGameSaveEntry black 6 ::tr(Black:)
  
  ttk::label $f.reslabel -textvar ::tr(Result:)
  ttk::frame $f.resentry
  ttk::radiobutton $f.resentry.w -text "1-0  " -variable resultVal -value 1
  ttk::radiobutton $f.resentry.b -text "0-1  " -variable resultVal -value 0
  ttk::radiobutton $f.resentry.r -text "1/2-1/2  " -variable resultVal -value =
  ttk::radiobutton $f.resentry.u -text "*" -variable resultVal -value *
  pack $f.resentry.w $f.resentry.b $f.resentry.r $f.resentry.u -side left 
  grid $f.reslabel -row 7 -column 0 -sticky w
  grid $f.resentry -row 7 -column 1 -sticky w -columnspan 4
  
  ttk::label $f.welolabel -text "$::tr(White) "
  
  ttk::combobox $f.wrtype -values [sc_info ratings] -width 7 -textvariable whiteRType
  
  ttk::entry $f.weloentry -width 5 -textvariable whiteElo -justify right
  
  ttk::label $f.belolabel -text "$::tr(Black) "
  
  ttk::combobox $f.brtype -values [sc_info ratings] -width 7 -textvariable blackRType
  
  ttk::entry $f.beloentry -width 5 -textvariable blackElo -justify right
  
  grid $f.welolabel -row 8 -column 0 -sticky w
  grid $f.wrtype -row 8 -column 1 -sticky w
  grid $f.weloentry -row 8 -column 2 -sticky w
  grid $f.belolabel -row 9 -column 0 -sticky w
  grid $f.brtype -row 9 -column 1 -sticky w
  grid $f.beloentry -row 9 -column 2 -sticky w
  
  ttk::label $f.ecolabel -text "ECO Code:"
  ttk::entry $f.ecoentry -width 6 -textvariable eco
  grid $f.ecolabel -row 10 -column 0 -sticky w
  grid $f.ecoentry -row 10 -column 1 -sticky w
  
  ttk::button $f.ecob -textvar ::tr(ClassifyGame) -command {set eco [sc_eco game]}
  grid $f.ecob -row 10 -column 2 -sticky w
  
  grid $f.title -row 0 -column 8 -sticky w  -padx "10 0"
  grid $f.list -row 1 -column 8 -rowspan 9 -sticky nw -padx "10 0"
  
  ttk::labelframe .save.extrafr -text "Extra Tags: (example format: Annotator \"Anand, V\") "
  autoscrollText y .save.extra .save.extra.text Treeview
  .save.extra.text configure -height 4 -width 40 -wrap none -state normal -relief sunken
  pack .save.extrafr -side top -fill both -expand 1
  pack .save.extra -in .save.extrafr -side left -fill both -expand 1
  # Override tab-binding for this text widget:
  bind .save.extra.text <Key-Tab> "[bind all <Key-Tab>]; break"
  ttk::button .save.extrafr.last -text $::tr(UseLastTag) -command {
    set extraTags [sc_game tag get -last Extra]
    .save.extra.text delete 1.0 end
    .save.extra.text insert 1.0 $extraTags
  }
  if {$gnum == 0} {
    pack .save.extrafr.last -side right -padx 10
  }
  .save.extra.text insert 1.0 $extraTags
  
  foreach i {entryevent entrysite dateyear datemonth dateday \
        entryround entrywhite entryblack resentry \
        weloentry beloentry ecoentry edateyear edatemonth edateday} {
    bind $f.$i <Return> {.save.buttons.save invoke}
  }
  
  # Bindings so Ctrl-1 to Ctrl-9 select a matching name in the player,
  # site, event and round entryboxes:
  
  set j 0
  foreach {i j} {entryevent "event" entrysite "site"
    entrywhite "white" entryblack "black"
    entryround "round" } {
    for {set z 1} {$z <= 9} {incr z} {
      bind $f.$i [format "<Control-Key-%d>" $z] \
          [format "eval {if {\$nameMatchCount >= %d} \
            {set %s \$nameMatches(%d)}}" $z $j $z ]
    }
  }
  
  ttk::frame .save.buttons
  if {$gnum == 0} {
    ttk::button .save.buttons.prev -text "As last game" -command {
    }
  }
  dialogbutton .save.buttons.save -textvar ::tr(Save) -underline 0 -command {
    set extraTags [.save.extra.text get 1.0 end-1c]
    if { [gsave $gsaveNum] } {
      destroy .save
    } else {
      ERROR::MessageBox
    }
  }
  bind .save <Alt-s> { .save.buttons.save invoke; break }
  
  dialogbutton .save.buttons.cancel -textvar ::tr(Cancel) -command {destroy .save}
  pack .save.buttons -side bottom -padx 10 -fill both -expand 1
  if {$gnum == 0} {
    #pack .save.buttons.prev -side left -padx 10
  }
  packdlgbuttons .save.buttons.cancel .save.buttons.save
  
  bind .save <Escape> { focus .; destroy .save; }
  focus .save.g.entryevent
  .save.g.entryevent selection range 0 end
  if {$gnum > 0} { focus .save.buttons.save }
}

# gsave:
#    Called by gameSave when the user presses the "Save" button
#    to save the game. Attempts to save and reports the result.
#
proc gsave { gnum } {
  global date year month day white black resultVal event site round
  global whiteElo blackElo whiteRType blackRType eco extraTags
  global edate eyear emonth eday
  
  set date [format "%s.%s.%s" $year $month $day]
  set edate [format "%s.%s.%s" $eyear $emonth $eday]
  set extraTagsList [split $extraTags "\n"]
  sc_game tags set -event $event -site $site -date $date -round $round \
      -white $white -black $black -result $resultVal \
      -whiteElo $whiteElo -whiteRatingType $whiteRType \
      -blackElo $blackElo -blackRatingType $blackRType \
      -eco $eco -eventdate $edate -extra $extraTagsList
  if {$gnum != 0} {
    if { [catch {sc_game save $gnum}] } { return 0 }
    ::notify::DatabaseModified [sc_base current]
    ::notify::GameChanged
  } else {
    set ply [sc_pos pgnOffset]
    if { [catch {sc_game save $gnum $::gameSave_toBase}] } { return 0 }
    ::notify::DatabaseModified $::gameSave_toBase
    ::file::SwitchToBase $::gameSave_toBase 0
    ::game::Load [sc_base numGames $::gameSave_toBase]
    ::move::PGNOffset $ply
  }
  return 1
}

# gameAdd:
#   Calls gameSave with a game number of zero.
#
proc gameAdd {} { gameSave 0 }

proc gameAddToClipbase {} {
  if {[catch {sc_game save 0 $::clipbase_db}]} {
    ERROR::MessageBox
  }
  ::notify::DatabaseModified $::clipbase_db
}

# gameReplace:
#   Calls gameSave with the current game number, which should be nonzero.
#
proc gameReplace {} { gameSave [sc_game number] }


# helpAbout:
#    Displays information about Scid.
#
proc helpAbout {} {
  ::utils::sound::PlaySound sound_move
  set str {}
  append str "Scid: Shane's chess information database\n\n"
  append str "Version $::scidVersion, $::scidVersionDate\n"
  append str "Copyright (C) 1999-2004 Shane Hudson\n"
  append str "Copyright (C) 2006-2009 Pascal Georges\n"
  append str "Copyright (C) 2008-2011 Alexander Wagner\n"
  append str "Copyright (C) 2011 Gerd Lorscheid\n"
  append str "Copyright (C) 2009-2019 Fulvio Benini\n"
  append str "\nScid is licenced under the\n"
  append str "GNU General Public License.\n"

  append str "\n\nUsing Tcl/Tk version: [info patchlevel]\n"
  append str "Loaded packages:\n"
  foreach {pkg} [lsort [package names]] {
    set ver [package provide $pkg]
    if {$ver != ""} { append str "$pkg\($ver),  " }
  }
  regsub ",  $" $str "." str

  tk_messageBox -title "About Scid" -message $str -type ok
}


proc MouseWheelRedirector {W X Y D} {
    # Generate an MWheel virtual event to the window that has the mouse pointer
    set w [winfo containing -displayof $W $X $Y]
    # Check for active "grab"
    set grabW [grab current $w]
    if {$grabW != ""} { set w $grabW}
    # For virtual events we can't use the attribute "-delta" and we get away
    # without a global variable by using instead "-data" which maps to %d
    event generate $w <<MWheel>> -data $D -rootx $X -rooty $Y
}

proc ShiftMouseWheelRedirector {W X Y D} {
    set w [winfo containing -displayof $W $X $Y]
    set grabW [grab current $w]
    if {$grabW != ""} { set w $grabW}
    event generate $w <<Shift-MWheel>> -data $D -rootx $X -rooty $Y
}

# On Windows, redirect mouse wheel events to those windows that have the
# mouse pointer:
# - Disable completely MouseWheel handling for all used classes. No class or
#   window should listen to MouseWheel to avoid double firing
# - Any window that receives a MouseWheel redirects these events as MWheel
#   to the window with the mouse pointer
# - Rebind all classes and windows that handle mouse wheel to MWheel events.
#   Bind code is from TCL 8.5
# On Linux and Macs this redirection is not necessary

if { [tk windowingsystem] == "win32" } {
    # Disable MouseWheel handling
    set mw_classes [list Text Listbox Treeview]
    foreach class $mw_classes { bind $class <MouseWheel> {} }

    # Transform MouseWheel events into MWheel
    bind all <MouseWheel> { MouseWheelRedirector %W %X %Y %D }
    bind all <Shift-MouseWheel> { ShiftMouseWheelRedirector %W %X %Y %D }

    # Bind classes to MWheel
    bind Listbox <<MWheel>> { %W yview scroll [expr {-(%d/120) * 4}] units}
    bind Treeview <<MWheel>> { %W yview scroll [expr {-(%d/120)}] units }
    bind Text <<MWheel>> {
	if {%d >= 0} { %W yview scroll [expr {-%d/3}] pixels
	} else { %W yview scroll [expr {(2-%d)/3}] pixels }
    }
}


#################
# Open files and databases:

# Check for arguments starting with "-" (or "/" on Windows):

set loadAtStart(spell) 1
set loadAtStart(eco) 1
set loadAtStart(tb) 1

proc getCommandLineOptions {} {
  global argc argv windowsOS loadAtStart
  
  if { $::macOS && ([string first "-psn" [lindex $argv 0]] == 0)} {
    # Remove Process Serial Number from argument list:
    set argv [lrange $argv 1 end]
    incr argc -1
  }
  
  while {$argc > 0} {
    set arg [lindex $argv 0]
    set firstChar [string index $arg 0]
    if {$firstChar == "-"  ||  ($windowsOS  &&  $firstChar == "/")} {
      # Seen option argument:
      incr argc -1
      set argv [lrange $argv 1 end]
      
      # Special argument "--" means no more options:
      if {$arg == "--"} { return }
      
      # Check for known option names:
      #   -f (/f), -fast (/fast): Fast start with no tablebases, etc.
      #   -xeco, -xe: Do not load ECO file.
      #   -xspell, -xs: Do not load spellcheck file.
      #   -xtb, -xt: Do not check tablebase directory.
      
      set argName [string range $arg 1 end]
      switch $argName {
        "f"    -
        "fast" {
          ::splash::add "Fast start: no tablebases, ECO or spelling file loaded."
          set loadAtStart(spell) 0
          set loadAtStart(eco) 0
          set loadAtStart(tb) 0
        }
        "xt" -
        "xtb" {
          set loadAtStart(tb) 0
        }
        "xe" -
        "xeco" {
          set loadAtStart(eco) 0
        }
        "xs" -
        "xspell" {
          set loadAtStart(spell) 0
        }
        default {
          ::splash::add "Warning: unknown option: \"$arg\""
        }
      }
    } else {
      # Seen first non-option argument:
      return
    }
  }
}

getCommandLineOptions

# Try to find tablebases:
if {$loadAtStart(tb)} {
  if {[sc_info tb]} {
    ::splash::add "Checking for endgame tablebase files..."
    set tbDirs {}
    foreach i {1 2 3 4} {
      if {$initialDir(tablebase$i) != ""} {
        if {$tbDirs != ""} { append tbDirs ";" }
        append tbDirs [file nativename $initialDir(tablebase$i)]
      }
    }
    set result 0
    if {$tbDirs != ""} {
      set result [sc_info tb $tbDirs]
    }
    if {$result == 0} {
      ::splash::add "    No tablebases were found."
    } else {
      ::splash::add "    Tablebases with up to $result pieces were found."
    }
  }
}

# Try to open the ECO classification file:
set result 0
set ecoFile_fullname [file nativename $ecoFile]

if {$loadAtStart(eco)} {
  ::splash::add "Trying to load the ECO openings file..."
  if {[catch { sc_eco read $ecoFile_fullname } result]} {
    # Could not load, so try "scid.eco" in the current directory:
    if {[catch {sc_eco read "scid.eco"} result]} {
      # Neither attempt worked, so do not use ECO classification
      ::splash::add "    Unable to open the ECO file: $ecoFile"
    } else {
      ::splash::add "    ECO file \"./scid.eco\" loaded: $result positions."
    }
  } else {
    ::splash::add "    ECO file \"[file tail $ecoFile_fullname]\"  loaded: $result positions."
  }
}

################################################################################
# returns a list of all toplevel windows, except some that are utilities
################################################################################
proc getTopLevel {} {
  
  set topl {}
  set exclude { ".glistExtra" ".menu" "." ".pgnPopup" }
  foreach c [winfo children .] {
    if { $c != [winfo toplevel $c] } { continue }
    # Tk report .__tk_filedialog as toplevel window even if the window has been closed
    if { [ lsearch $topl $c ] == -1 && [ lsearch $exclude $c ] == -1 && ![string match "\.__tk*" $c] } {
      lappend topl $c
    }
  }
  return $topl
}

# Disable ttk default left/right key bindings (they invoke ttk::notebook::CycleTab)
bind TNotebook <Key-Right> {}
bind TNotebook <Key-Left>  {}

wm iconname . "Scid"
wm protocol . WM_DELETE_WINDOW { ::file::Exit }
setMenu . .menu
keyboardShortcuts .

::docking::layout_restore 1

menuUpdateBases
menuUpdateThemes
setLanguageMenus

if {$startup(tip)} { ::tip::show }

# Try to load the spellcheck file:
if {$loadAtStart(spell)} {
  progressWindow "Scid - [tr Spellcheking]" "Loading $spellCheckFile ..."
  set err [catch {sc_name read $spellCheckFile} result]
  closeProgressWindow
}


after 1 {
  ::file::autoLoadBases.load

  # fullname:
  # Given a file name, returns its absolute name.
  #
  proc fullname {fname} {
    if {[file pathtype $fname] == "absolute"} { return $fname }
    set old [pwd]
    if {[catch {cd [file dirname $fname]}]} { return $fname }
    set fname [file join [pwd] [file tail $fname]]
    catch {cd $old}
    return $fname
  }
  # Loading a database if specified on the command line:
  foreach cmdbase $::argv {
    ::file::Open "[fullname $cmdbase]"
  }
}


### End of file: end.tcl
