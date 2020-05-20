# postinst.tcl --
# $Id: postinst.tcl,v 4.3 2011/02/13 18:12:01 arwagner Exp $
#
#	post-installation script to prompt user whether to email the
#	author upon successful installation.
#
# Copyright (c) 1999-2000 by D. J. Hagberg
#
# See the file "license.txt" for information on usage and redistribution
# of this file, and for a DISCLAIMER OF ALL WARRANTIES.

set to "dhagberg@millibits.com"
set doctxt "Please see the documentation in ezsmtp.html"

# Ask whether user wants to test the library...
output "Installation complete."
if {![prompt_yn "Test new ezsmtp library installation by emailing the\
        author?"]} {
    output_error "No email sent.\n$doctxt" {}
}

# Load the newly-installed library
package require ezsmtp

# Prompt the user for the SMTP email server to use:
set input [prompt "SMTP Email Server hostname or IP address"]
set mailhost [string trim $input]
if {![string length $mailhost]} {
    output_error "No SMTP email server specified.  Email test skipped\n$doctxt"
}

# Prompt the user for their email address:
set input [prompt "Your email address"]
set from [string trim $input]
if {![regexp -nocase {^[A-Z0-9][^@]*@[A-Z0-9][A-Z0-9.-]*$} $from]} {
    output_error "Bogus email address specified.  Email test skipped.\n$doctxt"
}

# Initial configuration
ezsmtp::config -mailhost $mailhost -from $from

set platform {}
foreach v [list machine platform os osVersion] {
    if {[info exists tcl_platform($v)]} {lappend platform $tcl_platform($v)}
}
set subject "New ezsmtp installation"
set body "A new user installed $inst(pkgname)
Version : $inst(pkgvers)
Platform: [join $platform { }]
Tcl Vers: [info patchlevel]"

# Show the user what will be sent:
output "Message to be sent --
From: $from
To: $to
Subject: $subject
$body"
if {![prompt_yn "OK to send?"]} {
    output_error "No email sent.\n$doctxt" {}
}

# and send the message
if {[catch [list ezsmtp::send -from $from -to $to -subject $subject \
		-body $body] err]} {
    output_error "test email failed. Reason:\n  $err\n$doctxt"
}

output_error "Email sent OK.\n$doctxt" ""
