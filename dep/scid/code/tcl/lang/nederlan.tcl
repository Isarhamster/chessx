#  nederlan.tcl:
#  Dutch language support for Scid.
#  Added by J.Kees Hofkamp.
#  Changes by J. Krabbenbos.
#  Changes by Leander Laruelle.
# Untranslated messages are marked with a "***" comment.

proc setLanguage_N {} {

# File menu:
menuText N File "Bestand" 0
menuText N FileNew "Nieuw..." 0 {Maak een nieuwe Scid database}
menuText N FileOpen "Openen..." 0 {Open een bestaande Scid database}
menuText N FileClose "Sluiten" 0 {Sluit de  actieve Scid database}
menuText N FileFinder "Bestandzoeker" 0 {Open het bestandszoekvenster}
menuText N FileBookmarks "Bladwijzers" 0 {Bladwijzer menu (sneltoets: Ctrl+B)}
menuText N FileBookmarksAdd "Toevoegen bladwijzer" 0 \
  {Bladwijzer naar huidige databasepartij en stelling}
menuText N FileBookmarksFile "Bladwijzer-bestand" 0 \
  {Maak een bladwijzerbestand voor de huidige partij en stelling}
menuText N FileBookmarksEdit "Wijzigen Bladwijzers..." 0 \
  {Bladwijzermenu's}
menuText N FileBookmarksList "Weergeven bladwijzerfolders als lijst" 0 \
  {Weergeven bladwijzerfolders als lijst, niet als submenu's}
menuText N FileBookmarksSub "Weergeven bladwijzerfolders als submenu's" 0 \
  {Weergeven bladwijzerfolders als submenu's, niet als lijst}
menuText N FileMaint "Onderhoud" 2 {Onderhoud Scid database}
menuText N FileMaintWin "Onderhoudsvenster" 2 \
  {Open/sluit het Scid onderhoudsvenster}
menuText N FileMaintCompact "Reorganiseer database..." 0 \
  {Reorganiseer database bestanden}
menuText N FileMaintClass "Partijen ECO-classificeren..." 10 \
  {Herbereken de ECO code van alle partijen}
menuText N FileMaintSort "Sorteren..." 0 \
  {Sorteer alle partijen in de database}
menuText N FileMaintDelete "Dubbels verwijderen..." 0 \
  {Vind dubbele partijen om ze te verwijderen}
menuText N FileMaintTwin "Dubbels-venster" 1 \
  {Open/bijwerken het dubbels-controle venster}
menuText N FileMaintName "Namen" 0 \
  {Corrigeer namen/spelling}
menuText N FileMaintNameEditor "Namen-bewerker" 6 \
  {Open/sluit het namen-correctie venster}
menuText N FileMaintNamePlayer "Corrigeer naam speler..." 15 \
  {Controleer spelling namen via het spellingcontrole bestand}
menuText N FileMaintNameEvent "Corrigeer naam evenement..." 15 \
  {Controleer spelling evenementen via spellingchecker bestand}
menuText N FileMaintNameSite "Corrigeer naam plaats..." 15 \
  {Controleer plaatsnamen via spellingchecker bestand}
menuText N FileMaintNameRound "Corrigeer ronde..." 10 \
  {Controleer rondenamen via spelling spellingchecker bestand}
menuText N FileReadOnly "Alleen lezen..." 7 \
  {Zet huidige database op alleen-lezen en voorkom veranderingen}
menuText N FileSwitch "Schakel naar andere  database" 0 \
  {Schakel naar een andere geopende database} 
menuText N FileExit "Einde programma" 0 {Einde Scid}
menuText N FileMaintFixBase "Repareer databank" 0 \
    {Probeer een corrupte databank te repareren}

# Edit menu:
menuText N Edit "Bewerken" 0
menuText N EditAdd "Nieuwe variant" 8 \
 {Voeg op dit punt een variant toe}
menuText N EditDelete "Variant verwijderen" 11 \
 {Verwijder een variant voor deze zet}
menuText N EditFirst "Maak hoofdvariant" 5 \
  {Maak deze variant de eerste in de lijst}
menuText N EditMain "Variatie op hoofdvariant" 13 \
   {Promoveerd de variant als hoofdvariant}
menuText N EditTrial "Probeer variatie" 0 \
  {Start/stop probeer modus, om een idee op het bord te testen}
menuText N EditStrip "Verwijder analyse" 2 {Verwijder commentaar of varianten uit deze partij}
menuText N EditUndo "Ongedaan" 0 {Maak laatset verandering ongedaan}
# ====== TODO To be translated ======
menuText N EditRedo "Redo" 0 {Redo last game change}
menuText N EditStripComments "Commentaar" 0 \
  {Verwijder alle commentaar en annotaties uit deze partij}
menuText N EditStripVars "Varianten" 0 {Verwijder alle varianten uit deze partij}
menuText N EditStripBegin " Zetten vanaf begin " 1 \
  {Verwijder alle zetten vanaf begin van de partij} ;
menuText N EditStripEnd " Zetten tot het einde  " 0 \
  {Verwijder alle verdere zetten tot het einde van de partij} ;
menuText N EditReset "Klembord leegmaken" 0 \
  {Maak het klembord helemaal leeg}
menuText N EditCopy "Partij naar klembord" 7 \
  {Kopieer deze partij naar het klembord}
menuText N EditPaste "Partij vanuit klembord" 7 \
  {Plak actieve klembord-partij hier}
menuText N EditPastePGN "Plak de Klipbord tekst als  PGN partij..." 10 \
  {Interpreteer de Klipbord tekst als een partij  in PGN notatie en plak die hier} ;
menuText N EditSetup "Stelling opzetten..." 0 \
  {Kies een start-stelling voor de partij}
menuText N EditCopyBoard "Copieer stelling " 6 \
  {Copieer de huidige stelling in FEN notatie naar de tekst het tekst clibbord } ;
menuText N EditPasteBoard "Invoegen start stelling" 12 \
  {Maak de start stelling van de huidige tekst selectie (klembord)}
# ====== TODO To be translated ======
menuText N ConfigureScid "Preferences..." 0 {Configure all options for SCID}

# Game menu:
menuText N Game "Partij" 0
menuText N GameNew "Partij leegmaken" 7 \
  {Maak partij leeg; sla veranderingen niet op}
menuText N GameFirst "Eerste partij laden" 5 {Laad de eerste gefilterde partij}
menuText N GamePrev "Vorige partij laden" 0 \
  {Laad vorige partij in het filter}
menuText N GameReload "Partij opnieuw laden" 7 \
  {Laad partij opnieuw; sla veranderingen niet op}
menuText N GameNext "Volgende partij laden" 3 \
  {Laad volgende partij in het filter}
menuText N GameLast "Laatste partij laden" 6 {Laad de laatste gefilterde partijd}
menuText N GameRandom "Laad willekeurige partij" 8 {Laad een willekeurige partij}
menuText N GameNumber "Laad partij nummer..." 12 \
  {Laad partijnummer:}
menuText N GameReplace "Partij overschrijven..." 11 \
  {Bewaar partij; overschrijf oude versie}
menuText N GameAdd "Partij toevoegen..." 7 \
  {Bewaar partij en voeg toe aan de database}
menuText N GameDeepest "Opening bepalen" 8 \
  {Ga naar de diepste stelling uit het ECO openingboek}
menuText N GameGotoMove "Zetnummer..." 0 \
  {Ga naar zetnummer .. in de partij}
menuText N GameNovelty "Vind nieuwtje..." 7 \
  {Vind de eerste zet in deze partij die nog niet eerder is gespeeld}

# Search menu:
menuText N Search "Selecteren" 0
menuText N SearchReset "Alle partijen" 0 \
  {Reset filter en toon alle partijen}
menuText N SearchNegate "Selectie omdraaien" 9 \
  {Draai filter om en toon de andere partijen uit de databse}
menuText N SearchCurrent "Zoek huidige stelling" 5 \
  {Zoek in database naar huidige stelling}
menuText N SearchHeader "Partijgegevens..." 5 \
  {Zoek op speciale informatie: speler, evenement enz.}
menuText N SearchMaterial "Materiaal/Kenmerken..." 0 \
  {Zoek op patroon: bord, materiaal enz.}
menuText N SearchUsing "Zoekopties" 0 \
  {Zoek met gebruikmaking van opgeslagen opties}

# Windows menu:
menuText N Windows "Venster" 0
menuText N WindowsComment "Bewerk commentaar" 0 \
 {Open/sluit commentaar bewerkings venster}
menuText N WindowsGList "Toon alle partijen" 0 \
  {Open/sluit lijst met partijen}
menuText N WindowsPGN "PGN-venster" 0 \
 {Open/sluit het PGN-notatie venster}
menuText N WindowsPList "Spelervinder" 2 {Open/sluit de spelervinder} ;
menuText N WindowsTmt "Toernooi Zoeker" 2 {Open/sluit het toernooi zoekvenster}
menuText N WindowsSwitcher  "Database wisselen" 0 \
  {Open/sluit het database-wisselen venster}
menuText N WindowsMaint "Onderhoudsvenster" 5 \
  {Open/sluit het onderhoudsvenster}
menuText N WindowsECO "ECO Browser" 0 {Open/sluit het ECO browser venster}
menuText N WindowsStats "Statistiek" 0 \
  {Open/sluit het filter statieken-venster}
menuText N WindowsTree "Openingoverzicht" 0 {Open/sluit het Openingsoverzichtvenster}
menuText N WindowsTB "Tablebase venster" 1 \
  {Open/sluit het Tablebase venster}
menuText N WindowsBook "Boek Venster" 0 {Open/sluit het Boek venster}
menuText N WindowsCorrChess "Correspondentieschaak venster" 0 \
    {Open/sluit het correspondentieschaak venster}

# Tools menu:
menuText N Tools "Gereedschappen" 0
menuText N ToolsAnalysis "Schaakengine ..." 3 \
  {Start/stop schaak-analyse programma 1}
menuText N ToolsAnalysis2 "Schaakengine #2..." 17 \
  {Start/stop  schaak-analyse programma 2}
menuText N ToolsCross "Kruistabel" 0 \
  {Toon toernooi-kruistabel voor huidige partij}
menuText N ToolsEmail "EmailBeheerder" 0 {Open/sluit het email venster}
menuText N ToolsFilterGraph "Filter grafiek" 7 \
  {Openen/sluiten grafisch filter venster}
menuText N ToolsAbsFilterGraph "Abs. Filter Grafiek" 7 {Open/sluit het filter grafiek venster voor absolute waarden}
menuText N ToolsOpReport "Openingen rapportering" 0 \
  {Genereer een openingenrapport voor de huidige stelling}
menuText N ToolsOpenBaseAsTree "Open base als boom" 0   {Open een databank en gebruik ze in een boomvenster}
menuText N ToolsOpenRecentBaseAsTree "Open een recente databank als boom" 0   {Open een recente databank en gebruik ze in een boom venster}
menuText N ToolsTracker "Stuk Spoorvolger "  0 {Open het Stuk Spoorvolger venster}
menuText N ToolsTraining "Training"  0 {Training gereedschappen (taktiek, openingen,...) }
menuText N ToolsTacticalGame "Taktische partij"  0 {Speel een taktische partij}
menuText N ToolsSeriousGame "Ernstige partij"  0 {Speel een ernstige partij}
menuText N ToolsTrainOpenings "Openingen "  0 {Repertoriumtraining}
menuText N ToolsTrainReviewGame "Herspeel Partij"  0 {Zoek de zetten gespeeld in een partij}
menuText N ToolsTrainTactics "Tactiek "  0 {Los tactische stellingen op }
menuText N ToolsTrainCalvar "Berekenen van varianten"  0 {Training in het berekenen van varianten}
menuText N ToolsTrainFindBestMove "Vind de beste zet"  0 {Vind de beste zet}
menuText N ToolsTrainFics "Speel op het internet"  0 {Speel of freechess.org}
# ====== TODO To be translated ======
menuText N ToolsEngineTournament "Engine tournament"  0 {Start a tournament between chess engines}
menuText N ToolsBookTuning "Openingenboek afstemming " 0 {Openingenboek afstemming}
menuText N ToolsConnectHardware "Verbind Hardware" 0 {Verbind externe hardware}
menuText N ToolsConnectHardwareConfigure "Configureer..." 0 {Configureer externe hardware en verbinding}
menuText N ToolsConnectHardwareNovagCitrineConnect "Verbind Novag Citrine" 0 {Verbind Novag Citrine}
menuText N ToolsConnectHardwareInputEngineConnect "Verbind Invoer Engine" 0 {Verbind Invoer Engine (e.g. DGT)}
menuText N ToolsPInfo "Speler Informatie"  7 \
  {Open/wijzig het spelerinformatievenster}
menuText N ToolsPlayerReport "Speler Rapport ..." 3 \
  {Genereer een Speler Rapport} ;
menuText N ToolsRating "Elo Klassering geschiedenis" 0 \
  {Grafiek van de Elo Klassering van de twee spelers}
menuText N ToolsScore "Partij score" 7 \
  {Laat het partij-score venster zien}
menuText N ToolsExpCurrent "Partij exporteren" 8 \
  {Exporteer huidige partij naar een bestand}
menuText N ToolsExpCurrentPGN "Partij in PGN-formaat exporteren..." 11 \
  {Schrijf huidige partij naar PGN-bestand}
menuText N ToolsExpCurrentHTML "Partij in HTML-formaat exporteren..." 11 \
  {Schrijf huidige partij naar HTML-bestand}
menuText N ToolsExpCurrentHTMLJS "Exporteer Partij naar een HTML en JavaScript Bestand..." 15 {Schrijf de huidige partij naar een HTML en JavaScript bestand}  
menuText N ToolsExpCurrentLaTeX "Partij in LaTeX-formaat exporteren..." 11 \
  {Schrijf huidige partij naar LaTex-bestand}
menuText N ToolsExpFilter "Alle partijen in filter exporteren" 17 \
  {Exporteer alle geslecteerde partijen naar een bestand}
menuText N ToolsExpFilterPGN "Filter in PGN-formaat exporteren..." 10 \
  {Schrijf selectie naar PGN-bestand}
menuText N ToolsExpFilterHTML "Filter in HTML-formaat exporteren..." 10 \
  {Schrijf selectie naar HTML-bestand}
menuText N ToolsExpFilterHTMLJS "Exporteer Filter naar HTML en JavaScript Bestand..." 17 {Schrijf alle gefilterde partijen naar een HTML en JavaScript bestand}  
menuText N ToolsExpFilterLaTeX "Filter in LaTeX-formaat exporteren..." 10 \
  {Schrijf selectie naar LaTex-bestand}
menuText N ToolsImportOne "Een PGN partij importeren..." 4 \
  {Importeer PGN partij}
menuText N ToolsImportFile "PGN database importeren..." 4 \
  {Importeer PGN bestand}
menuText N ToolsStartEngine1 "Start engine 1" 0  {Start engine 1}
menuText N ToolsStartEngine2 "Start engine 2" 0  {Start engine 2}
# ====== TODO To be translated ======
menuText N ToolsCaptureBoard "Capture Current Board..." 5  {Save the current board as an image.}
menuText N Play "Speel" 0
menuText N CorrespondenceChess "Correspondentieschaak" 0 {Functies voor eMail en Xfcc gebaseerde correspondentieschaak}
menuText N CCConfigure "Configureer..." 0 {Configureer externe gereedschappen en algemene instellingen}
# ====== TODO To be translated ======
menuText N CCConfigRelay "Observe games..." 10 {Configure games to be observed}
menuText N CCOpenDB "Open Database..." 0 {Open de standaard correspondentieschaak database}
menuText N CCRetrieve "Haal Partijen op" 0 {Haal partijen op via externe (Xfcc-)helper}
menuText N CCInbox "Verwerk Inbox" 0 {Verwerk alle bestanden in scids Inbox}
menuText N CCSend "Stuur de Zet op" 0 {Stuur de zet op via eMail of de externe (Xfcc-)helper}
menuText N CCResign "Geef op" 0 {Geef op (niet via eMail)}
menuText N CCClaimDraw "Eis Remise" 0 {Stuur de zet op en eis remise (niet via eMail)}
menuText N CCOfferDraw "Stel Remise voor" 0 {Stuur de zet op en stel remise voor (niet via eMail)}
menuText N CCAcceptDraw "Accepteer Remise" 0 {Accepteer een remise voorstel (niet via eMail)}
menuText N CCNewMailGame "Nieuwe eMail Partij..." 0 {Start een nieuwe eMail partij}
menuText N CCMailMove "Stuur de zet op..." 0 {Stuur de zet op naar de tegenspeler via eMail}
menuText N CCGamePage "Partij Pagina..." 0 {Roep de partij op met een web browser}
menuText N CCEditCopy "Kopieer de Partijlijst naar het Clipbord" 0 {Kopieer de Partijen als CSV lijst naar het clipbord}

# Options menu:
menuText N Options "Opties" 0
menuText N OptionsBoardGraphics "Vakken..." 0 {Selecteer textures voor de vakken}
translate N OptionsBGW {Selecteer het texture voor de vakken}
translate N OptionsBoardGraphicsText {Selecteer graphische bestanden voor de witte en de zwarte vakken:}
menuText N OptionsBoardNames "Mijn Spelersnamen ..." 0 {Bewerk mijn spelersnamen} ;
menuText N OptionsExport "Export" 1 {Wijzig tekst export opties}
menuText N OptionsFonts "Lettertypes" 0 {Wijzig lettertype}
menuText N OptionsFontsRegular "Standaard" 0 {Wijzig het standaard lettertype}
menuText N OptionsFontsMenu "Menu" 0 {Wijzig het menu lettertype}
menuText N OptionsFontsSmall "Klein" 0 {Wijzig het kleine lettertype}
# ====== TODO To be translated ======
menuText N OptionsFontsTiny "Tiny" 0 {Change the tiny font}
menuText N OptionsFontsFixed "Vaste grootte" 0 {Wijzig dit lettertype}
menuText N OptionsGInfo "Partij Informatie" 0 {Parij-informatie opties}
menuText N OptionsLanguage "Kies taal" 5 {Kies taal}
menuText N OptionsMovesTranslatePieces "Vertaal stukken" 0 {Vertaal de eerste letters van de stukken}
menuText N OptionsMovesHighlightLastMove "Markeer de laatste zet" 0 {Markeer de laatste zet}
menuText N OptionsMovesHighlightLastMoveDisplay "Toon" 0 {Toon de laatste zet markering}
menuText N OptionsMovesHighlightLastMoveWidth "Breedte" 0 {Variantdikte}
menuText N OptionsMovesHighlightLastMoveColor "Kleur" 0 {Varinatkleur}
# ====== TODO To be translated ======
menuText N OptionsMovesHighlightLastMoveArrow "include Arrow" 0 {Include Arrow with Highlight}
menuText N OptionsMoves "Zetten" 0 {Wijzig optie voor zet-invoer}
menuText N OptionsMovesAnimate "Stuk Animatietijd " 1 \
  {Zet animatietijd van de stukkenbewegingen} ;
menuText N OptionsMovesDelay "Tijdinstelling voor auto-spelen" 10 \
  {Stel de tijd in voor het automatisch spelen van de zetten}
menuText N OptionsMovesCoord "Zet-ingave" 0 \
  {Accepteer de volgende manier van zetten invoeren ("g1f3")}
menuText N OptionsMovesSuggest "Toon hint" 0 \
  {Schakel hints aan of uit}
menuText N OptionsShowVarPopup "Toon het varianten venster" 0 {Toon al dan niet het varianten venster} 
menuText N OptionsMovesSpace "Voeg spaties in na het nummer van de zet" 0 {Voeg spaties in na het nummer van de zet}  
menuText E OptionsMovesSpace "Add spaces after move number" 0 {Add spaces after move number}   
menuText N OptionsMovesKey "Auto-aanvullen" 0 \
  {Aan/uitschakelen van toetsenbordzet auto-aanvullen}
menuText N OptionsMovesShowVarArrows "Toon Pijlen voor Varianten" 0 {Zet aan/af pijlen om variantenzetten te tonen}
# ====== TODO To be translated ======
menuText N OptionsMovesGlossOfDanger "Color Coded Gloss of Danger" 0 {Turn on/off color coded gloss of danger}
menuText N OptionsNumbers "Getalformaat" 5 \
  {Kies de manier waarop getallen te zien zijn}
menuText N OptionsTheme "Theme" 0 {Verander het uitzicht van de interface}
menuText N OptionsWindows "Vensters" 0 {Venster opties}
menuText N OptionsSounds "Geluiden" 2 {Configureer zet aankondigingsgeluiden} ;
# ====== TODO To be translated ======
menuText N OptionsResources "Resources..." 0 {Choose resource files and folders}
menuText N OptionsWindowsDock "Veranker de vensters" 0 {Veranker de vensters}
menuText N OptionsWindowsSaveLayout "Opmaak opslaan" 0 {Opmaak opslaan}
menuText N OptionsWindowsRestoreLayout "Opmaak herstellen" 0 {Opmaak herstellen}
menuText N OptionsWindowsShowGameInfo "Toon de informatie over de partij" 0 {Toon de informatie over de partij}
menuText N OptionsWindowsAutoLoadLayout "Auto laad eerste layout" 0 {Auto laad eerste layout bij opstarten}
menuText N OptionsECO "ECO data laden" 0 \
  {Laad het ECO classificatie bestand}
menuText N OptionsSpell "Laad spelling (namen)" 5 \
  {Laad het Scid spellingbestand}
menuText N OptionsTable "Eindspel database" 9 \
  {Kies een eindspel database, alle in de directory aanwezige worden gebruikt}
menuText N OptionsRecent "Recente bestanden..." 0 \
  {Wijzig het aantal recent gebruikte bestanden in het Bestand menu}
menuText N OptionsBooksDir "Boeken map" 0 {Stel de map met openingsboeken in}
menuText N OptionsTacticsBasesDir "Databases map" 0 {Stel de map met de  taktiek (training) databases in}
# ====== TODO To be translated ======
menuText N OptionsPhotosDir "Photos directory" 0 {Sets the photos bases directory}
# ====== TODO To be translated ======
menuText N OptionsThemeDir "Theme(s) File:"  0 { Load a GUI theme package file }
menuText N OptionsSave "Opties bewaren" 0 \
  "Bewaar alle instellingen in het bestand $::optionsFile"
menuText N OptionsAutoSave "Automatisch bewaren opties tijdens afsluiten" 0 \
  {Automatisch bewaren alle gewijzigde opties bij het afsluiten van Scid}

# Help menu:
menuText N Help "Help" 0
menuText N HelpContents "Inhoud" 0 {Toon de Help Inhoudstabel} ;
menuText N HelpIndex "Index" 0 {Toon de help inhouds pagina}
menuText N HelpGuide "Snelle hulp" 0 {Laat de snelle-hulp pagina zien}
menuText N HelpHints "Hints" 0 {Laat de hints-hulp pagina zien}
menuText N HelpContact "Contact-info" 0 {Laat de contact-infopagina zien}
menuText N HelpTip "Tip van de dag" 0 {Laat een handige Scid tip zien}
menuText N HelpStartup "Startvenster" 5 {Laat het startvenster zien}
menuText N HelpAbout "Over Scid" 0 {Informatie over Scid}

# Game info box popup menu:
menuText N GInfoHideNext "Verberg volgende zet" 0
menuText N GInfoMaterial "Materiaalverhouding" 0
menuText N GInfoFEN "FEN" 0
menuText N GInfoMarks "Toon gekleurde velden en pijlen. " 5
menuText N GInfoWrap "Lange regels op schermbreedte splitsen." 0
menuText N GInfoFullComment "Volledig commentaar weergeven" 10
menuText N GInfoPhotos "Toon Fotos" 5 ;
menuText N GInfoTBNothing "Eindspel Tablebases: niets" 12
menuText N GInfoTBResult  "Eindspel Tablebases: alleen resultaat" 12
menuText N GInfoTBAll "Eindspel Tablebases: resultaat en beste zetten" 19
menuText N GInfoDelete "Partij wissen/terughalen" 9
menuText N GInfoMark "Partij markeren/niet markeren" 7
menuText N GInfoInformant "Configureer informant waarden" 0

# General buttons:
translate N Back {Terug}
translate N Browse {Bladeren} ;
translate N Cancel {Annuleren}
translate N Continue {Verder gaane}
translate N Clear {Leegmaken}
translate N Close {Sluiten}
translate N Contents {Inhoud} ;
translate N Defaults {Standaard}
translate N Delete {Wis}
translate N Graph {Grafiek}
translate N Help {Help}
translate N Import {Importeren}
translate N Index {Index}
translate N LoadGame {Partij laden}
translate N BrowseGame {Door de partij bladeren}
translate N MergeGame {Partij Samenvoegen}
translate N MergeGames {Voeg Partijen Samen}
translate N Preview {Proefbeeld}
translate N Revert {Terugkeren}
translate N Save {Bewaren}
translate N Search {Zoeken}
translate N Stop {Stop}
translate N Store {Opbergen}
translate N Update {Bijwerken}
translate N ChangeOrient {Wijzigen venster orientatie}
translate N ShowIcons {Toon Ikonen} ;
translate N None {Geen}
translate N First {Eerste}
translate N Current {Huidige}
translate N Last {Laatste}

# General messages:
translate N game {Partij}
translate N games {Partijen}
translate N move {Zet}
translate N moves {Zetten}
translate N all {Alle}
translate N Yes {Ja}
translate N No {Nee}
translate N Both {Beide}
translate N King {Koning}
translate N Queen {Dame}
translate N Rook {Toren}
translate N Bishop {Loper}
translate N Knight {Paard}
translate N Pawn {Pion}
translate N White {Wit}
translate N Black {Zwart}
translate N Player {Speler}
translate N Rating {Eloklassering}
translate N RatingDiff {EloklasseringsVerschil (Wit - Zwart)}
translate N AverageRating {Gemiddelde elo rangschikking} ;
translate N Event {Evenement}
translate N Site {Plaats}
translate N Country {Land}
translate N IgnoreColors {Kleuren negeren}
translate N Date {Datum}
translate N EventDate {Datum evenement}
translate N Decade {Decennium}
translate N Year {Jaar}
translate N Month {Maand}
translate N Months {Januari Februari Maart April Mei Juni Juli Augustus September October November December}
translate N Days {Zon Maa Din Woe Don Vri Zat}
translate N YearToToday {Een jaar geleden}
# ====== TODO To be translated ======
translate N YearToTodayTooltip {Set date from 1 year back to today}
translate N Result {Uitslag}
translate N Round {Ronde}
translate N Length {Lengte}
translate N ECOCode {ECO Code}
translate N ECO {ECO}
translate N Deleted {Verwijderd}
translate N SearchResults {Zoekresultaten}
translate N OpeningTheDatabase {Database aan het Openen}
translate N Database {Database}
translate N Filter {Filter}
translate N noGames {Geen partijen}
translate N allGames {Alle partijen}
translate N empty {leeg}
translate N clipbase {Klembord}
translate N score {Score}
translate N StartPos {Begin stelling}
translate N Total {Totaal}
translate N readonly {alleen-lezen}

# Standard error messages:
translate N ErrNotOpen {Deze database is niet geopend.} ;
translate N ErrReadOnly {Deze database is alleen lezen; kan niet veranderd worded.} ;
translate N ErrSearchInterrupted {Zoeken werd onderbroken; de resultaten zijn onvolledig.} ;

# Game information:
translate N twin {Dubbele partijen}
translate N deleted {Gewist}
translate N comment {Commentaar}
translate N hidden {Verborgen}
translate N LastMove {Laatste zet}
translate N NextMove {Volgende zet}
translate N GameStart {Start partij}
translate N LineStart {Start variant}
translate N GameEnd {Einde partij}
translate N LineEnd {Einde variant}

# Player information:
translate N PInfoAll {Resultaten voor <b>alle</b> partijen}
translate N PInfoFilter {Resultaten voor <b>filter</b> partijen}
translate N PInfoAgainst {Resultaten tegen}
translate N PInfoMostWhite {Meest gespeelde opening als Wit}
translate N PInfoMostBlack {Meest gespeelde opening als Zwart}
translate N PInfoRating {Geschiedenis Elo Klassering}
translate N PInfoBio {Biografie}
translate N PInfoEditRatings {Bewerk elo rangschikking} ;
# ====== TODO To be translated ======
translate N PInfoEloFile {File}

# Tablebase information:
translate N Draw {Remise}
translate N stalemate {Pat}
translate N withAllMoves {met alle zetten}
translate N withAllButOneMove {alle zetten behalve één}
translate N with {met}
translate N only {alleen}
translate N lose {verliezen}
translate N loses {verliest}
translate N allOthersLose {alle overigen verliezen}
translate N matesIn {Mat in}
translate N hasCheckmated {geeft mat}
translate N longest {langste}
translate N WinningMoves {Winnende zetten} ;
translate N DrawingMoves {Remise zetten} ;
translate N LosingMoves {Verliezende zetten} ;
translate N UnknownMoves {Onbekend resultaat zetten} ;
# ====== TODO To be translated ======
translate N SideLongest {Side    Longest    %     %     %\n}
# ====== TODO To be translated ======
translate N toNoveMate {to move   mate    Win  Draw  Loss\n}
# ====== TODO To be translated ======
translate N NoSummary {\nNo summary for this tablebase.}
# ====== TODO To be translated ======
translate N MutualZugzwang {Mutual zugzwangs: }
# ====== TODO To be translated ======
translate N SelectetZugzwang {\nSelected zugzwang positions:}
# ====== TODO To be translated ======
translate N WdrawBloss {White draws, Black loses:}
# ====== TODO To be translated ======
translate N BdrawWloss {Black draws, White loses:}
# ====== TODO To be translated ======
translate N AllLoss {" Whoever moves loses:     " }

# Tip of the day:
translate N Tip {Tip}
translate N TipAtStartup {Tip bij opstarten}

# Tree window menus:
menuText N TreeFile "BoomDataBestand" 0
menuText N TreeFileFillWithBase "Vul de Cache met DataBestand" 0 {Vul het cache bestand met alle partijen in het huidige databestand}
menuText N TreeFileFillWithGame "Vul de cache met de partij" 0 {Vul het cache bestand met het huidige speel in het huidige databestand}
menuText N TreeFileSetCacheSize "Cache grootte" 0 {Stel de grootte van het cache bestand in}
menuText N TreeFileCacheInfo "Cache info" 0 {Vraag informatie over het gebruik van de cache}
menuText N TreeFileSave " BoomData Bewaren" 0 {Bewaar de boomdata in een boomcache (.stc) bestand}
menuText N TreeFileFill "Vullen boomcache bestand" 0 \
  {Vul het boomcache bestand met algemene opening stellingen}
menuText N TreeFileBest "Lijst beste partijen" 0 {Weergeven van de lijst met beste partijen}
menuText N TreeFileGraph "Grafiek-venster" 0 \
  {Weergeven van de grafiek voor deze tak}
menuText N TreeFileCopy "Kopieer boom tekst naar klembord" 0 \
  {Kopieren van de boomstatistieken naar het klembord}
menuText N TreeFileClose "Zoekboom venster sluiten" 0 {Sluiten van het zoekboom venster}
menuText N TreeMask "Masker" 0
menuText N TreeMaskNew "Nieuw" 0 {Nieuw masker}
menuText N TreeMaskOpen "Open" 0 {Open mask}
menuText N TreeMaskOpenRecent "Open recent" 0 {Open recent masker}
menuText N TreeMaskSave "Bewaar" 0 {Bewaar het masker}
menuText N TreeMaskClose "Sluiten" 0 {Sluit het masker}
menuText N TreeMaskFillWithGame "Vullen met partij" 0 {Vul het masker met de partij}
menuText N TreeMaskFillWithBase "Vullen met databestand" 0 {Vul het masker met alle partijen in het databestand}
menuText N TreeMaskInfo "Info" 0 {Toon de statistieken voor het huidige masker}
menuText N TreeMaskDisplay "Toon masker overzicht" 0 {Toon maskerdata in boomvorm}
menuText N TreeMaskSearch "Zoek" 0 {Zoek in huidig masker}
menuText N TreeSort "Sorteren" 0
menuText N TreeSortAlpha "Alfabetisch" 0
menuText N TreeSortECO "ECO code" 0
menuText N TreeSortFreq "Frequentie" 0
menuText N TreeSortScore "Punten" 0
menuText N TreeOpt "Opties" 0
menuText N TreeOptSlowmode "Trage modus" 0 {Trage modus voor updates (hoge nauwkeurigheid)}
menuText N TreeOptFastmode "Snelle modus" 0 {Snelle modus voor updates (geen omwisselingen van zetten)}
menuText N TreeOptFastAndSlowmode "Snelle en trage modus" 0 {Eerst snelle modus en daarna trage modus voor updates}
menuText N TreeOptStartStop "Auto verversing" 0 {Zet automatische verversing van het boomvenster op/af}
menuText N TreeOptLock "Vergrendelen" 0 {Vergrendelen/Ontgrendelen van de boom bij de huidige database}
menuText N TreeOptTraining "Training" 0 {Aan/Uit zetten training modus}
menuText N TreeOptAutosave "Autom.cache-data Bewaren" 4 \
  {Automatisch bewaren van het cache bestand bij sluiten boomvenster}
menuText N TreeHelp "Help" 0
menuText N TreeHelpTree "Hulp bij zoekboom" 0
menuText N TreeHelpIndex "Index" 0
translate N SaveCache {Cache Bewaren}
translate N Training {Training}
translate N LockTree {Boom Vergrendelen}
translate N TreeLocked {Vergrendeld}
translate N TreeBest {Beste}
translate N TreeBestGames {Boom Beste partijen}
# Note: the next message is the tree window title row. After editing it,
# check the tree window to make sure it lines up with the actual columns.
translate N TreeTitleRow \
  {    Zet   ECO       Frekwentie    Score  GemElo Prest GemJaar %Remises} ;
translate N TreeTotal {TOTAAL}
translate N DoYouWantToSaveFirst {Wil u eerst de verandering bewaren?}
translate N AddToMask {Toevoegen aan het Masker}
translate N RemoveFromMask {Verwijderen uit het Masker}
translate N AddThisMoveToMask {Voeg deze zet toe aan het Masker}
translate N SearchMask {Zoek in Masker}
translate N DisplayMask {Toon Masker}
translate N Nag {Nag code}
translate N Marker {Aanwijzer}
translate N Include {Invoegen}
translate N Exclude {Weglaten}
translate N MainLine {Hoofdvariant}
translate N Bookmark {Bladwijzer}
translate N NewLine {Nieuwe variant}
translate N ToBeVerified {Na te kijken}
translate N ToTrain {Trainen}
translate N Dubious {Twijfelachtig}
translate N ToRemove {Verwijderen}
translate N NoMarker {Geen aanwijzer}
translate N ColorMarker {Kleur}
translate N WhiteMark {Wit}
translate N GreenMark {Groen}
translate N YellowMark {Geel}
translate N BlueMark {Blauw}
translate N RedMark {Rood}
translate N CommentMove {Commentaar op zet}
translate N CommentPosition {Commentaar op stelling}
translate N AddMoveToMaskFirst {Voeg de zet eerst toe aan het masker}
translate N OpenAMaskFileFirst {Open eerst een maskerbestand}
translate N Positions {Stellingen}
translate N Moves {Zetten}

# Finder window:
menuText N FinderFile "Bestand" 0
menuText N FinderFileSubdirs "Kijken in subdirectories" 0
menuText N FinderFileClose "Sluiten bestandszoeker" 0
menuText N FinderSort "Sorteren" 0
menuText N FinderSortType "Type" 0
menuText N FinderSortSize "Grootte" 0
menuText N FinderSortMod "Gewijzigd" 0
menuText N FinderSortName "Naam" 0
menuText N FinderSortPath "Pad" 0
menuText N FinderTypes "Types" 0
menuText N FinderTypesScid "Scid databases" 0
menuText N FinderTypesOld "Oud formaat Scid databases" 0
menuText N FinderTypesPGN "PGN bestanden" 0
menuText N FinderTypesEPD "EPD (boek) bestanden" 0
menuText N FinderTypesRep "Repertorium bestanden" 0
menuText N FinderHelp "Help" 0
menuText N FinderHelpFinder "Bestandszoeker Help" 0
menuText N FinderHelpIndex " Bestandszoeker Help Inhoud" 0
translate N FileFinder {Bestandszoeker}
translate N FinderDir {Folder}
translate N FinderDirs {Folders}
translate N FinderFiles {Bestanden}
translate N FinderUpDir {Hogere Folder}
translate N FinderCtxOpen {Open}
translate N FinderCtxBackup {Reservecopie}
translate N FinderCtxCopy {Copieren}
translate N FinderCtxMove {Verplaatsen}
translate N FinderCtxDelete {Verwijderen}

# Player finder:
menuText N PListFile "Bestand" 0
menuText N PListFileUpdate "Bijwerken" 0
menuText N PListFileClose "Dichtbij Speler Vinder" 0 ;
menuText N PListSort "Sorteren" 0
menuText N PListSortName "Naam" 0 ;
menuText N PListSortElo "Elo" 0
menuText N PListSortGames "Partijen" 0 ;
menuText N PListSortOldest "Oudste" 0 ;
menuText N PListSortNewest "Niewste" 0 ;

# Tournament finder:
menuText N TmtFile "Bestand" 0
menuText N TmtFileUpdate "Bijwerken" 0
menuText N TmtFileClose "Sluiten Toernooi zoeker" 0
menuText N TmtSort "Sorteren" 0
menuText N TmtSortDate "Datum" 0
menuText N TmtSortPlayers "Spelers" 0
menuText N TmtSortGames "Partijen" 0
menuText N TmtSortElo "Elo" 0
menuText N TmtSortSite "Plaats" 0
menuText N TmtSortEvent "Evenement" 1
menuText N TmtSortWinner "Winnaar" 0
translate N TmtLimit "Limiet Lijst"
translate N TmtMeanElo "Laagste gem. Elo" 
translate N TmtNone "Geen toernooien gevonden."

# Graph windows:
menuText N GraphFile "Bestand" 0
menuText N GraphFileColor "Bewaren als kleuren Postscript..." 8
menuText N GraphFileGrey "Bewaren als grijze Postscript..." 8
menuText N GraphFileClose "Venster sluiten" 0
menuText N GraphOptions "Opties" 0
menuText N GraphOptionsWhite "Wit" 0
menuText N GraphOptionsBlack "Zwart" 0
menuText N GraphOptionsBoth "Beide" 1
menuText N GraphOptionsPInfo "Speler informatie" 0
# ====== TODO To be translated ======
menuText N GraphOptionsEloFile "Elo from rating file" 0
# ====== TODO To be translated ======
menuText N GraphOptionsEloDB "Elo from database" 0
translate N GraphFilterTitle "Filtergrafiek: frequentie per 1000 partijen"
translate N GraphAbsFilterTitle "Filtergrafiek: frequentie van de partijen"
translate N ConfigureFilter {Configureer de X-as voor Jaar, Elo en Zetten}
translate N FilterEstimate "Schat"
translate N TitleFilterGraph "Scid: Filtergrafiek"

# Analysis window:
translate N AddVariation {Toevoegen variant}
translate N AddAllVariations {Voeg alle varianten toe}
translate N AddMove {Toevoegen zet}
translate N Annotate {Annotatie}
translate N ShowAnalysisBoard {Toon het analysebord}
translate N ShowInfo {Toon engine informatie}
translate N FinishGame {Beeindig de partij}
translate N StopEngine {Stop de engine}
translate N StartEngine {Start de engine}
translate N LockEngine {Fixeer de engine op de huidige stelling}
translate N AnalysisCommand {Analyse commando}
translate N PreviousChoices {Voorgaande keuzes}
translate N AnnotateTime {Geef de analysetijd in seconden per zet}
translate N AnnotateWhich {Voeg varianten toe}
translate N AnnotateAll {Voor zetten van beide zijden}
translate N AnnotateAllMoves {Annotateer alle zetten}
translate N AnnotateWhite {Alleen voor zetten door Wit}
translate N AnnotateBlack { Alleen voor zetten door Zwart}
translate N AnnotateBlundersOnly {Als de partijzet een klaarblijkelijke blunder is}
translate N AnnotateBlundersOnlyScoreChange {Blunder Analyse rapport, met scorewijziging van/naar: }
translate N BlundersThreshold {Ondergrens}
# TODO: Translate
translate N ScoreAllMoves {Waardeer alle zetten}
translate N LowPriority {Lage CPU prioriteit} ;
translate N ClickHereToSeeMoves {Klik hier om de zetten te zien}
translate N ConfigureInformant {Informant}
translate N Informant!? {Interessante zet}
translate N Informant? {Slechte zet}
translate N Informant?? {Blunder}
translate N Informant?! {Twijfelachtige zet}
translate N Informant+= {Wit heeft een klein voordeel}
translate N Informant+/- {Wit heeft een gematigd voordeel}
translate N Informant+- {Wit heeft een beslissend voordeel}
translate N Informant++- {Deze partij kan als gewonnen beschouwd worden}
translate N Book {Openingenboek}
translate N OtherBookMoves {Boek van de tegenspeler}
translate N OtherBookMovesTooltip {Zetten waarvoor de tegenspeler een antwoord heeft}

# Analysis Engine open dialog:
translate N EngineList {Analyse Engine Lijst}
translate N EngineName {Naam}
translate N EngineCmd {Commando}
translate N EngineArgs {Parameters}
translate N EngineDir {Folder}
translate N EngineElo {Elo}
translate N EngineTime {Datum}
translate N EngineNew {Nieuw}
translate N EngineEdit {Bewerk}
translate N EngineRequired {Velden in <b>vet</b> zijn vereist; de andere facultatief}

# PGN window menus:
menuText N PgnFile "Bestand" 0
menuText N PgnFileCopy "Copieer Partij naar Klipbord" 0 ;#
menuText N PgnFilePrint "Als pgn bestand Bewaren..." 0
menuText N PgnFileClose "PGN-venster sluiten" 0
menuText N PgnOpt "Opties" 0
menuText N PgnOptColor "Instellen kleuren" 10
menuText N PgnOptShort "Korte (3 regelige) kop" 0
menuText N PgnOptSymbols "Symbolen annotaties" 10
menuText N PgnOptIndentC "Inspringen (commentaar)" 12
menuText N PgnOptIndentV "Inspringen (variant)" 12
menuText N PgnOptColumn "Kolom stijl (een zet per regel)" 0
menuText N PgnOptSpace "Spatie na zetnummer" 0
menuText N PgnOptStripMarks "Verwijder gekleurde vierkante haken codes" 1
menuText N PgnOptBoldMainLine "Gebrui Dikke Tekst voo Hoofdvariant Zetten" 4 ;
menuText N PgnColor "Kleuren" 0
menuText N PgnColorHeader "Kop..." 0
menuText N PgnColorAnno "Annotaties..." 0
menuText N PgnColorComments "Commentaar..." 0
menuText N PgnColorVars "Varianten..." 0
menuText N PgnColorBackground "Achtergrond kleur..." 0
 
menuText N PgnColorMain "Main line..." 0
 
menuText N PgnColorCurrent "Current move background..." 1
 
menuText N PgnHelp "Help" 0
menuText N PgnHelpPgn "PGN help" 0
menuText N PgnHelpIndex "Inhoud" 0
translate N PgnWindowTitle {Partij Notatie - partij %u} ;

# Crosstable window menus:
menuText N CrosstabFile "Bestand" 0
menuText N CrosstabFileText "Bewaren in tekstformaat..." 13
menuText N CrosstabFileHtml " Bewaren in HTML-formaat..." 13
menuText N CrosstabFileLaTeX " Bewaren in LaTeX-formaat..." 13
menuText N CrosstabFileClose "Kruistabel sluiten" 0
menuText N CrosstabEdit "Bewerken" 0
menuText N CrosstabEditEvent "Evenement" 0
menuText N CrosstabEditSite "Plaats" 0
menuText N CrosstabEditDate "Datum" 0
menuText N CrosstabOpt "Opties" 0
menuText N CrosstabOptAll "Gesloten" 0
menuText N CrosstabOptSwiss "Zwitsers" 0
menuText N CrosstabOptKnockout "Knockout" 0
menuText N CrosstabOptAuto "Auto" 0
menuText N CrosstabOptAges "Leeftijd in jaren" 8
menuText N CrosstabOptNats "Nationaliteiten" 1
menuText N CrosstabOptRatings "Elo" 0
menuText N CrosstabOptTitles "Titels" 0
menuText N CrosstabOptBreaks "Tie-break scores" 4
menuText N CrosstabOptDeleted "Inclusief gewiste partijen" 8
menuText N CrosstabOptColors "Kleuren (alleen bij Zwitsers)" 0
menuText N CrosstabOptColumnNumbers "Genummerde kolommen (Alleen bij gesloten tabel)" 2
menuText N CrosstabOptGroup "Punten (groep)" 0
menuText N CrosstabSort "Sorteren" 0
menuText N CrosstabSortName "Naam" 0
menuText N CrosstabSortRating "Elo" 0
menuText N CrosstabSortScore "Score" 0
menuText N CrosstabColor "Kleuren" 0
menuText N CrosstabColorPlain "Tekst" 0
menuText N CrosstabColorHyper "Hypertekst" 1
menuText N CrosstabHelp "Help" 0
menuText N CrosstabHelpCross "Help (kruistabel)" 0
menuText N CrosstabHelpIndex "Inhoud" 0
translate N SetFilter {Zet Filter}
translate N AddToFilter {Toevoegen aan selectie}
translate N Swiss {Zwitsers}
translate N Category {Categorie} ;

# Opening report window menus:
menuText N OprepFile "Bestand" 0
menuText N OprepFileText "Bewaren in tekstformaat..." 13
menuText N OprepFileHtml " Bewaren in HTML-formaat..." 13
menuText N OprepFileLaTeX " Bewaren in LaTeX-formaat..." 13
menuText N OprepFileOptions "Opties..." 0
menuText N OprepFileClose "Sluit rapportvenster" 0
menuText N OprepFavorites "Favorieten" 1 ;
menuText N OprepFavoritesAdd "Voeg Rapport toe..." 0 ;
menuText N OprepFavoritesEdit "Bewerk Favoriete Rapport..." 0 ;
menuText N OprepFavoritesGenerate "Genereer Rapporten..." 0 ;
menuText N OprepHelp "Help" 0
menuText N OprepHelpReport "Help (openingsrapport)" 0
menuText N OprepHelpIndex "Inhoud" 0

# Header search:
translate N HeaderSearch {Zoek naar kop}
translate N EndSideToMove {Zijde aan zet bij partijeinde} ;
translate N GamesWithNoECO {Partijen zonder ECO?}
translate N GameLength {Lengte partij}
translate N FindGamesWith {Vind partijen met vlag}
translate N StdStart {Ab-normaal begin}
translate N Promotions {Promoties}
translate N Comments {Commentaar}
translate N Variations {Varianten}
translate N Annotations {Annotaties}
translate N DeleteFlag {Gewist-markeringen}
translate N WhiteOpFlag {Wit opening}
translate N BlackOpFlag {Zwart opening}
translate N MiddlegameFlag {Middenspel}
translate N EndgameFlag {Eindspel}
translate N NoveltyFlag {Nieuwtje}
translate N PawnFlag {Pionnenstructur}
translate N TacticsFlag {Tactiek}
translate N QsideFlag {Damevleugel}
translate N KsideFlag {Koningsvleugel}
translate N BrilliancyFlag {Briljant}
translate N BlunderFlag {Blunder}
translate N UserFlag {Gebruiker}
translate N PgnContains {PGN bevat tekst}
# ====== TODO To be translated ======
translate N Annotator {Annotator}
# ====== TODO To be translated ======
translate N Cmnts {Annotated games only}

# Game list window:
translate N GlistNumber {Nummer}
translate N GlistWhite {Wit}
translate N GlistBlack {Zwart}
translate N GlistWElo {W-Elo}
translate N GlistBElo {Z-Elo}
translate N GlistEvent {Evenement}
translate N GlistSite {Plaats}
translate N GlistRound {Ronde}
translate N GlistDate {Datum}
translate N GlistYear {Jaar}
translate N GlistEDate {Datum evenement}
translate N GlistResult {Uitslag}
translate N GlistLength {Lengte}
translate N GlistCountry {Country}
translate N GlistECO {ECO}
translate N GlistOpening {Opening}
translate N GlistEndMaterial {Eind-Material}
translate N GlistDeleted {Verwijderd}
translate N GlistFlags {Markeringen}
translate N GlistVars {Varianten}
translate N GlistComments {Commentaar}
translate N GlistAnnos {Annotaties}
translate N GlistStart {Start}
translate N GlistGameNumber {Partij nummer}
# ====== TODO To be translated ======
translate N GlistAverageElo {Average Elo}
# ====== TODO To be translated ======
translate N GlistRating {Rating}
translate N GlistFindText {Tekst vinden}
translate N GlistMoveField {Zet}
translate N GlistEditField {Configuratie}
translate N GlistAddField {Voeg toe}
translate N GlistDeleteField {Verwijder}
translate N GlistWidth {Breedte}
translate N GlistAlign {Uitlijnen}
# ====== TODO To be translated ======
translate N GlistAlignL {Align: left}
# ====== TODO To be translated ======
translate N GlistAlignR {Align: right}
# ====== TODO To be translated ======
translate N GlistAlignC {Align: center}
translate N GlistColor {Kleuren}
translate N GlistSep {Separator}
# ====== TODO To be translated ======
translate N GlistCurrentSep {-- Current --}
# ====== TODO To be translated ======
translate N GlistNewSort {New}
# ====== TODO To be translated ======
translate N GlistAddToSort {Add}
# ====== TODO To be translated ======
translate N GsortSort {Sort...}
# ====== TODO To be translated ======
translate N GsortDate {Date}
# ====== TODO To be translated ======
translate N GsortYear {Year}
# ====== TODO To be translated ======
translate N GsortEvent {Event}
# ====== TODO To be translated ======
translate N GsortSite {Site}
# ====== TODO To be translated ======
translate N GsortRound {Round}
# ====== TODO To be translated ======
translate N GsortWhiteName {White Name}
# ====== TODO To be translated ======
translate N GsortBlackName {Black Name}
# ====== TODO To be translated ======
translate N GsortECO {ECO}
# ====== TODO To be translated ======
translate N GsortResult {Result}
# ====== TODO To be translated ======
translate N GsortMoveCount {Move Count}
# ====== TODO To be translated ======
translate N GsortAverageElo {Average Elo}
# ====== TODO To be translated ======
translate N GsortCountry {Country}
# ====== TODO To be translated ======
translate N GsortDeleted {Deleted}
# ====== TODO To be translated ======
translate N GsortEventDate {Event Date}
# ====== TODO To be translated ======
translate N GsortWhiteElo {White Elo}
# ====== TODO To be translated ======
translate N GsortBlackElo {Black Elo}
# ====== TODO To be translated ======
translate N GsortComments {Comments}
# ====== TODO To be translated ======
translate N GsortVariations {Variations}
# ====== TODO To be translated ======
translate N GsortNAGs {NAGs}
# ====== TODO To be translated ======
translate N GsortAscending {Ascending}
# ====== TODO To be translated ======
translate N GsortDescending {Descending}
# ====== TODO To be translated ======
translate N GsortAdd {Add}
# ====== TODO To be translated ======
translate N GsortStore {Store}
# ====== TODO To be translated ======
translate N GsortLoad {Load}
 
translate N GlistRemoveThisGameFromFilter  {Remove this game from Filter}
 
translate N GlistRemoveGameAndAboveFromFilter  {Remove game (and all above it) from Filter}
 
translate N GlistRemoveGameAndBelowFromFilter  {Remove game (and all below it) from Filter}
translate N GlistDeleteGame {Maak de verwijdering van deze partij ongedaan} 
translate N GlistDeleteAllGames {Verwijder alle partijen in de filter} 
translate N GlistUndeleteAllGames {Maak de verwijdering van alle partijen in de filter ongedaan} 
translate N GlistMergeGameInBase {Voeg Partij toe aan een andere database} 

# Maintenance windows
translate N DatabaseName {Naam database:}
translate N TypeIcon {Type icoon:}
translate N NumOfGames {Partijen:}
translate N NumDeletedGames {Gewiste partijen:}
translate N NumFilterGames {Partijen in selectie:}
translate N YearRange {JaarBereik:}
translate N RatingRange {EloBereik (laag/hoog):}
translate N Description {Beschrijving} ;
translate N Flag {Markering}
translate N CustomFlags {Markering volgens keus}
translate N DeleteCurrent {Wis huidige partij}
translate N DeleteFilter {Wis geselecteerde partijen}
translate N DeleteAll {Wis alle partijen}
translate N UndeleteCurrent {Haal huidige partij terug}
translate N UndeleteFilter {Haal geselecteerde partijen terug}
translate N UndeleteAll {Haal alle partijen terug}
translate N DeleteTwins {Wis doublures}
translate N MarkCurrent {Markeer huidige partij}
translate N MarkFilter {Markeer geselecteerde partijen}
translate N MarkAll {Markeer alle partijen}
translate N UnmarkCurrent {Verwijder Markering huidige partij)}
translate N UnmarkFilter {Verwijder Markering geselecteerde partijen)}
translate N UnmarkAll {Verwijder Markering alle partijen)}
translate N Spellchecking {Spellingscontrole}
translate N Players {Spelers}
translate N Events {Evenementen}
translate N Sites {Plaatsen}
translate N Rounds {Rondes}
translate N DatabaseOps {Database bewerkingen}
translate N ReclassifyGames {Partijen ECO-classificeren...}
translate N CompactDatabase {Database compact maken = optimaliseren}
translate N SortDatabase {Database sorteren}
translate N AddEloRatings {Toevoegen Elo classificatie}
translate N AutoloadGame {Auto-laden partij nummer}
translate N StripTags {Verwijder PGN labels}
translate N StripTag {Verwijder label}
translate N Cleaner {Reiniger}
translate N CleanerHelp {
De Scid Reiniger zal alle onderhoudsactiviteiten die u selecteert uit onderstaande lijst, uitvoeren op de huidige database. 
De dialogen van de huidige instellingen in de ECO classificatie en verwijderen van doublures zullen worden toegepast indien u deze functies selecteert.
}
translate N CleanerConfirm {
Eens het Reiniger onderhoud is gestart, kan dit niet worden onderbroken!

Dit kan lang duren op een grote database, afhankelijk van de geselecteerde functies en de huidige instellingen.

Weet u zeker dat u de geselecteerde onderhoudsfuncties wilt uitvoeren?
}
translate N TwinCheckUndelete {omwisselen; "u" maakt de verwijdering van beide ongedaan)}
translate N TwinCheckprevPair {Vorige paar}
translate N TwinChecknextPair {Volgende paar}
translate N TwinChecker {Scid: Tweeling partij verifieerder}
translate N TwinCheckTournament {Partijen in het toernooi:}
translate N TwinCheckNoTwin {Geen tweeling  }
translate N TwinCheckNoTwinfound {Er werd geen tweeling gedetecteerd voor deze partij.\n
Om tweelingen te laten zien met dit venster moet U eerst de "Verwijder tweeling partijen..." functie gebruiken. }
translate N TwinCheckTag {Deel etiket...}
translate N TwinCheckFound1 {Scid vond $result tweeling partijen}
translate N TwinCheckFound2 { en zet hun verwijder vlaggen}
translate N TwinCheckNoDelete {Er zijn geen partijen in deze databank om te verwijderen.}
translate N TwinCriteria1 { Uw instellingen voor het vinden van tweeling partijen kunnen mogelijkerwijs \n tot gevolg hebben dat partijen met gelijkaardige zetten ten onrechte als tweelingen worden aangegeven.}
translate N TwinCriteria2 {Het wordt aangeraden dat indien U "Nee" selecteert voor "dezelfde zetten" dat U dan "Ja" selecteert voor de kleuren, event, plaats, ronde, jaar en maand instellingen.\n Wil U doorgaan en de tweelingen toch verwijderen?}
translate N TwinCriteria3 {Het wordt aangeraden dat U "Ja" selecteert voor tenminste twee instellingen uit "zelfde plaats", "zelfde ronde" en "zelfde jaar".\n Wil U doorgaan en de tweelingen toch verwijderen?}
translate N TwinCriteriaConfirm {Scid: Bevestig tweeling instellingen}
translate N TwinChangeTag "Veranderder de volgende partij labels:\n\n"
translate N AllocRatingDescription "Dit commando zal het huidige spellingscontrole bestand gebruiken om Elo punten toe te voegen aan partijen in deze databank. Indien een speler geen huidige Elo punten heeft maar zijn/haar punten op het moment van het spel zijn aanwezig in het spellingscontrole bestand dan zullen deze punten toegevoegd worden."
translate N RatingOverride "Overschrijf bestaande, van nul verschillende, punten?"
translate N AddRatings "Voeg Elo punten to aan:"
translate N AddedRatings {Scid voegde $r Elo punten toe aan $g partijen.}
translate N NewSubmenu "Nieuw submenu"

# Comment editor:
translate N AnnotationSymbols  {Symbolen voor annotatie:}
translate N Comment {Commentaar:}
translate N InsertMark {Voeg markering toe} ;
translate N InsertMarkHelp {
Voeg toe/verwijder markering: Selecteer  kleur, type, veld.
Voeg toe/verwijder pijl: Rechtsklik twee velden.
} ;

# Nag buttons in comment editor:
translate N GoodMove {Goede zet} ;
translate N PoorMove {Slechte zet} ;
translate N ExcellentMove {Excellente zet} ;
translate N Blunder {Blunder}
translate N InterestingMove {Interessante zet} ;
translate N DubiousMove {Twijfelachtige zet} ;
translate N WhiteDecisiveAdvantage {Wit heeft beslissend voordeel} ;
translate N BlackDecisiveAdvantage {Zwart heeft beslissend voordee} ;
translate N WhiteClearAdvantage {Wit heeft duidelijk voordeel} ;
translate N BlackClearAdvantage {Zwart heeft beslissend voordeel} ;
translate N WhiteSlightAdvantage {Wit heeft licht voordeel} ;
translate N BlackSlightAdvantage {Zwart heeft licht voordee} ;
translate N Equality {Gelijk} ;
translate N Unclear {Onduidelijk} ;
translate N Diagram {Diagram} ;

# Board search:
translate N BoardSearch {Zoeken Bord}
translate N FilterOperation {Toepassen op huidige selectie:}
translate N FilterAnd {AND (Selectie beperken)}
translate N FilterOr {OR (Selectie uitbreiden)}
translate N FilterIgnore {Selectie Ongedaan maken}
translate N SearchType {Zoek type stelling:}
translate N SearchBoardExact {Exacte stelling (stukken op dezelfde velden)}
translate N SearchBoardPawns {Pionnen (hetzelfde materiaal, alle pionnen op dezelfde velden)}
translate N SearchBoardFiles {Lijnen (hetzelfde materiaal, alle pionnen op dezelfde lijnen)}
translate N SearchBoardAny {Willekeurig (hetzelfde materiaal, pionnen en stukken willekeurig)}
translate N SearchInRefDatabase { Zoek in database }
translate N LookInVars {Zoek in varianten}

# Material search:
translate N MaterialSearch {Zoeken Materiaal}
translate N Material {Materiaal}
translate N Patterns {Patroon}
translate N Zero {Niets}
translate N Any {Willekeurig}
translate N CurrentBoard {Huidige stelling}
translate N CommonEndings {Veel Voorkomende Eindspelen}
translate N CommonPatterns { Veel Voorkomende patronen}
translate N MaterialDiff {Material onevenwicht}
translate N squares {Velden}
translate N SameColor {Gelijke kleur}
translate N OppColor {Ongelijke kleur}
translate N Either {Beide}
translate N MoveNumberRange {Zet bereik }
translate N MatchForAtLeast {Op z'n minst gelijk}
translate N HalfMoves {halve zetten}

# Common endings in material search:
translate N EndingPawns {Pionnen eindspel} ;
translate N EndingRookVsPawns {Toren tegen Pion(nen)} ;
translate N EndingRookPawnVsRook {Toren en 1 Pion tegen Toren} ;
translate N EndingRookPawnsVsRook {Toren en Pionnen tegen Toren} ;
translate N EndingRooks {Toren tegen Toren Eindspel} ;
translate N EndingRooksPassedA {Toren tegen Toren met een vrije a-pion} ;
translate N EndingRooksDouble {Dubbele Toren eindspelen} ;
translate N EndingBishops {Loper tegen Loper eindspel} ;
translate N EndingBishopVsKnight {Loper tegen Paard eindspe} ;
translate N EndingKnights {Paard tegenPaard eindspel} ;
translate N EndingQueens {Dame tegen Dame eindspel} ;
translate N EndingQueenPawnVsQueen {Dame +1 pion tegen Dame} ;
translate N BishopPairVsKnightPair {Loperpaar tegen 2 Paarden middenspel} ;

# Common patterns in material search:
translate N PatternWhiteIQP {Witte Geïsoleerde pion} ;
translate N PatternWhiteIQPBreakE6 {Witte Geïsoleerde pion: d4-d5 doorbraak vs. e6} ;
translate N PatternWhiteIQPBreakC6 {Witte Geïsoleerde pion: d4-d5 doorbraak vs. c6} ;
translate N PatternBlackIQP {Zwarte Geïsoleerde pion} ;
translate N PatternWhiteBlackIQP {Witte Geïsoleerde pion  vs. Zwarte Geïsoleerde pion} ;
translate N PatternCoupleC3D4 {Witte hangende pionnen c3+d4 } ;
translate N PatternHangingC5D5 {Zwarte hangende pionnen c5+d5} ;
translate N PatternMaroczy {Maroczy Centrum ( Pionnen op c4 en e4)} ;
translate N PatternRookSacC3 {Torenoffer op c3} ;
translate N PatternKc1Kg8 {O-O-O vs. O-O (Kc1 vs. Kg8)} ;
translate N PatternKg1Kc8 {O-O vs. O-O-O (Kg1 vs. Kc8)} ;
translate N PatternLightFian {Light-Square Fianchettos (Loper-g2 vs. Loper-b7)} ;
translate N PatternDarkFian {Dark-Square Fianchettos (Loper-b2 vs. Loper-g7)} ;
translate N PatternFourFian {Four Fianchettos (Lopers op b2,g2,b7,g7)} ;

# Game saving:
translate N Today {Nu}
translate N ClassifyGame {Partij classificeren}

# Setup position:
translate N EmptyBoard {Bord leegmaken}
translate N InitialBoard {Beginstelling}
translate N SideToMove {Aan zet:}
translate N MoveNumber {Zetnummer}
translate N Castling {Rokade}
translate N EnPassantFile {En Passant lijn}
translate N ClearFen {FEN leegmaken}
translate N PasteFen {FEN plakken}
translate N SaveAndContinue {Bewaar en ga door}
translate N DiscardChangesAndContinue {Laat veranderingen vallen\nen ga door}
translate N GoBack {Ga terug}

# Replace move dialog:
translate N ReplaceMove {Zet vervangen}
translate N AddNewVar {Voeg Nieuwe Variant Toe}
translate N NewMainLine {Nieuwe Hoofd Variant}
translate N ReplaceMoveMessage {Hier is al een zet.  

U kunt hem vervangen en alle volgende zetten wissen, of uw zet toevoegen als een nieuwe variant.

(U kunt deze boodschap in de toekeomst vermijden door de optie "Zet vervangen bevestigen" uit te zetten in het menu:Zetten)}

# Make database read-only dialog:
translate N ReadOnlyDialog {Als u deze database "alleen-lezen" maakt, zijn geen veranderingen toegestaan.
Er kunnen geen partijen meer worden opgeslagen of veranderd en ook geen wis-markeringen toegevoegd of verwijderd.
Elke sortering of ECO-classificering zal tijdelijk zijn dan voor deze database.

U kunt de database weer schrijf-toegankelijk maken door hem te sluiten en weer te openen.

Wilt u echt deze database alleen-lezen maken?}

# Clear game dialog:
translate N ClearGameDialog {Deze partij is gewijzigd.

Wilt u echt doorgaan en de wijzigingen niet doorvoeren?
}

# Exit dialog:
translate N ExitDialog {Wilt u Scid werkelijk afsluiten?}
translate N ExitUnsaved {De volgende databases hebben nog onopgeslagen wijzigingen in hun partijen.  Als u nu afsluit zullen deze wijzigingen verloren gaan.}

# Import window:
translate N PasteCurrentGame {Plak huidige partij}
translate N ImportHelp1 {Invoeren of plak een PGN-formaat partij in het venster hierboven.}
translate N ImportHelp2 {Alle import-fouten worden hier weergegeven.}
translate N OverwriteExistingMoves {Overschrijf bestaande zetten ?}

# ECO Browser:
translate N ECOAllSections {alle ECO code secties}
translate N ECOSection {ECO sectie}
translate N ECOSummary {Samenvatting voor}
translate N ECOFrequency {Frequentie van subcodes voor}

# Opening Report:
translate N OprepTitle {Openings Rapportage}
translate N OprepReport {Rapportage}
translate N OprepGenerated {Samengesteld door }
translate N OprepStatsHist {Statistieken en Geschiedenis}
translate N OprepStats {Statistieken}
translate N OprepStatAll {Alle Rapportage Partijen}
translate N OprepStatBoth {Beide Spelers Elo}
translate N OprepStatSince {Sinds}
translate N OprepOldest {Oudste partijen}
translate N OprepNewest {Meest recente partijen}
translate N OprepPopular {Huidige Populariteit}
translate N OprepFreqAll {Frequentie over alle jaren: }
translate N OprepFreq1   {In het afgelopen jaar:      }
translate N OprepFreq5   {In de afgelopen 5 jaar:    }
translate N OprepFreq10  {In de afgelopen 10 jaar:    }
translate N OprepEvery {Eens per %u partijen}
translate N OprepUp {%u%s hoger dan alle jaren}
translate N OprepDown {%u%s lager dan alle jaren}
translate N OprepSame {zelfde als alle jaren}
translate N OprepMostFrequent {Meest frequente spelers}
translate N OprepMostFrequentOpponents {Frekwentste tegenstanders} ;
translate N OprepRatingsPerf {Elo Classificatie en Resultaten}
translate N OprepAvgPerf {Gemiddelde Elo Classificatie en Resultaten }
translate N OprepWRating {Witte Elo Classificatie }
translate N OprepBRating {Zwarte Elo Classificatie }
translate N OprepWPerf {Prestatie wit}
translate N OprepBPerf {Prestatie zwart}
translate N OprepHighRating {Partijen met de hoogste gemiddelde Elo Classificatie }
translate N OprepTrends {Resultaten trends}
translate N OprepResults {Resultaat lengtes en frequenties}
translate N OprepLength {Partij lengte}
translate N OprepFrequency {Frequentie}
translate N OprepWWins {Overwinningen wit:   }
translate N OprepBWins {Overwinningen zwart: }
translate N OprepDraws {Remises: }
translate N OprepWholeDB {hele database}
translate N OprepShortest {Kortste winst}
# translate N OprepShortWhite {Kortste witte overwinningen}
# translate N OprepShortBlack {Kortste zwarte overwinningen}
translate N OprepMovesThemes {Zetten en thema's}
translate N OprepMoveOrders {Zetvolgorde om rapportstelling te bereiken}
translate N OprepMoveOrdersOne \
  {Er was slechts 1 volgorde om deze stelling te bereiken:}
translate N OprepMoveOrdersAll \
  {Er waren %u zet-volgordes om deze stelling te bereiken:}
translate N OprepMoveOrdersMany \
  {Er waren %u zet-volgordes om deze stelling te bereiken. De top %u zijn:}
translate N OprepMovesFrom {Zetten vanuit de rapportstelling:}
translate N OprepMostFrequentEcoCodes {Frekwentste ECO codes} ;
translate N OprepThemes {Positionele Thema's}
translate N OprepThemeDescription {Frekwentie van  themas in de eerste %u zetten van elke partij} ;
translate N OprepThemeSameCastling {Gelijke rochades}
translate N OprepThemeOppCastling {Tegengestelde rochades}
translate N OprepThemeNoCastling {Beide zijden niet gerocheerd}
translate N OprepThemeKPawnStorm {Pionnenstorm op koningsvleugel}
translate N OprepThemeQueenswap {Dameruil}
translate N OprepThemeWIQP {Witte Geïsoleerde Damepion} ;
translate N OprepThemeBIQP {Zwarte Geïsoleerde Damepion} ;
translate N OprepThemeWP567 {Witte pion op de 5/6/7de rij}
translate N OprepThemeBP234 {Zwarte pion op de 2/3/4de rij}
translate N OprepThemeOpenCDE {Open c/d/e lijn}
translate N OprepTheme1BishopPair {Slechts 1 kant heeft loperpaar}
translate N OprepEndgames {Eindspelen}
translate N OprepReportGames {Rapportage partijen}
translate N OprepAllGames {Alle partijen}
translate N OprepEndClass {Materiaal classificatie van eindstellingen}
translate N OprepTheoryTable {Theorie tabel}
translate N OprepTableComment {Samengesteld uit de %u partijen met de hoogste Elo classificatie.}
translate N OprepExtraMoves {Extra zetten in theorie tabel}
translate N OprepMaxGames {Maximaal aantal partijen in theorie tabel}
translate N OprepViewHTML { HTML zicht} ;
translate N OprepViewLaTeX {LaTeX zicht} ;

# Player Report:
translate N PReportTitle {Speler Rapport} ;
translate N PReportColorWhite {met Wit} ;
translate N PReportColorBlack {mee Zwart} ;
translate N PReportMoves {na %s} ;
translate N PReportOpenings {Openingen} ;
translate N PReportClipbase {Maak klipbord leeg en copieer de aan voorwaarden vervullende partijen ernaar} ;

# Piece Tracker window:
translate N TrackerSelectSingle {Linkse muisknop selecteert dit stuk.}
translate N TrackerSelectPair { Linkse muisknop selecteert dit stuk; de rechtermuisknop selecteert zijn buur.}
translate N TrackerSelectPawn { Linkse muisknop selcteert deze pion; ; de rechtermuisknop selecteert alle 8 pionnen.}
translate N TrackerStat {Statistiek}
translate N TrackerGames {% partijen met zet naar dit veld.}
translate N TrackerTime {% keer op ieder veld.}
translate N TrackerMoves {Zetten}
translate N TrackerMovesStart {Voer de zet in waar de Spoorvolger moet beginnen.}
translate N TrackerMovesStop { Voer de zet in waar de Spoorvolger moet stoppen.}

# Game selection dialogs:
translate N SelectAllGames {Alle partijen in de database}
translate N SelectFilterGames {Alleen partijen uit selectiefilter}
translate N SelectTournamentGames {Alleen partijen in huidig toernooi}
translate N SelectOlderGames {Alleen oudere partijen}

# Delete Twins window:
translate N TwinsNote {Om een dubbele partij te zijn moet deze minimaal dezelfde twee spelers en de onderstaande te selecteren criteria bevatten. Bij vondst van twee dubbele partijen wordt de kortste verwijderd. Hint: Controleer de database op spelfouten voordat doublures worden verwijderd. Dit verhoogt de kans op vinden van dubbele partijen. }
translate N TwinsCriteria {Criteria: Dubbele partijen moeten hebben...}
translate N TwinsWhich {Onderzoek welke partijen}
translate N TwinsColors {Spelers dezelfde kleur?}
translate N TwinsEvent {Hetzelfde evenement?}
translate N TwinsSite {Dezelfde lokatie?}
translate N TwinsRound {Dezelfde ronde?}
translate N TwinsYear {Hetzelfde jaar?}
translate N TwinsMonth {Dezelfde maand?}
translate N TwinsDay {Dezelfde dag?}
translate N TwinsResult {Hetzelfde resultaat?}
translate N TwinsECO {Dezelfde ECO code?}
translate N TwinsMoves {Dezelfde zetten?}
translate N TwinsPlayers {Vergelijken speler namen:}
translate N TwinsPlayersExact {Exacte overeenkomst}
translate N TwinsPlayersPrefix {Alleen eerste 4 letters}
translate N TwinsWhen {Wanner doublures verwijderen}
translate N TwinsSkipShort {Negeer alle partijen korter dan 5 zetten?}
translate N TwinsUndelete {Haal alle voor wissen gemarkeerde partijen eerst terug?}
translate N TwinsSetFilter {Selecteer alle verwijderde dubbele partijen?}
translate N TwinsComments {Altijd partijen met commentaar bewaren?}
translate N TwinsVars {Altijd partijen met varianten bewaren?}
translate N TwinsDeleteWhich {Welke partij wissen:}
translate N TwinsDeleteShorter {Kortste partij}
translate N TwinsDeleteOlder {Laagste partijnummer}
translate N TwinsDeleteNewer {Hoogste partijnummer}
translate N TwinsDelete {Verwijder partijen}

# Name editor window:
translate N NameEditType {Type naam om te wijzigen}
translate N NameEditSelect {Partijen om te wijzigen}
translate N NameEditReplace {Vervangen}
translate N NameEditWith {met}
translate N NameEditMatches {Gelijken: Druk Ctrl+1 tot Ctrl+9 om te selecteren}
# ====== TODO To be translated ======
translate N CheckGames {Check games}
# ====== TODO To be translated ======
translate N CheckGamesWhich {Check games}
# ====== TODO To be translated ======
translate N CheckAll {All games}
# ====== TODO To be translated ======
translate N CheckSelectFilterGames {Only games in filter}

# Classify window:
translate N Classify {Classificeren}
translate N ClassifyWhich {ECO-Classificatie: welke partijen}
translate N ClassifyAll {Alle partijen (overschrijven oude ECO codes)}
translate N ClassifyYear {Alle partijen gespeeld in het afgelopen jaar}
translate N ClassifyMonth {Alle partijen gespeeld in de afgelopen maand}
translate N ClassifyNew {Alleen partijen zonder ECO code}
translate N ClassifyCodes {ECO Codes om te gebruiken}
translate N ClassifyBasic {Alleen basis codes ("B12", ...)}
translate N ClassifyExtended {Scid extenties ("B12j", ...)}

# Compaction:
translate N NameFile {Namen Bestand}
translate N GameFile {Partijen Bestand}
translate N Names {Namen}
translate N Unused {Ongebruikt}
translate N SizeKb {Grootte (Kb)}
translate N CurrentState {Huidige toestand}
translate N AfterCompaction {Na comprimeren}
translate N CompactNames {Gecomprimeerde namen bestand}
translate N CompactGames {Gecomprimeerd partijen bestand}
translate N NoUnusedNames "Er zijn geen ongebruikte namen dus het naambestand is reeds volledig gecomprimeerd."
translate N NoUnusedGames "Het partijen bestand is reeds volledig gecomprimeerd."
translate N GameFileCompacted {Het namen bestand voor de databank werd comprimeerd.}

# Sorting:
translate N SortCriteria {Criteria}
translate N AddCriteria {Toevoegen criteria}
translate N CommonSorts {Algemene sorteringen}
translate N Sort {Sorteren}

# Exporting:
translate N AddToExistingFile {Toevoegen partijen aan bestaand bestand?}
translate N ExportComments {Exporteren commentaar?}
translate N ExportVariations {Exporteren varianten?}
translate N IndentComments {Inspringen commentaar?}
translate N IndentVariations {Inspringen varianten?}
translate N ExportColumnStyle {Kolomstijl (een zet per regel)?}
translate N ExportSymbolStyle {Symbolische annotatie stijl:}
translate N ExportStripMarks {Wis vierkante haken/pijlen markeer codes uit de commentaar?}

# Goto game/move dialogs:
translate N LoadGameNumber {Geef het nummer van de te laden partij:}
translate N GotoMoveNumber {Ga naar zetnummer:}

# Copy games dialog:
translate N CopyGames {Kopiëren partijen}
translate N CopyConfirm {
 Wilt u echt kopieren
 de [::utils::thousands $nGamesToCopy] geselecteerde partijen
 van database "$fromName"
 naar database "$targetName"?
}
translate N CopyErr {Kan partijen niet kopieren}
translate N CopyErrSource {de bron database}
translate N CopyErrTarget {de doel database}
translate N CopyErrNoGames {heeft geen partijen in het filter}
translate N CopyErrReadOnly {is alleen-lezen}
translate N CopyErrNotOpen {is niet geopend}

# Colors:
translate N LightSquares {Lichte velden}
translate N DarkSquares {Donkere velden}
translate N SelectedSquares {Geselecteerde velden}
translate N SuggestedSquares {Zetsuggestie velden}
translate N WhitePieces {Witte stukken}
translate N BlackPieces {Zwarte stukken}
translate N WhiteBorder {Witte rand}
translate N BlackBorder {Zwarte rand}

# Novelty window:
translate N FindNovelty {Vind Nieuwtje}
translate N Novelty {Nieuwtje}
translate N NoveltyInterrupt {Zoeken nieuwtje onderbroken}
translate N NoveltyNone {In deze partij is geen nieuwtje gevonden}
translate N NoveltyHelp {
Scid zal de eerste zet vinden in de huidige partij, waarna een stelling ontstaat die nog niet was gevonden in de database of in het ECO openingsboek.
}

# Sounds configuration:
translate N SoundsFolder {Geluidsbestanden Map} ;
translate N SoundsFolderHelp {De map moet de bestanden King.wav, a.wav, 1.wav, enz. bevatten} ;
translate N SoundsAnnounceOptions {Zet Aankondigingen Opties} ;
translate N SoundsAnnounceNew {Kondig nieuwe zetten aan als ze gedaan worden} ;
translate N SoundsAnnounceForward {Kondig  aan als één zet vooruit gedaan wordt} ;
translate N SoundsAnnounceBack {Kondig  aan als één zet terug gedaan of genomen wordt} ;
translate N SoundsSoundDisabled {Scid kon het snack-audiopakket niet vinden bij het opstarten;\nhet geluid is uitgeschakeld.}

# Upgrading databases:
translate N Upgrading {Bijwerken}
translate N ConfirmOpenNew {
Dit is een oud formaat (Scid 3) database welke in Scid 4 niet kan worden geopend. Maar een nieuwe versie (Scid 4) is reeds aangemaakt.

Wilt u de database in het nieuwe formaat openen?
}
translate N ConfirmUpgrade {
Dit is een oud formaat (Scid 3) database. Een versie in het nieuwe formaat moet worden gemaakt, voordat het in Scid 4 kan worden gebruikt.

Bijwerken creëert een nieuwe versie van de database.

Dit kan enige tijd duren, maar het hoeft slechts eenmalig plaats te vinden. U kunt het afbreken indien het te lang duurt.

Wilt u de database nu bijwerken?
}

# Recent files options:
translate N RecentFilesMenu {Aantal recente bestanden in Bestand menu}
translate N RecentFilesExtra {Aantal recente bestand in extra submenu}

# My Player Names options:
translate N MyPlayerNamesDescription {Voeg hieronder een lijst met voorkeur spelernamen in, 1 speler per regel. Jokers (bvb "?" voor elke letter, "*" voor een reeks letters) zijn toegelaten.
Telkens een partij uit de lijst word geladen, zal het schaakbord worden gedraaid indien nodig om de partij vanuit die speler zijn perspektief te tonen.
} ;

translate N showblunderexists {toon dat er een blunder is}
translate N showblundervalue {toon blunder waarde}
translate N showscore {toon score}
translate N coachgame {coach partij}
translate N configurecoachgame {configureer coach partij}
translate N configuregame {Partij instellingen}
translate N Phalanxengine {Phalanx engine}
translate N Coachengine {Coach machiee}
translate N difficulty {moeilijkheid}
translate N hard {erg moeilijk}
translate N easy {gemakkelijk}
translate N Playwith {Speel met}
 
translate N white {wit}
 
translate N black {zwart}
 
translate N both {beide}
 
translate N Play {Spel}
 
translate N Noblunder {Geen blunder}
 
translate N blunder {Blunder}
 
translate N Noinfo {-- Geen info --}
translate N PhalanxOrTogaMissing {Phalanx of Toga werden niet gevonden}
 
 
 
translate N moveblunderthreshold {zet is blunder als het verlies groter is dan}
 
translate N limitanalysis {limiet engine analyse tijd}
 
translate N seconds {seconden}
 
translate N Abort {Breek af}
translate N Resume {Herneem}
 
translate N OutOfOpening {Uit de opening}
 
translate N NotFollowedLine {Je volgde de variant niet}
 
translate N DoYouWantContinue {Wil je verder gaan?}
translate N CoachIsWatching {De coach kijkt toe}
translate N Ponder {Altijd denken}
translate N LimitELO {Beperk ELO sterkte}
translate N DubiousMovePlayedTakeBack {Twijfelachtige zet gespeeld. Wil je terugnemen ?}
translate N WeakMovePlayedTakeBack {Zwakke zet gespeeld, wil je terugnemen ?}
translate N BadMovePlayedTakeBack {Slechte zet gespeeld, wil je terugnemen ?}
 
translate N Iresign {Ik geef op}
 
translate N yourmoveisnotgood {Je zet is niet goed}
 
translate N EndOfVar {Eind van variant}
 
translate N Openingtrainer {Opening trainer}
 
translate N DisplayCM {Toon kandidaat zetten}
 
translate N DisplayCMValue {Toon waarde kandidaat zetten}
translate N DisplayOpeningStats {Toon statistieken}
translate N ShowReport {Toon het rapport}
translate N NumberOfGoodMovesPlayed {gespeelde goede zetten}
translate N NumberOfDubiousMovesPlayed {gespeelde twijfelachtige zetten}
translate N NumberOfMovesPlayedNotInRepertoire {gespeelde zetten niet in het repertorium}
translate N NumberOfTimesPositionEncountered {aantal keer dat deze stelling voorkwam}
 
translate N PlayerBestMove  {Laat alleen beste zetten toe}
 
translate N OpponentBestMove {Tegenstander speelt beste zetten}
 
translate N OnlyFlaggedLines {Enkel gemarkeerde varianten}
translate N resetStats {Herstart de statistieken}
 
translate N Repertoiretrainingconfiguration {Repertorium training configuratie}
 
translate N Loadingrepertoire {Repertorium Laden}
 
translate N Movesloaded {Zetten geladen}
 
translate N Repertoirenotfound {Repertorium niet gevonden}
 
translate N Openfirstrepertoirewithtype {Open eerste repertorium database met ikoon/type aan rechterzijde gezet}
 
translate N Movenotinrepertoire {Zet niet in Repertorium}
translate N PositionsInRepertoire {Stellingen in het repertorium}
translate N PositionsNotPlayed {Niet gespeelde stellingen}
translate N PositionsPlayed {Gespeelde stellingen}
translate N Success {Succes}
translate N DubiousMoves {Twijfelachtige zetten}
translate N OutOfRepertoire {Uit het repertorium}
  
translate N ConfigureTactics {Configureer takiek}
 
translate N ResetScores {Initialiseer scores}
 
translate N LoadingBase {Laad database}
 
translate N Tactics {Taktiek}
 
translate N ShowSolution {Toon oplossing}
translate N NextExercise {Volgende oefening}
translate N PrevExercise {Voorgaande oefening}
translate N StopTraining {Stop training}
 
translate N Next {Volgende}
 
translate N ResettingScore {Initialiseer score}
 
translate N LoadingGame {Laad partij}
 
translate N MateFound {Mat gevonden}
 
translate N BestSolutionNotFound {Beste oplossing NIET gevonden!}
 
translate N MateNotFound {Mat NIET gevonden}
 
translate N ShorterMateExists {Korter mat bestaat}
 
translate N ScorePlayed {Score gespeeld}
 
translate N Expected {verwacht}
 
translate N ChooseTrainingBase {Kies training base}
 
translate N Thinking {Denkend}
 
translate N AnalyzeDone {Analyse gedaan}
 
translate N WinWonGame {Win gewonnen partij}
 
translate N Lines {Varianten}
 
translate N ConfigureUCIengine {Configureer UCI engine}
 
translate N SpecificOpening {Specifieke opening}
translate N StartNewGame {Start nieuwe partij}
translate N FixedLevel {Vast niveau}
translate N Opening {Opening}
translate N RandomLevel {Willekeurig niveau}
translate N StartFromCurrentPosition {Start vanaf de huidige stelling}
translate N FixedDepth {Vaste diepte}
translate N Nodes {Knooppunten} 
translate N Depth {Diepte}
translate N Time {Tijd} 
translate N SecondsPerMove {Seconden per zet}
translate N Engine {Engine}
translate N TimeMode {Tijd methode}
translate N TimeBonus {Tiid + bonus}
translate N TimeMin {min}
translate N TimeSec {sec}
 
translate N AllExercisesDone {Alle oefeningen gedaan}
 
translate N MoveOutOfBook {Zet buiten boek}
 
translate N LastBookMove {Laatste zet boek}
 
translate N AnnotateSeveralGames {Becommentarieer verschillende partijen \n van huidig tot :}
 
translate N FindOpeningErrors {Vind openingsfouten}
translate N MarkTacticalExercises {Markeer de tactische oefeningen}
 
translate N UseBook {Gebruik boek}
 
translate N MultiPV {Meerder varianten}
 
translate N Hash {Hash Geheugentabel}
 
translate N OwnBook {Gebruik engine boek}
 
translate N BookFile {Opening boek}
 
translate N AnnotateVariations {Becommentarieer varianten}
translate N ShortAnnotations {Korte commentaren}
translate N addAnnotatorTag {Voeg een commentaar label toe}
translate N AddScoreToShortAnnotations {Voeg de score toe aan de korte commentaren}
 
translate N Export {Export}
 
translate N BookPartiallyLoaded {Boek gedeeltelijk geladen}
translate N Calvar {Berekening van de varianten}
translate N ConfigureCalvar {Configuratie}
 
 
translate N Reti {Reti}
 
translate N English {Engels}
 
translate N d4Nf6Miscellaneous {1.d4 Nf6 Verscheidene}
 
translate N Trompowsky {Trompowsky}
 
translate N Budapest {Boedapest}
 
translate N OldIndian {Oud Indisch}
 
translate N BenkoGambit {Benko Gambiet}
 
translate N ModernBenoni {Moderne Benoni}
 
translate N DutchDefence {Dutch Defence}
 
translate N Scandinavian {Scandinavian}
 
translate N AlekhineDefence {Alechin Verdediging}
 
translate N Pirc {Pirc}
 
translate N CaroKann {Caro-Kann}
 
translate N CaroKannAdvance {Caro-Kann Doorschuif}
 
translate N Sicilian {Siciliaans}
 
translate N SicilianAlapin {Siciliaans Alapin}
 
translate N SicilianClosed {Siciliaans Gesloten}
 
translate N SicilianRauzer {Siciliaans Rauzer}
 
translate N SicilianDragon {Siciliaans Draak}
 
translate N SicilianScheveningen {Siciliaans Scheveningen}
 
translate N SicilianNajdorf {Siciliaans Najdorf}
 
translate N OpenGame {Open Spel}
 
translate N Vienna {Weens}
 
translate N KingsGambit {Konings Gambiet}
 
translate N RussianGame {Russisch}
 
translate N ItalianTwoKnights {Italiaans/Tweepaardenspel}
 
translate N Spanish {Spaans}
 
translate N SpanishExchange {Spaans Ruil}
 
translate N SpanishOpen {Spaans Open}
 
translate N SpanishClosed {Spaans Gesloten}
 
translate N FrenchDefence {Frans}
 
translate N FrenchAdvance {Frans Doorschuif}
 
translate N FrenchTarrasch {Frans Tarrasch}
 
translate N FrenchWinawer {Frans Winawer}
 
translate N FrenchExchange {Frans Ruil}
 
translate N QueensPawn {Damepion}
 
translate N Slav {Slavisch}
 
translate N QGA {Aangenomen Damegambiet}
 
translate N QGD {Damegambiet}
 
translate N QGDExchange {Damegambiet Ruil}
 
translate N SemiSlav {Half-Slavisch}
 
translate N QGDwithBg5 {Damegambiet met Lg5}
 
translate N QGDOrthodox {Damegambiet Orthodox}
 
translate N Grunfeld {Grünfeld}
 
translate N GrunfeldExchange {Grünfeld Ruil}
 
translate N GrunfeldRussian {Grünfeld Russisch}
 
translate N Catalan {Catalaans}
 
translate N CatalanOpen {Catalaans Open}
 
translate N CatalanClosed {Catalaans Gesloten}
 
translate N QueensIndian {Dameindisch}
 
translate N NimzoIndian {Nimzo-Indisch}
 
translate N NimzoIndianClassical {Nimzo-Indisch Klassiek Dc2}
 
translate N NimzoIndianRubinstein {Nimzo-Indisch Rubinstein}
 
translate N KingsIndian {Koningsindisch}
 
translate N KingsIndianSamisch {Koningsindisch Sämisch}

translate N KingsIndianMainLine {Koningsindisch Hoofdvariant}
translate N ConfigureFics {Configureer FICS}
translate N FICSGuest {Login als Gast}
translate N FICSServerPort {Server poort}
translate N FICSServerAddress {IP Addres}
translate N FICSRefresh {Ververs}
translate N FICSTimesealPort {Tijdsoverschrijdingspoort}
translate N FICSSilence {Stilte}
translate N FICSOffers {Biedt aan}
translate N FICSConsole {Console}
translate N FICSGames {Partiejen}
translate N FICSUnobserve {Stop gadeslaan partij}
translate N FICSProfile {Toon jouw geschiedenis en profiel}
translate N FICSRelayedGames {Doorgezonden partijen}
translate N FICSFindOpponent {Zoek opponent}
translate N FICSTakeback {Terugnemen}
translate N FICSTakeback2 {Terugnemen 2}
translate N FICSInitTime {Initiële tijd (min)}
translate N FICSIncrement {Toename (sec)}
translate N FICSRatedGame {De Betreffende Partij}
translate N FICSAutoColour {Automatisch}
translate N FICSManualConfirm {Bevestig manueel}
translate N FICSFilterFormula {Filter met fromule}
translate N FICSIssueSeek {Zoek uitdager}
translate N FICSChallenge {Uitdagen}
translate N FICSAccept {Aanvaard}
translate N FICSDecline {weiger}
translate N FICSColour {Kleur}
translate N FICSSend {Zend}
translate N FICSConnect {Verbind}
translate N FICSdefaultuservars {Gebruik standaard waarden}
translate N FICSObserveconfirm {Wil je de partij gadeslaan}
# ====== TODO To be translated ======
translate N FICSpremove {Enable premove}
translate N GameReview {Partij herbekijken}
translate N GameReviewTimeExtended {Tijd overschreden}
translate N GameReviewMargin {Fout marge}
translate N GameReviewAutoContinue {Auto vervolg als de zet juist is}
translate N GameReviewReCalculate {Gebruik verlengde tijd}
translate N GameReviewAnalyzingMovePlayedDuringTheGame {Analyseer de gespeelde zet tijdens de partij}
translate N GameReviewAnalyzingThePosition {Analyseer de stelling}
translate N GameReviewEnterYourMove {Voer je zet in}
translate N GameReviewCheckingYourMove {Controle van je zet}
translate N GameReviewYourMoveWasAnalyzed {Je zet werd geanalyseerd}
translate N GameReviewYouPlayedSameMove {Je speelde dezelfde zet als in de partij}
translate N GameReviewScoreOfYourMove {Score van je zet}
translate N GameReviewGameMoveScore {Partijzet score}
translate N GameReviewEngineScore {Score van engine}
translate N GameReviewYouPlayedLikeTheEngine {Je speelde zoals de engine}
translate N GameReviewNotEngineMoveButGoodMove {Geen engine zet maar ook goed}
translate N GameReviewMoveNotGood {Deze zet is niet goed, de score is}
translate N GameReviewMovesPlayedLike {Zetten gespeeld zoals}
translate N GameReviewMovesPlayedEngine {Zetten gespeeld zoals de engine}
translate N CCDlgConfigureWindowTitle {Configureer Correspondentie Schaak}
translate N CCDlgCGeneraloptions {Algemene Opties}
translate N CCDlgDefaultDB {Default Databank:}
translate N CCDlgInbox {Inbox (pad):}
translate N CCDlgOutbox {Outbox (pad):}
translate N CCDlgXfcc {Xfcc Configuratie:}
translate N CCDlgExternalProtocol {Externe Protocol Verwerker (b.v. Xfcc)}
translate N CCDlgFetchTool {Verwerf Tool:}
translate N CCDlgSendTool {Zend Tool:}
translate N CCDlgEmailCommunication {eMail Communicatie}
translate N CCDlgMailPrg {Mail programma:}
translate N CCDlgBCCAddr {(B)CC Address:}
translate N CCDlgMailerMode {Mode:}
translate N CCDlgThunderbirdEg {b.v. Thunderbird, Mozilla Mail, Icedove...}
translate N CCDlgMailUrlEg {b.v. Evolution}
translate N CCDlgClawsEg {b.v Sylpheed Claws}
translate N CCDlgmailxEg {b.v. mailx, mutt, nail...}
translate N CCDlgAttachementPar {Aanhangsel parameter:}
translate N CCDlgInternalXfcc {Gebruik interne Xfcc ondersteuning}
translate N CCDlgConfirmXfcc {Bevestig de zetten}
translate N CCDlgSubjectPar {Onderwerp parameter:}
translate N CCDlgDeleteBoxes {Lege In-/Uitbox}
translate N CCDlgDeleteBoxesText {Wil je echt de  In- and Uitbox mappen voor Correspondentieschaak ledigen? Dit vereist een nieuwe synchronisatie om de laatste staat van je partijen te tonen.}
translate N CCDlgConfirmMove {Bevestig zet}
translate N CCDlgConfirmMoveText {Als je bevestigt zal volgende zet en commentaar naar de server gestuurd worden:}
translate N CCDlgDBGameToLong {Onverenigbare Hoofdvariant}
translate N CCDlgDBGameToLongError {De hoofdvariant in jouw database is langer dan de partij in je Inbox. Als de Inbox huidige partijen bevat bvb. vlak na een synchronisatie, dan werden enkele verkeerde zetten toegevoegd aan de hoofdvariant.\nIn dit geval verkort de hoofdvariant tot (max) zetten\n}
translate N CCDlgStartEmail {Start een nieuwe eMail partij}
translate N CCDlgYourName {Uw Naam:}
translate N CCDlgYourMail {Uw eMail Addres:}
translate N CCDlgOpponentName {Naam van de tegenstander:}
translate N CCDlgOpponentMail {EMail Addres van de tegenstander:}
translate N CCDlgGameID {Game ID (uniek):}
translate N CCDlgTitNoOutbox {Scid: Correspondentie Schaak Outbox}
translate N CCDlgTitNoInbox {Scid: Correspondentie Schaak Inbox}
translate N CCDlgTitNoGames {Scid: Geen Correspondentie Schaak partijen}
translate N CCErrInboxDir {Correspondentie Schaak inbox folder:}
translate N CCErrOutboxDir {Correspondentie Schaak outbox folder:}
translate N CCErrDirNotUsable {dit bestaat niet of is niet toegankelijk!\nVerifieer en corrigeer de instellingen.}
translate N CCErrNoGames {dit bevat geen partijen!\nHaal ze alstublieft eerst op.}
translate N CCDlgTitNoCCDB {Scid: No Correspondentie Databank}
translate N CCErrNoCCDB {Er werd geen databank van type 'Correspondentie' geopend. U moet er eerst een openen alvorens de correspondentie schaak functies te gebruiken.}
translate N CCFetchBtn {Haal de partijen van de server en verwerk de Inbox}
translate N CCPrevBtn {Ga naar de vorige partij}
translate N CCNextBtn {Ga naar de volgende}
translate N CCSendBtn {Stuur de zet op}
translate N CCEmptyBtn {Ledig In- en Outbox}
translate N CCHelpBtn {Hulp over iconen en status indicatoren.\nVoor algemen hulp: druk F1!}
translate N CCDlgServerName {Server Naam:}
translate N CCDlgLoginName  {Login Naam:}
translate N CCDlgPassword   {Paswoord:}
# ====== TODO To be translated ======
translate N CCDlgShowPassword {Show password}
translate N CCDlgURL        {Xfcc-URL:}
translate N CCDlgRatingType {Klassering Type:}
translate N CCDlgDuplicateGame {Niet-unieke partij ID}
translate N CCDlgDuplicateGameError {This game exists more than once in your database. Please delete all duplicates and compact your game file (File/Maintenance/Compact Database).}
translate N CCDlgSortOption {Rangschikken:}
translate N CCDlgListOnlyOwnMove {De enige partijen waarvan ik de zet heb}
translate N CCOrderClassicTxt {Plaats, Wedstrijd, Ronde, Resultaat, Wit, Zwart}
translate N CCOrderMyTimeTxt {Mijn klok}
translate N CCOrderTimePerMoveTxt {Tijd per zet tot de volgende tijdscontrole}
translate N CCOrderStartDate {Start datum}
translate N CCOrderOppTimeTxt {Tegenstander Klok}
# ====== TODO To be translated ======
translate N CCDlgConfigRelay {Observe games}
# ====== TODO To be translated ======
translate N CCDlgConfigRelayHelp {Go to the games page on http://www.iccf-webchess.com and display the game to be observed.  If you see the chessboard copy the URL from your browser to the list below. One URL per line only!\nExample: http://www.iccf-webchess.com/MakeAMove.aspx?id=266452}
translate N ExtHWConfigConnection {Configureer externe hardware}
translate N ExtHWPort {Poort}
translate N ExtHWEngineCmd {Engine commando}
translate N ExtHWEngineParam {Engine parameters}
translate N ExtHWShowButton {Toon de knop}
translate N ExtHWHardware {Hardware}
translate N ExtHWNovag {Novag Citrine}
translate N ExtHWInputEngine {Invoer Engine}
translate N ExtHWNoBoard {Geen bord}
translate N NovagReferee {Scheidsrechter}
translate N IEConsole {Invoer Engine Console}
translate N IESending {Zetten verzonden voor}
translate N IESynchronise {Synchroniseer}
translate N IERotate  {Roteer}
translate N IEUnableToStart {De Input engine start niet:}
translate N DoneWithPosition {Klaar met deze stelling}
translate N Board {Bord}
translate N showGameInfo {Toon de partij informatie}
translate N autoResizeBoard {Verander bord automatisch van grootte}
translate N DockTop {Zend naar eeste plaats}
translate N DockBottom {Zend naar laatste plaats}
translate N DockLeft {Zend naar links}
translate N DockRight {Zend naar rechts}
translate N Undock {Undock}
translate N ChangeIcon {Verander ikoon...}
# ====== TODO To be translated ======
translate N NewGameListWindow {New Game List Window}
# ====== TODO To be translated ======
translate N LoadatStartup {Load at startup}
# ====== TODO To be translated ======
translate N ShowHideDB {Show/Hide databases}
# ====== TODO To be translated ======
translate N ChangeFilter {Change filter}
# ====== TODO To be translated ======
translate N ChangeLayout {Load/Save/Change sorting criteria and column layout}
# ====== TODO To be translated ======
translate N ShowHideStatistic {Show/Hide statistics}
# ====== TODO To be translated ======
translate N BoardFilter {Show only games that matches the current board position}
# ====== TODO To be translated ======
translate N CopyGameTo {Copy Game to}
# ====== TODO To be translated ======
translate N FindBar {Find Bar}
# ====== TODO To be translated ======
translate N FindCurrentGame {Find current game}
# ====== TODO To be translated ======
translate N DeleteGame {Delete game}
# ====== TODO To be translated ======
translate N UndeleteGame {Undelete game}
# ====== TODO To be translated ======
translate N ResetSort {Reset sort}
# ====== TODO To be translated ======
translate N ConvertNullMove {Convert null moves to comments}
# ====== TODO To be translated ======
translate N SetupBoard {Setup Board}
# ====== TODO To be translated ======
translate N Rotate {Rotate}
# ====== TODO To be translated ======
translate N SwitchColors {Switch colors}
# ====== TODO To be translated ======
translate N FlipBoard {Flip Board}
# ====== TODO To be translated ======
translate N ImportPGN {Import PGN game}
# ====== TODO To be translated ======
translate N ImportingFiles {Importing PGN files in}
# ====== TODO To be translated ======
translate N ImportingFrom {Importing from}
# ====== TODO To be translated ======
translate N ImportingIn {Import games in}
# ====== TODO To be translated ======
translate N UseLastTag {Use last\ngame's tags}
# ====== TODO To be translated ======
translate N Random {Random}
# ====== TODO To be translated ======
translate N BackToMainline {Go back to mainline}
# ====== TODO To be translated ======
translate N LeaveVariant {Leave variant}
# ====== TODO To be translated ======
translate N Autoplay {Autoplay}
# ====== TODO To be translated ======
translate N ShowHideCoords {Show/Hide Coord.}
# ====== TODO To be translated ======
translate N ShowHideMaterial {Show/Hide Material}
# ====== TODO To be translated ======
translate N FullScreen {Full Screen}
# ====== TODO To be translated ======
translate N FilterStatistic {Filter Statistics}
# ====== TODO To be translated ======
translate N MakeCorrections {Make Corrections}
# ====== TODO To be translated ======
translate N Surnames {Surnames}
# ====== TODO To be translated ======
translate N Ambiguous {Ambiguous}
translate N OptionsToolbar "Gereedschappenbalk"
# ====== TODO To be translated ======
translate N OptionsBoard "Bord"
# ====== TODO To be translated ======
translate N OptionsMenuColor "Menu colors"
translate N OptionsBoardSize "Grootte van bord"
translate N OptionsBoardPieces "Bord Stukken Stijl"
# ====== TODO To be translated ======
translate N OptionsInternationalization "Internationalization"
# ====== TODO To be translated ======
translate N MenuColorForeground "Textcolor"
# ====== TODO To be translated ======
translate N MenuColorBackground "Background"
# ====== TODO To be translated ======
translate N MenuColorSelect "Select"
translate N OptionsTablebaseDir "Selecteer tot 4 tabelbasismappen:"
}
# End of nederlan.tcl
