###
### metadata.tcl: part of Scid.
### Copyright (C) 2008 Alexander Wagner
###
### $Id: metadata.tcl,v 4.3 2011/02/13 18:12:02 arwagner Exp $
###
### Last change: <Fri, 2008/12/26 13:08:31 arwagner ingata>
###
### Read/Write/Show descriptive information for a specific database
###
#======================================================================

set m .menu.tools

# $m add command -label "Database information..." -command {::metadata::show}

#======================================================================
#
#======================================================================
namespace eval metadata {

	set metadatafilename        ""
	set DBMetadata              ""

	# preset the data hash with the usual Dublin Core values
	set datahash(dc:title)                ""
	set datahash(dc:description)          ""
	set datahash(dc:date)                 ""
	set datahash(dc:subject)              ""
	set datahash(dc:type)                 "Dataset"
	set datahash(dc:source)               ""
	set datahash(dc:coverage)             ""
	set datahash(dc:creator)              "Shanes Chess Information Database (Scid) $::scidVersion"
	set datahash(dc:rights)               ""
	set datahash(dc:format)               "application/sciddb"
	set datahash(dc:identifier)           ""
	set datahash(dc:language)             ""
	set datahash(dc:audience)             ""
	set datahash(dc:provenance)           ""
	set datahash(dc:rightsholder)         ""
	set datahash(dc:instructionalmethod)  ""
	set datahash(dc:accrualmethos)        ""
	set datahash(dc:accrualperiodicity)   ""
	set datahash(dc:accrualpolidy)        ""


	#----------------------------------------------------------------------
	# Store the metadata to a Scid config file.
	#----------------------------------------------------------------------
	proc saveOptions {metadatafile} {

		set optionF ""

		# Copy data back from the hash to the list
		set ::metadata::DBMetadata ""
		foreach key [array names ::metadata::datahash *] {
			lappend ::metadata::DBMetadata [list $key $::metadata::datahash($key)]
		}

		if {[catch {open $metadatafile w} optionF]} {
			tk_messageBox -title "Scid: Unable to write file" -type ok -icon warning \
				-message "Unable to write options file: $metadatafile\n$optionF"
		} else {
			puts $optionF "# Scid options file"
			puts $optionF "# Version: $::scidVersion, $::scidVersionDate"
			puts $optionF "#"
			puts $optionF "# Descriptive Metadata"
			puts $optionF "# This file is generated automatically. Do NOT edit."

			set curtime [clock format [clock seconds]]
			puts $optionF "#"
			puts $optionF "# Last Update: $curtime"
			puts $optionF "#"

			foreach i {::metadata::DBMetadata } {
				puts $optionF "set $i [list [set $i]]"
			}
			close $optionF
		}

		# now write also a Dublin Core XML file
		regsub "sme$" $metadatafile "sg3" dbname
		regsub "sme$" $metadatafile "xml" metadatafile
		if {[catch {open $metadatafile w} optionF]} {
			tk_messageBox -title "Scid: Unable to write file" -type ok -icon warning \
				-message "Unable to write options file: $metadatafile\n$optionF"
		} else {
			puts $optionF "<rdf:RDF"
			puts $optionF "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\""
			puts $optionF "   xmlns:dc=\"http://purl.org/dc/elements/1.1/\">"
			puts $optionF "   <rdf:Description rdf:about=\"$dbname\">"
			foreach key [array names ::metadata::datahash *] {
				puts $optionF "      <$key>$::metadata::datahash($key)</$key>"
			}
			puts $optionF "   </rdf:Description>"
			puts $optionF "</rdf:RDF>"
			close $optionF
		}
	}

	#----------------------------------------------------------------------
	# Show the current metadata from the sme file and allow to edit
	# them. If no metadata are found use sensible defaults.
	#----------------------------------------------------------------------
	proc show {} {

		if { [sc_base current] < 9 } {

			set w .metadataWindow
			if {[winfo exists .metadataWindow]} {
				focus .
				destroy .metadataWindow
				return
			}

			toplevel $w
			wm title $w "Database Information"
			# the window is not resizable
			### wm resizable $w 0 0

			set currentdb [sc_base current]
			set metadatafile [file nativename [sc_base filename $currentdb]]

			set metadatafile "$metadatafile.sme"
			puts $metadatafile

			# set sensible defaults
			set ::metadata::DBMetadata ""
			set ::metadata::datahash(dc:title)               [file tail [sc_base filename $currentdb]]
			set ::metadata::datahash(dc:description)          ""
			set ::metadata::datahash(dc:date)                 [clock format [clock seconds]] 
			set ::metadata::datahash(dc:subject)              ""
			set ::metadata::datahash(dc:type)                 "Dataset"
			set ::metadata::datahash(dc:source)               ""
			set ::metadata::datahash(dc:coverage)             ""
			set ::metadata::datahash(dc:creator)              "Shanes Chess Information Database (Scid) $::scidVersion"
			set ::metadata::datahash(dc:rights)               ""
			set ::metadata::datahash(dc:format)               "application/sciddb"
			set ::metadata::datahash(dc:identifier)           ""
			set ::metadata::datahash(dc:language)             ""
			set ::metadata::datahash(dc:audience)             ""
			set ::metadata::datahash(dc:provenance)           ""
			set ::metadata::datahash(dc:rightsholder)         ""
			set ::metadata::datahash(dc:instructionalmethod)  ""
			set ::metadata::datahash(dc:accrualmethos)        ""
			set ::metadata::datahash(dc:accrualperiodicity)   ""
			set ::metadata::datahash(dc:accrualpolidy)        ""

			# hook up with scid's geometry manager
			setWinLocation $w
			setWinSize $w
			bind $w <Configure> "recordWinSize $w"

			set ::metadata::metadatafilename $metadatafile

			button $w.bOk     -text OK -command {
					::metadata::saveOptions $::metadata::metadatafilename
					destroy .metadataWindow
			}
			button $w.bCancel -text [::tr "Cancel"] -command "destroy $w"

			# enable the standard shortcuts
			keyboardShortcuts $w

			set textcolour "black"
			if {[catch {source $metadatafile} ]} {
				# copy default values if there is no set
				foreach key [array names ::metadata::datahash *] {
					lappend ::metadata::DBMetadata [list $key $::metadata::datahash($key)]
				}
				set textcolour "blue"
			}

			set line 0

			foreach i $metadata::DBMetadata {

				set tag [lindex $i 0]
				set val [lindex $i 1]

				label $w.$tag  -fg $textcolour -text $tag
				set ::metadata::datahash($tag) $val

				entry $w.v$tag -width 60 -textvariable "::metadata::datahash($tag)"

				grid $w.$tag  -stick w -column 0 -row $line
				grid $w.v$tag -stick w -column 1 -row $line

				set line [expr {$line + 1 }]
			}

			# Buttons and ESC-key
			grid $w.bOk      -stick e  -column 0 -row $line -pady 10
			grid $w.bCancel            -column 1 -row $line -pady 10
			bind $w <Escape> "$w.bCancel invoke"

			bind $w <F1> { helpWindow Metadata}
		}

	}

}


