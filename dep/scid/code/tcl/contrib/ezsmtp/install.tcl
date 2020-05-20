#!/bin/sh
# $Id: install.tcl,v 4.3 2011/02/13 18:12:01 arwagner Exp $ \
exec tclsh "$0" ${1+"$@"}

# install.tcl --
#
#	Simple installation script for single-tcl-script-only library.
#
# Copyright (c) 1999-2000 by D. J. Hagberg
#
# See the file "license.txt" for information on usage and redistribution
# of this file, and for a DISCLAIMER OF ALL WARRANTIES.

# Primary tcl script containing package
set inst(pkgfile) ezsmtp.tcl

# If we detect we are running under wish, post a message informing the 
# user of the error of their ways.
if {[llength [info commands winfo]] > 0} {
    wm protocol . WM_DELETE_WINDOW {exit}
    . configure -background "#000099"
    frame .f -relief raised -bd 1
    pack .f -anchor c -padx 10 -pady 10 -fill both -expand 1

    label .f.lt -text "Installation Messages:"
    set inst(w.t) [text .f.t -width 80 -height 14 -state disabled \
            -relief flat -bd 0]
    frame .f.sep1 -bd 1 -height 2 -relief raised
    pack .f.lt -side top -padx 3 -pady 3 -anchor w
    pack .f.t -side top -padx 3 -pady 0 -anchor w
    pack .f.sep1 -side top -padx 3 -pady 5 -fill x

    set inst(w.l) [label .f.l -text { }]
    set inst(w.ef) [frame .f.ef]
    set inst(w.e) [entry $inst(w.ef).e -width 40]
    set inst(w.yn) [frame $inst(w.ef).yn]
    set inst(w.yn.y) [radiobutton $inst(w.yn).y -text "Yes" -value 1 \
            -variable inst(radio_yn)]
    set inst(w.yn.n) [radiobutton $inst(w.yn).n -text "No" -value 0 \
            -variable inst(radio_yn)]
    pack $inst(w.yn.y) $inst(w.yn.n) -side left -padx 5
    pack $inst(w.l) -side top -padx 3 -pady 0 -anchor w
    pack $inst(w.ef) -side top -padx 3 -pady 4 -anchor w
    pack $inst(w.e)

    frame .f.btns
    pack .f.btns -side top -anchor c -padx 3 -pady 5 -ipadx 3 -ipady 3
    set inst(w.q) [button .f.btns.q -text " Quit " -width 12 -command exit]
    set inst(w.b) [button .f.btns.b -text " Next > " -width 12 -command {
        set inst(response) [$inst(w.e) get]
    }]
    bind $inst(w.b) <Return> [list $inst(w.b) invoke]
    bind $inst(w.e) <Return> [list $inst(w.b) invoke]
    pack $inst(w.q) $inst(w.b) -side left -padx 10

    set waitvar 0
    tkwait visibility $inst(w.b)
    set w [winfo width .]; set h [winfo height .]
    wm geometry . "+[expr int([winfo vrootwidth .]/2-$w/2)]+[expr\
            int([winfo vrootheight .]/2-$h/2)]"
    wm minsize . $w $h

    proc output {msg} {
        global inst
        $inst(w.t) configure -state normal
        $inst(w.t) insert end "$msg\n"
        $inst(w.t) see end
        $inst(w.t) configure -state disabled
    }

    proc output_error {msg {prefix {ERROR: }}} {
        global inst

        set inst(waitvar) 0
        $inst(w.t) configure -state normal
        $inst(w.t) delete 0.0 end
        output "$prefix\n$msg"
        $inst(w.l) configure -text { }
        $inst(w.e) configure -state disabled
        if {[winfo ismapped $inst(w.ef)]} {pack forget $inst(w.ef)}
        $inst(w.b) configure -state disabled
        bind $inst(w.q) <Return> {set inst(waitvar) 1}
        focus $inst(w.q)
        tkwait variable inst(waitvar)
        exit
    }

    proc prompt {msg {default {}} {validre {}}} {
        global inst
        foreach w [winfo children $inst(w.ef)] {pack forget $w}
        pack $inst(w.e)
        $inst(w.l) configure -text "${msg}:"
        $inst(w.e) delete 0 end
        if {[string length $default]} {$inst(w.e) insert end $default}
        focus $inst(w.e)
        tkwait variable inst(response)
        set inst(response)
    }

    proc prompt_yn {msg {default {Y}}} {
        global inst
        if {[string match {[Yy]} $default]} {
            set inst(radio_yn) 1
	} else {
            set inst(radio_yn) 0
	}
        foreach w [winfo children $inst(w.ef)] {pack forget $w}
        pack $inst(w.yn)
        $inst(w.l) configure -text "${msg}:"
        focus $inst(w.b)
        tkwait variable inst(response)
        set inst(radio_yn)
    }

} else {

    # Procedures for tclsh stdin/stdout prompts and messages.
    proc output {msg} {
        puts stdout $msg
        flush stdout
    }

    proc output_error {msg {prefix {ERROR: }}} {
        puts stdout "$prefix$msg"
        flush stdout
        exit 1
    }

    proc prompt {msg {default {}} {validre {}}} {
        if {[string length $default]} {
            puts -nonewline stdout "$msg \[$default\]: "
        } else {
            puts -nonewline stdout "${msg}: "
	}
        flush stdout
        set input [gets stdin]
        if {[string length $validre]} {
            if {![regexp $validre $input]} {
                output_error "Invalid value for $msg"
	    }
	}
        set input
    }

    proc prompt_yn {msg {default {Y}}} {
        global inst
        set res [expr {[regexp -nocase {^$|^y} [prompt "$msg (Y/N)" \
                $default {}]] ? 1 : 0}]
    }
}

# Procedure to trap and display error messages (req'd for GUI installs)
proc catcherr {cmds} {
    if {[catch [list uplevel 1 $cmds] err]} {output_error $err}
}


# Attempt to read the script file
if {![file exists $inst(pkgfile)]} {
    output_error "Please run this script while in the same directory\
            as $inst(pkgfile)"
}
set fh [open $inst(pkgfile) r]
while {[gets $fh ln] != -1} {
    if {![info complete $ln]} continue
    if {([string compare package [lindex $ln 0]] == 0) && \
            ([string compare provide [lindex $ln 1]] == 0)} {
        set inst(pkgname) [lindex $ln 2]
        set inst(pkgvers) [lindex $ln 3]
        break
    }
}
close $fh
if {!([info exists inst(pkgname)] && [info exists inst(pkgvers)]) } {
    output_error "Failed to find `package provide ...' line in $inst(pkgfile)"
}
if {!([string length $inst(pkgname)] && [string length $inst(pkgvers)])} {
    output_error "malformed `package provide ...' line in $inst(pkgfile) --\
            Missing package name and/or version"
}

output "Installing Tcl-only package $inst(pkgname) $inst(pkgvers) . . ."

# Look for -instdir DIRNAME on the command-line, otherwise, prompt
# user for installation directory.
if {[set idx [expr [lsearch -exact $argv -instdir]+1]] > 0} {
    set inst(instdir) [lindex $argv $idx]
    if {[string length $inst(instdir)] == 0} {
        unset inst(instdir)
    }
}
if {![info exists inst(instdir)]} {
    set defdir [file join [file dirname [info library]] \
            "$inst(pkgname)$inst(pkgvers)"]
    set input [prompt "Installation directory" $defdir]
    if {[string length $input] == 0} {
        set inst(instdir) $defdir
    } else {
        set inst(instdir) $input
    }
}

# Make directory if necessary
if {![file isdirectory $inst(instdir)]} {
    catcherr [list file mkdir $inst(instdir)]
}

# Check to see if target file exists
set target [file join $inst(instdir) $inst(pkgfile)]
if {[file exists $target]} {
    if {[file mtime $target] > [file mtime $inst(pkgfile)]} {
        set tmpname "$target.[file mtime $target]"
        catcherr [list file rename $target $tmpname]
        output "WARNING: target file $target newer than $inst(pkgfile)"
        output "  Renamed to $tmpname."
    } else {
        catcherr [list file delete $target]
    }
}

# Copy file to target location
catcherr [list file copy $inst(pkgfile) $target]
output "Copied $inst(pkgfile) --> $target OK"

# Create the pkgIndex file
set target [file join $inst(instdir) pkgIndex.tcl]
catcherr {
    set fh [open $target w]
    puts $fh "package ifneeded $inst(pkgname) $inst(pkgvers) \[list source\
            \[file join \$dir $inst(pkgfile)\]\]"
    close $fh
}
output "Created package index file $target OK"

# If there is a post-install script AND user didn't disable, run it.
if {[lsearch -exact $argv -nopostinst] == -1} {
    if {[file exists postinst.tcl]} {
        catcherr [list source postinst.tcl]
    }
}
