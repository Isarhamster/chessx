###
### book.tcl: part of Scid.
### Copyright (C) 2007  Pascal Georges
###
######################################################################
### Book window

namespace eval book {
  set isOpen 0
  set isReadonly 0
  set bookList ""
  set bookPath ""
  set currentBook "" ; # book in form abc.bin
  set currentTuningBook ""
  set bookMoves ""
  set cancelBookExport 0
  set exportCount 0
  set exportMax 3000
  set hashList ""
  set bookSlot 0
  set bookTuningSlot 2
  set oppMovesVisible 0
  
  
  ################################################################################
  # open a book, closing any previously opened one (called by annotation analysis)
  # arg name : gm2600.bin for example
  ################################################################################
  proc scBookOpen { name slot } {
    if {$slot == $::book::bookSlot} {
      if {$::book::currentBook != ""} {
        sc_book close $::book::bookSlot
      }
      set ::book::currentBook $name
    }
    if {$slot == $::book::bookTuningSlot} {
      if {$::book::currentTuningBook != ""} {
        sc_book close $::book::bookTuningSlot
      }
      set ::book::currentTuningBook $name
    }
    
    set bn [ file join $::scidBooksDir $name ]
    set ::book::isReadonly [sc_book load $bn $slot]
  }
  
  ################################################################################
  # Return a move in book for position fen. If there is no move in book, returns ""
  # Is used by engines, not book windows
  ################################################################################
  proc getMove { book fen slot} {
    set tprob 0
    ::book::scBookOpen $book $slot
    set bookmoves [sc_book moves $slot]
    if {[llength $bookmoves] == 0} {
      return ""
    }
    set r [expr {(int (rand() * 100))} ]
    for {set i 0} {$i<[llength $bookmoves]} {incr i 2} {
      set m [lindex $bookmoves $i]
      set prob [string range [lindex $bookmoves [expr $i + 1] ] 0 end-1 ]
      incr tprob $prob
      if { $tprob >= $r } {
        break
      }
    }
    sc_book close $slot
    return $m
  }
  
  ################################################################################
  #  Show moves leading to book positions
  ################################################################################
  proc togglePositionsDisplay {} {
    global ::book::oppMovesVisible
    if { $::book::oppMovesVisible == 0} {
      set ::book::oppMovesVisible 1
      pack .bookWin.f.fscroll1 -expand yes -fill both
    } else {
      set ::book::oppMovesVisible 0
      pack forget .bookWin.f.fscroll1
    }
  }
  
  ################################################################################
  #  Open a window to select book and display book moves
  # arg name : gm2600.bin for example
  ################################################################################
  proc open { {name ""} } {
    global ::book::bookList ::book::bookPath ::book::currentBook ::book::isOpen ::book::lastBook
    
    set w .bookWin
    
    if {[winfo exists $w]} { return }
    
    set ::book::isOpen 1
    
    ::createToplevel $w
    ::setTitle $w $::tr(Book)
    wm resizable $w 0 1
    
    ttk::frame $w.f
    
    # load book names
    if { $name == "" && $lastBook != "" } {
      set name $lastBook
    }
    set bookPath $::scidBooksDir
    set bookList [  lsort -dictionary [ glob -nocomplain -directory $bookPath *.bin ] ]
    
    # No book found
    if { [llength $bookList] == 0 } {
      tk_messageBox -title "Scid" -type ok -icon error -message "No books found. Check books directory"
      set ::book::isOpen 0
      set ::book::currentBook ""
      ::win::closeWindow $w
      return
    }
    
    set i 0
    set idx 0
    set tmp {}
    foreach file  $bookList {
      set f [ file tail $file ]
      lappend tmp $f
      if {$name == $f} {
        set idx $i
      }
      incr i
    }
    ttk::combobox $w.f.combo -width 12 -values $tmp
    
    catch { $w.f.combo current $idx }
    pack $w.f.combo
    
    # text displaying book moves
    autoscrollText y $w.f.fscroll $w.f.text Treeview
    $w.f.text configure -wrap word -state disabled -width 12
    
    ttk::button $w.f.b -text $::tr(OtherBookMoves)  -command { ::book::togglePositionsDisplay }
    ::utils::tooltip::Set $w.f.b $::tr(OtherBookMovesTooltip)

    autoscrollText y $w.f.fscroll1 $w.f.text1 Treeview
    $w.f.text1 configure -wrap word -state disabled -width 12

    pack $w.f.fscroll -expand yes -fill both
    pack $w.f.b -fill x
    if { $::book::oppMovesVisible == 1 } {
        pack $w.f.fscroll1 -expand yes -fill both
    }
    pack $w.f -expand 1 -fill both
    
    bind $w.f.combo <<ComboboxSelected>> ::book::bookSelect
    bind $w <Destroy> "::book::closeMainBook"
    # we make a redundant check here, another one is done a few line above
    if { [catch {bookSelect} ] } {
      tk_messageBox -title "Scid" -type ok -icon error -message "No books found. Check books directory"
      set ::book::isOpen 0
      set ::book::currentBook ""
      ::win::closeWindow .bookWin
    }
  }
  ################################################################################
  #
  ################################################################################
  proc closeMainBook {} {
    if { $::book::currentBook == "" } { return }
    focus .
    sc_book close $::book::bookSlot
    set ::book::isOpen 0
    set ::book::currentBook ""
  }
  ################################################################################
  #   updates book display when board changes
  ################################################################################
  proc refresh {} {
    global ::book::bookMoves
    
    foreach t [.bookWin.f.text tag names] {
      if { [string match "bookMove*" $t] } {
        .bookWin.f.text tag delete $t
      }
    }
    foreach t [.bookWin.f.text1 tag names] {
      if { [string match "bookMove*" $t] } {
        .bookWin.f.text1 tag delete $t
      }
    }
    set bookMoves [sc_book moves $::book::bookSlot]
    .bookWin.f.text configure -state normal
    .bookWin.f.text delete 1.0 end
    for {set i 0} {$i<[llength $bookMoves]} {incr i 2} {
      set line [expr $i /2 +1]
      set m ""
      append m [::trans [lindex $bookMoves $i]] "\t" [lindex $bookMoves [expr $i + 1] ] "\n"
      .bookWin.f.text insert end $m
      .bookWin.f.text tag add bookMove$line $line.0 $line.end
      .bookWin.f.text tag bind bookMove$line <ButtonPress-1> "::book::makeBookMove [lindex $bookMoves $i]"
    }
    .bookWin.f.text configure -state disabled -height [expr [llength $bookMoves] / 2 ]
    
    
    set oppBookMoves [sc_book positions $::book::bookSlot]
    .bookWin.f.text1 configure -state normal
    .bookWin.f.text1 delete 1.0 end
    for {set i 0} {$i<[llength $oppBookMoves]} {incr i 1} {
      set line [expr $i +1]
      set m ""
      append m [::trans [lindex $oppBookMoves $i]]  "\n"
      .bookWin.f.text1 insert end $m
      .bookWin.f.text1 tag add bookMove$line $line.0 $line.end
      .bookWin.f.text1 tag bind bookMove$line <ButtonPress-1> "::book::makeBookMove [lindex $oppBookMoves $i]"
    }
    .bookWin.f.text1 configure -state disabled -height [llength $oppBookMoves]
    if { $::book::oppMovesVisible == 0 } {
      pack forget .bookWin.f.scroll1
    }
  }
  ################################################################################
  #
  ################################################################################
  proc makeBookMove { move } {
    addSanMove $move
  }
  ################################################################################
  #
  ################################################################################
  proc bookSelect { { n "" }  { v  0} } {
    set ::book::lastBook [.bookWin.f.combo get]
    scBookOpen [.bookWin.f.combo get] $::book::bookSlot
    refresh
  }
  ################################################################################
  #
  ################################################################################
  proc tuning { {name ""} } {
    global ::book::bookList ::book::bookPath ::book::currentBook ::book::isOpen
    
    set w .bookTuningWin
    
    if {[winfo exists $w]} {
      return
    }
    
    ::createToplevel $w
    ::setTitle $w $::tr(Book)
    # wm resizable $w 0 0
    
    bind $w <F1> { helpWindow BookTuningWindow }
    setWinLocation $w
    
    ttk::frame $w.fcombo
    ttk::frame $w.f
    # load book names
    set bookPath $::scidBooksDir
    set bookList [  lsort -dictionary [ glob -nocomplain -directory $bookPath *.bin ] ]
    
    # No book found
    if { [llength $bookList] == 0 } {
      tk_messageBox -title "Scid" -type ok -icon error -message "No books found. Check books directory"
      set ::book::isOpen 0
      set ::book::currentBook ""
      ::win::closeWindow $w
      return
    }
    
    set i 0
    set idx 0
    set tmp {}
    foreach file  $bookList {
      set f [ file tail $file ]
      lappend tmp $f
      if {$name == $f} {
        set idx $i
      }
      incr i
    }
    
    ttk::combobox $w.fcombo.combo -width 12 -values $tmp
    catch { $w.fcombo.combo current $idx }
    pack $w.fcombo.combo -expand yes -fill x
    
    ttk::frame $w.fbutton
    
    
    ttk::menubutton $w.fbutton.mbAdd -text $::tr(AddMove) -menu $w.fbutton.mbAdd.otherMoves
    menu $w.fbutton.mbAdd.otherMoves
    
    
    ttk::button $w.fbutton.bExport -text $::tr(Export) -command ::book::export
    ttk::button $w.fbutton.bSave -text $::tr(Save) -command ::book::save
    
    pack $w.fbutton.mbAdd $w.fbutton.bExport $w.fbutton.bSave -side top -fill x -expand yes
    
    
    pack $w.fcombo $w.f $w.fbutton -side top
    
    bind $w.fcombo.combo <<ComboboxSelected>> ::book::bookTuningSelect
    
    bind $w <Destroy> "if {\[string equal $w %W\]} { ::book::closeTuningBook }"
    bind $w <F1> { helpWindow BookTuning }
    
    bookTuningSelect
    
  }
  ################################################################################
  #
  ################################################################################
  proc closeTuningBook {} {
    if { $::book::currentTuningBook == "" } { return }
    focus .
    sc_book close $::book::bookTuningSlot
    set ::book::currentTuningBook ""
  }
  ################################################################################
  #
  ################################################################################
  proc bookTuningSelect { { n "" }  { v  0} } {
    set w .bookTuningWin
    scBookOpen [.bookTuningWin.fcombo.combo get] $::book::bookTuningSlot
    if { $::book::isReadonly > 0 } {
      $w.fbutton.bSave configure -state disabled
    } else {
      $w.fbutton.bSave configure -state normal
    }
    refreshTuning
  }
  ################################################################################
  #   add a move to displayed bookmoves
  ################################################################################
  proc addBookMove { move } {
    global ::book::bookTuningMoves
    
    if { $::book::isReadonly > 0 } { return }
    
    set w .bookTuningWin
    set children [winfo children $w.f]
    set count [expr [llength $children] / 2]
    ttk::label $w.f.m$count -text [::trans $move]
    bind $w.f.m$count <ButtonPress-1> " ::book::makeBookMove $move"
    ttk::spinbox $w.f.sp$count -from 0 -to 100 -width 3
    $w.f.sp$count set 0
    grid $w.f.m$count -row $count -column 0 -sticky w
    grid $w.f.sp$count -row $count -column 1 -sticky w
    $w.fbutton.mbAdd.otherMoves delete [::trans $move]
    lappend ::book::bookTuningMoves $move
  }
  ################################################################################
  #   updates book display when board changes
  ################################################################################
  proc refreshTuning {} {
    
    if { $::book::isReadonly > 0 } { return }
    
    #unfortunately we need this as the moves on the widgets are translated
    #and widgets have no clientdata in tcl/tk
    global ::book::bookTuningMoves
    set ::book::bookTuningMoves {}
    set moves [sc_book moves $::book::bookTuningSlot]
    
    set w .bookTuningWin
    # erase previous children
    set children [winfo children $w.f]
    foreach c $children {
      destroy $c
    }
    
    set row 0
    for {set i 0} {$i<[llength $moves]} {incr i 2} {
      lappend ::book::bookTuningMoves [lindex $moves $i]
      ttk::label $w.f.m$row -text [::trans [lindex $moves $i]]
      bind $w.f.m$row <ButtonPress-1> " ::book::makeBookMove [lindex $moves $i] "
      ttk::spinbox $w.f.sp$row -from 0 -to 100 -width 3
      set pct [lindex $moves [expr $i+1] ]
      set value [string replace $pct end end ""]
      $w.f.sp$row set $value
      grid $w.f.m$row -row $row -column 0 -sticky w
      grid $w.f.sp$row -row $row -column 1 -sticky w
      incr row
    }
    # load legal moves
    $w.fbutton.mbAdd.otherMoves delete 0 end
    $w.fbutton.mbAdd.otherMoves add command -label $::tr(None)
    set moveList [ sc_pos moves ]
    foreach move $moveList {
      if { [ lsearch  $moves $move ] == -1 } {
        $w.fbutton.mbAdd.otherMoves add command -label [::trans $move] -command "::book::addBookMove $move"
      }
    }
  }
  ################################################################################
  # sends to book the list of moves and probabilities.
  ################################################################################
  proc save {} {
    global ::book::bookTuningMoves
    if { $::book::isReadonly > 0 } { return }
    
    set prob {}
    set w .bookTuningWin
    set children [winfo children $w.f]
    set count [expr [llength $children] / 2]
    for {set row 0} {$row < $count} {incr row} {
      lappend prob [$w.f.sp$row get]
    }
    set tempfile [file join $::scidUserDir tempfile.[pid]]
    sc_book movesupdate $::book::bookTuningMoves $prob $::book::bookTuningSlot $tempfile
    file delete $tempfile
    if {  [ winfo exists .bookWin ] } {
      ::book::refresh
    }
  }
  ################################################################################
  #
  ################################################################################
  proc export {} {
    ::windows::gamelist::Refresh
    updateTitle
    progressWindow "Scid" "ExportingBook..." $::tr(Cancel) "::book::sc_progressBar"
    set ::book::cancelBookExport 0
    set ::book::exportCount 0
    ::book::book2pgn
    set ::book::hashList ""
    closeProgressWindow
    if { $::book::exportCount >= $::book::exportMax } {
      tk_messageBox -title "Scid" -type ok -icon info \
          -message "$::tr(Movesloaded)  $::book::exportCount\n$::tr(BookPartiallyLoaded)"
    } else  {
      tk_messageBox -title "Scid" -type ok -icon info -message "$::tr(Movesloaded)  $::book::exportCount"
    }
    updateBoard -pgn
  }
  
  ################################################################################
  #
  ################################################################################
  proc book2pgn { } {
    global ::book::hashList
    
    if {$::book::cancelBookExport} { return  }
    if { $::book::exportCount >= $::book::exportMax } {
      return
    }
    set hash [sc_pos hash]
    if {[lsearch -sorted -integer -exact $hashList $hash] != -1} {
      return
    } else  {
      lappend hashList $hash
      set hashList [lsort -integer -unique $hashList]
    }
    
    updateBoard -pgn
    
    set bookMoves [sc_book moves $::book::bookTuningSlot]
    incr ::book::exportCount
    if {[expr $::book::exportCount % 50] == 0} {
      updateProgressWindow $::book::exportCount $::book::exportMax
      update
    }
    if {[llength $bookMoves] == 0} { return }
    
    for {set i 0} {$i<[llength $bookMoves]} {incr i 2} {
      set move [lindex $bookMoves $i]
      if {$i == 0} {
        sc_move addSan $move
        book2pgn
        sc_move back
      } else  {
        sc_var create
        sc_move addSan $move
        book2pgn
        sc_var exit
      }
    }
    
  }
  ################################################################################
  # cancel book export
  ################################################################################
  proc sc_progressBar {} {
    set ::book::cancelBookExport 1
  }
}
###
### End of file: book.tcl
###
