#!/usr/bin/perl -w
#=========================================================================
#
#    Mega2Scid.pl
#
#    Convert photos from ChessBase Mega Database to scid's internal
#    photo format.
#
#    $Id: Mega2Scid.pl,v 4.3 2011/02/13 18:12:00 arwagner Exp $
#    Last change: <Mon, 2009/02/23 12:09:05 arwagner ingata>
#    Author     : Alexander Wagner
#    Language   : Perl
#
#-------------------------------------------------------------------------
#
#    Copyright (C) 2008 by Alexander Wagner
#
#    This is free software; you can redistribute it and/or modify it
#    under the terms of the GNU Genereal Public License as published
#    by the Free Software Foundation; either version 2, or (at your
#    option) any later version.
#
#    This program is distributed in the hope that it will be usefull,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#    General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#=========================================================================

use XML::XPath;
use IO::Handle;

my @megaxml      = ("mega-2006.xml", 
                    "mega-historic.xml",
                    "mega-2007-addon.xml",
                    "mega-2008-addon.xml");

my $prefix   = "/media/cdrom/playbase";
my $indextool= "spf2spi.tcl";

my $geometry = "80x100>";

# Use the ramdisk /dev/shm to speed up
my $outimg   = "/dev/shm/tmp.gif";
my $outmime  = "/dev/shm/out.mime64";

#======================================================================
foreach $xmlfile (@megaxml) {

	my $output = $xmlfile;
	$output =~ s/xml$/spf/;
	open (OUT, ">$output") or die "Can't open $output: $!\n";

	print "Processing $xmlfile...\n";

# *OUT->autoflush(1);
	*STDOUT->autoflush(1);

	print OUT "# $output\n";
	print OUT "#\n";
	print OUT "# This file contains photos of players from ChessBase Playerbase.\n";
	print OUT "# These photos are converted from jpg to gif and resized to \n";
	print OUT "# around 80x100 pixels each and stored as base64-encoded image.\n";
	print OUT "#\n";
	print OUT "# Copyright Note: These photos may be copyright by ChessBase.\n";
	print OUT "# They are used legally only if you own a ChessBase MegaBase\n";
	print OUT "# and have converted them from this collection yourself.\n";
	print OUT "#\n\n\n";


	my $xp       = XML::XPath->new(filename=>$xmlfile);
	my $nodes    = $xp->find('/cb/p');
	my $names    = ();
	my $dublett  = 0;

	print "Scanning names...\n";
	my $counter = 0;
	# uniquify names
	foreach $player ($nodes->get_nodelist) {

		$counter++;
		$lastname   = $player->find('l');
		$firstname  = $player->find('f');
		$imagefile  = lc($player->find('i'));

		$index = "$lastname, $firstname";
		if (exists ($names{$index})) {
			$dublett++;
		} 
		else {
			$names{$index} = $imagefile;
		}
	}
	print "Found $counter players, $dublett dublett namings...\n";
	print "Starting conversion...\n";

	$counter = 0;
	for my $name (sort keys %names) {
		$counter++;
		my ($lastname, $firstname) = split(/, /, $name);
		$imagefile = $names{$name};
		# print "$lastname, $firstname: $imagefile\n";
		if ($counter % 100 == 0) {
			print "o";
		}
		else {
			print ".";
		}

		print OUT "photo \"$lastname, $firstname\" {\n";

		$inimg = "$prefix/$imagefile";
		
		# Convert images to about 80x100px height
		system ("convert  -geometry \"$geometry\" $inimg $outimg");
		system ("rm $outmime");
		
		# encode it using mpack
		system ("mpack -s none -o $outmime $outimg");

		open (IN, "<$outmime") or die "Can't open $outmime: $!\n";

		for (my $i=0; $i<14; $i++) {
			$line = <IN>;
		}

		while ($line = <IN>) {
			next if $line =~ m/^\n/;
			next if $line =~ m/^-----\n/;
			print OUT $line;
		}
		close(IN);

		print OUT "}\n\n";
	}
	close(OUT);
	sleep(5);
	print "Generating index...\n";
	my $outfile   = $xmlfile;
	$outfile      =~ s/xml$/spf/;
	my $indexfile = $xmlfile;
	$indexfile    =~ s/xml$/spi/;
	system("$indextool $outfile > $indexfile");
	print "\n\n$counter photos converted to $outfile.\n";
}
