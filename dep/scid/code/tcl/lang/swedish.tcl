# swedish.tcl:
# Text for menu names and status bar help messages in Swedish.
# Part of Scid (Shane's Chess Information Database).
# Contributed by Martin Skjöldebrand, martin@skjoldebrand.org
# Thanks to: Hans Eriksson, for looking over the translation file

proc setLanguage_W {} {

# File menu:
menuText W File "Arkiv" 0
menuText W FileNew "Ny..." 0 {Skapa en ny Scid databas}
menuText W FileOpen "Öppna..." 0 {Öppna en befintlig Scid databas}
menuText W FileClose "Stäng" 0 {Stäng den aktiva Scid databasen}
menuText W FileFinder "Sök filer" 4 {Öppna sökdialogen}
menuText W FileBookmarks "Bokmärken" 0 {Hantera bokmärken (kortkommando: Ctrl+B)}
menuText W FileBookmarksAdd "Nytt bokmärke" 0 \
  {Markera den aktiva ställningen i partiet}
menuText W FileBookmarksFile "Spara bokmärke" 0 \
  {Spara bokmärket för den aktiva ställningen i partiet}
menuText W FileBookmarksEdit "Redigera bokmärken..." 0 \
  {Redigera bokmärken}
menuText W FileBookmarksList "Visa bokmärken som lista" 19 \
  {Visar bokmärkena som lista, inte undermenyer}
menuText W FileBookmarksSub "Visa bokmärken i undermenyer" 17 \
  {Visar bokmärkena som undermenyer, inte lista}
menuText W FileMaint "Databasverktyg" 0 {Scids databasverktyg}
menuText W FileMaintWin "Verktygsfönster" 0 \
  {Öppna/ stäng verktygsfönstret}
menuText W FileMaintCompact "Komprimera databasen..." 0 \
  {Komprimera databasen, avlägsna raderade partier och oanvända namn}
menuText W FileMaintClass "Klassificera partier enligt ECO..." 2 \
  {Klassificera om alla partier enligt ECO-systemet}
menuText W FileMaintSort "Sortera databasen..." 0 \
  {Sortera partierna i den aktiva databasen}
menuText W FileMaintDelete "Radera dubbletter..." 0 \
  {Sök dubbletter och markera dem som raderingsbara}
menuText W FileMaintTwin "Sök dubbletter" 0 \
  {Öppna/ stäng dubblettfönstret för att söka dubblettpartier}
menuText W FileMaintName "Stavningskontroll" 0 {Namnredigering och stavningskontroll}
menuText W FileMaintNameEditor "Redigera namn" 0 \
  {Redigera spelarnamn utifrån rättstavningsfilen}
menuText W FileMaintNamePlayer "Stavningskontrollera namn..." 22 \
  {Stavningskontrollera namn utifrån rättstavningsfilen}
menuText W FileMaintNameEvent "Stavningskontrollera evenemang..." 21 \
  {Stavningskontrollera evenemang utifrån rättstavningsfilen}
menuText W FileMaintNameSite "Stavningskontrollera platser..." 21 \
  {Stavningskontrollera platser utifrån rättstavningsfilen}
menuText W FileMaintNameRound "Stavningskontrollera ronder..." 21 \
  {Stavningskontrollera ronder utifrån rättstavningsfilen}
menuText W FileReadOnly "Enbart läsbar..." 8 \
  {Avlägsna temporärt redigeringsmöjligheterna till databasen}
menuText W FileSwitch "Byt databas" 0 \
  {Byt till en annan öppnad databas} 
menuText W FileExit "Avsluta" 0 {Avsluta Scid}
menuText W FileMaintFixBase "Fixa trasig databas" 0 {Försök att fixa en trasig databas}

# Edit menu:
menuText W Edit "Redigera" 0
menuText W EditAdd "Lägg till variant" 0 {Skapa en variant vid denna ställning}
menuText W EditDelete "Radera variant" 0 {Radera en variant vid denna ställning}
menuText W EditFirst "Skapa huvudvariant" 6 \
  {Gör en av varianterna till huvudvariant}
menuText W EditMain "Skapa nytt textdrag" 11 \
  {Gör en av varianterna till partifortsättning}
menuText W EditTrial "Testa variant" 6 \
  {Starta/ stoppa testläge, undersök en idé på brädet}
menuText W EditStrip "Ta bort" 3 {Avlägsna kommentarer eller varianter från partiet}
menuText W EditUndo "Ångra" 0 {Ångra senaste ändring i parti}
# ====== TODO To be translated ======
menuText W EditRedo "Redo" 0 {Redo last game change}
menuText W EditStripComments "Kommentarer" 0 \
  {Avlägsna alla kommentarer och noteringar från partiet}
menuText W EditStripVars "Varianter" 0 {Avlägsna alla varianter från partiet}
menuText W EditStripBegin "Avlägsna tidigare drag" 9 \
  {Avlägsna dragen fram till den aktuella ställningen} 
menuText W EditStripEnd "Avlägsna resterande drag" 0 \
  {Avlägsna partiets resterande drag} 
menuText W EditReset "Töm Clipbase" 1 \
  {Tömmer den temporära databasen}
menuText W EditCopy "Kopiera partiet till Clipbase" 21 \
  {Kopierar det aktuella partiet till Clipbase}
menuText W EditPaste "Klistra in det senaste Clipbasepartiet" 8 \
  {Klistrar in det senaste tillagda i Clipbase i den aktiva databasen}
menuText W EditPastePGN "Klistra in Clipbasetext som PGNparti..." 10 \
  {Tolka Clipbasetexten som ett parti i PGN notation och klistra in det här}
menuText W EditSetup "Skapa ställning..." 0 \
  {Skapa en utgångsställning för aktuellt parti}
menuText W EditCopyBoard "Kopiera ställning" 0 \
  {Kopiera den aktuella ställningen i FEN kod till urklippshanteraren} 
menuText W EditPasteBoard "Klistra in utgångsställning" 10 \
  {Klistra in ställningen från aktuellt parti i den temporära databasen}
# ====== TODO To be translated ======
menuText W ConfigureScid "Preferences..." 0 {Configure all options for SCID}

# Game menu:
menuText W Game "Partier" 0
menuText W GameNew "Nytt parti" 0 \
  {Återställ brädet inför ett nytt parti (raderar alla ändringar)}
menuText W GameFirst "Ladda det första partiet" 10 {Laddar det första partiet i filtret}
menuText W GamePrev "Ladda föregående parti" 7 {Ladda föregående parti i filtret}
menuText W GameReload "Börja om partiet" 0 \
  {Återställ partiet (raderar alla ändringar)}
menuText W GameNext "Ladda nästa parti" 6 {Ladda nästa parti i filtret}
menuText W GameLast "Ladda det sista partiet" 10 {Ladda det sista partiet i filtret}
menuText W GameRandom "Ladda parti slumpmässigt" 14 \
  {Ladda ett av datorn slumpmässigt valt parti} 
menuText W GameNumber "Ladda parti nummer..." 6 \
  {Ladda ett parti genom att ange dess nummer}
menuText W GameReplace "Spara: Ersätt parti..." 7 \
  {Spara partiet och ersätt tidigare version}
menuText W GameAdd "Spara: Nytt parti..." 1 \
  {Spara ett nytt parti}
menuText W GameDeepest "Identifiera öppningen" 0 \
  {Gå till den mest detaljerade ställningen i ECO boken}
menuText W GameGotoMove "Gå till drag nummer..." 8 \
  {Gå till ett specifikt drag i partiet}
menuText W GameNovelty "Hitta nyhet..." 7 \
  {Hitta det första draget i partiet som inte spelats tidigare}

# Search Menu:
menuText W Search "Sök" 0
menuText W SearchReset "Återställ sökfilter" 0 {Återställ sökfiltret så att alla partiet ingår}
menuText W SearchNegate "Omvänt filter" 0 {Ta med partier som utesluts av filtret}
menuText W SearchCurrent "Aktuell position..." 8 {Sök partier med aktuell position på brädet}
menuText W SearchHeader "I huvud..." 2 {Använd fast information (spelare, evenemang, plats, mm)}
menuText W SearchMaterial "Material/ställning..." 0 {Sökning baserad på material eller ställning}
menuText W SearchUsing "Använd sökfil..." 10 {Använd en fil med lagrade sökvillkor}

# Windows menu:
menuText W Windows "Fönster" 0
menuText W WindowsComment "Kommentarseditor" 0 {Öppna/ stäng kommentarseditorn}
menuText W WindowsGList "Partilista" 5 {Öppna/ stäng partilistan}
menuText W WindowsPGN "PGN fönster" 0 \
  {Öppna/ stäng PGN fönstret}
menuText W WindowsPList "Spelarförteckning" 7 {Öppna/ stäng en förteckning över spelarna i den aktiva databasen} 
menuText W WindowsTmt "Turneringar" 0 {Lista turneringar}
menuText W WindowsSwitcher "Databasväxlaren" 0 \
  {Öppna/ stäng databasväxlaren}
menuText W WindowsMaint "Verktygsfönster" 0 \
  {Öppna/ stäng verktygsfönstret}
menuText W WindowsECO "ECO fönster" 0 {Öppna/ stäng ECO bläddraren}
menuText W WindowsStats "Statistikfönster" 0 \
  {Öppna/ stäng statistikfönstret}
menuText W WindowsTree "Trädfönster" 2 {Öppna/ stäng variantträdets fönster}
menuText W WindowsTB "Slutspelsdatabas" 2 \
  {Öppna/ stäng slutspelsdatabasfönstret}
menuText W WindowsBook "Bokfönster" 0 {Öppna/stäng Bokfönstret}
menuText W WindowsCorrChess "Korrespondensfönster" 0 {Öppna/stäng Korrespondensfönstret}

# Tools menu:
menuText W Tools "Verktyg" 0
menuText W ToolsAnalysis "Analysmotor..." 6 \
  {Starta/ stoppa en analysmotor}
menuText W ToolsAnalysis2 "Analysmotor 2..." 12 \
  {Starta/ stoppa en andra analysmotor}
menuText W ToolsCross "Resultattabell" 0 {Visa en resultattabell för den aktuella turneringen}
menuText W ToolsEmail "Eposthanterare" 0 \
  {Öppna/ stäng eposthanteraren}
menuText W ToolsFilterGraph "Filterdiagram" 7 \
  {Öppna/ stäng filterdiagramfönstret} 
menuText W ToolsAbsFilterGraph "Absolut Filtergraf" 7 {Öppna/stäng filtergraffönstret för absolutvärden}
menuText W ToolsOpReport "Öppningsrapport" 0 \
  {Skapa en öppningsrapport utifrån den aktuella ställningen}
menuText W ToolsOpenBaseAsTree "Öppna databas som träd" 0   {Öppna en databas och använd den i Trädfönstret}
menuText W ToolsOpenRecentBaseAsTree "Öppna senaste databasen som träd" 0   {Öppna den senaste databasen och använd den i Trädfönstret}
menuText W ToolsTracker "Sök material"  0 {Öppnar dialog för att söka efter en viss materiell balans} 
menuText W ToolsTraining "Träning"  0 {Träningsverktyg (taktik, öppningar,...) }
menuText W ToolsTacticalGame "Taktiskt parti"  0 {Spela ett parti med taktik}
menuText W ToolsSeriousGame "Seriöst parti"  0 {Spela ett seriöst parti}
menuText W ToolsTrainOpenings "Öppningar"  0 {Träna med en repetoar}
menuText W ToolsTrainReviewGame "Granska parti"  0 {Gissa drag spelade i ett parti}
menuText W ToolsTrainTactics "Taktik"  0 {Lösa taktik}
menuText W ToolsTrainCalvar "Variantberäkning"  0 {Variantberäkningsträning}
menuText W ToolsTrainFindBestMove "Hitta bäst drag"  0 {Hitta bästa draget}
menuText W ToolsTrainFics "Spela på Internet"  0 {Spela på freechess.org}
# ====== TODO To be translated ======
menuText W ToolsEngineTournament "Engine tournament"  0 {Start a tournament between chess engines}
menuText W ToolsBookTuning "Bokfininställning" 0 {Bokfininställning}
menuText W ToolsConnectHardware "Anslut Hårdvara" 0 {Anslut extern hårdvara}
menuText W ToolsConnectHardwareConfigure  "Konfiguration" 0 {Konfiguration}
menuText W ToolsConnectHardwareNovagCitrineConnect "Anslut Novag Citrine" 0 {Anslut Novag Citrine}
menuText W ToolsConnectHardwareInputEngineConnect "Anslut Inmatningsschackmotor" 0 {Anslut Inmatningsschackmotor (t.ex. DGT)}
menuText W ToolsPInfo "Spelarinformation"  0 \
  {Öppna/ uppdatera spelarinformation}
menuText W ToolsPlayerReport "Spelarrapport..." 3 \
  {Skapa en spelarrapport}
menuText W ToolsRating "Ratingdiagram" 0 \
  {Skapa ett ratingdiagram för spelarna i partiet}
menuText W ToolsScore "Resultatdiagram" 8 {Visa resultatdiagrammet}
menuText W ToolsExpCurrent "Exportera aktuellt parti" 1 \
  {Spara aktuellt parti till en textfil}
menuText W ToolsExpCurrentPGN "Exportera till PGN..." 15 \
  {Spara aktuellt parti till en PGN-fil}
menuText W ToolsExpCurrentHTML "Exportera till HTML..." 15 \
  {Spara aktuellt parti till en HTML-fil}
menuText W ToolsExpCurrentHTMLJS "Exportera till HTML och JavaScript-fil..." 15 {Spara aktuellt parti till en HTML och JavaScript-fil}  
menuText W ToolsExpCurrentLaTeX "Exportera till LaTeX..." 15 \
  {Spara aktuellt parti till en LaTeX-fil}
menuText W ToolsExpFilter "Exportera alla filtrerade partier" 15 \
  {Spara alla filterade partier till en textfil}
menuText W ToolsExpFilterPGN "Exportera till PGN..." 15 \
  {Spara alla filterade partier till en PGN-fil}
menuText W ToolsExpFilterHTML "Exportera till HTML..." 15 \
  {Spara alla filterade partier till en HTML-fil}
menuText W ToolsExpFilterHTMLJS "Exportera filtrerade till HTML och JavaScript-fil..." 17 {Spara alla filtrerade partier till en HTML och JavaScript-fil}  
menuText W ToolsExpFilterLaTeX "Exportera till LaTeX..." 15 \
  {Spara alla filterade partier till en LaTeX-fil}
menuText W ToolsImportOne "Importera ett parti i PGN-format..." 0 \
  {Importera ett parti i PGN-format}
menuText W ToolsImportFile "Importera flera partier i PGN-format..." 16 \
  {Importera flera partier i PGN-format från en fil}
menuText W ToolsStartEngine1 "Starta schackmotor 1" 0  {Starta schackmotor 1}
menuText W ToolsStartEngine2 "Starta schackmotor 2" 0  {Starta schackmotor 2}
menuText W ToolsCaptureBoard "Spara aktuellt schackbräde..." 0  {Spara det aktuella schackbrädet som en bild.}
menuText W Play "Spela" 0
menuText W CorrespondenceChess "Korrespondensschack" 0 {Funktioner för eMail och Xfcc baserad Korrespondensschack}
menuText W CCConfigure "Konfigurera..." 0 {Konfigurera externa verktyg och generella inställningar}
menuText W CCConfigRelay "Konfigurera observationer..." 10 {Konfigurera partier att observera}
menuText W CCOpenDB "Öppna Databas..." 0 {Öppna standardkorrespondensdatabasen}
menuText W CCRetrieve "Hämta Partier" 0 {Hämta partier via extern (Xfcc-)hjälpare}
menuText W CCInbox "Hantera Inkorg" 0 {Hantera alla filer i Scids Inkorg}
menuText W CCSend "Skicka Drag" 0 {Skicka ditt drag via eMail eller extern (Xfcc-)hjälpare}
menuText W CCResign "Ge upp" 0 {Ge upp (inte via eMail)}
menuText W CCClaimDraw "Hävda Remi" 0 {Skicka drag och hävda Remi (inte via eMail)}
menuText W CCOfferDraw "Erbjud Remi" 0 {Skicka drag och erbjud Remi (inte via eMail)}
menuText W CCAcceptDraw "Acceptera Remi" 0 {Acceptera en erbjuden Remi (inte via eMail)}
menuText W CCNewMailGame "Nytt eMail-parti..." 0 {Starta ett nytt eMail-parti}
menuText W CCMailMove "Skicka Drag..." 0 {Skicka draget via eMail till motståndaren}
menuText W CCGamePage "Partisida..." 0 {Starta upp partiet via webläsaren}
menuText W CCEditCopy "Kopiera partilista till Clipbase" 0 {Kopiera partierna som CSV-lista till Clipbase}

# Options menu:
menuText W Options "Alternativ" 2
menuText W OptionsBoardGraphics "Rutor..." 0 {Välj texturer för rutor}
translate W OptionsBGW {Välj textur för rutor}
translate W OptionsBoardGraphicsText {Välj grafikfiler för vita och svarta rutor:}
menuText W OptionsBoardNames "Spelarnamn..." 0 {Redigera spelares namn}
menuText W OptionsExport "Export" 0 {Ändra exportalternativ}
menuText W OptionsFonts "Typsnitt" 0 {Ändra typsnitt}
menuText W OptionsFontsRegular "Normal" 0 {Ändra det normala typsnittet}
menuText W OptionsFontsMenu "Menu" 0 {Ändra menytypsnittet}
menuText W OptionsFontsSmall "Liten" 0 {Ändra det lilla typsnittet}
menuText W OptionsFontsTiny "Tiny" 0 {Change the tiny font}
menuText W OptionsFontsFixed "Fixerad" 0 {Ändra det fixerade typsnittet}
menuText W OptionsGInfo "Partiinformation" 0 {Alternativ för partiinformation}
menuText W OptionsLanguage "Språk" 0 {Välj språk}
menuText W OptionsMovesTranslatePieces "Översätt pjäser" 0 {Översätt första bokstaven för pjäser}
menuText W OptionsMovesHighlightLastMove "Markera senaste draget" 0 {Markera senaste draget}
menuText W OptionsMovesHighlightLastMoveDisplay "Visa" 0 {Visa senaste markerade draget}
menuText W OptionsMovesHighlightLastMoveWidth "Bredd" 0 {Tjockhet på rad}
menuText W OptionsMovesHighlightLastMoveColor "Färg" 0 {Färg på rad}
# ====== TODO To be translated ======
menuText W OptionsMovesHighlightLastMoveArrow "include Arrow" 0 {Include Arrow with Highlight}
menuText W OptionsMoves "Drag" 0 {Alternativ för dragangivelse}
menuText W OptionsMovesAnimate "Fördröjning vid manuellt spel" 1 \
  {Ange tid mellan varje drag när dragen görs automatiskt}
menuText W OptionsMovesDelay "Fördröjning vid automatspel..." 1 \
  {Ange fördröjning mellan dragen när datorn spelar själv}
menuText W OptionsMovesCoord "Koordinater" 0 \
  {Acceptera koordinater ("g1f3") vid dragangivelse}
menuText W OptionsMovesSuggest "Visa föreslagna drag" 0 \
  {Visa/ Dölj föreslagna drag}
menuText W OptionsShowVarPopup "Visa Variantfönster" 0 {Starta/Avsluta visningen av ett Variantfönster}  
menuText W OptionsMovesSpace "Lägg till mellanslag efter dragnummer" 0 {Lägg till mellanslag efter dragnummer}  
menuText W OptionsMovesKey "Tangentkomplettering" 0 \
  {Starta/ stäng av dragkomplettering vid tangentinmatning}
menuText W OptionsMovesShowVarArrows "Visa pilar för variationer" 0 {Växlar på/av pilar som visar drag i variationer}
# ====== TODO To be translated ======
menuText W OptionsMovesGlossOfDanger "Color Coded Gloss of Danger" 0 {Turn on/off color coded gloss of danger}
menuText W OptionsNumbers "Talformat" 3 {Välj hur tal visas}
menuText W OptionsTheme "Tema" 0 {Ändra utseende på gränssnitt}
menuText W OptionsWindows "Fönster" 1 {Fönsteralternativ}
menuText W OptionsSounds "Ljud" 2 {Konfigurera ljud för att annonsera drag}
# ====== TODO To be translated ======
menuText W OptionsResources "Resources..." 0 {Choose resource files and folders}
menuText W OptionsWindowsDock "Dockningsfönster" 0 {Dockningsfönster}
menuText W OptionsWindowsSaveLayout "Spara layout" 0 {Sparar layout}
menuText W OptionsWindowsRestoreLayout "Återställ layout" 0 {Återställer layout}
menuText W OptionsWindowsShowGameInfo "Visa partiinformation" 0 {Visar partiinformation}
menuText W OptionsWindowsAutoLoadLayout "Automatiskt öppna första layouten" 0 {Öppnar automatiskt första layouten vid uppstart}
menuText W OptionsECO "Ladda ECO fil" 7 {Ladda ECO-klassificeringen vid start}
menuText W OptionsSpell "Ladda Rättstavningsfil" 7 \
  {Ladda rättstavningsfilen vid start}
menuText W OptionsTable "Katalog för slutspelsdatabaser" 0 \
  {Välj en fil som innehåller en slutspelsdatabas; alla övriga filer i samma katalog kommer att användas}
menuText W OptionsRecent "Senast använda filer..." 16 \
  {Ãndra antalet senast använda filer som visas i Arkivmenyn} 
menuText W OptionsBooksDir "Öppningsbokskatalog" 0 {Anger Öppningsbokskatalogen}
menuText W OptionsTacticsBasesDir "Taktikbaskatalog" 0 {Anger Taktikbaskatalog (för träning)}
# ====== TODO To be translated ======
menuText W OptionsPhotosDir "Photos directory" 0 {Sets the photos bases directory}
# ====== TODO To be translated ======
menuText W OptionsThemeDir "Theme(s) File:"  0 { Load a GUI theme package file }
menuText W OptionsSave "Spara alternativ" 7 \
  "Spara alla alternativ till en inställningsfil"
menuText W OptionsAutoSave "Autospara vid avslut" 1 \
  {Spara alla alternativ när du avslutar Scid}

# Help menu:
menuText W Help "Hjälp" 0
menuText W HelpContents "Innehåll" 0 {Visa innehåll} 
menuText W HelpIndex "Index" 0 {Hjälpsystemets indexsida}
menuText W HelpGuide "Snabbguide" 0 {Visa snabbguiden}
menuText W HelpHints "Tips" 0 {Visa tips}
menuText W HelpContact "Kontaktinformation" 0 {Visa kontaktinformation}
menuText W HelpTip "Dagens tips" 0 {Användbara Scid tips}
menuText W HelpStartup "Startfönster" 5 {Visa startfönstret}
menuText W HelpAbout "Om Scid" 0 {Information om Scid}

# Game info box popup menu:
menuText W GInfoHideNext "Dölj nästa drag" 0
menuText W GInfoMaterial "Visa materialvärden" 0
menuText W GInfoFEN "Visa FEN" 5
menuText W GInfoMarks "Visa färgade fält och pilar" 22 
menuText W GInfoWrap "Radbrytning" 0
menuText W GInfoFullComment "Visa fullständiga kommentarer" 18
menuText W GInfoPhotos "Visa bilder" 5 ;
menuText W GInfoTBNothing "Slutspelsdatabaser: inget" 20
menuText W GInfoTBResult "Slutspelsdatabaser: endast resultat" 28
menuText W GInfoTBAll "Slutspelsdatabaser: resultat och bästa drag" 33
menuText W GInfoDelete "Återta/Radera detta parti" 0
menuText W GInfoMark "(Av-)Markera detta parti" 5
menuText W GInfoInformant "Konfigurera Informant-parametrar" 0

# General buttons:
translate W Back {Tillbaka}
translate W Browse {Bläddra}
translate W Cancel {Avbryt}
translate W Continue {Fortsätt}
translate W Clear {Rensa}
translate W Close {Stäng}
translate W Contents {Innehåll}
translate W Defaults {Standard}
translate W Delete {Radera}
translate W Graph {Diagram}
translate W Help {Hjälp}
translate W Import {Importera}
translate W Index {Index}
translate W LoadGame {Ladda parti}
translate W BrowseGame {Bläddra genom partier}
translate W MergeGame {Slå samman parti}
translate W MergeGames {Slå samman partier}
translate W Preview {Förhandsgranska}
translate W Revert {Ångra inmatning}
translate W Save {Spara}
translate W Search {Sök}
translate W Stop {Stoppa}
translate W Store {Spara}
translate W Update {Uppdatera}
translate W ChangeOrient {Ãndra fönstrets orientering}
translate W ShowIcons {Visa Ikoner} ;# ***
translate W None {Ingen}
translate W First {Första}
translate W Current {Aktuella}
translate W Last {Sista}

# General messages:
translate W game {parti}
translate W games {partier}
translate W move {drag}
translate W moves {drag}
translate W all {alla}
translate W Yes {Ja}
translate W No {Nej}
translate W Both {Båda}
translate W King {Kung}
translate W Queen {Dam}
translate W Rook {Torn}
translate W Bishop {Löpare}
translate W Knight {Springare}
translate W Pawn {Bonde}
translate W White {Vit}
translate W Black {Svart}
translate W Player {Spelare}
translate W Rating {Rating}
translate W RatingDiff {Ratingskillnad (Vit - Svart)}
translate W AverageRating {Medelrating}
translate W Event {Evenemang}
translate W Site {Plats}
translate W Country {Land}
translate W IgnoreColors {Ignorera färger}
translate W Date {Datum}
translate W EventDate {Evenemangsdatum}
translate W Decade {Decennium}
translate W Year {År}
translate W Month {Månad}
translate W Months {Januari Februari Mars April Maj Juni Juli Augusti September Oktober November December}
translate W Days {Sön Mån Tis Ons Tor Fre Lör}
translate W YearToToday {Idag}
# ====== TODO To be translated ======
translate W YearToTodayTooltip {Set date from 1 year back to today}
translate W Result {Resultat}
translate W Round {Rond}
translate W Length {Längd}
translate W ECOCode {ECO kod}
translate W ECO {ECO}
translate W Deleted {Raderad}
translate W SearchResults {Sökresultat}
translate W OpeningTheDatabase {Öppnar databas}
translate W Database {Databas}
translate W Filter {Filter}
translate W noGames {inga partier}
translate W allGames {alla partier}
translate W empty {tom}
translate W clipbase {Clipbase}
translate W score {resultat}
translate W StartPos {Utgångsställning}
translate W Total {Totalt}
translate W readonly {bara läsbar} ;# ***

# Standard error messages:
translate W ErrNotOpen {Databasen är inte öppen.}
translate W ErrReadOnly {Databasen är skrivskyddad. Du kan inte ändra i den.}
translate W ErrSearchInterrupted {Sökningen avbröts; resultatet är inte fullständigt.}

# Game information:
translate W twin {dubblett}
translate W deleted {raderad}
translate W comment {kommentar}
translate W hidden {dold}
translate W LastMove {Senaste draget}
translate W NextMove {Nästa}
translate W GameStart {Utgångsställning}
translate W LineStart {Varianten börjar}
translate W GameEnd {Slutställning}
translate W LineEnd {Varianten slut}

# Player information:
translate W PInfoAll {Resultat för <b>alla</b> partier}
translate W PInfoFilter {Resultat för <b>filtrerade</b> partier}
translate W PInfoAgainst {Resultat mot}
translate W PInfoMostWhite {De vanligaste öppningarna som vit}
translate W PInfoMostBlack {De vanligaste öppningarna som svart}
translate W PInfoRating {Ratinghistoria}
translate W PInfoBio {Biografisk information}
translate W PInfoEditRatings {Redigera rating} 
# ====== TODO To be translated ======
translate W PInfoEloFile {File}

# Tablebase information:
translate W Draw {Remi}
translate W stalemate {patt}
translate W withAllMoves {med alla drag}
translate W withAllButOneMove {med alla drag utom ett}
translate W with {med}
translate W only {bara}
translate W lose {förlust}
translate W loses {förluster}
translate W allOthersLose {alla andra drag förlorar}
translate W matesIn {med matt i}
translate W hasCheckmated {har satt matt}
translate W longest {längst}
translate W WinningMoves {Vinstdrag} 
translate W DrawingMoves {Remidrag} 
translate W LosingMoves {Förlustdrag} 
translate W UnknownMoves {Okänt resultat} 
# ====== TODO To be translated ======
translate W SideLongest {Side    Longest    %     %     %\n}
# ====== TODO To be translated ======
translate W toNoveMate {to move   mate    Win  Draw  Loss\n}
# ====== TODO To be translated ======
translate W NoSummary {\nNo summary for this tablebase.}
# ====== TODO To be translated ======
translate W MutualZugzwang {Mutual zugzwangs: }
# ====== TODO To be translated ======
translate W SelectetZugzwang {\nSelected zugzwang positions:}
# ====== TODO To be translated ======
translate W WdrawBloss {White draws, Black loses:}
# ====== TODO To be translated ======
translate W BdrawWloss {Black draws, White loses:}
# ====== TODO To be translated ======
translate W AllLoss {" Whoever moves loses:     " }

# Tip of the day:
translate W Tip {Tips}
translate W TipAtStartup {Tips vid start}

# Tree window menus:
menuText W TreeFile "Fil" 0
menuText W TreeFileFillWithBase "Fyll Cache med databas" 0 {Fyll Cachefilen med alla partier i den aktuella databasen}
menuText W TreeFileFillWithGame "Fyll Cache med parti" 0 {Fyll Cachefilen med aktuellt parti i den aktuella databasen}
menuText W TreeFileSetCacheSize "Cachestorlek" 0 {Ange Cachestorleken}
menuText W TreeFileCacheInfo "Cacheinformation" 0 {Få information om Cacheanvändning}
menuText W TreeFileSave "Spara cachefil" 0 {Spara trädcache (*.stc)-filen}
menuText W TreeFileFill "Fyll cachefil" 0 \
 {Fyll cachefilen med vanliga öppningar}
menuText W TreeFileBest "Lista bästa partier" 0 {Visa lista över de bästa partierna i trädet}
menuText W TreeFileGraph "Diagramfönster" 0 {Visa diagrammet för denna gren i trädet}
menuText W TreeFileCopy "Kopiera träd till urklipp" 1 \
  {Kopierar trädrelaterad statistik till urklipp}
menuText W TreeFileClose "Stäng trädfönstret" 0 {Stäng trädfönstret}
menuText W TreeMask "Sökmask" 0
menuText W TreeMaskNew "Ny" 0 {Ny sökmask}
menuText W TreeMaskOpen "Öppna" 0 {Öppna sökmask}
menuText W TreeMaskOpenRecent "Öppna senaste" 0 {Öppna senaste sökmasken}
menuText W TreeMaskSave "Spara" 0 {Spara sökmask}
menuText W TreeMaskClose "Stäng" 0 {Stäng sökmask}
menuText W TreeMaskFillWithGame "Fyll med parti" 0 {Fyll sökmask med parti}
menuText W TreeMaskFillWithBase "Fyll med databas" 0 {Fyll sökmask med alla partier i databasen}
menuText W TreeMaskInfo "Info om sökmask" 0 {Visa statistik för aktuell sökmask}
menuText W TreeMaskDisplay "Visa sökmaskkarta" 0 {Visa sökmaskdata i trädform}
menuText W TreeMaskSearch "Sök" 0 {Sök i aktuell sökmask}
menuText W TreeSort "Sortera" 0
menuText W TreeSortAlpha "Alfabetiskt" 0
menuText W TreeSortECO "ECO kod" 0
menuText W TreeSortFreq "Frekvens" 0
menuText W TreeSortScore "Resultat" 0
menuText W TreeOpt "Alternativ" 0
menuText W TreeOptSlowmode "Långsam mod" 0 {Långsam mod för uppdateringar (hög noggrannhet)}
menuText W TreeOptFastmode "Snabb mod" 0 {Snabb mod för uppdateringar (ingen dragtranspositionering)}
menuText W TreeOptFastAndSlowmode "Snabb och långsam mod" 0 {Snabb mod sedan långsam mod för uppdateringar}
menuText W TreeOptStartStop "Automatisk uppdatering" 0 {Växlar automatisk uppdatering av trädfönstret}
menuText W TreeOptLock "Lås" 0 {Lås/ lås upp trädet för den aktuella databasen}
menuText W TreeOptTraining "Träna" 0 {Starta/ stäng av träningsläge}
menuText W TreeOptAutosave "Spara cache filen automatiskt" 0 \
  {Spara cachefilen automatiskt när trädfönstret stängs}
menuText W TreeHelp "Hjälp" 0
menuText W TreeHelpTree "Trädhjälp" 0
menuText W TreeHelpIndex "Hjälpindex" 0
translate W SaveCache {Spara cache}
translate W Training {Träna}
translate W LockTree {Lås}
translate W TreeLocked {Låst}
translate W TreeBest {Bäst}
translate W TreeBestGames {Bästa partier i trädet}
# Note: the next message is the tree window title row. After editing it,
# check the tree window to make sure it lines up with the actual columns.
translate W TreeTitleRow \
  { Drag      ECO     Frekvens      Res.    Elo~  Nivå  År~   %Remi} 
translate W TreeTotal {TOTALT} 
translate W DoYouWantToSaveFirst {Vill du spara först}
translate W AddToMask {Lägg till i sökmask}
translate W RemoveFromMask {Ta bort från sökmask}
translate W AddThisMoveToMask {Lägg till deta drag i sökmask}
translate W SearchMask {Sök i sökmask}
translate W DisplayMask {Visa sökmask}
translate W Nag {NAG-kod}
translate W Marker {Markera}
translate W Include {Inkludera}
translate W Exclude {Exkludera}
translate W MainLine {Huvudvariant}
translate W Bookmark {Bokmärke}
translate W NewLine {Ny variant}
translate W ToBeVerified {Att bli verifierad}
translate W ToTrain {Att träna}
translate W Dubious {Tveksam}
translate W ToRemove {Att ta bort}
translate W NoMarker {Ingen markering}
translate W ColorMarker {Färg}
translate W WhiteMark {Vit}
translate W GreenMark {Grön}
translate W YellowMark {Gul}
translate W BlueMark {Blå}
translate W RedMark {Röd}
translate W CommentMove {Kommentera drag}
translate W CommentPosition {Kommentera position}
translate W AddMoveToMaskFirst {Lägg till drag till sökmask först}
translate W OpenAMaskFileFirst {Öppna en sökmaskmaskfil först}
translate W Positions {Positioner}
translate W Moves {Drag}

# Finder window:
menuText W FinderFile "Fil" 0
menuText W FinderFileSubdirs "Sök i underkataloger" 0
menuText W FinderFileClose "Stäng Filsökaren" 0
menuText W FinderSort "Sortera" 0
menuText W FinderSortType "Typ" 0
menuText W FinderSortSize "Storlek" 0
menuText W FinderSortMod "Förändrad" 0
menuText W FinderSortName "Namn" 0
menuText W FinderSortPath "Sökväg" 0
menuText W FinderTypes "Typer" 0
menuText W FinderTypesScid "Sciddatabaser" 0
menuText W FinderTypesOld "Sciddatabaser i äldre format" 0
menuText W FinderTypesPGN "PGN-filer" 0
menuText W FinderTypesEPD "EPD-filer" 0
menuText W FinderTypesRep "Öppningsfiler" 0
menuText W FinderHelp "Hjälp" 0
menuText W FinderHelpFinder "Hjälp för Filsökaren" 0
menuText W FinderHelpIndex "Hjälpindex" 0
translate W FileFinder {Filsökaren}
translate W FinderDir {Katalog}
translate W FinderDirs {Kataloger}
translate W FinderFiles {Filer}
translate W FinderUpDir {upp}
translate W FinderCtxOpen {Öppna}
translate W FinderCtxBackup {Spara}
translate W FinderCtxCopy {Kopiera}
translate W FinderCtxMove {Flytta}
translate W FinderCtxDelete {Ta bort}

# Player finder:
menuText W PListFile "Fil" 0
menuText W PListFileUpdate "Uppdatera" 0
menuText W PListFileClose "Stäng spelarförteckningen" 1 
menuText W PListSort "Sortera" 0
menuText W PListSortName "Namn" 0
menuText W PListSortElo "Elo" 0
menuText W PListSortGames "Partier" 0
menuText W PListSortOldest "Äldst" 0 
menuText W PListSortNewest "Yngst" 0

# Tournament finder:
menuText W TmtFile "Fil" 0
menuText W TmtFileUpdate "Uppdatera" 0
menuText W TmtFileClose "Stäng turneringshanteraren" 0
menuText W TmtSort "Sortera" 0
menuText W TmtSortDate "Datum" 0
menuText W TmtSortPlayers "Spelare" 0
menuText W TmtSortGames "Partier" 0
menuText W TmtSortElo "Elo" 0
menuText W TmtSortSite "Plats" 0
menuText W TmtSortEvent "Evenemang" 1
menuText W TmtSortWinner "Vinnare" 0
translate W TmtLimit "Listbegränsningar"
translate W TmtMeanElo "Lägsta snitt Elo"
translate W TmtNone "Inga turneringar hittades."

# Graph windows:
menuText W GraphFile "Fil" 0
menuText W GraphFileColor "Spara som Postscript (i färg)" 8
menuText W GraphFileGrey "Spara som Postscript (i gråskala)" 8
menuText W GraphFileClose "Stäng fönster" 6
menuText W GraphOptions "Alternativ" 0
menuText W GraphOptionsWhite "Vit" 0
menuText W GraphOptionsBlack "Svart" 0
menuText W GraphOptionsBoth "Båda" 1
menuText W GraphOptionsPInfo "Spelarinformation" 0
# ====== TODO To be translated ======
menuText W GraphOptionsEloFile "Elo from rating file" 0
# ====== TODO To be translated ======
menuText W GraphOptionsEloDB "Elo from database" 0
translate W GraphFilterTitle "Filterdiagram: antal per 1000 partier"
translate W GraphAbsFilterTitle "Filtergraf: frekvens för partierna"
translate W ConfigureFilter {Konfigurera X-axlar för År, Rating och Drag}
translate W FilterEstimate "Uppskatta"
translate W TitleFilterGraph "Scid: Filtergraf"

# Analysis window:
translate W AddVariation {Lägg till variation}
translate W AddAllVariations {Lägg till alla variationer}
translate W AddMove {Lägg till drag}
translate W Annotate {Kommentera}
translate W ShowAnalysisBoard {Visa Analysbräde}
translate W ShowInfo {Visa schackmotorinformation}
translate W FinishGame {Avsluta parti}
translate W StopEngine {Stoppa schackmotor}
translate W StartEngine {Starta schackmotor}
translate W LockEngine {Lås schackmotor vid nuvarande position}
translate W AnalysisCommand {Analysera}
translate W PreviousChoices {Föregående val}
translate W AnnotateTime {Ange tid mellan drag i sekunder}
translate W AnnotateWhich {Lägg till variationer}
translate W AnnotateAll {För båda sidors drag}
translate W AnnotateAllMoves {Kommentera alla drag}
translate W AnnotateWhite {Endast vits drag}
translate W AnnotateBlack {Endast svarts drag}
translate W AnnotateBlundersOnly {När partidraget är en uppenbar blunder}
translate W AnnotateBlundersOnlyScoreChange {Analysen rapporterar blunder, med värderingsförändringar från/till: }
translate W BlundersThreshold {Tröskelvärde}
translate W ScoreAllMoves {Score all moves}
translate W LowPriority {Kör som lågprioriterad process} 
translate W ClickHereToSeeMoves {Klicka här för att se drag}
translate W ConfigureInformant {Informant-parametrar}
translate W Informant!? {Intressant drag}
translate W Informant? {Dåligt drag}
translate W Informant?? {Blunder}
translate W Informant?! {Tveksamt drag}
translate W Informant+= {Vit har en mindre fördel}
translate W Informant+/- {Vit har en liten fördel}
translate W Informant+- {Vit har en avgörande fördel}
translate W Informant++- {Partiet anses vara vunnet}
translate W Book {Bok}
translate W OtherBookMoves {Motståndarens bok}
translate W OtherBookMovesTooltip {Drag som motståndaren har ett svar på}

# Analysis Engine open dialog:
translate W EngineList {Lista över schackprogram}
translate W EngineName {Namn}
translate W EngineCmd {Startkommando}
translate W EngineArgs {Parametrar} 
translate W EngineDir {Katalog}
translate W EngineElo {Elo}
translate W EngineTime {Datum}
translate W EngineNew {Ny}
translate W EngineEdit {Redigera}
translate W EngineRequired {Fet stil indikerar obligatoriska fält; övriga fält är frivilliga} 

# PGN window menus:
menuText W PgnFile "Fil" 0
menuText W PgnFileCopy "Kopiera partiet till Clipbase" 0
menuText W PgnFilePrint "Skriv ut till..." 0
menuText W PgnFileClose "Stäng PGN-fönster" 0
menuText W PgnOpt "Presentation" 0
menuText W PgnOptColor "Färg" 0
menuText W PgnOptShort "Kort (3-raders) huvud" 0
menuText W PgnOptSymbols "Symbolbaserad kommentar" 1
menuText W PgnOptIndentC "Indragna kommentarer" 0
menuText W PgnOptIndentV "Indragna variationer" 7
menuText W PgnOptColumn "Spaltstil (ett drag per rad)" 1
menuText W PgnOptSpace "Utrymme efter dragnummer" 1
menuText W PgnOptStripMarks "Avlägsna koder för färgade fält och pilar" 1 
menuText W PgnOptBoldMainLine "Använd Fet Text för Huvudvariationsdrag" 4
menuText W PgnColor "Färger" 0
menuText W PgnColorHeader "Huvud..." 0
menuText W PgnColorAnno "Noteringar..." 0
menuText W PgnColorComments "Kommentarer..." 0
menuText W PgnColorVars "Variationer..." 0
menuText W PgnColorBackground "Bakgrund..." 0
menuText W PgnColorMain "Huvudvariation..." 0
menuText W PgnColorCurrent "Aktuellt drag bakgrund..." 1
menuText W PgnHelp "Hjälp" 0
menuText W PgnHelpPgn "PGN-hjälp" 0
menuText W PgnHelpIndex "Index" 0
translate W PgnWindowTitle {PGN-version av partiet %u} 

# Crosstable window menus:
menuText W CrosstabFile "Fil" 0
menuText W CrosstabFileText "Skriv ut till textfil..." 9
menuText W CrosstabFileHtml "Skriv ut till HTML-fil..." 9
menuText W CrosstabFileLaTeX "Skriv ut till LaTeX-fil..." 9
menuText W CrosstabFileClose "Stäng resultattabellsfönstret" 0
menuText W CrosstabEdit "Redigera" 0
menuText W CrosstabEditEvent "Evenemang" 0
menuText W CrosstabEditSite "Plats" 0
menuText W CrosstabEditDate "Datum" 0
menuText W CrosstabOpt "Presentation" 0
menuText W CrosstabOptAll "Alla-möter-alla" 0
menuText W CrosstabOptSwiss "Schweizer" 0
menuText W CrosstabOptKnockout "Knock Out" 0
menuText W CrosstabOptAuto "Auto" 1
menuText W CrosstabOptAges "Ålder i år" 8
menuText W CrosstabOptNats "Nationalitet" 0
menuText W CrosstabOptRatings "Rating" 0
menuText W CrosstabOptTitles "Titlar" 0
menuText W CrosstabOptBreaks "Tie-break poäng" 4
menuText W CrosstabOptDeleted "Inkludera raderade partier" 8 
menuText W CrosstabOptColors "Färg (endast Schweizer)" 0
menuText W CrosstabOptColumnNumbers "Numrerade kolumner (Endast alla-mot-alla)" 2 
menuText W CrosstabOptGroup "Gruppresultat" 0
menuText W CrosstabSort "Sortera" 0
menuText W CrosstabSortName "Namn" 0
menuText W CrosstabSortRating "Rating" 0
menuText W CrosstabSortScore "Resultat" 0
menuText W CrosstabColor "Färg" 0
menuText W CrosstabColorPlain "Ren text" 0
menuText W CrosstabColorHyper "Hypertext" 0
menuText W CrosstabHelp "Hjälp" 0
menuText W CrosstabHelpCross "Hjälp för resultattabell" 0
menuText W CrosstabHelpIndex "Hjälpindex" 0
translate W SetFilter {Bestäm filter}
translate W AddToFilter {Utöka filter}
translate W Swiss {Schweizer}
translate W Category {Kategori} 

# Opening report window menus:
menuText W OprepFile "Fil" 0
menuText W OprepFileText "Skriv ut till textfil..." 9
menuText W OprepFileHtml "Skriv ut till HTML-fil..." 9
menuText W OprepFileLaTeX "Skriv ut till LaTeX-fil..." 9
menuText W OprepFileOptions "Alternativ..." 0
menuText W OprepFileClose "Stäng rapportfönstret" 0
menuText W OprepFavorites "Favoriter" 1 
menuText W OprepFavoritesAdd "Lägg till rapport..." 0 
menuText W OprepFavoritesEdit "Redigera favoritrapport..." 0 
menuText W OprepFavoritesGenerate "Skapa rapport..." 0 
menuText W OprepHelp "Hjälp" 0
menuText W OprepHelpReport "Hjälp för öppningsrapporter" 0
menuText W OprepHelpIndex "Hjälpindex" 0

# Header search:
translate W HeaderSearch {Sök i partihuvud}
translate W EndSideToMove {Sida vid draget vid slutet av partiet}
translate W GamesWithNoECO {Partier utan ECO-kod?}
translate W GameLength {Partilängd}
translate W FindGamesWith {Hitta flaggade partier}
translate W StdStart {Normal utgångsställning}
translate W Promotions {Förvandlingar}
translate W Comments {Kommentarer}
translate W Variations {Variationer}
translate W Annotations {Noteringar}
translate W DeleteFlag {Raderingsflagga}
translate W WhiteOpFlag {Vits öppning}
translate W BlackOpFlag {Svarts öppning}
translate W MiddlegameFlag {Mittspel}
translate W EndgameFlag {Slutspel}
translate W NoveltyFlag {Nyhet}
translate W PawnFlag {Bondestruktur}
translate W TacticsFlag {Taktiska ställningar}
translate W QsideFlag {Damflygelsinitiativ}
translate W KsideFlag {Kungsflygelsinitiativ}
translate W BrilliancyFlag {Utmärkt parti}
translate W BlunderFlag {Bortsättningar}
translate W UserFlag {Användare}
translate W PgnContains {PGN innehåller text}
translate W Annotator {Kommentator}
translate W Cmnts {Bara kommenterade partier}

# Game list window:
translate W GlistNumber {Nummer}
translate W GlistWhite {Vit}
translate W GlistBlack {Svart}
translate W GlistWElo {Elo, vit}
translate W GlistBElo {Elo, svart}
translate W GlistEvent {Evenemang}
translate W GlistSite {Plats}
translate W GlistRound {Rond}
translate W GlistDate {Datum}
translate W GlistYear {År}
translate W GlistEDate {Startdatum}
translate W GlistResult {Resultat}
translate W GlistLength {Längd}
translate W GlistCountry {Land}
translate W GlistECO {ECO}
translate W GlistOpening {Öppning}
translate W GlistEndMaterial {Slutmaterial}
translate W GlistDeleted {Raderad}
translate W GlistFlags {Flaggor}
translate W GlistVars {Variationer}
translate W GlistComments {Kommentarer}
translate W GlistAnnos {Noteringar}
translate W GlistStart {Start}
translate W GlistGameNumber {Partinummer}
translate W GlistAverageElo {Medel-Elo}
translate W GlistRating {Rating}
translate W GlistFindText {Sök text}
translate W GlistMoveField {Drag}
translate W GlistEditField {Konfiguration}
translate W GlistAddField {Lägg till}
translate W GlistDeleteField {Ta bort}
translate W GlistWidth {Bredd}
translate W GlistAlign {Justering}
translate W GlistAlignL {Justera: vänster}
translate W GlistAlignR {Justera: höger}
translate W GlistAlignC {Justera: centrera}
translate W GlistColor {Färg}
translate W GlistSep {Separator}
translate W GlistCurrentSep {-- Aktuell --}
translate W GlistNewSort {Ny}
translate W GlistAddToSort {Lägg till}
translate W GsortSort {Sortera...}
translate W GsortDate {Datum}
translate W GsortYear {År}
translate W GsortEvent {Evenemang}
translate W GsortSite {Plats}
translate W GsortRound {Runda}
translate W GsortWhiteName {Vits namn}
translate W GsortBlackName {Svarts namn}
translate W GsortECO {ECO}
translate W GsortResult {Resultat}
translate W GsortMoveCount {Antal drag}
translate W GsortAverageElo {Medel-Elo}
translate W GsortCountry {Land}
translate W GsortDeleted {Borttagen}
translate W GsortEventDate {Evenemangsdatum}
translate W GsortWhiteElo {Vits Elo}
translate W GsortBlackElo {Svarts Elo}
translate W GsortComments {Kommentarer}
translate W GsortVariations {Variationer}
translate W GsortNAGs {Kommentarssymboler}
translate W GsortAscending {Stigande}
translate W GsortDescending {Sjunkande}
translate W GsortAdd {Lägg till}
translate W GsortStore {Spara}
translate W GsortLoad {Öppna}
translate W GlistRemoveThisGameFromFilter  {Ta bort detta parti från Filter}
translate W GlistRemoveGameAndAboveFromFilter  {Ta bort parti (och alla ovanför det) från Filter}
translate W GlistRemoveGameAndBelowFromFilter  {Ta bort parti (och alla nedanför det) från Filter}
translate W GlistDeleteGame {Ta tillbaka detta parti} 
translate W GlistDeleteAllGames {Ta bort alla partier i filtret} 
translate W GlistUndeleteAllGames {Ta tillbaka alla partier i filtret} 
translate W GlistMergeGameInBase {Lägg till parti i annan databas} 

# Maintenance window:
translate W DatabaseName {Databasnamn:}
translate W TypeIcon {Ikontyp:}
translate W NumOfGames {Partier:}
translate W NumDeletedGames {Raderade partier:}
translate W NumFilterGames {Partier i filter:}
translate W YearRange {Tidsperiod:}
translate W RatingRange {Ratingintervall:}
translate W Description {Beskrivning} 
translate W Flag {Flagga}
translate W CustomFlags {Anpassade flaggor}
translate W DeleteCurrent {Ta bort aktuellt parti}
translate W DeleteFilter {Ta bort partierna i filtret}
translate W DeleteAll {Ta bort alla partier}
translate W UndeleteCurrent {Återta aktuellt parti}
translate W UndeleteFilter {Återta partierna i filtret}
translate W UndeleteAll {Återta alla partier}
translate W DeleteTwins {Ta bort dubbletter}
translate W MarkCurrent {Markera aktuellt parti}
translate W MarkFilter {Markera partierna i filtret}
translate W MarkAll {Markera alla partier}
translate W UnmarkCurrent {Avmarkera aktuellt parti}
translate W UnmarkFilter {Avmarkera partierna i filtret}
translate W UnmarkAll {Avmarkera alla partier}
translate W Spellchecking {Rättstava}
translate W Players {Spelare}
translate W Events {Evenemang}
translate W Sites {Platser}
translate W Rounds {Ronder}
translate W DatabaseOps {Databashantering}
translate W ReclassifyGames {ECO-klassificera partier}
translate W CompactDatabase {Komprimera databasen}
translate W SortDatabase {Sortera databasen}
translate W AddEloRatings {Lägg till Elorating}
translate W AutoloadGame {Ladda partinummer automatiskt}
translate W StripTags {Avlägsna PGN-taggar}
translate W StripTag {Avlägsna taggar}
translate W Cleaner {Snygga till databasen}
translate W CleanerHelp {
Databasstädaren i Scid genomför allt det underhåll av databasen du väljer nedan på den aktiva databasen.

De nuvarande inställningarna i ECO-klassificering och Radera dubbletter kommer att appliceras om du väljer dessa åtgärder.
}
translate W CleanerConfirm {
När du väl startat Databasstädaren kan du inte avbryta den!

På en stor databas kan detta ta ett bra tag. Tidsåtgången beror på valda åtgärder och deras inställningar.

Ãr du säker på att du vill påbörja databasunderhållet nu?
}
translate W TwinCheckUndelete {att växla; "u" tar tillbaka båda)}
translate W TwinCheckprevPair {Tidigare par}
translate W TwinChecknextPair {Nästa par}
translate W TwinChecker {Scid: Dublettpartikontrollerare}
translate W TwinCheckTournament {Partier i turnering:}
translate W TwinCheckNoTwin {Ingen dublett  }
translate W TwinCheckNoTwinfound {Ingen dublett detekterades för detta parti.\nFör att visa dubletter med detta fönster, måste du först använda "Ta bort dublettpartier..."-funktionen. }
translate W TwinCheckTag {Dela taggar...}
translate W TwinCheckFound1 {Scid hittade $result dublettpartier}
translate W TwinCheckFound2 { och ange deras borttagningsflaggor}
translate W TwinCheckNoDelete {Det finns inga partier i denna databas att ta bort.}
translate W TwinCriteria1 { Dina inställningar för att hitta dublettpartier är potentiellt sannorlika att\norsaka att icke-dublettpartier med liknande drag blir markerade som dubletter.}
translate W TwinCriteria2 {Det är rekommenderat att om du väljer "Nej" för "samma drag" och att du väljer "Ja" för Färger, Evenemang, Plats, Runda, År och Månads inställningar.\nVill du fortsätta och ta bort dubletter ändå? }
translate W TwinCriteria3 {Det är rekommenderat att du specificerar "Ja" för åtminstonde två av "samma plats", "samma runda" och "samma år" inställningar.\nVill du fortsätta och ta bort dubletter ändå?}
translate W TwinCriteriaConfirm {Scid: Bekräfta dublettinställningar}
translate W TwinChangeTag "Ändra följande partitaggar:\n\n"
translate W AllocRatingDescription "Detta kommando kommer att använda den aktuella stavningskontrollfilen för att lägga till Elo-rating till partier i denna databas. Där en spelare inte har en aktuell rating men hans/hennes rating vid tiden för partiet är listat i stavningskontrollfilen, kommer denna rating att läggas till."
translate W RatingOverride "Skriv över existerande inte-noll rating?"
translate W AddRatings "Lägg till rating till:"
translate W AddedRatings {Scid lade till $r Elo-ratingar i $g partier.}
translate W NewSubmenu "Ny undermeny"

# Comment editor:
translate W AnnotationSymbols  {Symboler:}
translate W Comment {Kommentar:}
translate W InsertMark {Infoga symbol} 
translate W InsertMarkHelp {
Infoga/ta bort markering: Välj färg, typ av markering samt ruta.
Infoga/ta bort pil: Höger-klicka två rutor.
}

# Nag buttons in comment editor:
translate W GoodMove {Bra drag}
translate W PoorMove {Dåligt drag}
translate W ExcellentMove {Utmärkt drag}
translate W Blunder {Blunder}
translate W InterestingMove {Intressant drag}
translate W DubiousMove {Tveksamt drag}
translate W WhiteDecisiveAdvantage {Vit har en avgörande fördel}
translate W BlackDecisiveAdvantage {Svart har en avgörande fördel}
translate W WhiteClearAdvantage {Vit har en klar fördel}
translate W BlackClearAdvantage {Svart har en klar fördel}
translate W WhiteSlightAdvantage {Vit har en liten fördel}
translate W BlackSlightAdvantage {Svart har en liten fördel}
translate W Equality {Jämnvikt}
translate W Unclear {Oklar}
translate W Diagram {Diagram}

# Board search:
translate W BoardSearch {Positionssökningar}
translate W FilterOperation {Hantering av aktuellt filter:}
translate W FilterAnd {AND (Restriktivt filter)}
translate W FilterOr {OR (Ackumulativt filter)}
translate W FilterIgnore {IGNORE (Använd ej filter)}
translate W SearchType {Söktyp:}
translate W SearchBoardExact {Exakt position (alla pjäser på samma rutor)}
translate W SearchBoardPawns {Bönder (samma material, alla bönder på samma rutor)}
translate W SearchBoardFiles {Filer (samma material, alla bönder på samma filer)}
translate W SearchBoardAny {Obestämt (samma material, bönder och pjäser på valfria rutor)}
translate W SearchInRefDatabase {Sök i databas}
translate W LookInVars {Sök i variationer}

# Material search:
translate W MaterialSearch {Materialsökning}
translate W Material {Material}
translate W Patterns {Ställningar}
translate W Zero {Inga/-en}
translate W Any {Flera}
translate W CurrentBoard {Aktuell ställning}
translate W CommonEndings {Vanliga slutspel}
translate W CommonPatterns {Vanliga ställningar}
translate W MaterialDiff {Skillnad i material}
translate W squares {rutor}
translate W SameColor {Samma färg}
translate W OppColor {Motsatt färg}
translate W Either {Antingen eller}
translate W MoveNumberRange {Dragintervall}
translate W MatchForAtLeast {Träffa minst}
translate W HalfMoves {halvdrag}

# Common endings in material search:
translate W EndingPawns {Bondeslutspel}
translate W EndingRookVsPawns {Torn mot Bonde (Bönder)}
translate W EndingRookPawnVsRook {Torn och 1 Bonde mot Torn}
translate W EndingRookPawnsVsRook {Torn och Bonde (Bönder) mot Torn}
translate W EndingRooks {Torn mot Torn slutspel}
translate W EndingRooksPassedA {Torn mot Torn slutspel med en fri a-bonde}
translate W EndingRooksDouble {Dubbla Torn slutspel}
translate W EndingBishops {Löpare mot Löpare slutspel}
translate W EndingBishopVsKnight {Löpare mot Springare slutspel}
translate W EndingKnights {Springare mot Springare slutspel}
translate W EndingQueens {Dam mot Dam slutspel}
translate W EndingQueenPawnVsQueen {Dam och 1 Bonde mot Dam}
translate W BishopPairVsKnightPair {Två Löpare mot Två Springare mittspel}

# Common patterns in material search:
translate W PatternWhiteIQP {Vit Isolerad dambonde}
translate W PatternWhiteIQPBreakE6 {Vit Isolerad dambonde: d4-d5 brott mot e6}
translate W PatternWhiteIQPBreakC6 {Vit Isolerad dambonde: d4-d5 brott mot c6}
translate W PatternBlackIQP {Svart Isolerad dambonde}
translate W PatternWhiteBlackIQP {Vit Isolerad dambonde mot Svart Isolerad dambonde}
translate W PatternCoupleC3D4 {Vit c3+d4 Isolaterat Bondepar}
translate W PatternHangingC5D5 {Svart Hängande Bönder på c5 och d5}
translate W PatternMaroczy {Maroczy Center (med Bönder på c4 och e4)}
translate W PatternRookSacC3 {Tornoffer på c3}
translate W PatternKc1Kg8 {O-O-O mot O-O (Kc1 mot Kg8)}
translate W PatternKg1Kc8 {O-O mot O-O-O (Kg1 mot Kc8)}
translate W PatternLightFian {Ljus-Ruta Fianchetton (Löpare-g2 mot Löpare-b7)}
translate W PatternDarkFian {Mörk-Ruta Fianchetton (Löpare-b2 mot Löpare-g7)}
translate W PatternFourFian {Fyra Fianchetton (Löpare på b2,g2,b7,g7)}

# Game saving:
translate W Today {Idag}
translate W ClassifyGame {Klassificera parti}

# Setup position:
translate W EmptyBoard {Töm brädet}
translate W InitialBoard {Utgångsställning}
translate W SideToMove {Färg vid draget}
translate W MoveNumber {Antal drag}
translate W Castling {Rockad}
translate W EnPassantFile {En Passant fil}
translate W ClearFen {Rensa FEN}
translate W PasteFen {Klistra in FEN}
translate W SaveAndContinue {Spara och fortsätt}
translate W DiscardChangesAndContinue {Ignorera ändringar\noch fortsätt}
translate W GoBack {Gå tillbaka}

# Replace move dialog:
translate W ReplaceMove {Ersätt drag}
translate W AddNewVar {Lägg till ny variation}
translate W NewMainLine {Ny huvudvariation}
translate W ReplaceMoveMessage {Det finns redan ett drag i denna ställning.

Du kan ersätta detta drag, och förlora samtliga följande, eller lägga till ditt drag som en ny variation.

(Om du stänger av "Fråga före ersätt drag" i Alternativ:Drag menyn slipper du denna fråga i framtiden.)}

# Make database read-only dialog:
translate W ReadOnlyDialog {Om du ger denna databas endast läsbar kan du inte göra några ändringar i den.
Inga partier kan sparas eller ersättas, och du kan inte ändra flaggor för raderbara partier.
Alla sorteringsinställningar eller ECO-klassificeringar kommer att vara temporära.

Du kan göra den skrivbar igen genom att helt enkelt stänga och öppna den igen.

Vill du verkligen ange att databasen endast ska vara läsbar?}

# Clear game dialog:
translate W ClearGameDialog {Detta parti har ändrats.

Vill du verkligen fortsätta och förlora ändringarna?
}

# Exit dialog:
translate W ExitDialog {Vill du verkligen avsluta Scid?}
translate W ExitUnsaved {De följande databaserna har osparade förändringar. Om du avslutar nu, kommer dessa förändringar att gå förlorade.} 

# Import window:
translate W PasteCurrentGame {Klistra in aktuellt parti}
translate W ImportHelp1 {Ange eller klistra in ett parti i PGN-format i området ovan.}
translate W ImportHelp2 {Eventuella felaktigheter kommer att anges här.}
translate W OverwriteExistingMoves {Skriv över existerande drag ?}

# ECO Browser:
translate W ECOAllSections {alla ECO avdelningar}
translate W ECOSection {ECO avdelning}
translate W ECOSummary {Sammanfattning för}
translate W ECOFrequency {Underkodsfrekvens för}

# Opening Report:
translate W OprepTitle {Öppningsrapport}
translate W OprepReport {Rapport}
translate W OprepGenerated {Skapad av}
translate W OprepStatsHist {Statistik och historik}
translate W OprepStats {Statistik}
translate W OprepStatAll {Rapporterade partier}
translate W OprepStatBoth {Båda med rating}
translate W OprepStatSince {Sedan}
translate W OprepOldest {De äldsta partierna}
translate W OprepNewest {De yngsta partierna}
translate W OprepPopular {Aktuell popularitet}
translate W OprepFreqAll {Frekvens totalt:   }
translate W OprepFreq1   {Under det senaste året: }
translate W OprepFreq5   {Under de 5 senaste åren: }
translate W OprepFreq10  {Under de 10 senaste åren: }
translate W OprepEvery {en gång var %u parti}
translate W OprepUp {ökat %u%s gentemot samtliga år}
translate W OprepDown {minskat %u%s gentemot samtliga år}
translate W OprepSame {ingen förändring gentemot samtliga år}
translate W OprepMostFrequent {Spelas mest av}
translate W OprepMostFrequentOpponents {Vanligaste motståndaren}
translate W OprepRatingsPerf {Rating och resultat}
translate W OprepAvgPerf {Genomsnittrating och resultat}
translate W OprepWRating {Vits rating}
translate W OprepBRating {Svarts rating}
translate W OprepWPerf {Vits resultat}
translate W OprepBPerf {Svarts resultat}
translate W OprepHighRating {Partierna med högst genomsnittsrating}
translate W OprepTrends {Resultattrender}
translate W OprepResults {Längd och frekvens}
translate W OprepLength {Partiets längd}
translate W OprepFrequency {Frekvens}
translate W OprepWWins {Vita vinster: }
translate W OprepBWins {Svarta vinster: }
translate W OprepDraws {Remier:      }
translate W OprepWholeDB {Hela databasen}
translate W OprepShortest {Kortaste vinster}
# translate W OprepShortWhite {De kortaste vita vinsterna}
# translate W OprepShortBlack {De kortaste svarta vinsterna}
translate W OprepMovesThemes {Drag och teman}
translate W OprepMoveOrders {Dragordning för att nå rapportställningen}
translate W OprepMoveOrdersOne \
  {Ställningen nåddes bara genom en dragordning:}
translate W OprepMoveOrdersAll \
  {Det fanns %u dragordningar som ledde fram denna ställning:}
translate W OprepMoveOrdersMany \
  {Det fanns %u dragordningar som ledde fram denna ställning. De %u vanligaste är:}
translate W OprepMovesFrom {Drag från rapportställningen}
translate W OprepMostFrequentEcoCodes {De mest förekommande ECO-koderna}
translate W OprepThemes {Positionella teman}
translate W OprepThemeDescription {Temanfrekvens de första %u dragen av varje parti}
# translate W OprepThemeDescription {Antal teman vid drag %u}
translate W OprepThemeSameCastling {Rockad på samma flygel}
translate W OprepThemeOppCastling {Rockad på olika flyglar}
translate W OprepThemeNoCastling {Ingen har gjort rockad}
translate W OprepThemeKPawnStorm {Bondestorm på kungsflygeln}
translate W OprepThemeQueenswap {Dambyte}
translate W OprepThemeWIQP {Isolerad vit dambonde} 
translate W OprepThemeBIQP {Isolerad svart dambonde}
translate W OprepThemeWP567 {Vit bonde på 5e/6e/7e raden}
translate W OprepThemeBP234 {Svart bonde på 2a/3e/4e raden}
translate W OprepThemeOpenCDE {Öppen c/d/e linje}
translate W OprepTheme1BishopPair {Endast den ena sidan har löparparet}
translate W OprepEndgames {Slutspel}
translate W OprepReportGames {Antal partier i rapporten}
translate W OprepAllGames    {Samtliga partier}
translate W OprepEndClass {Material i slutställningen}
translate W OprepTheoryTable {Teorisammanställning}
translate W OprepTableComment {Skapad från de %u högst rankade partierna.}
translate W OprepExtraMoves {Ytterligare antal drag i notförteckningen}
translate W OprepMaxGames {Maximalt antal partier i sammanställningen}
translate W OprepViewHTML {Visa HTML}
translate W OprepViewLaTeX {Visa LaTeX}

# Player Report:
translate W PReportTitle {Spelarrapport} 
translate W PReportColorWhite {med de vita pjäserna} 
translate W PReportColorBlack {med de svarta pjäserna} 
translate W PReportMoves {efter %s} 
translate W PReportOpenings {Öppningar} 
translate W PReportClipbase {Töm Clipbase och kopiera liknande partier dit} 


# Piece Tracker window:
translate W TrackerSelectSingle {Vänsterklicka för att välja denna pjäs.} 
translate W TrackerSelectPair {Vänsterklicka för att välja denna pjäs; använd höger musknapp för att också välja den relaterade pjäsen.}
translate W TrackerSelectPawn {Vänsterklicka för att välja denna pjäs; använd höger musknapp för att välja alla åtta bönder.}
translate W TrackerStat {Statistik}
translate W TrackerGames {% partier med drag till rutan}
translate W TrackerTime {% tid på varje ruta}
translate W TrackerMoves {Drag}
translate W TrackerMovesStart {Ange vid vilket drag sökningen ska börja.}
translate W TrackerMovesStop {Ange vid vilket drag sökningen ska sluta.}

# Game selection dialogs:
translate W SelectAllGames {Alla partier i databasen}
translate W SelectFilterGames {Endast partierna i filtret}
translate W SelectTournamentGames {Endast partierna i den aktuella turneringen}
translate W SelectOlderGames {Endast äldre partier}

# Delete Twins window:
translate W TwinsNote {Partier måste minst ha samma spelare för att kunna identifieras som dubbletter samt uppfylla andra kriterier du kan ange nedan. När dubbletter hittas raderas det kortare partiet. Tips: det bästa är att använda rättstavningen innan dubblettjämförelsen eftersom detta förbättrar möjligheten för upptäckt av dubbletter.}
translate W TwinsCriteria {Kriterium: Dubbletter måste ha...}
translate W TwinsWhich {Ange vilka partier som ska jämföras}
translate W TwinsColors {Samma färger?}
translate W TwinsEvent {Samma evenemang?}
translate W TwinsSite {Samma plats?}
translate W TwinsRound {Samma rond?}
translate W TwinsYear {Samma år?}
translate W TwinsMonth {Samma månad?}
translate W TwinsDay {Samma dag?}
translate W TwinsResult {Samma resultat?}
translate W TwinsECO {Samma ECO-kod?}
translate W TwinsMoves {Samma drag?}
translate W TwinsPlayers {Jämför spelarnas namn:}
translate W TwinsPlayersExact {Exakt kopia}
translate W TwinsPlayersPrefix {Endast de 4 första bokstäverna}
translate W TwinsWhen {När dubbletter tas bort}
translate W TwinsSkipShort {Ignorera alla partier som är kortare än 5 drag?}
translate W TwinsUndelete {Återta alla partier först?}
translate W TwinsSetFilter {Filtrera alla borttagna dubbletter?}
translate W TwinsComments {Spara alltid partier med kommentarer?}
translate W TwinsVars {Spara alltid partier med variationer?}
translate W TwinsDeleteWhich {Ange vilket parti som ska tas bort:} 
translate W TwinsDeleteShorter {Det kortare partiet} 
translate W TwinsDeleteOlder {Partiet med lägst nummer} 
translate W TwinsDeleteNewer {Partiet med högst nummer} 
translate W TwinsDelete {Ta bort partier}

# Name editor window:
translate W NameEditType {Typ av namn att redigera}
translate W NameEditSelect {Partier att redigera}
translate W NameEditReplace {Ersätt}
translate W NameEditWith {med}
translate W NameEditMatches {Matchar: Tryck Ctrl+1 till Ctrl+9 för att välja}
translate W CheckGames {Kontrollera partier}
translate W CheckGamesWhich {Kontrollera partier}
translate W CheckAll {Alla partier}
translate W CheckSelectFilterGames {Bara partier i filter}

# Classify window:
translate W Classify {Klassificera}
translate W ClassifyWhich {ECO-klassificera vilka partier?}
translate W ClassifyAll {Alla partier (skriv över gamla ECO-koder)}
translate W ClassifyYear {Alla partier spelade under det senaste året}
translate W ClassifyMonth {Alla partier spelade den senaste månaden}
translate W ClassifyNew {Endast partier som ännu ej klassificerats}
translate W ClassifyCodes {ECO-koder som ska användas}
translate W ClassifyBasic {Enbart standardkoder ("B12", ...)}
translate W ClassifyExtended {Scidextensioner ("B12j", ...)}

# Compaction:
translate W NameFile {Namnfil}
translate W GameFile {Partifil}
translate W Names {Namn}
translate W Unused {Ej använd}
translate W SizeKb {Storlek (kb)}
translate W CurrentState {Aktuell status}
translate W AfterCompaction {Efter komprimering}
translate W CompactNames {Namn, komprimera namnfil}
translate W CompactGames {Partier, komprimera partifil}
translate W NoUnusedNames "Det finns inga oanvända namn, så namnfilen är redan fullt komprimerad."
translate W NoUnusedGames "Partifilen är redan fullt komprimerad."
translate W GameFileCompacted {Partifilen för databasen blev komprimerad.}

# Sorting:
translate W SortCriteria {Kriterium}
translate W AddCriteria {Lägg till kriterium}
translate W CommonSorts {Normal sortering}
translate W Sort {Sortering}

# Exporting:
translate W AddToExistingFile {Lägg till partier till en existerande fil?}
translate W ExportComments {Exportera kommentarer?}
translate W ExportVariations {Exportera variationer?}
translate W IndentComments {Dra in kommentarer?}
translate W IndentVariations {Dra in variationer?}
translate W ExportColumnStyle {Kolumnstil (ett drag per rad)?}
translate W ExportSymbolStyle {Symbolbaserade kommentarer:}
translate W ExportStripMarks {Avlägsna koder för fält och pilar från kommentarerna?} 

# Goto game/move dialogs:
translate W LoadGameNumber {Ange partiets nummer:}
translate W GotoMoveNumber {Gå till drag nummer:}

# Copy games dialog:
translate W CopyGames {kopiera partier}
translate W CopyConfirm {
 Vill du verkligen kopiera
 de [::utils::thousands $nGamesToCopy] filtrerade partierna
 ur databasen "$fromName"
 till databasen "$targetName"?
}
translate W CopyErr {Kan ej kopiera partier}
translate W CopyErrSource {källdatabasen}
translate W CopyErrTarget {måldatabasen}
translate W CopyErrNoGames {har inga partier i filtret}
translate W CopyErrReadOnly {kan bara läsas}
translate W CopyErrNotOpen {är ej öppen}

# Colors:
translate W LightSquares {Ljusa fält}
translate W DarkSquares {Mörka fält}
translate W SelectedSquares {Valda fält}
translate W SuggestedSquares {Föreslagna fält}
translate W WhitePieces {Vita pjäser}
translate W BlackPieces {Svarta pjäser}
translate W WhiteBorder {Vit kantlinje}
translate W BlackBorder {Svart kantlinje}

# Novelty window:
translate W FindNovelty {Hitta nyhet}
translate W Novelty {Nyhet}
translate W NoveltyInterrupt {Nyhetssökningen avbröts}
translate W NoveltyNone {Inga nyheter hittades i detta parti}
translate W NoveltyHelp {
Scid kommer att försöka hitta det första draget som leder till en position som inte annars finns i denna databas eller i spelöppingsboken baserad på ECO.
}

# Sounds configuration:
translate W SoundsFolder {Ljudfilskatalog}
translate W SoundsFolderHelp {Katalogen ska ha filerna King.wav, a.wav, 1.wav, etc}
translate W SoundsAnnounceOptions {Inställningar för Dragannonsering}
translate W SoundsAnnounceNew {Annonsera nya drag när de görs}
translate W SoundsAnnounceForward {Annonsera drag när du går fram ett drag}
translate W SoundsAnnounceBack {Annonsera drag när du går fram eller tillbaka ett drag}
# ====== TODO To be translated ======
translate W SoundsSoundDisabled {Scid could not find the Snack audio package at startup;\nSound is disabled.}

# Upgrading databases:
translate W Upgrading {Uppdaterar}
translate W ConfirmOpenNew {
Denna databas är i ett gammal format (Scid 3) och kan inte öppnas i Scid 4, men en databas i det nya formatet (Scid 4) har redan skapats.

Vill du öppna den senare databasen istället?
}
translate W ConfirmUpgrade {
Denna databas är i ett gammalt format (Scid 3). En databas i det nyare formatet måste skapas innan den kan användas i Scid 4.

Genom att uppdatera skapas en databas i det nya formatet med samma innehåll; uppdateringen ändrar ingenting i den gamla databasen.

Detta kan ta ett tag men behöver bara göras en gång. Om du tycker att det tar alltför lång tid kan du avbryta processen.

Vill du uppdatera denna databas nu?
}

# Recent files options:
translate W RecentFilesMenu {Antal senast öppnade filer i Arkivmenyn}
translate W RecentFilesExtra {Antal senast öppnade filer i extra undermeny}

# My Player Names options:
translate W MyPlayerNamesDescription {
Ange en lista på dina favoritspelare här nedanför. Skriv ett namn per rad. Jokertecken ("?", t ex, motsvarar ett enstaka tecken medan "*" står för flera tecken) är tillåtna.

Varje gång ett parti med en spelare vars namn står i denna lista öppnas kommer brädet automatiskt att vridas så att partiet visas från spelarens perspektiv.
} 
translate W showblunderexists {visa blunder finns}
translate W showblundervalue {visa blundervärde}
translate W showscore {visa värdering}
translate W coachgame {tränarparti}
translate W configurecoachgame {konfigurera tränarparti}
translate W configuregame {Konfigurera parti}
translate W Phalanxengine {Phalanx schackmotor}
translate W Coachengine {Tränarschackmotor}
translate W difficulty {svårighetsgrad}
translate W hard {svår}
translate W easy {lätt}
translate W Playwith {Spela med}
translate W white {vit}
translate W black {svart}
translate W both {båda}
translate W Play {Spela}
translate W Noblunder {Ingen blunder}
translate W blunder {blunder}
translate W Noinfo {-- Ingen information --}
translate W PhalanxOrTogaMissing {Phalanx eller Toga hittades inte}
translate W moveblunderthreshold {drag är en blunder om förlust är större än}
translate W limitanalysis {begränsa schackmotors analystid}
translate W seconds {sekunder}
translate W Abort {Avbryt}
translate W Resume {Fortsätt}
translate W OutOfOpening {utanför öppning}
translate W NotFollowedLine {Du följde inte variationen}
translate W DoYouWantContinue {Vill du fortsätta ?}
translate W CoachIsWatching {Tränaren observerar}
translate W Ponder {Permanent tänkande}
translate W LimitELO {Begränsa ELO-styrka}
translate W DubiousMovePlayedTakeBack {Ett tveksamt drag spelades, vill du ta tillbaka det ?}
translate W WeakMovePlayedTakeBack {Ett svagt drag spelades, vill du ta tillbaka det ?}
translate W BadMovePlayedTakeBack {Ett dåligt drag spelades, vill du ta tillbaka det ?}
translate W Iresign {Jag ger upp}
translate W yourmoveisnotgood {ditt drag är inte bra}
translate W EndOfVar {Slut på variation}
translate W Openingtrainer {Öppningstränare}
translate W DisplayCM {Visa kandidatdrag}
translate W DisplayCMValue {Visa kandidatdragens värden}
translate W DisplayOpeningStats {Visa statistik}
translate W ShowReport {Visa rapport}
translate W NumberOfGoodMovesPlayed {bra drag spelade}
translate W NumberOfDubiousMovesPlayed {tveksamma drag spelade}
translate W NumberOfMovesPlayedNotInRepertoire {drag spelade som inte finns i repetoar}
translate W NumberOfTimesPositionEncountered {gånger positionen påträffats}
translate W PlayerBestMove  {Tillåt bara bästa dragen}
translate W OpponentBestMove {Motståndare spelar bästa dragen}
translate W OnlyFlaggedLines {Bara markerade variationer}
translate W resetStats {Nollställ statistik}
translate W Repertoiretrainingconfiguration {Repetoartränings-konfiguration}
translate W Loadingrepertoire {Öppnar repetoar}
translate W Movesloaded {Öppnat drag}
translate W Repertoirenotfound {Repetoar hittades inte}
translate W Openfirstrepertoirewithtype {Öppna först en repetoardatabas med ikon/typ angivet på höger sida}
translate W Movenotinrepertoire {Drag finns inte i repetoar}
translate W PositionsInRepertoire {Positioner i repetoar}
translate W PositionsNotPlayed {Positioner inte spelade}
translate W PositionsPlayed {Positioner spelade}
translate W Success {Framgång}
translate W DubiousMoves {Tveksamma drag}
translate W OutOfRepertoire {Utanför repetoar}
translate W ConfigureTactics {Konfigurera taktik}
translate W ResetScores {Nollställ poäng}
translate W LoadingBase {Öppnar bas}
translate W Tactics {Taktik}
translate W ShowSolution {Visa lösning}
translate W NextExercise {Nästa övning}
translate W PrevExercise {Tidigare övning}
translate W StopTraining {Avbryt träning}
translate W Next {Nästa}
translate W ResettingScore {Nollställer poäng}
translate W LoadingGame {Öppnar parti}
translate W MateFound {Matt hittad}
translate W BestSolutionNotFound {Bästa lösningen hittades INTE !}
translate W MateNotFound {Matt hittades inte}
translate W ShorterMateExists {En kortare matt finns}
translate W ScorePlayed {Poäng spelad}
translate W Expected {förväntat}
translate W ChooseTrainingBase {Välj träningsbas}
translate W Thinking {Tänker}
translate W AnalyzeDone {Analys klar}
translate W WinWonGame {Vinn vunnet parti}
translate W Lines {Variationer}
translate W ConfigureUCIengine {Konfigurera UCI-schackmotor}
translate W SpecificOpening {Specifik öppning}
translate W StartNewGame {Starta nytt parti}
translate W FixedLevel {Fast nivå}
translate W Opening {Öppning}
translate W RandomLevel {Slumpmässig nivå}
translate W StartFromCurrentPosition {Starta från den aktuella positionen}
translate W FixedDepth {Fast sökdjup}
translate W Nodes {Noder} 
translate W Depth {Sökdjup}
translate W Time {Tid} 
translate W SecondsPerMove {Sekunder per drag}
translate W Engine {Schackmotor}
translate W TimeMode {Tidsmod}
translate W TimeBonus {Tid + bonus}
translate W TimeMin {min}
translate W TimeSec {sek}
translate W AllExercisesDone {Alla övningar gjorda}
translate W MoveOutOfBook {Drag utanför bok}
translate W LastBookMove {Sista bokdrag}
translate W AnnotateSeveralGames {Kommentera flera partier\nfrån aktuellt till :}
translate W FindOpeningErrors {Hitta öppningsfel}
translate W MarkTacticalExercises {Markera taktiska övningar}
translate W UseBook {Använd bok}
translate W MultiPV {Flera variationer}
translate W Hash {Hashminne}
translate W OwnBook {Använd schackmotorbok}
translate W BookFile {Öppningsbok}
translate W AnnotateVariations {Kommentera variationer}
translate W ShortAnnotations {Korta kommentarer}
translate W addAnnotatorTag {Lägg till kommentator-tagg}
translate W AddScoreToShortAnnotations {Lägg till värdering till korta kommentarer}
translate W Export {Exportera}
translate W BookPartiallyLoaded {Bok delvis öppnad}
translate W Calvar {Beräkning av variationer}
translate W ConfigureCalvar {Konfiguration}
translate W Reti {Reti}
translate W English {Engelsk}
translate W d4Nf6Miscellaneous {1.d4 Nf6 Diverse}
translate W Trompowsky {Trompowsky}
translate W Budapest {Budapest}
translate W OldIndian {Gammalindisk}
translate W BenkoGambit {Benko-gambit}
translate W ModernBenoni {Modern Benoni}
translate W DutchDefence {Holländskt försvar}
translate W Scandinavian {Skandinaviskt}
translate W AlekhineDefence {Alekhines försvar}
translate W Pirc {Pirc}
translate W CaroKann {Caro-Kann}
translate W CaroKannAdvance {Caro-Kann Avancemang}
translate W Sicilian {Siciliansk}
translate W SicilianAlapin {Siciliansk Alapin}
translate W SicilianClosed {Stängd Siciliansk}
translate W SicilianRauzer {Siciliansk Rauzer}
translate W SicilianDragon {Siciliansk Drake}
translate W SicilianScheveningen {Siciliansk Scheveningen}
translate W SicilianNajdorf {Siciliansk Najdorf}
translate W OpenGame {Öppet parti}
translate W Vienna {Wiener}
translate W KingsGambit {Kungsgambit}
translate W RussianGame {Ryskt parti}
translate W ItalianTwoKnights {Italianskt/Tvåspringar}
translate W Spanish {Spanskt}
translate W SpanishExchange {Spanskt Avbytesvariant}
translate W SpanishOpen {Öpett Spanskt}
translate W SpanishClosed {Stängt Spanskt}
translate W FrenchDefence {Franskt Försvar}
translate W FrenchAdvance {Fransk Avancemang}
translate W FrenchTarrasch {Fransk Tarrasch}
translate W FrenchWinawer {Fransk Winawer}
translate W FrenchExchange {Fransk Avbytesvariant}
translate W QueensPawn {Dambonde}
translate W Slav {Slav}
translate W QGA {Antagen Damgambit}
translate W QGD {Avböjd Damgambit}
translate W QGDExchange {Avböjd Damgambit Avbytesvariant}
translate W SemiSlav {Semi-Slav}
translate W QGDwithBg5 {Avböjd Damgambit med Bg5}
translate W QGDOrthodox {Avböjd Damgambit Ortodox}
translate W Grunfeld {Grünfeld}
translate W GrunfeldExchange {Grünfeld Avbytesvariant}
translate W GrunfeldRussian {Grünfeld Ryskt}
translate W Catalan {Catalan}
translate W CatalanOpen {Öppen Catalan}
translate W CatalanClosed {Stängd Catalan}
translate W QueensIndian {Damindiskt}
translate W NimzoIndian {Nimzo-Indiskt}
translate W NimzoIndianClassical {Nimzo-Indiskt Klassiskt}
translate W NimzoIndianRubinstein {Nimzo-Indiskt Rubinstein}
translate W KingsIndian {Kungsindiskt}
translate W KingsIndianSamisch {Kungsindiskt Sämisch}
translate W KingsIndianMainLine {Kungsindiskt Huvudvariation}
translate W ConfigureFics {Konfigurera FICS}
translate W FICSGuest {Logga in som Gäst}
translate W FICSServerPort {Server-port}
translate W FICSServerAddress {IP-adress}
translate W FICSRefresh {Uppdatera}
translate W FICSTimesealPort {Timeseal-port}
translate W FICSSilence {Tystnad}
translate W FICSOffers {Erbjuder}
translate W FICSConsole {Konsol}
translate W FICSGames {Partier}
translate W FICSUnobserve {Sluta observera parti}
translate W FICSProfile {Visa din historik och profil}
translate W FICSRelayedGames {Återutsända partier}
translate W FICSFindOpponent {Hitta motståndare}
translate W FICSTakeback {Ta tillbaka}
translate W FICSTakeback2 {Ta tillbaka 2}
translate W FICSInitTime {Initialtid (minuter)}
translate W FICSIncrement {Inkrementering (sekunder)}
translate W FICSRatedGame {Ratingparti}
translate W FICSAutoColour {automatisk}
translate W FICSManualConfirm {bekräfta manuellt}
translate W FICSFilterFormula {Filtrera med formel}
translate W FICSIssueSeek {Sökfråga}
translate W FICSChallenge {Utmana}
translate W FICSAccept {acceptera}
translate W FICSDecline {avböj}
translate W FICSColour {färg}
translate W FICSSend {skicka}
translate W FICSConnect {Anslut}
translate W FICSdefaultuservars {Använd standardvariabler}
translate W FICSObserveconfirm {Vill du observera parti} 
# ====== TODO To be translated ======
translate W FICSpremove {Enable premove}
translate W GameReview {Partiöversikt}
translate W GameReviewTimeExtended {Utökad tid}
translate W GameReviewMargin {Felmarginal}
translate W GameReviewAutoContinue {Fortsätt automatiskt när drag är korrekt}
translate W GameReviewReCalculate {Använd utökad tid}
translate W GameReviewAnalyzingMovePlayedDuringTheGame {Analysera drag spelat under partiet}
translate W GameReviewAnalyzingThePosition {Analysera positionen}
translate W GameReviewEnterYourMove {Ange ditt drag}
translate W GameReviewCheckingYourMove {Kontrollerar ditt drag}
translate W GameReviewYourMoveWasAnalyzed {Ditt drag analyserades}
translate W GameReviewYouPlayedSameMove {Du speladade samma drag som i partiet}
translate W GameReviewScoreOfYourMove {Värdering av ditt drag}
translate W GameReviewGameMoveScore {Partidragets värdering}
translate W GameReviewEngineScore {Schackmotorns värdering}
translate W GameReviewYouPlayedLikeTheEngine {Du spelade som schackmotorn}
translate W GameReviewNotEngineMoveButGoodMove {Inte schackmotordraget, men det är också ett bra drag}
translate W GameReviewMoveNotGood {Detta drag är inte bra, värderingen är}
translate W GameReviewMovesPlayedLike {Drag spelade som}
translate W GameReviewMovesPlayedEngine {Drag apelade som schackmotorn}
translate W CCDlgConfigureWindowTitle {Konfigurera Korrespondensschack}
translate W CCDlgCGeneraloptions {Generella inställningar}
translate W CCDlgDefaultDB {Standarddatabas:}
translate W CCDlgInbox {Inkorg (sökväg):}
translate W CCDlgOutbox {Utkorg (sökväg):}
translate W CCDlgXfcc {Xfcc-Konfiguration:}
translate W CCDlgExternalProtocol {Extern Protokollhanterare (t.ex. Xfcc)}
translate W CCDlgFetchTool {Hämtningsverktyg:}
translate W CCDlgSendTool {Sändverktyg:}
translate W CCDlgEmailCommunication {eMail-kommunikation}
translate W CCDlgMailPrg {eMail-program:}
translate W CCDlgBCCAddr {(B)CC-address:}
translate W CCDlgMailerMode {Mod:}
translate W CCDlgThunderbirdEg {t.ex. Thunderbird, Mozilla Mail, Icedove...}
translate W CCDlgMailUrlEg {t.ex. Evolution}
translate W CCDlgClawsEg {t.ex Sylpheed Claws}
translate W CCDlgmailxEg {t.ex. mailx, mutt, nail...}
translate W CCDlgAttachementPar {Bilage-parameter:}
translate W CCDlgInternalXfcc {Använd internt Xfcc-stöd}
translate W CCDlgConfirmXfcc {Bekräfta drag}
translate W CCDlgSubjectPar {Ämnes-parameter:}
translate W CCDlgDeleteBoxes {Töm In-/Utkorg} 
translate W CCDlgDeleteBoxesText {Vill du verkligen tömma dina In- och Utkorgskataloger för Korrespondensschack? Detta kräver en ny synkronisering för att visa den seanaste statusen på dina partier} 
translate W CCDlgConfirmMove {Bekräfta drag} 
translate W CCDlgConfirmMoveText {Om du bekräftar, kommer följande drag och kommentar att skickas till servern:} 
translate W CCDlgDBGameToLong {Inkonsekvent huvudvariation}
translate W CCDlgDBGameToLongError {Huvudvariationen i din databas är längre än partiet i din Inkorg. Om Inkorgen innehåller aktuella partier, dvs precis efter en synkronisering, lades några drag felaktigt till i huvudvariationen i databasen.\nI detta fall var god och förkorta huvudvariationen till (maximalt) drag\n}
translate W CCDlgStartEmail {Starta ett nytt eMail-parti}
translate W CCDlgYourName {Ditt Namn:}
translate W CCDlgYourMail {Din eMail-address:}
translate W CCDlgOpponentName {Motståndarnamn:}
translate W CCDlgOpponentMail {Motståndarens eMail-address:}
translate W CCDlgGameID {Parti-ID (unikt):}
translate W CCDlgTitNoOutbox {Scid: Korrespondensschackutkorg}
translate W CCDlgTitNoInbox {Scid: Korrespondensschackinkorg}
translate W CCDlgTitNoGames {Scid: Inga Korrespondensschackpartier}
translate W CCErrInboxDir {Korrespondensschacksinkorgskatalog:}
translate W CCErrOutboxDir {Korrespondensschacksutkorgskatalog:}
translate W CCErrDirNotUsable {finns inte eller är inte möjlig att använda!\nVar god och kontrollera och korrigera inställningarna.}
translate W CCErrNoGames {innehåller inga partier!\nVar god och hämta dem först.}
translate W CCDlgTitNoCCDB {Scid: Ingen Korrespondensdatabas}
translate W CCErrNoCCDB {Ingen Databas av typ 'Korrespondens' är öppnad. Var god och öppna en innan du använder Korrespondensschackfunktioner.}
translate W CCFetchBtn {Hämta partier från servern och bearbeta Inkorgen}
translate W CCPrevBtn {Gå till tidigare parti}
translate W CCNextBtn {Gå till nästa parti}
translate W CCSendBtn {Skicka drag}
translate W CCEmptyBtn {Töm Inkorgen och Utkorgen}
translate W CCHelpBtn {Hjälp med ikoner och statusindikatorer.\nFör generell Hjälp tryck på F1!}
translate W CCDlgServerName {Servernamn:}
translate W CCDlgLoginName  {Inloggningsnamn:}
translate W CCDlgPassword   {Lösenord:}
# ====== TODO To be translated ======
translate W CCDlgShowPassword {Show password}
translate W CCDlgURL        {Xfcc-URL:}
translate W CCDlgRatingType {Ratingtyp:}
translate W CCDlgDuplicateGame {Icke unikt parti-ID}
translate W CCDlgDuplicateGameError {Detta parti finns fler än en gång i din databas. Var god och ta bort alla dubbletter och komprimera din partifil (Arkiv/Underhåll/Komprimera Databas).}
translate W CCDlgSortOption {Sorterar:}
translate W CCDlgListOnlyOwnMove {Bara partier där jag är vid draget}
translate W CCOrderClassicTxt {Plats, Evenemang, Runda, Resultat, Vit, Svart}
translate W CCOrderMyTimeTxt {Min klocka}
translate W CCOrderTimePerMoveTxt {Tid per drag till nästa tidskontroll}
translate W CCOrderStartDate {Startdatum}
translate W CCOrderOppTimeTxt {Motståndarens klocka}
translate W CCDlgConfigRelay {Konfigurera ICCF-observationer}
translate W CCDlgConfigRelayHelp {Gå till partisidan på http://www.iccf-webchess.com och visa partiet att observera.  Om du ser schackbräder kopiera webadressen från din webläsare till listan nedanför. Bara en webadress per rad!\nExempel: http://www.iccf-webchess.com/MakeAMove.aspx?id=266452}
translate W ExtHWConfigConnection {Konfigurera extern hårdvara}
translate W ExtHWPort {Port}
translate W ExtHWEngineCmd {Schackmotorkommando}
translate W ExtHWEngineParam {Schackmotorparameter}
translate W ExtHWShowButton {Visa knapp}
translate W ExtHWHardware {Hårdvara}
translate W ExtHWNovag {Novag Citrine}
translate W ExtHWInputEngine {Inmatningsschackmotor}
translate W ExtHWNoBoard {Inget bräde}
translate W NovagReferee {Nedteckna bara dragen}
translate W IEConsole {Inmatningsschackmotorkonsol}
translate W IESending {Drag skickade för}
translate W IESynchronise {Synkronisera}
translate W IERotate  {Rotera}
translate W IEUnableToStart {Kunde inste starta Inmatningsschackmotor:}
translate W DoneWithPosition {Klar med position}
translate W Board {Bräde}
translate W showGameInfo {Visa partiinformation}
translate W autoResizeBoard {Ändra storleken på brädet automatiskt}
translate W DockTop {Drag överst}
translate W DockBottom {Drag nederst}
translate W DockLeft {Drag till vänster}
translate W DockRight {Drag till höger}
translate W Undock {Avdocka}
translate W ChangeIcon {Ändra ikon...}
# ====== TODO To be translated ======
translate W NewGameListWindow {New Game List Window}
# ====== TODO To be translated ======
translate W LoadatStartup {Load at startup}
# ====== TODO To be translated ======
translate W ShowHideDB {Show/Hide databases}
# ====== TODO To be translated ======
translate W ChangeFilter {Change filter}
# ====== TODO To be translated ======
translate W ChangeLayout {Load/Save/Change sorting criteria and column layout}
# ====== TODO To be translated ======
translate W ShowHideStatistic {Show/Hide statistics}
# ====== TODO To be translated ======
translate W BoardFilter {Show only games that matches the current board position}
# ====== TODO To be translated ======
translate W CopyGameTo {Copy Game to}
# ====== TODO To be translated ======
translate W FindBar {Find Bar}
# ====== TODO To be translated ======
translate W FindCurrentGame {Find current game}
# ====== TODO To be translated ======
translate W DeleteGame {Delete game}
# ====== TODO To be translated ======
translate W UndeleteGame {Undelete game}
# ====== TODO To be translated ======
translate W ResetSort {Reset sort}
# ====== TODO To be translated ======
translate W ConvertNullMove {Convert null moves to comments}
# ====== TODO To be translated ======
translate W SetupBoard {Setup Board}
# ====== TODO To be translated ======
translate W Rotate {Rotate}
# ====== TODO To be translated ======
translate W SwitchColors {Switch colors}
# ====== TODO To be translated ======
translate W FlipBoard {Flip Board}
# ====== TODO To be translated ======
translate W ImportPGN {Import PGN game}
# ====== TODO To be translated ======
translate W ImportingFiles {Importing PGN files in}
# ====== TODO To be translated ======
translate W ImportingFrom {Importing from}
# ====== TODO To be translated ======
translate W ImportingIn {Import games in}
# ====== TODO To be translated ======
translate W UseLastTag {Use last\ngame's tags}
# ====== TODO To be translated ======
translate W Random {Random}
# ====== TODO To be translated ======
translate W BackToMainline {Go back to mainline}
# ====== TODO To be translated ======
translate W LeaveVariant {Leave variant}
# ====== TODO To be translated ======
translate W Autoplay {Autoplay}
# ====== TODO To be translated ======
translate W ShowHideCoords {Show/Hide Coord.}
# ====== TODO To be translated ======
translate W ShowHideMaterial {Show/Hide Material}
# ====== TODO To be translated ======
translate W FullScreen {Full Screen}
# ====== TODO To be translated ======
translate W FilterStatistic {Filter Statistics}
# ====== TODO To be translated ======
translate W MakeCorrections {Make Corrections}
# ====== TODO To be translated ======
translate W Surnames {Surnames}
# ====== TODO To be translated ======
translate W Ambiguous {Ambiguous}
# ====== TODO To be translated ======
translate W OptionsToolbar "Verktygsfält"
translate W OptionsBoard "Brädet"
# ====== TODO To be translated ======
translate W OptionsMenuColor "Menu colors"
translate W OptionsBoardSize "Brädstorlek"
translate W OptionsBoardPieces "Pjäsutseende"
# ====== TODO To be translated ======
translate W OptionsInternationalization "Internationalization"
# ====== TODO To be translated ======
translate W MenuColorForeground "Textcolor"
# ====== TODO To be translated ======
translate W MenuColorBackground "Background"
# ====== TODO To be translated ======
translate W MenuColorSelect "Select"
# ====== TODO To be translated ======
translate W OptionsTablebaseDir "Select up to 4 table base folders:"
}

############################################################
#
# Swedish tips section:

set tips(W) {
  {
    Scid har över 30 <a Index>hjälpsidor</a>, och i de flesta fönster kan du
    genom att trycka <b>F1</b> få hjälp om det fönster du för tillfället har
    aktivt.
  }
  {
    Vissa fönster (t ex partiinformation och <a Switcher>databasväljaren</a>) 
    har en meny för höger musknapp. Prova att högerklicka i varje fönster så
    ser du vilka som har det och vilka funktioner du kommer åt den vägen.
  }
  {
    Du kan ange drag på mer än ett sätt, så du kan välja vilket som passar dig
    bäst. Du kan använda musen (med eller utan dragförslag) eller tangentbordet
    (med eller utan autokomplettering). Läs hjälpsidan för 
    <a Moves>att ange drag</a> för mer information.
  }
  {
    Om du har databaser du ofta öppnar så kan du lägga till ett 
    <a Bookmarks>bokmärke</a> för varje databas. Du kommer då att kunna öppna 
    databaserna snabbare via bokmärkesmenyn.
  }
  {
    Du kan se alla drag i det aktuella partiet (inklusive variationer och kommentarer)
    genom att öppna <a PGN>PGN-fönstret</a>.
    Du navigerar igenom partiet i PGN-fönstret genom att klicka på valfritt drag med
    vänster musknapp. Genom att klicka på draget med mellan- eller högermusknapp så
    får du en förhandsvisning av just den ställningen.
  }
  {
    Du kan kopiera partier mellan databaserna i <a Switcher>databasväljaren</a> genom
    att dra-och-släppa dem mellan respektive databas.
  }
  {
    Scid kan öppna PGN filer, även de som är komprimerade med Gzip (dvs har en .gz filändelse). 
    PGN filer öppnas med enbart läsrättigheter, så om du vill redigera en PGN fil i Scid 
    måste du kopiera filen till en Scid databas med hjälp av <a Switcher>databasväljaren</a>.
  }
  {
    Om du ofta använder <a Tree>Trädfönstret</a> med stora databaser, är det värt att välja
    <b>Fyll cache fil</b> från Arkivmenyn i Trädfönstret. Detta innebär att trädstatistik för 
    många av de vanligare ställningarna sparas, vilket ger en snabbare trädåtkomst för databasen
    ifråga.
  }
  {
    <a Tree>Trädfönstret</a> kan visa alla drag från den aktuella ställningen. Men om också vill 
    se alla dragföljder som har lett till denna ställning kan du få fram det genom att skapa en
   <a OpReport>öppningsrapport</a>.
  }
  {
    Du kan vänster- eller högerklicka i en kolumn i <a GameList>partilistan</a> för att ändra 
    dess bredd.
  }
  {
    Med <a PInfo>spelarinformationen</a> aktuell (klicka på endera spelarens namn under brädet i 
    huvudfönstret för att få fram den) är det enkelt att <a Searches Filter>filtrera</a> fram partier
    av en särskild spelares enskilda resultat. Klicka bara på valfritt värde som har angivits med
    <red>röd text</red>.
  }
  {
    När du studerar öppningar kan det vara en poäng att markera valen <b>Bönder</b> eller <b>Filer</b> i 
    <a Searches Board>Sök aktuell position</a>. Genom dessa val kan du hitta andra öppningar som ger
    liknande bondestrukturer.
  }
  {
    Du kan högerklicka i partiinformationsdelen av huvudfönstret (under brädet) för att få fram en 
    kontextmeny. Du kan t ex göra så att Scid döljer nästa drag i ett parti, vilket kan vara användbart 
    om du vill träna genom att "gissa" nästa drag.
  }
  {
    Om du ofta <a Maintenance>underhåller</a> stora databaser kan du utföra flera underhållsuppgifter 
    vid samma tillfälle via <a Maintenance Cleaner>databasverktyget</a>.
  }
  {
    Om du har en stor databas där de flesta partierna har ett evenemangsdatum och du vill ha partierna i 
    datumordning bör du överväga att <a Sorting>sortera</a> den på evenemangsdatum i första hand och 
    evenemang i andra hand, istället för datum och evenemang, då detta kommer att hålla turneringspartierna
    från olika datum samman. (Under förutsättning att alla partier har samma evenemangsdatum naturligtvis).
    Alternativt kan du se till att fälten evenemang, rond och datum är så enhetliga och korrekta som möjligt.
    (ms).
  }
  {
    Det kan vara en bra ide att <a Maintenance Spellcheck>stavningskontrollera</a> din databas innan du 
    <a Maintenance Twins>raderar dubbletter</a> då Scid har större möjlighet att hitta dubbletterna och 
    markera dessa för borttagning.
  }
  {
    <a Flags>Flaggor</a> är användbara för att markera upp partier med karaktäristika du vill söka på senare,
    såsom bondeställning, taktiska finesser, osv. Du kan söka på flaggor när du söker på fält i partihuvudet.
  }
  {
    Om du går igenom ett parti, och helt enkelt vill testa en ny variation utan att förändra partiet i sig, kan 
    du slå på Försöksläget (Trial mode) genom att trycka <b>Ctrl+Mellanslag</b> eller från verktygsraden. Återgå till
    ursprungspartiet när du är klar.
  }
  {
    Om du vill hitta det mest betydelsefulla partiet (med högst rankade spelare) i en viss position kan du
    öppna <a Tree>Trädfönstret</a> och i denna öppna listan med de bästa partierna. I trädfönstret kan du till
    och med begränsa partierna till endast ett särskilt resultat.
  }
  {
    Ett bra sätt att studera en öppning är att i en stor databas slå på träningsläget i <a Tree>Trädfönstret</a>, 
    och sedan spela igenom databasen för att se vilka varianter som förekommer oftast.
  }
  {
    Om du har två databaser öppna, och vill ha ett variantträd att studera medan du går igenom ett parti
    i den andra databasen kan du <b>låsa</b> trädet i den databasen och sedan byta till den andra.
  }
  {
    <a Tmt>Turneringssökaren (tournament finder)</a> är inte bara användbar för att lokalisera en särskild 
    turnering. Du kan också använda den för att söka efter turneringar en specifik spelare nyligen deltagit 
    i, eller att bläddra genom turneringar som genomförts i ett visst land.
  }
  {
    Det finns ett antal vanliga ställningstyper definierade i <a Searches Material>Sök material/ställning</a>
    fönstret som du kan ha nytta av när du studerar öppningar och mittspel.
  }
  {
    När du söker på <a Searches Material>material eller ställning</a> kan det ofta vara fördelaktigt att begränsa
    sökningen till sådana partier där ställningen eller materialet förekommit i åtminstone några drag. Du slipper
    du få med träffar där situationen du söker uppkom helt tillfälligt.
  }
  {
    Om du har en viktig databas du inte vill radera av misstag kan du välja  <b>Enbart läsbar</b> från <b>Arkiv</b>
    menyn efter att du har öppnat den. Alternativt kan du sätta dess filrättigheter till enbart läsrättigheter.
  }
  {
    Om du använder XBoard eller WinBoard (eller något annat program som kan hantera FEN notation via urklippshanteraren)
    och vill kopiera den aktuella ställningen från ditt program är det snabbaste sättet att göra det så här:
    Välj <b>Copy Position</b> från <b>File</b> menyn i Xboard/Winboard, välj sedan <b>Klistra in utgångsställning</b>
    i Redigera menyn i Scid.
  }
  {
    I <a Searches Header>Sök i huvud</a>, är spelare-, evenemang-, plats-, och rondnamn okänsliga för stora eller små
    bokstäver och ger träffar varhelst de finns i ett namn. Om du vill kan du ange att du istället vill att sökningen <b>ska</b>
    ta versaler/gemener i beaktande. Genom att använda jokertecken inom citationstecken (där "?" = motsvarar obestämt
    enskilt tecken och "*" = noll eller flera tecken). Om du exempelvis anger "*BEL" (med citationstecken) i det platsfältet
    hittar du alla partier spelade i Belgien, men exkluderar de som spelats i Belgrad.
  }
  {
    Om du vill redigera ett drag i ett parti utan att förlora alla de drag som spelats efter detta kan du öppna
    <a Import>Importera ett parti i PGN-format</a> fönstret i Verktygsmenyn. Klicka där på <b>Klistra in aktuellt parti</b> 
    , redigera partiet och avsluta med <b>Importera</b>.
  }
  {
    Om du har en ECO klassificeringsfil laddad, kan du nå den mest exakt klassificerade ställningen för det aktuella partiet
    genom att välja <b>Identifiera öppning</b> i <b>Partier</b> menyn (kortkommando: Ctrl+Shift+D).
  }
  {
    När du vill se hur stor en fil är, eller vill se när den senast redigerades innan du öppnar den kan du använda
    <a Finder>file finder</a> (Arkiv - Sök filer).
  }
  {
    En <a repetoire>repetoarfil</a> är ett utmärkt sätt att hålla koll på dina favoritöppningar, eller hitta partier där
    där de har spelats. När du väl har skapat en repetoarfil kan du genomsöka nya filer utifrån repetoarfilen, och titta 
    igenom alla partier med just dina öppningar.
  }
  {
    Genom att skapa en <a OpReport>Öppningsrapport</a> har du en utmärkt möjlighet att lära dig en ny spelöppning. Du kan få
    information om resultat, hur remiaktig den är, vilka vanliga positionella teman som dyker upp, och mycket mer.
  }
  {
    Du kan kommentera den aktuella ställningen med de vanligaste symbolerna (!, !?, +=, etc) utan att behöva använda 
    <a Comment>kommentarseditorn<a>. Där du exempelvis vill ange ett bra drag skriver du "!" och trycker sedan ENTER
    så läggs "!" symbolen till draget. Se även hjälpsidan <a Moves>ange drag</a> för mer detaljerad information.
  }
  {
    Om du bläddrar igenom öppningarna i en databas i <a Tree>trädfönstret</a>, får du en användbar överblick över hur
    väl öppningen fungerar i sentida partier mellan högrankade spelare om du öppnar statistikfönstret (kortkommando: Ctrl+I).
  }
  {
    Du ändrar enkelt huvudfönstrets brädstorlek genom att hålla nere <b>Ctrl</b> och <b>Shift</b> tangenterna, samtidigt
    som du trycker höger- eller vänster piltangent.
  }
  {
    Efter genomförd <a Searches>sökning</a>, är det enkelt att navigera genom urvalet genom att hålla nere <b>Ctrl</b> 
    tangenten samtidigt som du trycker upp- eller nerpiltangenterna för att gå till föregående eller nästa parti i 
    <a Searches Filter>urvalet</a>.
  }
{
  Du kan relativt enkelt rita pilar och färga rutor till dina kommentarer. Öppna <b>Kommentarseditorn</b>, klicka på <b>Infoga symbol</b> och välj önskad färg. Om du nu klicka på en första ruta, och därefter klickar på en andra så dras en pil i önskad färg från den första till den andra rutan. Klickar du bara på en ruta, blir den färgad.
  }
}

##########
#
# ECO Opening name translations:

translateECO W {
  Accelerated {, Accelererad}
  {: Accelerated} Accelererad
  Accepted {, Antagen}
  {: Accepted} Antagen
  Advance Avancemang
  {as Black} {som Svart}
  Attack Angrepp
  Bishop Löpare
  Bishop's Löpar
  Classical Klassisk
  Closed Stängd
  {Closed System} {Stängt system}
  Counterattack Motangrepp
  Countergambit Motgambit
  Declined {, Avböjd}
  Defence Försvar
  deferred förvägrad
  Deferred {, förvägrad}
  Early Tidig
  Exchange Avbyte
  Game Parti
  Improved förbättrad
  King's Kung
  Knight Springar
  Line Variant
  {Main Line} Huvudvariant
  Open Öppen
  Opening Öppning
  Queen's Dam
  Queenswap Damavbyte
  Symmetrical Symmetrisk
  Variation Variation
  Wing Flygel
  with med
  without utan

  Alekhine Aljechin
  Averbakh Averbach
  Botvinnik Botvinnik
  Chigorin Chigorin
  Polugaevsky Polugajevski
  Rauzer Rauzer
  Sveshnikov Sveschnikov

  Austrian Österrikisk
  Berlin Berlin
  Bremen Bremen
  Catalan Katalansk
  Czech Tjeckisk
  Dutch Holländsk
  English Engelsk
  French Fransk
  Hungarian Ungersk
  Indian Indisk
  Italian Italiensk
  Latvian Lettisk
  Meran Meran
  Moscow Moskva
  Polish Polsk
  Prague Prag
  Russian Rysk
  Scandinavian Skandinavisk
  Scheveningen Scheveningen
  Scotch Skottsk
  Sicilian Siciliansk
  Slav Slavisk
  Spanish Spansk
  Swedish Svensk
  Swiss Schweizisk
  Vienna Wiener
  Yugoslav Jugoslavisk

  {Accelerated Fianchetto} {Accelererad Fianchetto}
  {Accelerated Pterodactyl} {Accelererad Pterodactyl}
  {Alekhine Defence} Aljechins Försvar
  {Alekhine Variation} Aljechin-Variant
  {Alekhine: Sämisch Attack} {Aljechin: Wiener-system (Sämisch-Angrepp)}
  {Anderssen Opening} Anderssen-Öppning
  {Anti-King's Indian} Anti-Kungsindisk
  {Austrian Attack} {Österrikiskt Angrepp}
  {Benko Gambit} Volga-Gambit
  {Benko Opening} Benkö-Öppning
  {Berlin Defence} {Berlinförsvar}
  Chameleon Chamaleon
  Chelyabinsk Chelyabinsk
  {Classical Defence} {Klassiskt försvar}
  {Spanish: Classical Defence} {Spanskt: Klassiskt (Cordel-) försvar}
  {Classical Exchange} {Klassiskt Avbyte}
  {Classical Variation} {Klassisk Variant}
  {Closed Berlin} {Stängd Berlin}
  {Open Berlin} {Öppen Berlin}
  {Bird's,} {Bird,}
  {Bird's Defence} Birds försvar
  {Bird's Deferred} {Förvägrad Bird}
  {Bishop's Opening} Löparspel
  {Botvinnik System} Botvinnik-system
  {Central Variation} Centralvariant
  {Centre Attack} Centrumsangrepp
  {Centre Game} Mittgambit
  {Danish Gambit} {Nordisk Gambit}
  Dragon Drake
  {Dutch Variation} {Holländsk Variant}
  {Early Exchange} {Tidigt Avbyte}
  {Early Queenswap} {Tidigt Dambyte}
  {English Attack} {Engelskt Angrepp}
  {English: King's} {Engelskt: Kungsbonde}
  {English Variation} {Engelsk Variant}
  {Englund Gambit} Englunds Gambit
  {Exchange Variation} Avbytesvariant
  {Fianchetto Variation} Fianchettovariant
  {Flohr Variation} Flohr-Variant
  {Four Knights} Fyrspringar
  {Four Knights Game} Fyrspringarspel
  {Four Pawns} Fyrbonde
  {Four Pawns Attack} Fyrbondeangrepp
  {French Variation} {Fransk Variant}
  {From Gambit} {From-Gambit}
  {Goring Gambit} {Göring-Gambit}
  {Grob Gambit} {Grobs Gambit}
  {Hungarian Defence} {Ungerskt försvar}
  {Indian Variation} {Indisk Variant}
  {Italian Game} {Italienskt Parti}
  KGD {Avböjd Kungssgambit}
  {Classical KGD} {Klassisk avböjd Kungsgambit}
  {Keres Variation} Keres-Variant
  KGA {Antagen Kungsgambit}
  {KGA: Bishop's Gambit} Kungslöpargambit
  {KGA: King's Knight Gambit} Kungsspringargambit
  {King's Gambit} Kungsgambit
  {King's Gambit Accepted} {Antagen Kungsgambit}
  {King's Gambit Accepted (KGA)} {Antagen Kungsgambit}
  {King's Indian} Kungsindisk
  KIA {Kungsindiskt Angrepp}
  {King's Knight Gambit} Kungsspringargambit
  {King's Pawn} Kungsbonde
  {Lasker Variation} {Lasker-Variant}
  {Latvian Gambit} {Lettisk Gambit}
  {Maroczy Bind} {Maroczy-bindning}
  {Marshall Variation} Marshall-Variant
  {Modern Attack} {Modernt Angrepp}
  {Modern Steinitz} {Modern Steinitz}
  {Modern Variation} {Modern Variant}
  {Moscow Variation} {Moskva-variant}
  Nimzo-Indian Nimzoindisk
  {Old Benoni} {Klassisk Benoni}
  {Old Indian} Gammalindisk
  {Old Indian Attack} {Gammalindisk Indisk attack}
  {Old Steinitz} Gammal Steinitz-öppning
  {Open Game} {Öppet Parti}
  {Poisoned Pawn} {Förgiftad Bonde}
  {Polish Variation} {Polsk Variant}
  {Polugaevsky Variation} {Polugajevski-Variant}
  {Queen's Gambit} Damgambit
  {Queen's Gambit Accepted} {Antagen Damgambit}
  QGA {Antagen Damgambit}
  {Queen's Gambit Accepted (QGA)} {Antagen Damgambit}
  {Reversed QGA} {Omvänd antagen Damgambit}
  QGD {Avböjd Damgambit}
  {Queen's Gambit Declined (QGD)} {Avböjd Damgambit}
  {Reversed QGD} {Omvänd avböjd Damgambit}
  {Queen's Indian} Damindisk
  {Queen's Pawn} Dambonde
  {Queen's Pawn Game} Dambondespel
  {Reversed Slav} {Omvänd Slavisk}
  {Rubinstein Variation} Rubinstein-Variant
  {Russian Game} {Ryskt Parti}
  {Russian Game (Petroff Defence)} {Ryskt Parti}
  {Russian-Three Knights Game} {Ryskt trespringarspel}
  {Scandinavian (Centre Counter)} Skandinavisk
  Schliemann Jänisch
  {Schliemann (Jänisch)} {Jänisch-Gambit (Schliemann)}
  {Scotch Opening} {Skottsk öppning}
  {Sicilian Defence} {Sicilianskt försvar}
  {Sicilian Variation} {Siciliansk Variant}
  {Slav Defence} {Slaviskt försvar}
  Smith-Morra Morra
  {Smith-Morra Accepted} {Antagen Morra-Gambit}
  {Smith-Morra Gambit} Morra-Gambit
  {Spanish (Ruy Lopez)} {Spanskt Parti}
  {Start position} Utgångsställning
  {Steinitz Deferred} Förvägrad Rubinstein
  {Swedish Variation} {Svensk Variant}
  {Swiss Variation} {Schweizisk Variant}
  {Tarrasch's Gambit} {Tarrasch-Gambit}
  {Three Knights} Trespringar
  {3 Knights} Trespringar
  {Three Knights Game} Trespringarspel
  {Three Pawns Attack} Trebondeangrepp
  {Two Knights} Tvåspringar
  {Two Knights Defence} Tvåspringar-försvar
  {Two Knights Variation} Tvåspringar-variant
  {Two Pawns} Tvåbonde
  {Two Pawns Attack} Tvåbondeangrepp
  {Wing Gambit} Flygel-Gambit
  {Yugoslav Attack} {Jugoslaviskt Angrepp}
}

############################################################
#
# Swedish help pages:

# Basic help pages completed, no links no other sections.

##########
# Contents
#
set helpTitle(W,Contents) "Innehållsförteckning"
set helpText(W,Contents) {<h1>Hjälp med Scid: Innehållsförteckning</h1>

<h3>Komma igång och allmänt om Scid</h3>
<p>Följande texter och dess länkar är under översättning. Detta påbörjades i Scid 3.4 beta 1 (2002 07 27). 
 /Martin S.</p>
Detta betyder att menyerna är översatta. Rubriktexterna bör kunna vara översatta till Scid 3.4 men alla länkar
från rubriktexterna är förmodligen inte det. /Martin S.</p>

<ul>
<li><a Guide><b>Komma igång</b> med Scid</a> <red>(Läs detta först)</red></li>
<li><a Hints><b>Tips</b> om hur du får ut mer av Scid</a></li>
<li><a MainWindow><b>Huvudfönstret</b></a></li>
<li><a Menus><b>Menyerna</b></a> <red>(Uppdaterad!)</red></li>
<li><a Moves>Ange <b>drag</b></a> <red>(Uppdaterad!)</red></li>
<li><a Searches><b>Söka</b> i Scid</a></li>
<li><a Clipbase>Att använda <b>Clipbase (Urklippsdatabasen)</b></a></li>
<li><a Annotating><b>Kommentera partier</b></a> <red>(Ny!)</red></li>
</ul>

<h4>Andra Scid-fönster</h4>

<ul>
<li><a Analysis><b>Analysfönstret</a></li>
<li><a Book><b>Bokfönstret</b></a></li>
<li><a CalVar><b>Beräkning av variationer-fönstret</b></a></li>
<li><a Comment><b>Kommentareditor</b> </a></li>
<li><a Crosstable><b>Korstabellfönster</a></li>
<li><a Switcher><b>Databasväxlaren</b> </a></li>
<li><a Email><b>Email-fönster</a></li>
<li><a Finder><b>Sök filer</b> </a></li>
<li><a GameList><b>Partilistan</b> </a></li>
<li><a Import><b>Importfönstret</a></li>
<li><a OpeningTrainer><b>Öppningstränarfönstret</b>  </a></li>
<li><a Reports><b>Rapporter</b></a> <red>(Uppdaterad!)</red></li>
<li><a PGN><b>PGN-fönster</a></li>
<li><a PTracker><b>Pjässpåraren</b></a></li>
<li><a PList><b>Spelarlistan</b></a></li>
<li><a PInfo><b>Spelarinfo</b> </a></li>
<li><a Repertoire><b>Repetoar-Editor</a></li>
<li><a TacticalGame><b>Taktiskt partifönstret</b> </a></li>
<li><a Tmt><b>Sök turneringar</a></li>
<li><a Tree><b>Trädfönstret</a></li>
<li><a Graphs><b>Graffönster</a></li>
<li><a TB>Slutspelstabeller</a><red>(Uppdaterad!)</red></li>
</ul>

<h4>Andra funktioner och information</h4>
<ul>
<li><a Bookmarks><b>Bokmärken</b></a></li>
<li><a Cmdline>Kommandorad-alternativ</a></li>
<li><a Compact><b>Komprimera databas</a></li>
<li><a Correspondence>Korrespondensschack</a></li>
<li><a Maintenance><b>Underhåll databas</a></li>
<li><a ECO><b>ECO</a></li>
<li><a EPD><b>EPD</a></li>
<li><a Export><b>Exportera partier</a> <red>(Uppdaterad!)</red></li>
<li><a Flags>Parti-markeringarna</a></li>
<li><a LaTeX>Scid och LaTeX</a></li>
<li><a Options><b>Alternativ</a><red>(Uppdaterad!)</red></li>
<li><a Sorting><b>Sortera databas</a></li>
<li><a Pgnscid><b>Pgnscid</a></li>
<li><a NAGs>NAG-värden</a></li>
<li><a Formats>Filformat</a></li>
<li><a Author>Kontaktinformation</a></li>
</ul>

<p><footer>(Uppdaterad: Scid 3.6.1, December 2004)</footer></p>
}

###############
### Topic Index

set helpTitle(W,Index) "Hjälp-index"
set helpText(W,Index) {<h1>Scid: Hjälp-index</h1>

<h3>A</h3>
<ul>
<li><a Options>Alternativ</a></li>
<li><a Analysis>Analysfönstret</a></li>
<li><a Moves>Ange drag</a></li>
</ul>

<h3>B</h3>
<ul>
<li><a CalVar>Beräkning av variationer-fönstret</a></li>
<li><a GameList Browsing>Bläddra partilistor</a></li>
<li><a Book>Bokfönstret</a></li>
<li><a Bookmarks>Bokmärken</a></li>
<li><a Tree Best>Bästa partier</a></li>
</ul>

<h3>D</h3>
<ul>
<li><a Sorting>Databassortering</a></li>
<li><a Maintenance>Databasunderhåll</a></li>
<li><a Switcher>Databasväxlaren</a></li>
<li><a Translator>Den svenska översättningen</a></li>
</ul>

<h3>E</h3>
<ul>
<li><a ECO>ECO</a></li>
<li><a ECO Browser>ECO-bläddrar</a>-fönster</li>
<li><a ECO Codes>ECO kodsystem</a></li>
<li><a Email>Email-fönster</a></li>
<li><a EPD>EPD-filer</a></li>
<li><a Export>Exportfilter</a></li>
<li><a Export>Exportera partier som text</a></li>
</ul>

<h3>F</h3>
<ul>
<li><a Formats>Filformat</a></li>
</ul>

<h3>G</h3>
<ul>
<li><a Graphs Filter>Graffilter</a></li>
<li><a Graphs>Graffönster</a></li>
</ul>

<h3>H</h3>
<ul>
<li><a MainWindow>Huvudfönstret</a></li>
</ul>

<h3>I</h3>
<ul>
<li><a Import>Importfönstret</a></li>
<li><a Contents>Innehållsförteckning</a></li>
</ul>

<h3>K</h3>
<ul>
<li><a Guide>Komma igång med Scid</a></li>
<li><a Cmdline>Kommandorad-alternativ</a></li>
<li><a Comment>Kommentareditor</a></li>
<li><a Annotating Null>Kommentera Nulldrag</a></li>
<li><a Annotating>Kommentera partier</a></li>
<li><a Compact>Komprimera databas</a></li>
<li><a Correspondence>Korrespondensschack</a></li>
<li><a Crosstable>Korstabellfönster</a></li>
<li><a Author>Kontaktinformation</a></li>
</ul>

<h3>L</h3>
<ul>
<li><a LaTeX>LaTeX</a> utdataformat</li>
</ul>

<h3>M</h3>
<ul>
<li><a Menus>Menyerna</a></li>
<li><a Menus File>Meny Arkiv</a></li>
<li><a Menus Edit>Meny Redigera</a></li>
<li><a Menus Game>Meny Partier</a></li>
<li><a Menus Search>Meny Sök</a></li>
<li><a Menus Windows>Meny Fönster</a></li>
<li><a Menus Tools>Meny Verktyg</a></li>
<li><a Menus Options>Meny Alternativ</a></li>
<li><a Menus Help>Meny Hjälp</a></li>
</ul>

<h3>N</h3>
<ul>
<li><a NAGs>NAG-värden</a></li>
</ul>

<h3>P</h3>
<ul>
<li><a GameList>Partilistan</a></li>
<li><a Flags>Partimarkeringarna</a></li>
<li><a PGN>PGN-fönster</a></li>
<li><a Pgnscid>Pgnscid</a></li>
<li><a PTracker>Pjässpåraren</a></li>
</ul>

<h3>R</h3>
<ul>
<li><a Reports Opening>Rapporter</a></li>
<li><a Graphs Rating>Ratinggraf</a></li>
<li><a Repertoire>Repetoar-Editor</a></li>
</ul>

<h3>S</h3>
<ul>
<li><a Analysis List>Schackmotorlista</a></li>
<li><a LaTeX>Scid och LaTeX</a></li>
<li><a Hints>Scidtips</a></li>
<li><a MainWindow Autoplay>Självspelsmod</a></li>
<li><a TB>Slutspelstabeller</a></li>
<li><a Guide>Snabbguide till att använda Scid</a></li>
<li><a Sorting>Sortera databas</a></li>
<li><a PInfo>Spelarinfo</a></li>
<li><a Reports Player>Spelarrapport</a></li>
<li><a Maintenance Spellcheck>Stavningskontrollera namn</a></li>
<li><a Finder>Sök filer</a></li>
<li><a Searches Filter>Sökfilter</a></li>
<li><a Searches Header>Sök partidata</a></li>
<li><a PList>Sök spelare</a></li>
<li><a Searches Board>Sök ställning</a></li>
<li><a Searches>Söka i Scid</a></li>
<li><a Searches Material>Sök material</a></li>
<li><a Tmt>Sök turneringar</a></li>
</ul>

<h3>T</h3>
<ul>
<li><a TacticalGame>Taktiskt partifönstret</a></li>
<li><a Moves Trial>Testa variant mod</a></li>
<li><a Hints>Tips</a></li>
<li><a Tree>Trädfönstret</a></li>
<li><a Options Fonts>Typsnitt</a></li>
</ul>

<h3>U</h3>
<ul>
<li><a Maintenance>Underhåll databas</a></li>
<li><a Maintenance Cleaner>Underhåll: Databasrensaren</a></li>
<li><a Maintenance Editing>Underhåll: Ändra spelarnamn</a></li>
<li><a Maintenance Twins>Underhåll: Ta bort dubbletter</a></li>
<li><a Maintenance Spellcheck>Underhåll: Stavningskontrollera spelarnamn</a></li>
<li><a Clipbase>Urklippsdatabasen</a></li>
</ul>

<h3>V</h3>
<ul>
<li><a Annotating Vars>Varianter</a></li>
</ul>

<h3>Ö</h3>
<ul>
<li><a ECO>Öppningsklassificering (ECO)</a></li>
<li><a Repertoire>Öppningsrepetoar</a></li>
<li><a Reports Opening>Öppningsrapport</a></li>
<li><a OpeningTrainer>Öppningstränarfönstret</a></li>
</ul>

<p><footer>(Uppdaterad: Scid 3.5, February 2003)</footer></p>
}

####################
### Quick Guide help:

set helpTitle(W,Guide) "Komma igång med Scid"
set helpText(W,Guide) {<h1>Komma igång med Scid</h1>
<p>
Scid är en schackdatabashanterare som du kan använda till att bläddra och <a Searches>söka</a> (enligt kriterier du ställer upp) igenom 
databaser med schackpartier. Du kan också redigera de partier du vill.
</p>
<p>
Scid använder sitt eget <a Formats>databasformat</a> baserat på tre filer. Det är mycket kompakt och snabbt, men kan konvertera såväl 
till som från PGN (Portable Game Notation) standarden om man så önskar. Scids <a PGN>PGN-fönster</a> visar texten till det aktuella
partiet i PGN-format.
</p>
<p>
Du kan använda Scid till att lägga till och ta bort partier i en databas genom att använda såväl mus som tangentbordet för att skriva in
drag. Se vidare <a Moves>att skriva in drag</a> för en noggrannare beskrivning.
</p>
<p>
Du kan också använda Scid för att hantera <a PGN>PGN</a> filer genom att klistra in PGN-text i Scids <a Import>Importfönster</a> 
eller genom att öppna en PGN-fil i Scid. PGN-filer kan dock inte redigeras av Scid (de öppnas skrivskyddade). Eftersom PGN-filer
använder mer minne och laddas långsammare rekommenderas du att konvertera stora PGN-filer till en Scid databas med verktyget
<a Pgnscid>Pgnscid</a>.
</p>
<p>
Scids <a MainWindow>huvudfönster</a> (det med schackbrädet) visar det aktuella partiet och databasen i detalj. Du kan när som helst öppna 
upp till fyra databaser (fem om du räknar med Urklippsdatabasen <a Clipbase>Clipbase</a>). Var och ett av dessa kommer att ha sitt eget aktuella
parti. (Partier med ID-numret 0 visar att detta är ett parti som ännu inte ingår i den aktuella databasen). Du bläddrar mellan de öppnade databaserna
via <a Menus File>Filmenyn</a>.
</p>
<p>
Läs gärna övriga hjälpsidor i <a Index>Innehållsförteckningen</a> för mer information.
</p>
<p>
Läs <a Author>kontaktinformationssidan</a> om du vill komma i kontakt med skaparen av Scid. För frågor om den svenska översättningen ska du
<b>inte</b> kontakta upphovsmannen utan istället <a Translator>översättaren</a>.
</p>

<p><footer>(Uppdaterad: Scid 3.1, December 2001)</footer></p>
}


set helpTitle(W,Author) "Kontaktinformation"
set helpText(W,Author) {<h1>Kontaktinformation</h1>
<p>
Scids webbsajt finns på: <br>
<b><url http://scid.sourceforge.net/>http://scid.sourceforge.net/</url></b>
</p>
<p>
På hemsidan kan du hämta den senaste versionen av Scid och andra extrafiler som kan bli tillgängliga.
</p>
<p>Den mest uppdaterade hjälpen kommer alltid att finnas tillgänglig online hos:
<b><url http://scid.sourceforge.net/tutorial/>http://scid.sourceforge.net/tutorial/</url></b>
Typiskt kommer denna att vara mer uppdaterad än den hjälp som distribueras med Scid, eller dessa hjälp-
sidor i Scid.
</p>

<p>Den svenska översättningen av Scid har gjorts av Martin Skjöldebrand. Synpunkter på översättningen ska <b>inte</b>
skickas till Shane. Däremot får du gärna skicka kommentarer, frågor och förslag till rättningar till översättaren på adressen:<br>
<b>martin@skjoldebrand.org</b>
</p>
}

####################
### Hints page:
set helpTitle(W,Hints) "Scid-tips"
set helpText(W,Hints) {<h1>Scid-tips</h1>
<p>
Denna sida innehåller användbara tips i form av frågor och svar. Genom att läsa igenom denna sida, kan du senare komma att
använda Scid bättre. Om du nyligen börjat använda Scid, så läs till en början <a Guide>Kom igång</a>.
Det mesta av informationen på denna sida finns refererad mer utförligt på andra sidor i hjälpen. Du finner dem i <a Index>Innehållsförteckningen</a>.
Om har något du tips du tycker passar bra för denna sida, så skicka den till <a Author>upphovsmannen</a>.
</p>

<h4>Kan Scid ladda en databas när den startas?</h4>
<p>
Ja, du kan ange databaser, PGN-filer eller <a EPD>EPD-filer</a>
på kommandoraden som t ex;
<ul>
<li> <b>scid  mybase  games.pgn.gz</b> </li>
</ul>
som laddar Scid-databasen <b>mybase</b> och den Gzip-komprimerade PGN-filen <b>games.pgn.gz</b>.
</p>

<h4>Finns det enklare sätt att ändra storleken på brädet än via Alternativmenyn?</h4>
<p>
Ja, du kan använda kortkommandorna <b>Ctrl+Shift+VänsterPil</b> och <b>Ctrl+Shift+HögerPil</b> för att förminska eller 
förstora brädets storlek.
</p>

<h4>Jag brukar träna genom att spela igenom partier, så jag vill inte att Scid visar nästa drag i partiinformationen
nedanför brädet. Kan jag dölja detta på något sätt?</h4>
<p>
Du kan dölja nästa drag genom att högerklicka partiinformationen och välja  <b>Dölj nästa drag</b> i menyn som öppnas.
</p>

<h4>Var finner jag ECO-koden för öppningen på brädet?</h4>
<p>
ECO-koden visas på sista raden i partiinformationen nedanför brädet i  <a MainWindow>huvudfönstret</a>,
om du har laddat ECO-klassificeringsfilen (<b>scid.eco</b>). <br>
Hjälpsidan för <a ECO>ECO-koder</a> förklarar hur man laddar ECO-klassificeringsfilen och ställer in programmet så att
den laddas varje gång man startar Scid.
</p>

<h4>Jag håller på och matar in ett parti och befinner mig nu vid drag 30. Nu ser jag att drag 10 blev fel. Hur kan jag ändra
detta utan att göra om alltihop?</h4>
<p>
Du kan använda <a Import>Importfönstret</a>; se vidare <a Moves Mistakes>mata in drag</a> för mer information.
</p>

<h4>Hur kopierar jag partier från en databas till en annan?</h4>
<p>
Använd <a Switcher>Databasväxlaren</a>. Drag från källdatabasen till måldatabasen så kopierar du allt i källdatabasen till
måldatabasen. Genom att använda <a Searches Filter>Filter</a> kan du begränsa det du kopierar från den ena databasen till den andra.
</p>

<h4>Varje gång jag anger ett drag som redan finns får jag frågan "Ersätt drag?". Hur undviker jag detta?</h4>
<p>
Du kan antingen stänga av <b>Fråga före ersätt drag</b> valmöjligheten i menyn <menu>Altervativ: Drag</menu</menu>; eller
tänka på att göra ändringarna genom att ta tillbaka dragen genom att högerklicka med musen. Det senare alternativet raderar dragen helt ur
partiet.
</p>

<h4>Hur ändrar kolumnbredden i partilistan?</h4>
<p>
Höger- eller vänsterklicka på kolumntiteln för den kolumn du vill ändra.
</p>

<h4>Hur använder jag trädfönstret för ett filtrerade partier - till skillnad mot att använda den för hela databasen?</h4>
<p>
Använd <a Clipbase>Urklippsdatabasen (Clipbase)</a>. Filtera databasen att visa endast de partier du vill använda i trädet, 
kopiera sedan partierna till Urklippsdatabasen (Clipbase) via <a Switcher>Databasväxlaren</a>. Öppna därefter trädfönstret
med den senare databasen som aktiv.
</p>

<h4>Trädet är långsamt när jag jobbar med stora databaser. Går det att snabba upp den?</h4>
<p>
Spara trädcachen ofta så sparar du resultaten för framtida användning. Se vidare cache-sektionen i hjälpen för 
<a Tree>trädet</a> för mer detaljerad information.
</p>

<h4>Kan jag redigera PGN-texten för ett parti direkt?</h4>
<p>
Du kan inte använda <a PGN>PGN-</a>fönstret till att redigera det aktuella partiet, däremot kan du använda 
<a Import>Importfönstret</a>. Öppna fönstret (kortkommando: <b>Ctrl+Shift+I</b>) och klicka på klistra in aktuellt parti
<b>Paste current game</b>, redigera partiet och klicka sedan på <b>Importera</b>.
</p>

<h4>I min databas finns många spelarnamn som är felstavade. Hur rättar jag allt detta?</h4>
<p>
Du kan rätta enstaka namn, eller göra en stavningskontroll för hela databasen via menyvalen i <menu>Arkiv: Databasverktyg</menu>.
Se vidare hjälpen för <a Maintenance Editing>Databasunderhåll</a>.
</p>

<h4>Jag har två databaser öppna: den ena innehåller mina egna partier, den andra är många stormästarpartier.
Hur jämför jag ett av mina egna partier mot partierna i databasen med stormästarpartier?</h4>
<p>
Öppna <a Tree>Trädfönstret</a> i databasen med stormästarpartier och välj <term>Lås</term> för att låsa trädet i den databasen. 
När du sedan byter till den andra databasen kommer trädet att fortsätta visa informationen från databasen med stormästarpartier.
</p>

<p><footer>(Uppdaterad: Scid 2.6,  augusti 2001)</footer></p>
}


#################
# Authors, translators
#

set helpTitle(W,Translator) "Den svenska översättningen"
set helpText(W,Translator) {<h1>Den svenska översättningen</h1>
<p>
Scids webbsajt finns på: <br>
<b><url http://scid.sourceforge.net/>http://scid.sourceforge.net/</url></b>. Där kan du hämta den senaste versionen av Scid och 
de andra filerna till programmet som du kan ha behov av. Där finns också den senaste versionen av den svenska språkfilen.
</p>
<p>Den svenska översättningen av Scid har gjorts av Martin Skjöldebrand. Skicka gärna kommentarer, frågor och förslag till rättningar
till översättaren på adressen:<br>
<b>martin@skjoldebrand.org</b>
</p>
}

####################
### Main window help:

set helpTitle(W,MainWindow) "Huvudfönstret"
set helpText(W,MainWindow) {<h1>Scid: huvudfönstret</h1>
<p>
På brädet i huvudfönstret visas den aktuella ställningen i det aktiva partiet. Du får också information om partiet och den
aktiva databasen. För ytterligare information om <a Menus>Menyerna</a> och olika sätt att <a Moves>Ange drag</a> hänvisas till
andra hjälpsidor.
</p>

<h3>Att navigera i partier</h3>
<p>
Navigationsknapparna vid brädet har följande funktioner (från vänster till höger):
</p>
<ul>
<li> <button tb_start> Gå till utgångsställningen. </li>
<li> <button tb_prev> Gå tillbaka ett drag. </li>
<li> <button tb_next> Gå framåt ett drag. </li>
<li> <button tb_end> Gå till slutställningen. </li>
<li> <button tb_invar> Gå in i varianten. </li>
<li> <button tb_outvar> Gå ur varianten. </li>
<li> <button tb_addvar> Lägg till ny variant. </li>
<li> <button tb_play> Start/stopp för autospelsläge (se nedan). </li>
<li> <button tb_trial> Start/stopp för <a Moves Trial>Försöksläge</a>. </li>
<li> <button tb_flip> Rotera brädet 180 grader. </li>
<li> <button tb_coords> Visa/dölj koordinater. </li>
</ul>

<h4><name Autoplay>Autospelsläge</name></h4>
<p>
I autospelsläge gör Scid automatiskt båda spelarnas drag i det aktuella partiet. Fördröjningen mellan varje drag kan anges i
menyn <menu>Alternativ: Drag</menu> och sparas när du sparar ändringarna av inställningsmöjligheterna.
</p>
<p>
Kortkommandot <b>Ctrl+Z</b> startar eller avbryter autospelsläget. Du kan också gå ur autospelsläge genom att trycka
 <b>Esc</b>-tangenten.
</p>
<p>
Om du startar autospelsläge när <a Analysis>Analysfönstret</a> är öppet <term>kommenteras</term> partiet: ställningsbedömningen 
och analysen av varje ställning läggs till som en ny variant vid varje drag.
Se hjälpen för <a Analysis>Analysfönstret</a> för mer information.
</p>

<h3>Partiinformation</h3>
<p>
Nedanför brädet visas information om det aktuella partiet. Denna del av huvudskärmen kallas <term>partiinformationsområdet</term>.
På de tre första raderna ges grundinformation såsom spelarnas namn, resultat, datum och spelplats. Den fjärde raden aktuell
ställning och nästa drag.
</p>
<p>
Rad fem visar <a ECO>ECO</a> (Encyclopedia of Chess
Openings) koden för den aktuella ställningen, om ställningen ingår i den ECO-fil som används.
</p>
<p>
När man högerklickar på partiinformationsområdet visas en meny med inställningsmöjligheter som är relevanta för just denna del av
programmet. Du kan t ex välja att dölja nästa drag (användbart om du tränar genom att "gissa" nästa drag i  partiet) eller ta bort
/återta det aktuella partiet. Kortkommandot för denna funktion är funktionsknappen F9.
</p>

<h4>Slutspelsdatabaser</h4>
<p>
Partiinformationsområdet visar resultaten från slutspelsdatabaserna om du har några installerade och om ställningen återfinns 
i dessa. Se hjälpsidan för <a TB>Slutspelsdatabaser</a> för mer information.
</p>

<h3>Statusraden</h3>
<p>
Statusraden (längst ner i huvudfönstret) visar information om den aktuella databasen.
Det första fältet visar partiets status: <b>XX</b> betyder att det har förändrats men ännu inte sparats, medan <b>--</b> 
betyder att partiet inte har ändrats, <b>%%</b>, slutligen, betyder att databasen är skrivskyddad (det går inte att ändra i partiet.).
</p>
<p>
Om du vill, kan du öppna databasen skrivskyddad. Ãndra rättigheterna till scid-filerna (eller enbart indexfilen) genom att i UNIX(-lika)
system ge kommandot:
<b>chmod a-w myfile.si3</b>
vid kommandoraden.I MS-DOS eller Windows ändrar du filattributet Arkiv med Attrib filnamn +A (i MS-DOS) eller Filhanteraren/Utforskaren etc. (i Windows).)
</p>
<p>
Statusraden visar också hur många partiet som ingår i det aktiva <a Searches Filter>Filtret</a>.
</p>

<p><footer>(Uppdaterad: 3.6.11-beta Januari 2007)</footer></p>
}

####################
### Menus help screen:

set helpTitle(W,Menus) "Menyerna"
set helpText(W,Menus) {<h1>Menyerna</h1>

<h3><name File>Arkiv</name></h3>
<ul>
<li><menu>Ny</menu>: Skapar en ny, tom, Scid databas.</li>
<li><menu>Öppna</menu>: Öppnar en befintlig Scid-databas.</li>
<li><menu>Stäng</menu>: Stänger den aktiva Scid-databasen.</li>
<li><menu>Sök filer</menu>: Öppnar <a Finder>Sökdialogen</a> för filer.</li>
<li><menu>Bokmärken</menu>: Hanterar <a Bookmarks>bokmärken</a>.</li>
<li><menu>Nytt bokmärke</menu>: Markerar den aktiva ställningen i partiet som ett bokmärke.</li>
	<li><menu>Spara bokmärke</menu>: Sparar bokmärket för den aktiva ställningen i partiet.</li>
	<li><menu>Redigera bokmärken</menu>: Redigerar dina bokmärken.</li>
	<li><menu>Visa bokmärken som lista</menu>: Visar bokmärkena som lista, inte som undermenyer.</li>
	<li><menu>Visa bokmärken i undermenyer</menu>: Visar bokmärkena som undermenyer, inte som en lista.</li>
	</ul>
<li><menu>Databasverktyg</menu>: <a Maintenance>Underhåll</a> databasen.</li>
    </li>
	<ul>
	<li><menu>Verktygsfönster</menu>: Öppnar/ stänger verktygsfönstret.</li>
	<li><menu>Klassificera partier enligt ECO</menu>: Klassificerar alla partier enligt ECO-systemet.</li>
	<li><menu>Ta bort dubbletter</menu>: Tar bort <a Maintenance Twins>Dubblettpartier</a> i databasen.</li>
	<li><menu>Namn</menu>: Ersätter alle förekomster av en spelar-,
      resultat-, ort- eller rundeinmatning.</li>
      </ul>
      <li><menu>Skrivskyddad</menu>: Avlägsnar temporärt redigeringsmöjligheterna till databasen.</li>
<li><menu>Byt databas</menu>: Byter till en annan öppnad databas.</li>
	<ul>
		<li><menu>Base 1/2/3/4/5</menu>: Här byter du mellan de olika databaserna i databasväxlarna, inklusive <a Clipbase>Urklippsdatabasen</a>.</li>
	</ul>
	<ul>
	<li><menu>Komprimera databasen</menu>: Komprimerar databasen, avlägsna borttagna partier och oanvända namn.</li>
	<li><menu>Sortera databasen</menu>: Sorterar partierna i den aktiva databasen.</li>
  	<li><menu>Sök dubbletter"</menu>: Öppnar/ stänger dubblettfönstret för att söka dubblettpartier.</li>
	<li><menu>Stavningskontroll</menu>: Namnredigerar och stavningskontrollerar.</li>
		<ul>
		<li><menu>Redigera namn</menu>: Redigerar spelarnamn utifrån rättstavningsfilen.</li>
		<li><menu>Stavningskontrollera namn</menu>: Stavningskontrollerar namn utifrån rättstavningsfilen.</li>
		<li><menu>Stavningskontrollera evenemang</menu>: Stavningskontrollerar evenemang utifrån rättstavningsfilen.</li>
		<li><menu>Stavningskontrollera platser</menu>: Stavningskontrollerar platser utifrån rättstavningsfilen.</li>
		<li><menu>Stavningskontrollera ronder</menu>: Stavningskontrollerar ronder utifrån rättstavningsfilen.</li>
  		</ul>
	</ul>	
<li><menu>Avsluta</menu>: Avslutar Scid. </li>
</ul>

<h3>Redigera</h3>
<ul>
<li><menu>Lägg till variant</menu>: Skapar en variant vid denna ställning. Antingen för nästa drag, eller för föregående drag om det är det första draget.</li>
<li><menu>Ta bort variant</menu>: Visar en undermeny med de borttagbara varianterna.</li>
<li><menu>Skapa huvudvariant</menu>: Upphöjer en av varianterna till huvudvariant.</li>
<li><menu>Skapa nytt textdrag</menu>: Gör en av varianterna till nytt partidrag.</li>
<li><menu>Testa en idé</menu>: Slår på <a Moves Trial>Testläge</a> för att tillfälligt testa en idé. Förändrar inte partiet.</li>
<li><menu>Ta bort</menu>: Avlägsnar kommentarer eller varianter ur partiet.</li>
<br>
<li><menu>Töm Clipbase</menu>: Tömmer Urklippsdatabasen <a Clipbase>Clipbase</a> på partier.</li>
<li><menu>Kopiera partiet till Clipbase</menu>: Kopierar det aktuella partiet till Urklippsdatabasen <a Clipbase>Clipbase</a>.</li>
<li><menu>Klistra in det senaste Clipbasepartiet</menu>: Klistrar in det aktiva partiet i <a Clipbase>Clipbase</a> i den aktiva databasen och gör det aktivt.</li>
<br>
<li><menu>Skapa ställning</menu>: Skapar en utgångsställning för aktuellt parti.</li>
<li><menu>Kopiera ställning</menu>: Kopierar den aktuella ställningen i FEN-kod till Urklippsdatabasen.</li>
<li><menu>Klistra in utgångsställning</menu>: Klistrar in ställningen från aktuellt parti i den temporära databasen.</li>
</ul>

<h3>Partier</h3>
<ul>
<li><menu>Nytt parti</menu>: Ãterställer brädet inför ett nytt parti (detta tar bort alla ändringar).</li>
<li><menu>Ladda första/föregående/nästa/sista partiet</menu>: Dessa laddar respektive parti i <a Searches Filter>Sökfiltret</a>.</li>
<li><menu>Ladda om partiet</menu>: Laddar om aktuellt parti och tar bort alla gjorda ändringar..</li>
<li><menu>Ladda parti slumpmässigt</menu>:  Laddar ett av datorn slumpmässigt valt parti.</li> 
<li><menu>Ladda parti nummer...</menu>: Laddar ett parti genom att ange dess nummer.</li>
<br>
<li><menu>Spara: Ersätt parti...</menu>: Sparar partiet och ersätter tidigare version.</li>
<li><menu>Spara: Nytt parti...</menu>: Sparar det aktuella partiet och lägger till det till databasen.</li>
<br>
<li><menu>Identifiera öppningen</menu>: Går till den mest detaljerade ställningen i ECO-boken.</li>
<li><menu>Gå till drag nummer...</menu>: Går till ett specifikt drag i partiet.</li>
<li><menu>Hitta nyhet...</menu>: Hittar det första draget i partiet som inte spelats tidigare.</li>
</ul>

<h3>Sök</h3>
<ul>
<li><menu>Återställ sökfilter</menu>: Återställer <a Searches Filter>Sökfiltret</a> så att alla partiet ingår i urvalet.</li>
<li><menu>Omvänt filter</menu>: Tar med de partier som utesluts av filtret.</li>
<br>
<li><menu>Aktuell ställning...</menu>: Söker partier med <a Searches Board>Den aktuella ställningen</a> på brädet.</li>
<li><menu>I huvud...</menu>: Använder <a Searches Header>Fast information</a> (spelare, evenemang, plats, mm).</li>
<li><menu>Material/Ställning...</menu>: Söker baserad på <a Searches Material>Material</a> eller <a Searches Pattern>Ställning</a>.</li>
<br>
<li><menu>Använd sökfil...</menu>: Använder en fil med <a Searches Settings>lagrade</a>.</li>
</ul>

<h3>Fönster</h3>
<ul>
<li><menu>Kommentarseditor</menu>: Öppnar/ stänger <a Comment>Kommentarseditorn</a>.</li>
<li><menu>Partilista</menu>: Öppnar/ stänger <a GameList>Partilistan</a>.</li>
<li><menu>PGN fönster</menu>: Öppnar/ stänger <a PGN>PGN-fönstret</a>.</li>
<li><menu>Spelarförteckning</menu>: Öppnar/ stänger en förteckning över spelarna i den aktiva databasen.</li> 
<li><menu>Turneringar</menu>: Listar <a Tmt>Turneringar</a>.</li>
<br>
<li><menu>Databasväxlaren</menu>: Öppnar/ stänger <a Switcher>Databasväxlaren</a>. Databasväxlaren underlättar byte mellan olika databaser
och kopiering av partier från den ena databasen till den andra.</li>
<li><menu>Databasverktyg</menu>: Öppnar/ stänger <a Maintenance>Verktygsfönstret</a>.</li>
<br>
<li><menu>ECO fönster</menu>: Öppnar/ stänger <a ECO browser>ECO bläddraren</a>.</li>
<li><menu>Repetoareditor</menu>: Öppnar/ stänger verktyget för <a repetoire>Spelöppningshantering</a>.</li>
<li><menu>Statistikfönster</menu>: Öppnar/ stänger <term>Statistikfönstret</term>. Här hittar du en statistisk sammanfattning av partierna i  
 <a Searches Filter>Sökfiltret</a>.</li>
<li><menu>Trädfönster</menu>: Öppnar/ stänger <a Tree>Variantträdet</a>.</li>
<li><menu>Slutspelsdatabas</menu>: Öppnar/ stänger slutspelsdatabasfönstret som ger dig viss information om <a TB>Slutspelsdatabaserna</a>.</li>
</ul>

<h3>Verktyg</h3>
<ul>
<li><menu>Analysmotor...</menu>: Startar/ stoppar en analysmotor, t ex Crafty, som kontinuerligt bedömmer den aktuella ställningen
på brädet i ett <a Analysis>Analysfönster</a>.</li>
<li><menu>Analysmotor 2...</menu>: Startar/ stoppar en andra analysmotor, t ex Crafty.</li>
<li><menu>Resultattabell</menu>: Skapar en <a Crosstable>Resultattabell</a> för den aktuella turneringen/matchen som det aktuella partiet ingår i</li>
<li><menu>Eposthanteraren</menu>: Öppnar/ stänger <a Email>Eposthanteraren</a> för hantering av korrespondensschack.</li>
<br>
<li><menu>Öppningsrapport</menu>: Skapar en <a OpReport>Öppningsrapport</a> utifrån den aktuella ställningen.</li>
<li><menu>Sök material</menu>: Öppnar dialog för att <a PTracker>Söka efter en viss materiell balans</a>.</li>
<br>
<li><menu>Spelarinformation</menu>: Visar/ uppdaterar <a PInfo>Spelarinformation</a> för den ene av två spelare i det aktuella partiet.</li>
<li><menu>Ratingdiagram</menu>: Skapar ett <a Graphs Rating>Ratingdiagram</a> för spelarna i partiet.</li>
<li><menu>Resultatdiagram</menu>: Visar <a Graphs Score>Resultatdiagrammet</a>.</li>
<br>
<li><menu>Exportera aktuellt parti ...</menu>: Sparar aktuellt parti till olika format; text, HTML eller LaTeX. Se vidare
   hjälpsidan för  att <a Export>Exportera</a> partier.</li>
<li><menu>Exportera alla filtrerade partier</menu>: Sparar alla <a Searches Filter>filterade</a> partier till olika format; text, HTML eller LaTeX. Se vidare
   hjälpsidan för  att <a Export>Exportera</a> partier.</li>
<br>
<li><menu>Importera ett parti i PGN-format...</menu>: Öppnar <a Import>Importfönstret</a> för att ange eller klistra in ett parti i 
 <a PGN>PGN-format</a> för import till en Scid databas.</li>
<li><menu>Importera flera partier i PGN-format...</menu>: Importerar flera partier i PGN-format från en fil.</li>
</ul>

<h3>Alternativ</h3>
<p>
Denna meny ger tillgång till de flesta av de parametrar som styr hur Scid fungerar.
Menyvalet <menu>Spara alternativ</menu> sparar de aktuella inställningarna till filen
 "<b>~/.scid/scidrc</b>" (eller <b>scid.opt</b> i katalogen som innehåller den exekverbara
scid-filen, Scid.exe, om Windows); denna fil laddas varje gång du startar Scid.
</p>

<h3>Hjälp</h3>
<p>
Denna meny innehåller hjälpfunktioner och ger tillgång till bl a fönstret "Dagens tips"
och startfönstret som informerar om vilka filer Scid laddat vid uppstart. 
</p>

<p><footer>(Uppdaterad: Scid 3.5, februari 2003)</footer></p>
}
	
####################
### Entering moves help:

set helpTitle(W,Moves) "Ange drag"
set helpText(W,Moves) {<h1>Ange drag</h1>
<p>
I Scid, kan du ange partidrag såväl med musen som med tangentbordet. När du rör musmarkören över en ruta på
brädet kommer du att märka att såväl rutan som ytterligare en ruta får en annan färg. Om det finns ett legalt
drag till eller från den ruta du för musmarkören över. Detta är det <term>Föreslagna draget</term>.
Du utför detta drag genom att <term>Vänsterklicka</term> med musen. Om detta stör dig kan du stänga av denna funktion
i Alternativmenyn.

</p>
<p>
För att utföra ett annat drag kan du <term>klicka och hålla nere vänster</term> musknapp: flytta sedan musen till önskad
 ruta och släpp musknappen.
</p>
<p>
Om du vill kan du utföra drag genom att klicka två gånger istället för att använda klicka-och-drag tekniken. Klicka då 
först på utgångsrutan med den <term>Mittersta musknappen</term> och klicka sedan med samma musknapp på destinationsrutan.
(Tipset gäller kanske framförallt användare av UNIX(-lika) system, som ofta har stor nytta av 3-knappsmöss).
</p>

<h4>Ta tillbaka ett drag</h4>
<p>
Man kan ta tillbaka ett drag genom att högerklicka med musen. Detta går tillbaka ett drag och tar bort det senast utförda draget i partiet
eller varianten.
</p>

<h4>Ersätt tidigare drag</h4>
<p>
Om du anger ett drag i en ställning där ett drag redan angivits kommer Scid att fråga om du verkligen vill ersätta den tidigare
draget (draget och alla varianter längre fram som är beroende av draget tas bort), eller om du vill skapa en ny variant istället.
En del använder tycker att frågan är störande och vill alltid ersätta det tidigare draget, så man kan konfigurera Scid till att inte
ställa denna fråga. Du hittar möjligheten i  menyn <menu>Alternativ: Drag</menu> "<i>Fråga före ersätt drag</i>".
</p>

<h4><name Trial>Testläge</name></h4>
<p>
Om du går igenom ett parti och kommer till en ställning där du vill testa en idé utan att påverka det registrerade partiet så väljer du
<b>Testa en idé</b> från menyn <menu>Redigera</menu> för att sätta på testläge. I testläge kan du göra drag och förändringar av partiet som
är temporära, dvs de sparas inte när du återgår från testläge.
</p>

<h3><name Mistakes>Rätta fel</name></h3>
<p>
Om du skriver in ett parti och plötsligt upptäcker ett fel flera drag tidigare är det möjligt att rätta till detta utan att 
behöva göra om alla drag efter feldraget. Det enda sättet är att redigera PGN-versionen av partiet: Öppna <a Import>Importfönstret</a>, 
välj Klistra in aktivt parti, korrigera felet och välj därefter "Importera".
</p>

<h3>Ange drag via tangentbordet</h3>
<p>
Du kan ange drag via tangentbordet genom att helt enkelt skriva in dem med bokstäver och siffror. Lägg märke till att dragen
ska anges i <term>SAN-notation</term>, <i>utan</i> (x) tecknet för slag eller (=) tecknet för promovering. Dragnotationen är 
inte versal/gemenkänslig så t ex:
[n][f][3] är samma drag som Nf3 -- men kontrollera med noten nedan för drag som står i konflikt med varandra.
</p>
<p>
För att säkerställa att inga drag är prefix för ett annat drag så används en särskild metod att ange rockad. Kort och lång rockad
representeras med hjälp av bokstäver som följer:
kort rockad anges med  [O][K]
lång rockad anges med [O][Q] istället för det vanliga O-O och O-O-O.
</p>
<p>
När du anger drag kommer du i statusraden att se en lista över giltiga drag. Du kan, genom att trycka <b>[Mellanslag]</b> välja det första i listan och föra in det
i partiet. För att ta bort tecken använder du de vanliga raderingstangenterna.
</p>
<p>
<b>OBS</b> gemena tecken kopplas i första hand till bönder, vilket betyder att [b]kan betyda b-bonden istället för löparen (Bishop). Om du hamnar
i situationer när konflikter uppstår måste du istället ange pjäserna med versaler (B) i detta fallet.
</p>
<b>Översättarens kommentar</b> Scid förstår inte svenska. Detta betyder att du inte kan använda de svenska förkortningarna till att ange drag. [s][f][6] 
förstås inte av Scid (däremot funkar [n][f][6] utmärkt). Om du vill använda tangentbordet att skriva in drag får du istället använda notationen [g][8][f][6] för
samma drag.
</p>
<h4>Autokomplettering</h4>
<p>
I Alternativmenyn kan du slå på eller av <term>Autokomplettering</term>
av drag.
Med denna påslagen görs draget så fort du skrivit in tillräckligt mycket för att särskilja det från andra möjliga drag. Exempelvis räcker det att skriva [n][f] istället
för [n][f][3] för draget <b>Nf3</b>i utgångsställningen.
</p>
<p><b>Översättarens kommentar</b> Scid förstår som sagt inte svenska. Däremot fungerar det med rutangivelsen om t ex pjäsen på g1 bara har ett fält 
att gå till.</p>

<h3><name Null>Ange null-drag</name></h3>
<p>
<a Annotating Null>Null</a> (tomma, planerade) drag kan vara användbara i  kommentarer för att hoppa över den ena spelarens drag. Du anger null-drag
genom att slå den ena kungen med den andra, eller via tangentbordet genom att skriva  "<b>--</b>" (minustecknet två gånger).
</p>
<p><b>Översättarens kommentar</b> "null" är en dataterm för att beskriva ingenting (till skillnad mot noll som har ett värde om dock noll, null har inget värde).
Shane använder termen null här, någon som har förslag på bättre i den svenska översättningen? Kolla introduktionssidan för att föreslå någonting.</p>

<h3>Ange de vanligaste kommentarsymbolerna</h3>
<p>
Du kan även ange <a NAGs>Kommentarsymboler</a> via tangentbordet utan att behöva använda <a Comment>Kommentarseditorn</a>. 
Följande lista kan anges via tangentbordet:
<ul>
<li> !	: [!][ENTER] </li>
<li> ?	: [?][ENTER] </li>
<li> !?	: [!][?][ENTER] </li>
<li> ?!	: [?][!][ENTER] </li>
<li> !!	: [!][!][ENTER] </li>
<li> ??	: [?][?][ENTER] </li>
<li> </li>
<li> +-	: [+][-] </li>
<li> +/-	: [+][/] </li>
<li> +=	: [+][=] </li>
<li> =	: [=][ENTER] </li>
<li> -+	: [-][+] </li>
<li> -/+	: [-][/] </li>
<li> =+	: [=][+] </li>
</ul>

<p><footer>(Uppdaterad: Scid 3.4,  juli 2002)</footer></p>
}


########################################
### Searches help screen:

set helpTitle(W,Searches) "Söka i Scid"
set helpText(W,Searches) {<h1>Söka i Scid</h1>
<p>
I Scid kan man söka information på flera olika sätt. 
Det finns tre huvudmetoder att använda sig av beroende på vilken typ av information man
är ute efter:
<ul>
<li><b>1)</b> utifrån den aktuella ställningen på brädet, </li>
<li><b>2)</b> utifrån specifikt material eller en specifik ställningstyp; och </li>
<li><b>3)</b> utifrån fast information, t ex spelare, ort, resultat eller datum. </li>
</ul>
<p>
Utöver dessa metoder kan man använda sig av en automatisk sökfunktion, <a Tree>Trädfönstret</a>, som förklaras
separat.
</p>

<h3><name Filter>Sökfiltret</name></h3>
<p>
Sökningar baseras i  Scid på <term>Filter</term>.
Ett filter representerar en delmängd av den aktiva databasen. Vid varje specifikt tillfälle kommer ett parti att antingen vara
del av denna delmängd (filtret) eller uteslutas av detta. I varje typ av sökning (se ovan) kan du välja att begränsa, utöka eller
ignorera det aktiva filtret och istället söka i hela databasen. Detta gör att man stegvis kan bygga komplicerade sökningar. 
Du kan också kopiera alla filtrerade partier från den ena databasen till den andra genom att använda <a Switcher>Databasväxlaren</a>.
</p>

<p>
När du söker på exakt ställning, <a Tree>Träd</a> eller Material/Ställningstyp, sparas dragnumret i den första ställningen och varje
parti med identisk ställning lagras i minnet så att man, när man senare laddar de framsökta partierna, automatiskt kommer till den ställning
som var av intresse (som sökningen grundades på).
</p>
<p>
<b>OBS</b> sökningen gäller endast textdragen, inte varianterna.
</p>

<h3><name Board>Sök: Aktuell ställning</name></h3>
<p>
Med denna metod hittas partier med samma ställning som det på brädet i huvudfönstret. Rockadmöjligheter och 
rättigheter att göra <i>En passant</i> ignoreras. Det finns fyra undertyper att välja på, samtliga kräver att ställningen har exakt samma material och spelare vid draget för att en träff ska
anses ha inträffat:
<ul>
<li> [1] exakt (de båda ställningarna måste vara exakt lika), </li>
<li> [2] bönder (bondestrukturen måste vara identisk, men de övriga pjäsernas placering kan variera), </li>
<li> [3] linjer (antalet vita och svarta bönder på varje linje måste vara identisk - i övrigt kan pjäsplaceringen variera), och slutligen; </li>
<li> [4] material (bönder och pjäser kan stå var som helst på brädet.). </li>
</ul>
<p>
Att söka på bondeställning är användbart när man studerar spelöppningar med likartad bondstruktur, medan att söka på linjer och material är lämpliga metoder för
att finna liknande ställningar i slutspel. När man söker på specifika ställningar kan man skapa den först (från menyvalet <menu>Redigera: Skapa ställning</menu>) och starta sökningen därifrån.
(Man kan naturligtvis också spela upp ställningen på brädet, <i>Översättarens anmärkning</i>).
</p>
<p>
Du kan ange att sökningen även ska leta i varianter (istället för att bara inkludera de egentliga partidragen) genom att markera kryssrutan <b>Sök i varianter</b>, 
men detta kan göra sökningen långsammare om databasen är stor och har många partier med varianter.
</p>

<h3><name Material>Sök: Material/ställningstyp</name></h3>
<p>
Denna sökning är användbar när man vill finna slut- eller mittspelsteman. Du kan specificera minimalt och maximalt antal av varje pjästyp, och hitta ställningar
av typen "Löpare på f7" eller "Bonde på f-linjen". Det finns ett antal vanliga ställningstyper fördefinierade, t ex "Torn och Bondeslutspel" eller "Isolerade d-bönder".
</p>
<p>
<b>Tips:</b><br>
Tidsåtgången för denna typ av sökning kan variera kraftigt. Man kan minska tiden som behövs genom att använda sig av förnuftiga begränsningar.
Om man, som exempel,  vill undersöka slutspel kan man sätta det minimala antalet drag partiet ska innehålla till 20. Då kommer alla partier 
med färre än 20 drag att ignoreras.
</p>

<h3><name Header>Sök: fasta uppgifter ("header search")</name></h3>
<p>
Denna sökning används för att finna aspekter på partier som lagras i partihuvudet (jämför med eposthuvud) såsom datum, resultat, plats och ratingtal. Den
kräver ingen avkodning av dragen i partiet. För att en träff ska registreras krävs att alla fält du specificerar stämmer överens. Namnfälten  (Vit, Svart, 
Evenemang, Plats och Rond) är versal/gementoleranta. Träffar registrerars på all text i dessa fält och mellanslag ignoreras .
</p>
<p>
Du kan generera exakta sökningar och använda jokertecken i huvudfälten som nämnts ovan. Jokertecknet  <b>?</b> betyder "Ett
valfritt tecken" medan <b>*</b> betyder "Inget eller flera valfria tecken". Man genererar exakta sökningar genom att innesluta den text man
önskar hitta i citattecken ("). Exempel:
</p>

<p>
En sökning på platsen <b>USA</b> kommer att generera amerikanska städer, men även <b>Lausanne SUI</b>, vilket du förmodligen inte var ute efter! 
Detta löser man genom att istället söka efter platsen <b>"*USA"</b> (notera citattecknen) som endast kommer att presentera städer i USA.</p>
<p>
Om du söker efter en särskild spelare (eller två särskilda spelare) som vit eller svart och det inte spelar någon roll vet som har vilken färg väljer du med 
fördel <b>Ignorera färg</b> i stället för svart/vit i färgangivelsen.
</p>
<p>
Avslutningsvis kan fasta sökningar göras för att hitta valfri text (VERSAL/gemen-känsligt och utan jokertecken) i PGN-versionen av partierna. Du kan
ange upp till tre textsträngar, och alla måste finnas i ett parti för att de ska räknas som en träff. Denna sökning är mycket användbar när det gäller
kommentarsfältet eller extra märkord i partierna (exempelvis  <b>förlorar på tid</b> eller <b>Kommentator</b>), eller för en dragföljd som 
<b>Bxh7+</b> och <b>Kxh7</b> när en löpare har offrats (och accepterats) på h7.

Tänk dock på att denna typ av sökningar kan vara <i>mycket</i> tidskrävande eftersom alla partier som möter vissa kriterier måste avkodas och
genomsökas efter textsträngen/-arna. Det är därför en bra idé att begränsa denna typ av sökning så mycket som möjligt. Exempel:
</p>

<p>
För att hitta partier med underpromovering till torn säker man på <b>=R</b> och sätter <b>Förvandlingar</b> flaggan till Ja.
För att hitta text i kommentarer sätter du flaggan <b>Kommentarer</b> till Ja.
Om du söker på dragen <b>Bxh7+</b> och <b>Kxh7</b>, kan det vara en idé att begränsa partierna till de som har resultatet  1-0 resultatet och innehåller
minst 20 halvdrag. Eller gör en Material-/Ställningstypssökning för att hitta partier där löparen flyttas till h7.
</p>

<h3><name Settings>Spara sökkriterierna</name></h3>
<p>
I Material/Ställningstyp och Huvudsökningsfönstren finns valet att  
<term>Spara inställningarna</term>. Detta ger dig möjligheten att spara de aktuella sökkriterierna för senare användning.
Kriterierna sparas till en <term>Sökningsfil</term> med filändelsen  .sso. För att återanvända en tidigare sparad sökningfil
väljer du <menu>Öppna ...</menu> från menyn <menu>Sök</menu>.
</p>

<h3>Söktider och ignorerade partier</h3>
<p>
De flesta sökningar meddelar en hur lång tid sökningen tog och hur många partier som <term>Ignorerades</term>. Ignorerade partier
är de som exkluderats från en sökning utan att något av dess drag har avkodats (från Scids databasformat). Detta baseras på information
som lagrats i index. Se vidare hjälpfilen om <a Formats>Filformat</a> för ytterligare information. 
</p>

<p><footer>(Uppdaterad: Scid 3.0, november 2001)</footer></p>
}

#################
### Clipbase help:

set helpTitle(W,Clipbase) "Urklippsdatabasen"
set helpText(W,Clipbase) {<h1>Urklippsdatabasen</h1>
<p>
Förutom de databaser du öppnar själv, öppnar Scid ytterligare en - <term>Urklippsdatabasen</term>. Urklippsdatabasen,
fungerar precis som alla andra databaser, med ett undantag. Den existerar bara i datorns arbetsminne och sparar inte data
någonstans permanent. Urklippsdatabasen töms således när du stänger av datorn.
</p>
<p>
Urklippsdatabasen är användbar som en tillfällig lagringsplats, för att slå samman sökresultat från olika databaser, eller för att man där
kan behandla resultat från en sökning som en egen databas. 
</p>
<p>
Antag, exempelvis, att du vill förbereda dig för en särskild motståndare. Du har sökt igenom databasen efter partier av denne där motståndaren
spelar vit. Kopiera alla partier i <a Searches Filter>Sökfiltret</a> till Urklippsdatabasen. Du kan göra det genom att i <a Switcher>Databasväxlaren</a>
 dra dem från deras ordinarie databas till Urklippsdatabasen. Därefter kan du öppna Urklippsdatabasen och undersöka partierna i  <a Tree>Trädfönstret</a>, 
till exempel för att kartlägga motståndarens öppningsrepetoar.
</p>
<p>
Observera att du kan kopiera partier från en databas till en annan utan att använda Urklippsdatabasen som mellanlagringsplats. Notera också att
Urklippsdatabasen <i>inte</i> kan stängas via menyvalet <menu>Arkiv: Stäng</menu>medan du är i Urklippsdatabasen. Det som händer då är
motsvarigheten till  <menu>Redigera: Återställ Urklippsdatabasen</menu>, dvs databasen töms.
</p>
<p>
Du kan maximalt ha 100000 partier i minnet samtidigt.
</p>

<p><footer>(Uppdaterad: Scid 2.5,  juni 2001)</footer></p>
}

#################################
### Variations and comments help:

set helpTitle(W,Annotating) "Kommentera partier"
set helpText(W,Annotating) {<h1>Kommentera partier</h1>
<p>
I Scid kan lägga till noter till partierna. Det finns tre typer av anteckningar du kan lägga till till varje drag:
symboler, kommentarer och varianter.
</p>

<h3>Symboler och kommentarer</h3>
<p>
Symboler används för ställningsbedömningar om vem som står bäst (t ex "+-" eller "=") , om draget som just gjordes var bra eller dåligt
(t ex "!" eller "?") medan kommentarer kan bestå av valfri text. För att lägga till symboler och kommentarer använder man
<a Comment>Kommentarseditorn</a>. Det finns också en särskild hjälpsida för <a NAGs>Standardsymbolerna</a>.
</p>
<p>
Tänk på att även om varje drag kan ha fler symboler, kan det bara ha en kommentar. En kommentar för första draget skrivs ut som 
text före partidragen. 
</p>

<h3><name Vars>Varianter</name></h3>
<p>
En <term>variant</term> är en alternativ fortsättning av partiet som skulle ha kunnat utföras vid något
tillfälle i partiet. Varianter kan innehålla kommentarer, symboler och varianter. Knappen märkt "<b>V</b>" 
ovanför brädet i huvudfönstret, samt valmöjligheter från menyn <menu>Redigera</menu> kan användas till att skapa,
navigera i och redigera varianter.
</p>

<h4>Kortkommandon</h4>
<p>
När det finns varianter till ett drag visas dem i partiinformationsområdet. Den första varianten är <b>v1</b>, den andra
är <b>v2</b>, osv. För att följa en variant kan man klicka på den, eller trycka  "<b>v</b>" följt av variantens nummer 
(Om det bara finns en variant räcker det med att trycka <b>v</b>.). Kortkommandot för att gå ur en variant är "<b>z</b>".
</p>

<h3><name Null>Null-drag</name></h3>
<p>
Det kan ibland vara användbart att hoppa över drag i varianter. Du kan exempelvis till draget 14.Bd3 lägga till en variant och
nämna att det hotar 15.Bxh7+ Kxh7 16.Ng5+ med angrepp. Du kan också göra detta genom att använda <term>Null-drag</term>
mellan 14.Bd3 och 15.Bxh7+, i exemplet ovan. Ett null-drag visas som "<b>--</b>" och infogas genom att med musen utföra det
ogiltiga draget att med kungen slå den andra kungen, eller att helt enkelt skriva in "<b>--</b>" (två minustecken).
</p>
<p>
Observera att null-drag inte ingår i PGN standarden, så om du vill exportera partier som innehåller null-drag till en PGN-fil måste du 
välja mellan att bevara null-dragen(-t) eller konvertera dem till kommentarer för att bibehålla exportmöjligheter till andra
program. Se hjälpsidan för att <a Export>Exportera</a> partier för mer information.
</p>

<p><footer>(Uppdaterad: Scid 3.4, juli 2002)</footer></p>
}

####################
### Analysis window:

set helpTitle(W,Analysis) "Analysfönstret"
set helpText(W,Analysis) {<h1>Scids Analysfönster</h1>

<h3>Analysfönster</h3>
<p>
Scids analysfönster visar analysen från en schackmotor
av den aktuella positionen. När positionen ändras skickar Scid den 
nya positionen till schackmotorn samt visar dess värdering av 
denna position. 
</p>

<p>
Värderingen som visas i analysfönstret är alltid från vits perspektiv, 
så en negativ värdering indikerar att svart står bättre. Den nedre delen av fönstret 
(med scrollrutan) visar historiken för värderingar producerat av schackprogrammet för den 
aktuella positionen, så att du kan se hur värderingen har ändrats. 
</p>

<p>
För att lägga till det bästa draget valt av en schackmotor som ett nytt drag i det aktuella partiet 
klickar du på Lägg till drag knappen. 
</p>

<h3>Schackmotorlistan</h3>

<p>
Scid behåller en lista på de schackmotorer du har använt, tillsammans med en uppskattad Elo-rating 
(om du antar en) och datumet när varje schackmotor sist användes. Du kan sortera schackmotorlistan 
efter namn, Elo-rating, eller datum. Välj Ny eller Ändra knapparna för att lägga till en ny schackmotor till 
listan eller ändra detaljerna för en existerande schackmotor. 
</p>

<h3>Schackmotorkommandon och kataloger</h3>

<p>
För varje schackmotor måste du specificera den exekuterbara filen att köra samt i vilken katalog Scid ska 
köra den i. 
</p>

<p>
Den mest sannorlika orsaken till problem att starta en schackmotor är valet av vilken katalog schackmotorn 
ska köra i. Vissa schackmotorer kräver en inställnings- eller öppningsboksfil i sina startkataloger 
för att fungera bra. Andra schackmotorer (som Crafty) skriver loggfiler till katalogen de startar i, så 
du måste köra dem i en katalog där du har tillstånd att skriva i. Om kataloginställningarna för en 
schackmotor är ".", kommer Scid att starta en schackmotor i den aktuella katalogen. 
</p>

<p>
Så om en schackmotor som borde fungera bra i Scid inte startar försök att ändra dess kataloginställning. 
För att undvika att schackmotorer skapar loggfiler i många olika kataloger rekommenderar jag att starta schackmotorer 
i katalogen med Scids användarfiler (detta är där Scid.exe filen finns i Windows, 
eller ~/.scid/ i Unix); det finns en knapp i dialogrutan för att ändra schackmotordetaljer med namnet 
Scid.exe-katalogen i Windows eller ~/.scid i Unix som låter dig välja vilken schackmotor som ska starta i denna katalog. 
</p>

<h3>Träning</h3>

<p>
Med Träningsknappen kan du spela drag mot schackprogrammet. Tiden för varje drag 
är fixerad och analysresultet visas inte då. 
</p>

<h3>Kommentera ett parti</h3>

<p>
Lägg till variation knappen i analysfönstret lägger till den aktuella värderingen och bästa analyserade
dragföljd som en ny variation i partiet. 
</p>

<p>
Du kan göra detta automatiskt för ett antal drag (kommentera partiet) genom att klicka på 
kommentarknappen. Detta frågar dig efter några kommentarsalternativ och sedan startar 
självspelsmod. När självspelsmod används och analysfönstret är öppet läggs 
en variation innehållande värderingen och bästa dragföljden automatiskt till för varje 
position när självspelsmod går igenom partiet. Bara positioner från den aktuella 
positionen till slutet av partiet (eller tills du lämnar självspelsmod) kommenteras, 
så du kan hoppa över att kommentera öppningsdragen genom att gå till en mittspelsposition 
innan du startar självspel. 
</p>

<p>
För att avbryta kommentarer stäng av självspelsmod, till exampel genom att trycka
ner Esc-tangenten i huvudfönstret. 
</p>

<p>
Observera att för enkelhet finns kommentarknappen bara tillgänglig i fönstret öppnat som 
schackmotor 1. Om du öppnar en schackmotor som schackmotor 2 kan du inte använda den för att
kommentera partiet. 
</p>

<h3>Analysbrädet</h3>

<p>
Genom att klicka på schackbrädeknappen i ett analysfönster visar du eller döljer du analysbrädet, 
som visar positionen vid slutet av den av den bästa funna aktuella dragföljden hittad av 
schackmotorn. 
Detta fungerar för de flesta Scid-kompatibla schackmotorer men inte för alla; 
det beror på den dragnotation som schackmotorn använder. 
</p>

<h3>Schackmotorprioritet</h3>

<p>
Om en schackmotor använder för mycket CPU-tid och påverkar användningen av Scid eller 
andra program kan det hjälpa att sätta på "Låg CPU-prioritet" i proxyn; 
det ger schackmotorn en låg prioritet för CPU-användning. 
</p>

<h3>Gränssnittsdetaljer</h3>

<p>
För att använda analysfönstret behöver du ett schackprogram som stöder WinBoard/Xboard-protokollet. 
</p>

<p>
Scid skickar kommandorna <b>"xboard"</b>,<b>"post"</b> och <b>"protover 2"</b> till en schackmotor när den startar, 
och använder <b>"setboard"</b> och <b>"analyze"</b> kommandorna för mer effektiv kommunikation 
om schackprogrammet svarar vilket indikerar att det stöder dem. Om en schackmotor inte stöder 
<b>"setboard"</b> kommandot kan det inte analysera ett parti som börjar med en 
ickestandard startposition. 
</p>

<p>
För program som inte stöder analyskommandot skickar Scid följande 
kommandon varje gång positionen ändras: <b>"new"</b> och <b>"force"</b>, 
sedan dragen i partiet till den aktuella position sedan <b>"go"</b>. 
</p>

<p>
Bob Hyatts utmärkta gratis schackmotor Crafty är vad jag använder och rekommenderar för 
analysfönstret, men många andra WinBoard eller XBoard kompatibla schackmotorer har 
använts framgångsrikt med Scid. Några nedladdningssajter för några schackmotorer listas nedan. 
</p>

<p>
Crafty: <url ftp://ftp.cis.uab.edu/pub/hyatt/>ftp://ftp.cis.uab.edu/pub/hyatt/</url> 
</p>

<p>
Yace: <url http://home1.stofanet.dk/moq/>http://home1.stofanet.dk/moq/</url> 
</p>

<p>
Phalanx: <url ftp://ftp.math.muni.cz/pub/math/people/Dobes/>ftp://ftp.math.muni.cz/pub/math/people/Dobes/</url> 
</p>

<p>
Comet: <url http://members.aol.com/utuerke/comet/>http://members.aol.com/utuerke/comet/</url> 
</p>

<p>
Gnuchess: <url http://www.gnu.org/software/chess/chess.html>http://www.gnu.org/software/chess/chess.html</url> 
</p>

<p>
The Crazy Bishop: <url http://remi.coulom.free.fr/>http://remi.coulom.free.fr/</url> 
</p>
<p><footer>(Uppdaterat: Scid 3.4, September 2002)</footer></p>
}





###############################
### Comment editor window help:

set helpTitle(W,Comment) "Kommentareditor"
set helpText(W,Comment) {<h1>Scids Kommentareditor</h1>

<h3>Kommentarseditorfönstret</h3>

<p>
Kommentareditorfönstret låter dig lägga till eller ändra kommentarer och symboliska kommentarssymboler 
för drag i det aktuella schackpartiet. 
</p>

<h3>Kommentarsymboler</h3>

<p>
Scid använder PGN-standarden för kommentarsymboler, accepterar NAGs (Numeriska annotationsglypfer) 
värden för kommentarer. Några av de mest vanliga symbolerna (som "!" eller "+-") visas 
som symboler och har en knapp i kommentarseditorfönstret för snabb inmatning. För andra symboler, 
kan du skriva in det lämpliga numeriska NAG-värdet som är ett nummer från 1 till 255. Till exempel, 
NAG-värdet 36 betyder "Vit har initiativet" och visas som "$36" i PGN-texten till partiet.  
</p>

<p>
Se hjälpsidan om NAG-värden för NAG värden som definieras av bl a PGN-standard. 
</p>

<p>
Tips: Du kan lägga till de vanligaste dragvärderingssymbolerna (!, ?, !!, ??, !? och ?!) i huvudfönstret 
utan att behöva använda kommentarseditorfönstret, genom att skriva in symbolerna följt av att trycka 
på <b>[Enter]</b> tangenten. Detta är speciellt användbart om du skriver in schackdrag genom att använda
tangentbordet. 
</p>

<h3>Kommentarer</h3>

<p>
Du kan ändra kommentarer genom att skriva in text och använda Rensa, Ångra och Lagra 
knapparna. Du behöver inte trycka på Lagra knappen för att uppdatera en kommentar; 
den uppdateras automatiskt när du går till en annan position i partiet. 
</p>

<h3>Färga rutor</h3>

<p>
Du kan färga alla rutor med valfri färg genom att använda ett specialkommando,som kan 
finnas varsomhelst i en kommentar. Kommandoformatet är: 
</p>

<p>
[%mark square color]
</p>

<p>
där square är ett rutnamn som d4 och färg är varje definierat färgnamn 
(som red, blue, darkGreen, lightSteelBlue, etc) eller RGB-kod (a # följt av 
sex hexadecimala siffror, som #a0b0c8). Om färgen utelämnas blir rutan
röd. 
</p>

<p>
En kommentar kan innehålla obegränsat antal färgkommandon, men varje måste ha sin 
egen [%mark ...] markering. Till exempel, kommentarstexten
</p>

<p>
Nu är d6-rutan  [%mark d6] svag och springaren kan attackera den från b5. [%mark b5 #000070] 
</p>

<p>
färgar rutan d6 röd och rutan b5 färgas med den mörkblå färgen #000070. 
</p>

<h3>Rita pilar</h3>

<p>
Du kan rita en pil från en ruta till en annan genom att använda ett specialkommentarskommando 
liknande det för att färga rutor. Formatet är: 
</p>

<p>
[%arrow fromSquare toSquare color]
</p>

<p>
där fromSquare och toSquare är namn på rutor som d4 och färg är varje definierat 
färgnamn (som  red, blue, etc) eller RGB code (som #a0b0c0). Om färgen inte specificeras
blir den röd. 
</p>

<p>
Till exempel, kommentarstexten 
</p>

<p>
C3-springaren och c4-löparen kontrollerar den svaga d5 rutan. 
[%arrow c3 d5 red] [%arrow c4 d5 blue] 
</p>

<p>
kommer att rita en röd pil från c3 till d5 och en blå pil från c4 till d5. 
</p>

<p><footer>(Uppdaterat: Scid 3.2, February 2002)</footer></p>
}


###########################
### Crosstable window help:

set helpTitle(W,Crosstable) "Korstabellfönster"
set helpText(W,Crosstable) {<h1>Scids korstabellfönster</h1>

<h3>Korstabellfönstret</h3>

<p>
Korstabellfönstret visar turneringskorstabellen för det aktuella partiet. 
Varje gång du uppdaterar korstabellfönstret (genom att trycka på Uppdatera knappen, 
genom att trycka Enter tangenten i korstabellfönstret, eller genom att trycka <b>Ctrl+Shift+X</b> 
i huvud- eller partilistefönstren), söker Scid efter alla partier i samma turnering som det
aktuella partiet. 
</p>

<p>
Alla partier som spelas upp till tre månader före eller efter det aktuella partiet, 
med exakt samma Evenemangs- och Ortmarkeringar anses vara i turneringen. 
</p>

<p>
Ett enkelt klick med vänstra musknappen på ett resultat i korstabellen laddar detta  
parti. Du kan lägga till alla partier i turneringen till filtret med Lägg till filter knappen
i korstabellen. 
</p>

<h3>Korstabellfönstermenyer</h3>

<p>
<b>[Arkiv]</b> menyn låter dig spara den aktuella tabellen till en fil med text-, 
LaTeX- eller HTML-tabellformat. 
</p>

<p>
<b>[Visa]</b> menyn låter dig välja tabellformat: 
All-play-all, Swiss eller Knockout eller Auto. 
</p>

<p>
All-play-all formatet (för round-robin-typ evenemang) har en begränsning på 30 spelare, 
men Swiss formatet (för turneringar med många spelare) kan visa upp till 
200 spelare och upp till 20 rundor. Auto, som väljer det bästa formatet automatiskt 
för varje turnering är standardinställningen. 
</p>

<p>
Observera att Scid använder Rundemarkeringen för varje parti för att producera en Swisskorstabell, 
så du kommer inte att se några partier i Swisskorstabellen för en turnering om dess partier inte 
har numeriska rundevärden som: 1, 2, 3, etc. 
</p>

<p>
Visa menyn låter dig också ställa in de data som ska presenteras att inkludera eller exkludera rating, 
länder och spelartitlar. Du kan också välja om färgallokationer i Swisstabeller ska synas. 
</p>

<p>
Separat poänggrupps-alternativet påverkar bara utseendet på tabellen när spelarna sorteras
efter poäng: det ger en tomrad mellan varje grupp av spelare med samma poäng. 
</p>

<p>
<b>[Sortera]</b> menyn låter dig sortera spelarna efter namn, rating eller poäng; 
efter poäng är standardinställningen. 
</p>

<p>
<b>[Färg]</b> menyn låter dig sätta på eller av färg i hypertextvisning. Eftersom det kan ta lång tid att 
formattera och visa stora korstabeller i hypertext sparar det mycket tid att välja vanlig text
för stora evenemang. Men i enkel text kan du inte klicka på spelare eller partier. 
</p>

<h3>Duplikerade partier i korstabeller</h3>

<p>
För att få bra resultat med korstabeller bör du markera dubblettpartier för borttagning och 
ha en konsistent stavning i partierna av namn på spelare, ort och evenemang. Se Underhåll 
databas för hjälp med att ta bort dubbletter och ändra (eller stavningskontrollera) 
spelar-/evenemangs-/ortsnamn. 
</p>

<p><footer>(Uppdaterat: Scid 3.1, December 2001)</footer></p>
}


###########################
### Database switcher help:

set helpTitle(W,Switcher) "Databasväxlaren"
set helpText(W,Switcher) {<h1>Scids databasväxlare</h1>

<h3>Databasväxlaren</h3>

<p>
Databasväxlaren ger en möjlighet att enkelt byta mellan databaser 
eller kopiera partier mellan databaser. Namnet, filterinställningen 
pch en ikon för varje databas visas medan den aktiva databasen 
markeras med en gul bakgrund. 
</p>

<p>
Du kan öppna databasväxlaren från  <b>[Fönster]</b> menyn eller genom att 
använda dess tangentkombination: <b>Ctrl+D</b>. 
</p>

<p>
För att kopiera alla filtererade partier från en databas till en annan, drag med den vänstra 
musknappen nedtryckt från källdatabasen till destinationsdatabasen. Du kommer då att se en  
bekräftelsedialog (om destinationsdatabasen inte är Clipbase) om partierna kan 
kopieras eller se ett felmeddelande om partierna inte kan kopieras (till exempel, 
om en vald databas inte är öppnad). 
</p>

<p>
Genom att klicka med höger musknapp på en databas visas en popup-menu för 
denna databas, från vilken du kan ändra databasikon eller nollställa databasens 
filter. Du kan också använda denna meny för att ändra orienteringen av den 
(för att arrangera databasen vertikalt eller horisontellt) vilket är användbart för 
mindre bildskärmar. 
</p>

<p><footer>(Uppdaterat: Scid 3.1, December 2001)</footer></p>
}


######################
### Email window help:

set helpTitle(W,Email) "Email-fönster"
set helpText(W,Email) {<h1>Scids Email-fönster</h1>

<h3>Email-fönstret</h3>

<p>
Scids email-fönster ger en möjlighet att hantera korrespondensschackpartier 
med email. Om du inte spelar emailschack har du inget intresse för detta. 
Men om du spelar korrespondensschack via email kan du skicka dina 
emailmeddelanden direkt från Scid! 
</p>

<p>
För att använda emailhanteraren följ följande steg: 
</p>

<p>
<li>1) Skapa partierna för din motståndare i databasen. </li>
<li>2) I emailhanteringsfönstret välj Lägg till och skriv in detaljerna om dina motståndare: 
namn, email-adress samt antal partier i databasen. </li>
<li>3) Välj Skicka email i emailfönstret varje gång du har lagt till drag till partierna 
och vill skicka ett meddelande. </li>
När du skickar ett email-meddelande genererar Scid meddelandet med partierna 
i PGN-format utan kommentarer eller variationer eftersom du vanligtvis inte vill att din 
motståndare ska se din analys. Du kan ändra meddelandet innan du skickar det för att lägga
till villkorade drag eller annan text. 
</p>

<p>
För varje motståndare kan du ha obegränsat antal partier; en eller två är det mest vanliga. 
Observera att Scid inte kontrollerar om antalet partier ändras, så efter att ha ställt in detaljerna 
om dina motståndare var noga att undvika att ta bort partier eller sortera din databas med emailpartier 
eftersom detta kommer att ändra i partier och göra antalet partier för varje motståndare felaktigt. 
</p>

<h3>Begränsningar</h3>

<p>
Scid har inte ännu möjlighet att kontrollera din emailkatalog, så du måste fortfarande lägga till
dina motståndares drag till partierna manuellt. 
</p>

<h3>Konfiguration<h3>

<p>
En kopia av varje emailmeddelande som skickats av Scid sparas i filen ~/.scid/scidmail.log. 
Om du vill att de ska sparas i en annan fil behöver du ändra i filen tcl/start.tcl 
och omkompilera Scid. 
</p>

<p>
Scid kan skicka emailmeddelanden genom att använda en SMTP server eller sendmail. 
Använd inställningsknappen i emailhanteraren för att specificera vilken du vill använda. 
</p>

<p>
Scid lagrar motståndardetaljerna för en databas i en fil med samma namn som databasen
med filändelsen  ".sem". 
</p>

<p><footer>(Uppdaterat: Scid 3.0, November 2001)</footer></p>
}


############################
### File Finder window help:

set helpTitle(W,Finder) "Sök filer"
set helpText(W,Finder) {<h1>Sök filer</h1>

<h3>Sök filer<h3>

<p>
Sök filer hjälper dig att hitta filer som du kan använda i Scid: 
databaser, PGN-filer, EPD-filer samt repetoarfiler. 
</p>

<p>
Den visar användbar information om varje fil, som dess storlek 
(see nedan) och datum för senaste ändringen. Du kan öppna varje visad 
fil genom att välja den genom att klicka en gång med vänstra musknappen. 
</p>

<h3>Söka i underkataloger</h3>

<p>
När du vill hitta alla filer i alla underkataloger till den aktuella katalogen, 
så markerar du i markeringsrutan för Leta i underkataloger. Detta får Scid att rekursivt 
genomsöka varje underkatalog efter filer som kan öppnas i Scid. Detta kan 
ta lång tid om det finns många underkataloger, så du kanske inte vill göra detta 
för en katalog nära roten av filsystemet. Du kan avbryta filsökningen genom att 
trycka på Stoppknappen. 
</p>

<h3>Filstorlekar</h3>

<p>
Betydelsen av de filstorlekar som visas beror på filtypen. 
För Scid-databaser och PGN-filer är det antalet partier. För EPD-filer 
är det antalet positioner. För repetoarfiler är det antalet 
(inkludera- eller exkludera)öppningar. 
</p>

<p>
För alla filtyper utom Scid-databaser är den angivna filstorleken en uppskattning baserad på 
de första 64 kilobytesen av filen, så den angivna storleken kanske inte är korrekt för 
filer som är större än 64 kb. Uppskattade storlekar visas med ett ungefärtecken (~) 
för att visa att de inte är exakta. 
</p>

<p><footer>(Uppdaterat: Scid 2.7, September 2001)</footer></p>
}


#########################
### GameList window help:

set helpTitle(W,GameList) "Partilistan"
set helpText(W,GameList) {<h1>Partilistan</h1>
<p>
Partilistafönstret ger en enradig sammanfattning av varje
parti i det aktuella <term>Filtret</term> .
</p>

<h3>Navigationen i partilistan</h3>

<p>
Du kan bläddra i partilistan med skjutregulatorn eller med de fyra
knapparna i nedre delen av listan .
Du kan också använda tangenterna <b>[Pos1]</b>, <b>[End]</b>, <b>[Bild i]</b>, <b>[Bild ab]</b> såväl som
cursortangenterna , för att scrolla med tangentbordet.
</p>

<p>
Du kan hitta  nästa parti i listan, som innehåller en bestämd
text i Vit-, Svart-, Turnerings- eller Ortsfältet , med 
<b>Sök text</b>-inmatningsfälten.
</p>

<h3>Åtgärder med partier i listan</h3>

<p>
För att ladda ett parti på listan klickar du dubbelt med
den vänstra musknappen.
Ett klick med dem mittersta musknappen visar de första dragen i partiet;
det är nyttigt, när man vill se öppningen före öppnandet av ett parti.
</p>
<p>
Den högra musknappen ger en meny för det utvalda partiet,
där du kan ta bort partier (eller "ta tillbaka partier") eller utesluta partier från 
filtret. Observera att borttagandet inte görs direkt utan det görs först
när databasen  <a Compact>komprimeras</a>.
</p>

<h3>Konfigurera partilistan</h3>
<p>
För att konfigurera partilistan, klickar du med den vänstra eller
högra musknappen i en kolumnöverskrift. Du kan även ändra bredd,
, infoga eller ta bort samt ändra färg på varje kolumn.
</p>
<p>
När du bara vill ändra kolumnbredden, finns det för detta en
tangentkombination: klickar du med nedtryckt <b>[Ctrl]</b>- (eller
<b>[Shift]</b>)-tangent med den vänstra musknappen i 
den översta delen av kolumnen  blir kolumnen mindre med den högra
musknappen blir den bredare.
</p>

<h3>Ändra storlek på partilistan</h3>
<p>
Storleken på partilistan blir sparat i alternativfilen varje gång,
som du sparar dina inställningar.
Du kan också när du vill att partilistan standardmässigt ska visa 10 partier ,
förändra storleken på partilistafönstret och välja
<b>Spara inställningar</b> i <menu>Alternativ</menu>-menyn.
</p>

<h3><name Browsing>Se partier och infoga variant</name></h3>
<p>
Kontextmenyn för Partilistan (och några andra fönster, som bl a
<a Reports Opening>Öppningsrapport</a>-Fönstren och listan med de
<a Tree Best>Bästa partierna</a> i <a Tree>Dragträd</a>-Fönstret)
tillåter val av parti att ladda, titta på eller infogandet av en variant.
</p>
<p>
När <term>Betrakta parti</term> blir valt, blir dragen i
det utvalda partiet (utan kommentarer eller varianter) visat i ett
separat fönster. Det är en nyttig metod att titta på ett annat parti
i förgrunden utan att ändra i det aktuellt laddade partiet.
</p>
<p>
Funktionen <term>Infoga variant</term> gör det möjligt att infoga 
varianter i det utvalda partiet. Scid finner den
sista möjliga tidpunkten, då det valda partiet avvek från det aktuella
(med betraktande av dragomkastningar), och infogar denna position
som en variant. 
</p>

<p><footer>(Uppdaterat: Scid 3.2, Februari 2002)</footer></p>
}


#######################
### Import window help:

set helpTitle(W,Import) "Importfönstret"
set helpText(W,Import) {<h1>Importfönstet</h1>
<p>
Scids Importfönster möjliggör att enkelt infoga ett
parti i <a PGN>PGN-format</a> till Scid eller till ett annat 
program.
</p>
<p>
I det stora fönsterområdet skriver du  in eller infogar partitexten i
PGN-format och i det gråa området nedanför finns 
felmeddelanden eller varningar.
</p>

<h3>Bearbeta det aktuella partiet i Importfönstret</h3>
<p>
Importfönstret ger också en ytterligare bekväm möjlighet för
ändringar i det aktuella partiet: du kan i det aktuella partiet i
importfönster infoga (med funktionen <b>Infoga aktuellt parti</b>), 
ändra i texten och därefter klicka på <b>Importera</b> .
</p>

<h3>PGN-markeringar i Importfönstret</h3>
<p>
Scid förväntar sig PGN-markeringar ("header tags") som
<ul>
<li> <b>[Resultat "*"]</b> </li>
</ul>
före dragen, men du kan också infoga ett partifragment som
<ul>
<li> <b>1.e4 e5 2.Bc4 Bc5 3.Qh5?! Nf6?? 4.Qxf7# 1-0</b> </li>
</ul>
utan PGN-markeringar , och Scid importerar det.
</p>

<h3>Använda PGN-filer i Scid</h3>
<p>
När du vill använda en PGN-fil i Scid , utan att i förväg konvertera den med
<a Pgnscid>Pgnscid</a> , finns det två möjligheter.
</p>
<p>
I det första alternativet kan du importera parti-filen med menyalternativet
<menu>Verktyg: Importera PGN-partier ...</menu> .
</p>
<p>
Alternativet två vore att öppna PGN-filen direkt i Scid . Dessutom
blir PGN-filen bara öppnat för läsande och behöver mer minnesutrymme än
en jämförbar Scid-databas, denna möjlighet är alltså bara att rekommendera
för relativt små PGN-filer .
</p>

<p><footer>(Uppdaterat: Scid 2.5, Juni 2001)</footer></p>
}


########################
### Reports help:

set helpTitle(W,Reports) "Rapporter"
set helpText(W,Reports) {<h1>Rapporter</h1>
<p>
Hos Scid är en <term>Rapport</term> ett dokument, som innehåller information om
en bestämd Position och/eller en bestämd spelare . 
Scid kan skapa två typer av rapporter: 
Öppningsrapporter och Spelarrapporter.
</p>

<h3><name Opening>Öppningsrapport</name></h3>
<p>
Scid kan skapa en <term>Öppningsrapport</term> , som har interessant
information om den aktuella öppningspositionen. För att skapa 
öppningsrapporten förvissar du dig först att den aktuella positionen är den som
du vill ha rapporten för,
sedan väljer du <b>Öppningsrapport</b> i menyn <menu>Verktyg</menu>.
</p>
<p>
<term>Öppningsrapport</term>-fönstret ger resulatet av rapporten. 
<b>Data</b>-menyn innehåller alternativ,
för att spara rapporten i Text-, HTML- eller
<a LaTeX>LaTeX</a>-format.
</p>
<p>
De första avsnitten i rapporten presenterar information om de
partier, som har rapportpositionen, och de drag som har spelats i denna
ställning. Du kan se om öppningen är populär , om den leder till många korta remier 
och med vilka dragföljder (dragomkastningar) den blir uppnådd.
</p>
<p>
Avsnittet om positionella teman informerar om vanligheten av
bestämda typiska teman i rapportpartierna. Till detta blir
de första 20 dragen (alltså de första 40 positionerna från utgångsställningen)
i varje parti untersökt. För att ett parti ska finnas med som har ett bestämt tema
måste detta tema minst förekomma fyra gånger inom de första 20 dragen i partiet. 
Därmed undviks att ett kortsiktig förekomst av ett tema (som t ex en isolerad
dambonde, som genast blir slagen) att resultatet blir missvisande.
</p>
<p>
Den sista och längsta delen av rapporten är teoritabellerna. När du
sparar rapporten i en fil kan du välja om du bara vill ha teoritabellerna, 
en kompakt rapport utan teoritabellerna eller
hela rapporten.
</p>
<p>
Nästan alla avsnitt i rapporten kan ställas in med öppningsrapport-alternativ
eller stängas av eller på.
Du kan alltså anpassa rapporten, så att den bara innehåller den för dig intressanta 
informationen.
</p>
<p>
Med den mesta informationen i rapportfönstret, som är färgkodad, 
leder ett klick på vänster musknapp till en aktion. Till exempel kan du
ladda ett parti genom att klicka på det eller ställa in filtret genom att
klicka på ett positionstema så att rapporten bara innehåller partier med
detta tema .
</p>

<h4>Favoriter</h4>
<p>
Med  <menu>Favoriter</menu>-menyn i rapportfönstret har du 
möjlighet att förvalta en lista av fördefinierade öppningsrapportpositioner 
och mycket enkelt skapa rapporter för exakt dessa positioner. 
När du väljer "Infoga i rapport..." im Favorit-menyn
blir den aktuella ställningen vald som en särskild rapportposition. 
Du uppmanas då att ange ett namn, som då används när 
favoritrapporter skapas.
</p>
<p>
Välj "Skapa rapport..." i Favorit-menyn, för att skapa en rapport
för varje vald position i den aktuella databasen . 
I ett dialogfönster kan du då ange typ och format på rapporten 
såväl som den katalog, där rapportfilerna blir sparade. 
Den passande filändelsen för det valda utdataformatet
blir då automatiskt valt (till exempel ".html" för HTML-format).
</p>

<h3><name Player>Spelarrapport</name></h3>
<p>
En <term>Spelarrapport</term> liknar mycket en öppningsrapport,
men innehåller information om partier för en enskild spelare med
vit eller med svart. Du kan antingen få en spelarrapport via verktygs-menyn
eller från <a PInfo>Spelarinformations</a>-fönstret.
</p>
<p>
En Spelarrapport kan skapas antingen för alla partier/för en bestämd spelare
/för en bestämd färg eller bara för de partier, som
har fått den aktuella ställningen på huvudfönster-brädet.
</p>

<h3>Inskränkningar</h3>
<p>
För de flesta öppningsdata finns det en inskränkning till 2000 partier,
när öppningsposition innehåller fler än 2000 partier kan
några resultat vara lite oklara.
</p>
<p>
Dessutom finns det en begränsning till 500 partier för teori-tabellerna. När
öppningspositionen finns i fler än 500 partier , används bara de 500 partier med de 
högsta genomsnittliga Elo-talen. Antalet partier för
uppbyggnaden av teoritabellerna är inställningsbart.
</p>

<p><footer>(Uppdaterat: Scid 2.5, Juni 2001)</footer></p>
}


####################
### PGN window help:

set helpTitle(W,PGN) "PGN-fönster"
set helpText(W,PGN) {<h1>PGN-fönster</h1>
<p>
Scids PGN-fönster ger innehållet i det aktuella partiet i 
Standard-PGN-format. I Partitexten syns kommentarer i
{icke runda} och varianter i (runda) klamrar.
</p>

<h3>PGN-format</h3>
<p>
PGN (Portable Game Notation) är ett utbredd standard för överföring
av schackpartier mellan datorprogram. En PGN-parti består
av två delar.
Den första delen är PGN-markeringarna ("Header"), som innehåller inmatningar ("tags") som
till exempel
<b>[White "Kasparov, Gary"]</b>
och
<b>[Resultat "1/2-1/2"]</b> .
</p>
<p>
Den andra delen innehåller de egentliga partidragen i
algebraisk standardnotation (SAN) tillsammans med eventuella varianter,
<a NAGs>Kommentarsymboler</a> och <a Comment>Kommentarer</a>.
</p>

<h3>Åtgärder i PGN-fönster</h3>
<p>
Du kan navigera i partiet med PGN-fönstret: Med
ett klick med vänstra musknappen på ett drag går du till detta drag, med ett
klick med vänstra musknappen i en Kommentar kan du ändra i denna.
Cursortangenterna (liksom tangenterna <b>v</b> och <b>z</b> för
att börja respektive avsluta varianter) används för att navigera i 
partiet precis som i huvudfönstret.
</p>

<h3>Inställningar för PGN-presentation</h3>
<p>
Menyn PGN-fönster innehåller alternativ för fönstret i PGN presentationen. 
Scid kan visa partiet i färg eller visa det som en ren text
-- se  <menu>Presentations</menu>-menyn i PGN-fönstret.
Den färgade presentationen är lätt att läsa och tillåter dig att välja drag och
kommentarer med musen, men de är mycket långsammare än enkel text. 
För mycket långa partier bör du kanske välja enkel text.
</p>
<p>
Också formatet för kommentarer och varianten kan du förändra,
för att ge en bättre översikt eller för att låta ge ut i separata delar.
</p>
<p>
PGN-presentationsalternativ och storleken på PGN-fönstret blir alltid sparat i
konfigurationsfilen när du väljer <b>Spara alternativ</b> i <menu>Alternativ</menu>-Menyn.
</p>

<p><footer>(Uppdaterat: Scid 3.1, December 2001)</footer></p>
}


#######################
### Piece Tracker help:

set helpTitle(W,PTracker) "Pjässpåraren"
set helpText(W,PTracker) {<h1>Pjässpåraren</h1>

<h3>Pjässpåraren</h3>

<p>
Pjässpåraren är en funktion som spårar rörelserna för en särskild pjäs för alla partier 
i det aktuella filtret och genererar ett "fotavtryck" för att visa hur ofta varje pjäs 
har varit på en viss ruta. 
</p>

<p>
För att använda pjässpåraren se först till att filtret innehåller de partier du är 
intresserad av, som partier med en viss öppningsposition eller alla partier 
där en viss spelare hade de vita pjäserna. Välj sedan pjäs att spåra och 
ställ in andra alternativ; som förklaras här nedan. Efter detta tryck på 
Uppdateraknappen. 
</p>

<p>
Informationen om den spårade pjäsens rörelser visas på två sätt: ett grafiskt 
"fotavtryck" och en textlista med en rad med data per ruta. 
</p>

<h3>Välja pjäs att spåra</h3>

<p>
Schackpjäserna visas i startpositionen nedanför fotavtrycksdiagrammet. En enkel pjäs 
(som den vita b1 springaren eller den svarta d7 bonden) 
kan väljas med den vänstra musknappen och alla pjäser av samma typ och färg 
(som alla vita bönder eller båda svarts torn) kan väljas med den högra musknappen. 
</p>

<h3>Andra pjässpårarinställningar<h3>

<p>
Dragnummerområdet kontrollerar när spårningen ska starta och stoppa i varje parti. 
Standardinställningen på 1-20 (betyder att spårningen ska stoppas efter svarts drag nr 20) 
är lämpligt för att undersöka öppningsteman, men (till exempel) ett område som 15-35 
är bättre när man söker efter mittspelstrender. 
</p>

<p>
Det finns två typer av statistik som pjässpåraren kan generera: 
</p>

<p>
% games with move to square: visar vilken proportion av de filtrerade partierna som 
innehåller ett drag av den spårade pjäsen för varje ruta. Detta är standardinställningen 
och vanligtvis det mest passande valet. 
% time in each square: visar proportionen av tid den spårade pjäsen har spenderat 
på varje ruta. 
</p>

<h3>Tips</h3>

<p>
Det finns åtminstonde tre bra användningar av pjässpåraren: öppningspreparering, 
mittspelsteman och spelarpreparering. 
</p>

<p>
För öppningspreparering använd pjässpåraren med dragträdet öppnat. Genom att följa
pjäser kan du se trender i den aktuella öppningen som vanliga bondeframstötar, 
springarutposter samt var löparna oftast placeras. Du kan även finna det 
användbart att sätta dragantalet till att starta efter det aktuella draget i partiet, 
så att dragen fram till och med den aktuella positionen inte inkluderas i statistiken. 
</p>

<p>
För mittspelsteman kan pjässpåraren vara användbar när filtret har ställts in att 
innehålla en viss ECO-omfattning (genom en Partihuvudsökning) eller kanske ett 
mönster som en vit isolerad dambonde (genom en Material/mönstersökning). 
Sätt dragantalet till något lämpligt (som 20-40) och spåra pjäser för att t ex se 
bondeframstötar i det sena mittspelet eller tidiga slutspelet. 
</p>

<p>
För spelarpreparering använd en Partihuvudsökning eller Spelarinformationsfönstret
för att hitta alla partier av en viss spelare med en viss färg. Pjässpåraren kan då
användas för att t ex upptäcka hur sannorlikt det är att spelaren fianchettar löpare, 
gör lång rockad eller ställer upp bönderna på d5 eller e5. 
</p>

<p><footer>(Uppdaterat: Scid 3.3, April 2002)</footer></p>
}

#####################
### Player List help:

set helpTitle(W,PList) "Spelarlistan"
set helpText(W,PList) {<h1>Spelarlistan</h1>

<h3>Spelarlistan</h3>

<p>
Spelarlistan visar en lista på spelarnamn i den aktuella databasen. 
Genom att välja en spelare öppnas spelarinfofönstret för att visa mer detaljerad
information om den spelaren. 
</p>

<p>
Fem kolumner som syns visar varje spelares namn, Högsta uppnådda ELO, 
antal spelade partier samt året för deras äldsta och nyaste parti. 
Klicka på valfri kolumntitel på toppen av listan för att sortera listan efter den
kolumnen. 
</p>

<p>
Kontrollerna nedanför listan låter dig filtrera listinnehållet. 
Du kan ändra den maximala liststorleken, skriva in ett VERSAL-/gemenokänsligt spelarnamn 
prefix (som "ada" för att söka efter "Adams"), 
och begränsa Elo-omfattningen och antalet spelade partier. 
</p>

<p><footer>(Uppdaterat: Scid 3.4, August 2002)</footer></p>
}

#####################
### Player Info help:

set helpTitle(W,PInfo) "Spelarinfo"
set helpText(W,PInfo) {<h1>Spelarinfo</h1>

<h3>Spelarinfofönstret</h3>

<p>
Spelarinformationsfönstret skapas eller uppdateras när du klickar med den vänstra 
musknappen på ett spelarnamn i spelinformationsytan (nedanför schackbrädet) 
eller i korstabellfönstret. 
</p>

<p>
Den visar förhoppningsvis användbar information om spelaren, inklusive deras framgång 
med vit och svart, favoritöppningar (med ECO-kod), och ratinghistoria. 
</p>

<p>
Alla procenttal som visas är en förväntad poäng (mått på framgång), från den spelarens 
perspektiv -- så högre är alltid bättre för spelaren oavsett om de är vit eller svart. 
</p>

<p>
Du kan se spelarens ratinghistoria i en tabell genom att trycka på  Ratingtabellknappen. 
</p>

<p>
Varje antal skrivet i rött kan klickas med den vänstra musknappen för att ställa in filtret för de
partier den representerar. 
</p>

<p><footer>(Uppdaterat: Scid 2.5, Juni 2001)</footer></p>
}


###########################
### Repertoire editor help:

set helpTitle(W,Repertoire) "Repetoar-Editor"
set helpText(W,Repertoire) {<h1>Repetoar-Editor</h1>

<h3>Repertoar-Editor</h3>

<p>
Repetoar-editorn låter dig skapa, se och ändra repetoarfiler. 
En repetoarfil (*.sor) är en lista på de öppningspositioner du vill nå eller 
försöker undvika, och du kan använda den för att hantera dina 
schacköppningspreferenser men den går även att använda för att söka i
databaser med Scid. 
</p>

<h3>Repetoargrupper och linjer</h3>

<p>
En repetoar innehåller två typer av element: grupper och linjer. 
Grupper är inte en del av din repetoar; de används bara för att 
strukturera den på samma sätt som kataloger ger struktur till filerna på en hårddisk,CD,DVD
eller diskett. 
</p>

<p>
Linjer i en repetoar finns i två typer: Inkludera linjer, som representerar de 
öppningspositioner du är intereserad av och försöker uppnå medan exkluderalinjer,som är
de du inte är intresserade av att spela och försöker undvika. 
Till exempel, om du spelar en accepterad damgambit (QGA) dvs 1.d4 d5 2.c4 dxc4 som svart
och du spelar alla drag efter 3.e4 utom 3...Sf6, måste du ha 1.d4 d5 2.c4 dxc4 3.e4 som en 
Inkluderalinje och 1.d4 d5 2.c4 dxc4 3.e4 Nf6 som en Exkluderalinje. 
</p>

<h3>Kommentarer och anteckningar</h3>

<p>
Varje grupp eller linje kan ha kommentarer anknytna till sig. 
Det finns två typer: korta (enrads-) kommentarer som syns med rött i repetoarhierarkin brevid 
dragen i en grupp eller linje medan långa (flerradiga-) kommentarer bara visas när gruppen  
eller linjen väljs. 
</p>

<p>

<h3>Använda Repetoar-editorn</h3>

<p>
Den vänstra sidan av fönstret visar repetoarhierarkin. 
Du kan klicka på folderikoner för att expandera och krympa grupper, 
och klicka på dragen i en  grupp eller linje för att välja den och se dess kommentarer. 
Inkludelinjer visas med blå accepteraikoner medan exkluderalinjer visas med röda kors. 
</p>

<p>
Om en linje eller grupp har en kort kommentar, visas den efter dragen. 
Om den har en lång kommentar indikeras detta med ** efter dragen. 
Grupperna har en siffra inom parentes efter sina drag som visar det antal inkludera- och 
excluderalinjer de innehåller. 
</p>

<p>
Genom att klicka med höger musknapp på en grupp eller linje ger det en meny med funktioner
för att ta bort den eller förändra dess status. 
</p>

<h3>Den högra sidan av fönstret innehåller tre delar.</h3>

<p> 
Den första delen är en lista på dragen i den aktuellt valda linjen eller gruppen. 
Du kan klicka med vänster musknapp på den för att klistra in dragen i Importfönstret, 
vilket är användbart för att låta det aktuella partiet starta med en linje i repetoaren. 
Den andra delen innehåller en kort kommentar för linjen eller gruppen, 
och den tredje delen innehåller dess långa kommentar. 
</p>

<h3>Lägga till grupper och linjer till repetoaren</h3>

<p>
För att lägga till en linje eller grupp till fönstret mata bara in dragen på schackbrädet i 
huvudfönstret och använd sedan <b>[Redigera]</b> menyn i Repetoar-editorn för att lägga till det 
som en grupp, inkluderalinje eller excluderalinje. 
</p>

<p>
För att ta bort en grupp eller linje klicka med höger musknapp på den och välj det lämpliga 
kommandot från menyn som visas. 
</p>

<h3>Söka databaser med repetoarfiler</h3>

<p>
Repetoar-editorns <b>[Sök]</b> meny låter dig söka den aktuella databasen med repetoaren. 
Varje parti blir genomsökt efter positionerna i repetoaren och blir bara matchade i sökningen
om den djupaste repetoarpositionen som hittas är en inkluderalinje. 
</p>

<p>
Du kan välja att söka genom att använda hela repetoaren eller bara de visade linjerna. 
Att söka bara de visade linjerna är användbart när du bara vill använda en del av repetoaren. 
Till exempel kan en repetoar ha två huvudgrupper , 
en för 1.e4 och en för 1.d4. Om du är bara interesserad av 1.e4 linjerna, 
krymp bara 1.d4 gruppen och sök sedan bara de visade linjerna. 
</p>

<h3>Extra tips</h3>

<p>
En repetoarfil är bra för att hitta nya partier i dina öppningssystem. 
Till exempel varje gång du får en ny PGN-fil att lägga till till din huvuddatabas 
(som den excellenta vecko PGN-filen från The Week In Chess), 
öppna bara PGN-filen i Scid och gör en repetoarsökning. 
Sedan kan du bläddra i de filtrerade partierna för att se alla de spelade partierna som finns i 
din repetoar. 
</p>

<p>
Du kan vilja behålla två repertoarfiler: en för svart och en för vit, 
så att du kan söka med varje fil separat. 
</p>

<p>
En repertoarfil kan öppnas från kommandoraden, till exempel: 
scid minbas vit.sor 
</p>

<p>
Du kan ändra en repetoarfil (*.sor) i valfri texteditor, men var noga att bevara dess format 
så att den förblir laddbar eller sökbar i Scid. 
</p>

<p><footer>(Uppdaterat: Scid 2.6, Augusti 2001)</footer></p>
}


##################################
### Tournament Finder window help:

set helpTitle(W,Tmt) "Sök turneringar"
set helpText(W,Tmt) {<h1>Sök turneringar</h1>

<h3>Sök turneringar</h3>

<p>
Sök turneringar låter dig hitta turneringar i den aktuella databasen. 
Den söker i alla databasepartier och samlar data om de funna turneringarna. 
Observera att två partier anses vara i samma turnering om de har 
samma Evenemangsmarkering , samma Ortsmarkering och spelades inom tre månader. 
</p>

<p>
Du kan begränsa listan på turneringar med antalet spelare och partier, 
datum, genomsnittling Elo rating och land genom att ändra i fälten nedanför turneringslistan 
och sedan trycka på Uppdateraknappen. 
</p>

<p>
Den visade listan kan sorteras efter datum,antal spelare, antal partier, 
genomsnittlig Elo , Ort, Evenemang eller Namn . Välj kategori 
i <b>[Sorterings]</b> menyn eller klicka på en kolumntitel för att förändra i sorteringsfältet. 
</p>

<p>
För att ladda det första partiet i en visad turnering klicka bara med vänster musknapp 
när dess rad markerats. Detta kommer också att uppdatera Korstabellfönstret om det
är öppnat. 
Om du istället trycker höger musknapp kommer partiet att laddas och korstabellfönstret
blir öppnat även om det är stängt. 
</p>

<p>
För att snabba upp turneringssökningar är det en bra idé att välja en ganska litet område 
(som några år) eller välja ett speciellt land (efter dess trebokstavs standardkod). 
Genom detta minskas kraftigt det antal partier Scid måste gå igenom för att bilda 
turneringar av partierna i databasen. 
</p>

<p><footer>(Uppdaterat: Scid 3.3, April 2002)</footer></p>
}


####################
### Tree window help:

set helpTitle(W,Tree) "Trädfönstret"
set helpText(W,Tree) {<h1>Trädfönstret</h1>

<h3>Trädfönstret</h3>

<p>
Trädfönstret visar information om alla drag som gjorts från den aktuella 
positionen i partier i databasen. I trädmoden uppdateras trädfönstret 
automatiskt när brädet på huvudfönstret ändras. Detta kan vara långsamt 
för stora databaser. 
</p>

<p>
Observera att när trädfönstret uppdateras nollställs filtret och bara de partier 
som innehåller den aktuella positionen inkluderas. 
</p>

<p>
Genom att trycka på vänster musknapp på ett drag i trädfönstret så adderas detta drag
till partiet. 
</p>

<h3>Trädfönstrets innehåll</h3>

<p>
Trädfönstret visar ECO koden (om den finns), frekvens (både som antal 
partier och en procentsats) och värderingen av varje drag. 
Värderingen beräknas alltid från vits perspektiv, 
så 100% betyder att vit vinner alla partier  
medan 0% betyder att svart vinner alla partier. 
</p>

<p>
Dragen i trädfönstret kan sorteras efter drag (alfabetiskt), 
ECO-kod, frekvens, eller värdering. Du kan ändra sorteringsmetod i 
<b>[Sorterings]</b> menyn. 
</p>

<h3>Bästa partier</h3>

<p>
Trädfönstret har ett filmenykommando och knapp för att öppna de 
Bästa partierna, vilka visar en lista på de högst rankade partierna i 
den aktuellt visade trädgrenen. Partierna listas efter genomsnittlig 
rating och du kan begränsa listan till att visa partier med ett speciellt 
resultat. 
</p>

<h3>Trädgraffönstret</h3>

<p>
Trädfönsterknapparna inkluderar en knapp markerad Graph som ger 
en grafisk visning av det relativa utfallet för varje drag från den aktuella 
positionen. Alla drag som har spelat minst 1% och minst 5 gånger visas. 
Procentsiffror är alltid från vits perspektiv även när det är svarts drag. 
</p>

<p>
I trädgrafen plottas en röd linje för att visa genomsnittet av alla partier 
från den aktuella positionen och ytan mellan 50-55% (som de flesta 
standardöppningar förväntas få) är markerad med blått för att assistera 
jämförandet av drag. Observera att vit vanligtvis vinner cirka 55% av 
schackmästarpartierna. 
</p>

<h3>Låsa trädfönstret</h3>

<p>
Låsknappen i trädfönstret kan användas för att låsa trädet till den 
aktuella databasen. Detta betyder att trädet kommer att fortsätta att
använda denna databas även när du byter till en annan öppnad databas. 
Detta är användbart om du viss använda en stor databas som en referens 
när du spelar igenom ett parti i en annan databas: 
bara öppna trädet i referensdatabasen, lås det och växla sedan till
den andra databasen. 
</p>

<h3>Träning</h3>

<p>
När Träningscheckrutan i trädfönstret väljs kommer Scid  
slumpmässigt att göra ett drag varje gång du lägger till ett drag till partiet. 
Draget som Scid väljer beror på databasstatistiken, så ett drag som spelats
i 80% av databaspartierna kommer att väljas av Scid med 80% 
sannorlikhet. Att starta denna funktion och sedan dölja (eller ikonifiera)  
trädfönstret och spela öppningar mot en stor databas är ett bra sätt att
testa din kunskap om öppningsrepetoaren. 
</p>

<h3>Använda Trädet med öppnade EPD-filer</h3>

<p>
För varje öppnad EPD-fil kommer trädfönstret att innehålla en extra kolumn 
som visar en kort (fem tecken) sammanfattning av innehållet i 
EPD-filen för varje position som nås från de listade dragen. 
</p>

<p>
Sammanfattningen kan vara en värdering, en öppnings ECO-kod eller ett föreslaget drag; 
det kommer att vara innehållet i det första EPD-fältet hittat från följande lista: 
ce, eco, nic, pv, pm, bm, id
eller bara det första EPD-fältet om ingen av de ovan nämnda finns. 
</p>

<p>
För en beskrivning av EPD fälten, se EPD-fil hjälpsidan. 
Observera att om sammanfattningen är av ce fältet, 
visas det som en värdering i bönder från vits perspektiv
(istället för som em värdering i hundradelsbönder från perspektivet av sidan vid draget, 
som det sparas i EPD-filen) för att förbättra läsbarheten. 
</p>

<h3>Cache för snabbare resultat</h3>

<p>
Scid har en cache för trädsökresultat för de positioner med de 
mest matchande partierna. Om du går framåt och bakåt i ett parti 
i trädmod kommer du att se att trädfönstret uppdateras  nästan omedelbart 
när positionen som söks finns i cachen. 
</p>

<p>
Trädfönstret har ett filmenykommando kallat Spara Cache. 
När du väljer detta kommer det aktuella innehållet i trädcachen att sparas till 
en fil (med filändelsen * .stc) för att snabba upp framtida användning av 
Trädmoden med denna databas. 
</p>

<p>
Fyll cache filkommandot i filmenyn för trädfönstret fyller cachefilen med data för 
många öppningspositioner. Det gör en trädsökning för omkring 100 av de mest 
vanliga öppningspositionerna och sparar sedan cachefilen. 
</p>

<p>
Observera att en trädcachefil (*.stc) är helt överflödig; 
du kan ta bort den utan att påverka databasen, 
och den tas även bort av Scid när en åtgärd görs som kan göra den icke uppdaterad 
som till exempel att lägga till eller ersätta ett parti eller sortera databasen. 
</p>

<p><footer>(Uppdaterat: Scid 3.0, November 2001)</footer></p>
}


################
### Graphs help:

set helpTitle(W,Graphs) "Graffönster"
set helpText(W,Graphs) {<h1>Graffönster</h1>

<h3>Graffönster</h3>

<p>
Scid har ett antal fönster som visar information grafiskt. 
De förklaras här nedanför. 
</p>

<h3>Filtergraffönstret</h3>

<p>
Filtergraffönstret visar trender efter datum eller efter Elo-rating för 
partierna i det aktuella filtret jämfört med hela databasen. 
Till exempel är det användbart när trädet är öppnat som ett redskap 
för att visa hur den aktuella öppningspositionen har förändrats i popularitet 
på senare år eller tiotal år eller om det är speciellt populärt bland högre-rankade 
spelare som stormästare. Varje poäng på grafen representerar antalet partier i 
filtret per 1000 partier i hela databasen, 
för ett speciellt dator eller ett visst Eloratingområde. 
</p>

<p>
När filtergrafen plottas efter rating använder Scid den genomsnittliga 
(aritmetiskt medelvärde) ratingen för varje parti. Uppskattade ratings 
(som de som finns i stavningsfilen) används inte. Om en spelare i ett 
parti har en rating men motståndaren inte har det, 
antas motståndaren ha samma upp till 2200. 
Till exempel, om en spelare är rankad 2500 och motståndaren inte 
har en rating blir genomsnittsratingen (2500+2200)/2=2350. 
</p>

<h3>Ratinggraffönstret</h3>

<p>
Ratinggraffönstret visar ratinghistorian för en spelare eller två 
spelare i det aktuella partiet. Du kan producera grafen för en spelare 
genom att trycka på Ratinggrafknappen i spelarinformationsfönstret 
eller skapa den för de två spelarna i de aktuella partiet genom att välja Ratinggraf 
från <b>[Verktygs]</b> menyn. 
</p>

<h3>Värderingsgraffönstret</h3>

<p>
Värderingsgraffönstret visar de numeriska värderingarna (poäng) 
sparade i kommentarerna till det aktuella partiet som en graf. 
Du kan klicka med vänster musknapp någonstans i värderingsgrafen för att gå till
motsvarande position i partiet. 
</p>

<p>
Två typer av värderingskommentarer finns: dessa produceras av Scids analysfönster
(som har formatet 1.e4 {"+0.25 ...."} är alltid värderade från vits perspektiv) 
och de som produceras av Craftys analyskommando 
(som har formatet 1.e4 ({9:+0.25} ....) och också är värderade från vits perspektiv). 
</p>

<h3>Trädgraffönstret</h3>

<p>
Trädgraffönstret finns i trädfönstret. Det visar resultatet av de mest populära
dragen från den aktuella positionen. 
Mer information finns i avsnittet om trädfönster. 
</p>

<p><footer>(Uppdaterat: Scid 3.3, April 2002)</footer></p>
}


####################
### Tablebases help:

set helpTitle(W,TB) "Slutspelstabeller"
set helpText(W,TB) {<h1>Slutspelstabeller</h1>

<h3>Slutspelsdatabaser</h3>

<p>
En slutspelsdatabas är en fil som innehåller den perfekta resultatinformationen om alla positioner 
med ett visst material , som Kung och Torn mot Kung och Bonde. 
Slutspelstabeller för alla materiella situationer med upp till fem pjäser (inklusive Kungar) 
har genererats och några enkla 6-pjässlutspelstabeller finns också tillgängliga. 
</p>

<p>
Scid kan använda slutspelsdatabaser i Nalimov-format som används av många nyare schackmotorer. 
Dessa slutar ofta med ändelserna .nbw.emd eller .nbb.emd. Alla 3-, 4- och 5-pjäs 
Nalimov-slutspelstabeller kan användas i Scid. 
</p>

<h3>Använda slutspelstabeller i Scid</h3>

<p>
För att använda slutspelstabeller i Scid specificera bara deras kataloger
genom att välja Slutspelstabellkatalog i <b>[Alternativ]</b> menyn. Du kan välja upp till 
4 kataloger där dina slutspelstabeller är sparade. Du kan trycka på ... 
knappen till höger om en inmatning för att välja en katalog för att specificera att
katalogen till den ska användas. 
</p>

<p>
När en position som hittats i slutspelstabellen har nåtts visar partiinformationsytan 
nedanför schackbrädet slutspelstabellinformation. 
Du kan konfigurera mängden information som visas genom att klicka med den högra
musknappen i den eller välja Partiinformation från  <b>[Alternativ]</b> menyn. 
Genom att välja "Resultat och bästa drag"  ges den mest användbara informationen, 
men det är ofta mycket långsammare än "Bara resultat". 
</p>

<h3>Slutspelstabellfönster</h3>

<p>
Du kan få ännu mer slutspelstabellinformation om den aktuella positionen genom att 
öppna Slutspelstabellfönstet i ([Fönster] menyn, tangentkombination: <b>Ctrl+Shift+=</b>). 
Detta fönster visar resultatet med perfekt spel av alla legala drag från den 
aktuella positionen. 
</p>

<p>
Fönstret har två huvudsakliga delar. Sammanfattningsdelen (till vänster) visar 
vilka slutspelstabeller Scid har hittat på din dator och en sammanfattning för varje 
slutspelstabell. Resultatdelen (till höger) visar optimala resultat för alla drag från den
aktuella positionen som visas i huvudfönstret. 
</p>

<h3>Sammanfattningsdelen</h3>

<p>
Den övre delen av sammanfattningsdelen låter dig välja en speciell slutspelstabell. 
De som du har tillgängliga visas i blått och de slutspelstabeller som inte finns visas
i grått. Den nedre delen av sammanfattningsdelen visar sammanfattningsinformation
för den valda slutspelstabellen. 
(Alla slutspelstabeller har inte en sammanfattning i Scid) 
</p>

<p>
Sammanfattningen inkluderar frekvensen(hur många partier per million som når 
en position med detta material beräknat från en databas med mer än 
600000 mästerpartier), en längsta matt för en sida samt antalet 
ömsesidiga (eller "reciprokala") dragtvång. Ett ömsesidigt dragtvång är en 
position där vit vid draget får remi och svart vid draget förlorar eller där 
vit vid draget förlorar och svart vid draget får remi eller där den som är vid draget
förlorar. 
</p>

<p>
För några slutspelstabeller med ömsesidiga dragtvång visar sammanfattningen 
också en lista på alla dragtvångspositioner eller ett urval av dem. En fullständig
lista på alla slutspelstabeller är inte möjlig eftersom vissa slutspelstabeller har 
tusentals ömsesidiga dragtvång. 
</p>

<p>
Du kan sätta upp en slumpmässig position från den valda slutspelsdatabasen
genom att trycka på Slumpknappen. 
</p>

<h3>Resultatdelen</h3>

<p>
Resultatdelen uppdateras när schackbrädet i huvudfönstret ändras. 
Den första linjen visar hur många drag vinner (+), ger remi (=), förlorar (-), 
eller har ett okänt resultat (?). Resten av resultatdelen ger en mer detaljerad 
lista på resultat, rangordnande dem från kortaste till längsta matt, sedan remier, 
sedan längsta till kortaste förluster. Alla avstånd är till matt. 
</p>

<h3>Resultatbrädet</h3>

<p>
I en slutspelstabellposition är det ofta användbart att se vad 
slutspelstabellresultatet skulle vara om alla pjäser i den aktuella
positionen var på sina aktuella rutor men en särskild pjäs flyttas någon 
annanstans. Till exempel kan du vilja bestämma hur nära en kung måste vara
till en fribonde för att vinna eller få remi i en särskild position. I slutspelsböcker
kallas denna information ofta vinstzonen eller remizonen för en pjäs i en position. 
</p>

<p>
Du kan hitta denna information i Scid genom att trycka på knappen med en
bild på schackbrädet för att visa resultatbrädet i slutspelstabellfönstret. 
När du trycker på vänster musknapp på någon av pjäserna på brädet visas 
en symbol i varje tom ruta visande vad slutspelstabellresultatet blir 
(med samma sida vid draget som i den aktuella positionen på huvudfönstret) 
om den valda pjäsen var på den rutan. 
</p>

<p>
Det finns fem olika symboler en ruta kan ha: en vit # betyder att vit vinner; 
ett svart  # betyder att svart vinner; ett blått = betyder att positionen är remi; 
ett rött X betyder  att positionen är illegal (eftersom kungarna står brevid 
varandra eller sidan vid draget ger schack); och ett rött ? betyder att resultatet 
är okänt beroende på att den nödvändiga slutspelstabellen inte finns tillgänglig. 
</p>

<h3>Få tillgång till slutspelstabellsfiler</h3>

<p>
Se avsnittet om relaterade länkar för hjälp med att hitta slutspelstabeller på Internet. 
</p>

<p><footer>(Uppdaterat: Scid 3.4, September 2002)</footer></p>
}


###################
### Bookmarks help:

set helpTitle(W,Bookmarks) "Bokmärken"
set helpText(W,Bookmarks) {<h1>Bokmärken</h1>
<p>
Scid låter dig bokmärka viktiga partier för enkel framtida referens. 
Bokmärkesmenyn finns i <b>[Arkiv]</b> menyn och i verktygsraden och har
tangentkombinationen: 
<b>Ctrl+B</b> . 
</p>

<p>
När du väljer ett bokmärkt parti från Bokmärkesmenyn 
kommer Scid att öppna dess databas om det är nödvändigt, hitta partiet, 
och gå till den schackposition som var när den bokmärktes. 
</p>

<p>
Bara partier i en Scidformaterad databas (inte en PGN-fil eller Clipbasen) 
kan bokmärkas. 
</p>

<p>
Om databasen till ett bokmärkt parti sorteras eller kompimeras kan 
bokmärkesdetaljerna bli inaktuella. När det händer kommer Scid att
söka igenom databasen efter det bäst matchande partiet 
(jämför Spelare, Ort, Resultat, År etc) när bokmärket väljs, 
så att det bokmärkta partiet fortfarande blir laddat. Men om 
detaljerna i det bokmärkta partiet ändras är det möjligt att ett annat 
parti kommer att matcha bokmärkesdetaljerna bättre och laddas istället. 
Så det är en bra idé att om-bokmärka ett parti om du ändrar i dess 
Spelare, Ort, Resultat, Runda eller År. 
</p>

<h3>Ändra bokmärken</h3>
<p>
Med bokmärkeseditorn kan du ändra menytexten som visas för varje 
bokmärkt parti och lägga till kataloger för att kategorisera bokmärken. 
</p>
<h3>Tips</h3>
<p>
Du kan använda bokmärken för snabb tillgång till databaser du ofta 
använder genom att bokmärka ett parti från varje databas. En annan 
god användning för bokmärken är att lägga till viktiga partier du hittar
när du studerar en viss schacköppning. 
</p>

<p>
Bokmärkesmenyn innehåller två alternativ för att kontrollera visandet av 
bokmärkeskataloger : de kan visas som undermenyer 
(användbart när det finns många bokmärken) eller som en enkel lista. 
</p>

<p><footer>(Uppdaterat: Scid 3.0, November 2001)</footer></p>
}


##############################
### Command-line options help:

set helpTitle(W,Cmdline) "Kommandorad-alternativ"
set helpText(W,Cmdline) {<h1>Kommandorad-alternativ</h1>
<h3>Kommandorad-alternativ</h3>
<p>
När du startar Scid från ett gränsnitt eller consol, 
finns det kommandorad-alternativ du kan specificera. 
Scid-format databaser (med eller utan en filändelse som <b>"*.si3"</b>) 
och PGN-filer att öppna kan anges, till exempel: 
<b>scid minbas nyapartier.pgn</b> 
kommer att starta Scid och öppna Scid-databasen <b>mybase</b> och  
PGN-filen <b>nyapartier.pgn</b>. 
</p>

<p>
Det finns också optionella argument för att kontrollera vilka filer Scid 
ska leta efter och använda när det startar.
Du kan stänga av användningen av slutspelstabeller med <b>-xtb</b> (eller <b>-xt</b>) alternativet, 
undvika att ladda in ECO öppningsklassificeringsfiler med <b>-xeco</b> eller <b>-xe</b>, 
och undvika att ladda in stavningsfilen med -xspell eller -xs.
Utöver detta ger alternativet <b>-fast</b> eller <b>-f</b> alla dessa tre saker , 
så <b>scid -f</b> är samma som <b>scid -xeco -xspell -xtb</b>. 
</p>

(Updaterat: Scid 3.1, November 2001)


}


####################
### Compaction help:

set helpTitle(W,Compact) "Komprimera databas"
set helpText(W,Compact) {<h1>Komprimera databas</h1>
<p>
<h3>Komprimera databas</h3>

<p>
Att komprimera en databas är en speciell typ av underhåll 
som håller en databas så liten och effektiv som möjligt. 
Att komprimera en databas betyder att ta bort allt oanvänt
utrymme i dess filer . Det finns två typer: namnfils- och partifilskomprimering. 
</p>

<h3>Namnfilskomprimering</h3>

<p>
Över tiden kan du finna att databasen börjar att innehålla ett antal spelarnamn, 
evenemangs-, orts- eller rundenamn som inte längre används i något parti. 
Detta händer ofta efter att du stavningskontrollerar namn. 
De oanvända namnen slösar utrymme i namnfilen och kan sakta mer namnsökningar. 
Namnfilskomprimering tar bort alla namn som inte används i något parti. 
</p>

<h3>Partifilskomprimering</h3>

<p>
När ett parti ersätts eller tas bort lämnas oanvänt utrymme i partifilen
(den största av de tre filerna i en Scid databas). 
Partifilskomprimering tar bort allt oanvänt utrymme och lämnar inga borttagna partier
i databasen. Observera att denna operation är slutgiltig:
efter komprimering är de bortagna partierna borta för evigt! 
</p>

<p>
Partifilskomprimering rekommenderas också efter att ha sorterat en databas för att
hålla ordningen på partifilen konsistent med den sorterade indexfilen. 
</p>

<p><footer>(Uppdaterat: Scid 2.5, Juni 2001)</footer></p>
}


####################################
### Database maintenance tools help:

set helpTitle(W,Maintenance) "Underhåll databas"
set helpText(W,Maintenance) {<h1>Underhåll databas</h1>

<h3>Underhåll databas</h3>
<p>
Scid har ett antal redskap för att underhålla databaser, 
tillgångliga från Scids Arkivmeny. Databaskomprimeringen 
och sorteringsfunktionerna förklaras i separata avsnitt. 
</p>


<h3>Underhållsfönstret</h3>
<p>
Det mesta av Scids databasunderhåll kan göras från underhållsfönstret 
, som kan öppnas från 
<b>[Arkiv: Underhåll]</b> eller <b>[Fönster]</b> menyerna eller med tangentkombinationen:
<b>Ctrl+M</b>. 
</p>

<p>
Du kan använda detta fönster för att underhålla partimarkeringar, 
stavningskontrollera namn, komprimera eller sortera en databas. 
Observera att alla operationer som inte finns tillgängliga för den aktuella
databasen (till exempel, på grund av att den är skrivskyddad eller en 
PGN-fil) visas med grå färg. 
</p>

<h3>Ta bort dubbla partier</h3>
<p>
I <b>[Arkiv: Underhåll]</b> menyn finns ett kommando <b>[Ta bort dubbla partier...]</b> 
för att detektera extra kopior (dubbletter) av partier i databasen. 
Detta kommando hittar alla dubbletter och för varje par markeras det kortare 
partiet för borttagning medan det längre partiet lämnas kvar. 
Två partier anses vara dubbletter om spelarna 
(och alla andra markeringar som du kan specificera) matchar exakt.
</p>

<p>
Om du specificerar  "samma drag" alternativet måste varje par av partier ha 
samma drag upp till längden av det kortare partiet 
(eller upp till drag 60) för att vara dubbletter. 
</p>

<p>
När du har tagit bort dubbletter är det en bra idé att kontrollera att 
varje borttaget parti verkligen är en kopia av ett annat parti. Du kan göra detta
enkelt om du valde "Sätt filter till alla borttagna partier" alternativet i ta bort
dubbletter dialogrutan. Filtret kommer då att innehålla alla borttagna partier. 
Du kan bläddra genom dem  (med <b>p</b> och <b>n</b> tangenterna) med kontrollera 
dubbla partier fönstret  (finns i underhållsmenyn eller med tangentkombinationen <b>Ctrl+Shift+T</b>) 
för att verifiera att varje borttaget parti är borttaget för att det faktiskt är en dubblett av ett
annat parti. 
</p>

<h3>Ändra spelar, evenemang, ort och rundenamn</h3>

<p>
Du kan finna felstavade namn i din databas och vilja korrigera dem. 
Du kan göra detta i Scid med Namneditorfönstret 
(tangentkombination: <b>Ctrl+Shift+N</b>), som finns i <b>[Arkiv: Underhåll]</b>
undermenyn. 
</p>

<p>
Varje unikt namn lagras bara en gång i namnfilen, så om du ändrar ett namn så
ändrar du alla förekomster av det. 
</p>
<h3>Stavningskontrollera namn</h3>
<p>
Scid kommer med en stavningskontrollfil med namnet spelling.ssp, 
för att korrigera spelar-, evenemang-, orts- och rundenamn. 
Scid kommer att försöka ladda stavningskontrollfilen när det startar; 
om den inte laddas in då kan du ladda in den från <b>[Alternativ]</b> menyn. 
</p>

<p>
När stavningskontrollfilen är laddad kan du använda den på en Scid databas med 
stavningskontrollkommandorna i  <b>[Arkiv: Underhåll]</b> menyn eller från 
underhållsfönstret. 
</p>

<p>
När du stavningskontrollerar en databas producerar Scid en lista på korrektioner som du 
kan ändra i innan några ändringar görs, så att du kan ta bort alla korrektioner du inte
vill göra.
</p>

<p>
Stavningskontroll är speciellt viktigt för att standardisera en databas så att alla förekomster
av en speciell spelare stavas likadant. Till exampel, med standard stavningskontrollfilen, 
så korrigeras namnen "Kramnik,V.", "Vladimir Kramnik", och "V. Kramnik" till "Kramnik, Vladimir". 
</p>

<p>
Stavningskontrollfilen har en ytterligare användning: när den laddas används dess spelardata
för att förbättra spelarinformationsfönstret och korttabellfönstret: : du kommer att se FIDE 
mästartitlar som (gm = Internationell Stormästare, im = Internationell Mästare, etc) och 
landsinformation för spelarna listas i stavningskontrollfilen. 
Över 6500 starka spelare från historien och nutiden finns listade i <b>spelling.ssp</b> filen som 
kommer med Scid. 
</p>

<h3>Lägga till Elo rating till partier</h3>

<p>
"Lägg till Elo rating..." knappen i underhållsfönstret får Scid att söka igenom den aktuella 
databasen efter partier där en spelare inte har en rating, men där stavningskontrollfilen 
har en Elo rating listad för denna spelare vid tiden för partiet. Scid kommer att lägga till all 
sådan rating automatiskt. Detta är mycket användbart för en databas med mästarpartier 
som har få ratings.
</p>

<p>
Stavningskontrollfilen "spelling.ssp" som kommer med Scid innehåller inte Elo rating 
informationen som behövs för denna funktion, men en större variant av den kallad "ratings.ssp" 
finns på Scids webbsajt. 
</p>

<h3>Rensaren</h3>
<p>
Rensaren (som finns i Underhållsfönstret) är ett redskap för att göra ett antal 
underhållsarbeten på en databas med en åtgärd. Du kan välja vilka arbeten du vill 
göra och Scid kommer att utföra dem på den aktuella databasen utan att kräva
användarinteraktion. Detta är speciallt användbart för underhåll av stora databaser. 
</p>

<h3>Ställa in att databasen ska ladda in ett parti automatiskt</h3>
<p>
Vid automatiskt laddande av ett parti i en databas så laddas detta parti in när denna 
databas öppnas. För att ändra det automatiska laddandet av partiet i databasen, 
använd "Autoladda parti nummer..." knappen. Om du alltid vill att det sista partiet i en 
databas ska öppnas  (oavsett om hur många partier som finns i databasen),
så ställ in det till ett högt nummer som 9999999. 
</p>

<p><footer>(Uppdaterat: Scid 3.1, December 2001)</footer></p>
}


##############
### ECO guide:

set helpTitle(W,ECO) "ECO"
set helpText(W,ECO) {<h1>ECO-öppningsklassificering</h1>

<h3>ECO öppningsklassificering</h3>
<p>
Scid kan klassificera schackpartier enligt ECO 
(Encyclopedia of Chess Openings) schacköppningsklassificering. 
En standard ECO-kod består av en bokstav (A-E) följt av två siffror, 
så det finns 500 distinkta standard ECO-coder. 
</p>

<h3>Scids utvidgningar av ECOs öppningsklassificering</h3>

<p>
ECO klassificeringen är väldigt begränsad och inte tillräcklig för moderna partier:
några av de 500 koderna syns nästan aldrig, medan några syns väldigt ofta. 
För att förbättra denna situation tillåter Scid en optionell utökning av de grundläggande 
ECO-koderna: varje kod kan bli utökad med en bokstav (a-z) som dessutom kan utökas
ytterligare (med en siffra från 1-4)  men det används inte ännu i standard Scid ECO-filen . 
Så en utökad Scid ECO-kod ser ut antingen som "A41e" eller som "E99b2". 
Många av de mest vanliga ECO-koderna som finns i moderna mästarpartier har
utökningar definerade i Scids ECO-fil. 
</p>

<h3>ECO Bläddrarfönstret</h3>

<p>
ECO Bläddrarfönstret visar dig de positioner som används för att klassificera varje ECO-kod, 
samt frekvensen och resultatet av ECO-koderna i den aktuella databasen. 
</p>

<p>
Den övre delen visar frekvensen för varje ECO-kod i den aktuella databasen. 
Färgerna i grafen finns i tre sektioner: den lägsta (ljusaste färgen) visar antalet vita vinster, 
den mittersta visar antalet remier, och den högsta (mörkaste färgen) visar antalet svarta vinster. 
Detta låter dig snabbt se om en öppning är bra eller dålig för vit eller om remier är mycket vanliga. 
</p>

<p>
För att gå till en djupare ECO nivå klicka med vänster musknapp på en del av grafen
(eller skriv den bokstav eller siffra den motsvarar). För att gå tillbaka till en högre nivå, 
klicka med höger musknapp i grafen eller tryck på vänsterpilen (eller delete eller bakåt). 
</p>

<p>
Den nedre delen visar de positioner som utgör en speciell ECO-kod, 
enligt den ECO-fil som du har öppnat. 
</p>

<h3>Öppna Scids ECO-fil</h3>
<p>
ECO-filen som kommer med Scid heter scid.eco och Scid försöker att öppna den när det 
startar. Om Scid inte kan hitta den behöver du göra följande för att använda 
ECO-klassificeringen: 
</p>

<p>
(a) I Scid använd menykommandot <b>[Alternativ: Ladda ECO-fil]</b> och välj filen <b>scid.eco</b>. 
(b) Spara alternativ (i  <b>[Alternativ]</b> menyn). 
Efter att du har gjort deta kommer ECO-filen att laddas varje gång du startar Scid. 
</p>

<h3>ECO-koderna</h3>
<p>
Den grundläggande strukturen i ECO är:
</p>

A 1.d4 Nf6 2...; 1.d4 ...; 1.c4; 1.diverse
<ul>
<li>A0 1.diverse (A02-A03 1.f4: Birds Öppning, A04-A09 1.Nf3: Reti, Kungsindisk attack) </li>
<li>A1 1.c4 ...: Engelsk </li>
<li>A2 1.c4 e5: Kungsengelsk </li> 
<li>A3 1.c4 c5: Engelsk, Symmetriskt </li> 
<li>A4 1.d4 ...: Dambonde </li>
<li>A5 1.d4 Nf6 2.c4 ..: Indiskt försvar </li> 
<li>A6 1.d4 Nf6 2.c4 c5 3.d5 e6: Modern Benoni </li> 
<li>A7 A6 + 4.Nc3 exd5 5.cxd5 d6 6.e4 g6 7.Nf3 </li>
<li>A8 1.d4 f5: Holländskt försvar </li>
<li>A9 1.d4 f5 2.c4 e6: Holländskt försvar </li>
</ul>
B 1.e4 c5; 1.e4 c6; 1.e4 d6; 1.e4 diverse 
<ul>
<li>B0 1.e4 ... (B02-B05 1.e4 Nf6: Alekhines försvar; B07-B09 1.e4 d6: Pirc) </li>
<li>B1 1.e4 c6: Caro-Kann </li>
<li>B2 1.e4 c5: Sicilianskt försvar </li> 
<li>B3 1.e4 c5 2.Nf3 Nc6: Sicilianskt </li>
<li>B4 1.e4 c5 2.Nf3 e6: Sicilianskt </li>
<li>B5 1.e4 c5 2.Nf3 d6: Sicilianskt </li>
<li>B6 B5 + 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 Nc6 </li>
<li>B7 B5 + 4.Nxd4 Nf6 5.Nc3 g6: Siciliansk Drake </li>
<li>B8 B5 + 4.Nxd4 Nf6 5.Nc3 e6: Siciliansk Scheveningen </li> 
<li>B9 B5 + 4.Nxd4 Nf6 5.Nc3 a6: Siciliansk Najdorf </li>
</ul>
C 1.e4 e5; 1.e4 e6 

<ul>
<li>C0 1.e4 e6: Franskt Försvar </li>
<li>C1 1.e4 e6 2.d4 d5 3.Nc3: Franskt, Winawer/Klassisk </li> 
<li>C2 1.e4 e5: Öppet parti </li>
<li>C3 1.e4 e5 2.f4: Kungsgambit </li>
<li>C4 1.e4 e5 2.Nf3: Öppet parti </li>
<li>C5 1.e4 e5 2.Nf3 Nc6 3.Bc4: Italienskt; Tvåspringar </li>
<li>C6 1.e4 e5 2.Nf3 Nc6 3.Bb5: Spanskt (Ruy Lopez) </li>
<li>C7 1.e4 e5 2.Nf3 Nc6 3.Bb5 a6 4.Ba4: Spanskt </li>
<li>C8 C7 + 4...Nf6 5.O-O: Spanskt, Öppet och Stängt (C80-C83 5.O-O Nxe4: Spanskt, Öppet system; C84-C89 5.O-O Be7: Spanskt, Stängt system) </li>
<li>C9 C8 + 5...Be7 6.Re1 b5 7.Bb3 d6: Spanskt, Stängt </li>
</ul>
D 1.d4 d5; 1.d4 Nf6 2.c4 g6 med 3...d5 
<ul>
<li>D0 1.d4 d5: Dambonde </li>
<li>D1 1.d4 d5 2.c4 c6: Slaviskt försvar </li> 
<li>D2 1.d4 d5 2.c4 dxc4: Antagen damgambit </li>
<li>D3 1.d4 d5 2.c4 e6: Avböjd damgambit  </li>
<li>D4 D3 + 3.Nc3 Nf6 4.Nf3 c5/c6: Semi-Tarrasch; Semi-Slav </li>
<li>D5 D3 + 3.Nc3 Nf6 4.Bg5: Avböjd klassisk damgambit</li>
<li>D6 D5 + 4...Be7 5.e3 O-O 6.Nf3 Nbd7: Avböjd  ortodox damgambit </li>
<li>D7 1.d4 Nf6 2.c4 g6 with 3...d5: Grünfeld </li>
<li>D8 1.d4 Nf6 2.c4 g6 3.Nc3 d5: Grünfeld </li>
<li>D9 1.d4 Nf6 2.c4 g6 3.Nc3 d5 4.Nf3: Grünfeld </li>
</ul>
 
E 1.d4 Nf6 2.c4 e6; 1.d4 Nf6 2.c4 g6 

<ul>
<li>E0 1.d4 Nf6 2.c4 e6: Katalanskt, mm </li>
<li>E1 1.d4 Nf6 2.c4 e6 3.Nf3 (b6): Damindiskt, mm </li>
<li>E2 1.d4 Nf6 2.c4 e6 3.Nc3 (Bb4): Nimzo-Indiskt, mm </li>
<li>E3 E2 + 4.Bg5 or 4.Qc2: Nimzo-Indiskt </li>
<li>E4 E2 + 4.e3: Nimzo-Indiskt, Rubinstein </li>
<li>E5 E4 + 4...O-O 5.Nf3: Nimzo-Indiskt, huvudvariant </li> 
<li>E6 1.d4 Nf6 2.c4 g6: Kungindiskt </li>
<li>E7 1.d4 Nf6 2.c4 g6 3.Nc3 Bg7 4.e4: Kungsindiskt </li>
<li>E8 E7 + 4...d6 5.f3: Kungsindiskt, Sämisch </li>
<li>E9 E7 + 4...d6 5.Nf3: Kungsindiskt, huvudvarianter </li>
</ul>

<p><footer>(Uppdaterat: Scid 2.5, Juni 2001)</footer></p>
}

####################
### Calvar window help:

set helpTitle(W,CalVar) "Beräkning av variationer"
set helpText(W,CalVar) {<h1>Beräkning av variationer-fönstret</h1>
  <p>
Denna träningsövning är också känd som Stoyko-övningen.
Dess syfte är att analysera en komplex position och utvärdera så många sunda variationer som möjligt,
och ge en korrekt värdering för varje av dem.
  </p>
<p><h3>Konfiguration</h3></p>
Tre parametrars anges :
<ul>
<li>Den UCI-schackmotor som kommer att analysera olika variationer</li>
<li>Den tid, i sekunder, som schackmotorn kommer att använda för att analysera positionen</li>
<li>Den tid, i sekunder, som schackmotorn kommer att använda för att analysera varje variation som har angivits av användaren</li>
</ul>
<p><h3>Ange variationer</h3></p>
Drag anges som vanligt med musklick på brädet men de visas inte. Vid slutet på varje variation behöver användaren ange en värdering genom att klicka på en av knapparna med NAG-värden.
  <p><h3>Värderingsverifikation</h3></p>
Varje gång en värdering anges till en variation berknar schackmotorn dess värde och lägger till variationen och värderingen precis nedanför användarens.
  <p><h3>Klar med position</h3></p>
När användaren tror att han har hittat alla de bästa variationerna, kommer tryckandet på <b>Klar med position</b> att lägga till i partiet
(med kommentaren <b>Missad variation</b>), de variationer som har en högre värdering än de bästa variationerna som har angetts av användaren.

  <p><footer>(Uppdaterat: Scid 3.6.21, December 2007)</footer></p>
}

####################
### EPD files help:

set helpTitle(W,EPD) "EPD"
set helpText(W,EPD) {<h1>EPD-filer</h1>

<h3>EPD filer</h3>
<p>
En EPD (extended position description) fil är en samling av positioner, 
där varje position har någon tillhörande text. Det är precis som PGN är det 
en vanlig standard för schackinformation. 
</p>

<p>
En EPD-fil har ett antal definierade <b>opcodes (fält)</b> som är lagrade 
separade med semikolon <b>(;)</b> i filen men visas på olika rader 
i ett Scid EPD-fönster för att göra ändringar lättarer. Ett semikolon inom ett 
EPD-fält lagras som <b>"\s"</b> av Scid för att skilja det från en markering av slutet av fältet. 
Varje position och dess tillhörande opcodes lagras på en enkel rad i EPD-filen. 
</p>

<p>
Standard <b>EPD-opcodes</b> inkluderar: 
</p>

<p>
<li><b>acd</b> Analysräkning: sökdjup.</li> 
<li><b>acn</b> Analysräkning: antal sökta noder.</li> 
<li><b>acs</b> Analysräkning: söktid i sekunder.</li> 
<li><b>bm</b> Bästa drag: drag som anses bäst av någon anledning.</li> 
<li><b>ce</b> Hundradelsbondevärdering: värdering i hundradelar av en bonde för den sida 
som är vid draget (Observera att detta skiljer sig från Analysfönstret som visar 
värderingen i bönder från vits perspektiv).</li> 
<li><b>cX</b> Kommentar (där <b>X</b> är en siffra från 0-9).</li> 
<li><b>eco</b> ECO öppningskod.</li> 
<li><b>id</b> Unik identifikation av denna position.</li> 
<li><b>nic</b> New In Chess (NIC) öppningskod.</li> 
<li><b>pm</b> Förutspått drag: det första draget i den förutspådda variationen (PV).</li> 
<li><b>pv</b> Förutspådd variation: den bästa dragföljden.</li>
</p>

<p>
EPD-filer har ett antal användningar: Scid använder en EPD-fil för att klassificera
partier enligt Encyclopedia of Chess Openings (ECO) , 
och du kan skapa en EPD-fil för din öppningsrepetoar samt 
lägga till kommentarer för positioner du vanligtvis når i partier. 
</p>

<p>
Du kan skapa en ny EPD-fil eller öppna en existerande från  <b>[Ny]</b> och <b>[Öppna]</b> 
kommandorna i  <b>[Arkiv]</b> menyn. Som mest kan fyra EPD-filer vara öppnade. 
</p>

<h3>EPD fönster</h3>

<p>
För varje öppnad EPD-fil ser du will ett fönster som visar texten för den aktuella positionen. 
Du behöver inte trycka på <b>Spara</b> knappen för att spara de förändringar du gör med 
en positionstext; texten sparas när du går till en annan position i partiet. 
</p>

<h3>Navigera i EPD-filer</h3>

<p>
För att bläddra genom positionerna i en EPD-fil använd <b>[Nästa position]</b> och <b>[Tidigare position]</b> 
kommandorna från EPD-fönstret i <b>[Verktygs]</b> menyn eller använd tangentkombinationerna 
<b>Ctrl+NerPil</b> och <b>Ctrl+UppPil</b>. Dessa kommandon går till nästa eller föregående position i filen, 
tar bort det aktuella partiet och ställer in den nya startpositionen. 
</p>

<h3>Ta bort EPD-fält</h3>

<p>
EPD-filer du finner på Internet kan innehålla fält som inte intresserar dig och de kan 
slösa mycket utrymme i filen. Till exempel, en EPD-fil med datorvärderingar kan ha 
<b>ce</b>, <b>acd</b>, <b>acn</b>, <b>pm</b>, <b>pv</b> och <b>id</b> fält 
men du kanske bara behöver <b>ce</b> och <b>pv</b> fälten. 
</p>

<p>
Du kan ta bort en EPD-opcode från alla positioner i EPD-filen genom att använda <b>[Ta bort EPD fält]</b> 
från EPD-fönstret i <b>[Verktygs]</b> menyn. 
</p>

<h3>EPD-fönstrets resultat</h3>

<p>
Alla EPD-fönster visar: 
</p>

<ul>
<li>- filstatus (-- betyder oförändrad, <b>XX</b> betyder förändrad, och <b>%%</b> betyder skrivskyddad); </li>
<li>- filnamn; </li>
<li>- antal positioner i filen; </li>
<li>- legala drag från den aktuella positionen för att nå en annan position i denna EPD-fil.</li> 
</ul>
<p><footer>(Uppdaterat: Scid 2.5, Juni 2001)</footer></p>
}


###################
### Exporting help:

set helpTitle(W,Export) "Exportera partier"
set helpText(W,Export) {<h1>Exportera partier</h1>
<p>
Du kan använda funktionerna i <menu>Verktygs</menu>-menyn, för det
aktuella partiet eller för alla partier i det aktuella filtret för att exportera dragen
i en textfil.
</p>
<p>
Tre Textdata-format står till förfogande: <a PGN>PGN</a> (portable
game notation), HTML (för Webbsidor) och LaTeX (ett populärt
Textbearbetningssystem).
</p>
<p>
Vid export kan du välja att skapa en ny fil 
eller infoga partierna i en existierande partifil.
</p>

<h3>Diagram</h3>
<p>
När du exporterar i HTML- eller LaTeX-format , infogar Scid automatiskt
där ett diagram , där det finns en diagram-<a NAGs>NAG</a> ("D")
eller ett med tecknet "#" börjande <a Comment>Kommentar</a>
i partiet .
</p>

<h3><name Null>Null-drag vid PGN-Export</name></h3>
<p>
Scid tillåter att det sparas <a Annotating Null>Null- (inga drag)
drag</a> , då dessa kan vara hjälpsamma vid kommentarer av varianter. 
Det finns dock inga Null-drag i PGN-Standarden. 
När du då exporterar Scid-partier med Null-drag i en PGN-fil
kan andra program oftast inte läsa in partierna.
</p>
<p>
För att lösa detta problem ställer Scid en annan funktion till förfogande,
<b>Omvandla Null-drag till kommentarer</b>, när partier blir exporterade i 
PGN-format. När du vill skapa en PGN-fil, som kan användas av
andra program, använder du denna funktion som omvandlar varianter med nolldrag 
till kommentarer.
När du ändå vill skapa en PGN-fil, som senare kan importeras av Scid med bibehållande av
Null-drag stänger du av denna funktion.
</p>

<h3>HTML-Export</h3>
<p>
Scid kan exportera partier i en HTML-filer. För att infoga diagram ,
måste diagrambilderna finnas (i katalogen "<b>bitmaps/</b>" )
i en underkatalog till "<b>bitmaps/</b>" i katalogen under  HTML-filen.
</p>

<h3>LaTeX-Export</h3>
<p>
Scid kan exportera partier i en LaTeX-fil. Partier blir sparade i
två kolumner per sida och dragen blir sparade i algebraisk
notation med symboler för pjäserna.
</p>
<p>
För mer information se hjälpsidan <a LaTeX>Scid och LaTeX</a>.
</p>

<p><footer>(Uppdaterat: Scid 3.4, Juli 2002)</footer></p>
}


####################
### Flags help:

set helpTitle(W,Flags) "Parti-markeringarna"
set helpText(W,Flags) {<h1>Parti-markeringarna</h1>

<p>
En <term>Markering</term> ("Flags") är ett Kännetecken på någon
schackegenskap, som kan markeras för varje parti i databasen. 
Det finns 13 av användare inställbara
markeringar, som du kan sätta direkt i varje parti . Av dessa
har bara ta bort-markeringen en särskild betydelse: partier, som
är markerade med ta bort-markeringen, är markerade för att tas bort
och tas bort, när databasen blir
<a Compact>komprimerad</a> .
</p>
<p>
De andra 12 av användare inställbara markeringarna och deras
symboler är:
</p>

<ul>
<li>Öppningen Vit (W)</li>
<li>Öppningen Svart (B)</li>
<li>Mittspelet (M)</li>
<li>Slutspelet (E)</li>
<li>Nyhet (N)</li>
<li>Bondestruktur (P)</li>
<li>Taktik (T)</li>
<li>Spel på damflygeln (Q)</li>
<li>Spel på Kungsflygeln (K)</li>
<li>Utomordentligt (!)</li>
<li>Blunder (?)</li>
<li>Användar-definierat (U)</li>
</ul>

<p>
En markering kan sättas för det aktuella partiet, för alla partier i
filtret eller för alla partier i databasen i
<a Maintenance>Underhålls</a>-fönstret.
</p>
<p>
Du kan använda  <a Searches Header>Sökning efter Partidata</a> ,
för att finna alla datenbaspartier , som har en bestämd markering 
eller använda markeringarna som en del av en komplex sökning.
</p>
<p>
Då alla av användare inställbara markeringar inte har någon betydelse för
Scid (med undantag av ta bort-markeringar), kan du använda dessa för
dina egna syften. Till exempel kan använda markeringen "Spel på kungsflygeln" (K) 
för att leta efter bland annat bondestormar mot kungen , pjäsangrepp mot kungen 
eller slutspel med alla bönder på kungsflygeln.
</p>

<p><footer>(Uppdaterat: Scid 3.0, November 2001)</footer></p>
}


###############
### LaTeX help:

set helpTitle(W,LaTeX) "Scid och LaTeX"
set helpText(W,LaTeX) {<h1>Scid och LaTeX</h1>
<p>
Scid kan spara partier och öppningsresultat i LaTeX-format i filer. 
LaTeX är en utvidgning av TeX, ett populärt Textbehandlingssystem.
</p>
<p>
För att Scid ska kunna skapa LaTeX-filer , måste du
(naturligtvis) ha installerat LaTeX  och schackteckensnitts-paketet "chess12". 
Detta paket är normalt inte en del av
LaTeX-Standardinstallation, dvs om du har LaTeX , har
du kanske inte schackteckensnittet.
</p>
<p>
Information om nedladdning och installation av
LaTeX-schackteckensnittet hittar du på länken
<url http://scid.sourceforge.net/latex.html>Using LaTeX with Scid</url>
på <url http://scid.sourceforge.net/>Scids Webbsida</url>.
</p>

<p><footer>(Uppdaterat: Scid 2.5, Juni 2001)</footer></p>
}


###########################
### Options and Fonts help:

set helpTitle(W,Options) "Alternativ"
set helpText(W,Options) {<h1>Alternativ och preferenser</h1>
<p>
Många alternativ och preferenser i Scid (som till exempel brädstorleken, färger,
teckensnitt och standardinställningar) kan man ändra i <menu>Alternativ</menu>-menyn. 
Alla dessa (och fler inställningar, som till exempel den sista katalogen i vilket du har läst in en
databas och storleken på några fönster)
blir sparat i en inställningsfil, när du väljer 
<b>Spara inställningar</b> i Alternativ-menyn.
Varje gång som Scid startar blir denna inställningsfil laddad.
</p>
<p>
När du arbetar med Windows , finns inställningsfilen <b>scid.opt</b>
i samma katalog som Scids programfil. För användare av
Unix-Operativsystem (som Solaris eller Linux) är filen
<b>~/.scid/scidrc</b>.
</p>

<h3><name MyPlayerNames>Fastställa spelarnamn</name></h3>
<p>
Det kan vara så att du vill se brädet i huvudfönstret för en (eller flera) spelare 
från dennes eller deras perspektiv när dennes eller deras partier öppnas. 
Du kan ange en lista med sådana namn med <b>Mina Spelarnamn...
</b> i Menyn <menu>Alternativ/Schachbräde</menu>. 
I Dialogfönstret matar du då in ett spelarnamn per rad. 
Jokertecken som ("<b>?</b>" för ett okänt tecken och "<b>*</b>"
för en teckenkedja av noll eller flera tecken) är tillåtna.
</p>

<h3><name Fonts>Ställa in teckensnitt</name></h3>
<p>
Scid har tre teckensnitt som används i de flesta fönster
och du kan anpassa alla tre individuellt. De heter
<b>normal</b>, <b>liten</b> och <b>fast</b>.
</p>
<p>
Det fasta teckensnittet borde vara ett teckensnitt med fast bredd
(inte-proportionellt) . Det används av <a Tree>Dragträdet</a> och 
<a Crosstable>Korstabeller</a> .
</p>

<p><footer>(Uppdaterat: Scid 3.5, Februar 2003)</footer></p>
}


####################
### Pgnscid help:

set helpTitle(W,Pgnscid) "Pgnscid"
set helpText(W,Pgnscid) {<h1>Pgnscid</h1>
<p>
<term>Pgnscid</term> är ett separat program, som du kan
använda för att konvertera PGN-filer ("portable game notation") 
till Scid-Databaser.
</p>
<p>
För att konvertera en fil med namnet <i>partier.pgn</i> , skriver du
bara
<ul>
<li> <b>pgnscid partier.pgn</b> </li>
</ul>
och Scid-Databasen (bestående av filerna
<i>partier.si3</i>, <i>partier.sg3</i> och <i>partier.sn3</i>) blir
skapade.
Felmeddelanden eller varningar blir skrivna i filen <i>partier.err</i>
.
</p>
<p>
När du vill att databasen ska skapas i en annan katalog eller
med ett annat namn, kan du ange datenbasnamnen
i kommandoraden, till exempel skapar
<ul>
<li> <b>pgnscid partier.pgn databas</b> </li>
</ul>
en databas bestående av filerna <i>databas.si3</i>,
<i>databas.sg3</i> och <i>datenbas.sn3</i>.
</p>
<p>
Observera att pgnscid (och också Scid) direkt kan läsa gzip-komprimerade
PGN-filer som till exempel. <b>datenbas.pgn.gz</b>.
</p>

<h3>Alternativ</h3>
<p>
Pgnscid accepterar två optionella argument för filnamnen:
<b>-f</b> och <b>-x</b>.
</p>
<p>
Alternativet <b>-f</b> tvingar fram överskrivandet av en existerande
databas; normalt ändrar pgnscid inte i en redan
existerande databas.
</p>
<p>
Alternativet <b>-x</b> gör att pgnscid ignorerar text mellan partierna. 
Normalt blir text mellan partierna sparat som en kommentar före det
följande partiet. Detta alternativ gäller bara text mellan partier; 
standardkommentarer i partierna blir dessutom konverterade och sparade.
</p>

<h3>Formatera spelarnamn</h3>
<p>
För att minska antalet olika stavningar av samma spelare
genomför pgnscid några Grundformateringar
. Till exempel blir antalet mellanrum efter ett
komma satt till ett medan mellanrum i början eller slutet av ett namn
blir bortagna.
Holländska Namn som "van den" och "Van Der" blir dessutom
normaliserade, så att de har ett stort "V" och litet "d".
</p>
<p>
Du kan även ändra Spelar-, Turnerings-, Orts- och Rundebeteckningar i Scid
(och till och med utföra automatisk skrivkorrektur);
för detaljerna se hjälpsidan <a Maintenance Editing>Underhåll</a>.
</p>

<p><footer>(Uppdaterat: Scid 2.5, Juni 2001)</footer></p>
}





#################
### Sorting help:

set helpTitle(W,Sorting) "Sortera databas"
set helpText(W,Sorting) {<h1>Sortera databas</h1>
<p>
<term>Sortera</term>-Funktionerna sorterar alla partier i en
databas. Du kan välja flera sorteringskriterier .
När två partier är likvärdiga efter ett kriterium, blir
de sorterade efter det andra kriteriet och så vidare.
</p>

<h3>Sorteringskriterier</h3>
<p>
De tillgängliga sorteringskriterierna är:
</p>
<ul>
<li> Datum (äldsta partier först)
<li> År (som Datum, men bara året används)
<li> Turnering (Tilldragelse)
<li> Ort
<li> Land (de sista 3 bokstäverna i orten)
<li> Runda
<li> Vit
<li> Elo-tal (Genomsnitt av Vit och Svart, högst först)
<li> Svart
<li> Resultat (Vit vinner, därefter remier, därefter Svart vinner)
<li> Längd (Antal hela drag i partiet)
<li> ECO-kod enligt <a ECO>Encyclopedia of Chess Openings</a>)
</ul>

<h3>Sorteringsresultatet</h3>
<p>
När du inte sorterar en skrivskyddad Scid-Databas , blir 
sorteringsresultatet säkrat genom att långvarigt ändra i ordningsföljden 
i databaspartierna. Om du bara vill göra en temporär sortering ,
skyddar du först databasen mot överskrivande med med menyfunktionen
<menu>Fil: Skrivskydd</menu>.
</p>
<p>
När du sorterar en databas , som är skrivskyddad eller består av
en PGN-fil , kan sorteringsresultatet inte bli säkrat , vilket gör att 
den sorterade partiordningsföljden går förlorad när filen
stängs.
</p>
<p>
Observera att sorteringen av en databas i
<a Searches filtret>Sökfilter</a> ändrar i alla partier.
</p>

<h3>Viktig hjälp för databas-sortering:</h3>
<p>
När en Databas blir sorterad , är indexfilen ändrad, men
partifilen är oförändrad. Det vill säga, sorteringen av en databas
lämnar partidata-filen i en tillfällig ordningsföljd relativ till 
indexfilen. Detta kan göra <a Tree>Dragträd</a>-,
Positions- och Material/Mönster-<a Searches>Sökningar</a> märkbart
<b>långsammare</b>, du bör därför efter att ha sorterat databasen
ändra i partifilen genom att <a Compact>Komprimera</a> ,
för att få bra sökprestanda .
</p>

<p><footer>(Uppdaterat: Scid 2.5, Juni 2001)</footer></p>
}





####################
### NAG values help:

set helpTitle(W,NAGs) "NAG-värden"
set helpText(W,NAGs) {<h1>Standard-NAG-värden</h1>
<p>
Standard-NAG-värden (Numeric Annotation Glyph = "numeriska
Anmärkningglyfer"), som definieras i <a Author Related>PGN-Standard</a> ,
är:
</p>
<cyan>
<ul>
<li>  1   Bra drag (!) </li>
<li>  2   Dåligt drag (?) </li>
<li>  3   Utomordentligt drag (!!) </li>
<li>  4   Stort misstag / Blunder (??) </li>
<li>  5   Intressant drag (!?) </li>
<li>  6   Tveksamt drag (?!) </li>
<li>  7   Tvingat drag </li>
<li>  8   Det enda draget; det finns inga andra bra drag </li>
<li>  9   Sämsta draget </li>
<li> 10   Utjämnad position (=) </li>
<li> 11   Lika chanser, lugn ställning (=) </li>
<li> 12   Lika chanser, aktiv ställning (=) </li>
<li> 13   Oklar ställning (~) </li>
<li> 14   Vit har en liten fördel, står något bättre (+=) </li>
<li> 15   Svart har en liten fördel, står något bättre (=+) </li>
<li> 16   Vit har fördel, står bättre (+/-) </li>
<li> 17   Svart har fördel, står bättre (-/+) </li>
<li> 18   Vit har en avgörande fördel (+-) </li>
<li> 19   Svart har en avgörande fördel (-+) </li>
<li> 20   Vit har en överväldigande fördel (+-) </li>
<li> 21   Svart har en överväldigande fördel (-+) </li>
<li> 22   Vit är i dragtvång </li>
<li> 23   Svart är i dragtvång </li>
<li> 24   Vit har en liten utrymmesfördel </li>
<li> 25   Svart har en liten utrymmesfördel </li>
<li> 26   Vit har en utrymmesfördel </li>
<li> 27   Svart har en utrymmesfördel </li>
<li> 28   Vit har en avgörande utrymmesfördel </li>
<li> 29   Svart har en avgörande utrymmesfördel </li>
<li> 30   Vit har ett litet Tids-(Utvecklings)försprång </li>
<li> 31   Svart har ett litet Tids-(Utvecklings)försprång </li>
<li> 32   Vit har ett Tids-(Utvecklings)försprång </li>
<li> 33   Svart har ett Tids-(Utvecklings)försprång </li>
<li> 34   Vit har ett avgörande Tids-(Utvecklings)försprång </li>
<li> 35   Svart har ett avgörande Tids-(Utvecklings)försprång </li>
<li> 36   Vit har initiativet </li>
<li> 37   Svart har initiativet </li>
<li> 38   Vit har ett uthålligt initiativ </li>
<li> 39   Svart har ett uthålligt initiativ </li>
<li> 40   Vit har ett angrepp </li>
<li> 41   Svart har ett angrepp </li>
<li> 42   Vit har ingen tillräcklig kompensation för materialet </li>
<li> 43   Svart har ingen tillräcklig kompensation för materialet </li>
<li> 44   Vit har tillräcklig kompensation för materialet </li>
<li> 45   Svart har tillräcklig kompensation för materialet </li>
<li> 46   Vit har mer än tillräcklig kompensation för materialet </li>
<li> 47   Svart har mer än tillräcklig kompensation för materialet </li>
<li> 48   Vit har en lätt övervikt i centrum </li>
<li> 49   Svart har en lätt övervikt i centrum </li>
<li> 50   Vit har en övervikt i centrum </li>
<li> 51   Svart har en övervikt i centrum </li>
<li> 52   Vit har en avgörande övervikt i centrum </li>
<li> 53   Svart har en avgörande övervikt i centrum </li>
<li> 54   Vit har en lätt övervikt på kungsflygeln </li>
<li> 55   Svart har en lätt övervikt på kungsflygeln </li>
<li> 56   Vit har en övervikt på kungsflygeln </li>
<li> 57   Svart har en övervikt på kungsflygeln </li>
<li> 58   Vit har en avgörande övervikt på kungsflygeln </li>
<li> 59   Svart har en avgörande övervikt på kungsflygeln </li>
<li> 60   Vit har en lätt övervikt på damflygeln </li>
<li> 61   Svart har en lätt övervikt på damflygeln </li>
<li> 62   Vit har en övervikt på damflygeln </li>
<li> 63   Svart har en övervikt på damflygeln </li>
<li> 64   Vit har en avgörande övervikt på damflygeln </li>
<li> 65   Svart har en avgörande övervikt på damflygeln </li>
<li> 66   Vit har en svag ställning </li>
<li> 67   Svart har en svag ställning </li>
<li> 68   Vit har en bra säkrad ställning </li>
<li> 69   Svart har en bra säkrad ställning </li>
<li> 70   Vit har en dåligt säkrad kung </li>
<li> 71   Svart har en dåligt säkrad kung </li>
<li> 72   Vit har en bra säkrad kung </li>
<li> 73   Svart har en bra säkrad kung </li>
<li> 74   Den vita kungen står dåligt </li>
<li> 75   Den svarta kungen står dåligt </li>
<li> 76   Den vita kungen står bra </li>
<li> 77   Den svarta kungen står bra </li>
<li> 78   Vit har en mycket svag bondestruktur </li>
<li> 79   Svart har en mycket svag bondestruktur </li>
<li> 80   Vit har en svag bondestruktur </li>
<li> 81   Svart har en svag bondestruktur </li>
<li> 82   Vit har en stark bondestruktur </li>
<li> 83   Svart har en stark bondestruktur </li>
<li> 84   Vit har en mycket stark bondestruktur </li>
<li> 85   Svart har en mycket stark bondestruktur </li>
<li> 86   Den vita springaren står dåligt </li>
<li> 87   Den svarta springaren står dåligt </li>
<li> 88   Den vita springaren står bra </li>
<li> 89   Den svarta springaren står bra </li>
<li> 90   Den vita löparen står dåligt </li>
<li> 91   Den svarta löparen står dåligt </li>
<li> 92   Den vita löparen står bra </li>
<li> 93   Den svarta löparen står bra </li>
<li> 94   Det vita tornet står dåligt </li>
<li> 95   Det svarta tornet står dåligt </li>
<li> 96   Det vita tornet står bra </li>
<li> 97   Det svarta tornet står bra </li>
<li> 98   Den vita damen står dåligt </li>
<li> 99   Den svarta damen står dåligt </li>
<li>100   Den vita damen står bra </li>
<li>101   Den svarta damen står bra </li>
<li>102   De vita pjäserna är dåligt koordinerade </li>
<li>103   De svarta pjäserna är dåligt koordinerade </li>
<li>104   De vita pjäserna är bra koordinerade </li>
<li>105   De svarta pjäserna är bra koordinerade </li>
<li>106   Vit har behandlat öppningen mycket dåligt  </li>
<li>107   Svart har nehandlat öppningen mycket dåligt </li>
<li>108   Vit har behandlat öppningen dåligt </li>
<li>109   Svart har behandlat öppningen dåligt </li>
<li>110   Vit har behandlat öppningen bra  </li>
<li>111   Svart har behandlat öppningen bra  </li>
<li>112   Vit har behandlat öppningen mycket bra  </li>
<li>113   Svart har behandlat öppningen mycket bra  </li>
<li>114   Vit har behandlat mittspelet mycket dåligt </li>
<li>115   Svart har behandlat mittspelet mycket dåligt  </li>
<li>116   Vit har behandlat mittspelet dåligt  </li>
<li>117   Svart har behandlat mittspelet dåligt </li>
<li>118   Vit har behandlat mittspelet bra  </li>
<li>119   Svart har behandlat mittspelet bra  </li>
<li>120   Vit har behandlat mittspelet mycket bra  </li>
<li>121   Svart har behandlat mittspelet mycket bra  </li>
<li>122   Vit har behandlat slutspelet mycket dåligt  </li>
<li>123   Svart har behandlat slutspelet mycket dåligt  </li>
<li>124   Vit har behandlat slutspelet dåligt  </li>
<li>125   Svart har behandlat slutspelet dåligt  </li>
<li>126   Vit har behandlat slutspelet bra  </li>
<li>127   Svart har behandlat slutspelet bra  </li>
<li>128   Vit har behandlat slutspelet mycket bra  </li>
<li>129   Svart har behandlat slutspelet mycket bra  </li>
<li>130   Vit har lite motspel </li>
<li>131   Svart har lite motspel </li>
<li>132   Vit har motspel </li>
<li>133   Svart har motspel </li>
<li>134   Vit har avgörande motspel </li>
<li>135   Svart har avgörande motspel </li>
<li>136   Vit är i lätt tidsnöd </li>
<li>137   Svart är i lätt tidsnöd </li>
<li>138   Vit är i allvarlig tidsnöd </li>
<li>139   Svart är i allvarlig tidsnöd </li>
</ul>
</cyan>

<p>
Fler föreslagna Nag-värden för Schachinformator-symboler är bland annat:
</p>
<cyan>
<ul>
<li>140   Med iden ... </li>
<li>141   Riktat mot ... </li>
<li>142   Bättre är </li>
<li>143   Sämre är </li>
<li>144   Likvärdigt är </li>
<li>145   Redaktörens anmärkning ("RR") </li>
<li>146   Nyhet ("N") </li>
<li>147   Svag punkt </li>
<li>148   Slutspelet </li>
<li>149   Linje </li>
<li>150   Diagonal </li>
<li>151   Vit har löparparet </li>
<li>152   Svart har löparparet </li>
<li>153   Olikfärgade löpare </li>
<li>154   Likfärgade löpare </li>
</ul>
</cyan>

<p>
Andra förslag är:
</p>
<cyan>
<ul>
<li>190   Etc. </li>
<li>191   Dubbelbönder </li>
<li>192   Isolerade bönder </li>
<li>193   Förenade bönder </li>
<li>194   Hängande bönder </li>
<li>195   Bakåtvänd bonde </li>
</ul>
</cyan>

<p>
Av Scid definierade symboler för internt bruk:
</p>
<cyan>
<ul>
<li>201   Diagram ("D", ibland också "#") </li>
</ul>
</cyan>

<p><footer>(Uppdaterat: Scid 2.5, Juni 2001)</footer></p>
}


######################
### File formats help:

set helpTitle(W,Formats) "Filformat"
set helpText(W,Formats) {<h1>Scids Filformat</h1>
<p>
Scid-Databaser består av tre viktiga filer: en
Indexfil, en Namnfil och en Partidatafil. 
Alla filer har en trebokstavsändelse, 
som börjar med "s": 
<b>".si3"</b> för Indexfiler,
<b>".sn3"</b> för Namnfiler och 
<b>".sg3"</b> för Partidatafiler.
</p>

<h3>Indexfilen (.si3)</h3>
<p>
Denna fil innehåller en beskrivning av databasen där det finns en 
liten notering med fast längd för varje parti. Den aktuella storleken 
är 41 bytes per parti. Av dessa är 28 bytes väsentlig information 
som Resultat, Datum, Index för Spelar-, Turnerings- och
Ortsnamn (de aktuella namnen finns i  namnfilen) .
</p>
<p>
De resterande 13 bytes innehåller reduktant, men nyttig
information om partiet, som används för  Positions-,
Material- och Mönstersökningar. 
Se avsnittet <a Formats Fast>Söka snabbt</a> för mer
information.
</p>

<h3>Namnfilen (.sn3)</h3>
<p>
Denna fil innehåller alla Spelar-, Turnerings-, Orts- och Rundenamn, som
används i databasen. Varje Namn är bara sparat en gång,
även om det förekommer i många partier. Namnfilen är vanligtvis
den minsta av de tre viktiga databasfilerna.
</p>

<h3>Partidatafilen (.sg3)</h3>
<p>
Denna fil innehåller de aktuella dragen, varianter och kommentarer för varje
parti. Dragkodningen är mycket kompakt: de flesta dragen behöver bara en 
byte.
</p>
<p>
När ett parti blir ersatt, blir dess nya version sparat i <i>slutet</i>
av filen, så att det inte över tiden ansamlar sig icke använt sparutrymme. 
Du kan genom <a Compact>Komprimering</a>
återställa den minimala storleken på en databas.
</p>

<h3>Andra Scid-Filer</h3>
<p>
En <a EPD>EPD</a>-fil (Filändelse: ".epd") innehåller en rad 
schackpositioner, varje med en textkommentar.
EDP-filformatet beskrivs i <a Author Related>PGN-Standard</a>.
</p>
<p>
En E-Mail-fil (Filändelse: <b>".sem"</b>) sparas i en databas
av motståndaren, som ni skickar E-Mail till.
</p>
<p>
En Sökalternativfil (Filändelse: <b>".sso"</b>) innehåller inställningar för
en <a Searches Header>Partidata</a>- eller
<a Searches Material>Material/Mönster</a>-sökning.
</p>

<h3><name Fast>Söka snabbt i Scid</name></h3>
<p>
Som ovan nämns, innehåller Indexfilen några reduktanta, men
nyttiga informationer om varje parti, som används av Positions- eller
Materialsökning.
</p>
<p>
Till exempel sparas materialet i slutställningen. Om du
vill söka efter Torn- och Bondeslutspel, blir alla partier, som har en
Dam, en Löpare eller en Springare på brädet i slutställningen,
snabbt genomsökta.
</p>
<p>
En annan nyttig delinformation som sparas är ordningsföljden,
för Bönderna att lämna sin utgångsställning (genom drag eller
genom att de blir tagna). Detta blir använt för sökningar av bondedrag 
eller exakta sökningar, i synnerhet efter öppningspositioner. Till exempel
blir vid sökningen av utgångsställningen för den franska öppningen
(1.e4 e6) varje Parti, som börjar med 1.e4 c5 eller 1.d4 etc.
genomsökt, Partier, som börjar med 1.e4 e5 , måste även alltid
genomsökas.
</p>

<p><footer>(Uppdaterat: Scid 2.5, Juni 2001)</footer></p>
}



################
### Contact info

set helpTitle(W,Author) "Kontaktinformation"
set helpText(W,Author) {<h1>Kontaktinformation</h1>
<p>
Scids webbsajt finns på: <br>
<b><url http://scid.sourceforge.net/>http://scid.sourceforge.net/</url></b>
</p>
<p>
Där kan du hämta den senaste versionen av Scid och de andra filerna till Scid, som du kan ha behov av.
</p>
<p>Den svenska översättningen av Scid har gjorts av Martin Skjöldebrand. Synpunkter på översättningen ska <b>inte</b>
skickas till Shane. Däremot får du gärna skicka kommentarer, frågor och förslag till rättningar till översättaren på adressen:<br>
<b>martin@skjoldebrand.org</b>
</p>
<h3><name Related>Länkar</name></h3>
<p>
Om Ni har glädje av Scid kan de följande
Internetsidorna vara interessanta:
</p>
<ul>
<li><url http://www.tim-mann.org/chess.html>http://www.tim_mann.org/chess.html</url> --
Hemsida för Tim Mann's <b>xboard & winboard</b> program: grafiska
gränssnitt för schackprogram och anslutning till Internet Chess
Server. Den har också bra information om Crafty, GNUchess och
andra schackprogram.</li>

<li><url ftp://ftp.cis.uab.edu/pub/hyatt/>ftp://ftp.cis.uab.edu/pub/hyatt/</url> --
Det starka schackprogrammet Crafty.  <b>TB</b> -
underkatalogerna  innehåller nästan alla Nalimov-slutspelstabeller, som
används av några schackprogram och även används av Scid.</li>

<li><url http://www.chesscenter.com/twic/twic.html>http://www.chesscenter.com/twic/twic.html</url> --
TWIC (the week in chess), är ett framstående veckomagazin för
internationella schackpartier, tillgängliga i PGN-format. </li>

<li><url http://scid.sourceforge.net/doc/standard.txt>http://scid.sourceforge.net/doc/standard.txt</url> --
 <b>PGN-Standard</b>, skapad av Steven J. Edwards 1994.
Textfilen förklarar PGN- och EPD-Formaten i detalj.</li>
</ul>

<p><footer>(Uppdaterat: Scid 2.5, Juni 2001)</footer></p>
}

# Book window help
set helpTitle(W,Book) "Bokfönstret"
set helpText(W,Book) {<h1>Bokfönstret</h1>
  <p>
  Bokfönstret visar en lista på alla bokfiler som finns i Scids bokkatalog. Formatet på böckerna är
  det som används av Polyglot och schackmotorer som Fruit och Toga.
  </p>
  <p>
  För varje position, visar bokfönstret alla möjliga drag det innehåller och deras
  relativa vikter i procent. När ett drag görs, uppdateras bokfönstret.
  Om det klickas på ett drag i bokfönstret görs detta drag i den aktuella positionen.
  </p>
  <p>
  Den aktuella boken som används kan snabbt ändras genom att välja en annan bok i den övre listan.
  </p>
  <p><footer>(Uppdaterat: Scid 3.6.5, Februari 2007)</footer></p>
}

# Tactical game window help
set helpTitle(W,TacticalGame) "Taktiskt parti-fönstret"
set helpText(W,TacticalGame) {<h1>Taktiskt parti-fönstret</h1>
  <p>
  Spela ett taktiskt parti mot en schackmotor som simulerar en motståndare med en angiven ELO-rating.
  Motståndaren spelas av Phalanx och tränaren spelas av Crafty. De rätta schackmotorerna väljs automatiskt
  genom sina namn, och bör vara korrekt konfigurerade (detta är standard när Scid installeras).
  </p>
  <p>
  De följande parametrarna måste konfigureras :
  <ul>
  <li>svårighet : ELO-rating på motståndare</li>
  <li>specifik öppning : motståndaren spelar en specifik öppning (användbart för öppningsträning)</li>
  <li>spelarens fel kontrolleras av en schackmotor som agerar som en tränare. Tiden som används för att bedöma om ett
  drag är bra eller inte kan begränsas</li>
  </ul>
  <p>När du är klar klicka bara på spela</p> 
  <p><footer>(Uppdaterat: Scid 3.6.5, Mars 2007)</footer></p>
}

# Opening Trainer window help
set helpTitle(W,OpeningTrainer) "Öppningstränare"
set helpText(W,OpeningTrainer) {<h1>Öppningstränare</h1>
  <p>
  En Sciddatabas kan användas som en repetoar, där varje parti representerar en öppningsvariant.
  </p>
  
  <h3>Öppningstränarstatistik</h3>
  <p>
  Fyra tal visas för att visa statistik för den aktuella positionen :
  <ul>
  <li>Grön : antalet drag som spelades av användaren som är bra drag</li>
  <li>Gul : antalet tveksamma drag som spelades som är repertoaren</li>
  <li>Röd : antalet drag som spelades som inte finns i repertoaren</li>
  <li>Vit : antalet gånger positionen påträffades</li>
  </ul>
  </p>
  
  <h3>Rapporten</h3>
  <p>
  Rapporten visar statistik för hela repertoaren och ger information om användarens kunskaper :
  <ul>
  <li>Positioner i repertoaren : det totala antalet positioner i repertoaren (alla varianter i databasen)</li>
  <li>Positioner som inte spelats : de positioner som alldrig har påträffats</li>
  <li>Positioner spelade : de positioner som spelades och summan av antalet gånger de har påträffats. Notera att
  detta antal representerar antalet förekomster av en position i alla öppningsvarianter : detta antal kan snabbt bli stort
  för positioner som är nära till startpositionen</li>
  <li>Bra drag : antalet bra drag gjorda av användaren</li>
  <li>Tveksamma drag : antalet svaga drag gjorda av användaren</li>
  <li>Utanför repertoaren : antalet drag som användaren har gjort som inte finns i repertoaren</li>
  </ul>
  </p> 
  <p><footer>(Uppdaterat: Scid 3.6.11, April 2007)</footer></p>
}

####################
### Correspondence Chess help:
set helpTitle(W,Correspondence) "Korrespondensschack"
set helpText(W,Correspondence) {<h1>Korrespondensschack</h1>

<p>
Scid erbjuder nu två huvudfunktioner för korrespondens-
schack.
</p>
<ul>
   <li><b>eMail-schack</b> gör detta genom att skicka det aktuella partiet via eMail
   till din motståndare när du har gjort ditt drag. För detta skapas ett eMail-
   meddelande i ditt föredragna email-program med det aktuella partiet
   bifogat till det i PGN-format. Givetvis tas alla kommentarer och
   variationer bort först.
   </li>
   <li><b>Korrespondensschackservrar</b> stöds med hjälp av
   Xfcc-protokollet. Här används externa verktyg för att hämta partierna från
   ditt/dina konto/konton och levererar dem till Scid för synkronisering. Efter
   du har gjort ditt drag skickas detta till din motståndare genom
   Xfcc. Hämtnings- och sändningsverktygen är implementerade som externa verktyg för
   enkel utökning om andra protokoll kommer.
   </li>
</ul>

<p>
För att använda korrespondensschackfuntioner måste en databas av typ
"Korrespondensschack" öppnas innan några
korrespondensschackfunktioner används. OM du inte har en sådan databas
än, skapa bara en ny databas och ange dess typ till "Korrespondensschack"
med funktionen <a Maintenance>Underhåll</a>.
Det är viktigt att ange typen så att Scid kommer att känna igen databasen för
synkronisering genom denna typ. Då denna databas är tom efter
skapandet kommer Scid att behandla alla korrespondensschackpartier det får vid
den första synkroniseringen som nya partier och lägga till dem till denna databas.
</p>

<p>
Om ingen databas av typ "Korrespondensschack" är nu öppnad
kommer Scid att påminna dig om att göra så. Men, öppna inte mer än en
databas av denna typ då Scid inte då kan avgöra vilken som ska användas.
</p>

<h3>Grundfunktionalitet</h3>

<p>
Om allting är korrekt konfigurerat kan användningen av korrespondens-
schackfunktionerna lätt nås med följande knappar:
<ul>
   <li><button tb_CC_Retrieve> Hämtar korrespondensschackpartier.
   För detta startas ett externt hämtningsverktyg och alla partier hämtas
   så likväl som andra partier som lagras i Scids Inkorg
   (se nedan) är synkroniserade till den aktuella korrespondensschack-
   databasen.
   </li>
   <li><button tb_CC_Send> Skickar ditt drag till motståndaren genom att antingen
   skapa ett nytt eMail-meddelande i dett föredragna email-program eller genom att
   skicka ett drag till en schackserver om Xfcc används.
   </li>
</ul>
</p>

<h3>Konfiguration</h3>

<p>
Korrespondensschack inom Scid är baserat på en normal Scid-
databas som har partierna och några externa verktyg till Scid som
hanterar de "icke-schack-delarna". Dessa verktyg och deras parametrar måste ställas
in en gång, och lagras sedan för framtida användning.
</p>

<p>
För att nå konfigurationsdialogen välj <menu>Spela</menu>
<menu>Korrespondensschack</menu><menu>Konfigurera</menu> från
menyn. Detaljerna beskrivs i <a CCSetupDialog>Korrespondensschack-
inställningar</a>. När denna dialog avslutas med <b>[Ok]</b> knappen kommer
dina inställningar automatiskt att lagras.
</p>

<h3>Hämta partierna</h3>
<p>
Beroende på om du spelar korrespondensschack via eMail eller via en
schackserver skiljer sig den aktuella hämtningsprocessen lite. Generellt
resulterar den i ett antal partier i PGN-format placerade i Scids Inkorgs-
katalog. Detta erbjuder också möjligheten till automatisk hämtning via
extern programvara.
</p>
<p>
När partierna finns i Scids Inkorg kommer <menu>Behandla Inkorg</menu>
från menyn att bearbeta Inkorgen och lägga till nya drag till
de partier som redan finns i databasen. Dessutom kommer den att lägga till de partier
som inte hittas i den aktuella korrespondensschackdatabasen som nya partier.
</p>

<p><a CCeMailChess>Korrespondensschack via eMail</a> beskriver
detaljerna för användning av eMail, medan <a CCXfcc>Korrespondens-
schack via schackserverar</a> beskriver motsvarande för korrespondens-
schackservrar.
</p>

<h3>Stega igenom partier</h3>
<p>
Efter att partierna är hämtade laddas de i Scids klippboardsdatabas
och nya drag läggs till och lagras i den öppnade korrespondensschack-
databasen. Skillnades i
funktionen från menyn <menu>Partier</menu> är, att dessa två
knappar scrollar bara mellan partierna i Scids Inkorg som är
tänkt att vara dina aktuella pågående partier. Givetvis kan
Korrespondensschackdatabasen innehålla många fler partier, men
normalt vill du inte gå igenom  dessa för att se vad din
motståndare har gjort för drag i ett aktuellt parti.
</p>
<p>
Notera att en taggsökning krävs med några fält som
inte är indexerade av Scid. Därför är det , inte att rekommendera
att lagra partier i en stor referensdatabas då
söktiderna kan bli ganska långa. Om du spelar mycket och din egen
databas blir ganska stor, kan söktiden reduceras genom att flytta
avslutade partier till en arkivdatabas, eller genom att bara skapa en ny
databas för de pågående partierna. Scid kommer att behandla alla partier som inte finns
redan i korrespondensschackdatabasen som nya partier och lägga till dem
automatiskt. Därför, är det tillräckligt att öppna en tom databas av
typ "Korrespondensschack" och använda <menu>Behandla Inkorg</menu> för att
importera alla nu pågående partier.
</p>
<p>
Motsvarande till de två nämnda knapparna är <menu>Tidigare
Parti</menu> och <menu>Nästa Parti</menu> från <menu>Korrepondens-
schack</menu> menyn.
</p>
<p>
Notera att om du anger dina spelarnamn korrekt (i menyn
<menu>Mina Spelarnamn</menu>) kommer Scid att rotera brädet för dig att
alltid spela uppåt. Du kan ha flera spelarnamn. Se också <a
Options MyPlayerNames>Mina Spelarnamn</a> för detaljer.
</p>

<h3>Analysera och gör ett drag</h3>
<p>
Alla analysfunktioner kan användas för korrespondensschackpartier.
Variationer, kommentarer etc. kan läggas till precis som i en normal partianalys.
När det är klart, kommer Scid att ta den senaste halvdraget som lades till
partiet och behandla det som det aktuella draget att skicka. Ingen kontroll görs
här av om bara ett halvdrag lades till eller vilken sida som är vid draget,
därför, måste bara ett halvdrag i huvudvarianten läggas till!  Om en
schackserver används skickar Scid också den senaste kommentaren som lades till
servern vilket kan vara användbart för kommunikationen med motståndaren. I
eMail-schack kan detta göras med det normala email-meddelandet, så där tas alla
kommmentarer bort.
</p>
<p>
Genom att trycka på Sändknappen <button tb_CC_Send> kommer Scid att måsta
bestämma den typ av korrespondensschackparti som visas (eMail eller
ett serverparti) och starta antingen ditt eMail-program eller det externa skicka-
verktyget för att skicka ditt drag. Att använda <menu>Skicka drag</menu> är motsvarande
till denna knapp. Alternativt, kan <menu>eMail drag</menu> användas för att
skicka det aktuella draget via eMail. Om det är ett eMail-parti är denna
funktion motsvarande till <menu>Skicka drag</menu>. Om det är ett server-
baserat parti genereras ett eMail-meddelande. Notera dock, att detta kommer
inte nödvändigtvis att innehålla en riktig mottagare då eMail-addresser inte byts ut
i serverbaserad korrespondensschack.
</p>

<p><footer>(Uppdaterat: Scid 3.6.23, Februari 2008)</footer></p>
}


set helpTitle(W,CCXfcc) "Korrespondensschackservrar"
set helpText(W,CCXfcc) {<h1>Korrespondensschackservrar</h1>

<p>
Det finns flera korrespondensschackserverar på
Internet. Generellt, är de använda med en webbläsare, så ingen
specifik programvara krävs. Men många av dem erbjuder också ett
gränssnitt till specialiserad schackprogramvara via ett protokoll kallat Xfcc.
Integrationen av Xfcc görs i Scid via externa hjälpverktyg angivna
i <a CCSetupDialog>Konfiguration</a>-dialogen för korrespondens-
schack.
</p>

<h3>Starta ett nytt parti</h3>
<p>
Xfcc tillåter inte själv att ett nytt parti startas. Att söka efter en
motståndare och starta ett parti är istället hanterat av schackservern
på dess hemsida. När partiet är startat kan dock, Scid användas för att hämta
motståndarens drag, lägga till dem till den interna databasen,
analysera osv. Alla funktioner i Scid är till användarnas förfogande
men speciella moder av spel kanske inte tillåter dem (t.ex.  normala partier
tillåter vanligtvis inte användande av schackmotorer för analys).
</p>

<h3>Hämta partier</h3>
<p>
Öppna en databas som inehåller korrespondensschackpartier. Denna databas
måste vara av typ "Korrespondensschack". 
</p>

Notera: 
<ul>
   <li>Om Scid inte hittar en korrespondensschackdatabas kommer den att
   informera dig att öppna en.
   </li>
   <li>Om databasen inte har partierna som är hämtade från
   servern behandlas de som nya partier och läggs till databasen
   automatiskt.
   </li>
   <li>Scid kommer att använda den första databasen av typ "Korrespondensschack"
   som är öppnad. Av denna anledning bör bara en sådan databas i taget
   vara öppnad.
   </li>
</ul>
<p>
Xfcc hämtar alltid omedelbart alla partier som finns på en specificerad server för ditt
användarnamn. För att hämta partierna klicka bara på <button tb_CC_Retrieve>
ikonen eller välj <menu>Hämta Partier</menu> från
<menu>Korrespondensschack</menu>-menyn. Då en serveranslutning
krävs för att hämta nya partier var säker på att systemet har tillgång till nätverket.
Scid kommer att starta hämtningsverktyget som är konfigurerat i 
<a CCSetupDialog>Konfiguration</a>-dialogen vilket kommer att placera
partierna i PGN-format i Scids inkorg. Det kan ta lite tid att hämta
svaret, så vat tålmodig. Efter att partierna är hämtade uppdateras
korrespondensschackdatabasen med dem.
</p>
<p>
När partierna är hämtade söks deras motståndare i
korrespondensschackdatabasen och nya drag läggs till där. Då Xfcc
servrar kan erbjuda olika sätt att infoga drag (via Internet eller mobil eller
andra program...) kan det vara så att Scid måste lägga till hälften av
partiet till databasen. Detta innebär inget problem. Scid kommer att lägga till alla drag
som fanns i partiet från servern. Scid kommer dock inte att ersätta
partiet från början då etta kan ta bort all din analys.
Därför är det <b>viktigt att notera</b> att du inte får infoga drag till
huvudvarianten bortom ditt sista drag! För att lägga till fortsättningar var god
och använd variationer!
</p>
<p>
Notera att om andra källor har placerat partier i din inkorg (t.ex. från
din eMail-korrespondensschack) synkroniseras de också i
hämtningen till databasen då hela Inkorgen också bearbetas.
Detta tillåter att eMail-partier läggs till i Inkorgen, sedan kan du växla till Scid,
klicka på <button tb_CC_Retrieve> och alla partier är uppdaterade.Partier som inte
ännu finns i datasen behandlas som nya partier och läggs till
databasen.
</p>
<p>
<b>Notering för programmerare</b>: hämtningsverktyget startas med Inkorgssökvägen som
parameter. Det är tänkt atrt fungera genom alla serverkonton och placera
korrekt formatterade PGN-filer i den sökväg som anges till den. Dessa filer
bör innehålla ytteliggare fält som är kända av cmail-verktyget.
 (Se <a CCeMailChess>Korrespondensschack via eMail</a> för
information om de krävda fälten.)
</p>

<p><footer>(Uppdaterat: Scid 3.6.23, Februari 2008)</footer></p>
}

#############
# eMail Chess:
set helpTitle(W,CCeMailChess) "Korrespondensschack via eMail"
set helpText(W,CCeMailChess) {<h1>Korrespondensschack via eMail</h1>

<p>
eMail erbjuder ett väldigt bekvämt sätt att spela korrespondensschack.
Standardapplikationen för detta i Un*x-värden är till idag xboard
tillsammans med dess cmail-hjälpare. Då den tillåter nästan automatisk
hantering av korrespondensschack-eMail och därutöver inte lägger till någonting
som inte är PGN är det också modellen för Scid att hantera
eMail-schack. Genom att bara bevara hela PGN-huvudet kan sådana partier
spelas med en motståndare som har ett verktyg för att hantera PGN.
</p>

<h3>eMail-schack</h3>
<p>
Scid kan hantera eMail-korrespondensschackpartier nästan automatiskt.
Det sätt på hur detta görs hålls kompatibelt med cmail-verktyget som
kommer med xboard på Un*x system. (Att ha sagt detta antyder att du
kan spela mot en motståndare med cmail/xboard.) Det fungerar genom att skicka
partierna som hela PGN-filer till och från som email-bilagor, medan
huvudet innehåller vissa huvudfält som låter dem kännas igen och
sorteras tillsammans. Av denna anledning måste användaren vara noggrann med
att ändra i huvudfälten.  Notera att fälten med explicita värden måste
anges till exakt detta värde för eMail-schack. Att starta ett parti med
Scid gör det automatiskt, men du <b>får inte</b> skriva över
eller ta bort dem!
</p>
<p>
Nödvändiga huvudfält är:
</p>
<ul>
   <li><term>Event</term>: måste vara "Email correspondence game"
   </li>
   <li><term>Site</term>: måste vara "NET"
   </li>
   <li><term>Mode</term>: måste vara "EM"
   </li>
   <li><term>WhiteNA</term>: innehåller eMail-addressen för den vita spelaren. Notera
   att adressen bara lagras där i formen
   <term>user@host.org</term>.
   </li>
   <li><term>BlackNA</term>: innehåller eMail-addressen för den svarta spelaren
   liknande WhiteNA.
   </li>
   <li><term>CmailGameName</term>: Innehåller en <b>unik</b> identifierare för
   partiet. Detta används för att sortera partierna.
   <p>
   Medan Scid kan använda något databasindex är detta inte möjligt för
   icke-DB-baserade verktyg som cmail. Av denna anledning är
   <term>CmailGameName</term> parametern användarinmatad. Den måste vara
   unik! Det enklaste är någonting avformen
   <term>xx-yy-yyyymmdd</term> där xx är en beteckning på den vita
   spelaren, yy en beteckning för den svarta spelaren, och yyyymmdd är det aktuella datumet.
   </p>
   <p>För Xfcc-baserade partier måste detta fält anges till en unik
   identifierare men där kan servernamnet och det unika partinumret på
   denna server användas, dvs denna identifierare är av formen
   <term>MyXfccServer-12345</term>.
   </p>
   </li>
</ul>

<h3>Starta ett nytt parti</h3>
<p>
Detta öpppnar en dialog för inmatning av ditt eget och motståndarens namn som
de bör vara i huvudet likväl som eMail-addresserna för
båda spelarna. Bessutom måste ett <b>unikt</b> game ID infogas.
Det enklaste sättet för detta Game ID är någonting av formen
<term>xx-yy-yyyymmdd</term> där xx är en förkortning för den vita
spelaren, yy för den svarta spelaren, och yyyymmdd är det aktuella datumet.
Denna Game id är en text och det är viktigt för att identifiera partierna unikt.
Användare av cmail kommer också att känna denna Game ID som <i>partinamn</i>. Det får
brar innehålla bokstäveroch sifror, minustecknet och understrykning.
Var god och undvik andra tecken.
</p>
<p>
Efter att dialogen är avslutad genom att trycka på <b>[Ok]</b> knappen läggs ett
nytt parti till den aktuella öppnade korrespondensschackdatabasen
och PGN-huvudet fylls i ordentligt. Gör bara ditt drag och skicka det som
det beskrivs nedan.
</p>

<h3>Hämta partier</h3>

<p>
Scid hanterar inte din emailkorg automatiskt. Detta skulle,
med tanke på den stora variationen på möjliga email-program idag, involvera
en stor mängd programkod. Av denna anledning använder Scid ditt normala
eMail-program som är mycket mer lämpligt för detta syfte än Scid
kan någonsin bli. För att hämta ett parti till Scid spara bara den bifogade PGN-filen
i Scids inkorg och bearbeta inkorgen genom antingen <menu>Hämta
Partier</menu> eller  <button tb_CC_Retrieve> knappen eller
<menu>Bearbeta Inkorgen</menu>. Skillnaden mellan de två är att
den första också startar det externa hämtningsverktyget och fyller
Inkorgen ytterliggare med partier från andra källor (som Xfcc). Därför är
<button tb_CC_Retrieve> det mest bekväma sättet om du använder båda
typerna av korrespondensschackpartier.
</p>
<p>
<b>Notera</b> att <menu>Hämta Partier</menu> menyn eller  <button
tb_CC_Retrieve> knappen <b>inte</b> hämtar dina eMail-meddelanden! Du
måste spara dina PGN-filer i Scids Inkorg för hand. Troligtvis kan detta
bli automatiserat av ditt eMail-program (på Un*x system är det enkelt att sätta upp en
mime-hanterare genom <term>.mailcap</term>).
</p>

<h3>Skicka svaret</h3>

<p>
Efter att du har gjort ditt drag skicka det antingen med <menu>eMail drag</menu>
funktionen från menyn via <menu>Skicka drag</menu> vilket är motsvarande till
<button tb_CC_Send>. För det senare  måste Scid känna igen partiet
som eMail-korrespondens och skicka det med email medan den föregående metoden
kommer att få Scid att generera ett eMail-meddelande.
</p>
<p>
Givetvis tar Scid bort alla kommentarer och variationer
innan det bifogas i det utgående eMailet då du inte vill skicka med
din analys.
</p>
<p>
Om ett GUI-emailprogram används, öppnas dess vanliga skapafönster.
Addressen på din motståndare fylls i likväl som ett generiskt ämne
som innehåller game id för enkel filtrering och bcc-addresses om den
är specificerad i <a CCSetupDialog>Konfigurations</a>-dialogen. Email-
texten är angiven till att innehålla FEN på den sista positionen och listan på
de tidigare gjorda dragen. På detta sätt kan motståndaren snabbt se ditt
drag. SlutligenFinally, bifogar Scid det aktuelkla partiet inklusive ditt drag i
PGN-format till email-meddelandet.
</p>
<p>
När ett mailx kompatibelt verktyg används öppnas inget fönster och emailet skickas
osynligt geonom att starta verktyget specificerat i bakgrunden. I
detta fall innehåller det genererade emailet PGN även i email-texten.
</p>
<p>
Notera att då eMail-schack fungerar genom att skicka hela PGN-filen får du inte
lägga till mer än ditt drag. Scid kontrolerar inte här om
mer än ett drag lades till huvudvarianten, då Scid helt enkelt inte
vet vilket drag det är, då du skickar ditt drag.
</p>
<p><footer>(Uppdaterat: Scid 3.6.23, Februari 2008)</footer></p>
}

#############
# Correspondence Chess setup dialog help:
set helpTitle(W,CCSetupDialog) "Korrespondensschackinställningar"
set helpText(W,CCSetupDialog) {<h1>Korrespondensschackinställningar</h1>

<p>
Indata i konfigurationsdialogen finns fördefinierat till vissa standard-
parametrar men dessa måste inte nödvändigtvis matcha ditt system, så
anpassning kommer säkert att vara nödvändigt. Alla parametrar lagras i
konfigurationsfilen automatiskt när dialogen är stängd via
<b>[Ok]</b> knappen. Parametrarna gäller då omedelbart.
</p>

<p>
<b>Standarddatabas</b>:
Detta definierar standarddatabasen för
korrespondensschackpartier. Den kan enkelt nås via
<menu>Öppna Databas</menu> i  <menu>Korrespondensschack</menu>
menyn. Fördelen med denna metod är, att databasen inte
visas i <i>senaste databaser</i> listor och att du kan ha en
specifik fil för detta. Denna databas <b>måste vara</b> av typ
"Korrespondensschack". Att öppna en databas av denna typ på ett annat
sätt är också ok, så troligtvis vill du ignorera denna inställning
(t.ex. om du startar Scid med din korrespondensschackdatabas vid
uppstart.)
</p>

<p>
<b>Inkorg (sökväg)</b>:
I denna katalog söker Scid efter korrespondensschackpartier sparade
i PGN-format. Dessa partier används för synkroniseringen av
korrespondensschackdatabasen. Generellt, bryr sig inte Scid om hur
partierna kommer till denna katalog. Scid kommer bara att arbeta sig igenom alla PGN-filer
som finns där. Detta ger möjlighet att använda några externa verktyg
för att hämta partier till denna plats. Därutöver, i eMail-schack bör man
bara spara PGN-filerna från motståndaren i denna
katalog.
</p>
<p>
Scid kommer inte att läsa en mailkorg, utan hanterar bara
alla PGN-filer som är placerade i denna katalog. Notera också, att Scid kommer att
synkronisera partie med den aktuella databasen. Men, om ett parti
från denna katalog inte ännu finns i databasen kommer det att
behandlas som ett nytt parti och läggas till databasen.
</p>
<p>
För att synkroniseringsprocessen ska fungera måste PGN-filerna innehålla
någon ytterliggare huvudinformation som är i perfekt enlighet med
PGN-standarden. Var god och konsulera <a
CCeMailChess>)Korrespondensschack via eMail</a> om du vill skapa
ditt eget verktyg eller om du flyttar data från något annat system.
</p>

<p>
<b>Utkorg (sökväg)</b>:
Motsatsen till <i>Inkorgen</i>. Scid placerar här PGN-filer för de
utgånde partierna. För eMail-schack är detta nödvändigt då PGN-filerna måste
bifogas till ett eMail-meddelande.  För Xfcc, där bara draget skickas
, är detta inte nödvändigt, men Utkorgskatalogen erbjuder
ett bekvämt sätt att koppla upp din PDA eller någon annan användning då
PGN-filerna i Utkorgen också kommer att innehålla ditt senaste drag.
</p>

<p>
<b>Hämtningsverktyg</b>:
Detta program startas för att hämta korrespondensschack-
partier från en korrespondensschackserver. Denna hjälpare måste bara
hämta partier från den källa den tycker om, generera en korrekt PGN-
fil med de nödvändiga PGN-taggarna. Verktyg för att hämta partier
från Xfcc-servrar finns som externa program och dessa är de naturliga
verktygen att ställa in här. För framtida protokoll kan man enkelt generera
ett externt hämtningsverktyg som hanterar detta protokoll. Även automatisering
är möjlig om denna funktionalitet är gjord externt.
</p>
<p>
Notera: Detta verktug är <b>inte</b> startat för hämtning av eMailschack-
meddelanden!
</p>

<p>
<b>Skickaverktyg</b>: 
Detta är motsatsen till hämtningsverktygen, primärt även avsett för Xfcc
support eller någon annat framtida protokoll som kan dyka upp. Skickaverktyget,
dock, startas från Scid med flera parametrar som ser ut som:
<term>
SendTool Outbox Name GameID MoveCount Move "Comment" resign claimDraw offerDraw acceptDraw
</term>
</p>

<p>
Betydelsen av parametrarna är som följer:
   <ul>
      <li><term>Outbox</term>: Utkorgssökvägen anges i denna dialog. Skicka-
      verktyget ska generera en korrekt formatterad PGN-fil
      där.
      </li>
      <li><term>Name</term>: Namnet på spelaren vid draget som står
      i PGN-fältet. För Xfcc är detta loginnamnet. Det är
      identiskt som spelarnamnet i PGN-huvudet.
      </li>
      <li><term>MoveCount</term>: Dragnummer att skicka.
      </li>
      <li><term>Move</term>: Det aktuella draget i SAN.
      </li>
      <li><term>"Comment"</term>: En kommentar skickad till motståndaren. Scid
      infogar den sista kommentaren i partiet. Dvs dessa kommentarer är
      behandlade som kommentarer till motståndaren. Notera att kommentaren är
      citerad, så flerradiga kommentarer bör vara möjligt.
      </li>
      <li><term>resign</term>: 0 eller 1, specificerar om användaren
      vill ge upp. Ange till 1 om användaren använder
      <menu>Ge upp</menu> från <menu>Korrespondensschack</menu>
      menyn.
      </li>
      <li><term>claimDraw</term>: 0 eller 1, specificerar om användaren
      vill ha kräver remi. Ange till 1 om användaren använder
      <menu>Kräv remi</menu> från <menu>Korrespondensschack</menu>
      menyn.
      </li>
      <li><term>offerDraw</term>: 0 eller 1, specificerar om användaren
      vill erbjuda remi. Ange till 1 om användaren använder <menu>Erbjud
      remi</menu> från <menu>Korrespondensschack</menu> menyn.
      </li>
      <li><term>acceptDraw</term>: 0 eller 1, specificerar om användaren
      vill acceptera en remi som erbjuds av motståndaren. Ange till 1 om
      användaren använder <menu>Acceptera remi</menu> från
      <menu>Korrespondensschack</menu>
      menyn.
      </li>
   </ul>
</p>
<p>
Notera: Denna funktion är <b>not</b> startat för eMail-schack!
</p>

<p>
<b>eMail-program</b>:
Detta anger sökvägen till ditt föredragna eMail-program. Detta program startas
för eMail-schack för att skriva ett meddelande till motståndaren.
</p>

<p>
<b>(B)CC-Address</b>:
En kopia på det utgåene meddelandet skickas till denna address som en kopia.
Notera dock, att om ett GUI-emailprogram används har det normalt sin egen
utgående email-hantering. Därfär, kan angivande av denna address duplicera
meddelanden. Det kan dock användas för att skicka ett parti till en annan address.
</p>

<p>
<b>Mod</b>:
Olyckligtvis finns det ett stort antal email-klienter och de använder
väldigt olika startkonventioner. Några vanliga konventioner, och
exempel på program som använder dem, listas här. Emailprogrammet
kommer att startas med den valda konventionen. Om det inte är känt
vilken konvention som används kan en av alternativen kanske matcha och
fungera. Notera dock att ett stort antal emailprogram är inte
kapabla att skicka bilagor när de startas av ett annat program. I
detta fall måste du antingen byta din email-klient eller lägga till de bilagor
som har placerats i Scids Utkorg manuellt.
</p>
<p>Tips: mailx eller en av dess många kloner bör vara tillgängliga som en
kommandoradsapplikation på nästan alla system som ett enkelt att ställa in
verktyg. Om ingen av dessa konventioner fungerar med din föredragna
klient eller om denna klient inte kan hantera email med bilagor via startande från
kommandoraden, kan installation av mailx vara ett alternativ.
</p>
<p>Tips: mutt använder systemets emailtransport (dvs
sendmail/exim/postfix). Att ställa in dem är (troligtvis) inte enkelt
så mutt är ett perfekt alternativ. På en ordentlig Un*x med riktiga
inställningar bör det vara det mest enkla sättet att hantera eMail-schack.
(Men det finns inte många korrekt inställda system , speciellt inte i
Linuxvärlden.)
</p>
<p>
<b>Bilageparameter</b>: 
Denna parameter används för att specificera en bilaga. Detta används <b>bara</b>
i <term>mailx</term> mod.
</p>
<p>
<b>Ämnesparameter</b>:
Denna parameter används för att specificera ämnet för email-meddelandet. Det
används <b>bara</b> i <term>mailx</term> mod.
</p>
<p><footer>(Uppdaterat: Scid 3.6.23, Februari 2008)</footer></p>
}
############################################################
#
# Swedish tip of the day:

set tips(W) {
  {
    Scid har över 30 <a Index>Hjälpsidor</a>, och i de flesta fönster kan du
    genom att trycka <b>F1</b> få hjälp om det fönster du för tillfället har
    aktivt.
  }
  {
    Vissa fönster (t ex Partiinformation och <a Switcher>Databasväljaren</a>) 
    har en meny för höger musknapp. Prova att högerklicka i varje fönster så
    ser du vilka som har det och vilka funktioner du kommer åt den vägen.
  }
  {
    Du kan ange drag på mer än ett sätt, så du kan välja vilket som passar dig
    bäst. Du kan använda musen (med eller utan dragförslag) eller tangentbordet
    (med eller utan autokomplettering). Läs hjälpsidan för 
    <a Moves>Att ange drag</a> för mer information.
  }
  {
    Om du har databaser du ofta öppnar så kan du lägga till ett 
    <a Bookmarks>Bokmärke</a> för varje databas. Du kommer då att kunna öppna 
    databaserna snabbare via bokmärkesmenyn.
  }
  {
    Du kan se alla drag i det aktuella partiet (inklusive varianter och kommentarer)
    genom att öppna <a PGN>PGN-fönstret</a>.
    Du navigerar igenom partiet i PGN-fönstret genom att klicka på valfritt drag med
    vänster musknapp. Genom att klicka på draget med mellan- eller högermusknapp så
    får du en förhandsvisning av just den ställningen.
  }
  {
    Du kan kopiera partier mellan databaserna i <a Switcher>Databasväljaren</a> genom
    att dra-och-släppa dem mellan respektive databas.
  }
  {
    Scid kan öppna PGN filer, även de som är komprimerade med Gzip (dvs har en .gz filändelse). 
    PGN filer öppnas med enbart läsrättigheter, så om du vill redigera en PGN-fil i Scid 
    måste du kopiera filen till en Scid-databas med hjälp av <a Switcher>Databasväljaren</a>.
  }
  {
    Om du ofta använder <a Tree>Trädfönstret</a> med stora databaser, är det värt att välja
    <b>Fyll cachefil</b> från Arkivmenyn i Trädfönstret. Detta innebär att trädstatistik för 
    många av de vanligare ställningarna sparas, vilket ger en snabbare trädåtkomst för databasen
    ifråga.
  }
  {
    <a Tree>Trädfönstret</a> kan visa alla drag från den aktuella ställningen. Men om också vill 
    se alla dragföljder som har lett till denna ställning kan du få fram det genom att skapa en
   <a OpReport>Öppningsrapport</a>.
  }
  {
    Du kan vänster- eller högerklicka i en kolumn i <a GameList>Partilistan</a> för att ändra 
    dess bredd.
  }
  {
    Med <a PInfo>Spelarinformationen</a> aktuell (klicka på endera spelarens namn under brädet i 
    huvudfönstret för att få fram den) är det enkelt att <a Searches filtret>Filtrera</a> fram partier
    av en särskild spelares enskilda resultat. Klicka bara på valfritt värde som har angivits med
    <red>röd text</red>.
  }
  {
    När du studerar öppningar kan det vara en poäng att markera valen <b>Bönder</b> eller <b>Filer</b> i 
    <a Searches Board>Sök aktuell position</a>. Genom dessa val kan du hitta andra öppningar som ger
    liknande bondestrukturer.
  }
  {
    Du kan högerklicka i partiinformationsdelen av huvudfönstret (under brädet) för att få fram en 
    kontextmeny. Du kan t ex göra så att Scid döljer nästa drag i ett parti, vilket kan vara användbart 
    om du vill träna genom att "gissa" nästa drag.
  }
  {
    Om du ofta <a Maintenance>Underhåller</a> stora databaser kan du utföra flera underhållsuppgifter 
    vid samma tillfälle via <a Maintenance Cleaner>Databasverktyget</a>.
  }
  {
    Om du har en stor databas där de flesta partierna har ett evenemangsdatum och du vill ha partierna i 
    datumordning bör du överväga att <a Sorting>Sortera</a> den på evenemangsdatum i första hand och 
    evenemang i andra hand, istället för datum och evenemang, då detta kommer att hålla turneringspartierna
    från olika datum samman. (Under förutsättning att alla partier har samma evenemangsdatum naturligtvis).
    Alternativt kan du se till att fälten evenemang, rond och datum är så enhetliga och korrekta som möjligt.
    (ms).
  }
  {
    Det kan vara en bra idé att <a Maintenance Spellcheck>Stavningskontrollera</a> din databas innan du 
    <a Maintenance Twins>Tar bort dubbletter</a> då Scid har större möjlighet att hitta dubbletterna och 
    markera dessa för borttagning.
  }
  {
    <a Flags>Flaggor</a> är användbara för att markera upp partier med karaktäristika du vill söka på senare,
    såsom bondeställning, taktiska finesser, osv. Du kan söka på flaggor när du söker på fält i partihuvudet.
  }
  {
    Om du går igenom ett parti, och helt enkelt vill testa en ny variant utan att förändra partiet i sig, kan 
    du slå på Försöksläget (Trial mode) genom att trycka <b>Ctrl+Mellanslag</b> eller från verktygsraden. Återgå till
    ursprungspartiet när du är klar.
  }
  {
    Om du vill hitta det mest betydelsefulla partiet (med högst rankade spelare) i en viss position kan du
    öppna <a Tree>Trädfönstret</a> och i denna öppna listan med de bästa partierna. I trädfönstret kan du till
    och med begränsa partierna till endast ett särskilt resultat.
  }
  {
    Ett bra sätt att studera en öppning är att i en stor databas slå på träningsläget i <a Tree>Trädfönstret</a>, 
    och sedan spela igenom databasen för att se vilka varianter som förekommer oftast.
  }
  {
    Om du har två databaser öppna, och vill ha ett variantträd att studera medan du går igenom ett parti
    i den andra databasen kan du <b>Låsa</b> trädet i den databasen och sedan byta till den andra.
  }
  {
    <a Tmt>Turneringssökaren</a> är inte bara användbar för att lokalisera en särskild 
    turnering. Du kan också använda den för att söka efter turneringar en specifik spelare nyligen deltagit 
    i, eller att bläddra genom turneringar som genomförts i ett visst land.
  }
  {
    Det finns ett antal vanliga ställningstyper definierade i <a Searches Material>Sök Material/Ställning</a>
    fönstret som du kan ha nytta av när du studerar öppningar och mittspel.
  }
  {
    När du söker på <a Searches Material>Material eller Ställning</a> kan det ofta vara fördelaktigt att begränsa
    sökningen till sådana partier där ställningen eller materialet förekommit i åtminstone några drag. Du slipper
    du få med träffar där situationen du söker uppkom helt tillfälligt.
  }
  {
    Om du har en viktig databas du inte vill radera av misstag kan du välja  <b>Enbart läsbar</b> från <b>Arkiv</b>
    menyn efter att du har öppnat den. Alternativt kan du sätta dess filrättigheter till enbart läsrättigheter.
  }
  {
    Om du använder XBoard eller WinBoard (eller något annat program som kan hantera FEN-notation via Urklippshanteraren)
    och vill kopiera den aktuella ställningen från ditt program är det snabbaste sättet att göra det så här:
    Välj <b>Kopiera position</b> från <b>Arkiv</b>-menyn i Xboard/Winboard, välj sedan <b>Klistra i utgångsställning</b>
    i Redigera menyn i Scid.
  }
  {
    I <a Searches Header>Sök i huvud</a>, är spelare-, evenemang-, plats-, och rondnamn okänsliga för VERSALER och
    gemener och ger träffar varhelst de finns i ett namn. Om du vill kan du ange att du istället vill att sökningen <b>ska</b>
    ta VERSALER/gemener i beaktande. Genom att använda jokertecken inom citationstecken (där "?" = motsvarar obestämt
    enskilt tecken och "*" = noll eller flera tecken). Om du exempelvis anger "*BEL" (med citationstecken) i det platsfältet
    hittar du alla partier spelade i Belgien, men exkluderar de som spelats i Belgrad.
  }
  {
    Om du vill redigera ett drag i ett parti utan att förlora alla de drag som spelats efter detta kan du öppna
    <a Import>Importera ett parti i PGN-format</a> fönstret i Verktygsmenyn. Klicka där på <b>Klistra i aktuellt parti</b> 
    , redigera partiet och avsluta med <b>Importera</b>.
  }
  {
    Om du har en ECO klassificeringsfil laddad, kan du nå den mest exakt klassificerade ställningen för det aktuella partiet
    genom att välja <b>Identifiera öppning</b> i <b>Partier</b> menyn (kortkommando: <b>Ctrl+Shift+D</b>).
  }
  {
    När du vill se hur stor en fil är, eller vill se när den senast redigerades innan du öppnar den kan du använda
    <a Finder>Sök filer</a> (Arkiv - Sök filer).
  }
  {
    En <a repetoire>Repetoarfil</a> är ett utmärkt sätt att hålla koll på dina favoritöppningar, eller hitta partier där
    där de har spelats. När du väl har skapat en repetoarfil kan du genomsöka nya filer utifrån repetoarfilen, och titta 
    igenom alla partier med just dina öppningar.
  }
  {
    Genom att skapa en <a OpReport>Öppningsrapport</a> har du en utmärkt möjlighet att lära dig en ny spelöppning. Du kan få
    information om resultat, hur remiaktig den är, vilka vanliga positionella teman som dyker upp, och mycket mer.
  }
  {
    Du kan kommentera den aktuella ställningen med de vanligaste symbolerna (!, !?, +=, etc) utan att behöva använda 
    <a Comment>Kommentarseditorn<a>. Där du exempelvis vill ange ett bra drag skriver du "!" och trycker sedan ENTER
    så läggs "!" symbolen till draget. Se även hjälpsidan <a Moves>Ange drag</a> för mer detaljerad information.
  }
  {
    Om du bläddrar igenom öppningarna i en databas i <a Tree>Trädfönstret</a>, får du en användbar överblick över hur
    väl öppningen fungerar i sentida partier mellan högratade spelare om du öppnar statistikfönstret (kortkommando: Ctrl+I).
  }
  {
    Du ändrar enkelt huvudfönstrets brädstorlek genom att hålla nere <b>Ctrl</b> och <b>Shift</b> tangenterna, samtidigt
    som du trycker höger- eller vänster piltangent.
  }
  {
    Efter genomförd <a Searches>Sökning</a>, är det enkelt att navigera genom urvalet genom att hålla nere <b>Ctrl</b> 
    tangenten samtidigt som du trycker upp- eller nerpiltangenterna för att gå till föregående eller nästa parti i 
    <a Searches filtret>Urvalet</a>.
  }
{
  Du kan relativt enkelt rita pilar och färga rutor till dina kommentarer. Öppna <b>Kommentarseditorn</b>, klicka på <b>Infoga symbol</b> och välj önskad färg. Om du nu klickar på en första ruta, och därefter klickar på en andra så dras en pil i önskad färg från den första till den andra rutan. Klickar du bara på en ruta, blir den färgad.
  }
}
#}
