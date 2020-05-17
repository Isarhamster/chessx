###
### Correspondence.tcl: part of Scid.
### Copyright (C) 2008 Alexander Wagner
###
### $Id: correspondence.tcl,v 4.3 2011/02/13 18:12:02 arwagner Exp $
###
### Last change: <Mon, 2014/10/27 19:29:26 arwagner agamemnon>
###
### Add correspondence chess via eMail or external protocol to scid
###
#======================================================================

# http and tdom are required for the Xfcc protocol

#======================================================================
#
# Xfcc interface for scid
#
#======================================================================
namespace eval Xfcc {

	#----------------------------------------------------------------------
	# Header and footer of the SOAP-messages. Linebreaking is important
	#
	set SOAPstart {<?xml version="1.0" encoding="utf-8"?>
	<soap:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
	<soap:Body>
	}

	set SOAPend {</soap:Body>
	</soap:Envelope>}
	#
	#----------------------------------------------------------------------

	set xfccrc     ""
	set xfccstate  {}

	# list of server names for config dialog
	set lsrvname   {}

	# when was the last update was retrieved online?
	set lastupdate 0
	set update     0

	array unset xfccsrv
	# entry values for config dialog
	set Oldnum     0
	set Server     ""
	set Username   ""
	set Password   ""
	set URI        ""

	# To pass on directories on windows with a backslash
	set xfccrcfile ""

	# Set up a proper user agent
	# Something like
	#    Scid/3.7 (x11; Linux i686; rv:Devel 2009) Tcl/Tk 8.5.2
	set useragent "Scid/$::scidVersion ([tk windowingsystem]; $::tcl_platform(os) $::tcl_platform(machine); rv:$scidVersionDate) Tcl/Tk [info patchlevel]"

	#----------------------------------------------------------------------
	# Replace XML entities by their normal characters
	#----------------------------------------------------------------------
	proc xmldecrypt {chdata} {
		foreach from {{\&amp;} {\&lt;} {\&gt;} {\&quot;} {\&apos;}}   \
			to {{\&} < > {"} {'}} {                                     ;# '"
				regsub -all $from $chdata $to chdata
		 }
		 return $chdata
	}

	#----------------------------------------------------------------------
	# Replace normal characters by their XML entities
	#----------------------------------------------------------------------
	proc xmlencrypt {chdata} {
		foreach from {{\&} < > {"} {'}} \
				to {{\&amp;} {\&lt;} {\&gt;} {\&quot;} {\&apos;}} {    ;# '"
				regsub -all $from $chdata $to chdata
		 }
		 return $chdata
	}

	#----------------------------------------------------------------------
	# Configure Xfcc by means of rewriting the .xfccrc in xml
	#----------------------------------------------------------------------
	proc SaveXfcc {} {
		global ::Xfcc::xfccrc ::Xfcc::xfccrcfile
		# file delete $xfccrcfile
		if {[catch {open $xfccrcfile w} optionF]} {
			puts stderr "$xfccrcfile can not be created"
		} else {
			# devide by 4 as the size function returns all subarray entries
			set size [expr [ array size ::Xfcc::xfccsrv ] / 4]

			puts $optionF "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
			puts $optionF "<xfcc>"
			for {set i 0} {$i < $size } {incr i} {
				if { [regexp {^# } $::Xfcc::xfccsrv($i,0)] && \
					  [regexp {^# } $::Xfcc::xfccsrv($i,1)] && \
					  [regexp {^# } $::Xfcc::xfccsrv($i,2)] && \
					  [regexp {^# } $::Xfcc::xfccsrv($i,3)] } {
					if {$size == 1} {
						puts $optionF "\t<server>"
						puts $optionF "\t\t<name>Server</name>"
						puts $optionF "\t\t<uri>http://</uri>"
						puts $optionF "\t\t<user>User_Name</user>"
						puts $optionF "\t\t<pass>Password</pass>"
						puts $optionF "\t\t<rating>Rating</rating>"
						puts $optionF "\t</server>"
					}
				} else {
					set ::Xfcc::xfccsrv($i,0) [::Xfcc::xmlencrypt $::Xfcc::xfccsrv($i,0) ]
					set ::Xfcc::xfccsrv($i,1) [::Xfcc::xmlencrypt $::Xfcc::xfccsrv($i,1) ]
					set ::Xfcc::xfccsrv($i,2) [::Xfcc::xmlencrypt $::Xfcc::xfccsrv($i,2) ]
					set ::Xfcc::xfccsrv($i,3) [::Xfcc::xmlencrypt $::Xfcc::xfccsrv($i,3) ]
					set ::Xfcc::xfccsrv($i,4) [::Xfcc::xmlencrypt $::Xfcc::xfccsrv($i,4) ]
					puts $optionF "\t<server>"
					puts $optionF "\t\t<name>$::Xfcc::xfccsrv($i,0)</name>"
					puts $optionF "\t\t<uri>$::Xfcc::xfccsrv($i,1)</uri>"
					puts $optionF "\t\t<user>$::Xfcc::xfccsrv($i,2)</user>"
					puts $optionF "\t\t<pass>$::Xfcc::xfccsrv($i,3)</pass>"
					puts $optionF "\t\t<rating>$::Xfcc::xfccsrv($i,4)</rating>"
					puts $optionF "\t</server>"
				}
			}
			puts $optionF "</xfcc>"
			close $optionF
			::Xfcc::ReadConfig $xfccrcfile
		}
	}

	#----------------------------------------------------------------------
	# Delete the currently selected server entry
	#----------------------------------------------------------------------
	proc DeleteServer {} {
		# mark a deleted server by '#' allows the user to manually
		# undelete again by removing the '#' before hitting ok.
		set ::Xfcc::Server   "# $::Xfcc::xfccsrv($::Xfcc::Oldnum,0)"
		set ::Xfcc::Username "# $::Xfcc::xfccsrv($::Xfcc::Oldnum,2)"
		set ::Xfcc::Password "# $::Xfcc::xfccsrv($::Xfcc::Oldnum,3)"
		set ::Xfcc::Rating   "# $::Xfcc::xfccsrv($::Xfcc::Oldnum,4)"
		set ::Xfcc::URI      "# $::Xfcc::xfccsrv($::Xfcc::Oldnum,1)"
		set ::Xfcc::xfccsrv($::Xfcc::Oldnum) $::Xfcc::Server
		}

	#----------------------------------------------------------------------
	# Add a new, empty server entry to xfccsrv array
	#----------------------------------------------------------------------
	proc AddServer {} {
		set ::Xfcc::xfccsrv($::Xfcc::Oldnum,0) $::Xfcc::Server
		set ::Xfcc::xfccsrv($::Xfcc::Oldnum,2) $::Xfcc::Username
		set ::Xfcc::xfccsrv($::Xfcc::Oldnum,3) $::Xfcc::Password
		set ::Xfcc::xfccsrv($::Xfcc::Oldnum,1) $::Xfcc::URI
		set ::Xfcc::xfccsrv($::Xfcc::Oldnum,4) $::Xfcc::Rating

 		set size [expr [ array size ::Xfcc::xfccsrv ] / 4]

		# increement the list box with a new server entry
		.configXfccSrv.xfccSrvList configure -height [expr $size+2]

 		set ::Xfcc::xfccsrv($size,0) "Unique_ServerName"
 		set ::Xfcc::xfccsrv($size,2) "Your_Login"
 		set ::Xfcc::xfccsrv($size,3) "SeCrEt!"
 		set ::Xfcc::xfccsrv($size,4) "Rating"
 		set ::Xfcc::xfccsrv($size,1) "http://"

 		set ::Xfcc::Server    $::Xfcc::xfccsrv($size,0)
 		set ::Xfcc::Username  $::Xfcc::xfccsrv($size,2)
 		set ::Xfcc::Password  $::Xfcc::xfccsrv($size,3)
 		set ::Xfcc::Rating    $::Xfcc::xfccsrv($size,4)
 		set ::Xfcc::URI       $::Xfcc::xfccsrv($size,1)

 		lappend ::Xfcc::lsrvname [list $::Xfcc::xfccsrv($size,0)]

 		set ::Xfcc::Oldnum    $size
	}

	#----------------------------------------------------------------------
	# Store the current values to the xfccsrv-array
	#----------------------------------------------------------------------
	proc xfccsrvstore {} {
		set number [ .configXfccSrv.xfccSrvList curselection ]
		if {!($number > 0)} {
			set number 0
		}
		set ::Xfcc::xfccsrv($::Xfcc::Oldnum,0) $::Xfcc::Server
		set ::Xfcc::xfccsrv($::Xfcc::Oldnum,2) $::Xfcc::Username
		set ::Xfcc::xfccsrv($::Xfcc::Oldnum,3) $::Xfcc::Password
		set ::Xfcc::xfccsrv($::Xfcc::Oldnum,4) $::Xfcc::Rating
		set ::Xfcc::xfccsrv($::Xfcc::Oldnum,1) $::Xfcc::URI

		set ::Xfcc::Server    $::Xfcc::xfccsrv($number,0)
		set ::Xfcc::Username  $::Xfcc::xfccsrv($number,2)
		set ::Xfcc::Password  $::Xfcc::xfccsrv($number,3)
		set ::Xfcc::Rating    $::Xfcc::xfccsrv($number,4)
		set ::Xfcc::URI       $::Xfcc::xfccsrv($number,1)

		set ::Xfcc::Oldnum    $number
		.configXfccSrv.xfccSrvList selection set $number
	}

	#----------------------------------------------------------------------
	# Configure Xfcc by means of rewriting the .xfccrc in xml
	#----------------------------------------------------------------------
	proc config {configfile} {
		global ::Xfcc::xfccrc ::Xfcc::xfccrcfile

		set xfccrcfile $configfile

		::Xfcc::ReadConfig $xfccrcfile
		set size [expr [array size ::Xfcc::xfccsrv ] / 4]

		set w ".configXfccSrv"
		if {[winfo exists $w]} {
			focus $w
			return
		}

		set number            1
		set ::Xfcc::Oldnum    0
		set ::Xfcc::Server    $::Xfcc::xfccsrv($::Xfcc::Oldnum,0)
		set ::Xfcc::Username  $::Xfcc::xfccsrv($::Xfcc::Oldnum,2)
		set ::Xfcc::Password  $::Xfcc::xfccsrv($::Xfcc::Oldnum,3)
		set ::Xfcc::Rating    $::Xfcc::xfccsrv($::Xfcc::Oldnum,4)
		set ::Xfcc::URI       $::Xfcc::xfccsrv($::Xfcc::Oldnum,1)
		set ::Xfcc::showPass  0

		# create the window and buttons
		win::createDialog $w
		wm title $w "\[$xfccrcfile\]"

		ttk::frame $w.buttons
		ttk::button $w.bOk     -text OK -command "::Xfcc::xfccsrvstore; ::Xfcc::SaveXfcc; destroy .configXfccSrv"
		ttk::button $w.bAdd    -text  [::tr "GlistAddField"] -command {
			::Xfcc::AddServer
		}

		ttk::button $w.bDelete -text [::tr "GlistDeleteField"] -command {
			::Xfcc::DeleteServer
		}
		ttk::button $w.bCancel -text [::tr "Cancel"] -command "destroy $w"
		grid $w.bAdd    -in $w.buttons -row 0 -column 0 -pady "5 0" -sticky nw
		grid $w.bDelete -in $w.buttons -row 0 -column 1 -padx "10 0" -pady "5 0" -sticky nw
		grid $w.bOk     -in $w.buttons -row 0 -column 3 -padx 5 -pady "15 5"
		grid $w.bCancel -in $w.buttons -row 0 -column 4 -padx 5 -pady "15 5"
		grid columnconfigure $w.buttons 2 -weight 1

		listbox $w.xfccSrvList -height [expr [ array size ::Xfcc::xfccsrv ] / 4 + 1] -width 60 -selectmode single -list ::Xfcc::lsrvname
		# select the first entry
		$w.xfccSrvList selection set $::Xfcc::Oldnum

		ttk::label  $w.lxfccSrv   -text [::tr CCDlgServerName]
		ttk::label  $w.lxfccUid   -text [::tr CCDlgLoginName]
		ttk::label  $w.lxfccPas   -text [::tr CCDlgPassword]
		ttk::checkbutton $w.showPass -text [::tr CCDlgShowPassword] -variable ::Xfcc::showPass -command {
			if {$::Xfcc::showPass} {
				.configXfccSrv.xfccPas configure -show {}
			} else {
				.configXfccSrv.xfccPas configure -show *
			}
		}
		ttk::label  $w.lxfccURI   -text [::tr CCDlgURL]
		ttk::label  $w.lxfccrtype -text [::tr CCDlgRatingType]

		ttk::entry  .configXfccSrv.xfccSrv  -width 60 -textvariable ::Xfcc::Server
		ttk::entry  .configXfccSrv.xfccUid  -width 60 -textvariable ::Xfcc::Username
		ttk::entry  .configXfccSrv.xfccPas  -width 60 -textvariable ::Xfcc::Password -show *
		ttk::entry  .configXfccSrv.xfccURI  -width 60 -textvariable ::Xfcc::URI

		if {$::tcl_version >= 8.5} {
			ttk::combobox .configXfccSrv.xfccrtype -values [sc_info ratings] -width 7 -textvariable ::Xfcc::Rating
		} else {
			eval tk_optionMenu .configXfccSrv.xfccrtype ::Xfcc::Rating [sc_info ratings]
			.configXfccSrv.xfccrtype configure -indicatoron 0 -width 7 -takefocus 1
		}

		# Bind the change of selection to a proper update of variables
		# and internal representation
		bind .configXfccSrv.xfccSrvList <<ListboxSelect>> {
			::Xfcc::xfccsrvstore
		}

		grid $w.xfccSrvList  -sticky e -columnspan 6 -column  0 -row 0 -rowspan $number

		grid $w.lxfccSrv     -sticky e -columnspan 2 -column  0 -row [expr {$number + 1}]
		grid $w.lxfccUid     -sticky e -columnspan 2 -column  0 -row [expr {$number + 2}]
		grid $w.lxfccPas     -sticky e -columnspan 2 -column  0 -row [expr {$number + 3}]
		grid $w.showPass     -sticky w -columnspan 4 -column  2 -row [expr {$number + 4}]
		grid $w.lxfccURI     -sticky e -columnspan 2 -column  0 -row [expr {$number + 5}]
		grid $w.lxfccrtype   -sticky e -columnspan 2 -column  0 -row [expr {$number + 6}]

		grid $w.xfccSrv      -sticky w -columnspan 4 -column  2 -row [expr {$number + 1}]
		grid $w.xfccUid      -sticky w -columnspan 4 -column  2 -row [expr {$number + 2}]
		grid $w.xfccPas      -sticky w -columnspan 4 -column  2 -row [expr {$number + 3}]
		grid $w.xfccURI      -sticky w -columnspan 4 -column  2 -row [expr {$number + 5}]
		grid $w.xfccrtype    -sticky w -columnspan 4 -column  2 -row [expr {$number + 6}]

		# Add the buttons to the window
		grid $w.buttons -sticky news -columnspan 6 -column  0 -row [expr {$number + 7}]

		bind $w <Escape> "$w.bCancel invoke"
		bind $w <F1> { helpWindow CCXfccSetupDialog}
	}

	#----------------------------------------------------------------------
	# Read xfccrcfile (xml) config file and stores the xml structure as
	# is to the global $xfccrc
	#----------------------------------------------------------------------
	proc ReadConfig {xfccrcfile} {
		global xfccrc

		::CorrespondenceChess::updateConsole "info This is Scid's internal Xfcc-interface"
		::CorrespondenceChess::updateConsole "info Using $xfccrcfile..."
		if {[catch {open $xfccrcfile r} optionF]} {
			::CorrespondenceChess::updateConsole "info ERROR: Unable to open config file $xfccrcfile";
		} else {
			set xfccrc [read $optionF]

			set dom [dom parse $xfccrc]
			set doc [$dom documentElement]
			set aNodes [$doc selectNodes {/xfcc/server}]
			set number   0

			# reset the servernames before reading them in again
			set ::Xfcc::lsrvname {}

			foreach srv $aNodes {
				set name     [$srv selectNodes {string(name)}]
				set uri      [$srv selectNodes {string(uri)}]
				set username [$srv selectNodes {string(user)}]
				set password [$srv selectNodes {string(pass)}]
				set rating   [$srv selectNodes {string(rating)}]

				set ::Xfcc::xfccsrv($number,0) $name
				set ::Xfcc::xfccsrv($number,1) $uri
				set ::Xfcc::xfccsrv($number,2) $username
				set ::Xfcc::xfccsrv($number,3) $password
				set ::Xfcc::xfccsrv($number,4) $rating

				lappend ::Xfcc::lsrvname [list $name ]

				incr number
			}
			close $optionF
		}
	}

	#----------------------------------------------------------------------
	# SOAPError: parses $xml and searches for error messages from the
	# server to report them to the user.
	#----------------------------------------------------------------------
	proc SOAPError {server xml} {
		# Remove the SOAP-Envelope and make all server responses to a
		# common XML format as they use the same error messages anyway.
		regsub -all {.*<soap:Fault>} $xml {<error>} xml
		regsub -all {</soap:Fault>.*} $xml {</error>} xml

		regsub -all {.*<MakeAMoveResponse.*\">} $xml {<error>} xml
		regsub -all {</MakeAMoveResponse>.*} $xml {</error>} xml
		regsub -all {<MakeAMoveResult>} $xml {<faultstring>} xml
		regsub -all {</MakeAMoveResult>} $xml {</faultstring>} xml

		set dom [dom parse $xml]
		set doc [$dom documentElement]

		set aNodes [$doc selectNodes //error]
		foreach game $aNodes {
			set fcode   [$game selectNodes {string(faultcode)}]
			set fstring [$game selectNodes {string(faultstring)}]
			switch -regexp -- $fstring \
			"Success" {
				::CorrespondenceChess::updateConsole "info Processing successfull!"
			} \
			"ServerError" {
				::CorrespondenceChess::updateConsole "info Server Error!"
				set Title "Scid Error"
				set Error "$server reported an unknown error."
				tk_messageBox -icon warning -type ok -parent . \
					-title $Title -message $Error
			} \
			"FeatureUnavailable" {
				::CorrespondenceChess::updateConsole "info Feature unavailable!"
			} \
			"AuthenticationFailed" {
				::CorrespondenceChess::updateConsole "info Authentication failed!"
				set Title "Scid Authentication Failure!"
				set Error "Could not authenticate to the Xfcc-Server.\nPlease check Username and Password for $server."
				tk_messageBox -icon warning -type ok -parent . \
					-title $Title -message $Error
			} \
			"InvalidGameID" {
				::CorrespondenceChess::updateConsole "info Invalid Game-ID!"
			} \
			"NotYourGame" {
				::CorrespondenceChess::updateConsole "info Not your game!"
			} \
			"NotYourTurn" {
				::CorrespondenceChess::updateConsole "info Not your turn!"
			} \
			"InvalidMove" {
				::CorrespondenceChess::updateConsole "info Invalid move!"
			} \
			"InvalidMoveNumber" {
				::CorrespondenceChess::updateConsole "info Invalid move number!"
			} \
			"NoDrawWasOffered" {
				::CorrespondenceChess::updateConsole "info No draw was offered!"
			} \
			"LostOnTime" {
				::CorrespondenceChess::updateConsole "info Lost on time!"
			} \
			"YouAreOnLeave" {
				::CorrespondenceChess::updateConsole "info You are on leave!"
			} \
			"MoveIsAmbigous" {
				::CorrespondenceChess::updateConsole "info Move is ambigous!"
			}
		}
	}

	#----------------------------------------------------------------------
	# Process all servers found in the global xfccrc and store the
	# games in path/.
	#----------------------------------------------------------------------
	proc ProcessAll {path} {
		global xfccrc

		# empty the state array
		set ::Xfcc::xfccstate {}

		set dom [dom parse $xfccrc]
		set doc [$dom documentElement]

		set aNodes [$doc selectNodes {/xfcc/server}]

		foreach srv $aNodes {
			set name     [::Xfcc::xmlencrypt [$srv selectNodes {string(name)}]]
			set uri      [::Xfcc::xmlencrypt [$srv selectNodes {string(uri)} ]]
			set username [::Xfcc::xmlencrypt [$srv selectNodes {string(user)}]]
			set password [::Xfcc::xmlencrypt [$srv selectNodes {string(pass)}]]
			set rating   [$srv selectNodes {string(rating)}]

			if {$rating == ""} {
				set rating "ICCF"
			}

			::CorrespondenceChess::updateConsole "info Processing $username\@$name..."
			set xml [::Xfcc::Receive $uri $username $password]
			::Xfcc::SOAPError $name $xml
			::Xfcc::WritePGN $path $name $rating $xml
			::Xfcc::PrintStatus $path $name $xml
		}
	}

	#----------------------------------------------------------------------
	# Receive games via XFCC from the web service at uri using username
	# and password provided
	#----------------------------------------------------------------------
	proc Receive {uri username password} {
		# construct the SOAP-message for Xfcc Webservice
		set xmlmessage $::Xfcc::SOAPstart
			# generate the "Get my Games" call
			append xmlmessage {<GetMyGames xmlns="http://www.bennedik.com/webservices/XfccBasic">}
			append xmlmessage "<username>$username</username>"
			append xmlmessage "<password>$password</password>"
			append xmlmessage "</GetMyGames>"
		append xmlmessage $::Xfcc::SOAPend

		# send it to the web service note the space before the charset
		set token [::http::geturl $uri \
						-type "text/xml; charset=\"utf-8\"" \
						-query $xmlmessage]

		# retrieve result
		set xmlresult [::http::data $token]
		::http::cleanup $token

		###---###
		# if {[catch {open "/tmp/xfcc.xml" w} dbg]} {
		# 	::CorrespondenceChess::updateConsole "info ERROR: Unable to open debug file";
		# } else {
		# 	puts $dbg $xmlresult
		# }
		# close $dbg
		###---###

		return $xmlresult
	}

	#----------------------------------------------------------------------
	# Send move via XFCC to the web service at uri using username
	# and password provided. Gameid is the unique id on the server,
	# move count the current move number, move the move to send in SAN,
	# comment the comment sent to the opponent. The other variables are
	# flags that might be true/false.
	#----------------------------------------------------------------------
	proc SendMove {uri username password gameid movecount move comment \
						resign acceptdraw offerdraw claimdraw} {

		# Encrypt textual entities to conform to XML
		set uri      [::Xfcc::xmlencrypt $uri]
		set username [::Xfcc::xmlencrypt $username]
		set password [::Xfcc::xmlencrypt $password]
		set comment  [::Xfcc::xmlencrypt $comment]

		set xmlmessage $::Xfcc::SOAPstart
			append xmlmessage {<MakeAMove xmlns="http://www.bennedik.com/webservices/XfccBasic">}
			append xmlmessage "<username>$username</username>"
			append xmlmessage "<password>$password</password>"
			append xmlmessage "<gameId>$gameid</gameId>"
			append xmlmessage "<resign>$resign</resign>"
			append xmlmessage "<acceptDraw>$acceptdraw</acceptDraw>"
			append xmlmessage "<movecount>$movecount</movecount>"
			append xmlmessage "<myMove>$move</myMove>"
			append xmlmessage "<offerDraw>$offerdraw</offerDraw>"
			append xmlmessage "<claimDraw>$claimdraw</claimDraw>"
			append xmlmessage "<myMessage>$comment</myMessage>"
			append xmlmessage "</MakeAMove>"
		append xmlmessage $::Xfcc::SOAPend

		# if {[catch {open "/tmp/send.xml" w} debug]} {
		# 	::CorrespondenceChess::updateConsole "info unable to open debug file..."
		# } else {
		# 	puts $debug $xmlmessage
		# }
		# close $debug

		# send it to the web service note the space before the charset
		set token [::http::geturl $uri \
						-type "text/xml; charset=\"utf-8\"" \
						-query $xmlmessage]

		# retrieve result
		set xmlresult [::http::data $token]
		::http::cleanup $token
		return $xmlresult
	}

	#----------------------------------------------------------------------
	# Send move to server, extracting login data first from config file
	#----------------------------------------------------------------------
	proc Send {name gameid movecount move comment \
				  resign acceptdraw offerdraw claimdraw} {
		global xfccrc

		set dom [dom parse $xfccrc]
		set doc [$dom documentElement]

		set aNodes [$doc selectNodes {/xfcc/server}]

		foreach srv $aNodes {
			set server   [$srv selectNodes {string(name)}]
			set uri      [$srv selectNodes {string(uri)}]
			set username [$srv selectNodes {string(user)}]
			set password [$srv selectNodes {string(pass)}]

			if {$name == $server} {
				::CorrespondenceChess::updateConsole "info Processing $gameid for $username\@$name..."
				::CorrespondenceChess::updateConsole "info Sending $movecount\. $move \{$comment\}"

				if {$resign == "true"} {
					::CorrespondenceChess::updateConsole "info Resigning..."
				}
				if {$acceptdraw == "true"} {
					::CorrespondenceChess::updateConsole "info Accepting draw..."
				}
				if {$claimdraw == "true"} {
					::CorrespondenceChess::updateConsole "info Claiming draw..."
				}
				if {$offerdraw == "true"} {
					::CorrespondenceChess::updateConsole "info Offering draw..."
				}

				set xml [::Xfcc::SendMove $uri $username $password \
							$gameid $movecount $move $comment \
							$resign $acceptdraw $offerdraw $claimdraw]
				::Xfcc::SOAPError $name $xml

				# if {[catch {open "/tmp/answer.xml" w} debug]} {
				# 	::CorrespondenceChess::updateConsole "info unable to open debug file..."
				# } else {
				# 	puts $debug $xml
				# }
				# close $debug
			}
		}
	}

	#----------------------------------------------------------------------
	# Given the name of the Xfcc-Server and the XML-result from the web
	# server a PGN file with a single game is written. name is the name
	# of the server used for generation of the CmailGameID, xml is the
	# result from the web service. rating contains the string that
	# should be used to specify the rating system. It could be
	# something like Rating, Elo, ICCF, USCF, BCF etc. like usual in
	# Scid
	#----------------------------------------------------------------------
	proc WritePGN {path name rating xml} {
		# The following removes the SOAP-Envelope. tDOM does not seem to
		# like it for whatever reason, but it's not needed anyway.
		regsub -all {.*<GetMyGamesResult>} $xml {<GetMyGamesResult>} xml
		regsub -all {</GetMyGamesResult>.*} $xml {</GetMyGamesResult>} xml

		set dom [dom parse $xml]
		set doc [$dom documentElement]

		set aNodes [$doc selectNodes //XfccGame]
		foreach game $aNodes {

			set id           [::Xfcc::xmldecrypt [$game selectNodes {string(id)}]]
			set Event        [::Xfcc::xmldecrypt [$game selectNodes {string(event)}]]
			set Site         [::Xfcc::xmldecrypt [$game selectNodes {string(site)}]]
			set Date         [::Xfcc::xmldecrypt [$game selectNodes {string(eventDate)}]]
			set White        [::Xfcc::xmldecrypt [$game selectNodes {string(white)}]]
			set Black        [::Xfcc::xmldecrypt [$game selectNodes {string(black)}]]
			set WhiteElo     [::Xfcc::xmldecrypt [$game selectNodes {string(whiteElo)}]]
			set BlackElo     [::Xfcc::xmldecrypt [$game selectNodes {string(blackElo)}]]
			set TimeControl  [::Xfcc::xmldecrypt [$game selectNodes {string(timeControl)}]]
			set GameId       [::Xfcc::xmldecrypt [$game selectNodes {string(id)}]]
			set Source       [::Xfcc::xmldecrypt [$game selectNodes {string(gameLink)}]]
			set Round        [::Xfcc::xmldecrypt [$game selectNodes {string(round)}]]
			set Result       [::Xfcc::xmldecrypt [$game selectNodes {string(result)}]]
			set drawOffered  [::Xfcc::xmldecrypt [$game selectNodes {string(drawOffered)}]]
			set setup        [::Xfcc::xmldecrypt [$game selectNodes {string(setup)}]]
			set fen          [::Xfcc::xmldecrypt [$game selectNodes {string(fen)}]]
			set myTurn       [$game selectNodes {string(myTurn)}]
			set moves        [::Xfcc::xmldecrypt [$game selectNodes {string(moves)}]]
			set mess         [::Xfcc::xmldecrypt [$game selectNodes {string(message)}]]

			# These values may not be set, they were first introduced by
			# SchemingMind as extension to Xfcc.  If uppercase settings
			# (usual default) exist: use them and they should take
			# precedence. Note that the PNG header should use upper case
			# by convention
			set whiteCountry [::Xfcc::xmldecrypt [$game selectNodes {string(WhiteCountry)}]]
			set blackCountry [::Xfcc::xmldecrypt [$game selectNodes {string(BlackCountry)}]]
			set whiteIccfID  [::Xfcc::xmldecrypt [$game selectNodes {string(WhiteIccfID)}]]
			set blackIccfID  [::Xfcc::xmldecrypt [$game selectNodes {string(BlackIccfID)}]]
			set whiteFideID  [::Xfcc::xmldecrypt [$game selectNodes {string(WhiteFideID)}]]
			set blackFideID  [::Xfcc::xmldecrypt [$game selectNodes {string(BlackFideID)}]]
			set WhiteNA      [::Xfcc::xmldecrypt [$game selectNodes {string(WhiteNA)}]]
			set BlackNA      [::Xfcc::xmldecrypt [$game selectNodes {string(BlackNA)}]]

			if {$whiteCountry == ""} {
				set whiteCountry [::Xfcc::xmldecrypt [$game selectNodes {string(whiteCountry)}]]
			}
			if {$whiteIccfID == ""} {
				set whiteIccfID  [::Xfcc::xmldecrypt [$game selectNodes {string(whiteIccfID)}]]
			}
			if {$whiteFideID == ""} {
				set whiteFideID  [::Xfcc::xmldecrypt [$game selectNodes {string(whiteFideID)}]]
			}
			if {$blackCountry == ""} {
				set blackCountry [::Xfcc::xmldecrypt [$game selectNodes {string(blackCountry)}]]
			}
			if {$blackIccfID == ""} {
				set blackIccfID  [::Xfcc::xmldecrypt [$game selectNodes {string(blackIccfID)}]]
			}
			if {$blackFideID == ""} {
				set blackFideID  [::Xfcc::xmldecrypt [$game selectNodes {string(blackFideID)}]]
			}
			# White/BlackNA are normally left blank but if the user
			# allwos contain the mail addresses of the player
			if {$WhiteNA == ""} {
				set WhiteNA      [::Xfcc::xmldecrypt [$game selectNodes {string(whiteNA)}]]
			}
			if {$BlackNA == ""} {
				set BlackNA      [::Xfcc::xmldecrypt [$game selectNodes {string(blackNA)}]]
			}
			if {$WhiteNA == ""} {
				set WhiteNA "white@unknown.org"
			}
			if {$BlackNA == ""} {
				set BlackNA "black@unknown.org"
			}


			# get the variant as scid can not handle many of them.
			# a list of all possible tags can be found here:
			# http://wiki.schemingmind.com/PGNVariantValues
			# http://wiki.schemingmind.com/Variants
			set variant         [$game selectNodes {string(variant)}]

			set filename [file nativename [file join $path "$name-$id.pgn"]]
			file delete $filename

			# Drop games that are not "normal" chess as scid can not
			# handle variants. Note that the ICCF does not set the
			# variant flag. Additionally, it is enough to drop variant
			# games from the inbox to get proper playlists.
			if { ($Result == "Cancelled") } {
					::CorrespondenceChess::updateConsole "info $name-$id was cancelled...";
			} elseif {($variant == "chess") || ($variant == "") || ($variant == "randompieces") || ($variant == "upsidedown") || ($variant == "loosers") ||  ($variant == "nocastle")} {
				### --- Istvan --- ###
				### Racing Kings is not possible due to unambiguous moves
				### that are ambiguous if check is allowed
				### ($variant == "racingkings") ||
				### --- Istvan --- ###

				if {[catch {open $filename w} pgnF]} {
					::CorrespondenceChess::updateConsole "info ERROR: Unable to open config file $filename";
				} else {
					::CorrespondenceChess::updateConsole "info $name-$id..."
					puts $pgnF "\[Event \"$Event\"\]";
					puts $pgnF "\[Site \"$Site\"\]";
					puts $pgnF "\[Date \"$Date\"\]";
					puts $pgnF "\[Round \"$Round\"\]";
					puts $pgnF "\[White \"$White\"\]";
					puts $pgnF "\[Black \"$Black\"\]";
					puts $pgnF "\[White$rating \"$WhiteElo\"\]";
					puts $pgnF "\[Black$rating \"$BlackElo\"\]";
					puts $pgnF "\[TimeControl \"$TimeControl\"\]";
					puts $pgnF "\[GameId \"$GameId\"\]";
					puts $pgnF "\[Source \"$Source\"\]";
					puts $pgnF "\[WhiteNA \"$WhiteNA\"]";
					puts $pgnF "\[BlackNA \"$BlackNA\"]";
					puts $pgnF "\[Mode \"XFCC\"\]";
					puts $pgnF "\[CmailGameName \"$name-$id\"\]";

					if {$whiteCountry != ""} {
						puts $pgnF "\[WhiteCountry \"$whiteCountry\"\]";
					}
					if {$blackCountry != ""} {
						puts $pgnF "\[BlackCountry \"$blackCountry\"\]";
					}
					if {$whiteIccfID > 0} {
						puts $pgnF "\[WhiteIccfID \"$whiteIccfID\"\]";
					}
					if {$blackIccfID > 0} {
						puts $pgnF "\[BlackIccfID \"$blackIccfID\"\]";
					}
					if {$whiteFideID  > 0} {
						puts $pgnF "\[WhiteFideID \"$whiteFideID\"\]";
					}
					if {$blackFideID > 0} {
						puts $pgnF "\[BlackFideID \"$blackFideID\"\]";
					}
					if {$setup == "true"} {
						puts $pgnF "\[FEN \"$fen\"\]";
					}

					# add result to the header
					# Adjudication is handled like normal game results, that
					# is WhiteWins == WhiteWinAdjudicated etc.
					switch -regexp -- $Result \
					"Ongoing" {
						puts $pgnF "\[Result \"*\"\]\n";
					} \
					"AdjudicationPending" {
						puts $pgnF "\[Result \"*\"\]\n";
					} \
					"WhiteWin*" {
						puts $pgnF "\[Result \"1-0\"\]\n";
					} \
					"BlackWin*" {
						puts $pgnF "\[Result \"0-1\"\]\n";
					} \
					"Draw*" {
						puts $pgnF "\[Result \"1/2-1/2\"\]\n";
					} \
					"WhiteDefaulted" {
						puts $pgnF "\[Result \"0-1\"\]\n";
					} \
					"BlackDefaulted" {
						puts $pgnF "\[Result \"1-0\"\]\n";
					} \
					"BothDefaulted" {
						puts $pgnF "\[Result \"1/2-1/2\"\]\n";
					} \
					default {
						puts $pgnF "\[Result \"$Result\"\]\n";
					}

					# Add the game-id as comment before starting the game.
					# This might be helpful on certain mobile devices, that
					# can not deal with extensive header information, e.g.
					# OpenChess on PalmOS.
					puts $pgnF "{$name-$id}"
					puts $pgnF $moves

					# If the PGN already ends with a comment, do not place
					# the message string afterwards as scid will then
					# discard the comment in the movelist.
					if {[string range $moves end end] != "\}"} {
						if {($myTurn == "true") && ($mess != "")} {
							puts -nonewline $pgnF "\{"
							puts -nonewline $pgnF $mess
							puts $pgnF "\}"
						}
					}
					# If a game has finished and a message is sent always
					# add it here.
					if {($Result != "Ongoing") && ($mess != "")} {
						puts -nonewline $pgnF "\{"
						puts -nonewline $pgnF $mess
						puts $pgnF "\}"
					}

					# add result at the end
					switch -regexp -- $Result \
					"Ongoing" {
						puts $pgnF "*";
					} \
					"AdjudicationPending" {
						puts $pgnF "*";
					} \
					"WhiteWin*" {
						puts $pgnF "1-0\n";
					}\
					"BlackWin*" {
						puts $pgnF "0-1\n";
					}\
					"Draw*" {
						puts $pgnF "1/2-1/2\n";
					} \
					"WhiteDefaulted" {
						puts $pgnF "\{White Defaulted\} 0-1\n";
					}\
					"BlackDefaulted" {
						puts $pgnF "\{Black Defaulted\} 1-0\n";
					}\
					"BothDefaulted" {
						puts $pgnF "\{Both Defaulted\} 1/2-1/2\n";
					}
					close $pgnF
				}
			}
		}
	}

	#----------------------------------------------------------------------
	# Prints all status flags of the games in xml for server name.
	#----------------------------------------------------------------------
	proc PrintStatus {path name xml} {
		regsub -all {.*<GetMyGamesResult>} $xml {<GetMyGamesResult>} xml
		regsub -all {</GetMyGamesResult>.*} $xml {</GetMyGamesResult>} xml

		set dom [dom parse $xml]
		set doc [$dom documentElement]

		set aNodes [$doc selectNodes //XfccGame]
		foreach game $aNodes {
			set id              [$game selectNodes {string(id)}]
			set myTurn          [$game selectNodes {string(myTurn)}]
			set daysPlayer      [$game selectNodes {string(daysPlayer)}]
			set hoursPlayer     [$game selectNodes {string(hoursPlayer)}]
			set minutesPlayer   [$game selectNodes {string(minutesPlayer)}]
			set daysOpponent    [$game selectNodes {string(daysOpponent)}]
			set hoursOpponent   [$game selectNodes {string(hoursOpponent)}]
			set minutesOpponent [$game selectNodes {string(minutesOpponent)}]
			set drawOffered     [$game selectNodes {string(drawOffered)}]
			set setup           [$game selectNodes {string(setup)}]
			set fen             [$game selectNodes {string(fen)}]
			set variant         [$game selectNodes {string(variant)}]
			set noOpeningBooks  [$game selectNodes {string(noOpeningBooks)}]
			set noDatabases     [$game selectNodes {string(noDatabases)}]
			set noTablebases    [$game selectNodes {string(noTablebases)}]
			set noEngines       [$game selectNodes {string(noEngines)}]
			set Result          [$game selectNodes {string(result)}]
			set TimeControl     [$game selectNodes {string(timeControl)}]
			set mess            [::Xfcc::xmldecrypt [$game selectNodes {string(message)}]]
			set serverinfo      [::Xfcc::xmldecrypt [$game selectNodes {string(serverInfo)}]]

			# Set to official ICCF timing by default
			# as ICCF does not send TimeControl
			set TC "10/50d (?)"
			if { [regexp {\+} $TimeControl] } {
				set TC [split $TimeControl "+"]
				set gametime  [ expr {[lindex $TC 0] / 86400} ]
				set increment [ expr {[lindex $TC 1] / 86400} ]

				set TC $gametime
				append TC "d + "
				append TC $increment
				append TC "d (Fischer)"
			} elseif { [regexp {\/} $TimeControl] } {
				set TC [split $TimeControl "/"]
				set moves   [ expr {[lindex $TC 0]} ]
				set days    [ expr {[lindex $TC 1] / 86400 }]
				set TC $moves
				append TC " / "
				append TC $days
				append TC "d"
				# 10/50 is the official timing for ICCF
				if { ($moves == 10) && ($days == 50) } {
					set TC "$TC (ICCF)"
				}
			}

			set mytime  [expr $daysPlayer*24*60+$hoursPlayer*60+$minutesPlayer]
			set opptime [expr $daysOpponent*24*60+$hoursOpponent*60+$minutesOpponent]

			if {[$game selectNodes {string(hasWhite)}] == "true"} {
				set clockW [format "%2ud %2u:%2u" $daysPlayer $hoursPlayer $minutesPlayer]
				set clockB [format "%2ud %2u:%2u" $daysOpponent $hoursOpponent $minutesOpponent]
			} else {
				set clockB [format "%2ud %2u:%2u" $daysPlayer $hoursPlayer $minutesPlayer]
				set clockW [format "%2ud %2u:%2u" $daysOpponent $hoursOpponent $minutesOpponent]
			}
			lappend ::Xfcc::xfccstate [list \
				"$name-$id" \
				[list "myTurn" $myTurn] \
				[list "clockW" $clockW] \
				[list "clockB" $clockB] \
				[list "drawOffered"  $drawOffered ]\
				[list "setup" $setup] \
				[list "fen" $fen] \
				[list "variant" $variant] \
				[list "noOpeningBooks" $noOpeningBooks] \
				[list "noTablebases" $noTablebases] \
				[list "noDatabases" $noDatabases] \
				[list "noEngines" $noEngines] \
				[list "result" $Result] \
				[list "TimeControl" $TC] \
				[list "message" $mess] \
				[list "mytime" $mytime] \
				[list "opptime" $opptime] \
				[list "serverInfo" $serverinfo] ]
		}

		set filename [scidConfigFile xfccstate]
		file delete $filename

		if {[catch {open $filename w} stateF]} {
			::CorrespondenceChess::updateConsole "info ERROR: Unable to open state file $filename";
		} else {
			puts $stateF "# Scid options file"
			puts $stateF "# State file for correspondence chess"
			puts $stateF "# Version: $::scidVersion, $::scidVersionDate"
			puts $stateF "# This file is generated automatically. Do NOT edit."

			set ::Xfcc::update 1
			set ::Xfcc::lastupdate [clock seconds]
			set curtime [clock format $::Xfcc::lastupdate]
			puts $stateF "#"
			puts $stateF "# Last Update: $curtime"
			puts $stateF "#"
			foreach i { ::Xfcc::lastupdate     \
							::Xfcc::xfccstate } {
				puts $stateF "set $i [list [set $i]]"
			}
		}
		close $stateF
	}

	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	# source the options file to overwrite the above setup
	if {[catch {source [scidConfigFile xfccstate]} ]} {
	} else {
	  ::splash::add "Xfcc state found and restored."
	}

}

#======================================================================
#
# Correspondence chess menus, dialogs and functions
#
#======================================================================

#----------------------------------------------------------------------
# Correspnodence Chess functions
namespace eval CorrespondenceChess {

	# whether the console is already open or not
	set isOpen   0

	# default Database
	set CorrBase        [file nativename [file join $scidDataDir "Correspondence.si4"]]

	# incoming PGN files
	set Inbox           [file nativename [file join $scidDataDir "Inbox"]]
	# outgoing PGN files
	set Outbox          [file nativename [file join $scidDataDir "Outbox"]]

	# Connector config for game relay
	set Connector       [file nativename [file join $scidDataDir "connector.xml"]]

	# use internal xfcc-support
	set XfccInternal     1
	set xfccrcfile      [file nativename [file join $scidConfigDir "xfccrc"]]

	# Path for additional functions that should be available in the CC
	# window only. All files from here are sourced once the CC window
	# starts up.
	set PluginPath      [file nativename [file join $scidDataDir "Plugins/Correspondence"]]

	# external fetch  tool (eg. Xfcc)
	set XfccFetchcmd     "./Xfcc-Receive.pl"
	# external send tool (eg. Xfcc)
	set XfccSendcmd      "./Xfcc-Send.pl"

	# confirm before sending moves?
	set XfccConfirm          1

	# Relay games from ICCF: this list contains all MakeAMove-URLs for
	# the games to be relayed
	set RelayGames           {}

	# Show only games where the player has the move?
	set ListOnlyOwnMove      0
	# set sortoptlist        [list "Site, Event, Round, Result, White, Black" "My Time" "Time per Move" "Opponent Time"]

	# Sort criteria to use
	set CCOrderClassic       0
	set CCOrderMyTime        1
	set CCOrderTimePerMove   2
	set CCOrderStartDate     3
	set CCOrderOppTime       4

	# Which to use
	set ListOrder          $CCOrderClassic

	# email-programm capable of SMTP auth and attachments
	set mailer           "/usr/bin/nail"
	# mail a bcc of the outgoing mails to this address
	set bccaddr          ""
	# mailermode might be: mailx, mozilla, claws or mailurl
	set mailermode       "mailx"
	# parameter for attaching a file
	set attache          "-a"
	# parameter for the subject line
	set subject          "-s"

	set CorrSlot         -1

	# current number in game list
	set num              0

	# Content of CC windows games list
	set clipboardText    ""

	set glccstart        1
	set glgames          0

	#----------------------------------------------------------------------
	# Fetch a file via http
	#----------------------------------------------------------------------
	proc getPage { url } {
		set token [::http::geturl $url]
		set data [::http::data $token]
		::http::cleanup $token
		return $data
	}

	#----------------------------------------------------------------------
	# Open a File select dialog and returns the file selected
	# $i: title text after "Scid Correspondence Chess: Select "
	# $filespecs: the specs of the file (currently ignored)
	#----------------------------------------------------------------------
	proc chooseFile {i filespecs} {
		set idir [pwd]
		set fullname [tk_getOpenFile -initialdir $idir -title "Scid Correspondence Chess: Select $i"]
		if {$fullname == ""} { return }
		return $fullname
	}

	#----------------------------------------------------------------------
	# Set the default correspondence base to the file selected.
	# Open Database works on that file, but in principle every other
	# DB of the type "Correspondence" can be used by just loading by
	# hand before using the CC features.
	#----------------------------------------------------------------------
	proc chooseCorrBase {} {
		global ::CorrespondenceChess::CorrBase

		set filetype { "Scid databases" {".si4" ".si"} }
		set CorrBase [chooseFile "default correspondence chess DB..." $filetype]
	}

	#----------------------------------------------------------------------
	# Choose the path where to fetch Xfcc-games to. All pgn-files in
	# this path are used as input so this offers a way to incorporate
	# cmail games as well.
	#----------------------------------------------------------------------
	proc chooseInbox    {} {
		global ::CorrespondenceChess::Inbox

		set filetype { "All files" {".*"} }
		set Inbox [file dirname [chooseFile "default correspondence chess Inbox..." $filetype]]
	}

	#----------------------------------------------------------------------
	# In Outbox a pgn-version of the game after the users move is
	# stored. This includes all variations and comments! For
	# incorporation of cmail they need to be stripped.
	#----------------------------------------------------------------------
	proc chooseOutbox   {} {
		global ::CorrespondenceChess::Outbox               \

		set filetype { "All files" {".*"} }
		set Outbox [file dirname [chooseFile "default correspondence chess Outbox..." $filetype]]
	}

	#----------------------------------------------------------------------
	# Xfcc fetching is done by an external utility, currently perl as
	# this eases up XML parsing a lot. Having it natively would be
	# desireable though. On the other hand an external utility could
	# also fetch cmail games or whatever other source as it will be
	# transparent to scid. It just has to write the CmailGameName extra
	# tag within the header to a unique ID.
	#----------------------------------------------------------------------
	proc chooseFetch    {} {
		global ::CorrespondenceChess::XfccFetchcmd

		set filetype { "All files" {".*"} }
		set XfccFetchcmd [chooseFile "default correspondence chess Fetch Tool..." $filetype]
	}

	#----------------------------------------------------------------------
	# Xfcc send utility. Similar to fetch but just the other way round
	# ;)
	#----------------------------------------------------------------------
	proc chooseSend     {} {
		global ::CorrespondenceChess::XfccSendcmd

		set filetype { "All files" {".*"} }
		set XfccSendcmd [chooseFile "default correspondence chess Send Tool..." $filetype]
	}

	#----------------------------------------------------------------------
	# Check for xfccrc
	#----------------------------------------------------------------------
	proc checkXfccrc {} {
		global ::CorrespondenceChess::xfccrcfile

		if {![file exists $xfccrcfile]} {
			if {[catch {open $xfccrcfile w} optionF]} {
				tk_messageBox -title "Scid: Unable to write file" -type ok -icon warning \
					-message "Unable to write options file: $xfccrcfile\n$optionF"
			} else {
				puts $optionF "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
				puts $optionF "<xfcc>"
				puts $optionF "<server>"
				puts $optionF "   <name>ServerName</name>"
				puts $optionF "   <uri>http://</uri>"
				puts $optionF "   <user>UserName</user>"
				puts $optionF "   <pass>PassWord</pass>"
				puts $optionF "</server>"
				puts $optionF "</xfcc>"
				close $optionF
			}
		}
	}

	#----------------------------------------------------------------------
	# Check for the default DB, create it if it does not exist.
	#----------------------------------------------------------------------
	proc checkCorrBase {} {
		global ::CorrespondenceChess::CorrBase

		if {![file exists $CorrBase]} {
			set currbase [sc_base current]
			set fName [file rootname $CorrBase]
			if {[catch {sc_base create $fName} newbase]} {
				ERROR::MessageBox "$fName\n"
			}
			# Type 6 == Correspondence chess
			sc_base extra $newbase type 6
			sc_base close $newbase
			sc_base switch $currbase
		}
	}

	#----------------------------------------------------------------------
	# Check for In-/Outbox directories and create them if not available
	#----------------------------------------------------------------------
	proc checkInOutbox {} {
		global scidDataDir ::CorrespondenceChess::Inbox ::CorrespondenceChess::Outbox

		if {[file exists $Inbox]} {
			if {[file isfile $Inbox]} {
				file rename -force $Inbox "$Inbox.bak"
				file mkdir $Inbox
			}
		} else {
			if {[catch { file mkdir "$Inbox" } result]} {
				set ::CorrespondenceChess::Inbox [file nativename [file join $scidDataDir "Inbox"]]
				file mkdir $Inbox
			}
		}

		if {[file exists $Outbox]} {
			if {[file isfile $Outbox]} {
				file rename -force $Outbox "$Outbox.bak"
				file mkdir $Outbox
			}
		} else {
			if {[catch { file mkdir "$Outbox" } result]} {
				set ::CorrespondenceChess::Inbox [file nativename [file join $scidDataDir "Outbox"]]
				file mkdir $Outbox
			}
		}
	}

	#----------------------------------------------------------------------
	# Save the Correspondence Chess options
	#----------------------------------------------------------------------
	proc saveCCoptions {} {
		set optionF ""
		if {[catch {open [scidConfigFile correspondence] w} optionF]} {
			tk_messageBox -title "Scid: Unable to write file" -type ok -icon warning \
				-message "Unable to write options file: [scidConfigFile correspondence]\n$optionF"
		} else {
			# Check all paths etc. exist and contain valid data
			::CorrespondenceChess::checkInOutbox
			::CorrespondenceChess::checkXfccrc
			::CorrespondenceChess::checkCorrBase

			puts $optionF "# Scid options file"
			puts $optionF "# Version: $::scidVersion, $::scidVersionDate"
			puts $optionF "# This file contains commands in the Tcl language format."
			puts $optionF "# If you edit this file, you must preserve valid Tcl"
			puts $optionF "# format or it will not set your Scid options properly."
			puts $optionF ""

			foreach i { ::CorrespondenceChess::CorrBase       \
							::CorrespondenceChess::Inbox          \
							::CorrespondenceChess::Outbox         \
							::CorrespondenceChess::XfccFetchcmd   \
							::CorrespondenceChess::XfccSendcmd    \
							::CorrespondenceChess::mailer         \
							::CorrespondenceChess::bccaddr        \
							::CorrespondenceChess::mailermode     \
							::CorrespondenceChess::attache        \
							::CorrespondenceChess::subject        \
							::CorrespondenceChess::PluginPath     \
							::CorrespondenceChess::Connector      \
							::CorrespondenceChess::RelayGames     \
							::CorrespondenceChess::ListOrder  } {
				set path [set $i]

				puts $optionF "set $i [list [set $i]]"

				# If possible replace absolute path by a relative one to
				# $scidDataDir

				# first get rid of windows path separators as they get
				# interpreted by TCL
				# regsub -all {\\} $::scidDataDir "/" sdd
				# regsub -all {\\}  $path "/" pd

				# if { [regexp $sdd $pd] } {
				#	regsub -all $sdd $pd "scidDataDir" path
				#	# now convert back to nativename
				#	set path [file nativename $path]
				#	puts $optionF "set $i \$$path"
				#} else {
				#	puts $optionF "set $i [list [set $i]]"
				#}

			}
			foreach i { ::CorrespondenceChess::xfccrcfile     \
			} {
				puts $optionF "set $i [list [set $i]]"

				# set path [set $i]
				# regsub -all {\\} $::scidConfigDir "/" sdd
				# regsub -all {\\} $path "/" pd
				# if { [regexp $sdd $pd] } {
				#	regsub -all $sdd $pd "scidDataDir" path
				#	set path [file nativename $path]
				#	puts $optionF "set $i \$$path"
				#} else {
				#	puts $optionF "set $i [list [set $i]]"
				#}

			}
			if {$::CorrespondenceChess::XfccInternal < 0}  {
				puts $optionF {set ::CorrespondenceChess::XfccInternal 0}
			} else {
				puts $optionF "set ::CorrespondenceChess::XfccInternal $::CorrespondenceChess::XfccInternal"
			}
			if {$::CorrespondenceChess::XfccConfirm < 0}  {
				puts $optionF {set ::CorrespondenceChess::XfccConfirm 0}
			} else {
				puts $optionF "set ::CorrespondenceChess::XfccConfirm $::CorrespondenceChess::XfccConfirm"
			}
			if {$::CorrespondenceChess::ListOnlyOwnMove < 0}  {
				puts $optionF {set ::CorrespondenceChess::ListOnlyOwnMove 0}
			} else {
				puts $optionF "set ::CorrespondenceChess::ListOnlyOwnMove $::CorrespondenceChess::ListOnlyOwnMove"
			}

		}
		close $optionF
		set ::statusBar "Correspondence chess options were saved to: [scidConfigFile correspondence]"
	}

	#----------------------------------------------------------------------
	# yset / yview: enable synchronous scrolling of the CC game list, ie.
	# all text widgets involved scroll simultaneously by the same amount
	# in the vertical direction.
	#----------------------------------------------------------------------
	proc yset {args} {
		set w .ccWindow
		eval [linsert $args 0 $w.bottom.ysc set]
		yview moveto [lindex [$w.bottom.ysc get] 0]
	}

	proc yview {args} {
		set w .ccWindow
		eval [linsert $args 0 $w.bottom.id      yview]
		eval [linsert $args 0 $w.bottom.toMove  yview]
		eval [linsert $args 0 $w.bottom.event   yview]
		eval [linsert $args 0 $w.bottom.site    yview]
		eval [linsert $args 0 $w.bottom.white   yview]
		eval [linsert $args 0 $w.bottom.black   yview]
		eval [linsert $args 0 $w.bottom.clockW  yview]
		eval [linsert $args 0 $w.bottom.clockB  yview]
		eval [linsert $args 0 $w.bottom.var     yview]
		eval [linsert $args 0 $w.bottom.feature yview]
	}

	#----------------------------------------------------------------------
	# Translate the local menu
	#----------------------------------------------------------------------
	proc doConfigMenus { } {
		set lang $::language

		if {! [winfo exists .ccWindow]} {
			raiseWin .ccWindow
			return
		}

		set m .ccWindow.menu

		foreach idx {0 1} tag {CorrespondenceChess Edit} {
			configMenuText $m $idx $tag $lang
		}
		foreach idx {0 1 3 4 6 7 8 9 10 11 13 14} tag {CCConfigure CCConfigRelay CCRetrieve  CCInbox  CCSend  CCResign  CCClaimDraw CCOfferDraw CCAcceptDraw CCGamePage  CCNewMailGame CCMailMove } {
			configMenuText $m.correspondence $idx $tag $lang
		}
		foreach idx {0 } tag { CCEditCopy } {
			configMenuText $m.edit $idx $tag $lang
		}
	}

	#----------------------------------------------------------------------
	# Call the web page of the game. The URL is extracted from the
	# Source tag that is stored with each game.
	#----------------------------------------------------------------------
	proc CallWWWGame {} {
		::CorrespondenceChess::updateConsole "Calling web page..."
		set Extra  [sc_game tags get Extra]
		set extraTagsList [split $Extra "\n"]
		set source ""
		foreach i $extraTagsList {
			if { [string equal -nocase [lindex $i 0] "Source" ] } {
				set source [string range $i 8 end-1]
				openURL $source
			}
		}
	}

	#----------------------------------------------------------------------
	# Store the relays list, but only those URLs that match
	# iccf-webchess' games page.
	#----------------------------------------------------------------------
	proc RelaysOK { } {
		global ::CorrespondenceChess::RelayGames

		set w .editCCRelays

		if {[catch {open $::CorrespondenceChess::Connector r} connectF]} {
				set Title "Error"
				append Error "$::CorrespondenceChess::Connector\n"
				append Error [::tr CCErrDirNotUsable]
				tk_messageBox -icon warning -type ok -parent . \
					-title $Title -message $Error
				return
		} else {
			set connectxml [read $connectF]

			set dom [dom parse $connectxml]
			set doc [$dom documentElement]
			set aNodes [$doc selectNodes {/connector/server}]
			set number   0
			foreach srv $aNodes {
				set stripforid   [$srv selectNodes {string(stripforid)}]

				set text [string trim [$w.f.text get 1.0 end]]
				set ::CorrespondenceChess::RelayGames {}
				foreach game [split $text "\n"] {
					set game [string trim $game]
					if {[string match "*$stripforid*" $game]} {
						lappend ::CorrespondenceChess::RelayGames $game
					}
				}
			}
			close $connectF
		}

		::CorrespondenceChess::saveCCoptions
		destroy .editCCRelays
	}

	#----------------------------------------------------------------------
	# Configure the games to be relayed from ICCF Webchess
	#----------------------------------------------------------------------
	proc ConfigureRelay { } {
		global ::CorrespondenceChess::RelayGames

		puts stderr $::CorrespondenceChess::Connector
		if {![file exists $::CorrespondenceChess::Connector]} {
				if {[catch {open $::CorrespondenceChess::Connector w} connectF]} {

				} else {
					puts $connectF "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
					puts $connectF "<connector>";
					puts $connectF "\t<server>";
					puts $connectF "\t\t<name>ICCF</name>";
					puts $connectF "\t\t<stripforid>http://www.iccf-webchess.com/MakeAMove.aspx\?id=</stripforid>";
					puts $connectF "\t\t<pgnbaseurl>http://www.iccf-webchess.com/GetPGN.aspx?id=</pgnbaseurl>";
					puts $connectF "\t\t<cmailprefix>game</cmailprefix>";
					puts $connectF "\t</server>";
					puts $connectF "</connector>";
					close $connectF
				}
		}

		if {[catch {open $::CorrespondenceChess::Connector r} connectF]} {
				set Title "Error"
				append Error "$::CorrespondenceChess::Connector\n"
				append Error [::tr CCErrDirNotUsable]
				tk_messageBox -icon warning -type ok -parent . \
					-title $Title -message $Error
				return
		} else {
			close $connectF
			set w .editCCRelays
			set oldRelays $::CorrespondenceChess::RelayGames

			if {[winfo exists $w]} { return }
			toplevel $w
			::setTitle $w [::tr "CCDlgConfigRelay"]

			autoscrollText y $w.desc $w.desc.text Treeview
			$w.desc.text configure -width 60 -height 7 -wrap word -cursor top_left_arrow -state normal
			$w.desc.text insert end [::tr "CCDlgConfigRelayHelp"]
			$w.desc.text configure -state disabled
			pack $w.desc -side top -fill x

			autoscrollText y $w.f $w.f.text Treeview
			$w.f.text configure -width 60 -height 10 -wrap none -state normal -relief sunken

			foreach g $::CorrespondenceChess::RelayGames {
				$w.f.text insert end "$g\n"
			}
			pack $w.f -side top -fill both -expand yes

			pack [ttk::frame $w.b] -side bottom -fill x
			ttk::button $w.b.ok -text OK -command {
					::CorrespondenceChess::RelaysOK
			}
			ttk::button $w.b.cancel -text $::tr(Cancel) -command "grab release $w; destroy $w"
			pack $w.b.cancel $w.b.ok -side right -padx 5 -pady 5
		}
	}

	#----------------------------------------------------------------------
	# Fetch PGN file of games to be relayed and put them with the
	# proper header tags into Scid's inbox for display
	# As parameter use the MakeAMove-URL from ICCF.
	# Currently only relaying from ICCF is supported.
	#----------------------------------------------------------------------
	proc RelayGames { gameurl } {
		global ::CorrespondenceChess::Inbox

		if {[catch {open $::CorrespondenceChess::Connector r} connectF]} {
			::CorrespondenceChess::updateConsole "info ERROR: Unable to open connector $::CorrespondenceChess::Connector";
		} else {

			set connectxml [read $connectF]

			set dom [dom parse $connectxml]
			set doc [$dom documentElement]
			set aNodes [$doc selectNodes {/connector/server}]
			set number   0
			foreach srv $aNodes {
				set name         [$srv selectNodes {string(name)}]
				set stripforid   [$srv selectNodes {string(stripforid)}]
				set pgnbaseurl   [$srv selectNodes {string(pgnbaseurl)}]
				set cmailprefix  [$srv selectNodes {string(cmailprefix)}]

				if {[regexp "$stripforid" $gameurl]} {

					regsub -all "$stripforid" $gameurl {} gameid

					::CorrespondenceChess::updateConsole "info Fetching $gameid from $name";
					set cmailgamename   "$cmailprefix$gameid"
					set pgnurl          "$pgnbaseurl$gameid"

					# convert from latin-1 to utf-8
					set pgn [encoding convertfrom iso8859-1 [::CorrespondenceChess::getPage $pgnurl ]]

					# split by line endings for insertion of necessary header tags
					set gamelist [split $pgn "\n"]

					set filename [file nativename [file join $::CorrespondenceChess::Inbox "$cmailgamename.pgn"]]

					if {[catch {open $filename w} pgnF]} {
						::CorrespondenceChess::updateConsole "info ERROR: Unable to open $filename";
					} else {
						foreach line $gamelist {
							if {[string match "*Result *" $line]} {
								puts $pgnF $line
								puts $pgnF "\[CmailGameName \"$cmailgamename\"\]"
								puts $pgnF "\[Source \"$gameurl\"\]"
								puts $pgnF "\[Mode \"Relay\"\]"
							} else {
								puts $pgnF $line
							}
						}
						close $pgnF
					}
				}

			}
		}
		close $connectF
	}

	#----------------------------------------------------------------------
	# Resize the console window
	#----------------------------------------------------------------------
	proc ConsoleResize {} {
		set w .ccWindow

		# unbind configure event
		bind $w <Configure> {}

		# get old window width and height
		set oldheight $::winHeight($w)
		set oldwidth  $::winWidth($w)

		# get the new window width and height
		set temp [wm geometry $w]
		set n [scan $temp "%dx%d+%d+%d" width height x y]

		if {$height > 0 && $width > 0} {
			if {$height != $oldheight} {
				# resize the table of games
				foreach col {id toMove event site white black clockW clockB var feature} {
					$w.bottom.$col  configure -height $height
				}
				# record the new size
				recordWinSize $w
				# set the windows size to this new size explicitly to
				# avoid flicker
				setWinSize $w
			}
		}
		recordWinSize $w
		# rebind the configure event
		bind $w <Configure> { ::CorrespondenceChess::ConsoleResize }
	}

	#----------------------------------------------------------------------
	# Allow to disable engine analysis in case engines are not allowed
	# for the ongoing game.
	#----------------------------------------------------------------------
	proc EnableEngineAnalysis {on} {
		# Broken: depends on the old menu structure
		# TODO: use the new callback mechanism ::setPlayMode()
		return

		if {$on == 0} {
			set m .menu.tools
			$m entryconfigure 0 -state disabled
			$m entryconfigure 1 -state disabled
			$m entryconfigure 2 -state disabled
			$m entryconfigure 3 -state disabled

			# disable hotkeys, needs to be done for each window
			foreach w { .maintWin .sortWin .playerInfoWin .repWin \
							.fics .metadataWindow .crosstableWin .ecograph \
							.glistWin .plist .statsWin .baseWin .tourney \
							.pgnWin .main .nedit .ccWindow } {

				if {[winfo exists $w]} {
					bind $w <Control-A> {}
					bind $w <Control-Shift-2> {}
					bind $w <F2> {}
					bind $w <F3> {}
				}
			}
		} else {
			set m .menu.tools
			$m entryconfigure 0 -state normal
			$m entryconfigure 1 -state normal
			$m entryconfigure 2 -state normal
			$m entryconfigure 3 -state normal

			# disable hotkeys, needs to be done for each window
			foreach w { .maintWin .sortWin .playerInfoWin .repWin \
							.fics .metadataWindow .crosstableWin .ecograph \
							.glistWin .plist .statsWin .baseWin .tourney \
							.pgnWin .main .nedit .ccWindow } {

				if {[winfo exists $w]} {
					bind $w <Control-A> makeAnalysisWin
					bind $w <Control-Shift-2> "makeAnalysisWin 2"
					bind $w <F2> "::makeAnalysisWin 1 0"
					bind $w <F3> "::makeAnalysisWin 2 0"
				}
			}
		}
	}

	#----------------------------------------------------------------------
	# Copy the games list as CSV (tab separated) to the clipboard
	#----------------------------------------------------------------------
	proc List2Clipboard {} {
		clipboard clear
		clipboard append $::CorrespondenceChess::clipboardText
	}

	#----------------------------------------------------------------------
	# Generate the Correspondence Chess Window. This Window offers a
	# console displaying whats going on and which game is displayed
	# plus a gmae list containing current games synced in and their
	# status. Xfcc offers quite some information here whereas eMail
	# relies mostly on the user.
	# Additionally this window contains the buttons for easy navigation
	# and in case of Xfcc the special moves available (resign etc.)
	#----------------------------------------------------------------------
	proc CCWindow {} {
		global scidDataDir helpMessage

		set w .ccWindow
		if {[winfo exists .ccWindow]} {
			focus .
			destroy .ccWindow
			set ::CorrespondenceChess::isOpen 0
			return
		}
		set ::CorrespondenceChess::isOpen 1

		::createToplevel $w
		::setTitle $w [::tr "CorrespondenceChess"]

		# hook up with scid's geometry manager
		setWinLocation $w
		setWinSize $w

		::CorrespondenceChess::EnableEngineAnalysis 0

		# create the menu and add default CC menu items here as well
		menu $w.menu
		::setMenu $w $w.menu
		set m $w.menu
		$w.menu add cascade -label CorrespondenceChess -menu $w.menu.correspondence
		$w.menu add cascade -label Edit                -menu $w.menu.edit
		foreach i {correspondence edit} {
			menu $w.menu.$i -tearoff 0
		}

		$m.correspondence add command -label CCConfigure   -command {::CorrespondenceChess::config}
		set helpMessage($m.correspondence,0) CCConfigure
		$m.correspondence add command -label CCConfigRelay -command {::CorrespondenceChess::ConfigureRelay}
		set helpMessage($m.correspondence,1) CCConfigRelay

		$m.correspondence add separator
		$m.correspondence add command -label CCRetrieve    -command { ::CorrespondenceChess::FetchGames }
		set helpMessage($m.correspondence,3) CCRetrieve

		$m.correspondence add command -label CCInbox       -command { ::CorrespondenceChess::ReadInbox }
		set helpMessage($m.correspondence,4) CCInbox

		$m.correspondence add separator
		$m.correspondence add command -label CCSend        -command {::CorrespondenceChess::SendMove 0 0 0 0}
		set helpMessage($m.correspondence,6) CCSend
		$m.correspondence add command -label CCResign      -command {::CorrespondenceChess::SendMove 1 0 0 0}
		set helpMessage($m.correspondence,7) CCResign
		$m.correspondence add command -label CCClaimDraw   -command {::CorrespondenceChess::SendMove 0 1 0 0}
		set helpMessage($m.correspondence,8) CCClaimDraw
		$m.correspondence add command -label CCOfferDraw   -command {::CorrespondenceChess::SendMove 0 0 1 0}
		set helpMessage($m.correspondence,9) CCOfferDraw
		$m.correspondence add command -label CCAcceptDraw  -command {::CorrespondenceChess::SendMove 0 0 0 1}
		set helpMessage($m.correspondence,10) CCAcceptDraw
		$m.correspondence add command -label CCGamePage    -command {::CorrespondenceChess::CallWWWGame}
		set helpMessage($m.correspondence,11) CCGamePage
		$m.correspondence add separator
		$m.correspondence add command -label CCNewMailGame -command {::CorrespondenceChess::newEMailGame}
		set helpMessage($m.correspondence,13) CCNewMailGame
		$m.correspondence add command -label CCMailMove    -command {::CorrespondenceChess::eMailMove}
		set helpMessage($m.correspondence,14) CCMailMove

		$m.edit add command -label CCEditCopy -accelerator "Ctrl+C" -command { ::CorrespondenceChess::List2Clipboard }

		# Translate the menu
		::CorrespondenceChess::doConfigMenus

		ttk::frame $w.top
		ttk::frame $w.bottom

		pack $w.top -anchor w -expand no
		pack $w.bottom -fill both -expand yes

		ttk::scrollbar $w.top.ysc        -command { .ccWindow.top.console yview }
		text      $w.top.console    -height 3 -width 80 -wrap word -yscrollcommand "$w.top.ysc set"
		ttk::button    $w.top.retrieveCC -image tb_CC_Retrieve        -command {::CorrespondenceChess::FetchGames}
		ttk::button    $w.top.sendCC     -image tb_CC_Send            -command {::CorrespondenceChess::SendMove 0 0 0 0}
		ttk::button    $w.top.delinbox   -image tb_CC_delete          -command {::CorrespondenceChess::EmptyInOutbox}

		ttk::button    $w.top.openDB     -text  [::tr "CCOpenDB"]     -command {::CorrespondenceChess::OpenCorrespondenceDB}
		ttk::button    $w.top.inbox      -text  [::tr "CCInbox"]      -command {::CorrespondenceChess::ReadInbox}

		ttk::button    $w.top.resign     -text  [::tr "CCResign"]     -state disabled -command {::CorrespondenceChess::SendMove 1 0 0 0}
		ttk::button    $w.top.claimDraw  -text  [::tr "CCClaimDraw"]  -state disabled -command {::CorrespondenceChess::SendMove 0 1 0 0}
		ttk::button    $w.top.offerDraw  -text  [::tr "CCOfferDraw"]  -state disabled -command {::CorrespondenceChess::SendMove 0 0 1 0}
		ttk::button    $w.top.acceptDraw -text  [::tr "CCAcceptDraw"] -state disabled -command {::CorrespondenceChess::SendMove 0 0 0 1}

		ttk::button    $w.top.help       -image tb_help -width 24 -command { helpWindow CCIcons }

		ttk::label $w.top.plugins    -image tb_CC_spacer  -takefocus 0
		ttk::label $w.top.onoffline  -image tb_CC_offline -takefocus 0


		::utils::tooltip::Set $w.top.retrieveCC [::tr "CCFetchBtn"]
		::utils::tooltip::Set $w.top.sendCC     [::tr "CCSendBtn"]
		::utils::tooltip::Set $w.top.delinbox   [::tr "CCEmptyBtn"]
		::utils::tooltip::Set $w.top.help       [::tr "CCHelpBtn"]
		::utils::tooltip::Set $w.top.onoffline  [clock format $::Xfcc::lastupdate]

 		grid $w.top.retrieveCC  -stick ewns  -column  0 -row 0
 		grid $w.top.openDB      -stick ew    -column  0 -row 1 -columnspan 2
 		grid $w.top.inbox       -stick ew    -column  0 -row 2 -columnspan 2

 		grid $w.top.sendCC      -stick ewns  -column  1 -row 0

		grid $w.top.console                  -column  4 -row 0 -columnspan 8
		grid $w.top.ysc         -stick ns    -column 13 -row 0
		grid $w.top.help        -stick nsew  -column 14 -row 0 -columnspan 2

		grid $w.top.delinbox    -stick ewns  -column  5 -row 1 -rowspan 2
		grid $w.top.onoffline                -column  6 -row 1
		grid $w.top.plugins                  -column  6 -row 2

		grid $w.top.resign      -stick ew    -column  7 -row 1

		grid $w.top.claimDraw   -stick ew    -column  7 -row 2
		grid $w.top.offerDraw   -stick ew    -column  8 -row 2
		grid $w.top.acceptDraw  -stick ew    -column  9 -row 2

		# build the table in the bottom frame. This table of text widgets has to
		# scroll synchronously!
		ttk::scrollbar $w.bottom.ysc      -command ::CorrespondenceChess::yview

		set height $::winHeight($w)
		set width  $::winWidth($w)

		text $w.bottom.id       -cursor top_left_arrow -font font_Small -height $height -width 15 -setgrid 1 -relief flat -wrap none -yscrollcommand ::CorrespondenceChess::yset
		text $w.bottom.toMove   -cursor top_left_arrow -font font_Small -height $height -width 4  -setgrid 1 -relief flat -wrap none -yscrollcommand ::CorrespondenceChess::yset
		text $w.bottom.event    -cursor top_left_arrow -font font_Small -height $height -width 10 -setgrid 1 -relief flat -wrap none -yscrollcommand ::CorrespondenceChess::yset
		text $w.bottom.site     -cursor top_left_arrow -font font_Small -height $height -width 10 -setgrid 1 -relief flat -wrap none -yscrollcommand ::CorrespondenceChess::yset
		text $w.bottom.white    -cursor top_left_arrow -font font_Small -height $height -width 15 -setgrid 1 -relief flat -wrap none -yscrollcommand ::CorrespondenceChess::yset
		text $w.bottom.black    -cursor top_left_arrow -font font_Small -height $height -width 15 -setgrid 1 -relief flat -wrap none -yscrollcommand ::CorrespondenceChess::yset
		text $w.bottom.clockW   -cursor top_left_arrow -font font_Small -height $height -width 10 -setgrid 1 -relief flat -wrap none -yscrollcommand ::CorrespondenceChess::yset
		text $w.bottom.clockB   -cursor top_left_arrow -font font_Small -height $height -width 10 -setgrid 1 -relief flat -wrap none -yscrollcommand ::CorrespondenceChess::yset
		text $w.bottom.var      -cursor top_left_arrow -font font_Small -height $height -width 3  -setgrid 1 -relief flat -wrap none -yscrollcommand ::CorrespondenceChess::yset
		text $w.bottom.feature  -cursor top_left_arrow -font font_Small -height $height -width 16 -setgrid 1 -relief flat -wrap none -yscrollcommand ::CorrespondenceChess::yset

		grid $w.bottom.id       -column  0 -row 1
		grid $w.bottom.toMove   -column  1 -row 1
		grid $w.bottom.event    -column  2 -row 1
		grid $w.bottom.site     -column  3 -row 1
		grid $w.bottom.white    -column  4 -row 1
		grid $w.bottom.black    -column  5 -row 1
		grid $w.bottom.clockW   -column 15 -row 1
		grid $w.bottom.clockB   -column 16 -row 1
		grid $w.bottom.var      -column 17 -row 1
		grid $w.bottom.feature  -column 18 -row 1
		grid $w.bottom.ysc      -column 19 -row 1 -stick ns

		# Copy games list to clipboard
		bind $w <Control-Insert> { ::CorrespondenceChess::List2Clipboard }
		bind $w <Control-c>      { ::CorrespondenceChess::List2Clipboard }

		# Handle scrolling in the games list by keyboard
		bind $w <Control-Up>     { ::CorrespondenceChess::PrevGame}
		bind $w <Control-Down>   { ::CorrespondenceChess::NextGame}
		bind $w <Up>         { ::CorrespondenceChess::yview scroll -1 units}
		bind $w <Down>       { ::CorrespondenceChess::yview scroll  1 units}
		bind $w <Prior>      { ::CorrespondenceChess::yview scroll -1 pages}
		bind $w <Next>       { ::CorrespondenceChess::yview scroll  1 pages}
		bindMouseWheel $w "::CorrespondenceChess::yview scroll"

		# Help
		bind $w <F1>        { helpWindow Correspondence}
		bind $w "?"         { helpWindow CCIcons}

		bind $w <Configure> { ::CorrespondenceChess::ConsoleResize }
		bind $w <Destroy>   { ::CorrespondenceChess::EnableEngineAnalysis 1
			set ::CorrespondenceChess::isOpen 0 }

		foreach f [glob -nocomplain [file join "$CorrespondenceChess::PluginPath" *]] {
			$w.top.plugins    configure -image tb_CC_pluginactive
			source $f
		}

		::createToplevelFinalize $w
	}

	#--------------------------------------------------------------------------
	# Updates the game list with another event and all data available.
	# This just adds another line at the end of the current list, hence
	# the list has to be emptied if all games are resynced in.
	#--------------------------------------------------------------------------
	proc updateGamelist {id toMove event site date white black clockW \
								clockB var db books tb engines wc bc mess TC \
								lastmove drawoffer } {
		global ::CorrespondenceChess::num
		global ::CorrespondenceChess::clipboardText

		set w .ccWindow

		#----------------------------------------------------------------------
		# Layout for the gamelist: Xfcc offers more information about
		# the ongoing game then eMail, hence more is presented to the
		# user. ToMove and features use icons for easy reading.
		# Xfcc:
		# ID | ToMove? | White | Black | Event | Site | ClockW | ClockB # | Var | features
		# eMail:
		# ID |         | White | Black | Event | Site |        |         |     |

		foreach tag {id toMove event site white black clockW clockB var feature} {
			# enable additions
			$w.bottom.$tag      configure -state normal
			# make each line high enough for the icons to be placed
			$w.bottom.$tag      image create end -align center -image tb_CC_spacer
		}

		# Calculate the TimeDiff between the event date and the current
		# date. This diff is used to mark event that have not yet
		# started.
		set TimeDiff [expr [clock seconds] - [clock scan $date -format "%Y.%m.%d"] ]

		if { $::Xfcc::update > 0 } {
			$w.top.onoffline  configure -image tb_CC_online
			::utils::tooltip::Set $w.top.onoffline  [clock format $::Xfcc::lastupdate]
		}

		if {$mess != ""} {
			set curpos [$w.bottom.id index insert]
			$w.bottom.id image create end -align center -image tb_CC_message
			set endpos [$w.bottom.id index insert]

			$w.bottom.id tag add idmsg$id $curpos $endpos
			::utils::tooltip::SetTag $w.bottom.id "$mess" idmsg$id
		}
		# add the game id. Note the \n at the end is necessary!
		set curpos [$w.bottom.id index insert]
		$w.bottom.id      insert end "$id\n"
		set endpos [$w.bottom.id index insert]
		$w.bottom.id tag add id$id $curpos $endpos
		::utils::tooltip::SetTag $w.bottom.id "$id" id$id

		# ToMove may contain a mixture of text for game results plus
		# several icons displaying the current game status.
		if { (($clockW == " 0d  0: 0") || ($clockB == " 0d  0: 0")) && (($toMove == "yes") || ($toMove == "no")) } {
				$w.bottom.toMove image create end -align center -image tb_CC_outoftime
		}

		set text ""
		switch -regexp -- $toMove \
		"1-0" {
			set curpos [$w.bottom.toMove index insert]
			$w.bottom.toMove image create end -align center -image $::board::letterToPiece(K)25
			$w.bottom.toMove  insert end " $toMove"
			set endpos [$w.bottom.toMove index insert]
			set text "$lastmove ($toMove)"
		} \
		"0-1" {
			set curpos [$w.bottom.toMove index insert]
			$w.bottom.toMove image create end -align center -image $::board::letterToPiece(k)25
			$w.bottom.toMove  insert end " $toMove"
			set endpos [$w.bottom.toMove index insert]
			set text "$lastmove ($toMove)"
		} \
		" = " {
			set curpos [$w.bottom.toMove index insert]
			$w.bottom.toMove image create end -align center -image tb_CC_draw
			$w.bottom.toMove  insert end "$toMove"
			set endpos [$w.bottom.toMove index insert]
			set text "$lastmove ($toMove)"
		} \
		"yes" {
			set curpos [$w.bottom.toMove index insert]
			$w.bottom.toMove image create end -align center -image tb_CC_yourmove
			set endpos [$w.bottom.toMove index insert]
			set text "$lastmove"
		} \
		"no"  {
			set curpos [$w.bottom.toMove index insert]
			$w.bottom.toMove image create end -align center -image tb_CC_oppmove
			set endpos [$w.bottom.toMove index insert]
			set text "$lastmove"
		} \
		" ? " {
			set curpos [$w.bottom.toMove index insert]
			$w.bottom.toMove  insert end "$toMove"
			set endpos [$w.bottom.toMove index insert]
			set text "$lastmove"
		} \
		"POS" {
			set curpos [$w.bottom.toMove index insert]
			$w.bottom.toMove image create end -align center -image tb_CC_postal
			set endpos [$w.bottom.toMove index insert]
			set text "$lastmove"
		} \
		"EML" {
			set curpos [$w.bottom.toMove index insert]
			$w.bottom.toMove image create end -align center -image tb_CC_envelope
			set endpos [$w.bottom.toMove index insert]
			set text "$lastmove"
		} \
		"REL" {
			set curpos [$w.bottom.toMove index insert]
			$w.bottom.toMove image create end -align center -image tb_CC_relay
			set endpos [$w.bottom.toMove index insert]
			set text "$lastmove"
		}
		$w.bottom.toMove tag add toMove$id $curpos $endpos
		::utils::tooltip::SetTag $w.bottom.toMove "$text" toMove$id
		$w.bottom.toMove insert end "\n"


		# Add textual information to the edit fields
		set curpos [$w.bottom.event index insert]
		$w.bottom.event   insert end "$event\n"
		set endpos [$w.bottom.event index insert]
		$w.bottom.event tag add event$id $curpos $endpos


		set curpos [$w.bottom.site index insert]
		$w.bottom.site    insert end "$site\n"
		set endpos [$w.bottom.site index insert]
		$w.bottom.site tag add site$id $curpos $endpos
		::utils::tooltip::SetTag $w.bottom.site "$site" site$id

		set wc [getFlagImage $wc no]
		if {$wc != ""} {
		    $w.bottom.white   image create end -align center -image $wc
		    $w.bottom.white   insert end " "
		}
		$w.bottom.white   insert end "$white\n"

		set bc [getFlagImage $bc no]
		if {$bc != ""} {
		    $w.bottom.black   image create end -align center -image $bc
		    $w.bottom.black   insert end " "
		}
		$w.bottom.black   insert end "$black\n"

		$w.bottom.clockW  insert end "$clockW\n"
		$w.bottom.clockB  insert end "$clockB\n"
		$w.bottom.var     insert end "$var\n"

		# Xfcc defines noDB, noTablebase no etc.pp. Hence check for
		# false to display the icons for allowed features.
		if {$db == "false"} {
			$w.bottom.feature image create end -align center -image tb_CC_database
		}
		if {$books == "false"} {
			$w.bottom.feature image create end -align center -image tb_CC_book
		}
		if {$tb == "false"} {
			$w.bottom.feature image create end -align center -image tb_CC_tablebase
		}
		if {!($engines == "true")} {
			$w.bottom.feature image create end -align center -image tb_CC_engine
		}

		$w.bottom.feature insert end "\n"

		# Link the click on each field to jump to this specific game
		# easily, then lock the entry field from changes by the user.
		# SetSelection just sets the global $num to the actual row the
		# user clicked. This has to be a global variable and it has to
		# be passed to the ProcessServerResult masqueraded to prevent
		# from interpretation. See also Scid's gamelist.
		foreach tag {id toMove event site white black clockW clockB var feature} {
			bind $w.bottom.$tag <Button-1> {
				::CorrespondenceChess::SetSelection %x %y
				::CorrespondenceChess::ProcessServerResult $::CorrespondenceChess::num
				break }
			# lock the area from changes
			$w.bottom.$tag configure -state disable
		}

		if {$TimeDiff < -1} {
			foreach col {id toMove event site} {
				$w.bottom.$col tag configure $col$id -foreground DarkGray -font font_Bold
			}
			::utils::tooltip::SetTag $w.bottom.event "$event\nTime: $TC\n\nStart: $date" event$id
		} else {
			::utils::tooltip::SetTag $w.bottom.event "$event\nTime: $TC" event$id
		}

		regsub -all "flag_"  $wc "" wc1
		regsub -all "flag_"  $bc "" bc1

		set wc1 [string toupper $wc1]
		set bc1 [string toupper $bc1]

		set ::CorrespondenceChess::clipboardText "$::CorrespondenceChess::clipboardText\n$id\t $event\t$site\t$date\t$white\t$black\t$wc1\t$bc1\t$clockW\t$clockB\t$toMove\t$mess\t$lastmove\t$var\t$db\t$books\t$tb\t$engines\t$TC"
	}

	#----------------------------------------------------------------------
	# Visually highlight line $::CorrespondenceChess::num
	#----------------------------------------------------------------------
	proc SetHighlightedLine {} {
		global ::CorrespondenceChess::num
		set gamecount $::CorrespondenceChess::glgames

		# remove old highlighting
		foreach col {id toMove event site white black clockW clockB var feature} {
			.ccWindow.bottom.$col tag remove highlight 1.0 end
		}

		# highlight current games line
		foreach col {id toMove event site white black clockW clockB var feature} {
			.ccWindow.bottom.$col tag add highlight $num.0 [expr {$num+1}].0
			.ccWindow.bottom.$col tag configure highlight -background lightYellow2 -font font_Bold
		}
		updateConsole "info: switched to game $num/$gamecount"
	}

	#----------------------------------------------------------------------
	# Set the global $num to the row the user clicked upon
	#----------------------------------------------------------------------
	proc SetSelection {xcoord ycoord} {
		global ::CorrespondenceChess::num
		set gamecount $::CorrespondenceChess::glgames

		set num [expr {int([.ccWindow.bottom.id index @$xcoord,$ycoord]) + $::CorrespondenceChess::glccstart - 1 }]

		# Prevent clicking beyond the last game
		if { $num > $gamecount } {
				set num $gamecount
		}

		SetHighlightedLine
	}

	#----------------------------------------------------------------------
	# Empties the gamelist and reset global $num. This should be done
	# once the games are (re)synchronised.
	#----------------------------------------------------------------------
	proc emptyGamelist {} {
		set w .ccWindow
		foreach tag {id toMove event site white black clockW clockB var feature} {
			# unlock the list
			$w.bottom.$tag      configure -state normal
			# delete it
			$w.bottom.$tag      delete 1.0 end
		}
		# reset the number of processed games
		set ::CorrespondenceChess::num 0
		set ::CorrespondenceChess::clipboardText ""

	}

	#----------------------------------------------------------------------
	# Add a line to the status console
	#----------------------------------------------------------------------
	proc updateConsole {line} {
		set t .ccWindow.top.console
		if { [winfo exists $t] } {
			$t insert end "$line\n"
			$t yview moveto 1
		}
	}

	#----------------------------------------------------------------------
	# Opens a config dialog to set the default parameters. Currently
	# they are not stored to scid's setup though.
	#----------------------------------------------------------------------
	proc config {} {
		set w .correspondenceChessConfig
		if { [winfo exists $w]} {
			raiseWin $w
			return
		}
		win::createDialog $w
		wm title $w [::tr "CCDlgConfigureWindowTitle"]

		set ::CorrespondenceChess::sortoptlist [list \
			[::tr "CCOrderClassicTxt"] \
			[::tr "CCOrderMyTimeTxt"] \
			[::tr "CCOrderTimePerMoveTxt"] \
			[::tr "CCOrderStartDate"] \
			[::tr "CCOrderOppTimeTxt"] \
		]


	        ttk::frame $w.buttons
		ttk::button $w.bOk     -text OK -command {
				::CorrespondenceChess::saveCCoptions
				destroy .correspondenceChessConfig
		}
		ttk::button $w.bCancel -text [::tr "Cancel"] -command "destroy $w"

		ttk::labelframe $w.lgeneral -text [::tr "CCDlgCGeneraloptions"]
		ttk::label  $w.ldb      -text [::tr "CCDlgDefaultDB"]
		ttk::label  $w.linbox   -text [::tr "CCDlgInbox"]
		ttk::label  $w.loutbox  -text [::tr "CCDlgOutbox"]

		ttk::labelframe  $w.lxfccrc  -text [::tr "CCDlgXfcc"]
		ttk::label  $w.lxfcc    -text [::tr "CCDlgExternalProtocol"]
		ttk::label  $w.lfetch   -text [::tr "CCDlgFetchTool"]
		ttk::label  $w.lsend    -text [::tr "CCDlgSendTool"]
		ttk::label  $w.lsortopt -text [::tr "CCDlgSortOption"]

		ttk::labelframe  $w.lemail   -text [::tr "CCDlgEmailCommunication"]
		ttk::label  $w.lmailx   -text [::tr "CCDlgMailPrg"]
		ttk::label  $w.lbccaddr -text [::tr "CCDlgBCCAddr"]
		ttk::label  $w.lmoderb  -text [::tr "CCDlgMailerMode"]
		ttk::label  $w.lattache -text [::tr "CCDlgAttachementPar"]
		ttk::label  $w.lsubject -text [::tr "CCDlgSubjectPar"]

		ttk::checkbutton $w.internalXfcc -text [::tr "CCDlgInternalXfcc"] \
			-variable ::CorrespondenceChess::XfccInternal
		ttk::checkbutton $w.confirmXfcc -text [::tr "CCDlgConfirmXfcc"] \
			-variable ::CorrespondenceChess::XfccConfirm
		ttk::checkbutton $w.onlyOwnMove -text [::tr "CCDlgListOnlyOwnMove"] \
			-variable ::CorrespondenceChess::ListOnlyOwnMove

		ttk::scrollbar $w.ysc    -command { .correspondenceChessConfig.sortopt yview }
		listbox   $w.sortopt -height 3 -width 60 -exportselection 0 -selectmode single -list ::CorrespondenceChess::sortoptlist -yscrollcommand "$w.ysc set"
		$w.sortopt selection set $::CorrespondenceChess::ListOrder
		bind .correspondenceChessConfig.sortopt <<ListboxSelect>> {
			set ::CorrespondenceChess::ListOrder [ .correspondenceChessConfig.sortopt curselection ]
		}

		ttk::button $w.xfconf  -text [::tr CCConfigure] -command { ::CorrespondenceChess::checkXfccrc
			::Xfcc::config $::CorrespondenceChess::xfccrcfile}

		if {$::CorrespondenceChess::XfccInternal < 0} {
			$w.internalXfcc configure -state disabled
			$w.xfconf       configure -state disabled
		}

		ttk::entry  $w.db      -width 60 -textvariable ::CorrespondenceChess::CorrBase
		ttk::entry  $w.inbox   -width 60 -textvariable ::CorrespondenceChess::Inbox
		ttk::entry  $w.outbox  -width 60 -textvariable ::CorrespondenceChess::Outbox

		ttk::entry  $w.xfccrc  -width 60 -textvariable ::CorrespondenceChess::xfccrcfile
		ttk::entry  $w.fetch   -width 60 -textvariable ::CorrespondenceChess::XfccFetchcmd
		ttk::entry  $w.send    -width 60 -textvariable ::CorrespondenceChess::XfccSendcmd

		ttk::entry  $w.mailx   -width 60 -textvariable ::CorrespondenceChess::mailer
		ttk::entry  $w.bccaddr -width 60 -textvariable ::CorrespondenceChess::bccaddr
		ttk::entry  $w.attache -width 30 -textvariable ::CorrespondenceChess::attache
		ttk::entry  $w.subject -width 30 -textvariable ::CorrespondenceChess::subject

		ttk::radiobutton $w.moderb1 -text "Mozilla  \($::tr(CCDlgThunderbirdEg)\)" -value "mozilla" -variable ::CorrespondenceChess::mailermode
		ttk::radiobutton $w.moderb2 -text "Mail-URL  \($::tr(CCDlgMailUrlEg)\)"    -value "mailurl" -variable ::CorrespondenceChess::mailermode
		ttk::radiobutton $w.moderb3 -text "Claws  \($::tr(CCDlgClawsEg)\)"         -value "claws"   -variable ::CorrespondenceChess::mailermode
		ttk::radiobutton $w.moderb4 -text "mailx  \($::tr(CCDlgmailxEg)\)"         -value "mailx"   -variable ::CorrespondenceChess::mailermode

		ttk::button $w.bdb     -text "..." -command {::CorrespondenceChess::chooseCorrBase }
		ttk::button $w.binbox  -text "..." -command {::CorrespondenceChess::chooseInbox    }
		ttk::button $w.boutbox -text "..." -command {::CorrespondenceChess::chooseOutbox   }
		ttk::button $w.bfetch  -text "..." -command {::CorrespondenceChess::chooseFetch    }
		ttk::button $w.bsend   -text "..." -command {::CorrespondenceChess::chooseSend     }

	    grid $w.lgeneral -column 0 -row 0 -pady "10 0" -sticky we
		grid $w.ldb          -in $w.lgeneral -sticky e    -column 0 -row  1
		grid $w.db           -in $w.lgeneral -sticky we   -column 1 -row  1 -columnspan 2 -padx 5
		grid $w.bdb          -in $w.lgeneral -sticky w    -column 3 -row  1
		grid $w.linbox       -in $w.lgeneral -sticky e    -column 0 -row  2
		grid $w.inbox        -in $w.lgeneral -sticky we   -column 1 -row  2 -columnspan 2 -padx 5
		grid $w.binbox       -in $w.lgeneral -sticky w    -column 3 -row  2
		grid $w.loutbox      -in $w.lgeneral -sticky e    -column 0 -row  3
		grid $w.outbox       -in $w.lgeneral -sticky we   -column 1 -row  3 -columnspan 2 -padx 5
		grid $w.boutbox      -in $w.lgeneral -sticky w    -column 3 -row  3

            grid $w.lxfccrc -column 0 -row 1 -pady 10 -sticky we
		grid $w.internalXfcc -in  $w.lxfccrc -sticky w    -column 0 -row 0 -columnspan 2
		grid $w.xfconf       -in  $w.lxfccrc -sticky w    -column 2 -row 0
		grid $w.confirmXfcc  -in  $w.lxfccrc -sticky w    -column 0 -row  1
		grid $w.onlyOwnMove  -in  $w.lxfccrc -sticky w    -column 1 -row  1 -padx 5
		grid $w.lxfcc        -in  $w.lxfccrc              -column 0 -row  2
		grid $w.xfccrc       -in  $w.lxfccrc -sticky we   -column 1 -row  2 -columnspan 2 -padx 5

		grid $w.lfetch       -in  $w.lxfccrc -sticky e    -column 0 -row  3
		grid $w.fetch        -in  $w.lxfccrc -sticky we   -column 1 -row  3 -columnspan 2 -padx 5
		grid $w.bfetch       -in  $w.lxfccrc -sticky w    -column 3 -row  3
		grid $w.lsend        -in  $w.lxfccrc -sticky e    -column 0 -row  4
		grid $w.send         -in  $w.lxfccrc -sticky we   -column 1 -row  4 -columnspan 2 -padx 5
		grid $w.bsend        -in  $w.lxfccrc -sticky w    -column 3 -row  4

		grid $w.lsortopt     -in  $w.lxfccrc -sticky e    -column 0 -row  5
		grid $w.sortopt      -in  $w.lxfccrc -sticky we   -column 1 -row  5 -columnspan 2 -padx "5 0"
		grid $w.ysc          -in  $w.lxfccrc -sticky wns  -column 3 -row 5

	     grid $w.lemail -column 0 -row 2 -sticky we
		grid $w.lmailx       -in $w.lemail -sticky e    -column 0 -row 0
		grid $w.mailx        -in $w.lemail -sticky we   -column 1 -row 0 -padx 5
		grid $w.lbccaddr     -in $w.lemail -sticky e    -column 0 -row 1
		grid $w.bccaddr      -in $w.lemail -sticky we   -column 1 -row 1 -padx 5

		grid $w.lmoderb      -in $w.lemail -sticky e    -column 0 -row 2
		grid $w.moderb1      -in $w.lemail -sticky w    -column 1 -row 2 -padx 5
		grid $w.moderb2      -in $w.lemail -sticky w    -column 1 -row 3 -padx 5
		grid $w.moderb3      -in $w.lemail -sticky w    -column 1 -row 4 -padx 5
		grid $w.moderb4      -in $w.lemail -sticky w    -column 1 -row 5 -padx 5

		grid $w.lattache     -in $w.lemail -sticky e    -column 0 -row 6
		grid $w.attache      -in $w.lemail -sticky we   -column 1 -row 6 -padx 5

		grid $w.lsubject     -in $w.lemail -sticky e    -column 0 -row 7
		grid $w.subject      -in $w.lemail -sticky we   -column 1 -row 7 -padx 5

	        grid columnconfigure $w.lgeneral 1 -weight 1
	        grid columnconfigure $w.lxfccrc 1 -weight 1
	        grid columnconfigure $w.lemail 1 -weight 1
	        grid columnconfigure $w 0 -weight 1

		# Buttons and ESC-key
		packdlgbuttons $w.bCancel $w.bOk -in $w.buttons
		grid $w.buttons -column 0 -row 3 -sticky news
		bind $w <Escape> "$w.bCancel invoke"

		bind $w <F1> { helpWindow CCSetupDialog}
	}

	#----------------------------------------------------------------------
	# startEmailGame: create an empty new game and set the header for
	# to a cmail compatible format with the parameters entered by the
	# user (own and opponent names and mail addresses and unique id)
	#----------------------------------------------------------------------
	proc startEmailGame {ownname ownmail oppname oppmail gameid} {
		global ::CorrespondenceChess::Inbox

		# the following header tags have to be in this form for cmail to
		# recognise the mail as an eMail correspondence game.
		# Additionally scid searched for some of them to retrieve mail
		# addresses automagically and also to recognise this game as
		# eMail and not Xfcc.
		set Event         "Email correspondence game"
		set Site          "NET"
		set Round         "-"
		set CmailGameName "CmailGameName \"$gameid\""
		set WhiteNA       "WhiteNA \"$ownmail\""
		set BlackNA       "BlackNA \"$oppmail\""
		set whiteCountry  "WhiteCountry \"EUR\""
		set blackCountry  "BlackCountry \"EUR\""

		set Mode          "Mode \"EM\""

		set year          [::utils::date::today year]
		set month         [::utils::date::today month]
		set day           [::utils::date::today day]
		set today         "$year.$month.$day"

		# add a new game
		::game::Clear

		# set the header tags
		sc_game tags set -event     $Event
		sc_game tags set -site      $Site
		sc_game tags set -round     $Round
		sc_game tags set -white     $ownname
		sc_game tags set -black     $oppname
		sc_game tags set -date      $today
		sc_game tags set -eventdate $today

		# add cmails extra header tags
		sc_game tags set -extra [list $CmailGameName $WhiteNA $BlackNA $whiteCountry $blackCountry $Mode]

		updateBoard -pgn
		updateTitle

		# Call gameSave with argument 0 to append to the current
		# database. This also gives the Save-dialog for additional user
		# values.
		gameSave 0

		# construct a PGN in Inbox for CC gamelist to work
		set pgnfile "[file join $Inbox $gameid].pgn"
		sc_base export "current" "PGN" $pgnfile -append 0 -comments 0 -variations 0 \
					-space 1 -symbols 0 -indentC 0 -indentV 0 -column 0 -noMarkCodes 0 -convertNullMoves 1

		::CorrespondenceChess::ReadInbox
	}

	#----------------------------------------------------------------------
	# Generate a new email correspondence game in the style of cmail,
	# but with a friendly dialog presented to the user instead of
	# somewhat cryptic command line parameters.
	# This procedure adds a new game to the Correspondence DB and fills
	# in the header appropriately.
	#----------------------------------------------------------------------
	proc newEMailGame {} {
		global ::CorrespondenceChess::CorrSlot

		# Regardless how the user opened this DB, find it! ;)
		::CorrespondenceChess::CheckForCorrDB
		# Only proceed if a correspondence DB is already loaded
		if {$CorrSlot > -1} {
			set w .wnewEMailGame
			if { [winfo exists $w]} { return }
			toplevel $w
			wm title $w [::tr "CCDlgStartEmail"]

			set ownemail   ::CorrespondenceChess::bccaddr
			set ownname    ""
			set oppemail   ""
			set oppname    ""
			set gameid     ""

			ttk::label  $w.lownname -text [::tr CCDlgYourName]
			ttk::label  $w.lownmail -text [::tr CCDlgYourMail]
			ttk::label  $w.loppname -text [::tr CCDlgOpponentName]
			ttk::label  $w.loppmail -text [::tr CCDlgOpponentMail]
			ttk::label  $w.lgameid  -text [::tr CCDlgGameID]

			ttk::entry  $w.ownname -width 40 -textvariable ownname
			ttk::entry  $w.ownmail -width 40 -textvariable $ownemail
			ttk::entry  $w.oppname -width 40 -textvariable oppname
			ttk::entry  $w.oppmail -width 40 -textvariable oppemail
			ttk::entry  $w.gameid  -width 40 -textvariable gameid

			ttk::button $w.bOk     -text OK -command {
				::CorrespondenceChess::startEmailGame \
						[.wnewEMailGame.ownname get] \
						[.wnewEMailGame.ownmail get] \
						[.wnewEMailGame.oppname get] \
						[.wnewEMailGame.oppmail get] \
						[.wnewEMailGame.gameid  get]
				destroy .wnewEMailGame
			}
			ttk::button $w.bCancel -text [::tr "Cancel"] -command "destroy $w"

			grid $w.lownname   -sticky e -column 0 -row 0
			grid $w.lownmail   -sticky e -column 0 -row 1
			grid $w.loppname   -sticky e -column 0 -row 2
			grid $w.loppmail   -sticky e -column 0 -row 3
			grid $w.lgameid    -sticky e -column 0 -row 4

			grid $w.ownname    -sticky w -column 1 -row 0 -columnspan 2
			grid $w.ownmail    -sticky w -column 1 -row 1 -columnspan 2
			grid $w.oppname    -sticky w -column 1 -row 2 -columnspan 2
			grid $w.oppmail    -sticky w -column 1 -row 3 -columnspan 2
			grid $w.gameid     -sticky w -column 1 -row 4 -columnspan 2

			# Buttons and ESC-key
			grid $w.bOk       -column 1 -row  5 -pady 10
			grid $w.bCancel   -column 2 -row  5 -pady 10
			bind $w <Escape> "$w.bCancel invoke"
			bind $w <F1> { helpWindow CCeMailChess}
		}
	}

	#----------------------------------------------------------------------
	# Call an external program via a proper shell
	# open and exec call the external without a shell environment
	# For Windows make sure that the executable uses its short name
	#     catch {set mailer [file attributes $mailer -shortname]}
	# or it resides in a path without spaces
	# For Windows quoting is not possible as usual, < and > are not allowed
	# as textual arguments even if quoted properly.
	#----------------------------------------------------------------------
	proc CallExternal {callstring {param ""}} {
		global windowsOS

		if {$windowsOS} {
			# On Windows, use the "start" command:
			if {[string match $::tcl_platform(os) "Windows NT"]} {
				catch {exec $::env(COMSPEC) /c "$callstring $param" &}
			} else {
				catch {exec start "$callstring $param" &}
			}
		} else {
			# On Unix just call the shell with the converter tool
			catch {exec /bin/sh -c "$callstring $param" &}
		}
	}

	#----------------------------------------------------------------------
	# Check whether a Correspondence Database is loaded. Note that the
	# first one found is used as reference DB for game processing.
	#----------------------------------------------------------------------
	proc CheckForCorrDB {} {
		global ::windows::switcher::base_types
		global ::CorrespondenceChess::Inbox ::CorrespondenceChess::Outbox
		global ::CorrespondenceChess::CorrSlot

		set CorrSlot -1
		if {$CorrSlot < 0} {
			# check for the status window to exist, if not open it
			if {![winfo exists .ccWindow]} {
				::CorrespondenceChess::CCWindow
			}

			# check for In/Outbox to exist and be accessible
			if { [file exists $Inbox] == 0  && ([file isdirectory $Inbox] == 0) } {
				set Title [::tr CCDlgTitNoInbox]
				set Error [::tr CCErrInboxDir]
				append Error "\n   $Inbox\n"
				append Error [::tr CCErrDirNotUsable]
				tk_messageBox -icon warning -type ok -parent . \
					-title $Title -message $Error
				return
			}
			if { ([file exists $Outbox] == 0) && ([file isdirectory $Outbox] == 0) } {
				set Title [::tr CCDlgTitNoOutbox]
				set Error [::tr CCErrOutboxDir]
				append Error "\n   $Outbox\n"
				append Error [::tr CCErrDirNotUsable]
				tk_messageBox -icon warning -type ok -parent . \
					-title $Title -message $Error
				return
			}

			set typeCorr [lsearch $base_types {Correspondence chess} ]
			foreach x [sc_base list] {
					set type [getBaseType $x]
					if {$type == $typeCorr} {
						.ccWindow.top.openDB configure -state disabled
						set CorrSlot $x
						break
					}
			}
			if {$CorrSlot < 0} {
				set Title [::tr CCDlgTitNoCCDB]
				set Error [::tr CCErrNoCCDB]
				tk_messageBox -icon warning -type ok -parent . \
					-title $Title -message $Error
			}
		}
	}

	#----------------------------------------------------------------------
	# Opens the DB holding the correspondence games
	#----------------------------------------------------------------------
	proc OpenCorrespondenceDB {} {
		global ::CorrespondenceChess::CorrBase

		## set fName [file rootname $CorrBase]
		set fName $CorrBase

		if {[catch {::file::Open_ $fName} result]} {
			set err 1
			tk_messageBox -icon warning -type ok -parent . \
				-title "Scid: Error opening file" -message $result
		} else {
			if {[file extension $fName] == ".si3"} {
				# file has been converted to si4
				set CorrBase "[file rootname $CorrBase].si4"
				::CorrespondenceChess::saveCCoptions
			}
			set ::initialDir(base) [file dirname $fName]
		}
		updateBoard -pgn
		::notify::DatabaseChanged

		::CorrespondenceChess::CheckForCorrDB
	}

	#----------------------------------------------------------------------
	# Search for a game by Event, Site, White, Black and CmailGameName
	# This has to result in only one game matching the criteria.
	# No problem with cmail and Xfcc as GameIDs are unique.
	#----------------------------------------------------------------------
	proc SearchGame {Event Site White Black CmailGameName result refresh} {
		global ::CorrespondenceChess::CorrSlot

		# switch to the Correspondence Games DB
		sc_base switch $CorrSlot
		set move ""

		set sPgnlist {}
		lappend sPgnlist [string trim $CmailGameName]

		# Search the header for the game retrieved. Use as much info as
		# possible to get a unique result. In principle $sPgnList should
		# be enough. However searching indexed fields speeds up things
		# a lot in case of large DBs. Also: disregard deleted games,
		# this avoids the necessity to compact a db in case of
		# accidential duplication of a game.
		# -filter 2: Ignore previous searches
		set str [sc_search header \
					-event $Event    \
					-site $Site      \
					-white $White    \
					-black $Black    \
					-pgn $sPgnlist   \
					-flag! D \
					-filter 2        \
					-gnum [list 1 -1] \
					]

		CorrespondenceChess::updateConsole "info: search [sc_filter count]"

		# There should be only one result. If so, load it and place the
		# game pointer to the end of the game ::game::Load also handles
		# board rotation if Player Names are set up correctly.
		if {[sc_filter count] == 1} {
			set filternum [sc_filter first]

			# Refresh windows only if necessary
			if {$refresh == 1} {
				# ::game::Load  also checks the dirty flag and asks to
				# save the game in case necessary.
				::game::Load $filternum
			} else {
				sc_game load $filternum
			}

			set Mode [::CorrespondenceChess::CheckMode]

			sc_move end
			# Number of moves in the current DB game
			set mnCorr [expr {[sc_pos moveNumber]-1}]
			set side   [sc_pos side]

			# Number of moves in the new game in Clipbase
			sc_base switch $::clipbase_db
			sc_move end
			set mnClip [sc_pos moveNumber]

			if {$side == "white"} {
				set plyStart [expr {$mnCorr*2-1}]
			} else {
				set plyStart [expr {$mnCorr*2}]
			}

			set side   [sc_pos side]
			if {$side == "white"} {
				set plyEnd [expr {$mnClip*2-1}]
			} else {
				set plyEnd [expr {$mnClip*2}]
			}

			# Check if the games mainline in DB contains more ply than
			# the game in the clipbase. If so inform the user.
			if {($plyEnd-$plyStart < 2) && ($Mode == "XFCC") && ($result == "*")} {
				set Title [::tr CCDlgDBGameToLong]
				set Error [::tr CCDlgDBGameToLongError]
				tk_messageBox -icon warning -type ok -parent . \
					-title $Title -message "$Error $mnClip (= ply $plyEnd)"
			}

			# Add moves from the relayed games if the mode is not Postal.
			# On mixed ICCF Events also the ICCF server deliveres an
			# empty game via Xfcc, therefore this check is required
			if {$Mode != "Postal"} {

				# Add moves from clipbase to the DB game. This keeps
				# comments, but requires that tries are inserted as variants
				# as it is always appended to the end of the game
				for {set x $plyStart} {$x < $plyEnd} {incr x} {
					set basecomment  ""
					set comment      ""

					sc_base switch $::clipbase_db

					# move to the beginning of the new part
					sc_move start
					sc_move forward [expr {$x+1}]

					# Get the move in _untranslated_ form...
					set move [sc_game info nextMoveNT]
					# ... move on one ply ...
					sc_move forward
					# ... and get the comment
					set comment [sc_pos getComment]

					# switch to Correspondence DB and add the move and comment
					sc_base switch     $CorrSlot
					sc_move addSan     $move

					# Get the comment stored in the base for comparison
					set basecomment [sc_pos getComment]

					# Some servers keep old comments within the game
					# (SchemingMind) some don't (ICCF). Try to preserve
					# comments inserted by the user as well as add new
					# responses properly.
					set sbasecomment ""
					set scomment     ""

					# Strip of [%ccsnt...] like comments (SchemingMind time stamps)
					regsub -all {\[.*\]} $basecomment   "" sbasecomment
					regsub -all {^\s*}   $sbasecomment  "" sbasecomment
					# Strip of "Name: " to compare original text entered by
					# the user only.
					regsub -all "$White:" $sbasecomment "" sbasecomment
					regsub -all "$Black:" $sbasecomment "" sbasecomment

					# Same for the game delivered by Xfcc
					regsub -all {\[.*\]}  $comment      "" scomment
					regsub -all {^\s*}    $scomment     "" scomment
					regsub -all "$White:" $scomment     "" scomment
					regsub -all "$Black:" $scomment     "" scomment

					# Check what to preserve and which comment to set.
					if { [string length $sbasecomment] == 0} {
						sc_pos setComment "$comment"
					} elseif { [string length $scomment] < [string length $sbasecomment ]} {
						# base contains more text than the one retrieved
						if { [string first $scomment $sbasecomment] < 0 } {
							sc_pos  setComment "$basecomment $comment"
						}
					} else {
						# retrieved game contains more text than the stored
						if { [string first $sbasecomment $scomment] < 0 } {
							sc_pos setComment "$basecomment $comment"
						} else {
							sc_pos setComment "$comment"
						}
					}
				}
				sc_game tags set -result $result
				sc_base switch $CorrSlot
				sc_game save $filternum

				# Only refresh when SearchGame was triggered by the user,
				# otherwise just reload the game but leave the window in
				# state to save considerable amount of time
				if {$refresh == 1} {
					::game::Load $filternum
				} else {
					sc_game load $filternum
				}
			} else {
				# only switch to base for postal games
				sc_base switch $CorrSlot
			}
		} elseif {[sc_filter count] == 0} {
			# No matching game found, add it as a new one
			# Clear the current game first, then just paste the clipboard
			# game as it is. No need to do something as complex as for
			# already existing games above.
			game::Clear
			sc_clipbase paste
			# append the current game without asking and the header
			# supplied
			# gameAdd gets confused here with with an altered game opeing
			# another dialogue besides the save game
			sc_game save 0

			CorrespondenceChess::updateConsole "info: new game added"
		} else {
			if {[winfo exists .glistWin]} {
				raise .glistWin
			} else {
				::windows::gamelist::Open
			}
			set Title [::tr CCDlgDuplicateGame]
			set Error [::tr CCDlgDuplicateGameError]
			tk_messageBox -icon warning -type ok -parent . \
				-title $Title -message $Error
		}
		::notify::DatabaseChanged
	}

	#----------------------------------------------------------------------
	# If a Correspondence DB is loaded, switch to the clipbase and
	# use the game with the given id to find headers.
	# PGN file and jump to the game number given. Then extract the
	# header tags and call "SearchGame" to display the game in question
	# to the user.
	#----------------------------------------------------------------------
	proc ProcessServerResult {game} {
		global ::CorrespondenceChess::CorrSlot
		global emailData

		# Regardless how the user opened this DB, find it! ;)
		::CorrespondenceChess::CheckForCorrDB
		# Only proceed if a correspondence DB is already loaded
		if {$CorrSlot > -1} {
			sc_base switch $::clipbase_db
			sc_game load $game

			# get the header
			set Event  [sc_game tags get Event]
			set Site   [sc_game tags get Site ]
			set White  [sc_game tags get White]
			set Black  [sc_game tags get Black]
			set Extra  [sc_game tags get Extra]
			set result [sc_game tags get Result]
			# CmailGameName is an extra header :(
			set extraTagsList [split $Extra "\n"]

			# ... extract it as it contains the unique ID
			foreach i $extraTagsList {
				if { [string equal -nocase [lindex $i 0] "CmailGameName" ] } {
					set CmailGameName [string range $i 15 end-1]
				}
			}

			# set these variables for email games where they get no
			# values otherwise
			set noENG     "false"
			set drawoffer "false"
			# Search the game in the correspondence DB and display it
			foreach xfccextra $::Xfcc::xfccstate {
				if { [string equal -nocase [lindex $xfccextra 0] "$CmailGameName" ] } {
					foreach i $xfccextra {
						if { [string equal -nocase [lindex $i 0] "noEngines" ] } {
							set noENG [string range $i 10 end]
						}
						if { [string equal -nocase [lindex $i 0] "drawOffered" ] } {
							set drawoffer [string range $i 12 end]
						}
					}
				}
			}
			if {$noENG == "true"} {
				::CorrespondenceChess::EnableEngineAnalysis 0
			} else {
				::CorrespondenceChess::EnableEngineAnalysis 1
			}

			# After this search the windows need to be refreshed to show
			# the current state
			SearchGame $Event $Site $White $Black $CmailGameName $result 1

			set Mode [::CorrespondenceChess::CheckMode]

			# hook up with the old email manager: this implements the
			# manual timestamping required
			if {($Mode == "EM") || ($Mode == "Relay") || ($Mode == "Postal")} {
				set emailData [::tools::email::readOpponentFile]
				set done 0
				set idx  0
				# search through the whole list and check if the current
				# game was already defined to be an email game.
				foreach dataset $emailData {
					if { [lindex $dataset 0] == $CmailGameName} {
						set done 1
						# add the received flag and date
						::tools::email::addSentReceived $idx r
					}
					incr idx
				}
				if {$done < 1} {
					set idx [llength $emailData]
					lappend emailData [list "" "" "" "" ""]
					set emailData [lreplace $emailData $idx $idx \
									  [list "$CmailGameName" "somewhere@somehost.org" "scid game" [sc_filter first] "-- " ]]
					::tools::email::writeOpponentFile $emailData
					::tools::email::refresh
					# add the received flag and date
					::tools::email::addSentReceived $idx r
				}
			}
			# Jump to the end of the game and update the display
			::move::End
			if {$drawoffer == "true"} {
				.ccWindow.top.acceptDraw configure -state normal
				set comment   [sc_pos getComment]
				set drwstr    "- [::tr Draw] -"
				if { [regexp "$drwstr" $comment] } {
				} else {
					sc_pos setComment "$comment $drwstr"
					updateBoard -pgn
				}
			} else {
				.ccWindow.top.acceptDraw configure -state disabled
			}


			# Set some basic info also to the button tooltips
			::utils::tooltip::Set .ccWindow.top.resign     "$CmailGameName: $Event\n$Site\n\n$White - $Black"
			::utils::tooltip::Set .ccWindow.top.claimDraw  "$CmailGameName: $Event\n$Site\n\n$White - $Black"
			::utils::tooltip::Set .ccWindow.top.acceptDraw "$CmailGameName: $Event\n$Site\n\n$White - $Black"
			::utils::tooltip::Set .ccWindow.top.offerDraw  "$CmailGameName: $Event\n$Site\n\n$White - $Black"
		}
	}

	#----------------------------------------------------------------------
	# Checks the mode of the current game and return it (EM or XFCC).
	# Additionally update the button status in the Console window
	# accordingly and update the console itself with $Event, Mode and
	# $Site.
	#----------------------------------------------------------------------
	proc CheckMode {} {
		set Event [sc_game tags get Event]
		set Site  [sc_game tags get Site]
		set Extra [sc_game tags get Extra]
		# CmailGameName is an extra header :(
		set extraTagsList [split $Extra "\n"]

		# ... extract it as it contains the unique ID
		foreach i $extraTagsList {
			if { [string equal -nocase [lindex $i 0] "Mode" ] } {
				set Mode [string range $i 6 end-1]
			}
		}

		set m .menu.play.correspondence

		# do not set state of top.acceptDraw as this is set dynamically
		if {($Mode == "EM") || ($Mode == "Relay") || ($Mode == "Postal")} {
			if {$Mode == "EM"} {
				::CorrespondenceChess::updateConsole "info Event: $Event (eMail-based)"
			} elseif {$Mode == "Relay"} {
				::CorrespondenceChess::updateConsole "info Event: $Event (observed)"
			} elseif {$Mode == "Postal"} {
				::CorrespondenceChess::updateConsole "info Event: $Event (postal)"
			}

			# eMail/postal games: manual handling for resign and draw is
			# needed, no standard way/protocol exists => disable the
			# buttons and menu entries accordingly
			.ccWindow.top.resign     configure -state disabled
			.ccWindow.top.claimDraw  configure -state disabled
			.ccWindow.top.offerDraw  configure -state disabled
			# .ccWindow.top.acceptDraw configure -state disabled

			$m entryconfigure 8 -state disabled
			$m entryconfigure 9 -state disabled
			$m entryconfigure 10 -state disabled
			$m entryconfigure 11 -state disabled
		} else {
			.ccWindow.top.resign     configure -state normal
			.ccWindow.top.claimDraw  configure -state normal
			.ccWindow.top.offerDraw  configure -state normal
			# .ccWindow.top.acceptDraw configure -state normal
			::CorrespondenceChess::updateConsole "info Event: $Event (Xfcc-based)"

			$m entryconfigure 8 -state normal
			$m entryconfigure 9 -state normal
			$m entryconfigure 10 -state normal
			$m entryconfigure 11 -state normal
		}
		::CorrespondenceChess::updateConsole "info Site: $Site"

		return $Mode
	}

	#----------------------------------------------------------------------
	# Search the previous game from the input file in the correspondence DB
	# If at last game already nothing happens
	#----------------------------------------------------------------------
	proc PrevGame {} {
		global ::CorrespondenceChess::CorrSlot ::CorrespondenceChess::num
		set gamecount $::CorrespondenceChess::glgames

		busyCursor .
		# Regardless how the user opened this DB, find it! ;)
		::CorrespondenceChess::CheckForCorrDB
		if {$CorrSlot > -1} {
			if {$num > 1} {
				set num [expr {$num - 1}]
				SetHighlightedLine
				::CorrespondenceChess::ProcessServerResult $::CorrespondenceChess::num
			}
		}
		unbusyCursor .
	}

	#----------------------------------------------------------------------
	# Search the next game from the input file in the correspondence DB
	# If at last game already nothing happens
	#----------------------------------------------------------------------
	proc NextGame {} {
		global ::CorrespondenceChess::CorrSlot ::CorrespondenceChess::num
		set gamecount $::CorrespondenceChess::glgames

		busyCursor .
		# Regardless how the user opened this DB, find it! ;)
		::CorrespondenceChess::CheckForCorrDB
		if {$CorrSlot > -1} {
			if {$num < $gamecount} {
				set num [expr {$num + 1}]
				SetHighlightedLine
				::CorrespondenceChess::ProcessServerResult $::CorrespondenceChess::num
			}
		}
		unbusyCursor .
	}

	#----------------------------------------------------------------------
	# FetchGames: retrieve games via Xfcc
	#----------------------------------------------------------------------
	proc FetchGames {} {
		global ::CorrespondenceChess::Inbox ::CorrespondenceChess::XfccFetchcmd ::CorrespondenceChess::CorrSlot
		busyCursor .

		# Regardless how the user opened this DB, find it! ;)
		::CorrespondenceChess::CheckForCorrDB
		# Only proceed if a correspondence DB is already loaded
		if {$CorrSlot > -1} {
			if {$::CorrespondenceChess::XfccInternal == 1} {
				# use internal Xfcc-handling
				::Xfcc::ReadConfig $::CorrespondenceChess::xfccrcfile
				::Xfcc::ProcessAll $::CorrespondenceChess::Inbox
			} else {
				# call external protocol handler
				if {[file executable "$XfccFetchcmd"]} {
					::CorrespondenceChess::updateConsole "info Calling external fetch tool $XfccFetchcmd..."
					CallExternal "$XfccFetchcmd $Inbox"
				}
			}
			# Fetch games that should be relayed from the ICCF Server
			::CorrespondenceChess::updateConsole "info Fetching relayed games from ICCF..."
			foreach g $::CorrespondenceChess::RelayGames {
				::CorrespondenceChess::RelayGames $g
			}
			# process what was just retrieved
			::CorrespondenceChess::ReadInbox
		}
		unbusyCursor .
	}

	#----------------------------------------------------------------------
	# EmptyInOutbox: remove all pgn files currently stored in in- and
	# outbox directories to get a fresh fetch
	#----------------------------------------------------------------------
	proc EmptyInOutbox {} {
		global ::CorrespondenceChess::Inbox ::CorrespondenceChess::Outbox
		global windowsOS

		if {$windowsOS} {
			set inpath  "$Inbox\\"
			set outpath "$Outbox\\"
		} else {
			set inpath  "$Inbox/"
			set outpath "$Outbox/"
		}
		set result [tk_dialog .roDialog "Scid: [tr CCDlgDeleteBoxes]" \
				$::tr(CCDlgDeleteBoxesText) "" 1 $::tr(Yes) $::tr(No)]
		if {$result == 0} {
			foreach f [glob -nocomplain [file join $inpath *]] {
				file delete $f
			}
			foreach f [glob -nocomplain [file join $outpath *]] {
				file delete $f
			}
			set filename [scidConfigFile xfccstate]
			file delete $filename

			# clean also status information as they're now invalid!
			set ::Xfcc::xfccstate {}

			::CorrespondenceChess::emptyGamelist
		}

	}

	#----------------------------------------------------------------------
	# ReadInbox: process the inbox game per game by adding them one by
	# one to the clipboard.
	#----------------------------------------------------------------------
	proc ReadInbox {} {
		global ::CorrespondenceChess::Inbox ::CorrespondenceChess::CorrSlot
		global ::CorrespondenceChess::glccstart ::CorrespondenceChess::glgames windowsOS
		global ::Xfcc::lastupdate ::Xfcc::xfccstate

		set pgnopen 0

		busyCursor .

		if {$windowsOS} {
			set inpath  "$Inbox\\"
		} else {
			set inpath  "$Inbox/"
		}

		# Regardless how the user opened this DB, find it! ;)
		::CorrespondenceChess::CheckForCorrDB

		set games 0
		if {$CorrSlot > -1} {

			# extract the number of the last move using Scid's internal
			# PGN parser as comments etc. might appear, and this number
			# is not given via Xfcc. Similar for the event date.
			sc_clipbase clear
			sc_base switch $::clipbase_db"
			set game 0
			set gamemoves {}
			foreach f [glob -nocomplain [file join $inpath *]] {
				catch {sc_base import [sc_base current] $f}
				set game [expr {$game + 1}]
				sc_game load $game
				sc_move end
				set number [sc_pos moveNumber]
				set Date   [sc_game tags get Date ]
				set Extra  [sc_game tags get Extra]
				set extraTagsList [split $Extra "\n"]
				foreach i $extraTagsList {
					if { [string equal -nocase [lindex $i 0] "CmailGameName" ] } {
						set CmailGameName [string range $i 15 end-1]
					}
				}
				lappend gamemoves [list $CmailGameName $number $Date]
			}

			# generate a list of games retrieved by Xfcc. Add game-ID and
			# timing to two lists: one holds all games and one holds
			# those the user does not have the move (they may be skipped
			# in display)
			set xfcclist {}
			set filelist {}
			set skiplist {}
			set sortmode "-ascii"

			foreach xfccextra $::Xfcc::xfccstate {
				set CmailGameName [lindex $xfccextra 0]
				set criterion     0
				set timepermove   0
				set mytime        0
				set opptime       0
				set movestoTC     1
				set tincrement    0
				set moves         0
				set myTurn        "false"
				set TimeControl   "10/50"
				set idx           [lsearch -exact -index 0 $gamemoves $CmailGameName]
				set number        [lindex [lindex $gamemoves $idx] 1]
				set Date          [lindex [lindex $gamemoves $idx] 2]
				regsub -all {\.} $Date "" Date

				foreach i $xfccextra {
					if { [string equal -nocase [lindex $i 0] "myTurn" ] } {
						set myTurn [string range $i 7 end]
					}
					if { [string equal -nocase [lindex $i 0] "mytime" ] } {
						set mytime [string range $i 7 end]
					}
					if { [string equal -nocase [lindex $i 0] "opptime" ] } {
						set opptime [string range $i 8 end]
					}
					if { [string equal -nocase [lindex $i 0] "TimeControl" ] } {
						set TCstr [string range $i 13 end]
						# Calculate the moves to the next time control.
						# Makes sense only if no Fischer Clock is used.
						if { [regexp {/} $TCstr ]} {
							set TC [split $TCstr "/"]
							set moves      [ lindex $TC 0]
							set tincrement [ lindex $TC 1]
							regsub -all "d.*" $tincrement "" tincrement
							set movestoTC [ expr {$moves - ($number % $moves)}]
						} else {
							# Fischer Clock
							set moves 1
						}
					}
				}
				set mytime  [expr {int($mytime / 60.0 / 24.0)}]

				# Calculate the time per move till next TC: include also
				# the next time control periode in this calculation
				set timepermove1 [expr {($mytime+$tincrement) / ($movestoTC+$moves)}]
				set timepermove2 [expr {$mytime / $movestoTC}]

				# Time per move is the minimum of the two above
				set timepermove [expr min($timepermove1, $timepermove2)]

				# Define criteria to be added to the list to sort. Classic
				# mode is handled below by resorting the clipbase
				switch -regexp -- $::CorrespondenceChess::ListOrder \
					"$::CorrespondenceChess::CCOrderMyTime" {
						set criterion $mytime
						set sortmode "-integer"
					} \
					"$::CorrespondenceChess::CCOrderOppTime" {
						set criterion $opptime
						set sortmode "-integer"
					} \
					"$::CorrespondenceChess::CCOrderTimePerMove" {
						set criterion $timepermove
						set sortmode "-real"
					} \
					"$::CorrespondenceChess::CCOrderStartDate" {
						set criterion $Date
						set sortmode "-integer"
					}

				if {($myTurn == "false") && ($::CorrespondenceChess::ListOnlyOwnMove == 1) } {
					lappend skiplist [list $CmailGameName $criterion]
				} else {
					lappend filelist [list $CmailGameName $criterion]
				}
					lappend xfcclist [list $CmailGameName]
			}

			# sort file list by mytime, ascending
			set filelist [lsort -index 1 "$sortmode" $filelist]
			set skiplist [lsort -index 1 "$sortmode" $skiplist]

			::CorrespondenceChess::emptyGamelist
			sc_clipbase clear
			sc_base switch $::clipbase_db

			# Loop over all files and add all game files that are not
			# Xfcc (ie. eMail chess)
			foreach f [glob -nocomplain [file join $inpath *]] {
				set id [file tail $f]
				regsub -all ".pgn" $id "" id
				if { [lsearch $xfcclist "$id"] < 0 } {
					set filelist [concat $id $filelist]
				}
			}

			# import the games on basis of the sorted list created above
			foreach f $filelist {
				set filename "[file join $inpath [lindex $f 0]].pgn"
				if {[catch {sc_base import [sc_base current] $filename} result]} {
					::CorrespondenceChess::updateConsole "info Error retrieving server response from $filename"
				} else {
					# count the games processed successfully
					set games [expr {$games+1}]
				}
			}

			set glgames $games

			# For Classic sorting: sort the clipbase, this is easier
			# to implement than individual sorting upon import.
			if {$::CorrespondenceChess::ListOrder == $::CorrespondenceChess::CCOrderClassic} {
				tk_messageBox -message "oops, 300+ lines functions are not maintainable"
			}


			if {$glgames > 0} {
				# work through all games processed and fill in the gamelist
				# in the console window

				for {set game $glccstart} {$game < [expr {$games+1}]} {incr game} {

					set clockW "no update"; set clockB "no update";
					set var "";             set noDB "";
					set noBK "";            set noTB "";
					set noENG "";           set mess ""
					set TC "";              set drawoffer "false";
					set wc "";              set bc "";
					set YM " ? ";

					sc_base switch $::clipbase_db
					sc_game load $game
					# get the header
					set Event  [sc_game tags get Event]
					set Site   [sc_game tags get Site ]
					set Date   [sc_game tags get Date ]
					set White  [sc_game tags get White]
					set Black  [sc_game tags get Black]
					set Result [sc_game tags get Result]
					set Extra  [sc_game tags get Extra]
					# CmailGameName is an extra header :(
					set extraTagsList [split $Extra "\n"]
					foreach i $extraTagsList {
						if { [string equal -nocase [lindex $i 0] "CmailGameName" ] } {
							set CmailGameName [string range $i 15 end-1]
						}
					}
					#
					# Switch to the real database to retrieve locally
					# stored additions like addresses, countries etc.
					# Disable refresh for SearchGame to speed up the list
					# building considerably
					SearchGame $Event $Site $White $Black $CmailGameName $result 0
					sc_base switch $CorrSlot
					set Extra  [sc_game tags get Extra]
					set extraTagsList [split $Extra "\n"]

					# ... extract it as it contains the unique ID
					foreach i $extraTagsList {
						if { [string equal -nocase [lindex $i 0] "CmailGameName" ] } {
							set CmailGameName [string range $i 15 end-1]
						}
						if { [string equal -nocase [lindex $i 0] "Mode" ] } {
							set Mode [string range $i 6 end-1]
						}
						if { [string equal -nocase [lindex $i 0] "whiteCountry" ] } {
							set wc [string range $i 14 end-1]
							set wc [string tolower $wc]
						}
						if { [string equal -nocase [lindex $i 0] "blackCountry" ] } {
							set bc [string range $i 14 end-1]
							set bc [string tolower $bc]
						}
						if { [string equal -nocase [lindex $i 0] "WhiteCountry" ] } {
							set wc [string range $i 14 end-1]
							set wc [string tolower $wc]
						}
						if { [string equal -nocase [lindex $i 0] "BlackCountry" ] } {
							set bc [string range $i 14 end-1]
							set bc [string tolower $bc]
						}
						if { [string equal -nocase [lindex $i 0] "TimeControl" ] } {
							set TC [string range $i 13 end-1]
						}
					}
					sc_move end
					set number [sc_pos moveNumber]
					set move   [sc_game info previousMoveNT]
					set side   [sc_pos side]

					if {$side == "white"} {
						set number [expr {$number-1}]
						set lastmove "$number...$move"
					} else {
						set lastmove "$number. $move"
					}
					::CorrespondenceChess::updateConsole "info TC (base): $TC..."

					if {$Mode == "EM"} {
						::CorrespondenceChess::updateGamelist $CmailGameName "EML" \
								$Event $Site $Date $White $Black "" "" "" "" $TC "" "" \
								$wc $bc "" "" $lastmove "false"
					} elseif {$Mode == "Relay"} {
						::CorrespondenceChess::updateGamelist $CmailGameName "REL" \
								$Event $Site $Date $White $Black "" "" "" "" $TC "" "" \
								$wc $bc "" "" $lastmove "false"
					} elseif {$Mode == "Postal"} {
						::CorrespondenceChess::updateGamelist $CmailGameName "POS" \
								$Event $Site $Date $White $Black "" "" "" "" $TC "" "" \
								$wc $bc "" "" $lastmove "false"
					} else {
						# actually check the $xfccstate list for the current
						# values. If it is not set (e.g. only inbox processed
						# buy no current retrieval) set some default values.
							foreach xfccextra $::Xfcc::xfccstate {
							if { [string equal -nocase [lindex $xfccextra 0] "$CmailGameName" ] } {
								foreach i $xfccextra {
									if { [string equal -nocase [lindex $i 0] "myTurn" ] } {
										if {[string range $i 7 end] == "true"} {
											set YM "yes"
										} else {
											set YM "no"
										}
									}
									if { [string equal -nocase [lindex $i 0] "clockW" ] } {
										set clockW [string range $i 7 end]
										regsub -all "\{" $clockW "" clockW
										regsub -all "\}" $clockW "" clockW
									}
									if { [string equal -nocase [lindex $i 0] "clockB" ] } {
										set clockB [string range $i 7 end]
										regsub -all "\{" $clockB "" clockB
										regsub -all "\}" $clockB "" clockB
									}
									if { [string equal -nocase [lindex $i 0] "drawOffered" ] } {
										set drawoffer [string range $i 12 end]
									}
									if { [string equal -nocase [lindex $i 0] "variant" ] } {
										set var [string range $i 8 end]
									}
									if { [string equal -nocase [lindex $i 0] "noDatabases" ] } {
										set noDB [string range $i 12 end]
									}
									if { [string equal -nocase [lindex $i 0] "noOpeningBooks" ] } {
										set noBK [string range $i 15 end]
									}
									if { [string equal -nocase [lindex $i 0] "noTablebases" ] } {
										set noTB [string range $i 13 end]
									}
									if { [string equal -nocase [lindex $i 0] "noEngines" ] } {
										set noENG [string range $i 10 end]
									}
									if { [string equal -nocase [lindex $i 0] "TimeControl" ] } {
										set TC [string range $i 13 end-1]
									}
									if { [string equal -nocase [lindex $i 0] "message" ] } {
										set mess [string range $i 9 end-1]
									}
								}
							}
						}
						if {$Result == "1"} {
							set YM "1-0"
						} elseif {$Result == "0"} {
							set YM "0-1"
						} elseif {$Result == "="} {
							set YM " = "
						}
						::CorrespondenceChess::updateConsole "info TC (xfcc): $TC..."
						::CorrespondenceChess::updateGamelist $CmailGameName $YM \
								$Event $Site $Date $White $Black $clockW $clockB $var \
								$noDB $noBK $noTB $noENG $wc $bc $mess $TC $lastmove $drawoffer
					}
				}
				# ::CorrespondenceChess::num is the game currently shown
				set ::CorrespondenceChess::num 0
				# current game is game 0 -> go to game 1 in the list
				::CorrespondenceChess::NextGame
			} else {
				set Title [::tr CCDlgTitNoGames]
				set Error [::tr CCErrInboxDir]
				append Error "\n   $Inbox\n"
				append Error [::tr CCErrNoGames]
				tk_messageBox -icon warning -type ok -parent . \
					-title $Title -message $Error
			}
		}
		unbusyCursor .
	}

	#----------------------------------------------------------------------
	# Send the move to the opponent via eMail
	# This requires an external MTA that is capable of sending a pgn file
	# as attachment (content-type: application/x-chess-pgn). This can be
	# accomplished by nail with proper /etc/mime.types (default on debian).
	# Additionally the program has to handle SMTP-Auth in all its flavours
	# to be of any use in present days.
	# A stripped version of the game is placed in outbox and sent to the
	# opponent. As nail does not handle empty bodies it is sent as text
	# within the body and once attached for easy extraction with mail
	# programs that do not know a thing about piping.
	# After the mail is sent a full featured version of the pgn is placed
	#----------------------------------------------------------------------
	proc eMailMove { } {
		global ::CorrespondenceChess::Outbox \
			::CorrespondenceChess::mailer  ::CorrespondenceChess::mailermode \
			::CorrespondenceChess::attache ::CorrespondenceChess::subject \
			::CorrespondenceChess::bccaddr ::CorrespondenceChess::CorrSlot
		global emailData
		global windowsOS

		busyCursor .

		# Regardless how the user opened this DB, find it! ;)
		::CorrespondenceChess::CheckForCorrDB

		if {$CorrSlot > -1} {
			# move to end to show the location to send AND to get the right
			# side to move, ie. for the extraction of the correct To: address
			::move::End
			set Event  [sc_game tags get Event]
			set Site   [sc_game tags get Site ]
			set Date   [sc_game tags get Date]
			set Round  [sc_game tags get Round]
			set Result [sc_game tags get Result]
			set White  [sc_game tags get White]
			set Black  [sc_game tags get Black]
			set Extra  [sc_game tags get Extra]
			set Extra  [sc_game tags get Extra]
			set extraTagsList [split $Extra "\n"]

			foreach i $extraTagsList {
				if { [string equal -nocase [lindex $i 0] "CmailGameName" ] } {
					set CmailGameName [string range $i 15 end-1]
				}
				if { [string equal -nocase [lindex $i 0] "WhiteNA" ] } {
					set WhiteNA [string range $i 9 end-1]
				}
				if { [string equal -nocase [lindex $i 0] "BlackNA" ] } {
					set BlackNA [string range $i 9 end-1]
				}
			}

			# construct a PGN in Outbox, stripped bare of comments and variations
			set pgnfile "[file join $Outbox $CmailGameName].pgn"

			sc_base export "current" "PGN" $pgnfile -append 0 -comments 0 -variations 0 \
						-space 1 -symbols 0 -indentC 0 -indentV 0 -column 0 -noMarkCodes 0 -convertNullMoves 1

			# sc_game pgn -gameNumber $i -width 70 -tags 0 -variations 0 -comments 0]

			# try to handle some obscure problem that the file is not
			# existent yet when calling the mailer
			while {! [file exists $pgnfile]} {
				after 1500 puts "waiting..."
			}
			# send the game to the side to move
			set toMove  [sc_pos side]

			if {$toMove == "white"} {
				set to   $WhiteNA
				set from $BlackNA
			} else {
				set from $WhiteNA
				set to   $BlackNA
			}

			# get rid of spaces in names by using Windows internal real names
			if {$windowsOS} {
				catch {set mailer [file attributes $mailer -shortname]}
			}

			set title   "scid mail 1 game ($CmailGameName)"
			set body    "Final FEN: "
			append body [sc_pos fen]
			append body "\n\n"
			append body "Move List: "
			append body [sc_game moves]
			append body "\n\n"

			# Check what calling convention to use:
			# - mailx  : something like mailx, mutt, nail or whatever via
			#            commandline. This sends the mail without further
			#            intervention by the user
			# - mozilla: call a mozilla or descendent like icedove or
			#            thunderbird. This syntax is found somewhere in the
			#            developers docs and almost entirely undocumented
			# - mailurl: the same syntax as for mailto:-links in webpages
			#            (rfc 2368). This calling convention is supported by
			#            evolution
			# -claws   : Claws mailer, seems to be almost mailurl, but needs
			#            a parameter for attachments
			switch -regexp -- $::CorrespondenceChess::mailermode \
			"mailx" {
				set callstring "$mailer $subject \"$title\" -b $bccaddr $attache \"$pgnfile\" $to <\"$pgnfile\""
			} \
			"mozilla" {
				if {$windowsOS} {
					set callstring "$mailer -compose subject='$title',bcc=$bccaddr,attachment='file:///$pgnfile',to=$to,body=$body"
				} else {
					set callstring "$mailer -compose subject='$title',bcc=$bccaddr,attachment='file://$pgnfile',to='$to',body='$body'"
				}
			} \
			"mailurl" {
				set callstring "$mailer \'mailto:\<$to\>?bcc=$bccaddr\&subject=$title\&attach=$pgnfile\&body=$body\'"
			} \
			"claws" {
				set callstring "$mailer --compose \'mailto:$to?subject=$title&cc=$bccaddr&body=$body\' --attach \"$pgnfile\""
			}
			::CorrespondenceChess::updateConsole "info Calling eMail program: $mailer..."
			CallExternal $callstring

			# Save the game once the move is sent
			set num [sc_game number]
			sc_game save $num

			# Hook up with email manager: search the game in its internal
			# list and add the send flag automatically.
			set done 0
			set idx  0
			foreach dataset $emailData {
				if { [lindex $dataset 0] == $CmailGameName} {
					set done 1
					# add the sent flag and date
					::tools::email::addSentReceived $idx s
				}
				incr idx
			}
		}

		unbusyCursor .
	}

	#----------------------------------------------------------------------
	# Send the move to the opponent via XFCC or eMail
	#----------------------------------------------------------------------
	proc SendMove {resign claimDraw offerDraw acceptDraw } {
		global ::CorrespondenceChess::Outbox
		global ::CorrespondenceChess::XfccSendcmd
		global ::CorrespondenceChess::CorrSlot
		global ::CorrespondenceChess::XfccConfirm
		global ::CorrespondenceChess::num

		busyCursor .

		::CorrespondenceChess::CheckForCorrDB
		if {$CorrSlot > -1} {
			sc_move end

			set Extra [sc_game tags get Extra]
			set extraTagsList [split $Extra "\n"]

			# ... extract it as it contains the unique ID
			foreach i $extraTagsList {
				if { [string equal -nocase [lindex $i 0] "CmailGameName" ] } {
					set CmailGameName [string range $i 15 end-1]
				}
				if { [string equal -nocase [lindex $i 0] "WhiteAddress" ] } {
					set WhiteAdr [split [string range $i 14 end-1] ";"]
					set WhiteAdr [split [string range $i 14 end-1] ";"]
				}
				if { [string equal -nocase [lindex $i 0] "BlackAddress" ] } {
					set BlackAdr [string range $i 14 end-1]
					set BlackAdr [split [string range $i 14 end-1] ";"]
				}
			}

			set pgnfile "[file join $Outbox $CmailGameName].pgn"

			set IdList    [split $CmailGameName "-"]
			set name      [lindex $IdList 0]
			set gameid    [lindex $IdList 1]
			set movecount [sc_pos moveNumber]
			set ply       [sc_pos location]
			set move      [sc_game info previousMoveNT]
			set comment   [sc_pos getComment]
			set Event     [sc_game tags get Event]

			# Throw away everything in [] as often as it exists
			# This matches [%ccsnt] as well as scid marker codes
			regsub -all {\[[^\]]*\]} $comment {} comment

			# moveNumber gives the number of the next full move. This is
			# one to high in case of playing black. Note that for this
			# ply it is _white_ to move!
			if {[sc_pos side] == "white"} {
					set movecount [expr {$movecount-1}]
			}

			# Mark the ID background:
			# yellow while sending in progress,
			# green if the move was sent in the
			# current session (ie. without update)
			.ccWindow.bottom.id tag add hlsent$CmailGameName $num.0 [expr {$num+1}].0
			.ccWindow.bottom.id tag configure hlsent$CmailGameName -background yellow -font font_Bold

			set DlgBoxText "[::tr CCDlgConfirmMoveText]\n\n$name-$gameid:\n\t$movecount. $move\n\t{$comment}"
			if {$resign == 1} {
				set DlgBoxText "$DlgBoxText\n\n[::tr CCResign]"
				# When resigning usually no move is made before.
				# Therefore, we have to increase the ply by one (faking a
				# move) and recalculate the resulting move number if White
				# is to move.
				# This gives:
				# 1. e4 <resign> => ply 2 => no ply increment => move
				# number = 1, move number to send = 1
				# 1. e4 e5 <resign> => increment ply => ply = 3 => move
				# number = 1, move number to send = 2
				if {[sc_pos side] == "white"} {
					set movecount [expr {$ply / 2 + 1}]
					::CorrespondenceChess::updateConsole "info Increment ply $movecount"
				}
			} elseif {$acceptDraw == 1} {
				set DlgBoxText "$DlgBoxText\n\n[::tr CCAcceptDraw]"
			} elseif {$offerDraw  == 1} {
				set DlgBoxText "$DlgBoxText\n\n[::tr CCofferDraw]"
			} elseif {$claimDraw  == 1} {
				set DlgBoxText "$DlgBoxText\n\n[::tr CCClaimDraw]"
			}

			set result 0
			if {$::CorrespondenceChess::XfccConfirm == 1} {
				set result [tk_dialog .roDialog "Scid: [tr CCDlgConfirmMove]" \
						$DlgBoxText "" 1 $::tr(Yes) $::tr(No)]
			}
			if {$result == 0} {
				# Go to the last move is important to send the comment for
				# the last move only not the comment for the current game
				# position!

				# If Event = "Email correspondence game"
				# treat it as cmail game that is send by mail, otherwise it is
				# Xfcc and sent accordingly
				set Mode [::CorrespondenceChess::CheckMode]
				if {$Mode == "EM"} {
					eMailMove
				} elseif {$Mode == "XFCC"} {

					if {$::CorrespondenceChess::XfccInternal == 1} {
						# use internal Xfcc-handling
						::Xfcc::ReadConfig $::CorrespondenceChess::xfccrcfile
						::Xfcc::Send $name $gameid $movecount $move $comment \
								$resign $acceptDraw $offerDraw $claimDraw
					} else {
						if {[file executable "$XfccSendcmd"]} {
							set callstring "$XfccSendcmd $Outbox $name $gameid $movecount $move \"$comment\" $resign $claimDraw $offerDraw $acceptDraw &"

							::CorrespondenceChess::updateConsole "info Spawning external send tool $XfccSendcmd..."
							CallExternal $callstring
						}
					}
				} elseif {$Mode == "Postal"} {
					# produce a postcard
				}

				# Save the game once the move is sent
				sc_game save [sc_game number]

				# setting "noMarkCodes" to 1 would drop the timing comments
				# inserted e.g. by SchemingMind. Do not overwrite eMail based
				# games as the mailer might not have sent them and most
				# mailers load the file right before transmission.
				if {!(($Mode == "EM") || ($Mode == "Relay"))} {
					sc_base export "current" "PGN" $pgnfile -append 0 -comments 1 -variations 1 \
								-space 1 -symbols 1 -indentC 0 -indentV 0 -column 0 -noMarkCodes 0 -convertNullMoves 1
				}

				# Everything done, set background to green
				.ccWindow.bottom.id tag configure hlsent$CmailGameName -background green -font font_Bold
			} else {
				# mark games with unconfirmed moves in gray:
				.ccWindow.bottom.id tag configure hlsent$CmailGameName -background gray -font font_Small
			}
		}
		unbusyCursor .
	}

	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	# source the options file to overwrite the above setup
	if {[catch {source [scidConfigFile correspondence]} ]} {
	  #::splash::add "Unable to find the options file: [file tail $optionsFile]"
	} else {
	  ::splash::add "Correspondence Chess configuration was found and loaded."
	}

	if {[catch { package require http }]} {
	  ::splash::add "http package not found, disabling internal Xfcc support"
		set XfccInternal -1
	} else {
		::http::config -useragent $::Xfcc::useragent
		if {![catch { package require tls }]} {
			::tls::init -ssl3 false -ssl2 false -tls1 true
			http::register https 443 ::tls::socket
		}
	}

	if {[catch {package require tdom}]} {
		::splash::add "tDOM package not found, disabling internal Xfcc support"
		set XfccInternal -1
	}

	::CorrespondenceChess::checkInOutbox
	::CorrespondenceChess::checkXfccrc
	::CorrespondenceChess::checkCorrBase
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
}


###
### End of file: Correspondence.tcl
###
