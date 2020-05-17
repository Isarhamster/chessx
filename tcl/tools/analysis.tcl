
###
### analysis.tcl: part of Scid.
### Copyright (C) 1999-2003  Shane Hudson.
### Copyright (C) 2007  Pascal Georges

######################################################################
### Analysis window: uses a chess engine to analyze the board.

# analysis(logMax):
#   The maximum number of log message lines to be saved in a log file.
set analysis(logMax) 5000

# analysis(log_stdout):
#   Set this to 1 if you want Scid-Engine communication log messages
#   to be echoed to stdout.
#
set analysis(log_stdout) 0

set useAnalysisBook 1
set analysisBookSlot 1
set useAnalysisBookName ""
set wentOutOfBook 0
# State variable: 1 <=> engine is making an initial
# assessment of the current position, before progressing
# into the game
set initialAnalysis 0

# State variable: 1 <=> We will not add a variation to
# this move, since this cannot be different from the
# (engine variation to the) main line
set atStartOfLine 0

set batchEnd 1
set stack ""

################################################################################
#
################################################################################
# resetEngine:
#   Resets all engine-specific data.
#
proc resetEngine {n} {
    global analysis
    set analysis(pipe$n) ""             ;# Communication pipe file channel
    set analysis(seen$n) 0              ;# Seen any output from engine yet?
    set analysis(seenEval$n) 0          ;# Seen evaluation line yet?
    set analysis(score$n) 0             ;# Current score in centipawns
    set analysis(prevscore$n) 0         ;# Immediately previous score in centipawns
    set analysis(scoremate$n) 0         ;# Current mating distance (0 if infinite)
    set analysis(prevscoremate$n) 0     ;# Previous mating distance
    set analysis(prevmoves$n) ""        ;# Immediately previous best line out from engine
    set analysis(nodes$n) 0             ;# Number of (kilo)nodes searched
    set analysis(depth$n) 0             ;# Depth in ply
    set analysis(prev_depth$n) 0        ;# Previous depth
    set analysis(time$n) 0              ;# Time in centisec (or sec; see below)
    set analysis(moves$n) ""            ;# PV (best line) output from engine
    set analysis(seldepth$n) 0
    set analysis(currmove$n) ""         ;# current move output from engine
    set analysis(currmovenumber$n) 0    ;# current move number output from engine
    set analysis(hashfull$n) 0
    set analysis(nps$n) 0
    set analysis(tbhits$n) 0
    set analysis(sbhits$n) 0
    set analysis(cpuload$n) 0
    set analysis(movelist$n) {}         ;# Moves to reach current position
    set analysis(nonStdStart$n) 0       ;# Game has non-standard start
    set analysis(has_analyze$n) 0       ;# Engine has analyze command
    set analysis(has_setboard$n) 0      ;# Engine has setboard command
    set analysis(send_sigint$n) 0       ;# Engine wants INT signal
    set analysis(wants_usermove$n) 0    ;# Engine wants "usermove" before moves
    set analysis(isCrafty$n) 0          ;# Engine appears to be Crafty
    set analysis(wholeSeconds$n) 0      ;# Engine times in seconds not centisec
    set analysis(analyzeMode$n) 0       ;# Scid has started analyze mode
    set analysis(invertScore$n) 1       ;# Score is for side to move, not white
    set analysis(automove$n) 0
    set analysis(automoveThinking$n) 0
    set analysis(automoveTime$n) 4000
    set analysis(lastClicks$n) 0
    set analysis(after$n) ""
    set analysis(log$n) ""              ;# Log file channel
    set analysis(logCount$n) 0          ;# Number of lines sent to log file
    set analysis(wbEngineDetected$n) 0  ;# Is this a special Winboard engine?
    set analysis(multiPV$n) {}          ;# multiPV list sorted : depth score moves
    set analysis(multiPVraw$n) {}       ;# same thing but with raw UCI moves
    set analysis(uci$n) 0               ;# UCI engine
    # UCI engine options in format ( name min max ). This is not engine config but its capabilities
    set analysis(uciOptions$n) {}
    # the number of lines in multiPV. If =1 then act the traditional way
    set analysis(multiPVCount$n) 1      ;# number of N-best lines
    set analysis(uciok$n) 0             ;# uciok sent by engine in response to uci command
    set analysis(name$n) ""             ;# engine name
    set analysis(processInput$n) 0      ;# the time of the last processed event
    set analysis(waitForBestMove$n) 0
    set analysis(waitForReadyOk$n) 0
    set analysis(onUciOk$n) ""
    set analysis(movesDisplay$n) 1      ;# if false, hide engine lines, only display scores
    set analysis(lastHistory$n) {}      ;# last best line
    set analysis(maxmovenumber$n) 0     ;# the number of moves in this position
    set analysis(lockEngine$n) 0        ;# the engine is locked to current position
    set analysis(fen$n) {}              ;# the position that engine is analyzing
    set analysis(whenReady$n) {}        ;# list of commands to eval when the engine is ready
    array unset ::uciOptions$n
}

resetEngine 1
resetEngine 2

set annotateMode 0
set annotateModeButtonValue 0 ; # feedback of annotate mode

################################################################################
# calculateNodes:
#   Divides string-represented node count by 1000
################################################################################
proc calculateNodes {{n}} {
    set len [string length $n]
    if { $len < 4 } {
        return 0
    } else {
        set shortn [string range $n 0 [expr {$len - 4}]]
        scan $shortn "%d" nd
        return $nd
    }
}


# resetAnalysis:
#   Resets the analysis statistics: score, depth, etc.
#
proc resetAnalysis {{n 1}} {
    global analysis
    set analysis(score$n) 0
    set analysis(scoremate$n) 0
    set analysis(nodes$n) 0
    set analysis(prev_depth$n) 0
    set analysis(depth$n) 0
    set analysis(time$n) 0
    set analysis(moves$n) ""
    set analysis(multiPV$n) {}
    set analysis(multiPVraw$n) {}
    set analysis(lastHistory$n) {}
    set analysis(maxmovenumber$n) 0
}

namespace eval enginelist {}

set engines(list) {}

# engine:
#   Adds an engine to the engine list.
#   Calls to this function will be found in the user engines.lis
#   file, which is sourced below.
#
proc engine {arglist} {
    global engines
    array set newEngine {}
    foreach {attr value} $arglist {
        set newEngine($attr) $value
    }
    # Check that required attributes exist:
    if {! [info exists newEngine(Name)]} { return  0 }
    if {! [info exists newEngine(Cmd)]} { return  0 }
    if {! [info exists newEngine(Dir)]} { return  0 }
    # Fill in optional attributes:
    if {! [info exists newEngine(Args)]} { set newEngine(Args) "" }
    if {! [info exists newEngine(Elo)]} { set newEngine(Elo) 0 }
    if {! [info exists newEngine(Time)]} { set newEngine(Time) 0 }
    if {! [info exists newEngine(URL)]} { set newEngine(URL) "" }
    # puts this option here for compatibility with previous file format (?!)
    if {! [info exists newEngine(UCI)]} { set newEngine(UCI) 0 }
    if {! [info exists newEngine(UCIoptions)]} { set newEngine(UCIoptions) "" }
    
    lappend engines(list) [list $newEngine(Name) $newEngine(Cmd) \
            $newEngine(Args) $newEngine(Dir) \
            $newEngine(Elo) $newEngine(Time) \
            $newEngine(URL) $newEngine(UCI) $newEngine(UCIoptions)]
    return 1
}

# ::enginelist::read
#   Reads the user Engine list file.
#
proc ::enginelist::read {} {
    catch {source [scidConfigFile engines]}
}

# Change the name of an engine and write the "Engine list" file.
# Returns the new name on success or the old name on error.
proc ::enginelist::rename {oldname newname} {
    set idx [lsearch -exact -index 0 $::engines(list) $oldname]
    if {$idx <0 || [lsearch -exact -index 0 $::engines(list) $newname] >= 0} {
        return $oldname
    }
    lset ::engines(list) $idx 0 $newname
    ::enginelist::write
    return $newname
}

# Search a previous configuration with the same name and replace it.
# If necessary write the "Engine list" file.
proc ::enginelist::save {enginecfg} {
    lassign $enginecfg name
    set idx [lsearch -exact -index 0 $::engines(list) $name]
    if {$idx < 0} {
        return 0
    }
    lset enginecfg 8 [lmap elem [lindex $enginecfg 8] {
        lassign $elem name value type default min max var_list internal
        if {$internal || $value eq $default} { continue }
        list $name $value
    }]
    if {[lindex $::engines(list) $idx] eq $enginecfg} {
        return 0
    }
    lset ::engines(list) $idx $enginecfg
    ::enginelist::write
    return 1
}

# ::enginelist::write:
#   Writes the user Engine list file.
#
proc ::enginelist::write {} {
    global engines
    
    set enginesFile [scidConfigFile engines]
    set enginesBackupFile [scidConfigFile engines.bak]
    # Try to rename old file to backup file and open new file:
    catch {file rename -force $enginesFile $enginesBackupFile}
    if {[catch {open $enginesFile w} f]} {
        catch {file rename $enginesBackupFile $enginesFile}
        return 0
    }
    
    puts $f "\# Analysis engines list file for Scid $::scidVersion with UCI support"
    puts $f ""
    foreach e $engines(list) {
        set name [lindex $e 0]
        set cmd [lindex $e 1]
        set args [lindex $e 2]
        set dir [lindex $e 3]
        set elo [lindex $e 4]
        set time [lindex $e 5]
        set url [lindex $e 6]
        set uci [lindex $e 7]
        set opt [lindex $e 8]
        puts $f "engine {"
            puts $f "  Name [list $name]"
            puts $f "  Cmd  [list $cmd]"
            puts $f "  Args [list $args]"
            puts $f "  Dir  [list $dir]"
            puts $f "  Elo  [list $elo]"
            puts $f "  Time [list $time]"
            puts $f "  URL  [list $url]"
            puts $f "  UCI [list $uci]"
            puts $f "  UCIoptions [list $opt]"
            puts $f "}"
        puts $f ""
    }
    close $f
    return 1
}

# Read the user Engine List file now:
#
catch { ::enginelist::read }
if {[llength $engines(list)] == 0} {
    # No engines, so set up a default engine list:
    set scidlet "scidlet"
    set phalanx "phalanx-scid"
    set togaII "togaII"
    if { $::windowsOS } {
        set scidlet "scidlet.exe"
        set phalanx "phalanx-scid.exe"
        set togaII "TogaII.exe"
    }
    set scidEngPaths [list $::scidExeDir [file join $::scidExeDir "engines" ] [file join $::scidShareDir "engines" ] \
            [ file join $::scidUserDir "engines" ]  [ file join /usr local share scid engines ] \
            [ file join /usr local bin ] [ file join  /usr bin ] [ file join /usr local games ] [ file join /usr games ] \
            [file join $::scidExeDir "engines" "phalanx-scid" ] [file join $::scidExeDir "engines" "togaII1.2.1a" "src" ] ]
    
    # The next four lists should have the same length!
    set scidEngCmds [list $phalanx $togaII $scidlet ]
    set scidEngNames [list "Phalanx-Scid" "Toga II" "Scidlet" ]
    array set parentDirs "
    $phalanx { phalanx-scid Phalanx-XXII }
    $togaII  { togaII1.2.1a toga togaII [ file join togaII1.2.1a src ] }
    $scidlet { . }
    "
    
    set isUCI [list 0 1 0 ]
    
    # Let's search the engines:
    foreach cmd $scidEngCmds name $scidEngNames uci $isUCI {
        set leave 0
        foreach path $scidEngPaths {
            set c [ file join $path $cmd]
            if { [file executable $c ] && ! [ file isdirectory $c ] } {
                engine [list \
                        Name $name \
                        Cmd  $c \
                        Dir  . \
                        UCI  $uci \
                        UCIoptions {} \
                        ]
                set leave 1
            } else {
                foreach parent $parentDirs($cmd) {
                    set c [ file join $path $parent $cmd ]
                    if { [file executable $c] && ! [ file isdirectory $c ] } {
                        engine [list \
                                Name $name \
                                Cmd  $c \
                                Dir  . \
                                UCI  $uci \
                                UCIoptions {} \
                                ]
                        set leave 1
                        break
                    }
                }
            }
            if { $leave } { break }
        }
    }
}

# ::enginelist::date
#   Given a time in seconds since 1970, returns a
#   formatted date string.
proc ::enginelist::date {time} {
    return [clock format $time -format "%a %b %d %Y %H:%M"]
}

# ::enginelist::sort
#   Sort the engine list.
#   If the engine-open dialog is open, its list is updated.
#   The type defaults to the current engines(sort) value.
#
proc ::enginelist::sort {{type ""}} {
    global engines
    
    if {$type == ""} {
        set type $engines(sort)
    } else {
        set engines(sort) $type
    }
    switch $type {
        Name {
            set engines(list) [lsort -dictionary -index 0 $engines(list)]
        }
        Elo {
            set engines(list) [lsort -integer -decreasing -index 4 $engines(list)]
        }
        Time {
            set engines(list) [lsort -integer -decreasing -index 5 $engines(list)]
        }
    }
    
    # If the Engine-open dialog is open, update it:
    #
    set w .enginelist
    if {! [winfo exists $w]} { return }
    set f $w.list.list
    $f delete 0 end
    set count 0
    foreach engine $engines(list) {
        incr count
        set name [lindex $engine 0]
        set elo [lindex $engine 4]
        set time [lindex $engine 5]
        set uci [lindex $engine 7]
        set date [::enginelist::date $time]
        set text [format "%2u. %-21s " $count $name]
        set eloText "    "
        if {$elo > 0} { set eloText [format "%4u" $elo] }
        append text $eloText
        set timeText "  "
        if {$time > 0} { set timeText "   $date" }
        append text $timeText
        $f insert end $text
    }
    $f selection set 0
    
    # Show the sorted column heading in red text:
    $w.title configure -state normal
    foreach i {Name Elo Time} {
        $w.title tag configure $i -foreground {}
    }
    $w.title tag configure $engines(sort) -foreground red
    $w.title configure -state disabled
}
################################################################################
# ::enginelist::choose
#   Select an engine from the Engine List.
#   Returns an integer index into the engines(list) list variable.
#   If no engine is selected, returns the empty string.
################################################################################
proc ::enginelist::choose {} {
    global engines
    set w .enginelist
    if {[winfo exists $w]} {
        raise .enginelist
        return }
    win::createDialog $w
    ::setTitle $w "Scid: [tr ToolsAnalysis]"
    ttk::label $w.flabel -text $::tr(EngineList:) -font font_Bold -anchor center
    pack $w.flabel -side top -fill x
    pack [ttk::frame $w.buttons] -side bottom -fill x
    
    # Set up title frame for sorting the list:
    text $w.title -width 55 -height 1 -font font_Fixed -relief flat \
            -cursor top_left_arrow
    $w.title insert end "    "
    $w.title insert end $::tr(EngineName) Name
    for {set i [string length $::tr(EngineName)]} {$i < 21} { incr i } {
        $w.title insert end " "
    }
    $w.title insert end "  "
    $w.title insert end $::tr(EngineElo) Elo
    for {set i [string length $::tr(EngineElo)]} {$i < 4} { incr i } {
        $w.title insert end " "
    }
    $w.title insert end "  "
    $w.title insert end $::tr(EngineTime) Time
    foreach i {Name Elo Time} {
        $w.title tag bind $i <Any-Enter> \
                "$w.title tag configure $i -background yellow"
        $w.title tag bind $i <Any-Leave> \
                "$w.title tag configure $i -background {}"
        $w.title tag bind $i <1> [list ::enginelist::sort $i]
    }
    $w.title configure -state disabled
    pack $w.title -side top -fill x
    
    # The list of choices:
    set f $w.list
    pack [ttk::frame $f] -side top -expand yes -fill both
    listbox $f.list -height 10 -width 55  -selectmode browse \
            -setgrid 1 -yscrollcommand "$f.ybar set"
    applyThemeStyle Treeview $f.list
    $f.list configure -font font_Fixed -exportselection 0
    bind $f.list <Double-ButtonRelease-1> "$w.buttons.ok invoke; break"
    ttk::scrollbar $f.ybar -command "$f.list yview"
    pack $f.ybar -side right -fill y
    pack $f.list -side top -fill both -expand yes
    $f.list selection set 0
    
    set f $w.buttons
    dialogbutton $f.add -text $::tr(EngineNew...) -command {::enginelist::edit -1}
    dialogbutton $f.edit -text $::tr(EngineEdit...) -command {
        ::enginelist::edit [lindex [.enginelist.list.list curselection] 0]
    }
    dialogbutton $f.delete -text $::tr(Delete...) -command {
        ::enginelist::delete [lindex [.enginelist.list.list curselection] 0]
    }
    ttk::label $f.sep -text "   "
    dialogbutton $f.ok -text "OK" -command {
        set engines(selection) [lindex [.enginelist.list.list curselection] 0]
        destroy .enginelist
    }
    dialogbutton $f.cancel -text $::tr(Cancel) -command {
        set engines(selection) ""
        destroy .enginelist
    }
    packbuttons right $f.cancel $f.ok
    pack $f.add $f.edit $f.delete -side left -padx 1
    
    ::enginelist::sort
    focus $w.list.list
    wm protocol $w WM_DELETE_WINDOW "destroy $w"
    bind $w <F1> { helpWindow Analysis List }
    bind $w <Escape> "destroy $w"
    bind $w.list.list <Return> "$w.buttons.ok invoke; break"
    set engines(selection) ""
    catch {grab $w}
    tkwait window $w
    return $engines(selection)
}

# ::enginelist::setTime
#   Sets the last-opened time of the engine specified by its
#   index in the engines(list) list variable.
#   The time should be in standard format (seconds since 1970)
#   and defaults to the current time.
#
proc ::enginelist::setTime {index {time -1}} {
    global engines
    set e [lindex $engines(list) $index]
    if {$time < 0} { set time [clock seconds] }
    set e [lreplace $e 5 5 $time]
    set engines(list) [lreplace $engines(list) $index $index $e]
}

trace variable engines(newElo) w [list ::utils::validate::Integer [sc_info limit elo] 0]

# ::enginelist::delete
#   Removes an engine from the list.
#
proc ::enginelist::delete {index} {
    global engines
    if {$index == ""  ||  $index < 0} { return }
    set e [lindex $engines(list) $index]
    set msg "Name: [lindex $e 0]\n"
    append msg "Command: [lindex $e 1]\n\n"
    append msg "Do you really want to remove this engine from the list?"
    set answer [tk_messageBox -title Scid -icon question -type yesno \
            -message $msg]
    if {$answer == "yes"} {
        set engines(list) [lreplace $engines(list) $index $index]
        ::enginelist::sort
        ::enginelist::write
    }
}

# ::enginelist::edit
#   Opens a dialog for editing an existing engine list entry (if
#   index >= 0), or adding a new entry (if index is -1).
#
proc ::enginelist::edit {index} {
    global engines
    if {$index == ""} { return }
    
    if {$index >= 0  ||  $index >= [llength $engines(list)]} {
        set e [lindex $engines(list) $index]
    } else {
        set e [list "" "" "" . 0 0 "" 1]
    }
    
    set engines(newIndex) $index
    set engines(newName) [lindex $e 0]
    set engines(newCmd) [lindex $e 1]
    set engines(newArgs) [lindex $e 2]
    set engines(newDir) [lindex $e 3]
    set engines(newElo) [lindex $e 4]
    set engines(newTime) [lindex $e 5]
    set engines(newURL) [lindex $e 6]
    set engines(newUCI) [lindex $e 7]
    set ::uci::newOptions [lindex $e 8]
    
    set engines(newDate) $::tr(None)
    if {$engines(newTime) > 0 } {
        set engines(newDate) [::enginelist::date $engines(newTime)]
    }
    
    set w .engineEdit
    win::createDialog $w
    ::setTitle $w Scid
    
    set f [ttk::frame $w.f]
    pack $f -side top -fill x -expand yes
    set row 0
    foreach i {Name Cmd Args Dir URL} {
        ttk::label $f.l$i -text $i
        if {[info exists ::tr(Engine$i)]} {
            $f.l$i configure -text $::tr(Engine$i)
        }
        ttk::entry $f.e$i -textvariable engines(new$i) -width 40
        bindFocusColors $f.e$i
        grid $f.l$i -row $row -column 0 -sticky w
        grid $f.e$i -row $row -column 1 -sticky we
        
        # Browse button for choosing an executable file:
        if {$i == "Cmd"} {
            ttk::button $f.b$i -text "..." -command {
                if {$::windowsOS} {
                    set ftype {
                        {"Applications" {".bat" ".exe"} }
                        {"All files" {"*"} }
                    }
                    set fName [tk_getOpenFile -initialdir $engines(newDir) \
                        -title "Scid: [tr ToolsAnalysis]" -filetypes $ftype]
                } else {
                    set fName [tk_getOpenFile -initialdir $engines(newDir) \
                        -title "Scid: [tr ToolsAnalysis]"]
                }
                if {$fName != ""} {
                    set engines(newCmd) $fName
                    # Set the directory from the executable path if possible:
                    set engines(newDir) [file dirname $fName]
                    if {$engines(newDir) == ""} [ set engines(newDir) .]
                }
            }
            grid $f.b$i -row $row -column 2 -sticky we
        }
        
        if {$i == "Dir"} {
            ttk::button $f.current -text " . " -command {
                set engines(newDir) .
            }
            ttk::button $f.user -text "~/.scid" -command {
                set engines(newDir) $scidUserDir
            }
            if {$::windowsOS} {
                $f.user configure -text "scid.exe dir"
            }
            grid $f.current -row $row -column 2 -sticky we
            grid $f.user -row $row -column 3 -sticky we
        }
        
        if {$i == "URL"} {
            ttk::button $f.bURL -text [tr FileOpen] -command {
                if {$engines(newURL) != ""} { openURL $engines(newURL) }
            }
            grid $f.bURL -row $row -column 2 -sticky we
        }
        
        incr row
    }
    
    grid columnconfigure $f 1 -weight 1
    
    ttk::checkbutton $f.cbUci -text UCI -variable engines(newUCI) -style Bold.TCheckbutton
    ttk::button $f.bConfigUCI -text $::tr(ConfigureUCIengine) -command {
        ::uci::uciConfig 2 [ toAbsPath $engines(newCmd) ] $engines(newArgs) \
                [ toAbsPath $engines(newDir) ] $::uci::newOptions
    }
    # Mark required fields:
    $f.lName configure -font font_Bold
    $f.lCmd configure -font font_Bold
    $f.lDir configure -font font_Bold
    # $f.cbUci configure -font font_Bold
    
    ttk::label $f.lElo -text $::tr(EngineElo)
    ttk::entry $f.eElo -textvariable engines(newElo) -justify right -width 5
    bindFocusColors $f.eElo
    grid $f.lElo -row $row -column 0 -sticky w
    grid $f.eElo -row $row -column 1 -sticky w
    incr row
    grid $f.cbUci -row $row -column 0 -sticky w
    grid $f.bConfigUCI -row $row -column 1 -sticky w
    incr row
    
    ttk::label $f.lTime -text $::tr(EngineTime)
    ttk::label $f.eTime -textvariable engines(newDate) -anchor w -width 1
    grid $f.lTime -row $row -column 0 -sticky w
    grid $f.eTime -row $row -column 1 -sticky we
    ttk::button $f.clearTime -text $::tr(Clear) -command {
        set engines(newTime) 0
        set engines(newDate) $::tr(None)
    }
    ttk::button $f.nowTime -text $::tr(Update) -command {
        set engines(newTime) [clock seconds]
        set engines(newDate) [::enginelist::date $engines(newTime)]
    }
    grid $f.clearTime -row $row -column 2 -sticky we
    grid $f.nowTime -row $row -column 3 -sticky we
    
    addHorizontalRule $w
    set f [ttk::frame $w.buttons]
    ttk::button $f.ok -text OK -command {
        if {[string trim $engines(newName)] == ""  ||
            [string trim $engines(newCmd)] == ""  ||
            [string trim $engines(newDir)] == ""} {
            tk_messageBox -title Scid -icon info \
                    -message "The Name, Command and Directory fields must not be empty."
        } else {
            set newEntry [list $engines(newName) $engines(newCmd) \
                    $engines(newArgs) $engines(newDir) \
                    $engines(newElo) $engines(newTime) \
                    $engines(newURL) $engines(newUCI) $::uci::newOptions ]
            if {$engines(newIndex) < 0} {
                lappend engines(list) $newEntry
            } else {
                set engines(list) [lreplace $engines(list) \
                        $engines(newIndex) $engines(newIndex) $newEntry]
            }
            destroy .engineEdit
            ::enginelist::sort
            ::enginelist::write
            raise .enginelist
            focus .enginelist
        }
    }
    ttk::button $f.cancel -text $::tr(Cancel) -command "destroy $w; raise .enginelist; focus .enginelist"
    pack $f -side bottom -fill x
    pack $f.cancel $f.ok -side right -padx 2 -pady 2
    ttk::label $f.required -font font_Small -text $::tr(EngineRequired)
    pack $f.required -side left
    
    bind $w <Return> "$f.ok invoke"
    bind $w <Escape> "destroy $w; raise .enginelist; focus .enginelist"
    bind $w <F1> { helpWindow Analysis List }
    focus $w.f.eName
    wm resizable $w 1 0
    catch {grab $w}
}
# ################################################################################
#
################################################################################
proc autoplay {} {
    global autoplayDelay autoplayMode annotateMode analysis
    
    # Was autoplay stopped by the user since the last time the timer ran out?
    # If so, silently exit this handler
    #
    if { $autoplayMode == 0 } {
        return
    }
    
    # Add annotation if needed
    #
    if { $annotateMode } {
        addAnnotation
    }
    
    if { $::initialAnalysis } {
        # Stop analysis if it is running
        # We do not want initial super-accuracy
        #
        stopEngineAnalysis 1
        set annotateMode 1
        # First do the book analysis (if this is configured)
        # The latter condition is handled by the operation itself
        set ::wentOutOfBook 0
        bookAnnotation 1
        # Start the engine
        startEngineAnalysis 1 1
    
    # Autoplay comes in two flavours:
    # + It can run through a game, with or without annotation
    # + It can be annotating just opening sections of games
    # See if such streak ends here and now
    #
    } elseif { [sc_pos isAt end] || ($annotateMode && $::isBatchOpening && ([sc_pos moveNumber] > $::isBatchOpeningMoves)) } {
        
        # Stop the engine
        #
        stopEngineAnalysis 1
        
        # Are we running a batch analysis?
        #
        if { $annotateMode && $::isBatch } {
            # First replace the game we just finished
            #
            set gameNo [sc_game number]
            if { $gameNo != 0 } {
                sc_game save $gameNo
            }
            
            # See if we must advance to the next game
            #
            if { $gameNo < $::batchEnd } {
                incr gameNo
                sc_game load $gameNo
                updateTitle
                updateBoard -pgn
                # First do book analysis
                #
                set ::wentOutOfBook 0
                bookAnnotation 1
                # Start with initial assessment of the position
                #
                set ::initialAnalysis 1
                # Start the engine
                #
                startEngineAnalysis 1 1
            } else {
                # End of batch, stop
                #
                cancelAutoplay
                return
            }
        } else {
            # Not in a batch, just stop
            #
            cancelAutoplay
            return
        }
    } elseif { $annotateMode && $::isAnnotateVar } {
        # A construction to prune empty variations here and now
        # It makes no sense to discover only after some engine
        # time that we entered a dead end.
        #
        set emptyVar 1
        while { $emptyVar } {
            set emptyVar 0
            # Are we at the end of a variation?
            # If so, pop back into the parent
            #
            if { [sc_pos isAt vend] } {
                sc_var exit
                set lastVar [::popAnalysisData]
            } else {
                set lastVar [sc_var count]
            }
            # Is there a subvariation here?
            # If so, enter it after pushing where we are
            #
            if { $lastVar > 0 } {
                incr lastVar -1
                sc_var enter $lastVar
                ::pushAnalysisData $lastVar
                # Check if this line is empty
                # If so, we will pop back immediately in the next run
                #
                if { [sc_pos isAt vstart] && [sc_pos isAt vend] } {
                    set emptyVar 1
                } else {
                    # We are in a new line!
                    # Tell the annotator (he might be interested)
                    #
                    updateBoard -pgn
                    set ::atStartOfLine 1
                }
            } else {
                # Just move ahead following the current line
                #
                ::move::Forward
            }
        }
    } else {
        # Just move ahead following the main line
        #
        ::move::Forward
    }
    
    # Respawn
    #
    after $autoplayDelay autoplay
}

proc startAutoplay { } {
    set ::autoplayMode 1
    after 100 autoplay
}

proc setAnnotateModeButtonValue { value } {
    if { ! $value } {
       .analysisWin1.b1.annotate configure -image tb_annotate
       .analysisWin1.b1.annotate state !pressed
    } else {
       .analysisWin1.b1.annotate configure -image tb_annotate_on
       .analysisWin1.b1.annotate state pressed
    }
    set ::annotateModeButtonValue $value
}

proc cancelAutoplay {} {
    set ::autoplayMode 0
    set ::annotateMode 0
    setAnnotateModeButtonValue 0
    after cancel autoplay
    ::notify::PosChanged
}


proc configAnnotation {n} {
    global autoplayDelay tempdelay blunderThreshold annotateModeButtonValue
    
    set w .configAnnotation
    # Do not do anything if the window exists
    #
    if { [winfo exists $w] } {
        raise $w
        focus $w
        return
    }
    
    if { ! $annotateModeButtonValue } {
       setAnnotateModeButtonValue 1
    } else {
       setAnnotateModeButtonValue 0
    }
    # If the annotation button is pressed while annotation is
    # running, stop the annotation
    #
    if { ! $annotateModeButtonValue } {
        cancelAutoplay
        return
    }
    
    trace variable blunderThreshold w {::utils::validate::Regexp {^[0-9]*\.?[0-9]*$}}
    
    set tempdelay [expr {int($autoplayDelay / 1000.0)}]
    win::createDialog $w
    ::setTitle $w "Scid: $::tr(Annotate)"
    wm resizable $w 0 0
    set f [ttk::frame $w.f]
    pack $f -expand 1

    ttk::labelframe $f.analyse -text $::tr(GameReview)
    ttk::label $f.analyse.label -text $::tr(AnnotateTime)
    ttk::spinbox $f.analyse.spDelay -background white -width 4 -textvariable tempdelay -from 1 -to 999 -increment 1 \
        -validate key -validatecommand { return [string is digit %S] }
    ttk::radiobutton  $f.analyse.allmoves     -text $::tr(AnnotateAllMoves)     -variable annotateBlunders -value allmoves
    ttk::radiobutton  $f.analyse.blundersonly -text $::tr(AnnotateBlundersOnly) -variable annotateBlunders -value blundersonly
    ttk::frame $f.analyse.blunderbox
    ttk::label $f.analyse.blunderbox.label -text $::tr(BlundersThreshold:)
    ttk::spinbox $f.analyse.blunderbox.spBlunder -background white -width 4 -textvariable blunderThreshold \
            -from 0.1 -to 3.0 -increment 0.1
    ttk::checkbutton $f.analyse.cbBook  -text $::tr(UseBook) -variable ::useAnalysisBook
    # choose a book for analysis
    # load book names
    set bookPath $::scidBooksDir
    set bookList [  lsort -dictionary [ glob -nocomplain -directory $bookPath *.bin ] ]
    
    # No book found
    if { [llength $bookList] == 0 } {
        set ::useAnalysisBook 0
        $f.analyse.cbBook configure -state disabled
    }
    
    set tmp {}
    set idx 0
    set i 0
    foreach file  $bookList {
        lappend tmp [ file tail $file ]
        if {$::book::lastBook == [ file tail $file ] } {
            set idx $i
        }
        incr i
    }
    ttk::combobox $f.analyse.comboBooks -width 12 -values $tmp
    catch { $f.analyse.comboBooks current $idx }
    pack $f.analyse.comboBooks -side bottom -anchor w -padx 20
    pack $f.analyse.cbBook -side bottom -anchor w
    pack $f.analyse.blunderbox.label -side left -padx { 20 0 }
    pack $f.analyse.blunderbox.spBlunder -side left -anchor w
    pack $f.analyse.blunderbox -side bottom -anchor w
    pack $f.analyse.blundersonly -side bottom -anchor w
    pack $f.analyse.allmoves  -side bottom -anchor w
    pack $f.analyse.label -side left -anchor w
    pack $f.analyse.spDelay -side right -anchor e
    bind $w <Escape> { .configAnnotation.f.buttons.cancel invoke }
    bind $w <Return> { .configAnnotation.f.buttons.ok invoke }

    ttk::labelframe   $f.av -text $::tr(AnnotateWhich)
    ttk::radiobutton  $f.av.all     -text $::tr(AnnotateAll)   -variable annotateMoves -value all
    ttk::radiobutton  $f.av.white   -text $::tr(AnnotateWhite) -variable annotateMoves -value white
    ttk::radiobutton  $f.av.black   -text $::tr(AnnotateBlack) -variable annotateMoves -value black
    pack $f.av.all $f.av.white $f.av.black -side top -fill x -anchor w

    ttk::labelframe   $f.comment -text $::tr(Comments)
    ttk::checkbutton  $f.comment.cbAnnotateVar      -text $::tr(AnnotateVariations)         -variable ::isAnnotateVar
    ttk::checkbutton  $f.comment.cbShortAnnotation  -text $::tr(ShortAnnotations)           -variable ::isShortAnnotation
    ttk::checkbutton  $f.comment.cbAddScore         -text $::tr(AddScoreToShortAnnotations) -variable ::addScoreToShortAnnotations
    ttk::checkbutton  $f.comment.cbAddAnnotatorTag  -text $::tr(addAnnotatorTag)            -variable ::addAnnotatorTag
    # Checkmark to enable all-move-scoring
    ttk::checkbutton  $f.comment.scoreAll -text $::tr(ScoreAllMoves) -variable scoreAllMoves
    ttk::checkbutton  $f.comment.cbMarkTactics -text $::tr(MarkTacticalExercises) -variable ::markTacticalExercises
    if {! $::analysis(uci1)} {
        set ::markTacticalExercises 0
        $f.comment.cbMarkTactics configure -state disabled
    }
    
    pack $f.comment.scoreAll $f.comment.cbAnnotateVar $f.comment.cbShortAnnotation $f.comment.cbAddScore \
	$f.comment.cbAddAnnotatorTag $f.comment.cbMarkTactics -fill x -anchor w
    # batch annotation of consecutive games, and optional opening errors finder
    ttk::frame $f.batch
    ttk::frame $f.buttons
    grid $f.analyse -row 0 -column 0 -pady { 0 10 } -sticky nswe -padx { 0 10 }
    grid $f.comment -row 0 -column 1 -pady { 0 10 } -sticky nswe -padx { 10 0 }
    grid $f.av -row 1 -column 0 -pady { 10 0 } -sticky nswe -padx { 0 10 }
    grid $f.batch -row 1 -column 1 -pady { 10 0 } -sticky nswe -padx { 10 0 }
    grid $f.buttons -row 2 -column 1 -sticky we

    set to [sc_base numGames $::curr_db]
    if {$to <1} { set to 1}
    ttk::checkbutton $f.batch.cbBatch -text $::tr(AnnotateSeveralGames) -variable ::isBatch
    ttk::spinbox $f.batch.spBatchEnd -background white -width 8 -textvariable ::batchEnd \
            -from 1 -to $to -increment 1 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    ttk::checkbutton $f.batch.cbBatchOpening -text $::tr(FindOpeningErrors) -variable ::isBatchOpening
    ttk::spinbox $f.batch.spBatchOpening -background white -width 2 -textvariable ::isBatchOpeningMoves \
            -from 10 -to 20 -increment 1 -validate all -validatecommand { regexp {^[0-9]+$} %P }
    ttk::label $f.batch.lBatchOpening -text $::tr(moves)
    pack $f.batch.cbBatch -side top -anchor w -pady { 0 0 }
    pack $f.batch.spBatchEnd -side top -padx 20 -anchor w
    pack $f.batch.cbBatchOpening -side top -anchor w
    pack $f.batch.spBatchOpening -side left -anchor w -padx { 20 4 }
    pack $f.batch.lBatchOpening  -side left
    set ::batchEnd $to
    
    ttk::button $f.buttons.cancel -text $::tr(Cancel) -command {
        destroy .configAnnotation
        setAnnotateModeButtonValue 0
	.analysisWin1.b1.annotate configure -image tb_annotate
	.analysisWin1.b1.annotate state !pressed
    }
    ttk::button $f.buttons.ok -text "OK" -command {
        set ::useAnalysisBookName [.configAnnotation.f.analyse.comboBooks get]
        set ::book::lastBook $::useAnalysisBookName
        
        # tactical positions is selected, must be in multipv mode
        if {$::markTacticalExercises} {
            if { $::analysis(multiPVCount1) < 2} {
                # TODO: Why not put it at the (apparent) minimum of 2?
                #
                set ::analysis(multiPVCount1) 4
                changePVSize 1
            }
        }
        
        if {$tempdelay < 0.1} { set tempdelay 0.1 }
        set autoplayDelay [expr {int($tempdelay * 1000)}]
        destroy .configAnnotation
        cancelAutoplay
        setAnnotateModeButtonValue 1
	.analysisWin1.b1.annotate configure -image tb_annotate_on
        # Tell the analysis mode that we want an initial assessment of the
        # position. So: no comments yet, please!
        set ::initialAnalysis 1
        # And start the time slicer
        startAutoplay
    }
    pack $f.buttons.cancel $f.buttons.ok -side right -padx 5 -pady 5
    focus $f.analyse.spDelay
    bind $w <Destroy> { focus . }
}
################################################################################
# Part of annotation process : will check the moves if they are in te book, and add a comment
# when going out of it
################################################################################
proc bookAnnotation { {n 1} } {
    global analysis
    
    if {$::annotateMode && $::useAnalysisBook} {
        
        set prevbookmoves ""
        set bn [ file join $::scidBooksDir $::useAnalysisBookName ]
        sc_book load $bn $::analysisBookSlot
        
        set bookmoves [sc_book moves $::analysisBookSlot]
        while {[string length $bookmoves] != 0 && ![sc_pos isAt vend]} {
            # we are in book, so move immediately forward
            ::move::Forward
            set prevbookmoves $bookmoves
            set bookmoves [sc_book moves $::analysisBookSlot]
        }
        sc_book close $::analysisBookSlot
        set ::wentOutOfBook 1
        
        set verboseMoveOutOfBook " $::tr(MoveOutOfBook)"
        set verboseLastBookMove " $::tr(LastBookMove)"
        
        set theCatch 0
        if { [ string match -nocase "*[sc_game info previousMoveNT]*" $prevbookmoves ] != 1 } {
            if {$prevbookmoves != ""} {
                sc_pos setComment "[sc_pos getComment]$verboseMoveOutOfBook [::trans $prevbookmoves]"
            } else  {
                sc_pos setComment "[sc_pos getComment]$verboseMoveOutOfBook"
            }
            # last move was out of book: it needs to be analyzed, so take back
            #
            set theCatch [catch {sc_move back 1}]
        } else  {
            sc_pos setComment "[sc_pos getComment]$verboseLastBookMove"
        }
        
        if { ! $theCatch } {
            resetAnalysis
            updateBoard -pgn
        }
        set analysis(prevscore$n)     $analysis(score$n)
        set analysis(prevmoves$n)     $analysis(moves$n)
        set analysis(prevscoremate$n) $analysis(scoremate$n)
        set analysis(prevdepth$n)     $analysis(depth$n)
    }
}
################################################################################
# Will add **** to any position considered as a tactical shot
# returns 1 if an exercise was marked, 0 if for some reason it was not (obvious move for example)
################################################################################
proc markExercise { prevscore score nag} {
    
    sc_pos addNag $nag
    
    if {!$::markTacticalExercises} { return 0 }
    
    # check at which depth the tactical shot is found
    # this assumes analysis by an UCI engine
    if {! $::analysis(uci1)} { return 0 }
    
    set deltamove [expr {$score - $prevscore}]
    # filter tactics so only those with high gains are kept
    if { [expr abs($deltamove)] < $::informant("+/-") } { return 0 }
    # dismiss games where the result is already clear (high score,and we continue in the same way)
    if { [expr $prevscore * $score] >= 0} {
        if { [expr abs($prevscore) ] > $::informant("++-") } { return 0 }
        if { [expr abs($prevscore)] > $::informant("+-") && [expr abs($score) ] < [expr 2 * abs($prevscore)]} { return 0 }
    }
    
    # The best move is much better than others.
    if { [llength $::analysis(multiPV1)] < 2 } {
        puts "error, not enough PV"
        return 0
    }
    set sc2 [lindex [ lindex $::analysis(multiPV1) 1 ] 1]
    if { [expr abs( $score - $sc2 )] < 1.5 } { return 0 }
    
    # There is no other winning moves (the best move may not win, of course, but
    # I reject exercises when there are e.g. moves leading to +9, +7 and +5 scores)
    if { [expr $score * $sc2] > 0.0 && [expr abs($score)] > $::informant("+-") && [expr abs($sc2)] > $::informant("+-") } {
        return 0
    }
    
    # The best move does not lose position.
    if {[sc_pos side] == "white" && $score < [expr 0.0 - $::informant("+/-")] } { return 0 }
    if {[sc_pos side] == "black" && $score > $::informant("+/-") } { return 0}
    
    # Move is not obvious: check that it is not the first move guessed at low depths
    set pv [ lindex [ lindex $::analysis(multiPV1) 0 ] 2 ]
    set bm0 [lindex $pv 0]
    foreach depth {1 2 3} {
        set res [ sc_pos analyze -time 1000 -hashkb 32 -pawnkb 1 -searchdepth $depth ]
        set bm$depth [lindex $res 1]
    }
    if { $bm0 == $bm1 && $bm0 == $bm2 && $bm0 == $bm3 } {
        puts "obvious move"
        return 0
    }
    
    # find what time is needed to get the solution (use internal analyze function)
    set timer {1 2 5 10 50 100 200 1000}
    set movelist {}
    for {set t 0} {$t < [llength $timer]} { incr t} {
        set res [sc_pos analyze -time [lindex $timer $t] -hashkb 1 -pawnkb 1 -mindepth 0]
        set move_analyze [lindex $res 1]
        lappend movelist $move_analyze
    }
    
    # find at what timing the right move was reliably found
    # only the move is checked, not if the score is close to the expected one
    for {set t [expr [llength $timer] -1]} {$t >= 0} { incr t -1} {
        if { [lindex $movelist $t] != $bm0 } {
            break
        }
    }
    
    set difficulty [expr $t +2]
    
    # If the base opened is read only, like a PGN file, avoids an exception
    catch { sc_base gameflag [sc_base current] [sc_game number] set T }
    sc_pos setComment "****D${difficulty} [format %.1f $prevscore]->[format %.1f $score] [sc_pos getComment]"
    updateBoard
    
    return 1
}
################################################################################
#
################################################################################
proc addAnnotation { {n 1} } {
    global analysis annotateMoves annotateBlunders annotateMode blunderThreshold scoreAllMoves autoplayDelay
    
    # Check if we only need to register an initial
    # assessment of the position
    # If so, we do not generate any annotation yet
    #
    if { $::initialAnalysis } {
        set ::initialAnalysis 0
        
        if { $::isBatchOpening && ([sc_pos moveNumber] < $::isBatchOpeningMoves ) } {
            appendAnnotator "opBlunder [sc_pos moveNumber] ([sc_pos side])"
        }
        if { $::addAnnotatorTag } {
            appendAnnotator "$analysis(name1) ([expr {$autoplayDelay / 1000}] sec)"
        }
        
        set analysis(prevscore$n)     $analysis(score$n)
        set analysis(prevmoves$n)     $analysis(moves$n)
        set analysis(prevscoremate$n) $analysis(scoremate$n)
        set analysis(prevdepth$n)     $analysis(depth$n)
        
        return
    }
    
    # Check if we are at the start of a subline
    # If so, we will not include the engine line as a variation.
    # Rationale: this line cannot be different from the line for the
    # main move, that we will include anyway.
    #
    set skipEngineLine $::atStartOfLine
    set ::atStartOfLine 0
            
    # First look in the book selected
    # TODO: Is this dead code by now?
    # TODO: Seek for an opportunity to do book analysis on a move by
    #       move basis, thus allowing variations to be included
    #
    if { ! $::wentOutOfBook && $::useAnalysisBook } {
        bookAnnotation
        return
    }
    
    # Let's try to assess the situation:
    # We are here, now that the engine has analyzed the position reached by
    # our last move. Currently it is the opponent to move:
    #
    set tomove [sc_pos side]

    # And this is his best line:
    #
    set moves $analysis(moves$n)
    # For non-uci lines, trim space characters in <moveno>.[ *][...]<move> 
    set moves [regsub -all {\. *} $moves {.}]
    
    # The best line we could have followed, and the game move we just played instead, are here:
    #
    set prevmoves $analysis(prevmoves$n)
    # For non-uci lines, trim space characters in <moveno>.[ *][...]<move> 
    set prevmoves [regsub -all {\. *} $prevmoves {.}]

    set gamemove  [sc_game info previousMoveNT]
    
    # Bail out if we have a mate
    #
    if { [expr { [string index $gamemove end] == "#" }] } {
        set analysis(prevscore$n)     $analysis(score$n)
        set analysis(prevmoves$n)     $analysis(moves$n)
        set analysis(prevscoremate$n) $analysis(scoremate$n)
        set analysis(prevdepth$n)     $analysis(depth$n)
        return
    }
    
    # We will add a closing line at the end of variation or game
    #
    set addClosingLine 0
    if {  [sc_pos isAt vend] } {
        set addClosingLine 1
    }

    # We do not want to insert a best-line variation into the game
    # if we did play along that line. Even not when annotating all moves.
    # It simply makes no sense to do so (unless we are debugging the engine!)
    # Sooner or later the game will deviate anyway; a variation at that point will
    # do nicely and is probably more accurate as well.
    #
    set bestMovePlayed 0
    set bestMoveIsMate 0
    if { $prevmoves != "" } {
        # Following lines of code have only one goal:
        # Transform an engine move (e.g. "g1f3") into the short notation that we use
        # for moves in our games ("Nf3"), such that they can be (string) compared.
        # We create a scratch copy of the game, add the engine move and then ask
        # the game about the most recent move that was played.
        # This might not be the most subtle solution...
        sc_game push copyfast
        set bestmove [lindex $prevmoves 0]
        sc_move back 1
        sc_move_add $bestmove $n
        set bestmove [sc_game info previousMoveNT]
        sc_game pop
        
        if { $bestmove == $gamemove } {
            set bestMovePlayed 1
        }
        
        # Did we miss a mate in one?
        #
        set bestMoveIsMate [expr { [string index $bestmove end] == "#" }]
    }
    
    
    # As said, another reason not to include the engine line
    #
    set skipEngineLine [expr {$skipEngineLine + $bestMovePlayed}]

    # As to the engine evaluations
    # This is score the opponent will have if he plays his best move next
    #
    set score $analysis(score$n)
    
    # This is the score we could have had if we had played our best move
    #
    set prevscore $analysis(prevscore$n)
    
    # Let's help the engine a bit...
    # It makes no sense to criticise the players for moving insights at
    # engine end. So we upgrade the old score to the new score if the lines
    # start with the same move.
    #
    if { $bestMovePlayed } {
        set prevscore $score
    }
    
    # Note that the engine's judgement is in absolute terms, a negative score
    # being favorable to black, a positive score favorable to white
    # Looking primarily for blunders, we are interested in the score decay,
    # which, for white, is (previous-current)
    #
    set deltamove [expr {$prevscore - $score}]
    # and whether the game was already lost for us
    #
    set gameIsLost [expr {$prevscore < (0.0 - $::informant("++-"))}]
    
    # Invert this logic for black
    #
    if { $tomove == "white" } {
        set deltamove [expr {0.0 - $deltamove}]
        set gameIsLost [expr {$prevscore > $::informant("++-")}] 
    }
    
    # Note btw that if the score decay is - unexpectedly - negative, we played
    # a better move than the engine's best line!
    
    # Set an "isBlunder" filter.
    # Let's mark moves with a decay greater than the threshold.
    #
    set isBlunder 0
    if { $deltamove > $blunderThreshold } {
        set isBlunder 2
    } elseif { $deltamove > 0 } {
        set isBlunder 1
    }
    
    set absdeltamove [expr { abs($deltamove) } ]
    
    set exerciseMarked 0
    
    # to parse scores if the engine's name contains - or + chars (see sc_game_scores)
    #
    set engine_name  [string map {"-" " " "+" " "} $analysis(name$n)]
    
    # Prepare score strings for the opponent
    #
    if { $analysis(scoremate$n) != 0 } {
        set text [format "%d:M%d" $analysis(depth$n) $analysis(scoremate$n)]
    } else {
        set text [format "%d:%+.2f" $analysis(depth$n) $score]
    }
    # And for the my (missed?) chance
    #
    if { $analysis(prevscoremate$n) != 0 } {
        set prevtext [format "%d:M%d" $analysis(prevdepth$n) $analysis(prevscoremate$n)]
    } else {
        set prevtext [format "%d:%+.2f" $analysis(prevdepth$n) $prevscore]
    }
    
    # Must we annotate our own moves? If no, we bail out unless
    # - we must add a closing line
    #
    if { ( $annotateMoves == "white"  &&  $tomove == "white" ||
           $annotateMoves == "black"  &&  $tomove == "black"   ) && ! $addClosingLine } {
        set analysis(prevscore$n)     $analysis(score$n)
        set analysis(prevmoves$n)     $analysis(moves$n)
        set analysis(prevscoremate$n) $analysis(scoremate$n)
        set analysis(prevdepth$n)     $analysis(depth$n)
        
        # Add score for this position anyway if this is configured
        #
        if { $scoreAllMoves } {
            sc_pos setComment "[sc_pos getComment] $text"
        }

        updateBoard -pgn
    }
    

    # See if we have the threshold filter activated.
    # If so, take only bad moves and missed mates until the position is lost anyway
    #
    # Or that we must annotate all moves
    #
    if {  (  $annotateBlunders == "blundersonly"
          && ($isBlunder > 1 || ($isBlunder > 0 && [expr abs($score)] >= 327.0))
          && ! $gameIsLost)
       || ($annotateBlunders == "allmoves") } {
        if { $isBlunder > 0 } {
            # Add move score nag, and possibly an exercise
            #
            if {       $absdeltamove > $::informant("??") } {
                set exerciseMarked [ markExercise $prevscore $score "??" ]
            } elseif { $absdeltamove > $::informant("?")  } {
                set exerciseMarked [ markExercise $prevscore $score "?" ]
            } elseif { $absdeltamove > $::informant("?!") } {
                sc_pos addNag "?!"
            }
        } elseif { $absdeltamove > $::informant("!?") } {
            sc_pos addNag "!?"
        }
            
        # Add score comment and engine name if needed
        #
        if { ! $::isShortAnnotation } {
            sc_pos setComment "[sc_pos getComment] $engine_name: $text"
        } elseif { $::addScoreToShortAnnotations || $scoreAllMoves } {
            sc_pos setComment "[sc_pos getComment] $text"
        }
            
        # Add position score nag
        #
        sc_pos addNag [scoreToNag $score]
            
        # Add the variation
        #
        if { $skipEngineLine == 0 } {
            sc_move back
            if { $annotateBlunders == "blundersonly" } {
                # Add a diagram tag, but avoid doubles
                #
                if { [string first "D" "[sc_pos getNags]"] == -1 } {
                    sc_pos addNag "D"
                }
            }
            if { $prevmoves != ""} {
                sc_var create
                # Add the starting move
                sc_move_add [lrange $prevmoves 0 0] $n
                # Add its score
                if { ! $bestMoveIsMate } {
                    if { ! $::isShortAnnotation || $::addScoreToShortAnnotations } {
                        sc_pos setComment "$prevtext"
                    }
                }
                # Add remaining moves
                sc_move_add [lrange $prevmoves 1 end] $n
                # Add position NAG, unless the line ends in mate
                if { $analysis(prevscoremate$n) == 0 } {
                    sc_pos addNag [scoreToNag $prevscore]
                }
                sc_var exit
            }
            sc_move forward
        }
    } else {
        if { $isBlunder == 0 && $absdeltamove > $::informant("!?") } {
            sc_pos addNag "!?"
        }
        if { $scoreAllMoves } { 
            # Add a score mark anyway
            #
            sc_pos setComment "[sc_pos getComment] $text"
        }
    }
        
    if { $addClosingLine } {
        sc_move back
        sc_var create
        sc_move addSan $gamemove
        if { $analysis(scoremate$n) == 0 } {
            if { ! $::isShortAnnotation || $::addScoreToShortAnnotations } {
                sc_pos setComment "$text"
            }
        }
        sc_move_add $moves 1
        if { $analysis(scoremate$n) == 0 } {
            sc_pos addNag [scoreToNag $score]
        }
        sc_var exit
        # Now up to the end of the game
        ::move::Forward
    }
    
    set analysis(prevscore$n)     $analysis(score$n)
    set analysis(prevmoves$n)     $analysis(moves$n)
    set analysis(prevscoremate$n) $analysis(scoremate$n)
    set analysis(prevdepth$n)     $analysis(depth$n)

    updateBoard -pgn
}

# Informant index strings
array set ana_informantList { 0 "+=" 1 "+/-" 2 "+-" 3 "++-" }
# Nags. Note the slight inconsistency for the "crushing" symbol (see game.cpp)
array set ana_nagList  { 0 "=" 1 "+=" 2 "+/-" 3 "+-" 4 "+--" 5 "=" 6 "=+" 7 "-/+" 8 "-+" 9 "--+" }
################################################################################
#
################################################################################
proc scoreToNag {score} {
    global ana_informantList ana_nagList
    # Find the score in the informant map
    set tmp [expr { abs( $score ) }]
    for { set i 0 } { $i < 4 } { incr i } {
        if { $tmp < $::informant("$ana_informantList($i)") } {
            break
        }
    }
    # Jump into negative counterpart
    if { $score < 0.0 } {
        set i [expr {$i + 5}]
    }
    return $ana_nagList($i)
}
################################################################################
# will append arg to current game Annotator tag
################################################################################
proc appendAnnotator { s } {
    # Get the current collection of extra tags
    set extra [sc_game tags get "Extra"]

    set annot 0
    set other ""
    set nExtra {}
    # Walk through the extra tags, just copying the crap we do not need
    # If we meet the existing annotator tag, add our name to the list
    foreach line $extra {
        if { $annot == 1 } {
            lappend nExtra "Annotator \"$line, $s\"\n"
            set annot 2
        } elseif { $other != "" } {
            lappend nExtra "$other \"$line\"\n"
            set other ""
        } elseif {[string match "Annotator" $line]} {
            set annot 1
        } else {
            set other $line
        }
    }
    
    # First annotator: Create a tag
    if { $annot == 0 } {
        lappend nExtra "Annotator \"$s\"\n"
    }
    # Put the extra tags back to the game
    sc_game tags set -extra $nExtra
}
################################################################################
#
################################################################################
proc pushAnalysisData { { lastVar } { n 1 } } {
    global analysis
    lappend ::stack [list $analysis(prevscore$n) $analysis(prevscoremate$n) $analysis(prevdepth$n) \
                          $analysis(score$n)     $analysis(scoremate$n)     $analysis(depth$n) \
                          $analysis(prevmoves$n) $analysis(moves$n) $lastVar ]
}
################################################################################
#
################################################################################
proc popAnalysisData { { n 1 } } {
    global analysis
    # the start of analysis is in the middle of a variation
    if {[llength $::stack] == 0} {
        set analysis(prevscore$n) 0
        set analysis(prevscoremate$n) 0
        set analysis(prevdepth$n) 0
        set analysis(score$n) 0
        set analysis(scoremate$n) 0
        set analysis(depth$n) 0
        set analysis(prevmoves$n) ""
        set analysis(moves$n) ""
        set lastVar 0
        return
    }
    set tmp [lindex $::stack end]
    set analysis(prevscore$n) [lindex $tmp 0]
    set analysis(prevscoremate$n) [lindex $tmp 1]
    set analysis(prevdepth$n) [lindex $tmp 2]
    set analysis(score$n) [lindex $tmp 3]
    set analysis(scoremate$n) [lindex $tmp 4]
    set analysis(depth$n) [lindex $tmp 5]
    set analysis(prevmoves$n) [lindex $tmp 6]
    set analysis(moves$n) [lindex $tmp 7]
    set lastVar [lindex $tmp 8]
    set ::stack [lreplace $::stack end end]
    return $lastVar
}

################################################################################
#
################################################################################
proc addAnalysisVariation {{n 1}} {
    global analysis
    
    if {! [winfo exists .analysisWin$n]} { return }
    
    # Cannot add a variation to an empty variation:
    if {[sc_pos isAt vstart]  &&  [sc_pos isAt vend]} { return }
    
    # if we are at the end of the game, we cannot add variation
    # so we add the analysis one move before and append the last game move at the beginning of the analysis
    set addAtEnd [sc_pos isAt vend]

    set moves $analysis(moves$n)
    if {$analysis(uci$n)} {
        set tmp_moves [ lindex [ lindex $analysis(multiPV$n) 0 ] 2 ]
        set text [format "\[%s\] %d:%s" $analysis(name$n) $analysis(depth$n) [scoreToMate $analysis(score$n) $tmp_moves $n]]
    } else  {
        set text [format "\[%s\] %d:%+.2f" $analysis(name$n) $analysis(depth$n) $analysis(score$n)]
    }
    
    if {$addAtEnd} {
        # get the last move of the game
        set lastMove [sc_game info previousMoveUCI]
        #back one move
        sc_move back
    }
    
    # Add the variation:
    sc_var create
    # Add the comment at the start of the variation:
    sc_pos setComment "[sc_pos getComment] $text"
    if {$addAtEnd} {
        # Add the last move of the game at the beginning of the analysis
        sc_move_add $lastMove $n
    }
    # Add as many moves as possible from the engine analysis:
    sc_move_add $moves $n
    sc_var exit
    
    if {$addAtEnd} {
        #forward to the last move
        sc_move forward
    }

    ::notify::PosChanged -pgn
}
################################################################################
#
################################################################################
proc addAllVariations {{n 1}} {
    global analysis
    
    if {! [winfo exists .analysisWin$n]} { return }
    
    # Cannot add a variation to an empty variation:
    if {[sc_pos isAt vstart]  &&  [sc_pos isAt vend]} { return }
    
    # if we are at the end of the game, we cannot add variation
    # so we add the analysis one move before and append the last game move at the beginning of the analysis
    set addAtEnd [sc_pos isAt vend]

    foreach i $analysis(multiPVraw$n) j $analysis(multiPV$n) {
        set moves [lindex $i 2]
        
        set tmp_moves [ lindex $j 2 ]
        set text [format "\[%s\] %d:%s" $analysis(name$n) [lindex $i 0] [scoreToMate [lindex $i 1] $tmp_moves $n]]
        
        if {$addAtEnd} {
            # get the last move of the game
            set lastMove [sc_game info previousMoveUCI]
            sc_move back
        }
        
        # Add the variation:
        sc_var create
        # Add the comment at the start of the variation:
        sc_pos setComment "[sc_pos getComment] $text"
        if {$addAtEnd} {
            # Add the last move of the game at the beginning of the analysis
            sc_move_add $lastMove $n
        }
        # Add as many moves as possible from the engine analysis:
        sc_move_add $moves $n
        sc_var exit
        
        if {$addAtEnd} {
            #forward to the last move
            sc_move forward
        }
        
    }

    ::notify::PosChanged -pgn
}
################################################################################
#
################################################################################
proc makeAnalysisMove {{n 1} {comment ""}} {
    regexp {[^[:alpha:]]*(.*?)( .*|$)} $::analysis(moves$n) -> move
    if {![info exists move]} { return 0 }

    if { $::analysis(uci$n) } {
        ::addMoveUCI $move
    } else  {
        ::addSanMove $move
    }

    if {$comment != ""} {
        set tmp [sc_pos getComment]
        if {$tmp != ""} { lappend tmp " - " }
        sc_pos setComment "$tmp$comment"
    }

    return 1
}
################################################################################
#
################################################################################

# destroyAnalysisWin:
#   Closes an engine, because its analysis window is being destroyed.
#
proc destroyAnalysisWin {{n 1}} {
    
    global windowsOS analysis annotateMode
    
    if {$::finishGameMode} { toggleFinishGame }
    
    if { $n == 1 && $annotateMode } {
        cancelAutoplay
    }

    # Cancel scheduled commands
    if {$analysis(after$n) != ""} {
        after cancel $analysis(after$n)
    }
    
    # Check the pipe is not already closed:
    if {$analysis(pipe$n) == ""} {
        set ::analysisWin$n 0
        return
    }
    
    # Send interrupt signal if the engine wants it:
    if {(!$windowsOS)  &&  $analysis(send_sigint$n)} {
        catch {exec -- kill -s INT [pid $analysis(pipe$n)]}
    }
    
    # Some engines in analyze mode may not react as expected to "quit"
    # so ensure the engine exits analyze mode first:
    if {$analysis(uci$n)} {
        sendToEngine $n "stop"
        sendToEngine $n "quit"
    } else  {
        sendToEngine $n "exit"
        sendToEngine $n "quit"
    }
    catch { flush $analysis(pipe$n) }
    
    # Uncomment the following line to turn on blocking mode before
    # closing the engine (but probably not a good idea!)
    #   fconfigure $analysis(pipe$n) -blocking 1
    
    # Close the engine, ignoring any errors since nothing can really
    # be done about them anyway -- maybe should alert the user with
    # a message box?
    catch {close $analysis(pipe$n)}
    
    if {$analysis(log$n) != ""} {
        catch {close $analysis(log$n)}
        set analysis(log$n) ""
    }
    resetEngine $n
    set ::analysisWin$n 0
}

# sendToEngine:
#   Send a command to a running analysis engine.
#
proc sendToEngine {n text} {
    # puts " -------- Scid>> $text"
    logEngine $n "Scid  : $text"
    catch {puts $::analysis(pipe$n) $text}
}

# sendMoveToEngine:
#   Sends a move to a running analysis engine, using sendToEngine.
#   If the engine has indicated (with "usermove=1" on a "feature" line)
#   that it wants it, send with "usermove " before the move.
#
proc sendMoveToEngine {n move} {
    # Convert "e7e8Q" into "e7e8q" since that is the XBoard/WinBoard
    # standard for sending moves in coordinate notation:
    set move [string tolower $move]
    if {$::analysis(uci$n)} {
        # should be position fen [sc_pos fen] moves ?
        sendToEngine $n "position fen [sc_pos fen] moves $move"
    } else  {
        if {$::analysis(wants_usermove$n)} {
            sendToEngine $n "usermove $move"
        } else {
            sendToEngine $n $move
        }
    }
}

# logEngine:
#   Log Scid-Engine communication.
#
proc logEngine {n text} {
    global analysis
    
    # Print the log message to stdout if applicable:
    if {$::analysis(log_stdout)} {
        puts stdout $text
    }
    
    if { [ info exists ::analysis(log$n)] && $::analysis(log$n) != ""} {
        puts $::analysis(log$n) $text
        catch { flush $::analysis(log$n) }
        
        # Close the log file if the limit is reached:
        incr analysis(logCount$n)
        if {$analysis(logCount$n) >= $analysis(logMax)} {
            puts $::analysis(log$n) \
                    "NOTE  : Log file size limit reached; closing log file."
            catch {close $analysis(log$n)}
            set analysis(log$n) ""
        }
    }
}

# logEngineNote:
#   Add a note to the engine communication log file.
#
proc logEngineNote {n text} {
    logEngine $n "NOTE  : $text"
}

################################################################################
#
# makeAnalysisWin:
#   Produces the engine list dialog box for choosing an engine,
#   then opens an analysis window and starts the engine.
################################################################################
proc makeAnalysisWin { {n 1} {index -1} {autostart 1}} {
    global analysisWin$n font_Analysis analysisCommand analysis annotateModeButtonValue

    set w ".analysisWin$n"
    if {[winfo exists $w]} {
        focus .
        destroy $w
        return
    }

    resetEngine $n

    if { $index < 0 } {
        # engine selection dialog
        set index [::enginelist::choose]
        if { $index == "" ||  $index < 0 } { return }
        catch {
            ::enginelist::setTime $index
        }
    } else {
        # F2, F3
        set index [expr {$n - 1}]
    }

    set n_engines [llength $::engines(list)]
    if { $index >= $n_engines} {
        if { $n_engines > 0 } {
            tk_messageBox -message "Invalid Engine Number: [expr $index +1]"
            makeAnalysisWin $n -1
        }
        return
    }

    # Set the button in non-annotation state
    #
    if { $n == 1 } {
        set annotateModeButtonValue 0
    }

    set engineData [lindex $::engines(list) $index]
    set analysisName [lindex $engineData 0]
    set analysisCommand [ toAbsPath [lindex $engineData 1] ]
    set analysisArgs [lindex $engineData 2]
    set analysisDir [ toAbsPath [lindex $engineData 3] ]
    set analysis(uci$n) [ lindex $engineData 7 ]
    
    # If the analysis directory is not current dir, cd to it:
    set oldpwd ""
    if {$analysisDir != "."} {
        set oldpwd [pwd]
        catch {cd $analysisDir}
    }

    # Try to execute the analysis program:
    set open_err [catch {set analysis(pipe$n) [open "| [list $analysisCommand] $analysisArgs" "r+"]}]

    # Return to original dir if necessary:
    if {$oldpwd != ""} { catch {cd $oldpwd} }

    if {$open_err} {
        tk_messageBox -title "Scid: error starting analysis" \
                -icon warning -type ok \
                -message "Unable to start the program:\n$analysisCommand"
        resetEngine $n
        return
    }
    
    # Open log file if applicable:
    set analysis(log$n) ""
    if {$analysis(logMax) > 0} {
        if {! [catch {open [file join $::scidLogDir "engine$n.log"] w} log]} {
            set analysis(log$n) $log
            logEngine $n "Scid-Engine communication log file"
            logEngine $n "Engine: $analysisName"
            logEngine $n "Command: $analysisCommand"
            logEngine $n "Date: [clock format [clock seconds]]"
            logEngine $n ""
            logEngine $n "This file was automatically generated by Scid."
            logEngine $n "It is rewritten every time an engine is started in Scid."
            logEngine $n ""
        }
    }
    
    set analysis(name$n) $analysisName
    
    # Configure pipe for line buffering and non-blocking mode:
    fconfigure $analysis(pipe$n) -buffering line -blocking 0
    
    #
    # Set up the  analysis window:
    #
    ::createToplevel $w
    set analysisWin$n 1
    if {$n == 1} {
        ::setTitle $w "Analysis: $analysisName"
    } else {
        ::setTitle $w "Analysis $n: $analysisName"
    }
    bind $w <F1> { helpWindow Analysis }
    
    ::board::new $w.bd 25
    $w.bd configure -relief solid -borderwidth 1
    set analysis(showBoard$n) 0
    set analysis(showEngineInfo$n) 0
    
    ttk::frame $w.b1
    pack $w.b1 -side bottom -fill x
    ttk::button $w.b1.automove -image tb_training  -command "toggleAutomove $n"
    ::utils::tooltip::Set $w.b1.automove $::tr(Training)
    
    ttk::button $w.b1.lockengine -image tb_lockengine -command "toggleLockEngine $n"
    ::utils::tooltip::Set $w.b1.lockengine $::tr(LockEngine)
    .analysisWin$n.b1.lockengine configure -state disabled
    
    ttk::button $w.b1.line -image tb_addvar -command "addAnalysisVariation $n"
    ::utils::tooltip::Set $w.b1.line $::tr(AddVariation)
    
    ttk::button $w.b1.alllines -image tb_addallvars -command "addAllVariations $n"
    ::utils::tooltip::Set $w.b1.alllines $::tr(AddAllVariations)
    
    ttk::button $w.b1.move -image tb_addmove -command "makeAnalysisMove $n"
    ::utils::tooltip::Set $w.b1.move $::tr(AddMove)

    ttk::spinbox $w.b1.multipv -from 1 -to 8 -increment 1 -textvariable analysis(multiPVCount$n) -state disabled -width 2 \
            -command "after idle changePVSize $n"
    ::utils::tooltip::Set $w.b1.multipv $::tr(Lines)
    
    # add a button to start/stop engine analysis
    ttk::button $w.b1.bStartStop -image tb_eng_on -command "toggleEngineAnalysis $n"
    ::utils::tooltip::Set $w.b1.bStartStop "$::tr(StartEngine) (F[expr 3 + $n])"

    if {$n == 1} {
        set ::finishGameMode 0
        ttk::button $w.b1.bFinishGame -image tb_finish_off -command "toggleFinishGame $n"
        ::utils::tooltip::Set $w.b1.bFinishGame $::tr(FinishGame)
    }
    ttk::button $w.b1.showboard -image tb_coords -command "toggleAnalysisBoard $n"
    ::utils::tooltip::Set $w.b1.showboard $::tr(ShowAnalysisBoard)
    
    ttk::button $w.b1.showinfo -image tb_engineinfo -command "toggleEngineInfo $n"
    ::utils::tooltip::Set $w.b1.showinfo $::tr(ShowInfo)
    
    if {$n == 1} {
        ttk::button $w.b1.annotate -image tb_annotate -command "configAnnotation $n"
        ::utils::tooltip::Set $w.b1.annotate $::tr(Annotate...)
    }
    ttk::button $w.b1.priority -image tb_cpu_hi -command "setAnalysisPriority $w $n"
    ::utils::tooltip::Set $w.b1.priority $::tr(LowPriority)
    
    ttk::button $w.b1.update -image tb_update -command "if {$analysis(uci$n)} {sendToEngine $n .}" ;# UCI does not support . command
    ::utils::tooltip::Set $w.b1.update $::tr(Update)
    
    ttk::button $w.b1.help -image tb_help -command { helpWindow Analysis }
    ::utils::tooltip::Set $w.b1.help $::tr(Help)
    
    pack $w.b1.bStartStop $w.b1.lockengine $w.b1.move $w.b1.line -side left
    if {$analysis(uci$n)} {
	pack $w.b1.alllines -side left
    }
    if {$n ==1} {
        pack $w.b1.multipv $w.b1.annotate $w.b1.automove $w.b1.bFinishGame -side left
    } else  {
        pack $w.b1.multipv $w.b1.automove -side left
    }
    pack $w.b1.help $w.b1.priority $w.b1.showboard -side right
    if {! $analysis(uci$n)} {
	pack $w.b1.update -side right
    } else {
	pack $w.b1.showinfo -side right
    }
    text $w.text
    applyThemeStyle Treeview $w.text
    if {$analysis(uci$n)} {
        $w.text configure -width 60 -height 1 -font font_Bold -wrap word -setgrid 1 ;# -spacing3 2
    } else {
        $w.text configure -width 60 -height 4 -font font_Fixed -wrap word -setgrid 1
    }
    ttk::frame $w.hist -relief sunken -borderwidth 1
    text $w.hist.text -width 60 -height 8 -font font_Fixed \
            -wrap word -setgrid 1 -yscrollcommand "$w.hist.ybar set"
    applyThemeStyle Treeview $w.hist.text
    $w.hist.text configure -font font_Fixed
    $w.hist.text tag configure indent -lmargin2 [font measure font_Fixed "xxxxxxxxxxxx"]
    ttk::scrollbar $w.hist.ybar -command "$w.hist.text yview" -takefocus 0
    pack $w.text -side top -fill both
    pack $w.hist -side top -expand 1 -fill both
    pack $w.hist.ybar -side right -fill y
    pack $w.hist.text -side left -expand 1 -fill both
    
    bind $w.hist.text <ButtonPress-$::MB3> "toggleMovesDisplay $n"
    $w.text tag configure blue -foreground blue
    $w.text tag configure bold -font font_Bold
    $w.text tag configure small -font font_Small
    $w.hist.text tag configure blue -foreground blue -lmargin2 [font measure font_Fixed "xxxxxxxxxxxx"]
    $w.hist.text tag configure gray -foreground gray
    if {$autostart != 0} {
    $w.text insert end "Please wait a few seconds for engine initialisation (with some engines, you will not see any analysis \
            until the board changes. So if you see this message, try changing the board \
            by moving backward or forward or making a new move.)" small
    }
    $w.text configure -state disabled
    bind $w <Destroy> "if {\[string equal $w %W\]} { destroyAnalysisWin $n }"
    bind $w <Escape> "focus .; destroy $w"
    bind $w <Key-a> "$w.b1.bStartStop invoke"
    wm minsize $w 25 0
    ::createToplevelFinalize $w

    set analysis(onUciOk$n) "onUciOk $n $w.b1.multipv $autostart [list [ lindex $engineData 8 ]]"
    if {$analysis(uci$n)} {
        fileevent $analysis(pipe$n) readable "::uci::processAnalysisInput $n"
    } else  {
        fileevent $analysis(pipe$n) readable "processAnalysisInput $n"
    }
    after 1000 "checkAnalysisStarted $n"

    # We hope the engine is correctly started at that point, so we can send the first analyze command
    # this problem only happens with winboard engine, as we don't know when they are ready
    if { !$analysis(uci$n) && $autostart != 0 } {
        initialAnalysisStart $n
    }

    catch {
        ::enginelist::sort
        ::enginelist::write
    }
    
}

proc onUciOk {{n} {multiPv_spin} {autostart} {uci_options}} {
    foreach opt $::analysis(uciOptions$n) {
        if { [lindex $opt 0] == "MultiPV" } {
            set min [lindex $opt 1]
            set max [lindex $opt 2]
            $multiPv_spin configure -from $min -to $max -state normal
            break
        }
    }
    foreach {option} $uci_options {
        array set ::uciOptions$n $option
    }
    ::uci::sendUCIoptions $n
    if {$autostart} {
        ::uci::whenReady $n [list startEngineAnalysis $n]
    }
}



################################################################################
#
################################################################################
proc toggleMovesDisplay { {n 1} } {
    set ::analysis(movesDisplay$n) [expr 1 - $::analysis(movesDisplay$n)]
    set h .analysisWin$n.hist.text
    $h configure -state normal
    $h delete 1.0 end
    $h configure -state disabled
    updateAnalysisText $n
}

################################################################################
# will truncate PV list if necessary and tell the engine to send N best lines
################################################################################
proc changePVSize { n } {
    global analysis
    if { $analysis(multiPVCount$n) < [llength $analysis(multiPV$n)] } {
        set analysis(multiPV$n) {}
        set analysis(multiPVraw$n) {}
    }
    set h .analysisWin$n.hist.text
    if {[winfo exists $h] && $analysis(multiPVCount$n) == 1} {
        $h configure -state normal
        $h delete 0.0 end
        $h configure -state disabled
        set analysis(lastHistory$n) {}
    }
    if { ! $analysis(uci$n) } { return }

    array set ::uciOptions$n [list "MultiPv" "$analysis(multiPVCount$n)"]
    ::uci::sendUCIoptions $n
    if {$analysis(analyzeMode$n)} {
        ::uci::whenReady $n [list updateAnalysis $n]
    }
}
################################################################################
# setAnalysisPriority
#   Sets the priority class (in Windows) or nice level (in Unix)
#   of a running analysis engine.
################################################################################
proc setAnalysisPriority {w n} {
    global analysis
    
    # Get the process ID of the analysis engine:
    if {$analysis(pipe$n) == ""} { return }
    set pidlist [pid $analysis(pipe$n)]
    if {[llength $pidlist] < 1} { return }
    set pid [lindex $pidlist 0]
    
    # Set the priority class (idle or normal):
    set priority "normal"
    if {[lindex [$w.b1.priority configure -image] end] eq "tb_cpu_hi"} { set priority "idle" }
    catch {sc_info priority $pid $priority}
    
    # Re-read the priority class for confirmation:
    if {[catch {sc_info priority $pid} newpriority]} { return }
    if {$newpriority > 0} {
        $w.b1.priority configure -image tb_cpu
        $w.b1.priority state pressed
    } else {
        $w.b1.priority configure -image tb_cpu_hi
        $w.b1.priority state !pressed
    }
 }
################################################################################
# checkAnalysisStarted
#   Called a short time after an analysis engine was started
#   to send it commands if Scid has not seen any output from
#   it yet.
################################################################################
proc checkAnalysisStarted {n} {
    global analysis
    if {$analysis(seen$n)} { return }
    # Some Winboard engines do not issue any output when
    # they start up, so the fileevent above is never triggered.
    # Most, but not all, of these engines will respond in some
    # way once they have received input of some type.  This
    # proc will issue the same initialization commands as
    # those in processAnalysisInput below, but without the need
    # for a triggering fileevent to occur.
    
    logEngineNote $n {Quiet engine (still no output); sending it initial commands.}
    
    if {$analysis(uci$n)} {
        # in order to get options
        sendToEngine $n "uci"
        # egine should respond uciok
        set analysis(seen$n) 1
    } else  {
        sendToEngine $n "xboard"
        sendToEngine $n "protover 2"
        sendToEngine $n "ponder off"
        sendToEngine $n "post"
        # Prevent some engines from making an immediate "book"
        # reply move as black when position is sent later:
        sendToEngine $n "force"
    }
}
################################################################################
# with wb engines, we don't know when the startup phase is over and when the
# engine is ready : so wait for the end of initial output and take some margin
# to issue an analyze command
################################################################################
proc initialAnalysisStart {n} {
    global analysis
    
    update
    
    if { $analysis(processInput$n) == 0 } {
        after 500 initialAnalysisStart $n
        return
    }
    set cl [clock clicks -milliseconds]
    if {[expr $cl - $analysis(processInput$n)] < 1000} {
        after 200 initialAnalysisStart $n
        return
    }
    after 200 startEngineAnalysis $n 1
}
################################################################################
# processAnalysisInput (only for win/xboard engines)
#   Called from a fileevent whenever there is a line of input
#   from an analysis engine waiting to be processed.
################################################################################
proc processAnalysisInput {{n 1}} {
    global analysis
    
    # Get one line from the engine:
    set line [gets $analysis(pipe$n)]
    
    # this is only useful at startup but costs less than 10 microseconds
    set analysis(processInput$n) [clock clicks -milliseconds]
    
    logEngine $n "Engine: $line"
    
    if { ! [ checkEngineIsAlive $n ] } { return }
    
    if {! $analysis(seen$n)} {
        set analysis(seen$n) 1
        # First line of output from the program, so send initial commands:
        logEngineNote $n {First line from engine seen; sending it initial commands now.}
        sendToEngine $n "xboard"
        sendToEngine $n "protover 2"
        sendToEngine $n "ponder off"
        sendToEngine $n "post"
    }
    
    # Check for "feature" commands so we can determine if the engine
    # has the setboard and analyze commands:
    #
    if {! [string compare [string range $line 0 6] "feature"]} {
        if {[string match "*analyze=1*" $line]} { set analysis(has_analyze$n) 1 }
        if {[string match "*setboard=1*" $line]} { set analysis(has_setboard$n) 1 }
        if {[string match "*usermove=1*" $line]} { set analysis(wants_usermove$n) 1 }
        if {[string match "*sigint=1*" $line]} { set analysis(send_sigint$n) 1 }
        if {[string match "*myname=*" $line] } {
            if { !$analysis(wbEngineDetected$n) } { detectWBEngine $n $line  }
            if { [regexp "myname=\"(\[^\"\]*)\"" $line dummy name]} {
                if {$n == 1} {
                    catch {::setTitle .analysisWin$n "Analysis: $name"}
                } else {
                    catch {::setTitle .analysisWin$n "Analysis $n: $name"}
                }
            }
        }
        return
    }
    
    # Check for a line starting with "Crafty", so Scid can work well
    # with older Crafty versions that do not recognize "protover":
    #
    if {! [string compare [string range $line 0 5] "Crafty"]} {
        logEngineNote $n {Seen "Crafty"; assuming analyze and setboard commands.}
        set major 0
        if {[scan $line "Crafty v%d.%d" major minor] == 2  &&  $major >= 18} {
            logEngineNote $n {Crafty version is >= 18.0; assuming scores are from White perspective.}
            set analysis(invertScore$n) 0
        }
        # Turn off crafty logging, to reduce number of junk files:
        sendToEngine $n "log off"
        # Set a fairly low noise value so Crafty is responsive to board changes,
        # but not so low that we get lots of short-ply search data:
        sendToEngine $n "noise 1000"
        set analysis(isCrafty$n) 1
        set analysis(has_setboard$n) 1
        set analysis(has_analyze$n) 1
        return
    }
    
    # Scan the line from the engine for the analysis data:
    #
    set res [scan $line "%d%c %d %d %s %\[^\n\]\n" \
            temp_depth dummy temp_score \
            temp_time temp_nodes temp_moves]
    if {$res == 6} {
        if {$analysis(invertScore$n)  && (![string compare [sc_pos side] "black"])} {
            set temp_score [expr { 0.0 - $temp_score } ]
        }
        set analysis(depth$n) $temp_depth
        set analysis(score$n) $temp_score
        # Convert score to pawns from centipawns:
        set analysis(score$n) [expr {double($analysis(score$n)) / 100.0} ]
        set analysis(moves$n) [formatAnalysisMoves $temp_moves]
        set analysis(time$n) $temp_time
        set analysis(nodes$n) [calculateNodes $temp_nodes]
        
        # Convert time to seconds from centiseconds:
        if {! $analysis(wholeSeconds$n)} {
            set analysis(time$n) [expr {double($analysis(time$n)) / 100.0} ]
        }
        
        updateAnalysisText $n
        
        if {! $analysis(seenEval$n)} {
            # This is the first evaluation line seen, so send the current
            # position details to the engine:
            set analysis(seenEval$n) 1
        }
        
        return
    }
    
    # Check for a "stat01:" line, the reply to the "." command:
    #
    if {! [string compare [string range $line 0 6] "stat01:"]} {
        if {[scan $line "%s %d %s %d" \
                    dummy temp_time temp_nodes temp_depth] == 4} {
            set analysis(depth$n) $temp_depth
            set analysis(time$n) $temp_time
            set analysis(nodes$n) [calculateNodes $temp_nodes]
            # Convert time to seconds from centiseconds:
            if {! $analysis(wholeSeconds$n)} {
                set analysis(time$n) [expr {double($analysis(time$n)) / 100.0} ]
            }
            updateAnalysisText $n
        }
        return
    }
    
    # Check for other engine-specific lines:
    # The following checks are intended to make Scid work with
    # various WinBoard engines that are not properly configured
    # by the "feature" line checking code above.
    #
    # Many thanks to Allen Lake for testing Scid with many
    # WinBoard engines and providing this code and the detection
    # code in wbdetect.tcl
    if { !$analysis(wbEngineDetected$n) } {
        detectWBEngine $n $line
    }
    
}
################################################################################
# returns 0 if engine died abruptly or 1 otherwise
################################################################################
proc checkEngineIsAlive { {n 1} } {
    global analysis

    if {$analysis(pipe$n) == ""} { return 0 }
    
    if {[eof $analysis(pipe$n)]} {
        fileevent $analysis(pipe$n) readable {}
        set exit_status 0
        if {[catch {close $analysis(pipe$n)} standard_error] != 0} {
            global errorCode
            if {"CHILDSTATUS" == [lindex $errorCode 0]} {
                set exit_status [lindex $errorCode 2]
            }
        }
        set analysis(pipe$n) ""
        if { $exit_status != 0 } {
            logEngineNote $n {Engine terminated with exit code $exit_status: "\"$standard_error\""}
            tk_messageBox -type ok -icon info -parent . -title "Scid" \
                          -message "The analysis engine terminated with exit code $exit_status: \"$standard_error\""
        } else {
            logEngineNote $n {Engine terminated without exit code: "\"$standard_error\""}
            tk_messageBox -type ok -icon info -parent . -title "Scid" \
                          -message "The analysis engine terminated without exit code: \"$standard_error\""
        }
        catch {destroy .analysisWin$n}
        return 0
    }
    return 1
}
################################################################################
# formatAnalysisMoves:
#   Given the text at the end of a line of analysis data from an engine,
#   this proc tries to strip out some extra stuff engines add to make
#   the text more compatible for adding as a variation.
################################################################################
proc formatAnalysisMoves {text} {
    # Yace puts ".", "t", "t-" or "t+" at the start of its moves text,
    # unless directed not to in its .ini file. Get rid of it:
    if {[strIsPrefix ". " $text]} { set text [string range $text 2 end]}
    if {[strIsPrefix "t " $text]} { set text [string range $text 2 end]}
    if {[strIsPrefix "t- " $text]} { set text [string range $text 3 end]}
    if {[strIsPrefix "t+ " $text]} { set text [string range $text 3 end]}
    
    # Trim any initial or final whitespace:
    set text [string trim $text]
    
    # Yace often adds "H" after a move, e.g. "Bc4H". Remove them:
    regsub -all "H " $text " " text
    
    # Crafty adds "<HT>" for a hash table comment. Change it to "{HT}":
    regsub "<HT>" $text "{HT}" text
    
    return $text
}

set finishGameMode 0

################################################################################
# will ask engine(s) to play the game till the end
################################################################################
proc toggleFinishGame { { n 1 } } {
	global analysis
	set b ".analysisWin$n.b1.bFinishGame"
	if { $::annotateModeButtonValue || $::autoplayMode } { return }
	if { ! $analysis(uci$n) } {
		if { !$analysis(analyzeMode$n) || ! [sc_pos isAt vend] } { return }

		if {!$::finishGameMode} {
			set ::finishGameMode 1
			$b configure -image tb_finish_on
			after $::autoplayDelay autoplayFinishGame
		} else {
			set ::finishGameMode 0
			$b configure -image tb_finish_off
			after cancel autoplayFinishGame
		}
		return
	}

	# UCI engines
	# Default values
	if {! [info exists ::finishGameEng1] } { set ::finishGameEng1 1 }
	if {! [info exists ::finishGameEng2] } { set ::finishGameEng2 1 }
	if {! [info exists ::finishGameCmd1] } { set ::finishGameCmd1 "movetime" }
	if {! [info exists ::finishGameCmdVal1] } { set ::finishGameCmdVal1 5 }
	if {! [info exists ::finishGameCmd2] } { set ::finishGameCmd2 "movetime" }
	if {! [info exists ::finishGameCmdVal2] } { set ::finishGameCmdVal2 5 }
	if {! [info exists ::finishGameAnnotate] } { set ::finishGameAnnotate 1 }
	if {! [info exists ::finishGameAnnotateShort] } { set ::finishGameAnnotateShort 1 }
	# On exit save values in options.dat
	options.save ::finishGameEng1
	options.save ::finishGameEng2
	options.save ::finishGameCmd1
	options.save ::finishGameCmdVal1
	options.save ::finishGameCmd2
	options.save ::finishGameCmdVal2
	options.save ::finishGameAnnotate
	options.save ::finishGameAnnotateShort

	if {$::finishGameMode} {
		set ::finishGameMode 0
		sendToEngine 1 "stop"
		set analysis(waitForReadyOk1) 0
		set analysis(waitForBestMove1) 0
		sendToEngine 2 "stop"
		set analysis(waitForReadyOk2) 0
		set analysis(waitForBestMove2) 0
		$b configure -image tb_finish_off
		grab release .analysisWin$n
		.analysisWin$n.b1.bStartStop configure -state normal
		.analysisWin$n.b1.move configure -state normal
		.analysisWin$n.b1.line configure -state normal
		.analysisWin$n.b1.alllines configure -state normal
		.analysisWin$n.b1.annotate configure -state normal
		.analysisWin$n.b1.automove configure -state normal
		return
	}

	set w .configFinishGame
	win::createDialog $w
	wm resizable $w 0 0
	::setTitle $w "Scid: $::tr(FinishGame)"

	ttk::labelframe $w.wh_f -text "$::tr(White)" -padding 5
	grid $w.wh_f -column 0 -row 0 -columnspan 2 -sticky we -pady 8
	ttk::label $w.wh_f.p -image wk$::board::_size(.main.board)
	grid $w.wh_f.p -column 0 -row 0 -rowspan 3
	ttk::radiobutton $w.wh_f.e1 -text $analysis(name1) -variable ::finishGameEng1 -value 1
	if {[winfo exists .analysisWin2] && $analysis(uci2) } {
		ttk::radiobutton $w.wh_f.e2 -text $analysis(name2) -variable ::finishGameEng1 -value 2
	} else {
		set ::finishGameEng1 1
		ttk::radiobutton $w.wh_f.e2 -text $::tr(StartEngine) -variable ::finishGameEng1 -value 2 -state disabled
	}
	grid $w.wh_f.e1 -column 1 -row 0 -columnspan 3 -sticky w
	grid $w.wh_f.e2 -column 1 -row 1 -columnspan 3 -sticky w
	ttk::spinbox $w.wh_f.cv -width 4 -textvariable ::finishGameCmdVal1 -from 1 -to 999
	ttk::radiobutton $w.wh_f.c1 -text $::tr(seconds) -variable ::finishGameCmd1 -value "movetime"
	ttk::radiobutton $w.wh_f.c2 -text $::tr(FixedDepth) -variable ::finishGameCmd1 -value "depth"
	grid $w.wh_f.cv -column 1 -row 2 -sticky w
	grid $w.wh_f.c1 -column 2 -row 2 -sticky w
	grid $w.wh_f.c2 -column 3 -row 2 -sticky w
	grid columnconfigure $w.wh_f 2 -weight 1

	ttk::labelframe $w.bk_f -text "$::tr(Black)" -padding 5
	grid $w.bk_f -column 0 -row 1 -columnspan 2 -sticky we -pady 8
	ttk::label $w.bk_f.p -image bk$::board::_size(.main.board)
	grid $w.bk_f.p -column 0 -row 0 -rowspan 3
	ttk::radiobutton $w.bk_f.e1 -text $analysis(name1) -variable ::finishGameEng2 -value 1
	if {[winfo exists .analysisWin2] && $analysis(uci2) } {
		ttk::radiobutton $w.bk_f.e2 -text $analysis(name2) -variable ::finishGameEng2 -value 2
	} else {
		set ::finishGameEng2 1
		ttk::radiobutton $w.bk_f.e2 -text $::tr(StartEngine) -variable ::finishGameEng2 -value 2 -state disabled
	}
	grid $w.bk_f.e1 -column 1 -row 0 -columnspan 3 -sticky w
	grid $w.bk_f.e2 -column 1 -row 1 -columnspan 3 -sticky w
	ttk::spinbox $w.bk_f.cv -width 4 -textvariable ::finishGameCmdVal2 -from 1 -to 999
	ttk::radiobutton $w.bk_f.c1 -text $::tr(seconds) -variable ::finishGameCmd2 -value "movetime"
	ttk::radiobutton $w.bk_f.c2 -text $::tr(FixedDepth) -variable ::finishGameCmd2 -value "depth"
	grid $w.bk_f.cv -column 1 -row 2 -sticky w
	grid $w.bk_f.c1 -column 2 -row 2 -sticky w
	grid $w.bk_f.c2 -column 3 -row 2 -sticky w
	grid columnconfigure $w.bk_f 2 -weight 1

	ttk::checkbutton $w.annotate -text $::tr(Annotate) -variable ::finishGameAnnotate
	grid $w.annotate -column 0 -row 2 -sticky w -padx 5 -pady 8
	ttk::checkbutton $w.annotateShort -text $::tr(ShortAnnotations) -variable ::finishGameAnnotateShort
	grid $w.annotateShort -column 1 -row 2 -sticky w -padx 5 -pady 8

	ttk::frame $w.fbuttons
	ttk::button $w.fbuttons.cancel -text $::tr(Cancel) -command { destroy .configFinishGame }
	ttk::button $w.fbuttons.ok -text "OK" -command {
		if {$::finishGameEng1 == $::finishGameEng2} {
			set ::finishGameMode 1
		} else {
			set ::finishGameMode 2
		}
		set tmp [sc_pos getComment]
		sc_pos setComment "$tmp $::tr(FinishGame) $::tr(White): $analysis(name$::finishGameEng1) $::tr(Black): $analysis(name$::finishGameEng2)"
		destroy .configFinishGame
	}
	packbuttons right $w.fbuttons.cancel $w.fbuttons.ok
	grid $w.fbuttons -row 3 -column 1 -columnspan 2 -sticky we
	focus $w.fbuttons.ok
	bind $w <Escape> { .configFinishGame.cancel invoke }
	bind $w <Return> { .configFinishGame.ok invoke }
	bind $w <Destroy> { focus .analysisWin1 }
	::tk::PlaceWindow $w widget .analysisWin1
	grab $w
	bind $w <ButtonPress> {
		set w .configFinishGame
		if {%x < 0 || %x > [winfo width $w] || %y < 0 || %y > [winfo height $w] } { ::tk::PlaceWindow $w pointer }
	}
	tkwait window $w
	if {!$::finishGameMode} { return }

	set gocmd(1) "go $::finishGameCmd1 $::finishGameCmdVal1"
	set gocmd(2) "go $::finishGameCmd2 $::finishGameCmdVal2"
	if {$::finishGameCmd1 == "movetime" } { append gocmd(1) "000" }
	if {$::finishGameCmd2 == "movetime" } { append gocmd(2) "000" }
	if {[sc_pos side] == "white"} {
		set current_cmd 1
		set current_engine $::finishGameEng1
	} else {
		set current_cmd 2
		set current_engine $::finishGameEng2
	}

	stopEngineAnalysis 1
	stopEngineAnalysis 2
	$b configure -image tb_finish_on
	.analysisWin$n.b1.bStartStop configure -state disabled
	.analysisWin$n.b1.move configure -state disabled
	.analysisWin$n.b1.line configure -state disabled
	.analysisWin$n.b1.alllines configure -state disabled
	.analysisWin$n.b1.annotate configure -state disabled
	.analysisWin$n.b1.automove configure -state disabled
	grab .analysisWin$n

	while { [string index [sc_game info previousMove] end] != "#"} {
		set analysis(waitForReadyOk$current_engine) 1
		sendToEngine $current_engine "isready"
		vwait analysis(waitForReadyOk$current_engine)
		if {!$::finishGameMode} { break }
		sendToEngine $current_engine "position fen [sc_pos fen]"
		sendToEngine $current_engine $gocmd($current_cmd)
		set analysis(fen$current_engine) [sc_pos fen]
		set analysis(maxmovenumber$current_engine) 0
		set analysis(waitForBestMove$current_engine) 1
		vwait analysis(waitForBestMove$current_engine)
		if {!$::finishGameMode} { break }

		if { ! [sc_pos isAt vend] } { sc_var create }
		if {$::finishGameAnnotate} {
			set moves [ lindex [ lindex $analysis(multiPV$current_engine) 0 ] 2 ]
			if {$::finishGameAnnotateShort} {
				set text [format "%d:%+.2f" \
					$analysis(depth$current_engine) \
					$analysis(score$current_engine) ]
				makeAnalysisMove $current_engine $text
			} else {
				set text [format "%d:%+.2f" \
					$analysis(depth$current_engine) \
					$analysis(score$current_engine) ]
				makeAnalysisMove $current_engine $text
				sc_var create
				set moves $analysis(moves$current_engine)
				sc_move_add $moves $current_engine
				sc_var exit
				sc_move forward
			}
			storeEmtComment 0 0 [expr {int($analysis(time$current_engine))}]
		} else {
			makeAnalysisMove $current_engine
		}

		incr current_cmd
		if {$current_cmd > 2} { set current_cmd 1 }
		if {$::finishGameMode == 2} {
			incr current_engine
			if {$current_engine > 2 } { set current_engine 1 }
		}
	}
	if {$::finishGameMode} { toggleFinishGame }
}
################################################################################
#
################################################################################
proc autoplayFinishGame { {n 1} } {
    if {!$::finishGameMode || ![winfo exists .analysisWin$n]} {return}
    .analysisWin$n.b1.move invoke
    if { [string index [sc_game info previousMove] end] == "#"} {
        toggleFinishGame $n
        return
    }
    after $::autoplayDelay autoplayFinishGame
}

################################################################################
#
################################################################################
proc startEngineAnalysis { {n 1} {force 0} } {
    global analysis
    
    if { !$analysis(analyzeMode$n) } {
        set b ".analysisWin$n.b1.bStartStop"
        
        startAnalyzeMode $n $force
        $b configure -image tb_eng_off
        ::utils::tooltip::Set $b "$::tr(StopEngine)(a)"
        # enable lock button
        .analysisWin$n.b1.lockengine configure -state normal
    }
}

################################################################################
#
################################################################################
proc stopEngineAnalysis { {n 1} } {
    global analysis
    
    if { $analysis(analyzeMode$n) } {
        set b ".analysisWin$n.b1.bStartStop"

        stopAnalyzeMode $n
        $b configure -image tb_eng_on
        ::utils::tooltip::Set $b "$::tr(StartEngine)"
        # reset lock mode and disable lock button
        set analysis(lockEngine$n) 1
        toggleLockEngine $n
        .analysisWin$n.b1.lockengine configure -state disabled
    }
}

################################################################################
#
################################################################################
proc toggleEngineAnalysis { { n 1 } { force 0 } } {
    global analysis
    
    if { $n == 1} {
        if { ($::annotateMode || $::finishGameMode) && ! $force } {
            return
        }
    }
    
    if {$analysis(analyzeMode$n)} {
        stopEngineAnalysis $n
    } else  {
        startEngineAnalysis $n $force
    }
}
################################################################################
# startAnalyzeMode:
#   Put the engine in analyze mode.
################################################################################
proc startAnalyzeMode {{n 1} {force 0}} {
    global analysis

    # Check that the engine has not already had analyze mode started:
    if {$analysis(analyzeMode$n) && ! $force } { return }
    set analysis(analyzeMode$n) 1
    if { $analysis(uci$n) } {
        updateAnalysis $n
    } else  {
        if {$analysis(has_setboard$n)} {
            sendToEngine $n "setboard [sc_pos fen]"
        }
        if { $analysis(has_analyze$n) } {
            sendToEngine $n "analyze"
        } else  {
            updateAnalysis $n ;# in order to handle special cases (engines without setboard and analyse commands)
        }
    }
}
################################################################################
# stopAnalyzeMode
################################################################################
proc stopAnalyzeMode { {n 1} } {
    global analysis
    if {! $analysis(analyzeMode$n)} { return }
    set analysis(analyzeMode$n) 0
    if { $analysis(uci$n) } {
        ::uci::sendStop $n
    } else  {
        sendToEngine $n "exit"
    }
    set analysis(fen$n) {}
}
################################################################################
# toggleLockEngine
#   Toggle whether engine is locked to current position.
################################################################################
proc toggleLockEngine {n} {
    global analysis
    if { $analysis(lockEngine$n) } {
	set analysis(lockEngine$n) 0
    } else {
	set analysis(lockEngine$n) 1
    }
    if { $analysis(lockEngine$n) } {
        set state disabled
        set analysis(lockN$n) [sc_pos moveNumber]
        set analysis(lockSide$n) [sc_pos side]
	.analysisWin$n.b1.lockengine state pressed
    } else {
        set state normal
	.analysisWin$n.b1.lockengine state !pressed
    }
    set w ".analysisWin$n"
    $w.b1.move configure -state $state
    $w.b1.line configure -state $state
    if {$analysis(uci$n)} {
        $w.b1.multipv configure -state $state
    }
    $w.b1.alllines configure -state $state
    $w.b1.automove configure -state $state
    if { $n == 1 } {
        $w.b1.annotate configure -state $state
        $w.b1.bFinishGame configure -state $state
    }
    updateAnalysis $n
}
################################################################################
# updateAnalysisText
#   Update the text in an analysis window.
################################################################################
proc updateAnalysisText {{n 1}} {
    global analysis
    
    set nps 0
    if {$analysis(currmovenumber$n) > $analysis(maxmovenumber$n) } {
        set analysis(maxmovenumber$n) $analysis(currmovenumber$n)
    }
    if {$analysis(time$n) > 0.0} {
        set nps [expr {round($analysis(nodes$n) / $analysis(time$n))} ]
    }
    set score $analysis(score$n)
    
    set t .analysisWin$n.text
    set h .analysisWin$n.hist.text
    
    $t configure -state normal
    $t delete 0.0 end
    
    if { $analysis(uci$n) } {
        if { [expr abs($score)] >= 327.0 } {
            if { [catch { set tmp [format "M%d " $analysis(scoremate$n)]} ] } {
                set tmp [format "%+.1f " $score]
            }
        } else {
            set tmp [format "%+.1f " $score]
        }
        $t insert end $tmp
        
        $t insert end "[tr Depth]: "
        if {$analysis(showEngineInfo$n) && $analysis(seldepth$n) != 0} {
            $t insert end [ format "%2u/%u " $analysis(depth$n) $analysis(seldepth$n)] small
        } else {
            $t insert end [ format "%2u " $analysis(depth$n) ] small
        }
        $t insert end "[tr Nodes]: "
        $t insert end [ format "%6uK (%u kn/s) " $analysis(nodes$n) $nps ] small
        $t insert end "[tr Time]: "
        $t insert end [ format "%6.2f s" $analysis(time$n) ] small
        if {$analysis(showEngineInfo$n)} {
            $t insert end "\n" small
            $t insert end "[tr Current]: "
            $t insert end [ format "%s (%s/%s) " [::trans $analysis(currmove$n)] $analysis(currmovenumber$n) $analysis(maxmovenumber$n)] small
            $t insert end "TB Hits: "
            $t insert end [ format "%u " $analysis(tbhits$n)] small
            $t insert end "Nps: "
            $t insert end [ format "%u n/s " $analysis(nps$n)] small
            $t insert end "Hash Full: "
            set hashfull [expr {round($analysis(hashfull$n) / 10)}]
            $t insert end [ format "%u%% " $hashfull ] small
            $t insert end "CPU Load: "
            set cpuload [expr {round($analysis(cpuload$n) / 10)}]
            $t insert end [ format "%u%% " $cpuload ] small
            
            #$t insert end [ format "\nCurrent: %s (%s) - Hashfull: %u - nps: %u - TBhits: %u - CPUload: %u" $analysis(currmove$n) $analysis(currmovenumber$n) $analysis(hashfull$n) $analysis(nps$n) $analysis(tbhits$n) $analysis(cpuload$n) ]
        }
    } else {
        set newStr [format "Depth:   %6u      Nodes: %6uK (%u kn/s)\n" $analysis(depth$n) $analysis(nodes$n) $nps]
        append newStr [format "Score: %+8.2f      Time: %9.2f seconds\n" $score $analysis(time$n)]
        $t insert 1.0 $newStr small
    }
    
    
    if {$analysis(automove$n)} {
        if {$analysis(automoveThinking$n)} {
            set moves "   Thinking..... "
        } else {
            set moves "   Your move..... "
        }
        
        if { ! $analysis(uci$n) } {
            $t insert end $moves blue
        }
        $t configure -state disabled
        updateAnalysisBoard $n ""
        return
    }
    
    if {! $::analysis(movesDisplay$n)}  {
        $h configure -state normal
        $h delete 0.0 end
        $h insert end "     $::tr(ClickHereToSeeMoves)\n" blue
        updateAnalysisBoard $n ""
        $h configure -state disabled
        return
    }
    
    if { $analysis(uci$n) } {
        set moves [ lindex [ lindex $analysis(multiPV$n) 0 ] 2 ]
    } else  {
        set moves $analysis(moves$n)
    }
    
    $h configure -state normal
    set cleared 0
    if { $analysis(depth$n) < $analysis(prev_depth$n)  || $analysis(prev_depth$n) == 0 } {
        $h delete 1.0 end
        set cleared 1
    }
    
    ################################################################################
    if { $analysis(uci$n) } {
        if {$cleared} { set analysis(multiPV$n) {} ; set analysis(multiPVraw$n) {} }
        if {$analysis(multiPVCount$n) == 1} {
            set newhst [format "%2d %s %s" $analysis(depth$n) [scoreToMate $score $moves $n] [addMoveNumbers $n [::trans $moves]]]
            if {$newhst != $analysis(lastHistory$n) && $moves != ""} {
                $h insert end [format "%s (%.2f)\n" $newhst $analysis(time$n)] indent
                $h see end-1c
                set analysis(lastHistory$n) $newhst
            }
        } else {
            $h delete 1.0 end
            # First line
            set pv [lindex $analysis(multiPV$n) 0]
            if { $pv != "" } {
                catch { set newStr [format "%2d %s " [lindex $pv 0] [scoreToMate $score [lindex $pv 2] $n] ] }
            
                $h insert end "1 " gray
                append newStr "[addMoveNumbers $n [::trans [lindex $pv 2]]] [format (%.2f)\n [lindex $pv 4]]"
                $h insert end $newStr blue
            
                set lineNumber 1
                foreach pv $analysis(multiPV$n) {
                    if {$lineNumber == 1} { incr lineNumber ; continue }
                    $h insert end "$lineNumber " gray
                    set score [scoreToMate [lindex $pv 1] [lindex $pv 2] $n]
                    $h insert end [format "%2d %s %s (%.2f)\n" [lindex $pv 0] $score [addMoveNumbers $n [::trans [lindex $pv 2]]] [lindex $pv 4]] indent
                    incr lineNumber
                }
            }
        }
        ################################################################################
    } else  {
        # original Scid analysis display
        $h insert end [format "%2d %+5.2f %s (%.2f)\n" $analysis(depth$n) $score [::trans $moves] $analysis(time$n)] indent
        $h see end-1c
    }
    
    $h configure -state disabled
    set analysis(prev_depth$n) $analysis(depth$n)
    if { ! $analysis(uci$n) } {
        $t insert end [::trans $moves] blue
    }
    # $t tag add score 2.0 2.13
    $t configure -state disabled
    
    updateAnalysisBoard $n $analysis(moves$n)
}
################################################################################
# args = score, pv
# returns M X if mate detected (# or ++) or original score
################################################################################
proc scoreToMate { score pv n } {
    
    if {$::analysis(lockEngine$n)} {
        return [format "%+5.2f" $score]
    }
    
    if { [string index $pv end] == "#" || [string index $pv end] == "+" && [string index $pv end-1] == "+"} {
        set plies [llength $pv]
        
        set mate [expr $plies / 2 + 1 ]
        
        set sign ""
        if {[expr $plies % 2] == 0 && [sc_pos side] == "white" || [expr $plies % 2] == 1 && [sc_pos side] == "black"} {
            set sign "-"
        }
        if {[sc_pos side] == "white" } {
            if { $sign == "" } {
                set mate [expr $plies / 2 + 1 ]
            } else  {
                set mate [expr $plies / 2 ]
            }
        } else  {
            if { $sign == "" } {
                set mate [expr $plies / 2 ]
            } else  {
                set mate [expr $plies / 2 + 1 ]
            }
        }
        
        set ret "M$sign$mate"
    } else  {
        set ret [format "%+5.2f" $score]
    }
    
    return $ret
}
################################################################################
# returns the pv with move numbers added
# ::pgn::moveNumberSpaces controls space between number and move
################################################################################
proc addMoveNumbers { e pv } {
    global analysis

    if { $analysis(lockEngine$e) } {
      set n $analysis(lockN$e)
      set turn $analysis(lockSide$e)
    } else {
      set n [sc_pos moveNumber]
      set turn [sc_pos side]
    }

    if {$::pgn::moveNumberSpaces} {
      set spc { }
    } else {
      set spc {}
    }

    set ret ""
    set start 0
    if {$turn == "black"} {
        set ret "$n.$spc... [lindex $pv 0] "
        incr start
        incr n
    }
    for {set i $start} {$i < [llength $pv]} {incr i} {
        set m [lindex $pv $i]
        if { [expr $i % 2] == 0 && $start == 0 || [expr $i % 2] == 1 && $start == 1 } {
            append ret "$n.$spc$m "
        } else  {
            append ret "$m "
            incr n
        }
    }
    return $ret
}
################################################################################
# toggleAnalysisBoard
#   Toggle whether the small analysis board is shown.
################################################################################
proc toggleAnalysisBoard {n} {
    global analysis
    if { $analysis(showBoard$n) } {
        set analysis(showBoard$n) 0
        pack forget .analysisWin$n.bd
        setWinSize .analysisWin$n
	.analysisWin$n.b1.showboard state !pressed
    } else {
        bind .analysisWin$n <Configure> ""
        set analysis(showBoard$n) 1
        pack .analysisWin$n.bd -side right -before .analysisWin$n.b1 -padx 4 -pady 4 -anchor n
        update
        .analysisWin$n.hist.text configure -setgrid 0
        .analysisWin$n.text configure -setgrid 0
        set x [winfo reqwidth .analysisWin$n]
        set y [winfo reqheight .analysisWin$n]
        wm geometry .analysisWin$n ${x}x${y}
        .analysisWin$n.hist.text configure -setgrid 1
        .analysisWin$n.text configure -setgrid 1
	.analysisWin$n.b1.showboard state pressed
    }
}
################################################################################
# toggleEngineInfo
#   Toggle whether engine info are shown.
################################################################################
proc toggleEngineInfo {n} {
    global analysis
    if { $analysis(showEngineInfo$n) } {
	set analysis(showEngineInfo$n) 0
        .analysisWin$n.text configure -height 1
	.analysisWin$n.b1.showinfo state !pressed
    } else {
	set analysis(showEngineInfo$n) 1
        .analysisWin$n.text configure -height 2
	.analysisWin$n.b1.showinfo state pressed
    }
    updateAnalysisText $n
}
################################################################################
#
################################################################################
# updateAnalysisBoard
#   Update the small analysis board in the analysis window,
#   showing the position after making the specified moves
#   from the current main board position.
#
proc updateAnalysisBoard {n moves} {
    global analysis
    # PG : this should not be commented
    if {! $analysis(showBoard$n)} { return }
    
    set bd .analysisWin$n.bd
    # Push a temporary copy of the current game:
    sc_game push copyfast
    
    # Make the engine moves and update the board:
    sc_move_add $moves $n
    ::board::update $bd [sc_pos board]
    
    # Pop the temporary game:
    sc_game pop
}

################################################################################
# updateAnalysis
#   Update an analysis window by sending the current board
#   to the engine.
################################################################################
proc updateAnalysis {{n 1}} {
    global analysis
    if {$analysis(pipe$n) == ""} { return }
    # Just return if no output has been seen from the analysis program yet:
    if {! $analysis(seen$n)} { return }
    # No need to update if no analysis is running
    if { ! $analysis(analyzeMode$n) } { return }
    # No need to send current board if engine is locked
    if { $analysis(lockEngine$n) } { return }

    if { $analysis(uci$n) } {
        set analysis(depth$n) 0
        set analysis(multiPV$n) {}
        set analysis(multiPVraw$n) {}
        set analysis(fen$n) [sc_pos fen]
        set analysis(maxmovenumber$n) 0
        set analysis(movelist$n) [sc_game UCI_currentPos]
        set analysis(nonStdStart$n) [sc_game startBoard]
        ::uci::sendPositionGo $n "infinite"
    } else {
        #TODO: remove 0.3s delay even for other engines

        global analysisWin windowsOS

        # If too close to the previous update, and no other future update is
        # pending, reschedule this update to occur in another 0.3 seconds:
        #
        if {[catch {set clicks [clock clicks -milliseconds]}]} {
            set clicks [clock clicks]
        }
        set diff [expr {$clicks - $analysis(lastClicks$n)} ]
        if {$diff < 300  &&  $diff >= 0} {
            if {$analysis(after$n) == ""} {
                set analysis(after$n) [after 300 updateAnalysis $n]
            }
            return
        }
        set analysis(lastClicks$n) $clicks
        set analysis(after$n) ""
        after cancel updateAnalysis $n

        set old_movelist $analysis(movelist$n)
        set movelist [sc_game moves coord list]
        set analysis(movelist$n) $movelist
        set nonStdStart [sc_game startBoard]
        set old_nonStdStart $analysis(nonStdStart$n)
        set analysis(nonStdStart$n) $nonStdStart

        # This section is for engines that support "analyze":
        if {$analysis(has_analyze$n)} {
            sendToEngine $n "exit"   ;# Get out of analyze mode, to send moves.
            
            # On Crafty, "force" command has different meaning when not in
            # XBoard mode, and some users have noticed Crafty not being in
            # that mode at this point -- although I cannot reproduce this.
            # So just re-send "xboard" to Crafty to make sure:
            if {$analysis(isCrafty$n)} { sendToEngine $n "xboard" }
            
            sendToEngine $n "force"  ;# Stop engine replying to moves.
            # Check if the setboard command must be used -- that is, if the
            # previous or current position arose from a non-standard start.
            
            #if {$analysis(has_setboard$n)  &&  ($old_nonStdStart  || $nonStdStart)}
            # We skip all code below if the engine has setboard capability : this is provides less error prone behavior
            if {$analysis(has_setboard$n)} {
                sendToEngine $n "setboard [sc_pos fen]"
                # Most engines with setboard do not recognize the crafty "mn"
                # command (it is not in the XBoard/WinBoard protocol), so only send it to crafty:
                if {$analysis(isCrafty$n)} { sendToEngine $n "mn [sc_pos moveNumber]" }
                sendToEngine $n "analyze"
                return
            }
            
            # If we need a non-standard start and the engine does not have
            # setboard, the user is out of luck:
            if {$nonStdStart} {
                set analysis(moves$n) "  Sorry, this game has a non-standard start position."
                updateAnalysisText $n
                return
            }
            
            # Here, the engine has the analyze command (and no setboard) but this game does
            # not have a non-standard start position.
            
            set oldlen [llength $old_movelist]
            set newlen [llength $movelist]
            
            # Check for optimization to minimize the commands to be sent:
            # Scid sends "undo" to backup wherever possible, and avoid "new" as
            # on many engines this would clear hash tables, causing poor
            # hash table performance.
            
            # Send just the new move if possible (if the new move list is exactly
            # the same as the previous move list, with one extra move):
            if {($newlen == $oldlen + 1) && ($old_movelist == [lrange $movelist 0 [expr {$oldlen - 1} ]])} {
                sendMoveToEngine $n [lindex $movelist $oldlen]
                
            } elseif {($newlen + 1 == $oldlen) && ($movelist == [lrange $old_movelist 0 [expr {$newlen - 1} ]])} {
                # Here the new move list is the same as the old list but with one
                # less move, just send one "undo":
                sendToEngine $n "undo"
                
            } elseif {$newlen == $oldlen  &&  $old_movelist == $movelist} {
                
                # Here the board has not changed, so send nothing
                
            } else {
                
                # Otherwise, undo and re-send all moves:
                for {set i 0} {$i < $oldlen} {incr i} {
                    sendToEngine $n "undo"
                }
                foreach m $movelist {
                    sendMoveToEngine $n $m
                }
                
            }
            
            sendToEngine $n "analyze"
            
        } else {
            
            # This section is for engines without the analyze command:
            # In this case, Scid just sends "new", "force" and a bunch
            # of moves, then sets a very long search time/depth and
            # sends "go". This is not ideal but it works OK for engines
            # without "analyze" that I have tried.
            
            # If Unix OS and engine wants it, send an INT signal:
            if {(!$windowsOS)  &&  $analysis(send_sigint$n)} {
                catch {exec -- kill -s INT [pid $analysis(pipe$n)]}
            }
            sendToEngine $n "new"
            sendToEngine $n "force"
            if { $nonStdStart && ! $analysis(has_setboard$n) } {
                set analysis(moves$n) "  Sorry, this game has a non-standard start position."
                updateAnalysisText $n
                return
            }
            if {$analysis(has_setboard$n)} {
                sendToEngine $n "setboard [sc_pos fen]"
            } else  {
                foreach m $movelist {
                    sendMoveToEngine $n $m
                }
            }
            # Set engine to be white or black:
            sendToEngine $n [sc_pos side]
            # Set search time and depth to something very large and start search:
            sendToEngine $n "st 120000"
            sendToEngine $n "sd 50"
            sendToEngine $n "post"
            sendToEngine $n "go"
        }
    }
}
################################################################################
#
################################################################################

set temptime 0
trace variable temptime w {::utils::validate::Regexp {^[0-9]*\.?[0-9]*$}}

proc setAutomoveTime {{n 1}} {
    global analysis temptime dialogResult
    set ::tempn $n
    set temptime [expr {$analysis(automoveTime$n) / 1000.0} ]
    set w .apdialog
    win::createDialog $w
    #wm transient $w .analysisWin
    ::setTitle $w "Scid: Engine thinking time"
    wm resizable $w 0 0
    ttk::frame $w.f
    pack $w.f -expand 1
    ttk::label $w.f.label -text "Set the engine thinking time per move in seconds:"
    pack $w.f.label -side top -pady 5 -padx 5
    ttk::entry $w.f.entry -background white -width 10 -textvariable temptime
    pack $w.f.entry -side top -pady 5
    bind $w.f.entry <Escape> { .apdialog.buttons.cancel invoke }
    bind $w.f.entry <Return> { .apdialog.buttons.ok invoke }
    
    addHorizontalRule $w
    
    set dialogResult ""
    set b [ttk::frame $w.buttons]
    pack $b -side top -fill x
    ttk::button $b.cancel -text $::tr(Cancel) -command {
        focus .
        catch {grab release .apdialog}
        destroy .apdialog
        focus .
        set dialogResult Cancel
    }
    ttk::button $b.ok -text "OK" -command {
        catch {grab release .apdialog}
        if {$temptime < 0.1} { set temptime 0.1 }
        set analysis(automoveTime$tempn) [expr {int($temptime * 1000)} ]
        focus .
        catch {grab release .apdialog}
        destroy .apdialog
        focus .
        set dialogResult OK
    }
    pack $b.cancel $b.ok -side right -padx 5 -pady 5
    focus $w.f.entry
    update
    catch {grab .apdialog}
    tkwait window .apdialog
    if {$dialogResult != "OK"} {
        return 0
    }
    return 1
}

proc toggleAutomove {{n 1}} {
    global analysis
    .analysisWin1.b1.automove state !pressed
    if { $analysis(automove$n) } {
	set analysis(automove$n) 0
        cancelAutomove $n
    } else {
        set analysis(automove$n) 0
        if {! [setAutomoveTime $n]} {
            return
        }
        set analysis(automove$n) 1
	.analysisWin1.b1.automove state pressed
        automove $n
    }
}

proc cancelAutomove {{n 1}} {
    global analysis
    set analysis(automove$n) 0
    after cancel "automove $n"
    after cancel "automove_go $n"
}

proc automove {{n 1}} {
    global analysis autoplayDelay
    if {! $analysis(automove$n)} { return }
    after cancel "automove $n"
    set analysis(automoveThinking$n) 1
    after $analysis(automoveTime$n) "automove_go $n"
}

proc automove_go {{n 1}} {
    global analysis
    if {$analysis(automove$n)} {
        if {[makeAnalysisMove $n]} {
            set analysis(autoMoveThinking$n) 0
            updateBoard -pgn
            after cancel "automove $n"
            ::tree::doTraining $n
        } else {
            after 1000 "automove $n"
        }
    }
}
################################################################################
# If UCI engine, add move through a dedicated function in uci namespace
# returns the error caught by catch
################################################################################
proc sc_move_add { moves n } {
    if { $::analysis(uci$n) } {
        return [::uci::sc_move_add $moves]
    } else  {
        return [ catch { sc_move addSan $moves } ]
    }
}
################################################################################
# append scid directory if path starts with .
################################################################################
proc toAbsPath { path } {
    set new $path
    if {[string index $new 0] == "." } {
        set scidInstallDir [file dirname [info nameofexecutable] ]
        set new [ string replace $new 0 0  $scidInstallDir ]
    }
    return $new
}
################################################################################
#
################################################################################

###
### End of file: analysis.tcl
###
###
