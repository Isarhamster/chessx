# norsk.tcl
# Text for menu names and status bar help messages in Norwegian.
# First created for Scid 3.2.beta2 by morten.skarstad@sapphire.no.
# Edited for Scid 3.2.beta4
# Thanks to Martin Skjöldebrand, swedish.tcl was of great help to me.

proc setLanguage_O {} {

# File menu:
menuText O File "Fil" 0
menuText O FileNew "Ny..." 0 {Lag en ny Scid-database}
menuText O FileOpen "Åpne..." 0 {Åpne en Scid-database}
menuText O FileClose "Lukk" 0 {Lukk aktiv Scid-database}
menuText O FileFinder "Søk" 0 {Åpne søkevinduet}
menuText O FileBookmarks "Bokmerker" 0 {Bokmerkemeny (snarvei: Ctrl+B)}
menuText O FileBookmarksAdd "Legg til bokmerke" 0 \
  {Sett bokmerke til nåværende database, parti og stilling}
menuText O FileBookmarksFile "Lagre bokmerke" 0 \
  {Lagre et bokmerke for nåværende database, parti og stilling}
menuText O FileBookmarksEdit "Ordne bokmerker" 0 \
  {Vedlikehold av bokmerkene}
menuText O FileBookmarksList "Vis kataloger som enkel liste" 24 \
  {Vis bokmerker som liste, ikke undermenyer}
menuText O FileBookmarksSub "Vis kataloger som undermenyer" 18 \
  {Vis bokmerke-katalogene som undermenyer, ikke liste}
menuText O FileMaint "Vedlikehold" 0 {Verktøy for å vedlikeholde Scid-databasen}
menuText O FileMaintWin "Vedlikeholdsvindu" 0 \
  {Åpne/lukk Scids vedlikeholdsvindu}
menuText O FileMaintCompact "Komprimer database..." 0 \
  {Sletter ubrukte partier og navn}
menuText O FileMaintClass "ECO-klassifiser partier..." 0 \
  {Regn om ECO-koden for alle partier}
menuText O FileMaintSort "Sorter database..." 0 \
  {Sorter alle partier i databasen}
menuText O FileMaintDelete "Slett duplikater..." 6 \
  {Finner like partier og merker en av dem for sletting}
menuText O FileMaintTwin "Duplikatsjekkvindu" 1 \
  {Åpne/oppdatere duplikatsjekkvinduet}
menuText O FileMaintName "Navnestaving" 0 {Redigering og stavekontroll av navn}
menuText O FileMaintNameEditor "Navneredigering" 5 \
  {Åpne/lukk vinduet for navneredigering}
menuText O FileMaintNamePlayer "Kontroller navn..." 0 \
  {Sammenlign navn med stavekontroll-filen}
menuText O FileMaintNameEvent "Kontroller anledninger..." 11 \
  {Sammenlign anledninger med stavekontroll-filen}
menuText O FileMaintNameSite "Kontroller stedsnavn..." 12 \
  {Sammenlign stedsnavn med stavekontroll-filen}
menuText O FileMaintNameRound "Kontroller runde-navn..." 11 \
  {Sammenlign rundenavn med stavekontroll-filen}
menuText O FileReadOnly "Skrivebeskytt..." 6 \
  {Hindrer endringer på databasen}
menuText O FileSwitch "Switch to database" 0 \
  {Switch to a different opened database} ;# ***
menuText O FileExit "Avslutt" 0 {Avslutt Scid}
# ====== TODO To be translated ======
menuText O FileMaintFixBase "Fix corrupted base" 0 {Try to fix a corrupted base}

# Edit menu:
menuText O Edit "Rediger" 0
menuText O EditAdd "Legg til variasjon" 0 {Legg til en variasjon av dette trekket i partiet}
menuText O EditDelete "Slett variasjon" 0 {Slett en variasjon av dette trekket}
menuText O EditFirst "Flytt variasjon øverst" 0 \
  {Gjør variasjonen til den øverste på listen}
menuText O EditMain "Gjør variasjon til hovedlinje" 0 \
  {Gjør variasjon til hovedlinje}
menuText O EditTrial "Prøv variasjon" 0 \
  {Start/avslutt prøvemodus, for å teste ut en ide på brettet}
menuText O EditStrip "Fjern" 0 {Fjern kommentarer eller variasjoner fra partiet}
# ====== TODO To be translated ======
menuText O EditUndo "Undo" 0 {Undo last game change}
# ====== TODO To be translated ======
menuText O EditRedo "Redo" 0 {Redo last game change}
menuText O EditStripComments "Kommentarer" 0 \
  {Fjern alle kommentarer og annotasjoner fra dette partiet}
menuText O EditStripVars "Variasjoner" 0 {Fjern alle variasjoner fra dette partiet}
menuText O EditStripBegin "Moves from the beginning" 1 \
  {Strip moves from the beginning of the game} ;# ***
menuText O EditStripEnd "Moves to the end" 0 \
  {Strip moves to the end of the game} ;# ***
menuText O EditReset "Rensk utklippsbase" 0 \
  {Tømmer utklippsbasen fullstendig}
menuText O EditCopy "Kopier partiet til utklippsbasen" 0 \
  {Kopier dette partiet til utklippsbasen}
menuText O EditPaste "Lim inn siste parti fra utklippsbasen" 0 \
  {Limer inn gjeldende parti fra utklippsbasen her}
menuText O EditPastePGN "Paste Clipboard text as PGN game..." 10 \
  {Interpret the clipboard text as a game in PGN notation and paste it here} ;# ***
menuText O EditSetup "Still opp stilling..." 0 \
  {Lag en startstilling for dette partiet}
menuText O EditCopyBoard "Copy position" 6 \
  {Copy the current board in FEN notation to the text selection (clipboard)} ;# ***
menuText O EditPasteBoard "Lim inn stilling" 0 \
  {Lag en startstilling fra gjeldende utvalg (utklippstavle)}
# ====== TODO To be translated ======
menuText O ConfigureScid "Preferences..." 0 {Configure all options for SCID}

# Game menu:
menuText O Game "Parti" 0
menuText O GameNew "Nytt parti" 0 \
  {Sletter alle trekk og starter et nytt parti}
menuText O GameFirst "Hent første parti" 0 {Henter første parti fra filteret}
menuText O GamePrev "Hent forrige parti" 0 {Henter forrige parti fra filteret}
menuText O GameReload "Hent dette partiet" 0 \
  {Henter dette partiet på nytt og fjerner alle endringer}
menuText O GameNext "Hent neste parti" 0 {Henter neste parti fra filteret}
menuText O GameLast "Hent siste parti" 0 {Henter siste parti fra filteret}
menuText O GameRandom "Hent tilfeldig parti" 8 {Hent tilfeldig parti fra filteret}
menuText O GameNumber "Hent parti nummer..." 0 \
  {Finn et parti ved å angi nummeret}
menuText O GameReplace "Lagre: Erstatt parti..." 0 \
  {Lagre og overskriv dette partiet}
menuText O GameAdd "Lagre som nytt parti..." 0 \
  {Lagre dette partiet som et nytt parti}
menuText O GameDeepest "Identifiser åpning" 0 \
  {Gå til dypeste posisjon i dette partiet listet i ECO-boken}
menuText O GameGotoMove "Gå til trekk..." 0 \
  {Gå til et angitt trekk i dette partiet}
menuText O GameNovelty "Finn avvik..." 0 \
  {Finn det første trekket i dette partiet som ikke har blitt spilt tidligere}

# Search Menu:
menuText O Search "Søk" 0
menuText O SearchReset "Nullstill filter" 0 {Nullstill filteret til å inneholde alle partier}
menuText O SearchNegate "Snu filter" 0 {Reverserer filteret til å kun inneholde eksluderte partier}
menuText O SearchCurrent "Stilling..." 0 {Søk etter denne stillingen}
menuText O SearchHeader "Header..." 0 {Søk etter Header (partier, turnering etc) informasjon}
menuText O SearchMaterial "Kvalitet/stilling..." 0 {Søk på kvalitet eller stilling}
menuText O SearchUsing "Bruk søkefil..." 0 {Søk vha lagrede søkevilkår}

# Windows menu:
menuText O Windows "Vinduer" 0
menuText O WindowsComment "Kommentarer" 0 {Åpne/lukk kommentarvinduet}
menuText O WindowsGList "Partioversikt" 0 {Åpne/lukk partioversikten}
menuText O WindowsPGN "PGN-vindu" 0 \
  {Åpne/lukk PGN-vinduet}
menuText O WindowsPList "Player Finder" 2 {Open/close the player finder} ;# ***
menuText O WindowsTmt "Turneringsøker" 0 {Åpne/lukk turneringssøkeren}
menuText O WindowsSwitcher "Databasebytter" 0 \
  {Åpne/lukk databasebytteren}
menuText O WindowsMaint "Vedlikeholdsvindu" 0 \
  {Åpne/lukk vedlikeholdsvinduet}
menuText O WindowsECO "ECO-oversikt" 0 {Åpne/lukk ECO-oversikten}
menuText O WindowsStats "Statistikkvindu" 0 \
  {Åpne/lukk statistikk for filteret}
menuText O WindowsTree "Trevindu" 0 {Åpne/lukk trevinduet}
menuText O WindowsTB "Sluttspilltabellvindu" 0 \
  {Åpne/lukk sluttspilltabellen}
# ====== TODO To be translated ======
menuText O WindowsBook "Book Window" 0 {Open/close the Book window}
# ====== TODO To be translated ======
menuText O WindowsCorrChess "Correspondence Window" 0 {Open/close the Correspondence window}

# Tools menu:
menuText O Tools "Verktøy" 0
menuText O ToolsAnalysis "Analysemotor..." 0 \
  {Start/stopp en sjakkanalysemotor}
menuText O ToolsAnalysis2 "Analysemotor #2..." 0 \
  {Start/stopp enda en sjakkanalysemotor}
menuText O ToolsCross "Krysstabell" 0 {Vis turnerigskrystabellen for dette partiet}
menuText O ToolsEmail "Epostbehandler" 0 \
  {Åpne/lukk epostbehandlingsvinduet}
menuText O ToolsFilterGraph "Filtergraf" 7 \
  {Åpne/lukk filtergrafvinduet}
# ====== TODO To be translated ======
menuText O ToolsAbsFilterGraph "Abs. Filter Graph" 7 {Open/close the filter graph window for absolute values}
menuText O ToolsOpReport "Åpningsrapport" 0 \
  {Lager en åpningsrapport for denne stillingen}
# ====== TODO To be translated ======
menuText O ToolsOpenBaseAsTree "Open base as tree" 0   {Open a base and use it in Tree window}
# ====== TODO To be translated ======
menuText O ToolsOpenRecentBaseAsTree "Open recent base as tree" 0   {Open a recent base and use it in Tree window}
menuText O ToolsTracker "Brikkesporing"  0 {Åpne brikkesporingsvinduet}
# ====== TODO To be translated ======
menuText O ToolsTraining "Training"  0 {Training tools (tactics, openings,...) }
# ====== TODO To be translated ======
menuText O ToolsTacticalGame "Tactical game"  0 {Play a game with tactics}
# ====== TODO To be translated ======
menuText O ToolsSeriousGame "Serious game"  0 {Play a serious game}
# ====== TODO To be translated ======
menuText O ToolsTrainOpenings "Openings"  0 {Train with a repertoire}
# ====== TODO To be translated ======
menuText O ToolsTrainReviewGame "Review game"  0 {Guess moves played in a game}
# ====== TODO To be translated ======
menuText O ToolsTrainTactics "Tactics"  0 {Solve tactics}
# ====== TODO To be translated ======
menuText O ToolsTrainCalvar "Calculation of variations"  0 {Calculation of variations training}
# ====== TODO To be translated ======
menuText O ToolsTrainFindBestMove "Find best move"  0 {Find best move}
# ====== TODO To be translated ======
menuText O ToolsTrainFics "Play on internet"  0 {Play on freechess.org}
# ====== TODO To be translated ======
menuText O ToolsEngineTournament "Engine tournament"  0 {Start a tournament between chess engines}
# ====== TODO To be translated ======
menuText O ToolsBookTuning "Book tuning" 0 {Book tuning}
# ====== TODO To be translated ======
menuText O ToolsConnectHardware "Connect Hardware" 0 {Connect external hardware}
# ====== TODO To be translated ======
menuText O ToolsConnectHardwareConfigure "Configure..." 0 {Configure external hardware and connection}
# ====== TODO To be translated ======
menuText O ToolsConnectHardwareNovagCitrineConnect "Connect Novag Citrine" 0 {Connect Novag Citrine}
# ====== TODO To be translated ======
menuText O ToolsConnectHardwareInputEngineConnect "Connect Input Engine" 0 {Connect Input Engine (e.g. DGT)}
menuText O ToolsPInfo "Spillerinformasjon"  0 \
  {Åpne/oppdater informasjonsvinduet for denne spilleren}
menuText O ToolsPlayerReport "Player Report..." 3 \
  {Generate a player report} ;# ***
menuText O ToolsRating "Ratinggraf" 0 \
  {Tegner ratingen til disse spillerene over tid}
menuText O ToolsScore "Poeng-graf" 0 {Viser poeng-grafen}
menuText O ToolsExpCurrent "Eksporter parti" 0 \
  {Skriv dette partiet til en tekstfil}
menuText O ToolsExpCurrentPGN "Eksporter parti til PGN-fil..." 0 \
  {Skriv dette partiet til en PGN-fil}
menuText O ToolsExpCurrentHTML "Eksporter parti til HTML fil..." 0 \
  {Skriv dette partiet til en HTML-fil}
# ====== TODO To be translated ======
menuText O ToolsExpCurrentHTMLJS "Export Game to HTML and JavaScript File..." 15 {Write current game to a HTML and JavaScript file}  
menuText O ToolsExpCurrentLaTeX "Eksporter partil til LaTeX fil..." 0 \
  {Skriv dette partiet til en LaTeX-fil}
menuText O ToolsExpFilter "Eksporter alle partier i filteret" 0 \
  {Skriv alle partier i filteret til en tekstfil}
menuText O ToolsExpFilterPGN "Eksporter filter til PGN-fil..." 0 \
  {Skriv alle partier i filteret til en PGN-fil}
menuText O ToolsExpFilterHTML "Eksporter filter til HTML-fil..." 0 \
  {Skriv alle partier i filteret til en HTML-fil}
# ====== TODO To be translated ======
menuText O ToolsExpFilterHTMLJS "Export Filter to HTML and JavaScript File..." 17 {Write all filtered games to a HTML and JavaScript file}  
menuText O ToolsExpFilterLaTeX "Eksporter filter til LaTeX-fil..." 0 \
  {Skriv alle partier i filteret til en LaTeX-fil}
menuText O ToolsImportOne "Importer ett PGN-parti..." 0 \
  {Importer ett parti fra en PGN-fil}
menuText O ToolsImportFile "Importer fil med PGN-partier..." 0 \
  {Importer partier fra en PGN-fil}
# ====== TODO To be translated ======
menuText O ToolsStartEngine1 "Start engine 1" 0  {Start engine 1}
# ====== TODO To be translated ======
menuText O ToolsStartEngine2 "Start engine 2" 0  {Start engine 2}
# ====== TODO To be translated ======
menuText O ToolsCaptureBoard "Capture Current Board..." 0  {Save the current board as an image.}
# ====== TODO To be translated ======
menuText O Play "Play" 0
# ====== TODO To be translated ======
menuText O CorrespondenceChess "Correspondence Chess" 0 {Functions for eMail and Xfcc based correspondence chess}
# ====== TODO To be translated ======
menuText O CCConfigure "Configure..." 0 {Configure external tools and general setup}
# ====== TODO To be translated ======
menuText O CCConfigRelay "Configure observations..." 10 {Configure games to be observed}
# ====== TODO To be translated ======
menuText O CCOpenDB "Open Database..." 0 {Open the default Correspondence database}
# ====== TODO To be translated ======
menuText O CCRetrieve "Retrieve Games" 0 {Retrieve games via external (Xfcc-)helper}
# ====== TODO To be translated ======
menuText O CCInbox "Process Inobx" 0 {Process all files in scids Inbox}
# ====== TODO To be translated ======
menuText O CCSend "Send Move" 0 {Send your move via eMail or external (Xfcc-)helper}
# ====== TODO To be translated ======
menuText O CCResign "Resign" 0 {Resign (not via eMail)}
# ====== TODO To be translated ======
menuText O CCClaimDraw "Claim Draw" 0 {Send move and claim a draw (not via eMail)}
# ====== TODO To be translated ======
menuText O CCOfferDraw "Offer Draw" 0 {Send move and offer a draw (not via eMail)}
# ====== TODO To be translated ======
menuText O CCAcceptDraw "Accept Draw" 0 {Accept a draw offer (not via eMail)}
# ====== TODO To be translated ======
menuText O CCNewMailGame "New eMail Game..." 0 {Start a new eMail game}
# ====== TODO To be translated ======
menuText O CCMailMove "Mail Move..." 0 {Send the move via eMail to the opponent}
# ====== TODO To be translated ======
menuText O CCGamePage "Game Page..." 0 {Call up the game via the web browser}
# ====== TODO To be translated ======
menuText O CCEditCopy "Copy Gamelist to Clipbase" 0 {Copy the games as CSV list to clipbase}

# Options menu:
menuText O Options "Innstillinger" 0
# ====== TODO To be translated ======
menuText O OptionsBoardGraphics "Squares..." 0 {Select textures for squares}
# ====== TODO To be translated ======
translate O OptionsBGW {Select texture for squares}
# ====== TODO To be translated ======
translate O OptionsBoardGraphicsText {Select graphic files for white and black squares:}
menuText O OptionsBoardNames "My Player Names..." 0 {Edit my player names} ;# ***
menuText O OptionsExport "Eksport" 0 {Endre innstillinger for eksport av tekst}
menuText O OptionsFonts "Skrifttyper" 0 {Endre skrifttyper}
menuText O OptionsFontsRegular "Vanlig" 0 {Endre den vanlige skrifttypen}
menuText O OptionsFontsMenu "Menu" 0 {Change the menu font} ;# ***
menuText O OptionsFontsSmall "Liten" 0 {Endre den lille skrifttypen}
# ====== TODO To be translated ======
menuText O OptionsFontsTiny "Tiny" 0 {Change the tiny font}
menuText O OptionsFontsFixed "Fastbredde" 0 {Endre fastbreddeskrifttypen}
menuText O OptionsGInfo "Partiinformasjon" 0 {Informasjonsinnstillinger}
menuText O OptionsLanguage "Språk" 0 {Velg menyspråk}
# ====== TODO To be translated ======
menuText O OptionsMovesTranslatePieces "Translate pieces" 0 {Translate first letter of pieces}
# ====== TODO To be translated ======
menuText O OptionsMovesHighlightLastMove "Highlight last move" 0 {Highlight last move}
# ====== TODO To be translated ======
menuText O OptionsMovesHighlightLastMoveDisplay "Show" 0 {Display last move Highlight}
# ====== TODO To be translated ======
menuText O OptionsMovesHighlightLastMoveWidth "Width" 0 {Thickness of line}
# ====== TODO To be translated ======
menuText O OptionsMovesHighlightLastMoveColor "Color" 0 {Color of line}
# ====== TODO To be translated ======
menuText O OptionsMovesHighlightLastMoveArrow "include Arrow" 0 {Include Arrow with Highlight}
menuText O OptionsMoves "Trekk" 0 {Innstillinger for hvordan trekk angis}
menuText O OptionsMovesAnimate "Animation time" 1 \
  {Set the amount of time used to animate moves} ;# ***
menuText O OptionsMovesDelay "Autospillforsinkelse..." 0 \
  {Sett forsinkelsen ved automatisk gjennomgang av partiet}
menuText O OptionsMovesCoord "Koordinater" 0 \
  {Tillat angivelse av koordinater ("g1f3")}
menuText O OptionsMovesSuggest "Vis foreslåtte trekk" 0 \
  {Slå av/på forslag til trekk}
# ====== TODO To be translated ======
menuText O OptionsShowVarPopup "Show variations window" 0 {Turn on/off the display of a variations window}  
# ====== TODO To be translated ======
menuText O OptionsMovesSpace "Add spaces after move number" 0 {Add spaces after move number}  
menuText O OptionsMovesKey "Autofullfør" 0 \
  {Slå av/på autofullføriring av tastaturtrekk}
# ====== TODO To be translated ======
menuText O OptionsMovesShowVarArrows "Show Arrows for Variations" 0 {Turn on/off arrows showing moves in variations}
# ====== TODO To be translated ======
menuText O OptionsMovesGlossOfDanger "Color Coded Gloss of Danger" 0 {Turn on/off color coded gloss of danger}
menuText O OptionsNumbers "Tallformat" 0 {Angi tallformat}
# ====== TODO To be translated ======
menuText O OptionsTheme "Theme" 0 {Change look of interface}
menuText O OptionsWindows "Vinduer" 0 {Vindusinnstillinger}
menuText O OptionsSounds "Sounds" 2 {Configure move announcement sounds} ;# ***
# ====== TODO To be translated ======
menuText O OptionsResources "Resources..." 0 {Choose resource files and folders}
# ====== TODO To be translated ======
menuText O OptionsWindowsDock "Dock windows" 0 {Dock windows}
# ====== TODO To be translated ======
menuText O OptionsWindowsSaveLayout "Save layout" 0 {Save layout}
# ====== TODO To be translated ======
menuText O OptionsWindowsRestoreLayout "Restore layout" 0 {Restore layout}
# ====== TODO To be translated ======
menuText O OptionsWindowsShowGameInfo "Show game info" 0 {Show game info}
# ====== TODO To be translated ======
menuText O OptionsWindowsAutoLoadLayout "Auto load first layout" 0 {Auto load first layout at startup}
menuText O OptionsECO "Hent ECO-fil" 0 {Henter filen med ECO-klassifiseringer}
menuText O OptionsSpell "Hent stavekontrollfil" 0 \
  {Henter filen med stavekontrollen til Scid}
menuText O OptionsTable "Tabellbasekatalog" 0 \
  {Velg en tabellbase; Alle tabellbaser i katalogen vil bli brukt}
menuText O OptionsRecent "Recent files..." 0 \
  {Change the number of recent files displayed in the File menu} ;# ***
# ====== TODO To be translated ======
menuText O OptionsBooksDir "Books directory" 0 {Sets the opening books directory}
# ====== TODO To be translated ======
menuText O OptionsTacticsBasesDir "Bases directory" 0 {Sets the tactics (training) bases directory}
# ====== TODO To be translated ======
menuText O OptionsPhotosDir "Photos directory" 0 {Sets the photos bases directory}
# ====== TODO To be translated ======
menuText O OptionsThemeDir "Theme(s) File:"  0 { Load a GUI theme package file }
menuText O OptionsSave "Lagre innstillinger" 0 \
  "Lagre alle instillinger til $::optionsFile"
menuText O OptionsAutoSave "Autolagre innstillinger ved avslutning" 0 \
  {Autolagre alle innstillinger når Scid avsluttes}

# Help menu:
menuText O Help "Hjelp" 0
menuText O HelpContents "Contents" 0 {Show the help contents page} ;# ***
menuText O HelpIndex "Innhold" 0 {Hjelpesystemets innhold}
menuText O HelpGuide "Lynguide" 0 {Vis lynguiden}
menuText O HelpHints "Hint" 0 {Vis hint}
menuText O HelpContact "Kontaktinformasjon" 0 {Vis kontaktinformasjon}
menuText O HelpTip "Dagens tips" 0 {Vis et nyttig Scid-tips}
menuText O HelpStartup "Oppstartsvindu" 0 {Vis oppstartsvindu}
menuText O HelpAbout "Om Scid" 0 {Informasjon om Scid}

# Game info box popup menu:
menuText O GInfoHideNext "Skjul neste trekk" 0
menuText O GInfoMaterial "Vis kvalitet" 0
menuText O GInfoFEN "Vis FEN" 0
menuText O GInfoMarks "Vis fargede ruter og piler" 0
menuText O GInfoWrap "Tekstbryting" 0
menuText O GInfoFullComment "Vis full kommentar" 0
menuText O GInfoPhotos "Show Photos" 5 ;# ***
menuText O GInfoTBNothing "Tabellbaser: ingenting" 0
menuText O GInfoTBResult "Tabellbaser: kun resultater" 0
menuText O GInfoTBAll "Tabellbaser: resultater og beste trekk" 0
menuText O GInfoDelete "Slett/gjenopprett dette partiet" 0
menuText O GInfoMark "(Av)Merk dette partiet" 0
# ====== TODO To be translated ======
menuText O GInfoInformant "Configure informant values" 0

# General buttons:
translate O Back {Tilbake}
translate O Browse {Browse} ;# ***
translate O Cancel {Avbryt}
# ====== TODO To be translated ======
translate O Continue {Continue}
translate O Clear {Slett}
translate O Close {Lukk}
translate O Contents {Contents} ;# ***
translate O Defaults {Standard}
translate O Delete {Slett}
translate O Graph {Graf}
translate O Help {Hjelp}
translate O Import {Importer}
translate O Index {Innhold}
translate O LoadGame {Hent parti}
translate O BrowseGame {Bla gjennom parti}
translate O MergeGame {Smelt parti}
# ====== TODO To be translated ======
translate O MergeGames {Merge Games}
translate O Preview {Forhåndsvis}
translate O Revert {Gjør om}
translate O Save {Lagre}
translate O Search {Søk}
translate O Stop {Stopp}
translate O Store {Lagre}
translate O Update {Oppdater}
translate O ChangeOrient {Vend vindu}
translate O ShowIcons {Show Icons} ;# ***
translate O None {Ingen}
translate O First {Første}
translate O Current {Gjeldende}
translate O Last {Siste}

# General messages:
translate O game {parti}
translate O games {partier}
translate O move {trekk}
translate O moves {trekk}
translate O all {alle}
translate O Yes {Ja}
translate O No {Nei}
translate O Both {Begge}
translate O King {Konge}
translate O Queen {Dronning}
translate O Rook {Tårn}
translate O Bishop {Løper}
translate O Knight {Springer}
translate O Pawn {Bonde}
translate O White {Hvit}
translate O Black {Sort}
translate O Player {Spiller}
translate O Rating {Rating}
translate O RatingDiff {Ratingforskjell (Hvit - Sort)}
translate O AverageRating {Average Rating} ;# ***
translate O Event {Anledning}
translate O Site {Sted}
translate O Country {Land}
translate O IgnoreColors {Overse farger}
translate O Date {Dato}
translate O EventDate {Anledningsdato}
translate O Decade {Decade} ;# ***
translate O Year {År}
translate O Month {Måned}
translate O Months {Januar Februar Mars April Mai Juni Juli August September Oktober November Desember}
translate O Days {Søn Man Tir Ons Tor Fre Lør}
translate O YearToToday {År til i dag}
# ====== TODO To be translated ======
translate O YearToTodayTooltip {Set date from 1 year back to today}
translate O Result {Utfall}
translate O Round {Runde}
translate O Length {Lengde}
translate O ECOCode {ECO-kode}
translate O ECO {ECO}
translate O Deleted {Slettet}
translate O SearchResults {Søk i resultater}
translate O OpeningTheDatabase {Åpningsdatabase}
translate O Database {Database}
translate O Filter {Filter}
translate O noGames {ingen partier}
translate O allGames {alle partier}
translate O empty {tom}
translate O clipbase {utklippsbase}
translate O score {poeng}
translate O StartPos {Utgangsstilling}
translate O Total {Sammenlagt}
translate O readonly {skrivebeskyttet}

# Standard error messages:
translate O ErrNotOpen {This is not an open database.} ;# ***
translate O ErrReadOnly {This database is read-only; it cannot be altered.} ;# ***
translate O ErrSearchInterrupted {Search was interrupted; results are incomplete.} ;# ***

# Game information:
translate O twin {duplikat}
translate O deleted {slettet}
translate O comment {kommentar}
translate O hidden {skjult}
translate O LastMove {Siste trekk}
translate O NextMove {Neste}
translate O GameStart {Begynnelse på parti}
translate O LineStart {Begynnelse på linje}
translate O GameEnd {Slutt på parti}
translate O LineEnd {Slutt på linje}

# Player information:
translate O PInfoAll {Resultater for <b>alle</b> partier}
translate O PInfoFilter {Resultater for partier i <b>filteret</b>}
translate O PInfoAgainst {Resultater mot}
translate O PInfoMostWhite {Hyppigste åpning som hvit}
translate O PInfoMostBlack {Hyppigste åpning som sort}
translate O PInfoRating {Historisk rating}
translate O PInfoBio {Biografi}
translate O PInfoEditRatings {Edit Ratings} ;# ***
# ====== TODO To be translated ======
translate O PInfoEloFile {File}

# Tablebase information:
translate O Draw {Uavgjort}
translate O stalemate {patt}
translate O withAllMoves {med alle trekk}
translate O withAllButOneMove {med alle trekk unntatt ett}
translate O with {med}
translate O only {kun}
translate O lose {taper} ;# tap?
translate O loses {taper}
translate O allOthersLose {alle andre taper}
translate O matesIn {setter matt i}
translate O hasCheckmated {har satt matt}
translate O longest {lengste}
translate O WinningMoves {Winning moves} ;# ***
translate O DrawingMoves {Drawing moves} ;# ***
translate O LosingMoves {Losing moves} ;# ***
translate O UnknownMoves {Unknown-result moves} ;# ***
# ====== TODO To be translated ======
translate O SideLongest {Side    Longest    %     %     %\n}
# ====== TODO To be translated ======
translate O toNoveMate {to move   mate    Win  Draw  Loss\n}
# ====== TODO To be translated ======
translate O NoSummary {\nNo summary for this tablebase.}
# ====== TODO To be translated ======
translate O MutualZugzwang {Mutual zugzwangs: }
# ====== TODO To be translated ======
translate O SelectetZugzwang {\nSelected zugzwang positions:}
# ====== TODO To be translated ======
translate O WdrawBloss {White draws, Black loses:}
# ====== TODO To be translated ======
translate O BdrawWloss {Black draws, White loses:}
# ====== TODO To be translated ======
translate O AllLoss {" Whoever moves loses:     " }

# Tip of the day:
translate O Tip {Tips}
translate O TipAtStartup {Tips ved oppstart}

# Tree window menus:
menuText O TreeFile "Fil" 0
# ====== TODO To be translated ======
menuText O TreeFileFillWithBase "Fill Cache with base" 0 {Fill the cache file with all games in current base}
# ====== TODO To be translated ======
menuText O TreeFileFillWithGame "Fill Cache with game" 0 {Fill the cache file with current game in current base}
# ====== TODO To be translated ======
menuText O TreeFileSetCacheSize "Cache size" 0 {Set the cache size}
# ====== TODO To be translated ======
menuText O TreeFileCacheInfo "Cache info" 0 {Get info on cache usage}
menuText O TreeFileSave "Lagre hurtigbuffer" 0 {Lagrer hurtigbufferfilen (.stc)}
menuText O TreeFileFill "Fyll hurtigbuffer" 0 \
  {Fyller hurtigbufferet med de hyppigste åpningsstillingene}
menuText O TreeFileBest "Idealparti" 0 {Show the best tree games list}
menuText O TreeFileGraph "Grafikk" 0 {Vis grafen for denne grenen av treet}
menuText O TreeFileCopy "Kopier treet til utklippstavlen" 0 \
  {Kopier statistikken for treet til utklippstavlen}
menuText O TreeFileClose "Lukk trevindu" 0 {Lukker trevinduet}
# ====== TODO To be translated ======
menuText O TreeMask "Mask" 0
# ====== TODO To be translated ======
menuText O TreeMaskNew "New" 0 {New mask}
# ====== TODO To be translated ======
menuText O TreeMaskOpen "Open" 0 {Open mask}
# ====== TODO To be translated ======
menuText O TreeMaskOpenRecent "Open recent" 0 {Open recent mask}
# ====== TODO To be translated ======
menuText O TreeMaskSave "Save" 0 {Save mask}
# ====== TODO To be translated ======
menuText O TreeMaskClose "Close" 0 {Close mask}
# ====== TODO To be translated ======
menuText O TreeMaskFillWithGame "Fill with game" 0 {Fill mask with game}
# ====== TODO To be translated ======
menuText O TreeMaskFillWithBase "Fill with base" 0 {Fill mask with all games in base}
# ====== TODO To be translated ======
menuText O TreeMaskInfo "Info" 0 {Show statistics for current mask}
# ====== TODO To be translated ======
menuText O TreeMaskDisplay "Display mask map" 0 {Show mask data in a tree form}
# ====== TODO To be translated ======
menuText O TreeMaskSearch "Search" 0 {Search in current mask}
menuText O TreeSort "Sorter" 0
menuText O TreeSortAlpha "Alfabetisk" 0
menuText O TreeSortECO "ECO-kode" 0
menuText O TreeSortFreq "Hyppighet" 0
menuText O TreeSortScore "Poeng" 0
menuText O TreeOpt "Innstillinger" 0
# ====== TODO To be translated ======
menuText O TreeOptSlowmode "slow mode" 0 {Slow mode for updates (high accuracy)}
# ====== TODO To be translated ======
menuText O TreeOptFastmode "Fast mode" 0 {Fast mode for updates (no move transposition)}
# ====== TODO To be translated ======
menuText O TreeOptFastAndSlowmode "Fast and slow mode" 0 {Fast mode then slow mode for updates}
# ====== TODO To be translated ======
menuText O TreeOptStartStop "Auto refreshing" 0 {Toggles automatic refreshing of the tree window}
menuText O TreeOptLock "Lås" 0 {Låser treet til nåværende database}
menuText O TreeOptTraining "Trening" 0 {Slår av/på treningsmodus}
menuText O TreeOptAutosave "Autolagre hurtigbuffer" 0 \
  {Autolagrer hurtigbufferet når vinduet lukkes}
menuText O TreeHelp "Hjelp" 0
menuText O TreeHelpTree "Tre hjelp" 0
menuText O TreeHelpIndex "Innholdsfortegnelse" 0
translate O SaveCache {Lagre hurtigbuffer}
translate O Training {Trening}
translate O LockTree {Lås}
translate O TreeLocked {låst}
translate O TreeBest {Beste}
translate O TreeBestGames {Idealtrepartier}
# Note: the next message is the tree window title row. After editing it,
# check the tree window to make sure it lines up with the actual columns.
translate O TreeTitleRow \
  {    Move   ECO       Frequency    Score  AvElo Perf AvYear %Draws} ;# ***
translate O TreeTotal {SAMMENLAGT}
# ====== TODO To be translated ======
translate O DoYouWantToSaveFirst {Do you want to save first}
# ====== TODO To be translated ======
translate O AddToMask {Add to Mask}
# ====== TODO To be translated ======
translate O RemoveFromMask {Remove from Mask}
# ====== TODO To be translated ======
translate O AddThisMoveToMask {Add this move to Mask}
# ====== TODO To be translated ======
translate O SearchMask {Search in Mask}
# ====== TODO To be translated ======
translate O DisplayMask {Display Mask}
# ====== TODO To be translated ======
translate O Nag {Nag code}
# ====== TODO To be translated ======
translate O Marker {Marker}
# ====== TODO To be translated ======
translate O Include {Include}
# ====== TODO To be translated ======
translate O Exclude {Exclude}
# ====== TODO To be translated ======
translate O MainLine {Main line}
# ====== TODO To be translated ======
translate O Bookmark {Bookmark}
# ====== TODO To be translated ======
translate O NewLine {New line}
# ====== TODO To be translated ======
translate O ToBeVerified {To be verified}
# ====== TODO To be translated ======
translate O ToTrain {To train}
# ====== TODO To be translated ======
translate O Dubious {Dubious}
# ====== TODO To be translated ======
translate O ToRemove {To remove}
# ====== TODO To be translated ======
translate O NoMarker {No marker}
# ====== TODO To be translated ======
translate O ColorMarker {Color}
# ====== TODO To be translated ======
translate O WhiteMark {White}
# ====== TODO To be translated ======
translate O GreenMark {Green}
# ====== TODO To be translated ======
translate O YellowMark {Yellow}
# ====== TODO To be translated ======
translate O BlueMark {Blue}
# ====== TODO To be translated ======
translate O RedMark {Red}
# ====== TODO To be translated ======
translate O CommentMove {Comment move}
# ====== TODO To be translated ======
translate O CommentPosition {Comment position}
# ====== TODO To be translated ======
translate O AddMoveToMaskFirst {Add move to mask first}
# ====== TODO To be translated ======
translate O OpenAMaskFileFirst {Open a mask file first}
# ====== TODO To be translated ======
translate O Positions {Positions}
# ====== TODO To be translated ======
translate O Moves {Moves}

# Finder window:
menuText O FinderFile "Fil" 0
menuText O FinderFileSubdirs "Let i underkataloger" 0
menuText O FinderFileClose "Lukk søkevindu" 0
menuText O FinderSort "Sorter" 0
menuText O FinderSortType "Type" 0
menuText O FinderSortSize "Størrelse" 0
menuText O FinderSortMod "Endret" 0
menuText O FinderSortName "Navn" 0
menuText O FinderSortPath "Sti" 0
menuText O FinderTypes "Typer" 0
menuText O FinderTypesScid "Scid-databaser" 0
menuText O FinderTypesOld "Scid-databaser i gammelt format" 0
menuText O FinderTypesPGN "PGN-filer" 0
menuText O FinderTypesEPD "EPD-filer" 0
menuText O FinderTypesRep "Repertoire-filer" 0
menuText O FinderHelp "Hjelp" 0
menuText O FinderHelpFinder "Søkehjelp" 0
menuText O FinderHelpIndex "Innholdsfortegnelse" 0
translate O FileFinder {Søkeverktøy}
translate O FinderDir {Katalog}
translate O FinderDirs {Kataloger}
translate O FinderFiles {Filer}
translate O FinderUpDir {opp}
# ====== TODO To be translated ======
translate O FinderCtxOpen {Open}
# ====== TODO To be translated ======
translate O FinderCtxBackup {Backup}
# ====== TODO To be translated ======
translate O FinderCtxCopy {Copy}
# ====== TODO To be translated ======
translate O FinderCtxMove {Move}
# ====== TODO To be translated ======
translate O FinderCtxDelete {Delete}

# Player finder:
menuText O PListFile "Fil" 0
menuText O PListFileUpdate "Oppdater" 0
menuText O PListFileClose "Close Player Finder" 0 ;# ***
menuText O PListSort "Sorter" 0
menuText O PListSortName "Name" 0 ;# ***
menuText O PListSortElo "Elo" 0
menuText O PListSortGames "Partier" 0
menuText O PListSortOldest "Oldest" 0 ;# ***
menuText O PListSortNewest "Newest" 0 ;# ***

# Tournament finder:
menuText O TmtFile "Fil" 0
menuText O TmtFileUpdate "Oppdater" 0
menuText O TmtFileClose "Lukk turneringssøker" 0
menuText O TmtSort "Sorter" 0
menuText O TmtSortDate "Dato" 0
menuText O TmtSortPlayers "Players" 0
menuText O TmtSortGames "Partier" 0
menuText O TmtSortElo "Elo" 0
menuText O TmtSortSite "Sted" 0
menuText O TmtSortEvent "Anledning" 1
menuText O TmtSortWinner "Vinner" 0
translate O TmtLimit "Listegrense"
translate O TmtMeanElo "Laveste middel-ELO"
translate O TmtNone "Ingen passende turneringer funnet."

# Graph windows:
menuText O GraphFile "Fil" 0
menuText O GraphFileColor "Lagre som farget PostScript..." 0
menuText O GraphFileGrey "Lagre som gråtonet PostScript..." 0
menuText O GraphFileClose "Lukk vindu" 0
menuText O GraphOptions "Innstillinger" 0
menuText O GraphOptionsWhite "Hvit" 0
menuText O GraphOptionsBlack "Sort" 0
menuText O GraphOptionsBoth "Begge" 0
menuText O GraphOptionsPInfo "Spiller Info spiller" 0
# ====== TODO To be translated ======
menuText O GraphOptionsEloFile "Elo from rating file" 0
# ====== TODO To be translated ======
menuText O GraphOptionsEloDB "Elo from database" 0
translate O GraphFilterTitle "Filtergraf: hyppighet per 1000 partier"
# ====== TODO To be translated ======
translate O GraphAbsFilterTitle "Filter Graph: frequency of the games"
# ====== TODO To be translated ======
translate O ConfigureFilter {Configure X-Axes for Year, Rating and Moves}
# ====== TODO To be translated ======
translate O FilterEstimate "Estimate"
# ====== TODO To be translated ======
translate O TitleFilterGraph "Scid: Filter Graph"

# Analysis window:
translate O AddVariation {Legg til variasjon}
# ====== TODO To be translated ======
translate O AddAllVariations {Add All Variations}
translate O AddMove {Legg til trekk}
translate O Annotate {Annoter}
# ====== TODO To be translated ======
translate O ShowAnalysisBoard {Show analysis board}
# ====== TODO To be translated ======
translate O ShowInfo {Show engine info}
# ====== TODO To be translated ======
translate O FinishGame {Finish game}
# ====== TODO To be translated ======
translate O StopEngine {Stop engine}
# ====== TODO To be translated ======
translate O StartEngine {Start engine}
# ====== TODO To be translated ======
translate O LockEngine {Lock engine to current position}
translate O AnalysisCommand {Analysekommando}
translate O PreviousChoices {Tidligere valg}
translate O AnnotateTime {Tid mellom trekk i sekunder}
translate O AnnotateWhich {Legg til variasjoner}
translate O AnnotateAll {For trekk av begge sider}
# ====== TODO To be translated ======
translate O AnnotateAllMoves {Annotate all moves}
translate O AnnotateWhite {Kun for hvite trekk}
translate O AnnotateBlack {Kun for sorte trekk}
# ====== TODO To be translated ======
translate O AnnotateBlundersOnly {When game move is an obvious blunder}
# ====== TODO To be translated ======
translate O AnnotateBlundersOnlyScoreChange {Analysis reports blunder, with score change from/to: }
# ====== TODO To be translated ======
translate O BlundersThreshold {Threshold}
# TODO: Translate
translate O ScoreAllMoves {Score all moves}
translate O LowPriority {Low CPU priority} ;# ***
# ====== TODO To be translated ======
translate O ClickHereToSeeMoves {Click here to see moves}
# ====== TODO To be translated ======
translate O ConfigureInformant {Informant}
# ====== TODO To be translated ======
translate O Informant!? {Interesting move}
# ====== TODO To be translated ======
translate O Informant? {Poor move}
# ====== TODO To be translated ======
translate O Informant?? {Blunder}
# ====== TODO To be translated ======
translate O Informant?! {Dubious move}
# ====== TODO To be translated ======
translate O Informant+= {White has a slight advantage}
# ====== TODO To be translated ======
translate O Informant+/- {White has a moderate advantage}
# ====== TODO To be translated ======
translate O Informant+- {White has a decisive advantage}
# ====== TODO To be translated ======
translate O Informant++- {The game is considered won}
# ====== TODO To be translated ======
translate O Book {Book}
# ====== TODO To be translated ======
translate O OtherBookMoves {Opponent's book}
# ====== TODO To be translated ======
translate O OtherBookMovesTooltip {Moves to which the opponent has a reply}

# Analysis Engine open dialog:
translate O EngineList {Liste over analysemotorer}
translate O EngineName {Navn}
translate O EngineCmd {Kommandolinje}
translate O EngineArgs {Parametre}
translate O EngineDir {Katalog}
translate O EngineElo {Elo}
translate O EngineTime {Dato}
translate O EngineNew {Ny}
translate O EngineEdit {Rediger}
translate O EngineRequired {Fete felter er nødvendige, de øvrige er valgfrie}

# PGN window menus:
menuText O PgnFile "Fil" 0
menuText O PgnFileCopy "Copy Game to Clipboard" 0 ;# ***
menuText O PgnFilePrint "Skriv til fil..." 0
menuText O PgnFileClose "Lukk PGN-vindu" 0
menuText O PgnOpt "Vis" 0
menuText O PgnOptColor "Vis farger" 0
menuText O PgnOptShort "Kort (3 linjer) header" 0
menuText O PgnOptSymbols "Symbolnotasjon" 0
menuText O PgnOptIndentC "Rykk inn kommentarer" 0
menuText O PgnOptIndentV "Rykk inn variasjoner" 0
menuText O PgnOptColumn "Bruk kolonner (ett trekk per linje)" 0
menuText O PgnOptSpace "Mellomrom etter trekknummer" 0
menuText O PgnOptStripMarks "Fjern fargekoder" 0
menuText O PgnOptBoldMainLine "Use Bold Text for Main Line Moves" 4 ;# ***
menuText O PgnColor "Farger" 0
menuText O PgnColorHeader "Header..." 0
menuText O PgnColorAnno "Annotasjoner..." 0
menuText O PgnColorComments "Kommentarer..." 0
menuText O PgnColorVars "Variasjoner..." 0
menuText O PgnColorBackground "Bakgrunn..." 0
# ====== TODO To be translated ======
menuText O PgnColorMain "Main line..." 0
# ====== TODO To be translated ======
menuText O PgnColorCurrent "Current move background..." 1
menuText O PgnHelp "Hjelp" 0
menuText O PgnHelpPgn "PGN-hjelp" 0
menuText O PgnHelpIndex "Innholdsfortegnelse" 0
translate O PgnWindowTitle {Game Notation - game %u} ;# ***

# Crosstable window menus:
menuText O CrosstabFile "Fil" 0
menuText O CrosstabFileText "Skriv til tekstfil..." 0
menuText O CrosstabFileHtml "Skriv til HTML-fil..." 0
menuText O CrosstabFileLaTeX "Skriv til LaTeX-fil..." 0
menuText O CrosstabFileClose "Lukk krysstabellvindu" 0
menuText O CrosstabEdit "Rediger" 0
menuText O CrosstabEditEvent "Anledning" 0
menuText O CrosstabEditSite "Sted" 0
menuText O CrosstabEditDate "Dato" 0
menuText O CrosstabOpt "Vis" 0
menuText O CrosstabOptAll "Alle-mot-alle" 0
menuText O CrosstabOptSwiss "Sveitsisk" 0
menuText O CrosstabOptKnockout "Kun vinnere" 0
menuText O CrosstabOptAuto "Auto" 0
menuText O CrosstabOptAges "Aldre i år" 0
menuText O CrosstabOptNats "Nasjonaliteter" 0
menuText O CrosstabOptRatings "Ratinger" 0
menuText O CrosstabOptTitles "Titler" 0
menuText O CrosstabOptBreaks "Poengfordel" 0
menuText O CrosstabOptDeleted "Include deleted games" 8 ;# ***
menuText O CrosstabOptColors "Farger (kun sveitsisk)" 0
menuText O CrosstabOptColumnNumbers "Nummererte kolonner (kun alle-mot-alle-tabeller)" 0
menuText O CrosstabOptGroup "Gruppepoeng" 0
menuText O CrosstabSort "Sorter" 0
menuText O CrosstabSortName "Navn" 0
menuText O CrosstabSortRating "Rating" 0
menuText O CrosstabSortScore "Poeng" 0
menuText O CrosstabColor "Farge" 0
menuText O CrosstabColorPlain "Ren tekst" 0
menuText O CrosstabColorHyper "Hypertekst" 0
menuText O CrosstabHelp "Hjelp" 0
menuText O CrosstabHelpCross "Krysstabell hjelp" 0
menuText O CrosstabHelpIndex "Innholdsfortegnelse" 0
translate O SetFilter {Sett filter}
translate O AddToFilter {Legg til i filter}
translate O Swiss {Sveitsisk}
translate O Category {Category} ;# ***

# Opening report window menus:
menuText O OprepFile "Fil" 0
menuText O OprepFileText "Skriv til tekstfil..." 0
menuText O OprepFileHtml "Skriv til HTML-fil..." 0
menuText O OprepFileLaTeX "Skriv til LaTeX-fil..." 0
menuText O OprepFileOptions "Innstillinger..." 0
menuText O OprepFileClose "Lukk rapport-vindu" 0
menuText O OprepFavorites "Favorites" 1 ;# ***
menuText O OprepFavoritesAdd "Add Report..." 0 ;# ***
menuText O OprepFavoritesEdit "Edit Report Favorites..." 0 ;# ***
menuText O OprepFavoritesGenerate "Generate Reports..." 0 ;# ***
menuText O OprepHelp "Hjelp" 0
menuText O OprepHelpReport "Åpningsrapport hjelp" 0
menuText O OprepHelpIndex "Innholdsfortegnelse" 0

# Header search:
translate O HeaderSearch {Headersøk}
translate O EndSideToMove {Side to move at end of game} ;# ***
translate O GamesWithNoECO {Partier uten ECO?}
translate O GameLength {Partilengde}
translate O FindGamesWith {Finn partier merket med...}
translate O StdStart {Vanlig utgangsstilling}
translate O Promotions {Forfremmelser}
translate O Comments {Kommentarer}
translate O Variations {Variasjoner}
translate O Annotations {Annotasjoner}
translate O DeleteFlag {Slettet}
translate O WhiteOpFlag {Hvit åpning}
translate O BlackOpFlag {Sort åpning}
translate O MiddlegameFlag {Midtspill}
translate O EndgameFlag {Sluttspill}
translate O NoveltyFlag {Avvik}
translate O PawnFlag {Bondestruktur}
translate O TacticsFlag {Taktikk}
translate O QsideFlag {Spill på dronningsiden}
translate O KsideFlag {Spill på kongesiden}
translate O BrilliancyFlag {Brillians}
translate O BlunderFlag {Tabbe}
translate O UserFlag {Bruker}
translate O PgnContains {PGN inneholder tekst}
# ====== TODO To be translated ======
translate O Annotator {Annotator}
# ====== TODO To be translated ======
translate O Cmnts {Annotated games only}

# Game list window:
translate O GlistNumber {Nummer}
translate O GlistWhite {Hvit}
translate O GlistBlack {Sort}
translate O GlistWElo {H-Elo}
translate O GlistBElo {S-Elo}
translate O GlistEvent {Anledning}
translate O GlistSite {Sted}
translate O GlistRound {Runde}
translate O GlistDate {Dato}
translate O GlistYear {År}
translate O GlistEDate {AnledningDato}
translate O GlistResult {Resultat}
translate O GlistLength {Lengde}
translate O GlistCountry {Land}
translate O GlistECO {ECO}
translate O GlistOpening {Åpning}
translate O GlistEndMaterial {Slutt-kvalitet}
translate O GlistDeleted {Slettet}
translate O GlistFlags {Flagg}
translate O GlistVars {Variasjoner}
translate O GlistComments {Kommentarer}
translate O GlistAnnos {Annotasjoner}
translate O GlistStart {Start}
translate O GlistGameNumber {Parti nummer}
# ====== TODO To be translated ======
translate O GlistAverageElo {Average Elo}
# ====== TODO To be translated ======
translate O GlistRating {Rating}
translate O GlistFindText {Finn tekst}
translate O GlistMoveField {Trekk}
translate O GlistEditField {Still inn}
translate O GlistAddField {Legg til}
translate O GlistDeleteField {Fjern}
translate O GlistWidth {Bredde}
translate O GlistAlign {Justering}
# ====== TODO To be translated ======
translate O GlistAlignL {Align: left}
# ====== TODO To be translated ======
translate O GlistAlignR {Align: right}
# ====== TODO To be translated ======
translate O GlistAlignC {Align: center}
translate O GlistColor {Farge}
translate O GlistSep {Skilletegn}
# ====== TODO To be translated ======
translate O GlistCurrentSep {-- Current --}
# ====== TODO To be translated ======
translate O GlistNewSort {New}
# ====== TODO To be translated ======
translate O GlistAddToSort {Add}
# ====== TODO To be translated ======
translate O GsortSort {Sort...}
# ====== TODO To be translated ======
translate O GsortDate {Date}
# ====== TODO To be translated ======
translate O GsortYear {Year}
# ====== TODO To be translated ======
translate O GsortEvent {Event}
# ====== TODO To be translated ======
translate O GsortSite {Site}
# ====== TODO To be translated ======
translate O GsortRound {Round}
# ====== TODO To be translated ======
translate O GsortWhiteName {White Name}
# ====== TODO To be translated ======
translate O GsortBlackName {Black Name}
# ====== TODO To be translated ======
translate O GsortECO {ECO}
# ====== TODO To be translated ======
translate O GsortResult {Result}
# ====== TODO To be translated ======
translate O GsortMoveCount {Move Count}
# ====== TODO To be translated ======
translate O GsortAverageElo {Average Elo}
# ====== TODO To be translated ======
translate O GsortCountry {Country}
# ====== TODO To be translated ======
translate O GsortDeleted {Deleted}
# ====== TODO To be translated ======
translate O GsortEventDate {Event Date}
# ====== TODO To be translated ======
translate O GsortWhiteElo {White Elo}
# ====== TODO To be translated ======
translate O GsortBlackElo {Black Elo}
# ====== TODO To be translated ======
translate O GsortComments {Comments}
# ====== TODO To be translated ======
translate O GsortVariations {Variations}
# ====== TODO To be translated ======
translate O GsortNAGs {NAGs}
# ====== TODO To be translated ======
translate O GsortAscending {Ascending}
# ====== TODO To be translated ======
translate O GsortDescending {Descending}
# ====== TODO To be translated ======
translate O GsortAdd {Add}
# ====== TODO To be translated ======
translate O GsortStore {Store}
# ====== TODO To be translated ======
translate O GsortLoad {Load}
# ====== TODO To be translated ======
translate O GlistRemoveThisGameFromFilter  {Remove this game from Filter}
# ====== TODO To be translated ======
translate O GlistRemoveGameAndAboveFromFilter  {Remove game (and all above it) from Filter}
# ====== TODO To be translated ======
translate O GlistRemoveGameAndBelowFromFilter  {Remove game (and all below it) from Filter}
# ====== TODO To be translated ======
translate O GlistDeleteGame {(Un)Delete this game} 
# ====== TODO To be translated ======
translate O GlistDeleteAllGames {Delete all games in filter} 
# ====== TODO To be translated ======
translate O GlistUndeleteAllGames {Undelete all games in filter} 
# ====== TODO To be translated ======
translate O GlistMergeGameInBase {Merge Game in other base} 

# Maintenance window:
translate O DatabaseName {Databasenavn:}
translate O TypeIcon {Ikon:}
translate O NumOfGames {Partier:}
translate O NumDeletedGames {Slettede partier:}
translate O NumFilterGames {Partier i filter:}
translate O YearRange {Til/fra år:}
translate O RatingRange {Til/fra rating:}
translate O Description {Description} ;# ***
translate O Flag {Flagg}
# ====== TODO To be translated ======
translate O CustomFlags {Custom flags}
translate O DeleteCurrent {Slett gjeldende parti}
translate O DeleteFilter {Slett filtrerte partier}
translate O DeleteAll {Slett alle partier}
translate O UndeleteCurrent {Gjenopprett gjeldende parti}
translate O UndeleteFilter {Gjenopprett filtrerte partier}
translate O UndeleteAll {Gjenopprett alle partier}
translate O DeleteTwins {Slett duplikatpartier}
translate O MarkCurrent {Merk gjeldende parti}
translate O MarkFilter {Merk filtrerte partier}
translate O MarkAll {Merk alle partier}
translate O UnmarkCurrent {Fjern merking på gjeldende parti}
translate O UnmarkFilter {Fjern merking på filtrerte partier}
translate O UnmarkAll {Fjern merking på alle partier}
translate O Spellchecking {Stavekontroll}
translate O Players {Spillere}
translate O Events {Anledninger}
translate O Sites {Steder}
translate O Rounds {Runder}
translate O DatabaseOps {Databasebehandling}
translate O ReclassifyGames {ECO-klassifiser partier}
translate O CompactDatabase {Komprimer database}
translate O SortDatabase {Sorter database}
translate O AddEloRatings {Legg til ELO-ratinger}
translate O AutoloadGame {Start med parti nummer}
translate O StripTags {Fjern PGN-merker}
translate O StripTag {Fjern merke}
translate O Cleaner {Opprydding}
translate O CleanerHelp {
Scid-opprydding vil utføre alle handlinger du velger fra listen under på gjeldende database.

Gjeldende innstillinger i ECO-klassifiseringen og duplikatslettingdialogene vil virke inn om du velger disse funksjonene.
}
translate O CleanerConfirm {
Når oppryddingen er i gang kan den ikke avbrytes!

Dette kan ta lang tid på en stor databse, avhengig av funksjonene du har valgt og disses gjeldende innstillinger.

Er du sikker på at du vil starte vedlikeholdsfunksjonene du har valgt?
}
# ====== TODO To be translated ======
translate O TwinCheckUndelete {to flip; "u" undeletes both)}
# ====== TODO To be translated ======
translate O TwinCheckprevPair {Previous pair}
# ====== TODO To be translated ======
translate O TwinChecknextPair {Next pair}
# ====== TODO To be translated ======
translate O TwinChecker {Scid: Twin game checker}
# ====== TODO To be translated ======
translate O TwinCheckTournament {Games in tournament:}
# ====== TODO To be translated ======
translate O TwinCheckNoTwin {No twin  }
# ====== TODO To be translated ======
translate O TwinCheckNoTwinfound {No twin was detected for this game.\nTo show twins using this window, you must first use the "Delete twin games..." function. }
# ====== TODO To be translated ======
translate O TwinCheckTag {Share tags...}
# ====== TODO To be translated ======
translate O TwinCheckFound1 {Scid found $result twin games}
# ====== TODO To be translated ======
translate O TwinCheckFound2 { and set their delete flags}
# ====== TODO To be translated ======
translate O TwinCheckNoDelete {There are no games in this database to delete.}
# ====== TODO To be translated ======
translate O TwinCriteria1 { Your settings for finding twin games are potentially likely to\ncause non-twin games with similar moves to be marked as twins.}
# ====== TODO To be translated ======
translate O TwinCriteria2 {It is recommended that if you select "No" for "same moves", you should select "Yes" for the colors, event, site, round, year and month settings.\nDo you want to continue and delete twins anyway? }
# ====== TODO To be translated ======
translate O TwinCriteria3 {It is recommended that you specify "Yes" for at least two of the "same site", "same round" and "same year" settings.\nDo you want to continue and delete twins anyway?}
# ====== TODO To be translated ======
translate O TwinCriteriaConfirm {Scid: Confirm twin settings}
# ====== TODO To be translated ======
translate O TwinChangeTag "Change the following game tags:\n\n"
# ====== TODO To be translated ======
translate O AllocRatingDescription "This command will use the current spellcheck file to add Elo ratings to games in this database. Wherever a player has no currrent rating but his/her rating at the time of the game is listed in the spellcheck file, that rating will be added."
# ====== TODO To be translated ======
translate O RatingOverride "Overwrite existing non-zero ratings?"
# ====== TODO To be translated ======
translate O AddRatings "Add ratings to:"
# ====== TODO To be translated ======
translate O AddedRatings {Scid added $r Elo ratings in $g games.}
# ====== TODO To be translated ======
translate O NewSubmenu "New submenu"

# Comment editor:
translate O AnnotationSymbols  {Notasjonssymboler:}
translate O Comment {Kommentar:}
translate O InsertMark {Insert mark} ;# ***
translate O InsertMarkHelp {
Insert/remove mark: Select color, type, square.
Insert/remove arrow: Right-click two squares.
} ;# ***

# Nag buttons in comment editor:
translate O GoodMove {Good move} ;# ***
translate O PoorMove {Poor move} ;# ***
translate O ExcellentMove {Excellent move} ;# ***
translate O Blunder {Tabbe}
translate O InterestingMove {Interesting move} ;# ***
translate O DubiousMove {Dubious move} ;# ***
translate O WhiteDecisiveAdvantage {White has a decisive advantage} ;# ***
translate O BlackDecisiveAdvantage {Black has a decisive advantage} ;# ***
translate O WhiteClearAdvantage {White has a clear advantage} ;# ***
translate O BlackClearAdvantage {Black has a clear advantage} ;# ***
translate O WhiteSlightAdvantage {White has a slight advantage} ;# ***
translate O BlackSlightAdvantage {Black has a slight advantage} ;# ***
translate O Equality {Equality} ;# ***
translate O Unclear {Unclear} ;# ***
translate O Diagram {Diagram} ;# ***

# Board search:
translate O BoardSearch {Stillingsøk}
translate O FilterOperation {Operasjoner på gjeldende filter:}
translate O FilterAnd {AND (Kun på filter)}
translate O FilterOr {OR (Legg til filter)}
translate O FilterIgnore {IGNORE (Nullstill filter)}
translate O SearchType {Søketype:}
translate O SearchBoardExact {Nøyaktig stilling (alle brikker på samme felt)}
translate O SearchBoardPawns {Bønder (samme kvalitet, alle bønder på samme felt)}
translate O SearchBoardFiles {Kolonner (samme kvalitet, alle bønder på samme kolonner)}
translate O SearchBoardAny {Alle (samme kvalitet, bønder og brikker hvor som helst)}
# ====== TODO To be translated ======
translate O SearchInRefDatabase { Search in base }
translate O LookInVars {Søk i variasjoner}

# Material search:
translate O MaterialSearch {Kvalitetssøk}
translate O Material {Kvalitet}
translate O Patterns {Mønstre}
translate O Zero {Null}
translate O Any {Hvilke som helst}
translate O CurrentBoard {Gjeldende stilling}
translate O CommonEndings {Vanlige sluttspill}
translate O CommonPatterns {Vanlige mønstre}
translate O MaterialDiff {Kvalitetsforskjell}
translate O squares {felt}
translate O SameColor {Samme farge}
translate O OppColor {Motsatt farge}
translate O Either {Begge}
translate O MoveNumberRange {Fra/til trekk nummer}
translate O MatchForAtLeast {Treff i minst}
translate O HalfMoves {halvtrekk}

# Common endings in material search:
translate O EndingPawns {Pawn endings} ;# ***
translate O EndingRookVsPawns {Rook vs. Pawn(s)} ;# ***
translate O EndingRookPawnVsRook {Rook and 1 Pawn vs. Rook} ;# ***
translate O EndingRookPawnsVsRook {Rook and Pawn(s) vs. Rook} ;# ***
translate O EndingRooks {Rook vs. Rook endings} ;# ***
translate O EndingRooksPassedA {Rook vs. Rook endings with a passed a-pawn} ;# ***
translate O EndingRooksDouble {Double Rook endings} ;# ***
translate O EndingBishops {Bishop vs. Bishop endings} ;# ***
translate O EndingBishopVsKnight {Bishop vs. Knight endings} ;# ***
translate O EndingKnights {Knight vs. Knight endings} ;# ***
translate O EndingQueens {Queen vs. Queen endings} ;# ***
translate O EndingQueenPawnVsQueen {Queen and 1 Pawn vs. Queen} ;# ***
translate O BishopPairVsKnightPair {Two Bishops vs. Two Knights middlegame} ;# ***

# Common patterns in material search:
translate O PatternWhiteIQP {White IQP} ;# ***
translate O PatternWhiteIQPBreakE6 {White IQP: d4-d5 break vs. e6} ;# ***
translate O PatternWhiteIQPBreakC6 {White IQP: d4-d5 break vs. c6} ;# ***
translate O PatternBlackIQP {Black IQP} ;# ***
translate O PatternWhiteBlackIQP {White IQP vs. Black IQP} ;# ***
translate O PatternCoupleC3D4 {White c3+d4 Isolated Pawn Couple} ;# ***
translate O PatternHangingC5D5 {Black Hanging Pawns on c5 and d5} ;# ***
translate O PatternMaroczy {Maroczy Center (with Pawns on c4 and e4)} ;# ***
translate O PatternRookSacC3 {Rook Sacrifice on c3} ;# ***
translate O PatternKc1Kg8 {O-O-O vs. O-O (Kc1 vs. Kg8)} ;# ***
translate O PatternKg1Kc8 {O-O vs. O-O-O (Kg1 vs. Kc8)} ;# ***
translate O PatternLightFian {Light-Square Fianchettos (Bishop-g2 vs. Bishop-b7)} ;# ***
translate O PatternDarkFian {Dark-Square Fianchettos (Bishop-b2 vs. Bishop-g7)} ;# ***
translate O PatternFourFian {Four Fianchettos (Bishops on b2,g2,b7,g7)} ;# ***

# Game saving:
translate O Today {I dag}
translate O ClassifyGame {Klassifiser parti}

# Setup position:
translate O EmptyBoard {Tøm brett}
translate O InitialBoard {Utgangsstilling}
translate O SideToMove {Side i trekk}
translate O MoveNumber {Trekk nummer}
translate O Castling {Rokkade}
translate O EnPassantFile {En Passant-kolonne}
translate O ClearFen {Slett FEN}
translate O PasteFen {Lim inn FEN}
# ====== TODO To be translated ======
translate O SaveAndContinue {Save and continue}
# ====== TODO To be translated ======
translate O DiscardChangesAndContinue {Discard changes\nand continue}
# ====== TODO To be translated ======
translate O GoBack {Go back}

# Replace move dialog:
translate O ReplaceMove {Erstatt trekk}
translate O AddNewVar {Legg til ny variasjon}
# ====== TODO To be translated ======
translate O NewMainLine {New Main Line}
translate O ReplaceMoveMessage {Et trekk eksisterer allerede her.

Du kan erstatte det og slette alle følgende trekk, eller du kan legge det til som en variasjon.

(Du kan unngå denne meldingen i fremtiden ved å slå av Spør før trekk erstattes-innstillingen i Innstillinger:Trekk-menyen.)}

# Make database read-only dialog:
translate O ReadOnlyDialog {Hvis du skrivebeskytter denne databasen tillates ikke lenger endringer på den. Ingen partier kan lagres eller erstattes, og ingen sletteflagg kan endres. All sortering eller ECO-klassifisering vil være midlertidig.

Du kan enkelt slå av skrivebeskyttelsen på databasen ved å lukke og gjenåpne den.

Vil du virkelig skrivebeskytte denne databasen?}

# Clear game dialog:
translate O ClearGameDialog {Dette partiet har blitt endret.

Vil du virkelig forkaste endringene som er gjort?
}

# Exit dialog:
translate O ExitDialog {Vil du virkelig avslutte Scid?}
translate O ExitUnsaved {Følgende baser har ulagrede endringer i partier. Hvis du avslutter nå vil disse endringene gå tapt.}

# Import window:
translate O PasteCurrentGame {Lim inn gjeldende parti}
translate O ImportHelp1 {Skriv eller lim inn et PGN-parti i feltet over.}
translate O ImportHelp2 {Feil i importen vil vises her.}
# ====== TODO To be translated ======
translate O OverwriteExistingMoves {Overwrite existing moves ?}

# ECO Browser:
translate O ECOAllSections {alle ECO-deler}
translate O ECOSection {ECO-del}
translate O ECOSummary {Sammendrag for}
translate O ECOFrequency {Hyppighet eller underkoder for}

# Opening Report:
translate O OprepTitle {Åpningsrapport}
translate O OprepReport {Rapport}
translate O OprepGenerated {Generert av}
translate O OprepStatsHist {Statistikk og historie}
translate O OprepStats {Statistikk}
translate O OprepStatAll {Alle rapporterte partier}
translate O OprepStatBoth {Begge med rating}
translate O OprepStatSince {Siden}
translate O OprepOldest {Eldste partier}
translate O OprepNewest {Nyeste partier}
translate O OprepPopular {Trend}
translate O OprepFreqAll {Frekvens i år:   }
translate O OprepFreq1   {Det siste året: }
translate O OprepFreq5   {De siste 5 årene: }
translate O OprepFreq10  {De siste 10 årene: }
translate O OprepEvery {hvert %u parti}
translate O OprepUp {opp %u%s fra alle år}
translate O OprepDown {ned %u%s fra alle år}
translate O OprepSame {ingen endring fra alle år}
translate O OprepMostFrequent {Hyppigste spillere}
translate O OprepMostFrequentOpponents {Most frequent opponents} ;# ***
translate O OprepRatingsPerf {Ratinger og resultater}
translate O OprepAvgPerf {Middelratinger og -resultater}
translate O OprepWRating {Hvit rating}
translate O OprepBRating {Sort rating}
translate O OprepWPerf {Hvite resultater}
translate O OprepBPerf {Sorte resultater}
translate O OprepHighRating {Parti med høyeste gjennomsnittsrating}
translate O OprepTrends {Resultattrend}
translate O OprepResults {Resultatlengde og -hyppighet}
translate O OprepLength {Partilengde}
translate O OprepFrequency {Hyppighet}
translate O OprepWWins {Hvit vinner: }
translate O OprepBWins {Sort vinner: }
translate O OprepDraws {Uavgjort:      }
translate O OprepWholeDB {hele databasen}
translate O OprepShortest {Korteste seire}
translate O OprepMovesThemes {Trekk og temaer}
translate O OprepMoveOrders {Trekkrekkefølger til rapportert stilling}
translate O OprepMoveOrdersOne \
  {Det var kun en trekkrekkefølge som førte til denne stillingen:}
translate O OprepMoveOrdersAll \
  {Det var %u trekkrekkefølger som førte til denne stillingen:}
translate O OprepMoveOrdersMany \
  {There were %u move orders reaching this position. The top %u are:}
translate O OprepMovesFrom {Trekk fra rapportert stilling}
translate O OprepMostFrequentEcoCodes {Most frequent ECO codes} ;# ***
translate O OprepThemes {Stillingstemaer}
translate O OprepThemeDescription {Frequency of themes in the first %u moves of each game} ;# ***
translate O OprepThemeSameCastling {Rokkade til samme side}
translate O OprepThemeOppCastling {Rokkade til motsatt side}
translate O OprepThemeNoCastling {Ingen konger rokert}
translate O OprepThemeKPawnStorm {Bondestorm på kongesiden}
translate O OprepThemeQueenswap {Byttede dronninger}
translate O OprepThemeWIQP {White Isolated Queen Pawn} ;# ***
translate O OprepThemeBIQP {Black Isolated Queen Pawn} ;# ***
translate O OprepThemeWP567 {Hvit bonde på rekke 5/6/7}
translate O OprepThemeBP234 {Sort bonde på rekke 2/3/4}
translate O OprepThemeOpenCDE {Åpen c/d/e-kolonne}
translate O OprepTheme1BishopPair {Kun en side har to løpere}
translate O OprepEndgames {Sluttspill}
translate O OprepReportGames {Rapporterte partier}
translate O OprepAllGames    {Alle partier}
translate O OprepEndClass {Materiale i sluttspillet}
translate O OprepTheoryTable {Teoritabell}
translate O OprepTableComment {Generert fra de %u høyest ratede partiene.}
translate O OprepExtraMoves {Ytterligere trekk notert i teoritabellen}
translate O OprepMaxGames {Maksimalt antall partier i teoritabellen}
translate O OprepViewHTML {View HTML} ;# ***
translate O OprepViewLaTeX {View LaTeX} ;# ***

# Player Report:
translate O PReportTitle {Player Report} ;# ***
translate O PReportColorWhite {with the White pieces} ;# ***
translate O PReportColorBlack {with the Black pieces} ;# ***
translate O PReportMoves {after %s} ;# ***
translate O PReportOpenings {Openings} ;# ***
translate O PReportClipbase {Empty clipbase and copy matching games to it} ;# ***

# Piece Tracker window:
translate O TrackerSelectSingle {Venstre museknapp merker denne brikken}
translate O TrackerSelectPair {Venstre museknapp merker denne brikken; høyre merker alle brikker av denne typen.}
translate O TrackerSelectPawn {Venstre museknapp merker denne brikken; høyre merker alle brikker av denne typen.}
translate O TrackerStat {Statistikk}
translate O TrackerGames {% partier med trekk til felt}
translate O TrackerTime {% tid på hvert felt}
translate O TrackerMoves {Trekk}
translate O TrackerMovesStart {Oppgi trekket hvor sporingen skal begynne.}
translate O TrackerMovesStop {Oppgi trekket hvor sporingen skal stoppe.}

# Game selection dialogs:
translate O SelectAllGames {Alle partier i databasen}
translate O SelectFilterGames {Kun partier i filteret}
translate O SelectTournamentGames {Kun partier i gjeldende turnering}
translate O SelectOlderGames {Kun eldre partier}

# Delete Twins window:
translate O TwinsNote {For å være duplikater må to partier minst ha de samme to spillerene, og kriterier du kan angi under. Når to duplikater finnes blir det korteste partiet slettet. Tips: Det er best å kjøre en stavekontroll før duplikatsjekken, da det forbedrer søket.}
translate O TwinsCriteria {Kriterier: Duplikater må ha...}
translate O TwinsWhich {Undersøk hvilke partier}
translate O TwinsColors {Spillere samme farge?}
translate O TwinsEvent {Samme anledning?}
translate O TwinsSite {Samme sted?}
translate O TwinsRound {Samme runde?}
translate O TwinsYear {Samme år?}
translate O TwinsMonth {Samme måned?}
translate O TwinsDay {Samme dag?}
translate O TwinsResult {Samme utfall?}
translate O TwinsECO {Samme ECO-kode?}
translate O TwinsMoves {Samme trekk?}
translate O TwinsPlayers {Sammenlign spillernavn:}
translate O TwinsPlayersExact {Nøyaktig like}
translate O TwinsPlayersPrefix {Kun første 4 bokstaver}
translate O TwinsWhen {Når duplikatpartier slettes}
translate O TwinsSkipShort {Overse alle partier kortere enn 5 trekk?}
translate O TwinsUndelete {Gjenopprett alle partier først?}
translate O TwinsSetFilter {Sett filter til alle slettede duplikatpartier?}
translate O TwinsComments {Behold alltid partier med kommentarer?}
translate O TwinsVars {Behold alltid partier med variasjoner?}
translate O TwinsDeleteWhich {Slett hvilket parti:}
translate O TwinsDeleteShorter {Korteste parti}
translate O TwinsDeleteOlder {Laveste partinummer}
translate O TwinsDeleteNewer {Høyeste partinummer}
translate O TwinsDelete {Slett partier}

# Name editor window:
translate O NameEditType {Navnetype som skal redigeres}
translate O NameEditSelect {Partier som skal redigeres}
translate O NameEditReplace {Erstatt}
translate O NameEditWith {med}
translate O NameEditMatches {Treff: Trykk Ctrl+1 til Ctrl+9 for å velge}
# ====== TODO To be translated ======
translate O CheckGames {Check games}
# ====== TODO To be translated ======
translate O CheckGamesWhich {Check games}
# ====== TODO To be translated ======
translate O CheckAll {All games}
# ====== TODO To be translated ======
translate O CheckSelectFilterGames {Only games in filter}

# Classify window:
translate O Classify {Klassifiser}
translate O ClassifyWhich {ECO-klassifiser hvilke partier}
translate O ClassifyAll {Alle partier (overskriv gamle ECO-koder)}
translate O ClassifyYear {Aller partier det siste året}
translate O ClassifyMonth {Alle partier den siste måneden}
translate O ClassifyNew {Kun partier uten ECO-kode}
translate O ClassifyCodes {ECO-koder som skal brukes}
translate O ClassifyBasic {Kun standardkoder ("B12", ...)}
translate O ClassifyExtended {Scid-utvidet ("B12j", ...)}

# Compaction:
translate O NameFile {Navnefil}
translate O GameFile {Partifil}
translate O Names {Navn}
translate O Unused {Ubrukte}
translate O SizeKb {Størrelse (kb)}
translate O CurrentState {Nåværende}
translate O AfterCompaction {Etter komprimering}
translate O CompactNames {Komrimer navnefil}
translate O CompactGames {Komprimer partifil}
# ====== TODO To be translated ======
translate O NoUnusedNames "There are no unused names, so the name file is already fully compacted."
# ====== TODO To be translated ======
translate O NoUnusedGames "The game file is already fully compacted."
# ====== TODO To be translated ======
translate O GameFileCompacted {The game file for the database was compacted.}

# Sorting:
translate O SortCriteria {Kriterier}
translate O AddCriteria {Legg til kriterie}
translate O CommonSorts {Vanlige sorteringer}
translate O Sort {Sorter}

# Exporting:
# These lines do not use the excact notation of the original english.tcl, because this would cause inconsistency with the PGN window menus
translate O AddToExistingFile {Legg partier til en eksisterende fil}
translate O ExportComments {Eksporter kommentarer}
translate O ExportVariations {Eksporter variasjoner}
translate O IndentComments {Rykk inn kommentarer}
translate O IndentVariations {Rykk inn variasjoner}
translate O ExportColumnStyle {Bruk kolonner (ett trekk per linje)}
translate O ExportSymbolStyle {Symbolnotasjoner}
translate O ExportStripMarks {Fjern alle fargekoder og piler fra kommentarer?}

# Goto game/move dialogs:
translate O LoadGameNumber {Angi partinummer som skal lastes inn:}
translate O GotoMoveNumber {Gå til trekk nummer:}

# Copy games dialog:
translate O CopyGames {Kopier partier}
translate O CopyConfirm {
Vil du virkelig kopiere de [::utils::thousands $nGamesToCopy] filtrerte partiene fra "$fromName"
 til databasen "$targetName"?
}
translate O CopyErr {Kan ikke kopiere partiene}
translate O CopyErrSource {kildedatabasen}
translate O CopyErrTarget {måldatabasen}
translate O CopyErrNoGames {har ingen partier i sitt filter}
translate O CopyErrReadOnly {er skrivebeskyttet}
translate O CopyErrNotOpen {er ikke åpnet}

# Colors:
translate O LightSquares {Lyse felt}
translate O DarkSquares {Mørke felt}
translate O SelectedSquares {Merkede felt}
translate O SuggestedSquares {Foreslåtte trekkfelt}
translate O WhitePieces {Hvite brikker}
translate O BlackPieces {Sorte brikker}
translate O WhiteBorder {Hvit ramme}
translate O BlackBorder {Sort ramme}

# Novelty window:
translate O FindNovelty {Finn avvik}
translate O Novelty {Avvik}
translate O NoveltyInterrupt {Avvikssøk avbrutt}
translate O NoveltyNone {Ingen avvik ble funnet i dette partiet}
translate O NoveltyHelp {
Scid vil finne det første trekket i det gjeldende partiet som fører til en stilling som ikke finnes i den valgte databasen eller i ECO-åpningsboken.}

# Sounds configuration:
translate O SoundsFolder {Sound Files Folder} ;# ***
translate O SoundsFolderHelp {The folder should contain the files King.wav, a.wav, 1.wav, etc} ;# ***
translate O SoundsAnnounceOptions {Move Announcement Options} ;# ***
translate O SoundsAnnounceNew {Announce new moves as they are made} ;# ***
translate O SoundsAnnounceForward {Announce moves when moving forward one move} ;# ***
translate O SoundsAnnounceBack {Announce when retracting or moving back one move} ;# ***
# ====== TODO To be translated ======
translate O SoundsSoundDisabled {Scid could not find the Snack audio package at startup;\nSound is disabled.}

# Upgrading databases:
translate O Upgrading {Oppgraderer}
translate O ConfirmOpenNew {
Dette er en database i gammelt format (Scid 3) som ikke kan åpnes i Scid 4, men en ny versjon (Scid 4) har allerede blitt opprettet.

Vil du åpne versjonen som er i det nye formatet?
}
translate O ConfirmUpgrade {
Dette er en database i gammelt format (Scid3). En versjon i nytt format må opprettes før den kan brukes i Scid 4.

Oppgradering vil opprette en ny versjon av databasen.

Dette kan ta en stund, men det trenger kun å gjøres en gang. Du kan avbryte om det tar for lang tid.

Ønsker du å oppgradere databasen nå?
}

# Recent files options:
translate O RecentFilesMenu {Number of recent files in File menu} ;# ***
translate O RecentFilesExtra {Number of recent files in extra submenu} ;# ***

# My Player Names options:
translate O MyPlayerNamesDescription {
Enter a list of preferred player names below, one name per line. Wildcards (e.g. "?" for any single character, "*" for any sequence of characters) are permitted.

Every time a game with a player in the list is loaded, the main window chessboard will be rotated if necessary to show the game from that players perspective.
} ;# ***
# ====== TODO To be translated ======
translate O showblunderexists {show blunder exists}
# ====== TODO To be translated ======
translate O showblundervalue {show blunder value}
# ====== TODO To be translated ======
translate O showscore {show score}
# ====== TODO To be translated ======
translate O coachgame {coach game}
# ====== TODO To be translated ======
translate O configurecoachgame {configure coach game}
# ====== TODO To be translated ======
translate O configuregame {Game configuration}
# ====== TODO To be translated ======
translate O Phalanxengine {Phalanx engine}
# ====== TODO To be translated ======
translate O Coachengine {Coach engine}
# ====== TODO To be translated ======
translate O difficulty {difficulty}
# ====== TODO To be translated ======
translate O hard {hard}
# ====== TODO To be translated ======
translate O easy {easy}
# ====== TODO To be translated ======
translate O Playwith {Play with}
# ====== TODO To be translated ======
translate O white {white}
# ====== TODO To be translated ======
translate O black {black}
# ====== TODO To be translated ======
translate O both {both}
# ====== TODO To be translated ======
translate O Play {Play}
# ====== TODO To be translated ======
translate O Noblunder {No blunder}
# ====== TODO To be translated ======
translate O blunder {blunder}
# ====== TODO To be translated ======
translate O Noinfo {-- No info --}
# ====== TODO To be translated ======
translate O PhalanxOrTogaMissing {Phalanx or Toga not found}
# ====== TODO To be translated ======
translate O moveblunderthreshold {move is a blunder if loss is greater than}
# ====== TODO To be translated ======
translate O limitanalysis {limit engine analysis time}
# ====== TODO To be translated ======
translate O seconds {seconds}
# ====== TODO To be translated ======
translate O Abort {Abort}
# ====== TODO To be translated ======
translate O Resume {Resume}
# ====== TODO To be translated ======
translate O OutOfOpening {Out of opening}
# ====== TODO To be translated ======
translate O NotFollowedLine {You did not follow the line}
# ====== TODO To be translated ======
translate O DoYouWantContinue {Do you want yo continue ?}
# ====== TODO To be translated ======
translate O CoachIsWatching {Coach is watching}
# ====== TODO To be translated ======
translate O Ponder {Permanent thinking}
# ====== TODO To be translated ======
translate O LimitELO {Limit ELO strength}
# ====== TODO To be translated ======
translate O DubiousMovePlayedTakeBack {Dubious move played, do you want to take back ?}
# ====== TODO To be translated ======
translate O WeakMovePlayedTakeBack {Weak move played, do you want to take back ?}
# ====== TODO To be translated ======
translate O BadMovePlayedTakeBack {Bad move played, do you want to take back ?}
# ====== TODO To be translated ======
translate O Iresign {I resign}
# ====== TODO To be translated ======
translate O yourmoveisnotgood {your move is not good}
# ====== TODO To be translated ======
translate O EndOfVar {End of variation}
# ====== TODO To be translated ======
translate O Openingtrainer {Opening trainer}
# ====== TODO To be translated ======
translate O DisplayCM {Display candidate moves}
# ====== TODO To be translated ======
translate O DisplayCMValue {Display candidate moves value}
# ====== TODO To be translated ======
translate O DisplayOpeningStats {Show statistics}
# ====== TODO To be translated ======
translate O ShowReport {Show report}
# ====== TODO To be translated ======
translate O NumberOfGoodMovesPlayed {good moves played}
# ====== TODO To be translated ======
translate O NumberOfDubiousMovesPlayed {dubious moves played}
# ====== TODO To be translated ======
translate O NumberOfMovesPlayedNotInRepertoire {moves played not in repertoire}
# ====== TODO To be translated ======
translate O NumberOfTimesPositionEncountered {times position encountered}
# ====== TODO To be translated ======
translate O PlayerBestMove  {Allow only best moves}
# ====== TODO To be translated ======
translate O OpponentBestMove {Opponent plays best moves}
# ====== TODO To be translated ======
translate O OnlyFlaggedLines {Only flagged lines}
# ====== TODO To be translated ======
translate O resetStats {Reset statistics}
# ====== TODO To be translated ======
translate O Repertoiretrainingconfiguration {Repertoire training configuration}
# ====== TODO To be translated ======
translate O Loadingrepertoire {Loading repertoire}
# ====== TODO To be translated ======
translate O Movesloaded {Moves loaded}
# ====== TODO To be translated ======
translate O Repertoirenotfound {Repertoire not found}
# ====== TODO To be translated ======
translate O Openfirstrepertoirewithtype {Open first a repertoire database with icon/type set to the right side}
# ====== TODO To be translated ======
translate O Movenotinrepertoire {Move not in repertoire}
# ====== TODO To be translated ======
translate O PositionsInRepertoire {Positions in repertoire}
# ====== TODO To be translated ======
translate O PositionsNotPlayed {Positions not played}
# ====== TODO To be translated ======
translate O PositionsPlayed {Positions played}
# ====== TODO To be translated ======
translate O Success {Success}
# ====== TODO To be translated ======
translate O DubiousMoves {Dubious moves}
# ====== TODO To be translated ======
translate O OutOfRepertoire {OutOfRepertoire}
# ====== TODO To be translated ======
translate O ConfigureTactics {Configure tactics}
# ====== TODO To be translated ======
translate O ResetScores {Reset scores}
# ====== TODO To be translated ======
translate O LoadingBase {Loading base}
# ====== TODO To be translated ======
translate O Tactics {Tactics}
# ====== TODO To be translated ======
translate O ShowSolution {Show solution}
# ====== TODO To be translated ======
translate O NextExercise {Next exercise}
# ====== TODO To be translated ======
translate O PrevExercise {previous exercise}
# ====== TODO To be translated ======
translate O StopTraining {Stop training}
# ====== TODO To be translated ======
translate O Next {Next}
# ====== TODO To be translated ======
translate O ResettingScore {Resetting score}
# ====== TODO To be translated ======
translate O LoadingGame {Loading game}
# ====== TODO To be translated ======
translate O MateFound {Mate found}
# ====== TODO To be translated ======
translate O BestSolutionNotFound {Best solution NOT found !}
# ====== TODO To be translated ======
translate O MateNotFound {Mate not found}
# ====== TODO To be translated ======
translate O ShorterMateExists {Shorter mate exists}
# ====== TODO To be translated ======
translate O ScorePlayed {Score played}
# ====== TODO To be translated ======
translate O Expected {expected}
# ====== TODO To be translated ======
translate O ChooseTrainingBase {Choose training base}
# ====== TODO To be translated ======
translate O Thinking {Thinking}
# ====== TODO To be translated ======
translate O AnalyzeDone {Analyze done}
# ====== TODO To be translated ======
translate O WinWonGame {Win won game}
# ====== TODO To be translated ======
translate O Lines {Lines}
# ====== TODO To be translated ======
translate O ConfigureUCIengine {Configure UCI engine}
# ====== TODO To be translated ======
translate O SpecificOpening {Specific opening}
# ====== TODO To be translated ======
translate O StartNewGame {Start new game}
# ====== TODO To be translated ======
translate O FixedLevel {Fixed level}
# ====== TODO To be translated ======
translate O Opening {Opening}
# ====== TODO To be translated ======
translate O RandomLevel {Random level}
# ====== TODO To be translated ======
translate O StartFromCurrentPosition {Start from current position}
# ====== TODO To be translated ======
translate O FixedDepth {Fixed depth}
# ====== TODO To be translated ======
translate O Nodes {Nodes} 
# ====== TODO To be translated ======
translate O Depth {Depth}
# ====== TODO To be translated ======
translate O Time {Time} 
# ====== TODO To be translated ======
translate O SecondsPerMove {Seconds per move}
# ====== TODO To be translated ======
translate O Engine {Engine}
# ====== TODO To be translated ======
translate O TimeMode {Time mode}
# ====== TODO To be translated ======
translate O TimeBonus {Time + bonus}
# ====== TODO To be translated ======
translate O TimeMin {min}
# ====== TODO To be translated ======
translate O TimeSec {sec}
# ====== TODO To be translated ======
translate O AllExercisesDone {All exercises done}
# ====== TODO To be translated ======
translate O MoveOutOfBook {Move out of book}
# ====== TODO To be translated ======
translate O LastBookMove {Last book move}
# ====== TODO To be translated ======
translate O AnnotateSeveralGames {Annotate several games\nfrom current to :}
# ====== TODO To be translated ======
translate O FindOpeningErrors {Find opening errors}
# ====== TODO To be translated ======
translate O MarkTacticalExercises {Mark tactical exercises}
# ====== TODO To be translated ======
translate O UseBook {Use book}
# ====== TODO To be translated ======
translate O MultiPV {Multiple variations}
# ====== TODO To be translated ======
translate O Hash {Hash memory}
# ====== TODO To be translated ======
translate O OwnBook {Use engine book}
# ====== TODO To be translated ======
translate O BookFile {Opening book}
# ====== TODO To be translated ======
translate O AnnotateVariations {Annotate variations}
# ====== TODO To be translated ======
translate O ShortAnnotations {Short annotations}
# ====== TODO To be translated ======
translate O addAnnotatorTag {Add annotator tag}
# ====== TODO To be translated ======
translate O AddScoreToShortAnnotations {Add score to short annotations}
# ====== TODO To be translated ======
translate O Export {Export}
# ====== TODO To be translated ======
translate O BookPartiallyLoaded {Book partially loaded}
# ====== TODO To be translated ======
translate O Calvar {Calculation of variations}
# ====== TODO To be translated ======
translate O ConfigureCalvar {Configuration}
# ====== TODO To be translated ======
translate O Reti {Reti}
# ====== TODO To be translated ======
translate O English {English}
# ====== TODO To be translated ======
translate O d4Nf6Miscellaneous {1.d4 Nf6 Miscellaneous}
# ====== TODO To be translated ======
translate O Trompowsky {Trompowsky}
# ====== TODO To be translated ======
translate O Budapest {Budapest}
# ====== TODO To be translated ======
translate O OldIndian {Old Indian}
# ====== TODO To be translated ======
translate O BenkoGambit {Benko Gambit}
# ====== TODO To be translated ======
translate O ModernBenoni {Modern Benoni}
# ====== TODO To be translated ======
translate O DutchDefence {Dutch Defence}
# ====== TODO To be translated ======
translate O Scandinavian {Scandinavian}
# ====== TODO To be translated ======
translate O AlekhineDefence {Alekhine Defence}
# ====== TODO To be translated ======
translate O Pirc {Pirc}
# ====== TODO To be translated ======
translate O CaroKann {Caro-Kann}
# ====== TODO To be translated ======
translate O CaroKannAdvance {Caro-Kann Advance}
# ====== TODO To be translated ======
translate O Sicilian {Sicilian}
# ====== TODO To be translated ======
translate O SicilianAlapin {Sicilian Alapin}
# ====== TODO To be translated ======
translate O SicilianClosed {Sicilian Closed}
# ====== TODO To be translated ======
translate O SicilianRauzer {Sicilian Rauzer}
# ====== TODO To be translated ======
translate O SicilianDragon {Sicilian Dragon}
# ====== TODO To be translated ======
translate O SicilianScheveningen {Sicilian Scheveningen}
# ====== TODO To be translated ======
translate O SicilianNajdorf {Sicilian Najdorf}
# ====== TODO To be translated ======
translate O OpenGame {Open Game}
# ====== TODO To be translated ======
translate O Vienna {Vienna}
# ====== TODO To be translated ======
translate O KingsGambit {King's Gambit}
# ====== TODO To be translated ======
translate O RussianGame {Russian Game}
# ====== TODO To be translated ======
translate O ItalianTwoKnights {Italian/Two Knights}
# ====== TODO To be translated ======
translate O Spanish {Spanish}
# ====== TODO To be translated ======
translate O SpanishExchange {Spanish Exchange}
# ====== TODO To be translated ======
translate O SpanishOpen {Spanish Open}
# ====== TODO To be translated ======
translate O SpanishClosed {Spanish Closed}
# ====== TODO To be translated ======
translate O FrenchDefence {French Defence}
# ====== TODO To be translated ======
translate O FrenchAdvance {French Advance}
# ====== TODO To be translated ======
translate O FrenchTarrasch {French Tarrasch}
# ====== TODO To be translated ======
translate O FrenchWinawer {French Winawer}
# ====== TODO To be translated ======
translate O FrenchExchange {French Exchange}
# ====== TODO To be translated ======
translate O QueensPawn {Queen's Pawn}
# ====== TODO To be translated ======
translate O Slav {Slav}
# ====== TODO To be translated ======
translate O QGA {QGA}
# ====== TODO To be translated ======
translate O QGD {QGD}
# ====== TODO To be translated ======
translate O QGDExchange {QGD Exchange}
# ====== TODO To be translated ======
translate O SemiSlav {Semi-Slav}
# ====== TODO To be translated ======
translate O QGDwithBg5 {QGD with Bg5}
# ====== TODO To be translated ======
translate O QGDOrthodox {QGD Orthodox}
# ====== TODO To be translated ======
translate O Grunfeld {Grünfeld}
# ====== TODO To be translated ======
translate O GrunfeldExchange {Grünfeld Exchange}
# ====== TODO To be translated ======
translate O GrunfeldRussian {Grünfeld Russian}
# ====== TODO To be translated ======
translate O Catalan {Catalan}
# ====== TODO To be translated ======
translate O CatalanOpen {Catalan Open}
# ====== TODO To be translated ======
translate O CatalanClosed {Catalan Closed}
# ====== TODO To be translated ======
translate O QueensIndian {Queen's Indian}
# ====== TODO To be translated ======
translate O NimzoIndian {Nimzo-Indian}
# ====== TODO To be translated ======
translate O NimzoIndianClassical {Nimzo-Indian Classical}
# ====== TODO To be translated ======
translate O NimzoIndianRubinstein {Nimzo-Indian Rubinstein}
# ====== TODO To be translated ======
translate O KingsIndian {King's Indian}
# ====== TODO To be translated ======
translate O KingsIndianSamisch {King's Indian Sämisch}
# ====== TODO To be translated ======
translate O KingsIndianMainLine {King's Indian Main Line}
# ====== TODO To be translated ======
translate O ConfigureFics {Configure FICS}
# ====== TODO To be translated ======
translate O FICSGuest {Login as Guest}
# ====== TODO To be translated ======
translate O FICSServerPort {Server port}
# ====== TODO To be translated ======
translate O FICSServerAddress {IP Address}
# ====== TODO To be translated ======
translate O FICSRefresh {Refresh}
# ====== TODO To be translated ======
translate O FICSTimesealPort {Timeseal port}
# ====== TODO To be translated ======
translate O FICSSilence {Silence}
# ====== TODO To be translated ======
translate O FICSOffers {Offers}
# ====== TODO To be translated ======
translate O FICSConsole {Console}
# ====== TODO To be translated ======
translate O FICSGames {Games}
# ====== TODO To be translated ======
translate O FICSUnobserve {Stop observing game}
# ====== TODO To be translated ======
translate O FICSProfile {Display your history and profile}
# ====== TODO To be translated ======
translate O FICSRelayedGames {Relayed games}
# ====== TODO To be translated ======
translate O FICSFindOpponent {Find opponent}
# ====== TODO To be translated ======
translate O FICSTakeback {Takeback}
# ====== TODO To be translated ======
translate O FICSTakeback2 {Takeback 2}
# ====== TODO To be translated ======
translate O FICSInitTime {Initial time (min)}
# ====== TODO To be translated ======
translate O FICSIncrement {Increment (sec)}
# ====== TODO To be translated ======
translate O FICSRatedGame {Rated Game}
# ====== TODO To be translated ======
translate O FICSAutoColour {automatic}
# ====== TODO To be translated ======
translate O FICSManualConfirm {confirm manually}
# ====== TODO To be translated ======
translate O FICSFilterFormula {Filter with formula}
# ====== TODO To be translated ======
translate O FICSIssueSeek {Issue seek}
# ====== TODO To be translated ======
translate O FICSChallenge {Challenge}
# ====== TODO To be translated ======
translate O FICSAccept {accept}
# ====== TODO To be translated ======
translate O FICSDecline {decline}
# ====== TODO To be translated ======
translate O FICSColour {Colour}
# ====== TODO To be translated ======
translate O FICSSend {send}
# ====== TODO To be translated ======
translate O FICSConnect {Connect}
# ====== TODO To be translated ======
translate O FICSdefaultuservars {Use default variables}
# ====== TODO To be translated ======
translate O FICSObserveconfirm {Do you want to observe game}
# ====== TODO To be translated ======
translate O FICSpremove {Enable premove}
# ====== TODO To be translated ======
translate O GameReview {Game review}
# ====== TODO To be translated ======
translate O GameReviewTimeExtended {Time extended}
# ====== TODO To be translated ======
translate O GameReviewMargin {Error margin}
# ====== TODO To be translated ======
translate O GameReviewAutoContinue {Auto continue when move is correct}
# ====== TODO To be translated ======
translate O GameReviewReCalculate {Use extended time}
# ====== TODO To be translated ======
translate O GameReviewAnalyzingMovePlayedDuringTheGame {Analyzing move played during the game}
# ====== TODO To be translated ======
translate O GameReviewAnalyzingThePosition {Analyzing the position}
# ====== TODO To be translated ======
translate O GameReviewEnterYourMove {Enter your move}
# ====== TODO To be translated ======
translate O GameReviewCheckingYourMove {Checking your move}
# ====== TODO To be translated ======
translate O GameReviewYourMoveWasAnalyzed {Your move was analyzed}
# ====== TODO To be translated ======
translate O GameReviewYouPlayedSameMove {You played the same move as in match}
# ====== TODO To be translated ======
translate O GameReviewScoreOfYourMove {Score of your move}
# ====== TODO To be translated ======
translate O GameReviewGameMoveScore {Game move's score}
# ====== TODO To be translated ======
translate O GameReviewEngineScore {Engine's score}
# ====== TODO To be translated ======
translate O GameReviewYouPlayedLikeTheEngine {You played like the engine}
# ====== TODO To be translated ======
translate O GameReviewNotEngineMoveButGoodMove {Not the engine move, but is also a good move}
# ====== TODO To be translated ======
translate O GameReviewMoveNotGood {This move is not good, score is}
# ====== TODO To be translated ======
translate O GameReviewMovesPlayedLike {Moves played like}
# ====== TODO To be translated ======
translate O GameReviewMovesPlayedEngine {Moves played like engine}
# ====== TODO To be translated ======
translate O CCDlgConfigureWindowTitle {Configure Correspondence Chess}
# ====== TODO To be translated ======
translate O CCDlgCGeneraloptions {General Options}
# ====== TODO To be translated ======
translate O CCDlgDefaultDB {Default Database:}
# ====== TODO To be translated ======
translate O CCDlgInbox {Inbox (path):}
# ====== TODO To be translated ======
translate O CCDlgOutbox {Outbox (path):}
# ====== TODO To be translated ======
translate O CCDlgXfcc {Xfcc Configuration:}
# ====== TODO To be translated ======
translate O CCDlgExternalProtocol {External Protocol Handler (e.g. Xfcc)}
# ====== TODO To be translated ======
translate O CCDlgFetchTool {Fetch Tool:}
# ====== TODO To be translated ======
translate O CCDlgSendTool {Send Tool:}
# ====== TODO To be translated ======
translate O CCDlgEmailCommunication {eMail Communication}
# ====== TODO To be translated ======
translate O CCDlgMailPrg {Mail program:}
# ====== TODO To be translated ======
translate O CCDlgBCCAddr {(B)CC Address:}
# ====== TODO To be translated ======
translate O CCDlgMailerMode {Mode:}
# ====== TODO To be translated ======
translate O CCDlgThunderbirdEg {e.g. Thunderbird, Mozilla Mail, Icedove...}
# ====== TODO To be translated ======
translate O CCDlgMailUrlEg {e.g. Evolution}
# ====== TODO To be translated ======
translate O CCDlgClawsEg {e.g Sylpheed Claws}
# ====== TODO To be translated ======
translate O CCDlgmailxEg {e.g. mailx, mutt, nail...}
# ====== TODO To be translated ======
translate O CCDlgAttachementPar {Attachment parameter:}
# ====== TODO To be translated ======
translate O CCDlgInternalXfcc {Use internal Xfcc support}
# ====== TODO To be translated ======
translate O CCDlgConfirmXfcc {Confirm moves}
# ====== TODO To be translated ======
translate O CCDlgSubjectPar {Subject parameter:}
# ====== TODO To be translated ======
translate O CCDlgDeleteBoxes {Empty In-/Outbox}
# ====== TODO To be translated ======
translate O CCDlgDeleteBoxesText {Do you really want to empty your In- and Outbox folders for Correspondence Chess? This requires a new sync to show the last state of your games}
# ====== TODO To be translated ======
translate O CCDlgConfirmMove {Confirm move}
# ====== TODO To be translated ======
translate O CCDlgConfirmMoveText {If you confirm, the following move and comment will be sent to the server:}
# ====== TODO To be translated ======
translate O CCDlgDBGameToLong {Inconsistent Mainline}
# ====== TODO To be translated ======
translate O CCDlgDBGameToLongError {The mainline in your database is longer than the game in your Inbox. If the Inbox contains current games, i.e. right after a sync, some moves were added to the mainline in the database erroneously.\nIn this case please shorten the mainline to (at max) move\n}
# ====== TODO To be translated ======
translate O CCDlgStartEmail {Start new eMail game}
# ====== TODO To be translated ======
translate O CCDlgYourName {Your Name:}
# ====== TODO To be translated ======
translate O CCDlgYourMail {Your eMail Address:}
# ====== TODO To be translated ======
translate O CCDlgOpponentName {Opponents Name:}
# ====== TODO To be translated ======
translate O CCDlgOpponentMail {Opponents eMail Address:}
# ====== TODO To be translated ======
translate O CCDlgGameID {Game ID (unique):}
# ====== TODO To be translated ======
translate O CCDlgTitNoOutbox {Scid: Correspondence Chess Outbox}
# ====== TODO To be translated ======
translate O CCDlgTitNoInbox {Scid: Correspondence Chess Inbox}
# ====== TODO To be translated ======
translate O CCDlgTitNoGames {Scid: No Correspondence Chess Games}
# ====== TODO To be translated ======
translate O CCErrInboxDir {Correspondence Chess inbox directory:}
# ====== TODO To be translated ======
translate O CCErrOutboxDir {Correspondence Chess outbox directory:}
# ====== TODO To be translated ======
translate O CCErrDirNotUsable {does not exist or is not accessible!\nPlease check and correct the settings.}
# ====== TODO To be translated ======
translate O CCErrNoGames {does not contain any games!\nPlease fetch them first.}
# ====== TODO To be translated ======
translate O CCDlgTitNoCCDB {Scid: No Correspondence Database}
# ====== TODO To be translated ======
translate O CCErrNoCCDB {No Database of type 'Correspondence' is opened. Please open one before using correspondence chess functions.}
# ====== TODO To be translated ======
translate O CCFetchBtn {Fetch games from the server and process the Inbox}
# ====== TODO To be translated ======
translate O CCPrevBtn {Goto previous game}
# ====== TODO To be translated ======
translate O CCNextBtn {Goto next game}
# ====== TODO To be translated ======
translate O CCSendBtn {Send move}
# ====== TODO To be translated ======
translate O CCEmptyBtn {Empty In- and Outbox}
# ====== TODO To be translated ======
translate O CCHelpBtn {Help on icons and status indicators.\nFor general Help press F1!}
# ====== TODO To be translated ======
translate O CCDlgServerName {Server Name:}
# ====== TODO To be translated ======
translate O CCDlgLoginName  {Login Name:}
# ====== TODO To be translated ======
translate O CCDlgPassword   {Password:}
# ====== TODO To be translated ======
translate O CCDlgShowPassword {Show password}
# ====== TODO To be translated ======
translate O CCDlgURL        {Xfcc-URL:}
# ====== TODO To be translated ======
translate O CCDlgRatingType {Rating Type:}
# ====== TODO To be translated ======
translate O CCDlgDuplicateGame {Non-unique game ID}
# ====== TODO To be translated ======
translate O CCDlgDuplicateGameError {This game exists more than once in your database. Please delete all duplicates and compact your game file (File/Maintenance/Compact Database).}
# ====== TODO To be translated ======
translate O CCDlgSortOption {Sorting:}
# ====== TODO To be translated ======
translate O CCDlgListOnlyOwnMove {Only games I have the move}
# ====== TODO To be translated ======
translate O CCOrderClassicTxt {Site, Event, Round, Result, White, Black}
# ====== TODO To be translated ======
translate O CCOrderMyTimeTxt {My Clock}
# ====== TODO To be translated ======
translate O CCOrderTimePerMoveTxt {Time per move till next time control}
# ====== TODO To be translated ======
translate O CCOrderStartDate {Start date}
# ====== TODO To be translated ======
translate O CCOrderOppTimeTxt {Opponents Clock}
# ====== TODO To be translated ======
translate O CCDlgConfigRelay {Configure ICCF observations}
# ====== TODO To be translated ======
translate O CCDlgConfigRelayHelp {Go to the games page on http://www.iccf-webchess.com and display the game to be observed.  If you see the chessboard copy the URL from your browser to the list below. One URL per line only!\nExample: http://www.iccf-webchess.com/MakeAMove.aspx?id=266452}
# ====== TODO To be translated ======
translate O ExtHWConfigConnection {Configure external hardware}
# ====== TODO To be translated ======
translate O ExtHWPort {Port}
# ====== TODO To be translated ======
translate O ExtHWEngineCmd {Engine command}
# ====== TODO To be translated ======
translate O ExtHWEngineParam {Engine parameter}
# ====== TODO To be translated ======
translate O ExtHWShowButton {Show button}
# ====== TODO To be translated ======
translate O ExtHWHardware {Hardware}
# ====== TODO To be translated ======
translate O ExtHWNovag {Novag Citrine}
# ====== TODO To be translated ======
translate O ExtHWInputEngine {Input Engine}
# ====== TODO To be translated ======
translate O ExtHWNoBoard {No board}
# ====== TODO To be translated ======
translate O NovagReferee {Referee}
# ====== TODO To be translated ======
translate O IEConsole {Input Engine Console}
# ====== TODO To be translated ======
translate O IESending {Moves sent for}
# ====== TODO To be translated ======
translate O IESynchronise {Synchronise}
# ====== TODO To be translated ======
translate O IERotate  {Rotate}
# ====== TODO To be translated ======
translate O IEUnableToStart {Unable to start Input Engine:}
# ====== TODO To be translated ======
translate O DoneWithPosition {Done with position}
# ====== TODO To be translated ======
translate O Board {Board}
# ====== TODO To be translated ======
translate O showGameInfo {Show game info}
# ====== TODO To be translated ======
translate O autoResizeBoard {Automatic resize of board}
# ====== TODO To be translated ======
translate O DockTop {Move to top}
# ====== TODO To be translated ======
translate O DockBottom {Move to bottom}
# ====== TODO To be translated ======
translate O DockLeft {Move to left}
# ====== TODO To be translated ======
translate O DockRight {Move to right}
# ====== TODO To be translated ======
translate O Undock {Undock}
# ====== TODO To be translated ======
translate O ChangeIcon {Change icon...}
# ====== TODO To be translated ======
translate O NewGameListWindow {New Game List Window}
# ====== TODO To be translated ======
translate O LoadatStartup {Load at startup}
# ====== TODO To be translated ======
translate O ShowHideDB {Show/Hide databases}
# ====== TODO To be translated ======
translate O ChangeFilter {Change filter}
# ====== TODO To be translated ======
translate O ChangeLayout {Load/Save/Change sorting criteria and column layout}
# ====== TODO To be translated ======
translate O ShowHideStatistic {Show/Hide statistics}
# ====== TODO To be translated ======
translate O BoardFilter {Show only games that matches the current board position}
# ====== TODO To be translated ======
translate O CopyGameTo {Copy Game to}
# ====== TODO To be translated ======
translate O FindBar {Find Bar}
# ====== TODO To be translated ======
translate O FindCurrentGame {Find current game}
# ====== TODO To be translated ======
translate O DeleteGame {Delete game}
# ====== TODO To be translated ======
translate O UndeleteGame {Undelete game}
# ====== TODO To be translated ======
translate O ResetSort {Reset sort}
# ====== TODO To be translated ======
translate O ConvertNullMove {Convert null moves to comments}
# ====== TODO To be translated ======
translate O SetupBoard {Setup Board}
# ====== TODO To be translated ======
translate O Rotate {Rotate}
# ====== TODO To be translated ======
translate O SwitchColors {Switch colors}
# ====== TODO To be translated ======
translate O FlipBoard {Flip Board}
# ====== TODO To be translated ======
translate O ImportPGN {Import PGN game}
# ====== TODO To be translated ======
translate O ImportingFiles {Importing PGN files in}
# ====== TODO To be translated ======
translate O ImportingFrom {Importing from}
# ====== TODO To be translated ======
translate O ImportingIn {Import games in}
# ====== TODO To be translated ======
translate O UseLastTag {Use last\ngame's tags}
# ====== TODO To be translated ======
translate O Random {Random}
# ====== TODO To be translated ======
translate O BackToMainline {Go back to mainline}
# ====== TODO To be translated ======
translate O LeaveVariant {Leave variant}
# ====== TODO To be translated ======
translate O Autoplay {Autoplay}
# ====== TODO To be translated ======
translate O ShowHideCoords {Show/Hide Coord.}
# ====== TODO To be translated ======
translate O ShowHideMaterial {Show/Hide Material}
# ====== TODO To be translated ======
translate O FullScreen {Full Screen}
# ====== TODO To be translated ======
translate O FilterStatistic {Filter Statistics}
# ====== TODO To be translated ======
translate O MakeCorrections {Make Corrections}
# ====== TODO To be translated ======
translate O Surnames {Surnames}
# ====== TODO To be translated ======
translate O Ambiguous {Ambiguous}
translate O OptionsToolbar "Verktøylinje i hovedvindu"
translate O OptionsBoard "Brett"
# ====== TODO To be translated ======
translate O OptionsMenuColor "Menu colors"
translate O OptionsBoardSize "Brettstørrelse"
translate O OptionsBoardPieces "Brikkeutseende"
# ====== TODO To be translated ======
translate O OptionsInternationalization "Internationalization"
# ====== TODO To be translated ======
translate O MenuColorForeground "Textcolor"
# ====== TODO To be translated ======
translate O MenuColorBackground "Background"
# ====== TODO To be translated ======
translate O MenuColorSelect "Select"
# ====== TODO To be translated ======
translate O OptionsTablebaseDir "Select up to 4 table base folders:"
}

# norsk.tcl

# Sjekkliste:
# - Snarveier
