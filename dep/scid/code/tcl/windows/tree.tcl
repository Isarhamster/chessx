
############################################################
### TREE window
### (C) 2007 Pascal Georges : multiple Tree windows support

namespace eval ::tree {
  set trainingBase 0
  array set cachesize {}
  set scoreHighlight_MinGames 15
  set scoreHighlight_WhiteExpectedScoreBonus 3.8 ; # on average white achieves a score of 53.8
  set scoreHighlight_Margin 3.0 ; # if +/- this value, something special happened
}
# ################################################################################
proc ::tree::doConfigMenus { baseNumber  { lang "" } } {
  if {! [winfo exists .treeWin$baseNumber]} { return }
  if {$lang == ""} { set lang $::language }
  set m .treeWin$baseNumber.menu
  foreach idx {0 1 2 3 4} tag {File Mask Sort Opt Help} {
    configMenuText $m $idx Tree$tag $lang
  }
  foreach idx {0 1 2 3} tag {SetCacheSize Graph Copy Close} {
    configMenuText $m.file $idx TreeFile$tag $lang
  }
  foreach idx {0 1 2 3 4 5 6 7 8 9} tag {New Open OpenRecent Save Close FillWithGame FillWithBase Search Info Display} {
    configMenuText $m.mask $idx TreeMask$tag $lang
  }
  foreach idx {0 1 2 3} tag {Alpha ECO Freq Score } {
    configMenuText $m.sort $idx TreeSort$tag $lang
  }
  foreach idx {0 1} tag {Lock Training } {
    configMenuText $m.opt $idx TreeOpt$tag $lang
  }
  foreach idx {0 1} tag {Tree Index} {
    configMenuText $m.helpmenu $idx TreeHelp$tag $lang
  }
}

# ################################################################################
proc ::tree::ConfigMenus { { lang "" } } {
  for {set i [sc_info limit bases] } {$i > 0} {incr i -1} {
    ::tree::doConfigMenus $i $lang
  }
}
################################################################################
proc ::tree::menuCopyToSelection { baseNumber } {
  clipboard clear
  clipboard append [ .treeWin$baseNumber.f.tl get 1.0 end ]
}
################################################################################
proc ::tree::make { { baseNumber -1 } {locked 0} } {
  global tree highcolor geometry helpMessage
  
  if {$baseNumber == -1} {set baseNumber [sc_base current]}
  
  if {[winfo exists .treeWin$baseNumber]} {
    ::tree::closeTree $baseNumber
    return
  }

  if {$baseNumber == [sc_base current]} { set ::treeWin 1 }
  set w .treeWin$baseNumber
  
  ::createToplevel .treeWin$baseNumber
  
  # Set the tree window title now:
  ::setTitle $w "Scid: [tr WindowsTree] $baseNumber"
  set ::treeWin$baseNumber 1
  set tree(training$baseNumber) 0
  set tree(autorefresh$baseNumber) 1
  set tree(locked$baseNumber) $locked
  set tree(base$baseNumber) $baseNumber
  set tree(status$baseNumber) ""
  set tree(order$baseNumber) "frequency"
  set tree(allgames$baseNumber) 1
  
  bind $w <Destroy> "::tree::closeTree $baseNumber"
  
  # Bind left button to close ctxt menu:
  bind $w <ButtonPress-1> "::tree::hideCtxtMenu $baseNumber"
  
  menu $w.menu
  ::setMenu $w $w.menu
  $w.menu add cascade -label TreeFile -menu $w.menu.file
  $w.menu add cascade -label TreeMask -menu $w.menu.mask
  $w.menu add cascade -label TreeSort -menu $w.menu.sort
  $w.menu add cascade -label TreeOpt  -menu $w.menu.opt
  $w.menu add cascade -label TreeHelp -menu $w.menu.helpmenu
  foreach i {file mask sort opt helpmenu} {
    menu $w.menu.$i -tearoff 0
  }
  
  menu $w.menu.file.size
  foreach i { 250 500 1000 2000 5000 10000 } {
    $w.menu.file.size add radiobutton -label "$i" -value $i -variable ::tree::cachesize($baseNumber) -command "::tree::setCacheSize $baseNumber $i"
  }
  
  $w.menu.file add cascade -menu $w.menu.file.size -label TreeFileSetCacheSize
  set helpMessage($w.menu.file,2) TreeFileSetCacheSize

  $w.menu.file add command -label TreeFileGraph -command "::tree::graph $baseNumber 1"
  set helpMessage($w.menu.file,5) TreeFileGraph
  $w.menu.file add command -label TreeFileCopy -command "::tree::menuCopyToSelection $baseNumber"
  set helpMessage($w.menu.file,7) TreeFileCopy
  $w.menu.file add command -label TreeFileClose -command ".treeWin$baseNumber.buttons.close invoke"
  set helpMessage($w.menu.file,9) TreeFileClose
  
  $w.menu.mask add command -label TreeMaskNew -command "::tree::mask::new"
  set helpMessage($w.menu.mask,0) TreeMaskNew
  $w.menu.mask add command -label TreeMaskOpen -command "::tree::mask::open"
  set helpMessage($w.menu.mask,1) TreeMaskOpen
  
  menu $w.menu.mask.recent
  foreach f $::tree::mask::recentMask {
    $w.menu.mask.recent add command -label $f -command "::tree::mask::open $f"
  }
  $w.menu.mask add cascade -label TreeMaskOpenRecent -menu $w.menu.mask.recent
  set helpMessage($w.menu.mask,2) TreeMaskOpenRecent
  
  $w.menu.mask add command -label TreeMaskSave -command "::tree::mask::save"
  set helpMessage($w.menu.mask,3) TreeMaskSave
  $w.menu.mask add command -label TreeMaskClose -command "::tree::mask::close"
  set helpMessage($w.menu.mask,4) TreeMaskClose
  $w.menu.mask add command -label TreeMaskFillWithGame -command "::tree::mask::fillWithGame"
  set helpMessage($w.menu.mask,5) TreeMaskFillWithGame
  $w.menu.mask add command -label TreeMaskFillWithBase -command "::tree::mask::fillWithBase"
  set helpMessage($w.menu.mask,6) TreeMaskFillWithBase

  $w.menu.mask add command -label TreeMaskSearch -command "::tree::mask::searchMask $baseNumber"
  set helpMessage($w.menu.mask,7) TreeMaskSearch
  $w.menu.mask add command -label TreeMaskInfo -command "::tree::mask::infoMask"
  set helpMessage($w.menu.mask,8) TreeMaskInfo
  $w.menu.mask add command -label TreeMaskDisplay -command "::tree::mask::displayMask"
  set helpMessage($w.menu.mask,9) TreeMaskDisplay
  
  foreach label {Alpha ECO Freq Score} value {alpha eco frequency score} {
    $w.menu.sort add radiobutton -label TreeSort$label \
        -variable tree(order$baseNumber) -value $value -command " ::tree::refresh $baseNumber "
  }
  
  $w.menu.opt add checkbutton -label TreeOptLock -variable tree(locked$baseNumber) -command "::tree::toggleLock $baseNumber"
  set helpMessage($w.menu.opt,0) TreeOptLock
  
  $w.menu.opt add checkbutton -label TreeOptTraining -variable tree(training$baseNumber) -command "::tree::toggleTraining $baseNumber"
  set helpMessage($w.menu.opt,1) TreeOptTraining
  
  $w.menu.helpmenu add command -label TreeHelpTree -accelerator F1 -command {helpWindow Tree}
  $w.menu.helpmenu add command -label TreeHelpIndex -command {helpWindow Index}
  
  ::tree::doConfigMenus $baseNumber
  
  autoscrollText y $w.f $w.f.tl Treeview
  $w.f.tl configure -wrap none  -setgrid 1 -exportselection 1 -font font_Fixed
  #define default tags
  $w.f.tl tag configure greybg -background gray95
  $w.f.tl tag configure whitebg -background white
  $w.f.tl tag configure bluefg -foreground blue
  $w.f.tl tag configure greenfg -foreground SeaGreen
  $w.f.tl tag configure redfg -foreground red
  
  selection handle $w.f.tl "::tree::copyToSelection $baseNumber"

  ttk::frame $w.statusframe
  pack $w.statusframe -side bottom -fill x
  grid rowconfigure $w.statusframe 0 -weight 1
  grid columnconfigure $w.statusframe 0 -weight 1
  ttk::label $w.status -anchor w -font font_Small -relief sunken
  grid $w.status -in $w.statusframe -column 0 -row 0 -sticky nsew
  canvas $w.progress -height 0 -bg white -relief solid -border 1
  $w.progress create rectangle 0 0 0 0 -fill blue -outline blue -tags bar

  pack [ttk::frame $w.buttons -relief sunken] -side bottom -fill x
  pack $w.f -side top -expand 1 -fill both
  
  ttk::button $w.buttons.best -image tb_list -style Pad0.Small.TButton -command "::tree::best $baseNumber"
  ttk::button $w.buttons.graph -image tb_bargraph -style Pad0.Small.TButton -command "::tree::graph $baseNumber 1"
  # add a button to start/stop tree refresh
  ttk::button $w.buttons.bStartStop -image tb_search_on -style Pad0.Small.TButton -command "::tree::toggleRefresh $baseNumber" ;# -relief flat
  
  ttk::checkbutton $w.buttons.allgames -textvar ::tr(allGames) -variable tree(allgames$baseNumber) -command "::tree::refresh $baseNumber"
  ttk::checkbutton $w.buttons.training -textvar ::tr(Training) -variable tree(training$baseNumber) -command "::tree::toggleTraining $baseNumber"
  
  foreach {b t} { best TreeFileBest graph TreeFileGraph allgames TreeOptLock  training TreeOptTraining bStartStop TreeOptStartStop } {
    set helpMessage($w.buttons.$b) $t
  }
  
  dialogbutton $w.buttons.stop -textvar ::tr(Stop)
  dialogbutton $w.buttons.close -textvar ::tr(Close) -command "::tree::closeTree $baseNumber"
  
  pack $w.buttons.best $w.buttons.graph $w.buttons.bStartStop $w.buttons.allgames $w.buttons.training \
      -side left -padx 3 -pady 2
  packbuttons right $w.buttons.close $w.buttons.stop
  $w.buttons.stop configure -state disabled
  
  wm minsize $w 40 5
  ::createToplevelFinalize $w
  
  ::tree::refresh $baseNumber
  set ::tree::cachesize($baseNumber) [sc_tree cacheinfo $baseNumber]
}
################################################################################
proc ::tree::hideCtxtMenu { baseNumber } {
  set w .treeWin$baseNumber.f.tl.ctxtMenu
  if {[winfo exists $w]} {
    destroy $w
    focus .treeWin$baseNumber
  }
}
################################################################################
proc ::tree::selectCallback { baseNumber move } {
  if {$::tree(autorefresh$baseNumber)} {
    tree::select $move $baseNumber
  }
}

################################################################################
# close the corresponding base if it is flagged as locked
proc ::tree::closeTree {baseNumber} {
  global tree
  ::tree::mask::close
  
  ::tree::hideCtxtMenu $baseNumber
    #  .treeWin$baseNumber.buttons.stop invoke

  set ::geometry(treeWin$baseNumber) [wm geometry .treeWin$baseNumber]
  focus .
  
  if {[winfo exists .treeGraph$baseNumber]} { destroy .treeGraph$baseNumber }
  destroy .treeBest$baseNumber
  destroy .treeWin$baseNumber
  if {$::tree(locked$baseNumber)} { ::file::Close $baseNumber }
  set curr_base [sc_base current]
  set ::treeWin [winfo exists .treeWin$curr_base]
}
################################################################################
proc ::tree::toggleTraining { baseNumber } {
  global tree
  
  for {set i 1 } {$i <= [sc_info limit bases]} {incr i} {
    if {! [winfo exists .treeWin$baseNumber] || $i == $baseNumber } { continue }
    set tree(training$i) 0
  }

  set ::tree::trainingBase 0
  if {$tree(training$baseNumber)} {
    set ::tree::trainingBase $baseNumber
    set ::tree::trainingColor [sc_pos side]
  }
  ::tree::refresh $baseNumber
}

################################################################################
proc ::tree::doTraining { { n 0 } } {
  global tree
  if {$n != 1  &&  [winfo exists .analysisWin1]  &&  $::analysis(automove1)} {
    automove 1
    return
  }
  if {$n != 2  &&  [winfo exists .analysisWin2]  &&  $::analysis(automove2)} {
    automove 2
    return
  }
  if {[::tb::isopen]  &&  $::tbTraining} {
    ::tb::move
    return
  }
  if {! [winfo exists .treeWin$::tree::trainingBase]} { return }
  if { $::tree::trainingBase == 0 } { return }
  
  # Before issuing a training move, annotate player's move
  if { $::tree::mask::maskFile != ""  } {
    set move_done [sc_game info previousMoveNT]
    if {$move_done != ""} {
      sc_move back
      set fen [ ::tree::mask::toShortFen [sc_pos fen] ]
      sc_move forward
      if { [info exists ::tree::mask::mask($fen)] } {
        set moves [ lindex $::tree::mask::mask($fen) 0 ]
        
        # if move out of Mask, and there exists moves in Mask, set a warning
        if { ! [ ::tree::mask::moveExists $move_done $fen ] } {
          if {[llength $moves] != 0} {
            set txt ""
            foreach elt $moves {
              append txt "[::trans [lindex $elt 0]][lindex $elt 1] "
            }
            sc_pos setComment "[sc_pos getComment] Mask : $txt"
          }
        }
        
        # if move was bad, set a warning
        set nag_played [::tree::mask::getNag $move_done $fen]
        set nag_order { "??" " ?" "?!" $::tree::mask::emptyNag "!?" " !" "!!"}
        set txt ""
        foreach elt $moves {
          set n [lindex $elt 1]
          if { [lsearch $nag_order $nag_played] < [lsearch $nag_order $n]} {
            append txt "[::trans [lindex $elt 0]][lindex $elt 1] "
          }
        }
        if {$txt != ""} {
          sc_pos addNag [string trim $nag_played]
          sc_pos setComment "[sc_pos getComment] Mask : $txt"
        }
        
        # if move was on an exclude line, set a warning (img = tb_cross)
        if { [::tree::mask::getImage $move_done 0] ==  "tb_cross" || \
              [::tree::mask::getImage $move_done 1] == "tb_cross"} {
          sc_pos setComment "[sc_pos getComment] Mask : excluded line"
        }
      }
    }
  }
  
  set moves [sc_filter treestats $::tree::trainingBase "tree"]
  set tot_freq 0
  foreach move $moves {
      lassign $move san freq
      if { $san ne {[end]} } { incr tot_freq $freq }
  }
  set random_move [expr {int( rand()*$tot_freq )}]
  set freq_move 0
  foreach move $moves {
      lassign $move san freq
      if { $san ne {[end]} } {
          incr freq_move $freq
          if {$random_move <= $freq_move } {
              return [addSanMove $move]
          }
      }
  }
}

################################################################################
proc ::tree::toggleLock { baseNumber } {
  ::tree::refresh $baseNumber
}

################################################################################
proc ::tree::select { move baseNumber } {
  global tree
  
  if {! [winfo exists .treeWin$baseNumber]} { return }
  
  catch { addSanMove $move }
}

################################################################################

# Returns the list of databases whose tree filter needs to be updated when
# the position of the main board changes.
# It must also disable commands that depend on the current position.
proc ::tree::listTreeBases {{base ""}} {
    if { $base == "" } {
        set base [sc_base list]
    }
    set bases {}
    foreach baseNumber $base {
        set w .treeWin$baseNumber
        if { [winfo exists $w] && $::tree(autorefresh$baseNumber) } {
            $w.buttons.best configure -state disabled
            $w.buttons.graph configure -state disabled
            $w.buttons.bStartStop configure -state disabled
            $w.buttons.training configure -state disabled
            $w.buttons.allgames configure -state disabled
            $w.buttons.close configure -state disabled
            $w.buttons.stop configure -state normal
            $w.progress coords bar 0 0 0 0
            grid $w.progress -in $w.statusframe -column 0 -row 0 -sticky nsew
            set progress "$w.progress [lindex [grid bbox $w.statusframe] 2] 100"
            $w.buttons.stop configure -command "
                ::cancelUpdateTreeFilter [list $progress]
                ::tree::restoreButtons $w
            "

            set filter "tree"
            if { $::tree(allgames$baseNumber) == 0 } {
                set filter [sc_filter compose $baseNumber "dbfilter" "tree"]
            }

            lappend bases [list $baseNumber $filter $progress]
        }
    }
    return $bases
}

proc ::tree::restoreButtons {w} {
    grid forget $w.progress
    $w.buttons.best configure -state normal
    $w.buttons.graph configure -state normal
    $w.buttons.bStartStop configure -state normal
    $w.buttons.training configure -state normal
    $w.buttons.allgames configure -state normal
    $w.buttons.close configure -state normal
    $w.buttons.stop configure -state disabled -command {}
}

proc ::tree::refresh { { baseNumber "" }} {
    if { $baseNumber != "" && [lsearch -exact [sc_base list] $baseNumber] == -1 } {
        displayLines $baseNumber "Error: the file is not open."
        return
    }
    ::updateTreeFilter $baseNumber
}

################################################################################
proc ::tree::dorefresh { baseNumber } {
  global tree treeWin
  set w .treeWin$baseNumber
  
  if {![winfo exists $w]} { return }
  if { ! $tree(autorefresh$baseNumber) } { return }

  set filter "tree"
  if { $tree(allgames$baseNumber) == 0 } {
    set filter [sc_filter compose $baseNumber "dbfilter" "tree"]
  }
  set err [ catch { sc_tree stats $baseNumber $filter $tree(training$baseNumber) \
                                  $tree(order$baseNumber) } moves]
  if { $err } {
    set tree(status$baseNumber) ""
    set moves [ERROR::getErrorMsg]
  }
  catch {$w.f.tl itemconfigure 0 -foreground darkBlue}

  ::tree::restoreButtons $w
  ::tree::status "" $baseNumber
  if {[winfo exists .treeGraph$baseNumber]} { ::tree::graph $baseNumber }

  displayLines $baseNumber $moves  

  if {$::tree::trainingBase != 0 && $::tree::trainingColor == [sc_pos side]} {
    ::tree::doTraining
  }
}

################################################################################
#
################################################################################
proc ::tree::displayLines { baseNumber moves } {
  global ::tree::mask::maskFile
  
  ::tree::mask::setCacheFenIndex
  
  set lMoves {}
  set w .treeWin$baseNumber
  
  $w.f.tl configure -state normal
  
  set moves [split $moves "\n"]
  
  # for the graph display
  set ::tree::treeData$baseNumber $moves
  
  set len [llength $moves]
  $w.f.tl delete 1.0 end
  
  foreach t [$w.f.tl tag names] {
    if { [ string match "tagclick*" $t ] || [ string match "tagtooltip*" $t ] } {
      $w.f.tl tag delete $t
    }
  }
  
  # Position comment
  set hasPositionComment 0
  if { $maskFile != "" } {
    set posComment [::tree::mask::getPositionComment]
    if {$posComment != ""} {
      set hasPositionComment 1
      set firstLine [ lindex [split $posComment "\n"] 0 ]
      $w.f.tl insert end "$firstLine\n" [ list bluefg tagtooltip_poscomment ]
      ::utils::tooltip::SetTag $w.f.tl $posComment tagtooltip_poscomment
      $w.f.tl tag bind tagtooltip_poscomment <Double-Button-1> "::tree::mask::addComment"
    }
  }
  
  # Display the first line
  if { $maskFile != "" } {
    $w.f.tl image create end -image tb_empty -align center
    $w.f.tl image create end -image tb_empty -align center
    $w.f.tl insert end "    "
    $w.f.tl tag bind tagclick0 <ButtonPress-$::MB3> "::tree::mask::contextMenu $w.f.tl dummy %x %y %X %Y ; break"
  }
  $w.f.tl insert end "[lindex $moves 0]\n" tagclick0
  
  for { set i 1 } { $i < [expr $len - 3 ] } { incr i } {
    set line [lindex $moves $i]
    if {$line == ""} { continue }
    set move [lindex $line 1]
    set move [::untrans $move]
    lappend lMoves $move
    set colorScore [::tree::getColorScore $line]
    if { $move == "\[end\]" } { set colorScore "" }
    
    set tagfg ""
    
    if { $maskFile != "" && $i > 0 && $i < [expr $len - 3] } {
      if { [::tree::mask::moveExists $move] } {
        set tagfg "bluefg"
      }
    }
    if { $maskFile != "" } {
      if { $i > 0 && $i < [expr $len - 3] && $move != "\[end\]" } {
        # images
        foreach j { 0 1 } {
          set img [::tree::mask::getImage $move $j]
          $w.f.tl image create end -image $img -align center
        }
        # color tag
        $w.f.tl tag configure color$i -background [::tree::mask::getColor $move]
        $w.f.tl insert end "  " color$i
        # NAG tag
        $w.f.tl insert end [::tree::mask::getNag $move]
      } else  {
        $w.f.tl image create end -image tb_empty -align center
        $w.f.tl image create end -image tb_empty -align center
        $w.f.tl insert end "    "
      }
    }
    
    # Move and stats
    if {[expr $i % 2] && $i < [expr $len -3] } {
      $w.f.tl insert end "$line" [list greybg $tagfg tagtooltip$i]
    } else  {
      $w.f.tl insert end "$line" [list whitebg $tagfg tagtooltip$i]
    }
    if {$colorScore != ""} {
      $w.f.tl tag add $colorScore end-31c end-26c
    }
    if {$move != "" && $move != "---" && $move != "\[end\]" && $i != [expr $len -2] && $i != 0} {
      $w.f.tl tag bind tagclick$i <Button-1> "[list ::tree::selectCallback $baseNumber $move ] ; break"
    }
    
    if { $maskFile != "" } {
      # Move comment
      set comment [::tree::mask::getComment $move]
      if {$comment != ""} {
        set firstLine [ lindex [split $comment "\n"] 0 ]
        $w.f.tl insert end " $firstLine" tagtooltip$i
        ::utils::tooltip::SetTag $w.f.tl $comment tagtooltip$i
        $w.f.tl tag bind tagtooltip$i <Double-Button-1> "::tree::mask::addComment $move"
      }
    }
    
    if { $maskFile != "" } {
      # Bind right button to popup a contextual menu:
      $w.f.tl tag bind tagclick$i <ButtonPress-$::MB3> "::tree::mask::contextMenu $w.f.tl $move %x %y %X %Y ; break"
    }
    $w.f.tl tag add tagclick$i [expr $i +1 + $hasPositionComment].0 [expr $i + 1 + $hasPositionComment].end
    
    $w.f.tl insert end "\n"
    
  } ;# end for loop
  
  # Display the last lines (total)
  for { set i [expr $len - 3 ] } { $i < [expr $len - 1 ] } { incr i } {
    if { $maskFile != "" } {
      $w.f.tl image create end -image tb_empty -align center
      $w.f.tl image create end -image tb_empty -align center
      $w.f.tl insert end "    "
    }
    $w.f.tl insert end "[lindex $moves $i]\n"
  }
  
  # Add moves present in Mask and not in Tree
  set idx $len
  if { $maskFile != "" } {
    set movesMask [::tree::mask::getAllMoves]
    foreach m $movesMask {
      if {  [ scan [$w.f.tl index end] "%d.%d" currentLine dummy] != 2 } {
        puts "ERROR scan index end [$w.f.tl index end]"
      }
      # move nag color move_anno
      if {[lsearch $lMoves [lindex $m 0]] != -1 || [lindex $m 0] == "null"} {
        continue
      }
      
      $w.f.tl tag bind tagclick$idx <Button-1> "[list ::tree::selectCallback $baseNumber [lindex $m 0] ] ; break"
      # images
      foreach j {4 5} {
        if {[lindex $m $j] == ""} {
          $w.f.tl image create end -image tb_empty -align center
        } else  {
          $w.f.tl image create end -image [lindex $m $j] -align center
        }
      }
      
      # color tag
      $w.f.tl tag configure color$idx -background [lindex $m 2]
      $w.f.tl insert end "  " color$idx
      # NAG tag
      $w.f.tl insert end [::tree::mask::getNag [lindex $m 0]]
      # move
      $w.f.tl insert end "[::trans [lindex $m 0] ]" bluefg
      # comment
      set comment [lindex $m 3]
      set firstLine [ lindex [split $comment "\n"] 0 ]
      $w.f.tl insert end " $firstLine\n" tagtooltip$idx
      ::utils::tooltip::SetTag $w.f.tl $comment tagtooltip$idx
      
      # Bind right button to popup a contextual menu:
      $w.f.tl tag bind tagclick$idx <ButtonPress-$::MB3> "::tree::mask::contextMenu $w.f.tl  [lindex $m 0] %x %y %X %Y ; break"
      $w.f.tl tag add tagclick$idx [ expr $currentLine -1].0 [ expr $currentLine -1].end
      incr idx
    }
  }
  
  $w.f.tl configure -state disabled
}
################################################################################
# returns a list with (ngames freq success eloavg perf) or
# {} if there was a problem during parsing
# 1: e4     B00     37752: 47.1%   54.7%  2474  2513  2002   37%
################################################################################
proc ::tree::getLineValues { l } {
  set ret {}
  if {[scan [string range $l 14 24] "%d:" ngames] != 1} {
    return {}
  } else  {
    lappend ret $ngames
  }
  
  if {[scan [string range $l 25 29] "%f%%" freq] != 1} {
    return {}
  } else  {
    lappend ret $freq
  }
  
  if {[scan [string range $l 33 37] "%f%%" success] != 1} {
    return {}
  } else  {
    lappend ret $success
  }
  
  if {[scan [string range $l 40 44] "%d" eloavg] != 1} {
    return {}
  } else  {
    lappend ret $eloavg
  }
  
  if {[scan [string range $l 46 50] "%d" perf] != 1} {
    return {}
  } else  {
    lappend ret $perf
  }
  
  return $ret
}
################################################################################
# returns the color to use for score (red, green) or ""
################################################################################
proc ::tree::getColorScore { line } {
  set data [::tree::getLineValues $line]
  if { $data == {} } { return "" }
  set ngames [lindex $data 0]
  set freq [lindex $data 1]
  set success [lindex $data 2]
  set eloavg [lindex $data 3]
  set perf [lindex $data 4]
  if { $ngames < $::tree::scoreHighlight_MinGames } {
    return ""
  }
  set wavg [ expr 50 + $::tree::scoreHighlight_WhiteExpectedScoreBonus ]
  set bavg [ expr 50 - $::tree::scoreHighlight_WhiteExpectedScoreBonus ]
  if { [sc_pos side] == "white" && $success > [ expr $wavg + $::tree::scoreHighlight_Margin ] || \
        [sc_pos side] == "black" && $success < [ expr $wavg - $::tree::scoreHighlight_Margin ] } {
    return greenfg
  }
  if { [sc_pos side] == "white" && $success < [ expr $wavg - $::tree::scoreHighlight_Margin ] || \
        [sc_pos side] == "black" && $success > [ expr $wavg + $::tree::scoreHighlight_Margin ] } {
    return redfg
  }
  return ""
}
################################################################################
proc ::tree::status { msg baseNumber } {
  global tree

  set w .treeWin$baseNumber
  if { $tree(status$baseNumber) == "" } {
    catch {sc_base filename $baseNumber} tree(status$baseNumber)
    set tree(status$baseNumber) [file tail $tree(status$baseNumber)]
    ::setTitle $w "Scid: [tr WindowsTree] $baseNumber: $tree(status$baseNumber)"
  }

  if {$msg != ""} {
    set status $msg
  } else {
    set status "  $::tr(Database) $baseNumber: $tree(status$baseNumber)"
    if {$tree(locked$baseNumber)} { append status " ($::tr(TreeLocked))" }
    if {! $tree(allgames$baseNumber)} {
      append status "   $::tr(Filter)"
      append status ": [::windows::gamelist::filterText "" $baseNumber]"
    }
  }
  $w.status configure -text $status
}

################################################################################
set tree(standardLines) {
  {}
  {1.c4}
  {1.c4 c5}
  {1.c4 c5 2.Nf3}
  {1.c4 e5}
  {1.c4 Nf6}
  {1.c4 Nf6 2.Nc3}
  {1.d4}
  {1.d4 d5}
  {1.d4 d5 2.c4}
  {1.d4 d5 2.c4 c6}
  {1.d4 d5 2.c4 c6 3.Nf3}
  {1.d4 d5 2.c4 c6 3.Nf3 Nf6}
  {1.d4 d5 2.c4 c6 3.Nf3 Nf6 4.Nc3}
  {1.d4 d5 2.c4 c6 3.Nf3 Nf6 4.Nc3 dxc4}
  {1.d4 d5 2.c4 c6 3.Nf3 Nf6 4.Nc3 e6}
  {1.d4 d5 2.c4 c6 3.Nf3 Nf6 4.Nc3 e6 5.e3}
  {1.d4 d5 2.c4 e6}
  {1.d4 d5 2.c4 e6 3.Nc3}
  {1.d4 d5 2.c4 e6 3.Nc3 Nf6}
  {1.d4 d5 2.c4 e6 3.Nf3}
  {1.d4 d5 2.c4 dxc4}
  {1.d4 d5 2.c4 dxc4 3.Nf3}
  {1.d4 d5 2.c4 dxc4 3.Nf3 Nf6}
  {1.d4 d5 2.Nf3}
  {1.d4 d5 2.Nf3 Nf6}
  {1.d4 d5 2.Nf3 Nf6 3.c4}
  {1.d4 d6}
  {1.d4 d6 2.c4}
  {1.d4 Nf6}
  {1.d4 Nf6 2.c4}
  {1.d4 Nf6 2.c4 c5}
  {1.d4 Nf6 2.c4 d6}
  {1.d4 Nf6 2.c4 e6}
  {1.d4 Nf6 2.c4 e6 3.Nc3}
  {1.d4 Nf6 2.c4 e6 3.Nc3 Bb4}
  {1.d4 Nf6 2.c4 e6 3.Nf3}
  {1.d4 Nf6 2.c4 g6}
  {1.d4 Nf6 2.c4 g6 3.Nc3}
  {1.d4 Nf6 2.c4 g6 3.Nc3 Bg7}
  {1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4}
  {1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6}
  {1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6 5.Nf3}
  {1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6 5.Nf3 O-O}
  {1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4 d6 5.Nf3 O-O 6.Be2}
  {1.d4 Nf6 2.c4 g6 3.Nf3}
  {1.d4 Nf6 2.Bg5}
  {1.d4 Nf6 2.Bg5 Ne4}
  {1.d4 Nf6 2.Nf3}
  {1.d4 Nf6 2.Nf3 e6}
  {1.d4 Nf6 2.Nf3 g6}
  {1.e4}
  {1.e4 c5}
  {1.e4 c5 2.c3}
  {1.e4 c5 2.c3 d5}
  {1.e4 c5 2.c3 Nf6}
  {1.e4 c5 2.Nc3}
  {1.e4 c5 2.Nc3 Nc6}
  {1.e4 c5 2.Nf3}
  {1.e4 c5 2.Nf3 d6}
  {1.e4 c5 2.Nf3 d6 3.d4}
  {1.e4 c5 2.Nf3 d6 3.d4 cxd4}
  {1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4}
  {1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6}
  {1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3}
  {1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 a6}
  {1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 e6}
  {1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 g6}
  {1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 Nc6}
  {1.e4 c5 2.Nf3 d6 3.Bb5+}
  {1.e4 c5 2.Nf3 e6}
  {1.e4 c5 2.Nf3 Nc6}
  {1.e4 c5 2.Nf3 Nc6 3.d4}
  {1.e4 c5 2.Nf3 Nc6 3.Bb5}
  {1.e4 c6}
  {1.e4 c6 2.d4}
  {1.e4 c6 2.d4 d5}
  {1.e4 c6 2.d4 d5 3.e5}
  {1.e4 c6 2.d4 d5 3.Nc3}
  {1.e4 c6 2.d4 d5 3.Nd2}
  {1.e4 d5}
  {1.e4 d6}
  {1.e4 d6 2.d4}
  {1.e4 d6 2.d4 Nf6}
  {1.e4 d6 2.d4 Nf6 3.Nc3}
  {1.e4 e5}
  {1.e4 e5 2.Nf3}
  {1.e4 e5 2.Nf3 Nc6}
  {1.e4 e5 2.Nf3 Nc6 3.d4}
  {1.e4 e5 2.Nf3 Nc6 3.Bb5}
  {1.e4 e5 2.Nf3 Nc6 3.Bb5 a6}
  {1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4}
  {1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6}
  {1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4 Nf6 5.O-O}
  {1.e4 e5 2.Nf3 Nc6 3.Bc4}
  {1.e4 e5 2.Nf3 Nf6}
  {1.e4 e6}
  {1.e4 e6 2.d4}
  {1.e4 e6 2.d4 d5}
  {1.e4 e6 2.d4 d5 3.Nc3}
  {1.e4 e6 2.d4 d5 3.Nc3 Bb4}
  {1.e4 e6 2.d4 d5 3.Nc3 Nf6}
  {1.e4 e6 2.d4 d5 3.Nd2}
  {1.e4 e6 2.d4 d5 3.Nd2 c5}
  {1.e4 e6 2.d4 d5 3.Nd2 Nf6}
  {1.e4 Nf6}
  {1.e4 Nf6 2.e5}
  {1.e4 Nf6 2.e5 Nd5}
  {1.Nf3}
  {1.Nf3 Nf6}
}
# if there is a treecache file source it, otherwise use hard coded
# values above
catch {source [scidConfigFile treecache]}


################################################################################
# ::tree::best
#   Open/Close the window of best (highest-rated) tree games.
#
proc ::tree::best { baseNumber } {
  set w .treeBest$baseNumber
  if {[winfo exists $w]} {
    destroy $w
  } else {
    .treeWin$baseNumber.buttons.best state pressed
    ::windows::gamelist::OpenTreeBest $::tree(base$baseNumber) $w
    bind $w <Destroy> "+.treeWin$baseNumber.buttons.best state !pressed"
  }
}

################################################################################
# ::tree::graphRedraw
#   Redraws the tree graph window.
#
proc ::tree::graphRedraw { baseNumber } {
  .treeGraph$baseNumber.c itemconfigure text -width [expr {[winfo width .treeGraph$baseNumber.c] - 50}]
  .treeGraph$baseNumber.c coords text [expr {[winfo width .treeGraph$baseNumber.c] / 2}] 10
  ::utils::graph::configure tree$baseNumber -height [expr {[winfo height .treeGraph$baseNumber.c] - 100}]
  ::utils::graph::configure tree$baseNumber -width [expr {[winfo width .treeGraph$baseNumber.c] - 50}]
  ::utils::graph::redraw tree$baseNumber
}

################################################################################
# ::tree::graph
#   Updates the tree graph window, creating it if necessary.
#   bpress: the button/menu was selected => bring window to front
#
proc ::tree::graph { baseNumber {bpress 0}} {
  set w .treeGraph$baseNumber
  if {! [winfo exists .treeWin$baseNumber]} { return }
  if {! [winfo exists $w]} {
    toplevel $w
    setWinLocation $w
    bind $w <Escape> "destroy $w"
    bind $w <F1> {helpWindow Tree Graph}
    
    menu $w.menu
    ::setMenu $w $w.menu
    $w.menu add cascade -label GraphFile -menu $w.menu.file
    menu $w.menu.file
    $w.menu.file add command -label GraphFileColor -command "::tools::graphs::Save color $w.c"
    $w.menu.file add command -label GraphFileGrey -command "::tools::graphs::Save gray $w.c"
    $w.menu.file add separator
    $w.menu.file add command -label GraphFileClose -command "destroy $w"
    
    canvas $w.c -width 500 -height 300 -selectforeground [ttk::style lookup . -foreground] -background [ttk::style lookup . -background]
    pack $w.c -side top -fill both -expand yes
    $w.c create text 25 10 -tag text -justify center -width 1 -font font_Regular -anchor n
    update
    bind $w <Configure> "::tree::graphRedraw $baseNumber"
    bind $w.c <Button-1> "::tree::graph $baseNumber"
    ::setTitle $w "Scid: Tree Graph $baseNumber: [file tail [sc_base filename $baseNumber]]"
    # wm minsize $w 300 200
    keyboardShortcuts $w
    ::tree::configGraphMenus "" $baseNumber
  } elseif {$bpress == 1} {
    focus $w
    raise $w
  }

  $w.c itemconfigure text -width [expr {[winfo width $w.c] - 50}]
  $w.c coords text [expr {[winfo width $w.c] / 2}] 10
  set height [expr {[winfo height $w.c] - 100}]
  set width [expr {[winfo width $w.c] - 50}]
  ::utils::graph::create tree$baseNumber -width $width -height $height -xtop 25 -ytop 60 \
      -xmin 0.5 -xtick 1 -ytick 5 -font font_Small -canvas $w.c
  
  set data {}
  set xlabels {}
  set othersCount 0
  set numOthers 0
  set othersName "..."
  set count 0
  set othersScore 0.0
  set mean 50.0
  set totalGames 0
  set treeData [subst $[subst {::tree::treeData$baseNumber} ] ]
  # [.treeWin$baseNumber.f.tl get 0 end]
  
  set numTreeLines [llength $treeData]
  set totalLineIndex [expr $numTreeLines - 2]
  
  for {set i 0} {$i < [llength $treeData]} {incr i} {
    # Extract info from each line of the tree window:
    # Note we convert "," decimal char back to "." where necessary.
    set line [lindex $treeData $i]
    set mNum [string trim [string range $line  0  1]]
    set freq [string trim [string range $line 17 23]]
    set fpct [string trim [string range $line 25 29]]
    regsub -all {,} $fpct . fpct
    set move [string trim [string range $line  4 9]]
    set score [string trim [string range $line 33 37]]
    regsub -all {,} $score . score
    if {$score > 99.9} { set score 99.9 }
    # Check if this line is "TOTAL:" line:
    if {$i == $totalLineIndex} {
      set mean $score
      set totalGames $freq
    }
    # Add info for this move to the graph if necessary:
    if {[string index $line 2] == ":"  &&  [string compare "<end>" $move]} {
      if {$fpct < 1.0  ||  $freq < 5  ||  $i > 5} {
        incr othersCount $freq
        incr numOthers
        set othersScore [expr {$othersScore + (double($freq) * $score)}]
        set m $move
        if {$numOthers > 1} { set m "..." }
      } else {
        incr count
        lappend data $count
        lappend data $score
        lappend xlabels [list $count "$move ([expr round($score)]%)\n$freq: [expr round($fpct)]%"]
      }
    }
  }
  
  # Add extra bar for other moves if necessary:
  if {$numOthers > 0  &&  $totalGames > 0} {
    incr count
    set fpct [expr {double($othersCount) * 100.0 / double($totalGames)}]
    set sc [expr {round($othersScore / double($othersCount))}]
    set othersName "$m ($sc%)\n$othersCount: [expr round($fpct)]%"
    lappend data $count
    lappend data [expr {$othersScore / double($othersCount)}]
    lappend xlabels [list $count $othersName]
  }
  
  # Plot fake bounds data so graph at least shows range 40-65:
  ::utils::graph::data tree$baseNumber bounds -points 0 -lines 0 -bars 0 -coords {1 41 1 64}
  
  # Replot the graph:
  ::utils::graph::data tree$baseNumber data -color red -points 0 -lines 0 -bars 1 \
      -barwidth 0.75 -outline black -coords $data
  ::utils::graph::configure tree$baseNumber -xlabels $xlabels -xmax [expr {$count + 0.5}] \
      -hline [list {gray80 1 each 5} {gray50 1 each 10} {black 2 at 50} \
      {black 1 at 55} [list red 2 at $mean]] \
      -brect [list [list 0.5 55 [expr {$count + 0.5}] 50 LightSkyBlue1]]
  
  ::utils::graph::redraw tree$baseNumber
  set moves ""
  catch {set moves [sc_game firstMoves -1]}
  if {[string length $moves] == 0} { set moves $::tr(StartPos) }
  set title "$moves ([::utils::thousands $totalGames] $::tr(games))"
  $w.c itemconfigure text -text $title
}

################################################################################
proc ::tree::configGraphMenus { lang baseNumber } {
  if {! [winfo exists .treeGraph$baseNumber]} { return }
  if {$lang == ""} { set lang $::language }
  set m .treeGraph$baseNumber.menu
  foreach idx {0} tag {File} {
    configMenuText $m $idx Graph$tag $lang
  }
  foreach idx {0 1 3} tag {Color Grey Close} {
    configMenuText $m.file $idx GraphFile$tag $lang
  }
}

# ################################################################################
proc ::tree::toggleRefresh { baseNumber } {
  global tree
  set b .treeWin$baseNumber.buttons.bStartStop
  
  if {$tree(autorefresh$baseNumber)} {
    $b configure -image tb_search_off
    set tree(autorefresh$baseNumber) 0
  } else  {
    $b configure -image tb_search_on
    set tree(autorefresh$baseNumber) 1
    ::tree::refresh $baseNumber
  }
}
################################################################################
#
################################################################################
proc ::tree::setCacheSize { base size } {
  sc_tree cachesize $base $size
}
################################################################################
#
################################################################################


################################################################################
#
#                                 Mask namespace
#
#  All function calls with move in english
#  Images are 17x17
################################################################################
namespace eval ::tree::mask {
  
  # mask(fen) contains data for a position <fen> : ( moves, comment )
  # where moves is ( move nag color move_anno img1 img2 )
  array set mask {}
  set maskSerialized {}
  set maskFile ""
  set defaultColor white
  set emptyNag "  "
  set textComment ""
  set cacheFenIndex -1
  set dirty 0 ; # if Mask data has changed
  # Mask Search
  set searchMask_usenag 0
  set searchMask_usemarker0 0
  set searchMask_usemarker1 0
  set searchMask_usecolor 0
  set searchMask_usemovecomment 0
  set searchMask_useposcomment 0
  set displayMask_showNag 1
  set displayMask_showComment 1
  
  array set marker2image { Include tb_tick Exclude tb_cross MainLine tb_mainline Bookmark tb_bkm \
        White tb_white Black tb_black \
        NewLine tb_new ToBeVerified tb_rfilter ToTrain tb_msearch Dubious tb_help_small ToRemove tb_cut }
  set maxRecent 10
}
################################################################################
#
################################################################################
proc ::tree::mask::open { {filename ""} } {
  global ::tree::mask::maskSerialized ::tree::mask::mask ::tree::mask::recentMask
  
  if {$filename == ""} {
    set types {
      {{Tree Mask Files}       {.stm}        }
    }
    set filename [tk_getOpenFile -initialdir $::initialDir(stm) -filetypes $types -defaultextension ".stm"]
	 set ::initialDir(stm) [file dirname $filename]
  }
  
  if {$filename != ""} {
    ::tree::mask::askForSave
    array unset ::tree::mask::mask
    array set ::tree::mask::mask {}
    source $filename
    array set mask $maskSerialized
    set maskSerialized {}
    set ::tree::mask::maskFile $filename
    set ::tree::mask::dirty 0
    ::tree::refresh
    
    if { [lsearch $recentMask $filename ] == -1 } {
      set recentMask [ linsert $recentMask 0 $filename]
      if {[llength $recentMask] > $::tree::mask::maxRecent } {
        set recentMask [ lreplace $recentMask  [ expr $::tree::mask::maxRecent -1 ] end ]
      }
      
      # update recent masks menu entry
      foreach i [sc_base list] {
        set w .treeWin$i
        if { [winfo exists $w] } {
          $w.menu.mask.recent delete 0 end
          foreach f $::tree::mask::recentMask {
            $w.menu.mask.recent add command -label $f -command "::tree::mask::open $f"
          }
        }
      }
      
    }
  }
  
}
################################################################################
#
################################################################################
proc ::tree::mask::askForSave {} {
  if {$::tree::mask::dirty} {
    set answer [tk_messageBox -title Scid -icon warning -type yesno \
        -message "[ tr DoYouWantToSaveFirst ]\n$::tree::mask::maskFile ?"]
    if {$answer == "yes"} {
      ::tree::mask::save
    }
  }
}
################################################################################
#
################################################################################
proc ::tree::mask::new {} {
  
  set types {
    {{Tree Mask Files}       {.stm}        }
  }
  set filename [tk_getSaveFile -filetypes $types -defaultextension ".stm"]
  
  if {$filename != ""} {
    if {[file extension $filename] != ".stm" } {
      append filename ".stm"
    }
    ::tree::mask::askForSave
    set ::tree::mask::dirty 0
    set ::tree::mask::maskFile $filename
    array unset ::tree::mask::mask
    array set ::tree::mask::mask {}
    ::tree::refresh
  }
}
################################################################################
#
################################################################################
proc ::tree::mask::close {} {
  if { $::tree::mask::maskFile == "" } {
    return
  }
  ::tree::mask::askForSave
  set ::tree::mask::dirty 0
  array unset ::tree::mask::mask
  array set ::tree::mask::mask {}
  set ::tree::mask::maskFile ""
  ::tree::refresh
}
################################################################################
#
################################################################################
proc ::tree::mask::save {} {
  set f [ ::open $::tree::mask::maskFile w ]
  puts $f "set ::tree::mask::maskSerialized [list [array get ::tree::mask::mask]]"
  ::close $f
  set ::tree::mask::dirty 0
}
################################################################################
#
################################################################################
proc ::tree::mask::contextMenu {win move x y xc yc} {
  update idletasks
  
  set mctxt $win.ctxtMenu
  if { [winfo exists $mctxt] } {
    destroy $mctxt
  }
  
  if {$move == "dummy"} {
    set state "disabled"
  } else  {
    set state "normal"
  }
  menu $mctxt
  $mctxt add command -label [tr AddToMask] -command "::tree::mask::addToMask $move" -state $state
  $mctxt add command -label [tr RemoveFromMask] -command "::tree::mask::removeFromMask $move" -state $state
  $mctxt add separator
  
  menu $mctxt.nag
  $mctxt add cascade -label [tr Nag] -menu $mctxt.nag -state $state
  
  foreach nag [ list "!!" " !" "!?" "?!" " ?" "??" " ~" [::tr "None"]  ] {
    $mctxt.nag add command -label $nag -command "::tree::mask::setNag [list $move $nag]" -state $state
  }
  
  foreach j { 0 1 } {
    menu $mctxt.image$j
    $mctxt add cascade -label "[tr Marker] [expr $j +1]" -menu $mctxt.image$j -state $state
    foreach e { Include Exclude MainLine Bookmark White Black NewLine ToBeVerified ToTrain Dubious ToRemove } {
      set i  $::tree::mask::marker2image($e)
      $mctxt.image$j add command -label [ tr $e ] -image $i -compound left -command "::tree::mask::setImage $move $i $j"
    }
    $mctxt.image$j add command -label [tr NoMarker] -command "::tree::mask::setImage $move {} $j"
  }
  menu $mctxt.color
  $mctxt add cascade -label [tr ColorMarker] -menu $mctxt.color  -state $state
  foreach c { "White" "Green" "Yellow" "Blue" "Red"} {
    $mctxt.color add command -label [ tr "${c}Mark" ] -background $c -command "::tree::mask::setColor $move $c"
  }
  
  $mctxt add separator
  $mctxt add command -label [ tr CommentMove] -command "::tree::mask::addComment $move" -state $state
  $mctxt add command -label [ tr CommentPosition] -command "::tree::mask::addComment"
  
  $mctxt add separator
  set lMatchMoves [sc_pos matchMoves ""]
  if {[llength $lMatchMoves ] > 16} {
    # split the moves in several menus
    for {set idxMenu 0} { $idxMenu <= [expr int([llength $lMatchMoves ] / 16) ]} {incr idxMenu} {
      menu $mctxt.matchmoves$idxMenu
      $mctxt add cascade -label "[ tr AddThisMoveToMask ] ([expr $idxMenu + 1 ])" -menu $mctxt.matchmoves$idxMenu
      for {set i 0} {$i < 16} {incr i} {
        if {[expr $i + $idxMenu * 16 +1] > [llength $lMatchMoves ] } {
          break
        }
        set m [lindex $lMatchMoves [expr $i + $idxMenu * 16]]
        if {$m == "OK"} { set m "O-O" }
        if {$m == "OQ"} { set m "O-O-O" }
        $mctxt.matchmoves$idxMenu add command -label [::trans $m] -command "::tree::mask::addToMask $m"
      }
    }
  } else  {
    menu $mctxt.matchmoves
    $mctxt add cascade -label [ tr AddThisMoveToMask ] -menu $mctxt.matchmoves
    foreach m [sc_pos matchMoves ""] {
      if {$m == "OK"} { set m "O-O" }
      if {$m == "OQ"} { set m "O-O-O" }
      $mctxt.matchmoves add command -label [::trans $m] -command "::tree::mask::addToMask $m"
    }
  }
  
  $mctxt post [winfo pointerx .] [winfo pointery .]
}
################################################################################
#
################################################################################
proc ::tree::mask::addToMask { move {fen ""} } {
  global ::tree::mask::mask
  
  if {$fen == ""} { set fen $::tree::mask::cacheFenIndex }
  
  if {![info exists mask($fen)]} {
    set mask($fen) { {} {} }
  }
  set ::tree::mask::dirty 1
  set moves [ lindex $mask($fen) 0 ]
  if {[lsearch $moves $move] == -1} {
    lappend moves [list $move {} $::tree::mask::defaultColor {} {} {}]
    set newpos [lreplace $mask($fen) 0 0 $moves]
    set mask($fen) $newpos
    ::tree::refresh
  }
}
################################################################################
#
################################################################################
proc ::tree::mask::removeFromMask { move {fen ""} } {
  global ::tree::mask::mask
  
  if {$fen == ""} { set fen $::tree::mask::cacheFenIndex }
  
  if {![info exists mask($fen)]} {
    return
  }
  set ::tree::mask::dirty 1
  
  set moves [ lindex $mask($fen) 0 ]
  set idxm [lsearch -regexp $moves "^$move *"]
  if { $idxm != -1} {
    set moves [lreplace $moves $idxm $idxm]
    lset mask($fen) 0 $moves
    ::tree::refresh
  }
  
  # if the position has no move left and no comment, unset it
  if { [llength [lindex $mask($fen) 0] ] == 0 && [lindex $mask($fen) 1] == "" } {
    array unset mask $fen
  }
}
################################################################################
# returns 1 if the move is already in mask
################################################################################
proc ::tree::mask::moveExists { move {fen ""} } {
  global ::tree::mask::mask
  
  if {$fen == ""} { set fen $::tree::mask::cacheFenIndex }
  
  if {![info exists mask($fen)] || $move == "\[end\]" } {
    return 0
  }
  set moves [ lindex $mask($fen) 0 ]
  if {[lsearch -regexp $moves "^$move *"] == -1} {
    return 0
  }
  return 1
}
################################################################################
# return the list of moves with their data
################################################################################
proc ::tree::mask::getAllMoves {} {
  global ::tree::mask::mask
  if {![info exists mask($::tree::mask::cacheFenIndex)]} {
    return ""
  }
  set moves [ lindex $mask($::tree::mask::cacheFenIndex) 0 ]
  return $moves
}
################################################################################
#
################################################################################
proc ::tree::mask::getColor { move {fen ""}} {
  global ::tree::mask::mask
  
  if {$fen == ""} { set fen $::tree::mask::cacheFenIndex }
  
  if {![info exists mask($fen)]} {
    return $::tree::mask::defaultColor
  }
  
  set moves [ lindex $mask($fen) 0 ]
  set idxm [lsearch -regexp $moves "^$move *"]
  if { $idxm == -1} {
    return $::tree::mask::defaultColor
  }
  set col [ lindex $moves $idxm 2 ]
  
  return $col
}
################################################################################
#
################################################################################
proc ::tree::mask::setColor { move color {fen ""}} {
  global ::tree::mask::mask
  
  if {$fen == ""} { set fen $::tree::mask::cacheFenIndex }
  
  if {![info exists mask($fen)]} {
    tk_messageBox -title "Scid" -type ok -icon warning -message [ tr AddMoveToMaskFirst ]
    return
  }
  set ::tree::mask::dirty 1
  set moves [ lindex $mask($fen) 0 ]
  set idxm [lsearch -regexp $moves "^$move *"]
  if { $idxm == -1} {
    tk_messageBox -title "Scid" -type ok -icon warning -message [ tr AddMoveToMaskFirst ]
    return
  }
  set newmove [lreplace [lindex $moves $idxm] 2 2 $color ]
  set moves [lreplace $moves $idxm $idxm $newmove ]
  set mask($fen) [ lreplace $mask($fen) 0 0 $moves ]
  ::tree::refresh
}
################################################################################
# defaults to "  " (2 spaces)
################################################################################
proc ::tree::mask::getNag { move { fen "" }} {
  global ::tree::mask::mask ::tree::mask::emptyNag
  
  if {$fen == ""} { set fen $::tree::mask::cacheFenIndex }
  
  if {![info exists mask($fen)]} {
    return $emptyNag
  }
  set moves [ lindex $mask($fen) 0 ]
  set idxm [lsearch -regexp $moves "^$move *"]
  if { $idxm == -1} {
    return $emptyNag
  }
  set nag [ lindex $moves $idxm 1 ]
  if {$nag == ""} {
    set nag $emptyNag
  }
  if { [string length $nag] == 1} { set nag " $nag" }
  return $nag
}
################################################################################
#
################################################################################
proc ::tree::mask::setNag { move nag {fen ""} {refresh 1} } {
  global ::tree::mask::mask
  
  if { $nag == [::tr "None"] } {
    set nag ""
  }
  
  if {$fen == ""} { set fen $::tree::mask::cacheFenIndex }
  
  if {![info exists mask($fen)]} {
    tk_messageBox -title "Scid" -type ok -icon warning -message [ tr AddMoveToMaskFirst ]
    return
  }
  set ::tree::mask::dirty 1
  set moves [ lindex $mask($fen) 0 ]
  set idxm [lsearch -regexp $moves "^$move *"]
  if { $idxm == -1} {
    tk_messageBox -title "Scid" -type ok -icon warning -message [ tr AddMoveToMaskFirst ]
    return
  }
  set newmove [lreplace [lindex $moves $idxm] 1 1 $nag ]
  set moves [lreplace $moves $idxm $idxm $newmove ]
  set mask($fen) [ lreplace $mask($fen) 0 0 $moves ]
  if {$refresh} { ::tree::refresh }
}
################################################################################
#
################################################################################
proc ::tree::mask::getComment { move { fen "" } } {
  global ::tree::mask::mask
  
  if {$fen == ""} { set fen $::tree::mask::cacheFenIndex }
  
  if {![info exists mask($fen)] || $move == "" || $move == "\[end\]" } {
    return ""
  }
  
  set moves [ lindex $mask($fen) 0 ]
  set idxm [lsearch -regexp $moves "^$move *"]
  if { $idxm == -1} {
    return ""
  }
  set comment [ lindex $moves $idxm 3 ]
  if {$comment == ""} {
    set comment "  "
  }
  return $comment
}
################################################################################
#
################################################################################
proc ::tree::mask::setComment { move comment { fen "" } } {
  global ::tree::mask::mask
  
  if {$fen == ""} { set fen $::tree::mask::cacheFenIndex }
  
  set comment [string trim $comment]
  
  if {![info exists mask($fen)]} {
    tk_messageBox -title "Scid" -type ok -icon warning -message [ tr AddMoveToMaskFirst ]
    return
  }
  set ::tree::mask::dirty 1
  set moves [ lindex $mask($fen) 0 ]
  set idxm [lsearch -regexp $moves "^$move *"]
  if { $idxm == -1} {
    tk_messageBox -title "Scid" -type ok -icon warning -message [ tr AddMoveToMaskFirst ]
    return
  }
  set newmove [lreplace [lindex $moves $idxm] 3 3 $comment ]
  set moves [lreplace $moves $idxm $idxm $newmove ]
  set mask($fen) [ lreplace $mask($fen) 0 0 $moves ]
}
################################################################################
#
################################################################################
proc ::tree::mask::getPositionComment {{fen ""}} {
  global ::tree::mask::mask
  
  if {$fen == ""} { set fen $::tree::mask::cacheFenIndex }
  
  if { ! [ info exists mask($fen) ] } {
    return ""
  }
  
  set comment [ lindex $mask($fen) 1 ]
  set comment [ string trim $comment ]
  
  return $comment
}
################################################################################
#
################################################################################
proc ::tree::mask::setPositionComment { comment {fen ""} } {
  global ::tree::mask::mask
  
  if {$fen == ""} { set fen $::tree::mask::cacheFenIndex }
  set comment [ string trim $comment ]
  set ::tree::mask::dirty 1
  # add position automatically
  if {![info exists mask($fen)]} {
    set mask($fen) { {} {} }
  }
  
  set newpos [ lreplace $mask($fen) 1 1 $comment ]
  set mask($fen) $newpos
}
################################################################################
#
################################################################################
proc ::tree::mask::setImage { move img nmr } {
  global ::tree::mask::mask
  set fen $::tree::mask::cacheFenIndex
  if {![info exists mask($fen)]} {
    tk_messageBox -title "Scid" -type ok -icon warning -message [ tr AddMoveToMaskFirst ]
    return
  }
  set ::tree::mask::dirty 1
  set moves [ lindex $mask($fen) 0 ]
  set idxm [lsearch -regexp $moves "^$move *"]
  if { $idxm == -1} {
    tk_messageBox -title "Scid" -type ok -icon warning -message [ tr AddMoveToMaskFirst ]
    return
  }
  set loc [expr 4 + $nmr]
  set newmove [lreplace [lindex $moves $idxm] $loc $loc $img ]
  set moves [lreplace $moves $idxm $idxm $newmove ]
  set mask($fen) [ lreplace $mask($fen) 0 0 $moves ]
  
  ::tree::refresh
}
################################################################################
# nmr = 0 or 1 (two images per line)
################################################################################
proc ::tree::mask::getImage { move nmr } {
  global ::tree::mask::mask
  
  set fen $::tree::mask::cacheFenIndex
  if {![info exists mask($fen)]} {
    return tb_empty
  }
  set moves [ lindex $mask($fen) 0 ]
  set idxm [lsearch -regexp $moves "^$move *"]
  if { $idxm == -1} {
    return tb_empty
  }
  set loc [expr 4 + $nmr]
  set img [lindex $moves $idxm $loc]
  if {$img == ""} { set img tb_empty }
  return $img
}

################################################################################
# if move is null, this is a position comment
################################################################################
proc ::tree::mask::addComment { { move "" } } {
  
  # first check the move is present in Mask
  if { $move != "" } {
    if { ![::tree::mask::moveExists $move] } {
      tk_messageBox -title "Scid" -type ok -icon warning -message [ tr AddMoveToMaskFirst ]
      return
    }
  }
  set w .treeMaskAddComment
  toplevel .treeMaskAddComment
  if {$move == ""} {
    set oldComment [::tree::mask::getPositionComment]
    ::setTitle $w [::tr CommentPosition]
  } else  {
    set oldComment [::tree::mask::getComment $move ]
    ::setTitle $w [::tr CommentMove]
  }
  set oldComment [ string trim $oldComment ]
  autoscrollframe $w.f text $w.f.e -width 40 -height 5 -wrap word -setgrid 1
  $w.f.e insert end $oldComment
  ttk::button $w.ok -text OK -command "::tree::mask::updateComment $move ; destroy $w ; ::tree::refresh"
  pack  $w.f  -side top -expand 1 -fill both
  pack  $w.ok -side bottom
  focus $w.f.e
}
################################################################################
#
################################################################################
proc ::tree::mask::updateComment { { move "" } } {
  set e .treeMaskAddComment.f.e
  set newComment [$e get 1.0 end]
  set newComment [ string trim $newComment ]
  set ::tree::mask::dirty 1
  if {$move == ""} {
    ::tree::mask::setPositionComment $newComment
  } else  {
    ::tree::mask::setComment $move $newComment
  }
  ::tree::refresh
}
################################################################################
#
################################################################################
proc ::tree::mask::fillWithBase {} {
  if {$::tree::mask::maskFile == ""} {
    tk_messageBox -title "Scid" -type ok -icon warning -message [ tr OpenAMaskFileFirst]
    return
  }

  set n [sc_base numGames $::curr_db]
  progressWindow "Scid" "[tr TreeMaskFillWithBase]" $::tr(Stop)
  for {set gnum 1} { $gnum <= $n} {incr gnum} {
    if {[catch { updateProgressWindow $gnum $n }]} { break }
    ::tree::mask::fillWithGame $::curr_db $gnum 0
  }
  closeProgressWindow
  ::notify::PosChanged
}
################################################################################
#
################################################################################
proc ::tree::mask::fillWithGame { {base ""} {gnum ""} {refresh 1} } {
  if {$::tree::mask::maskFile == ""} {
    tk_messageBox -title "Scid" -type ok -icon warning -message [ tr OpenAMaskFileFirst]
    return
  }

  if {$base == ""} {
    set base [sc_base current]
    set gnum [sc_game number]
  }

  set lastRAVd 0
  set lastRAVn 0
  set iFEN(0) 0
  set iFENvar(0) 0
  set game [sc_base getGame $base $gnum]
  set n [llength $game]
  for {set i 0} { $i < $n} {incr i} {
    # Quick assign
    foreach {RAVd RAVn FEN NAGs comment lastMoveSAN} [lindex $game $i] {}

    if { $RAVd > $lastRAVd || ($RAVd == $lastRAVd && $RAVn != $lastRAVn) } {
      # New variation
      set parent [expr { $RAVd - 1 }]
      set iFEN($RAVd) $iFENrav($parent)
    }

    set fromFEN [lindex [lindex $game $iFEN($RAVd)] 2]
    ::tree::mask::feedMask "$fromFEN" "$lastMoveSAN" "$NAGs" "$comment"

    set lastRAVd $RAVd
    set lastRAVn $RAVn
    set iFENrav($RAVd) $iFEN($RAVd)
    set iFEN($RAVd) $i
  }

  set ::tree::mask::dirty 1
  if {$refresh} { ::notify::PosChanged }
}
################################################################################
# Take current position information and fill the mask (move, nag, comments, etc)
################################################################################
proc ::tree::mask::feedMask { fen move nag comment } {
  global ::tree::mask::mask

  set stdNags { "!!" "!" "!?" "?!" "??" "~"}
  set fen [toShortFen $fen]
  
  if {$move == ""} {
    set move "null"
  }

  # add move if not in mask
  if { ![moveExists $move $fen]} {
    if {![info exists mask($fen)]} {
      set mask($fen) { {} {} }
    }
    set moves [ lindex $mask($fen) 0 ]
    if {[lsearch $moves $move] == -1} {
      lappend moves [list $move {} $::tree::mask::defaultColor {} {} {}]
      set newpos [lreplace $mask($fen) 0 0 $moves]
      set mask($fen) $newpos
    }
  }

  if {$move == "null"} {
    set comment "$comment [getPositionComment]"
    setPositionComment $comment $fen
    return
  }
  
  # NAG
  if {$nag == 0} { set nag "" }
  if {$nag != ""} {
    # append the NAGs to comment if not standard
    if {[lsearch $stdNags $nag ] == -1 } {
      set comment "$nag $comment"
      set nag ""
    } else  {
      set oldNag [getNag $move]
      if {$oldNag != $::tree::mask::emptyNag && $oldNag != $nag} {
        set comment "<$oldNag>(?!?) $comment"
      }
      setNag $move $nag $fen 0
    }
  }

  # append comment
  set oldComment [getComment $move $fen]
  if { $oldComment != "" && $oldComment != $comment } {
    set comment "$oldComment\n$comment"
  }
  setComment $move $comment $fen
}
################################################################################
#  trim the fen to keep position data only
################################################################################
proc ::tree::mask::toShortFen {fen} {
  set ret [lreplace $fen end-1 end]
  return $ret
}
################################################################################
#
################################################################################
proc ::tree::mask::setCacheFenIndex {} {
  set ::tree::mask::cacheFenIndex [ toShortFen [sc_pos fen] ]
}
################################################################################
#
################################################################################
proc ::tree::mask::infoMask {} {
  global ::tree::mask::mask
  
  set npos [array size mask]
  # set nmoves 0
  set nmoves [lindex [ split [array statistics mask] "\n" ] end ]
  # foreach pos $mask {
  # incr nmoves [llength [lindex $pos 1]]
  # }
  tk_messageBox -title "Mask info" -type ok -icon info -message "Mask : $::tree::mask::maskFile\n[tr Positions] : $npos\n[tr Moves] : $nmoves"
}
################################################################################
# Dumps mask content in a tree view widget
# The current position is the reference base
################################################################################
proc ::tree::mask::displayMask {} {
  global ::tree::mask::mask
  
  set w .displaymask
  if { [winfo exists $w] } {
    focus $w
    return
  }
  toplevel $w
  wm title $w [::tr DisplayMask]
  setWinLocation $w
  setWinSize $w
  
  ttk::frame $w.f
  pack $w.f -fill both -expand 1

  ttk::frame $w.fcb
  pack $w.fcb -fill x
  ttk::button $w.fcb.bupdate -text [::tr "Update"] -command ::tree::mask::updateDisplayMask
  ttk::checkbutton $w.fcb.nag -text [::tr "Nag"] -variable ::tree::mask::displayMask_showNag -command ::tree::mask::updateDisplayMask
  ttk::checkbutton $w.fcb.comment -text [::tr "Comments"] -variable ::tree::mask::displayMask_showComment -command ::tree::mask::updateDisplayMask
  pack $w.fcb.bupdate $w.fcb.nag $w.fcb.comment -side right -padx 5 -pady "2 5"
  
  ttk::treeview $w.f.tree -yscrollcommand "$w.f.ybar set" -xscrollcommand "$w.f.xbar set" -show tree -selectmode browse
  # workaround for a bug in treeview (xscrollbar does not get view size)
  $w.f.tree column #0 -minwidth 1200
  ttk::scrollbar $w.f.xbar -command "$w.f.tree xview" -orient horizontal
  ttk::scrollbar $w.f.ybar -command "$w.f.tree yview"
  
  pack $w.f.xbar -side bottom -fill x
  pack $w.f.ybar -side right -fill y
  pack $w.f.tree -side left -expand 1 -fill both
  
  updateDisplayMask
  
  bind $w <Escape> { destroy  .displaymask }
  bind $w <Configure>  {
    recordWinSize .displaymask
  }
  
  $w.f.tree tag bind dblClickTree <Double-Button-1> {::tree::mask::maskTreeUnfold }
}
################################################################################
#
################################################################################
proc ::tree::mask::updateDisplayMask {} {
  global ::tree::mask::mask
  
  set tree  .displaymask.f.tree
  $tree delete [ $tree children {} ]
  set fen [toShortFen [sc_pos fen] ]
  # use clipbase to enter a dummy game
  set currentbase [sc_base current]
  sc_base switch $::clipbase_db
  sc_game push copyfast
  
  if {[catch {sc_game startBoard $fen} err]} {
    puts "sc_game startBoard $fen => $err"
  }
  if { [info exists mask($fen) ] } {
    set moves [lindex $mask($fen) 0]
    ::tree::mask::populateDisplayMask $moves {} $fen {} [lindex $mask($fen) 1]
  }
  sc_game pop
  sc_base switch $currentbase
}
################################################################################
# creates a new image whose name is name1_name2, and concatenates two images.
# parameters are the markers, not the images names
################################################################################
proc ::tree::mask::createImage {marker1 marker2} {
  
  if {[lsearch [image names] "$marker1$marker2" ] != -1} {
    return
  }
  set img1 $::tree::mask::marker2image($marker1)
  set img2 $::tree::mask::marker2image($marker2)
  set w1 [image width $img1]
  set w2 [image width $img2]
  set h1 [image height $img1]
  set h2 [image height $img2]
  set margin 2
  image create photo $marker1$marker2 -height $h1 -width [expr $w1 + $w2 + $margin]
  $marker1$marker2 copy $img1 -from 0 0 -to 0 0
  $marker1$marker2 copy $img2 -from 0 0 -to [expr $w1 +$margin] 0
}
################################################################################
#
################################################################################
proc  ::tree::mask::maskTreeUnfold {} {
  set t .displaymask.f.tree
  
  proc unfold {id} {
    set t .displaymask.f.tree
    foreach c [$t children $id] {
      $t item $c -open true
      unfold $c
    }
  }
  
  set id [$t selection]
  unfold $id
}
################################################################################
# returns the first line of multi-line string (separated with \n)
################################################################################
proc ::tree::mask::trimToFirstLine {s} {
  set s [ lindex [ split $s "\n" ] 0 ]
  return $s
}
################################################################################
#
################################################################################
proc ::tree::mask::populateDisplayMask { moves parent fen fenSeen posComment} {
  global ::tree::mask::mask
  
  set posComment [ trimToFirstLine $posComment ]
  
  if { $posComment != ""} {
    set posComment "\[$posComment\] "
  }
  
  set tree .displaymask.f.tree
  
  foreach m $moves {
    set move [lindex $m 0]
    if {$move == "null"} { continue }
    set img ""
    if {[lindex $m 4] != "" && [lindex $m 5] == ""} {
      set img [lindex $m 4]
    }
    if {[lindex $m 4] == "" && [lindex $m 5] != ""} {
      set img [lindex $m 5]
    }
    if {[lindex $m 4] != "" && [lindex $m 5] != ""} {
      set l [array get ::tree::mask::marker2image]
      set idx [ lsearch $l [lindex $m 4] ]
      set mark1 [lindex $l [expr $idx -1 ] ]
      set idx [ lsearch $l [lindex $m 5] ]
      set mark2 [lindex $l [expr $idx -1 ] ]
      createImage $mark1 $mark2
      set img $mark1$mark2
    }
    
    set nag ""
    if { $::tree::mask::displayMask_showNag } {
      set nag [lindex $m 1]
    }
    
    if {[lindex $m 3] != "" && $::tree::mask::displayMask_showComment} {
      set move_comment " [lindex $m 3]"
      set move_comment [ trimToFirstLine $move_comment ]
    } else  {
      set move_comment ""
    }
    if { ! $::tree::mask::displayMask_showComment} {
      set posComment ""
    }
    set id [ $tree insert $parent end -text "$posComment[::trans $move][set nag]$move_comment" -image $img -tags dblClickTree ]
    if {[catch {sc_game startBoard $fen} err]} {
      puts "ERROR sc_game startBoard $fen => $err"
    }
    sc_move addSan $move
    
    set newfen [toShortFen [sc_pos fen] ]
    if {[lsearch $fenSeen $newfen] != -1} { return }
    if { [info exists mask($newfen) ] } {
      set newmoves [lindex $mask($newfen) 0]
      
      while { [llength $newmoves] == 1 } {
        lappend fenSeen $newfen
        sc_move addSan [ lindex $newmoves { 0 0 } ]
        set newfen [toShortFen [sc_pos fen] ]
        if {[lsearch $fenSeen $newfen] != -1} { return }
        lappend fenSeen $newfen
        if {[lindex $newmoves 0 3] != "" && $::tree::mask::displayMask_showComment } {
          set move_comment " [lindex $newmoves 0 3]"
          set move_comment [ trimToFirstLine $move_comment ]
        } else  {
          set move_comment ""
        }
        
        if {[lindex $newmoves 1] != "" && $::tree::mask::displayMask_showComment } {
          set pos_comment " \[[lindex $newmoves 1]\]"
          set pos_comment [ trimToFirstLine $pos_comment ]
        } else  {
          set pos_comment ""
        }
        set nag ""
        if { $::tree::mask::displayMask_showNag } {
          set nag [ lindex $newmoves { 0 1 }  ]
        }
        $tree item $id -text "[ $tree item $id -text ] $pos_comment[::trans [ lindex $newmoves { 0 0 }  ] ][ set nag  ]$move_comment"
        if { ! [info exists mask($newfen) ] } {
          break
        }
        set newmoves [lindex $mask($newfen) 0]
      }
      
      if { [info exists mask($newfen) ] } {
        set newmoves [lindex $mask($newfen) 0]
        ::tree::mask::populateDisplayMask $newmoves $id $newfen $fenSeen [lindex $mask($newfen) 1]
      }
    }
  }
  
}
################################################################################
#
################################################################################
proc ::tree::mask::searchMask { baseNumber } {
  
  set w .searchmask
  if { [winfo exists $w] } {
    focus $w
    return
  }
  toplevel $w
  wm title $w [::tr SearchMask]
  ttk::frame $w.f1
  ttk::frame $w.f2
  pack $w.f1 -side top -fill both -expand 1 -anchor e
  pack $w.f2 -side top -fill both -expand 1 -anchor e
  
  # NAG selection
  ttk::checkbutton $w.f1.nagl -text [tr Nag] -variable ::tree::mask::searchMask_usenag
  menu $w.f1.nagmenu
  ttk::menubutton $w.f1.nag -textvariable ::tree::mask::searchMask_nag -menu $w.f1.nagmenu -style pad0.TMenubutton
  set ::tree::mask::searchMask_nag  [::tr "None"]
  foreach nag [ list "!!" " !" "!?" "?!" " ?" "??" " ~" [::tr "None"]  ] {
    $w.f1.nagmenu add command -label $nag -command "set ::tree::mask::searchMask_nag $nag"
  }
  grid $w.f1.nagl -column 0 -row 0 -sticky w -padx 10
  grid $w.f1.nag -column 0 -row 1 -sticky w -padx 10
  
  # Markers 1 & 2
  foreach j { 0 1 } {
    ttk::checkbutton $w.f1.ml$j -text "[tr Marker] [expr $j +1]" -variable ::tree::mask::searchMask_usemarker$j
    menu $w.f1.menum$j
    ttk::menubutton $w.f1.m$j -textvariable ::tree::mask::searchMask_trm$j -menu $w.f1.menum$j -style pad0.TMenubutton
    set ::tree::mask::searchMask_trm$j [tr "Include"]
    set ::tree::mask::searchMask_m$j $::tree::mask::marker2image(Include)
    foreach e { Include Exclude MainLine Bookmark White Black NewLine ToBeVerified ToTrain Dubious ToRemove } {
      set i $::tree::mask::marker2image($e)
      $w.f1.menum$j add command -label [ tr $e ] -image $i -compound left \
          -command "set ::tree::mask::searchMask_trm$j \"[tr $e ]\" ; set ::tree::mask::searchMask_m$j $i"
    }
      grid $w.f1.ml$j -column [expr 1 + $j] -row 0 -sticky w -padx [expr $j*10]
    grid $w.f1.m$j -column [expr 1 + $j] -row 1 -sticky w -padx [expr $j*10]
  }
  
  # Color
  ttk::checkbutton $w.f1.colorl -text [tr ColorMarker] -variable ::tree::mask::searchMask_usecolor
  menu $w.f1.colormenu
  ttk::menubutton $w.f1.color -textvariable ::tree::mask::searchMask_trcolor -menu $w.f1.colormenu  -style pad0.TMenubutton
  set ::tree::mask::searchMask_trcolor  [::tr "White"]
  set ::tree::mask::searchMask_color "White"
  foreach c { "White" "Green" "Yellow" "Blue" "Red"} {
    $w.f1.colormenu add command -label [ tr "${c}Mark" ] \
        -command "set ::tree::mask::searchMask_trcolor [ tr ${c}Mark ] ; set ::tree::mask::searchMask_color $c"
  }
  grid $w.f1.colorl -column 3 -row 0 -sticky w
  grid $w.f1.color -column 3 -row 1 -sticky w
  
  # Move annotation
  ttk::checkbutton $w.f1.movecommentl -text "Move comment" -variable ::tree::mask::searchMask_usemovecomment
  ttk::entry $w.f1.movecomment -textvariable ::tree::mask::searchMask_movecomment -width 12
  grid $w.f1.movecommentl -column 4 -row 0 -sticky w -padx 10
  grid $w.f1.movecomment -column 4 -row 1 -sticky w -padx 10
  
  # Position annotation
  ttk::checkbutton $w.f1.poscommentl -text "Position comment" -variable ::tree::mask::searchMask_useposcomment
  ttk::entry $w.f1.poscomment -textvariable ::tree::mask::searchMask_poscomment -width 12
  grid $w.f1.poscommentl -column 5 -row 0 -sticky w
  grid $w.f1.poscomment -column 5 -row 1 -sticky w
  
  ttk::button $w.f1.search -text [tr "Search"] -command " ::tree::mask::performSearch $baseNumber "
  grid $w.f1.search -column 5 -row 2 -sticky e -pady "2 5" -padx "0 5"
  
  # display search result
  text $w.f2.text -yscrollcommand "$w.f2.ybar set" -height 40
  ttk::scrollbar $w.f2.ybar -command "$w.f2.text yview"
  pack $w.f2.text -side left -fill both -expand yes
  pack $w.f2.ybar -side left -fill y
  
  setWinLocation $w
  setWinSize $w
  
  bind $w.f2.text <ButtonPress-1> " ::tree::mask::searchClick %x %y %W $baseNumber "
  bind $w <Escape> { destroy  .searchmask }
  bind $w <Configure> "recordWinSize $w"
  
}
################################################################################
#
################################################################################
proc  ::tree::mask::performSearch  { baseNumber } {
  global ::tree::mask::mask
  set t .searchmask.f2.text
  # contains the search result (FEN)
  set res {}
  
  set pos_count 0
  set move_count 0
  set pos_total 0
  set move_total 0
  
  $t delete 1.0 end
  
  # Display FEN + moves and comments. Clicking on a line starts filtering current base
  foreach fen [array names mask] {
    incr pos_total
    
    # Position comment
    set poscomment [ lindex $mask($fen) 1 ]
    if { $::tree::mask::searchMask_useposcomment  } {
      if { [string match -nocase "*$::tree::mask::searchMask_poscomment*"  $poscomment] } {
        lappend res "$fen $poscomment"
        incr pos_count
      } else  {
        continue
      }
    }
    
    set moves [ lindex $mask($fen) 0 ]
    foreach m $moves {
      incr move_total
      
      # NAG
      if { $::tree::mask::searchMask_usenag } {
        set nag $::tree::mask::searchMask_nag
        if { $nag == [::tr "None"] } {  set nag ""  }
        if { [ string trim [lindex $m 1] ] != $nag } {
          continue
        }
      }
      
      # Markers 1 & 2
      if { $::tree::mask::searchMask_usemarker0 } {
        if { $::tree::mask::searchMask_m0 != [lindex $m 4] } {
          continue
        }
      }
      if { $::tree::mask::searchMask_usemarker1 } {
        if { $::tree::mask::searchMask_m1 != [lindex $m 5] } {
          continue
        }
      }
      
      # Color
      if { $::tree::mask::searchMask_usecolor } {
        if { [ string compare -nocase $::tree::mask::searchMask_color [lindex $m 2] ] != 0 } {
          continue
        }
      }
      
      # Move annotation
      set movecomment [lindex $m 3]
      if { $::tree::mask::searchMask_usemovecomment } {
        if {  ! [string match -nocase "*$::tree::mask::searchMask_movecomment*"  $movecomment]  } {
          continue
        }
      }
      
      lappend res "$fen [::trans [lindex $m 0]] $movecomment"
      incr move_count
    }
  }
  
  # output the result
  foreach l $res {
    $t insert end "$l\n"
  }
  wm title .searchmask "[::tr SearchMask] [::tr Positions] $pos_count / $pos_total - [::tr moves] $move_count / $move_total"
}
################################################################################
#
################################################################################
proc  ::tree::mask::searchClick {x y win baseNumber} {
  set idx [ $win index @$x,$y ]
  if { [ scan $idx "%d.%d" l c ] != 2 } {
    # should never happen
    return
  }
  set elt [$win get $l.0 $l.end]
  
  if {[llength $elt] < 4} {
    return
  }
  
  set fen [ lrange $elt 0 3 ]
  
  # load the position in a temporary game (in clipbase), update the Trees then switch to Tree's base
  sc_base switch $::clipbase_db
  sc_game push copyfast
  
  if {[catch {sc_game startBoard $fen} err]} {
    puts "sc_game startBoard $fen => $err"
  } else  {
    # TODO : call sc_search board maybe wiser ?
    ::tree::refresh
    # updateBoard -pgn
  }
  
  sc_game pop

  sc_base switch $baseNumber
  # ::file::SwitchToBase $baseNumber
  if {[sc_filter first != 0]} {
    ::game::Load [sc_filter first]
  } else  {
    updateBoard -pgn
  }
  
  # updateBoard -pgn
  
}
################################################################################
#
################################################################################
