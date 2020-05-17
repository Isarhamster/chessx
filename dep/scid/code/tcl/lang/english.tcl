

# Text for menu names and status bar help messages in English.
# Part of Scid (Shane's Chess Information Database).
#
# Instructions for adding a new language:
#
# (1) Choose a letter code for the language. Currently assigned codes are:
#      E=English, D=Deutsch, F=Francais, S=Spanish, B=Brasil Portuguese,
#      P=Polish, N=Nederlands, W=Swedish, O=Norsk, C=Czech, H=Hungarian,
#      Y=Serbian.
#
# (2) Edit the code below that has the lines "addLanguage ..." and add your
#     new language. The final digit in each line is the index of the
#     letter to underline in the menu, counting from 0 as the first letter.
#
# (3) Copy the section of English menu and help message details below (all
#     the commands that start "menuText E ..." or "helpMsg E ..." or
#     "translate E ...") and change all the "E" letters to the letter you
#     assigned for the new language.
#
# (4) Write the translations. This involves changing anything "in quotes"
#     or {in braces} for the menuText, helpMsg and translate commands.
#
#     A menu command has the format:
#         menuText L tag "Name..." underline {HelpMessage...}
#
#     A status bar help message for a button has the format:
#         helpMsg L tag {HelpMessage...}
#
#     A general word or message translation has the format:
#         translate L tag {message...}
#
#     where "L" is the language letter, "tag" is the name of the menu entry
#     or the button widget name, and "underline" is the index of the letter
#     to underline, counting from zero as the first letter. Two menu entries
#     that appear in the same menu should have a different underlined letter.
#     If in doubt, just make them all "0" and I'll assign sensible underlined
#     letters when your translations are incorporated into Scid.
#
# Additions, corrections? Email: sgh@users.sourceforge.net

proc setLanguage_E {} {

# File menu:
menuText E File "File" 0
menuText E FileNew "New..." 0 {Create a new Scid database}
menuText E FileOpen "Open..." 0 {Open an existing Scid database}
menuText E FileClose "Close" 0 {Close the active Scid database}
menuText E FileFinder "Finder" 0 {Open the File Finder window}
menuText E FileBookmarks "Bookmarks" 0 {Bookmarks menu}
menuText E FileBookmarksAdd "Add Bookmark" 0 \
  {Bookmark the current database game and position}
menuText E FileBookmarksFile "File Bookmark" 0 \
  {File a bookmark for the current game and position}
menuText E FileBookmarksEdit "Edit Bookmarks..." 0 \
  {Edit the bookmarks menus}
menuText E FileBookmarksList "Display Folders as Single List" 0 \
  {Display bookmark folders as a single list, not submenus}
menuText E FileBookmarksSub "Display Folders as Submenus" 0 \
  {Display bookmark folders as submenus, not a single list}
menuText E FileMaint "Maintenance" 0 {Scid database maintenance tools}
menuText E FileMaintWin "Maintenance Window" 0 \
  {Open/close the Scid database maintenance window}
menuText E FileMaintCompact "Compact Database..." 0 \
  {Compact database files, removing deleted games and unused names}
menuText E FileMaintClass "ECO-Classify Games..." 2 \
  {Recompute the ECO code of all games}
menuText E FileMaintSort "Sort Database..." 0 \
  {Sort all games in the database}
menuText E FileMaintDelete "Delete Twin Games..." 0 \
  {Find twin games and set them to be deleted}
menuText E FileMaintTwin "Twin Checker Window" 0 \
  {Open/update the Twin checker window}
menuText E FileMaintName "Name Spelling" 0 {Name editing and spelling tools}
menuText E FileMaintNameEditor "Name Editor" 0 \
  {Open/close the name editor window}
menuText E FileMaintNamePlayer "Spellcheck Player Names..." 11 \
  {Spellcheck player names using the spellcheck file}
menuText E FileMaintNameEvent "Spellcheck Event Names..." 11 \
  {Spellcheck event names using the spellcheck file}
menuText E FileMaintNameSite "Spellcheck Site Names..." 11 \
  {Spellcheck site names using the spellcheck file}
menuText E FileMaintNameRound "Spellcheck Round Names..." 11 \
  {Spellcheck round names using the spellcheck file}
menuText E FileReadOnly "Read-only..." 0 \
  {Treat the current database as read-only, preventing changes}
menuText E FileSwitch "Switch to Database" 0 \
  {Switch to a different opened database}
menuText E FileExit "Exit" 1 {Exit Scid}
menuText E FileMaintFixBase "Repair base" 0 {Try to repair a corrupted base}
  
# Edit menu:
menuText E Edit "Edit" 0
menuText E EditAdd "Add Variation" 0 {Add a variation at this move in the game}
menuText E EditDelete "Delete Variation" 0 {Delete a variation for this move}
menuText E EditFirst "Make First Variation" 5 \
  {Promote a variation to be first in the list}
menuText E EditMain "Promote Variation to Main Line" 21 \
  {Promote a variation to be the main line}
menuText E EditTrial "Try Variation" 0 \
  {Start/stop trial mode, for testing an idea on the board}
menuText E EditStrip "Strip" 2 {Strip comments or variations from this game}
menuText E EditUndo "Undo" 0 {Undo last game change}
menuText E EditRedo "Redo" 0 {Redo last game change}
menuText E EditStripComments "Comments" 0 \
  {Strip all comments and annotations from this game}
menuText E EditStripVars "Variations" 0 {Strip all variations from this game}
menuText E EditStripBegin "Moves from the beginning" 1 \
  {Strip moves from the beginning of the game}
menuText E EditStripEnd "Moves to the end" 0 \
  {Strip moves to the end of the game}
menuText E EditReset "Empty Clipbase" 0 \
  {Reset the clipbase to be completely empty}
menuText E EditCopy "Copy This Game to Clipbase" 0 \
  {Copy this game to the Clipbase database}
menuText E EditPaste "Paste Last Clipbase Game" 0 \
  {Paste the active Clipbase game here}
menuText E EditPastePGN "Paste Clipboard text as PGN game..." 18 \
  {Interpret the clipboard text as a game in PGN notation and paste it here}
menuText E EditSetup "Set Up Start Board..." 0 \
  {Set the start position for this game}
menuText E EditCopyBoard "Copy Position" 6 \
  {Copy the current board in FEN notation to the text selection (clipboard)}
menuText E EditPasteBoard "Paste Start Board" 12 \
  {Set the start board from the current text selection (clipboard)}
menuText E ConfigureScid "Preferences..." 0 {Configure all options for SCID}

# Game menu:
menuText E Game "Game" 0
menuText E GameNew "New Game" 0 {Reset to an empty game}
menuText E GameFirst "Load First Game" 5 {Load the first filtered game}
menuText E GamePrev "Load Previous Game" 5 {Load the previous filtered game}
menuText E GameReload "Re-Load Current Game" 3 \
  {Reload this game, discarding any changes made}
menuText E GameNext "Load Next Game" 7 {Load the next filtered game}
menuText E GameLast "Load Last Game" 8 {Load the last filtered game}
menuText E GameRandom "Load Random Game" 8 {Load a random filtered game}
menuText E GameNumber "Load Game Number..." 5 \
  {Load a game by entering its number}
menuText E GameReplace "Save: Replace game..." 6 \
  {Save this game, replacing the old version}
menuText E GameAdd "Save: Add New Game..." 6 \
  {Save this game as a new game in the database}
menuText E GameDeepest "Identify Opening" 0 \
  {Goto the deepest game position listed in the ECO book}
menuText E GameGotoMove "Goto Move Number..." 5 \
  {Go to a specified move number in the current game}
menuText E GameNovelty "Find Novelty..." 7 \
  {Find the first move of this game that has not played before}

# Search Menu:
menuText E Search "Search" 0
menuText E SearchReset "Reset Filter" 0 {Reset the filter so all games are included}
menuText E SearchNegate "Negate Filter" 0 {Negate the filter to only include excluded games}
menuText E SearchCurrent "Current Board..." 0 {Search for the current board position}
menuText E SearchHeader "Header..." 0 {Search by Header (player, event, etc) information}
menuText E SearchMaterial "Material/Pattern..." 0 {Search for material or board patterns}
menuText E SearchUsing "Using Search File..." 0 {Search using a SearchOptions file}

# Windows menu:
menuText E Windows "Windows" 0
menuText E WindowsComment "Comment Editor" 0 {Open/close the comment editor}
menuText E WindowsGList "Game List" 0 {Open/close the game list window}
menuText E WindowsPGN "PGN Window" 0 \
  {Open/close the PGN (game notation) window}
menuText E WindowsPList "Player Finder" 2 {Open/close the player finder}
menuText E WindowsTmt "Tournament Finder" 2 {Open/close the tournament finder}
menuText E WindowsSwitcher "Database switcher" 0 \
  {Open/close the Database Switcher window}
menuText E WindowsMaint "Maintenance Window" 0 \
  {Open/close the Maintenance window}
menuText E WindowsECO "ECO Browser" 0 {Open/close the ECO Browser window}
menuText E WindowsStats "Statistics Window" 0 \
  {Open/close the filter statistics window}
menuText E WindowsTree "Tree Window" 0 {Open/close the Tree window}
menuText E WindowsTB "Endgame Tablebase Window" 1 {Open/close the Tablebase window}
menuText E WindowsBook "Book Window" 0 {Open/close the Book window}
menuText E WindowsCorrChess "Correspondence Window" 0 {Open/close the Correspondence window}

# Tools menu:
menuText E Tools "Tools" 0
menuText E ToolsAnalysis "Analysis Engine..." 0 \
  {Start/stop a chess analysis engine}
menuText E ToolsAnalysis2 "Analysis Engine #2..." 17 \
  {Start/stop the 2nd chess analysis engine}
menuText E ToolsCross "Crosstable" 0 {Show tournament crosstable for this game}
menuText E ToolsEmail "Email Manager" 0 {Open/close the email chess manager window}
menuText E ToolsFilterGraph "Rel. Filter Graph" 12 {Open/close the filter graph window for relative values}
menuText E ToolsAbsFilterGraph "Abs. Filter Graph" 7 {Open/close the filter graph window for absolute values}
menuText E ToolsOpReport "Opening Report" 0 {Generate an opening report for the current position}
menuText E ToolsOpenBaseAsTree "Open base as tree..." 0   {Open a base and use it in Tree window}
menuText E ToolsOpenRecentBaseAsTree "Open recent base as tree" 0   {Open a recent base and use it in Tree window}
menuText E ToolsTracker "Piece Tracker"  6 {Open the Piece Tracker window}
menuText E ToolsTraining "Training"  0 {Training tools (tactics, openings,...) }
menuText E ToolsTacticalGame "Tactical game"  0 {Play a game with tactics}
menuText E ToolsSeriousGame "Serious game"  0 {Play a serious game}
menuText E ToolsTrainOpenings "Openings"  0 {Train with a repertoire}
menuText E ToolsTrainReviewGame "Review game"  0 {Guess moves played in a game}
menuText E ToolsTrainTactics "Tactics"  0 {Solve tactics}
menuText E ToolsTrainCalvar "Calculation of variations"  0 {Calculation of variations training}
menuText E ToolsTrainFindBestMove "Find best move"  0 {Find best move}
menuText E ToolsTrainFics "Play on FICS"  0 {Play on freechess.org}
menuText E ToolsEngineTournament "Engine tournament"  0 {Start a tournament between chess engines}
menuText E ToolsBookTuning "Book tuning" 0 {Book tuning}
menuText E ToolsConnectHardware "Connect Hardware" 8 {Connect external hardware}
menuText E ToolsConnectHardwareConfigure "Configure..." 0 {Configure external hardware and connection}
menuText E ToolsConnectHardwareNovagCitrineConnect "Connect Novag Citrine" 8 {Connect Novag Citrine with Scid}
menuText E ToolsConnectHardwareInputEngineConnect "Connect Input Engine" 8 {Connect Input Engine (e.g. DGT board) with Scid}

menuText E ToolsPInfo "Player Information"  0 \
  {Open/update the Player Information window}
menuText E ToolsPlayerReport "Player Report..." 3 \
  {Generate a player report}
menuText E ToolsRating "Rating Graph" 0 \
  {Graph the rating history of the current game players}
menuText E ToolsScore "Score Graph" 0 {Show the score graph window}
menuText E ToolsExpCurrent "Export Current Game" 8 \
  {Write current game to a text file}
menuText E ToolsExpCurrentPGN "Export Game to PGN File..." 15 \
  {Write current game to a PGN file}
menuText E ToolsExpCurrentHTML "Export Game to HTML File..." 15 \
  {Write current game to a HTML file}
menuText E ToolsExpCurrentHTMLJS "Export Game to HTML and JavaScript File..." 15 {Write current game to a HTML and JavaScript file}  
menuText E ToolsExpCurrentLaTeX "Export Game to LaTeX File..." 15 \
  {Write current game to a LaTeX file}
menuText E ToolsExpFilter "Export All Filter Games" 1 \
  {Write all filtered games to a text file}
menuText E ToolsExpFilterPGN "Export Filter to PGN File..." 17 \
  {Write all filtered games to a PGN file}
menuText E ToolsExpFilterHTML "Export Filter to HTML File..." 17 \
  {Write all filtered games to a HTML file}
menuText E ToolsExpFilterHTMLJS "Export Filter to HTML and JavaScript File..." 17 {Write all filtered games to a HTML and JavaScript file}  
menuText E ToolsExpFilterLaTeX "Export Filter to LaTeX File..." 17 \
  {Write all filtered games to a LaTeX file}
menuText E ToolsImportOne "Import One PGN Game..." 0 \
  {Import a game from PGN text}
menuText E ToolsImportFile "Import File(s) of PGN Games..." 7 {Import games from PGN file(s)}
menuText E ToolsStartEngine1 "Start engine 1" 13  {Start engine 1}
menuText E ToolsStartEngine2 "Start engine 2" 13  {Start engine 2}
menuText E ToolsCaptureBoard "Capture Current Board..." 5  {Save the current board as an image.}

# Play menu
menuText E Play "Play" 0

# --- Correspondence Chess
menuText E CorrespondenceChess "Correspondence Chess" 0 {Functions for eMail and Xfcc based correspondence chess}
menuText E CCConfigure "Configure..." 0 {Configure external tools and general setup}
menuText E CCConfigRelay "Observe games..." 10 {Configure games to be observed}
menuText E CCOpenDB "Open Database..." 0 {Open the default Correspondence database}
menuText E CCRetrieve "Retrieve Games" 0 {Retrieve games via external (Xfcc-)helper}
menuText E CCInbox "Process Inbox" 8 {Process all files in scids Inbox}
menuText E CCSend "Send Move" 0 {Send your move via eMail or external (Xfcc-)helper}

menuText E CCResign "Resign" 1 {Resign (not via eMail)}
menuText E CCClaimDraw "Claim Draw" 6 {Send move and claim a draw (not via eMail)}
menuText E CCOfferDraw "Offer Draw" 1 {Send move and offer a draw (not via eMail)}
menuText E CCAcceptDraw "Accept Draw" 0 {Accept a draw offer (not via eMail)}

menuText E CCNewMailGame "New eMail Game..." 2 {Start a new eMail game}
menuText E CCMailMove "Mail Move..." 0 {Send the move via eMail to the opponent}
menuText E CCGamePage "Game Page..." 0 {Call up the game via the web browser}

# menu in cc window:
menuText E CCEditCopy "Copy Gamelist to Clipbase" 0 {Copy the games as CSV list to clipbase}

#  B    GHiJKL    Q  TUV XYZ

# Options menu:
menuText E Options "Options" 0
menuText E OptionsBoardGraphics "Squares..." 0 {Select textures for squares}
translate E OptionsBGW {Select texture for squares}
translate E OptionsBoardGraphicsText {Select graphic files for white and black squares:}
menuText E OptionsBoardNames "My Player Names..." 0 {Edit my player names}
menuText E OptionsExport "Exporting" 0 {Change text export options}
menuText E OptionsFonts "Fonts" 0 {Change fonts}
menuText E OptionsFontsRegular "Regular" 0 {Change the regular font}
menuText E OptionsFontsMenu "Menu" 0 {Change the menu font}
menuText E OptionsFontsSmall "Small" 0 {Change the small font}
menuText E OptionsFontsTiny "Tiny" 0 {Change the tiny font}
menuText E OptionsFontsFixed "Fixed" 0 {Change the fixed-width font}
menuText E OptionsGInfo "Game Information" 0 {Game information options}
menuText E OptionsLanguage "Language" 0 {Select menu language}
menuText E OptionsMovesTranslatePieces "Translate pieces" 0 {Translate first letter of pieces}
menuText E OptionsMovesHighlightLastMove "Highlight last move" 0 {Highlight last move}
menuText E OptionsMovesHighlightLastMoveDisplay "Show" 0 {Display last move Highlight}
menuText E OptionsMovesHighlightLastMoveWidth "Width" 0 {Thickness of line}
menuText E OptionsMovesHighlightLastMoveColor "Color" 0 {Color of line}
menuText E OptionsMovesHighlightLastMoveArrow "include Arrow" 0 {Include Arrow with Highlight}
menuText E OptionsMoves "Moves" 0 {Move entry options}
menuText E OptionsMovesAsk "Ask Before Replacing Moves" 0 \
  {Ask before overwriting any existing moves}
menuText E OptionsMovesAnimate "Animation Time" 1 \
  {Set the amount of time used to animate moves}
menuText E OptionsMovesDelay "Autoplay Time Delay..." 1 \
  {Set the time delay for autoplay mode}
menuText E OptionsMovesCoord "Coordinate Move Entry" 0 \
  {Accept coordinate-style move entry (e.g. "g1f3")}
menuText E OptionsMovesSuggest "Show Suggested Moves" 0 \
  {Turn on/off move suggestion}
menuText E OptionsShowVarPopup "Show variations window" 0 {Turn on/off the display of a variations window}  
menuText E OptionsMovesSpace "Add spaces after move number" 0 {Add spaces after move number}  
menuText E OptionsMovesKey "Keyboard Completion" 0 \
  {Turn on/off keyboard move auto-completion}
menuText E OptionsMovesShowVarArrows "Show Arrows for Variations" 0 {Turn on/off arrows showing moves in variations}
menuText E OptionsMovesGlossOfDanger "Color Coded Gloss of Danger" 0 {Turn on/off color coded gloss of danger}
menuText E OptionsNumbers "Number Format" 0 {Select the number format}
menuText E OptionsStartup "Startup" 3 {Select windows to open at startup}
menuText E OptionsTheme "Theme" 0 {Change look of interface}
menuText E OptionsWindows "Windows" 0 {Window options}
menuText E OptionsWindowsIconify "Auto-Iconify" 5 \
  {Iconify all windows when the main window is iconified}
menuText E OptionsWindowsRaise "Auto-Raise" 5 \
  {Raise certain windows (e.g. progress bars) whenever they are obscured}
menuText E OptionsSounds "Sounds" 2 {Configure move announcement sounds}
menuText E OptionsResources "Resources..." 0 {Choose resource files and folders}
menuText E OptionsWindowsDock "Dock windows" 0 {Dock windows (needs restart)}
menuText E OptionsWindowsSaveLayout "Save layout" 0 {Save layout}
menuText E OptionsWindowsRestoreLayout "Restore layout" 0 {Restore layout}
menuText E OptionsWindowsShowGameInfo "Show game info" 0 {Show game info}
menuText E OptionsWindowsAutoLoadLayout "Auto load first layout" 0 {Auto load first layout at startup}
menuText E OptionsECO "ECO File" 7 {Load the ECO classification file}
menuText E OptionsSpell "Spellcheck File" 11 \
  {Load the Scid spellcheck file}
menuText E OptionsTable "Tablebase Directory" 10 \
  {Select a tablebase file; all tablebases in its directory will be used}
menuText E OptionsRecent "Recent Files" 0 {Change the number of recent files displayed in the File menu}
menuText E OptionsBooksDir "Books directory" 0 {Sets the opening books directory}
menuText E OptionsTacticsBasesDir "Bases directory" 0 {Sets the tactics (training) bases directory}
menuText E OptionsPhotosDir "Photos directory" 0 {Sets the photos bases directory}
menuText E OptionsThemeDir "Theme(s) File"  0 { Load a GUI theme package file }
menuText E OptionsSave "Save Options" 0 "Save all settable options to the file $::optionsFile"
menuText E OptionsAutoSave "Auto-Save Options on Exit" 0 \
  {Auto-save all options when exiting Scid}

# Help menu:
menuText E Help "Help" 0
menuText E HelpContents "Contents" 0 {Show the help contents page}
menuText E HelpIndex "Index" 0 {Show the help index page}
menuText E HelpGuide "Quick Guide" 0 {Show the quick guide help page}
menuText E HelpHints "Hints" 0 {Show the hints help page}
menuText E HelpContact "Contact Info" 1 {Show the contact information help page}
menuText E HelpTip "Tip of the Day" 0 {Show a useful Scid tip}
menuText E HelpStartup "Startup Window" 0 {Show the startup window}
menuText E HelpAbout "About Scid" 0 {Information about Scid}

# Game info box popup menu:
menuText E GInfoHideNext "Hide Next Move" 0
menuText E GInfoMaterial "Show Material Values" 0
menuText E GInfoFEN "Show FEN" 5
menuText E GInfoMarks "Show Colored Squares and Arrows" 5
menuText E GInfoWrap "Wrap Long Lines" 0
menuText E GInfoFullComment "Show Full Comment" 10
menuText E GInfoPhotos "Show Photos" 5
menuText E GInfoTBNothing "Tablebases: Nothing" 12
menuText E GInfoTBResult "Tablebases: Result Only" 12
menuText E GInfoTBAll "Tablebases: Result and Best Moves" 19
menuText E GInfoDelete "(Un)Delete This Game" 4
menuText E GInfoMark "(Un)Mark This Game" 4
menuText E GInfoInformant "Configure informant values" 0

# General buttons:
translate E Back {Back}
translate E Browse {Browse}
translate E Cancel {Cancel}
translate E Continue {Continue}
translate E Clear {Clear}
translate E Close {Close}
translate E Contents {Contents}
translate E Defaults {Defaults}
translate E Delete {Delete}
translate E Graph {Graph}
translate E Help {Help}
translate E Import {Import}
translate E Index {Index}
translate E LoadGame {Load Game}
translate E BrowseGame {Browse Game}
translate E MergeGame {Merge Game}
translate E MergeGames {Merge Games}
translate E Preview {Preview}
translate E Revert {Revert}
translate E Save {Save}
translate E Search {Search}
translate E Stop {Stop}
translate E Store {Store}
translate E Update {Update}
translate E ChangeOrient {Change window orientation}
translate E ShowIcons {Show Icons}
translate E None {None}
translate E First {First}
translate E Current {Current}
translate E Last {Last}

# General messages:
translate E game {game}
translate E games {games}
translate E move {move}
translate E moves {moves}
translate E all {all}
translate E Yes {Yes}
translate E No {No}
translate E Both {Both}
translate E King {King}
translate E Queen {Queen}
translate E Rook {Rook}
translate E Bishop {Bishop}
translate E Knight {Knight}
translate E Pawn {Pawn}
translate E White {White}
translate E Black {Black}
translate E Player {Player}
translate E Rating {Rating}
translate E RatingDiff {Rating difference (White - Black)}
translate E AverageRating {Average Rating}
translate E Event {Event}
translate E Site {Site}
translate E Country {Country}
translate E IgnoreColors {Ignore colors}
translate E Date {Date}
translate E EventDate {Event date}
translate E Decade {Decade}
translate E Year {Year}
translate E Month {Month}
translate E Months {January February March April May June July August September October November December}
translate E Days {Sun Mon Tue Wed Thu Fri Sat}
translate E YearToToday {-1Y}
translate E YearToTodayTooltip {Set date from 1 year back to today}
translate E Result {Result}
translate E Round {Round}
translate E Length {Length}
translate E ECOCode {ECO code}
translate E ECO {ECO}
translate E Deleted {Deleted}
translate E SearchResults {Search Results}
translate E OpeningTheDatabase {Opening database}
translate E Database {Database}
translate E Filter {Filter}
translate E noGames {no games}
translate E allGames {all games}
translate E empty {empty}
translate E clipbase {clipbase}
translate E score {score}
translate E StartPos {Start position}
translate E Total {Total}
translate E readonly {read-only}

# Standard error messages:
translate E ErrNotOpen {This is not an open database.}
translate E ErrReadOnly {This database is read-only; it cannot be altered.}
translate E ErrSearchInterrupted {Search was interrupted; results are incomplete.}

# Game information:
translate E twin {twin}
translate E deleted {deleted}
translate E comment {comment}
translate E hidden {hidden}
translate E LastMove {Last move}
translate E NextMove {Next}
translate E GameStart {Start of game}
translate E LineStart {Start of line}
translate E GameEnd {End of game}
translate E LineEnd {End of line}

# Player information:
translate E PInfoAll {Results for <b>all</b> games}
translate E PInfoFilter {Results for <b>filter</b> games}
translate E PInfoAgainst {Results against}
translate E PInfoMostWhite {Most common openings as White}
translate E PInfoMostBlack {Most common openings as Black}
translate E PInfoRating {Rating history}
translate E PInfoBio {Biography}
translate E PInfoEditRatings {Edit Ratings}
translate E PInfoEloFile {File}

# Tablebase information:
translate E Draw {Draw}
translate E stalemate {stalemate}
translate E withAllMoves {with all moves}
translate E withAllButOneMove {with all but one move}
translate E with {with}
translate E only {only}
translate E lose {lose}
translate E loses {loses}
translate E allOthersLose {all others lose}
translate E matesIn {mates in}
translate E hasCheckmated {has checkmated}
translate E longest {longest}
translate E WinningMoves {Winning moves}
translate E DrawingMoves {Drawing moves}
translate E LosingMoves {Losing moves}
translate E UnknownMoves {Unknown-result moves}
translate E SideLongest {Side    Longest    %     %     %\n}
translate E toNoveMate {to move   mate    Win  Draw  Loss\n}
translate E NoSummary {\nNo summary for this tablebase.}
translate E MutualZugzwang {Mutual zugzwangs: }
translate E SelectetZugzwang {\nSelected zugzwang positions:}
translate E WdrawBloss {White draws, Black loses:}
translate E BdrawWloss {Black draws, White loses:}
translate E AllLoss {" Whoever moves loses:     " }

# Tip of the day:
translate E Tip {Tip}
translate E TipAtStartup {Tip at startup}

# Tree window menus:
menuText E TreeFile "File" 0
menuText E TreeFileFillWithBase "Fill Cache with base" 0 {Fill the cache file with all games in current base}
menuText E TreeFileFillWithGame "Fill Cache with game" 0 {Fill the cache file with current game in current base}
menuText E TreeFileSetCacheSize "Cache size" 0 {Set the cache size}
menuText E TreeFileCacheInfo "Cache info" 0 {Get info on cache usage}
menuText E TreeFileSave "Save Cache File" 0 {Save the tree cache (.stc) file}
menuText E TreeFileFill "Fill Cache File" 0 \
  {Fill the cache file with common opening positions}
menuText E TreeFileBest "Best Games List" 0 {Show the best tree games list}
menuText E TreeFileGraph "Graph Window" 0 {Show the graph for this tree branch}
menuText E TreeFileCopy "Copy Tree Text to Clipboard" 1 \
  {Copy the tree statisctics to the clipboard}
menuText E TreeFileClose "Close Tree Window" 0 {Close the tree window}
menuText E TreeMask "Mask" 0
menuText E TreeMaskNew "New" 0 {New mask}
menuText E TreeMaskOpen "Open" 0 {Open mask}
menuText E TreeMaskOpenRecent "Open recent" 0 {Open recent mask}
menuText E TreeMaskSave "Save" 0 {Save mask}
menuText E TreeMaskClose "Close" 0 {Close mask}
menuText E TreeMaskFillWithGame "Fill with game" 0 {Fill mask with game}
menuText E TreeMaskFillWithBase "Fill with base" 0 {Fill mask with all games in base}
menuText E TreeMaskInfo "Info" 0 {Show statistics for current mask}
menuText E TreeMaskDisplay "Display mask map" 0 {Show mask data in a tree form}
menuText E TreeMaskSearch "Search" 0 {Search in current mask}
menuText E TreeSort "Sort" 0
menuText E TreeSortAlpha "Alphabetical" 0
menuText E TreeSortECO "ECO Code" 0
menuText E TreeSortFreq "Frequency" 0
menuText E TreeSortScore "Score" 0
menuText E TreeOpt "Options" 0
menuText E TreeOptSlowmode "slow mode" 0 {Slow mode for updates (high accuracy)}
menuText E TreeOptFastmode "Fast mode" 0 {Fast mode for updates (no move transposition)}
menuText E TreeOptFastAndSlowmode "Fast and slow mode" 0 {Fast mode then slow mode for updates}
menuText E TreeOptStartStop "Auto refreshing" 0 {Toggles automatic refreshing of the tree window}
menuText E TreeOptLock "Lock" 0 {Lock/unlock the tree to the current database}
menuText E TreeOptTraining "Training" 0 {Turn on/off tree training mode}
menuText E TreeOptAutosave "Auto-Save Cache File" 0 \
  {Auto-save the cache file when closing the tree window}
menuText E TreeHelp "Help" 0
menuText E TreeHelpTree "Tree Help" 0
menuText E TreeHelpIndex "Help Index" 0
translate E SaveCache {Save Cache}
translate E Training {Training}
translate E LockTree {Lock}
translate E TreeLocked {locked}
translate E TreeBest {Best}
translate E TreeBestGames {Best Tree Games}
# Note: the next message is the tree window title row. After editing it,
# check the tree window to make sure it lines up with the actual columns.
translate E TreeTitleRow \
  {    Move   ECO       Frequency    Score  AvElo Perf AvYear %Draws}
translate E TreeTotal {TOTAL}
translate E DoYouWantToSaveFirst {Do you want to save first}
translate E AddToMask {Add to Mask}
translate E RemoveFromMask {Remove from Mask}
translate E AddThisMoveToMask {Add this move to Mask}
translate E SearchMask {Search in Mask}
translate E DisplayMask {Display Mask}
translate E Nag {Nag code}
translate E Marker {Marker}
translate E Include {Include}
translate E Exclude {Exclude}
translate E MainLine {Main line}
translate E Bookmark {Bookmark}
translate E NewLine {New line}
translate E ToBeVerified {To be verified}
translate E ToTrain {To train}
translate E Dubious {Dubious}
translate E ToRemove {To remove}
translate E NoMarker {No marker}
translate E ColorMarker {Color}
translate E WhiteMark {White}
translate E GreenMark {Green}
translate E YellowMark {Yellow}
translate E BlueMark {Blue}
translate E RedMark {Red}
translate E CommentMove {Comment move}
translate E CommentPosition {Comment position}
translate E AddMoveToMaskFirst {Add move to mask first}
translate E OpenAMaskFileFirst {Open a mask file first}
translate E Positions {Positions}
translate E Moves {Moves}

# Finder window:
menuText E FinderFile "File" 0
menuText E FinderFileSubdirs "Look in Subdirectories" 0
menuText E FinderFileClose "Close File Finder" 0
menuText E FinderSort "Sort" 0
menuText E FinderSortType "Type" 0
menuText E FinderSortSize "Size" 0
menuText E FinderSortMod "Modified" 0
menuText E FinderSortName "Name" 0
menuText E FinderSortPath "Path" 0
menuText E FinderTypes "Types" 0
menuText E FinderTypesScid "Scid Databases" 0
menuText E FinderTypesOld "Old-Format Scid Databases" 0
menuText E FinderTypesPGN "PGN Files" 0
menuText E FinderTypesEPD "EPD Files" 0
menuText E FinderTypesRep "Repertoire Files" 0
menuText E FinderHelp "Help" 0
menuText E FinderHelpFinder "File Finder Help" 0
menuText E FinderHelpIndex "Help Index" 0
translate E FileFinder {File Finder}
translate E FinderDir {Directory}
translate E FinderDirs {Directories}
translate E FinderFiles {Files}
translate E FinderUpDir {up}
translate E FinderCtxOpen {Open}
translate E FinderCtxBackup {Backup}
translate E FinderCtxCopy {Copy}
translate E FinderCtxMove {Move}
translate E FinderCtxDelete {Delete}
 
# Player finder:
menuText E PListFile "File" 0
menuText E PListFileUpdate "Update" 0
menuText E PListFileClose "Close Player Finder" 0
menuText E PListSort "Sort" 0
menuText E PListSortName "Name" 0
menuText E PListSortElo "Elo" 0
menuText E PListSortGames "Games" 0
menuText E PListSortOldest "Oldest" 0
menuText E PListSortNewest "Newest" 2

# Tournament finder:
menuText E TmtFile "File" 0
menuText E TmtFileUpdate "Update" 0
menuText E TmtFileClose "Close Tournament Finder" 0
menuText E TmtSort "Sort" 0
menuText E TmtSortDate "Date" 0
menuText E TmtSortPlayers "Players" 0
menuText E TmtSortGames "Games" 0
menuText E TmtSortElo "Elo" 0
menuText E TmtSortSite "Site" 0
menuText E TmtSortEvent "Event" 1
menuText E TmtSortWinner "Winner" 0
translate E TmtLimit "List Limit"
translate E TmtMeanElo "Mean Elo"
translate E TmtNone "No matching tournaments were found."

# Graph windows:
menuText E GraphFile "File" 0
menuText E GraphFileColor "Save as Color PostScript..." 8
menuText E GraphFileGrey "Save as Greyscale PostScript..." 8
menuText E GraphFileClose "Close Window" 6
menuText E GraphOptions "Options" 0
menuText E GraphOptionsWhite "White" 0
menuText E GraphOptionsBlack "Black" 0
menuText E GraphOptionsBoth "Both" 1
menuText E GraphOptionsPInfo "Player Info player" 0
menuText E GraphOptionsEloFile "Elo from rating file" 0
menuText E GraphOptionsEloDB "Elo from database" 0
translate E GraphFilterTitle "Filter Graph: frequency per 1000 games"
translate E GraphAbsFilterTitle "Filter Graph: frequency of the games"
translate E ConfigureFilter "Configure X-Axes for Year, Rating and Moves"
translate E FilterEstimate "Estimate"
translate E TitleFilterGraph "Scid: Filter Graph"

# Analysis window:
translate E AddVariation {Add Variation}
translate E AddAllVariations {Add All Variations}
translate E AddMove {Add Move}
translate E Annotate {Annotate}
translate E ShowAnalysisBoard {Show analysis board}
translate E ShowInfo {Show engine info}
translate E FinishGame {Finish game}
translate E StopEngine {Stop engine}
translate E StartEngine {Start engine}
translate E LockEngine {Lock engine to current position}
translate E AnalysisCommand {Analysis Command}
translate E PreviousChoices {Previous Choices}
translate E AnnotateTime {Time per move in sec.}
translate E AnnotateWhich {Add variations}
translate E AnnotateAll {For moves by both sides}
translate E AnnotateAllMoves {Annotate all moves}
translate E AnnotateWhite {For White moves only}
translate E AnnotateBlack {For Black moves only}
translate E AnnotateBlundersOnly {When game move is a blunder}
translate E AnnotateBlundersOnlyScoreChange {Analysis reports blunder, with score change from/to: }
translate E BlundersThreshold {Threshold}
translate E ScoreAllMoves {Score all moves}
translate E LowPriority {Low CPU Priority}
translate E ClickHereToSeeMoves {Click here to see moves}
translate E ConfigureInformant {Informant values}
translate E Informant!? {Interesting move}
translate E Informant? {Poor move}
translate E Informant?? {Blunder}
translate E Informant?! {Dubious move}
translate E Informant+= {White has a slight advantage}
translate E Informant+/- {White has a moderate advantage}
translate E Informant+- {White has a decisive advantage}
translate E Informant++- {The game is considered won}
  
# Book window
translate E Book {Book}
translate E OtherBookMoves {Opponent's book}
translate E OtherBookMovesTooltip {Moves to which the opponent has a reply}

# Analysis Engine open dialog:
translate E EngineList {Analysis Engine List}
translate E EngineName {Name}
translate E EngineCmd {Command}
translate E EngineArgs {Parameters}
translate E EngineDir {Directory}
translate E EngineElo {Elo}
translate E EngineTime {Date}
translate E EngineNew {New}
translate E EngineEdit {Edit}
translate E EngineRequired {Fields in bold are required; others are optional}

# PGN window menus:
menuText E PgnFile "File" 0
menuText E PgnFileCopy "Copy Game to Clipboard" 0
menuText E PgnFilePrint "Print to File..." 0
menuText E PgnFileClose "Close PGN Window" 10
menuText E PgnOpt "Display" 0
menuText E PgnOptColor "Color Display" 0
menuText E PgnOptShort "Short (3-line) Header" 0
menuText E PgnOptSymbols "Symbolic Annotations" 1
menuText E PgnOptIndentC "Indent Comments" 0
menuText E PgnOptIndentV "Indent Variations" 7
menuText E PgnOptColumn "Column Style (one move per line)" 1
menuText E PgnOptSpace "Space after Move Numbers" 1
menuText E PgnOptStripMarks "Strip out Colored Square/Arrow Codes" 1
menuText E PgnOptBoldMainLine "Use Bold Text for Main Line Moves" 4
menuText E PgnColor "Colors" 0
menuText E PgnColorHeader "Header..." 0
menuText E PgnColorAnno "Annotations..." 0
menuText E PgnColorComments "Comments..." 0
menuText E PgnColorVars "Variations..." 0
menuText E PgnColorBackground "Background..." 0
menuText E PgnColorMain "Main line..." 0
menuText E PgnColorCurrent "Current move background..." 1
menuText E PgnHelp "Help" 0
menuText E PgnHelpPgn "PGN Help" 0
menuText E PgnHelpIndex "Index" 0
translate E PgnWindowTitle {Notation - game %u}

# Crosstable window menus:
menuText E CrosstabFile "File" 0
menuText E CrosstabFileText "Print to Text File..." 9
menuText E CrosstabFileHtml "Print to HTML File..." 9
menuText E CrosstabFileLaTeX "Print to LaTeX File..." 9
menuText E CrosstabFileClose "Close Crosstable Window" 0
menuText E CrosstabEdit "Edit" 0
menuText E CrosstabEditEvent "Event" 0
menuText E CrosstabEditSite "Site" 0
menuText E CrosstabEditDate "Date" 0
menuText E CrosstabOpt "Display" 0
menuText E CrosstabOptAll "All-play-all" 0
menuText E CrosstabOptSwiss "Swiss" 0
menuText E CrosstabOptKnockout "Knockout" 0
menuText E CrosstabOptAuto "Auto" 1
menuText E CrosstabOptAges "Ages in Years" 8
menuText E CrosstabOptNats "Nationalities" 0
menuText E CrosstabOptRatings "Ratings" 0
menuText E CrosstabOptTitles "Titles" 0
menuText E CrosstabOptBreaks "Tie-Break Scores" 4
menuText E CrosstabOptDeleted "Include Deleted Games" 8
menuText E CrosstabOptColors "Colors (Swiss table only)" 0
menuText E CrosstabOptColumnNumbers "Numbered Columns (All-play-all table only)" 2
menuText E CrosstabOptGroup "Group Scores" 0
menuText E CrosstabSort "Sort" 0
menuText E CrosstabSortName "Name" 0
menuText E CrosstabSortRating "Rating" 0
menuText E CrosstabSortScore "Score" 0
menuText E CrosstabColor "Color" 0
menuText E CrosstabColorPlain "Plain Text" 0
menuText E CrosstabColorHyper "Hypertext" 0
menuText E CrosstabHelp "Help" 0
menuText E CrosstabHelpCross "Crosstable Help" 0
menuText E CrosstabHelpIndex "Help Index" 0
translate E SetFilter {Set Filter}
translate E AddToFilter {Add to Filter}
translate E Swiss {Swiss}
translate E Category {Category}

# Opening report window menus:
menuText E OprepFile "File" 0
menuText E OprepFileText "Print to Text File..." 9
menuText E OprepFileHtml "Print to HTML File..." 9
menuText E OprepFileLaTeX "Print to LaTeX File..." 9
menuText E OprepFileOptions "Options..." 0
menuText E OprepFileClose "Close Report Window" 0
menuText E OprepFavorites "Favorites" 1
menuText E OprepFavoritesAdd "Add Report..." 0
menuText E OprepFavoritesEdit "Edit Report Favorites..." 0
menuText E OprepFavoritesGenerate "Generate Reports..." 0
menuText E OprepHelp "Help" 0
menuText E OprepHelpReport "Opening Report Help" 0
menuText E OprepHelpIndex "Help Index" 0

# Header search:
translate E HeaderSearch {Header Search}
translate E EndSideToMove {Side to move at end of game}
translate E GamesWithNoECO {Games with no ECO?}
translate E GameLength {Game length}
translate E FindGamesWith {Find games with flags}
translate E StdStart {Non-standard start}
translate E Promotions {Promotions}
translate E Comments {Comments}
translate E Variations {Variations}
translate E Annotations {Annotations}
translate E DeleteFlag {Delete flag}
translate E WhiteOpFlag {White opening}
translate E BlackOpFlag {Black opening}
translate E MiddlegameFlag {Middlegame}
translate E EndgameFlag {Endgame}
translate E NoveltyFlag {Novelty}
translate E PawnFlag {Pawn structure}
translate E TacticsFlag {Tactics}
translate E QsideFlag {Queenside play}
translate E KsideFlag {Kingside play}
translate E BrilliancyFlag {Brilliancy}
translate E BlunderFlag {Blunder}
translate E UserFlag {User}
translate E PgnContains {PGN contains text}
translate E Annotator {Annotator}
translate E Cmnts {Annotated games only}

# Game list window:
translate E GlistNumber {Number}
translate E GlistWhite {White}
translate E GlistBlack {Black}
translate E GlistWElo {W-Elo}
translate E GlistBElo {B-Elo}
translate E GlistEvent {Event}
translate E GlistSite {Site}
translate E GlistRound {Round}
translate E GlistDate {Date}
translate E GlistYear {Year}
translate E GlistEDate {EventDate}
translate E GlistResult {Result}
translate E GlistLength {Length}
translate E GlistCountry {Country}
translate E GlistECO {ECO}
translate E GlistOpening {Opening}
translate E GlistEndMaterial {End-Material}
translate E GlistDeleted {Deleted}
translate E GlistFlags {Flags}
translate E GlistVars {Variations}
translate E GlistComments {Comments}
translate E GlistAnnos {Annotations}
translate E GlistStart {Start}
translate E GlistGameNumber {Game number}
translate E GlistAverageElo {Average Elo}
translate E GlistRating {Rating}
translate E GlistFindText {Find text}
translate E GlistMoveField {Move}
translate E GlistEditField {Configure}
translate E GlistAddField {Add}
translate E GlistDeleteField {Remove}
translate E GlistWidth {Width}
translate E GlistAlign {Align}
translate E GlistAlignL {Align: left}
translate E GlistAlignR {Align: right}
translate E GlistAlignC {Align: center}
translate E GlistColor {Color}
translate E GlistSep {Separator}
translate E GlistCurrentSep {-- Current --}
translate E GlistNewSort {New}
translate E GlistAddToSort {Add}


# base sorting
translate E GsortSort {Sort...}
translate E GsortDate {Date}
translate E GsortYear {Year}
translate E GsortEvent {Event}
translate E GsortSite {Site}
translate E GsortRound {Round}
translate E GsortWhiteName {White Name}
translate E GsortBlackName {Black Name}
translate E GsortECO {ECO}
translate E GsortResult {Result}
translate E GsortMoveCount {Move Count}
translate E GsortAverageElo {Average Elo}
translate E GsortCountry {Country}
translate E GsortDeleted {Deleted}
translate E GsortEventDate {Event Date}
translate E GsortWhiteElo {White Elo}
translate E GsortBlackElo {Black Elo}
translate E GsortComments {Comments}
translate E GsortVariations {Variations}
translate E GsortNAGs {NAGs}
translate E GsortAscending {Ascending}
translate E GsortDescending {Descending}
translate E GsortAdd {Add}
translate E GsortStore {Store}
translate E GsortLoad {Load}

# menu shown with right mouse button down on game list. 
translate E GlistRemoveThisGameFromFilter  {Remove this game from Filter}
translate E GlistRemoveGameAndAboveFromFilter  {Remove game (and all above it) from Filter}
translate E GlistRemoveGameAndBelowFromFilter  {Remove game (and all below it) from Filter}
translate E GlistDeleteGame {(Un)Delete this game} 
translate E GlistDeleteAllGames {Delete all games in filter} 
translate E GlistUndeleteAllGames {Undelete all games in filter}  
translate E GlistMergeGameInBase {Merge Game in}

# Maintenance window:
translate E DatabaseName {Database Name:}
translate E TypeIcon {Type Icon:}
translate E NumOfGames {Games:}
translate E NumDeletedGames {Deleted games:}
translate E NumFilterGames {Games in filter:}
translate E YearRange {Year range:}
translate E RatingRange {Rating range:}
translate E Description {Description}
translate E Flag {Flag}
translate E CustomFlags {Custom flags}
translate E DeleteCurrent {Delete current game}
translate E DeleteFilter {Delete filter games}
translate E DeleteAll {Delete all games}
translate E UndeleteCurrent {Undelete current game}
translate E UndeleteFilter {Undelete filter games}
translate E UndeleteAll {Undelete all games}
translate E DeleteTwins {Delete twin games}
translate E MarkCurrent {Mark current game}
translate E MarkFilter {Mark filter games}
translate E MarkAll {Mark all games}
translate E UnmarkCurrent {Unmark current game}
translate E UnmarkFilter {Unmark filter games}
translate E UnmarkAll {Unmark all games}
translate E Spellchecking {Spell-checking}
translate E Players {Players}
translate E Events {Events}
translate E Sites {Sites}
translate E Rounds {Rounds}
translate E DatabaseOps {Database operations}
translate E ReclassifyGames {ECO-classify games}
translate E CompactDatabase {Compact database}
translate E SortDatabase {Sort database}
translate E AddEloRatings {Add Elo ratings}
translate E AutoloadGame {Autoload game number}
translate E StripTags {Strip PGN tags}
translate E StripTag {Strip tag}
translate E Cleaner {Cleaner}
translate E CleanerHelp {
The Scid Cleaner will perform all the maintenance actions you select from the list below, on the current database.
Current settings in the ECO classification and twin deletion dialogs will apply if you select those functions.
}
translate E CleanerConfirm {
Once Cleaner maintenance is started, it cannot be interrupted!

This may take a long time on a large database, depending on the functions you have selected and their current settings.

Are you sure you want to commence the maintenance functions you selected?
}
# Twinchecker
translate E TwinCheckUndelete {to flip; "u" undeletes both)}
translate E TwinCheckprevPair {Previous pair}
translate E TwinChecknextPair {Next pair}
translate E TwinChecker {Scid: Twin game checker}
translate E TwinCheckTournament {Games in tournament:}
translate E TwinCheckNoTwin {No twin  }
translate E TwinCheckNoTwinfound {No twin was detected for this game.\nTo show twins using this window, you must first use the "Delete twin games..." function. }
translate E TwinCheckTag {Share tags...}
translate E TwinCheckFound1 {Scid found $result twin games}
translate E TwinCheckFound2 { and set their delete flags}
translate E TwinCheckNoDelete {There are no games in this database to delete.}
translate E TwinCriteria1 { Your settings for finding twin games are potentially likely to\ncause non-twin games with similar moves to be marked as twins.}
translate E TwinCriteria2 {It is recommended that if you select "No" for "same moves", you should select "Yes" for the colors, event, site, round, year and month settings.\nDo you want to continue and delete twins anyway? }
translate E TwinCriteria3 {It is recommended that you specify "Yes" for at least two of the "same site", "same round" and "same year" settings.\nDo you want to continue and delete twins anyway?}
translate E TwinCriteriaConfirm {Scid: Confirm twin settings}
translate E TwinChangeTag "Change the following game tags:\n\n"
translate E AllocRatingDescription "This command will use the current spellcheck file to add Elo ratings to games in this database. Wherever a player has no current rating but his/her rating at the time of the game is listed in the spellcheck file, that rating will be added."
translate E RatingOverride "Overwrite existing non-zero ratings"
translate E AddRatings "Add ratings to:"
translate E AddedRatings {Scid added $r Elo ratings in $g games.}

#Bookmark editor
translate E NewSubmenu "New submenu"

# Comment editor:
translate E AnnotationSymbols  {Annotation Symbols:}
translate E Comment {Comment:}
translate E InsertMark {Insert mark}
translate E InsertMarkHelp {
Insert/remove mark: Select color, type, square.
Insert/remove arrow: Right-click two squares.
}

# Nag buttons in comment editor:
translate E GoodMove {Good move}
translate E PoorMove {Poor move}
translate E ExcellentMove {Excellent move}
translate E Blunder {Blunder}
translate E InterestingMove {Interesting move}
translate E DubiousMove {Dubious move}
translate E WhiteDecisiveAdvantage {White has a decisive advantage}
translate E BlackDecisiveAdvantage {Black has a decisive advantage}
translate E WhiteClearAdvantage {White has a clear advantage}
translate E BlackClearAdvantage {Black has a clear advantage}
translate E WhiteSlightAdvantage {White has a slight advantage}
translate E BlackSlightAdvantage {Black has a slight advantage}
translate E Equality {Equality}
translate E Unclear {Unclear}
translate E Diagram {Diagram}

# Board search:
translate E BoardSearch {Board Search}
translate E FilterOperation {Operation on current filter:}
translate E FilterAnd {AND (Restrict filter)}
translate E FilterOr {OR (Add to filter)}
translate E FilterIgnore {IGNORE (Reset filter)}
translate E SearchType {Search type:}
translate E SearchBoardExact {Exact position (all pieces on same squares)}
translate E SearchBoardPawns {Pawns (same material, all pawns on same squares)}
translate E SearchBoardFiles {Files (same material, all pawns on same files)}
translate E SearchBoardAny {Any (same material, pawns and pieces anywhere)}
translate E SearchInRefDatabase { Search in reference database }
translate E LookInVars {Look in variations}

# Material search:
translate E MaterialSearch {Material Search}
translate E Material {Material}
translate E Patterns {Patterns}
translate E Zero {Zero}
translate E Any {Any}
translate E CurrentBoard {Current Board}
translate E CommonEndings {Common Endings}
translate E CommonPatterns {Common Patterns}
translate E MaterialDiff {Material difference}
translate E squares {squares}
translate E SameColor {Same color}
translate E OppColor {Opposite color}
translate E Either {Either}
translate E MoveNumberRange {Move number range}
translate E MatchForAtLeast {Match for at least}
translate E HalfMoves {half-moves}

# Common endings in material search:
translate E EndingPawns {Pawn endings}
translate E EndingRookVsPawns {Rook vs. Pawn(s)}
translate E EndingRookPawnVsRook {Rook and 1 Pawn vs. Rook}
translate E EndingRookPawnsVsRook {Rook and Pawn(s) vs. Rook}
translate E EndingRooks {Rook vs. Rook endings}
translate E EndingRooksPassedA {Rook vs. Rook endings with a passed a-pawn}
translate E EndingRooksDouble {Double Rook endings}
translate E EndingBishops {Bishop vs. Bishop endings}
translate E EndingBishopVsKnight {Bishop vs. Knight endings}
translate E EndingKnights {Knight vs. Knight endings}
translate E EndingQueens {Queen vs. Queen endings}
translate E EndingQueenPawnVsQueen {Queen and 1 Pawn vs. Queen}
translate E BishopPairVsKnightPair {Two Bishops vs. Two Knights middlegame}

# Common patterns in material search:
translate E PatternWhiteIQP {White IQP}
translate E PatternWhiteIQPBreakE6 {White IQP: d4-d5 break vs. e6}
translate E PatternWhiteIQPBreakC6 {White IQP: d4-d5 break vs. c6}
translate E PatternBlackIQP {Black IQP}
translate E PatternWhiteBlackIQP {White IQP vs. Black IQP}
translate E PatternCoupleC3D4 {White c3+d4 Isolated Pawn Couple}
translate E PatternHangingC5D5 {Black Hanging Pawns on c5 and d5}
translate E PatternMaroczy {Maroczy Center (with Pawns on c4 and e4)}
translate E PatternRookSacC3 {Rook Sacrifice on c3}
translate E PatternKc1Kg8 {O-O-O vs. O-O (Kc1 vs. Kg8)}
translate E PatternKg1Kc8 {O-O vs. O-O-O (Kg1 vs. Kc8)}
translate E PatternLightFian {Light-Square Fianchettos (Bishop-g2 vs. Bishop-b7)}
translate E PatternDarkFian {Dark-Square Fianchettos (Bishop-b2 vs. Bishop-g7)}
translate E PatternFourFian {Four Fianchettos (Bishops on b2,g2,b7,g7)}

# Game saving:
translate E Today {Today}
translate E ClassifyGame {Classify game}

# Setup position:
translate E EmptyBoard {Empty board}
translate E InitialBoard {Initial board}
translate E SideToMove {Side to move}
translate E MoveNumber {Move number}
translate E Castling {Castling}
translate E EnPassantFile {En Passant file}
translate E ClearFen {Clear FEN}
translate E PasteFen {Paste FEN}

translate E SaveAndContinue {Save and continue}
translate E DiscardChangesAndContinue {Discard changes and continue}
translate E GoBack {Go back}

# Replace move dialog:
translate E ReplaceMove {Replace move}
translate E AddNewVar {Add new variation}
translate E NewMainLine {New Main Line}
translate E ReplaceMoveMessage {A move already exists here.

You can replace it, discarding all moves after it, or add your move as a new variation.

(You can avoid seeing this message in future by turning off the "Ask before replacing moves" option in the Options:Moves menu.)}

# Make database read-only dialog:
translate E ReadOnlyDialog {If you make this database read-only, no changes will be permitted.
No games can be saved or replaced, and no delete flags can be altered.
Any sorting or ECO-classification results will be temporary.

You can easily make the database writable again, by closing and reopening it.

Do you really want to make this database read-only?}

# Clear game dialog:
translate E ClearGameDialog {This game has been altered.

Do you really want to continue and discard the changes made to it?
}

# Exit dialog:
translate E ExitDialog {Do you really want to exit Scid?}
translate E ExitUnsaved {The following databases have unsaved game changes. If you exit now, these changes will be lost.}

# Import window:
translate E PasteCurrentGame {Paste current game}
translate E ImportHelp1 {Enter or paste a PGN-format game in the frame above.}
translate E ImportHelp2 {Any errors importing the game will be displayed here.}
translate E OverwriteExistingMoves {Overwrite existing moves ?}

# ECO Browser:
translate E ECOAllSections {all ECO sections}
translate E ECOSection {ECO section}
translate E ECOSummary {Summary for}
translate E ECOFrequency {Frequency of subcodes for}

# Opening Report:
translate E OprepTitle {Opening Report}
translate E OprepReport {Report}
translate E OprepGenerated {Generated by}
translate E OprepStatsHist {Statistics and History}
translate E OprepStats {Statistics}
translate E OprepStatAll {All report games}
translate E OprepStatBoth {Both rated}
translate E OprepStatSince {Since}
translate E OprepOldest {Oldest games}
translate E OprepNewest {Newest games}
translate E OprepPopular {Current popularity}
translate E OprepFreqAll {Frequency in all years:   }
translate E OprepFreq1   {In the  1 year  to today: }
translate E OprepFreq5   {In the  5 years to today: }
translate E OprepFreq10  {In the 10 years to today: }
translate E OprepEvery {once every %u games}
translate E OprepUp {up %u%s from all years}
translate E OprepDown {down %u%s from all years}
translate E OprepSame {no change from all years}
translate E OprepMostFrequent {Most frequent players}
translate E OprepMostFrequentOpponents {Most frequent opponents}
translate E OprepRatingsPerf {Ratings and Performance}
translate E OprepAvgPerf {Average ratings and performance}
translate E OprepWRating {White rating}
translate E OprepBRating {Black rating}
translate E OprepWPerf {White performance}
translate E OprepBPerf {Black performance}
translate E OprepHighRating {Games with highest average rating}
translate E OprepTrends {Result Trends}
translate E OprepResults {Result lengths and frequencies}
translate E OprepLength {Game length}
translate E OprepFrequency {Frequency}
translate E OprepWWins {White wins: }
translate E OprepBWins {Black wins: }
translate E OprepDraws {Draws:      }
translate E OprepWholeDB {whole database}
translate E OprepShortest {Shortest wins}
translate E OprepMovesThemes {Moves and Themes}
translate E OprepMoveOrders {Move orders reaching the report position}
translate E OprepMoveOrdersOne \
  {There was only one move order reaching this position:}
translate E OprepMoveOrdersAll \
  {There were %u move orders reaching this position:}
translate E OprepMoveOrdersMany \
  {There were %u move orders reaching this position. The top %u are:}
translate E OprepMovesFrom {Moves from the report position}
translate E OprepMostFrequentEcoCodes {Most frequent ECO codes}
translate E OprepThemes {Positional Themes}
translate E OprepThemeDescription {Frequency of themes in the first %u moves of each game}
translate E OprepThemeSameCastling {Same-side castling}
translate E OprepThemeOppCastling {Opposite castling}
translate E OprepThemeNoCastling {Both Kings uncastled}
translate E OprepThemeKPawnStorm {Kingside pawn storm}
translate E OprepThemeQueenswap {Queens exchanged}
translate E OprepThemeWIQP {White Isolated Queen Pawn}
translate E OprepThemeBIQP {Black Isolated Queen Pawn}
translate E OprepThemeWP567 {White Pawn on 5/6/7th rank}
translate E OprepThemeBP234 {Black Pawn on 2/3/4th rank}
translate E OprepThemeOpenCDE {Open c/d/e file}
translate E OprepTheme1BishopPair {Only one side has Bishop pair}
translate E OprepEndgames {Endgames}
translate E OprepReportGames {Report games}
translate E OprepAllGames    {All games}
translate E OprepEndClass {Material at the end of each game}
translate E OprepTheoryTable {Theory Table}
translate E OprepTableComment {Generated from the %u highest-rated games.}
translate E OprepExtraMoves {Extra note moves in theory table}
translate E OprepMaxGames {Maximum games in theory table}
translate E OprepViewHTML {View HTML}
translate E OprepViewLaTeX {View LaTeX}

# Player Report:
translate E PReportTitle {Player Report}
translate E PReportColorWhite {with the White pieces}
translate E PReportColorBlack {with the Black pieces}
translate E PReportMoves {after %s}
translate E PReportOpenings {Openings}
translate E PReportClipbase {Empty clipbase and copy matching games to it}

# Piece Tracker window:
translate E TrackerSelectSingle {Left mouse button selects this piece.}
translate E TrackerSelectPair {Left mouse button selects this piece; right button also selects its sibling.}
translate E TrackerSelectPawn {Left mouse button selects this pawn; right button selects all 8 pawns.}
translate E TrackerStat {Statistic}
translate E TrackerGames {% games with move to square}
translate E TrackerTime {% time on each square}
translate E TrackerMoves {Moves}
translate E TrackerMovesStart {Enter the move number where tracking should begin.}
translate E TrackerMovesStop {Enter the move number where tracking should stop.}

# Game selection dialogs:
translate E SelectAllGames {All games in the database}
translate E SelectFilterGames {Only games in the filter}
translate E SelectTournamentGames {Only games in the current tournament}
translate E SelectOlderGames {Only older games}

# Delete Twins window:
translate E TwinsNote {To be twins, two games must at least have the same two players, and criteria you can set below. When a pair of twins is found, the shorter game is deleted. Hint: it is best to spellcheck the database before deleting twins, since it improves twin detection. }
translate E TwinsCriteria {Criteria: Twin games must have...}
translate E TwinsWhich {Examine which games}
translate E TwinsColors {Same player colors}
translate E TwinsEvent {Same event}
translate E TwinsSite {Same site}
translate E TwinsRound {Same round}
translate E TwinsYear {Same year}
translate E TwinsMonth {Same month}
translate E TwinsDay {Same day}
translate E TwinsResult {Same result}
translate E TwinsECO {Same ECO code}
translate E TwinsMoves {Same moves}
translate E TwinsPlayers {Comparing player names}
translate E TwinsPlayersExact {Exact match}
translate E TwinsPlayersPrefix {First 4 letters only}
translate E TwinsWhen {When deleting twin games}
translate E TwinsSkipShort {Ignore all games under 5 moves long}
translate E TwinsUndelete {Undelete all games first}
translate E TwinsSetFilter {Set filter to all deleted twin games}
translate E TwinsComments {Always keep games with comments}
translate E TwinsVars {Always keep games with variations}
translate E TwinsDeleteWhich {Delete game}
translate E TwinsDeleteShorter {Shorter game}
translate E TwinsDeleteOlder {Smaller game number}
translate E TwinsDeleteNewer {Larger game number}
translate E TwinsDelete {Delete games}

# Name editor window:
translate E NameEditType {Type of name to edit}
translate E NameEditSelect {Games to edit}
translate E NameEditReplace {Replace}
translate E NameEditWith {with}
translate E NameEditMatches {Matches: Press Ctrl+1 to Ctrl+9 to select}

# Check games window:
translate E CheckGames {Check games}
translate E CheckGamesWhich {Check games}
translate E CheckAll {All games}
translate E CheckSelectFilterGames {Only games in filter}

# Classify window:
translate E Classify {Classify}
translate E ClassifyWhich {ECO-Classify which games}
translate E ClassifyAll {All games (overwrite old ECO codes)}
translate E ClassifyYear {All games played in the last year}
translate E ClassifyMonth {All games played in the last month}
translate E ClassifyNew {Only games with no ECO code yet}
translate E ClassifyCodes {ECO Codes to use}
translate E ClassifyBasic {Basic codes only ("B12", ...)}
translate E ClassifyExtended {Scid extensions ("B12j", ...)}

# Compaction:
translate E NameFile {Name file}
translate E GameFile {Game file}
translate E Names {Names}
translate E Unused {Unused}
translate E SizeKb {Size (kb)}
translate E CurrentState {Current State}
translate E AfterCompaction {After compaction}
translate E CompactNames {Compact name file}
translate E CompactGames {Compact game file}
translate E NoUnusedNames "There are no unused names, so the name file is already fully compacted."
translate E NoUnusedGames "The game file is already fully compacted."
translate E GameFileCompacted {The game file for the database was compacted.}

# Sorting:
translate E SortCriteria {Criteria}
translate E AddCriteria {Add criteria}
translate E CommonSorts {Common sorts}
translate E Sort {Sort}

# Exporting:
translate E AddToExistingFile {Add games to an existing file}
translate E ExportComments {Export comments}
translate E ExportVariations {Export variations}
translate E IndentComments {Indent comments}
translate E IndentVariations {Indent variations}
translate E ExportColumnStyle {Column style (one move per line)}
translate E ExportSymbolStyle {Symbolic annotation style:}
translate E ExportStripMarks {Strip square/arrow mark\ncodes from comments}

# Goto game/move dialogs:
translate E LoadGameNumber {Enter the game number to load:}
translate E GotoMoveNumber {Goto move number:}

# Copy games dialog:
translate E CopyGames {Copy games}
translate E CopyConfirm {
 Do you really want to copy
 the [::utils::thousands $nGamesToCopy] filtered games
 in the database "$fromName"
 to the database "$targetName"?
}
translate E CopyErr {Cannot copy games}
translate E CopyErrSource {the source database}
translate E CopyErrTarget {the target database}
translate E CopyErrNoGames {has no games in its filter}
translate E CopyErrReadOnly {is read-only}
translate E CopyErrNotOpen {is not open}

# Colors:
translate E LightSquares {Light squares}
translate E DarkSquares {Dark squares}
translate E SelectedSquares {Selected squares}
translate E SuggestedSquares {Suggested move squares}
translate E WhitePieces {White pieces}
translate E BlackPieces {Black pieces}
translate E WhiteBorder {White border}
translate E BlackBorder {Black border}

# Novelty window:
translate E FindNovelty {Find Novelty}
translate E Novelty {Novelty}
translate E NoveltyInterrupt {Novelty search interrupted}
translate E NoveltyNone {No novelty was found for this game}
translate E NoveltyHelp {Scid will find the first move of the current game that reaches a position not found in the selected database or in the ECO openings book.}

# Sounds configuration:
translate E SoundsFolder {Sound Files Folder}
translate E SoundsFolderHelp {The folder should contain the files King.wav, a.wav, 1.wav, etc}
translate E SoundsAnnounceOptions {Move Announcement Options}
translate E SoundsAnnounceNew {Announce new moves as they are made}
translate E SoundsAnnounceForward {Announce moves when moving forward one move}
translate E SoundsAnnounceBack {Announce when retracting or moving back one move}
translate E SoundsSoundDisabled {Scid could not find the Snack audio package at startup;\nSound is disabled.}

# Upgrading databases:
translate E Upgrading {Upgrading}
translate E ConfirmOpenNew {
This is an old-format (Scid 3) database that cannot be opened in Scid 4, but a new-format (Scid 4) version has already been created.

Do you want to open the new-format version of the database?
}
translate E ConfirmUpgrade {
This is an old-format (Scid 3) database. A new-format version of the database must be created before it can be used in Scid 4.

Upgrading will create a new version of the database and after that remove the original files.

This may take a while, but it only needs to be done one time. You can cancel if it takes too long.

Do you want to upgrade this database now?
}

# Recent files options:
translate E RecentFilesMenu {Number of recent files in File menu}
translate E RecentFilesExtra {Number of recent files in extra submenu}

# My Player Names options:
translate E MyPlayerNamesDescription {
Enter a list of preferred player names below, one name per line. Wildcards (e.g. "?" for any single character, "*" for any sequence of characters) are permitted.

Every time a game with a player in the list is loaded, the main window chessboard will be rotated if necessary to show the game from that players perspective.
}

#Coach
translate E showblunderexists {show blunder exists}
translate E showblundervalue {show blunder value}
translate E showscore {show score}
translate E coachgame {coach game}
translate E configurecoachgame {Configure tactical game}
translate E configuregame {Game configuration}
translate E Phalanxengine {Phalanx engine}
translate E Coachengine {Coach engine}
translate E difficulty {difficulty}
translate E hard {hard}
translate E easy {easy}
translate E Playwith {Play with}
translate E white {white}
translate E black {black}
translate E both {both}
translate E Play {Play}
translate E Noblunder {No blunder}
translate E blunder {blunder}
translate E Noinfo {-- No info --}
translate E PhalanxOrTogaMissing {Phalanx or Toga not found}
translate E moveblunderthreshold {move is a blunder if loss is greater than}
translate E limitanalysis {limit engine analysis time}
translate E seconds {seconds}
translate E Abort {Abort}
translate E Resume {Resume}
translate E OutOfOpening {Out of opening}
translate E NotFollowedLine {You did not follow the line}
translate E DoYouWantContinue {Do you want yo continue ?}
translate E CoachIsWatching {Coach is watching}
translate E Ponder {Permanent thinking}
translate E LimitELO {Limit ELO strength}
translate E DubiousMovePlayedTakeBack {Dubious move played, do you want to take back ?}
translate E WeakMovePlayedTakeBack {Weak move played, do you want to take back ?}
translate E BadMovePlayedTakeBack {Bad move played, do you want to take back ?}
translate E Iresign {I resign}
translate E yourmoveisnotgood {your move is not good}
translate E EndOfVar {End of variation}
translate E Openingtrainer {Opening trainer}
translate E DisplayCM {Display candidate moves}
translate E DisplayCMValue {Display candidate moves value}
translate E DisplayOpeningStats {Show statistics}
translate E ShowReport {Show report}
translate E NumberOfGoodMovesPlayed {good moves played}
translate E NumberOfDubiousMovesPlayed {dubious moves played}
translate E NumberOfMovesPlayedNotInRepertoire {moves played not in repertoire}
translate E NumberOfTimesPositionEncountered {times position encountered}
translate E PlayerBestMove  {Allow only best moves}
translate E OpponentBestMove {Opponent plays best moves}
translate E OnlyFlaggedLines {Only flagged lines}
translate E resetStats {Reset statistics}
translate E Repertoiretrainingconfiguration {Repertoire training configuration}
translate E Loadingrepertoire {Loading repertoire}
translate E Movesloaded {Moves loaded}
translate E Repertoirenotfound {Repertoire not found}
translate E Openfirstrepertoirewithtype {Open first a repertoire database with icon/type set to the right side}
translate E Movenotinrepertoire {Move not in repertoire}
translate E PositionsInRepertoire {Positions in repertoire}
translate E PositionsNotPlayed {Positions not played}
translate E PositionsPlayed {Positions played}
translate E Success {Success}
translate E DubiousMoves {Dubious moves}
translate E OutOfRepertoire {OutOfRepertoire}
translate E ConfigureTactics {Configure tactics}
translate E ResetScores {Reset scores}
translate E LoadingBase {Loading base}
translate E Tactics {Tactics}
translate E ShowSolution {Show solution}
translate E NextExercise {Next exercise}
translate E PrevExercise {Previous exercise}
translate E StopTraining {Stop training}
translate E Next {Next}
translate E ResettingScore {Resetting score}
translate E LoadingGame {Loading game}
translate E MateFound {Mate found}
translate E BestSolutionNotFound {Best solution NOT found !}
translate E MateNotFound {Mate not found}
translate E ShorterMateExists {Shorter mate exists}
translate E ScorePlayed {Score played}
translate E Expected {expected}
translate E ChooseTrainingBase {Choose training base}
translate E Thinking {Thinking}
translate E AnalyzeDone {Analyze done}
translate E WinWonGame {Win won game}
translate E Lines {Lines}
translate E ConfigureUCIengine {Configure UCI engine}
translate E SpecificOpening {Specific opening}
translate E StartNewGame {Start new game}
translate E FixedLevel {Fixed level}
translate E Opening {Opening}
translate E RandomLevel {Random level}
translate E StartFromCurrentPosition {Start from current position}
translate E FixedDepth {Fixed depth}
translate E Nodes {Nodes}
translate E Depth {Depth}
translate E Time {Time} 
translate E SecondsPerMove {Seconds per move}
translate E Engine {Engine}
translate E TimeMode {Time mode}
translate E TimeBonus {Time + bonus}
translate E TimeMin {min}
translate E TimeSec {sec}
translate E AllExercisesDone {All exercises done}
translate E MoveOutOfBook {Move out of book}
translate E LastBookMove {Last book move}
translate E AnnotateSeveralGames {From actual game to game:}
translate E FindOpeningErrors {Find opening errors}
translate E MarkTacticalExercises {Mark tactical exercises}
translate E UseBook {Use book}
translate E MultiPV {Multiple variations}
translate E Hash {Hash memory}
translate E OwnBook {Use engine book}
translate E BookFile {Opening book}
translate E AnnotateVariations {Annotate variations}
translate E ShortAnnotations {Short annotations}
translate E addAnnotatorTag {Add annotator tag}
translate E AddScoreToShortAnnotations {Add score to annotations}
translate E Export {Export}
translate E BookPartiallyLoaded {Book partially loaded}
translate E Calvar {Calculation of variations}
translate E ConfigureCalvar {Configuration}
# Opening names used in tacgame.tcl
translate E Reti {Reti}
translate E English {English}
translate E d4Nf6Miscellaneous {1.d4 Nf6 Miscellaneous}
translate E Trompowsky {Trompowsky}
translate E Budapest {Budapest}
translate E OldIndian {Old Indian}
translate E BenkoGambit {Benko Gambit}
translate E ModernBenoni {Modern Benoni}
translate E DutchDefence {Dutch Defence}
translate E Scandinavian {Scandinavian}
translate E AlekhineDefence {Alekhine Defence}
translate E Pirc {Pirc}
translate E CaroKann {Caro-Kann}
translate E CaroKannAdvance {Caro-Kann Advance}
translate E Sicilian {Sicilian}
translate E SicilianAlapin {Sicilian Alapin}
translate E SicilianClosed {Sicilian Closed}
translate E SicilianRauzer {Sicilian Rauzer}
translate E SicilianDragon {Sicilian Dragon}
translate E SicilianScheveningen {Sicilian Scheveningen}
translate E SicilianNajdorf {Sicilian Najdorf}
translate E OpenGame {Open Game}
translate E Vienna {Vienna}
translate E KingsGambit {King's Gambit}
translate E RussianGame {Russian Game}
translate E ItalianTwoKnights {Italian/Two Knights}
translate E Spanish {Spanish}
translate E SpanishExchange {Spanish Exchange}
translate E SpanishOpen {Spanish Open}
translate E SpanishClosed {Spanish Closed}
translate E FrenchDefence {French Defence}
translate E FrenchAdvance {French Advance}
translate E FrenchTarrasch {French Tarrasch}
translate E FrenchWinawer {French Winawer}
translate E FrenchExchange {French Exchange}
translate E QueensPawn {Queen's Pawn}
translate E Slav {Slav}
translate E QGA {QGA}
translate E QGD {QGD}
translate E QGDExchange {QGD Exchange}
translate E SemiSlav {Semi-Slav}
translate E QGDwithBg5 {QGD with Bg5}
translate E QGDOrthodox {QGD Orthodox}
translate E Grunfeld {Grünfeld}
translate E GrunfeldExchange {Grünfeld Exchange}
translate E GrunfeldRussian {Grünfeld Russian}
translate E Catalan {Catalan}
translate E CatalanOpen {Catalan Open}
translate E CatalanClosed {Catalan Closed}
translate E QueensIndian {Queen's Indian}
translate E NimzoIndian {Nimzo-Indian}
translate E NimzoIndianClassical {Nimzo-Indian Classical}
translate E NimzoIndianRubinstein {Nimzo-Indian Rubinstein}
translate E KingsIndian {King's Indian}
translate E KingsIndianSamisch {King's Indian Sämisch}
translate E KingsIndianMainLine {King's Indian Main Line}

# FICS
translate E ConfigureFics {Configure FICS}
translate E FICSGuest {Login as Guest}
translate E FICSServerPort {Server port}
translate E FICSServerAddress {IP Address}
translate E FICSRefresh {Refresh}
translate E FICSTimesealPort {Timeseal port}
translate E FICSSilence {Console filter}
translate E FICSOffers {Offers}
translate E FICSConsole {Console}
translate E FICSGames {Games}
translate E FICSUnobserve {Stop observing game}
translate E FICSProfile {Display your history and profile}
translate E FICSRelayedGames {Relayed games}
translate E FICSFindOpponent {Find opponent}
translate E FICSTakeback {Takeback}
translate E FICSTakeback2 {Takeback 2}
translate E FICSInitTime {Initial time (min)}
translate E FICSIncrement {Increment (sec)}
translate E FICSRatedGame {Rated Game}
translate E FICSAutoColour {automatic}
translate E FICSManualConfirm {confirm manually}
translate E FICSFilterFormula {Filter with formula}
translate E FICSIssueSeek {Issue seek}
translate E FICSChallenge {Challenge}
translate E FICSAccept {accept}
translate E FICSDecline {decline}
translate E FICSColour {Colour}
translate E FICSSend {send}
translate E FICSConnect {Connect}
translate E FICSdefaultuservars {Use default variables}
translate E FICSObserveconfirm {Do you want to observe game}
translate E FICSpremove {Enable premove}

# Game review
translate E GameReview {Game review}
translate E GameReviewTimeExtended {Time extended}
translate E GameReviewMargin {Error margin}
translate E GameReviewAutoContinue {Auto continue when move is correct}
translate E GameReviewReCalculate {Use extended time}
translate E GameReviewAnalyzingMovePlayedDuringTheGame {Analyzing move played during the game}
translate E GameReviewAnalyzingThePosition {Analyzing the position}
translate E GameReviewEnterYourMove {Enter your move}
translate E GameReviewCheckingYourMove {Checking your move}
translate E GameReviewYourMoveWasAnalyzed {Your move was analyzed}
translate E GameReviewYouPlayedSameMove {You played the same move as in match}
translate E GameReviewScoreOfYourMove {Score of your move}
translate E GameReviewGameMoveScore {Game move's score}
translate E GameReviewEngineScore {Engine's score}
translate E GameReviewYouPlayedLikeTheEngine {You played as good as engine's}
translate E GameReviewNotEngineMoveButGoodMove {Not the engine move, but is also a good move}
translate E GameReviewMoveNotGood {This move is not good, score is}
translate E GameReviewMovesPlayedLike {Moves played like}
translate E GameReviewMovesPlayedEngine {Moves played like engine}

# Correspondence Chess Dialogs:
translate E CCDlgConfigureWindowTitle {Configure Correspondence Chess}
translate E CCDlgCGeneraloptions {General Options}
translate E CCDlgDefaultDB {Default Database:}
translate E CCDlgInbox {Inbox (path):}
translate E CCDlgOutbox {Outbox (path):}
translate E CCDlgXfcc {Xfcc Configuration:}
translate E CCDlgExternalProtocol {External Protocol\nHandler (e.g. Xfcc)}
translate E CCDlgFetchTool {Fetch Tool:}
translate E CCDlgSendTool {Send Tool:}
translate E CCDlgEmailCommunication {eMail Communication}
translate E CCDlgMailPrg {Mail program:}
translate E CCDlgBCCAddr {(B)CC Address:}
translate E CCDlgMailerMode {Mode:}
translate E CCDlgThunderbirdEg {e.g. Thunderbird, Mozilla Mail, Icedove...}
translate E CCDlgMailUrlEg {e.g. Evolution}
translate E CCDlgClawsEg {e.g Sylpheed Claws}
translate E CCDlgmailxEg {e.g. mailx, mutt, nail...}
translate E CCDlgAttachementPar {Attachment\nparameter:}
translate E CCDlgInternalXfcc {Use internal Xfcc support}
translate E CCDlgConfirmXfcc {Confirm moves}
translate E CCDlgSubjectPar {Subject\nparameter:}
translate E CCDlgDeleteBoxes {Empty In-/Outbox}
translate E CCDlgDeleteBoxesText {Do you really want to empty your In- and Outbox folders for Correspondence Chess?\nThis requires a new sync to show the last state of your games.}
translate E CCDlgConfirmMove {Confirm move}
translate E CCDlgConfirmMoveText {If you confirm, the following move and comment will be sent to the server:}
translate E CCDlgDBGameToLong {Inconsistent Mainline}
translate E CCDlgDBGameToLongError {The mainline in your database is longer than the game in your Inbox. If the Inbox contains current games, i.e. right after a sync, some moves were added to the mainline in the database erroneously.

In this case please shorten the mainline to (at max) move
}


translate E CCDlgStartEmail {Start new eMail game}
translate E CCDlgYourName {Your Name:}
translate E CCDlgYourMail {Your eMail Address:}
translate E CCDlgOpponentName {Opponents Name:}
translate E CCDlgOpponentMail {Opponents eMail Address:}
translate E CCDlgGameID {Game ID (unique):}

translate E CCDlgTitNoOutbox {Scid: Correspondence Chess Outbox}
translate E CCDlgTitNoInbox {Scid: Correspondence Chess Inbox}
translate E CCDlgTitNoGames {Scid: No Correspondence Chess Games}
translate E CCErrInboxDir {Correspondence Chess inbox directory:}
translate E CCErrOutboxDir {Correspondence Chess outbox directory:}
translate E CCErrDirNotUsable {does not exist or is not accessible!\nPlease check and correct the settings.}
translate E CCErrNoGames {does not contain any games!\nPlease fetch them first.}

translate E CCDlgTitNoCCDB {Scid: No Correspondence Database}
translate E CCErrNoCCDB {No Database of type 'Correspondence' is opened. Please open one before using correspondence chess functions.}

translate E CCFetchBtn {Fetch games from the server and process the Inbox}
translate E CCPrevBtn {Goto previous game}
translate E CCNextBtn {Goto next game}
translate E CCSendBtn {Send move}
translate E CCEmptyBtn {Empty In- and Outbox}
translate E CCHelpBtn {Help on icons and status indicators.\nFor general Help press F1!}

translate E CCDlgServerName {Server Name:}
translate E CCDlgLoginName  {Login Name:}
translate E CCDlgPassword   {Password:}
translate E CCDlgShowPassword {Show password}
translate E CCDlgURL        {Xfcc-URL:}
translate E CCDlgRatingType {Rating Type:}

translate E CCDlgDuplicateGame {Non-unique game ID}
translate E CCDlgDuplicateGameError {This game exists more than once in your database. Please delete all duplicates and compact your game file (File/Maintenance/Compact Database).}

translate E CCDlgSortOption {Sorting:}
translate E CCDlgListOnlyOwnMove {Only games I have the move}
translate E CCOrderClassicTxt {Site, Event, Round, Result, White, Black}
translate E CCOrderMyTimeTxt {My Clock}
translate E CCOrderTimePerMoveTxt {Time per move till next time control}
translate E CCOrderStartDate {Start date}
translate E CCOrderOppTimeTxt {Opponents Clock}

translate E CCDlgConfigRelay {Observe games}
translate E CCDlgConfigRelayHelp {Go to the games page on http://www.iccf-webchess.com and display the game to be observed.  If you see the chessboard copy the URL from your browser to the list below. One URL per line only!\nExample: http://www.iccf-webchess.com/MakeAMove.aspx?id=266452}

# Connect Hardware dialogs
translate E ExtHWConfigConnection {Configure external hardware}
translate E ExtHWPort {Port}
translate E ExtHWEngineCmd {Engine command}
translate E ExtHWEngineParam {Engine parameter}
translate E ExtHWShowButton {Show button}
translate E ExtHWHardware {Hardware}
translate E ExtHWNovag {Novag Citrine}
translate E ExtHWInputEngine {Input Engine}
translate E ExtHWNoBoard {No board}
translate E NovagReferee {Referee}

# Input Engine dialogs
translate E IEConsole {Input Engine Console}
translate E IESending {Moves sent for}
translate E IESynchronise {Synchronise}
translate E IERotate  {Rotate}
translate E IEUnableToStart {Unable to start Input Engine:}


# Calculation of Variations
translate E DoneWithPosition {Done with position}

translate E Board {Board}
translate E showGameInfo {Show game info}
translate E autoResizeBoard {Automatic resize of board}
translate E DockTop {Move to top}
translate E DockBottom {Move to bottom}
translate E DockLeft {Move to left}
translate E DockRight {Move to right}
translate E Undock {Undock}

# Switcher window
translate E ChangeIcon {Choose database icon...}
translate E NewGameListWindow {New Game List Window}
translate E LoadatStartup {Load at startup}

# Gamelist window
translate E ShowHideDB {Show/Hide databases}
translate E ChangeFilter {Change filter}
translate E ChangeLayout {Load/Save/Change sorting criteria and column layout}
translate E ShowHideStatistic {Show/Hide statistics}
translate E BoardFilter {Show only games that matches the current board position}
translate E CopyGameTo {Copy Game to}
translate E FindBar {Find Bar}
translate E FindCurrentGame {Find current game}
translate E DeleteGame {Delete game}
translate E UndeleteGame {Undelete game}
translate E ResetSort {Reset sort}

translate E ConvertNullMove {Convert null moves to comments}
translate E SetupBoard {Setup Board}
translate E Rotate {Rotate}
translate E SwitchColors {Switch colors}
translate E FlipBoard {Flip Board}
translate E ImportPGN {Import PGN game}
translate E ImportingFiles {Importing PGN files in}
translate E ImportingFrom {Importing from}
translate E ImportingIn {Import games in}
translate E UseLastTag {Use last\ngame's tags}
translate E Random {Random}
translate E BackToMainline {Go back to mainline}
translate E LeaveVariant {Leave variant}
translate E Autoplay {Autoplay}
translate E ShowHideCoords {Show/Hide Coord.}
translate E ShowHideMaterial {Show/Hide Material}
translate E FullScreen {Full Screen}
translate E FilterStatistic {Filter Statistics}
translate E MakeCorrections {Make Corrections}
translate E Surnames {Surnames}
translate E Ambiguous {Ambiguous}

#Preferences Dialog
translate E OptionsToolbar "Toolbar"
translate E OptionsBoard "Chessboard"
translate E OptionsMenuColor "Menu colors"
translate E OptionsBoardSize "Board size"
translate E OptionsBoardPieces "Piece Style"
translate E OptionsInternationalization "Internationalization"
translate E MenuColorForeground "Textcolor"
translate E MenuColorBackground "Background"
translate E MenuColorSelect "Select"
translate E OptionsTablebaseDir "Select up to 4 table base folders:"
}
# end of english.tcl
