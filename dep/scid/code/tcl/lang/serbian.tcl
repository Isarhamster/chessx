### serbian.tcl:
# Serbian menus for Scid.
# Contributed by Milos Pejovic.

proc setLanguage_Y {} {

# File menu:
menuText Y File "Fajl" 0
menuText Y FileNew "Novi..." 0 {Kreiraj novu Scid bazu podataka}
menuText Y FileOpen "Otvori..." 0 {Otvori postojeæu Scid bazu podataka}
menuText Y FileClose "Zatvori" 0 {Zatvori aktivnu Scid bazu podataka}
menuText Y FileFinder "Tragaè" 0 {Pokreni Fajl Tragaè}
menuText Y FileBookmarks "Markeri" 0 {Meni sa markerima (preèica: Ctrl+B)}
menuText Y FileBookmarksAdd "Dodaj u markere" 0 \
  {Dodaj u markere tekuæu partiju iz baze i poziciju}
menuText Y FileBookmarksFile "Arhiviraj marker" 0 \
  {Arhiviraj marker za tekuæu partiju i poziciju}
menuText Y FileBookmarksEdit "Izmeni markere..." 0 \
  {Izmeni menije markera}
menuText Y FileBookmarksList "Prika¾i foldere kao jedinstvenu listu" 0 \
  {Prika¾i foldere markera kao jedinstvenu listu, bez podmenija}
menuText Y FileBookmarksSub "Prika¾i foldere kao podmenije" 0 \
  {Prika¾i foldere markera kao podmenije, ne jedinstvenu listu}
menuText Y FileMaint "Odr¾avanje" 0 {Alati za odr¾avanje Scid baze podataka}
menuText Y FileMaintWin "Prozor odr¾avanja" 0 \
  {Otvori/zatvori prozor odr¾avanja Scid baze podataka}
menuText Y FileMaintCompact "Komprimuj bazu..." 0 \
  {Uèini fajlove baze kompaktnijim, uklanjajuæi obrisane partije i nekori¹æena imena}
menuText Y FileMaintClass "E©O-Klasifikacija partija..." 2 \
  {Klasifikacija svih partija po otvaranju}
menuText Y FileMaintSort "Sortiraj bazu..." 0 \
  {Sortiraj sve partije u bazi podataka}
menuText Y FileMaintDelete "Izbri¹i udvojene partije..." 0 \
  {Naði udvojene partije i oznaèi ih za brisanje}
menuText Y FileMaintTwin "Provera udvojenih partija" 0 \
  {Otvori/osve¾i prozor za proveru udvojenih partija}
menuText Y FileMaintName "Imena" 0 {Promena imena i kontrola pravopisa}
menuText Y FileMaintNameEditor "Editor imena" 0 \
  {Otvori/zatvori editor imena}
menuText Y FileMaintNamePlayer "Provera pravopisa imena igraèa..." 11 \
  {Provera pravopisa imena igraèa pomoæu spellcheck fajla}
menuText Y FileMaintNameEvent "Provera pravopisa imena turnira..." 11 \
  {Provera pravopisa imena turnira pomoæu spellcheck fajla}
menuText Y FileMaintNameSite "Provera pravopisa imena mesta..." 11 \
  {Provera pravopisa imena mesta pomoæu spellcheck fajla}
menuText Y FileMaintNameRound "Provera pravopisa kola..." 11 \
  {Provera pravopisa kola pomoæu spellcheck fajla}
menuText Y FileReadOnly "Read-only..." 0 \
  {Spreèi izmene u tekuæoj bazi}
menuText Y FileSwitch "Switch to database" 0 \
  {Switch to a different opened database} ;# ***
menuText Y FileExit "Izaði" 1 {Izlazak iz Scid-a}
# ====== TODO To be translated ======
menuText Y FileMaintFixBase "Fix corrupted base" 0 {Try to fix a corrupted base}

# Edit menu:
menuText Y Edit "Izmene" 0
menuText Y EditAdd "Dodaj varijantu" 0 {Dodaj varijantu potezu u partiji}
menuText Y EditDelete "Obri¹i varijantu" 0 {Obri¹i varijantu za ovaj potez}
menuText Y EditFirst "Postavi prvu varijantu" 5 \
  {Postavi varijantu kao prvu u listi}
menuText Y EditMain "Postavi varijantu na glavnu liniju" 21 \
  {Postavi varijantu na glavnu liniju}
menuText Y EditTrial "Probaj varijantu" 0 \
  {Pokreni/zaustavi mod za testiranje ideje na tabli}
menuText Y EditStrip "Ukloni" 2 {Ukloni komentare ili varijante iz ove partije}
# ====== TODO To be translated ======
menuText Y EditUndo "Undo" 0 {Undo last game change}
# ====== TODO To be translated ======
menuText Y EditRedo "Redo" 0 {Redo last game change}
menuText Y EditStripComments "Komentare" 0 \
  {Ukloni sve komentare i napomene iz ove partije}
menuText Y EditStripVars "Varijante" 0 {Ukloni sve varijante iz ove partije}
menuText Y EditStripBegin "Moves from the beginning" 1 \
  {Strip moves from the beginning of the game} ;# ***
menuText Y EditStripEnd "Moves to the end" 0 \
  {Strip moves to the end of the game} ;# ***
menuText Y EditReset "Isprazni Clipbase" 0 \
  {Resetuje clipbase da bude potpuno prazan}
menuText Y EditCopy "Kopiraj ovu partiju na Clipbase" 0 \
  {Kopiraj ovu partiju na clipbase partija}
menuText Y EditPaste "Prenesi poslednju clipbase partiju" 0 \
  {Prenesi aktivnu clipbase partiju ovde}
menuText Y EditPastePGN "Paste Clipboard text as PGN game..." 10 \
  {Interpret the clipboard text as a game in PGN notation and paste it here} ;# ***
menuText Y EditSetup "Namesti poèetnu poziciju..." 0 \
  {Namesti poèetnu poziciju za ovu partiju}
menuText Y EditCopyBoard "Copy position" 6 \
  {Copy the current board in FEN notation to the text selection (klipbord)} ;# ***
menuText Y EditPasteBoard "Prenesi poèetnu poziciju" 9 \
  {Postavi poèetnu poziciju na osnovu trenutno izabranog teksta (klipbord)}
# ====== TODO To be translated ======
menuText Y ConfigureScid "Preferences..." 0 {Configure all options for SCID}

# Game menu:
menuText Y Game "Partija" 0
menuText Y GameNew "Nova partija" 0 \
  {Pokreni novu partiju, sa gubljenjem svih izmena}
menuText Y GameFirst "Uèitaj prvu partiju" 5 {Uèitaj prvu filtriranu partiju}
menuText Y GamePrev "Uèitaj prethodnu partiju" 5 {Uèitaj prethodnu filtriranu partiju}
menuText Y GameReload "Osve¾i trenutnu partiju" 3 \
  {Osve¾i ovu partiju, sa gubljenjem svih izmena}
menuText Y GameNext "Uèitaj sledeæu partiju" 5 {Uèitaj sledeæu filtriranu partiju}
menuText Y GameLast "Uèitaj poslednju partiju" 8 {Uèitaj poslednju filtriranu partiju}
menuText Y GameRandom "Load Random Game" 8 {Load a random filtered game} ;# ***
menuText Y GameNumber "Uèitaj partiju broj..." 5 \
  {Uèitaj partiju, unoseæi njen broj}
menuText Y GameReplace "Saèuvaj: Zameni partiju..." 6 \
  {Saèuvaj ovu partiju preko stare partiju u bazi}
menuText Y GameAdd "Saèuvaj: Dodaj novu partiju..." 6 \
  {Saèuvaj ovu partiju kao novu u bazi}
menuText Y GameDeepest "Identifikuj otvaranje" 0 \
  {Idi na poslednju poziciju partije koja odgovara knjizi otvaranja}
menuText Y GameGotoMove "Idi na potez broj..." 5 \
  {Idi na odreðeni broj potez u tekuæoj partiji}
menuText Y GameNovelty "Pronaði novost..." 8 \
  {Pronaði prvi potez ove partije koji nije igran ranije}

# Search Menu:
menuText Y Search "Pretra¾i" 3
menuText Y SearchReset "Resetuj filter" 0 {Resetuj filter da bi sve partije bile ukljuèene}
menuText Y SearchNegate "Invertuj filter" 0 {Invertuj filter da ukljuèi samo iskljuèene partije}
menuText Y SearchCurrent "Tekuæa pozicija..." 0 {Pretra¾uj prema tekuæoj poziciji na tabli}
menuText Y SearchHeader "Zaglavlje..." 0 {Pretra¾uj prema informacijama iz zaglavlja (igraè, turnir, itd)}
menuText Y SearchMaterial "Materijal/Pozicije..." 0 {Pretra¾uj prema materijalu i poziciji}
menuText Y SearchUsing "Pomoæu Search fajla..." 0 {Pretra¾uj pomoæu Search fajla}

# Windows menu:
menuText Y Windows "Prozori" 1
menuText Y WindowsComment "Editor komentara" 0 {Otvori/zatvori editor komentara}
menuText Y WindowsGList "Lista partija" 0 {Otvori/zatvori listu partija}
menuText Y WindowsPGN "PGN prozor" 0 \
  {Otvori/zatvori PGN prozor (sa podacima o partiji)}
menuText Y WindowsPList "Player Finder" 2 {Open/close the player finder} ;# ***
menuText Y WindowsTmt "Turnir tragaè" 2 {Otvori/zatvori turnir tragaè}
menuText Y WindowsSwitcher "Biraè baza" 0 \
  {Otvori/zatvori biraè baza}
menuText Y WindowsMaint "Prozor odr¾avanja" 0 \
  {Otvori/zatvori prozor odr¾avanja}
menuText Y WindowsECO "E©O prikazivaè" 0 {Otvori/zatvori E©O prikazivaè}
menuText Y WindowsStats "Statistièki prozor" 0 \
  {Otvori/zatvori prozor statistika filtera}
menuText Y WindowsTree "Stablo varijanata" 0 {Otvori/zatvori stablo varijanata}
menuText Y WindowsTB "Tabela zavr¹nica" 1 \
  {Otvori/zatvori tabelu zavr¹nica}
# ====== TODO To be translated ======
menuText Y WindowsBook "Book Window" 0 {Open/close the Book window}
# ====== TODO To be translated ======
menuText Y WindowsCorrChess "Correspondence Window" 0 {Open/close the Correspondence window}

# Tools menu:
menuText Y Tools "Alati" 0
menuText Y ToolsAnalysis "Program za analizu..." 0 \
  {Pokreni/zaustavi ¹ahovski program za analizu}
menuText Y ToolsAnalysis2 "Program za analizu #2..." 17 \
  {Pokreni/zaustavi drugi ¹ahovski program za analizu}
menuText Y ToolsCross "Tabela turnira" 0 {Poka¾i tabelu turnira za ovu partiju}
menuText Y ToolsEmail "Email menad¾er" 0 \
  {Otvori/zatvori menad¾er za korespodentske partije preko email-a}
menuText Y ToolsFilterGraph "Filter graph" 7 \
  {Open/close the filter graph window} ;# ***
# ====== TODO To be translated ======
menuText Y ToolsAbsFilterGraph "Abs. Filter Graph" 7 {Open/close the filter graph window for absolute values}
menuText Y ToolsOpReport "Izve¹taj o otvaranjima" 0 \
  {Napravi izve¹taj o otvaranjima za trenutnu poziciju}
# ====== TODO To be translated ======
menuText Y ToolsOpenBaseAsTree "Open base as tree" 0   {Open a base and use it in Tree window}
# ====== TODO To be translated ======
menuText Y ToolsOpenRecentBaseAsTree "Open recent base as tree" 0   {Open a recent base and use it in Tree window}
menuText Y ToolsTracker "Piece Tracker"  0 {Open the Piece Tracker window} ;# ***
# ====== TODO To be translated ======
menuText Y ToolsTraining "Training"  0 {Training tools (tactics, openings,...) }
# ====== TODO To be translated ======
menuText Y ToolsTacticalGame "Tactical game"  0 {Play a game with tactics}
# ====== TODO To be translated ======
menuText Y ToolsSeriousGame "Serious game"  0 {Play a serious game}
# ====== TODO To be translated ======
menuText Y ToolsTrainOpenings "Openings"  0 {Train with a repertoire}
# ====== TODO To be translated ======
menuText Y ToolsTrainReviewGame "Review game"  0 {Guess moves played in a game}
# ====== TODO To be translated ======
menuText Y ToolsTrainTactics "Tactics"  0 {Solve tactics}
# ====== TODO To be translated ======
menuText Y ToolsTrainCalvar "Calculation of variations"  0 {Calculation of variations training}
# ====== TODO To be translated ======
menuText Y ToolsTrainFindBestMove "Find best move"  0 {Find best move}
# ====== TODO To be translated ======
menuText Y ToolsTrainFics "Play on internet"  0 {Play on freechess.org}
# ====== TODO To be translated ======
menuText Y ToolsEngineTournament "Engine tournament"  0 {Start a tournament between chess engines}
# ====== TODO To be translated ======
menuText Y ToolsBookTuning "Book tuning" 0 {Book tuning}
# ====== TODO To be translated ======
menuText Y ToolsConnectHardware "Connect Hardware" 0 {Connect external hardware}
# ====== TODO To be translated ======
menuText Y ToolsConnectHardwareConfigure "Configure..." 0 {Configure external hardware and connection}
# ====== TODO To be translated ======
menuText Y ToolsConnectHardwareNovagCitrineConnect "Connect Novag Citrine" 0 {Connect Novag Citrine}
# ====== TODO To be translated ======
menuText Y ToolsConnectHardwareInputEngineConnect "Connect Input Engine" 0 {Connect Input Engine (e.g. DGT)}
menuText Y ToolsPInfo "Informacije o igraèu"  0 \
  {Otvori/osve¾i informacije o igraèu}
menuText Y ToolsPlayerReport "Player Report..." 3 \
  {Generate a player report} ;# ***
menuText Y ToolsRating "Grafikon rejtinga" 0 \
  {Iscrtava grafik istorije rejtinga igraèa u tekuæoj partiji}
menuText Y ToolsScore "Grafikon skora" 0 {Prika¾i grafikon skora}
menuText Y ToolsExpCurrent "Izvezi tekuæu partiju" 8 \
  {Snimi tekuæu partiju u tekstualni fajl}
menuText Y ToolsExpCurrentPGN "Izvezi partiju u PGN fajl..." 17 \
  {Snimi tekuæu partiju u PGN fajl}
menuText Y ToolsExpCurrentHTML "Izvezi partiju u HTML fajl..." 17 \
  {Snimi tekuæu partiju u HTML fajl}
# ====== TODO To be translated ======
menuText Y ToolsExpCurrentHTMLJS "Export Game to HTML and JavaScript File..." 15 {Write current game to a HTML and JavaScript file}  
menuText Y ToolsExpCurrentLaTeX "Izvezi partiju u LaTeX fajl..." 17 \
  {Snimi tekuæu partiju u LaTeX fajl}
menuText Y ToolsExpFilter "Izvezi sve filtrirane partije" 1 \
  {Snimi sve filtrirane partije u tekstualni fajl}
menuText Y ToolsExpFilterPGN "Izvezi filtrirane partije u PGN fajl..." 28 \
  {Snimi sve filtrirane partije u PGN fajl}
menuText Y ToolsExpFilterHTML "Izvezi filtrirane partije u HTML fajl..." 28 \
  {Snimi sve filtrirane partije u HTML fajl}
# ====== TODO To be translated ======
menuText Y ToolsExpFilterHTMLJS "Export Filter to HTML and JavaScript File..." 17 {Write all filtered games to a HTML and JavaScript file}  
menuText Y ToolsExpFilterLaTeX "Izvezi filtrirane partije u LaTeX fajl..." 28 \
  {Snimi sve filtrirane partije u LaTeX fajl}
menuText Y ToolsImportOne "Uvezi jednu PGN partiju..." 0 \
  {Uvezi jednu partiju iz PGN fajla}
menuText Y ToolsImportFile "Uvezi vi¹e PGN partija..." 6 \
  {Uvezi vi¹e partija iz PGN fajla}
# ====== TODO To be translated ======
menuText Y ToolsStartEngine1 "Start engine 1" 0  {Start engine 1}
# ====== TODO To be translated ======
menuText Y ToolsStartEngine2 "Start engine 2" 0  {Start engine 2}
# ====== TODO To be translated ======
menuText Y ToolsCaptureBoard "Capture Current Board..." 0  {Save the current board as an image.}
# ====== TODO To be translated ======
menuText Y Play "Play" 0
# ====== TODO To be translated ======
menuText Y CorrespondenceChess "Correspondence Chess" 0 {Functions for eMail and Xfcc based correspondence chess}
# ====== TODO To be translated ======
menuText Y CCConfigure "Configure..." 0 {Configure external tools and general setup}
# ====== TODO To be translated ======
menuText Y CCConfigRelay "Configure observations..." 10 {Configure games to be observed}
# ====== TODO To be translated ======
menuText Y CCOpenDB "Open Database..." 0 {Open the default Correspondence database}
# ====== TODO To be translated ======
menuText Y CCRetrieve "Retrieve Games" 0 {Retrieve games via external (Xfcc-)helper}
# ====== TODO To be translated ======
menuText Y CCInbox "Process Inobx" 0 {Process all files in scids Inbox}
# ====== TODO To be translated ======
menuText Y CCSend "Send Move" 0 {Send your move via eMail or external (Xfcc-)helper}
# ====== TODO To be translated ======
menuText Y CCResign "Resign" 0 {Resign (not via eMail)}
# ====== TODO To be translated ======
menuText Y CCClaimDraw "Claim Draw" 0 {Send move and claim a draw (not via eMail)}
# ====== TODO To be translated ======
menuText Y CCOfferDraw "Offer Draw" 0 {Send move and offer a draw (not via eMail)}
# ====== TODO To be translated ======
menuText Y CCAcceptDraw "Accept Draw" 0 {Accept a draw offer (not via eMail)}
# ====== TODO To be translated ======
menuText Y CCNewMailGame "New eMail Game..." 0 {Start a new eMail game}
# ====== TODO To be translated ======
menuText Y CCMailMove "Mail Move..." 0 {Send the move via eMail to the opponent}
# ====== TODO To be translated ======
menuText Y CCGamePage "Game Page..." 0 {Call up the game via the web browser}
# ====== TODO To be translated ======
menuText Y CCEditCopy "Copy Gamelist to Clipbase" 0 {Copy the games as CSV list to clipbase}

# Options menu:
menuText Y Options "Opcije" 0
# ====== TODO To be translated ======
menuText Y OptionsBoardGraphics "Squares..." 0 {Select textures for squares}
# ====== TODO To be translated ======
translate Y OptionsBGW {Select texture for squares}
# ====== TODO To be translated ======
translate Y OptionsBoardGraphicsText {Select graphic files for white and black squares:}
menuText Y OptionsBoardNames "My Player Names..." 0 {Edit my player names} ;# ***
menuText Y OptionsExport "Izvoz" 0 {Promeni opcije tekstualnog izvoza}
menuText Y OptionsFonts "Fontovi" 0 {Promeni fontove}
menuText Y OptionsFontsRegular "Obièan" 0 {Promeni obièan font}
menuText Y OptionsFontsMenu "Menu" 0 {Change the menu font} ;# ***
menuText Y OptionsFontsSmall "Mali" 0 {Promeni mali font}
# ====== TODO To be translated ======
menuText Y OptionsFontsTiny "Tiny" 0 {Change the tiny font}
menuText Y OptionsFontsFixed "Neproporcionalni" 0 {Promeni neproporcionalni font}
menuText Y OptionsGInfo "Informacije o Partiji" 0 {Opcije informacija o partiji}
menuText Y OptionsLanguage "Jezik" 0 {Izaberi jezik}
# ====== TODO To be translated ======
menuText Y OptionsMovesTranslatePieces "Translate pieces" 0 {Translate first letter of pieces}
# ====== TODO To be translated ======
menuText Y OptionsMovesHighlightLastMove "Highlight last move" 0 {Highlight last move}
# ====== TODO To be translated ======
menuText Y OptionsMovesHighlightLastMoveDisplay "Show" 0 {Display last move Highlight}
# ====== TODO To be translated ======
menuText Y OptionsMovesHighlightLastMoveWidth "Width" 0 {Thickness of line}
# ====== TODO To be translated ======
menuText Y OptionsMovesHighlightLastMoveColor "Color" 0 {Color of line}
# ====== TODO To be translated ======
menuText Y OptionsMovesHighlightLastMoveArrow "include Arrow" 0 {Include Arrow with Highlight}
menuText Y OptionsMoves "Potezi" 0 {Opcije uno¹enja poteza}
menuText Y OptionsMovesAnimate "Animation time" 1 \
  {Set the amount of time used to animate moves} ;# ***
menuText Y OptionsMovesDelay "Odlaganje izmeðu poteza..." 1 \
  {Podesi vreme odlaganja pri automatskom pregledanju poteza}
menuText Y OptionsMovesCoord "Uno¹enje koordinata poteza" 0 \
  {Prihvati uno¹enje poteza pomoæu koordinata ("g1f3")}
menuText Y OptionsMovesSuggest "Poka¾i preporuèene poteze" 0 \
  {Ukljuèi/iskljuèi preporuèene poteze}
# ====== TODO To be translated ======
menuText Y OptionsShowVarPopup "Show variations window" 0 {Turn on/off the display of a variations window}  
# ====== TODO To be translated ======
menuText Y OptionsMovesSpace "Add spaces after move number" 0 {Add spaces after move number}  
menuText Y OptionsMovesKey "Dopunjavanje poteza" 0 \
  {Ukljuèi/iskljuèi dopunjavanje poteza zadatih tastaturom}
# ====== TODO To be translated ======
menuText Y OptionsMovesShowVarArrows "Show Arrows for Variations" 0 {Turn on/off arrows showing moves in variations}
# ====== TODO To be translated ======
menuText Y OptionsMovesGlossOfDanger "Color Coded Gloss of Danger" 0 {Turn on/off color coded gloss of danger}
menuText Y OptionsNumbers "Format brojeva" 0 {Izaberi format brojeva}
# ====== TODO To be translated ======
menuText Y OptionsTheme "Theme" 0 {Change look of interface}
menuText Y OptionsWindows "Prozori" 0 {Opcije prozora}
menuText Y OptionsSounds "Sounds" 2 {Configure move announcement sounds} ;# ***
# ====== TODO To be translated ======
menuText Y OptionsResources "Resources..." 0 {Choose resource files and folders}
# ====== TODO To be translated ======
menuText Y OptionsWindowsDock "Dock windows" 0 {Dock windows}
# ====== TODO To be translated ======
menuText Y OptionsWindowsSaveLayout "Save layout" 0 {Save layout}
# ====== TODO To be translated ======
menuText Y OptionsWindowsRestoreLayout "Restore layout" 0 {Restore layout}
# ====== TODO To be translated ======
menuText Y OptionsWindowsShowGameInfo "Show game info" 0 {Show game info}
# ====== TODO To be translated ======
menuText Y OptionsWindowsAutoLoadLayout "Auto load first layout" 0 {Auto load first layout at startup}
menuText Y OptionsECO "Uèitaj E©O fajl" 7 {Uèitaj E©O klasifikacioni fajl}
menuText Y OptionsSpell "Uèitaj fajl za kontrolu pravopisa" 13 \
  {Uèitaj fajl za kontrolu pravopisa}
menuText Y OptionsTable "Direktorijum baza zavr¹nica" 0 \
  {Izaberi fajl baze zavr¹nica; sve baze u direktorijumu æe biti kori¹æene}
menuText Y OptionsRecent "Recent files..." 0 \
  {Change the number of recent files displayed in the File menu} ;# ***
# ====== TODO To be translated ======
menuText Y OptionsBooksDir "Books directory" 0 {Sets the opening books directory}
# ====== TODO To be translated ======
menuText Y OptionsTacticsBasesDir "Bases directory" 0 {Sets the tactics (training) bases directory}
# ====== TODO To be translated ======
menuText Y OptionsPhotosDir "Photos directory" 0 {Sets the photos bases directory}
# ====== TODO To be translated ======
menuText Y OptionsThemeDir "Theme(s) File:"  0 { Load a GUI theme package file }
menuText Y OptionsSave "Saèuvaj opcije" 0 \
  "Saèuvaj sve opcije u fajl $::optionsFile"
menuText Y OptionsAutoSave "Auto-saèuvaj opcije na izlasku" 0 \
  {Auto-saèuvaj sve opcije pri izlasku iz Scid-a}

# Help menu:
menuText Y Help "Pomoæ" 2
menuText Y HelpContents "Contents" 0 {Show the help contents page} ;# ***
menuText Y HelpIndex "Sadr¾aj" 0 {Prika¾i glavnu stranu pomoæi}
menuText Y HelpGuide "Brzi vodiè" 0 {Prika¾i brzi vodiè}
menuText Y HelpHints "Saveti" 0 {Prika¾i savete}
menuText Y HelpContact "Kontakt" 0 {Prika¾i informacije o kontaktu}
menuText Y HelpTip "Tip of the day" 0 {Show a useful Scid tip} ;# ***
menuText Y HelpStartup "Startup prozor" 0 {Prika¾i startup prozor}
menuText Y HelpAbout "O programu Scid" 0 {Informacije o programu Scid}

# Game info box popup menu:
menuText Y GInfoHideNext "Sakrij sledeæi potez" 0
menuText Y GInfoMaterial "Prika¾i vrednosti materijala" 0
menuText Y GInfoFEN "Prika¾i FEN" 8
menuText Y GInfoMarks "Show colored squares and arrows" 5 ;# ***
menuText Y GInfoWrap "Prelomi dugaèke linije" 0
menuText Y GInfoFullComment "Show Full Comment" 10 ;# ***
menuText Y GInfoPhotos "Show Photos" 5 ;# ***
menuText Y GInfoTBNothing "Baza zavr¹nica: ni¹ta" 16
menuText Y GInfoTBResult "Baza zavr¹nica: samo rezultati" 16
menuText Y GInfoTBAll "Baza zavr¹nica: rezultat i najbolji potezi" 16
menuText Y GInfoDelete "Obri¹i/vrati ovu partiju" 0
menuText Y GInfoMark "Mark/Unmark this game" 0 ;# ***
# ====== TODO To be translated ======
menuText Y GInfoInformant "Configure informant values" 0

# General buttons:
translate Y Back {Nazad}
translate Y Browse {Browse} ;# ***
translate Y Cancel {Poni¹ti}
# ====== TODO To be translated ======
translate Y Continue {Continue}
translate Y Clear {Oèisti}
translate Y Close {Zatvori}
translate Y Contents {Contents} ;# ***
translate Y Defaults {Podrazumevano}
translate Y Delete {Obri¹i}
translate Y Graph {Grafikon}
translate Y Help {Pomoæ}
translate Y Import {Uvoz}
translate Y Index {Indeks}
translate Y LoadGame {Uèitaj partiju}
translate Y BrowseGame {Browse game} ;# ***
translate Y MergeGame {Merge game} ;# ***
# ====== TODO To be translated ======
translate Y MergeGames {Merge Games}
translate Y Preview {Preview} ;# ***
translate Y Revert {Vrati se}
translate Y Save {Saèuvaj}
translate Y Search {Tra¾i}
translate Y Stop {Stop}
translate Y Store {Saèuvaj}
translate Y Update {Osve¾i}
translate Y ChangeOrient {Promeni orijentaciju prozora}
translate Y ShowIcons {Show Icons} ;# ***
translate Y None {None} ;# ***
translate Y First {Prvu}
translate Y Current {Trenutna}
translate Y Last {Poslednju}

# General messages:
translate Y game {partija}
translate Y games {partije}
translate Y move {potez}
translate Y moves {potezi}
translate Y all {sve}
translate Y Yes {Da}
translate Y No {Ne}
translate Y Both {Oba}
translate Y King {Kralj}
translate Y Queen {Dama}
translate Y Rook {Top}
translate Y Bishop {Lovac}
translate Y Knight {Skakaè}
translate Y Pawn {Pe¹ak}
translate Y White {Beli}
translate Y Black {Crni}
translate Y Player {Igraè}
translate Y Rating {Rejting}
translate Y RatingDiff {Razlika u rejtingu (Beli - Crni)}
translate Y AverageRating {Average Rating} ;# ***
translate Y Event {Turnir}
translate Y Site {Mesto}
translate Y Country {Zemlja}
translate Y IgnoreColors {Ignori¹i boje}
translate Y Date {Datum}
translate Y EventDate {Datum turnira}
translate Y Decade {Decade} ;# ***
translate Y Year {Godina}
translate Y Month {Mesec}
translate Y Months {Januar Februar Mart April Maj Jun Jul Avgust Septembar Oktobar Novembar Decembar}
translate Y Days {Ned Pon Uto Sre Èet Pet Sub}
translate Y YearToToday {Godina do danas}
# ====== TODO To be translated ======
translate Y YearToTodayTooltip {Set date from 1 year back to today}
translate Y Result {Rezultat}
translate Y Round {Kolo}
translate Y Length {Du¾ina}
translate Y ECOCode {E©O kôd}
translate Y ECO {E©O}
translate Y Deleted {Izbrisano}
translate Y SearchResults {Tra¾i rezultate}
translate Y OpeningTheDatabase {Otvaranje baze}
translate Y Database {Baza}
translate Y Filter {Filter}
translate Y noGames {ni jedna partija}
translate Y allGames {sve partije}
translate Y empty {prazno}
translate Y clipbase {clipbase}
translate Y score {skor}
translate Y StartPos {Poèetna pozicija}
translate Y Total {Ukupno}
translate Y readonly {read-only} ;# ***

# Standard error messages:
translate Y ErrNotOpen {This is not an open database.} ;# ***
translate Y ErrReadOnly {This database is read-only; it cannot be altered.} ;# ***
translate Y ErrSearchInterrupted {Search was interrupted; results are incomplete.} ;# ***

# Game information:
translate Y twin {udvojena}
translate Y deleted {izbrisana}
translate Y comment {komentar}
translate Y hidden {sakrivena}
translate Y LastMove {Poslednji potez}
translate Y NextMove {Sledeæi}
translate Y GameStart {Poèetak partije}
translate Y LineStart {Poèetak serije poteza}
translate Y GameEnd {Kraj partije}
translate Y LineEnd {Kraj serije poteza}

# Player information:
translate Y PInfoAll {Rezultati <b>svih</b> partija}
translate Y PInfoFilter {Rezultati <b>filtriranih</b> partija}
translate Y PInfoAgainst {Rezultati protiv}
translate Y PInfoMostWhite {Najèe¹æe igrana otvaranja sa belim figurama}
translate Y PInfoMostBlack {Najèe¹æe igrana otvaranja sa crnim figurama}
translate Y PInfoRating {Istorija rejtinga}
translate Y PInfoBio {Biography} ;# ***
translate Y PInfoEditRatings {Edit Ratings} ;# ***
# ====== TODO To be translated ======
translate Y PInfoEloFile {File}

# Tablebase information:
translate Y Draw {Remi}
translate Y stalemate {pat}
translate Y withAllMoves {sa svim potezima}
translate Y withAllButOneMove {sa svim osim jednog poteza}
translate Y with {sa}
translate Y only {samo}
translate Y lose {gubi}
translate Y loses {gubi}
translate Y allOthersLose {svi ostali gube}
translate Y matesIn {matira u}
translate Y hasCheckmated {je matirao}
translate Y longest {najdu¾i}
translate Y WinningMoves {Winning moves} ;# ***
translate Y DrawingMoves {Drawing moves} ;# ***
translate Y LosingMoves {Losing moves} ;# ***
translate Y UnknownMoves {Unknown-result moves} ;# ***
# ====== TODO To be translated ======
translate Y SideLongest {Side    Longest    %     %     %\n}
# ====== TODO To be translated ======
translate Y toNoveMate {to move   mate    Win  Draw  Loss\n}
# ====== TODO To be translated ======
translate Y NoSummary {\nNo summary for this tablebase.}
# ====== TODO To be translated ======
translate Y MutualZugzwang {Mutual zugzwangs: }
# ====== TODO To be translated ======
translate Y SelectetZugzwang {\nSelected zugzwang positions:}
# ====== TODO To be translated ======
translate Y WdrawBloss {White draws, Black loses:}
# ====== TODO To be translated ======
translate Y BdrawWloss {Black draws, White loses:}
# ====== TODO To be translated ======
translate Y AllLoss {" Whoever moves loses:     " }

# Tip of the day:
translate Y Tip {Tip} ;# ***
translate Y TipAtStartup {Tip at startup} ;# ***

# Tree window menus:
menuText Y TreeFile "Fajl" 0
# ====== TODO To be translated ======
menuText Y TreeFileFillWithBase "Fill Cache with base" 0 {Fill the cache file with all games in current base}
# ====== TODO To be translated ======
menuText Y TreeFileFillWithGame "Fill Cache with game" 0 {Fill the cache file with current game in current base}
# ====== TODO To be translated ======
menuText Y TreeFileSetCacheSize "Cache size" 0 {Set the cache size}
# ====== TODO To be translated ======
menuText Y TreeFileCacheInfo "Cache info" 0 {Get info on cache usage}
menuText Y TreeFileSave "Saèuvaj cache fajl" 0 {Saèuvaj cache (.stc) fajl stabla}
menuText Y TreeFileFill "Ispuni cache fajl" 0 \
  {Ispuni cache fajl èestim pozicijama otvaranja}
menuText Y TreeFileBest "Lista najboljih partija" 0 {Prika¾i listu najboljih partija stabla}
menuText Y TreeFileGraph "Prozor grafika" 0 {Prika¾i grafik za ovu granu stabla}
menuText Y TreeFileCopy "Kopiraj tekst stabla u clipboard" 0 \
  {Kopiraj statistike stabla u clipboard}
menuText Y TreeFileClose "Zatvori stablo varijanata" 0 {Zatvori prozor stabla}
# ====== TODO To be translated ======
menuText Y TreeMask "Mask" 0
# ====== TODO To be translated ======
menuText Y TreeMaskNew "New" 0 {New mask}
# ====== TODO To be translated ======
menuText Y TreeMaskOpen "Open" 0 {Open mask}
# ====== TODO To be translated ======
menuText Y TreeMaskOpenRecent "Open recent" 0 {Open recent mask}
# ====== TODO To be translated ======
menuText Y TreeMaskSave "Save" 0 {Save mask}
# ====== TODO To be translated ======
menuText Y TreeMaskClose "Close" 0 {Close mask}
# ====== TODO To be translated ======
menuText Y TreeMaskFillWithGame "Fill with game" 0 {Fill mask with game}
# ====== TODO To be translated ======
menuText Y TreeMaskFillWithBase "Fill with base" 0 {Fill mask with all games in base}
# ====== TODO To be translated ======
menuText Y TreeMaskInfo "Info" 0 {Show statistics for current mask}
# ====== TODO To be translated ======
menuText Y TreeMaskDisplay "Display mask map" 0 {Show mask data in a tree form}
# ====== TODO To be translated ======
menuText Y TreeMaskSearch "Search" 0 {Search in current mask}
menuText Y TreeSort "Sortiraj" 0
menuText Y TreeSortAlpha "Abecedno" 0
menuText Y TreeSortECO "E©O kôd" 0
menuText Y TreeSortFreq "Uèestalost" 0
menuText Y TreeSortScore "Skor" 0
menuText Y TreeOpt "Opcije" 0
# ====== TODO To be translated ======
menuText Y TreeOptSlowmode "slow mode" 0 {Slow mode for updates (high accuracy)}
# ====== TODO To be translated ======
menuText Y TreeOptFastmode "Fast mode" 0 {Fast mode for updates (no move transposition)}
# ====== TODO To be translated ======
menuText Y TreeOptFastAndSlowmode "Fast and slow mode" 0 {Fast mode then slow mode for updates}
# ====== TODO To be translated ======
menuText Y TreeOptStartStop "Auto refreshing" 0 {Toggles automatic refreshing of the tree window}
menuText Y TreeOptLock "Zakljuèaj" 0 {Zakljuèaj/otkljuèaj stablo za tekuæu bazu}
menuText Y TreeOptTraining "Trening" 0 {Ukljuèi/iskljuèi trening mod stabla}
menuText Y TreeOptAutosave "Automatski saèuvaj cache fajl" 0 \
  {Automatski saèuvaj cache fajl pri zatvaranju prozora stabla}
menuText Y TreeHelp "Pomoæ" 0
menuText Y TreeHelpTree "Pomoæ za stablo" 0
menuText Y TreeHelpIndex "Sadr¾aj pomoæi" 0
translate Y SaveCache {Saèuvaj cache}
translate Y Training {Trening}
translate Y LockTree {Zakljuèaj}
translate Y TreeLocked {zakljuèano}
translate Y TreeBest {Najbolje}
translate Y TreeBestGames {Najbolje partije stabla}
# Note: the next message is the tree window title row. After editing it,
# check the tree window to make sure it lines up with the actual columns.
translate Y TreeTitleRow \
  {    Move   ECO       Frequency    Score  AvElo Perf AvYear %Draws} ;# ***
translate Y TreeTotal {UKUPNO}
# ====== TODO To be translated ======
translate Y DoYouWantToSaveFirst {Do you want to save first}
# ====== TODO To be translated ======
translate Y AddToMask {Add to Mask}
# ====== TODO To be translated ======
translate Y RemoveFromMask {Remove from Mask}
# ====== TODO To be translated ======
translate Y AddThisMoveToMask {Add this move to Mask}
# ====== TODO To be translated ======
translate Y SearchMask {Search in Mask}
# ====== TODO To be translated ======
translate Y DisplayMask {Display Mask}
# ====== TODO To be translated ======
translate Y Nag {Nag code}
# ====== TODO To be translated ======
translate Y Marker {Marker}
# ====== TODO To be translated ======
translate Y Include {Include}
# ====== TODO To be translated ======
translate Y Exclude {Exclude}
# ====== TODO To be translated ======
translate Y MainLine {Main line}
# ====== TODO To be translated ======
translate Y Bookmark {Bookmark}
# ====== TODO To be translated ======
translate Y NewLine {New line}
# ====== TODO To be translated ======
translate Y ToBeVerified {To be verified}
# ====== TODO To be translated ======
translate Y ToTrain {To train}
# ====== TODO To be translated ======
translate Y Dubious {Dubious}
# ====== TODO To be translated ======
translate Y ToRemove {To remove}
# ====== TODO To be translated ======
translate Y NoMarker {No marker}
# ====== TODO To be translated ======
translate Y ColorMarker {Color}
# ====== TODO To be translated ======
translate Y WhiteMark {White}
# ====== TODO To be translated ======
translate Y GreenMark {Green}
# ====== TODO To be translated ======
translate Y YellowMark {Yellow}
# ====== TODO To be translated ======
translate Y BlueMark {Blue}
# ====== TODO To be translated ======
translate Y RedMark {Red}
# ====== TODO To be translated ======
translate Y CommentMove {Comment move}
# ====== TODO To be translated ======
translate Y CommentPosition {Comment position}
# ====== TODO To be translated ======
translate Y AddMoveToMaskFirst {Add move to mask first}
# ====== TODO To be translated ======
translate Y OpenAMaskFileFirst {Open a mask file first}
# ====== TODO To be translated ======
translate Y Positions {Positions}
# ====== TODO To be translated ======
translate Y Moves {Moves}

# Finder window:
menuText Y FinderFile "Fajl" 0
menuText Y FinderFileSubdirs "Pogledaj u poddirektorijumima" 0
menuText Y FinderFileClose "Zatvori Fajl tragaè" 0
menuText Y FinderSort "Sortiraj" 0
menuText Y FinderSortType "Tip" 0
menuText Y FinderSortSize "Velièina" 0
menuText Y FinderSortMod "Modifikovano" 0
menuText Y FinderSortName "Ime" 0
menuText Y FinderSortPath "Staza" 0
menuText Y FinderTypes "Tipovi" 0
menuText Y FinderTypesScid "Scid baze" 0
menuText Y FinderTypesOld "Scid baze u starom formatu" 0
menuText Y FinderTypesPGN "PGN fajlovi" 0
menuText Y FinderTypesEPD "EPD (book) fajlovi" 0
menuText Y FinderTypesRep "Repertoar fajlovi" 0
menuText Y FinderHelp "Pomoæ" 0
menuText Y FinderHelpFinder "Pomoæ - Fajl tragaè" 0
menuText Y FinderHelpIndex "Sadr¾aj pomoæi" 0
translate Y FileFinder {Fajl tragaè}
translate Y FinderDir {Direktorijum}
translate Y FinderDirs {Direktorijumi}
translate Y FinderFiles {Fajlovi}
translate Y FinderUpDir {gore}
# ====== TODO To be translated ======
translate Y FinderCtxOpen {Open}
# ====== TODO To be translated ======
translate Y FinderCtxBackup {Backup}
# ====== TODO To be translated ======
translate Y FinderCtxCopy {Copy}
# ====== TODO To be translated ======
translate Y FinderCtxMove {Move}
# ====== TODO To be translated ======
translate Y FinderCtxDelete {Delete}

# Player finder:
menuText Y PListFile "Fajl" 0
menuText Y PListFileUpdate "Osve¾i" 0
menuText Y PListFileClose "Close Player Finder" 0 ;# ***
menuText Y PListSort "Sortiraj" 0
menuText Y PListSortName "Name" 0 ;# ***
menuText Y PListSortElo "Elo" 0
menuText Y PListSortGames "Partije" 0
menuText Y PListSortOldest "Oldest" 0 ;# ***
menuText Y PListSortNewest "Newest" 0 ;# ***

# Tournament finder:
menuText Y TmtFile "Fajl" 0
menuText Y TmtFileUpdate "Osve¾i" 0
menuText Y TmtFileClose "Zatvori Turnir tragaè" 0
menuText Y TmtSort "Sortiraj" 0
menuText Y TmtSortDate "Datum" 0
menuText Y TmtSortPlayers "Igraèi" 0
menuText Y TmtSortGames "Partije" 0
menuText Y TmtSortElo "Elo" 0
menuText Y TmtSortSite "Mesto" 0
menuText Y TmtSortEvent "Turnir" 1
menuText Y TmtSortWinner "Pobednik" 0
translate Y TmtLimit "Ogranièenje liste"
translate Y TmtMeanElo "Najni¾i Elo"
translate Y TmtNone "Nije pronaðen odgovarajuæi turnir."

# Graph windows:
menuText Y GraphFile "Fajl" 0
menuText Y GraphFileColor "Saèuvaj kao Postscript u boji..." 25
menuText Y GraphFileGrey "Saèuvaj kao Postscript u sivim nivoima..." 25
menuText Y GraphFileClose "Zatvori prozor" 6
menuText Y GraphOptions "Opcije" 0
menuText Y GraphOptionsWhite "Beli" 0
menuText Y GraphOptionsBlack "Crni" 0
menuText Y GraphOptionsBoth "Oba" 1
menuText Y GraphOptionsPInfo "Informacije o igraèu" 0
# ====== TODO To be translated ======
menuText Y GraphOptionsEloFile "Elo from rating file" 0
# ====== TODO To be translated ======
menuText Y GraphOptionsEloDB "Elo from database" 0
translate Y GraphFilterTitle "Filter graph: frequency per 1000 games" ;# ***
# ====== TODO To be translated ======
translate Y GraphAbsFilterTitle "Filter Graph: frequency of the games"
# ====== TODO To be translated ======
translate Y ConfigureFilter {Configure X-Axes for Year, Rating and Moves}
# ====== TODO To be translated ======
translate Y FilterEstimate "Estimate"
# ====== TODO To be translated ======
translate Y TitleFilterGraph "Scid: Filter Graph"

# Analysis window:
translate Y AddVariation {Dodaj varijantu}
# ====== TODO To be translated ======
translate Y AddAllVariations {Add All Variations}
translate Y AddMove {Dodaj potez}
translate Y Annotate {Dodaj napomenu}
# ====== TODO To be translated ======
translate Y ShowAnalysisBoard {Show analysis board}
# ====== TODO To be translated ======
translate Y ShowInfo {Show engine info}
# ====== TODO To be translated ======
translate Y FinishGame {Finish game}
# ====== TODO To be translated ======
translate Y StopEngine {Stop engine}
# ====== TODO To be translated ======
translate Y StartEngine {Start engine}
# ====== TODO To be translated ======
translate Y LockEngine {Lock engine to current position}
translate Y AnalysisCommand {Komanda analize}
translate Y PreviousChoices {Prethodni izbori}
translate Y AnnotateTime {Set the time between moves in seconds} ;# ***
translate Y AnnotateWhich {Add variations} ;# ***
translate Y AnnotateAll {For moves by both sides} ;# ***
# ====== TODO To be translated ======
translate Y AnnotateAllMoves {Annotate all moves}
translate Y AnnotateWhite {For White moves only} ;# ***
translate Y AnnotateBlack {For Black moves only} ;# ***
# ====== TODO To be translated ======
translate Y AnnotateBlundersOnly {When game move is an obvious blunder}
# ====== TODO To be translated ======
translate Y AnnotateBlundersOnlyScoreChange {Analysis reports blunder, with score change from/to: }
# ====== TODO To be translated ======
translate Y BlundersThreshold {Threshold}
# TODO: Translate
translate Y ScoreAllMoves {Score all moves}
translate Y LowPriority {Low CPU priority} ;# ***
# ====== TODO To be translated ======
translate Y ClickHereToSeeMoves {Click here to see moves}
# ====== TODO To be translated ======
translate Y ConfigureInformant {Configure Informant}
# ====== TODO To be translated ======
translate Y Informant!? {Interesting move}
# ====== TODO To be translated ======
translate Y Informant? {Poor move}
# ====== TODO To be translated ======
translate Y Informant?? {Blunder}
# ====== TODO To be translated ======
translate Y Informant?! {Dubious move}
# ====== TODO To be translated ======
translate Y Informant+= {White has a slight advantage}
# ====== TODO To be translated ======
translate Y Informant+/- {White has a moderate advantage}
# ====== TODO To be translated ======
translate Y Informant+- {White has a decisive advantage}
# ====== TODO To be translated ======
translate Y Informant++- {The game is considered won}
# ====== TODO To be translated ======
translate Y Book {Book}
# ====== TODO To be translated ======
translate Y OtherBookMoves {Opponent's book}
# ====== TODO To be translated ======
translate Y OtherBookMovesTooltip {Moves to which the opponent has a reply}

# Analysis Engine open dialog:
translate Y EngineList {Analysis Engine List} ;# ***
translate Y EngineName {Name} ;# ***
translate Y EngineCmd {Command} ;# ***
translate Y EngineArgs {Parameters} ;# ***
translate Y EngineDir {Directory} ;# ***
translate Y EngineElo {Elo}
translate Y EngineTime {Datum}
translate Y EngineNew {New} ;# ***
translate Y EngineEdit {Edit} ;# ***
translate Y EngineRequired {Fields in bold are required; others are optional} ;# ***

# PGN window menus:
menuText Y PgnFile "Fajl" 0
menuText Y PgnFileCopy "Copy Game to Clipboard" 0 ;# ***
menuText Y PgnFilePrint "©tampaj u fajl..." 1
menuText Y PgnFileClose "Zatvori PGN prozor" 0
menuText Y PgnOpt "Prikaz" 0
menuText Y PgnOptColor "Prikaz u boji" 0
menuText Y PgnOptShort "Kratko (3-linije) zaglavlje" 0
menuText Y PgnOptSymbols "Simbolièki komentari" 0
menuText Y PgnOptIndentC "Uvuci pasus kod komentara" 0
menuText Y PgnOptIndentV "Uvuci pasus kod varijanata" 16
menuText Y PgnOptColumn "Pasus stil (jedan potez po liniji)" 1
menuText Y PgnOptSpace "Blanko posle broja poteza" 0
menuText Y PgnOptStripMarks "Strip out colored square/arrow codes" 1 ;# ***
menuText Y PgnOptBoldMainLine "Use Bold Text for Main Line Moves" 4 ;# ***
menuText Y PgnColor "Boje" 0
menuText Y PgnColorHeader "Zaglavlje..." 0
menuText Y PgnColorAnno "Napomene..." 0
menuText Y PgnColorComments "Komentari..." 0
menuText Y PgnColorVars "Varijante..." 0
menuText Y PgnColorBackground "Pozadina..." 0
# ====== TODO To be translated ======
menuText Y PgnColorMain "Main line..." 0
# ====== TODO To be translated ======
menuText Y PgnColorCurrent "Current move background..." 1
menuText Y PgnHelp "Pomoæ" 1
menuText Y PgnHelpPgn "PGN pomoæ" 0
menuText Y PgnHelpIndex "Sadr¾aj" 0
translate Y PgnWindowTitle {Game Notation - game %u} ;# ***

# Crosstable window menus:
menuText Y CrosstabFile "Fajl" 0
menuText Y CrosstabFileText "©tampaj u Tekst fajl..." 10
menuText Y CrosstabFileHtml "©tampaj u HTML fajl..." 10
menuText Y CrosstabFileLaTeX "©tampaj u LaTeX fajl..." 10
menuText Y CrosstabFileClose "Zatvori tabelu turnira" 0
menuText Y CrosstabEdit "Izmena" 0
menuText Y CrosstabEditEvent "Turnir" 0
menuText Y CrosstabEditSite "Mesto" 0
menuText Y CrosstabEditDate "Datum" 0
menuText Y CrosstabOpt "Prikaz" 0
menuText Y CrosstabOptAll "Svako sa svakim" 0
menuText Y CrosstabOptSwiss "©vajcarski sistem" 0
menuText Y CrosstabOptKnockout "Nokaut" 0
menuText Y CrosstabOptAuto "Auto" 0 ;# ***
menuText Y CrosstabOptAges "Starost u godinama" 10
menuText Y CrosstabOptNats "Narodnost" 0
menuText Y CrosstabOptRatings "Rejtinzi" 0
menuText Y CrosstabOptTitles "Titule" 0
menuText Y CrosstabOptBreaks "Tie-break rezultati" 4
menuText Y CrosstabOptDeleted "Include deleted games" 8 ;# ***
menuText Y CrosstabOptColors "Boje (samo za ¹vajcarski sistem)" 0
menuText Y CrosstabOptColumnNumbers "Numbered columns (All-play-all table only)" 2 ;# ***
menuText Y CrosstabOptGroup "Grupi¹i rezultate" 0
menuText Y CrosstabSort "Sortiraj" 0
menuText Y CrosstabSortName "Ime" 0
menuText Y CrosstabSortRating "Rejting" 0
menuText Y CrosstabSortScore "Rezultat" 0
menuText Y CrosstabColor "Boja" 0
menuText Y CrosstabColorPlain "Obièan tekst" 0
menuText Y CrosstabColorHyper "Hipertekst" 0
menuText Y CrosstabHelp "Pomoæ" 0
menuText Y CrosstabHelpCross "Pomoæ - Tabela turnira" 0
menuText Y CrosstabHelpIndex "Sadr¾aj pomoæi" 0
translate Y SetFilter {Postavi filter}
translate Y AddToFilter {Dodaj u filter}
translate Y Swiss {©vajcarski sistem}
translate Y Category {Category} ;# ***

# Opening report window menus:
menuText Y OprepFile "Fajl" 0
menuText Y OprepFileText "©tampaj u Tekst fajl..." 10
menuText Y OprepFileHtml "©tampaj u HTML fajl..." 10
menuText Y OprepFileLaTeX "©tampaj u LaTeX fajl..." 10
menuText Y OprepFileOptions "Opcije..." 0
menuText Y OprepFileClose "Zatvori izve¹taj o otvaranjima" 0
menuText Y OprepFavorites "Favorites" 1 ;# ***
menuText Y OprepFavoritesAdd "Add Report..." 0 ;# ***
menuText Y OprepFavoritesEdit "Edit Report Favorites..." 0 ;# ***
menuText Y OprepFavoritesGenerate "Generate Reports..." 0 ;# ***
menuText Y OprepHelp "Pomoæ" 0
menuText Y OprepHelpReport "Pomoæ - Izve¹taj o otvaranjima" 0
menuText Y OprepHelpIndex "Sadr¾aj pomoæi" 0

# Header search:
translate Y HeaderSearch {Pretraga zaglavlja}
translate Y EndSideToMove {Side to move at end of game} ;# ***
translate Y GamesWithNoECO {Partije bez E©O-a?}
translate Y GameLength {Du¾ina partije}
translate Y FindGamesWith {Pronaði partije sa}
translate Y StdStart {standardnim poèetkom}
translate Y Promotions {sa promocijama}
translate Y Comments {Komentari}
translate Y Variations {Varijante}
translate Y Annotations {Napomene}
translate Y DeleteFlag {Znak za brisanje}
translate Y WhiteOpFlag {Otvaranje belog}
translate Y BlackOpFlag {Otvaranje crnog}
translate Y MiddlegameFlag {Sredi¹njica}
translate Y EndgameFlag {Zavr¹nica}
translate Y NoveltyFlag {Novost}
translate Y PawnFlag {Pe¹aèka struktura}
translate Y TacticsFlag {Taktika}
translate Y QsideFlag {Igra na daminom krilu}
translate Y KsideFlag {Igra na kraljevom krilu}
translate Y BrilliancyFlag {Brilijantnost}
translate Y BlunderFlag {Gre¹ka}
translate Y UserFlag {Korisnik}
translate Y PgnContains {PGN contains text} ;# ***
# ====== TODO To be translated ======
translate Y Annotator {Annotator}
# ====== TODO To be translated ======
translate Y Cmnts {Annotated games only}

# Game list window:
translate Y GlistNumber {Broj}
translate Y GlistWhite {Beli}
translate Y GlistBlack {Crni}
translate Y GlistWElo {B-Elo}
translate Y GlistBElo {C-Elo}
translate Y GlistEvent {Turnir}
translate Y GlistSite {Mesto}
translate Y GlistRound {Kolo}
translate Y GlistDate {Datum}
translate Y GlistYear {Year}
translate Y GlistEDate {Datum turnira}
translate Y GlistResult {Rezultat}
translate Y GlistLength {Du¾ina}
translate Y GlistCountry {Zemlja}
translate Y GlistECO {E©O}
translate Y GlistOpening {Otvaranje}
translate Y GlistEndMaterial {End-Material} ;# ***
translate Y GlistDeleted {Izbrisano}
translate Y GlistFlags {Oznaka}
translate Y GlistVars {Varijante}
translate Y GlistComments {Komentari}
translate Y GlistAnnos {Napomene}
translate Y GlistStart {Start} ;# ***
translate Y GlistGameNumber {Broj partije}
# ====== TODO To be translated ======
translate Y GlistAverageElo {Average Elo}
# ====== TODO To be translated ======
translate Y GlistRating {Rating}
translate Y GlistFindText {Naði tekst}
translate Y GlistMoveField {Move} ;# ***
translate Y GlistEditField {Configure} ;# ***
translate Y GlistAddField {Add} ;# ***
translate Y GlistDeleteField {Remove} ;# ***
translate Y GlistWidth {Width} ;# ***
translate Y GlistAlign {Align} ;# ***
# ====== TODO To be translated ======
translate Y GlistAlignL {Align: left}
# ====== TODO To be translated ======
translate Y GlistAlignR {Align: right}
# ====== TODO To be translated ======
translate Y GlistAlignC {Align: center}
translate Y GlistColor {Color} ;# ***
translate Y GlistSep {Separator} ;# ***
# ====== TODO To be translated ======
translate Y GlistCurrentSep {-- Current --}
# ====== TODO To be translated ======
translate Y GlistNewSort {New}
# ====== TODO To be translated ======
translate Y GlistAddToSort {Add}
# ====== TODO To be translated ======
translate Y GsortSort {Sort...}
# ====== TODO To be translated ======
translate Y GsortDate {Date}
# ====== TODO To be translated ======
translate Y GsortYear {Year}
# ====== TODO To be translated ======
translate Y GsortEvent {Event}
# ====== TODO To be translated ======
translate Y GsortSite {Site}
# ====== TODO To be translated ======
translate Y GsortRound {Round}
# ====== TODO To be translated ======
translate Y GsortWhiteName {White Name}
# ====== TODO To be translated ======
translate Y GsortBlackName {Black Name}
# ====== TODO To be translated ======
translate Y GsortECO {ECO}
# ====== TODO To be translated ======
translate Y GsortResult {Result}
# ====== TODO To be translated ======
translate Y GsortMoveCount {Move Count}
# ====== TODO To be translated ======
translate Y GsortAverageElo {Average Elo}
# ====== TODO To be translated ======
translate Y GsortCountry {Country}
# ====== TODO To be translated ======
translate Y GsortDeleted {Deleted}
# ====== TODO To be translated ======
translate Y GsortEventDate {Event Date}
# ====== TODO To be translated ======
translate Y GsortWhiteElo {White Elo}
# ====== TODO To be translated ======
translate Y GsortBlackElo {Black Elo}
# ====== TODO To be translated ======
translate Y GsortComments {Comments}
# ====== TODO To be translated ======
translate Y GsortVariations {Variations}
# ====== TODO To be translated ======
translate Y GsortNAGs {NAGs}
# ====== TODO To be translated ======
translate Y GsortAscending {Ascending}
# ====== TODO To be translated ======
translate Y GsortDescending {Descending}
# ====== TODO To be translated ======
translate Y GsortAdd {Add}
# ====== TODO To be translated ======
translate Y GsortStore {Store}
# ====== TODO To be translated ======
translate Y GsortLoad {Load}
# ====== TODO To be translated ======
translate Y GlistRemoveThisGameFromFilter  {Remove this game from Filter}
# ====== TODO To be translated ======
translate Y GlistRemoveGameAndAboveFromFilter  {Remove game (and all above it) from Filter}
# ====== TODO To be translated ======
translate Y GlistRemoveGameAndBelowFromFilter  {Remove game (and all below it) from Filter}
# ====== TODO To be translated ======
translate Y GlistDeleteGame {(Un)Delete this game} 
# ====== TODO To be translated ======
translate Y GlistDeleteAllGames {Delete all games in filter} 
# ====== TODO To be translated ======
translate Y GlistUndeleteAllGames {Undelete all games in filter} 
# ====== TODO To be translated ======
translate Y GlistMergeGameInBase {Merge Game in other base} 

# Maintenance window:
translate Y DatabaseName {Ime baze:}
translate Y TypeIcon {Ikona:}
translate Y NumOfGames {Partija:}
translate Y NumDeletedGames {Obrisanih partija:}
translate Y NumFilterGames {Partija u filteru:}
translate Y YearRange {Raspon godina:}
translate Y RatingRange {Raspon rejtinga:}
translate Y Description {Description} ;# ***
translate Y Flag {Oznaka}
# ====== TODO To be translated ======
translate Y CustomFlags {Custom flags}
translate Y DeleteCurrent {Obri¹i tekuæu partiju}
translate Y DeleteFilter {Obri¹i filtrirane partije}
translate Y DeleteAll {Obri¹i sve partije}
translate Y UndeleteCurrent {Obnovi tekuæu partiju}
translate Y UndeleteFilter {Obnovi filtrirane partije}
translate Y UndeleteAll {Obnovi sve partije}
translate Y DeleteTwins {Obri¹i udvojene partije}
translate Y MarkCurrent {Markiraj tekuæu partiju}
translate Y MarkFilter {Markiraj filtrirane partije}
translate Y MarkAll {Markiraj sve partije}
translate Y UnmarkCurrent {Demarkiraj tekuæu partiju}
translate Y UnmarkFilter {Demarkiraj filtrirane partije}
translate Y UnmarkAll {Demarkiraj sve partije}
translate Y Spellchecking {Provera pravopisa}
translate Y Players {Igraèi}
translate Y Events {Turniri}
translate Y Sites {Mesta}
translate Y Rounds {Kola}
translate Y DatabaseOps {Operacije sa bazom}
translate Y ReclassifyGames {Klasifikuj partije po E©O}
translate Y CompactDatabase {Komprimuj bazu}
translate Y SortDatabase {Sortiraj bazu}
translate Y AddEloRatings {Dodaj Elo rejtinge}
translate Y AutoloadGame {Autoload game number} ;# ***
translate Y StripTags {Strip PGN tags} ;# ***
translate Y StripTag {Strip tag} ;# ***
translate Y Cleaner {Èistaè}
translate Y CleanerHelp {
Scid Èistaè æe da primeni sve akcije odr¾avanja koje izaberete iz donje liste, na tekuæu bazu.

Trenutna pode¹avanja u dijalozima E©O klasifikacija i brisanje udvojenih partija æe se primeniti ako ste izabrali te funkcije.
}
translate Y CleanerConfirm {
Kada je Èistaè jednom pokrenut, ne mo¾e se prekinuti!

Ovo mo¾e potrajati na velikoj bazi, zavisno od funkcija koje ste izabrali i njihovih trenutnih pode¹avanja.

Da li ste sigurni da ¾elite da poènete funkcije odr¾avanja koje ste izabrali?
}
# ====== TODO To be translated ======
translate Y TwinCheckUndelete {to flip; "u" undeletes both)}
# ====== TODO To be translated ======
translate Y TwinCheckprevPair {Previous pair}
# ====== TODO To be translated ======
translate Y TwinChecknextPair {Next pair}
# ====== TODO To be translated ======
translate Y TwinChecker {Scid: Twin game checker}
# ====== TODO To be translated ======
translate Y TwinCheckTournament {Games in tournament:}
# ====== TODO To be translated ======
translate Y TwinCheckNoTwin {No twin  }
# ====== TODO To be translated ======
translate Y TwinCheckNoTwinfound {No twin was detected for this game.\nTo show twins using this window, you must first use the "Delete twin games..." function. }
# ====== TODO To be translated ======
translate Y TwinCheckTag {Share tags...}
# ====== TODO To be translated ======
translate Y TwinCheckFound1 {Scid found $result twin games}
# ====== TODO To be translated ======
translate Y TwinCheckFound2 { and set their delete flags}
# ====== TODO To be translated ======
translate Y TwinCheckNoDelete {There are no games in this database to delete.}
# ====== TODO To be translated ======
translate Y TwinCriteria1 { Your settings for finding twin games are potentially likely to\ncause non-twin games with similar moves to be marked as twins.}
# ====== TODO To be translated ======
translate Y TwinCriteria2 {It is recommended that if you select "No" for "same moves", you should select "Yes" for the colors, event, site, round, year and month settings.\nDo you want to continue and delete twins anyway? }
# ====== TODO To be translated ======
translate Y TwinCriteria3 {It is recommended that you specify "Yes" for at least two of the "same site", "same round" and "same year" settings.\nDo you want to continue and delete twins anyway?}
# ====== TODO To be translated ======
translate Y TwinCriteriaConfirm {Scid: Confirm twin settings}
# ====== TODO To be translated ======
translate Y TwinChangeTag "Change the following game tags:\n\n"
# ====== TODO To be translated ======
translate Y AllocRatingDescription "This command will use the current spellcheck file to add Elo ratings to games in this database. Wherever a player has no currrent rating but his/her rating at the time of the game is listed in the spellcheck file, that rating will be added."
# ====== TODO To be translated ======
translate Y RatingOverride "Overwrite existing non-zero ratings?"
# ====== TODO To be translated ======
translate Y AddRatings "Add ratings to:"
# ====== TODO To be translated ======
translate Y AddedRatings {Scid added $r Elo ratings in $g games.}
# ====== TODO To be translated ======
translate Y NewSubmenu "New submenu"

# Comment editor:
translate Y AnnotationSymbols  {Simboli napomena:}
translate Y Comment {Komentari:}
translate Y InsertMark {Insert mark} ;# ***
translate Y InsertMarkHelp {
Insert/remove mark: Select color, type, square.
Insert/remove arrow: Right-click two squares.
} ;# ***

# Nag buttons in comment editor:
translate Y GoodMove {Good move} ;# ***
translate Y PoorMove {Poor move} ;# ***
translate Y ExcellentMove {Excellent move} ;# ***
translate Y Blunder {Blunder} ;# ***
translate Y InterestingMove {Interesting move} ;# ***
translate Y DubiousMove {Dubious move} ;# ***
translate Y WhiteDecisiveAdvantage {White has a decisive advantage} ;# ***
translate Y BlackDecisiveAdvantage {Black has a decisive advantage} ;# ***
translate Y WhiteClearAdvantage {White has a clear advantage} ;# ***
translate Y BlackClearAdvantage {Black has a clear advantage} ;# ***
translate Y WhiteSlightAdvantage {White has a slight advantage} ;# ***
translate Y BlackSlightAdvantage {Black has a slight advantage} ;# ***
translate Y Equality {Equality} ;# ***
translate Y Unclear {Unclear} ;# ***
translate Y Diagram {Diagram} ;# ***

# Board search:
translate Y BoardSearch {Pretraga po poziciji}
translate Y FilterOperation {Operacije na tekuæem filteru:}
translate Y FilterAnd {I (Ogranièi filter)}
translate Y FilterOr {ILI (Dodaj u filter)}
translate Y FilterIgnore {IGNORI©I (Resetuj filter)}
translate Y SearchType {Tip pretrage:}
translate Y SearchBoardExact {Ista pozicija (sve figure na istim poljima)}
translate Y SearchBoardPawns {Pe¹aci (isti materijal, svi pe¹aci na istim poljima)}
translate Y SearchBoardFiles {Linije (isti materijal, svi pe¹aci na istim linijama)}
translate Y SearchBoardAny {Materijal (isti materijal, pe¹aci i figure bilo gde)}
# ====== TODO To be translated ======
translate Y SearchInRefDatabase { Search in base }
translate Y LookInVars {Pogledaj u varijantama}

# Material search:
translate Y MaterialSearch {Pretraga po materijalu}
translate Y Material {Materijal}
translate Y Patterns {Pozicije}
translate Y Zero {Ni¹ta}
translate Y Any {Bilo koje}
translate Y CurrentBoard {Trenutna pozicija}
translate Y CommonEndings {Tipiène zavr¹nice}
translate Y CommonPatterns {Tipiène pozicije}
translate Y MaterialDiff {Material difference} ;# ***
translate Y squares {polja}
translate Y SameColor {Ista boja}
translate Y OppColor {Suprotna boja}
translate Y Either {Oba}
translate Y MoveNumberRange {Raspon broja poteza}
translate Y MatchForAtLeast {Naði za najmanje}
translate Y HalfMoves {polupoteza}

# Common endings in material search:
translate Y EndingPawns {Pawn endings} ;# ***
translate Y EndingRookVsPawns {Rook vs. Pawn(s)} ;# ***
translate Y EndingRookPawnVsRook {Rook and 1 Pawn vs. Rook} ;# ***
translate Y EndingRookPawnsVsRook {Rook and Pawn(s) vs. Rook} ;# ***
translate Y EndingRooks {Rook vs. Rook endings} ;# ***
translate Y EndingRooksPassedA {Rook vs. Rook endings with a passed a-pawn} ;# ***
translate Y EndingRooksDouble {Double Rook endings} ;# ***
translate Y EndingBishops {Bishop vs. Bishop endings} ;# ***
translate Y EndingBishopVsKnight {Bishop vs. Knight endings} ;# ***
translate Y EndingKnights {Knight vs. Knight endings} ;# ***
translate Y EndingQueens {Queen vs. Queen endings} ;# ***
translate Y EndingQueenPawnVsQueen {Queen and 1 Pawn vs. Queen} ;# ***
translate Y BishopPairVsKnightPair {Two Bishops vs. Two Knights middlegame} ;# ***

# Common patterns in material search:
translate Y PatternWhiteIQP {White IQP} ;# ***
translate Y PatternWhiteIQPBreakE6 {White IQP: d4-d5 break vs. e6} ;# ***
translate Y PatternWhiteIQPBreakC6 {White IQP: d4-d5 break vs. c6} ;# ***
translate Y PatternBlackIQP {Black IQP} ;# ***
translate Y PatternWhiteBlackIQP {White IQP vs. Black IQP} ;# ***
translate Y PatternCoupleC3D4 {White c3+d4 Isolated Pawn Couple} ;# ***
translate Y PatternHangingC5D5 {Black Hanging Pawns on c5 and d5} ;# ***
translate Y PatternMaroczy {Maroczy Center (with Pawns on c4 and e4)} ;# ***
translate Y PatternRookSacC3 {Rook Sacrifice on c3} ;# ***
translate Y PatternKc1Kg8 {O-O-O vs. O-O (Kc1 vs. Kg8)} ;# ***
translate Y PatternKg1Kc8 {O-O vs. O-O-O (Kg1 vs. Kc8)} ;# ***
translate Y PatternLightFian {Light-Square Fianchettos (Bishop-g2 vs. Bishop-b7)} ;# ***
translate Y PatternDarkFian {Dark-Square Fianchettos (Bishop-b2 vs. Bishop-g7)} ;# ***
translate Y PatternFourFian {Four Fianchettos (Bishops on b2,g2,b7,g7)} ;# ***

# Game saving:
translate Y Today {Danas}
translate Y ClassifyGame {Klasifikuj partiju}

# Setup position:
translate Y EmptyBoard {Isprazni tablu}
translate Y InitialBoard {Poèetna pozicija}
translate Y SideToMove {Na potezu je}
translate Y MoveNumber {Broj poteza}
translate Y Castling {Rokada}
translate Y EnPassantFile {En Passant linija}
translate Y ClearFen {Oèisti FEN}
translate Y PasteFen {Prenesi FEN}
# ====== TODO To be translated ======
translate Y SaveAndContinue {Save and continue}
# ====== TODO To be translated ======
translate Y DiscardChangesAndContinue {Discard changes\nand continue}
# ====== TODO To be translated ======
translate Y GoBack {Go back}

# Replace move dialog:
translate Y ReplaceMove {Zameni potez}
translate Y AddNewVar {Dodaj novu varijantu}
# ====== TODO To be translated ======
translate Y NewMainLine {New Main Line}
translate Y ReplaceMoveMessage {Ovde veæ postoji potez.

Mo¾ete ga zameniti, odbacujuæi sve poteze posle njega, ili dodajte svoj potez kao novu varijantu.

(Mo¾ete izbeæi pojavljivanje ove poruke ubuduæe ako iskljuèite "Pitaj pre zamene poteza" opciju u meniju Opcije:Potezi.)}

# Make database read-only dialog:
translate Y ReadOnlyDialog {Ako omoguæite samo èitanje iz ove baze, neæe biti dozvoljene izmene.
Nijedna partije neæe biti saèuvana ili zamenjena, i nijedan znak za brisanje ne mo¾e biti izmenjen.
Bilo kakvo sortiranje ili rezultati E©O klasifikacije æe biti privremeni.

Mo¾ete lako ponovo omoguæiti pisanje po bazi, ako je zatvorite i ponovo otvorite.

Da li zaista ¾elite da ova baza bude samo za èitanje?}

# Clear game dialog:
translate Y ClearGameDialog {Ova partija je izmenjena.

Da li zaista ¾elite da nastavite i odbacite sve uèinjene promene?
}

# Exit dialog:
translate Y ExitDialog {Da li zaista ¾elite da napustite Scid?}
translate Y ExitUnsaved {The following databases have unsaved game changes. If you exit now, these changes will be lost.} ;# ***

# Import window:
translate Y PasteCurrentGame {Prenesi tekuæu partiju}
translate Y ImportHelp1 {Unesi ili prenesi partiju u PGN formatu u gornji okvir.}
translate Y ImportHelp2 {Bilo koja gre¹ka u uvozu partije æe biti prikazana ovde.}
# ====== TODO To be translated ======
translate Y OverwriteExistingMoves {Overwrite existing moves ?}

# ECO Browser:
translate Y ECOAllSections {sve E©O sekcije}
translate Y ECOSection {E©O sekcija}
translate Y ECOSummary {Pregled za}
translate Y ECOFrequency {Uèestanost subkodova za}

# Opening Report:
translate Y OprepTitle {Izve¹taj o otvaranjima}
translate Y OprepReport {Izve¹taj}
translate Y OprepGenerated {Generisao}
translate Y OprepStatsHist {Statistika i Istorija}
translate Y OprepStats {Statistika}
translate Y OprepStatAll {Sve partije iz iz izve¹taja}
translate Y OprepStatBoth {Oba sa rejtingom}
translate Y OprepStatSince {Od}
translate Y OprepOldest {Najstarije partije}
translate Y OprepNewest {Najnovije partije}
translate Y OprepPopular {Trenutna popularnost}
translate Y OprepFreqAll {Uèetanost za sve godine:    }
translate Y OprepFreq1   {Tokom  1 poslednje godine:  }
translate Y OprepFreq5   {Tokom  5 poslednjih godina: }
translate Y OprepFreq10  {Tokom 10 poslednjih godina: }
translate Y OprepEvery {jednom svakih %u partija}
translate Y OprepUp {vi¹e za %u%s tokom svih godina}
translate Y OprepDown {manje za %u%s tokom svih godina}
translate Y OprepSame {bez promena tokom svih godina}
translate Y OprepMostFrequent {Najèe¹æi igraèi}
translate Y OprepMostFrequentOpponents {Most frequent opponents} ;# ***
translate Y OprepRatingsPerf {Rejtinzi i ume¹nost}
translate Y OprepAvgPerf {Proseèni rejtinzi i ume¹nost}
translate Y OprepWRating {Rejting belog}
translate Y OprepBRating {Rejting crnog}
translate Y OprepWPerf {Ume¹nost belog}
translate Y OprepBPerf {Ume¹nost crnog}
translate Y OprepHighRating {Partije sa najveæim proseènim rejtingom}
translate Y OprepTrends {Rezultatski trendovi}
translate Y OprepResults {Du¾ine rezultata i uèestanosti}
translate Y OprepLength {Du¾ina partije}
translate Y OprepFrequency {Uèestanost}
translate Y OprepWWins {Pobede belog: }
translate Y OprepBWins {Pobede crnog: }
translate Y OprepDraws {Remiji:       }
translate Y OprepWholeDB {cela baza}
translate Y OprepShortest {Najkraæa pobeda} ;# ***
# translate Y OprepShortWhite {Najkraæa pobeda belog}
# translate Y OprepShortBlack {Najkraæa pobeda crnog}
translate Y OprepMovesThemes {Potezi i teme}
translate Y OprepMoveOrders {Redosledi poteza koji su dosegli poziciju iz izve¹taja}
translate Y OprepMoveOrdersOne \
  {Samo je jedan redosled poteza koji je dostigao ovu poziciju:}
translate Y OprepMoveOrdersAll \
  {Bilo je %u redosleda poteza koji su dostigli ovu poziciju:}
translate Y OprepMoveOrdersMany \
  {Bilo je %u redosleda poteza koji su dostigli ovu poziciju. Prvih %u su:}
translate Y OprepMovesFrom {Potezi iz pozicije iz izve¹taja}
translate Y OprepMostFrequentEcoCodes {Most frequent ECO codes} ;# ***
translate Y OprepThemes {Pozicione teme}
translate Y OprepThemeDescription {Frequency of themes in the first %u moves of each game} ;# ***
translate Y OprepThemeSameCastling {Rokade sa iste strane}
translate Y OprepThemeOppCastling {Rokade sa suprotne strane}
translate Y OprepThemeNoCastling {Obe strane bez rokada}
translate Y OprepThemeKPawnStorm {Prodor pe¹aka na kraljevom krilu}
translate Y OprepThemeQueenswap {Zamena dama}
translate Y OprepThemeWIQP {White Isolated Queen Pawn} ;# ***
translate Y OprepThemeBIQP {Black Isolated Queen Pawn} ;# ***
translate Y OprepThemeWP567 {Beli pe¹ak na 5., 6. ili 7. redu}
translate Y OprepThemeBP234 {Crni pe¹ak na 2., 3. ili 4. redu}
translate Y OprepThemeOpenCDE {Otvorena c, d ili e linija}
translate Y OprepTheme1BishopPair {Jedna strana ima par lovaca}
translate Y OprepEndgames {Zavr¹nice}
translate Y OprepReportGames {Partije iz izve¹taja}
translate Y OprepAllGames {Sve partije}
translate Y OprepEndClass {Materijal na kraju svake partije}
translate Y OprepTheoryTable {Tabela teorije}
translate Y OprepTableComment {Generisano iz %u najbolje rejtiranih partija.}
translate Y OprepExtraMoves {Posebno notirani potezi u tabeli teorije}
translate Y OprepMaxGames {Maksimum partija u tabeli teorije}
translate Y OprepViewHTML {View HTML} ;# ***
translate Y OprepViewLaTeX {View LaTeX} ;# ***

# Player Report:
translate Y PReportTitle {Player Report} ;# ***
translate Y PReportColorWhite {with the White pieces} ;# ***
translate Y PReportColorBlack {with the Black pieces} ;# ***
translate Y PReportMoves {after %s} ;# ***
translate Y PReportOpenings {Openings} ;# ***
translate Y PReportClipbase {Empty clipbase and copy matching games to it} ;# ***

# Piece Tracker window:
translate Y TrackerSelectSingle {Left mouse button selects this piece.} ;# ***
translate Y TrackerSelectPair {Left mouse button selects this piece; right button also selects its sibling.}
translate Y TrackerSelectPawn {Left mouse button selects this pawn; right button selects all 8 pawns.}
translate Y TrackerStat {Statistic}
translate Y TrackerGames {% games with move to square}
translate Y TrackerTime {% time on each square}
translate Y TrackerMoves {Moves}
translate Y TrackerMovesStart {Enter the move number where tracking should begin.}
translate Y TrackerMovesStop {Enter the move number where tracking should stop.}

# Game selection dialogs:
translate Y SelectAllGames {Sve partije u bazi}
translate Y SelectFilterGames {Samo partije u filteru}
translate Y SelectTournamentGames {Samo partije u tekuæem turniru}
translate Y SelectOlderGames {Samo starije partije}

# Delete Twins window:
translate Y TwinsNote {Da bi bile udvojene, dve partije moraju da najmanje imaju istu dvojicu igraèa, i kriterijume koje ni¾e mo¾ete podesiti. Kada se par udvojenih partija otkrije, kraæa partija se bri¹e. Savet: Najbolje je proveriti pravopis u bazi pre brisanja udvojenih partija, jer to unapreðuje detekciju. }
translate Y TwinsCriteria {Kriterijumi: Udvojene partije moraju imati...}
translate Y TwinsWhich {Koje partije da proverim?}
translate Y TwinsColors {Iste boje igraèa?}
translate Y TwinsEvent {Isti turnir?}
translate Y TwinsSite {Isto mesto?}
translate Y TwinsRound {Isto kolo?}
translate Y TwinsYear {Ista godina?}
translate Y TwinsMonth {Isti mesec?}
translate Y TwinsDay {Isti dan?}
translate Y TwinsResult {Isti rezultat?}
translate Y TwinsECO {Isti E©O kôd?}
translate Y TwinsMoves {Isti potezi?}
translate Y TwinsPlayers {Uporeðivanje imena igraèa:}
translate Y TwinsPlayersExact {Potpuna podudarnost}
translate Y TwinsPlayersPrefix {Samo prva 4 slova}
translate Y TwinsWhen {Kod brisanja udvojenih partija}
translate Y TwinsSkipShort {Ignore all games under 5 moves long?} ;# ***
translate Y TwinsUndelete {Obnovi prvo sve partije?}
translate Y TwinsSetFilter {Postavi filter na sve obrisane udvojene partije?}
translate Y TwinsComments {Uvek saèuvaj partije sa komentarima?}
translate Y TwinsVars {Uvek saèuvaj partije sa varijantama?}
translate Y TwinsDeleteWhich {Delete which game:} ;# ***
translate Y TwinsDeleteShorter {Shorter game} ;# ***
translate Y TwinsDeleteOlder {Smaller game number} ;# ***
translate Y TwinsDeleteNewer {Larger game number} ;# ***
translate Y TwinsDelete {Obri¹i partije}

# Name editor window:
translate Y NameEditType {Tip imena za izmenu}
translate Y NameEditSelect {Partije za izmenu}
translate Y NameEditReplace {Zameni}
translate Y NameEditWith {sa}
translate Y NameEditMatches {Podudara se: Pritisni Ctrl+1 do Ctrl+9 za izbor}
# ====== TODO To be translated ======
translate Y CheckGames {Check games}
# ====== TODO To be translated ======
translate Y CheckGamesWhich {Check games}
# ====== TODO To be translated ======
translate Y CheckAll {All games}
# ====== TODO To be translated ======
translate Y CheckSelectFilterGames {Only games in filter}

# Classify window:
translate Y Classify {Klasifikuj}
translate Y ClassifyWhich {Klasifikuj partije po E©O}
translate Y ClassifyAll {Sve partije (prebri¹i stare E©O kôdove)}
translate Y ClassifyYear {Sve partije igrane poslednje godine}
translate Y ClassifyMonth {Sve partije igrane poslednjeg meseca}
translate Y ClassifyNew {Samo partije koje su jo¹ bez E©O kôda}
translate Y ClassifyCodes {Koristi E©O kôdove}
translate Y ClassifyBasic {Samo osnovne kôdove ("B12", ...)}
translate Y ClassifyExtended {Pro¹ireni Scid ("B12j", ...)}

# Compaction:
translate Y NameFile {Fajl imena}
translate Y GameFile {Fajl partija}
translate Y Names {Imena}
translate Y Unused {Nekori¹æena}
translate Y SizeKb {Velièina (kb)}
translate Y CurrentState {Trenutno stanje}
translate Y AfterCompaction {Posle komprimacije}
translate Y CompactNames {Komprimuj fajl imena}
translate Y CompactGames {Komprimuj fajl partija}
# ====== TODO To be translated ======
translate Y NoUnusedNames "There are no unused names, so the name file is already fully compacted."
# ====== TODO To be translated ======
translate Y NoUnusedGames "The game file is already fully compacted."
# ====== TODO To be translated ======
translate Y GameFileCompacted {The game file for the database was compacted.}

# Sorting:
translate Y SortCriteria {Kriterijumi}
translate Y AddCriteria {Dodaj kriterijum}
translate Y CommonSorts {Uobièajena sortiranja}
translate Y Sort {Sortiraj}

# Exporting:
translate Y AddToExistingFile {Dodaj partije u postojeæi fajl?}
translate Y ExportComments {Izvezi komentare?}
translate Y ExportVariations {Izvezi varijante?}
translate Y IndentComments {Uvuci pasus kod komentara?}
translate Y IndentVariations {Uvuci pasus kod varijanata?}
translate Y ExportColumnStyle {Pasus stil (jedan potez po liniji)?}
translate Y ExportSymbolStyle {Stil simbolièkih napomena:}
translate Y ExportStripMarks {Strip square/arrow mark codes from comments?} ;# ***

# Goto game/move dialogs:
translate Y LoadGameNumber {Unesi broj partije za uèitavanje:}
translate Y GotoMoveNumber {Idi na potez broj:}

# Copy games dialog:
translate Y CopyGames {Kopiraj partije}
translate Y CopyConfirm {
 Da li zaista ¾elite da kopirate
 [::utils::thousands $nGamesToCopy] filtriranih partija
 iz baze "$fromName"
 u bazu "$targetName"?
}
translate Y CopyErr {Ne mogu da kopiram partije}
translate Y CopyErrSource {izvori¹na baza}
translate Y CopyErrTarget {odredi¹na baza}
translate Y CopyErrNoGames {nema partija u svom filteru}
translate Y CopyErrReadOnly {je samo za èitanje}
translate Y CopyErrNotOpen {nije otvorena}

# Colors:
translate Y LightSquares {Svetla polja}
translate Y DarkSquares {Tamna polja}
translate Y SelectedSquares {Izabrana polja}
translate Y SuggestedSquares {Polja preporuèenog poteza}
translate Y WhitePieces {Bele figure}
translate Y BlackPieces {Crne figure}
translate Y WhiteBorder {Ivica belih figura}
translate Y BlackBorder {Ivica crnih figura}

# Novelty window:
translate Y FindNovelty {Pronaði novost}
translate Y Novelty {Novost}
translate Y NoveltyInterrupt {Tra¾enje novosti prekinuto}
translate Y NoveltyNone {Nijedna novost nije pronaðena u ovoj partiji}
translate Y NoveltyHelp {
Scid æe naæi prvi potez tekuæe partije, koji dosti¾e poziciju koja nije naðena u izabranoj bazi ili u E©O knjizi otvaranja.
}

# Sounds configuration:
translate Y SoundsFolder {Sound Files Folder} ;# ***
translate Y SoundsFolderHelp {The folder should contain the files King.wav, a.wav, 1.wav, etc} ;# ***
translate Y SoundsAnnounceOptions {Move Announcement Options} ;# ***
translate Y SoundsAnnounceNew {Announce new moves as they are made} ;# ***
translate Y SoundsAnnounceForward {Announce moves when moving forward one move} ;# ***
translate Y SoundsAnnounceBack {Announce when retracting or moving back one move} ;# ***
# ====== TODO To be translated ======
translate Y SoundsSoundDisabled {Scid could not find the Snack audio package at startup;\nSound is disabled.}

# Upgrading databases:
translate Y Upgrading {Unapreðivanje}
translate Y ConfirmOpenNew {
Ovo je baza u starom formatu (Scid 3), koju ne mo¾e da otvori Scid 4, veæ je kreirana verzija u novom formatu (Scid 4).

Da li ¾elite da otvorite verziju baze u novom formatu?
}
translate Y ConfirmUpgrade {
Ovo je baza u starom formatu (Scid 3). Verzija baze u novom formatu mora biti kreirana pre kori¹æenja u Scid-u 4.

Unapreðivanjem æe se kreirati nova verzija baze.

Ovo mo¾e potrajati, ali treba da se uradi samo jednom. Mo¾ete prekinuti ukoliko bude predugo trajalo.

Da li ¾elite da sada unapredite ovu bazu?
}

# Recent files options:
translate Y RecentFilesMenu {Number of recent files in File menu} ;# ***
translate Y RecentFilesExtra {Number of recent files in extra submenu} ;# ***

# My Player Names options:
translate Y MyPlayerNamesDescription {
Enter a list of preferred player names below, one name per line. Wildcards (e.g. "?" for any single character, "*" for any sequence of characters) are permitted.

Every time a game with a player in the list is loaded, the main window chessboard will be rotated if necessary to show the game from that players perspective.
} ;# ***
# ====== TODO To be translated ======
translate Y showblunderexists {show blunder exists}
# ====== TODO To be translated ======
translate Y showblundervalue {show blunder value}
# ====== TODO To be translated ======
translate Y showscore {show score}
# ====== TODO To be translated ======
translate Y coachgame {coach game}
# ====== TODO To be translated ======
translate Y configurecoachgame {configure coach game}
# ====== TODO To be translated ======
translate Y configuregame {Game configuration}
# ====== TODO To be translated ======
translate Y Phalanxengine {Phalanx engine}
# ====== TODO To be translated ======
translate Y Coachengine {Coach engine}
# ====== TODO To be translated ======
translate Y difficulty {difficulty}
# ====== TODO To be translated ======
translate Y hard {hard}
# ====== TODO To be translated ======
translate Y easy {easy}
# ====== TODO To be translated ======
translate Y Playwith {Play with}
# ====== TODO To be translated ======
translate Y white {white}
# ====== TODO To be translated ======
translate Y black {black}
# ====== TODO To be translated ======
translate Y both {both}
# ====== TODO To be translated ======
translate Y Play {Play}
# ====== TODO To be translated ======
translate Y Noblunder {No blunder}
# ====== TODO To be translated ======
translate Y blunder {blunder}
# ====== TODO To be translated ======
translate Y Noinfo {-- No info --}
# ====== TODO To be translated ======
translate Y PhalanxOrTogaMissing {Phalanx or Toga not found}
# ====== TODO To be translated ======
translate Y moveblunderthreshold {move is a blunder if loss is greater than}
# ====== TODO To be translated ======
translate Y limitanalysis {limit engine analysis time}
# ====== TODO To be translated ======
translate Y seconds {seconds}
# ====== TODO To be translated ======
translate Y Abort {Abort}
# ====== TODO To be translated ======
translate Y Resume {Resume}
# ====== TODO To be translated ======
translate Y OutOfOpening {Out of opening}
# ====== TODO To be translated ======
translate Y NotFollowedLine {You did not follow the line}
# ====== TODO To be translated ======
translate Y DoYouWantContinue {Do you want yo continue ?}
# ====== TODO To be translated ======
translate Y CoachIsWatching {Coach is watching}
# ====== TODO To be translated ======
translate Y Ponder {Permanent thinking}
# ====== TODO To be translated ======
translate Y LimitELO {Limit ELO strength}
# ====== TODO To be translated ======
translate Y DubiousMovePlayedTakeBack {Dubious move played, do you want to take back ?}
# ====== TODO To be translated ======
translate Y WeakMovePlayedTakeBack {Weak move played, do you want to take back ?}
# ====== TODO To be translated ======
translate Y BadMovePlayedTakeBack {Bad move played, do you want to take back ?}
# ====== TODO To be translated ======
translate Y Iresign {I resign}
# ====== TODO To be translated ======
translate Y yourmoveisnotgood {your move is not good}
# ====== TODO To be translated ======
translate Y EndOfVar {End of variation}
# ====== TODO To be translated ======
translate Y Openingtrainer {Opening trainer}
# ====== TODO To be translated ======
translate Y DisplayCM {Display candidate moves}
# ====== TODO To be translated ======
translate Y DisplayCMValue {Display candidate moves value}
# ====== TODO To be translated ======
translate Y DisplayOpeningStats {Show statistics}
# ====== TODO To be translated ======
translate Y ShowReport {Show report}
# ====== TODO To be translated ======
translate Y NumberOfGoodMovesPlayed {good moves played}
# ====== TODO To be translated ======
translate Y NumberOfDubiousMovesPlayed {dubious moves played}
# ====== TODO To be translated ======
translate Y NumberOfMovesPlayedNotInRepertoire {moves played not in repertoire}
# ====== TODO To be translated ======
translate Y NumberOfTimesPositionEncountered {times position encountered}
# ====== TODO To be translated ======
translate Y PlayerBestMove  {Allow only best moves}
# ====== TODO To be translated ======
translate Y OpponentBestMove {Opponent plays best moves}
# ====== TODO To be translated ======
translate Y OnlyFlaggedLines {Only flagged lines}
# ====== TODO To be translated ======
translate Y resetStats {Reset statistics}
# ====== TODO To be translated ======
translate Y Repertoiretrainingconfiguration {Repertoire training configuration}
# ====== TODO To be translated ======
translate Y Loadingrepertoire {Loading repertoire}
# ====== TODO To be translated ======
translate Y Movesloaded {Moves loaded}
# ====== TODO To be translated ======
translate Y Repertoirenotfound {Repertoire not found}
# ====== TODO To be translated ======
translate Y Openfirstrepertoirewithtype {Open first a repertoire database with icon/type set to the right side}
# ====== TODO To be translated ======
translate Y Movenotinrepertoire {Move not in repertoire}
# ====== TODO To be translated ======
translate Y PositionsInRepertoire {Positions in repertoire}
# ====== TODO To be translated ======
translate Y PositionsNotPlayed {Positions not played}
# ====== TODO To be translated ======
translate Y PositionsPlayed {Positions played}
# ====== TODO To be translated ======
translate Y Success {Success}
# ====== TODO To be translated ======
translate Y DubiousMoves {Dubious moves}
# ====== TODO To be translated ======
translate Y OutOfRepertoire {OutOfRepertoire}
# ====== TODO To be translated ======
translate Y ConfigureTactics {Configure tactics}
# ====== TODO To be translated ======
translate Y ResetScores {Reset scores}
# ====== TODO To be translated ======
translate Y LoadingBase {Loading base}
# ====== TODO To be translated ======
translate Y Tactics {Tactics}
# ====== TODO To be translated ======
translate Y ShowSolution {Show solution}
# ====== TODO To be translated ======
translate Y NextExercise {Next exercise}
# ====== TODO To be translated ======
translate Y PrevExercise {previous exercise}
# ====== TODO To be translated ======
translate Y StopTraining {Stop training}
# ====== TODO To be translated ======
translate Y Next {Next}
# ====== TODO To be translated ======
translate Y ResettingScore {Resetting score}
# ====== TODO To be translated ======
translate Y LoadingGame {Loading game}
# ====== TODO To be translated ======
translate Y MateFound {Mate found}
# ====== TODO To be translated ======
translate Y BestSolutionNotFound {Best solution NOT found !}
# ====== TODO To be translated ======
translate Y MateNotFound {Mate not found}
# ====== TODO To be translated ======
translate Y ShorterMateExists {Shorter mate exists}
# ====== TODO To be translated ======
translate Y ScorePlayed {Score played}
# ====== TODO To be translated ======
translate Y Expected {expected}
# ====== TODO To be translated ======
translate Y ChooseTrainingBase {Choose training base}
# ====== TODO To be translated ======
translate Y Thinking {Thinking}
# ====== TODO To be translated ======
translate Y AnalyzeDone {Analyze done}
# ====== TODO To be translated ======
translate Y WinWonGame {Win won game}
# ====== TODO To be translated ======
translate Y Lines {Lines}
# ====== TODO To be translated ======
translate Y ConfigureUCIengine {Configure UCI engine}
# ====== TODO To be translated ======
translate Y SpecificOpening {Specific opening}
# ====== TODO To be translated ======
translate Y StartNewGame {Start new game}
# ====== TODO To be translated ======
translate Y FixedLevel {Fixed level}
# ====== TODO To be translated ======
translate Y Opening {Opening}
# ====== TODO To be translated ======
translate Y RandomLevel {Random level}
# ====== TODO To be translated ======
translate Y StartFromCurrentPosition {Start from current position}
# ====== TODO To be translated ======
translate Y FixedDepth {Fixed depth}
# ====== TODO To be translated ======
translate Y Nodes {Nodes} 
# ====== TODO To be translated ======
translate Y Depth {Depth}
# ====== TODO To be translated ======
translate Y Time {Time} 
# ====== TODO To be translated ======
translate Y SecondsPerMove {Seconds per move}
# ====== TODO To be translated ======
translate Y Engine {Engine}
# ====== TODO To be translated ======
translate Y TimeMode {Time mode}
# ====== TODO To be translated ======
translate Y TimeBonus {Time + bonus}
# ====== TODO To be translated ======
translate Y TimeMin {min}
# ====== TODO To be translated ======
translate Y TimeSec {sec}
# ====== TODO To be translated ======
translate Y AllExercisesDone {All exercises done}
# ====== TODO To be translated ======
translate Y MoveOutOfBook {Move out of book}
# ====== TODO To be translated ======
translate Y LastBookMove {Last book move}
# ====== TODO To be translated ======
translate Y AnnotateSeveralGames {Annotate several games\nfrom current to :}
# ====== TODO To be translated ======
translate Y FindOpeningErrors {Find opening errors}
# ====== TODO To be translated ======
translate Y MarkTacticalExercises {Mark tactical exercises}
# ====== TODO To be translated ======
translate Y UseBook {Use book}
# ====== TODO To be translated ======
translate Y MultiPV {Multiple variations}
# ====== TODO To be translated ======
translate Y Hash {Hash memory}
# ====== TODO To be translated ======
translate Y OwnBook {Use engine book}
# ====== TODO To be translated ======
translate Y BookFile {Opening book}
# ====== TODO To be translated ======
translate Y AnnotateVariations {Annotate variations}
# ====== TODO To be translated ======
translate Y ShortAnnotations {Short annotations}
# ====== TODO To be translated ======
translate Y addAnnotatorTag {Add annotator tag}
# ====== TODO To be translated ======
translate Y AddScoreToShortAnnotations {Add score to short annotations}
# ====== TODO To be translated ======
translate Y Export {Export}
# ====== TODO To be translated ======
translate Y BookPartiallyLoaded {Book partially loaded}
# ====== TODO To be translated ======
translate Y Calvar {Calculation of variations}
# ====== TODO To be translated ======
translate Y ConfigureCalvar {Configuration}
# ====== TODO To be translated ======
translate Y Reti {Reti}
# ====== TODO To be translated ======
translate Y English {English}
# ====== TODO To be translated ======
translate Y d4Nf6Miscellaneous {1.d4 Nf6 Miscellaneous}
# ====== TODO To be translated ======
translate Y Trompowsky {Trompowsky}
# ====== TODO To be translated ======
translate Y Budapest {Budapest}
# ====== TODO To be translated ======
translate Y OldIndian {Old Indian}
# ====== TODO To be translated ======
translate Y BenkoGambit {Benko Gambit}
# ====== TODO To be translated ======
translate Y ModernBenoni {Modern Benoni}
# ====== TODO To be translated ======
translate Y DutchDefence {Dutch Defence}
# ====== TODO To be translated ======
translate Y Scandinavian {Scandinavian}
# ====== TODO To be translated ======
translate Y AlekhineDefence {Alekhine Defence}
# ====== TODO To be translated ======
translate Y Pirc {Pirc}
# ====== TODO To be translated ======
translate Y CaroKann {Caro-Kann}
# ====== TODO To be translated ======
translate Y CaroKannAdvance {Caro-Kann Advance}
# ====== TODO To be translated ======
translate Y Sicilian {Sicilian}
# ====== TODO To be translated ======
translate Y SicilianAlapin {Sicilian Alapin}
# ====== TODO To be translated ======
translate Y SicilianClosed {Sicilian Closed}
# ====== TODO To be translated ======
translate Y SicilianRauzer {Sicilian Rauzer}
# ====== TODO To be translated ======
translate Y SicilianDragon {Sicilian Dragon}
# ====== TODO To be translated ======
translate Y SicilianScheveningen {Sicilian Scheveningen}
# ====== TODO To be translated ======
translate Y SicilianNajdorf {Sicilian Najdorf}
# ====== TODO To be translated ======
translate Y OpenGame {Open Game}
# ====== TODO To be translated ======
translate Y Vienna {Vienna}
# ====== TODO To be translated ======
translate Y KingsGambit {King's Gambit}
# ====== TODO To be translated ======
translate Y RussianGame {Russian Game}
# ====== TODO To be translated ======
translate Y ItalianTwoKnights {Italian/Two Knights}
# ====== TODO To be translated ======
translate Y Spanish {Spanish}
# ====== TODO To be translated ======
translate Y SpanishExchange {Spanish Exchange}
# ====== TODO To be translated ======
translate Y SpanishOpen {Spanish Open}
# ====== TODO To be translated ======
translate Y SpanishClosed {Spanish Closed}
# ====== TODO To be translated ======
translate Y FrenchDefence {French Defence}
# ====== TODO To be translated ======
translate Y FrenchAdvance {French Advance}
# ====== TODO To be translated ======
translate Y FrenchTarrasch {French Tarrasch}
# ====== TODO To be translated ======
translate Y FrenchWinawer {French Winawer}
# ====== TODO To be translated ======
translate Y FrenchExchange {French Exchange}
# ====== TODO To be translated ======
translate Y QueensPawn {Queen's Pawn}
# ====== TODO To be translated ======
translate Y Slav {Slav}
# ====== TODO To be translated ======
translate Y QGA {QGA}
# ====== TODO To be translated ======
translate Y QGD {QGD}
# ====== TODO To be translated ======
translate Y QGDExchange {QGD Exchange}
# ====== TODO To be translated ======
translate Y SemiSlav {Semi-Slav}
# ====== TODO To be translated ======
translate Y QGDwithBg5 {QGD with Bg5}
# ====== TODO To be translated ======
translate Y QGDOrthodox {QGD Orthodox}
# ====== TODO To be translated ======
translate Y Grunfeld {Grünfeld}
# ====== TODO To be translated ======
translate Y GrunfeldExchange {Grünfeld Exchange}
# ====== TODO To be translated ======
translate Y GrunfeldRussian {Grünfeld Russian}
# ====== TODO To be translated ======
translate Y Catalan {Catalan}
# ====== TODO To be translated ======
translate Y CatalanOpen {Catalan Open}
# ====== TODO To be translated ======
translate Y CatalanClosed {Catalan Closed}
# ====== TODO To be translated ======
translate Y QueensIndian {Queen's Indian}
# ====== TODO To be translated ======
translate Y NimzoIndian {Nimzo-Indian}
# ====== TODO To be translated ======
translate Y NimzoIndianClassical {Nimzo-Indian Classical}
# ====== TODO To be translated ======
translate Y NimzoIndianRubinstein {Nimzo-Indian Rubinstein}
# ====== TODO To be translated ======
translate Y KingsIndian {King's Indian}
# ====== TODO To be translated ======
translate Y KingsIndianSamisch {King's Indian Sämisch}
# ====== TODO To be translated ======
translate Y KingsIndianMainLine {King's Indian Main Line}
# ====== TODO To be translated ======
translate Y ConfigureFics {Configure FICS}
# ====== TODO To be translated ======
translate Y FICSGuest {Login as Guest}
# ====== TODO To be translated ======
translate Y FICSServerPort {Server port}
# ====== TODO To be translated ======
translate Y FICSServerAddress {IP Address}
# ====== TODO To be translated ======
translate Y FICSRefresh {Refresh}
# ====== TODO To be translated ======
translate Y FICSTimesealPort {Timeseal port}
# ====== TODO To be translated ======
translate Y FICSSilence {Silence}
# ====== TODO To be translated ======
translate Y FICSOffers {Offers}
# ====== TODO To be translated ======
translate Y FICSConsole {Console}
# ====== TODO To be translated ======
translate Y FICSGames {Games}
# ====== TODO To be translated ======
translate Y FICSUnobserve {Stop observing game}
# ====== TODO To be translated ======
translate Y FICSProfile {Display your history and profile}
# ====== TODO To be translated ======
translate Y FICSRelayedGames {Relayed games}
# ====== TODO To be translated ======
translate Y FICSFindOpponent {Find opponent}
# ====== TODO To be translated ======
translate Y FICSTakeback {Takeback}
# ====== TODO To be translated ======
translate Y FICSTakeback2 {Takeback 2}
# ====== TODO To be translated ======
translate Y FICSInitTime {Initial time (min)}
# ====== TODO To be translated ======
translate Y FICSIncrement {Increment (sec)}
# ====== TODO To be translated ======
translate Y FICSRatedGame {Rated Game}
# ====== TODO To be translated ======
translate Y FICSAutoColour {automatic}
# ====== TODO To be translated ======
translate Y FICSManualConfirm {confirm manually}
# ====== TODO To be translated ======
translate Y FICSFilterFormula {Filter with formula}
# ====== TODO To be translated ======
translate Y FICSIssueSeek {Issue seek}
# ====== TODO To be translated ======
translate Y FICSChallenge {Challenge}
# ====== TODO To be translated ======
translate Y FICSAccept {accept}
# ====== TODO To be translated ======
translate Y FICSDecline {decline}
# ====== TODO To be translated ======
translate Y FICSColour {Colour}
# ====== TODO To be translated ======
translate Y FICSSend {send}
# ====== TODO To be translated ======
translate Y FICSConnect {Connect}
# ====== TODO To be translated ======
translate Y FICSdefaultuservars {Use default variables}
# ====== TODO To be translated ======
translate Y FICSObserveconfirm {Do you want to observe game}
# ====== TODO To be translated ======
translate Y FICSpremove {Enable premove}
# ====== TODO To be translated ======
translate Y GameReview {Game review}
# ====== TODO To be translated ======
translate Y GameReviewTimeExtended {Time extended}
# ====== TODO To be translated ======
translate Y GameReviewMargin {Error margin}
# ====== TODO To be translated ======
translate Y GameReviewAutoContinue {Auto continue when move is correct}
# ====== TODO To be translated ======
translate Y GameReviewReCalculate {Use extended time}
# ====== TODO To be translated ======
translate Y GameReviewAnalyzingMovePlayedDuringTheGame {Analyzing move played during the game}
# ====== TODO To be translated ======
translate Y GameReviewAnalyzingThePosition {Analyzing the position}
# ====== TODO To be translated ======
translate Y GameReviewEnterYourMove {Enter your move}
# ====== TODO To be translated ======
translate Y GameReviewCheckingYourMove {Checking your move}
# ====== TODO To be translated ======
translate Y GameReviewYourMoveWasAnalyzed {Your move was analyzed}
# ====== TODO To be translated ======
translate Y GameReviewYouPlayedSameMove {You played the same move as in match}
# ====== TODO To be translated ======
translate Y GameReviewScoreOfYourMove {Score of your move}
# ====== TODO To be translated ======
translate Y GameReviewGameMoveScore {Game move's score}
# ====== TODO To be translated ======
translate Y GameReviewEngineScore {Engine's score}
# ====== TODO To be translated ======
translate Y GameReviewYouPlayedLikeTheEngine {You played like the engine}
# ====== TODO To be translated ======
translate Y GameReviewNotEngineMoveButGoodMove {Not the engine move, but is also a good move}
# ====== TODO To be translated ======
translate Y GameReviewMoveNotGood {This move is not good, score is}
# ====== TODO To be translated ======
translate Y GameReviewMovesPlayedLike {Moves played like}
# ====== TODO To be translated ======
translate Y GameReviewMovesPlayedEngine {Moves played like engine}
# ====== TODO To be translated ======
translate Y CCDlgConfigureWindowTitle {Configure Correspondence Chess}
# ====== TODO To be translated ======
translate Y CCDlgCGeneraloptions {General Options}
# ====== TODO To be translated ======
translate Y CCDlgDefaultDB {Default Database:}
# ====== TODO To be translated ======
translate Y CCDlgInbox {Inbox (path):}
# ====== TODO To be translated ======
translate Y CCDlgOutbox {Outbox (path):}
# ====== TODO To be translated ======
translate Y CCDlgXfcc {Xfcc Configuration:}
# ====== TODO To be translated ======
translate Y CCDlgExternalProtocol {External Protocol Handler (e.g. Xfcc)}
# ====== TODO To be translated ======
translate Y CCDlgFetchTool {Fetch Tool:}
# ====== TODO To be translated ======
translate Y CCDlgSendTool {Send Tool:}
# ====== TODO To be translated ======
translate Y CCDlgEmailCommunication {eMail Communication}
# ====== TODO To be translated ======
translate Y CCDlgMailPrg {Mail program:}
# ====== TODO To be translated ======
translate Y CCDlgBCCAddr {(B)CC Address:}
# ====== TODO To be translated ======
translate Y CCDlgMailerMode {Mode:}
# ====== TODO To be translated ======
translate Y CCDlgThunderbirdEg {e.g. Thunderbird, Mozilla Mail, Icedove...}
# ====== TODO To be translated ======
translate Y CCDlgMailUrlEg {e.g. Evolution}
# ====== TODO To be translated ======
translate Y CCDlgClawsEg {e.g Sylpheed Claws}
# ====== TODO To be translated ======
translate Y CCDlgmailxEg {e.g. mailx, mutt, nail...}
# ====== TODO To be translated ======
translate Y CCDlgAttachementPar {Attachment parameter:}
# ====== TODO To be translated ======
translate Y CCDlgInternalXfcc {Use internal Xfcc support}
# ====== TODO To be translated ======
translate Y CCDlgConfirmXfcc {Confirm moves}
# ====== TODO To be translated ======
translate Y CCDlgSubjectPar {Subject parameter:}
# ====== TODO To be translated ======
translate Y CCDlgDeleteBoxes {Empty In-/Outbox}
# ====== TODO To be translated ======
translate Y CCDlgDeleteBoxesText {Do you really want to empty your In- and Outbox folders for Correspondence Chess? This requires a new sync to show the last state of your games}
# ====== TODO To be translated ======
translate Y CCDlgConfirmMove {Confirm move}
# ====== TODO To be translated ======
translate Y CCDlgConfirmMoveText {If you confirm, the following move and comment will be sent to the server:}
# ====== TODO To be translated ======
translate Y CCDlgDBGameToLong {Inconsistent Mainline}
# ====== TODO To be translated ======
translate Y CCDlgDBGameToLongError {The mainline in your database is longer than the game in your Inbox. If the Inbox contains current games, i.e. right after a sync, some moves were added to the mainline in the database erroneously.\nIn this case please shorten the mainline to (at max) move\n}
# ====== TODO To be translated ======
translate Y CCDlgStartEmail {Start new eMail game}
# ====== TODO To be translated ======
translate Y CCDlgYourName {Your Name:}
# ====== TODO To be translated ======
translate Y CCDlgYourMail {Your eMail Address:}
# ====== TODO To be translated ======
translate Y CCDlgOpponentName {Opponents Name:}
# ====== TODO To be translated ======
translate Y CCDlgOpponentMail {Opponents eMail Address:}
# ====== TODO To be translated ======
translate Y CCDlgGameID {Game ID (unique):}
# ====== TODO To be translated ======
translate Y CCDlgTitNoOutbox {Scid: Correspondence Chess Outbox}
# ====== TODO To be translated ======
translate Y CCDlgTitNoInbox {Scid: Correspondence Chess Inbox}
# ====== TODO To be translated ======
translate Y CCDlgTitNoGames {Scid: No Correspondence Chess Games}
# ====== TODO To be translated ======
translate Y CCErrInboxDir {Correspondence Chess inbox directory:}
# ====== TODO To be translated ======
translate Y CCErrOutboxDir {Correspondence Chess outbox directory:}
# ====== TODO To be translated ======
translate Y CCErrDirNotUsable {does not exist or is not accessible!\nPlease check and correct the settings.}
# ====== TODO To be translated ======
translate Y CCErrNoGames {does not contain any games!\nPlease fetch them first.}
# ====== TODO To be translated ======
translate Y CCDlgTitNoCCDB {Scid: No Correspondence Database}
# ====== TODO To be translated ======
translate Y CCErrNoCCDB {No Database of type 'Correspondence' is opened. Please open one before using correspondence chess functions.}
# ====== TODO To be translated ======
translate Y CCFetchBtn {Fetch games from the server and process the Inbox}
# ====== TODO To be translated ======
translate Y CCPrevBtn {Goto previous game}
# ====== TODO To be translated ======
translate Y CCNextBtn {Goto next game}
# ====== TODO To be translated ======
translate Y CCSendBtn {Send move}
# ====== TODO To be translated ======
translate Y CCEmptyBtn {Empty In- and Outbox}
# ====== TODO To be translated ======
translate Y CCHelpBtn {Help on icons and status indicators.\nFor general Help press F1!}
# ====== TODO To be translated ======
translate Y CCDlgServerName {Server Name:}
# ====== TODO To be translated ======
translate Y CCDlgLoginName  {Login Name:}
# ====== TODO To be translated ======
translate Y CCDlgPassword   {Password:}
# ====== TODO To be translated ======
translate Y CCDlgShowPassword {Show password}
# ====== TODO To be translated ======
translate Y CCDlgURL        {Xfcc-URL:}
# ====== TODO To be translated ======
translate Y CCDlgRatingType {Rating Type:}
# ====== TODO To be translated ======
translate Y CCDlgDuplicateGame {Non-unique game ID}
# ====== TODO To be translated ======
translate Y CCDlgDuplicateGameError {This game exists more than once in your database. Please delete all duplicates and compact your game file (File/Maintenance/Compact Database).}
# ====== TODO To be translated ======
translate Y CCDlgSortOption {Sorting:}
# ====== TODO To be translated ======
translate Y CCDlgListOnlyOwnMove {Only games I have the move}
# ====== TODO To be translated ======
translate Y CCOrderClassicTxt {Site, Event, Round, Result, White, Black}
# ====== TODO To be translated ======
translate Y CCOrderMyTimeTxt {My Clock}
# ====== TODO To be translated ======
translate Y CCOrderTimePerMoveTxt {Time per move till next time control}
# ====== TODO To be translated ======
translate Y CCOrderStartDate {Start date}
# ====== TODO To be translated ======
translate Y CCOrderOppTimeTxt {Opponents Clock}
# ====== TODO To be translated ======
translate Y CCDlgConfigRelay {Configure ICCF observations}
# ====== TODO To be translated ======
translate Y CCDlgConfigRelayHelp {Go to the games page on http://www.iccf-webchess.com and display the game to be observed.  If you see the chessboard copy the URL from your browser to the list below. One URL per line only!\nExample: http://www.iccf-webchess.com/MakeAMove.aspx?id=266452}
# ====== TODO To be translated ======
translate Y ExtHWConfigConnection {Configure external hardware}
# ====== TODO To be translated ======
translate Y ExtHWPort {Port}
# ====== TODO To be translated ======
translate Y ExtHWEngineCmd {Engine command}
# ====== TODO To be translated ======
translate Y ExtHWEngineParam {Engine parameter}
# ====== TODO To be translated ======
translate Y ExtHWShowButton {Show button}
# ====== TODO To be translated ======
translate Y ExtHWHardware {Hardware}
# ====== TODO To be translated ======
translate Y ExtHWNovag {Novag Citrine}
# ====== TODO To be translated ======
translate Y ExtHWInputEngine {Input Engine}
# ====== TODO To be translated ======
translate Y ExtHWNoBoard {No board}
# ====== TODO To be translated ======
translate Y NovagReferee {Referee}
# ====== TODO To be translated ======
translate Y IEConsole {Input Engine Console}
# ====== TODO To be translated ======
translate Y IESending {Moves sent for}
# ====== TODO To be translated ======
translate Y IESynchronise {Synchronise}
# ====== TODO To be translated ======
translate Y IERotate  {Rotate}
# ====== TODO To be translated ======
translate Y IEUnableToStart {Unable to start Input Engine:}
# ====== TODO To be translated ======
translate Y DoneWithPosition {Done with position}
# ====== TODO To be translated ======
translate Y Board {Board}
# ====== TODO To be translated ======
translate Y showGameInfo {Show game info}
# ====== TODO To be translated ======
translate Y autoResizeBoard {Automatic resize of board}
# ====== TODO To be translated ======
translate Y DockTop {Move to top}
# ====== TODO To be translated ======
translate Y DockBottom {Move to bottom}
# ====== TODO To be translated ======
translate Y DockLeft {Move to left}
# ====== TODO To be translated ======
translate Y DockRight {Move to right}
# ====== TODO To be translated ======
translate Y Undock {Undock}
# ====== TODO To be translated ======
translate Y ChangeIcon {Change icon...}
# ====== TODO To be translated ======
translate Y NewGameListWindow {New Game List Window}
# ====== TODO To be translated ======
translate Y LoadatStartup {Load at startup}
# ====== TODO To be translated ======
translate Y ShowHideDB {Show/Hide databases}
# ====== TODO To be translated ======
translate Y ChangeFilter {Change filter}
# ====== TODO To be translated ======
translate Y ChangeLayout {Load/Save/Change sorting criteria and column layout}
# ====== TODO To be translated ======
translate Y ShowHideStatistic {Show/Hide statistics}
# ====== TODO To be translated ======
translate Y BoardFilter {Show only games that matches the current board position}
# ====== TODO To be translated ======
translate Y CopyGameTo {Copy Game to}
# ====== TODO To be translated ======
translate Y FindBar {Find Bar}
# ====== TODO To be translated ======
translate Y FindCurrentGame {Find current game}
# ====== TODO To be translated ======
translate Y DeleteGame {Delete game}
# ====== TODO To be translated ======
translate Y UndeleteGame {Undelete game}
# ====== TODO To be translated ======
translate Y ResetSort {Reset sort}
# ====== TODO To be translated ======
translate Y ConvertNullMove {Convert null moves to comments}
# ====== TODO To be translated ======
translate Y SetupBoard {Setup Board}
# ====== TODO To be translated ======
translate Y Rotate {Rotate}
# ====== TODO To be translated ======
translate Y SwitchColors {Switch colors}
# ====== TODO To be translated ======
translate Y FlipBoard {Flip Board}
# ====== TODO To be translated ======
translate Y ImportPGN {Import PGN game}
# ====== TODO To be translated ======
translate Y ImportingFiles {Importing PGN files in}
# ====== TODO To be translated ======
translate Y ImportingFrom {Importing from}
# ====== TODO To be translated ======
translate Y ImportingIn {Import games in}
# ====== TODO To be translated ======
translate Y UseLastTag {Use last\ngame's tags}
# ====== TODO To be translated ======
translate Y Random {Random}
# ====== TODO To be translated ======
translate Y BackToMainline {Go back to mainline}
# ====== TODO To be translated ======
translate Y LeaveVariant {Leave variant}
# ====== TODO To be translated ======
translate Y Autoplay {Autoplay}
# ====== TODO To be translated ======
translate Y ShowHideCoords {Show/Hide Coord.}
# ====== TODO To be translated ======
translate Y ShowHideMaterial {Show/Hide Material}
# ====== TODO To be translated ======
translate Y FullScreen {Full Screen}
# ====== TODO To be translated ======
translate Y FilterStatistic {Filter Statistics}
# ====== TODO To be translated ======
translate Y MakeCorrections {Make Corrections}
# ====== TODO To be translated ======
translate Y Surnames {Surnames}
# ====== TODO To be translated ======
translate Y Ambiguous {Ambiguous}
# ====== TODO To be translated ======
translate Y OptionsToolbar "Toolbar glavnog prozora"
# ====== TODO To be translated ======
translate Y OptionsBoard "Chessboard"
# ====== TODO To be translated ======
translate Y OptionsMenuColor "Menu colors"
translate Y OptionsBoardSize "Velièina table"
# ====== TODO To be translated ======
translate Y OptionsBoardPieces "Piece Style"
# ====== TODO To be translated ======
translate Y OptionsInternationalization "Internationalization"
# ====== TODO To be translated ======
translate Y MenuColorForeground "Textcolor"
# ====== TODO To be translated ======
translate Y MenuColorBackground "Background"
# ====== TODO To be translated ======
translate Y MenuColorSelect "Select"
# ====== TODO To be translated ======
translate Y OptionsTablebaseDir "Select up to 4 table base folders:"
}
# end of serbian.tcl
