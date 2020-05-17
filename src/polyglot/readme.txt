
Legal details
-------------

PolyGlot 1.4 Copyright 2004-2006 Fabien Letouzey.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
USA

See the file "copying.txt" for details.


General
-------

PolyGlot 1.4 (2006/01/16).

PolyGlot is a "UCI adapter".  It connects a UCI chess engine to an
xboard interface such as WinBoard.  UCI2WB is another such adapter
(for Windows).

PolyGlot tries to solve known problems with other adapters.  For
instance, it detects and reports draws by fifty-move rule, repetition,
etc ...


Official distribution URL
-------------------------

The official distribution web site is Leo Dijksman's WBEC Ridderkerk:
http://wbec-ridderkerk.nl/  This is where you should be looking for
PolyGlot updates in the future.


Install
-------

PolyGlot can be placed in its own directory, or anywhere it can access
the DLL file from (on Windows).

On Windows the files "polyglot.exe" and "cygwin1.dll" (which you can
download from http://wbec-ridderkerk.nl/) are needed.  On Linux and
Mac OS X only the file "polyglot_linux" or "polyglot_mac" is required.


Compiling
---------

The distribution comes up with Windows, Linux and Mac OS X binaries.
Compiling should therefore not be necessary on those systems unless
you want to make a change in the program.  In any case this section
describes the compiling procedure, it is safe to skip it.

PolyGlot is a POSIX application (Unix compatible), and was developed
on Linux using g++ (the GNU C++ compiler).

1) Unix

You should be able to compile it on any POSIX-compliant operating
system (*not* Windows) with the following command line (or similar):

> g++ -O2 -o polyglot *.cpp

IMPORTANT: In "io.cpp", the variable "UseCR" should be set to "false".

A Makefile is provided but might not work on your system ...

2) Windows

On Windows, you *must* use Cygnus GCC to compile PolyGlot.

IMPORTANT: In "io.cpp", the variable "UseCR" should be set to "true".


Usage
-----

PolyGlot acts as an xboard engine.  There should be no difference with
a normal chess program as far as the interface (e.g. WinBoard) is
concerned.

PolyGlot is invoked using "polyglot <INI file>".  Note that PolyGlot
will look for the INI file in the current directory.  If no <INI file>
is given, "polyglot.ini" is selected.

To use PolyGlot with XBoard, you would type something like this:
> xboard -fd 'ini_dir' -fcp 'polyglot engine.ini'

Quotes are important when there is a space in the argument.

IMPORTANT: some users seem confused by the concept of "current
directory".  PolyGlot needs to know where to read (INI file) and write
(log file) files.  Although it's possible to specify the full path to
each file, a better solution is to provide a directory when launching
PolyGlot, e.g. with the "-fd" XBoard option above.  The directory
should be where the INI file is.


INI file
--------

There should be a different INI file for each engine.  Sections are
composed of "variable = value" lines.  See the sample INI files in the
"example" directory.

NOTE: There can be spaces in variable names or values.  Do not use
quotes.

1) [PolyGlot] section

This section is used by PolyGlot only.  The engine is unaware of these
options.  The list of available options is detailed below in this
document.

2) [Engine] section

This section contains engine UCI options.  PolyGlot does not
understand them, but sends the information to the engine at startup
(converted to UCI form).  You can add any UCI option that makes sense
to the engine (not just the common options about hash-table size and
tablebases).

NOTE: use INI syntax, not UCI.  For example "OwnBook = true" is
correct.  It will be replaced by PolyGlot with "setoption name OwnBook
value true" at engine startup.

Standard UCI options are "Hash", "NalimovPath", "NalimovCache" and
"OwnBook".  Hidden options like "Ponder" or "UCI_xxx" are automatic
and should not be put in an INI file.

The other options are engine-specific.  Check their name using a UCI
GUI or launch the engine in a console and type "uci".


Options
-------

These should be put in the [PolyGlot] section.

- "EngineName" (default: UCI name)

This is the name that will appear in the xboard interface.  It is
cosmetic only.  You can use different names for tweaked versions of
the same engine.

If no "Engine Name" is given, the UCI name will be used.

- "EngineDir" (default: ".")

Full path of the directory where the engine is installed.  You can use
"." (without the quotes) if you know that PolyGlot will be launched in
the engine directory or the engine is in the "path" and does not need
any data file.

- "EngineCommand"

Put here the name of the engine executable file.  You can also add
command-line arguments.  Path searching is used and the current
directory will be "EngineDir".

NOTE: Unix users are recommended to prepend "./"; this is required on
some secure systems.

- "Log" (default: false)

Whether PolyGlot should log all transactions with the interface and
the engine.  This should be necessary only to locate problems.

- "LogFile"

The name of the log file.  Note that it is put where PolyGlot was
launched from, not into the engine directory.

WARNING: Log files are not cleared between sessions, and can become
very large.  It is safe to remove them though.

- "Resign" (default: false)

Set this to "true" if you want PolyGlot to resign on behalf of the
engine.

NOTE: Some engines display buggy scores from time to time although the
best move is correct.  Use this option only if you know what you are
doing (e.g. you always check the final position of games).

- "ResignMoves" (default: 3)

Number of consecutive moves with "resign" score (see below) before
PolyGlot resigns for the engine.  Positions with only one legal move
are ignored.

- "ResignScore" (default: 600)

This is the score in centipawns that will trigger resign "counting".

- "ShowPonder" (default: true)

Show search information during engine pondering.  Turning this off
might be better for interactive use in some interfaces.

- "KibitzMove" (default: false)

Whether to kibitz when playing a move.

- "KibitzPV" (default: false)

Whether to kibitz when the PV is changed (new iteration or new best move).

- "KibitzCommand" (default: "tellall")

xboard command to use for kibitzing, normally "tellall" for kibitzing
or "tellothers" for whispering.

- "KibitzDelay" (default: 5)

How many seconds to wait before starting kibitzing.  This has an
affect only if "KibitzPV" is selected, move kibitzes are always sent
regardless of the delay.


Work arounds
------------

Work arounds are identical to options except that they should be used
only when necessary.  Their purpose is to try to hide problems with
various software (not just engines).  The default value is always
correct for bug-free software.

IMPORTANT: Any of these work arounds might be removed in future
versions of PolyGlot.  You are strongly recommended to contact the
author of faulty software and truly fix the problem.

PolyGlot 1.4 supports the following work arounds:

- "UCIVersion" (default: 2)

The default value of 2 corresponds to UCI+.  Use 1 to select plain
UCI for engines that have problems with UCI+.

- "CanPonder" (*** NEW ***, default: false)

PolyGlot now conforms to the documented UCI behaviour: the engine will
be allowed to ponder only if it (the engine) declares the "Ponder" UCI
option.  However some engines which can actually ponder do not declare
the option.  This work around lets PolyGlot know that they can ponder.

- "SyncStop" (*** NEW ***, default: false)

When a ponder miss occurs, Polyglot interrupts the engine and
IMMEDIATELY launches a new search.  While there should be no problem
with this, some engines seem confused and corrupt their search board.
"SyncStop" forces PolyGlot to wait for the (now useless) ponder search
to finish before launching the new search.

- "PromoteWorkAround" (*** NEW ***, default: false)

Some engines do not specify a promotion piece, e.g. they send "e7e8"
instead of the correct "e7e8q".  This work around enables the
incorrect form (and of course promotes into a queen).


Opening Book
------------

PolyGlot 1.4 provides a simplistic opening-book implementation.

The following options can be added to the [PolyGlot] section:

- "Book" (default: false)

Indicates whether a PolyGlot book should be used.  This has no effect
on the engine own book (which can be controlled with the UCI option
"OwnBook" in the [Engine] section).  In particular, it is possible to
use both a PolyGlot book and an engine book.  In that case, the engine
book will be used whenever PolyGlot is out of book.  Remember that
PolyGlot is unaware of whether the engine is itself using a book or
not.

- "BookFile"

The name of the (binary) book file.  Note that PolyGlot will look for
it in the directory it was launched from, not in the engine directory.
Of course, full path can be used in which case the current directory
does not matter.

Note that there is no option to control book usage.  All parameters
are fixed when compiling a PGN file into a binary book (see below).
This is purposeful and is not likely to change.

Using a book does not require any additional memory, this can be
important for memory-limited tournaments.

A default book "fruit.bin" is provided in the archive.  Note that this
book is very small and should probably not be used in serious games.
I hope that users will make other books available in the future.


Book Making
-----------

You can compile a PGN file into a binary book using PolyGlot on the
command line.  At the moment, only a main (random) book is provided.
It is not yet possible to control opening lines manually.  I am
working on it though.

Usage: "polyglot make-book <options>".

"make-book" options are:

- "-pgn"

Name of the input PGN file.  PolyGlot should support any
standard-conforming file.  Let me know if you encounter a problem.

- "-bin"

Name of the output binary file.  I suggest ".bin" as the extension but
in fact PolyGlot does not care.

- "-max-ply" (default: infinite)

How many plies (half moves) to read for each game.  E.g. if set to
"20", only the first 10 full moves of each game will be scanned.

- "-min-game" (default: 3)

How many times must a move be played to be kept in the book.  In other
words, moves that were played too rarely will be left out.  If you
scan full games "2" seems a minimum, but if you selected lines
manually "1" will make sense.

- "-only-white" *** NEW ***

Save only white moves.  This allows to use different parameters for
white and black books, and merge them into a single file with the
"merge-book" command, see below.

- "-only-black" *** NEW ***

Same for black moves.

- "-uniform" *** NEW ***

By default, a probability is calculated by PolyGlot for each move
depending on how popular it is (how often it was playing in the
provided PGN file) and how much it "scored".  This option bypasses the
default mechanism and affects equal probability to all moves.  This
allows more variety of play.

This option is normally used only with hand-selected lines (e.g. "user
books").

---

Example: "polyglot make-book -pgn games.pgn -bin book.bin -max-ply 30".

Building a book is usually very fast (a few minutes at most).  Note
however that a lot of memory may be required.  To reduce memory usage,
select a ply limit.


Book Merging
------------

*** NEW ***

Usage: "polyglot merge-book -in1 <file1> -in2 <file2> -out <file>"

Merge two bin files into a single one.  <file1> has "priority"; this
means that if a position is present in both input books, data from
<file2> will be ignored for this position.

The two main applications are:

1) combine a white book and a black book (in which case priority does
   not matter)

2) combine a "user book" of manually-selected lines with a broader one
   from a large game set

What follows is an admitedly complicated example of how this can be
used.  DO NOT MAILBOMB ME IF YOU DO NOT UNDERSTAND!

My hope is that at least one advanced user will get what I mean and
writes a better explanation on a web page or forum thread (yes, that's
YOU, thanks by the way) ...

---

Imagine that we've got 4 PGN files as follows:

w1.pgn: fixed white lines, all moves manually checked
w2.pgn: selected games (for random book as with PolyGlot 1.3)

b1.pgn and b2.pgn: same for black

The first step is to build 4 .bin files with appropriate options.
Lines starting with "> " indicate what is typed on the command line.

> polyglot make-book -min-game 1 -uniform -only-white -pgn w1.pgn -bin w1.bin

I added "-uniform" because it allows randomness in the fixed lines
(e.g. d4+e4 at 50%).  It has no effect if lines are deterministic
(only one move for a given position).

"-min-game 1" is characteristic for user books.  All moves are supposed
to be safe so there is no reason to filter them with other heuristics.

> polyglot make-book -min-score 50 -only-white -pgn w2.pgn -bin w2.bin

This shows how min-score can actually be different for white and black
(as with multiple books).  I don't use "max-ply" because "min-game"
default value of 3 will limit depth somewhat.  You are of course free
to use it.

Same for black:

> polyglot make-book -min-game 1 -uniform -only-black -pgn b1.pgn -bin b1.bin
> polyglot make-book -min-score 40 -only-black -pgn b2.pgn -bin b2.bin

At this point we have 4 .bin files.  Notice that different parameters
were used for white and for black (not to mention that different PGN
files can be used).

---

Let's now merge the white books.

> polyglot merge-book -in1 w1.bin -in2 w2.bin -out w.bin

Input files are not symmetrical, "in1" has priority over "in2".

"skipped xxx entries." message from PolyGlot means there were some
position conflicts.  This is normal since we want to overwrite some
random moves with fixed lines instead.

Same for black:

> polyglot merge-book -in1 b1.bin -in2 b2.bin -out b.bin

Now we can finally merge the white and black books.

> polyglot merge-book -in1 w.bin -in2 b.bin -out book.bin

It's important to check that there are no conflicts, otherwise
something went wrong.

Note that this last operation was only made possible thanks to colour
filtering, otherwise nearly all positions would lead to conflicts.
For this reason, it does not make much sense to mix old .bin files
(which contain moves for both colours).

All these command lines might seem numerous and complicated but they
can be put together into batch files.


Chess 960
---------

*** NEW ***

PolyGlot now supports Chess 960.

However note that most xboard interfaces like WinBoard do not (except
perhaps on an Internet chess server)!

Here are pointers to modified XBoard/WinBoard versions that are known
to work with PolyGlot in Chess960 mode:

http://www.ascotti.org/programming/chess/winboard_x.htm (Windows)
http://www.glaurungchess.com/xboard-960.tar.bz2 (Unix)
http://www.milix.net/aice (?)

It is also possible that PolyGlot is useful in combination with
Arena(!): Arena Chess960 works correctly in xboard mode but it seems
not compatible with the official UCI standard.  With PolyGlot it is
possible to include Chess960 UCI engines by using the xboard protocol
instead.


History
-------

2004/04/30: PolyGlot 1.0

- first public release.

2004/10/01: PolyGlot 1.1

- added "StartupWait" and "PonderWorkAround" ("AutoQuit" was available
  in version 1.0 but not documented).

- fixed a minor bug that could prevent "AutoQuit" from working with
  some engines.

2005/01/29: PolyGlot 1.2

- rewrote engine initialisation and UCI parsing to increase
  UCI-standard compliance

- added multi-move resign

- added an internal work around for engines hanging with WinBoard

2005/06/03: PolyGlot 1.3

- added opening book

- added kibitzing

- added "ShowPonder" option

2006/01/16: PolyGlot 1.4

- added Chess960 (requires "fischerandom" xboard variant)

- added "-only-white", "-only-black" and "-uniform" book-making
  options

- added "merge-book" command

- added "CanPonder", "SyncStop" and "PromoteWorkAround" work arounds

- fixed "Move Now" (the engine was interrupted but the move was
  ignored)

- fixed an UCI+draw problem that could occur with some engines after a
  draw by 50 moves or repetition

- fixed pondering behaviour: the engine will ponder only if it
  declares the "Ponder" UCI option


Known problems
--------------

The addition of Chess960 support lead to a change in internal-move
representation for castling.  This slightly affected the opening-book
format.  I recommend that you recompile books with this version.

Fruit 2.2 and above handle both book formats though.

---

Several users reported engines losing on time.  The playing conditions
always mixed playing on an Internet server with pondering.  Early
log-file analysis did not reveal any misbehaviour by PolyGlot, but I
have others to study.

It is not yet clear what the source of the problem is, but let me
state one more time that there is a forever incompatibility between
the xboard and UCI protocol regarding a complex
pondering/remaining-time relation.  I suspect this might be related to
the problem described above and if so, it is possible that there is no
clean solution to it!

In any case I have other log file to study that might reveal
something, stay tuned!


Thanks
------

Big thanks go to:

- Leo Dijksman for compiling, hosting the PolyGlot distribution on his web site
  (see Links) and also for thorough testing

- Tord Romstad, Joshua Shriver and George Sobala for compiling and
  testing on Mac OS X

- all those who reported problems or proposed improvements; I am not
  well organised enough to provide their names!


Links
-----

- Tim Mann's Chess Pages: http://www.tim-mann.org/xboard.html
- Leo Dijksman's WBEC Ridderkerk: http://wbec-ridderkerk.nl/
- Volker Pittlik's Winboard Forum: http://wbforum.volker-pittlik.name/


Contact me
----------

You can contact me at fabien_letouzey@hotmail.com; expect SLOW answer,
if at all!

If I am not available, you can discuss PolyGlot issues in Volker
Pittlik's Winboard Forum: http://wbforum.volker-pittlik.name/

In fact for questions regarding specific Windows-only engines, you are
advised to ask directly in the WinBoard forum, as I don't have Windows
myself.


The end
-------

Fabien Letouzey, 2006/01/16.

