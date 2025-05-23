## September 2024: Version 1.6.4

- Bugfix: Qt6 will not serialize Engine options 

## July 2024: Version 1.6.2

- Bugfix:#333 Accented letters in annotations in UTF8
- Bugfix: #349 Incorrect Pinned Move in Analysis Window
- Bugfix: Three fold repetition if ep was possible
- Bugfix: Openings Summary in Player Info
- Bugfix: Random number selection issues on some OS
- Bugfix: #341 ChessX Draw Game by Insufficient Material
- Bugfix: CR #336 Nalimov TB gives wrong evaluation
- Bugfix: Jumping through sibling variations
- Bugfix: CR #334 Pressing 'Pin engine' resets the analysis
- Bugfix: Move indicator shows wrong line in case of Multi-PV, prefer TB
- Bugfix: CR #320 (PGN Setup->SetUp tag)
- New feature: Lichess Broadcasts
- New feature: Repainting same arrow or square removes special annotation
- New feature: Shift+Click opens first link in comment
- New feature: Export as localized text
- New feature: Workaround Chessable's tokenizer by adding space bf comment
- New feature: Load range of games from online bases
- New feature: Allows piece specific sounds
- New feature: Add a volume slider for sounds
- New feature: CR #209 Hide lines in Analysiswidget
- New feature: Give coordinates a color setting of their own
- New feature: Configurable Text to Speech 
- Changed: AnalysisWidget toolbutton

## January 2023: Version 1.5.8

- New feature: Allow preserving existing ECO codes 
- New Feature: Add optional source tag when merging databases
- New Feature: Lichess Opening Databases
- New feature: Copy text fragments from game browser
- New feature: Boardview as drop target for databases and games
- New feature: Edit tags from the game list view
- Changed: Allow editing ECO codes in the game list view
- Changed: Cycling through variations
- Changed: Performance update PGN Parser
- Changed: #290 Windows hides dialogs if off-screen
- Changed: CR #185 Recognize and annotate draw conditions
- Changed: CR #191 Engine analysis not inserted in notation
- Changed: Transfer board rotation from Board Setup
- Bugfix: Fix issue with sending UCI BOOLs
- Bugfix: Fix error message when theme cannot be installed
- Bugfix: Fix font issue in Qt for windows
- Bugfix: Fix broken DB download URLs
- Bugfix: Crash drawing a piece from the board
- Bugfix: Fix issue #307 Board style resets after editing preferences (macOS)
- Bugfix: Memory Leak while reading evaluations 
- Bugfix: Tablebase evaluations 
- Bugfix: Update chart widget during auto analysis 
- Bugfix: Open Recent does not respect UTF8
- Bugfix: Loading of large games
- Bugfix: Fix dirty notification from database to GUI
- Bugfix: #287 Linux archive / build environment
- Bugfix: #288 No speech engine available
- Bugfix: Avoid nan in arrow painting

## December 2020: Version 1.5.6

- New Feature: Implement read support for Scid binary database (*.si4)
- New feature: Spray fields in pen mode by pressing alt+Left Mouse
- New feature: Add option to hide special annotations in view
- New feature: Show variation arrows
- New feature: Engine draws an arrow for its best move
- New feature: Support online chess.com / lichess data access
- New feature: Scroll through game with ChartWidget
- New feature: Display eval annotations in the chart (%eval)
- New feature: Open file from scratchpad / Drag file names to scratchpad
- New feature: Visualization training
- New feature: Indicate Draw percentage in OpeningTree
- Bugfix: Opening tree - switching between Database and Filter did not trigger tree update
- Bugfix: Display of symbolic pieces in OpeningTreeWidget and variation list
- Bugfix: Hotkeys are not serialized properly
- Bugfix: Duplicate hotkeys for menus with same name
- Bugfix: Do not ask to save clipboard
- Bugfix: Dark theme handling
- Changed: Raise scratchpad when New or Open is invoked
- Changed: Resize text fonts with Ctrl+ Ctrl- or Ctrl+Alt+Wheel
- Changed: Increase max size for writable databases
- Changed: Disable Raise of game list after filtering

## March 2020: Version 1.5.4

- New Feature: Support lc0 for Mac OS
- New feature: blunder checking
- New feature: Speak moves
- New feature: Refactor complete database
- Changed: Command-Key to force query for promotion dialog (instead of Meta)
- Bugfix: Undo setting up board required two steps
- Bugfix: Clearing database was broken
- Bugfix: entering nags/null movees via keyboard

## February 2018: Version 1.5.0

- New Feature: Provide multi-selection for the database list view
- New Feature: Load favorite databases at startup
- New Feature: Read Chessbase Books (ctg)
- New Feature: Read Arena Books (.abk)
- New Feature: Drag databases into list of games
- New feature: Dark Theme
- New feature: FICS console with commandline
- New feature: Syzygy 7 men online tablebase
- New feature: Change font size via mouse wheel
- New feature: Search depth for analysis
- New feature: Filter operation from gamelist
- New feature: Allow user-provided ECO files
- New feature: Save dialog allows to store extra tags
- New feature: Restrict search to start of word / end of word
- New feature: Edit shortcuts by pressing key
- New feature: Annotations beneath board view
- New feature: Turn board for given player
- New feature: Send database to Mail-Client
- New feature: Show analysis of position with one piece moved
- New feature: Merge internal and external themes, new option to scale board images
- Bugfix: Reduced Memory Footprint
- Bugfix: Removing a sub-variation kept some invisble moves
- Bugfix: Internal engine produced silly suggestions in case of a check
- Bugfix: Parse more than one tag in a line
- Bugfix: Changing book in analysis should update moves immediately
- Bugfix: Do not list read-only databases as copy targets
- Bugfix: Update game list columns after leaving preferences
- Bugfix: Console output and event info lacks termination of bold text
- Bugfix: Avoid empty annotation when merging a game
- Bugfix: "New Game" disabled after loading game
- Bugfix: Invalid notation when inserting line from analysis
- Bugfix: Puzzle mode not terminated properly
- Bugfix: Performance reading Polyglot positions
- Bugfix: Disconnecting a FICS session leaked some memory
- Bugfix: Progress bar missing occasionally
- Changed: Draw arrows with right mouse
- Changed: Index file format updated
- Changed: Support engines with state issues
- Changed: time representation to ISO format
- Changed: Do not ask to save deleted games
- Changed: Copy current game instead of first selected game
- Changed: Kick out hotkeys for database changes
- Changed: Analysis and Endgametables display localised piece strings
- Changed: Books support localized piece representation
- Changed: Accept PGN with obscure RAVs
- Changed: Index file format stores data size
- Changed: If loading modifies db, mark db as changed
- Changed: Always use DefaultDataPath (instead of last used folder) for Save Dialogs
- Changed: Index file version

## April 2017: Version 1.4.6

- New feature: Indicate underprotected pieces
- New feature: Indicate covered squares
- New feature: Book building progress
- Bugfix: Annotations remain visible after adding variation
- Bugfix: Saving games on non-current database
- Bugfix: Name of downloaded files
- Bugfix: Crash engine accessing closed book
- Changed: Use more threads for book building

## January 2017: Version 1.4.4

- New feature: Game time for 2-player mode
- New Feature: Drag databases into list of games
- New feature: Copy lists to IMG/HTML in Clipboard
- New feature: Workaround UCI option issue in Stockfish 8
- New feature: Extended polyglot book generation
- New feature: Reverse moving with mouse
- New feature: Dragging piece with CTRL+SHIFT asks main engine / tablebase for eval
- New feature: FICS with Clock above / below board
- New feature: cz language pack
- New feature: Message History
- New feature: Rendering for Retina displays
- New feature: Indicate number of pieces in Board Setup
- New feature: Indicate King in check
- New feature: Paint Tools
- New feature: Show possible targets for selected piece
- New feature: FICS rematch
- New feature: Search Help pages for texts
- New feature: FICS requests with color
- New feature: Show all good tablebase moves
- Bugfix: UCI_Chess960 occasionally not sent to Engine
- Bugfix: Handling of UCI empty string tag
- Bugfix: Renaming a player / event tag gave inconsistent index in case of existing target tag
- Bugfix: Restoration of floating docks
- Bugfix: Chess960 castling direction wrong
- Bugfix: Chess960 with UCI engines
- Bugfix: Resizing the application would sometimes fail
- Bugfix: Matchparameter Dialog misplaced
- Bugfix: Premoving allowed moving opponent pieces
- Changed: Analysis time up to 10 minutes
- Changed: Bundle Stockfish 8
- Changed: Icons in context menus
- Changed: Matchparameter Dialog
- Changed: Square annotations
- Changed: Tock at 30..20..10s before Timeout
- Changed: Move some preferences onto toolbar

## February 2016: Version 1.4.0

- New Feature: Filter duplicate games
- New Feature: Chess960 / Fischerandom Support
- New Feature: Game matches with sudden death timing
- New Feature: Let two engines play out a game (engine match)
- New Feature: #104 Display arbitrary tags in game list
- New Feature: Scratchpad
- New feature: Playerinfo filters complete openings
- New feature: Support SCID format piece sets
- New feature: Favorite web source with auto-increment
- New feature: List of recently used games
- New feature: Store some more FICS preferences
- New feature: Start threat with '--'<Enter>
- New feature: Allow castling in normal game as in Chess960 with dragging King onto Rook
- New feature: Parse mal-formed FEN with - - - - at end
- New feature: Show folders on Linux desktops (similar to mac OS/Windows)
- New feature: Engine logging to file
- New feature: Helpbrowser with Index Combobox
- New feature: Castling support for chess960
- New feature: Completer for FICS say messages
- New feature: Added checkbox so that engine starts game
- New feature: Premove against engine
- New feature: GUI provides book moves for engine games
- New feature: Plock 10s before end of FICS game
- New feature: Select Promotion Dialog with Meta-Modifier
- New feature: Show time annotation also for previous move (if available)
- New feature: Accept resignations from winboard engines
- New feature: FICS start button
- New feature: Assign rating to databases
- New feature: Test 50 moves/position repetition when playing out games
- New feature: No hints during training or puzzling
- New feature: Drag/drop PGN-Files/Text to other applications
- New feature: Make images flipped if mainboard is flipped
- New feature: B/W images of board
- Bugfix: Crash loading small file from command line
- Bugfix: Eco Info formatting
- Bugfix: Castling not possible sometimes
- Bugfix: Moving pieces with keyboard in auto-responder mode
- Bugfix: Frame around square where drag starts
- Bugfix: Undo move in FICS puzzle mode
- Bugfix: FICS timeout handling
- Bugfix: Undo move in FICS puzzle mode
- Bugfix: FICS timeout handling
- Bugfix: Openingthread crashes at end of FICS game
- Bugfix: Fics matches were not continued
- Bugfix: Copied games occasionally did not show up in game list immediately
- Bugfix: Closing databases at shutdwon if saving was neglected was wrong / duplicated queries for saving
- Bugfix: Copying games to active database occasionally forgot to update GUI
- Bugfix: Opening Tree might crash
- Bugfix: Player Opening Statistics wrong
- Bugfix: Do not indicate premove in FICS puzzle mode
- Bugfix: Output would occasionally write orphaned x... to HTML
- Bugfix: Size of slider text (thx to Byrial Jensen)
- Bugfix: Tablebase results would sometimes not fit to actual board
- Bugfix: FICS text "puzzle almost solved" ends puzzle mode
- Bugfix: Handle invalid password upon connecting to FICS
- Bugfix: Terminate auto analysis upon changing of database
- Bugfix: Doing moves while not at end of game broken
- Bugfix: Autoplayer did not work together with backward analysis enabled
- Bugfix: Chartwidget repaints occasionally fail
- Bugfix: Copy games from main menu did not work for selections
- Bugfix: Pasting into new game asserts
- Bugfix: Progress not indicated for searching operations
- Bugfix: Rendering board would occasionally draw offset twice
- Bugfix: Qt4 compatibility
- Changed: Skip empty parts when copying images
- Changed: Always draw square around board
- Changed: Training with Options
- Changed: Force writing variations from Engine even if no move exists
- Changed: Set Result at end of game against engine
- Changed: Do not elide tab text of boards
- Changed: Reduce memory needed for RW-Databases

## May 2015: Version 1.3.2

- New feature: Variation indicator in toolbox
- New feature: FICS - Toggle board at beginning of puzzle
- New feature: Toolbox for adding Nags (e.g. on WhiteBoards)
- New feature: FICS - Filter computers and unrated users
- New feature: Show date of last access to a database
- New feature: Pin Opening Tree at a position
- New feature: Clear filter before starting search from opening tree
- New feature: Select board for searching position from open boards
- New feature: Show threat after making move
- Bugfix: Index file usage broken
- Bugfix: Missing newline copying games into a closed file
- Bugfix: Better status message when copying multiple games
- Bugfix: Gamelist header configuration does not persist
- Bugfix: Windows Installation does not show Pin Icon

## April 2015: Version 1.3.0

- New Feature: Play games on FICS with telnet or timeseal
- New Feature: Boards now 1:1 with open databases
- New Feature: Remove found items from filter
- New feature: Flip board to side to move while loading game in training
- New feature: Setup Board Dialog with Coordinates
- New feature: #137 Extend Opening tree (Back button)
- New feature: Undo last move in Opening Tree
- New feature: Player Info filters games also for specific color
- New feature: Player Info shows regularly played openings
- New feature: Automatic analysis backward from end of game
- New feature: Refactoring from context menu of game text
- New feature: PlayerInfo with performance per eco code
- New feature: Hide selected game from game list
- New feature: Allow editing arbitrary tags
- New feature: Draw annotations whith Shit+Left-Click with last color used
- New feature: Saving w/o dialog
- New feature: Material profile
- New feature: New board "lanta"
- New feature: Allow filtering of non-active DB from OpeningTree
- New feature: Send PGN by mail
- New feature: load project databases from Help menu
- Bugfix: Autoanalysis runs over remaining game after adding variation
- Bugfix: Annotation menu seems to contain duplicate entry
- Bugfix: Do not limit num pv in analysis
- Bugfix: Pressing Shift while dragging piece creates illegal baord view
- Bugfix: Guess generation crashes when given an illegal position
- Bugfix: In auto responder mode, arrows where painted too late
- Bugfix: Dialogs non-modal
- Bugfix: Loading translations failed
- Bugfix: Open file with relative path from commandline

## October 2014: Version 1.2.2

- Bugfix: Entering a rook move or 0-0 did not work via keyboard
- Bugfix: Plycount wrong saving a game from the non-current database
- Bugfix: Crash saving a database upon closing chessx if database was not current DB
- Bugfix: Enter-Key did not terminate entering a nag
- Bugfix: Occasionally random move would get highlighted
- Bugfix: Searching for Rounds and Moves did not work
- Bugfix: Searching for ranges did not include limits
- Bugfix: UCI Options containing a '.' were parsed incorrectly
- Bugfix: Entering half-ply count was limited to 99 moves (thx to Niklas)
- Bugfix: Retracing a game would fail at move one
- Bugfix: Retracing a game beyond a move with variations would leave invisible variations
- Bugfix: Pasting PGN into new game did not set modified flag
- Bugfix: Pasting FEN into new game did not create new game
- Bugfix: Changes to square and arrow annotations in annotation dialog are not painted immediately
- New feature: CR#141 Edit game details
- New feature: CR#140 Remember maximize state (and full-screen state)
- New feature: Arrow as current move hint
- New feature: Menu entry to clear clipboard database
- New feature: Sorting of game list by clicking header (for databases<=4K)
- New feature: Multi-Selection in the game list
- Enhanced: Speed up polyglot book building
- Changed: F3/F4 now load previous / next game

## July 2014: Version 1.2.0

- New Feature: Use polyglot books in Opening Tree
- New Feature: Build polyglot books from Databaselist
- New Feature: Enter games via keyboard
- New Feature: Undo and Redo of game editing actions
- New Feature: Support for external tools
- New Feature: Paste variation from clipboard into game
- New Feature: Danish Translation (thanks to Byrial Jensen)
- New Feature: Allow removing found items from filter
- New feature: Show title of game in Copy Game Dialog
- New feature: Opening names in ECO Widget
- New feature: Make position searches more comfortable
- New feature: CR#206 Remember settings for more windows.
- New feature: CR#203 Center the board inside its container
- New feature: CR#118 Gameview: Make tags clickable
- New feature: Auto analysis writes engine's name into final annotation
- New feature: Asynchronous Searching
- New feature: Allow several unsaved games at the same time (one per db)
- New feature: Allow disabling menu icons
- New feature: Allow constant move indicator
- New feature: #117 Export screenshot to file
- New feature: Filter in ECO widgets support regular expression
- New feature: Checkbox in opening tree to add moves to game too
- Changed: Pinned games have infinite analysis depth
- Changed: Dialogs remember sizes and screen positions
- Changed: CR#127 New navigation buttons order.
- Changed: board tabs carry name of db instead of numbers
- Changed: Auto analysis won't add move at end of game
- Changed: Assignment of CTRL+C key to direct pasting of PGN
- Changed: #121 Thicker, more visible arrows and colored squares
- Changed: Show game title now in three lines
- Changed: Copy of a game via context menu copies selected game
- Changed: ECO Widget shows names of openings
- Bugfix: Nags with bishops now exclusive
- Bugfix: CR#205 line does not resume from the same indentation.
- Bugfix: CR#204 Database is not released from memory upon closing
- Bugfix: Annotations were not handled correctly when compacting database
- Bugfix: Analysis/"Mate in" has inconsistent color
- Bugfix: EGT Time annotation broken
- Bugfix: Do not send ucinewgame after making a move

## February 2014: Version 1.1.0

- New feature: Traning with Toolbar Icon
- New feature: Play out game against engine
- New feature: Limit time that engines may use for analysis
- New feature: Auto Responder for training openings etc.
- New feature: Show name of opening in addition to ECO-Code
- New feature: Use any open database as source for Opening Tree
- New feature: Opening Tree with Filter as Source
- New feature: Splitter in Detail Views
- New Feature: CR#6 Configurable shortcuts
- New feature: Visual indicator for results
- New feature: Show location of AppSettings in About Dlg
- New feature: CR#108 Lock engine to current position
- New feature: Null moves at unusual positions
- New feature: Incremental Opening Tree
- New feature: Adjust columns in list views
- New feature: Tabs switch if something is dragged to them (CR#112)
- New feature: Hilite new favorite item
- New feature: Select font for ChessBrowser from Preferences
- New feature: Additional link targets in info widgets
- New feature: PromotionDialog as suggested in CR#113 Pieces in promotion dialog
- New feature: Added possibility to force loading english lang in German and French
- Bugfix: #193 wrong nag $1-$6
- Bugfix: #191 Cannot edit an annotation-only comment
- Bugfix: Time malformed
- Bugfix: #189 Missing PNG for new board button
- Bugfix: Searching for result 1-0 was not possible
- Bugfix: Filtering if filter widget was changed from another widget
- Bugfix: CR#105 few remarks - issue 1 - coherence with icons
- Bugfix: CR#187 comments before first move are not read
- Bugfix: HTML-Output in some special cases wrong
- Bugfix: Worked around potential leak with smart pointer
- Bugfix: Issue closing chessx while loading a database
- Bugfix: Some columns won't display their content if the local setup is old enough
- Bugfix: #180 - part 3 (user is asked several times to load database)
- Bugfix: Nullmove issue
- Bugfix: #180: Load Order of DLLs (did not start on Windows)
- Bugfix: Better output of end of variations
- Bugfix: Workaround for Apache Issue: #183 Download of PGN databases fails because of blocked user-agent
- Bugfix: Guessengine crashes when accessing uninitialized memory
- Bugfix: CR#182 Minimize - restore = tab switch bug
- Bugfix: CR#181 typos
- Bugfix: CR#178 Deleting a variation does not clear the annotation
- Bugfix: Unterverwandlung 
- Fixed: textbrowserex.h case sensitivity (did not compile on Linux)
- Fixed: a static leak (thanks to Albert for mentioning this)
- Fixed: portability issue #185 Build error in enginelist.cpp for kFreeBSD
- Changed: Flip Icon consistent with main GUI
- Changed: Slider 'Move Interval' moved from Preferences to StatusBar

## September 2013: Version 1.0.0

- New feature: Get translation files from server
- New feature: Copy Text or HTML from Notation
- New feature: Offer to load a default database for first time users
- New feature: CR#10 Configurable Game View - customize appearance of notation
- New feature: Inserting moves (in addition to adding moves as variation
- New Feature: CR#86 Merge games with Drag&Drop
- New Feature: CR#46 Games whose position is [end] selectable
- New Feature: CR#43 Frequency and score ~0% instead of 0% when not exactly 0
- New feature: Setup Dialog has new functions to mirror board or colors.
- New feature: Select Language from Preferences (requires restart)
- New feature: Online check for updates
- New Feature: Column date of last modification
- New Feature: Localized Piece Initials (see CR#3592733 Output / Input PGN
- New feature: Create a Diagram Nag (201 like in SCID) from GUI
- New Feature: Draw Board Coordinates (see CR#1715710	Numbers and ...)
- New Feature: HTML Output with readable basic tags instead of PGN look
- New feature: Diagrams in HTML Export and Notation
- New feature: HTML Output with symbolic nags in HTML/UTF8
- New features: Navigation within siblings of variations with keyboard
- New feature: Enter or Exit Variation now moves within mainline
- New feature: Toolbar for commonly used actions
- New Feature: ECO based Opening Overview of the current database
- New Feature: MDI-Interface
- New feature: Opening tree has its own board view.
- New Feature: CR#3607996 Always on top feature
- Bugfix: ChessBrowser does not indicate '?'-tags
- Bugfix: HTML-Output contained one HTML-Header for each game
- Bugfix: Sorting by size was not ideal
- Bugfix: CR#171 - Board Setup dialog: wrong board size
- Bugfix: Removing last arrow annotation leaves ugly comment
- Bugfix: Stay On Top accidentally active in OS which were Non-Windows-OS
- Bugfix: Concurrent loading of games (fix from James Coons)
- Bugfix: Game List was not updated after bulk copy of games
- Bugfix: Black ELo was only saved if a white ELO was given
- Bugfix: CR#3612157 tablebase
- Bugfix: Performance Boost for filtering with result-cache
- Bugfix: CR#3611434 Local vs global menu inconsistency
- Bugfix: Update Widgets after saving a database
- Bugfix: Created PGN was invalid due to invalid date format
- Bugfix: Opening new game did not clear selection on game list

## March 2013 Version 0.9.6

- Bugfix / Feature removal: Filtering opening tree was somewhat active
- Bugfix: (thx to James!) Promoting into different pieces cannot start
- Bugfix: Before closing a database ask to save a new or modified game
- Bugfix: CR#3596577 - Game modifications lost when changing a game
- Bugfix: Ctrl+Drag in BoardSetup-Dialog loses a button
- Bugfix: DatabaseList view shows Linked local files with annoying scheme
- Bugfix: Drawing board when resizing off by 1
- Bugfix: Enable Escape-Key for Dialogs
- Bugfix: Flickering with OpeningTree open
- Bugfix: New Game did not clear the tags from the last game
- Bugfix: Openening ECO files missed some checks
- Bugfix: Opening ZIP or URL from Commandline or event loop was missing
- Bugfix: Parsing half move count in FEN (Francis Monkman)
- Bugfix: Removing a variation removed the wrong variation
- Bugfix: Saving a database did not reset the modified flag resulting in
- Bugfix: Selected wrong game when deleting games from Database
- New Feature: Accept FENs which do not have a valid EnPassant Encoding
- New Feature: AutoPlayer
- New Feature: Continuous Blunder Checking
- New Feature: Drag Drop complete db onto other db
- New Feature: Player List with linked ECO filtering
- New Feature: PlayerList gives hints on openings played
- New Feature: Sorting the list of databases with header click
- New Feature: Support Team Tags when saving games
- New feature: Accept FEN with invalid movenumber (James Coons suggestion)
- New feature: Reload last game at open of database
- New feature: Mass changing tags in database (Events, Player Names)
- New feature: Shift+Rightclick on Boardview allows editing of colored
- New feature: Training mode
- New feature: Numbering variations
- New feature: Refactor complete game (uncomment / remove all variations)
- New feature: Saving a game can now be either cancelled (the new option)
- New feature: Edit square colors like in chessbase with Shift+Right-
- New feature: Delete games from a database
- New feature: Auto Analysis with Engine 1 of one or more games
- New feature: Clear cache between database changes
- New feature: Reorder variations - CR#3558001
- New feature: Force adding a move as variation by pressing ALT while
- New feature: Copy Fen in human readable form - CR#3580030
- New feature: DatabaseList double click opens file
- New feature: Highlight last move done when scrolling through a game

## December 2012 Version 0.9.4

- Bugfix: Writing of files automatically made use of UTF8 instead of ANSI
- Bugfix: Crash at loading PolyGlot 1.4
- Bugfix: Comment at start of empty game
- Bugfix: Databases larger than 4G
- Bugfix: Databases with UNIX-LF
- Bugfix: Some nags were not properly accessible from keyboard
- Bugfix: No keyboard nags for first white move
- New Feature: Time Editor for elapsed game time
- New Feature: CR#3580029 Add only first move from engine analysis
- New feature: Close database from the database list with context menu
- New feature: Toggle UTF8 flag in database list
- New feature: Open compressed archives (from HTTP or local file)
- New feature: Open a database from a URL (HTTP)
- New Feature: Clear Nags from keyboard with *Del
- New Feature: Create Index File thus accelerating re-loading of large PGN enormously
- New Feature: Searching of ranges, e.g. ELO "2000-2300"
- New Feature: Automatic classification with ECO of unclassified / new games

## November 2012 Version 0.9.2

- Bugfix: Support engines which send multiple spaces
- Bugfix: Close Database while updateing opening tree
- Bugfix: Filtering for number of moves fails if an empty game is in the database
- Bugfix: Not well behaving Xboard engines crash ChessX
- Bugfix: Search of Board crashes when started together with OpeninTree Update
- Bugfix: Press CTRL+drag to irritate the guessMove implementation
- New Feature: Move Indicator adjusts itself in size (James Coons)
- New Feature: Common Font Size for all widgets except game view
- New Feature: Separate handling of UTF8 files
- New feature: List views can be customized (right click title line)
- New feature: UCI Option Parser
- New Feature: CTRL+drag, CTRL-Shift-Drag replaces or inserts a move
- New Feature: F1 starts Help View and some help contents are already there
- New Feature: Games with errors insided are marked red in game list
- New feature: Added Stockfish as internal Engine in case no engine is configured
- New board themes Brazilwood and Maple
- New Icons
- Usability: Filterflag only influences gamelist

## September 2012 Version 0.9

-  New Features
- Player List as Dock Window
- New Database List
- Drag and Drop of games
- Drag and Drop of Databases
- Second analysis window
- Locate Databases on harddisk in Explorer/Finder
- Multi-Threading Support for opening DB and updating Opening Tree
- Copy / Paste of PGN
- Create Images of Boards
- New Board Styles
- All Resources internal
- Parsing PGN to draw arrows on board (cf. chessbase)
- Parsing PGN to color Squares (cf. chessbase)
- Parse / Insert Zero Moves to document / analyze threats
- Drag'n'Drop Support setting up a board
- Relaxed FEN handling for easier pasting of FENs
- Version Resource in Windows
- Icon Resource in Windows
- UTF8 Support
- bugfixes in Parser
- Fixed some strange PGN parsing issues - especially issues like the one described in #3396532
- bugfixes in GUI
- It is not possible to add a variation from Analysis to a game text if standing at end of a variation
- Bugfixes starting up engines
- Disabled MultiPV on XBoard engines
- Added elapsed time for analysis lines

**issues closed:**

- CR#2984375	Saving Clipboard fails silently
- CR#3043278	Problem with LaTeX output
- CR#3135871	No control performed on input date
- CR#3135871	No control performed on input date
- CR#2872974	Remove->Previous moves should be smarter
- CR#3542101	Changing and saving a file does not update the game list view
- CR#2881228 Crash on File->Close (or Quit) with OpeningTree updating
- CR#3297478 Crash on move gxh2+
- CR#2823746 Crash when trying to analyze with non-existing New Engine
- CR#2881228 Crash on File->Close (or Quit) with OpeningTree updating
- CR#3170534 Crash if start of engine fails (0.8)
- CR#3179203 Do not crash trying to show a special NAG; this should solve bug .
- CR#2824024 Confusion between slash and backslash in file paths
- CR#1773747 "Change guessed move on right click". The feature can be configured, as currently we have "take back a move" om mouse right click (if configured, CTRL+right-mouse will move back instead)
- CR#1651405 "Comments in UTF-8 are broken"
- CR#1715720
- CR#1984912 Names in GameList vs QuickTagSearch
- CR#3043278 Problem with LaTeX output
- CR#3396532 errors reading multi line pgn formats
- CR#2955889.

**Building**

- Separate Debug/Release/Static folders

## November 20120 Version 0.8

-  bug fixes: engine stuff
-  feature:Add engine variation to the game
-  feature:Implement MultiPV analysis for UCI engines (based on patch by Juan Carlos Girardi)
-  feature:Add basic NAG editing
-  feature:Simplify adding moves: always add variation if move exists.
-  feature:Add menu command to remove/promote variation.
-  feature:Show TableBase result in Analysis window.

## Wed 10 Feb 2010, 01:40:00 CET Version 0.7

-  bugfix: do not crash when removing engines
-  bugfix: no problems with more than 10 engines
-  bugfix: do not stop running engine on minimize/hide
-  usability: browse for executable when adding new engines
-  bugfix: correctly display NAGs below the board
-  bugfix: correctly truncate previous moves
-  feature: global shortcut F2 to start/stop engine
-  change: speedup by canceling previous OpeningTree search before the next
-  bugfix: ignore clicks on illegal OpeningTree moves like [end]
-  feature: make editable PGN size limit configurable
-  feature: show progress bar when opening/saving files
-  feature: show progress bar for OpeningTree
-  feature: promote variation

## Wed 1 Jul 2009, 00:50:22 CET Version 0.6

-  feature: warn when quitting with unsaved databases
-  feature: improve Engine tab of Preferences dialog, add reordering
-  feature: remember current engine
-  bugfix: show correct score for Crafty
-  bugfix: correctly display variations leading to mate in UCI engines
-  bugfix: update ECO database for opening detection and move guesser
-  bugfix: improve focus handling in game list and game text
-  feature: timer-delayed game loading when pressing Ctrl+Down/Up
-  bugfix: remove bold column names in TableView
-  bugfix: support NAGs not preceeded with space in PGN parser
-  bugfix: make PGN parser more robust
-  bugfix: optimize PGN output
-  bugfix: enable leading comment in mainline and variations
-  bugfix: slightly improve PGN export formatting
-  feature: allow creating new PGN databases
-  change: drop CXD databases support
-  bugfix: guess move only with mouse over the board
-  feature: truncate from the beginning
-  bugfix: correct tag order for QuickSearch
-  feature: search games by number or number range
-  bugfix: do not read a game beyong the end of file
-  feature: allow editing pre-move comments
-  bugfix: PGN standard compliant ep square in FEN
-  bugfix: reparent some dialogs
-  bugfix: remove flickering at startup
-  feature: make comments editable
-  feature: correctly read PGNs with FEN
-  feature: new Leipzig theme
-  feature: allow removing variations
-  feature: enable HTML export
-  feature: allow setting small fonts in chess text widgets (useful for laptops).
-  bugfix: save all PGN tags
-  bugfix: display correct move number in starting position
-  feature: allow copying games between databases
-  feature: allow copying pieces in BoardSetup by using Ctrl
-  change: add Advanced setting to BoardSetup
-  feature: PlayerInfo dialog with player statistics
-  feature: display FEN in BoardSetup
-  feature: copy/paste position in BoardSetup
-  bugfix: correctly recognize promotions in UCI engines
-  bugfix: correctly handle promotion in tablebases
-  bugfix: support 1/2 as a draw symbol

## Wed 13 Jun 2007 19:47:32 CET	Version 0.5

-  feature: engine analysis
-  feature: Opening Tree
-  feature: Setup Position dialog enhancements
-  feature: use ECO information to guess moves
-  feature: option to combine searches
-  feature: enable translations
-  feature: make frame color configurable
-  feature: remove last move with right click
-  feature: remember matching position in position search
-  feature: improve layout of Game Info
-  feature: display game length in Game List
-  feature: remove last move with right-click
-  feature: shortcuts for Game List and Game Text
-  bugfix: fix move loading failure in large databases
-  bugfix: open PGN files from command line
-  bugfix: don't flood tablebase servers with too many requests
-  bugfix: sometimes online tablebase wasn't used even if enabled
-  bugfix: flip setup board if main board is flipped
-  bugfix: various small fixes to move guesser
-  bugfix: some errors in Game/tags handling
-  bugfix: do not crash when loading random game from empty filter
-  change: lower network usage for EGTB server
-  change: new, much faster Board implementation
-  change: much faster Tag search
-  change: do not highlight first move in starting position
-  change: redesign Board tab in Preferences dialog

## Wed 9 May 2007 18:59:47 CET	Version 0.4

-  feature: new, much improved themes
-  feature: move guesser
-  feature: support for online tablebases
-  feature: binary ECO file format (faster startup)
-  feature: simple tag search dialog for single tag searches
-  feature: improve look&feel and usability of Tip of the day
-  feature: Apply button for Preferences dialog
-  feature: configurable square highlight color
-  bugfix: update theme when pieces are changed
-  bugfix: fix some drag&drop problems
-  bugfix: segfault when truncating moves from empty game
-  bugfix: ask for promoted piece only for legal moves
-  bugfix: allow variations in starting position
-  bugfix: correctly redraw board frame
-  bugfix: correctly show side to move in Setup Position dialog
-  bugfix: fix rare bug when # symbol was displayed instead of +
-  bugfix: correctly calculate hash value for en passant
-  bugfix: re-enable FEN tag support
-  change: optimize PGN loading (loads 10-20% faster)

## Mon 23 Apr 2007 18:59:47 CET	Version 0.3

-  feature: display ECO information
-  feature: move pieces by drag&drop
-  feature: remove moves/variations and truncate moves
-  feature: display side to move
-  feature: position search
-  feature: internal support for editable PGN databases (no GUI yet!)
-  feature: export games in filter/all games to PGN
-  feature: small sample player database (look for Adams)
-  bugfix: fix incorrect SAN representation of en-passant moves
-  bugfix: correctly calculate game length displayed in GameInfo
-  bugfix: display empty game
-  bugfix: more robust PGN parsing
-  bugfix: Sun-Solaris compilation (patch by Atul)
-  bugfix: some improvements to Windows installer
-  bugfix: more usual PGN format (space after move number etc.)

## Thu 28 Feb 2007 00:59:47 CET	Version 0.2

-  feature: highlight and display current move
-  feature: add variations
-  feature: support promotion when entering moves
-  feature: redesign Setup Position to improve usability
-  feature: check board validity in Setup position dialog
-  bugfix: correctly display move numbers for non-standard games
-  bugfix: keep move number and side to move when setting position
-  bugfix: copy/paste move number to/from FEN
-  bugfix: accept non-standard castling (0-0 instead of O-O)
-  bugfix: correctly draw large boards in unscaled pixmap mode
-  bugfix: insert all pieces in given color in Setup Position
-  bugfix: fix segfault when removing piece from empty square
-  bugfix: correctly finding installed data path on Debian.
-  change: clean configuration file format
