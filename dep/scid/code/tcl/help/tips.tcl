
namespace eval ::tip {}

proc ::tip::show {{n -1}} {
  global tips language
  set w .tipsWin

  if {! [winfo exists .tipsWin]} {
    win::createDialog $w
    wm title $w "Scid: [tr HelpTip]"
    autoscrollText y $w.text_frame $w.text TLabel
    $w.text configure -width 40 -height 8 -wrap word
    ::htext::init $w.text
    pack $w.text_frame -side top -fill both
    pack [ttk::frame $w.b] -side bottom -fill x
    ttk::checkbutton $w.b.start -textvar ::tr(TipAtStartup) -variable startup(tip) -style Small.TCheckbutton
    dialogbuttonsmall $w.b.prev [list -text "<" ]
    dialogbuttonsmall $w.b.next [list -text ">" ]
    dialogbuttonsmall $w.b.close [list -textvar ::tr(Close) -command "destroy $w" ]
    pack $w.b.start -side left -padx 2
    packdlgbuttons $w.b.close $w.b.next $w.b.prev

    bind $w <Up> "$w.text yview scroll -1 units"
    bind $w <Down> "$w.text yview scroll 1 units"
    bind $w <Prior> "$w.text yview scroll -1 pages"
    bind $w <Next> "$w.text yview scroll 1 pages"
    bind $w <Key-Home> "$w.text yview moveto 0"
    bind $w <Key-End> "$w.text yview moveto 0.99"
    bind $w <Escape> "$w.b.close invoke"
    raiseWin $w
    focus $w
  }
  $w.text configure -state normal
  $w.text delete 1.0 end
  if {[info exists tips($language)]} {
    set tiplist $tips($language)
  } else {
    set tiplist $tips(E)
  }

  set ntips [llength $tiplist]
  if {$n < 0} {
    set n [expr int(double($ntips) * rand())]
  }
  set prev [expr $n - 1]
  if {$prev < 0} {set prev [expr $ntips - 1]}
  set next [expr ($n + 1) % $ntips]
  $w.b.prev configure -command "::tip::show $prev"
  $w.b.next configure -command "::tip::show $next"
  set tip "<center><b>$::tr(Tip) [expr $n + 1]:</b></center><br><br>"
  append tip [string trim [lindex $tiplist $n]]
  ::htext::display $w.text $tip "" 0
}

set tips(E) {
  {
    Scid has over 30 <a Index>help pages</a>, and in most Scid windows
    pressing the <b>F1</b> key will produce the help page about that
    window.
  }
  {
    Some Scid windows (e.g. the game information area, database
    <a Switcher>switcher</a>) have a right-mouse button menu. Try
    pressing the right mouse button in each window to see if it has
    one and what functions are available.
  }
  {
    Scid offers you more than one way to enter chess moves, letting
    you choose which suits you best. You can use the mouse
    (with or without move suggestion) or the keyboard
    (with or without move completion). Read the
    <a Moves>entering chess moves</a> help page for details.
  }
  {
    If you have a few databases that you open often, add a
    <a Bookmarks>bookmark</a> for each one, and then you will be able
    to open them faster using the bookmarks menu.
  }
  {
    You can see all the moves of the current game
    (with any variations and comments) using the <a PGN>PGN window</a>.
    In the PGN window, you can go to any move by clicking
    the left mouse button on it, or use the middle or right mouse button
    to see a preview of that position.
  }
  {
    You can copy games from one database to another using drag and drop
    with the left mouse button in the <a Switcher>database switcher</a> window.
  }
  {
    Scid can open PGN files, even if they are compressed with Gzip
    (with a .gz filename suffix). PGN files are opened read-only, so
    if you want to edit a PGN file in Scid, create a new Scid database
    and copy the PGN file games to it using the
    <a Switcher>database switcher</a>.
  }
  {
    If you have a large database you use with the <a Tree>tree</a> window
    often, it is worth selecting <b>Fill cache file</b> from the tree
    window File menu. This will remember tree statistics for many common
    opening positions, making tree access faster for the database.
  }
  {
    The <a Tree>tree</a> window can show you all moves played from the
    current position, but if you also want to see all the move orders
    that reached this position, you can find them by generating
    an <a OpReport>opening report</a>.
  }
  {
    In the <a GameList>game list</a> window, press the left or right mouse
    button on the heading of each column to adjust its width.
  }
  {
    With the <a PInfo>player information</a> window (just click on either
    player name in the game info area below the main window chessboard
    to open it), you can easily set the <a Searches Filter>filter</a> to
    contain all games by a certain player with a certain result by
    clicking on any value that is displayed <red>in red text</red>.
  }
  {
    When studying an opening, it can be very useful to do a
    <a Searches Board>board search</a> with the <b>Pawns</b> or
    <b>Files</b> option on an important opening position, as this may
    reveal other openings that reach the same pawn structure.
  }
  {
    In the game information area (below the chessboard), you can press
    the right mouse button to produce a menu for customising it. For
    example, you can make Scid hide the next move which is useful for
    training by playing though a game guessing the moves.
  }
  {
    If you often do a lot of database <a Maintenance>maintenance</a> on
    a large database, you can do several maintenance jobs at once using
    the <a Maintenance Cleaner>cleaner</a>.
  }
  {
    If you have a large database where most games have an EventDate and
    you want the games in date order, consider <a Sorting>sorting</a> it
    by EventDate then Event instead of Date then Event, as this will
    help to keep games in the same tournament with different dates
    together (assuming they all have the same EventDate, of course).
  }
  {
    Before <a Maintenance Twins>deleting twin games</a>, it is a good idea
    to <a Maintenance Spellcheck>spellcheck</a> your database since this
    will enable to Scid find more twins and mark them for deletion.
  }
  {
    <a Flags>Flags</a> are useful for marking database games with
    characteristics you may want to search for at a later time, such
    as pawn structure, tactics, etc. You can search by flags with a
    <a Searches Header>header search</a>.
  }
  {
    If you are playing through a game and want to try out some moves
    without altering the game, simply turn on Trial mode (with the
    <b>Ctrl+space</b> shortcut or from the toolbar icon), then turn it off
    again to return to the original game when you are done.
  }
  {
    To find the most prominent games (with high-rated opponents)
    reaching a particular position, open the <a Tree>tree</a> window
    and from there, open the best games list. You can even restrict
    the best games list to show only games with a particular result.
  }
  {
    A great way to study an opening using a large database of games is
    to turn on training mode in the <a Tree>tree</a> window, then play
    against the database to see which lines occur often.
  }
  {
    If you have two databases open, and want to see <a Tree>tree</a>
    statistics of the first database while examining a game from the
    second database, just press the <b>Lock</b> button in the tree
    window to lock it to the first database and then switch to the
    second base.
  }
  {
    The <a Tmt>tournament finder</a> is not only useful for finding
    a particular tournament, but can also be used to see what tournaments
    a certain player has competed in recently or browse the top
    tournaments played in a particular country.
  }
  {
    There are a number of common patterns defined in the
    <a Searches Material>Material/Pattern</a> search window that you
    may find useful for openings or middlegame study.
  }
  {
    When searching for a particular material situation in the
    <a Searches Material>Material/Pattern</a> search window, it is
    often useful to restrict the search to games that match for at
    least a few half-moves to eliminate games where the searched-for
    situation only occurred briefly.
  }
  {
    If you have an important database you do not want to accidentally
    alter, select <b>Read-only...</b> from the <b>File</b> menu after
    opening it, or change its file permissions to be read-only.
  }
  {
    If you use XBoard or WinBoard (or some other chess program that
    can copy a chess position in standard FEN notation to the clipboard)
    and want to copy its current chess position to Scid, the fastest and
    easiest way is to select <b>Copy Position</b> from the File menu in
    XBoard/WinBoard, then <b>Paste start board</b> from the Edit menu
    in Scid.
  }
  {
    In a <a Searches Header>header search</a>, player/event/site/round
    names are case-insensitive and match anywhere in a name. You can choose
    to do a case-sensitive wildcard search instead
    (where "?" = any single character and "*" = zero or more characters)
    by entering the search text "in quotes". For example, type "*BEL"
    (with the quote characters) in the site field to find all games played
    in Belgium but not Belgrade.
  }
  {
    If you want to correct a move in a game without losing all the moves
    played after it, open the <a Import>Import</a> window, press the
    <b>Paste current game</b> button, edit the incorrect move and then
    press <b>Import</b>.
  }
  {
    If you have an ECO classification file loaded, you can go to the
    deepest classified position in the current game with
    <b>Identify opening</b> in the <b>Game</b> menu
    (shortcut: Ctrl+Shift+D).
  }
  {
    If you want to check the size of a file or its date of last modification
    before opening it, use the <a Finder>file finder</a> to open it.
  }
  {
    An <a OpReport>opening report</a> is great for learning more about
    a particular position. You can see how well it scores, whether it
    leads to frequent short draws, and common positional themes.
  }
  {
    You can add the most common annotation symbols (!, !?, +=, etc) to the
    current move or position with keyboard shortcuts without needing to
    use the <a Comment>comment editor<a> -- for example, type "!" then
    the Return key to add a "!" annotation symbol. See the
    <a Moves>Entering chess moves</a> help page for details.
  }
  {
    If you are browsing openings in a database with the <a Tree>tree</a>,
    you can see a useful overview of how well the current opening is
    scoring recently and between high-rated players by opening the
    Statistics window (shortcut: Ctrl+I).
  }
  {
    You can change the main window board size by holding down the <b>Ctrl</b>
    and <b>Shift</b> keys, and pressing the <b>Left</b> or <b>Right</b>
    arrow key.
  }
  {
    After a <a Searches>search</a>, you can easily browse through all
    the matching games by holding down <b>Ctrl</b> and pressing the
    <b>Up</b> or <b>Down</b> key to load the previous or next
    <a Searches Filter>filter</a> game.
  }
  {
    Windows can be docked by checking the relevant entry in the option menu.
    Tabs can be dragged and dropped from one notebook to another and layed out
    by right clicking on the tab widget.
  }
  {
    Scid GUI can be customized by TTK-Themes. This allow different look and feels
    according the favorite preference of the user. Use "Options - Load Theme" and
    look in the <a Appearance>help section</a>.
  }
}

