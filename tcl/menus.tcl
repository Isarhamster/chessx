### Menus.tcl: part of Scid.
### Copyright (C) 2001-2003 Shane Hudson.
### Copyright (C) 2015 Fulvio Benini


############################################################
### Main window menus:

menu .menu

## Mac Application menu has to be before any call to configure.
if { $macOS } {
  # Application menu:
  .menu add cascade -label Scid -menu .menu.apple
  menu .menu.apple
  
  set m .menu.apple
  
  $m add command -label HelpAbout -command helpAbout
  
  $m add separator
  
  # To Quit
  bind all <Command-q> "exit"
  bind all <Command-Q> "exit"
  
  ## To get Help
  bind all <Command-?> {helpWindow Contents}
  bind all <Help> {helpWindow Contents}
}


### File menu:
set m .menu.file
menu $m -postcommand "updateMenuStates $m"
.menu add cascade -label File -menu $m
$m add command -label FileNew -command ::file::New
$m add command -label FileOpen -acc "Ctrl+O" -command ::file::Open
$m add command -label FileFinder -acc "Ctrl+/" -command ::file::finder::Open
menu $m.bookmarks
$m add cascade -label FileBookmarks -menu $m.bookmarks
$m add separator
# naming is weird because the menus are moved from Tools to File menus
$m add command -label ToolsOpenBaseAsTree -command ::file::openBaseAsTree
menu $m.recenttrees
$m add cascade -label ToolsOpenRecentBaseAsTree -menu $m.recenttrees
$m add separator
set ::menuFileRecentIdx [expr [$m index end] +1]
$m add command -label FileExit -accelerator "Ctrl+Q" -command ::file::Exit


### Database menu:
set m .menu.db
menu $m
.menu add cascade -label Database -menu $m
$m add command -label FileClose -acc "Ctrl+W" -command ::file::Close
$m add separator
menu $m.exportfilter
  $m.exportfilter add command -label ToolsExpFilterPGN \
      -command {exportGames filter PGN}
  $m.exportfilter add command -label ToolsExpFilterHTML \
      -command {exportGames filter HTML}
  $m.exportfilter add command -label ToolsExpFilterHTMLJS \
      -command {::html::exportCurrentFilter}
  $m.exportfilter add command -label ToolsExpFilterLaTeX \
      -command {exportGames filter LaTeX}
  $m.exportfilter add separator
  set ::menuDbExportFilterIdx [expr [$m.exportfilter index end] +1]
$m add cascade -label ToolsExpFilter -menu $m.exportfilter
$m add command -label ToolsImportFile -command { importPgnFile $::curr_db }
$m add separator
menu $m.utils
  $m.utils add checkbutton -label FileMaintWin -accelerator "Ctrl+M" -variable maintWin -command ::maint::OpenClose
  $m.utils add command -label FileMaintCompact -command compactDB
  $m.utils add command -label FileMaintClass -command classifyAllGames
  $m.utils add checkbutton -label FileMaintNameEditor -command nameEditor -variable nameEditorWin -accelerator "Ctrl+Shift+N"
  $m.utils add command -label StripTags -command stripTags
  $m.utils add separator
  $m.utils add command -label FileMaintDelete -state disabled -command markTwins
  $m.utils add command -label FileMaintTwin -command updateTwinChecker
  $m.utils add separator
  $m.utils add command -label Cleaner -command cleanerWin
$m add cascade -label FileMaint -menu $m.utils
menu $m.spell
  $m.spell add command -label FileMaintNamePlayer -command {openSpellCheckWin Player}
  $m.spell add command -label FileMaintNameEvent -command {openSpellCheckWin Event}
  $m.spell add command -label FileMaintNameSite -command {openSpellCheckWin Site}
  $m.spell add command -label FileMaintNameRound -command {openSpellCheckWin Round}
  $m.spell add command -label AddEloRatings -command {allocateRatings}
$m add cascade -label FileMaintName -menu $m.spell
$m add separator
set ::menuDbSwitchIdx [expr [$m index end] +1]


### Edit menu:
set m .menu.edit
menu $m
.menu add cascade -label Edit -menu $m
$m add command -label EditUndo -accelerator "Ctrl+z" -command { undoFeature undo }
$m add command -label EditRedo -accelerator "Ctrl+y" -command { undoFeature redo }
$m add separator
$m add command -label EditSetup -accelerator "S" -command setupBoard
$m add command -label EditCopyBoard -accelerator "Ctrl+Shift+C" -command copyFEN
$m add command -label EditPasteBoard -accelerator "Ctrl+Shift+V" -command pasteFEN
$m add command -label PgnFileCopy -command ::pgn::PgnClipboardCopy
$m add command -label EditPastePGN -command importClipboardGame
$m add separator
menu $m.strip
  $m.strip add command -label EditStripComments -command {::game::Strip comments}
  $m.strip add command -label EditStripVars -command {::game::Strip variations}
  $m.strip add command -label EditStripBegin -command {::game::TruncateBegin}
  $m.strip add command -label EditStripEnd -command {::game::Truncate}
$m add cascade -label EditStrip -menu $m.strip
$m add separator
$m add command -label EditReset -command ::windows::gamelist::ClearClipbase
$m add command -label EditCopy -command ::gameAddToClipbase
$m add command -label EditPaste -command {
  sc_clipbase paste
  ::notify::GameChanged
}


### Game menu:
set m .menu.game
menu $m -postcommand "updateMenuStates $m"
.menu add cascade -label Game -menu $m
$m add command -label GameNew -accelerator "Ctrl+N" -command ::game::Clear
$m add command -label GameReload -command ::game::Reload
$m add separator
$m add command -label GameReplace -command gameReplace -accelerator "Ctrl+S"
$m add command -label GameAdd -command gameAdd  -accelerator "Ctrl+Shift+S"
menu $m.exportcurrent
  $m.exportcurrent add command -label ToolsExpCurrentPGN \
      -command {exportGames current PGN}
  $m.exportcurrent add command -label ToolsExpCurrentHTML \
      -command {exportGames current HTML}
  $m.exportcurrent add command -label ToolsExpCurrentHTMLJS \
      -command {::html::exportCurrentGame}
  $m.exportcurrent add command -label ToolsExpCurrentLaTeX \
      -command {exportGames current LaTeX}
$m add cascade -label ToolsExpCurrent -menu $m.exportcurrent
$m add separator
$m add command -label GameFirst -accelerator "Ctrl+Shift+Up" -command {::game::LoadNextPrev first}
$m add command -label GamePrev -accelerator "Ctrl+Up" -command {::game::LoadNextPrev previous}
$m add command -label GameNext -accelerator "Ctrl+Down" -command {::game::LoadNextPrev next}
$m add command -label GameLast -accelerator "Ctrl+Shift+Down" -command {::game::LoadNextPrev last}
$m add command -label GameRandom -command ::game::LoadRandom -accelerator "Ctrl+?"
$m add separator
$m add command -label GameDeepest -accelerator "Ctrl+Shift+D" -command {
  sc_move ply [sc_eco game ply]
  updateBoard
}
$m add command -label GameGotoMove -accelerator "Ctrl+U" -command ::game::GotoMoveNumber
$m add command -label GameNovelty -accelerator "Ctrl+Shift+Y" -command findNovelty


### Search menu:
set m .menu.search
menu $m
.menu add cascade -label Search -menu $m
$m add command -label SearchCurrent -command ::search::board -accelerator "Ctrl+Shift+B"
$m add command -label SearchHeader -command ::search::header -accelerator "Ctrl+Shift+H"
$m add command -label SearchMaterial -command ::search::material -accelerator "Ctrl+Shift+M"
$m add separator
$m add checkbutton -label WindowsPList -variable plistWin -command ::plist::toggle -accelerator "Ctrl+Shift+P"
$m add checkbutton -label WindowsTmt -variable tourneyWin -command ::tourney::toggle -accelerator "Ctrl+Shift+T"
$m add separator
$m add command -label SearchUsing -accel "Ctrl+Shift+U" -command ::search::usefile


### Play menu:
set m .menu.play
menu $m -postcommand "updateMenuStates $m"
.menu add cascade -label Play -menu $m
$m add command -label ToolsSeriousGame -command ::sergame::config
$m add command -label ToolsTacticalGame -command ::tacgame::config
$m add command -label ToolsTrainFics -command ::fics::config
$m add separator
menu $m.training
  $m.training add command -label ToolsTrainOpenings -command ::opening::config
  $m.training add command -label ToolsTrainTactics -command ::tactics::config
  $m.training add command -label ToolsTrainReviewGame -command ::reviewgame::start
  $m.training add command -label ToolsTrainCalvar -command ::calvar::config
$m add cascade -label ToolsTraining -menu $m.training
$m add separator
menu $m.correspondence
  $m.correspondence add command -label CCConfigure   -command {::CorrespondenceChess::config}
  $m.correspondence add command -label CCConfigRelay -command {::CorrespondenceChess::ConfigureRelay}
  $m.correspondence add separator
  $m.correspondence add command -label CCOpenDB      -command {::CorrespondenceChess::OpenCorrespondenceDB; ::CorrespondenceChess::ReadInbox} \
      -accelerator "Ctrl+F12"
  $m.correspondence add separator
  $m.correspondence add command -label CCRetrieve    -command { ::CorrespondenceChess::FetchGames }
  $m.correspondence add command -label CCInbox       -command { ::CorrespondenceChess::ReadInbox }
  $m.correspondence add separator
  $m.correspondence add command -label CCSend        -command {::CorrespondenceChess::SendMove 0 0 0 0}
  $m.correspondence add command -label CCResign      -command {::CorrespondenceChess::SendMove 1 0 0 0}
  $m.correspondence add command -label CCClaimDraw   -command {::CorrespondenceChess::SendMove 0 1 0 0}
  $m.correspondence add command -label CCOfferDraw   -command {::CorrespondenceChess::SendMove 0 0 1 0}
  $m.correspondence add command -label CCAcceptDraw  -command {::CorrespondenceChess::SendMove 0 0 0 1}
  $m.correspondence add command -label CCGamePage    -command {::CorrespondenceChess::CallWWWGame}
  $m.correspondence add separator
  $m.correspondence add command -label CCNewMailGame -command {::CorrespondenceChess::newEMailGame}
  $m.correspondence add command -label CCMailMove    -command {::CorrespondenceChess::eMailMove}
$m add cascade -label CorrespondenceChess -menu $m.correspondence


### Windows menu:
set m .menu.windows
menu $m
.menu add cascade -label Windows -menu $m
$m add checkbutton -label WindowsComment -var ::windows::commenteditor::isOpen -command "::makeCommentWin toggle" -accelerator "Ctrl+E"
$m add checkbutton -label WindowsPGN -variable pgnWin -command ::pgn::OpenClose  -accelerator "Ctrl+P"
$m add checkbutton -label OptionsWindowsShowGameInfo -variable showGameInfo -command ::toggleGameInfo
$m add separator
$m add command -label WindowsGList -command ::windows::gamelist::Open  -accelerator "Ctrl+L"
$m add command -label "Engine" -command ::enginewin::Open
$m add checkbutton -label WindowsSwitcher -variable baseWin -accelerator "Ctrl+D" -command ::windows::switcher::Open
$m add command -label ToolsCross -accelerator "Ctrl+Shift+X" -command ::crosstab::Open
$m add checkbutton -label WindowsECO -accelerator "Ctrl+Y" -variable ::windows::eco::isOpen -command {::windows::eco::OpenClose}
$m add checkbutton -label WindowsStats -variable ::windows::stats::isOpen -accelerator "Ctrl+I" -command ::windows::stats::Open
$m add checkbutton -label WindowsTree -variable treeWin -command ::tree::make -accelerator "Ctrl+T"
$m add checkbutton -label WindowsTB -variable ::tb::isOpen -command ::tb::Open -accelerator "Ctrl+="
$m add checkbutton -label WindowsBook -variable ::book::isOpen -command ::book::open -accelerator "F6"
$m add checkbutton -label WindowsCorrChess -variable ::CorrespondenceChess::isOpen \
    -command ::CorrespondenceChess::CCWindow -accelerator "F12"


### Tools menu:
set m .menu.tools
menu $m
.menu add cascade -label Tools -menu $m
$m  add command -label ToolsAnalysis \
    -command makeAnalysisWin -accelerator "Ctrl+Shift+A"
$m  add command -label ToolsAnalysis2 \
    -command "makeAnalysisWin 2" -accelerator "Ctrl+Shift+2"
$m  add checkbutton -label ToolsStartEngine1 -variable analysisWin1 \
    -command "makeAnalysisWin 1 0" -accelerator "F2"
$m  add checkbutton -label ToolsStartEngine2 -variable analysisWin2 \
    -command "makeAnalysisWin 2 0" -accelerator "F3"
$m add separator
$m add checkbutton -label ToolsEmail \
    -accelerator "Ctrl+Shift+E" -variable emailWin -command ::tools::email
$m add checkbutton -label ToolsFilterGraph \
    -accelerator "Ctrl+Shift+G" -variable filterGraph -command tools::graphs::filter::Open
$m add checkbutton -label ToolsAbsFilterGraph \
    -accelerator "Ctrl+Shift+J" -variable absfilterGraph -command tools::graphs::absfilter::Open
$m add command -label ToolsOpReport \
    -accelerator "Ctrl+Shift+O" -command ::optable::makeReportWin
$m add command -label ToolsTracker \
    -accelerator "Ctrl+Shift+K" -command ::ptrack::make
$m add command -label ToolsBookTuning -command ::book::tuning
menu $m.hardware
  $m.hardware add command -label ToolsConnectHardwareConfigure -command ::ExtHardware::config
  $m.hardware add command -label ToolsConnectHardwareInputEngineConnect -command ::inputengine::connectdisconnect
  $m.hardware add command -label ToolsConnectHardwareNovagCitrineConnect -command ::novag::connect
$m add cascade -label ToolsConnectHardware -menu $m.hardware
$m add separator
menu $m.pinfo
  $m.pinfo add command -label GraphOptionsWhite -command { ::pinfo::playerInfo [sc_game info white] }
  $m.pinfo add command -label GraphOptionsBlack -command { ::pinfo::playerInfo [sc_game info black] }
$m add cascade -label ToolsPInfo -menu $m.pinfo
$m add command -label ToolsPlayerReport -command ::preport::preportDlg
$m add command -label ToolsRating -command {::tools::graphs::rating::Refresh both}
$m add command -label ToolsScore -command ::tools::graphs::score::Refresh ;# -accelerator "Ctrl+Shift+Z"


### Options menu:
set m .menu.options
menu $m
.menu add cascade -label Options -menu $m
menu $m.language
  foreach l $::languages {
      $m.language add radiobutton -label $::langName($l) \
          -underline $::langUnderline($l) -variable language -value $l \
          -command setLanguage
  }
$m add cascade -label OptionsLanguage -menu $m.language
if { $::macOS } { $m entryconfigure end -state disabled }
menu $m.theme -tearoff 1
set ::menuThemeListIdx [expr [$m.theme index end] +1]
$m add cascade -label OptionsTheme -menu $m.theme
menu $m.savelayout
menu $m.restorelayout
    foreach i {"1 (default)" "2" "3"} slot {1 2 3} {
      $m.savelayout add command -label $i -command "::docking::layout_save $slot"
      $m.restorelayout add command -label $i -command "::docking::layout_restore $slot"
    }
$m add command -label ConfigureScid -command { ::preferences::Open toggle }
$m add command -label OptionsResources -command ::preferences::resources
menu $m.export
  $m.export add command -label "PGN file text" -underline 0 -command "setExportText PGN"
  $m.export add command -label "HTML file text" -underline 0 -command "setExportText HTML"
  $m.export add command -label "LaTeX file text" -underline 0 -command "setExportText LaTeX"
$m add cascade -label OptionsExport -menu $m.export
$m add separator
$m add checkbutton -label OptionsWindowsDock -variable windowsDock
$m add cascade -label OptionsWindowsSaveLayout -menu $m.savelayout
$m add cascade -label OptionsWindowsRestoreLayout -menu $m.restorelayout
$m add separator
$m add command -label OptionsSave -command options.write
$m add checkbutton -label OptionsAutoSave -variable optionsAutoSave \
    -command { if {!$::optionsAutoSave} { options.autoSaveHack } }


### Help menu:
set m .menu.helpmenu
menu $m
.menu add cascade -label Help -menu $m
set acc [expr {$macOS ? "Command-?" : "F1"}]
$m add command -label HelpContents -command {helpWindow Contents} -accelerator "$acc"
$m add command -label HelpIndex -command {helpWindow Index}
$m add command -label HelpGuide -command {helpWindow Guide}
$m add command -label HelpHints -command {helpWindow Hints}
$m add command -label HelpContact -command {helpWindow Author}
$m add separator
$m add command -label HelpTip -command ::tip::show
$m add separator
$m  add command -label HelpAbout -command helpAbout


##################################################
# Store menu labels for translations and help messages
set ::menuHelpMessage {}
proc storeMenuLabels {m} {
    bind $m <<MenuSelect>> {
        set idx [%W index active]
        if {$idx != "none"} {
            # Tcl/Tk seems to generate strange window names for menus that
            # are configured to be a toplevel window main menu, e.g.
            # .menu.file get reported as ".#menu.#menu#file" and
            # .menu.file.utils is ".#menu.#menu#file.#menu#file#utils"
            # I have no idea why it does this, but to avoid it we
            # convert a window paths with hashes to its true value:
            regsub -all "\#" [winfo name %W] . win
            set ::menuHelpMessage {}
            catch {
                set lbl $::MenuLabels($win,$idx)
                set ::menuHelpMessage $::helpMessage($::language,$lbl)
            }
            updateStatusBar
        } elseif {$::menuHelpMessage ne ""} {
            set ::menuHelpMessage {}
            updateStatusBar
        }
    }

    set n [$m index end]
    for {set i 0} {$n != "none" && $i <= $n} {incr i} {
        set type [$m type $i]
        if {$type != "separator" && $type != "tearoff"} {
            set ::MenuLabels($m,$i) [$m entrycget $i -label]
        }
        if {$type == "cascade"} {
            storeMenuLabels [$m entrycget $i -menu]
        }
    }
}
# Issue a command to a menu entry
proc menuConfig {{m} {label} {cmd} args} {
    foreach {key lbl} [array get ::MenuLabels "$m*"] {
        if {$lbl == $label} {
            set idx [lindex [split $key ","] 1]
            $m $cmd $idx {*}$args
            break
        }
    }
}
storeMenuLabels .menu
set fileExitHack [.menu.file index end]
set ::MenuLabels(.menu.file,end) $::MenuLabels(.menu.file,$fileExitHack)
array unset ::MenuLabels ".menu.file,$fileExitHack"


##################################################
# updateMenuStates:
#   Update all the menus, rechecking which state each item should be in.
#
proc updateMenuStates {{menuname}} {
  set m .menu
  switch -- $menuname {
  {.menu.file} {
      ::bookmarks::Refresh

      # update recent Tree list (open base as Tree)
      set ntreerecent [::recentFiles::treeshow .menu.file.recenttrees]

      # Remove and reinsert the Recent files list and Exit command:
      set idx2 [expr {[$m.file index end] -1}]
      $m.file delete $::menuFileRecentIdx $idx2
      set nrecent [::recentFiles::show $m.file $::menuFileRecentIdx]
      if {$nrecent > 0} {
        $m.file insert [expr $::menuFileRecentIdx + $nrecent] separator
      }
    }
  {.menu.play} {
      set n [$m.play index end]
      set st normal
      if {[info exists ::playMode]} { set st disabled }
      for {set i 0} {$i <= $n} {incr i} {
        catch { $m.play entryconfig $i -state $st }
      }
    }
  {.menu.game} {
    set isReadOnly [sc_base isReadOnly $::curr_db]
    # Load first/last/random/game number buttons:
    set filtercount [sc_filter count]
    if {$filtercount == 0} {set state disabled} else {set state normal}
    $m.game entryconfig [tr GameFirst] -state $state
    $m.game entryconfig [tr GameLast] -state $state
    $m.game entryconfig [tr GameRandom] -state $state
    
    # Load previous button:
    if {[sc_filter previous]} {set state normal} else {set state disabled}
    $m.game entryconfig [tr GamePrev] -state $state
    .main.tb.gprev configure -state $state
    
    # Reload button:
    if {[sc_game number]} {set state normal} else {set state disabled}
    $m.game entryconfig [tr GameReload] -state $state
    
    # Load next button:
    if {[sc_filter next]} {set state normal} else {set state disabled}
    $m.game entryconfig [tr GameNext] -state $state
    .main.tb.gnext configure -state $state
    
    # Save add button:
    set state normal
    $m.game entryconfig [tr GameAdd] -state $state
    
    # Save replace button:
    set state normal
    if {[sc_game number] == 0  ||  $isReadOnly } {
      set state disabled
    }
    $m.game entryconfig [tr GameReplace] -state $state
  }
  }
}

# Update the dynamic menus relative to current/open databases
proc menuUpdateBases {} {
  set ::currentSlot $::curr_db
  .menu.db delete $::menuDbSwitchIdx end
  .menu.db.exportfilter delete $::menuDbExportFilterIdx end

  foreach i [sc_base list] {
    set fname [file tail [sc_base filename $i]]

    .menu.db add radiobutton -variable currentSlot -value $i \
        -label "Base $i: $fname" \
        -underline 5 -accelerator "Ctrl+$i"\
        -command [list ::file::SwitchToBase $i]

    if {$i != $::curr_db && ![sc_base isReadOnly $i]} {
        .menu.db.exportfilter add command -label "Base $i: $fname" \
            -command "::windows::gamelist::CopyGames {} $::curr_db $i"
    }
  }

  #Current database
  set notClipbase [expr {$::curr_db != $::clipbase_db ? "normal" : "disabled"}]
  set canChange   [expr {![sc_base isReadOnly $::curr_db] ? "normal" : "disabled"}]
  set canCompact  [expr {[baseIsCompactable] ? "normal" : "disabled"}]
  set notEmpty    [expr {[sc_base numGames $::curr_db] != 0 ? "normal" : "disabled"}]

  menuConfig .menu.db FileClose entryconfig -state $notClipbase
  menuConfig .menu.db ToolsExpFilter entryconfig -state $notEmpty
  menuConfig .menu.db FileMaintName entryconfig -state $canChange
  menuConfig .menu.db.utils Cleaner          entryconfig -state $canChange
  menuConfig .menu.db.utils StripTags        entryconfig -state $canChange
  menuConfig .menu.db.utils FileMaintDelete  entryconfig -state $canChange
  menuConfig .menu.db.utils FileMaintClass   entryconfig -state $canChange
  menuConfig .menu.db.utils FileMaintTwin    entryconfig -state $canChange
  menuConfig .menu.db.utils FileMaintCompact entryconfig -state $canCompact
  menuConfig .menu.db.utils FileMaintNameEditor entryconfig -state $canChange
}

proc menuUpdateThemes {} {
  set m .menu.options.theme
  $m delete $::menuThemeListIdx end
  foreach i [lsort [ttk::style theme names]] {
      $m add radiobutton -label "$i" -value $i -variable ::lookTheme \
      -command {ttk::style theme use $::lookTheme}
  }
}

##############################
# Multiple-language menu support functions.

# configMenuText:
#    Reconfigures the main window menus. Called when the language is changed.
#
proc configMenuText {menu entry tag lang} {
  global menuLabel menuUnder
  if {[info exists menuLabel($lang,$tag)] && [info exists menuUnder($lang,$tag)]} {
    $menu entryconfig $entry -label $menuLabel($lang,$tag) -underline $menuUnder($lang,$tag)
  } else {
    $menu entryconfig $entry -label $menuLabel(E,$tag) -underline $menuUnder(E,$tag)
  }
}

proc setLanguageMenus {} {
  set lang $::language
  foreach {key lbl} [array get ::MenuLabels] {
    foreach {m idx} [split $key ","] {
      set under 0
      catch { set under $::menuUnder($lang,$lbl) }
      $m entryconfig $idx -label [tr $lbl] -underline $under
    }
  }

  ::pgn::ConfigMenus
  ::tree::ConfigMenus
  ::crosstab::ConfigMenus
  ::optable::ConfigMenus
  ::preport::ConfigMenus
  ::tools::graphs::score::ConfigMenus
  ::tools::graphs::rating::ConfigMenus

  # Check for duplicate menu underline characters in this language:
  # set ::verifyMenus 1
  if {[info exists ::verifyMenus] && $::verifyMenus} {
    foreach m {file edit game search windows tools options help} {
      set list [checkMenuUnderline .menu.$m]
      if {[llength $list] > 0} {
        puts stderr "Menu $m has duplicate underline letters: $list"
      }
    }
  }
}

################################################################################
# checkMenuUnderline:
#  Given a menu widget, returns a list of all the underline
#  characters that appear more than once.
################################################################################
proc checkMenuUnderline {menu} {
  array set found {}
  set duplicates {}
  set last [$menu index last]
  for {set i [$menu cget -tearoff]} {$i <= $last} {incr i} {
    if {[string equal [$menu type $i] "separator"]} {
      continue
    }
    set char [string index [$menu entrycget $i -label] \
        [$menu entrycget $i -underline]]
    set char [string tolower $char]
    if {$char == ""} {
      continue
    }
    if {[info exists found($char)]} {
      lappend duplicates $char
    }
    set found($char) 1
  }
  return $duplicates
}

################################################################################
#
################################################################################
proc configInformant { w } {
  global informant
  
  ttk::frame $w.spinF
  set idx 0
  set row 0
  
  foreach i [lsort [array names informant]] {
    ttk::label $w.spinF.labelExpl$idx -text [ ::tr "Informant[ string trim $i "\""]" ]
    ttk::label $w.spinF.label$idx -text $i
     # Allow the configuration of "won game" up to "Mate found"
     if {$i == "\"++-\""} {
         ttk::spinbox $w.spinF.sp$idx -textvariable informant($i) -width 5 -from 0.0 -to 328.0 -increment 1.0 -validate all -validatecommand { regexp {^[0-9]\.[0-9]$} %P }
     } else {
         ttk::spinbox $w.spinF.sp$idx -textvariable informant($i) -width 5 -from 0.0 -to 9.9 -increment 0.1 -validate all -validatecommand { regexp {^[0-9]\.[0-9]$} %P }
     }
    grid $w.spinF.labelExpl$idx -row $row -column 0 -sticky w
    incr row
    grid $w.spinF.label$idx -row $row -column 0 -sticky w
    grid $w.spinF.sp$idx -row $row -column 1 -sticky w -padx "0 5" -pady "0 5"
    incr row
    incr idx
  }
  pack $w.spinF
}

proc openTableBaseDirs {nr widget} {
  set dirname [$widget get]
  if {$dirname ne "" && ![file isdirectory $dirname]} {
    $widget configure -style Error.TEntry
    return
  }
  $widget configure -style TEntry
  if {$dirname eq $::initialDir(tablebase$nr)} {
    return
  }
  set ::initialDir(tablebase$nr) $dirname

  set tableBaseDirs ""
  foreach i {1 2 3 4} {
    set tbDir [string trim $::initialDir(tablebase$i)]
    if {$tbDir != ""} {
      if {$tableBaseDirs != ""} { append tableBaseDirs ";" }
      append tableBaseDirs [file nativename $tbDir]
    }
  }

  set npieces [sc_info tb $tableBaseDirs]
  if {$npieces == 0} {
    set msg "No tablebases were found."
  } else {
    set msg "Tablebases with up to $npieces pieces were found.\n\n"
    append msg "If you want these tablebases be used whenever\n"
    append msg "you start Scid, select \"Save Options\" from the\n"
    append msg "Options menu before you exit Scid."
  }
  tk_messageBox -type ok -icon info -title "Scid: Tablebase results" \
      -message $msg -parent [winfo toplevel $widget]

  grab [winfo toplevel $widget]
}

proc chooseTableBaseDir {widget} {
  set fullname [tk_chooseDirectory -initialdir [$widget get] -mustexist 1 \
      -title "Scid: Select a Tablebase directory" -parent [winfo toplevel $widget] ]
  if {$fullname ne ""} {
    $widget delete 0 end
    $widget insert end [file nativename $fullname]
  }
}
################################################################################

proc getBooksDir { widget } {
  global scidBooksDir
  set dir [tk_chooseDirectory -initialdir $scidBooksDir -parent [winfo toplevel $widget] -mustexist 1]
  if {$dir != ""} {
      setBooksDir $dir
      $widget delete 0 end
      $widget insert end $dir
  }
}

proc setBooksDir { dir } {
  global scidBooksDir
  set scidBooksDir $dir
}

proc getTacticsBasesDir { widget } {
  global scidBasesDir
  set dir [tk_chooseDirectory -initialdir $scidBasesDir -parent [winfo toplevel $widget] -mustexist 1]
  if {$dir != ""} {
      setTacticsBasesDir $dir
      $widget delete 0 end
      $widget insert end $dir
  }
}

proc setTacticsBasesDir { dir } {
  global scidBasesDir
  set scidBasesDir $dir
}

proc getPhotoDir { widget } {
  set idir [pwd]
  if { [info exists ::scidPhotoDir] } { set idir $::scidPhotoDir }
  set dir [tk_chooseDirectory -initialdir $idir -parent [winfo toplevel $widget] -mustexist 1]
  if {$dir != ""} {
      if { [setPhotoDir $dir] } {
          $widget delete 0 end
          $widget insert end $dir
      }
  }
}

proc setPhotoDir { dir } {
    set ::scidPhotoDir $dir
    options.save ::scidPhotoDir
    set n [loadPlayersPhoto]
    set ret [lindex $n 0]
    tk_messageBox -parent .resDialog -message "Found $ret images in [lindex $n 1] file(s)"
    ::notify::GameChanged
    return $ret
}

proc getThemePkgFile { widget} {
  global initialDir
  set fullname [tk_getOpenFile -parent [winfo toplevel $widget] -title "Select a pkgIndex.tcl file for themes" -initialdir [file dirname $::ThemePackageFile] -initialfile $::ThemePackageFile \
	       -filetypes { {Theme "pkgIndex.tcl"} }]
  if { $fullname != "" && $fullname != $::ThemePackageFile && ! [readThemePkgFile $fullname] } {
      $widget delete 0 end
      $widget insert end $fullname
  }
}

proc readThemePkgFile { fullname } {
    if {$fullname ne "" && $fullname != $::ThemePackageFile } {
        set count [llength [ttk::style theme names]]
        set ret [ catch { ::safeSourceStyle $fullname } ]
        set newthemes [expr [llength [ttk::style theme names]] - $count]
        if { $ret == 0 && $newthemes > 0  } {
            menuUpdateThemes
            set ::ThemePackageFile $fullname
        }
        tk_messageBox -parent .resDialog -message "$newthemes new theme(s) found."
    } else {
        set ::ThemePackageFile $fullname
        tk_messageBox -parent .resDialog -message "No new themes loaded."
        set ret 0
    }
    return $ret
}

proc getECOFile { widget } {
  global ecoFile
  set ftype { { "Scid ECO files" {".eco"} } }
  set fullname [tk_getOpenFile -parent [winfo toplevel $widget] -initialdir [file dirname $ecoFile] -filetypes $ftype -title "Load ECO file"]
  if { [readECOFile $fullname] } {
      $widget delete 0 end
      $widget insert end $fullname
  }
}

proc readECOFile { fullname } {
  global ecoFile
  if {[string compare $fullname ""]} {
    if {[catch {sc_eco read $fullname} result]} {
      tk_messageBox -title "Scid" -type ok -icon warning -message $result -parent .resDialog
    } else {
      set ecoFile $fullname
      tk_messageBox -title "Scid: ECO file loaded." -type ok -icon info -parent .resDialog \
          -message "ECO file $fullname loaded: $result positions.\n\nTo have this file automatically loaded when you start Scid, select \"Save Options\" from the Options menu before exiting."
      return 1
    }
  } else {
      set ecoFile $fullname
  }
  return 0
}

proc updateLocale {} {
  global locale
  sc_info decimal $locale(numeric)
  ::windows::gamelist::Refresh
  updateTitle
}

proc chooseFont {fType} {
  global fontOptions
  set fontOptions(temp) [FontDialog font_$fType $fontOptions($fType)]
  if {$fontOptions(temp) != ""} { set fontOptions($fType) $fontOptions(temp) }
  switch $fType {
    {Regular} {
      set font [font configure font_Regular -family]
      set fontsize [font configure font_Regular -size]
      font configure font_Bold -family $font -size $fontsize
      font configure font_Italic -family $font -size $fontsize
      font configure font_BoldItalic -family $font -size $fontsize
      font configure font_H1 -family $font -size [expr {$fontsize + 8} ]
      font configure font_H2 -family $font -size [expr {$fontsize + 6} ]
      font configure font_H3 -family $font -size [expr {$fontsize + 4} ]
      font configure font_H4 -family $font -size [expr {$fontsize + 2} ]
      font configure font_H5 -family $font -size [expr {$fontsize + 0} ]
    }
    {Small} {
      set font [font configure font_Small -family]
      set fontsize [font configure font_Small -size]
      font configure font_SmallBold -family $font -size $fontsize
      font configure font_SmallItalic -family $font -size $fontsize
    }
  }
}

proc chooseHighlightColor {} {
  set col [ tk_chooseColor -initialcolor $::highlightLastMoveColor -title "Scid"]
  if { $col != "" } {
    set ::highlightLastMoveColor $col
    updateBoard
  }
}


### End of file: menus.tcl

