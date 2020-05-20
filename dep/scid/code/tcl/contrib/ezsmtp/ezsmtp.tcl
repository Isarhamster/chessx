# ezsmtp.tcl --
#
#	"Easy" SMTP-base mail sending library.  See the ezsmtp.html
#	file for documentation on using this package.
#
# Copyright (c) 1999-2000 by D. J. Hagberg and other parties
#
# See the file "license.txt" for information on usage and redistribution
# of this file, and for a DISCLAIMER OF ALL WARRANTIES.

package provide ezsmtp 1.0.0

namespace eval ::ezsmtp {
    global env tcl_platform
    variable mail

    # Make CVS version ID accessible.
    set mail(cvsid) \
            {$Id: ezsmtp.tcl,v 4.3 2011/02/13 18:12:01 arwagner Exp $}

    # Global variables that may be altered through ezsmtp::config.
    set mail(vars) [list verbose mailhost port from batchsize \
                         logproc strictaddr]
    set mail(verbose) 0                 ;# No logging output
    set mail(mailhost) localhost        ;# Host with smtp daemon
    set mail(port) 25                   ;# port for smtp daemon
    set mail(batchsize) 0               ;# no batching of RCPT TO's.
    set mail(logproc) {}		;# empty means log to stdout.
    set mail(strictaddr) 0		;# should we enforce RFC821 addresses?

    # Try to figure out a reasonable default FROM.
    # Tcl8.1 already has figured out the username for us.
    # Older versions are not so nice-- need to look in the environment
    if {[info exists tcl_platform(user)]} {set mail(from) $tcl_platform(user)}\
    elseif {[info exists env(LOGNAME)]}   {set mail(from) $env(LOGNAME)} \
    elseif {[info exists env(USERNAME)]}  {set mail(from) $env(USERNAME)} \
    elseif {[info exists env(USER)]}      {set mail(from) $env(USER)} \
    else   {set mail(from) {}}
    
    # Trim any leading/trailing spaces and add @ current host name
    set mail(from) [string trim $mail(from)]
    if {[string length $mail(from)] > 0} {
        append mail(from) @ [info hostname]
    }

    # Regular-expression for validating email addresses.
    set mail(hostre) {[A-Za-z]([A-Za-z0-9-]*[A-Za-z0-9])*|\#[0-9]+|}
    append mail(hostre) {\[[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+\]}
    set mail(hostre) "($mail(hostre))(\\.($mail(hostre)))*"
    set mail(strictre) "^\[^\x01-\x20\x7F-\xFF<>@\]+@"
    append mail(strictre) $mail(hostre) {$}
    set mail(!strictre) {^[^@]+|[^@]+@[^@]+$}

    # Default character set/encoding settings
    set mail(tclcharset) ascii
    set mail(mimecharset) us-ascii

    # Package-wide header settings to be sent in the DATA section of the email.
    # Note each header variable is a list -- one element per line.
    set mail(x-mailer) "Tcl ezsmtp Mailer Package"
    if {[llength $mail(cvsid)] > 2} {
        append mail(x-mailer) " (build " [lindex $mail(cvsid) 3] ")"
    }
    set mail(x-mailer:)     [list $mail(x-mailer)]
    set mail(mime-version:) [list "1.0"]
}


#############################################################################
##
## PUBLICALLY-DOCUMENTED PROCS
##
#############################################################################


# ezsmtp::config --
#
#	See documentation for details.
#
# Arguments:
#	args		Options parsed by the procedure.
# Results:
#	See documentation
#
proc ::ezsmtp::config {args} {
    variable mail

    if {[llength $args] == 0} {
        set resl {}
        foreach v $mail(vars) {lappend resl "-$v" [set mail($v)]}
        return $resl
    }

    # Build up a list of valid switches.
    set swl {}
    foreach sw $mail(vars) {lappend swl "-$sw"}

    # If we got a single arg, return its associated variable value
    if {[llength $args] == 1} {
        set v [lindex $args 0]
        if {[lsearch $swl $v] == -1} {
            error "invalid config option, must be one of: $swl"
	}
        return $mail([string trimleft $v -])
    }

    # If we got multiple args, make sure we got an even number.
    set argc [llength $args]
    if {$argc % 2} {
        error "must have an even number of config -option value's."
    }

    # Try each -option/value
    foreach {sw v} $args {
        if {[lsearch $swl $sw] == -1} {
            error "invalid config option, must be one of $swl"
	}

        # Verify numeric & other config values.
        switch -regexp -- $sw {
            {^-(verbose|batchsize)} { 
                set v [expr {int($v)}]
                if {$v < 0} {
                    error "config value for $sw must be a number >= 0"
		}
	    }
            {^-strictaddr$} {
                set v [expr {$v ? 1 : 0}]
	    }
            {^-from$} {
                set v [private_valid_address $v -from]
	    }
            default { }
        }
        set mail([string trimleft $sw -]) $v
    }

    return 1
}


# ezsmtp::send --
#
#	See documentation for details.
#
# Arguments:
#	args		Options parsed by the procedure.
# Results:
#	Throws an error if anything goes wrong.
#
proc ::ezsmtp::send {args} {
    global errorInfo errorCode
    variable mail

    # Set defaults for the current send based on global variables
    foreach v [list mailhost port from batchsize tclcharset mimecharset] {
        set cur($v) [set mail($v)]
    }

    # Start with an empty recipient list and return-receipt disabled
    set cur(rcpt) {}
    set cur(receipt) 0

    # process arguments to smtp::send.
    set validopts [list -subject -from -mailhost -port -channel -body \
            -batchsize]
    set argc [llength $args]
    for {set i 0} {$i < $argc} {incr i} {
        set arg [lindex $args $i]
        switch -regexp -- $arg {
            {^-headers$} {
                incr i; set arg [lindex $args $i]
                if {[llength $arg] % 2} {
                    error "Invalid -headers.  Must be a well-formatted list\
                            of even length with alternating keys/values."
                }
                foreach {k v} $arg { private_add_header cur $k $v }
            }
            {^-(to|cc|bcc|tolist|cclist|bcclist|replyto|replytolist)$} {
                incr i
                set recip [lindex $args $i]
                private_add_recip cur [string trimleft $arg -] $recip
            }
            {^-receipt$} {
                incr i
                private_return_receipt cur [lindex $args $i]
            }
            {^-charset$} {
                incr i
                private_set_charset cur [lindex $args $i]
            }
	    {^-} {
                if {[lsearch $validopts $arg] == -1} {
                    error "Unknown option: $arg.  Must be one of: -headers\
                            -to -cc -bcc -tolist -cclist -bcclist -replyto\
                            -replytolist $validopts"
		}
                incr i
                set cur([string trimleft $arg -]) [lindex $args $i]
            }
            default {
                break
            }
        }
    }

    # Throw an error on any remaining arguments.  Older versions (< 1.0)
    # allowed addresses to be specified after the last arg...
    if {$i < [llength $args]} {
        error "Unknown argument(s): [lrange $args $i [expr\
                {[llength $args]-1}]].  Please read the docs about\
                ezsmtp::send -to/-cc/-bcc/-tolist/-cclist/-bcclist."
    }

    # Make sure we have at least one recipient.
    if {[llength $cur(rcpt)] < 1} {
        error "Missing RECIPIENTs.  Must be: ...send ?options?\
                ?RECIPIENT? ?RECIPIENT...?"
    }

    # Make sure we have a from address and header configured
    set cur(from) [string trim $cur(from)]
    if {![string length $cur(from)]} {
        error "no from address has been specified (length=0)"
    }
    if {![info exists cur(from:)]} {private_add_header cur from $cur(from)}
    set cur(from) [private_valid_address $cur(from) -from]

    # Make sure we have either -channel or -body specified.
    if {[info exists cur(channel)] && [info exists cur(body)]} {
        error "Cannot specify both -channel and -body options."
    } elseif {!([info exists cur(channel)] || [info exists cur(body)])} {
        error "Must specify either -channel or -body option."
    }

    # Open a socket to the SMTP server and send the message, catching any
    # errors to ensure the socket gets closed.  Rethrow error after close.
    set s [socket $cur(mailhost) $cur(port)]
    if {[catch [list private_smtp_begin $s cur] msg]} {
        set tmp_EI $::errorInfo; set tmp_EC $::errorCode
        private_log 9 "errorCode=$tmp_EC errorInfo=$tmp_EI"
        catch [list close $s]
        error $msg $tmp_EI $tmp_EC
    }
    catch [list close $s]
    return $msg
}



#############################################################################
##
## PRIVATE PROCS - SUBJECT TO RADICAL CHANGE ON THE SLIGHTEST WHIM.
##
#############################################################################


# ezsmtp::private_add_header --
#
#	Validate and add a single header to the list of email headers to be 
#	sent before the message.
#
# Arguments:
#	var		Variable name for associative array in the calling 
#			proc containing the specification for this send.
#	key		An RFC822-compliant header name without the trailing
#			colon.  Example: Reply-To
#	value		Value to be sent for the above header name.  Multi-line
#			headers may be specified using newline (\n) separators.
# Results:
#	Throws an error if anything goes wrong.
#
proc ::ezsmtp::private_add_header {var key value} {
    upvar 1 $var cur
    variable mail

    # Force the header name to lower case and ensure RFC compliance.
    set key [string tolower $key]
    if {[regexp "\[\x01-\x20\x7F-\xFF:\]" $key]} {
        error "Invalid header name ($key).  RFC 822 stipulates ASCII\
                Characters excluding CTRL characters, space, and `:'"
    }

    # split value into lines, trimming leading and trailing space.
    set vall {}
    foreach ln [split $value \n] {
        set ln [string trim $ln]
        if {[string length $ln] > 65} {
            private_log 2 "Header $key contains line longer than 65\
                    chars:\n$ln"
        }
        lappend vall $ln
    }
    
    # set in the current send session's array by appending colon to key.
    private_log 6 "Header $key=[join $vall "\n\t"]"
    set cur([set key]:) $vall
}


# ezsmtp::private_set_charset --
#
#	Validate and set the Tcl socket encoding and default MIME charset 
#	for this send.
#
# Arguments:
#	var		Variable name for associative array in the calling 
#			proc containing the specification for this send.
#	value		A Tcl list with either 1 or 2 elements, per the
#			docs for ezsmtp::send -charset
# Results:
#	Throws an error if anything goes wrong.
#
proc ::ezsmtp::private_set_charset {var value} {
    upvar 1 $var cur
    global tcl_version

    if {$tcl_version < 8.1} {
        error "Cannot specify -charset running in Tcl versions < 8.1"
    }

    if {[llength $value] == 1} {
        set cstcl [string tolower [lindex $value 0]]
        if {[string compare $cstcl ascii] == 0} {
            set csmime us-ascii
	} else {
            set csmime $cstcl
	}
    } elseif {[llength $value] == 2} {
        set cstcl [string tolower [lindex $value 0]]
        set csmime [string tolower [lindex $value 1]]
    } else {
        error "-charset value must be a Tcl list with either one or two\
                elements"
    }
    if {[lsearch -exact [encoding names] $cstcl] == -1} {
        error "charset $cstcl is not a valid Tcl encoding"
    }
    set cur(mimecharset) $csmime
    set cur(tclcharset) $cstcl
}


# ezsmtp::private_add_recip --
#
#	Add a single or list of recipients/respondants as either `To:', 
#	`Cc:', Bcc, or `Reply-To:' addresses.
#
# Arguments:
#	var		Variable name for associative array in the calling 
#			proc containing the specification for this send.
#	type		One of to, cc, bcc, replyto, tolist, cclist, bcclist,
#			or replytolist.
#	value		If the `type' specified is one of tolist, cclist, 
#			bcclist, or replytolist this must be a well-formed Tcl
#			list of addresses.  If the `type' specified is one of
#			to, cc, bcc, or replyto this must be a single email 
#			address to add.
# Results:
#	Throws an error if anything goes wrong.
#
proc ::ezsmtp::private_add_recip {var type value} {
    upvar 1 $var cur
    variable mail
    set rcpt_added 0
    set is_replyto 0
    set is_bcc 0

    # decide whether "value" is a list or a single-element address 
    # based on whether the type ends in "list".  If it ends in "list", 
    # strip that off the end.  If no recipients, return immediately.
    if {[string match *list $type]} {
        set reciplist $value
        regsub {list$} $type {} type
    } else {
        set reciplist [list $value]
    }
    if {[llength $reciplist] < 1} {
        return $rcpt_added
    }

    # At this point $type will be "bcc", "to", "cc", or "replyto".
    # Determine which header variable we will be affecting and make
    # sure it exists, skipping bcc.  "replyto" should be "reply-to" in
    # the headers, so we switch it here.
    if {[string compare $type bcc] == 0} {
        set is_bcc 1
    } elseif {[string compare $type replyto] == 0} {
        set is_replyto 1
        set type reply-to
    }
    if {!$is_bcc} {
        set headvar "cur([set type]:)"
        if {[info exists $headvar]} {
            set curlist [set $headvar]
        } else {
            set curlist {}
        }

        # If current stuff in header and we need to add more to the header,
        # make sure the last element ends with a comma (,).
        if {([llength $curlist] > 0) && ([llength $reciplist] > 0)} {
            set idxend [expr {[llength $curlist]-1}]
            set lastelem [lindex $curlist $idxend]
            append lastelem ","
            set curlist [lreplace $curlist $idxend $idxend $lastelem]
        }
    }

    # Loop through all the addresses passed to this proc
    foreach addr $reciplist {
        set addr [string trim $addr]
        if {![string length $addr]} {
            private_log 1 "WARNING: empty address ignored"
            continue
        }

        # Add the address to `To:', `Cc:', and `Reply-To:' headers verbatim.
        if {!$is_bcc} { lappend curlist "$addr," }

        # Do fussy-validation on the recipient address before adding to rcpt's
        set addr [private_valid_address $addr -$type]

        # Add all but Reply-To addresses to the recipient list
        if {!$is_replyto} {
            lappend cur(rcpt) $addr
	}

        incr rcpt_added
    }

    # Nuke any trailing comma in To:/Cc:/Reply-To: header var's, 
    # then and set header var.
    if {!$is_bcc} {
        if {[llength $curlist] > 0} {
            set idxend [expr {[llength $curlist]-1}]
            set lastelem [string trimright [lindex $curlist $idxend] ,]
            set curlist [lreplace $curlist $idxend $idxend $lastelem]
        }
        set $headvar $curlist
    }

    # Return number of addresses added
    set rcpt_added
}


# ezsmtp::private_return_receipt --
#
#	Sets the return-receipt behavior of this send.  See the docs
#	for ezsmtp::send -receipt to describe the arglist argument.
#
# Arguments:
#	var		Variable name for associative array in the calling 
#			proc containing the specification for this send.
#	arglist		May be either a single boolean element or a key/value
#			list of return-receipt settings, per the docs.
# Results:
#	Throws an error if anything goes wrong.
#
proc ::ezsmtp::private_return_receipt {var arglist} {
    upvar 1 $var cur

    # List of valid keys for -receipt list options
    set validopts [list delivery delay nsmail returnfull envelopeid]

    # Set receipt off and array values to default settings.
    array set cur [list receipt 0 receipt,delivery 1 receipt,delay 1 \
            receipt,nsmail 0 receipt,returnfull 0 receipt,envelopeid {}]

    # Single-argument to -receipt should be a boolean on/off for defaults.
    if {[llength $arglist] == 1} {
        set cur(receipt) [expr {[lindex $arglist 0] ? 1 : 0}]
        return
    }

    # Otherwise, we have a list of arguments that must be validated & set.
    # First, make sure the argument list has a valid length...
    if {([llength $arglist] < 2) || ([llength $arglist] % 2)} {
        error "ERROR: keyvaluelist passed to ezsmtp::send -receipt must have\
                an even number of elements and a length of at least 2 elements"
    }

    # And validate each setting, setting appropriate current-send variable.
    foreach {k v} $arglist {
        switch -exact -- $k {
            envelopeid {
                if {[string length [set v [private_xtext $v]]] > 38} {
                    error "ERROR: encoded envelope ID ($v) is too long.  Must\
                            be < 38 chars encoded."
		}
                set cur(receipt,$k) $v
	    }
            default {
                if {[lsearch -exact $validopts $k] == -1} {
                    error "ERROR: bad setting for -receipt key.  Must be 1 of:\
                             [join $validopts { }]"
		}
                set cur(receipt,$k) [expr {$v ? 1 : 0}]
	    }
	}
    }
    set cur(receipt) 1
}


# ezsmtp::private_smtp_begin --
#
#	Start the send by trying EHLO/HELO greeting followed by one or more
#	SMTP conversations, optionally batching some number of recipients.
#
# Arguments:
#	s		Open socket to SMTP server.
#	var		Variable name for associative array in the calling 
#			proc containing the specification for this send.
# Results:
#	Throws an error if anything goes wrong.
#
proc ::ezsmtp::private_smtp_begin {s var} {
    upvar 1 $var cur
    global tcl_version
    variable mail

    # Set up the socket for line-buffering/translation.  Only set encoding
    # if we're running in Tcl 8.1 or newer that supports encodings.
    if {$tcl_version < 8.1} {
        fconfigure $s -buffering line -blocking 1 -eofchar {} \
                -translation {auto crlf}
    } else {
        fconfigure $s -buffering line -blocking 1 -eofchar {} \
                -translation {auto crlf} -encoding $cur(tclcharset)
    }
    private_log 2 "Opened connection to SMTP server $cur(mailhost)"
    private_read_all $s 3 220

    # Try to set up for ESMTP conversation
    set cur(esmtp) 0
    private_send_line $s 3 "EHLO [info hostname]"
    set buf [private_read_all $s 3 {[0-9][0-9][0-9]}]
    if {[string match "250*" $buf]} {
        set cur(esmtp) 1
        set buf [split $buf \n]
        foreach ln [lrange $buf 1 [expr {[llength $buf]-1}]] {
            if {[regexp -nocase \
		    "^250\[ -\](\[A-Z0-9\]\[A-Z0-9-\]*) (\[^\x01-\x20\x7F\])"\
                    $ln nil ehlo_keyword ehlo_param]} {
                set ehlo_keyword [string tolower $ehlo_keyword]
                set ehlo_param [string tolower $ehlo_param]
                set cur(esmtp,$ehlo_keyword) $ehlo_param
	    } elseif {[regexp -nocase "^250\[ -\](\[A-Z0-9\]\[A-Z0-9-\]*)" \
                    $ln nil ehlo_keyword]} {
                set ehlo_keyword [string tolower $ehlo_keyword]
                set cur(esmtp,$ehlo_keyword) 1
	    }
	}
    } else {
        # Otherwise, set up for normal RFC-821 SMTP conversation.
        private_send_line $s 3 "HELO [info hostname]"
        private_read_all $s 3 250
    }

    # If user requested return-receipt but not supported by server, bail.
    if {$cur(receipt) && ![info exists cur(esmtp,dsn)]} {
        error "ERROR: Server does not support DSN for return receipt"
    }

    # If user requested 8bit charset but not supported by server, bail.
    if {([string compare $cur(tclcharset) ascii] != 0) && \
            ![info exists cur(esmtp,8bitmime)]} {
        error "Server does not support 8-bit characters"
    }

    # If we are not doing any batching, do a straightforward send
    set sentcnt 0
    set rcptlen [llength $cur(rcpt)]
    if {($cur(batchsize) <= 0) || ($rcptlen <= $cur(batchsize))} {
        private_log 1 "Starting mail send to [join $cur(rcpt) {, }]"
        private_smtp_batch $s cur 0 [expr {$rcptlen-1}]
        incr sentcnt $rcptlen
        private_log 1 "Mail sent OK"
        return $sentcnt
    }

    # If we are batching sends and reading message body from a channel,
    # find out what position we are in the channel if we are sending to
    # more addr's than our batch size, to allow multiple send batches.
    if {[info exists cur(channel)] && [info exists cur(batchsize)]} {
        set cur(channelpos) [tell $cur(channel)]
        if {($cur(channelpos) == -1) && ($cur(batchsize) > 0)} {
            error "Channel must be seek-able when a batch size has been\
                    specified."
	}
    }

    # Loop through the batches of recipients
    set i 0
    while {$i < $rcptlen} {
        set e [expr {$i+$cur(batchsize)-1}]
        if {$e >= $rcptlen} {
            set e [expr {$rcptlen-1}] 
        }
        private_log 1 "Starting mail send for recipients $i to $e:\n \
                [join [lrange $cur(rcpt) $i $e] "\n  "]"
        private_smtp_batch $s cur $i $e
        incr sentcnt [expr {$e-$i+1}]
        private_log 1 "Mail sent OK"

        # If we need to, send server a reset and rewind the channel position.
        if {($e < ($rcptlen-1)) && [info exists cur(channelpos)]} {
            seek $cur(channelpos)
        }

        set i [expr {$e + 1}]
    }

    # Return the count of unique recipients
    set sentcnt
}


# ezsmtp::private_smtp_batch --
#
#	Implement the SMTP conversation necessary to send the message
#	to a batch of recipients.  This proc works off a subset of the
#	cur(rcpt) array of recipient email addresses specified by the
#	rcptstart and rcptend arguments.
#
# Arguments:
#	s		Open socket to SMTP server.
#	var		Variable name for associative array in the calling 
#			proc containing the specification for this send.
#	rcptstart	index in the cur(rcpt) array to start
#	rcptend		index in the cur(rcpt) array to end
# Results:
#	Throws an error if anything goes wrong.
#
proc ::ezsmtp::private_smtp_batch {s var rcptstart rcptend} {
    upvar 1 $var cur
    variable mail

    # Reset the SMTP server's state to begin a new conversation
    private_send_line $s 3 "RSET"
    private_read_all $s 3

    # Try to build a valid "MAIL FROM:" command, including return-receipt info
    set cmd "MAIL FROM:<$cur(from)>"
    if {$cur(receipt)} {
        if {$cur(receipt,returnfull)} {append cmd " RET=FULL"} \
        else {append cmd " RET=HDRS"}
        if {[string length $cur(receipt,envelopeid)]} {
            append cmd " ENVID=" $cur(receipt,envelopeid)
	}
    }
    private_send_line $s 3 $cmd
    private_read_all $s 3

    # Build up our `NOTIFY=' extension if necessary
    set notifystr {}
    if {$cur(receipt)} {
        set notifyl [list FAILURE]
        if {$cur(receipt,delivery)} { lappend notifyl SUCCESS }
        if {$cur(receipt,delay)}    { lappend notifyl DELAY }
        append notifystr " NOTIFY=" [join $notifyl ,]
    }

    # Each recipent may be specified as "emailaddr" or "Full Name <emailaddr>"
    set rcptlist [lrange $cur(rcpt) $rcptstart $rcptend]
    foreach r $rcptlist {
        if {$cur(receipt)} {
            set orcptstr " ORCPT=rfc822;[private_xtext $r]"
            private_send_line $s 3 "RCPT TO:<$r>${notifystr}${orcptstr}"
	} else {
            private_send_line $s 3 "RCPT TO:<$r>"
	}
        private_read_all $s 3 {25[01]}
    }

    private_send_line $s 3 "DATA"
    private_read_all $s 3 354

    # Send all the mail headers followed by a blank line
    private_send_mail_headers $s cur

    private_log 4 ">>\[message body\]"
    # If we were given a -body in the send command, split into lines and send,
    # ensuring that we strip any stray CR's.
    if {[info exists cur(body)]} {
        set linenum 0
        regsub -all "\r" $cur(body) {} cur(body)
        foreach line [split $cur(body) \n] {
            incr linenum

            # Force breaks at 998 chars to comply with RFC 821 limit.
            while {[string length $line] > 998} {
                private_log 1 "WARNING: >998 chars split at line $linenum"
                set first998 [string range $line 0 997]
                set line [string range 998 [expr \
                        {[string length $line]-1}]]
                if {[string match ".*" $first998]} {
                    set first998 ".$first998"
                }
                private_send_line $s 5 $first998
	    }

            # Send remainder <= 998 chars.
            if {[string match ".*" $line]} {          ;# Double up leading '.'
                set line ".$line"
            }
            private_send_line $s 5 $line
	}
    }

    # If we were given a -channel in the send command, read from the channel
    # one line at a time and send.  Assuming channel configured to strip CRs.
    if {[info exists cur(channel)]} {
        set linenum 0
        while {[gets $cur(channel) line] != -1} {
            incr linenum

            # Force breaks at 998 chars to comply with RFC 821 limit.
            while {[string length $line] > 998} {
                private_log 1 "WARNING: >998 chars split at line $linenum"
                set first998 [string range $line 0 997]
                set line [string range 998 [expr \
                        {[string length $line]-1}]]
                if {[string match ".*" $first998]} {
                    set first998 ".$first998"
                }
                private_send_line $s 5 $first998
	    }

            # Send remainder <= 998 chars.
            if {[string match ".*" $line]} {          ;# Double up leading '.'
                set line ".$line"
            }
            private_send_line $s 5 $line
        }
    }

    # Finally, send the message terminator and read any result.
    private_send_line $s 4 "."
    private_read_all $s 3
}


# ezsmtp::private_send_mail_headers --
#
#	Output all the available email headers at the beginning of the 
#	message, followed by a single empty line.
#
# Arguments:
#	s		Open socket to SMTP server.
#	var		Variable name for associative array in the calling 
#			proc containing the specification for this send.
# Results:
#	Throws an error if anything goes wrong.
#
proc ::ezsmtp::private_send_mail_headers {s var} {
    upvar 1 $var cur
    variable mail

    set doNotSend [list date: subject: from: bcc: content-type: \
            content-transfer-encoding:]

    # If user didn't force a custom date, use the current time in GMT.
    set frm "%a, %d %b %Y %H:%M:%S +0000"
    if {[info exists cur(date:)]} {
        private_send_header $s date: $cur(date:)
    } else {
        private_send_line $s 4 "Date: [clock format [clock seconds] -gmt 1 \
                -format $frm]"
    }

    # If user didn't specify a custom "From:" header, use the one specified
    # by -from parameter or system default.
    if {[info exists cur(from:)]} {
        private_send_header $s from: $cur(from:)
    } else {
        private_send_line $s 4 "From: $cur(from)"
    }

    # Send -subject parameter if specified, or from Subject: custom header.
    if {[info exists cur(subject)]} {
        private_send_line $s 4 "Subject: $cur(subject)"
    } elseif {[info exists cur(subject:)]} {
        private_send_header $s subject: $cur(subject:)
    }

    # Send the required MIME content-type and content-transfer-encoding.
    if {[info exists cur(content-type:)]} {
        private_send_header $s content-type: $cur(content-type:)
    } else {
        private_send_line $s 4 "Content-Type: text/plain;\
                charset=$cur(mimecharset)"
    }
    if {[info exists cur(content-transfer-encoding:)]} {
        private_send_header $s content-transfer-encoding: \
                $cur(content-transfer-encoding:)
    } elseif {[string compare $cur(tclcharset) ascii] == 0} {
        private_send_line $s 4 "Content-Transfer-Encoding: 7bit"
    } else {
        private_send_line $s 4 "Content-Transfer-Encoding: 8bit"
    }

    # Send the custom headers for this current session.
    foreach h [array names cur *:] {
        if {[lsearch $doNotSend $h] != -1} continue
        private_send_header $s $h $cur($h)
    }

    # Send our global headers IFF they are not excluded or custom to 
    # the current sending session.
    foreach h [array names mail *:] {
        if {[lsearch $doNotSend $h] != -1} continue
        if {[info exists cur($h)]} continue
        private_send_header $s $h $mail($h)
    }

    # Check to see if the user requested netscape-style return-receipt on open
    if {$cur(receipt)} {
        if {$cur(receipt,nsmail) && \
                ![info exists cur(disposition-notification-to:)]} {
            private_send_line $s 4 "Disposition-Notification-To: $cur(from)"
	}
    }

    private_send_line $s 4 ""
}


# ezsmtp::private_send_header --
#
#	Sends a header (possibly multi-line) to the SMTP server socket, 
#	formatting the header in proper-text and indenting subsequent
#	lines with a single tab.
#
# Arguments:
#	s		Open socket to SMTP server.
#	name		Name of the header being sent w/trailing colon,
#			like reply-to:
#	valuel		value of header as a list -- one line per list
#			element.
# Results:
#	Header is formatted and output.  Returns number of lines output.
#
proc ::ezsmtp::private_send_header {s name valuel} {
    set lines 0
    set first [lindex $valuel 0]
    set remain [lrange $valuel 1 [expr {[llength $valuel]-1}]]
    private_send_line $s 4 "[private_proper_header $name] $first"
    incr lines
    foreach ln $remain {
        private_send_line $s 4 "\t$ln"
        incr lines
    }
    set lines
}
       

# ezsmtp::private_send_line --
#
#	Sends a line of text to the SMTP server socket, logging at the
#	specified level.
#
# Arguments:
#	s		Open socket to SMTP server.
#	level		Log level between 0 and 9, where 0 is always,
#			1 is whenever -verbose is on, and 9 is the highest
#			debug level available.  Suggested: 4
#	line		Line of text to be sent to the SMTP server, minus
#			any trailing newline or carriage-return characters.
# Results:
#	Closes socket $s and un-sets buffer and status variables.
#
proc ::ezsmtp::private_send_line {s level line} {
    private_log $level "S: $line"
    puts $s $line
    flush $s
}


# ezsmtp::private_read_all --
#
#	Read all available data from the SMTP server socket, handling
#	multi-line responses if necessary, and checking  for a response 
#	code.
#
# Arguments:
#	s		Open socket to SMTP server.
#	level		log level for data read from the socket.
#	resp		glob-style expression to match against the start of
#			data received from the SMTP server.  If left un-
#			specified, the standard 250 (OK) is expected.
# Results:
#	Throws an error if anything goes wrong or an unexpected response 
#	is seen.  Otherwise, returns the data read from the server, with
#	multi-line responses separated by newline (\n) characters.
#
proc ::ezsmtp::private_read_all {s level {resp 250}} {
    variable mail

    # Read response from SMTP server.  Single line or last line of multi-line
    # response will have three digits followed by a space.  Other multi-line
    # responses will have three digits followed by a dash (RFC 821 appendix E).
    # Anything else, including eof, is an error.
    set buf {}
    while {1} {
        if {[eof $s]} {
            error "ERROR: unexpected EOF waiting for output from SMTP server"
        }
        set ln [gets $s]
        append buf $ln \n
        private_log $level "R: $ln"
        if {[string match {[0-9][0-9][0-9] *} $ln]} {
            break
	}
        if {![string match {[0-9][0-9][0-9]-*} $ln]} {
            error "ERROR: unexpected response from SMTP server.  Expected\
                    three digits followed by either a space or a dash (-)"
	}
    }

    # Validate the start of the response
    if {![string match "$resp*" $buf]} {
        error "ERROR: bad response `$buf', wanted: `$resp'"
    }

    # and return the contents of all lines read, separated by newlines.
    set buf
}


# ezsmtp::private_proper_header --
#
#	Proper-cases an RFC 822-style email header name.
#
# Arguments:
#	h		header name like reply-to
# Results:
#	Returns the header name capitalized as most email users 
#	expect: the first letter of each dash-separated word is 
#	capitalized and the remainder is lower-cased, thus the
#	result from above would be Reply-To
#
proc ::ezsmtp::private_proper_header {h} {
    set h [string tolower $h]
    set resl {}

    # RFC 1521 clearly specifies the exact case, verbatim, for mime-version.
    if {[regexp -nocase mime-version: $h]} {
        return "MIME-Version:"
    }

    # For other headers, we capitalize the leading letter of each dash-
    # separated word.
    foreach elem [split $h -] {
        set str [string toupper [string index $elem 0]]
        append str [string range $elem 1 [expr {[string length $elem]-1}]]
        lappend resl $str
    }
    join $resl -
}


# ezsmtp::private_xtext --
#
#	Encode a string as an RFC 1891-compliant xtext value.  Characters 
#	outside the range ! (33) to ~ (126) or the plus (+ (43)) or 
#	equals (= (61)) characters are coded as the plus sign (+) followed
#	by two upper-case hexadecimal digits representing the character code.
#
# Arguments:
#	str		string to be encoded
# Results:
#	xtext-encoded value of str, per the above spec.
#
proc ::ezsmtp::private_xtext {str} {
    set result {}
    foreach c [split $str {}] {
        scan $c %c x
        if {($x < 33) || ($x > 126) || ($x == 43) || ($x == 61)} {
            append result + [format %02X $x]
        } else {
            append result $c
	}
    }
    set result
}


# ezsmtp::private_valid_address --
#
#	Validate and return the essential part of an email address,
#	using the contents of <addr> inside a `Full Name <addr>' spec.
#
# Arguments:
#	addrspec	address spec like `user@a.com' or
#			`Full Name <user@b.com>'
#	setting		setting to be output in an error message, like
#			-from, -to, ...
# Results:
#	stripped-down email address, if valid.  If invalid, an error is 
#	thrown.
#
proc ::ezsmtp::private_valid_address {addrspec setting} {
    variable mail

    set addrspec [string trim $addrspec]
    if {[regexp {<([^>]*)>$} $addrspec nil inneraddr]} {
        set addrspec [string trim $inneraddr]
    }
    if {$mail(strictaddr)} {set re $mail(strictre)} \
    else {set re $mail(!strictre)}
    if {![regexp $re $addrspec]} {
        error "ERROR: $setting address specified ($addrspec) is not compliant.\
                Must be in a form like `user@a.domain' or `Full Name\
                <user@b.domain>'"
    }
    set addrspec
}


# ezsmtp::private_log --
#
#	Log a progress/debug message to stdout or to a user-customized 
#	procedure.
#
# Arguments:
#	level		numeric level at which to log, usu. between 0 (always)
#			and 9 (low-level full debug).
#	msg		message text to log
# Results:
#	xtext-encoded value of str, per the above spec.
#
proc ::ezsmtp::private_log {level msg} {
    variable mail

    if {$mail(verbose) >= $level} {
        if {[string length $mail(logproc)]} {
            uplevel #0 $mail(logproc) $msg
	} else {
            puts stdout "$msg"
            flush stdout
	}
    }
}
