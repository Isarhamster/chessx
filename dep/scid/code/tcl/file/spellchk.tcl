### spellchk.tcl
### Part of Scid.
### Copyright (C) 2000-2003 Shane Hudson.

set spellcheckType Player

# Maximum nr of corrections to be scanned
# Set to zero to find them all
# Set to some positive number to limit
#
set spell_maxCorrections 0

set spellcheckSurnames 1
set spellcheckAmbiguous 0

# Remember what we are doing, being
# - "idle"       - nothing special
# - "scanning"   - finding corrections
# - "correcting" - making corrections
#
set spellstate idle

# readSpellCheckFile:
#    Presents a File Open dialog box for a Scid spellcheck file,
#    then tries to read the file. If the parameter "message" is true
#    (which is the default), a message box indicating the results
#    is displayed.
#
proc getSpellCheckFile { widget } {
    global spellCheckFile
    set ftype { { "Scid Spellcheck files" {".ssp"} } }
    set fullname [tk_getOpenFile -initialdir [file dirname $spellCheckFile] -filetypes $ftype -title "Open Spellcheck file" -parent [winfo toplevel $widget]]
    if { $fullname != "" && [readSpellCheckFile $fullname] } {
        $widget delete 0 end
        $widget insert end $fullname
    }
}

proc readSpellCheckFile { fullname {message 1}} {
  global spellCheckFile

    if { $fullname != ""} {
        progressWindow "Scid - [tr Spellcheking]" "Loading $fullname ..."
        set err [catch {sc_name read $fullname} result]
        closeProgressWindow
        if {$err} {
            if {$message} {
                tk_messageBox -title "ERROR: Unable to read file" -type ok -parent .resDialog \
                    -icon error -message "Scid could not correctly read the spellcheck file you selected:\n\n$result\n$fullname"
            }
            return 0
        }
        if {$message} {
            tk_messageBox -title "Spellcheck file loaded." -type ok -icon info -parent .resDialog \
                -message "Spellcheck file [file tail $fullname] loaded:\n[lindex $result 0] players, [lindex $result 1] events, [lindex $result 2] sites, [lindex $result 3] rounds.\n\nTo have this file automatically loaded every time you start Scid, select the \"Save Options\" from the Options menu before exiting."
        }
    }
    set spellCheckFile $fullname
    return 1
}

# Set the environment when the correction scan starts
#
proc startScanning {} {
    global spellstate
    global spellcheckType
    
    # Remember that we are scanning
    #
    set spellstate scanning
    
    # Disable all buttons except the cancel button that we
    # transfer into a stop button
    #
    .spellcheckWin.buttons.ambig  configure -state disabled
    .spellcheckWin.buttons.ok     configure -state disabled
    .spellcheckWin.buttons.cancel configure -text "Stop"
    bind .spellcheckWin <Alt-s> ".spellcheckWin.buttons.cancel invoke; break"
    if {$spellcheckType == "Player"} {
        .spellcheckWin.buttons.surnames configure -state disabled
    }
}

# Set the environment when the correction scan stops
#
proc stopScanning {} {
    global spellstate
    global spellcheckType
    
    # Remember that we are not scanning
    #
    set spellstate idle
    
    # Enable all buttons and set the cancel button back
    #
    .spellcheckWin.buttons.ambig  configure -state enabled
    .spellcheckWin.buttons.ok     configure -state enabled
    .spellcheckWin.buttons.cancel configure -text $::tr(Cancel)
    bind .spellcheckWin <Alt-c> ".spellcheckWin.buttons.cancel invoke; break"
    if {$spellcheckType == "Player"} {
        .spellcheckWin.buttons.surnames configure -state enabled
    }
}


# Set the environment when correction starts
#
proc startCorrecting {} {
    global spellstate
    global spellcheckType
    
    # Remember that we are correcting
    #
    set spellstate correcting
    
    # Disable all buttons
    #
    .spellcheckWin.buttons.ambig  configure -state disabled
    .spellcheckWin.buttons.ok     configure -state disabled
    .spellcheckWin.buttons.cancel configure -state disabled

    if {$spellcheckType == "Player"} {
        .spellcheckWin.buttons.surnames configure -state disabled
    }
}


# Start the correction scan and dump the results into the
# text window. After this the user may edit the correction
# 'script' and actually make the corrections.
#
# While the scan is running, all buttons except a stop button
# are disabled.
#
proc updateSpellCheckWin {type} {
    global spellcheckType spell_maxCorrections spellcheckSurnames
    global spellcheckAmbiguous

    .spellcheckWin.text.text delete 1.0 end
    .spellcheckWin.text.text insert end "Scid is finding spelling corrections.\nPlease wait..."

    # Enable the progress bar
    #

    startScanning

    update idletasks
    progressBarSet .spellcheckWin.progress 451 21
    set err [catch {sc_name spellcheck -max $spell_maxCorrections \
                                   -surnames $spellcheckSurnames \
                                   -ambiguous $spellcheckAmbiguous $type} result]
    stopScanning
    if {$err} {
        ERROR::MessageBox "" "Scid: Spellcheck results"
        return
    }

    .spellcheckWin.text.text delete 1.0 end
    .spellcheckWin.text.text insert end $result
}


# Create the spell checking window with its event handlers
# and start the initial correction scan
#
proc openSpellCheckWin {type {parent .}} {
    global spellcheckType spellcheckSurnames
    global spellcheckAmbiguous
    global spellstate

    set w .spellcheckWin

    if {[winfo exists $w]} {
        tk_messageBox -type ok -icon info -title "Scid: Spellcheck error" \
                      -parent $parent \
                      -message "The spellcheck window is already open; close it first."
        return
    }

    if {[lindex [sc_name read] 0] == 0} {
        # No spellcheck file loaded, so try to open one:
        if {![readSpellCheckFile]} {
            return
        }
    }
    set spellcheckType $type

    win::createDialog $w
    wm title $w "Scid: $::tr(Spellchecking) $::tr(Result)"
    wm minsize $w 0 15

    bind $w <F1> { helpWindow Maintenance }
    bind $w <Configure> "recordWinSize $w"

    # Prepare the text pad
    #
    set f $w.text
    autoscrollText both $w.text $f.text Treeview
    pack $w.text -side top -expand true -fill both
    $f.text configure -tabs [font measure font_Regular  "xxxxxxxxxxxxxxxxxxxxxxxxx"] \
        -width $::winWidth($w) -height $::winHeight($w) -wrap none -state normal -setgrid 1
    grid $f.text -row 0 -column 0 -sticky nswe

    grid rowconfig $w.text 0 -weight 1 -minsize 0
    grid columnconfig $w.text 0 -weight 1 -minsize 0
  
    focus $f.text
    # Draw a canvas ("progress") to hold the progress bar
    # and put it above the buttons at the bottom of the window
    #
    canvas $w.progress -width 450 -height 20 -bg white -relief solid -border 1
    $w.progress create rectangle 0 0 0 0 -fill blue -outline blue -tags bar
    $w.progress create text 445 10 -anchor e -font font_Regular -tags time \
                                   -fill black -text "0:00 / 0:00"
    pack $w.progress -side top -pady 5

    # Create the button pad at the bottom of the window
    #
    set f [ttk::frame $w.buttons]
    pack $f -side bottom -fill x

    # The ambiguous check mark
    # Hitting it starts a new correction scan
    ttk::checkbutton $f.ambig -variable spellcheckAmbiguous \
                              -text $::tr(Ambiguous) -command "updateSpellCheckWin $type"
    pack $f.ambig -side left -padx 2 -ipady 2 -ipadx 3

    # When correcting player names, we add a surnames option
    #
    if {$type == "Player"} {
        # The surnames check mark
        # Hitting it starts a new correction scan
        #
        ttk::checkbutton $f.surnames -variable spellcheckSurnames \
                                     -text $::tr(Surnames) -command "updateSpellCheckWin Player"
        pack $f.surnames -side left -padx 2 -ipady 2 -ipadx 3
    }

    # The button to start the correction making...
    #
    ttk::button $f.ok -text $::tr(MakeCorrections) -underline 0 -command {
        busyCursor .
        set spelltext ""
        catch {set spelltext [.spellcheckWin.text.text get 1.0 end-1c]}
        .spellcheckWin.text.text delete 1.0 end
        .spellcheckWin.text.text insert end \
            "Scid is making the spelling corrections.\nPlease wait..."

        # Enable the progress bar
        #
        update idletasks
        set spell_result ""
        startCorrecting
        progressBarSet .spellcheckWin.progress 451 21
        set err [catch {sc_name correct $spellcheckType $spelltext} spell_result]
        if ($err) {
            ERROR::MessageBox
        } else {
            set msg "Number of names to be corrected: "
            append msg "[lindex $spell_result 0] \n"
            append msg "Number of names errors: "
            append msg "[lindex $spell_result 1] \n\n"
            append msg "Number of games corrected: "
            append msg "[lindex $spell_result 2] \n"
            append msg "Number of games NOT corrected (date<birth or >death): "
            append msg "[lindex $spell_result 3]"
            tk_messageBox -type ok -parent .spellcheckWin \
                -title "Scid: $::tr(Spellchecking) $::tr(Result)" -message $msg
        }
        unbusyCursor .
        focus .
        destroy .spellcheckWin
        sc_game tags reload
        updateBoard -pgn
        ::windows::gamelist::Refresh
    }
    bind $w <Alt-m> "$f.ok invoke; break"

    # The cancel button operates in an either/or context
    # While some process is running, it simply stops it
    # In other cases, spell checking is left
    #
    ttk::button $f.cancel -text $::tr(Cancel) -underline 0 -command {
        if {$spellstate == "scanning" || $spellstate == "correcting"} {
            progressBarCancel
        } else {
            focus .
            destroy .spellcheckWin
        }
    }
    bind $w <Alt-c> "$f.cancel invoke; break"
    pack $f.cancel $f.ok -side right -padx 5 -fill x


    # Start the initial search for spelling corrections
    #
    updateSpellCheckWin $type
}


