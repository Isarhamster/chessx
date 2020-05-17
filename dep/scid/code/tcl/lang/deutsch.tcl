### deutsch.tcl:
#  German language support for Scid.
#  Contributors: Bernhard Bialas, Jürgen Clos et al.
#  Untranslated messages are marked with a "***" comment.
#
#  Hinweis (aus tcl/lang/english.tcl):
#
# (4) [...]
#
#     A menu command has the format:
#         menuText L tag "Name..." underline {HelpMessage...}
#
#     [...]
#
#     where "L" is the language letter, "tag" is the name of the menu entry
#     or the button widget name, and "underline" is the index of the letter
#     to underline, counting from zero as the first letter. Two menu entries
#     that appear in the same menu should have a different underlined letter.

proc setLanguage_D {} {

# File menu:
menuText D File "Datei" 0
menuText D FileNew "Neu..." 0 {Neue Scid-Datenbank anlegen}
menuText D FileOpen "Öffnen..." 0 {Existierende Scid-Datenbank öffnen}
menuText D FileClose "Schließen" 0 {Aktive Scid-Datenbank schließen}
menuText D FileFinder "Dateisuche" 0 {Dateisuchfenster öffnen}
menuText D FileBookmarks "Lesezeichen" 0 {Lesezeichenmenü}
menuText D FileBookmarksAdd "Lesezeichen zur Liste hinzufügen" 0 \
  {Lesezeichen für die aktuelle Partie und Datenbank}
menuText D FileBookmarksFile "Lesezeichen hinzufügen in Verzeichnis" 26 \
  {Lesezeichen für die aktuelle Stellung und Partie zum Verzeichnis hinzufügen}
menuText D FileBookmarksEdit "Lesezeichen editieren..." 12 \
  {Lesezeichen bearbeiten}
menuText D FileBookmarksList "Listenansicht" 1 \
  {Lesezeichen als Liste zeigen, ohne Unterverzeichnisse}
menuText D FileBookmarksSub "Verzeichnisansicht" 3 \
  {Lesezeichen in Verzeichnissen zeigen, nicht als Gesamtliste}
menuText D FileMaint "Wartung" 0 {Das Scid-Datenbankwartungsfenster}
menuText D FileMaintWin "Wartungfenster" 0 \
  {Datenbank-Wartungsfenster öffnen/schließen}
menuText D FileMaintCompact "Datenbank komprimieren..." 10 \
  {Datenbank komprimieren, gelöschte Spiele und unbenutzte Namen entfernen}
menuText D FileMaintClass "Partien ECO-klassifizieren..." 8 \
{Neuklassifizierung aller Partien nach dem ECO-Code}
menuText D FileMaintSort "Sortieren..." 0 \
  {Alle Partien in der aktuellen Datenbank sortieren}
menuText D FileMaintDelete "Dubletten löschen..." 0 \
  {Dubletten finden und Löschkennzeichen setzen}
menuText D FileMaintTwin "Dubletten prüfen" 10 \
  {Dublettenfenster öffnen/erneuern}
menuText D FileMaintName "Namen" 0 \
  {Namenseditor und Rechtschreibprüfung}
menuText D FileMaintNameEditor "Namenseditor" 0 \
  {Namenseditorfenster öffnen/schließen}
menuText D FileMaintNamePlayer "Schreibkorrektur Spieler..." 17 \
  {Schreibkorrektur der Spielernamen mit Hilfe der .ssp-Datei}
menuText D FileMaintNameEvent "Schreibkorrektur Ereignis..." 17 \
  {Schreibkorrektur der Ereignisse mit Hilfe der .ssp-Datei}
menuText D FileMaintNameSite "Schreibkorrektur Ort..." 17 \
  {Schreibkorrektur der Orte mit Hilfe der .ssp-Datei}
menuText D FileMaintNameRound "Schreibkorrektur Runde..." 17 \
  {Schreibkorrektur der Runden mit Hilfe der .ssp-Datei}
menuText D FileReadOnly "Schreibschutz..." 3 \
  {Datenbank als schreibgeschützt behandeln, Änderungen verhindern}
menuText D FileSwitch "Datenbank wechseln" 0 \
  {Zu einer anderen geöffneten Datenbank umschalten}
menuText D FileExit "Ende" 0 {Scid beenden}
menuText D FileMaintFixBase "Inkonsistenzen beseitigen" 0 {Versuche eine inkonsistente Datenbank zu reparieren}

# Edit menu:
menuText D Edit "Bearbeiten" 0
menuText D EditAdd "Variante hinzufügen" 0 \
  {Füge zu diesem Zug eine Variante hinzu}
menuText D EditDelete "Variante löschen" 9 \
  {Lösche eine Variante zu diesen Zug}
menuText D EditFirst "Als erste Variante setzen" 4 \
  {Variante an erste Stelle in der Liste setzten}
menuText D EditMain "Variante als Partiefortsetzung" 13 \
  {Variante zur Partiefolge machen (und umgekehrt)}
menuText D EditTrial "Variante testen" 9 \
  {Testmodus starten/stoppen, um eine Idee am Brett auszuprobieren}
menuText D EditStrip "Entfernen" 1 \
   {Kommentare oder Varianten aus der Partie entfernen}
menuText D EditUndo "Rückgängig" 0 {Macht die letzte Änderung rückgängig}
menuText D EditRedo "Wiederherstellen" 0 {Redo last game change}
menuText D EditStripComments "Kommentare" 0 \
  {Alle Kommentare und Kommentarzeichen aus dieser Partie entfernen}
menuText D EditStripVars "Varianten" 0 \
  {Alle Varianten aus der Partie entfernen}
menuText D EditStripBegin "Züge ab Anfang" 8 \
  {Entferne Züge ab Partieanfang}
menuText D EditStripEnd "Züge bis Ende" 9 \
  {Entferne Züge bis Partieende}
menuText D EditReset "Ablage leeren" 0 \
   {Inhalt der Ablage-Datenbank löschen}
menuText D EditCopy "Partie in Ablage kopieren" 17 \
  {Diese Partie in die Ablage-Datenbank kopieren}
menuText D EditPaste "Partie aus Ablage einfügen" 19 \
  {Die aktuele Partie aus der Ablagedatenbank hier einfügen}
menuText D EditPastePGN "PGN-Partie aus Ablage einfügen..." 1 \
  {Interpretiere den Inhalt der Zwischenablage als PGN-Notation und füge ihn hier ein}
menuText D EditSetup "Stellungseingabe..." 0 \
  {Neue Stellung eingeben (FEN oder manuell)}
menuText D EditCopyBoard "Stellung kopieren" 10 \
  {Aktuelle Brettposition in die Zwischenablage kopieren (im FEN-Format)}
menuText D EditPasteBoard "Stellung einfügen" 12 \
  {Stellung aus der Zwischenablage (im FEN-Format) einfügen}
menuText D ConfigureScid "Einstellungen..." 0 {Alle Optionen für SCID einstellen}

# Game menu:
menuText D Game "Partie" 0
menuText D GameNew "Neue Partie" 5 {Neue Partie beginnen}
menuText D GameFirst "Erste Partie laden" 0 {Erste Partie im Filter laden}
menuText D GamePrev "Vorherige Partie laden" 0 \
  {Vorherige Partie im Filter laden}
menuText D GameReload "Partie wiederladen" 7 \
  {Diese Partie erneut laden, dabei alle Änderungen verwerfen}
menuText D GameNext "Nächste Partie laden" 0 {Nächste Partie im Filter laden}
menuText D GameLast "Letzte Partie laden" 0 {Letzte Partie im Filter laden}
menuText D GameRandom "Zufällige Partie laden" 1 \
  {Zufällig ausgewählte Partie im Filter laden}
menuText D GameNumber "Lade Partie Nummer..." 14 \
  {Partie durch Angabe der Nummer laden}
menuText D GameReplace "Partie ersetzen..." 8 \
  {Diese Partie sichern, dabei alte Version überschreiben}
menuText D GameAdd "Partie speichern..." 7 \
  {Diese Partie als neue Partie in der Datenbank sichern}
menuText D GameDeepest "Eröffnung identifizieren" 10 \
  {Zur Position der längstmöglichen Zugfolge nach ECO-Klassifikation gehen}
menuText D GameGotoMove "Zugnummer..." 0 \
  {Zur angegebenen Zugnummer in der aktuellen Partie gehen}
menuText D GameNovelty "Finde Neuerung..." 0 \
  {Ersten Zug dieser Partie finden, der vorher noch nie gespielt wurde}

# Search menu:
menuText D Search "Suchen" 0
menuText D SearchReset "Filter zurücksetzen" 0 \
  {Alle Partien in den Filter einschließen}
menuText D SearchNegate "Filter invertieren" 7 \
  {Alle ausgeschlossenen Partien in den Filter nehmen}
menuText D SearchCurrent "Brett..." 0 \
  {Aktuelle Brettposition suchen}
menuText D SearchHeader "Partiedaten..." 0 \
  {Partiedaten (Spieler, Turnier etc.) suchen}
menuText D SearchMaterial "Material/Muster..." 0 \
  {Nach Material- oder Stellungsmustern suchen}
menuText D SearchUsing "Mit Suchoptionsdatei..." 4 \
  {Mit Suchoptionsdatei suchen}

# Windows menu:
menuText D Windows "Fenster" 0
menuText D WindowsComment "Kommentareditor" 0 {Kommentareditor öffnen/schließen}
menuText D WindowsGList "Partieliste" 6 {Partieliste öffnen/schließen}
menuText D WindowsPGN "PGN-Fenster" 0 {PGN-Fenster (Partienotation) öffnen/schließen}
menuText D WindowsPList "Spielersuche" 0 {Spielersuche öffnen/schließen}
menuText D WindowsTmt "Turniersuche" 0 {Turniersuche öffnen/schließen}
menuText D WindowsSwitcher "Datenbank-Umschalter" 0 \
  {Datenbank-Umschalter öffnen/schließen}
menuText D WindowsMaint "Wartungsfenster" 0 {(Datenbank-)Wartungsfenster öffnen/schließen}
menuText D WindowsECO "ECO-Auswertung" 0 {ECO-Auswertung öffnen/schließen}
menuText D WindowsStats "Statistik" 4 {Filterstatistik öffnen/schließen}
menuText D WindowsTree "Zugbaum" 0 {Zugbaum öffnen/schließen}
menuText D WindowsTB "Endspieltabellen..." 1 {Endspieltabellen öffnen/schließen}
menuText D WindowsBook "Buchfenster" 0 {Buchfenster öffnen/schließen}
menuText D WindowsCorrChess "Fernschachfenster" 0 {Öffnet/schließt das Fernschachfenster}

# Tools menu:
menuText D Tools "Werkzeuge" 0
menuText D ToolsAnalysis "Analyse-Engine..." 0 \
  {Schachanalyse-Programm starten/beenden}
menuText D ToolsAnalysis2 "Analyse-Engine #2..." 16 \
  {Schachanalyse-Programm Nr.2 starten/beenden}
menuText D ToolsCross "Kreuztabelle" 0 {Kreuztabelle für diese Partie anzeigen}
menuText D ToolsEmail "E-Mail-Manager" 7 \
  {E-Mail-Manager öffnen/schließen}
menuText D ToolsFilterGraph "Rel. Filtergrafik" 0 \
  {Filtergrafik mit relativen Werten öffnen/schließen}
menuText D ToolsAbsFilterGraph "Abs. Filtergrafik" 11 \
  {Filtergrafik mit absoluten Werten öffnen/schließen}
menuText D ToolsOpReport "Eröffnungsbericht" 0 \
  {Ausführliche Eröffnungsübersicht für die aktuelle Position erstellen}
menuText D ToolsOpenBaseAsTree "Öffne DB als Baum" 0   {Öffne DB und verwende sie im Zugbaum-Fenster}
menuText D ToolsOpenRecentBaseAsTree "Letzte DB als Baum" 0   {Öffne zuletzt verwendete DB im Zugbaum-Fenster}
menuText D ToolsTracker "Figurenverteilung"  7 \
  {Figurenverteilungsfenster öffnen}
menuText D ToolsTraining "Training"  0 {Trainingswerkzeuge (Taktik, Eröffnungen,...) }
menuText D ToolsTacticalGame "Trainingspartie"  0 {Trainingspartie spielen}
menuText D ToolsSeriousGame "Ernste Partie"  0 {Ernste Partie spielen}
menuText D ToolsTrainOpenings "Eröffnungen"  0 {Eröffnungsrepertoire trainieren}
menuText D ToolsTrainReviewGame "Partie nachspielen"  0 {Finden von Zügen wie in vorgegebener Partie}
menuText D ToolsTrainTactics "Taktik"  0 {Taktische Stellungen lösen}
menuText D ToolsTrainCalvar "Varianten berechnen"  0 {Training zum Berechnen von Varianten}
menuText D ToolsTrainFindBestMove "Besten Zug finden"  0 {Find best move}
menuText D ToolsTrainFics "Internetpartie (FICS)"  0 {Internetpartie auf freechess.org}
menuText D ToolsEngineTournament "Engine Turnier"  0 {Start a tournament between chess engines}
menuText D ToolsBookTuning "Buch abstimmen" 0 {Buch abstimmen}
menuText D ToolsConnectHardware "Hardware verbinden" 0 {Externe Hardware mit Scid verbinden}
menuText D ToolsConnectHardwareConfigure "Konfigurieren..." 0 {Hardware und Verbindung konfigurieren}
menuText D ToolsConnectHardwareNovagCitrineConnect "Novag Citrine verbinden" 0 {Novag Citrine mit Scid verbinden}
menuText D ToolsConnectHardwareInputEngineConnect "Input Engine verbinden" 0 {Input Engine (z.B. DGT Brett) mit Scid verbinden}
menuText D ToolsPInfo "Spielerinformation"  0 \
  {Spielerinformation öffnen/schließen}
menuText D ToolsPlayerReport "Spielerbericht..." 7 \
  {Erzeuge einen Spielerbericht}
menuText D ToolsRating "ELO-Zahl-Verlauf" 4 \
  {Wertungsverlauf beider Spieler grafisch darstellen}
menuText D ToolsScore "Partiebewertung" 0 {Partie-Bewertung und Zeitverbrauch anzeigen}
menuText D ToolsExpCurrent "Partie exportieren" 8 \
  {Aktuelle Partie in eine Textdatei schreiben}
menuText D ToolsExpCurrentPGN "Partie in PGN-Datei exportieren..." 10 \
  {Aktuelle Partie in eine PGN-Datei schreiben}
menuText D ToolsExpCurrentHTML "Partie in HTML-Datei exportieren..." 10 \
  {Aktuelle Partie in eine HTML-Datei schreiben}
menuText D ToolsExpCurrentHTMLJS "Partie in HTML/JavaScript-Datei exportieren..." 15 {Aktuelle Partie wird in eine HTML und JavaScript Datei exportiert.}  
menuText D ToolsExpCurrentLaTeX "Partie in LaTeX-Datei exportieren..." 10 \
  {Aktuelle Partie in eine LaTeX-Datei schreiben}
menuText D ToolsExpFilter "Alles im Filter exportieren" 16 \
  {Alle Partien im Filter in eine Textdatei schreiben}
menuText D ToolsExpFilterPGN "Filter in PGN-Datei exportieren..." 10 \
  {Alle Partien im Filter in eine PGN-Datei schreiben}
menuText D ToolsExpFilterHTML "Filter in HTML-Datei exportieren..." 10 \
  {Alle Partien im Filter in eine HTML-Datei schreiben}
menuText D ToolsExpFilterHTMLJS "Filter in HTML/JavaScript exportieren..." 17 {Alle Partien im Filter werden in eine HTML und JavaScript Datei exportiert.}  
menuText D ToolsExpFilterLaTeX "Filter in LaTeX-Datei exportieren..." 10 \
  {Alle Partien im Filter in eine LaTeX-Datei schreiben}
menuText D ToolsImportOne "Eine PGN-Partie importieren..." 16 \
  {Eine Partie im PGN-Format eingeben oder einfügen}
menuText D ToolsImportFile "PGN-Partien importieren..." 0 \
  {Partien aus einer PGN-Datei lesen}
menuText D ToolsStartEngine1 "Starte Engine1" 0  {Starte Analyse von Analyse-Engine1}
menuText D ToolsStartEngine2 "Starte Engine2" 0  {Starte Analyse von Analyse-Engine2}
menuText D ToolsCaptureBoard "Brettfoto..." 0  {Aktuelles Brett als Bild speichern.}
#-----AW------
###  BC   GHIJKL  O Q  TUV XY 
menuText D Play "Spielen" 0 {Partien spielen}
menuText D CorrespondenceChess "Fernschach" 0 {Funktionen für eMail und Xfcc basiertes Fernschach}
menuText D CCConfigure "Einstellungen..." 0 {Generelle Einstellungen und externe Werkzeuge}
menuText D CCConfigRelay "Beobachtete Partien..." 10 {Configure games to be observed}
menuText D CCOpenDB "Datenbank öffnen..." 0 {Öffnet die Standarddatenbank für Fernschachpartien}
menuText D CCRetrieve "Partien synchronisieren" 0 {Synchronisiert Partien mit dem Posteingang und mittels (Xfcc-)Hilfsprogramm}
menuText D CCInbox "Posteingang synchronisieren" 12 {Synchronisiert alle Partien im Posteingang}
menuText D CCSend "Zug senden" 0 {Verschickt den letzten Zug via eMail oder exterem (Xfcc-)Hilfsprogramm}
menuText D CCResign "Aufgeben" 0 {Partie aufgeben (funktioniert nicht via eMail)}
menuText D CCClaimDraw "Remis erklären" 1 {Zug senden und Partie Remis erklären (funktioniert nicht via eMail)}
menuText D CCOfferDraw "Remis anbieten" 0 {Zug senden und Remis anbieten (funktioniert nicht via eMail)}
menuText D CCAcceptDraw "Remis annehmen" 10 {Remis annehmen (funktioniert nicht via eMail)}
menuText D CCNewMailGame "Neue eMail Partie..." 0 {Beginnt eine neue Partie via eMail}
menuText D CCMailMove "Zug per eMail senden..." 9 {Verschickt den letzten Zug per eMail}
menuText D CCGamePage "Partieseite (WWW)..." 13 {Ruft die Webseite der Partie auf}
menuText D CCEditCopy "Partieliste in Ablage" 0 {Kopiert die Parieliste als CSV in die Zwischenablage}
#-----AW------

# Options menu:
menuText D Options "Optionen" 0
menuText D OptionsBoardGraphics "Brettfelder..." 3 {Grafik für Brettfelder auswählen}
translate D OptionsBGW {Grafikdatei für Felder auswählen}
translate D OptionsBoardGraphicsText {Grafikdateien für helle und dunkle Felder auswählen:}
menuText D OptionsBoardNames "Meine Spielernamen..." 0 {Editiere meine Spielernamen}
menuText D OptionsExport "Export" 1 {Textausgabeoptionen ändern}
menuText D OptionsFonts "Schriftarten" 3 {Schriftarten auswählen}
menuText D OptionsFontsRegular "Normal" 0 {Standardzeichensatz}
menuText D OptionsFontsMenu "Menü" 0 {Schriftart für Menüs}
menuText D OptionsFontsSmall "Klein" 0 {Kleine Zeichen}
menuText D OptionsFontsTiny "Winzig" 0 {Kleinster Font}
menuText D OptionsFontsFixed "Fest" 0 {Schriftart mit fester Breite (nicht-proportional)}
menuText D OptionsGInfo "Partieinformation" 0 {Optionen für Informationsbereich}
menuText D OptionsLanguage "Sprache" 0 {Sprache wählen}
menuText D OptionsMovesTranslatePieces "Figurennamen übersetzen" 0 {Übersetze den ersten Buchstaben der Figurenbezeichnung}
menuText D OptionsMovesHighlightLastMove "Letzen Zug hervorheben" 0 {Rahmt Start- und Zielfeld des letzten Zuges farbig ein}
menuText D OptionsMovesHighlightLastMoveDisplay "Anzeigen" 0 {Hervorhebung des letzen Zuges anzeigen}
menuText D OptionsMovesHighlightLastMoveWidth "Dicke" 0 {Dicke der Linie}
menuText D OptionsMovesHighlightLastMoveColor "Farbe" 0 {Farbe der Linie}
menuText D OptionsMovesHighlightLastMoveArrow "inklusive Pfeil" 0 {Pfeil ebenfalls hervorheben}
menuText D OptionsMoves "Züge" 0 {Zugeingabeoptionen}
menuText D OptionsMovesAnimate "Animation" 0 \
  {Zeit für Zuganimation einstellen}
menuText D OptionsMovesDelay "Autom. Vorspielen..." 7 \
  {Zeit für automatisches Vorspielen einstellen}
menuText D OptionsMovesCoord "Tastatureingabe" 0 \
  {Zugeingabe über Koordinaten ("g1f3") akzeptieren}
menuText D OptionsMovesSuggest "Zugvorschlag zeigen" 0 \
  {Zugvorschlag ein-/ausschalten}
menuText D OptionsShowVarPopup "Variantenfenster zeigen" 0 { Anzeige des Variantenauswahlfensters ein/ausschalten}  
menuText D OptionsMovesSpace "Leerzeichen nach Zugnummer einfügen" 0 {Leerzeichen nach Zugnummer einfügen}  
menuText D OptionsMovesKey "Autom. Zugergänzung" 10 \
  {Automatische Zugergänzung ein-/ausschalten}
menuText D OptionsMovesShowVarArrows "Pfeile für Varianten anzeigen" 0 {Zeige Pfeile in Varianten an: ja/nein}
menuText D OptionsMovesGlossOfDanger "Bedrohte Figuren einfärben" 0 {Markieren von gefärdeten Figuren ein/ausschalten}
menuText D OptionsNumbers "Zahlenformat" 5 {Zahlenformat wählen}
menuText D OptionsTheme "Design" 0 {Verändert das Aussehen der Oberfläche}
menuText D OptionsWindows "Fenster" 6 {Fenster-Optionen}
menuText D OptionsSounds "Sprachausgabe" 0 {Sprachausgabe für Zugankündigung einstellen}
menuText D OptionsResources "Ressourcen..." 0 {Standard Dateien und Verzeichnisse einstellen}
menuText D OptionsWindowsDock "Fenster anheften" 0 {Fenster in der Hauptanwendung öffnen}
menuText D OptionsWindowsSaveLayout "Layout speichern" 0 {Speichern des aktuellen Fensterlayout}
menuText D OptionsWindowsRestoreLayout "Layout laden" 0 {Gespeichertes Fensterlayout wieder herstellen}
menuText D OptionsWindowsShowGameInfo "Partieinformationen" 0 {Partieinformation anzeigen}
menuText D OptionsWindowsAutoLoadLayout "Automatisch laden: 1. Layout" 0 {Lädt das erste definierte Layout automatisch beim Programmstart}
menuText D OptionsECO "ECO-Datei" 0 {ECO-Klassifikationsdatei laden}
menuText D OptionsSpell "Schreibkorrekturdatei" 7 \
  {Datei für Scid-Rechtschreibprüfung laden}
menuText D OptionsTable "Verzeichnisse der Endspieltabellen" 16 \
  {Eine Endspieltabellendatei wählen (und damit alle Tabellen in diesem Verzeichnis)}
menuText D OptionsRecent "Letzte Dateien" 9 \
   {Anzahl der aktuellen Dateien im Dateimenü ändern}
menuText D OptionsBooksDir "Verzeichnis für Eröffnungsbücher" 0 {Eröffnungsbuchverzeichnis einstellen}
menuText D OptionsTacticsBasesDir "Verzeichnis für Taktikdatenbanken" 0 {Verzeichnis für taktische Trainingsdatenbanken einstellen}
menuText D OptionsPhotosDir "Verzeichnis für Spielerbilder" 0 {Verzeichnis für Bilder einstellen}
menuText D OptionsThemeDir "Datei mit Design(s)"  0 { Packetdatei für GUI Design Themen einstellen }
menuText D OptionsSave "Optionen speichern" 0 \
  "Alle einstellbaren Optionen in der Datei $::optionsFile sichern"
menuText D OptionsAutoSave "Speichern beim Beenden" 0 \
  {Alle Optionen beim Beenden von Scid automatisch speichern}

# Help menu:
menuText D Help "Hilfe" 0
menuText D HelpContents "Inhalt" 0 {Gehe zum Inhaltsverzeichnis}
menuText D HelpIndex "Index" 4 {Gehe zum Hilfeindex}
menuText D HelpGuide "Kurzanleitung" 4 {Ein Scid-Schnelldurchgang}
menuText D HelpHints "Hinweise" 0 {Die Scid-Kurztips}
menuText D HelpContact "Kontaktinformation" 0 {Hilfe zur Kontaktinformation}
menuText D HelpTip "Tagestip" 0 {Zeigt einen nützlichen Tip an}
menuText D HelpStartup "Startfenster" 0 {Startfenster zeigen}
menuText D HelpAbout "Über Scid" 0 {Informationen zu Scid}

# Game info box popup menu:
menuText D GInfoHideNext "Nächsten Zug verstecken" 13
menuText D GInfoMaterial "Materialwerte zeigen" 0
menuText D GInfoFEN "FEN zeigen" 0
menuText D GInfoMarks "Gefärbte Felder und Pfeile zeigen" 0
menuText D GInfoWrap "Umbruch bei langen Zeilen" 0
menuText D GInfoFullComment "Vollständigen Kommentar zeigen" 14
menuText D GInfoPhotos "Fotos zeigen" 1
menuText D GInfoTBNothing "Endspieltabellen: nichts" 18
menuText D GInfoTBResult "Endspieltabellen: nur Ergebnis" 22
menuText D GInfoTBAll "Endspieltabellen: Ergebnis und bester Zug" 31
menuText D GInfoDelete "(Ent)Lösche diese Partie" 5
menuText D GInfoMark "(Un)Markiere diese Partie" 4
menuText D GInfoInformant "Informatorwerte konfigurieren" 0

# General buttons:
translate D Back {Zurück}
translate D Browse {Auswählen}
translate D Cancel {Abbrechen}
translate D Continue {Weiter}
translate D Clear {Leeren}
translate D Close {Schließen}
translate D Contents {Inhalt}
translate D Defaults {Standard}
translate D Delete {Lösche}
translate D Graph {Grafik}
translate D Help {Hilfe}
translate D Import {Importieren}
translate D Index {Index}
translate D LoadGame {Partie laden}
translate D BrowseGame {Partie betrachten}
translate D MergeGame {Partie zusammenfügen}
translate D MergeGames {Partien kombinieren}
translate D Preview {Vorschau} ;# Voransicht!? (KDE)
translate D Revert {Umkehren}
translate D Save {Speichern}
translate D Search {Suchen}
translate D Stop {Halt}
translate D Store {Speichern}
translate D Update {Aktualisieren}
translate D ChangeOrient {Fensterausrichtung ändern}
translate D ShowIcons {Symbole anzeigen}
translate D None {Keine}
translate D First {Erste}
translate D Current {Aktuelle}
translate D Last {Letzte}

# General messages:
translate D game {Partie}
translate D games {Partien}
translate D move {Zug}
translate D moves {Züge}
translate D all {Alle}
translate D Yes {Ja}
translate D No {Nein}
translate D Both {Beide}
translate D King {König}
translate D Queen {Dame}
translate D Rook {Turm}
translate D Bishop {Läufer}
translate D Knight {Springer}
translate D Pawn {Bauer}
translate D White {Weiß}
translate D Black {Schwarz}
translate D Player {Spieler}
translate D Rating {Elo}
translate D RatingDiff {Elo-Differenz (Weiß - Schwarz)}
translate D AverageRating {Elo-Durchschnitt}
translate D Event {Turnier}
translate D Site {Ort}
translate D Country {Land}
translate D IgnoreColors {Farben ignorieren}
translate D Date {Datum}
translate D EventDate {Turnierdatum}
translate D Decade {Dekade}
translate D Year {Jahr}
translate D Month {Monat}
translate D Months {Januar Februar März April Mai Juni Juli August September Oktober November Dezember}
translate D Days {Son Mon Die Mit Don Fre Sam}
translate D YearToToday {-1J}
translate D YearToTodayTooltip {Setze das Datum 1 Jahr zurück bis heute}
translate D Result {Ergebnis}
translate D Round {Runde}
translate D Length {Länge}
translate D ECOCode {ECO-Code}
translate D ECO {ECO}
translate D Deleted {Gelöscht}
translate D SearchResults {Suchergebnisse}
translate D OpeningTheDatabase {Datenbank öffnen}
translate D Database {Datenbank}
translate D Filter {Filter}
translate D noGames {keine Partien}
translate D allGames {alle Partien}
translate D empty {leer}
translate D clipbase {Ablage}
translate D score {Punkte}
translate D StartPos {Stellung}
translate D Total {Summe}
translate D readonly {schreibgeschützt}

# Standard error messages:
translate D ErrNotOpen {Dies ist keine geöffnete Datenbank.}
translate D ErrReadOnly \
  {Diese Datenbank ist schreibgeschützt; sie kann nicht geändert werden.}
translate D ErrSearchInterrupted \
  {Suche wurde unterbrochen; Ergebnisse sind unvollständig.}

# Game information:
translate D twin {Dublette}
translate D deleted {gelöscht}
translate D comment {Kommentar}
translate D hidden {versteckt}
translate D LastMove {letzter Zug}
translate D NextMove {nächster Zug}
translate D GameStart {Partieanfang}
translate D LineStart {Beginn der Zugfolge}
translate D GameEnd {Partieende}
translate D LineEnd {Ende der Zugfolge}

# Player information:
translate D PInfoAll {Ergebnisse für <b>alle</b> Spiele}
translate D PInfoFilter {Ergebnisse für <b>Filter</b>-Spiele}
translate D PInfoAgainst {Ergebnisse gegen}
translate D PInfoMostWhite {Häufigste Eröffnungen als Weißer}
translate D PInfoMostBlack {Häufigste Eröffnungen als Schwarzer}
translate D PInfoRating {ELO-Historie}
translate D PInfoBio {Biographie}
translate D PInfoEditRatings {Editiere Ratings}
translate D PInfoEloFile {Datei}

# Tablebase information:
translate D Draw {Remis}
translate D stalemate {Patt}
translate D withAllMoves {mit allen Zügen}
translate D withAllButOneMove {mit allen außer einem Zug}
translate D with {mit}
translate D only {nur}
translate D lose {verlieren}
translate D loses {verliert}
translate D allOthersLose {alle anderen verlieren}
translate D matesIn {setzt Matt in}
translate D hasCheckmated {hat Matt gesetzt}
translate D longest {längste}
translate D WinningMoves {Gewinnzüge}
translate D DrawingMoves {Remiszüge}
translate D LosingMoves {Verlustzüge}
translate D UnknownMoves {Züge mit unbekanntem Resultat}
translate D SideLongest {Seite   Längstes  %     %     %\n}
translate D toNoveMate  {am Zug    Matt   Gew. Remis Verlust\n}
translate D NoSummary {\nKeine Zusammenfassung für diese Tablebase.}
translate D MutualZugzwang {Gegenseitiger Zugzwang: }
translate D SelectetZugzwang {\nAusgewählte Zugzwangpositionen:}
translate D WdrawBloss {Weiß remis,Schwarz verliert:}
translate D BdrawWloss {Schwarz remis, Weiß verliert: }
translate D AllLoss {" Wer zieht verliert:     " }

# Tip of the day:
translate D Tip {Tip}
translate D TipAtStartup {Tip beim Starten}

# Tree window menus:
menuText D TreeFile "Datei" 0
menuText D TreeFileFillWithBase "Cache mit Datenbank füllen" 0 {Die Cache-Datei wird mit allen Partien der aktuellen Datenbank befüllt.}
menuText D TreeFileFillWithGame "Cache mit Partie füllen" 0 {Die Cache-Datei wird mit der aktuellen Partien befüllt.}
menuText D TreeFileSetCacheSize "Cache Größe" 0 {Cachegröße auswählen.}
menuText D TreeFileCacheInfo "Cache info" 0 {Info über Cachenutzung anzeigen.}
menuText D TreeFileSave "Cache-Datei sichern" 12 \
  {Speichere die Zugbaum-Cache-Datei (.stc)}
menuText D TreeFileFill "Cache-Datei füllen" 12 \
  {Fülle die Cache-Datei mit häufigen Eröffnungspositionen}
menuText D TreeFileBest "Beste Partien" 0 \
  {Zeige die Liste bester Partien im Baum}
menuText D TreeFileGraph "Grafikfenster" 0 \
  {Zeige die Grafik für diesen Ast}
menuText D TreeFileCopy "Kopiere Baumfenster in Zwischenablage" 0 \
  {Kopiere die Zugbaum-Statistik in die Zwischenablage}
menuText D TreeFileClose "Baumfenster schließen" 12 {Schließe Zugbaum}
menuText D TreeMask "Maskieren" 0
menuText D TreeMaskNew "Neu" 0 {Neue Maske anlegen}
menuText D TreeMaskOpen "Öffnen" 0 {Maske öffnen}
menuText D TreeMaskOpenRecent "Aktuelle Masken" 0 {Zuletzt geöffnete Masken erneut laden}
menuText D TreeMaskSave "Speichern" 0 {Maske speichern}
menuText D TreeMaskClose "Schließen" 0 {Maske schließen}
menuText D TreeMaskFillWithGame "Mit aktueller Partie füllen" 0 {Maske mit der aktuellen Partie füllen}
menuText D TreeMaskFillWithBase "Mit Datenbank füllen" 0 {Maske mit Datenbankpartien füllen}
menuText D TreeMaskInfo "Info" 0 {Statistik für die aktuelle Maske anzeigen}
menuText D TreeMaskDisplay "Maske als Baum" 0 {Zeigt den Inhalt der aktuellen Maske als Zugbaum}
menuText D TreeMaskSearch "Suchen" 0 {Suchen innerhalb der aktuellen Maske}
menuText D TreeSort "Sortieren" 0
menuText D TreeSortAlpha "Alphabetisch" 0
menuText D TreeSortECO "ECO-Code" 0
menuText D TreeSortFreq "Häufigkeit" 0
menuText D TreeSortScore "Punkte" 0
menuText D TreeOpt "Optionen" 0
menuText D TreeOptSlowmode "Gründliche Suche" 0 {Update mit hoher Genauigkeit, langsamer}
menuText D TreeOptFastmode "Schneller Modus" 0 {Schnelles Update (ignoriert Zugumstellungen)}
menuText D TreeOptFastAndSlowmode "Gründlicher und Schneller Modus" 0 {Zunächst schneller Updatemodus, dann nacharbeit im gründlichen Modus}
menuText D TreeOptStartStop "Automatisch aktualisieren" 0 {Schaltet das automatische aktualisieren ein/aus}
menuText D TreeOptLock "Anbinden" 0 \
  {Zugbaum an aktive Datenbank anbinden(/lösen)}
menuText D TreeOptTraining "Training" 0 {Trainingsmodus ein-/ausschalten}
menuText D TreeOptAutosave "Autom. Cache-Datei sichern" 4 \
  {Beim Schließen des Zugbaums automatisch Cache-Datei sichern}
menuText D TreeHelp "Hilfe" 0
menuText D TreeHelpTree "Zugbaumhilfe" 0
menuText D TreeHelpIndex "Index" 0
translate D SaveCache {Cache sichern}
translate D Training {Training}
translate D LockTree {Anbinden}
translate D TreeLocked {angebunden}
translate D TreeBest {Beste}
translate D TreeBestGames {Beste Zugbaumpartien}
# Note: the next message is the tree window title row. After editing it,
# check the tree window to make sure it lines up with the actual columns.
translate D TreeTitleRow \
  {    Zug    ECO       Häufigkeit    Pkte   Elo  Erflg Jahr %Remis}
translate D TreeTotal {SUMME}
translate D DoYouWantToSaveFirst {Soll zuerst gespeichert werden}
translate D AddToMask {Zur Maske hinzufügen}
translate D RemoveFromMask {Entferne von Maske}
translate D AddThisMoveToMask {Diesen Zug zur Maske hinzufügen}
translate D SearchMask {Suche in einer Maske}
translate D DisplayMask {Maske als Zugbaum}
translate D Nag {NAG Code}
translate D Marker {Marker}
translate D Include {Enthalten}
translate D Exclude {Ausgeschlossen}
translate D MainLine {Hauptvariante}
translate D Bookmark {Lesezeichen}
translate D NewLine {Neue Variante}
translate D ToBeVerified {Überprüfen}
translate D ToTrain {Trainieren}
translate D Dubious {Zweifelhaft}
translate D ToRemove {Entfernen}
translate D NoMarker {Keine Kennzeichen}
translate D ColorMarker {Farbe}
translate D WhiteMark {Weiß}
translate D GreenMark {Grün}
translate D YellowMark {Gelb}
translate D BlueMark {Blau}
translate D RedMark {Rot}
translate D CommentMove {Zug kommentieren}
translate D CommentPosition {Position kommentieren}
translate D AddMoveToMaskFirst {Zug zuerst zur Maske hinzufüngen}
translate D OpenAMaskFileFirst {Zunächst Maske öffnen}
translate D Positions {Positionen}
translate D Moves {Züge}

# Finder window:
menuText D FinderFile "Datei" 0
menuText D FinderFileSubdirs "mit Unterverzeichnissen" 0
menuText D FinderFileClose "Dateisuche schließen" 0
menuText D FinderSort "Sortieren" 0
menuText D FinderSortType "Typ" 0
menuText D FinderSortSize "Größe" 0
menuText D FinderSortMod "Modifiziert" 0
menuText D FinderSortName "Namen" 0
menuText D FinderSortPath "Pfad" 0
menuText D FinderTypes "Typen" 0
menuText D FinderTypesScid "Scid-Datenbanken" 0
menuText D FinderTypesOld "Datenbanken im alten Format" 0
menuText D FinderTypesPGN "PGN-Dateien" 0
menuText D FinderTypesEPD "EPD-Dateien" 0
menuText D FinderTypesRep "Repertoire-Dateien" 0
menuText D FinderHelp "Hilfe" 0
menuText D FinderHelpFinder "Hilfe für Dateisuche" 0
menuText D FinderHelpIndex "Index" 0
translate D FileFinder {Dateisuche}
translate D FinderDir {Verzeichnis}
translate D FinderDirs {Verzeichnisse}
translate D FinderFiles {Dateien}
translate D FinderUpDir {hoch}
translate D FinderCtxOpen {Öffnen}
translate D FinderCtxBackup {Sicherungskopie}
translate D FinderCtxCopy {Kopieren}
translate D FinderCtxMove {Verschieben}
translate D FinderCtxDelete {Löschen}

# Player finder:
menuText D PListFile "Datei" 0
menuText D PListFileUpdate "Aktualisieren" 0
menuText D PListFileClose "Spielersuche schließen" 7
menuText D PListSort "Sortieren" 0
menuText D PListSortName "Name" 0
menuText D PListSortElo "Elo" 0
menuText D PListSortGames "Partien" 0
menuText D PListSortOldest "Älteste" 0
menuText D PListSortNewest "Neueste" 0

# Tournament finder:
menuText D TmtFile "Datei" 0
menuText D TmtFileUpdate "Aktualisieren" 0
menuText D TmtFileClose "Turniersuche schließen" 0
menuText D TmtSort "Sortieren" 0
menuText D TmtSortDate "Datum" 0
menuText D TmtSortPlayers "Spieler" 0
menuText D TmtSortGames "Partien" 0
menuText D TmtSortElo "Elo" 0
menuText D TmtSortSite "Ort" 0
menuText D TmtSortEvent "Turnier" 1
menuText D TmtSortWinner "Gewinner" 0
translate D TmtLimit "Anzeigelimit"
translate D TmtMeanElo "Elo-Durchschnitt"
translate D TmtNone "Keine zutreffenden Turniere gefunden."

# Graph windows:
menuText D GraphFile "Datei" 0
menuText D GraphFileColor "Als Farb-Postscript speichern..." 4
menuText D GraphFileGrey "Als Graustufen-Postscript speichern..." 4
menuText D GraphFileClose "Fenster schließen" 0
menuText D GraphOptions "Optionen" 0
menuText D GraphOptionsWhite "Weiß" 0
menuText D GraphOptionsBlack "Schwarz" 0
menuText D GraphOptionsBoth "Beide" 0
menuText D GraphOptionsPInfo "Spielerinfo Spieler" 1
menuText D GraphOptionsEloFile "Elo von Rating-Datei" 0
menuText D GraphOptionsEloDB "Elo von Datenbank" 0
translate D GraphFilterTitle "Filtergrafik: Häufigkeit pro 1000 Partien"
translate D GraphAbsFilterTitle "Filtergrafik: Häufigkeit der Partien"
translate D ConfigureFilter "X-Achse für Jahr, Elo und Züge konfigurieren"
translate D FilterEstimate "Schätzen"
translate D TitleFilterGraph "Scid: Filtergrafik"

# Analysis window:
translate D AddVariation {Variante hinzufügen}
translate D AddAllVariations {Alle Varianten hinzufügen}
translate D AddMove {Zug hinzufügen}
translate D Annotate {Autom. kommentieren}
translate D ShowAnalysisBoard {Analysebrett anzeigen}
translate D ShowInfo {Engine-Information anzeigen}
translate D FinishGame {Partie beenden}
translate D StopEngine {Engine anhalten}
translate D StartEngine {Engine starten}
translate D LockEngine {Anbinden an aktuelle Position}
translate D AnalysisCommand {Analysebefehl}
translate D PreviousChoices {Vorherige Wahl}
translate D AnnotateTime {Zeit pro Zug in Sek.}
translate D AnnotateWhich {Varianten hinzufügen}
translate D AnnotateAll {Für Züge beider Seiten}
translate D AnnotateAllMoves {Alle Züge kommentieren}
translate D AnnotateWhite {Nur für Züge von Weiß}
translate D AnnotateBlack {Nur für Züge von Schwarz}
translate D AnnotateBlundersOnly {Wenn der Partiezug ein Fehler ist}
translate D AnnotateBlundersOnlyScoreChange {Analyse berichtet Fehler, Bewertungsänderung von/nach: }
translate D BlundersThreshold {Schwelle}
translate D ScoreAllMoves {Alle Züge bewerten}
translate D LowPriority {Niedrige CPU-Priorität}
translate D ClickHereToSeeMoves {Hier klicken um Züge anzuzeigen}
translate D ConfigureInformant {Informatorwerte}
translate D Informant!? {Interessanter Zug}
translate D Informant? {Schwacher Zug}
translate D Informant?? {Fehler}
translate D Informant?! {Zweifelhafter Zug}
translate D Informant+= {Weiß hat leichten Vorteil}
translate D Informant+/- {Weiß hat mäßigen Vorteil}
translate D Informant+- {Weiß hat großen Vorteil}
translate D Informant++- {Partie wird gewonnen}
translate D Book {Buch}
translate D OtherBookMoves {Eröffnungsbuch des Gegners}
translate D OtherBookMovesTooltip {Züge, für die der Gegner eine Antwort kennt}

# Analysis Engine open dialog:
translate D EngineList {Analyse-Engines}
translate D EngineName {Name}
translate D EngineCmd {Befehl}
translate D EngineArgs {Parameter}
translate D EngineDir {Verzeichnis}
translate D EngineElo {Elo}
translate D EngineTime {Datum}
translate D EngineNew {Neu}
translate D EngineEdit {Bearbeiten}
translate D EngineRequired {Fettgedruckte Parameter obligatorisch, andere optional}

# PGN window menus:
menuText D PgnFile "Datei" 0
menuText D PgnFileCopy "Kopiere Spiel in Zwischenablage" 0
menuText D PgnFilePrint "Drucken in Datei..." 0
menuText D PgnFileClose "PGN-Fenster schließen" 0
menuText D PgnOpt "Ausgabe" 0
menuText D PgnOptColor "Farbige Darstellung" 0
menuText D PgnOptShort "Kurzer (3-Zeilen) Vorspann (Header)" 8
menuText D PgnOptSymbols "Symbolische Darstellung" 0
menuText D PgnOptIndentC "Kommentare einrücken" 0
menuText D PgnOptIndentV "Varianten einrücken" 0
menuText D PgnOptColumn "Tabellarisch (ein Zug pro Zeile)" 0
menuText D PgnOptSpace "Leerzeichen nach Zugnummer" 0
menuText D PgnOptStripMarks "Farbfelder-/Pfeile-Zeichen entfernen" 27
menuText D PgnOptBoldMainLine "Partiezüge in Fettdruck" 0
menuText D PgnColor "Farben" 0
menuText D PgnColorHeader "Vorspann..." 0
menuText D PgnColorAnno "Anmerkungen..." 0
menuText D PgnColorComments "Kommentare..." 0
menuText D PgnColorVars "Varianten..." 0
menuText D PgnColorBackground "Hintergrund..." 0
menuText D PgnColorMain "Partieverlauf..." 0
menuText D PgnColorCurrent "Hintergrund aktueller Zug..." 1
menuText D PgnHelp "Hilfe" 0
menuText D PgnHelpPgn "PGN-Hilfe" 0
menuText D PgnHelpIndex "Index" 0
translate D PgnWindowTitle {Partienotation - Partie %u}

# Crosstable window menus:
menuText D CrosstabFile "Datei" 0
menuText D CrosstabFileText "Ausgabe in Textdatei..." 11
menuText D CrosstabFileHtml "Ausgabe in HTML-Datei..." 11
menuText D CrosstabFileLaTeX "Ausgabe in LaTeX-Datei..." 11
menuText D CrosstabFileClose "Kreuztabelle schließen" 0
menuText D CrosstabEdit "Bearbeiten" 0
menuText D CrosstabEditEvent "Ereignis" 0
menuText D CrosstabEditSite "Ort" 0
menuText D CrosstabEditDate "Datum" 0
menuText D CrosstabOpt "Ausgabe" 0
menuText D CrosstabOptAll "Jeder gegen jeden" 0
menuText D CrosstabOptSwiss "Schweizer System" 0
menuText D CrosstabOptKnockout "K.o.-System" 0
menuText D CrosstabOptAuto "Automatisch" 1
menuText D CrosstabOptAges "Alter in Jahren" 0
menuText D CrosstabOptNats "Nationalität" 0
menuText D CrosstabOptRatings "Elo" 0
menuText D CrosstabOptTitles "Titel" 0
menuText D CrosstabOptBreaks "Stichkampfpunkte" 10
menuText D CrosstabOptDeleted "Inklusive gelöschte Partien" 10
menuText D CrosstabOptColors "Farben (nur Schweizer System)" 0
menuText D CrosstabOptColumnNumbers "Numerierte Spalten (Nur jeder-gegen-jeden-Tabelle)" 2
menuText D CrosstabOptGroup "Punktgruppen" 5
menuText D CrosstabSort "Sortieren" 0
menuText D CrosstabSortName "Name" 0
menuText D CrosstabSortRating "Elo" 0
menuText D CrosstabSortScore "Punkte" 0
menuText D CrosstabColor "Farben" 0
menuText D CrosstabColorPlain "Text" 0
menuText D CrosstabColorHyper "Hypertext" 0
menuText D CrosstabHelp "Hilfe" 0
menuText D CrosstabHelpCross "Kreuztabelle-Hilfe" 0
menuText D CrosstabHelpIndex "Index" 0
translate D SetFilter {Filter setzen}
translate D AddToFilter {Zum Filter hinzufügen}
translate D Swiss {Schweizer}
translate D Category {Kategorie}

# Opening report window menus:
menuText D OprepFile "Datei" 0
menuText D OprepFileText "Ausgabe in Textdatei..." 11
menuText D OprepFileHtml "Ausgabe in HTML-Datei..." 11
menuText D OprepFileLaTeX "Ausgabe in LaTeX-Datei..." 11
menuText D OprepFileOptions "Optionen..." 0
menuText D OprepFileClose "Berichtsfenster schließen" 0
menuText D OprepFavorites "Favoriten" 0
menuText D OprepFavoritesAdd "Bericht hinzufügen..." 8
menuText D OprepFavoritesEdit "Favoritenbericht editieren..." 0
menuText D OprepFavoritesGenerate "Berichte erzeugen..." 9

menuText D OprepHelp "Hilfe" 0
menuText D OprepHelpReport "Eröffnungsbericht-Hilfe" 0
menuText D OprepHelpIndex "Index" 0

# Header search:
translate D HeaderSearch {Partiedaten suchen}
translate D EndSideToMove {Wer ist beim Partieende am Zug?}
translate D GamesWithNoECO {Partien ohne ECO}
translate D GameLength {Partielänge}
translate D FindGamesWith {Finde Partien mit Kennzeichen}
translate D StdStart {Grundstellung}
translate D Promotions {Umwandlung}
translate D Comments {Kommentare}
translate D Variations {Varianten}
translate D Annotations {Anmerkungen}
translate D DeleteFlag {Löschkennzeichen}
translate D WhiteOpFlag {Eröffnung Weiß}
translate D BlackOpFlag {Eröffnung Schwarz}
translate D MiddlegameFlag {Mittelspiel}
translate D EndgameFlag {Endspiel}
translate D NoveltyFlag {Neuerung}
translate D PawnFlag {Bauernstruktur}
translate D TacticsFlag {Taktik}
translate D QsideFlag {Damenflügel}
translate D KsideFlag {Königsflügel}
translate D BrilliancyFlag {Glänzend}
translate D BlunderFlag {Grober Fehler}
translate D UserFlag {Benutzer}
translate D PgnContains {PGN enthält Text}
translate D Annotator {Kommentator}
translate D Cmnts {Nur kommentierte Partien}

# Game list window:
translate D GlistNumber {Nummer}
translate D GlistWhite {Weiß}
translate D GlistBlack {Schwarz}
translate D GlistWElo {W-Elo}
translate D GlistBElo {S-Elo}
translate D GlistEvent {Turnier}
translate D GlistSite {Ort}
translate D GlistRound {Runde}
translate D GlistDate {Datum}
translate D GlistYear {Jahr}
translate D GlistEDate {Turnierdatum}
translate D GlistResult {Ergebnis}
translate D GlistLength {Länge}
translate D GlistCountry {Land}
translate D GlistECO {ECO}
translate D GlistOpening {Eröffnung}
translate D GlistEndMaterial {Endmaterial}
translate D GlistDeleted {Gelöscht}
translate D GlistFlags {Kennzeichen}
translate D GlistVars {Varianten}
translate D GlistComments {Kommentare}
translate D GlistAnnos {Anmerkungen}
translate D GlistStart {Stellung}
translate D GlistGameNumber {Partie Nummer}
translate D GlistAverageElo {ELO Durchschnitt}
translate D GlistRating {Wertungszahl}
translate D GlistFindText {Text finden}
translate D GlistMoveField {Züge}
translate D GlistEditField {Konfigurieren}
translate D GlistAddField {Hinzufügen}
translate D GlistDeleteField {Löschen}
translate D GlistWidth {Breite}
translate D GlistAlign {Ausrichtung}
translate D GlistAlignL {Ausrichtung: links}
translate D GlistAlignR {Ausrichtung: rechts}
translate D GlistAlignC {Ausrichtung: zentriert}
translate D GlistColor {Farbe}
translate D GlistSep {Trennlinie}
translate D GlistCurrentSep {-- Aktuell --}
translate D GlistNewSort {Neu}
translate D GlistAddToSort {Hinzufügen}

# base sorting
translate D GsortSort {Sortieren...}
translate D GsortDate {Datum}
translate D GsortYear {Jahr}
translate D GsortEvent {Event}
translate D GsortSite {Ort}
translate D GsortRound {Runde}
translate D GsortWhiteName {Name Weiß}
translate D GsortBlackName {Name Schwarz}
translate D GsortECO {ECO}
translate D GsortResult {Ergebnis}
translate D GsortMoveCount {Zahl der Züge}
translate D GsortAverageElo {Eloschnitt}
translate D GsortCountry {Land}
translate D GsortDeleted {Gelöscht}
translate D GsortEventDate {Event Datum}
translate D GsortWhiteElo {ELO Weiß}
translate D GsortBlackElo {ELO Schwarz}
translate D GsortComments {Kommentare}
translate D GsortVariations {Varianten}
translate D GsortNAGs {NAGs}
translate D GsortAscending {Aufsteigend}
translate D GsortDescending {Fallend}
translate D GsortAdd {Hinzufügen}
translate D GsortStore {Speichern}
translate D GsortLoad {Laden}

# menu shown with right mouse button down on game list.
translate D GlistRemoveThisGameFromFilter  {Partie aus Filter entfernen}
translate D GlistRemoveGameAndAboveFromFilter  {Partie (und alle oberhalb) aus Filter entfernen}
translate D GlistRemoveGameAndBelowFromFilter  {Partie (und alle darunter) aus Filter entfernen}
translate D GlistDeleteGame {(Ent)Loesche Partie}
translate D GlistDeleteAllGames {Lösche alle Partien im Filter} 
translate D GlistUndeleteAllGames {Alle Partien im Filter wiederherstellen}
translate D GlistMergeGameInBase {Mit Partie in anderer Datebank zusammenführen} 

# Maintenance window:
translate D DatabaseName {Datenbankname:}
translate D TypeIcon {Symbol:}
translate D NumOfGames {Partien:}
translate D NumDeletedGames {Gelöschte Partien:}
translate D NumFilterGames {Partien im Filter:}
translate D YearRange {Jahr-Spanne:}
translate D RatingRange {Elo-Spanne:}
translate D Description {Beschreibung}
translate D Flag {Kennzeichen}
translate D CustomFlags {Benutzerdefinierte Kennzeichen}
translate D DeleteCurrent {Lösche aktuelle Partie}
translate D DeleteFilter {Lösche Partien im Filter}
translate D DeleteAll {Lösche alle Partien}
translate D UndeleteCurrent {Aktuelle Partie wiederherstellen}
translate D UndeleteFilter {Partien im Filter wiederherstellen}
translate D UndeleteAll {Alle Partien wiederherstellen}
translate D DeleteTwins {Lösche Dubletten}
translate D MarkCurrent {Markiere aktuelle Partie}
translate D MarkFilter {Markiere Partien im Filter}
translate D MarkAll {Markiere alle Partien}
translate D UnmarkCurrent {Entmarkiere aktuelle Partie}
translate D UnmarkFilter {Entmarkiere Partien im Filter}
translate D UnmarkAll {Entmarkiere alle Partien}
translate D Spellchecking {Schreibkorrektur}
translate D Players {Spieler}
translate D Events {Ereignis}
translate D Sites {Ort}
translate D Rounds {Runde}
translate D DatabaseOps {Datenbankoperationen}
translate D ReclassifyGames {Partien ECO-klassifizieren}
translate D CompactDatabase {Datenbank komprimieren}
translate D SortDatabase {Datenbank sortieren}
translate D AddEloRatings {ELO-Zahlen hinzufügen}
translate D AutoloadGame {Lade automatisch Partie Nr.}
translate D StripTags {PGN-Kennungen entfernen}
translate D StripTag {Kennungen entfernen}
translate D Cleaner {Bereiniger}
translate D CleanerHelp {
Der Scid-Bereiniger wird für die aktuelle Datenbank alle Wartungsarbeiten ausführen, welche aus der unten stehenden Liste ausgewählt werden.
Aktuelle Einstellungen in den Menüs "ECO-Klassifikation" und "Dubletten löschen" werden bei Auswahl dieser Funktionen verwendet.}
translate D CleanerConfirm {
Ist der Bereiniger einmal gestartet, kann er nicht mehr unterbrochen werden!

Dies kann lange dauern, speziell bei großen Datenbanken, abhängig von den
ausgewählten Funktionen und deren Einstellungen.

Sind Sie sicher, daß Sie die ausgewählten Wartungsarbeiten starten möchten?
}
# Twinchecker
translate D TwinCheckUndelete { umdrehen; "u" beide wiederherstellen)}
translate D TwinCheckprevPair {Vorheriges Paar}
translate D TwinChecknextPair {Nächstes Paar}
translate D TwinChecker {Scid: Dublettenprüfer}
translate D TwinCheckTournament {Partien im Turnier:}
translate D TwinCheckNoTwin {Keine Dublette}
translate D TwinCheckNoTwinfound {Kein Dublette für diese Partie gefunden.

Um Dubletten anzuzeigen, bitte zuerst die Funktion "Lösche Dubletten" benutzen.}
translate D TwinCheckTag {Nutze Tag...}
translate D TwinCheckFound1 {Scid hat $result Dubletten gefunden}
translate D TwinCheckFound2 { und das Gelöscht-Flag gesetzt}
translate D TwinCheckNoDelete {In dieser Datenbank sind keine Partien zu löschen.}
translate D TwinCriteria1 {Ihre Auswahlkriterien der Dublettensuche haben eine hohe Wahrscheinlichkeit auch Partien mit ähnlichen Zügen als Dubletten zu erkennen.
}
translate D TwinCriteria2 {Es wird empfohlen, dass bei der Auswahl "Nein" für "gleiche Züge" die Auswahl "Ja" für Farbe, Ereignis, Ort, Runde, Jahr und Monat ausgewählt wird.

Wollen Sie fortsetzen und mit dieser Auswahl die Dubletten löschen? }
translate D TwinCriteria3 {Es wird empfohlen, dass bei der Auswahl "Ja" für mindestens 2 der Parameter "gleicher Ort", "gleiche Runde" und "gleiches Jahr" gesetzt werden.
    
Wollen Sie fortsetzen und mit dieser Auswahl die Dubletten löschen?}
translate D TwinCriteriaConfirm {Scid: Bestätigen der Dublettensuchparameter}
translate D TwinChangeTag "Ändern der folgenden Partie-Tags:\n\n"
translate D AllocRatingDescription "Es werden die ELO-Werte aus der Schreibkorrekturdatei den Partien hinzugefügt, wenn der/die Spieler/in keinen aktuellen Wert in der Partie besitzt, aber in der Korrekturdatei ein Wert für den Zeitraum der Partie vorhanden ist."
translate D RatingOverride "Existierende ELO-Werte überschreiben"
translate D AddRatings "Elo-Werte hinzufügen"
translate D AddedRatings {Scid hat $r Elo-Werte in $g Partien hinzugefügt.}

#Bookmark editor
translate D NewSubmenu "Neues Untermenü"

# Comment editor:
translate D AnnotationSymbols  {Kommentarzeichen:}
translate D Comment {Kommentar:}
translate D InsertMark {Markierung einfügen}
translate D InsertMarkHelp {
Markierung einfügen/löschen: Farbe, Typ, Feld wählen.
Pfeil einfügen/löschen: Rechtsklick auf zwei Felder.
}

# Nag buttons in comment editor:
translate D GoodMove {Guter Zug}
translate D PoorMove {Schwacher Zug}
translate D ExcellentMove {Ausgezeichneter Zug}
translate D Blunder {Grober Fehler}
translate D InterestingMove {Interessanter Zug}
translate D DubiousMove {Zweifelhafter Zug}
translate D WhiteDecisiveAdvantage {Weiß hat Vorteil}
translate D BlackDecisiveAdvantage {Schwarz hat Vorteil}
translate D WhiteClearAdvantage {Weiß hat klaren Vorteil}
translate D BlackClearAdvantage {Schwarz hat klaren Vorteil}
translate D WhiteSlightAdvantage {Weiß hat leichten Vorteil}
translate D BlackSlightAdvantage {Schwarz hat leichten Vorteil}
translate D Equality {Gleiche Chancen}
translate D Unclear {Unklar}
translate D Diagram {Diagramm}

# Board search:
translate D BoardSearch {Brettsuche}
translate D FilterOperation {Suche im aktuellen Filter:}
translate D FilterAnd {UND\nBeschränke Filter}
translate D FilterOr {ODER\nZum Filter hinzufügen}
translate D FilterIgnore {IGNORIEREN\nFilter zurücksetzen}
translate D SearchType {Suche nach Typ:}
translate D SearchBoardExact {Exakte Position (alle Steine auf gleichen Feldern)}
translate D SearchBoardPawns {Bauern (gleiche Figuren, alle Bauern auf gleichen Feldern)}
translate D SearchBoardFiles {Linien (gleiches Material, alle Bauern auf gleichen Linien)}
translate D SearchBoardAny {Material (gleiches Material, Bauern und Figuren beliebig)}
translate D SearchInRefDatabase { In folgender Datenbank suchen }
translate D LookInVars {Schaue in Varianten}

# Material search:
translate D MaterialSearch {Materialsuche}
translate D Material {Material}
translate D Patterns {Muster}
translate D Zero {Null}
translate D Any {Irgendeine}
translate D CurrentBoard {Aktuelle Stellung}
translate D CommonEndings {Endspiele}
translate D CommonPatterns {Häufige Muster}
translate D MaterialDiff {Materialdifferenz}
translate D squares {Felder}
translate D SameColor {Gleichfarbige}
translate D OppColor {Ungleichfarbige}
translate D Either {Beides}
translate D MoveNumberRange {Zugnummernbereich}
translate D MatchForAtLeast {Zutreffend für mindestens}
translate D HalfMoves {Halbzüge}

# Common endings in material search:
translate D EndingPawns {Bauernendspiele}
translate D EndingRookVsPawns {Turm gegen Bauer(n)}
translate D EndingRookPawnVsRook {Turm und 1 Bauer gegen Turm}
translate D EndingRookPawnsVsRook {Turm und Bauer(n) gegen Turm}
translate D EndingRooks {Turm gegen Turm}
translate D EndingRooksPassedA {Turm gegen Turm mit Freibauer}
translate D EndingRooksDouble {Doppelturm-Endspiele}
translate D EndingBishops {Läufer gegen Läufer}
translate D EndingBishopVsKnight {Läufer gegen Springer}
translate D EndingKnights {Springer gegen Springer}
translate D EndingQueens {Dame gegen Dame} ;# *** Damenendspiele !?
translate D EndingQueenPawnVsQueen {Dame und 1 Bauer gegen Dame}
translate D BishopPairVsKnightPair {Zwei Läufer gegen zwei Springer im Mittelspiel}

# Common patterns in material search:
translate D PatternWhiteIQP {Weißer isolierter Damenbauer}
translate D PatternWhiteIQPBreakE6 {Weißer Isolani: Durchbruch d4-d5 gegen e6}
translate D PatternWhiteIQPBreakC6 {Weißer Isolani: Durchbruch d4-d5 gegen c6}
translate D PatternBlackIQP {Schwarzer isolierter Damenbauer}
translate D PatternWhiteBlackIQP {Weißer gegen schwarzer Damenbauerisolani}
translate D PatternCoupleC3D4 {Isoliertes Bauernpaar c3+d4}
translate D PatternHangingC5D5 {Hängende Bauern c5 und d5 von Schwarz}
translate D PatternMaroczy {Maroczy-Zentrum (mit Bauern auf c4 und e4)}
translate D PatternRookSacC3 {Turmopfer auf c3}
translate D PatternKc1Kg8 {0-0-0 gegen 0-0 (Kc1 gegen Kg8)}
translate D PatternKg1Kc8 {0-0 gegen 0-0-0 (Kg1 gegen Kc8)}
translate D PatternLightFian {Weißfeldrige Fianchettos (Läufer g2 gegen Läufer b7)}
translate D PatternDarkFian {Schwarzfeldrige Fianchettos (Läufer b2 gegen Läufer g7)}
translate D PatternFourFian {Beiderseitiges Doppelfianchetto (Läufer auf b2,g2,b7,g7)}

# Game saving:
translate D Today {Heute}
translate D ClassifyGame {Partie klassifizieren}

# Setup position:
translate D EmptyBoard {Brett leeren}
translate D InitialBoard {Initialisiere Brett}
translate D SideToMove {Zugrecht}
translate D MoveNumber {Zugnummer}
translate D Castling {Rochade}
translate D EnPassantFile {EnPassant-Linie}
translate D ClearFen {FEN löschen}
translate D PasteFen {FEN einfügen}
translate D SaveAndContinue {Speichern und weiter}
translate D DiscardChangesAndContinue {Verwerfen und weiter}
translate D GoBack {Zurück}

# Replace move dialog:
translate D ReplaceMove {Zug ersetzen}
translate D AddNewVar {Neue Variante}
translate D NewMainLine {Neue Hauptvariante}
translate D ReplaceMoveMessage {Hier existiert bereits ein Zug.

Sie können diesen Zug ersetzen - unter Verlust aller nachfolgender Züge - oder mit dem Zug eine neue Variante hinzufügen.

(Sie können diese Anzeige vermeiden, indem Sie die Option "Fragen vor Ersetzen" im Menü Optionen:Züge deaktivieren)}

# Make database read-only dialog:
translate D ReadOnlyDialog {Wenn Sie diese Datenbank mit Schreibschutz
versehen, sind keine Änderungen möglich. Es können keine Partien gespeichert
oder ersetzt und keine Löschkennzeichen geändert werden. Alle Sortierungen oder
ECO-Klassifikationsergebnisse sind nur temporär.

Sie können den Schreibschutz einfach entfernen, indem Sie die Datenbank
schließen und wieder öffnen.
Wollen Sie diese Datenbank wirklich schreibschützen?}

# Clear game dialog:
translate D ClearGameDialog {Diese Partie wurde geändert.

Wollen Sie wirklich fortsetzen und die Änderungen verwerfen?
}

# Exit dialog:
translate D ExitDialog {Möchten Sie Scid beenden?}
translate D ExitUnsaved {Die folgenden Datenbanken haben ungesicherte Partieänderungen. Wenn Sie jetzt beenden, gehen diese Änderungen verloren.}

# Import window:
translate D PasteCurrentGame {Aktuelle Partie einfügen}
translate D ImportHelp1 {Eingeben oder Einfügen einer Partie im PGN-Format in den oberen Rahmen.}
translate D ImportHelp2 {Hier werden Fehler beim Importieren angezeigt.}
translate D OverwriteExistingMoves {Bestehende Züge überschreiben?}

# ECO Browser:
translate D ECOAllSections {alle ECO-Gruppen}
translate D ECOSection {ECO-Gruppe}
translate D ECOSummary {Zusammenfassung für}
translate D ECOFrequency {Häufigkeit der Untercodes für}

# Opening Report:
translate D OprepTitle {Eröffnungsbericht}
translate D OprepReport {Bericht}
translate D OprepGenerated {Erzeugt durch}
translate D OprepStatsHist {Statistiken und Geschichte}
translate D OprepStats {Statistiken}
translate D OprepStatAll {Alle Berichtspartien}
translate D OprepStatBoth {Beide Spieler}
translate D OprepStatSince {Nach}
translate D OprepOldest {Älteste Partien}
translate D OprepNewest {Neuste Partien}
translate D OprepPopular {Popularität}
translate D OprepFreqAll {Häufigkeit in allen Jahren: }
translate D OprepFreq1   {im letzten Jahr:            }
translate D OprepFreq5   {der letzten  5 Jahre:       }
translate D OprepFreq10  {der letzten 10 Jahre:       }
translate D OprepEvery {Eine pro %u Partien}
translate D OprepUp {mehr als %u%s von allen Jahren}
translate D OprepDown {weniger als %u%s von allen Jahren}
translate D OprepSame {keine Änderung in allen Jahren}
translate D OprepMostFrequent {Häufigste Spieler}
translate D OprepMostFrequentOpponents {Häufigste Gegner}
translate D OprepRatingsPerf {ELO und Performance}
translate D OprepAvgPerf {Durchschnitts-ELO und Performance}
translate D OprepWRating {ELO Weiß}
translate D OprepBRating {ELO Schwarz}
translate D OprepWPerf {Performance Weiß}
translate D OprepBPerf {Performance Schwarz}
translate D OprepHighRating {Spiele mit höchstem ELO-Durchschnitt}
translate D OprepTrends {Ergebnistrend}
translate D OprepResults {Ergebnis nach Längen und Häufigkeiten}
translate D OprepLength {Partielänge}
translate D OprepFrequency {Häufigkeit}
translate D OprepWWins {Weißsiege:    }
translate D OprepBWins {Schwarzsiege: }
translate D OprepDraws {Remis:        }
translate D OprepWholeDB {ganze Datenbank}
translate D OprepShortest {Kürzester Sieg}
translate D OprepMovesThemes {Züge und Themen}
translate D OprepMoveOrders {Zugfolgen zum Erreichen\nder Berichtsposition}
translate D OprepMoveOrdersOne \
  {Es gab nur eine Zugfolge zur erreichten Position:}
translate D OprepMoveOrdersAll \
  {Es gab %u Zugfolgen zur erreichten Position:}
translate D OprepMoveOrdersMany \
  {Es gab  %u Zugfolgen zur erreichten Position. Die ersten %u sind:}
translate D OprepMovesFrom {Züge ab der Berichtsposition}
translate D OprepMostFrequentEcoCodes {Häufigste ECO-Codes}
translate D OprepThemes {Themen}
translate D OprepThemeDescription {Häufigkeit der Themen in den ersten %u Zügen jeder Partie}
translate D OprepThemeSameCastling {Gleichseitige Rochaden}
translate D OprepThemeOppCastling {Verschiedenseitige Rochaden}
translate D OprepThemeNoCastling {Beide Seiten unrochiert}
translate D OprepThemeKPawnStorm {Bauernsturm auf König}
translate D OprepThemeQueenswap {Damen getauscht}
translate D OprepThemeWIQP {Weißer isolierter Damenbauer}
translate D OprepThemeBIQP {Schwarzer isolierter Damenbauer}
translate D OprepThemeWP567 {Weißer Bauer auf Reihe 5/6/7}
translate D OprepThemeBP234 {Schwarzer Bauer auf Reihe 2/3/4}
translate D OprepThemeOpenCDE {Offene c/d/e-Linie}
translate D OprepTheme1BishopPair {Eine Seite hat Läuferpaar}
translate D OprepEndgames {Endspiele}
translate D OprepReportGames {Berichtspartien}
translate D OprepAllGames {Alle Partien}
translate D OprepEndClass {Materialklassifikation von Endspielstellungen}
translate D OprepTheoryTable {Theorietabelle}
translate D OprepTableComment {Erzeugt aus %u Partien mit höchster ELO-Zahl.}
translate D OprepExtraMoves {Zusätzliche Züge in der Theorietabelle}
translate D OprepMaxGames {Maximale Partien in der Theorietabelle}
translate D OprepViewHTML {Zeige HTML}
translate D OprepViewLaTeX {Zeige LaTeX}

# Player Report:
translate D PReportTitle {Spielerbericht}
translate D PReportColorWhite {mit den weißen Steinen}
translate D PReportColorBlack {mit den schwarzen Steinen}
translate D PReportMoves {nach %s}
translate D PReportOpenings {Eröffnungen}
translate D PReportClipbase {Leere Zwischenablage und kopiere gefundene Spiele}

# Piece Tracker window:
translate D TrackerSelectSingle {Linke Maustaste wählt diese Figur.}
translate D TrackerSelectPair \
  {Linke Maustaste wählt diese Figur; rechte Maustaste wählt das Figurenpaar.}
translate D TrackerSelectPawn \
  {Linke Maustaste wählt diesen Bauern; rechte Maustaste wählt alle 8 Bauern.}
translate D TrackerStat {Statistik}
translate D TrackerGames {% der Partien mit Zug auf das Feld}
translate D TrackerTime {% der Zeit auf jedem Feld}
translate D TrackerMoves {Züge}
translate D TrackerMovesStart \
  {Zugnummer, ab der die Verteilungsberechnung beginnen soll.}
translate D TrackerMovesStop \
  {Zugnummer, wo die Verteilungsberechnung enden soll.}

# Game selection dialogs:
translate D SelectAllGames {Alle Partien in der Datenbank}
translate D SelectFilterGames {Nur Partien im Filter}
translate D SelectTournamentGames {Nur Partien des aktuellen Turniers}
translate D SelectOlderGames {Nur ältere Partien}

# Delete Twins window:
translate D TwinsNote {Damit zwei Spiele Dubletten sind, müssen diese mindestens die beiden selben Spieler haben und die folgenden Kriterien, die Sie auswählen können, erfüllen. Wenn zwei Dubletten gefunden werden, so wird die kürzere der beiden Spiele gelöscht. Tip: am besten führt man erst eine Schreibkorrektur durch, da dadurch das Finden von Dubletten verbessert wird.}
translate D TwinsCriteria {Kriterium: Dubletten müssen haben ...}
translate D TwinsWhich {Überprüfe Partien}
translate D TwinsColors {gleiche Spielerfarbe}
translate D TwinsEvent {gleiches Ereignis}
translate D TwinsSite {gleichen Ort}
translate D TwinsRound {gleiche Runde}
translate D TwinsYear {gleiches Jahr}
translate D TwinsMonth {gleichen Monat}
translate D TwinsDay {gleichen Tag}
translate D TwinsResult {gleiches Ergebnis}
translate D TwinsECO {gleichen ECO-Code}
translate D TwinsMoves {gleiche Züge}
translate D TwinsPlayers {Vergleiche Spielernamen}
translate D TwinsPlayersExact {Exakte Treffer}
translate D TwinsPlayersPrefix {Nur erste 4 Buchstaben}
translate D TwinsWhen {Beim Löschen der Dubletten}
translate D TwinsSkipShort {Partien unter 5 Zügen ignorieren}
translate D TwinsUndelete {Zuerst alle Partien entlöschen}
translate D TwinsSetFilter {Filter auf Dubletten setzen}
translate D TwinsComments {Spiele mit Kommentar immer behalten}
translate D TwinsVars {Spiele mit Varianten immer behalten}
translate D TwinsDeleteWhich {Lösche Partie}
translate D TwinsDeleteShorter {Kürzere Partie}
translate D TwinsDeleteOlder {Kleinere Partienummer}
translate D TwinsDeleteNewer {Größere Partienummer}
translate D TwinsDelete {Lösche Spiele}

# Name editor window:
translate D NameEditType {Namen ändern von}
translate D NameEditSelect {Welche Spiele sollen geändert werden?}
translate D NameEditReplace {Ersetze}
translate D NameEditWith {durch}
translate D NameEditMatches {Vorschlag mit Strg+1 - Strg+9 auswählen}
translate D CheckGames {Konsistenzprüfung}
translate D CheckGamesWhich {Konsistenzprüfung}
translate D CheckAll {Alle Partien}
translate D CheckSelectFilterGames {Partien im Filter}

# Classify window:
translate D Classify {Klassifiziere}
translate D ClassifyWhich {Partien ECO-klassifizieren}
translate D ClassifyAll {Alle Spiele (überschreibe alte ECO-Codes)}
translate D ClassifyYear {Alle Spiele  aus dem letzten Jahr}
translate D ClassifyMonth {Alle Spiele aus dem letzten Monat}
translate D ClassifyNew {Nur Spiele ohne ECO-Code}
translate D ClassifyCodes {ECO-Codes verwenden}
translate D ClassifyBasic {Standard Codes ("B12", ...)}
translate D ClassifyExtended {mit Scid-Erweiterung ("B12j", ...)}

# Compaction:
translate D NameFile {Namendatenbank}
translate D GameFile {Partiendatenbank}
translate D Names {Namen}
translate D Unused {Unbenutzt}
translate D SizeKb {Größe (kB)}
translate D CurrentState {Momentaner Stand}
translate D AfterCompaction {nach Kompression}
translate D CompactNames {Komprimiere Namen}
translate D CompactGames {Komprimiere Partien}
translate D NoUnusedNames "Es gibt keine unbenutzen Namen, die Namensdatei ist vollständig komprimiert."
translate D NoUnusedGames "Die Partiedatei ist vollständig komprimiert."
translate D GameFileCompacted {Die Partiedatei der Datenbank wurde komprimiert.}

# Sorting:
translate D SortCriteria {Kriterium}
translate D AddCriteria {Füge Sortierkriterium hinzu}
translate D CommonSorts {Übliche Sortierkriterien}
translate D Sort {Sortiere}

# Exporting:
translate D AddToExistingFile {Anhängen an eine bestehende Datei}
translate D ExportComments {Kommentare exportieren}
translate D ExportVariations {Varianten exportieren}
translate D IndentComments {Kommentare einrücken}
translate D IndentVariations {Varianten einrücken}
translate D ExportColumnStyle {Tabellarisch: ein Zug pro Zeile}
translate D ExportSymbolStyle {Symbolische Notation }
translate D ExportStripMarks \
  {Felder-/Pfeilemarkierzeichen\naus den Kommentaren entfernen}

# Goto game/move dialogs:
translate D LoadGameNumber {Geben Sie die zu ladende Spielnr. ein:}
translate D GotoMoveNumber {Gehe zu Zugnr.:}

# Copy games dialog:
translate D CopyGames {Kopiere Spiele}
translate D CopyConfirm {
 Möchten sie wirklich die [::utils::thousands $nGamesToCopy]
 Spiele aus dem Filter
 in der Datenbank "$fromName"
 in die Datenbank "$targetName"
 kopieren?
}
translate D CopyErr {Kann Spiele nicht kopieren}
translate D CopyErrSource {Die Quelldatenbank}
translate D CopyErrTarget {Die Zieldatenbank}
translate D CopyErrNoGames {hat keine Spiele im Filter}
translate D CopyErrReadOnly {ist schreibgeschützt}
translate D CopyErrNotOpen {ist nicht geöffnet}

# Colors:
translate D LightSquares {Helle Felder}
translate D DarkSquares {Dunkle Felder}
translate D SelectedSquares {Ausgewählte Felder}
translate D SuggestedSquares {Zugvorschlagsfelder}
translate D WhitePieces {Weiße Steine}
translate D BlackPieces {Schwarze Steine}
translate D WhiteBorder {Weiße Umrandung}
translate D BlackBorder {Schwarze Umrandung}

# Novelty window:
translate D FindNovelty {Finde Neuerung}
translate D Novelty {Neuerung}
translate D NoveltyInterrupt {Neuerungensuche abgebrochen}
translate D NoveltyNone {In dieser Partie wurde keine Neuerung gefunden}
translate D NoveltyHelp {Scid wird den ersten Zug aus der aktuellen Partie finden, welcher zu einer Position führt, die nicht in der gewählten Datenbank oder in dem ECO-Eröffnungsbuch enthalten ist.}

# Sounds configuration:
translate D SoundsFolder {Verzeichnis für Sprachausgabedateien}
translate D SoundsFolderHelp {Das Verzeichnis sollte enthalten: King.wav, a.wav, 1.wav etc.}
translate D SoundsAnnounceOptions {Optionen für Zugankündigung}
translate D SoundsAnnounceNew {Kündige neue Züge an, wenn sie ausgeführt werden}
translate D SoundsAnnounceForward {Künde Zug an beim Vorwärtspielen}
translate D SoundsAnnounceBack {Kündige Zug an beim Zurückgehen}
translate D SoundsSoundDisabled {Scid hat das Snack Audio Paket beim Start nicht gefunden.\nTon ist abgeschaltet.}

# Upgrading databases:
translate D Upgrading {Upgrading}
translate D ConfirmOpenNew {
Dies ist eine Datenbank im alten (Scid 3.x) Format, die nicht in Scid 4.x
geöffnet werden kann. Aber eine Version im neuen Format wurde schon erstellt.

Wollen Sie die Version der Datenbank im neuen Format öffnen?
}
translate D ConfirmUpgrade {
Dies ist eine Datenbank im alten (Scid 3.x) Format. Vor der Verwendung in
Scid 4 muß eine Version im neuen Format der Datenbank erstellt werden.

Beim Erstellen der neuen Version der Datenbank bleiben die Dateien der alten Version erhalten.

Dieser Vorgang kann eine Zeitlang dauern, muß aber nur einmal durchgeführt
werden. Sie können jederzeit abbrechen, wenn es Ihnen zu lange dauert.

Soll das Erstellen der Datenbank im neuen Format jetzt durchgeführt werden?
}

# Recent files options:
translate D RecentFilesMenu {Anzahl letzter Dateien im Dateimenü}
translate D RecentFilesExtra {Anzahl letzter Dateien im Untermenü}

# My Player Names options:
translate D MyPlayerNamesDescription {
Geben Sie unten eine Liste der bevorzugten Spielernamen ein, ein Name pro Zeile. Platzhalterzeichen (z.B. "?" für ein beliebiges einzelnes Zeichen, "*" für jede beliebige Folge von Zeichen) sind erlaubt.

Jedesmal, wenn ein Spiel mit einem aufgelisteten Spielernamen geladen wird, wird das Schachbrett im Hauptfenster erforderlichenfalls gedreht, um das Spiel aus der Sicht des betreffenden Spielers zu zeigen.
}
translate D showblunderexists {Enginefehler anzeigen}
translate D showblundervalue {Fehlerbewertung anzeigen}
translate D showscore {Bewertung anzeigen}
translate D coachgame {Coach Partie}
translate D configurecoachgame {Trainingspartie konfigurieren}
translate D configuregame {Spiel konfigurieren}
translate D Phalanxengine {Phalanx engine}
translate D Coachengine {Coach Engine}
translate D difficulty {Schwierigkeit}
translate D hard {schwer}
translate D easy {leicht}
translate D Playwith {Spiel mit}
translate D white {Weiß}
translate D black {Schwarz}
translate D both {beide}
translate D Play {Spielen}
translate D Noblunder {Kein Fehler}
translate D blunder {Fehler}
translate D Noinfo {-- Keine Info --}
translate D PhalanxOrTogaMissing {Phalanx oder Toga nicht gefunden!}
translate D moveblunderthreshold {fehlerhafter Zug, wenn Verlust größer als}
translate D limitanalysis {Analysezeit der Engine begrenzen}
translate D seconds {Sekunden}
translate D Abort {Abbrechen}
translate D Resume {Fortfahren}
translate D OutOfOpening {Ende der Eröffnung}
translate D NotFollowedLine {You did not follow the line}
translate D DoYouWantContinue {Möchten sie fortfahren?}
translate D CoachIsWatching {Coach schaut zu}
translate D Ponder {Berechnen im Hintergrund}
translate D LimitELO {Stärke begrenzen (ELO)}
translate D DubiousMovePlayedTakeBack {Zweifelhafter Zug gespielt, wollen Sie ihn zurücknehmen?}
translate D WeakMovePlayedTakeBack {Dies ist kein guter Zug, wollen Sie ihn zurücknehmen?}
translate D BadMovePlayedTakeBack {Dies ist ein schlechter Zug, wollen Sie ihn zurücknehmen?}
translate D Iresign {Ich gebe auf}
translate D yourmoveisnotgood {Ihr Zug ist nicht gut}
translate D EndOfVar {Variantenende}
translate D Openingtrainer {Eröffnungstrainer}
translate D DisplayCM {Kandidatenzüge anzeigen}
translate D DisplayCMValue {Wert der Kandidatenzüge anzeigen}
translate D DisplayOpeningStats {Statistik anzeigen}
translate D ShowReport {Bericht anzeigen}
translate D NumberOfGoodMovesPlayed {gute Züge gespielt}
translate D NumberOfDubiousMovesPlayed {zweischneidige Züge gespielt}
translate D NumberOfMovesPlayedNotInRepertoire {Züge außerhalb des Repertoirs gespielt}
translate D NumberOfTimesPositionEncountered {Wiederholungen der Position}
translate D PlayerBestMove  {Nur beste Züge erlauben}
translate D OpponentBestMove {Gegner spielt besten Zug}
translate D OnlyFlaggedLines {Nur markierte Linien}
translate D resetStats {Statistik zurücksetzen}
translate D Repertoiretrainingconfiguration {Konfiguration Trainingsrepertoire}
translate D Loadingrepertoire {Lade Repertoire}
translate D Movesloaded {Züge geladen}
translate D Repertoirenotfound {Repertoire nicht gefunden}
translate D Openfirstrepertoirewithtype {Zuerst eine Eröffnungsdatenbank mit entsprechendem Type/Icon öffnen.}
translate D Movenotinrepertoire {Zug nicht im Repertoire}
translate D PositionsInRepertoire {Positionen im Repertoire}
translate D PositionsNotPlayed {nicht gespielte Positionen}
translate D PositionsPlayed {gespielte Positionen}
translate D Success {Erfolgreich}
translate D DubiousMoves {Zweifelhafte Züge}
translate D OutOfRepertoire {Nicht im Repertoire}
translate D ConfigureTactics {Taktik konfigurieren}
translate D ResetScores {"Gelöst" zurücksetzten}
translate D LoadingBase {Lade Datenbank}
translate D Tactics {Taktik}
translate D ShowSolution {Lösung zeigen}
translate D NextExercise {Nächste Aufgabe}
translate D PrevExercise {Vorherige Aufgabe}
translate D StopTraining {Training beenden}
translate D Next {Nächste}
translate D ResettingScore {"Gelöst" zurücksetzen}
translate D LoadingGame {Lade Partie}
translate D MateFound {Matt gefunden}
translate D BestSolutionNotFound {Beste Lösung NICHT gefunden!}
translate D MateNotFound {Matt nicht gefunden}
translate D ShorterMateExists {Kürzeres Matt existiert}
translate D ScorePlayed {Bewertung gepielt}
translate D Expected {erwarted}
translate D ChooseTrainingBase {Trainingsdatenbank auswählen}
translate D Thinking {Denke...}
translate D AnalyzeDone {Analyse beendet}
translate D WinWonGame {Gewinne gewonnene Partie}
translate D Lines {Variantenzahl}
translate D ConfigureUCIengine {UCI Engine konfigurieren}
translate D SpecificOpening {Ausgewählte Eröffnung}
translate D StartNewGame {Neue Partie}
translate D FixedLevel {Feste Stärke}
translate D Opening {Eröffnung}
translate D RandomLevel {Zufallsstärke}
translate D StartFromCurrentPosition {Von aktueller Position starten}
translate D FixedDepth {Feste Tiefe}
translate D Nodes {Knoten} 
translate D Depth {Tiefe}
translate D Time {Zeit} 
translate D SecondsPerMove {Sekunden pro Zug}
translate D Engine {Engine}
translate D TimeMode {Zeitmodus}
translate D TimeBonus {Zeit+\nBonus}
translate D TimeMin {min}
translate D TimeSec {s}
translate D AllExercisesDone {Alle Übungen gemacht}
translate D MoveOutOfBook {Zug nicht mehr im Buch}
translate D LastBookMove {Letzter Buchzug}
translate D AnnotateSeveralGames {Von aktueller Partie bis Partie:}
translate D FindOpeningErrors {Eröffnungsfehler finden}
translate D MarkTacticalExercises {Taktische Übungen markieren}
translate D UseBook {Buch benutzen}
translate D MultiPV {Multivariantenmodus}
translate D Hash {Hash Speicher}
translate D OwnBook {Engine Buch verwenden}
translate D BookFile {Eröffnungsbuch}
translate D AnnotateVariations {Varianten kommentieren}
translate D ShortAnnotations {Kurze Kommentare}
translate D addAnnotatorTag {Kommentar Tag hinzufügen}
translate D AddScoreToShortAnnotations {Bewertung hinzufügen}
translate D Export {Export}
translate D BookPartiallyLoaded {Buch teilweise geladen}
translate D Calvar {Training: Variantenberechnung}
translate D ConfigureCalvar {Konfiguration}
translate D Reti {Reti}
translate D English {Englische Eröffnung}
translate D d4Nf6Miscellaneous {1.d4 Nf6 Verschiedene}
translate D Trompowsky {Trompowsky}
translate D Budapest {Budapest}
translate D OldIndian {Altindische Verteidigung}
translate D BenkoGambit {Benko Gambit}
translate D ModernBenoni {Moderne/Benoni-Verteidigung}
translate D DutchDefence {Holländische Verteidigung}
translate D Scandinavian {Skandinavische Verteidigung}
translate D AlekhineDefence {Aljechin Verteidigung}
translate D Pirc {Pirc-Verteidigung}
translate D CaroKann {Caro-Kann}
translate D CaroKannAdvance {Caro-Kann Vorstoßvariante}
translate D Sicilian {Sizilianisch}
translate D SicilianAlapin {Sizilianisch, Alapin Variante}
translate D SicilianClosed {Geschlossene Sizilianische Verteidigung}
translate D SicilianRauzer {Sizilianisch, Rauzer Angriff}
translate D SicilianDragon {Sizilianisch, Drachenvariante}
translate D SicilianScheveningen {Sizilianisch, Scheveningen}
translate D SicilianNajdorf {Sizilianisch, Najdorf}
translate D OpenGame {Offene Spiele}
translate D Vienna {Wiener Verteidigung}
translate D KingsGambit {Königsgambit}
translate D RussianGame {Russische Verteidigung}
translate D ItalianTwoKnights {Italienische Eröffnung}
translate D Spanish {Spanisch Partie}
translate D SpanishExchange {Spanisch, Abtauschvariante}
translate D SpanishOpen {Spanisch, offene Systeme}
translate D SpanishClosed {Spanisch, geschlossene Systeme}
translate D FrenchDefence {Französische Verteidigung}
translate D FrenchAdvance {Französisch, Vorstoßvariante}
translate D FrenchTarrasch {Französisch, Tarrasch}
translate D FrenchWinawer {Französisch, Winawer}
translate D FrenchExchange {Französisch, Abtauschvariante}
translate D QueensPawn {Damenbauernspiel}
translate D Slav {Slavisch}
translate D QGA {Angenommenes Damengambit}
translate D QGD {Abgelehntes Damengambit}
translate D QGDExchange {Abgelehntes Damengambit, Abtauschvariante}
translate D SemiSlav {Semi-Slawisch}
translate D QGDwithBg5 {Abgelehntes Damengambit mit Lg5}
translate D QGDOrthodox {Abgelehntes Damengambit, Orthodoxe Variante}
translate D Grunfeld {Grünfeld-Verteidigung}
translate D GrunfeldExchange {Grünfeld, Abtauschvariante}
translate D GrunfeldRussian {Grünfeld-Verteidigung, Russische Variante}
translate D Catalan {Katalanische Verteidigung}
translate D CatalanOpen {Katalanisch, offen}
translate D CatalanClosed {Katalanisch, geschlossen}
translate D QueensIndian {Dameninsche Verteidigung}
translate D NimzoIndian {Nimzoindische Verteidigung}
translate D NimzoIndianClassical {Nimzoindisch, Klassische Variante}
translate D NimzoIndianRubinstein {Nimzoinsisch, Rubinstein}
translate D KingsIndian {Königsindische Verteidigung}
translate D KingsIndianSamisch {Königsindisch, Sämisch}
translate D KingsIndianMainLine {Königsindisch, Hauptvariante}
translate D ConfigureFics {FICS Konfigurieren}
translate D FICSGuest {Gastlogin}
translate D FICSServerPort {Serverport}
translate D FICSServerAddress {IP Adresse}
translate D FICSRefresh {Aktualisieren}
translate D FICSTimesealPort {Timeseal Port}
translate D FICSSilence {Konsolenfilter}
translate D FICSOffers {Herausforderungen}
translate D FICSConsole {Konsole}
translate D FICSGames {laufende Partien}
translate D FICSUnobserve {Beobachtung abbrechen}
translate D FICSProfile {Eigenes Profil ausgeben}
translate D FICSRelayedGames {Übertragene Partien}
translate D FICSFindOpponent {Gegner suchen}
translate D FICSTakeback {Zurücknehmen}
translate D FICSTakeback2 {2 zurücknehmen}
translate D FICSInitTime {Zeit (min)}
translate D FICSIncrement {Inkrement (s)}
translate D FICSRatedGame {Wertungspartie}
translate D FICSAutoColour {automatisch}
translate D FICSManualConfirm {manuell bestätigen}
translate D FICSFilterFormula {Filterformel anwenden}
translate D FICSIssueSeek {Partie anbieten}
translate D FICSChallenge {Herausfordern}
translate D FICSAccept {Annehmen}
translate D FICSDecline {Ablehnen}
translate D FICSColour {Farbe auswählen}
translate D FICSSend {senden}
translate D FICSConnect {Verbinden}
translate D FICSdefaultuservars {Standardvariablen verwenden}
translate D FICSObserveconfirm {Bestätige Beobachtung der Partie}
translate D FICSpremove {Premoves aktivieren}
translate D GameReview {Partieanalyse}
translate D GameReviewTimeExtended {Lange Bedenkzeit}
translate D GameReviewMargin {Fehlergrenze}
translate D GameReviewAutoContinue {Bei korrektem Zug Automatisch weiter}
translate D GameReviewReCalculate {Verwende lange Bedenkzeit}
translate D GameReviewAnalyzingMovePlayedDuringTheGame {Analysiere den Partiezug}
translate D GameReviewAnalyzingThePosition {Analysiere Stellung}
translate D GameReviewEnterYourMove {Zug eingeben}
translate D GameReviewCheckingYourMove {Überprüfe Antwort}
translate D GameReviewYourMoveWasAnalyzed {Antwort wurde analysiert}
translate D GameReviewYouPlayedSameMove {Identischer Zug zur Partie}
translate D GameReviewScoreOfYourMove {Wertung des Antwortzuges}
translate D GameReviewGameMoveScore {Wertung des Partiezuges}
translate D GameReviewEngineScore {Bewertung der Engine}
translate D GameReviewYouPlayedLikeTheEngine {Antworzug wie Engine}
translate D GameReviewNotEngineMoveButGoodMove {Nicht der Computerzug, aber ebenfalls ein guter Zug!}
translate D GameReviewMoveNotGood {Dieser Zug ist nicht gut. Bewertung:}
translate D GameReviewMovesPlayedLike {Züge gespielt wie Partie}
translate D GameReviewMovesPlayedEngine {Züge wie Computer gespielt}

# Correspondence Chess Dialogs:
translate D CCDlgConfigureWindowTitle {Einstellungen für Fernschach}
translate D CCDlgCGeneraloptions {Allgemeine Einstellungen}
translate D CCDlgDefaultDB {Standarddatenbank:}
translate D CCDlgInbox {Posteingang (Pfad):}
translate D CCDlgOutbox {Postausgang (Pfad):}
translate D CCDlgXfcc {Xfcc Einstellungen:}
translate D CCDlgExternalProtocol {Externe Protokoll\nHandler (Xfcc)}
translate D CCDlgFetchTool {Programm zum\nEmpfangen:}
translate D CCDlgSendTool {Programm zum\nSenden:}
translate D CCDlgEmailCommunication {eMail Kommunikation}
translate D CCDlgMailPrg {eMail Programm:}
translate D CCDlgBCCAddr {Adresse für\nAusgangskopie:}
translate D CCDlgMailerMode {Modus:}
translate D CCDlgThunderbirdEg {z.B. Thunderbird, Mozilla Mail, Icedove...}
translate D CCDlgMailUrlEg {z.B. Evolution}
translate D CCDlgClawsEg {z.B. Sylpheed Claws}
translate D CCDlgmailxEg {z.B. mailx, mutt, nail...}
translate D CCDlgAttachementPar {Parameter für\nAnhang:}
translate D CCDlgInternalXfcc {Interne Xfcc-Unterstützung verwenden}
translate D CCDlgConfirmXfcc {Züge bestätigen}
translate D CCDlgSubjectPar {Parameter für\nBetreff:}
translate D CCDlgDeleteBoxes {Leeren des Postein- und Ausgangs}
translate D CCDlgDeleteBoxesText {Wollen Sie wirklich Ihren Postein- und Ausgang leeren?\nDies erfordert ein erneutes Synchronisieren zum den aktuellen Zustand Ihrer Partien anzuzeigen.}
translate D CCDlgConfirmMove {Zug bestätigen}
translate D CCDlgConfirmMoveText {Durch bestätigen wird folgender Zug und Kommentar an den Server übertragen:}
translate D CCDlgDBGameToLong {Inkonsistente Hauptvariante}
translate D CCDlgDBGameToLongError {Die Hauptvariante dieser Partie in der Datenbank enthält mehr Züge als in der Partie tatsächlich gespielt. Sofern die Partie im Posteingang vollständig und aktuell ist (z. B. direkt nach einer Synchronisation) muß die Hauptvariante in der Datenbank ensprechend gekürzt werden. Der letzte Zug in der Partie ist Nr. \n}
translate D CCDlgStartEmail {Neue eMail Partie}
translate D CCDlgYourName {Ihr Name:}
translate D CCDlgYourMail {Ihre eMail Adresse:}
translate D CCDlgOpponentName {Name des Gegners:}
translate D CCDlgOpponentMail {eMail Adresse des Gegners:}
translate D CCDlgGameID {Partiekennung (eineindeutig):}
translate D CCDlgTitNoOutbox {Scid: Fernschach Postausgang}
translate D CCDlgTitNoInbox {Scid: Fernschach Posteingang}
translate D CCDlgTitNoGames {Scid: Keine Fernschachpartien}
translate D CCErrInboxDir {Der Posteingang für Fernschachpartien unter:}
translate D CCErrOutboxDir {Der Postausgang für Fernschachpartien unter:}
translate D CCErrDirNotUsable {existiert nicht oder ist nicht benutzbar!\nBitte überprüfen und die Einstellungen korrigieren.}
translate D CCErrNoGames {enthält keine Partien!\nBitte sychronisieren Sie zunächst.}

translate D CCDlgTitNoCCDB {Scid: Keine Fernschach-Datenbank}
translate D CCErrNoCCDB {Es wurde keine Datenbank vom Typ 'Fernschach' geöffnet. Bitte öffen Sie eine solche bevor Sie Funktionen des Fernschachmoduls benutzen.}
translate D CCFetchBtn {Partien vom Server abholen\nund Inbox bearbeiten}
translate D CCPrevBtn {Vorhergehende Partie}
translate D CCNextBtn {Nächste Partie}
translate D CCSendBtn {Zug versenden}
translate D CCEmptyBtn {Postein- und ausgang leeren}
translate D CCHelpBtn {Hilfe zu den Icons und Statusindikatoren\nFür allgemeine Hilfe bitte F1!}
translate D CCDlgServerName {Server Name:}
translate D CCDlgLoginName  {Login Name:}
translate D CCDlgPassword   {Passwort:}
translate D CCDlgShowPassword {Passwort anzeigen}
translate D CCDlgURL        {Xfcc-URL:}
translate D CCDlgRatingType {Wertungszahl:}
translate D CCDlgDuplicateGame {Nichteindeutige Partie}
translate D CCDlgDuplicateGameError {Diese Partie existiert mehr als einmal in der Datenbank. Bitte löschen Sie alle Doubletten und komprimieren Sie die Datenbank (Datei/Wartung/Datenbank komprimieren).}
translate D CCDlgSortOption {Sortierung:}
translate D CCDlgListOnlyOwnMove {Partien mit anhängigen Zügen}
translate D CCOrderClassicTxt {Ort, Turnier, Runde, Ergebnis, Weiß, Schwarz}
translate D CCOrderMyTimeTxt {Eigene Bedenkzeit}
translate D CCOrderTimePerMoveTxt {Zeit pro Zug bis zur nächsten Zeitkontrolle}
translate D CCOrderStartDate {Startdatum}
translate D CCOrderOppTimeTxt {Gegnerische Bedenkzeit}
translate D CCDlgConfigRelay {Partien auf iccf-webchess beobachten}
translate D CCDlgConfigRelayHelp {Besuchen Sie http://www.iccf-webchess.com und lassen Sie die Partie anzeigen, die Sie beobachten wollen. Wenn das Brett sichtbar ist bitte die Adresse aus dem Browser in untenstehende Liste kopieren. Nur eine URL pro Zeile!\nBeispiel: http://www.iccf-webchess.com/MakeAMove.aspx?id=266452}
translate D ExtHWConfigConnection {Hardware Konfigurieren}
translate D ExtHWPort {Schnittstelle}
translate D ExtHWEngineCmd {Engine Kommando}
translate D ExtHWEngineParam {Engine Parameter}
translate D ExtHWShowButton {Knopf in Werkzeugleiste anzeigen}
translate D ExtHWHardware {Hardware}
translate D ExtHWNovag {Novag Citrine}
translate D ExtHWInputEngine {Input Engine}
translate D ExtHWNoBoard {Kein Brett verbunden}
translate D NovagReferee {Nur Züge aufzeichnen}
translate D IEConsole {Input Engine Konsole}
translate D IESending {Sende Züge für}
translate D IESynchronise {Synchronisieren}
translate D IERotate  {Brett drehen}
translate D IEUnableToStart {Input Engine konnte nicht gestartet werden:}
translate D DoneWithPosition {Positionsbearbeitung beendet}
translate D Board {Brett}
translate D showGameInfo {Partieinformation anzeigen}
translate D autoResizeBoard {Brettgröße automatisch berechnen}
translate D DockTop {Nach oben}
translate D DockBottom {Nach unten}
translate D DockLeft {Nach links}
translate D DockRight {Nach rechts}
translate D Undock {Fenster lösen}
#Gamelist
translate D ChangeIcon {Datenbank Icon ändern...}
translate D NewGameListWindow {Neue Partieliste}
translate D LoadatStartup {Beim Starten laden}
translate D ShowHideDB {Datenbank anzeigen/verbergen}
translate D ChangeFilter {Filter ändern}
translate D ChangeLayout {Sortierkriterien und Spaltenlayout laden/speichern/ändern }
translate D ShowHideStatistic {Statistik anzeigen/verbergen}
translate D BoardFilter {Nur Partien, die gleich der aktuellen Brettstellung sind, anzeigen}
translate D CopyGameTo {Kopiere Partie nach}
translate D FindBar {Suchleiste}
translate D FindCurrentGame {Aktuelle Partie finden}
translate D DeleteGame {Partie löschen}
translate D UndeleteGame {Partie wiederherstellen}
translate D ResetSort {Sortierung zurücksetzten}

translate D ConvertNullMove {Null-Züge in Kommentare umwandeln}
translate D SetupBoard {Stellung eingeben}
translate D Rotate {Drehen}
translate D SwitchColors {Farbe wechseln}
translate D FlipBoard {Brett drehen}
translate D ImportPGN {Importiere PGN Partieen}
translate D ImportingFiles {Importiere PGN Dateien nach}
translate D ImportingFrom {Importiere von}
translate D ImportingIn {Importiere Partien nach}
translate D UseLastTag {Letzte Tags\nverwenden}
translate D Random {Zufällig}
translate D BackToMainline {Zurück zur Hauptvariante}
translate D LeaveVariant {Variante verlassen}
translate D Autoplay {Vorspielen}
translate D ShowHideCoords {Beschriftung ein/aus}
translate D ShowHideMaterial {Material ein/aus}
translate D FullScreen {Vollbild}
translate D FilterStatistic {Filter Statistik}
translate D MakeCorrections {Korrigieren}
translate D Surnames {Nachnamen}
translate D Ambiguous {Mehrdeutig}
}
#Preferences Dialog
translate D OptionsToolbar "Werkzeugleiste"
translate D OptionsBoard "Schachbrett"
translate D OptionsMenuColor "Menüfarben"
translate D OptionsBoardSize "Brettgröße"
translate D OptionsBoardPieces "Figurensatz"
translate D OptionsInternationalization "Sprachumgebung"
translate D MenuColorForeground "Text"
translate D MenuColorBackground "Hintergrund"
translate D MenuColorSelect "Auswahl"
translate D OptionsTablebaseDir "Bis zu 4 Verzeichnisse der Endspieltabellen auswählen:"

##########
#
# ECO Opening name translations:

translateECO D {
  Accelerated {, Beschleunigt}
  {: Accelerated} Beschleunigt
  Accepted {, Angenommen}
  {: Accepted} Angenommen
  Advance Vorstoß
  {as Black} {mit Schwarz}
  Attack Angriff
  Bishop Läufer
  Bishop's Läufer
  Classical Klassisch
  Closed Geschlossen
  {Closed System} {Geschlossenes System}
  Counterattack Gegenangriff
  Countergambit Gegengambit
  Declined {, Abgelehnt}
  Defence Verteidigung
  deferred verzögert
  Deferred {, verzögert}
  Early Frühe
  Exchange Abtausch
  Game Partie
  Improved verbessert
  King's Königs
  Knight Springer
  Line Variante
  {Main Line} Hauptvariante
  Open Offen
  Opening Eröffnung
  Queen's Damen
  Queenswap Damentausch
  Symmetrical Symmetrisch
  Variation Variante
  Wing Flügel
  with mit
  without ohne

  Alekhine Aljechin
  Averbakh Awerbach
  Botvinnik Botwinnik
  Chigorin Tschigorin
  Polugaevsky Polugajewski
  Rauzer Rauser
  Sveshnikov Sweschnikow

  Austrian Österreichisch
  Berlin Berliner
  Bremen Bremer
  Catalan Katalanisch
  Czech Tschechisch
  Dutch Holländisch
  English Englisch
  French Französisch
  Hungarian Ungarisch
  Indian Indisch
  Italian Italienisch
  Latvian Lettisch
  Meran Meraner
  Moscow Moskau
  Polish Polnisch
  Prague Prager
  Russian Russisch
  Scandinavian Skandinavisch
  Scheveningen Scheveninger
  Scotch Schottisch
  Sicilian Sizilianisch
  Slav Slawisch
  Spanish Spanisch
  Swedish Schwedisch
  Swiss Schweizer
  Vienna Wiener
  Yugoslav Jugoslawisch

  {Accelerated Fianchetto} {Beschleunigtes Fianchetto}
  {Accelerated Pterodactyl} {Beschleunigter Pterodactylus}
  {Alekhine Defence} Aljechin-Verteidigung
  {Alekhine Variation} Aljechin-Variante
  {Alekhine: Sämisch Attack} {Aljechin: Wiener System (Sämisch-Angriff)}
  {Anderssen Opening} Anderssen-Eröffnung
  {Anti-King's Indian} Anti-Königsindisch
  {Austrian Attack} {Österreichischer Angriff}
  {Benko Gambit} Wolga-Gambit
  {Benko Opening} Benkö-Eröffnung
  {Berlin Defence} {Berliner Verteidigung}
  Chameleon Chamäleon
  Chelyabinsk Tscheljabinsk
  {Classical Defence} {Klassische Verteidigung}
  {Spanish: Classical Defence} {Spanisch: Klassische (Cordel-) Verteidigung}
  {Classical Exchange} {Klassischer Abtausch}
  {Classical Variation} {Klassische Variante}
  {Closed Berlin} {Geschlossener Berliner}
  {Open Berlin} {Offener Berliner}
  {Bird's,} {Bird,}
  {Bird's Defence} Bird-Verteidigung
  {Bird's Deferred} {Verzögerte Bird}
  {Bishop's Opening} Läuferspiel
  {Botvinnik System} Botwinnik-System
  {Central Variation} Zentralvariante
  {Centre Attack} Zentrumsangriff
  {Centre Game} Mittelgambit
  {Danish Gambit} {Nordisches Gambit}
  Dragon Drachen
  {Dutch Variation} {Holländische Variante}
  {Early Exchange} {Früher Abtausch}
  {Early Queenswap} {Früher Damentausch}
  {English Attack} {Englischer Angriff}
  {English: King's} {Englisch: Königsbauer}
  {English Variation} {Englische Variante}
  {Englund Gambit} Englund-Gambit
  {Exchange Variation} Abtauschvariante
  {Fianchetto Variation} Fianchettovariante
  {Flohr Variation} Flohr-Variante
  {Four Knights} Vierspringer
  {Four Knights Game} Vierspringerspiel
  {Four Pawns} Vierbauern
  {Four Pawns Attack} Vierbauernangriff
  {French Variation} {Französische Variante}
  {From Gambit} {Froms Gambit}
  {Goring Gambit} {Göring-Gambit}
  {Grob Gambit} {Grobs Gambit}
  {Hungarian Defence} {Ungarische Verteidigung}
  {Indian Variation} {Indische Variante}
  {Italian Game} {Italienische Partie}
  KGD {Abgel. Königsgambit}
  {Classical KGD} {Klassisches abgelehntes Königsgambit}
  {Keres Variation} Keres-Variante
  KGA {Angen. Königsgambit}
  {KGA: Bishop's Gambit} Königsläufergambit
  {KGA: King's Knight Gambit} Königsspringergambit
  {King's Gambit} Königsgambit
  {King's Gambit Accepted} {Angen. Königsgambit}
  {King's Gambit Accepted (KGA)} {Angen. Königsgambit}
  {King's Indian} Königsindisch
  KIA {Königsindischer Angriff}
  {King's Knight Gambit} Königsspringergambit
  {King's Pawn} Königsbauer
  {Lasker Variation} {Lasker-Variante}
  {Latvian Gambit} {Lettisches Gambit}
  {Maroczy Bind} {Maroczy-Aufbau}
  {Marshall Variation} Marshall-Variante
  {Modern Attack} {Moderner Angriff}
  {Modern Steinitz} {Moderne Steinitz}
  {Modern Variation} {Moderne Variante}
  {Moscow Variation} {Moskauer Variante}
  Nimzo-Indian Nimzoindisch
  {Old Benoni} {Klassisches Benoni}
  {Old Indian} Altindisch
  {Old Indian Attack} {Altindisch i.A.}
  {Old Steinitz} Steinitz-Verteidigung
  {Open Game} {Offene Partie}
  {Poisoned Pawn} {Vergifteter Bauer}
  {Polish Variation} {Polnische Variante}
  {Polugaevsky Variation} {Polugajewski-Variante}
  {Queen's Gambit} Damengambit
  {Queen's Gambit Accepted} {Angen. Damengambit}
  QGA {Angen. Damengambit}
  {Queen's Gambit Accepted (QGA)} {Angenommenes Damengambit}
  {Reversed QGA} {Angen. Damengambit i.A.}
  QGD {Abgel. Damengambit}
  {Queen's Gambit Declined (QGD)} {Abgelehntes Damengambit}
  {Reversed QGD} {Abgel. Damengambit i.A.}
  {Queen's Indian} Damenindisch
  {Queen's Pawn} Damenbauer
  {Queen's Pawn Game} Damenbauerspiel
  {Reversed Slav} {Slawisch i.A.}
  {Rubinstein Variation} Rubinstein-Variante
  {Russian Game} {Russische Partie}
  {Russian Game (Petroff Defence)} {Russische Partie}
  {Russian-Three Knights Game} {Russisches Dreispringerspiel}
  {Scandinavian (Centre Counter)} Skandinavisch
  Schliemann Jänisch
  {Schliemann (Jänisch)} {Jänisch-Gambit (Schliemann)}
  {Scotch Opening} {Schottische Eröffnung}
  {Sicilian Defence} {Sizilianische Verteidigung}
  {Sicilian Variation} {Sizilianische Variante}
  {Slav Defence} {Slawische Verteidigung}
  Smith-Morra Morra
  {Smith-Morra Accepted} {Angenommenes Morra-Gambit}
  {Smith-Morra Gambit} Morra-Gambit
  {Spanish (Ruy Lopez)} {Spanische Partie}
  {Start position} Ausgangsstellung
  {Steinitz Deferred} Rubinstein-Aufbau
  {Swedish Variation} {Schwedische Variante}
  {Swiss Variation} {Schweizer Variante}
  {Tarrasch's Gambit} {Tarrasch-Gambit}
  {Three Knights} Dreispringer
  {3 Knights} Dreispringer
  {Three Knights Game} Dreispringerspiel
  {Three Pawns Attack} Dreibauernangriff
  {Two Knights} Zweispringer
  {Two Knights Defence} Zweispringer-Verteidigung
  {Two Knights Variation} Zweispringer-Variante
  {Two Pawns} Zweibauern
  {Two Pawns Attack} Zweibauernangriff
  {Wing Gambit} Flügel-Gambit
  {Yugoslav Attack} {Jugoslawischer Angriff}
}


############################################################
#
# German help pages:


##########
# Contents
#
set helpTitle(D,Contents) "Inhaltsverzeichnis"
set helpText(D,Contents) {<h1>Inhaltsverzeichnis Scid-Hilfe</h1>


<h4>Start und allgemeine Hilfe</h4>
<ul>
<li><a Guide><b>Kurzanleitung</b> Benutzung von Scid</a> <red>(Zuerst lesen!)</red></li>
<li><a Hints><b>Hinweise</b> zum effizienteren Arbeiten mit Scid</a></li>
<li><a MainWindow>Das Scid-<b>Hauptfenster</b></a></li>
<li><a Menus>Die Scid-<b>Menüs</b></a></li>
<li><a Moves><b>Züge</b> eingeben</a></li>
<li><a Searches><b>Suchen</b> in Scid</a></li>
<li><a Clipbase>Die <b>Ablage</b>-Datenbank</a></li>
<li><a Annotating>Partien <b>kommentieren</b></a> </li>
</ul>

<h4>Andere Scid-Fenster</h4>
<ul>
<li><a Analysis><b>Analyse</b>-Fenster</a></li>
<li><a Reports><b>Berichte</b></a> </li>
<li><a Book><b>Buch</b>-Fenster</a></li>
<li><a Finder><b>Dateisuche</b>-Fenster</a></li>
<li><a Switcher><b>Datenbank-Umschalter</b></a></li>
<li><a Email><b>E-Mail</b>-Schach-Manager</a></li>
<li><a TB><b>Endspieltabellen</b> in Scid verwenden</a> </li>
<li><a PTracker><b>Figurenverteilung</b></a></li>
<li><a Graphs><b>Grafik</b>-Fenster</a></li>
<li><a Comment><b>Kommentareditor</b></a></li>
<li><a Crosstable><b>Kreuztabellen</b>-Fenster</a></li>
<li><a GameList><b>Partieliste</b>-Fenster</a></li>
<li><a Import><b>Partie-Import</b>-Fenster</a></li>
<li><a PGN><b>PGN</b> (Partietext)-Fenster</a></li>
<li><a Repertoire><b>Repertoire-Editor</b></a></li>
<li><a PList><b>Spielersuche</b></a></li>
<li><a PInfo><b>Spieler-Information</b></a></li>
<li><a TacticalGame><b>Trainingspartie</b>-Fenster</a></li>
<li><a SeriousGame><b>Ernste Partie</b></a></li>
<li><a CalVar><b>Training Varianten berechnen</b>-Fenster</a></li>
<li><a OpeningTrainer><b>Eröffnungstrainer</b>-Fenster</a></li>
<li><a Tmt><b>Turniersuche</b></a></li>
<li><a Tree><b>Zugbaum</b>-Fenster</a></red></li>
</ul>

<h4>Weitere Hilfsprogramme und Informationen</h4>
<ul>
<li><a Maintenance><b>Datenbank-Wartung</b></a></li>
<li><a Sorting>Datenbank<b> sortieren</b></a></li>
<li><a Compact>Datenbank<b> komprimieren</b></a></li>
<li><a ECO><b>ECO</b>-Eröffnungsklassifikation</a></li>
<li><a EPD><b>EPD</b>-Dateien</a></li>
<li><a Correspondence><b>Fernschach</b></a></li>
<li><a Export>Partien in Textdateien <b>exportieren</b></a></li>
<li><a Cmdline><b>Kommandozeilen</b>-Optionen</a></li>
<li><a LaTeX>Scid und <b>LaTeX</b></a> </li>
<li><a Bookmarks><b>Lesezeichen</b></a></li>
<li><a Flags>Partie-<b>Kennzeichen</b> (Flags)</a></red></li>
<li><a Options><b>Optionen</b> und Präferenzen</a></li>
<li><a Pgnscid><b>Pgnscid</b>: PGN-Dateien konvertieren</a></li>
<li><a NAGs><b>NAG</b>-Standard-Kommentarwerte</a></li>
<li><a Formats>Scids Datenbank-<b>Dateiformate</b></a></li>
<li><a Author>Kontakt-Information</a></li>
</ul>

<p><footer>(Aktualisiert: Scid 4.7, August 2018)</footer></p>
}


###############
### Topic Index

set helpTitle(D,Index) "Hilfe-Index"
set helpText(D,Index) {<h1>Scid: Hilfe-Index</h1>

<h3>A</h3>
<ul>
<li><a Clipbase>Ablage-Datenbank</a></li>
<li><a Analysis List>Analyse-Engines, Liste der</a></li>
<li><a Analysis>Analysefenster</a></li>
<li><a MainWindow Autoplay>Automatisches Vorspielen</a></li>
<li><a Author>Autor, Kontaktaufnahme</a></li>
</ul>

<h3>B</h3>
<ul>
<li><a Menus Edit>Bearbeiten-Menü</a></li>
<li><a Maintenance Cleaner>Bereiniger</a></li>
<li><a Tree Best>Beste Partien</a></li>
<li><a Graphs Rating>Bewertungsgraph</a></li>
<li><a GameList Browsing>Blättern in Partien</a></li>
<li><a Searches Board>Brettsuche</a></li>
</ul>

<h3>D</h3>
<ul>
<li><a Finder>Dateisuche</a></li>
<li><a Formats>Dateiformate</a></li>
<li><a Menus File>Datei-Menü</a></li>
<li><a Formats>Datenbank-Dateiformate</a></li>
<li><a Compact>Datenbank-Komprimierung</a></li>
<li><a Sorting>Datenbank sortieren</a></li>
<li><a Switcher>Datenbank-Umschalter</a></li>
<li><a Maintenance>Datenbank-Wartung</a></li>
<li><a Maintenance Twins>Dubletten (doppelte Partien)</a></li>
</ul>

<h3>E</h3>
<ul>
<li><a ECO Browser>ECO-Auswertung</a></li>
<li><a ECO Codes>ECO-Codesystem</a></li>
<li><a ECO>ECO-Eröffnungsklassifikation</a></li>
<li><a Email>E-Mail</a></li>
<li><a CCeMailChess>Email Schach</a></li>
<li><a TB>Endspieltabellen</a></li>
<li><a EPD>EPD-Dateien</a></li>
<li><a SeriousGame>Ernste Partie spielen</a></li>
<li><a Reports Opening>Eröffnungsbericht</a></li>
<li><a ECO>Eröffnungsklassifizierung (ECO)</a></li>
<li><a Repertoire>Eröffnungsrepertoire</a></li>
<li><a Export>Exportieren, Partien in Textdateien</a></li>
</ul>

<h3>F</h3>
<ul>
<li><a Menus Windows>Fenster-Menü</a></li>
<li><a Correspondence><b>Fernschach</b></a></li>
<li><a CCIcons><b>Fernschach Statusanzeigen</b></a></li>
<li><a PTracker>Figurenverteilungs-Fenster</a></li>
<li><a Searches Filter>Filter</a></li>
<li><a Export>Filter, exportieren</a></li>
<li><a Graphs Filter>Filtergrafik</a></li>
</ul>

<h3>G</h3>
<ul>
<li><a Graphs>Grafikfenster</a></li>
</ul>

<h3>H</h3>
<ul>
<li><a MainWindow>Hauptfenster</a></li>
<li><a Menus Help>Hilfe-Menü</a></li>
<li><a Hints>Hinweise</a></li>
</ul>

<h3>I</h3>
<ul>
<li><a Import>Importieren</a></li>
<li><a Contents>Inhaltsverzeichnis</a></li>
</ul>

<h3>K</h3>
<ul>
<li><a Cmdline>Kommandozeilen-Optionen</a></li>
<li><a Comment>Kommentareditor</a></li>
<li><a NAGs>Kommentarzeichen</a></li>
<li><a Annotating>Kommentieren</a></li>
<li><a Compact>Komprimieren (Datenbank)</a></li>
<li><a Author>Kontaktinformation</a></li>
<li><a Crosstable>Kreuztabellen</a></li>
<li><a Guide>Kurzanleitung für Scid</a></li>
</ul>

<h3>L</h3>
<ul>
<li><a LaTeX>LaTeX</a>-Ausgabeformat</li>
<li><a Bookmarks>Lesezeichen</a></li>
<li><a Maintenance Twins>Löschen, Dubletten</a></li>
</ul>

<h3>M</h3>
<ul>
<li><a Flags>Kennzeichen</a></li>
<li><a Searches Material>Material-/Muster-Suche</a></li>
<li><a Menus>Menüs</a></li>
</ul>

<h3>N</h3>
<ul>
<li><a NAGs>NAG-Kommentarwerte</a></li>
<li><a Maintenance Editing>Namen bearbeiten</a></li>
<li><a Maintenance Spellcheck>Namen, Schreibweisen</a></li>
<li><a Annotating Null>Nullzüge</a></li>
</ul>

<h3>O</h3>
<ul>
<li><a Options>Optionen</a></li>
<li><a Appearance>Oberfläche anpassen</a></li>
</ul>

<h3>P</h3>
<ul>
<li><a Searches Header>Partiedaten-Suche</a></li>
<li><a GameList>Partielisten</a></li>
<li><a Flags>Partie-Kennzeichen</a></li>
<li><a Menus Game>Partie-Menü</a></li>
<li><a PGN>PGN-Fenster</a></li>
<li><a Pgnscid>Pgnscid</a></li>
</ul>

<h3>R</h3>
<ul>
<li><a Repertoire>Repertoire-Editor</a></li>
</ul>

<h3>S</h3>
<ul>
<li><a Moves>Schachzüge eingeben</a></li>
<li><a Maintenance Spellcheck>Schreibkorrektur von Namen</a></li>
<li><a Sorting>Sortieren einer Datenbank</a></li>
<li><a Reports Player>Spielerbericht</a></li>
<li><a PInfo>Spielerinformation</a></li>
<li><a PList>Spielersuche</a></li>
<li><a CCIcons><b>Statusanzeigen im Fernschachfenster</b></a></li>
<li><a Searches>Suchen</a></li>
<li><a Menus Search>Suchen-Menü</a></li>
<li><a Searches Filter>Such-Filter</a></li>
</ul>

<h3>T</h3>
<ul>
<li><a Moves Trial>Testmodus</a></li>
<li><a Tmt>Turniersuche</a></li>
<li>Training</li>
<ul>
<li><a OpeningTrainer>Eröffnungstrainer</a></li>
<li><a CalVar>Varianten berechnen</a></li>
</ul>
</ul>

<h3>V</h3>
<ul>
<li><a Annotating Vars>Varianten</a></li>
</ul>

<h3>W</h3>
<ul>
<li><a Maintenance>Wartung</a></li>
<li><a Menus Tools>Werkzeug-Menü</a></li>
</ul>

<h3>X</h3>
<ul>
<li><a CCXfcc>Xfcc-Unterstützung</a></li>
</ul>

<h3>Z</h3>
<ul>
<li><a Options Fonts>Zeichensätze</a></li>
<li><a Tree>Zugbaumfenster</a></li>
<li><a Moves>Züge eingeben</a></li>
<li><a GameList Browsing>Zusammenführen, Partien</a></li>
</ul>

<p><footer>(Aktualisiert: Scid 3.6.23, März 2008)</footer></p>
}


###############
### Quick guide

set helpTitle(D,Guide) "Kurzanleitung zur Benutzung von Scid"
set helpText(D,Guide) {<h1>Kurzanleitung zur Benutzung von Scid</h1>
<p>
Scid ist eine Schachdatenbank, mit welcher man Partiesammlungen
nach einer Vielzahl von Kriterien durchsuchen kann, Partien editieren
und mit Hilfe von Winboard- und UCI-kompatiblen Schachprogrammen
analysieren kann.
</p>
<p>
Scid benutzt ein eigenes, spezielles Drei-Dateien-Datenbankformat,
welches sehr schnell und kompakt ist, aber es kann auch das PGN-
(Portable Game Notation) Format lesen und schreiben. Scids
<a PGN>PGN-Fenster</a> zeigt die Notation der aktuellen Partie im
PGN-Format.
</p>
<p>
Man kann in Scid Partien zur Datenbank hinzufügen, mit Hilfe
der Tastatur oder der Maus. Mehr Details unter der Hilfeseite für
<a Moves>Zugeingabe</a>.
</p>
<p>
Man kann mit Scid auch <a PGN>PGN</a> - Partiesammlungen bearbeiten:
durch Import der PGN-Datei in Scids - <a Import>Import</a> Fenster
oder direkt durch öffnen von PGN-Dateien in Scid. Jedoch: PGN-Dateien
bennötigen mehr Speicher und werden langsamer geladen. Für große PGN - Partiesammlungen
wird empfohlen, sie mit Hilfe des Scid-Programms <a Pgnscid>pgnscid</a>
in eine Scid-Datenbank umzuwandeln (dabei bleibt die PGN-Datei erhalten).
Die Bearbeitung von PGN-Datei ist eingeschränkt. Man kann Partien hinzufügen
und löschen, aber keine Partien überschreiben.
</p>
<p>
Das <a MainWindow>Hauptfenster</a> von Scid (mit dem grafischen
Schachbrett) zeigt Details der aktuellen Partie und Datenbank. Man
kann zu gleicher Zeit bis zu vier Datenbanken geöffnet haben (fünf
inklusive der <a Clipbase>Ablage-Datenbank</a>), und jede hat eine
eigene aktuelle Partie. (Die Partie mit der Nr. 0 dient zu Eingabe
einer neuen Partie und ist nicht Bestandteil der Datenbank).
Man kann zwischen den offenen Datenbanken im
<a Menus File>Datei-Menü</a> umschalten.
</p>
<p>
Für zusätzliche Information lesen Sie bitte die anderen
Hilfe-Seiten, die im <a Index>Index</a> aufgeführt sind.
</p>
<p>
<a Author>Kontaktinformationen über den Autor von Scid</a> findet man in
der entsprechenden Hilfeseite.
</p>

<p><footer>(Aktualisiert: Scid 4.7, August 2018)</footer></p>
}


###############
### Hints page:

set helpTitle(D,Hints) "Scid Hinweise"
set helpText(D,Hints) {<h1>Scid Hinweise</h1>
<p>
Diese Seite enthält einige nützliche Hinweise in Form von Fragen und
Antworten, um Ihnen zu helfen, Scids Fähigkeiten besser zu nutzen.
Wenn Sie das erste Mal mit Scid arbeiten, lesen Sie bitte zuerst die
<a Guide>Kurzanleitung</a> zur Benutzung von Scid.
Die meisten Informationen auf dieser Seite finden Sie detailliert auf
den anderen Hilfeseiten, die im <a Index>Hilfe-Index</a>
aufgeführt sind.
Falls Sie einen nützlichen Hinweis haben, der dieser Seite hinzugefügt
werden sollte, senden Sie ihn bitte (in Englisch) an den
<a Author>Autor von Scid</a>.
</p>

<h4>Kann Scid eine Datenbank automatisch beim Start laden?</h4>
<p>
Ja, Sie können Datenbanken, PGN-Dateien oder <a EPD>EPD-Dateien</a>
in der Kommandozeile angeben. Beispiel:
<ul>
<li> <b>scid  datenbank  partien.pgn.gz</b></li>
</ul>
lädt die Scid-Datenbank <b>datenbank</b> und gleichzeitig
die mit gzip komprimierte PGN-Datei <b>partien.pgn.gz</b>.
</p>

<h4>Gibt es eine leichtere Möglichkeit, die Brettgröße
zu ändern, als über das Optionen-Menü?</h4>
<p>
Ja, Sie können mit den Tastenkombinationen <b>Control+Shift+LeftArrow</b>
und <b>Control+Shift+RightArrow</b> das Brett verkleinern bzw. vergrößern.
</p>

<h4>Wenn ich trainiere, indem ich eine Partie nachspiele, möchte ich
nicht, daß Scid den nächsten Zug im Informationsbereich anzeigt.
Kann ich ihn verstecken?</h4>
<p>
Sie können den nächsten Zug verstecken, indem Sie die <b>rechte</b>
Maustaste im Informationsbereich drücken und im darauf erscheinenden
Menü <b>Verstecke nächsten Zug</b> auswählen.
</p>

<h4>Wie kann ich den ECO-Eröffnungsschlüssel der aktuellen Position
ermitteln?</h4>
<p>
Der ECO-Schlüssel wird in der letzten Zeile des Informationsbereichs
angezeigt, unterhalb des Schachbretts im <a MainWindow>Hauptfenster</a>,
wenn Sie die ECO-Datei (<b>scid.eco</b>) geladen haben.<br>
Auf der <a ECO>ECO-Code</a> Hilfeseite wird erklärt, wie man die ECO-Datei
lädt und die Optionen so einstellt, daß sie jedesmal beim Programmstart
geladen wird.
</p>

<h4>Beim Eingeben einer Partie bin ich gerade beim 30. Zug, als ich
bemerke, daß der 10. Zug falsch eingegeben wurde. Wie kann ich ihn
korrigieren und dabei alle nachfolgenden Züge erhalten?</h4>
<p>
Sie können das im <a Import>Import</a>-Fenster tun; siehe die Hilfeseite
<a Moves Mistakes>Zugeingabe</a> für weitere Informationen.
</p>

<h4>Wie kopiere ich Partien von einer Datenbank in die andere?</h4>
<p>
Im <a Switcher>Datenbank-Umschalterfenster</a>: kopieren Sie alle
Dateien im Datenbank-<a Searches Filter>Filter</a>, indem Sie sie mit
der Maus von einer Datenbank in die andere ziehen.
</p>

<h4>Jedesmal, wenn ich einen bereits vorhandenen Zug eingebe, erhalte
ich ein "Zug ersetzen?"-Dialogfenster. Wie kann ich das verhindern?</h4>
<p>
Schalten Sie es aus mit der Option <b>Fragen vor Ersetzen</b> im Menü
<menu>Optionen: Züge</menu>.
Oder gewöhnen Sie sich an, Züge mit der rechten Maustaste zurückzunehmen,
die jeweils den letzen Zug der Partie entfernt, wenn Sie sich
am Partiende befinden.
</p>

<h4>Wie ändere ich die Spaltenbreite im Partielistenfenster?</h4>
<p>
Klicken Sie die jeweilige Spaltenüberschrift mit der linken oder
rechten Maustaste an und wählen Sie <b>Konfigurieren</b>.
</p>

<h4>Wie kann ich das Zugbaumfenster statt für die ganze Datenbank nur
für eine Auswahl von Partien nutzen?</h4>
<p>
Nutzen Sie die <a Clipbase>Ablage-Datenbank</a>. Wählen Sie den
Datenbank-Filter so, daß er die von Ihnen gewünschten Partien enthält,
dann kopieren Sie diese über den <a Switcher>Datenbank-Umschalter</a>
in die Ablage-Datenbank. Danach brauchen Sie nur noch das Zugbaumfenster
in der Ablage-Datenbank zu öffnen.
</p>

<h4>Der Zugbaum ist bei großen Datenbanken zu langsam. Wie kann ich
ihn beschleunigen?</h4>
<p>
Sichern Sie die Cache-Datei häufig, um die Zugbaum-Resultate zum
späteren Gebrauch zu bewahren. Zu den Details siehe den Abschnitt
<b>Caching</b> auf der <a Tree>Zugbaum</a>-Hilfeseite.
</p>

<h4>Wie kann ich direkt die PGN-Notation der Datei bearbeiten?</h4>
<p>
Im <a PGN>PGN</a>-Fenster können sie die aktuelle Partie nicht bearbeiten,
aber Sie können immer noch die PGN-Notation editieren, indem Sie das
Fenster <a Import>Partie importieren</a> verwenden.
Öffnen Sie es (mit <b>Control+Shift+I</b>) und wählen Sie
<b>Aktuelle Partie einfügen</b>, dann bearbeiten Sie die Partie
und klicken danach <b>Importieren</b> an.

</p>

<h4>In meiner Datenbank sind einige Spielernamen unterschiedlich
geschrieben. Wie kann ich sie korrigieren?</h4>
<p>
Mit den Kommandos im Menü <menu>Datei: Wartung</menu> können Sie einzelne
Namen editieren oder auch Schreibweisen aller Namen überprüfen.
Siehe die Seite <a Maintenance Editing>Wartung</a>.
</p>

<h4>Ich habe zwei Datenbanken geöffnet: eine mit meinen eigenen Partien
und eine große Datenbank mit Großmeisterpartien. Wie kann ich eine
meiner Partien mit denen in der großen Datenbank vergleichen?</h4>
<p>
Öffnen Sie das <a Tree>Zugbaumfenster</a> in der großen Datenbank und
und wechseln Sie mit Hilfe des Datenbankwechslers in die zu
vergleichende Datenbank.
</p>

<p><footer>(Aktualisiert: Scid 3.6.28, August 2008)</footer></p>
}


###############
### Main window

set helpTitle(D,MainWindow) "Scid Hauptfenster"
set helpText(D,MainWindow) {<h1>Scid Hauptfenster</h1>
<p>
Das Hauptfenster in Scid zeigt die aktuelle Stellung der jeweiligen
Partie und Information über diese Partie und die Datenbank.
Separate Hilfeseiten beschreiben die <a Menus>Menüs</a> und die
<a Moves>Zugeingabe</a>.
</p>

<h3>Symbolleiste: Navigieren durch die Partie</h3>
<p>
Die direkt über dem Schachbrett angebracht Symbole haben die
folgende Bedeutung (von links nach rechts):
<ul>
<li> <button tb_start> Gehe zum Beginn der Partie. </li>
<li> <button tb_prev> Gehe einen Zug zurück. </li>
<li> <button tb_next> Gehe einen Zug vor. </li>
<li> <button tb_end> Gehe zum Ende der Partie. </li>
<li> <button tb_invar> Gehe in Variante. </li>
<li> <button tb_outvar> Verlasse die Variante. </li>
<li> <button tb_addvar> Neue Variante hinzufügen. </li>
<li> <button tb_play> Starte/beende automatisches Vorspielen (siehe unten). </li>
<li> <button tb_trial> Starte/beende <a Moves Trial>Testmodus</a>. </li>
<li> <button tb_flip> Brett drehen. </li>
<li> <button tb_coords> Zeige/Verdecke/Zeige unten und links/ die Brettkoordinaten. </li>
</ul>

<h4><name Autoplay>Automatisches Vorspielen</name></h4>
<p>
Im diesem Modus spielt Scid automatisch die Züge in der
aktuellen Partie vor, bis zum Spielende. Die Zeit zwischen den Zügen
kann im Menü <menu>Optionen: Züge</menu> eingestellt werden und wird
mit "Optionen speichern" gesichert.
</p>
<p>
Die Tastenkombination <b>Strg+Z</b> startet oder stoppt das autom.
Vorspielen, zusätzlich kann das autom. Vorpielen auch durch
Drücken der <b>Esc</b>-Taste beendet werden.
</p>
<p>
Wenn der Modus "Automatisches Vorspielen" beim geöffneten
<a Analysis>Analysefenster</a> (Engine 1) gestartet wird, so wird
die von der Engine berechnte jeweilige Fortsetzung als Variante jedem Zug
hinzugefügt <term>Autom. Kommentieren</term>. Siehe auch die
Hilfeseite zum <a Analysis>Analysefenster</a>.
</p>

<h3>Der Informationsbereich</h3>
<p>
Der Bereich unter dem Schachbrett, mit Informationen über die
Partie, wird <term>Informationsbereich</term> genannt. Die ersten 3
Zeilen zeigen Information zu den Spielern, Ergebnis, ECO-Code, Datum,
Ort und Ereignis. Die vierte Zeile zeigt die Informationen zur
aktuellen Stellung sowie den nächsten Zug.
</p>
<p>
Die fünfte Zeile zeigt den <a ECO>ECO-Code</a> der aktuelle Stellung,
falls enthalten in der gerade benutzen ECO-Datei.
</p>
<p>
Der Informationsbereich hat ein Menü, welches man mit der
rechten Maustaste aktivieren kann. Hierüber kann man bestimmte
Informationen zum Spiel zeigen oder verstecken sowie Kennzeichen
für die aktuelle Partie setzten und löschen. Man kann das
Menü auch über die <b>[F9]</b> Funktionstaste erreichen.
</p>

<h4>Endspieltabellen</h4>
<p>
Der Informationsbereich zeigt auch Ergebnisse aus den
Endspieltabellen zur aktuellen Stellung, wenn diese darin enthalten
ist. Mehr dazu in den Hilfeseiten zu <a TB>Endspieltabellen</a>.
</p>

<h3>Die Statusanzeige</h3>
<p>
Die Statusanzeige zeigt Information über die aktuelle
Datenbank. Das erste Feld zeigt den Partiestatus an: <b>XX</b>
bedeutet, die Partie wurde geändert, aber noch nicht
gespeichert, während <b>--</b> bedeutet, es gibt keine
Änderungen, und <b>%%</b> zeigt an, daß die Datenbank
schreibgeschützt geöffnet wurde.
</p>
<p>
Wenn man eine Datenbank immer schreibgeschützt öffen will,
kann man auch die Rechte für die entsprechenden Scid-Dateien
setzen, oder zumindestens die der Indexdatei; z.B. (unter Linux) durch
den Befehl: <b>chmod a-w datenbank1.si3</b>.
Danach öffnet Scid die entsprechende Datenbank immer
schreibgeschützt.
</p>
<p>
Die Statusanzeige zeigt auch, wieviel Spiele im aktuellen
<a Searches Filter>Filter</a>
sind.
</p>

<p><footer>(Aktualisiert: Scid 3.1, Dezember 2001)</footer></p>
}


######################
### Menus help screen:

set helpTitle(D,Menus) "Menüs"
set helpText(D,Menus) {<h1>Die Scid-Menüs</h1>

<h3><name File>Datei</name></h3>
<ul>
<li><menu>Neu</menu>: Legt eine neue, leere Scid-Datenbank an.</li>
<li><menu>Öffnen</menu>: Öffnet eine existierende Scid-Datenbank.</li>
<li><menu>Schließen</menu>: Schließt die aktive Scid-Datenbank.</li>
<li><menu>Finder</menu>: Öffnet das <a Finder>Dateisuche</a>-Fenster.</li>
<li><menu>Lesezeichen</menu>: Das <a Bookmarks>Lesezeichen</a>-Menü mit
     einigen Lesezeichen-Funktionen.</li>
<li><menu>Wartung</menu>: Datenbank-<a Maintenance>Wartungs</a>-Funktionen.
    </li>
<ul>
  <li><menu>Wartungsfenster</menu>: Öffnet/schließt das
      Datenbank-Wartungsfenster.</li>
  <li><menu>Partien ECO-klassifizieren</menu>: Bestimmt den
      <a ECO>ECO-Code</a> für alle Partien der Datenbank neu.</li>
  <li><menu>Dubletten löschen</menu>: Findet <a Maintenance Twins>Dubletten</a>
      in der Datenbank.</li>
  <li><menu>Namen</menu>: Ersetzt alle Vorkommen eines Spieler-,
      Ereignis-, Ort- oder Rundeneintrags.</li>
</ul>
<li><menu>Schreibschutz</menu>: Markiert die aktive Datenbank als schreibgeschützt.</li>
<li><menu>Datenbank 1/2/3/4/5</menu>: Mit diesen Kommandos können Sie
    zwischen den vier verfügbaren Datenbanken und der
    <a Clipbase>Ablage-Datenbank</a> wechseln.</li>
<li><menu>Ende</menu>: Beendet Scid. </li>
</ul>

<h3><name Edit>Bearbeiten</name></h3>
<ul>
<li><menu>Variante hinzufügen</menu>: Fügt eine neue, leere Variante zum nächsten
    Zug hinzu bzw. zum vorherigen, falls es noch keinen nächsten Zug gibt.</li>
<li><menu>Variante löschen</menu>: Öffnet ein Varianten-Untermenü für
    den aktuellen Zug, um eine Variante zu löschen.</li>
<li><menu>Als erste Variante setzen</menu>: Macht eine Variante zur
    ersten Variante des aktuellen Zuges.</li>
<li><menu>Variante als Hauptvariante setzen</menu>: Tauscht eine Variante
    mit der Partiefolge aus.</li>
<li><menu>Variante testen</menu>: Schaltet in den <a Moves Trial>Testmodus</a>,
    um eine Variante auszuprobieren, ohne die aktuelle Partie zu verändern.</li>
<li><menu>Entfernen</menu>: Entfernt alle Kommentare oder Varianten
    aus der aktuellen Partie.</li>
<br>
<li><menu>Ablage leeren</menu>: Leert die
    <a Clipbase>Ablage</a>-Datenbank.</li>
<li><menu>Partie in Ablage kopieren</menu>: Kopiert die aktuelle Partie
    in die <a Clipbase>Ablage</a>-Datenbank.</li>
<li><menu>Partie aus Ablage einfügen</menu>: Fügt die aktive Partie der
    <a Clipbase>Ablage</a> als aktive Partie in die aktuelle Datenbank
    ein.</li>
<br>
<li><menu>Stellungseingabe</menu>: Legt die Ausgangsstellung für die
    aktuelle Partie fest.</li>
<li><menu>Stellung einfügen</menu>: Fügt den markierten Text
    (Zwischenablage) als Ausgangsstellung ein.</li>
</ul>

<h3><name Game>Partie</name></h3>
<ul>
<li><menu>Neue Partie</menu>: Verwirft alle nicht gesicherten Änderungen
     und beginnt eine neue Partie ab der Grundstellung.</li>
<li><menu>Lade Erste/Vorherige/Nächste/Letzte Partie</menu>: Diese Menüpunkte
    laden die erste, vorherige, nächste bzw. letzte Partie im <a Searches Filter>Filter</a>.</li>
<li><menu>Partie wiederladen</menu>: Lädt die aktuelle Partie erneut, wobei
    alle bisherigen Änderungen verworfen werden.</li>
<li><menu>Lade Partie Nummer</menu>: Lädt die Partie mit der angegebenen
    Nummer in die aktuelle Datenbank.</li>
<br>
<li><menu>Partie ersetzen</menu>: Sichert die aktuelle Partie, wobei sie
    die Originalversion in der Datenbank ersetzt.</li>
<li><menu>Partie speichern</menu>: Sichert die aktuelle Partie als neue
    Partie, die ans Ende der Datenbank angehängt wird.</li>
<br>
<li><menu>Eröffnung identifizieren</menu>: Findet die letztmögliche
    Position der aktuellen Partie in der ECO-Datei.</li>
<li><menu>Zugnummer</menu>: Geht zu der angegebenen Zugnummer in
    der aktuellen Partie.</li>
<li><menu>Finde Neuerung</menu>: Findet den ersten Zug der aktuellen Partie,
    der vorher noch nie gespielt wurde.</li>
</ul>

<h3><name Search>Suchen</name></h3>
<ul>
<li><menu>Filter zurücksetzen</menu>: Setzt den <a Searches Filter>Filter</a>
    zurück, so daß er alle Partien enthält.</li>
<li><menu>Filter negieren</menu>: Invertiert den Filter, so daß er alle
    vorher ausgeschlossenen Partien enthält.</li>
<br>
<li><menu>Brett</menu>: Sucht die
    <a Searches Board>aktuelle Brettposition</a>.</li>
<li><menu>Partiedaten</menu>: Sucht nach <a Searches Header>Partiedaten</a>
    wie z.B. Spielernamen.</li>
<li><menu>Material/Muster</menu>: Sucht nach
    <a Searches Material>Material</a> oder Stellungsmustern</a>.</li>
<br>
<li><menu>Mit Suchoptionsdatei</menu>: Sucht mit Hilfe von
    <a Searches Settings>Einstellungen</a> aus einer Suchoptionsdatei.</li>
</ul>

<h3><name Windows>Fenster</name></h3>
<ul>
<li><menu>Kommentareditor</menu>: Öffnet/schließt das
    <a Comment>Kommentareditor</a>-Fenster.</li>
<li><menu>Partieliste</menu>: Öffnet/schließt das
    <a GameList>Partielistenfenster</a>.</li>
<li><menu>PGN-Fenster</menu>: Öffnet/schließt das
    <a PGN>PGN-Fenster</a>.</li>
<li><menu>Turniersuche</menu>: Öffnet/schließt den
    <a Tmt>Turniersuche</a>.</li>
<br>
<li><menu>Datenbank-Umschalter</menu>: Öffnet/schließt den
    <a Switcher>Datenbank-Umschalter</a>, mit dem man auf einfache Weise zu einer
    anderen Datenbank wechseln oder Partien zwischen Datenbanken kopieren kann.</li>
<li><menu>Wartungsfenster</menu>: Öffnet/schließt das
    Datenbank-<a Maintenance>Wartungs</a>-Fenster.</li>
<br>
<li><menu>ECO-Auswertung</menu>: Öffnet/schließt das Fenster
    <a ECO browser>ECO-Auswertung</a>.</li>
<li><menu>Repertoire-Editor</menu>: Öffnet/schließt den
    <a Repertoire>Repertoire-Editor</a>.</li>
<li><menu>Statistik</menu>: Öffnet/schließt das
    <term>Filter-Statistikfenster</term>, welches eine Gewinn/Verlust-Zusammenstellung
    aller Partien im <a Searches Filter>Filter</a> liefert.</li>
<li><menu>Zugbaum</menu>: Öffnet/schließt das <a Tree>Zugbaumfenster</a>.</li>
<li><menu>Endspieltabellen</menu>: Öffnet/schließt das Fenster, das
    die <a TB>Endspieltabellen</a>-Informationen ausgibt.</li>
</ul>

<h3><name Tools>Werkzeuge</name></h3>
<ul>
<li><menu>Analyse-Engine</menu>: Startet/beendet die "Schachanalyse-Engine",
    die die Auswertung der aktuellen Position
    im <a Analysis>Analysefenster</a> darstellt.</li>
<li><menu>Analyse-Engine #2</menu>: Startet/beendet eine zweite
    "Schachanlyse-Engine".</li>
<li><menu>Kreuztabelle</menu>: Erstellt eine
    Turnier-<a Crosstable>Kreuztabelle</a> für die aktuelle Partie. </li>
<li><menu>E-Mail-Manager</menu>: Öffnet/schließt den <a Email>E-Mail-Manager</a>
    für Fernschachpartien.</li>
<br>
<li><menu>Rel. Filtergrafik</menu>: Öffnet das <a Graphs>Fenster für die Relative Filtergrafik</a></li>
<li><menu>Abs. Filtergrafik</menu>: Öffnet das <a Graphs>Fenster für die Absolute Filtergrafik</a></li>
<li><menu>Eröffnungsbericht</menu>: Erstellt einen <a Reports Opening>Eröffnungsbericht</a>
    zur aktuellen Position.</li>
<li><menu>Figurenverteilung</menu>: Öffnet das Fenster <a PTracker>Figurenverteilung</a>.
    </li>
<li><menu>Spieler-Information</menu>: Gibt <a PInfo>Spieler-Informationen</a>
    für einen Spieler der aktuellen Partie aus.</li>
<li><menu>ELO-Zahl-Verlauf</menu>: gibt den
    <a Graphs Rating>ELO-Zahl-Verlauf</a> aus.</li>
<li><menu>Partie-Bewertungsgraph</menu>: Gibt den
    <a Graphs Score>Partie-Bewertungsgraph</a> aus.</li>
<br>
<li><menu>Partie exportieren</menu>: Schreibt die aktuelle Partie in eine
    Textdatei im PGN-, HTML- oder LaTeX-Format. Siehe die
    <a Export>Export</a>-Hilfeseite.</li>
<li><menu>Alle Partien im Filter exportieren</menu>: Schreibt alle Partien
    im Such-<a Searches Filter>Filter</a> in eine Textdatei im PGN-, HTML-
    oder LaTeX-Format. Siehe die <a Export>Export</a>-Hilfeseite.</li>
<br>
<li><menu>Eine PGN-Partie importieren</menu>: Öffnet das <a Import>Importfenster</a>,
    um den Inhalt einer Partie im <a PGN>PGN-Format</a> direkt
    einzutippen oder einzufügen.</li>
<li><menu>Datei mit PGN-Partien importieren</menu>: Importiert eine ganze
    Datei mit Partien im PGN-Format in die aktuelle Datenbank.</li>
<br>    
<li><menu>Starte Engine1</menu>: Starte direkt die letzte ausgewählte Engine1 im Analysemodus</li>  
<li><menu>Starte Engine2</menu>: Starte direkt die letzte ausgewählte Engine2 im Analysemodus</li>  
</ul>

<h3><name Options>Optionen</name></h3>
<p>
Dieses Menü stellt einige Einträge bereit, um die meisten konfigurierbaren
Optionen von Scid einzustellen.
Der Menüpunkt <menu>Autom. speichern bei Beenden</menu> sichert die aktuellen
Optionen  in der Datei "<b>~/.scid/scidrc</b>" (bzw.
<b>scid.opt</b> im Verzeichnis des ausführbaren Scid-Programms für Windows-User);
diese Datei wird bei jedem Programmstart von Scid geladen.
</p>

<h3>Hilfe</h3>
<p>
Dieses Menü enthält die Hilfe-Funktionen sowie Zugriff auf das
"Tip-des-Tages"-Fenster oder das Startfenster, welches Informationen
über die Dateien liefert, die Scid beim Programmstart geladen hat.
</p>

<p><footer>(Aktualisiert: Scid 3.6.21, Dezember 2007)</footer></p>
}


########################
### Entering moves help:

set helpTitle(D,Moves) "Züge eingeben"
set helpText(D,Moves) {<h1>Schachzüge eingeben</h1>
<p>
In Scid können Sie die Partiezüge mit der Maus oder über die
Tastatur eingeben. Wenn Sie den Mauszeiger auf ein Feld bewegen,
wird dieses und ein weiteres Feld seine Farbe ändern, falls es
einen legalen Zug auf dieses bzw. von diesem Feld gibt. Das ist
der <term>Zugvorschlag</term>. Sie können diesen Zug einfach durch
Klicken der <term>linken</term> Maustaste ausführen. Falls Sie der
Zugvorschlag stört, können Sie ihn im Optionsmenü abschalten.
</p>
<p>
Um einen anderen als den vorgeschlagenen Zug auszuführen, können Sie
die <term>linke</term> Maustaste verwenden: Drücken Sie einfach die
Taste über einem Feld und lassen Sie sie über einem anderen Feld los.
</p>
<p>
Wenn Sie es vorziehen, Züge mit zwei Mausklicks auszuführen statt mit
gedrückter Maustaste, können Sie die <term>mittlere</term> Taste
verwenden: Klicken Sie erst das eine Feld an, danach das andere.
</p>

<h4>Einen Zug zurücknehmen</h4>
<p>
Um einen Zug zurückzunehmen, drücken Sie die rechte Maustaste. Damit gehen
Sie einen Zug zurück; gleichzeitig löschen Sie diesen Zug, falls es der
letzte Zug der Partie oder einer Variante war.
</p>

<h4>Alte Züge ersetzen</h4>
<p>
Wenn Sie einen Zug an einer Stelle in der Partie eingeben, wo bereits
ein Zug existiert, wird Scid Sie in einem Dialogfenster fragen, ob Sie
wirklich den alten Zug ersetzen wollen (der alte Zug und alle nachfolgenden
sind dann verloren) oder statt dessen den neuen Zug als Variante eingeben
wollen. Manche Leute mögen dieses Dialogfenster als störend empfinden
und wollen alte Züge immer ersetzen. Sie können es daher im Menü
<menu>Optionen: Züge</menu> mit der Option "<i>Vor Ersetzen nachfragen</i>"
abschalten.
</p>

<h4><name Trial>Der Testmodus</name></h4>
<p>
Wenn Sie eine Partie nachspielen und eine Position erreichen, wo Sie
eine alternative Variante am Brett ausprobieren wollen, ohne die Partie
zu verändern, wählen Sie <b>Variante testen</b> im Menü <menu>Bearbeiten</menu>,
um den Testmodus einzuschalten. In diesem Modus können Sie Züge eingeben
und die Partie zeitweilig verändern; sobald Sie den Testmodus verlassen,
kehren Sie zur Ausgangsposition zurück.
</p>

<h3><name Mistakes>Fehler korrigieren</name></h3>
<p>
Wenn Sie eine Partie eingeben und plötzlich einige Züge vorher einen
falschen Zug bemerken, können Sie diesen korrigieren, ohne die
nachfolgenden Züge zu verlieren. Der einzige Weg besteht darin, die
PGN-Darstellung der Datei zu bearbeiten: Öffnen Sie das
<a Import>Import</a>-Fenster, wählen Sie "Aktuelle Partie einfügen",
korrigieren Sie den falschen Zug, und wählen Sie dann "Importieren".
</p>

<h3>Zugeingabe über die Tastatur</h3>
<p>
Um Züge über die Tastatur einzugeben, drücken Sie einfach die jeweiligen
Buchstaben- und Zahlentasten. Die Züge sollten in <term>SAN-Notation</term>
<i>ohne</i> die Symbole für Schlagen (x) oder Umwandeln (=) eingegeben
werden. Groß- und Kleinschreibung wird ignoriert, Sie können also
beispielsweise [n][f][3] anstelle von Nf3 eingeben  -- beachten Sie
aber die Anmerkung weiter unten zu den Konflikten bei Bauernzügen.
</p>
<p>
Um sicherzustellen, daß kein Zug in einem anderen enthalten ist,
lautet die Eingabe für die kurze und lange Rochade [0][K] resp. [0][Q]
anstelle der üblichen Notation 0-0 bzw. 0-0-0.
</p>
<p>
Wenn Sie einen Zug eingeben, zeigt die Statusleiste die Liste der
möglichen Züge an. Sie können jederzeit die [Leertaste] drücken, um
den ersten möglichen Zug der Liste als Eingabe auszuwählen.
Um ein Zeichen zu löschen, drücken Sie [Rück] oder [Entf].
</p>
<p>
<b>Beachten</b> Sie, daß ein Kleinbuchstabe zuerst als Bauer
interpretiert wird; mit [b] kann also ein Bauer oder Läufer ("Bishop")
gemeint sein, im Zweifelsfall müssen Sie daher ein großes [B] für den
Läufer verwenden.
</p>

<h4>Automatische Zugergänzung</h4>
<p>
Im Options-Menü können Sie die <term>Automatische Zugergänzung</term>
ein- oder ausschalten.
Mit Zugergänzung wird ein Zug ausgeführt, sobald er durch Ihre Eingabe
von jedem anderen möglichen Zug unterschieden werden kann. Beispielsweise
müßten Sie mit automatischer Zugergänzung in der Ausgangsstellung nur
[n][f] statt <b>Nf3</b> eintippen.
</p>

<h3><name Null>Nullzüge eingeben</name></h3>
<p>
<a Annotating Null>Null</a>- (leere) Züge können in Varianten nützlich
sein, wenn Sie den Zug einer Seite auslassen wollen. Sie können einen
Nullzug mit der Maus eingeben, indem Sie einen König mit dem anderen
schlagen, oder mit der Tastatur, indem Sie "<b>--</b>" eingeben (d.h.
die Minustaste zweimal drücken).
</p>

<h3>Standard-Kommentarzeichen eingeben</h3>
<p>
Sie können die üblichen <a NAGs>Kommentarzeichen</a> im Hauptfenster auch
über die Tastatur ohne den <a Comment>Kommentar-Editor</a> hinzufügen.
Die folgende Liste zeigt Ihnen, welche Zeichen Sie mit welchen Tasten
eingeben können:
<ul>
<li> !  : [!][Eingabe] </li>
<li> ?  : [?][Eingabe] </li>
<li> !? : [!][?][Eingabe] </li>
<li> ?! : [?][!][Eingabe] </li>
<li> !! : [!][!][Eingabe] </li>
<li> ?? : [?][?][Eingabe] </li>
<li> </li>
<li> +- : [+][-] </li>
<li> +/-: [+][/] </li>
<li> += : [+][=] </li>
<li> =  : [=][Eingabe] </li>
<li> -+ : [-][+] </li>
<li> -/+: [-][/] </li>
<li> =+ : [=][+] </li>
</ul>

<p><footer>(Aktualisiert: Scid 3.4, Juli 2002)</footer></p>
}


#########################
### Searches help screen:

set helpTitle(D,Searches) "Suchen"
set helpText(D,Searches) {<h1>Suchen in Scid</h1>
<p>
Scid kann auf viele verschiedene Weisen in einer Datenbank suchen.
Die drei wichtigsten Arten zu suchen sind:
<ul>
<li><b>1)</b> nach der aktuellen Brettposition, </li>
<li><b>2)</b> nach angegebenem Material und Stellungsmustern, </li>
<li><b>3)</b> nach Partiedaten-Informationen wie Spieler, Ergebnis, Datum. </li>
</ul>
<p>
Außerdem gibt es noch einen automatischen Suchmodus, den sog.
<a Tree>Zugbaum</a>, der an anderer Stelle erklärt wird.
</p>

<h3><name Filter>Der Such-Filter</name></h3>
<p>
Suchen basiert bei Scid auf dem Konzept eines <term>Filters</term>.
Der Filter repräsentiert eine Teilmenge der aktuellen Datenbank;
eine Partie ist immer entweder im Filter enthalten oder vom
Filter ausgeschlossen.
</p>
<p>
Bei jeder Art der Suche können Sie wählen, den vorhandenen Filter
weiter einzugrenzen, zu diesem hinzuzufügen oder ihn zu ignorieren
und die gesamte Datenbank zu durchsuchen. Diese Auswahl erlaubt es,
komplexe Suchvorgänge nacheinander aufzubauen.
</p>
<p>
Sie können auch mit dem <a Switcher>Datenbank-Umschalter</a> alle Partien
im Filter einer Datenbank in eine andere Datenbank kopieren.
</p>
<p>
Bei der Suche nach einer exakten Postion, der <a Tree>Zugbaum</a>- oder
Material/Muster-Suche wird die Zugnummer der ersten passenden Position
jeder passenden Partie gespeichert; wenn Sie also jede dieser Partien
laden, wird automatisch die passende Stellung angezeigt.
</p>
<p>
<b>Beachten</b> Sie, daß beim Suchen nur mit den Partiezügen
verglichen wird, nicht mit den Zügen der Varianten.
</p>

<h3><name Board>Suchen: Brett...</name></h3>
<p>
Hier finden Sie Partien, die die gerade dargestellte Position enthalten,
ohne Berücksichtigung der Rochade- und <i>en-passant</i>-Rechte.
</p>
<p>
Vier Suchtypen stehen zur Verfügung. Alle vier verlangen von einer
passenden Stellung exakt gleiches Material und die gleiche Seite am Zug.
Die Typen sind:
<ul>
<li> [1] exakt (die beiden Stellungen müssen auf jedem Feld übereinstimmen),
     </li>
<li> [2] Bauern (die Bauernstruktur muß übereinstimmen, aber andere Figuren
     können irgenwo stehen), </li>
<li> [3] Linien (die Anzahl weißer und schwarzer Bauern auf jeder Linie muß
     übereinstimmen, aber andere Figuren können irgendwo stehen), and </li>
<li> [4] Material (Bauern und Figuren können irgendwo stehen). </li>
</ul>
<p>
Die Bauernsuche ist hilfreich, um Eröffnungen nach Bauernstrukuren zu
studieren, und Linien- und Materialsuche helfen beim Auffinden ähnlicher
Endspielstellungen.
</p>
<p>
Um nach einer beliebigen Stellung zu suchen, können Sie zuerst
die Position aufbauen (im Menü <menu>Bearbeiten: Stellungseingabe</menu>)
und dann die Suche starten.
</p>
<p>
Sie können Varianten (statt nur die jeweiligen Partiezüge) durchsuchen,
indem Sie den Schalter <b>Schaue in Varianten</b> wählen; das kann
aber die Suche stark verlangsamen, falls die Datenbank sehr groß ist
und viele Partien mit Varianten enthält.
</p>

<h3><name Material>Suchen: Material/Muster</name></h3>
<p>
Diese Suche ist nützlich, um End- oder Mittelspielthemen zu finden. Sie
können eine minimale und maximale Anzahl von Figuren jeden Typs angeben,
und Sie können Stellungsmuster wie Läufer auf f7 oder f-Bauer finden.
</p>
<p>
Eine Reihe von häufigen Materialkonstellationen und Stellungsmustern
sind schon bereitgestellt, wie etwa "Endspiel Turm gegen Bauern" oder
"isolierter Damenbauer".
</p>
<p>
<b>Hinweise:</b><br>
Die Geschwindigkeit der Suche nach Stellungsmustern kann stark variieren.
Sie können aber durch eine geschickte Wahl der Bedingungen die benötigte
Zeit reduzieren. Wenn Sie beispielsweise für ein Endspiel die Mindestzahl
von Zügen auf 20 setzen, werden alle Partien, die vor dem 20. Zug enden,
übersprungen.
</p>

<h3><name Header>Suchen: Partiedaten</name></h3>
<p>
Mit dieser Suche können Sie Partiedaten finden, die im Vorspann ("Header")
gespeichert sind (wie z.B. Datum, Ergebnis, Namen und Wertungszahlen),
es müssen daher keine Partiezüge dekodiert werden.
</p>
<p>
Um bei der Suche nach Partiedaten eine passende Partie zu finden,
müssen <b>alle</b> von Ihnen angegebenen Felder übereinstimmen.
</p>
<p>
Die Namensfelder (Weiß, Schwarz, Turnier, Ort und Runde) passen auf
jeden Text im Namen, ohne Berücksichtigung von Groß- und Kleinschreibung
oder Leerzeichen.
</p>
<p>
Sie können für das Weiß-, Schwarz-, Turnier-, Ort- und Rundenfeld mit
Unterscheidung von Groß- und Kleinschreibung und mit "Wildcards" suchen
(mit <b>?</b> für ein und <b>*</b> für null oder mehr Zeichen), indem
Sie den Suchtext in Anführungszeichen setzen. Eine Suche mit der Ortsangabe
<b>USA</b> wird beispielsweise amerikanische Städte finden, aber auch
<b>Lausanne SUI</b>, was vermutlich nicht von Ihnen beabsichtigt war! Eine
Suche mit dem Ort <b>"*USA"</b> (denken Sie an die Anführungszeichen)
liefert dagegen nur Städte in den USA.
</p>
<p>
Wenn Sie einen bestimmten Spieler (oder zwei bestimmte Kontrahenten) als
Weiß oder Schwarz suchen und die jeweilige Farbe keine Rolle spielt,
wählen Sie die Option <b>Farben ignorieren</b>.
</p>
<p>
Schließlich kann die Suche nach Partiedaten auch verwendet werden, um
irgendeinen Text (mit Berücksichtigung von Groß- und Kleinschreibung,
ohne "Wildcards") in der PGN-Darstellung dieser Partie zu finden. Sie
können bis zu drei Textteile eingeben, und sie müssen alle in einer
passenden Partie vorkommen.
Diese Methode ist sehr hilfreich, um in den Kommentaren oder zusätzlichen
Daten einer Partie zu suchen (wie <b>Zeitüberschreitung</b> oder
<b>Kommentator</b>), oder nach einer Zugfolge wie <b>Bxh7+</b> und
<b>Kxh7</b> für ein angenommenes Läuferopfer auf h7.
Allerdings kann diese Art zu suchen <i>sehr</i> langsam sein, da alle
Partien, auf die andere Kriterien zutreffen, dekodiert und auf diese
Texte durchsucht werden müssen.
Es ist daher eine gute Idee, diese Suchen so weit wie möglich einzugrenzen.
Hier sind einige Beispiele:
Um Partien mit einer Unterverwandlung in einen Turm zu finden,
suchen Sie nach <b>=R</b> und setzen gleichzeitig die Kennung
<b>Umwandlung</b> auf Ja.
Wenn Sie nach Text in Kommentaren suchen, setzen Sie die Kennung
<b>Kommentare</b> auf Ja.
Falls Sie nach den Zügen <b>Bxh7+</b> und <b>Kxh7</b> suchen, möchten
Sie z.B. die Suche möglicherweise auf Partien mit dem Ergebnis 1-0 und
mit mindestens 40 Halbzügen beschränken, oder Sie suchen zuerst nach
Material oder Stellungsmustern, um Partien mit einem weißen Läuferzug
nach h7 zu finden.
</p>

<h3><name Settings>Sucheinstellungen sichern</name></h3>
<p>
Die Suchfenster Material/Mustern und Partiedaten besitzen einen Schalter
zum <term>Speichern</term>. Das ermöglicht Ihnen, die aktuellen
Sucheinstellungen zur späteren Verwendung in einer
<term>Suchoptions</term>-Datei zu speichern (Endung .sso).
Um mit Hilfe einer vorher gespeicherten Suchoptionsdatei (.sso) zu
suchen, wählen Sie <menu>Öffnen</menu> im Menü <menu>Suchen</menu>.
</p>

<h3>Suchdauer und übergangene Dateien</h3>
<p>
Die meisten Suchen liefern eine Nachricht, die die benötigte Zeit und
die Anzahl der <term>übergangenen</term> Partien angibt. Eine übergangene
Partie ist eine, die aufgrund der im Index gespeicherten Informationen
von der Suche ausgeschlossen werden kann, ohne daß ihre Züge dekodiert
werden müssen. Siehe die Hilfeseite über
<a Formats>Dateiformate</a> für weitergehende Informationen.
</p>

<p><footer>(Aktualisiert: Scid 3.0, November 2001)</footer></p>
}


############
### Clipbase

set helpTitle(D,Clipbase) "Die Ablage-Datenbank"
set helpText(D,Clipbase) {<h1>Die Ablage-Datenbank</h1>
<p>
Zusätzlich zu den geöffneten Datenbanken stellt Scid eine
<term>Ablage-Datenbank</term> (als Datenbank 5) zur Verfügung, welche
sich wie eine normale Datenbank verhält, mit der Ausnahme, daß sie
nur im RAM-Speicher existiert und keine Dateien zur Speicherung auf
der Festplatte besitzt.
</p>
<p>
Die Ablage-Datenbank (kurz: Ablage) ist nützlich als eine temporäre
Datenbank, zum Zusammenführen von Suchergebnissen aus Suche in
mehreren Datenbanken oder zur Behandlung der Suchergebnisse als eine
eigenständige Datenbank.
</p>
<p>
Angenommen, zum Beispiel, man will sich auf einen Gegner vorbereiten, und
hat eine Datenbank auf alle Partien des Gegners mit Weiß durchsucht,
so daß diese Partien im <a Searches Filter>Filter</a> sind.
Man kann nun diese Partien in die Ablage kopieren (durch Ziehen mit der
rechten Maustaste von der Originaldatenbank zur Ablage, im Fenster
<a Switcher>Datenbank-Umschalter</a>), danach im Datei-Menü die Ablage
als aktive Datenbank auswählen, und dann kann man sich im
<a Tree>Zugbaumfenster</a> das Eröffnungsrepertoire des Gegners
ansehen.
</p>
<p>
Sind 2 Datenbanken geöffnet, und hat man im Filter Partien, die man
von der einen Datenbank in die andere kopieren möchte, kann man dies
direkt (ohne Umweg über die Ablage) tun. Dazu benutzt man wiederum das
Fenster <a Switcher>Datenbank-Umschalter</a>.
</p>
<p>
Die Ablage kann nicht geschlossen werden: mit dem Befehl
<menu>Ablage leeren</menu> aus dem Menü <menu>Bearbeiten</menu> wird
der Inhalt der Ablage gelöscht.
</p>
<p>
Die Ablage-Datenbank ist begrenzt auf 10.000 Partien zur gleichen Zeit,
da sie nur im Speicher existiert.
</p>

<p><footer>(Aktualisiert: Scid 2.5, Juni 2001)</footer></p>
}


#################################
### Variations and comments help:

set helpTitle(D,Annotating) "Partien kommentieren"
set helpText(D,Annotating) {<h1>Partien kommentieren</h1>
<p>
Mit Scid können Sie den Partien Anmerkungen hinzufügen. Es gibt
drei Arten von Anmerkungen, die Sie nach einem Zug eingeben können:
Symbole, ein Kommentar und Varianten.
</p>

<h3>Symbole und Kommentare</h3>
<p>
Symbole verwendet man, um eine Stellungsbeurteilung anzuzeigen
(etwa "+-" or "=") oder um auf gute ("!") und schlechte ("?")
Züge hinzuweisen, während ein Kommentar ein beliebiger Text sein
kann. Zur Eingabe von Symbolen und Kommentaren benutzen Sie den
<a Comment>Kommentar-Editor</a>. Es gibt auch eine Hilfeseite, in
der die <a NAGs>Standard-Symbolwerte</a> aufgelistet sind.
</p>
<p>
Beachten Sie, daß jeder Zug mehr als ein Anmerkungssymbol haben kann,
aber nur einen Kommentar. Ein Kommentar vor dem ersten Partiezug wird
als Text vor dem Beginn der Partie ausgegeben.
</p>

<h3><name Vars>Varianten</name></h3>
<p>
Eine <term>Variante</term> eines Zuges ist eine alternative Zugfolge
an einem bestimmten Punkt der Partie. Varianten können Kommentare
enthalten und sogar rekursiv Untervarianten. Mit den Schaltern oberhalb
des Schachbretts mit einem "<b>V</b>"-Symbol sowie den Kommandos im
Menü <menu>Bearbeiten</menu> kann man Varianten erstellen, editieren
und sich darin bewegen.
</p>

<h4>Tastatureingaben</h4>
<p>
Wenn es zu einem Zug Varianten gibt, werden sie im Informationsbereich
angezeigt. Die erste wird mit <b>v1</b> bezeichnet, die zweite mit
<b>v2</b> etc. Um in eine Variante zu gehen, können Sie sie anklicken
oder "<b>v</b>" gefolgt von der Variantennummer eingeben. (Falls es
nur eine Variante gibt, genügt die Eingabe von <b>v</b>.)
Mit der Taste "<b>z</b>" verlassen Sie die Variante wieder.
</p>

<h3><name Null>Nullzüge</name></h3>
<p>
Gelegentlich kann es in einer Variante nützlich sein, den Zug einer
Seite auszulassen. Beispiel: Sie wollen den Zug 14.Ld3 als Variante
hinzufügen und möchten darauf hinweisen, daß er 15.Lxh7+ Kxh7 16.Sg5+
mit Angriff droht. Das können Sie erreichen, indem Sie im obigen
Beispiel einen <term>Nullzug</term> zwischen 14.Ld3 und 15.Bxh7+
ausführen. Ein Nullzug wird als "<b>--</b>" dargestellt und kann
eingefügt werden, indem Sie mit der Maus den illegalen Zug "König
schlägt König" ausführen oder mit der Tastatur "<b>--</b>" (zwei
Minuszeichen) eingeben.
</p>
<p>
Beachten Sie, daß Nullzüge nicht zum PGN-Standard gehören, wenn Sie
also Partien mit Nullzügen in eine PGN-Datei exportieren, wird Scid
(neben einigen anderen Exportoptionen) eine Option anbieten,
Nullzüge zu erhalten oder sie in Kommentare umzuwandeln, damit
Kompatibilität zu anderen Programmen gewahrt bleibt.
Siehe auch die <a Export>Export</a>-Hilfeseite.
</p>

<p><footer>(Aktualisiert: Scid 3.4, Juli 2002)</footer></p>
}


####################
### Analysis window:

set helpTitle(D,Analysis) "Analysefenster"
set helpText(D,Analysis) {<h1>Das Analysefenster</h1>
<p>
Scids Analysefenster zeigt die Analyse der aktuellen Brettstellung
durch ein Schachprogramm (eine sog. <term>Engine</term>).
Immer, wenn sich die Brettstellung ändert, übermittelt Scid die neue
Position an die Engine, und die zeigt ihre Einschätzung dieser Stellung.
</p>
<p>
Die Bewertung im Analysefenster erfolgt immer aus der Perspektive
von Weiß, eine negative Bewertung heißt also, daß Schwarz besser steht.
Der untere Fensterbereich (mit dem Rollbalken) zeigt den Verlauf der
Auswertungen der aktuellen Stellung durch die Engine, Sie können also
sehen, wie sich die Einschätzung verändert hat.
</p>
<p>
Um den von der Engine ausgewählten besten Zug als neuen Zug der
aktuellen Partie zu nehmen, klicken Sie einfach auf den Schalter
<b>Zug hinzufügen</b>.
</p>
<p>
Tip: Über die Tasten F2 und F3 können direkt die letzten beiden Analyse-Engines gestartet werden, ohne dies nochmal auswählen zu müssen.
</p>

<h3><name List>Die Liste der Analyse-Engines</name></h3>
<p>
Scid behält eine Liste der von Ihnen benutzen Engines mit einer
geschätzten Elo-Zahl (wenn Sie eine Schätzung machen) und dem Datum
der letzten Benutzung. Sie können diese Liste nach Namen, Elo-Zahl
oder Datum sortieren.
Wählen Sie <b>Neu</b> oder <b>Bearbeiten</b>, um eine neue Engine
zur Liste hinzuzufügen oder Details eines bestehenden Eintrags zu
bearbeiten.
</p>

<h3><name Start>Engine-Kommandos und Verzeichnisse</name></h3>
<p>
Für jede Engine müssen Sie die aufzurufende Programmdatei angeben
und das Verzeichnis, in dem Scid sie starten soll.
</p>
<p>
Die häufigste Ursache von Problemen beim Start einer Engine ist die Wahl
des Verzeichnisses, in dem die Engine laufen soll. Einige Engines
benötigen eine Initialisierung oder eine Eröffnungsbuchdatei in ihrem
Startverzeichnis, um ordnungsgemäß zu laufen.
Andere Engines (so wie Crafty) schreiben Logdateien in dem Verzeichnis,
in dem sie gestartet wurden, Sie müssen sie also in einem Verzeichnis
aufrufen, für das Sie Schreibrecht haben.
Wenn für das Verzeichnis einer Engine "." angegeben ist, wird Scid sie
einfach im aktuellen Verzeichnis starten.
</p>
<p>
Wenn also eine Engine, die in Scid laufen sollte, nicht startet,
versuchen Sie es mit anderen Verzeichniseinstellungen. Um zu vermeiden,
daß Engines Logdateien in vielen verschiedenen Verzeichnissen erstellen,
empfehle ich, Engines im Verzeichnis der Scid-Benutzerdateien zu starten
(da, wo sich die Datei <b>scid.exe</b> bei Windows befindet bzw.
<b>~/.scid/</b> bei Unix); in der Dialogbox für die Engine-Einstellungen
gibt es einen Schalter <b>scid.exe dir</b> für Windows bzw. <b>~/.scid</b>
für Unix, mit dem Sie die Engine in diesem Verzeichnis starten lassen
können.
</p>

<h3>Training</h3>
<p>
Mit dem Schalter <b>Training</b> können Sie gegen die Analyse-Engine
spielen. Die Bedenkzeit für jeden Zug ist fest vorgegeben und die
Analyse-Ergebnisse werden nicht angezeigt, wenn der Trainingsmodus
eingeschaltet ist.
</p>

<h3>Partie kommentieren</h3>
<p>
Der Schalter <b>Variante hinzufügen</b> im Analysefenster fügt die
laufende Bewertung und beste Variante als neue Variante der Partie hinzu.
</p>
<p>
Man kann das für mehrere Züge mit dem Schalter <b>Kommentieren</b>
automatisieren. Sie werden zur Eingabe einiger Kommentar-Optionen
aufgefordert, dann wird in den Autoplay-Modus geschaltet.
Wenn der Autoplay-Modus aktiviert und das Analysefenster geöffnet ist,
wird die beste Variante mit Stellungsbewertung zu jeder Position
hinzugefügt, da sich der Autoplay-Modus durch die Partie bewegt.
Nur Stellungen von der aktuellen Stellung bis zum Partieende (oder bis
zum Abbruch des Autoplay-Modus) werden kommentiert, so daß Sie Kommentare
zu Eröffungszügen unterdrücken können, indem Sie vor Aktivierung des
Autoplay-Modus zu einer Mittelspielstellung gehen.
</p>
<p>
Die automatische Kommentierung kann jederzeit durch Ausschalten des
Autoplayer-Modus abgebrochen werden, z.B. durch Drücken der
<b>Esc</b>-Taste im Hauptfenster.
</p>
<p>
Zur Vereinfachung steht der <b>Kommentar</b>-Schalter nur in dem Fenster
zur Verfügung, das als erste Analyse-Engine geöffnet wurde. Wenn Sie
eine Engine als Analyse-Engine Nr.2 öffnen, können Sie sie nicht zur
Partiekommentierung nutzen.
</p>

<h3>Das Analysebrett</h3>
<p>
Anklicken des Schachbrett-Icons in einem Analysefenster zeigt oder
versteckt das Analysebrett, welches die Position nach der aktuell
besten Variante der Engine zeigt. Das funktioniert für die meisten
Scid-kompatiblen Engines, aber nicht unbedingt für alle; es hängt
ganz davon ab, welche Zugnotation eine Engine verwendet.
</p>

<h3>Engine-Priorität</h3>
<p>
Falls eine Engine zu viel CPU-Zeit verbraucht und die Benutzung von
Scid oder anderen Applikationen beeinträchtigt, kann das Einschalten
der Checkbox <b>Niedrige CPU-Priorität</b> helfen; es weist der
Engine eine geringe Priorität fürs Prozeß-Scheduling zu.
</p>

<h3>Schnittstellen-Details</h3>
<p>
Für die Benutzung des Analysefensters benötigen Sie ein Schachprogramm,
welches das <term>UCI</term> oder <term>WinBoard/Xboard</term>-Protokoll unterstützt.
</p>
<p>
Bei Winboard-Engines sendet Scid beim Programmstart die Befehle <b>xboard</b>, <b>post</b> und
<b>protover 2</b> zur Engine, und nutzt die Befehle <b>setboard</b>
und <b>analyze</b> zur effizienteren Kommunikation, wenn die Engine in
ihrer Antwort anzeigt, daß sie diese unterstützt.
Wenn die Engine den Befehl <b>setboard</b> nicht unterstützt, wird sie
für Partien, die nicht mit der Standard-Ausgangsstellung beginnen, keine
Analysen liefern können.
</p>
<p>
Programmen, die den Befehl <b>analyze</b> nicht unterstützen, sendet
Scid nach jeder Änderung der Stellung folgende Kommandos: <b>new</b>
und <b>force</b>, dann die Partiezüge zur aktuellen Position, danach
<b>go</b>.
</p>
<p>
Bob Hyatt's exzellentes frei verfügbares Schachprogramm <term>Crafty</term>
benutze und empfehle ich zur Analyse mit Scid. Aber auch andere WinBoard-
oder XBoard-kompatible Programme sind erfolgreich getestet worden.
Ein paar Websites zum Herunterladen einiger Engines sind nachfolgend
aufgeführt.
</p>
<p>
Crafty:
<url ftp://ftp.cis.uab.edu/pub/hyatt/>ftp://ftp.cis.uab.edu/pub/hyatt/</url>
</p>
<p>
wbec-ridderkerk.nl (mit vielen Links auf aktuelle Schach-Engines):
<url http://wbec-ridderkerk.nl/html/lastupdates.htm>http://wbec-ridderkerk.nl/html/lastupdates.htm</url>
</p>
<p>
Gnuchess:
<url http://www.gnu.org/software/chess/chess.html>http://www.gnu.org/software/chess/chess.html</url>
</p>
<p>
The Crazy Bishop:
<url http://remi.coulom.free.fr/>http://remi.coulom.free.fr/</url>
</p>

<p><footer>(Aktualisiert: Scid 3.6.21, Dezember 2007)</footer></p>
}


###############################
### Comment editor window help:

set helpTitle(D,Comment) "Kommentareditor"
set helpText(D,Comment) {<h1>Der Kommentareditor</h1>
<p>
Mit dem Kommentareditor können Sie den Zügen der aktuellen Partie
Kommentare und symbolische Kommentarzeichen hinzufügen.
</p>

<h3>Kommentarzeichen</h3>
<p>
Scid verwendet den <a Author Related>PGN-Standard</a> für
Kommentarzeichen und akzeptiert <a NAGs>NAG</a>-Werte für Anmerkungen
(NAG = numeric annotation glyph, "numerisches Anmerkungszeichen").
Einige der meistgebräuchlichen Symbole (wie "!" oder "+-") werden als
solche ausgegeben und haben zur schnelleren Eingabe einen Schalter im
Kommentareditor. Für andere Symbole können Sie den jeweiligen numerischen
NAG-Wert, eine Zahl von 1 bis 255, eingeben. Zum Beispiel bedeutet
der NAG-Wert 36 "Weiß besitzt die Initiative" und wird im
<a PGN>PGN-Text</a> der Partie als "$36" ausgegeben.
</p>
<p>
Zu den NAG-Werten, die durch den PGN-Standard definiert werden,
siehe auch die Hilfeseite <a NAGs>NAG-Werte</a>.
</p>
<p>
<b>Hinweis:</b> Sie können die üblichen Bewertungssymbole für Schachzüge
(!, ?, !!, ??, !? und ?!) direkt im Haupfenster eingeben, ohne den
Kommentareditor zu verwenden, indem Sie das jeweilige Symbol gefolgt
von der [Eingabe]-Taste eintippen.
Das ist besonders nützlich beim <a Moves>Schachzüge eingeben</a>
über die Tastatur.
</p>

<h3>Kommentare</h3>
<p>
Sie können Kommentare im bereitgestellten Textbereich editieren und
dabei die Leeren-, Umkehren- und Speichern-Schalter verwenden.
Sie brauchen den Schalter "Speichern" nicht anzuklicken, um einen Kommentar
zu aktualisieren; dies geschieht automatisch, wann immer Sie zu einer
anderen Partiestellung gehen.
</p>

<h3>Felder einfärben</h3>
<p>
Sie können jedes Feld mit einer beliebigen Farbe einfärben, indem Sie
ein besonderes, an irgendeiner Stelle im Kommentar eingebettetes
Kommando verwenden. Das Format dieses Kommandos ist:
</p>
<ul>
<li><b>[%mark feld farbe]</b> ,<li>
</ul>
<p>
wobei <b>feld</b> ein Feldname wie d4 ist und <b>farbe</b> irgendeine
bekannte Farbbezeichnung (wie etwa red, blue4, darkGreen,
lightSteelBlue, etc.) oder ein RGB-Code (ein <b>#</b> gefolgt von 6
Hexadezimalziffern, wie z.B. #a0b0c8). Die Standardfarbe,
falls die Farbangabe weggelassen wird, ist <red>Rot</red>.
</p>
<p>
Ein Kommentar kann eine beliebige Anzahl von Farbkommandos enthalten,
aber jedes muß ein eigenes <b>[%mark ...]</b>-Tag-Feld haben.
Beispielsweise wird der Kommentartext</p>
<p>
  "Jetzt ist d6 [%mark d6] geschwächt und kann vom Springer auf
  b5 [%mark b5 #000070] angegriffen werden."
</p>
<p>
das Feld d6 <red>rot</red> und das Feld b5 in der dunkelblauen Farbe
<darkblue>#000070</darkblue> einfärben.
</p>

<h3>Pfeile einzeichnen</h3>
<p>
Sie können mit einem speziellen Kommentar-Kommando, ähnlich dem oben
beschriebenen zum Färben von Feldern, einen Pfeil von einem Feld zum
anderen zeichnen.
Das Format ist:
</p>
<ul>
<li><b>[%arrow vonFeld nachFeld farbe]</b><li>
</ul>
<p>
wobei <b>vonFeld</b> und <b>nachFeld</b> Feldbezeichnungen wie d4
sind und <b>farbe</b> ein bekannter Farbname (wie z.B. red, blue4, etc.)
oder RGB-Code (wie #a0b0c0) ist.
Ohne Farbangabe wird standardmäßig <red>Rot</red> genommen.
</p>
<p>
Beispiel: Der Kommentartext
</p>
<p>
"Springer auf c3 und Läufer auf c4 kontrollieren das schwache Feld d5.
 [%arrow c3 d5 red] [%arrow c4 d5 blue]"
</p>
<p>
zeichnet einen roten Pfeil von c3 nach d5 und einen blauen von c4 nach d5.
</p>

<p><footer>(Aktualisiert: Scid 3.2, Februar 2002)</footer></p>
}


###########################
### Crosstable window help:

set helpTitle(D,Crosstable) "Kreuztabelle"
set helpText(D,Crosstable) {<h1>Das Kreuztabellenfenster</h1>
<p>
Das Kreuztabellenfenster zeigt die Turnierkreuztabelle zur aktuellen
Partie. Jedesmal, wenn sie das Fenster aktualisieren (indem Sie den
"Aktualisieren"-Schalter anklicken, im Kreuztabellenfenster die
[Eingabe]-Taste drücken oder im <a MainWindow>Haupt</a>- oder
<a GameList>Partielisten</a>-Fenster die Tastenkombination
<b>[Strg]-[Umschalt]-X</b> betätigen), sucht Scid alle Partien
vom selben Turnier wie die aktuelle Partie.
</p>
<p>
Jede Partie, die bis zu <b>drei Monate vor oder nach</b> der aktuellen
Partie gespielt wurde und <b>exakt dieselben Turnier- und Ortsangaben</b>
enthält, wird als Partie dieses Turniers betrachtet.
</p>
<p>
Ein Einfachklick mit der linken Maustaste auf irgendein Ergebnis in der
Kreuztabelle lädt die entsprechende Partie.
Sie können alle Partien des Turniers mit dem Schalter
<b>Zum Filter addieren</b> des Kreuztabellenfensters zum
<a Searches Filter>Filter</a> hinzufügen.
</p>

<h4>Menüs des Kreuztabellenfensters</h4>
<p>
Im <menu>Datei</menu>-Menü können Sie die aktuelle Tabelle im Text-,
LaTeX- oder HTML-Format in eine Datei ausgeben.
</p>
<p>
Das <menu>Ausgabe</menu>-Menü erlaubt Ihnen, das Tabellenformat
auszuwählen: <b>Jeder gegen jeden</b>, <b>Schweizer System</b>,
<b>K.o.-System</b> oder <b>Automatisch</b>.
</p>
<p>
Das Format "Jeder gegen jeden" (für Rundenturniere) ist auf 30 Spieler
limitiert, aber "Schweizer System" (für Turniere mit vielen Spielern)
kann bis zu 200 Spieler und 20 Runden ausgeben. Die Standardeinstellung
ist <b>Automatisch</b>, was für jedes Turnier das beste Format auswählt.
</p>
<p>
Beachten Sie, daß Scid den <b>Runden</b>-Eintrag jeder Partie verwendet,
um eine Kreuztabelle nach Schweizer System zu erstellen, Sie werden daher
keine Partien in der Schweizer-System-Tabelle sehen, wenn sie keine
numerischen Runden-Angaben haben:  1, 2, 3, etc.
</p>
<p>
Im Ausgabe-Menü können Sie auch das Format der Datenausgabe nach Ihren
Wünschen einstellen, um Wertungszahlen, Länder oder Titel ein- oder
auzublenden. Sie können auch wählen, ob beim Schweizer System die
Farbzuteilung angezeigt werden soll.
</p>
<p>
Die Option <b>Punktgruppen</b> wirkt sich nur dann auf das Aussehen der
Tabelle aus, wenn die Spieler nach Punkten sortiert werden: sie bewirkt
das Einfügen einer Leerzeile zwischen Spielergruppen mit gleicher Punktzahl.
</p>
<p>
Das Menü <menu>Sortieren</menu> erlaubt es Ihnen, die Spieler nach
Namen, Elo-Zahlen oder Punkten zu sortieren; Standardeinstellung ist
nach Punkten.
</p>
<p>
Das <menu>Farben</menu>-Menü ermöglicht es Ihnen, Farbausgabe (Hypertext)
ein- oder auszuschalten. Da es sehr zeitaufwendig ist, große Kreuztabellen
in HTML zu formatieren und auszugeben, wird Ihnen die Option <b>Text</b>
bei großen Turnieren eine Menge Zeit einsparen.
Allerdings können Sie im Text-Modus keine Spieler oder Partien anklicken.
</p>

<h4>Dubletten in Kreuztabellen</h4>
<p>
Um sinnvolle Resultate mit der Kreuztabelle zu erzielen, sollten Sie
Dubletten zum Löschen markieren, und Ihre Partien sollten eine
einheitliche Schreibweise für Spieler-, Orts- und Turniernamen haben.
Zu Hilfen beim Löschen der Dubletten und Editieren (oder Überprüfen
der Schreibweisen) von Spieler-, Turnier-, Ortsnamen siehe die Seite
<a Maintenance>Datenbank-Wartung</a>.
</p>

<p><footer>(Aktualisiert: Scid 3.1, Dezember 2001)</footer></p>
}


###########################
### Database switcher help:

set helpTitle(D,Switcher) "Datenbank-Umschalter"
set helpText(D,Switcher) {<h1>Der Datenbank-Umschalter</h1>
<p>
Der Datenbank-Umschalter bietet eine Darstellung, die es besonders
einfach macht, zwischen Datenbanken zu wechseln oder Dateien zu kopieren.
Der Name, <a Searches Filter>Filter</a>-Status und Icon jeder Datenbank
werden angezeigt, und die aktive Datenbank ist durch einen gelben
Hintergrund hervorgehoben.
</p>
<p>
Sie können den Datenbank-Umschalter aus dem <menu>Fenster</menu>-Menü
starten oder durch die Tastenkombination <b>Strg+D</b>.
</p>
<p>
Um alle ausgewählten (gefilterten) Dateien einer Datenbank in eine andere
zu kopieren, ziehen Sie sie mit gedrückter linker Maustaste in das Zielfeld.
Sie sehen dann eine Nachfrage (falls die Zieldatenbank nicht die
<a Clipbase>Ablage</a> ist), ob die Dateien kopiert werden sollen, oder
eine Fehlermeldung, falls die Dateien nicht kopiert werden können
(z.B. wenn die gewählte Datenbank nicht geöffnet ist).
</p>
<p>
Drücken Sie die rechten Maustaste über einer Datenbank, erscheint ein
Kontextmenü zu dieser Datenbank, mit dem Sie das Datenbank-Icon ändern oder
den <a Searches Filter>Filter</a> zurücksetzen können.
In diesem Menü können Sie auch die Fensterausrichtung ändern (um die
Datenbankfelder vertikal oder horizontal anzuordnen), was für kleinere
Bildschirme recht nützlich ist.
</p>

<p><footer>(Aktualisiert: Scid 3.1, Dezember 2001)</footer></p>
}


######################
### Email window help:

set helpTitle(D,Email) "Email-Fenster"
set helpText(D,Email) {<h1>Das Email-Fenster</h1>
<p>
Scids Email-Manager ermöglicht es Ihnen, Ihre Email-Fernschachkorrespondenz
zu verwalten.
Wenn Sie kein Email-Schach  spielen, ist dies für Sie nicht von
Interesse. Aber wenn Sie Fernschach per Email spielen, können Sie
Ihre Email-Nachrichten direkt von Scid aus versenden!
</p>
<p>
Um den Email-Manager zu benutzen:
<ul>
<li><b>1)</b> Erstellen Sie die Partie(en) für Ihren Gegner
    in der Datenbank. </li>
<li><b>2)</b> Wählen Sie <b>Hinzufügen</b> im Email-Manager und geben
    Sie die Daten Ihrer Gegner ein: Name, Email-Adresse und die Zahl
    der Partien in der Datenbank. </li>
<li><b>3)</b> Wählen Sie <b>Email versenden</b> im Email-Fenster, wenn
    Sie Partiezüge ausgeführt haben und eine Nachricht versenden wollen. </li>
</ul>

<p>
Wenn Sie eine E-Mail verschicken, erstellt Scid die Nachricht mit den
Partien im PGN-Format <b>ohne</b> jegliche Kommentare, Anmerkungen oder
Varianten, denn Sie wollen Ihrem Gegner in der Regel nicht Ihre Analysen
zeigen.  Bevor Sie die Nachricht verschicken, können Sie sie noch
editieren, um bedingte Züge oder sonstigen Text hinzuzufügen.
</p>
<p>
Für jeden Gegner können Sie eine beliebige Anzahl von Partien haben;
meistens sind es eine oder zwei. Beachten Sie, daß Scid nicht überprüft,
ob sich die Zahl der Partien geändert hat. Passen Sie also auf, daß Sie
nicht Partien löschen oder die Datenbank sortieren, nachdem Sie die Daten
Ihrer Gegner eingegeben haben, denn das würde die Partien umstellen und
die Partienummern für Ihre Gegner wären nicht mehr korrekt.
</p>

<h3>Einschränkungen</h3>
<p>
Scid besitzt noch nicht die Fähigkeit, Ihren E-Mail-Ordner zu überprüfen,
Sie müssen daher die Züge Ihrer Kontrahenten manuell eingeben.
</p>

<h3>Konfiguration</h3>
<p>
Eine Kopie jeder von Scid gesendeten E-Mail wird in der Datei
<b>~/.scid/scidmail.log</b> gespeichert. Wenn
Sie sie in einer anderen Datei speichern möchten, müssen Sie die Datei
<b>tcl/start.tcl</b> bearbeiten und Scid neu erstellen ("make scid").
</p>
<p>
Scid kann E-Mails über einen SMTP-Server oder mit sendmail versenden.
Mit dem Schalter <b>Einstellungen</b> im E-Mail-Manager können Sie
auswählen, welche Möglichkeit Sie verwenden wollen.
</p>
<p>
Scid speichert die Daten Ihrer Gegner für eine Datenbank in einer Datei
mit demselben Namen wie die Datenbank und der Endung "<b>.sem</b>".
</p>

<p><footer>(Aktualisiert: Scid 3.0, November 2001)</footer></p>
}


############################
### File Finder window help:

set helpTitle(D,Finder) "Dateisuche"
set helpText(D,Finder) {<h1>Die Dateisuche</h1>
<p>
Die <term>Dateisuche</term> hilft Ihnen, Dateien jedes von Scid
benutzten Typs zu finden:  Datenbanken, <a PGN>PGN</a>-Dateien,
<a EPD>EPD</a>-Dateien und <a Repertoire>Repertoire</a>-Dateien.
</p>
<p>
Die Dateisuche zeigt Ihnen nützliche Informationen zu jeder Datei, wie
etwa ihre Größe (siehe unten) und Datum der letzten Modifikation.
Sie können jede angezeigte Datei durch einen linken Mausklick öffnen.
</p>

<h3>In Unterverzeichnissen nachsehen</h3>
<p>
Wenn Sie alle Dateien in allen Unterverzeichnissen des aktuellen
Verzeichnisses finden wollen, schalten Sie <b>Unterverzeichnisse
beachten</b> ein. Dann durchsucht Scid rekursiv jedes Unterverzeichnis
nach Dateien, die Scid öffnen kann. Wenn viele Unterverzeichnisse
vorhanden sind, kann dies eine Weile dauern, Sie sollten das also
nicht für ein Verzeichnis nahe dem Hauptverzeichnis des Dateisystems
tun. Sie können die Dateisuche mit dem <b>Stop</b>-Schalter unterbrechen.
</p>

<h3>Dateigrößen</h3>
<p>
Die Bedeutung der angegebenen Dateigröße hängt vom Typ der
Datei ab. Für Scid-Datenbanken und -PGN-Dateien ist es die Anzahl der
Partien. Für EPD-Dateien ist es die Anzahl der Positionen. Für
Repertoiredateien ist es die Anzahl der (gewählten oder ausgeschlossenen)
Varianten.
</p>
<p>
Für Scid-Datenbanken entspricht die Größe der Anzahl gespeicherter Partien.
Für alle anderern Datenbanken (z.B. pgn-Dateien) wird die Dateigröße angegeben.
</p>

<p><footer>(Aktualisiert: Scid 4.7, September 2018)</footer></p>
}


#########################
### GameList window help:

set helpTitle(D,GameList) "Partieliste"
set helpText(D,GameList) {<h1>Das Partielistenfenster</h1>
<p>
Das Partielistenfenster gibt eine einzeilige Zusammenfassung jeder
Partie im aktuellen <term>Filter</term> aus.
</p>

<h3>Navigation in der Partieliste</h3>
<p>
Sie können in der Partieliste mit dem Schieberegler oder den vier
Schaltern unterhalb der Liste blättern.
Sie können auch die Tasten [Pos1], [End], [Bild auf], [Bild ab] sowie
die Cursortasten verwenden, um mit der Tastatur zu scrollen.
</p>
<p>
Sie können die nächste Partie in der Liste, die einen bestimmten
Text im Weiß-, Schwarz-, Turnier- oder Ortsfeld enthält, mit dem
<b>Text finden</b>-Eingabefeld finden.
</p>

<h3>Aktionen mit Partien in der Liste</h3>
<p>
Um eine Datei aus der Liste zu laden, klicken Sie sie doppelt mit
der linken Maustaste an.
Ein Klick mit der mittleren Maustaste zeigt die Anfangszüge der Partie;
das ist nützlich, wenn man vor dem Laden einer Partie die Eröffnung
nachsehen will.
</p>
<p>
Die rechte Maustaste produziert ein Menü für die ausgewählte Partie,
in dem Sie die Partie löschen (oder "entlöschen") oder aus dem Filter
ausschließen können. Beachten Sie, daß das Löschen einer Datei lediglich
ihre Löschmarkierung setzt; sie verbleibt solange in der Datenbank,
bis Sie diese <a Compact>komprimieren</a>.
</p>

<h3>Die Partieliste konfigurieren</h3>
<p>
Um die Partieliste zu konfigurieren, klicken Sie mit der linken oder
rechten Maustaste auf eine Spaltenüberschrift. Sie können die Breite
verändern, Spalten hinzufügen oder entfernen und die Farbe jeder
Spalte verändern.
</p>
<p>
Wenn Sie nur die Spaltenbreite ändern wollen, dafür gibt es eine
Tastenkombination: klicken Sie bei gedrückter <b>[Strg]</b>- (oder
<b>[Umschalt]</b>)-Taste mit der linken Maustaste auf die
Spaltenüberschrift, wird die Spalte kleiner, mit der rechten
Maustaste, wird sie breiter.
</p>

<h3>Größe der Partieliste ändern</h3>
<p>
Die Größe der Partieliste wird jedesmal in der Optionsdatei gespeichert,
wenn Sie Ihre Einstellungen sichern.
Wenn Sie also wollen, daß die Dateiliste standardmäßig 10 Partien anzeigt,
verändern Sie einfach die Größe des Partielistenfensters und wählen
dann <b>Optionen speichern</b> im <menu>Optionen</menu>-Menü.
</p>

<h3><name Browsing>Partien betrachten und mischen</name></h3>
<p>
Das Kontextmenü der Partieliste (und einiger andere Fenster, wie etwa
des <a Reports Opening>Eröffnungsbericht</a>-Fensters und der Liste der
<a Tree Best>beste(n) Partien</a> des <a Tree>Zugbaum</a>-Fensters)
erlauben die Auswahl, eine Partie zu laden, zu betrachten oder zu
mischen.
</p>
<p>
Wenn <term>Partie betrachten</term> ausgewählt wird, werden die Züge
der ausgewählten Partie (ohne Kommentare oder Varianten) in einem
separaten Fenster ausgegeben. Das ist eine nützliche Methode, um eine
andere Partie im voraus anzuschauen, ohne die aktuell geladene Partie
zu berühren.
</p>
<p>
Die Funktion <term>Partie mischen</term> ermöglicht es, die ausgewählte
Partie als Variante der aktuellen Partie einzufügen. Scid findet den
letztmöglichen Zeitpunkt, wo die gewählte Partie von der aktuellen
abweicht (unter Berücksichtigung von Zugumstellungen), und fügt an
dieser Position eine Variante ein. Sie können die Zahl der angezeigten
Züge der gewählten Partie ändern, je nachdem, ob Sie die ganze Partie
oder nur die Eröffnungsphase hinzufügen wollen.
</p>

<p><footer>(Aktualisiert: Scid 3.2, Februar 2002)</footer></p>
}


#######################
### Import window help:

set helpTitle(D,Import) "Importfenster"
set helpText(D,Import) {<h1>Das Importfenster</h1>
<p>
Scids Importfenster ermöglicht es Ihnen, auf einfache Weise eine
Partie im <a PGN>PGN-Format</a> aus einer anderen Anwendung oder
einem anderen Fenster in Scid einzufügen.
</p>
<p>
In den großen Fensterbereich tippen oder fügen Sie den Partietext im
PGN-Format ein, und im grauen Bereich darunter erscheinen
Fehlermeldungen oder Warnungen.
</p>

<h3>Die aktuelle Partie im Importfenster bearbeiten</h3>
<p>
Das Importfenster bietet auch eine weitere, bequeme Möglichkeit für
Änderungen an der aktuellen Partie: Sie können die aktuelle Partie in
das Importfenster einfügen (mit dem Schalter <b>Aktuelle Partie
einfügen</b>), den Text editieren und danach auf <b>Import</b> klicken.
</p>

<h3>PGN-Tags im Importfenster</h3>
<p>
Scid erwartet PGN-Einträge ("header tags") wie
<ul>
<li> <b>[Result "*"]</b> </li>
</ul>
vor den Zügen, aber Sie können auch ein Partiefragment wie
<ul>
<li> <b>1.e4 e5 2.Bc4 Bc5 3.Qh5?! Nf6?? 4.Qxf7# 1-0</b> </li>
</ul>
ohne PGN-Tags einfügen, und Scid wird es importieren.
</p>

<h3>PGN-Dateien in Scid verwenden</h3>
<p>
Wenn Sie eine PGN-Datei in Scid verwenden wollen, ohne sie vorher mit
<a Pgnscid>pgnscid</a> zu konvertieren, gibt es zwei Möglichkeiten.
</p>
<p>
Zum einen können Sie die Partien der Datei mit der Menü-Option
<menu>Werkzeuge: Datei mit PGN-Partien importieren ...</menu> importieren.
</p>
<p>
Die Alternative wäre, die PGN-Datei direkt in Scid zu öffnen. Allerdings
werden PGN-Dateien nur zum Lesen geöffnet und benötigen mehr Speicher als
eine vergleichbare Scid-Datenbank, diese Möglichkeit ist also nur für
relativ kleine PGN-Dateien zu empfehlen.
</p>

<p><footer>(Aktualisiert: Scid 2.5, Juni 2001)</footer></p>
}


########################
### Reports help:

set helpTitle(D,Reports) "Berichte"
set helpText(D,Reports) {<h1>Berichte</h1>
<p>
Bei Scid ist ein <term>Bericht</term> ein Dokument, das Informationen über
eine bestimmte Position und/oder einen bestimmten Spieler enthält. Scid
kann zwei Berichtstypen erstellen: Eröffnungsberichte und Spielerberichte.
</p>

<h3><name Opening>Eröffnungsberichte</name></h3>
<p>
Scid kann einen <term>Eröffnungsbericht</term> erstellen, der interessante
Informationen über die aktuelle Eröffnungsposition ausgibt. Um den
Eröffnungsbericht zu erstellen, vergewissern Sie sich als erstes, daß die
dargestellte Position auch die ist, für die Sie den Bericht haben wollen,
dann wählen Sie <b>Eröffnungsbericht</b> im Menü <menu>Werkzeuge</menu>.
</p>
<p>
Das <term>Eröffnungsberichts</term>-Fenster gibt die Ergebnisse des von
Scid erstellten Berichts aus. Das <b>Datei</b>-Menü enthält Optionen,
um den Bericht in einer Datei zu speichern, im Text-, HTML- oder
<a LaTeX>LaTeX</a>-Format.
</p>
<p>
Die ersten Abschnitte des Berichts präsentieren Informationen zu den
Partien, die die Berichtsposition erreicht haben, und zu den Zügen, die
in dieser Stellung gespielt wurden. Sie können erkennen, ob die Eröffnung
populärer wird, ob sie zu vielen Kurzremisen führt und mit welchen
Zugfolgen (Zugumstellungen) sie erreicht wird.
</p>
<p>
Der Abschnitt über positionelle Themen informiert über die Häufigkeit
bestimmter typischer Themen der Berichtspartien. Zu diesem Zweck werden
die ersten 20 Züge (also die ersten 40 Positionen ab Ausgangsstellung)
jeder Partie untersucht. Um als eine Partie eingestuft zu werden, die
ein bestimmtes Thema enthält, muß dieses Thema mindestens viermal
innerhalb der ersten 20 Züge der Partie vorkommen. Damit wird vermieden,
daß durch kurzzeitiges Auftreten eines Themas (wie etwa ein isolierter
Damenbauer, der sofort geschlagen wird) die Ergebnisse verfälscht werden.
</p>
<p>
Der letzte und längste Teil des Berichts ist die Theorie-Tabelle. Wenn Sie
den Bericht in einer Datei sichern, können Sie wählen, ob Sie nur die
Theorie-Tabelle, einen kompakten Bericht ohne die Theorie-Tabelle oder
den gesamten Bericht sichern wollen.
</p>
<p>
Fast alle Abschnitte des Berichts können mit den Eröffnungsberichts-Optionen
eingestellt oder ein- und ausgeschaltet werden, Sie können also den
Bericht so anpassen, daß er nur die für Sie interesanten Informationen
enthält.
</p>
<p>
Bei den meisten Informationen im Berichtsfenster, die farbig dargestellt
sind, führt ein linker Mausklick zu einer Aktion. Zum Beispiel können Sie
eine angeführte Partie durch Anklicken laden oder den Filter durch
Klicken auf ein Positionsthema so einstellen, daß er nur die
Berichtspartien mit diesem Thema enthält.
</p>

<h4>Favoriten</h4>
<p>
Mit dem <menu>Favoriten</menu>-Menü des Berichtsfensters haben Sie die
Möglichkeit, eine Liste bevorzugter Eröffnungsbericht-Positionen zu
verwalten und sehr einfach Berichte für genau diese Positionen zu
erstellen. Wenn Sie "Bericht hinzufügen..." im Favoriten-Menü
wählen, wird die aktuelle Stellung als besondere Berichtsposition
aufgenommen. Sie werden aufgefordert, einen Namen anzugeben, der dann
als Dateiname verwendet wird, wenn Favoritenberichte erstellt werden.
</p>
<p>
Wählen Sie "Berichte erzeugen..." im Favoriten-Menü, um einen Bericht
für jede Ihrer bevorzugten Positionen aus der aktuellen Datenbank zu
erstellen. In einem Dialogfenster können Sie dann Typ und Format der
Berichte angeben sowie ein Verzeichnis, wo die Berichtsdateien
gespeichert werden sollen. Die passende Dateiendung für das gewählte
Ausgabeformat wird dann automatisch ergänzt (z.B. ".html" für das
HTML-Format).
</p>

<h3><name Player>Spielerberichte</name></h3>
<p>
Ein <term>Spielerbericht</term> ähnelt sehr stark einem Eröffnungsbericht,
enthält aber Informationen über Partien eines einzelnen Spielers mit
Weiß oder mit Schwarz. Sie können einen Spielerbericht über das
Werkzeuge-Menü oder aus dem <a PInfo>Spielerinformations</a>-Fenster
heraus erzeugen.
</p>
<p>
Ein Spielerbericht kann entweder für alle Partien eines bestimmten Spielers
mit einer bestimmten Farbe erstellt werden oder nur für die Partien, die
die aktuelle Stellung des Hauptfenster-Bretts erreicht haben.
</p>

<h3>Beschränkungen</h3>
<p>
Für die meisten Berichtsdaten gibt es eine Beschränkung auf 2000 Partien,
wenn die Berichtsposition also mehr als 2000 Partien enthält, können
einige Resultate etwas ungenau sein.
</p>
<p>
Außerdem gibt es ein Limit von 500 Partien für die Theorie-Tabelle. Wenn
die Berichtsposition in mehr als 500 Partien vorkommt, werden zur
Erstellung der Theorie-Tabelle nur die 500 Partien mit den höchsten
durchschnittlichen Elo-Zahlen herangezogen. Die Anzahl der Partien zum
Aufbau der Theorie-Tabelle ist einstellbar.
</p>

<p><footer>(Aktualisiert: Scid 3.5, Februar 2003)</footer></p>
}


####################
### PGN window help:

set helpTitle(D,PGN) "PGN-Fenster"
set helpText(D,PGN) {<h1>Das PGN-Fenster</h1>
<p>
Scids PGN-Fenster gibt den Inhalt der aktuellen Partie in der
Standard-PGN-Darstellung aus. Im Partietext erscheinen Kommentare in
{geschweiften} und Varianten in (runden) Klammern.
</p>

<h3>PGN-Format</h3>
<p>
PGN (Portable Game Notation) ist ein verbreiteter Standard zum Transfer
von Schachpartien zwischen Computerprogrammen. Eine PGN-Partie besteht
aus zwei Abschnitten.
Der erste Teil ist der Vorspann ("Header"), der Eintragungen ("tags") wie
z.B.
<b>[White "Kasparov, Gary"]</b>
und
<b>[Result "1/2-1/2"]</b> enthält.
</p>
<p>
Der zweite Abschnitt enthält die eigentlichen Partiezüge in
algebraischer Standardnotation (SAN) zusammen mit eventuellen Varianten,
<a NAGs>Kommentarsymbolen</a> und <a Comment>Kommentaren</a>.
</p>

<h3>Aktionen im PGN-Fenster</h3>
<p>
Sie können mit dem PGN-Fenster innerhalb der Partie navigieren: Mit
einem Linksklick auf einen Zug gehen Sie zu diesem Zug, mit einem
Linksklick auf einen Kommentar können Sie diesen editieren.
Die Cursortasten (sowie die Tasten <b>v</b> und <b>z</b> für
Variante beginnen bzw. verlassen) funktionieren zum Navigieren in der
Partie genau wie im Hauptfenster.
</p>

<h3>Einstellungen für PGN-Ausgabe</h3>
<p>
Das Menü des PGN-Fensters enthält Optionen, die die Ausgabe des Fensters
betreffen. Scid kann die Partie farbig oder als reinen Text darstellen
-- siehe das <menu>Ausgabe</menu>-Menü im PGN-Fenster.
Die farbige Darstellung ist leichter zu lesen und erlaubt Ihnen, Züge und
Kommentare mit der Maus auszuwählen, aber sie ist viel langsamer zu
aktualisieren. Für sehr lange Partien sollten Sie vielleicht die
einfache Textausgabe wählen.
</p>
<p>
Auch das Format der Kommentare und Varianten können Sie verändern,
indem Sie sie zwecks besserer Übersichtlichkeit eingerückt oder in
separaten Zeilen ausgeben lassen.
</p>
<p>
Die PGN-Ausgabe-Optionen und die Größe des PGN-Fensters werden, wann
immer Sie <b>Optionen speichern</b> im <menu>Optionen</menu>-Menü wählen,
in der Konfigurationsdatei gesichert.
</p>

<p><footer>(Aktualisiert: Scid 3.1, Dezember 2001)</footer></p>
}


#######################
### Piece Tracker help:

set helpTitle(D,PTracker) "Figurenverteilung"
set helpText(D,PTracker) {<h1>Das Figurenverteilungsfenster</h1>
<p>
Die <term>Figurenverteilung </term> bzw. das Figurenverteilungsfenster
ist ein Werkzeug, um die Bewegungen bestimmter Figuren in allen Partien
des aktuellen Filters nachzuvollziehen und ein Bewegungsmuster (eine
"Fußspur") zu erstellen, das aufzeigt, wie oft jedes Feld von dieser
Figur besetzt wurde.
</p>
<p>
Um die Figurenverteilung zu benutzen, vergewissern Sie sich zuerst, daß
der Filter die Partien enthält, die Sie interessieren, z.B. Partien mit
einer bestimmten Eröffnungsposition oder alle Weiß-Partien eines
bestimmten Spielers. Dann wählen Sie die Figur, deren Bewegung Sie
nachvollziehen wollen, und wählen die unten im Text erläuterten Optionen.
Danach klicken Sie auf den Schalter <b>Aktualisieren</b>.
</p>
<p>
Die Information über die Figurenbewegungen wird auf zwei Arten dargestellt:
eine grafische "Fußspur" und eine Liste mit einer Textzeile pro Feld.
</p>

<h3>Auswahl der Figur</h3>
<p>
Die Schachfiguren sind unterhalb der "Karte" für die "Fußspur" gemäß der
Ausgangsstellung angeordnet. Eine einzelne Figur (wie etwa der weiße
Springer b1 oder der schwarze Bauer d7) kann mit der linken Maustaste
ausgewählt werden, alle Figuren von gleichem Typ und gleicher Farbe
(wie z.B. alle weißen Bauern oder beide schwarzen Türme) mit der
rechten Maustaste.
</p>

<h3>Weitere Einstellungen</h3>
<p>
Der Zugnummer-Bereich kontrolliert, wann die Figurenverfolgung anfangen
und enden soll. Die Standardeinstellung 1-20 (d.h. die Verfolgung endet
nach dem 20. Zug von Schwarz) ist gut geeignet zur Untersuchung von
Eröffnungsthemen, aber (z.B.) ein Bereich von 15-35 wäre besser, wenn
man Entwicklungen im Mittelspiel betrachten will.
</p>
<p>
Es können zwei Arten von Statistiken erstellt werden:
<ul>
<li> <b>% der Partien mit Zug auf das Feld</b>: zeigt, welcher Anteil
     an Filterpartien einen Zug der zu beobachtenden Figur auf jedes
     Feld enthält. Das ist die Standardeinstellung und normalerweise
     die beste Wahl.
<li> <b>% der Zeit auf jedem Feld</b>: zeigt den Anteil der
     Verweildauer der zu beobachtenden Figur auf jedem Feld.
</ul>
</p>

<h3>Hinweise</h3>
<p>
Es gibt (mindestens) drei gute Verwendungsmöglichkeiten für die
Figurenverteilung: Eröffnungsvorbereitung, Mittelspielthemen und
Spielervorbereitung.
</p>
<p>
Für die Eröffnungsvorbereitung verwenden Sie die Figurenverteilung
zusammen mit dem <a Tree>Zugbaum</a>. Indem Sie die Bewegungen von
Figuren nachvollziehen, können Sie Entwicklungen in der aktuellen
Eröffnung erkennen, beispielsweise übliche Bauernvorstöße,
Springervorposten und die häufigsten Läuferplazierungen. Dabei ist es
hilfreich, den Zugbereich nach dem aktuellen Partiezug beginnen zu
lassen, so daß die Züge, die zur aktuellen Stellung führten, nicht in
die Statistik einfließen.
</p>
<p>
Für Mittelspielthemen kann die Figurenverteilung nützlich sein, wenn
der Filter einen bestimmten Elo-Bereich enthält (mit der
<a Searches Header>Partiedaten-Suche</a>) oder vielleicht ein Muster
wie "Isolierter Damenbauer von Weiß"
(mit der <a Searches Material>Material/Muster-Suche</a>).
Stellen Sie den Zugbereich auf einen sinnvollen Wert ein (z.B. 20-40)
und verfolgen Sie Figuren, um beispielsweise Bauernvorstöße im späten
Mittelspiel oder frühen Endspiel zu erkennen
</p>
<p>
Zur Spielervorbereitung verwenden Sie die
<a Searches Header>Partiedaten-Suche</a> oder die
<a PInfo>Spieler-Information</a>, um alle Partien einer Farbe eines
bestimmten Spielers zu finden. Die Figurenverteilung kann dann genutzt
werden um z.B. festzustellen, wie gern der Spieler Läufer fianchettiert,
lang rochiert oder einen Bauernkeil bis d5 oder e5 treibt.
</p>

<p><footer>(Aktualisiert: Scid 3.3, April 2002)</footer></p>
}

#####################
### Player List help:

set helpTitle(D,PList) "Spielersuche"
set helpText(D,PList) {<h1>Die Spielersuche</h1>
<p>
Die <term>Spielersuche</term> gibt eine Namensliste von Spielern
aus der aktuellen Datenbank aus. Die Auswahl eines Spielers öffnet
das <a PInfo>Spieler-Informations</a>-Fenster mit detaillierteren
Informationen über diesen Spieler.
</p>
<p>
Es werden 5 Spalten ausgegeben, die jeweils den Namen, die höchste
Elo-Zahl, Anzahl der gespielten Partien sowie das Jahr der
ältesten und neueste Partie anzeigen.
Ein Klick auf eine Spalten-Überschrift sortiert die Liste nach
dieser Spalte.
</p>
<p>
Die Kontrollschalter unterhalb der Liste ermöglichen Ihnen, den
Inhalt der Liste zu filtern. Sie können die maximale Listengröße
ändern, einen Spielernamen-Anfang eingeben (Groß-/Kleinschreibung
wird ignoriert, z.B. sucht "ada" nach "Adams") sowie den
Elozahlen-Bereich oder die Anzahl der gespielten Partien eingrenzen.
</p>

<p><footer>(Aktualisiert: Scid 4.7, September 2018)</footer></p>
}

#####################
### Player Info help:

set helpTitle(D,PInfo) "Spielerinformation"
set helpText(D,PInfo) {<h1>Die Spieler-Information</h1>
<p>
Das Fenster <term>Spieler-Information</term> wird jedesmal aufgebaut
oder aktualisiert, wenn Sie im Informationsbereich (unterhalb des
Schachbretts) oder im <a Crosstable>Kreuztabellen</a>-Fenster mit der
linken Maustaste auf einen Spielernamen klicken.
</p>
<p>
Es gibt nützliche Informationen über die Spieler aus,
einschließlich ihrer Erfolgsrate mit Weiß und Schwarz, ihrer bevorzugten
Eröffnungen (nach <a ECO>ECO-Code</a>) und ihrer Elo-Entwicklung.<br>
Die Daten für den Elo-Verlauf können wahlweise aus der aktuellen Datenbank
oder aus der Schreibkorrekturdatei/Ratingdatei gelesen werden. Wenn Sie ein
gepflegte Rating-Datei verwenden, können auch die Zeiträume dargestellt werden,
für die in der Datenbank keine Spiele vorhanden sind.<br>
Eine Ratingdatei mit aktuellen ELO-Zahlen von über 300.000 registrierten
FIDE-Spieler kann unter <url https://sourceforge.net/projects/scid/files/Player Data/>
https://sourceforge.net/projects/scid/files/Player Data/</url> heruntergeladen werden.
</p>
<p>
Jede Prozentangabe ist das Ergebnis, das man aus Sicht des Spielers
erwartet (Erfolgsrate) -- d.h. ein höherer Wert ist für die Spieler
immer besser, ob als Weiß oder Schwarz.
</p>
<p>
Sie können die ELO-Entwicklung des Spielers grafisch dargestellt
sehen, wenn Sie die Taste <a Graphs Rating>ELO-Zahl-Verlauf</a> anklicken.
</p>
<p>
Jede rot dargestellte Zahl können Sie mit der linken Maustaste anklicken,
um den <a Searches Filter>Filter</a> so einzustellen, daß er die
entsprechenden Partien enthält.
</p>

<p><footer>(Aktualisiert: Scid 4.7, November 2018)</footer></p>
}


###########################
### Repertoire editor help:

set helpTitle(D,Repertoire) "Repertoire-Editor"
set helpText(D,Repertoire) {<h1>Der Repertoire-Editor</h1>
<p>
Mit dem Repertoire-Editor können Sie <term>Repertoire</term>-Dateien
erstellen, lesen oder bearbeiten. Eine Repertoiredatei ist eine Liste
der Eröffnungspositionen, die Sie erreichen wollen oder zu vermeiden
suchen; Sie können sie zur Organisation Ihrer Eröffnungspräferenzen
und zur Datenbanksuche in Scid verwenden.
</p>

<h3>Repertoire-Gruppen und -Varianten</h3>
<p>
Ein Repertoire enthält zwei Elementtypen: <term>Gruppen</term> und
<term>Varianten</term>. Gruppen sind nicht tatsächlich Teil Ihres
Repertoires; sie werden nur zur Strukturierung verwendet, so wie
Verzeichnisse Dateien auf einem Datenträger strukturieren.
</p>
<p>
In einem Repertoire gibt es in zwei Arten von Varianten:
<term>eingeschlossene</term> Varianten repräsentieren Eröffnungspositionen,
an denen Sie interessiert sind und die Sie erreichen wollen, und
<term>ausgeschlossene</term> Varianten, die Sie nicht spielen wollen
und zu vermeiden suchen.
Wenn Sie z.B. Angenommenes Damengambit (1.d4 d5 2.c4 dxc4) mit Schwarz
spielen und nach 3.e4 alle Züge <i>außer </i> 3...Sf6 spielen, hätten
Sie 1.d4 d5 2.c4 dxc4 3.e4 als eingeschlossene Variante und 1.d4 d5 2.c4
dxc4 3.e4 Sf6 als ausgeschlossene Variante.
</p>

<h3>Kommentare und Anmerkungen</h3>
<p>
Jede Gruppe oder Variante kann Kommentare enthalten. Es gibt zwei Typen:
kurze (einzeilige) Kommentare erscheinen (in Rot) in der
Repertoire-Hierarchie neben den Zügen der Gruppe oder Variante, während
ein langer (mehrzeiliger) Kommentar nur angezeigt wird, wenn die Gruppe
oder Variante ausgewählt wird.
</p>

<h3>Das Fenster des Repertoire-Editors</h3>
<p>
Die <b>linke</b> Seite des Fensters zeigt die Repertoire-Hierarchie.
Sie können die Gruppendarstellung mit einem Klick auf den Ordner auf-
bzw. zuklappen, und mit einem Klick auf die Züge der Gruppen oder
Varianten wählen Sie diese aus und sehen ihre Kommentare.
Eingeschlossene Varianten werden mit einem blauen Haken angezeigt,
ausgeschlossene Varianten haben ein rotes Kreuz.
</p>
<p>
Wenn eine Variante oder Gruppe einen Kurzkommentar hat, wird er hinter
den Zügen angezeigt. Wenn sie einen langen Kommentar hat, wird dies mit
<b><red>**</red></b> nach den Zügen angezeigt. Bei den Gruppen steht
nach den Zügen eine Zahl in Klammern, die die Anzahl der (ein- und
ausgeschlossenen) Varianten anzeigt, die sie jeweils enthalten.
</p>
<p>
Nach einem Rechtsklick auf eine Gruppe oder Variante erscheint ein Menü
mit den verfügbaren Funktionen wie Löschen oder Statusänderung.
</p>
<p>
Die <b>rechte</b> Seite des Fensters besteht aus drei Bereichen. Der
erste enthält die Züge der gerade ausgewählten Variante oder Gruppe. Sie
können ihn mit der linken Maustaste anklicken, womit Sie die Züge in das
<a Import>Import</a>-Fenster einfügen, was recht nützlich ist, um die
aktuelle Partie mit einer Variante des Repertoires beginnen zu lassen.
Der zweite Bereich enthält den Kurzkommentar der Variante oder Gruppe,
der dritte Bereich ihren langen Kommentar.
</p>

<h3>Gruppen und Varianten dem Repertoire hinzufügen</h3>
<p>
Um eine Variante oder Gruppe hinzuzufügen, führen Sie einfach ihre Züge
auf dem Schachbrett im Hauptfenster aus, wählen dann das Menü
<menu>Bearbeiten</menu> im Repertoire-Editor und fügen sie als Gruppe,
eingeschlossene Variante oder ausgeschlossene Variante hinzu.
</p>
<p>
Um eine Gruppe oder Variante zu löschen, klicken Sie sie mit der rechten
Maustaste an und wählen im erschienenen Menü das entsprechende Kommando.
</p>

<h3><name Search>Datenbanksuche mit Repertoire-Dateien</name></h3>
<p>
Das Menü <menu>Suchen</menu> des Repertoire-Editors ermöglicht es Ihnen,
mit dem Repertoire in der aktuellen Datenbank zu suchen. Jede Partie
wird mit den Positionen im Repertoire verglichen und nur dann erkannt,
wenn die <i>weitestverzweigte</i> Repertoire-Position für eine
<i>eingeschlossene</i> Variante gefunden wurde.
</p>
<p>
Sie können wählen, ob Sie nach dem gesamten Repertoire oder nur nach den
ausgegebenen Varianten suchen wollen. Die Suche nach den ausgegebenen
Varianten ist nur dann sinnvoll, wenn Sie nur nach einigen Varianten
des Repertoires suchen. Beispiel: Ein Repertoire habe zwei Hauptgruppen
auf der obersten Ebene, eine für 1.e4 und eine für 1.d4. Wenn Sie nur
an den e4-Varianten interessiert sind, klappen Sie einfach die Gruppe
1.d4 zu und suchen dann nur nach den ausgegebenen Varianten.
</p>

<h3>Weitere Hinweise</h3>
<p>
Eine Repertoire-Datei eignet sich sehr gut zum Finden neuer Partien in
Ihrem Eröffnungssystem. Beispiel: Jedesmal, wenn Sie eine PGN-Datei
erhalten und sie Ihrer Haupt-Datenbank hinzufügen (wie etwa
die ausgezeichneten wöchentlichen PGN-Dateien von
<url http://www.chesscenter.com/twic/>The Week In Chess</url>),
öffnen Sie einfach die PGN-Datei in Scid und führen eine Repertoire-Suche
durch. Dann können Sie die gefilterten Partien betrachten und haben
alle Partien, die in Ihrem Repertoire enthalten sind.
</p>
<p>
Sie sollten vielleicht zwei Repertoire-Dateien anlegen: eine für Schwarz
und eine für Weiß, so daß Sie mit jeder Datei getrennt suchen können.
</p>
<p>
Eine Repertoire-Datei kann von der Kommanozeile aus geöffnet werden,
zum Beispiel:<br>
<b>scid datenbank weiss.sor</b>
</p>
<p>
Sie können eine Repertoire-Datei (.sor) mit jedem Editor bearbeiten,
aber passen Sie auf, daß Sie nicht ihr Format verändern, sonst kann
sie von Scid nicht geladen oder gesucht werden.
</p>

<p><footer>(Aktualisiert: Scid 2.6, August 2001)</footer></p>
}


##################################
### Tournament Finder window help:

set helpTitle(D,Tmt) "Turniersuche"
set helpText(D,Tmt) {<h1>Die Turniersuche</h1>
<p>
Die <term>Turniersuche</term> ermöglicht es, Turniere in der
aktuellen Datenbank zu finden. Es wird angenommen, daß
zwei Partien zum selben Turnier gehören, wenn sie denselben Turnier-
und Ortseintrag haben und innerhalb von drei Monaten gespielt wurden.
</p>
<p>
Sie können die Liste der Turniere durch die Anzahl der Spieler und
Partien, das Datum, die durchschnittliche Elo-Zahl und das Land
einschränken, wenn Sie die Felder unterhalb der Turnierliste ausfüllen
und dann auf <b>Aktualisieren</b> klicken.
</p>
<p>
Die ausgegebene Liste kann nach Datum, Spielerzahl, Partienzahl,
durchschnittliche Elo-Zahl, Ort, Turnier oder Nachname des Gewinners
sortiert werden. Wählen Sie die Kategorie im
<menu>Sortieren</menu>-Menü oder klicken Sie auf die Spaltenüberschrift,
um das Sortierkriterium zu ändern.
</p>
<p>
Um die erste Partie eines der aufgeführten Turniere zu laden, klicken
Sie einfach mit der linken Maustaste, wenn die entsprechende Zeile
hervorgehoben ist. Damit wird auch das
<a Crosstable>Kreuztabellen</a>-Fenster aktualisiert, falls es geöffnet
ist.
Wenn Sie statt dessen die rechte Maustaste drücken, wird die Partie
geladen und das Kreuztabellenfenster geöffnet, auch wenn es vorher
geschlossen war.
</p>
<p>
Es ist eine gute Idee, zur Beschleunigung des Suchprozesses den
Datenbereich angemessen einzuschränken (z.B. höchstens einige Jahre)
oder ein einzelnes Land auszuwählen (mit dem 3stelligen
Standard-Ländercode). Damit wird die Zahl der Partien, die Scid bei
der Konstruktion von Turnieren aus den Partien berücksichtigen muß,
außerordentlich verringert.
</p>

<p><footer>(Aktualisiert: Scid 4.7, September 2018)</footer></p>
}


####################
### Tree window help:

set helpTitle(D,Tree) "Zugbaum"
set helpText(D,Tree) {<h1>Das Zugbaumfenster</h1>
<p>
Das <term>Zugbaum</term>-Fenster liefert Informationen über alle Züge,
die in der aktuellen Position in den Datenbankpartien gespielt wurden.
Im Zugbaum-Modus wird das Fenster automatisch aktualisiert, sowie sich
die Stellung im Hauptfenster ändert. Für große Datenbanken ist das
evtl. etwas langsam.
</p>
<p>
Jedesmal, wenn das Zugbaumfenster aktualisiert wird, wird der
<a Searches Filter>Filter</a> zurückgesetzt, und nur die Partien, die
die die aktuelle Position enthalten, sind erfaßt.
</p>
<p>
Ein Klick mit der linken Maustaste auf einen Zug im Zugbaumfenster
fügt diesen Zug der Partie hinzu.
</p>

<h3>Inhalt des Zugbaumfensters</h3>
<p>
Das Zugbaumfenster zeigt den <a ECO>ECO-Code</a> (falls vorhanden),
die Häufigkeit (als Anzahl der Partien und in Prozent) und die
Punkte jedes Zuges an. Die <term>Punkte</term> werden immer aus
der Sicht von <b>Weiß</b> berechnet, 100% bedeutet also, alle
Weißspieler gewinnen, und 0% heißt nur Schwarzsiege.
</p>
<p>
Die Züge im Zugbaumfenster können nach Zügen (alphabetisch), ECO-Code,
Häufigkeit oder Punkten sortiert werden. Sie können die Sortiermethode
im Menü <menu>Sortieren</menu> ändern.
</p>

<h3><name Best>Das Fenster "Beste Partien"</name></h3>
<p>
Das Zugbaumfenster besitzt ein Dateimenü-Kommando und einen Schalter,
um das Fenster <term>Beste Partien</term> zu öffnen, welches eine Liste
der am höchsten bewerteten Partien des aktuell gezeigten Astes ausgibt.
Die Partien sind nach durchschnittlicher Elo-Zahl sortiert, und Sie
können die Liste auf Partien mit einem bestimmten Ergebnis beschränken.
</p>

<h3><name Graph>Zugbaumgrafik</name></h3>
<p>
Das Zugbaumfenster enthält einen mit <term>Grafik</term> bezeichneten
Schalter, der eine grafische Darstellung der relativen Erfolgsrate
jedes Zuges in der aktuellen Stellung liefert.
Alle Züge, die in mindestens 1% der Partien und mindestens 5mal
gespielt wurden, werden dargestellt.
Die prozentualen Ergebnisse werden immer aus der Perspektive von
Weiß gesehen, auch wenn Schwarz am Zug ist.
</p>
<p>
In der Zugbaumgrafik ist eine rote Linie eingezeichnet, die den
Durchschnitt aller Partien in der aktuellen Position anzeigt, und der
Bereich zwischen 50 und 55% (wo der Erwartungswert der meisten
Standarderöffnungen liegt) ist blau gefärbt. In Meisterpartien erreicht
Weiß üblicherweise etwa 55%.
</p>

<h3><name Lock>Das Zugbaumfenster anbinden</name></h3>
<p>
Jedes Zugbaumfenster ist mit einer geladenen Datenbank verbunden, so
dass, sollten mehre Datenbanken geöffnet sein, auch mehrere
Zugbaumfenster parallel existieren können, je eines für jede
Datenbank.
Mit dem Schalter <term>Anbinden</term> im Zugbaumfenster kann man nun
erreichen, dass eine Datenbank automatisch geschlossen wird, sobald
das zugehörige Zugbaumfenster geschlossen wird. Weiterhin werden dann
auch alle anderen, mit dem Zugbaumfenster verbundenen Fenster "Beste
Partien" oder eine Zugbaumgrafik automatisch mit geschlossen.
</p>

<h3><name Training>Training</name></h3>
<p>
Wenn der <term>Trainings</term>-Schalter im Zugbaumfenster aktiviert
ist, wird Scid jedesmal, wenn Sie einen Zug in der Partie ausführen,
einen zufällig ausgewählten Zug erwidern. Der Zug, den Scid auswählt,
hängt von der Datenbankstatistik ab, d.h. ein Zug, der in 80% der
Datenbankpartien gespielt wurde, wird von Scid mit 80%iger
Wahrscheinlichkeit ausgewählt. Diese Funktion einschalten und dann das
Zugbaumfenster verstecken (oder minimieren) und Eröffnungen gegen eine
große Datenbank spielen ist eine ausgezeichnete Methode, Ihre Kenntnisse
Ihres Eröffnungsrepertoires zu testen.
</p>

<h3>Den Zugbaum in geöffneten EPD-Dateien verwenden</h3>
<p>
Für jede geöffnete <a EPD>EPD-Datei</a> enthält das Zugbaumfenster
eine weitere Spalte, die für jede Position, die mit den angegebenen
Zügen erreicht wird, eine kurze (fünf Zeichen) Zusammenfassung des
Dateiinhalts anzeigt.
</p>
<p>
Die Zusammenfassung könnte eine Bewertung, ein Eröffnungscode oder ein
Zugvorschlag sein; sie wird der Inhalt des ersten gefundenen EPD-Feldes
aus der folgenden Liste sein: <b>ce, eco, nic, pv, pm, bm, id</b>, oder
einfach des ersten EPD-Feldes, falls keines der obigen vorhanden ist.
</p>
<p>
Für eine Beschreibung der EPD-Felder siehe die Hilfeseite
<a EPD>EPD-Dateien</a>. Wenn die Zusammenfassung das <b>ce</b>-Feld
ist, wird sie zur besseren Lesbarkeit als Bewertung in Bauern aus der
Sicht von Weiß angezeigt (anstatt als Bewertung in Hundertstel Bauern
aus der Sicht der am Zug befindlichen Seite, wie sie in der EPD-Datei
gespeichert ist).
</p>

<h3>Masken zur Konfiguration der Darstellung</h3>
<p>
Benutzerdefinierte Maskendateien (.stm) können angelegt werden, um die
Anzeige der Statistik im Zugabaumfenster zu erweitern. Hiermit können
einerseits farbige Symbole, NAG-Codes oder Kommentare für eine
bestimmte Stellung hinzugefügt werden, um beispielsweise kritische
Positionen zu markieren.
</p>
<p>
Um Markierungen anzulegen bewegt man sich zunächst in einer
bestehenden Partie zur entsprechenden Position und klickt dann mit der
Maus die entsprechene Variante mit der rechten Taste an. Sodann können
aus dem sich öffnenden Kontextmenue entsprechende Markierungen
ausgewählt oder Kommentare eingegeben werden. Nach dem Hinzufügen
entsprechender Annotationen muß die Maske mit dem Punkt
<term>Speichern</term> gesichert werden.
</p>
<p>
Masken können auch aus bestehenden Partien, welche Kommentare und
Varianten enthalten gefüllt werden. Hierzu dient der Punkt <term>Mit
aktueller Partie füllen</term> bzw. <term>Mit Datenbank Füllen</term>
</p>

<h3>Schnellere Ergebnisse durch Zwischenspeichern</h3>
<p>
Scid nutzt für die am häufigsten vorkommenden Positionen einen
Cachespeicher für die Ergebnisse der Zugbaumsuche. Wenn Sie sich im
Zugbaum-Modus in einer Partie vorwärts und rückwärts bewegen, werden
Sie bemerken, daß das Zugbaumfenster praktisch sofort aktualisiert
wird, wenn die gesuchte Stellung im Cachespeicher ist.
</p>
<p>
Das Zugbaumfenster hat ein Dateimenü-Kommando <term>Cache-Datei
sichern</term>. Wenn Sie das auswählen, wird der aktuelle Inhalt des
Zugbaum-Zwischenspeichers in eine Datei geschrieben (mit der Endung
<b>.stc</b>), um spätere Anwendungen des Zugbaum-Modus mit dieser
Datenbank zu beschleunigen.
</p>
<p>
Die Option <term>Cache-Datei füllen</term> im Dateimenü des
Zugbaumfensters füllt die Cache-Datei mit Daten für viele
Eröffnungspositionen. Es werden etwa 100 der häufigsten
Eröffnungsstellungen gesucht, dann wird die Cache-Datei geschrieben.
</p>
<p>
Das neue aufbauen des Zugbaumfensters kann erheblich beschleunigt
werden, wenn eine Datenbank nach ECO-Codes sortiert abgespeichert
wird. (Zum sortieren von Datenbanken siehe das <a
Maintenance>Wartungsfenster</a>.) Dieses Sortieren kann allerdings
viele Stunden in Anspruch nehmen. Danach kann durch einschalten der
Option <term>Schneller Modus</term> die Suche um bis zu einen Faktor
20 beschleunigt werden, wobei allerdings kleinere Ungenauigkeiten in
Kauf genommen werden müssen. Um eine schnelle Voranzeige der Statistik
zu erreichen aber trotzdem ein genaues ergenbis zu erziehlen dient die
Option <term>Schneller und Gründlicher Modus</term>
</p>
<p>
Beachten Sie, daß eine Zugbaum-Cache-Datei (.stc) völlig redundant ist;
Sie können sie ohne Auswirkungen auf die Datenbank löschen, und
tatsächlich wird sie jedesmal von Scid gelöscht, wenn sie nach
irgendeiner Aktion veraltet sein könnte -- zum Beispiel nach Hinzufügen
oder Ersetzen einer Partie, oder nach Sortieren der Datenbank.
</p>

<p><footer>(Aktualisiert: Scid 3.6.26, Oktober 2008)</footer></p>
}


################
### Graphs help:

set helpTitle(D,Graphs) "Grafikfenster"
set helpText(D,Graphs) {<h1>Grafikfenster</h1>
<p>
Scid hat einige Fenster, die Informationen grafisch darstellen.
Diese werden im nachfolgenden erklärt.
</p>

<h3><name Filter>Relative und absolute Filtergrafik</name></h3>
<p>
Das Fenster <term>relative Filtergrafik</term> zeigt Entwicklungen nach Datum
oder Elo-Zahl für die aktuellen Filterpartien im Vergleich zur gesamten
Datenbank. Das ist beispielsweise im <a Tree>Zugbaum</a>-Fenster eine
nützliche Hilfe um zu zeigen, wie sich die Popularität der aktuellen
Eröffnungsposition in den letzten Jahren oder Jahrzehnten geändert hat
oder ob sie bei hochrangigen Spielern, etwa bei Großmeistern, besonders
beliebt ist. Jeder Punkt in der Grafik repräsentiert für ein bestimmtes
Datum oder einen bestimmten Elo-Bereich die Anzahl der Partien im Filter
pro 1000 Partien der gesamten Datenbank.
<br><br>
Neben der relative Filtergrafik gibt es eine <term>absolute Filtergrafik</term>, die die
Partien im Filter nach der absoluten Anzahl der Partien darstellt. Es kann nach
Jahr, Elo-Zahl und Zugzahl der Partie ausgewertet werden.<br>
Mit dem kleinen Grafik-Knopf kann ein Dialog geöffnet werden. Hier kann für jede der drei
Auswertungen der Bereich (von, bis) und die Intervallgröße der X-Achse
eingestellt werden. Wird "Schätzen" ausgewählt, so wird der ELO-Wert
geschätzt (s.u.), ansonst wird ein fehlender ELO-Wert eines Spielers
mit 0 bewertet. "Schätzen aus" entspricht dabei der
min. Elo-Auswertung im Statistik-Fenster. Die Decaden sind fest eingestellt, bitte hier die Jahres-Option benutzen.
Mit dem "Aktualisieren"-Knopf kann man sofort eine neue Auswertung erstellen, ohne den Dialog verlassen zu müssen.
<br>
Bitte beachten: Bei kleinen Intervallen und großem Bereich, kann die Auswertung etwas länger dauern und die Übersichtlichkeit geht verloren.
</p>
<p>

Wenn die Filtergrafik nach Elo-Zahl ausgegeben wird, verwendet Scid
für jede Partie die durchschnittliche (mittlere) Wertung. Geschätzte
Elo-Zahlen (wie z.B. aus der Schreibkorrekturdatei) werden nicht verwendet.
Falls in einer Partie nur einer der beiden Spieler eine Elo-Zahl hat,
wird angenommen, daß der Gegner die gleiche Elo-Zahl besitzt bis zu
einer Höchstgrenze von 2200. Wenn also beispielsweise ein Spieler eine
Elo-Zahl von 2500 hat und sein Gegner keine Elo-Zahl, ist die mittlere
Wertungszahl (2500+2200)/2 = 2350.
</p>

<h3><name Rating>ELO-Zahl-Verlauf</name></h3>
<p>
Das Fenster <term>ELO-Zahl-Verlauf</term> zeigt die Entwicklung der
Elo-Zahl(en) eines Spieler oder beider Spieler der aktuellen Partie.
Sie können den Graphen für einen einzelnen Spieler mit dem Schalter
<term>ELO-Zahl-Verlauf</term> im Fenster <a PInfo>Spielerinformation</a>
erzeugen oder für beide Spieler der aktuellen Partie, indem Sie
<term>ELO-Zahl-Verlauf</term> im <menu>Werkzeuge</menu>-Menü auswählen.
</p>

<h3><name Score>Partiebewertung</name></h3>
<p>
Das Fenster <term>Partiebewertung</term> zeigt die numerische
Bewertung (Stand) der aktuellen Partie, wie sie in den Kommentaren
gespeichert ist, als Grafik an.
Sie können mit der linken Maustaste irgendwo auf den Graphen klicken,
um zu der korrespondierenden Partiestellung zu gelangen.
</p>
  <p>
  Es werden zwei Arten von Bewertungskommentaren erkannt: 
  <ul>
  Alle Kommentare, die eine Vorzeichen behaftete Zahl enthalten, z.B. 
  <li><b>+1.23, -0.23</b></li>. Gibt es mehrere Zahlen im Kommentar wird nur die erste verwendet.
  </ul>
  <ul>
  Tags mit dem Namen [%eval .. ]
  <li><b>[%eval -6.64/23]</b></li>
  </ul>
  </p>
  <p>
    Einige Partien werden nur aus der Perspektive der Weißen angezeigt. Sie können die Punktzahl für Weiß oder Schwarz invertieren.
    Um dies zu korrigieren können Sie im Optionsmenü die zu intertierende Seite auswählen: <b>Weiß</b> und/oder <b>Schwarz</b>.
  </p>
  <p>
    In einem <term>Zeitdiagramm</term> kann die Zeit für den Zeitverbrauch pro Zug oder die verbleibende Zeit auf der Uhr kann ebenfalls dargestellt werden. Für die Auswertung der Zeit müssen im Kommentar die Tags [%clk 0:01:19] und [%emt 0:00:19] enthalten sein.
    Die Werte werden immer in Minuten angezeigt.
    Über das Menü können Sie auswählen, welche Kombination der Diagramme angezeigt werden soll:
    <li><b>Beide:</b> Die Partiebewertung und die Zeit werden angezeigt.
    <li><b>Zeit:</b> Nur die Zeit wird angezeigt ("Beide" muss abgewählt werden!)</li>
    <li><b>Summe:</b> In der Zeitanzeige wird die Summe der verbrauchten Zeit angezeigt, wenn die Zeit als [%emt 00:12] zur Verfügung steht.
  </p>
<h3><name Tree>Zugbaumgrafik</name></h3>
<p>
Das Fenster <term>Zugbaumgrafik</term> ist aus dem Zugbaumfenster
zu erreichen. Es zeigt die Erfolgsrate der häufigsten Züge in der
aktuellen Position. Weitere Informationen finden Sie auf der
<a Tree Graph>Zugbaum</a>-Hilfeseite.
</p>

<p><footer>(Aktualisiert: Scid 4.7, Juni 2018)</footer></p>
}


####################
### Tablebases help:

set helpTitle(D,TB) "Endspieltabellen"
set helpText(D,TB) {<h1>Endspieltabellen</h1>

<p>
Eine <term>Endspieltabelle</term> ist eine Datei, die die vollständige
Ergebnisinformation über alle Positionen einer bestimmten
Materialkonstellation enthält, wie z.B. König und Turm gegen König und
Bauer. Es gibt Endspieltabellen für alle Materialkonstellation mit bis
zu 5 Steinen (inkl. Könige), und einige einfache 6-Steine-Tabellen
sind ebenfalls verfügbar.
</p>
<p>
Scid kann Endspieltabellen im Nalimov-Format verwenden, die von vielen
modernen Schachprogrammen genutzt werden. Sie haben oftmals die
Dateiendung <b>.nbw.emd</b>  oder <b>.nbb.emd</b>.
</p>

<h3>Endspieltabellen in Scid verwenden</h3>
<p>
Um die Endspieltabellendateien in Scid zu verwenden, bestimmen Sie
einfach ihre Verzeichnisse mit <b>Endspieltabellen-Verzeichnis...</b>
im <menu>Optionen</menu>-Menü. Sie können bis zu 4 Verzeichnisse
angeben, wo Ihre Endspieltabellendateien gespeichert sind. Mit dem
Schalter <b>...</b> rechts nebem dem Datei-Eingabefeld können Sie
eine Datei suchen, deren Verzeichnis verwendet werden soll.
</p>
<p>
Wenn eine in der Endspieltabelle gefundene Stellung erreicht wurde, zeigt
der Informationsbereich (unterhalb des Schachbretts)
Endspieltabellen-Informationen an. Sie können den Umfang der angezeigten
Information konfigurieren, indem Sie mit der rechten Maustaste diesen
Bereich anklicken oder im <menu>Optionen</menu>-Menü den Menüpunkt
<b>Partieinformation</b> wählen. Die Option "Ergebnis und bester Zug"
liefert die nützlichste Information, ist aber oftmals sehr viel
langsamer als die Option "nur Ergebnis".
</p>

<h3>Das Endspieltabellenfenster</h3>
<p>
Sie können sogar noch mehr Endspieltabellen-Informationen zur aktuellen
Position erhalten, indem Sie das <term>Endspieltabellenfenster</term>
öffnen (<menu>Fenster</menu>-Menü, Tastenkombination Strg+Umschalt+[=]).
Dieses Fenster zeigt das Ergebnis jedes zulässigen Zuges in der aktuellen
Stellung bei perfektem Spiel.
</p>
<p>
Das Fenster enthält zwei Bereiche. Der Übersichtsbereich (links) zeigt
die Endspieltabellen, die Scid auf Ihrem Rechner gefunden hat, mit einer
Inhaltsangabe jeder Tabelle. Der Ergebnisbereich (rechts) zeigt die
optimalen Ergebnisse aller Züge in der aktuellen, im Haupfenster
dargestellten Position.
</p>

<h4>Der Übersichtsbereich</h4>
<p>
Im oberen Teil des Übersichtsbereichs können Sie eine bestimmte
Endspieltabelle auswählen. Die verfügbaren Tabellen werden blau
angezeigt, die übrigen grau, aber Sie können jede Tabelle auswählen.
Der untere Teil des Übersichtsbereichs zeigt eine Inhaltsangabe
der Informationen der ausgewählten Endspieltabelle. (Noch haben nicht
alle Endspieltabellen einen entsprechenden Datensatz in Scid.)
</p>
<p>
Diese Inhaltsangabe enthält die Häufigkeit (wie viele Partien von
einer Million haben eine Stellung mit diesem Material erreicht,
errechnet anhand einer Datenbank mit über 600.000 Meisterpartien),
das späteste Matt einer Seite sowie Anzahl wechselseitiger ("umgekehrter")
Zugzwangpositionen. Eine wechselseitige Zugzwangposition liegt dann vor,
wenn Weiß am Zug zum Remis führt und Schwarz am Zug verliert, wenn
Weiß am Zug verliert und Schwarz am Zug Remis ergibt oder wenn derjenige
am Zug verliert.
</p>
<p>
Bei einigen Tabellen mit wechselseitigem Zugzwang enthält die Übersicht
eine Liste aller Zugzwangpositionen oder eine Auswahl davon. Eine
vollständige Liste für jede Endspieltabelle ist nicht machbar, da einige
Tabellen tausende von Zugzwangstellungen enthalten.
</p>
<p>
Mit dem Schalter <b>Random</b> können Sie eine zufällige Position aus
der ausgewählten Endspieltabelle aufbauen.
</p>

<h4>Der Ergebnisbereich</h4>
<p>
Der Ergebnisbereich wird immer dann aktualisiert, wenn sich die
Brettstellung im Hauptfenster ändert. Die erste Zeile zeigt an, wie viele
Züge gewinnen (+), zum Remis führen (=), verlieren (-) oder ein nicht
bekanntes Ergebnis (?) haben. Der übrige Teil liefert Ihnen eine
detailliertere Ergebnisliste, mit der Reihenfolge kürzeste bis längste
Matts, danach Remis, dann längste bis kürzeste Verluste. Länge jeweils
bis zum Matt.
</p>

<h4>Das Ergebnisbrett</h4>
<p>
In einer Endspieltabellenstellung ist es oft nützlich zu wissen,
wie die Ergebnisse aussähen, wenn alle Steine in ihrer gegenwärtigen
blieben, aber ein bestimmter Stein woanders stünde. Zum Beispiel
wenn Sie feststellen möchten, wie nah ein König einem Freibauern sein
muß, um eine bestimmte Stellung zu gewinnen oder Remis zu machen.
In Endspielbüchern wird diese Information oft die <i>Gewinnzone</i>
bzw. <i>Remiszone</i> eines Steins in einer bestimmten Stellung
genannt.
</p>
<p>
Mit Hilfe von Scid können Sie diese Information erhalten, wenn Sie
den Schalter mit dem Schachbrett anklicken, um das
<term>Ergebnisbrett</term> im Endspieltabellenfenster zu zeigen.
Wenn Sie mit der linken Maustaste irgendeinen Stein auf dem Brett
anklicken, erscheint auf jedem freien Feld ein Symbol mit dem Resultat
der Endspieltabelle, wenn der Stein auf diesem Feld stünde (mit derselben
Seite am Zug wie in der aktuellen Position des Hauptfensters).
</p>
<p>
Ein Feld kann fünf mögliche Symbole haben:
ein weißes "<b>#</b>" bedeutet, Weiß gewinnt;
ein schwarzes "<b>#</b>" bedeutet, Schwarz gewinnt;
ein blaues "<b>=</b>" bedeutet, die Stellung ist remis;
ein rotes "<b>X</b>" bedeutet, die Position ist illegal (weil die Könige
nebeneinander stehen oder die Seite am Zug Schach bietet); und
ein rotes "<b>?</b>" heißt, das Result ist unbekannt, weil die nötige
Endspieltabellendatei nicht zur Verfügung steht.
</p>

<h3>Endspieltabellen-Quellen</h3>
<p>
Zur Hilfe beim Auffinden von Endspieltabellendateien im Internet
siehe die entsprechenden <a Author Related>Links</a>.
</p>

<p><footer>(Aktualisiert: Scid 3.4, September 2002)</footer></p>
}


###################
### Bookmarks help:

set helpTitle(D,Bookmarks) "Lesezeichen"
set helpText(D,Bookmarks) {<h1>Lesezeichen</h1>
<p>
Scid erlaubt Ihnen, wichtige Partien für einen leichten späteren Zugriff
mit <term>Lesezeichen</term> zu markieren. Das Lesezeichen-Menü ist aus
dem <menu>Datei</menu>-Menü, der Werkzeugleiste oder mit der
Tastenkombination <B>Strg+B</b> zu erreichen.
</p>
<p>
Wenn Sie eine markierte Partie aus dem Lesezeichen-Menü wählen, öffnet
Scid falls nötig die Datenbank, findet die Partie und geht zu der
Partiestellung, die mit dem Lesezeichen versehen wurde.
</p>
<p>
Nur Partien in Datenbanken im Scid-Format (keine PGN-Datei, nicht die
Ablage-Datenbank) können mit einem Lesezeichen markiert werden.
</p>
<p>
Wenn die Datenbank einer markierten Partie sortiert oder komprimiert
wurde, sind die Lesezeichendaten möglicherweise veraltet. Falls das
passiert, durchsucht Scid die Datenbank nach der passendsten Partie
(vergleicht Spielernamen, Ort etc.), wenn das Lesezeichen ausgewählt
wird, die markierte Datei sollte also immer noch geladen werden. Wenn
sich allerdings Daten der markierten Partie ändern, ist es möglich,
daß eine andere Partie besser auf die Lesezeichendaten paßt und statt
dessen geladen wird. Es ist also eine gute Idee, eine Partie erneut zu
markieren, wenn Sie ihre Spieler-, Orts-, Ergebnis-, Runden- oder
Jahresdaten bearbeiten.
</p>

<h3>Lesezeichen bearbeiten</h3>
<p>
Mit dem Lesezeichen-Editor können Sie den für jede markierte Partie
ausgegebenen Menütext ändern und Verzeichnisse erstellen, um die
Lesezeichen zu kategorisieren.
</p>

<h3>Hinweise</h3>
<p>
Sie können Lesezeichen zum schnellen Zugriff auf Datenbanken verwenden,
die Sie oft benutzen, indem Sie eine Partie jeder Datenbank markieren.
Eine weitere gute Verwendungsmöglichkeit für Lesezeichen ist es,
wichtige Partien hinzuzufügen, die Sie beim Studieren einer bestimmten
Schacheröffnung finden.
</p>
<p>
Das Lesezeichen-Menü enthält einen Eintrag zur Kontrolle der
Verzeichnisdarstellung: sie können als Untermenüs angezeigt werden
(nützlich bei sehr vielen Lesezeichen) oder als einzelne Liste.
</p>

<p><footer>(Aktualisiert: Scid 3.0, November 2001)</footer></p>
}


##############################
### Command-line options help:

set helpTitle(D,Cmdline) "Kommandozeilen-Optionen"
set helpText(D,Cmdline) {<h1>Kommandozeilen-Optionen</h1>
<p>
Wenn Sie Scid von der Shell oder Konsole starten, können Sie
Kommandozeilen-Optionen spezifizieren. Datenbanken im Scid-Format (mit
oder ohne Dateiendung wie z.B. ".si3") und PGN-Dateien, die geöffnet
werden sollen, können angegeben werden. Beispiel:
<ul>
<li> <b> scid datenbank partien.pgn</b></li>
</ul>
startet Scid und und öffnet die Scid-Datenbank "datenbank" und die
PGN-Datei "partien.pgn".
</p>
<p>
Außerdem gibt es optionale Argumente, die kontrollieren, welche Dateien
Scid suchen und beim Start verwenden soll. Sie können die Benutzung
von <a TB>Endspieltabellen</a>  mit der Option <b>-xtb</b> (oder
<b>-xt</b>) ausschalten, das Laden der Datei zur
<a ECO>ECO-Eröffnungsklassifikation</a> mit <b>-xeco</b> oder
<b>-xe</b> verhindern und mit <b>-xspell</b> oder <b>-xs</b> vermeiden,
daß die Datei zur <a Maintenance Spellcheck>Schreibkorrektur</a>
geladen wird. Die Option <b>-fast</b> oder <b>-f</b> bewirkt das in
einem, d.h. <b>scid -f</b> ist äquivalent zu
<b>scid -xeco -xspell -xtb</b>.
</p>

<p><footer>(Aktualisiert: Scid 3.1, November 2001)</footer></p>
}


####################
### Compaction help:

set helpTitle(D,Compact) "Datenbank komprimieren"
set helpText(D,Compact) {<h1>Datenbank komprimieren</h1>
<p>
Datenbank-<term>Komprimierung</term> ist ein besonderer Teil der
<a Maintenance>Wartung</a>, der die Datenbank so klein und effizient
wie möglich hält.
Eine Datenbank komprimieren heißt, jeden ungenutzten Bereich in ihren
Dateien zu löschen.
Es gibt zwei Arten: Namens- und Partiendatenbank-Komprimierung.
</p>

<h3>Namendatenbank-Komprimierung</h3>
<p>
Nach einer gewissen Zeit werden Sie evtl. feststellen, daß die Datenbank
eine Reihe von Spieler-, Turnier- oder Rundennamen enthält, die in keiner
Partie mehr verwendet werden. Das wird regelmäßig nach Namenskorrekturen
der Fall sein. Die unbenutzten Namen verschwenden Speicherplatz in der
Namendatei und können die Suche nach Namen verlangsamen.
Namendatenbank-Komprimierung löscht alle Namen, die nicht in irgendeiner
Partie verwendet werden.
</p>

<h3>Partiendatenbank-Komprimierung</h3>
<p>
Immer, wenn eine Partie ersetzt oder gelöscht wird, verbleibt
ungenutzter Platz in der Partiendatei (die größte der drei Dateien
einer Scid-Datenbank). Partiendatenbank-Komprimierung löscht jeden
ungenutzten Speicher, keine gelöschte Datei bleibt in der Datenbank.
Beachten Sie, daß diese Operation unumkehrbar ist: nach der
Komprimierung sind die gelöschten Dateien für immer verschwunden!
</p>
<p>
Partiendatenbank-Komprimierung ist auch empfehlenswert nach dem
<a Sorting>Sortieren</a> einer Datenbank, um die Reihenfolge
innerhalb der Partiendatei mit der sortierten Indexdatei synchron
zu halten.
</p>

<p><footer>(Aktualisiert: Scid 2.5, Juni 2001)</footer></p>
}


####################################
### Database maintenance tools help:

set helpTitle(D,Maintenance) "Datenbank-Wartung"
set helpText(D,Maintenance) {<h1>Datenbank-Wartung</h1>
<p>
Scid stellt einige Funktionen zur Datenbank-Wartung zur Verfügung,
die aus dem <a Menus File>Datei</a>-Menü zu erreichen sind. Die
Datenbank-<a Compact>Komprimierung</a> und -<a Sorting>Sortierung</a>
werden in separaten Hilfeseiten erklärt.
</p>

<h3>Das Wartungsfenster</h3>
<p>
Die meisten Datenbank-Wartungsarbeiten in Scid können aus dem
Wartungsfenster erledigt werden, welches man mit dem Menü
<menu>Datei: Wartungsfenster</menu> oder <menu>Fenster</menu>
oder der Tastenkombination <b>Strg+M</b> öffnen kann.
</p>
<p>
Sie können in diesem Fenster <a Flags>Partie-Kennzeichen</a>
bearbeiten, Schreibkorrekturen durchführen, eine Datenbank
<a Compact>komprimieren</a> oder <a Sorting>sortieren</a>.
Alle Operationen, die für die aktuelle Datenbank nicht zur Verfügung
stehen (z.B., weil sie schreibgeschützt oder eine PGN-Datei ist),
werden durch graue Schrift angezeigt.
</p>

<h3><name Twins>Dubletten löschen</name></h3>
<p>
Das Menü <menu>Datei: Wartung</menu> hat ein Kommando <menu>Dubletten
löschen...</menu>, um Kopien von Partien (Dubletten) in der Datenbank
zu entdecken. Dieses Kommando findet alle Paare von doppelten Partien
und markiert die kürzere Partie als gelöscht, während die längere Partie
erhalten bleibt. Zwei Partien werden als gleich angesehen, wenn ihre
Spieler (und alle anderen optionalen Partieeinträge) exakt übereinstimmen.
</p>
<p>
Wenn Sie die Option "gleiche Züge" angeben, müssen zwei Partien die
gleichen Partiezüge bis zur Länge der kürzeren Partie haben (oder bis
zum 60. Zug, je nachdem, was zuerst eintritt), um Dubletten zu sein.
</p>
<p>
Wenn Sie Dubletten gelöscht haben, ist es eine gute Idee zu überprüfen,
ob jede gelöschte Partie tatsächlich Kopie einer anderen Partie ist.
Das können Sie ganz einfach machen, wenn Sie die Option "<b>Filter auf
Dubletten setzen</b>" im Dialogfenster "Dubletten löschen" gewählt
haben. Der Filter wird dann alle gelöschten Partien enthalten. Sie können
sie mit dem <term>Dublettenprüfer</term> (erreichbar aus dem
Wartungsmenü oder mit der Tastenkombination <b>Strg+Umsch+T</b>)
betrachten (blättern mit den Tasten <b>p</b> und <b>n</b>), um zu
verifizieren, daß jede Partie deshalb gelöscht wurde, weil sie
tatsächlich die Dublette einer anderen Partie ist.
</p>

<h3><name Editing>Spieler, Turniere, Orte und Runden editieren</name></h3>
<p>
Sie haben vielleicht falsch geschriebene Namen in Iher Datenbank und
wollen sie korrigieren. Sie können das in Scid mit dem
<term>Namenseditor</term> (Tastenkombination: <b>Strg+Umsch+N</b>), den
Sie aus dem Untermenü <menu>Datei: Wartung</menu> erreichen.
</p>
<p>
Jeder eindeutige Name ist nur einmal in der Namendatei gespeichert,
mit einer Namensänderung werden daher tatsächlich alle Vorkommen
geändert.
</p>

<h3><name Spellcheck>Schreibkorrektur</name></h3>
<p>
Zu Scid gehört eine <term>Schreibkorrektur</term>-Datei namens
<b>spelling.ssp</b>, um Namen von Spielern, Turnieren, Orten und Runden
zu korrigieren.
Scid versucht die Datei beim Programmstart zu laden; wenn Scid sie
nicht laden kann, können Sie sie aus dem <menu>Optionen</menu>-Menü
laden.
</p>
<p>
Wenn die Korrekturdatei erst einmal geladen ist, können Sie sie mit
den Schreibkorrektur-Befehlen im Menü <menu>Datei: Wartung</menu>  oder
aus dem Wartungsfenster auf eine Datenbank anwenden.
</p>
<p>
Wenn Sie für eine Datenbank eine Schreibkorrektur durchführen, erzeugt
Scid eine Liste mit Korrekturen, die Sie editieren können, bevor die
Korrekturen tatsächlich ausgeführt werden. Sie können also jede
nicht gewollte Korrektur löschen.
</p>
<p>
Schreibkorrekturen sind insbesondere nützlich, um eine Datenbank zu
standardisieren, damit alle Vorkommen eines bestimmten Spielers gleich
geschrieben werden.
Beispielsweise würden mit der Standarddatei die Namen"Kramnik,V.",
"Vladimir Kramnik" und "V. Kramnik" alle zu "Kramnik, Vladimir"
berichtigt.
</p>
<p>
Die Schreibkorrekturdatei hat einen weiteren Nutzen: wenn sie geladen
ist, werden ihre Spielerdaten genutzt, um die Fenster
<a PInfo>Spielerinformation</a> und <a Crosstable>Kreuztabelle</a>
zu erweitern: Sie sehen dann die FIDE-Titel (<b>gm</b> = Internationaler
Großmeister, <b>im</b> = Internationaler Meister etc.) und
Länderinformationen zu jedem Spieler, der in der Datei enthalten ist.
Über 6500 starke Spieler der Vergangenheit und Gegenwart sind in der
zu Scid gehörenden Datei <b>spelling.ssp</b> enthalten.
</p>

<h3><name Ratings>Elo-Zahlen zu den Partien hinzufügen</name></h3>
<p>
Der Schalter "Elo-Zahlen hinzufügen..." im Wartungsfenster veranlaßt
Scid, die aktuelle Datenbank nach Spielern, zu durchsuchen, die keine
Elo-Zahl haben, für die die Schreibkorrekturdatei aber eine Elo-Zahl
des jeweiligen Spielers zum Zeitpunkt der Partie enthält. Scid ergänzt
alle diese Elo-Zahlen automatisch. Das ist sehr nützlich für eine
Datenbank mit Meisterpartien, die nur wenige Elo-Angaben enthält.
</p>
<p>
Die mit Scid gelieferte Datei "spelling.ssp" enthält die für diese
Funktion benötigten Elo-Zahlen nicht, aber eine größere Version von
ihr namens "ratings.ssp" steht auf der  <a Author>Scid-Website</a>
zur Verfügung.
</p>

<h3><name Cleaner>Der Bereiniger</name></h3>
<p>
Der <term>Bereiniger</term> von Scid (vom Wartungsfenster zu erreichen)
ist ein Werkzeug, um eine Reihe von Wartungsarbeiten für eine Datenbank
in einem durchzuführen. Sie können wählen, welche Aufgaben Sie erledigen
wollen, und Scid führt sie für die Datenbank aus, ohne weitere Eingaben
des Benutzers zu benötigen. Das ist besonders bei der Wartung sehr
großer Datenbanken hilfreich.
</p>

<h3>Partien automatisch laden</h3>
<p>
Der <term>Automatische Lader</term> lädt jedesmal, wenn Sie die
Datenbank öffnen, eine Partie automatisch. Um die Partie, die geladen
wird, zu ändern, wählen Sie den Schalter "Automatisch Partie Nr.
laden..." Wenn Sie jedesmal die letzte Partie der Datenbank geöffnet
haben wollen (unabhängig von der Anzahl der Partien in der Datenbank),
wählen Sie einfach eine besonders große Zahl wie 9999999.
</p>

<p><footer>(Aktualisiert: Scid 3.1, Dezember 2001)</footer></p>
}


##############
### ECO guide:

set helpTitle(D,ECO) "ECO-Führer"
set helpText(D,ECO) {<h1>ECO-Eröffnungsklassifikation</h1>
<p>
Scid kann Schachpartien gemäß der <b>ECO</b>- (Enzyklopädie der
Schacheröffnungen) -Eröffnungsklassifikation einordnen. Ein
Standard-ECO-Code besteht aus einem Buchstaben (A..E) gefolgt von
zwei Ziffern, so daß es 500 verschiedene Standard-ECO-Codes gibt.
</p>

<h3>Scids Erweiterung des ECO-Systems</h3>
<p>
Das ECO-System ist sehr limitiert und für moderne Partien nicht
ausreichend: einige der 500 Codes sieht man praktisch überhaupt nicht
mehr, während einige andere sehr häufig vorkommen. Um diese Situation
zu verbessern, erlaubt Scid eine optionale Erweiterung der
ECO-Basiscodes: jeder Code kann um einen Buchstaben (a..z) erweitert
werden, wobei eine weitere Ergänzung (noch eine Ziffer, 1..4) möglich
ist, aber noch nicht in Scids Standard-ECO-Datei benutzt wird.
Ein erweiterter Scid-ECO-Code sieht also etwa aus wie "<b>A41e</b>"
oder "<b>E99b2</b>". Viele der in modernen Meisterpartien vorkommenden
üblichen ECO-Codes haben eine in Scids ECO-Datei definierte
Erweiterung.
</p>

<h3><name Browser>Die ECO-Auswertung</name></h3>
<p>
Das Fenster <term>ECO-Auswertung</term> zeigt Ihnen die Positionen,
die zur Klassifizierung jedes ECO-Codes verwendet werden, sowie die
Häufigkeit und Erfolgsrate der ECO-Codes in der der aktuellen Datenbank.
</p>
<p>
Der obere Teil zeigt die Häufigkeit jedes ECO-Codes in der aktuellen
Datenbank. Die Balken in der Grafik haben drei Bereiche: der unterste
(hellste Farbe) steht für die Anzahl der Weißsiege, der mittlere für
die Zahl der Remis und der oberste (dunkelste) für die Anzahl der
Schwarzsiege. Damit können Sie auf einen Blick die Charakteristik einer
Eröffnung erkennen: z.B., ob Weiß sehr erfolgreich ist oder ob Remis
häufig vorkommen.
</p>
<p>
Um zu einem tieferen ECO-Niveau zu gehen, klicken Sie mit der linken
Maustaste auf einen Balken in der Grafik (oder tippen Sie den
korrespondierenden Buchstaben (bzw. Ziffer) ein). Um auf ein höheres
Niveau zurückzugehen, klicken Sie mit der rechten Maustaste irgendwo
auf die Grafik oder drücken Sie die linke Cursortaste (oder [Entf]
oder [Rück]).
</p>
<p>
Der untere Teil zeigt die Positionen, die ein bestimmter ECO-Code
enthält, und zwar entsprechend der von Ihnen geladenen ECO-Datei.
</p>

<h3>Die ECO-Datei laden</h3>
<p>
Die zu Scid gehörende ECO-Datei heißt <b>scid.eco</b>, und Scid
versucht sie beim Programmstart zu laden.
Falls Scid sie nicht findet, müssen Sie folgendes tun, um die
ECO-Klassifikation zu ermöglichen:
<ul>
<li>(a) Mit dem Menüpunkt <menu>Optionen: ECO-Datei laden</menu>
        wählen Sie die Datei <b>scid.eco</b>. </li>
<li>(b) Speichern Sie die Optionen (im <menu>Optionen</menu>-Menü). </li>
</ul>
Nachdem Sie das getan haben, wird die ECO-Datei jedesmal geladen, wenn
Sie Scid starten.
</p>

<h3>Das ECO-Code-System</h3>
<p>
Die Basisstruktur des ECO-Systems ist folgende:
</p>
<p>
<b><blue><run ::windows::eco::Refresh A>A</run></blue></b>
    1.d4 Sf6 2...;  1.d4 ...;  1.c4;  1.Verschiedenes
<ul>
<li>  <b>A0</b>  1.<i>Verschiedenes</i>
      (<b>A02-A03</b> 1.f4: <i>Bird-Eröffnung</i>,
      <b>A04-A09</b>  1.Sf3: <i>Reti, Königsindischer Angriff</i>) </li>
<li>  <b>A1</b>  1.c4 ...: <i>Englisch</i> </li>
<li>  <b>A2</b>  1.c4 e5: <i>Englisch, Königsbauer</i> </li>
<li>  <b>A3</b>  1.c4 c5: <i>Englisch, Symmetrisch</i> </li>
<li>  <b>A4</b>  1.d4 ...: <i>Damenbauer</i> </li>
<li>  <b>A5</b>  1.d4 Sf6 2.c4 ..: <i>Indische Verteidigung </i> </li>
<li>  <b>A6</b>  1.d4 Sf6 2.c4 c5 3.d5 e6: <i>Modernes Benoni </i> </li>
<li>  <b>A7</b>  A6 + 4.Sc3 exd5 5.cxd5 d6 6.e4 g6 7.Sf3 </li>
<li>  <b>A8</b>  1.d4 f5: <i>Holländische Verteidigung</i> </li>
<li>  <b>A9</b>  1.d4 f5 2.c4 e6: <i>Holländische Verteidigung</i> </li>
</ul>

<p>
<b><blue><run ::windows::eco::Refresh B>B</run></blue></b>   1.e4 c5;  1.e4 c6;  1.e4 d6;  1.e4 <i>Verschidenes</i>
<ul>
<li>  <b>B0</b>  1.e4 ...
      (<b>B02-B05</b>  1.e4 Sf6: <i>Aljechin-Verteidigung</i>;
      <b>B07-B09</b>  1.e4 d6: <i>Pirc</i>) </li>
<li>  <b>B1</b>  1.e4 c6: <i>Caro-Kann</i> </li>
<li>  <b>B2</b>  1.e4 c5: <i>Sizilianische Verteidigung</i> </li>
<li>  <b>B3</b>  1.e4 c5 2.Sf3 Nc6: <i>Sizilianisch</i> </li>
<li>  <b>B4</b>  1.e4 c5 2.Sf3 e6: <i>Sizilianisch</i> </li>
<li>  <b>B5</b>  1.e4 c5 2.Sf3 d6: <i>Sizilianisch</i> </li>
<li>  <b>B6</b>  B5 + 3.d4 cxd4 4.Sxd4 Sf6 5.Sc3 Sc6 </li>
<li>  <b>B7</b>  B5 + 4.Sxd4 Sf6 5.Sc3 g6: <i>Sizilianisch, Drachen</i> </li>
<li>  <b>B8</b>  B5 + 4.Sxd4 Sf6 5.Sc3 e6: <i>Sizilianisch, Scheveninger</i> </li>
<li>  <b>B9</b>  B5 + 4.Sxd4 Sf6 5.Sc3 a6: <i>Sizilianisch, Najdorf</i> </li>
</ul>

<p>
<b><blue><run ::windows::eco::Refresh C>C</run></blue></b>   1.e4 e5;  1.e4 e6
<ul>
<li>  <b>C0</b>  1.e4 e6: <i>Französische Verteidigung</i> </li>
<li>  <b>C1</b>  1.e4 e6 2.d4 d5 3.Sc3: <i>Französisch, Winawer/Klassisch</i> </li>
<li>  <b>C2</b>  1.e4 e5: <i>Offene Partie</i> </li>
<li>  <b>C3</b>  1.e4 e5 2.f4: <i>Königsgambit</i> </li>
<li>  <b>C4</b>  1.e4 e5 2.Sf3: <i>Offene Partie</i> </li>
<li>  <b>C5</b>  1.e4 e5 2.Sf3 Sc6 3.Lc4: <i>Italienisch; Zweispringerspiel</i> </li>
<li>  <b>C6</b>  1.e4 e5 2.Sf3 Sc6 3.Lb5: <i>Spanische Partie</i> </li>
<li>  <b>C7</b>  1.e4 e5 2.Sf3 Sc6 3.Lb5 a6 4.La4: <i>Spanisch</i> </li>
<li>  <b>C8</b>  C7 + 4...Sf6 5.0-0: <i>Spanisch, Geschlossen und Offen</i>
      (<b>C80-C83</b>  5.0-0 Sxe4: <i>Spanisch, Offenes System</i>;
      <b>C84-C89</b>  5.0-0 Le7: <i>Spanisch, Geschlossenes System</i>) </li>
<li>  <b>C9</b>  C8 + 5...Le7 6.Te1 b5 7.Lb3 d6: <i>Spanisch, Geschlossen</i> </li>
</ul>

<p>
<b><blue><run ::windows::eco::Refresh D>D</run></blue></b>   1.d4 d5; 1.d4 Sf6 2.c4 g6 with 3...d5
<ul>
<li>  <b>D0</b>   1.d4 d5: <i>Damenbauernspiele</i> </li>
<li>  <b>D1</b>   1.d4 d5 2.c4 c6: <i>Slawisch</i> </li>
<li>  <b>D2</b>  1.d4 d5 2.c4 dxc4: <i>Angenommenes Damengambit</i> </li>
<li>  <b>D3</b>  1.d4 d5 2.c4 e6: <i>Abgelehntes Damengambit</i> </li>
<li>  <b>D4</b>  D3 + 3.Sc3 Sf6 4.Sf3 c5/c6: <i>Semi-Tarrasch; Semi-Slawisch</i> </li>
<li>  <b>D5</b>  D3 + 3.Sc3 Sf6 4.Lg5: <i>Klassisches Damengambit</i> </li>
<li>  <b>D6</b>  D5 + 4...Le7 5.e3 0-0 6.Sf3 Sbd7: <i>Orthodoxes Damengambit</i> </li>
<li>  <b>D7</b>  1.d4 Sf6 2.c4 g6 with 3...d5: <i>Grünfeld-Indisch</i> </li>
<li>  <b>D8</b>  1.d4 Sf6 2.c4 g6 3.Sc3 d5: <i>Grünfeld-Indisch</i> </li>
<li>  <b>D9</b>  1.d4 Sf6 2.c4 g6 3.Sc3 d5 4.Sf3: <i>Grünfeld-Indisch</i> </li>
</ul>

<p>
<b><blue><run ::windows::eco::Refresh E>E</run></blue></b>   1.d4 Sf6 2.c4 e6; 1.d4 Sf6 2.c4 g6 </li>
<ul>
<li>  <b>E0</b>  1.d4 Sf6 2.c4 e6: <i>Katalanisch etc.</i> </li>
<li>  <b>E1</b>  1.d4 Sf6 2.c4 e6 3.Sf3 (b6): <i>Damenindisch etc.</i> </li>
<li>  <b>E2</b>  1.d4 Sf6 2.c4 e6 3.Sc3 (Lb4): <i>Nimzo-Indisch etc.</i> </li>
<li>  <b>E3</b>  E2 + 4.Lg5 or 4.Dc2: <i>Nimzo-Indisch</i> </li>
<li>  <b>E4</b>  E2 + 4.e3: <i>Nimzo-Indisch, Rubinstein</i> </li>
<li>  <b>E5</b>  E4 + 4...0-0 5.Sf3: <i>Nimzo-Indisch, Hauptvariante</i> </li>
<li>  <b>E6</b>  1.d4 Sf6 2.c4 g6: <i>Königsindisch</i> </li>
<li>  <b>E7</b>  1.d4 Sf6 2.c4 g6 3.Sc3 Lg7 4.e4: <i>Königsindisch</i> </li>
<li>  <b>E8</b>  E7 + 4...d6 5.f3: <i>Königsindisch, Sämisch</i> </li>
<li>  <b>E9</b>  E7 + 4...d6 5.Sf3: <i>Königsindisch, Hauptvarianten</i> </li>
</ul>

<p><footer>(Aktualisiert: Scid 2.5, Juni 2001)</footer></p>
}

####################
### Calvar window help:

set helpTitle(D,CalVar) "Varianten berechnen"
set helpText(D,CalVar) {<h1>Das Varianten-berechnen-Fenster</h1>
  <p>
Dieses Training ist auch als Stoyko-Übung bekannt. 
Das Ziel dieser Übung ist die Analyse einer komplexen Stellung und daraus so viele wie mögliche plausible Varianten zu ermitteln. Jeder Variante wird mit einer Bewertung versehen. 
  </p> 
<p><h3>Konfiguration</h3></p>
Drei Parameter werden eingestellt:
<ul>
<li>Die zur Analyse der verschiedenen Varianten eingesetze UCI-Engine</li>
<li>Zeit, in Sekunden, die zur Analyse der Stellung von der Engine verwendet wird.</li>
<li>Zeit, in Sekunden, die zur Analyse der vom Anwender eingegebenen Variante durch die Engine verwendet wird.</li>
</ul>
<p><h3>Eingabe der Varianten</h3></p>
Die Zugeingabe erfolgt durch Mausklicks auf die Felder des Brettes, allerdings wird auf dem Brett nichts angezeigt. Die angeklickten Felder erscheinen im Varianten-berechnen-Fenster. Jeder Variante wird am Ende mit einer Bewertung abgeschlossen: Es wird einer der Knöpfe mit den NAG-Codes gedrückt.
  <p><h3>Auswertung der Bewertung</h3></p>
Wurde eine Variante mit einer NAG-Code-Bewertung abgeschlossen, berechnet die Engine ihre Bewertung der Variante und hängt ihre Variante und Bewertung direkt an die Benutzer-Variante an. 
  <p><h3>Fertig mit der Stellung</h3></p>
Ist der Anwender der Meinung alle besten Varianten gefunden zu haben, drückt er <b>Fertig mit Stellung</b>. Es werden dann noch alle Varianten (mit dem Kommentar <b>Fehlende Varianten</b>) mit einer höheren Bewertung als die beste Variante des Anwenders hinzugefügt. 
  
  <p><footer>(Aktualisiert: Scid 3.6.21, Dezember 2007)</footer></p>
}

####################
### EPD files help:

set helpTitle(D,EPD) "EPD-Dateien"
set helpText(D,EPD) {<h1>EPD-Dateien</h1>
<p>
Eine EPD-Datei (extended position description = erweiterte
Stellungsbeschreibung) ist eine Sammlung von Positionen, wobei jeder
Position ein Text zugeordnet ist. Wie <a PGN>PGN</a> ist EPD ein
verbreiteter Standard für Schachinformationen.
</p>
<p>
Eine EPD-Datei hat eine Reihe definierter "<term>Opcodes</term>"(-Felder),
die in der Datei durch Semikolons (<b>;</b>) getrennt gespeichert sind,
in einem EPD-Fenster von Scid aber in separaten Zeilen dargestellt
werden, um das Bearbeiten zu erleichtern. Ein Semikolon innerhalb eines
EPD-Feldes wird von Scid als "<b>\s</b>" gespeichert, um es von der
Feldende-Kennung zu unterscheiden.
Jede Position und ihre zugehöriger Opcode wird in der EPD-Datei in
einer einzelnen Zeile gespeichert.
</p>
<p>
Standard-EPD-Opcodes sind u.a.:
<ul>
<li> <b>acd</b> Analyse-Zähler: Suchtiefe.</li>
<li> <b>acn</b> Analyse-Zähler: Anzahl der durchsuchten Knoten.</li>
<li> <b>acs</b> Analyse-Zähler: Suchzeit in Sekunden.</li>
<li> <b>bm</b> Beste Züge: aus irgendeinem Grund als beste
eingestufte Züge.</li>
<li> <b>ce</b> Stellungsbeurteilung: Beurteilung in Hundertstel Bauern
[ce = "centipawn evaluation"] aus Sicht der <b>am Zug</b>
befindlichen Seite -- beachten Sie, daß sich dies vom Analysefenster
unterscheidet, welches die Einschätzung in Bauerneinheiten aus der
Sicht von Weiß zeigt. </li>
<li> <b>cX</b> Kommentar (wobei <b>X</b> eine Ziffer ist, 0-9).</li>
<li> <b>eco</b> Eröffnungscode des <a ECO>ECO</a>-Systems.</li>
<li> <b>id</b> Eindeutige Identifizierung dieser Stellung.</li>
<li> <b>nic</b> Eröffnungscode des <i>New-In-Chess</i>-Systems.</li>
<li> <b>pm</b> Vorhergesagter Zug: der erste Zug der PV.</li>
<li> <b>pv</b> Vorhergesagte Variante: die beste Variante.</li>
</ul>
</p>
<p>
EPD-Dateien haben einen vielfachen Nutzen: Scid verwendet EPD-Dateien,
um Partien nach dem System der <a ECO>Enzyklopädie der
Schacheröffnungen</a> (ECO) zu klassifizieren, und Sie können eine
EPD-Datei für Ihr Eröffnungsrepertoire erstellen, mit Kommentaren
zu Stellungen, die Sie regelmäßig auf dem Brett haben.
</p>
<p>
Mit den Optionen <menu>Neu</menu> und <menu>Öffnen</menu>
im <menu>Datei</menu>-Menü können Sie eine neue EPD-Datei erstellen
bzw. eine bestehende öffnen. Maximal vier EPD-Dateien können
gleichzeitig geöffnet sein.
</p>

<h3>EPD-Fenster</h3>
<p>
Für jede geöffnete EPD-Datei sehen Sie ein Fenster, welches den Text zur
aktuellen Stellung zeigt. Sie müssen nicht den Schalter "Speichern"
betätigen, um Änderungen des Textes zu sichern; der Text wird jedesmal
gespeichert, wenn Sie zu einer anderen Partiestellung gehen.
</p>

<h3>In EPD-Dateien navigieren</h3>
<p>
Um sich durch die Stellungen in einer EPD-Datei zu bewegen, verwenden
Sie die Kommandos <menu>Nächste Position</menu> und <menu>Vorherige
Position</menu> im <menu>Werkzeuge</menu>-Menü des EPD-Fensters oder
die Tastenkombinationen <b>Strg+Ab</b> and <b>Strg+Auf</b>.
Diese Befehle gehen zur nächsten/vorherigen Position in der Datei,
wobei sie die aktuelle Partie löschen und die Ausgangsposition
herstellen.
</p>

<h3>EPD-Felder entfernen</h3>
<p>
EPD-Dateien, die Sie im Internet finden, enthalten möglicherweise
Felder, die Sie nicht interessieren und die eine Menge Speicherplatz
in der Datei verschwenden.
Beispielsweise könnte eine EPD-Datei mit Computerauswertungen die Felder
ce, acd, acn, pm, pv und id enthalten, Sie brauchen aber vielleicht
nur das ce- und pv-Feld.
</p>
<p>
Sie können einen EPD-Opcode mit dem Menüpunkt <menu>EPD-Feld
entfernen</menu> im <menu>Werkzeuge</menu>-Menü des EPD-Fensters
aus allen Stellungen in der EPD-Datei entfernen.
</p>

<h3>Die Statusanzeige des EPD-Fensters</h3>
<p>
Die Statuszeile jedes EPD-Fensters zeigt:
<ul>
<li>- den Dateistatus (<b>--</b> heißt unverändert, <b>XX</b> heißt
      verändert und <b>%%</b> heißt schreibgeschützt); </li>
<li>- den Dateinamen; </li>
<li>- die Anzahl der Positionen in der Datei; </li>
<li>- zulässige Züge in der aktuellen Stellung, die eine andere
Stellung in der EPD-Datei erreichen .</li>
</ul>

<p><footer>(Aktualisiert: Scid 2.5, Juni 2001)</footer></p>
}


###################
### Exporting help:

set helpTitle(D,Export) "Partien exportieren"
set helpText(D,Export) {<h1>Partien exportieren</h1>
<p>
Sie können die Kommandos im <menu>Werkzeuge</menu>-Menü benutzen, um die
aktuelle Partie oder alle Partien im aktuellen Filter in eine Textdatei
zu exportieren.
</p>
<p>
Drei Textdatei-Formate stehen zur Verfügung: <a PGN>PGN</a> (portable
game notation), HTML (für Webseiten) und LaTeX (ein populäres
Textsatzsystem).
</p>
<p>
Beim Exportieren können Sie wählen, eine neue Datei zu erstellen
oder die Partien einer existierenden Partiendatei hinzuzufügen.
</p>

<h3>Diagramme</h3>
<p>
Wenn Sie im HTML- oder LaTeX-Format exportieren, fügt Scid automatisch
überall dort ein Diagramm ein, wo ein Diagramm-<a NAGs>NAG</a> ("D")
oder ein mit dem Zeichen "#" beginnender <a Comment>Kommentar</a>
in der Partie vorkommt.
</p>

<h3><name Null>Nullzüge beim PGN-Export</name></h3>
<p>
Scid erlaubt es, in einer Partie <a Annotating Null>Null- (leere)
Züge</a> zu speichern, da diese beim Kommentieren mit Varianten
hilfreich sein können. Allerdings gibt es im PGN-Standard keine
Nullzüge. Wenn Sie also Scid-Partien mit Nullzügen in eine PGN-Datei
exportieren, werden andere Programme zum PGN-Lesen nicht in der Lage
sein, die Nullzüge zu lesen.
</p>
<p>
Um dieses Problem zu lösen, stellt Scid eine weitere Option bereit,
<b>Nullzüge in Kommentare umwandeln</b>, wenn Partien in PGN-Format
exportiert werden. Wenn Sie eine PGN-Datei erstellen wollen, die von
anderer Software verwendet werden kann, schalten Sie diese Option ein,
und Varianten, die Nullzüge enthalten, werden in Kommentare umgewandelt.
Wenn Sie jedoch eine PGN-Dateie erstellen wollen, die später wieder
unter Beibehaltung der Nullzüge von Scid importiert werden kann,
lassen Sie diese Option aus.
</p>

<h3>HTML-Export</h3>
<p>
Scid kann Partien in eine HTML-Datei exportieren. Um Diagramme einzufügen,
müssen die Diagrammbilder (im Verzeichnis "<b>bitmaps/</b>" mitgeliefert)
in einem Unterverzeichnis "<b>bitmaps/</b>" unterhalb des Verzeichnisses
sein, wo sich die HTML-Datei befindet.
</p>

<h3>LaTeX-Export</h3>
<p>
Scid kann Partien in eine LaTeX-Datei exportieren. Partien werden in
zwei Spalten pro Seite gedruckt und Züge in figuriner algebraischer
Notation.
</p>
<p>
Für weitere Informationen siehe die Hilfeseite <a LaTeX>Scid und LaTeX</a>.
</p>

<p><footer>(Aktualisiert: Scid 3.4, Juli 2002)</footer></p>
}


####################
### Flags help:

set helpTitle(D,Flags) "Partie-Kennzeichen"
set helpText(D,Flags) {<h1>Partie-Kennzeichen</h1>

<p>
Eine <term>Kennzeichen</term> ("flag") ist ein Kennzeichen irgendeiner
Schach-Charakteristik, die für jede Partie der Datenbank ein- oder
ausgeschaltet werden kann. Es gibt 13 vom Benutzer einstellbare
Kennzeichen, die Sie direkt für jede Partie setzen können. Von diesen
hat nur die Lösch-Kennzeichen eine besondere Bedeutung: Partien, bei
denen die Lösch-Kennzeichen eingeschaltet ist, sind zur Löschung
vorgemerkt und werden entfernt, wenn die Datenbank
<a Compact>komprimiert</a> wird.
</p>
<p>
Die anderen 12 vom Benutzer einstellbaren Kennzeichen und ihre
Symbole sind:
</p>

<ul>
<li>Eröffnung Weiß (W)</li>
<li>Eröffnung Schwarz (B)</li>
<li>Mittelspiel (M)</li>
<li>Endspiel (E)</li>
<li>Neuerung (N)</li>
<li>Bauernstruktur (P)</li>
<li>Taktik (T)</li>
<li>Spiel auf dem Damenflügel (Q)</li>
<li>Spiel auf dem Köngsflügel (K)</li>
<li>Brilliant (!)</li>
<li>Patzer (?)</li>
<li>Benutzer-definiert (U)</li>
</ul>

<p>
Eine Kennzeichen kann für die aktuelle Partie, für alle Partien im
Filter oder für alle Partien der Datenbank im
<a Maintenance>Wartungs</a>-Fenster gesetzt werden.
</p>
<p>
Sie können die <a Searches Header>Suche nach Partiedaten</a> benutzen,
um alle Datenbankpartien zu finden, die ein bestimmtes Kennzeichen ein-
oder ausgeschaltet haben, oder die Kennzeichen als Teil einer
komplexeren Suche verwenden.
</p>
<p>
Da alle vom Benutzer einstellbaren Kennzeichen keine Bedeutung für
Scid haben (mit Ausnahme der Lösch-Kennzeichen), können Sie diese für
Ihre eigenen Bedürfnisse  verwenden. Zum Beispiel könnten Sie die
Kennzeichen "Spiel auf dem Köngsflügel" (K) für Bauernstürme auf den
König verwenden, für Figurenangriffe auf den König oder auch für
Endspiele mit allen Bauern auf dem Königsflügel.
</p>

<p><footer>(Aktualisiert: Scid 4.7, Juni 2018)</footer></p>
}


###############
### LaTeX help:

set helpTitle(D,LaTeX) "Scid und LaTeX"
set helpText(D,LaTeX) {<h1>Scid und LaTeX</h1>
<p>
Scid kann Partien und Eröffnungsberichte im LaTeX-Format in Dateien
sichern. LaTeX ist eine Erweiterung von TeX, einem populären Textsatzsystem.
</p>
<p>
Um die von Scid erzeugten LaTeX-Dateien darzustellen, müssen Sie
(natürlich) LaTeX haben und das Schachzeichensatz-Paket "chess12"
installiert haben. Dieses Paket ist normalerweise nicht Teil der
LaTeX-Standardinstallation, d.h selbst wenn Sie LaTeX haben, haben
Sie vielleicht nicht den Schachzeichensatz.
</p>
<p>
Informationen zum Herunterladen und Installieren des
LaTeX-Schachzeichensatzes finden Sie auf der Seite
<url http://scid.sourceforge.net/latex.html>Using LaTeX with Scid</url>
der <url http://scid.sourceforge.net/>Scid-Website</url>.
</p>

<p><footer>(Aktualisiert: Scid 2.5, Juni 2001)</footer></p>
}


###########################
### Options and Fonts help:

set helpTitle(D,Options) "Options"
set helpText(D,Options) {<h1>Optionen und Präferenzen</h1>
<p>
Viele Optionen und Präferenzen in Scid (z.B. die Brettgröße, Farben, Schachbrettmuster,
Zeichensätze und Standardwerte) kann man im <menu>Optionen</menu>-Menü
verändern. Alle diese (und mehr, wie etwa das letzte Verzeichnis, aus
dem Sie eine Datenbank gelesen haben, und die Größe einiger Fenster)
werden in einer Optionendatei gesichert, wenn Sie im Optionen-Menü
<b>Optionen speichern</b> wählen.
Jedesmal, wenn Sie Scid starten, wird die Optionendatei geladen.
</p>
<p>
Wenn Sie mit Windows arbeiten, ist die Optionendatei <b>options.dat</b>
im Unterverzeichnis "config" der Scids Programmdatei abgelegt. Für Benutzer von
Unix-Betriebssystemen (wie Solaris oder Linux) ist die Datei
<b>~/.scid/scidrc</b>.
</p>

<h3><name MyPlayerNames>Ihre Spielernamen festlegen</name></h3>
<p>
Es kann sein, daß Sie für einen (oder mehrere) Spieler das Brett im
Haupfenster aus der Sicht dieses Spielers aufgestellt haben möchten,
wenn eine seiner Partien geladen wird. Sie können eine Liste solcher
Namen mit <b>Meine Spielernamen...</b> im Menü <menu>Optionen/Schachbrett</menu>
anlegen. In dem Dialogfenster geben Sie dann einen Spielernamen pro Zeile
ein. Jokerzeichen ("<b>?</b>" für genau ein beliebiges Zeichen, "<b>*</b>"
für eine Zeichenkette von null oder mehr Zeichen) sind erlaubt.
</p>

<h3><name Fonts>Zeichensätze einstellen</name></h3>
<p>
Scid hat drei Basis-Zeichensätze, die in den meisten Fenstern verwendet
werden, und Sie können alle drei individuell anpassen. Sie heißen
<b>normal</b>, <b>klein</b> und <b>fest</b>.
</p>
<p>
Der feste Zeichensatz sollte ein Zeichensatz mit fester Breite
(nicht-proportional) sein. Er wird für den <a Tree>Zugbaum</a> und die
<a Crosstable>Kreuztabelle</a> verwendet.
</p>

<p><footer>(Aktualisiert: Scid 3.6.21, November 2007)</footer></p>
}


####################
### Pgnscid help:

set helpTitle(D,Pgnscid) "Pgnscid"
set helpText(D,Pgnscid) {<h1>Pgnscid</h1>
<p>
<term>Pgnscid</term> ist ein separates Program, das Sie zur
Konvertierung von PGN-Dateien ("portable game notation") nach
Scid-Datenbanken benötigen.
</p>
<p>
Um eine Datei namens <i>partien.pgn</i> umzuwandeln, tippen Sie
einfach
<ul>
<li> <b>pgnscid partien.pgn</b> </li>
</ul>
ein, und die Scid-Datenbank (bestehend aus den Dateien
<i>partien.si3</i>, <i>partien.sg3</i> und <i>partien.sn3</i>) wird
erstellt.
Fehlermeldungen oder Warnungen werden in die Datei <i>partien.err</i>
geschrieben.
</p>
<p>
Wenn Sie wollen, daß die Datenbank in einem anderen Verzeichnis oder
mit einem anderen Namen erstellt wird, können Sie den Datenbanknamen
in der Kommandozeile mit angeben, zum Beispiel erstellt
<ul>
<li> <b>pgnscid partien.pgn datenbank</b> </li>
</ul>
eine Datenbank bestehend aus den Dateien <i>datenbank.si3</i>,
<i>datenbank.sg3</i> und <i>datenbank.sn3</i>.
</p>
<p>
Beachten Sie, daß pgnscid (und auch scid) mit gzip komprimierte
PGN-Dateien (z.B. <b>datenbank.pgn.gz</b>) direkt lesen kann, wenn Sie
also eine große PGN-Datei mit gzip komprimiert haben, um Speicherplatz
zu sparen, müssen Sie sie nicht zuerst dekomprimieren.
</p>

<h3>Optionen</h3>
<p>
Pgnscid akzeptiert zwei optionale Argumente vor dem Dateinamen:
<b>-f</b> und <b>-x</b>.
</p>
<p>
Die Option <b>-f</b> erzwingt das Überschreiben einer existierenden
Datenbank; standardmäßig wandelt pgnscid nicht in eine bereits
bestehende Datenbank um.
</p>
<p>
Die Option <b>-x</b> veranlaßt pgnscid, Text zwischen den Partien zu
ignorieren. Gemäß Voreinstellung wird Text zwischen Partien als
Kommentar vor der folgenden Partie gespeichert. Diese Option betrifft
nur Text zwischen Partien; Standardkommentare innerhalb der Partien
werden weiterhin konvertiert und gespeichert.
</p>

<h3>Spielernamen formatieren</h3>
<p>
Um die Anzahl verschiedener Namensschreibweisen desselben Spielers
zu reduzieren, werden von pgnscid einige Grundformatierungen
durchgeführt. Zum Beispiel wird die Anzahl der Leerzeichen nach einem
Komma auf eins gesetzt, Leerzeichen am Anfang oder Ende eines Namens
werden ebenso wie ein Punkt am Namensende entfernt.
Holländische Namenszusätze wie "van den" und "Van Der" werden ebenfalls
normalisiert, so daß sie ein großes "V" und kleines "d" haben.
</p>
<p>
Sie können Spieler-, Turnier-, Orts- und Rundenbezeichnungen in Scid
bearbeiten (und sogar automatische Schreibkorrekturen durchführen);
zu den Details siehe die Hilfeseite <a Maintenance Editing>Wartung</a>.
</p>

<p><footer>(Aktualisiert: Scid 2.5, Juni 2001)</footer></p>
}


#################
### Sorting help:

set helpTitle(D,Sorting) "Datenbank sortieren"
set helpText(D,Sorting) {<h1>Eine Datenbank sortieren</h1>
<p>
Die <term>Sortier</term>-Funktionen sortieren alle Partien einer
Datenbank. Sie können mehrere Sortierkriterien auswählen.
Wenn zwei Partien nach einem Kriterium gleichrangig sind, werden
sie nach dem zweiten Kriterium sortiert, und so weiter.
</p>

<h3>Sortierkriterien</h3>
<p>
Die verfügbaren Sortierkriterien sind:
</p>
<ul>
<li> Datum (älteste Partien zuerst)
<li> Jahr (wie Datum, aber nur das Jahr wird verwendet)
<li> Turnier (Ereignis)
<li> Ort
<li> Land (die letzten 3 Buchstaben des Ortes)
<li> Runde
<li> Weiß
<li> Elo-Zahl (Durchschnitt von Weiß und Schwarz, höhere zuerst)
<li> Schwarz
<li> Ergebnis (Weiß gewinnt, danach Remis, danach Schwarz gewinnt)
<li> Länge (Anzahl ganzer Züge in der Partie)
<li> ECO (Code der <a ECO>Enzyklopädie der Schacheröffnungen</a>)
</ul>

<h3>Sortierergebnis</h3>
<p>
Wenn Sie eine nicht schreibgeschützte Scid-Datenbank sortieren, wird das
Sortierergebnis gesichert, so daß die Reihenfolge der Datenbankpartien
dauerhaft geändert ist. Falls Sie nur eine temporäre Sortierung wollen,
schützen Sie die Datenbank zuerst gegen Überschreiben mit dem Menüpunkt
<menu>Datei: Schreibschutz</menu>.
</p>
<p>
Wenn Sie eine Datenbank sortieren, die schreibgeschützt ist oder aus
einer PGN-Datei besteht, kann das Sortierergebnis nicht gesichert werden,
so daß die sortierte Partiereihenfolge verloren geht, wenn die Datei
geschlossen wird.
</p>
<p>
Beachten Sie, daß das Sortieren einer Datenbank den
<a Searches Filter>Suchfilter</a> zurücksetzt auf alle Partien.
</p>

<h3>Wichtiger Hinweis zum Datenbank sortieren:</h3>
<p>
Wenn eine Datenbank sortiert wurde, ist die Indexdatei geändert, die
Partiendatei aber unverändert. Das heißt, das Sortieren einer Datenbank
hinterläßt die Partiedatensätze der Datei in einer zufälligen
Reihenfolge relativ zur Indexdatei. Das kann die <a Tree>Zugbaum</a>-,
Positions- und Material/Muster-<a Searches>Suche</a> merklich
<b>verlangsamen</b>, Sie sollten daher nach dem Datenbank sortieren
die Partiendatei durch <a Compact>Komprimieren</a> neuorganisieren,
um gute Suchleistungen zu erhalten.
</p>

<p><footer>(Aktualisiert: Scid 2.5, Juni 2001)</footer></p>
}


####################
### NAG values help:

set helpTitle(D,NAGs) "NAG-Werte"
set helpText(D,NAGs) {<h1>Standard-NAG-Werte</h1>
<p>
Standard-NAG-Werte (Numeric Annotation Glyph = "numerisches
Anmerkungszeichen"), im <a Author Related>PGN-Standard</a> definiert,
sind:
</p>
<cyan>
<ul>
<li>  1   Guter Zug (!) </li>
<li>  2   Schwacher Zug (?) </li>
<li>  3   Ausgezeichneter Zug (!!) </li>
<li>  4   Patzer / Grober Fehler (??) </li>
<li>  5   Interessanter Zug (!?) </li>
<li>  6   Zweifelhafter Zug (?!) </li>
<li>  7   Erzwungener Zug </li>
<li>  8   Der einzige Zug; keine vernünftige Alternative </li>
<li>  9   Schlechtester Zug </li>
<li> 10   Ausgeglichene Position (=) </li>
<li> 11   Gleiche Chancen, ruhige Stellung (=) </li>
<li> 12   Gleiche Chancen, aktive Stellung (=) </li>
<li> 13   Unklare Stellung (~) </li>
<li> 14   Weiß hat leichten Vorteil, steht etwas besser (+=) </li>
<li> 15   Schwarz hat leichten Vorteil, steht etwas besser (=+) </li>
<li> 16   Weiß hat Vorteil, steht besser (+/-) </li>
<li> 17   Schwarz hat Vorteil, steht besser (-/+) </li>
<li> 18   Weiß hat entscheidenden Vorteil (+-) </li>
<li> 19   Schwarz hat entscheidenden Vorteil (-+) </li>
<li> 20   Weiß hat überwältigenden Vorteil (+-) </li>
<li> 21   Schwarz hat überwältigenden Vorteil (-+) </li>
<li> 22   Weiß ist in Zugzwang </li>
<li> 23   Schwarz ist in Zugzwang </li>
<li> 24   Weiß hat leichten Raumvorteil </li>
<li> 25   Schwarz hat leichten Raumvorteil </li>
<li> 26   Weiß hat Raumvorteil </li>
<li> 27   Schwarz hat Raumvorteil </li>
<li> 28   Weiß hat entscheidenden Raumvorteil </li>
<li> 29   Schwarz hat entscheidenden Raumvorteil </li>
<li> 30   Weiß hat leichten Zeit-(Entwicklungs)vorsprung </li>
<li> 31   Schwarz hat leichten Zeit-(Entwicklungs)vorsprung </li>
<li> 32   Weiß hat Zeit-(Entwicklungs)vorsprung </li>
<li> 33   Schwarz hat Zeit-(Entwicklungs)vorsprung </li>
<li> 34   Weiß hat entscheidenden Zeit-(Entwicklungs)vorsprung </li>
<li> 35   Schwarz hat entscheidenden Zeit-(Entwicklungs)vorsprung </li>
<li> 36   Weiß hat die Initiative </li>
<li> 37   Schwarz hat die Initiative </li>
<li> 38   Weiß hat andauernde Initiative </li>
<li> 39   Schwarz hat andauernde Initiative </li>
<li> 40   Weiß hat Angriff </li>
<li> 41   Schwarz hat Angriff </li>
<li> 42   Weiß hat keine ausreichende Kompensation für das Material </li>
<li> 43   Schwarz hat keine ausreichende Kompensation für das Material </li>
<li> 44   Weiß hat ausreichende Kompensation für das Material </li>
<li> 45   Schwarz hat ausreichende Kompensation für das Material </li>
<li> 46   Weiß hat mehr als ausreichende Kompensation für das Material </li>
<li> 47   Schwarz hat mehr als ausreichende Kompensation für das Material </li>
<li> 48   Weiß hat leichtes Übergewicht im Zentrum </li>
<li> 49   Schwarz hat leichtes Übergewicht im Zentrum </li>
<li> 50   Weiß hat Übergewicht im Zentrum </li>
<li> 51   Schwarz hat Übergewicht im Zentrum </li>
<li> 52   Weiß hat entscheidendes Übergewicht im Zentrum </li>
<li> 53   Schwarz hat entscheidendes Übergewicht im Zentrum </li>
<li> 54   Weiß hat leichtes Übergewicht am Königsflügel </li>
<li> 55   Schwarz hat leichtes Übergewicht am Königsflügel </li>
<li> 56   Weiß hat Übergewicht am Königsflügel </li>
<li> 57   Schwarz hat Übergewicht am Königsflügel </li>
<li> 58   Weiß hat entscheidendes Übergewicht am Königsflügel </li>
<li> 59   Schwarz hat entscheidendes Übergewicht am Königsflügel </li>
<li> 60   Weiß hat leichtes Übergewicht am Damenflügel </li>
<li> 61   Schwarz hat leichtes Übergewicht am Damenflügel </li>
<li> 62   Weiß hat Übergewicht am Damenflügel </li>
<li> 63   Schwarz hat Übergewicht am Damenflügel </li>
<li> 64   Weiß hat entscheidendes Übergewicht am Damenflügel </li>
<li> 65   Schwarz hat entscheidendes Übergewicht am Damenflügel </li>
<li> 66   Weiß hat eine schwache Grundreihe </li>
<li> 67   Schwarz hat eine schwache Grundreihe </li>
<li> 68   Weiß hat eine gut gesicherte Grundreihe </li>
<li> 69   Schwarz hat eine gut gesicherte Grundreihe </li>
<li> 70   Weiß hat einen schlecht gesicherten König </li>
<li> 71   Schwarz hat einen schlecht gesicherten König </li>
<li> 72   Weiß hat einen gut gesicherten König </li>
<li> 73   Schwarz hat einen gut gesicherten König </li>
<li> 74   Der weiße König steht schlecht </li>
<li> 75   Der schwarze König steht schlecht </li>
<li> 76   Der weiße König steht gut </li>
<li> 77   Der schwarze König steht gut </li>
<li> 78   Weiß hat eine sehr schwache Bauernstruktur </li>
<li> 79   Schwarz hat eine sehr schwache Bauernstruktur </li>
<li> 80   Weiß hat eine schwache Bauernstruktur </li>
<li> 81   Schwarz hat eine schwache Bauernstruktur </li>
<li> 82   Weiß hat eine starke Bauernstruktur </li>
<li> 83   Schwarz hat eine starke Bauernstruktur </li>
<li> 84   Weiß hat eine sehr starke Bauernstruktur </li>
<li> 85   Schwarz hat eine sehr starke Bauernstruktur </li>
<li> 86   Der weiße Springer steht schlecht </li>
<li> 87   Der schwarze Springer steht schlecht </li>
<li> 88   Der weiße Springer steht gut </li>
<li> 89   Der schwarze Springer steht gut </li>
<li> 90   Der weiße Läufer steht schlecht </li>
<li> 91   Der schwarze Läufer steht schlecht </li>
<li> 92   Der weiße Läufer steht gut </li>
<li> 93   Der schwarze Läufer steht gut </li>
<li> 94   Der weiße Turm steht schlecht </li>
<li> 95   Der schwarze Turm steht schlecht </li>
<li> 96   Der weiße Turm steht gut </li>
<li> 97   Der schwarze Turm steht gut </li>
<li> 98   Die weiße Dame steht schlecht </li>
<li> 99   Die schwarze Dame steht schlecht </li>
<li>100   Die weiße Dame steht gut </li>
<li>101   Die schwarze Dame steht gut </li>
<li>102   Die weißen Figuren sind schlecht koordiniert </li>
<li>103   Die schwarzen Figuren sind schlecht koordiniert </li>
<li>104   Die weißen Figuren sind gut koordiniert </li>
<li>105   Die schwarzen Figuren sind gut koordiniert </li>
<li>106   Weiß hat die Eröffnung sehr schwach behandelt </li>
<li>107   Schwarz hat die Eröffnung sehr schwach behandelt </li>
<li>108   Weiß hat die Eröffnung schwach behandelt </li>
<li>109   Schwarz hat die Eröffnung schwach behandelt </li>
<li>110   Weiß hat die Eröffnung gut behandelt </li>
<li>111   Schwarz hat die Eröffnung gut behandelt </li>
<li>112   Weiß hat die Eröffnung sehr gut behandelt </li>
<li>113   Schwarz hat die Eröffnung sehr gut behandelt </li>
<li>114   Weiß hat das Mittelspiel sehr schwach behandelt </li>
<li>115   Schwarz hat das Mittelspiel sehr schwach behandelt </li>
<li>116   Weiß hat das Mittelspiel schwach behandelt </li>
<li>117   Schwarz hat das Mittelspiel schwach behandelt </li>
<li>118   Weiß hat das Mittelspiel gut behandelt </li>
<li>119   Schwarz hat das Mittelspiel gut behandelt </li>
<li>120   Weiß hat das Mittelspiel sehr gut behandelt </li>
<li>121   Schwarz hat das Mittelspiel sehr gut behandelt </li>
<li>122   Weiß hat das Endspiel sehr schwach behandelt </li>
<li>123   Schwarz hat das Endspiel sehr schwach behandelt </li>
<li>124   Weiß hat das Endspiel schwach behandelt </li>
<li>125   Schwarz hat das Endspiel schwach behandelt </li>
<li>126   Weiß hat das Endspiel gut behandelt </li>
<li>127   Schwarz hat das Endspiel gut behandelt </li>
<li>128   Weiß hat das Endspiel sehr gut behandelt </li>
<li>129   Schwarz hat das Endspiel sehr gut behandelt </li>
<li>130   Weiß hat etwas Gegenspiel </li>
<li>131   Schwarz hat etwas Gegenspiel </li>
<li>132   Weiß hat Gegenspiel </li>
<li>133   Schwarz hat Gegenspiel </li>
<li>134   Weiß hat entscheidendes Gegenspiel </li>
<li>135   Schwarz hat entscheidendes Gegenspiel </li>
<li>136   Weiß ist in leichter Zeitnot </li>
<li>137   Schwarz ist in leichter Zeitnot </li>
<li>138   Weiß ist in erheblicher Zeitnot </li>
<li>139   Schwarz ist in erheblicher Zeitnot </li>
</ul>
</cyan>

<p>
Weitere vorgeschlagene NAG-Werte für Schachinformator-Symbole sind u.a.:
</p>
<cyan>
<ul>
<li>140   Mit der Idee ... </li>
<li>141   Gerichtet gegen ... </li>
<li>142   Besser ist </li>
<li>143   Schlechter ist </li>
<li>144   Gleichwertig ist </li>
<li>145   Anmerkung des Herausgebers ("RR") </li>
<li>146   Neuerung ("N") </li>
<li>147   Schwacher Punkt </li>
<li>148   Endspiel </li>
<li>149   Linie </li>
<li>150   Diagonale </li>
<li>151   Weiß hat das Läuferpaar </li>
<li>152   Schwarz hat das Läuferpaar </li>
<li>153   Verschiedenfarbige Läufer </li>
<li>154   Gleichfarbige Läufer </li>
</ul>
</cyan>

<p>
Andere Vorschläge sind:
</p>
<cyan>
<ul>
<li>190   Etc. </li>
<li>191   Doppelbauern </li>
<li>192   Isolierte Bauern </li>
<li>193   Verbundene Bauern </li>
<li>194   Hängende Bauern </li>
<li>195   Rückständiger Bauer </li>
</ul>
</cyan>

<p>
Von Scid definierte Symbole zum internen Gebrauch:
</p>
<cyan>
<ul>
<li>201   Diagramm ("D", manchmal auch "#") </li>
</ul>
</cyan>

<p><footer>(Aktualisiert: Scid 2.5, Juni 2001)</footer></p>
}


######################
### File formats help:

set helpTitle(D,Formats) "Datei-Formate"
set helpText(D,Formats) {<h1>Scids Datei-Formate</h1>
<p>
Scid-Datenbanken bestehen aus drei essentiellen Dateien: eine
Indexdatei, eine Namendatei und eine Partiendatei. Alle haben eine
zweibuchstabige Dateiendung, beginnend mit "s": ".si" für Indexdateien,
".sn" für Namendateien und ".sg" für Partiendateien.
</p>

<h3>Die Indexdatei (.si)</h3>
<p>
Diese Datei enthält eine Beschreibung der Datenbank und für jede Partie
einen kleinen Eintrag fester Länge. Die aktuelle Größe ist 41 Bytes
pro Partie. Von diesen sind 28 Bytes wesentliche Informationen wie
Ergebnis, Datum, Indizes für Spieler-, Turnier-, Ortsnamen (die
tatsächlichen Namen befinden sich in der Namendatei) etc.
</p>
<p>
Die verbleibenden 13 Bytes enthalten redundante, aber nützliche
Informationen zur Partie, die zur Beschleunigung der Positions-,
Material- und Mustersuche genutzt werden. Siehe den Abschnitt
<a Formats Fast>Schnelles Suchen</a> weiter unten für weitere
Informationen.
</p>

<h3>Die Namendatei (.sn)</h3>
<p>
Diese Datei enthält alle Spieler-, Turnier-, Orts- und Rundennamen, die
in der Datenbank verwendet werden. Jeder Name ist nur einmal gespeichert,
auch wenn er in vielen Partien vorkommt. Die Namendatei ist üblicherweise
die kleinste der drei essentiellen Datenbankdateien.
</p>

<h3>Die Partiendatei (.sg)</h3>
<p>
Diese Datei enthält die aktuellen Züge, Varianten und Kommentare jeder
Partie. Die Zugkodierung ist sehr kompakt: die meisten Züge benötigen
nur ein Byte.
</p>
<p>
Wenn eine Partie ersetzt wird, wird ihre neue Version am <i>Ende</i>
der Datei gespeichert, so daß sich über die Zeit nicht genutzter
Speicherplatz ansammelt. Sie können durch <a Compact>Komprimieren</a>
die minimale Größe einer Datenbank wiederherstellen.
</p>

<h3>Andere Scid-Dateien</h3>
<p>
Eine <a EPD>EPD</a>-Datei (Dateiendung: ".epd") enthält eine Reihe von
Schachpositionen, jede mit einem Textkommentar.
Das EDP-Dateiformat ist beim <a Author Related>PGN-Standard</a>
beschrieben.
</p>
<p>
Eine E-Mail-Datei (Dateiendung: ".sem") einer Datenbank speichert
Daten der Gegner, denen Sie E-Mails senden.
</p>
<p>
Eine Suchoptionsdatei (Dateiendung: ".sso") enthält Einstellungen für
eine <a Searches Header>Partiedaten</a>- oder
<a Searches Material>Material/Muster</a>-Suche.
</p>

<h3><name Fast>Schnelles Suchen in Scid</name></h3>
<p>
Wie oben erwähnt, enthält die Indexdatei einige redundante, aber
nützliche Informationen über jede Partie, um Positions- oder
Materialsuchen zu beschleunigen.
</p>
<p>
Beispielsweise ist das Material der Schlußstellung gespeichert. Wenn Sie
nach Turm- und Bauernendspielen suchen, werden alle Partien, die mit
einer Dame, einem Läufer oder einem Springer auf dem Brett enden,
schnell übergangen.
</p>
<p>
Eine andere nützliche Teilinformation ist in der Reihenfolge gespeichert,
in der Bauern ihre Ausgangsstellung verlassen haben (durch Ziehen oder
Geschlagenwerden). Dies wird zur Beschleunigung von Zugbaum- oder exakten
Suchen verwendet, insbesondere nach Eröffnungspositionen. Beispielsweise
wird bei der Suche nach der Ausgangsstellung der Französischen
Verteidigung (1.e4 e6) jede Partie, die mit 1.e4 c5 oder 1.d4 etc.
beginnt, ausgelassen, Partien, die mit 1.e4 e5 anfangen, müssen aber
immer noch durchsucht werden.
</p>

<p><footer>(Aktualisiert: Scid 2.5, Juni 2001)</footer></p>
}


################
### Contact info

set helpTitle(D,Author) "Kontakt-Information"
set helpText(D,Author) {<h1>Kontakt-Information</h1>
<p>
Die Scid Internet - Seite ist erreichbar unter: <br>
<b><url http://scid.sourceforge.net/>http://scid.sourceforge.net/</url></b>
</p>
<p>
Sie bietet Downloads der neuesten Versionen von Scid und einigen anderen
zusätzlichen und nützlichen Dateien.
</p>
<p>
Bitte senden Sie Ihre Kommentare, Fragen, Vorschläge oder Fehlerberichte
an den Autor von Scid, Shane Hudson, an folgende E-Mail-Adresse: <br>
<b>scid-users@lists.sourceforge.net</b>
</p>

<h3><name Related>Links</name></h3>
<p>
Falls Sie Freude an Scid haben, könnten Sie die folgenden
Internetseiten interessieren:
</p>
<ul>
<li><url http://www.tim-mann.org/chess.html>http://www.tim_mann.org/chess.html</url> --
Homepage von Tim Mann's <b>xboard & winboard</b> Programm: eine grafische
Oberfläche für Schachprogramme und Schnittstelle für Internet Schach
Server. Sie hat auch gute Informationen über Crafty, GNUchess und
andere Schachprogramme.</li>

<li><url http://theweekinchess.com/twic>http://theweekinchess.com/twic</url> --
TWIC (the week in chess), ein hervorragendes Wochenmagazin für
internationale Schachpartien, verfügbar im PGN-Format. </li>

<li><url http://www.saremba.de/chessgml/standards/pgn/pgn-complete.htm>http://www.saremba.de/chessgml/standards/pgn/pgn-complete.htm</url> --
der <b>PGN-Standard</b>, geschaffen von Steven J. Edwards im Jahre 1994.
Die Seite erklärt das PGN- und EPD-Format im Detail.</li>
</ul>

<p><footer>(Aktualisiert: Scid 4.7, August 2018)</footer></p>
}

# Book window help
set helpTitle(D,Book) "Buch-Fenster"
set helpText(D,Book) {<h1>Buch-Fenster</h1>
  <p>
 Das Buch-Fenster zeigt eine Liste aller im Buch-Verzeichnis von Scid verfügbaren Buch-Datenbanken an. Das Format der Bücher entspricht dem Format, welches von Polygot und den Engines Fruit und Toga verwendet wird.
  </p>
  <p>
 Das Buch-Fenster zeigt (aus dem ausgewählten Buch) für die aktuelle Position alle möglichen Züge, die im Buch enthalten sind und deren relativen Gewichtungen in Prozent an. Bei jedem Zug wird das Buch-Fenster aktualisiert. Beim Anklicken eines Zuges im Buch-Fenster wird dieser Zug in die aktuelle Stellung übernommen.
  </p>
  <p>
Zwischen den Büchern kann schnell gewechselt werden, in dem man in der oberen Liste ein anderes Buch auswählt.
  </p>
  <p><footer>(Aktualisiert: Scid 3.6.21, Dezember 2007)</footer></p>
}

# Tactical game window help
set helpTitle(D,TacticalGame) "Trainingspartie-Fenster"
set helpText(D,TacticalGame) {<h1>Trainingspartie-Fenster</h1>
  <p>
  Hier kann eine Trainingspartie gegene eine Engine gespielt werden, die einen Gegner mit einer bestimmten ELO-Zahl simuliert.  
  Der Gegner wird von Phalanx gespielt, als Coach wird eine UCI-Engine eingesetzt. Diese Engines werden automatisch über ihre Namen ausgewählt und sollten korrekt konfiguriert sein. Standardmäßig werden diese Engines bei der Installation von Scid eingerichtet. 
  </p>
  <p>
  Folgende Parameter sind einzustellen:
  <ul>
  <li><term>Feste Stärke</term>: setzt eine feste Elo-Bewertung für den Gegner</li>.
  <li><term>Zufallsstärke</term>: Wählt eine zufällige Stärke zwischen der
  minimalen und maximalen Stärke, der durch den linken und rechten Schieberegler festgelegt wird, aus.
  </li>
  <li><term>Eröffnung:</term>
  <ul>
     <li><term>Neue Partie</term>: startet ein neues Spiel mit einer zufälligen Eröffnung.</li>
     <li><term>Von aktueller Position starten</term>: das Spiel bei der aktuellen Position fortsetzen.</li>
     <li><term>Ausgewählte Eröffnung</term>: der Gegner spielt eine Öffnung, die aus der Liste ausgewählt werden kann. Das ist nützlich für das Eröffnungstraining.</li>
  </ul>
  <li><term>Analysezeit der Engine begrenzen</term> erlaubt es, die vom Coach verwendete Zeit, um die Züge der Spieler auf Fehler zu überprüfen, zu begrenzen. Wenn dies nicht der Fall ist, darf der Coach im Hintergrund rechnen.</li><br>
  <p>Tipp: Schalten Sie die <a Graphs>Partiebewertung</a> ein, dann können Sie während der Partie den Verlauf der Zeit und die Bewertung der Stellung sehen.</p>
  <p>Eine ähnliche Funktion kann bei einer <a SeriousGame>Ernsten Partie</a> durch das limitieren der Spielstärke einer UCI-Engine erreicht werden.</p>
  <p>Zum Starten der Partie "Spielen" drücken.</p> 
  <p><footer>(Aktualisiert: Scid 4.7, August 2018)</footer></p>
}

# Opening Trainer window help
set helpTitle(D,OpeningTrainer) "Eröffnungstrainer"
set helpText(D,OpeningTrainer) {<h1>Eröffnungstrainer</h1>
  <p>
   Eine SCID-Datenbank kann als Repertoire verwendet werden, wobei jedes Spiel eine Eröffnungslinie darstellt. Die Spiele können Varianten enthalten. Auch NAG-Werte können angegeben werden, um so bevorzugte oder zweifelhafte Zeilen zu markieren. Eine Datenbank für den Eröffnungstrainer muss auch vom richtigen Typ sein: Das heißt, wenn Öffnungen für Weiß trainiert werden sollen, muss die Datenbank vom Typ Öffnungen für Weiß sein (ähnlich für Schwarz und beide Farben). Der Typ kann über das Fenster Wartung durch Auswahl des Symbols oder über das Kontextmenü des Datenbankumschalters eingestellt werden. 
  Eine Scid Datenbank kann als Repertoire genutzt werden, wenn sie mit dem entsprechenden Typ/Icon markiert wurde. Die Datenbank muss einer der folgenen Typen sein:
  <li>Openings for White</li>
  <li>Openings for Black</li>
  <li>Openings for either Color</li> <br>
  Der Typ muss mit der Auswahl "Konfiguration Trainingsrepertoire" (Weiß, Schwarz, Beide) übereinstimmen. Jede Partie repräsentiert eine Eröffnungsvariante. 
  </p>
  
  <h3>Eröffnungstrainer Statistik</h3>
  <p>
   Vier Zahlen zeigen die Statistik für die aktuelle Stellung:
  <ul>
  <li>Grün : Anzahl bereits korrekt gespielten Züge.</li>
  <li>Gelb : Anzahl zweifelhafter gespielter Züge im Repertoire</li>
  <li>Rot : Anzahl der Züge, die nicht im Repertoire enthalten sind.</li>
  <li>Weiß : Häufigkeit der Stellung</li>
  </ul>
  </p>
  
  <h3>Der Bericht</h3>
  <p>
  Der Bericht zeigt für das ganze Repertoire Statistiken und gibt Informationen über das Wissen des Anwenders:  
  <ul>
  <li>Positionen im Repertoire: Anzahl der Positione im Repertoire (alle Varianten in der Datenbank)</li>
  <li>nicht gespielte Positionen : Anzahl der Stellungen, die bisher nie gespielt wurden</li>
  <li>gespielte Positionen : gespielte Stellungen und die Summe wie häufig sie ausgespielt wurden. Anmerkung: Diese Zahl repräsentiert das Vorkommen der Stellung in allen Eröffnungsvarianten: Diese Zahl kann für Stellungen nahe der Startaufstellung schnell sehr groß werden.</li>
  <li>Erfolgreich : Anzahl der guten Züge durch den Spieler</li>
  <li>Zweifelhafte Züge : Anzahl der schwachen/schlechten Züge durch den Spieler</li>
  <li>Nicht im Repertoire : Anzahl der Züge des Spieler, die nicht im Repertoire enthalten sind.</li>
  </ul>
  </p> 
  <p><footer>(Aktualisiert: Scid 3.6.21, Dezember 2007)</footer></p>
}


############################################################
#
# German tip of the day

set tips(D) {
  {
    Scid hat mehr als 30 <a Index>Hilfeseiten</a> und in den meisten
    Scid-Fenstern liefert die <b>F1</b>-Taste die Hilfeseite zu diesem
    Fenster.
  }
  {
    Einige Scid-Fenster (z.B. Informationsbereich und
    Datenbank-<a Switcher>Umschalter</a>) haben ein Kontextmenu.
    Drücken Sie einfach die rechte Maustaste in jedem Fenster, um zu
    sehen, ob es ein Menü hat und welche Funktionen zur Verfügung
    stehen.
  }
  {
    Scid bietet Ihnen mehr als eine Möglichkeit, Schachzüge einzugeben,
    und läßt Ihnen die Wahl, welche Ihnen am meisten zusagt. Sie können
    die Maus verwenden (mit oder ohne Zugvorschlag) oder die Tastatur
    (mit oder ohne Zugergänzung). Für Details lesen Sie die Hilfeseite
    <a Moves>Züge eingeben</a>.
  }
  {
    Wenn Sie einige Datenbanken häufig öffnen, fügen Sie zu jeder ein
    <a Bookmarks>Lesezeichen</a> hinzu, dann können Sie sie schneller
    mit dem Lesezeichen-Menü öffnen.
  }
  {
    Sie können alle Züge einer Partie (mit allen Varianten und
    Kommentaren) mit dem <a PGN>PGN-Fenster</a> betrachten. Im
    PGN-Fenster können Sie zu jedem beliebigen Zug gehen, indem Sie
    ihn mit der linken Maustaste anklicken oder mit der mittleren
    Maustaste eine Voransicht dieser Stellung erhalten.
  }
  {
    Sie können Partien mit der linken Maustaste per "Drag & Drop" im
    <a Switcher>Datenbank-Umschalter</a> von einer Datenbank in eine
    andere kopieren.
  }
  {
    Scid kann PGN-Dateien selbst dann öffnen, wenn sie mit Gzip
    komprimiert wurden (Dateiname mit Erweiterung .gz). PGN-Dateien
    werden schreibgeschützt geöffnet. Wenn Sie also eine PGN-Datei
    editieren wollen, erstellen Sie eine neue Scid-Datenbank und
    kopieren die PGN-Datei mit dem <a Switcher>Datenbank-Umschalter</a>
    dorthin.
  }
  {
    Wenn Sie eine große Datenbank haben, die Sie oftmals mit dem
    <a Tree>Zugbaumfenster</a> nutzen, lohnt es sich,
    <b>Cache-Datei füllen</b> im Dateimenü des Zugbaumfensters zu
    wählen. Damit merken Sie sich Zugbaum-Statistiken für viele
    häufige Eröffnungspositionen und beschleunigen den Zugriff auf
    den Zugbaum für die Datenbank.
  }
  {
    Das <a Tree>Zugbaum</a>-Fenster kann Ihnen alle Züge zeigen, die
    in der aktuellen Position gespielt wurden, aber wenn Sie auch alle
    Zugfolgen sehen wollen, mit denen diese Stellung erreicht wurde,
    finden Sie diese, wenn Sie einen <a Reports Opening>Eröffnungsbericht</a>
    erstellen.
  }
  {
    Klicken Sie mit der linken oder rechten Maustaste auf die Überschrift
    einer Spalte im <a GameList>Partieliste</a>-Fenster , um ihre
    Breite anzupassen.
  }
  {
    Mit dem Fenster <a PInfo>Spielerinformation</a> (um es zu öffnen,
    klicken Sie einfach auf einen Spielernamen im Informationsbereich
    unter dem Hauptfenster-Schachbrett) können Sie auf einfache Weise
    den <a Searches Filter>Filter</a> so einstellen, daß er alle Partien
    eines bestimmten Spielers mit einem bestimmten Ergebnis enthält,
    indem Sie auf irgendeinen <red>in Rot</red> dargestellten Wert
    klicken.
  }
  {
    Beim Eröffnungsstudium kann es sehr hilfreich sein, für eine wichtige
    Position eine <a Searches Board>Brettsuche</a> mit der <b>Bauern</b>-
    oder <b>Linien</b>-Option durchzuführen, was andere Eröffnungen
    entdecken könnte, die zur selben Bauernstruktur führen.
  }
  {
    Im Informationsbereich (unterhalb des Schachbretts) können Sie mit
    der rechten Maustaste ein Menü zur Gestaltung der Ausgabe aktivieren.
    Zum Beispiel können Sie Scid veranlassen, den nächsten Zug zu
    verstecken, was nützlich ist, wenn man zum Trainieren eine Partie
    nachspielt und die Züge zu erraten versucht.
  }
  {
    Wenn Sie für eine große Datenbank oftmals eine umfangreiche
    Datenbank-<a Maintenance>Wartung</a> durchführen, können Sie
    mehrere Wartungsarbeiten gleichzeitig mit dem
    <a Maintenance Cleaner>Bereiniger</a> ausführen.
  }
  {
    Wenn Sie eine große Datenbank haben, wo die meisten Partien einen
    Turniereintrag besitzen und Sie wollen die Partien nach Datum
    sortiert haben, erwägen Sie ein <a Sorting>Sortieren</a> nach
    Turnierdatum und Turnier anstatt nach Datum und Turnier, da
    Ihnen das hilft, Partien desselben Turniers mit unterschiedlichen
    Daten zusammen zu halten (natürlich unter der Voraussetzung, daß
    sie alle dasselbe Turnierdatum haben).
  }
  {
    Es ist eine gute Idee, vor einem <a Maintenance Twins>Dubletten
    löschen</a> eine <a Maintenance Spellcheck>Schreibkorrektur</a>
    Ihrer Datenbank durchzuführen, da es dies Scid ermöglicht, mehr
    Dubletten zu finden und zum Löschen vorzumerken.
  }
  {
    <a Flags>Kennzeichen</a> sind nützlich, um Datenbankpartien mit
    Charakteristiken zu versehen, nach denen Sie zu einem späteren
    Zeitpunkt vielleicht suchen wollen, wie etwa Bauernstruktur, Taktik
    etc. Sie können mit der <a Searches Header>Partiedaten-Suche</a>
    nach Kennzeichen suchen.
  }
  {
    Wenn Sie eine Partie nachspielen und einige Züge ausprobieren
    wollen, ohne die Partie zu verändern, schalten Sie einfach den
    Testmodus ein (mit der Tastenkombination <b>Strg+Leerzeichen</b>
    oder mit dem Symbol aus der Werkzeugleiste), und wenn Sie fertig
    sind, schalten Sie ihn wieder aus, um zur ursprünglichen Partie
    zurückzukommen.
  }
  {
    Um die prominentesten Partien (Spieler mit hohen Elo-Zahlen)
    zu finden, die eine bestimmte Position erreicht haben, öffnen
    Sie das <a Tree>Zugbaum</a>-Fenster und dort die Liste der
    besten Partien. Sie können sogar die Liste der besten Partien
    auf Partien mit einem bestimmten Ergebnis begrenzen.
  }
  {
    Eine ausgezeichnete Methode, eine Eröffnung mit Hilfe einer großen
    Datenbank zu studieren, ist, den Trainingsmodus im
    <a Tree>Zugbaum</a>-Fenster zu aktivieren und dann gegen die
    Datenbank zu spielen, um zu sehen, welche Züge häufig vorkommen.
  }
  {
    Wenn Sie zwei Datenbanken geöffnet haben und die
    <a Tree>Zugbaum</a>-Statistik der ersten Datenbank sehen wollen,
    während Sie eine Partie der zweiten Datenbank untersuchen, drücken
    Sie einfach den Schalter <b>Anbinden</b> im Zugbaumfenster, um die
    Partie an die erste Datenbank zu binden, und wechseln dann zur
    zweiten Datenbank.
  }
  {
    Die <a Tmt>Turniersuche</a> ist nicht nur zum Auffinden eines
    bestimmten Turniers nützlich, sondern auch um zu sehen, an welchen
    Turnieren ein bestimmter Spieler vor kurzem teilgenommen hat,
    oder um die Spitzenturniere in einem bestimmten Land anzuzeigen.
  }
  {
    Es gibt eine Reihe häufiger Stellungsmuster, die im Fenster
    <a Searches Material>Material/Muster</a>-Suche definiert sind und
    die Ihnen beim Eröffnungs- oder Mittelspielstudium nützlich sein
    können.
  }
  {
    Wenn Sie im Fenster <a Searches Material>Material/Muster</a>-Suche
    nach einer bestimmten Materialkonstellation suchen, ist es oftmals
    hilfreich, die Suche auf Partien zu beschränken, die mindestens für
    einige Halbzüge auf das Suchmuster passen, um Partien auszusondern,
    wo die gesuchte Konstellation nur kurz vorkam.
  }
  {
    Wenn Sie eine wichtige Datenbank haben, die Sie nicht versehentlich
    verändern wollen, wählen Sie <b>Schreibschutz...</b> im
    <b>Datei</b>-Menü, nachdem Sie sie geöffnet haben, oder ändern Sie
    ihre Dateizugriffsrechte auf "nur Lesen".
  }
  {
    Wenn Sie XBoard oder WinBoard benutzen (oder ein anderes
    Schachprogramm, das Schachpositionen in FEN-Standardnotation in die
    Zwischenablage kopieren kann) und wollen dessen aktuelle
    Schachposition nach Scid kopieren, ist der schnellste und einfachste
    Weg, <b>Position sichern</b> im Dateimenü von XBoard/Winboard zu
    wählen, danach <b>Stellung einfügen</b> in Scids Menü "Bearbeiten".
  }
  {
    Die <a Searches Header>Partiedaten-Suche</a> ignoriert bei
    Spieler/Turnier/Ort/Runden-Namen Groß- und Kleinschreibung.  Sie
    können sie aber berücksichtigen und gleichzeitig
    Platzhalterzeichen verwenden (mit "?" = irgendein einzelnes
    Zeichen und "*" = null oder mehr Zeichen), wenn Sie den Suchtext
    in Anführungszeichen ("...")  eingeben. Beispielsweise geben Sie
    "*BEL" (mit den Anführungszeichen) im Ortsfeld ein, um alle in
    Belgien, nicht aber in Belgrad gespielten Partien zu finden.
  }
  {
    Wenn Sie einen Partiezug korrigieren wollen, ohne die danach
    gespielten Züge zu verlieren, öffnen Sie das
    <a Import>Import</a>-Fenster, klicken auf <b>Aktuelle Partie
    einfügen</b>, editieren den falschen Zug und wählen dann
    <b>Importieren</b>.
  }
  {
    Wenn Sie eine ECO-Klassifikationsdatei geladen haben, können Sie
    zur am weitest klassifizierten Position der aktuellen Partie mit
    <b>Eröffnung identifizieren</b> im <b>Partie</b>-Menü gehen
    (Tastenkombination: Strg+Umsch+D).
  }
  {
    Wenn Sie vor dem Öffnen einer Datei ihre Größe oder das Datum ihrer
    letzten Modifikation überprüfen wollen, öffnen Sie sie mit der
    <a Finder>Dateisuche</a>.
  }
  {
    Eine <a Repertoire>Repertoire</a>-Datei ist eine ausgezeichnete
    Methode, Ihre favorisierten Eröffnungsvarianten im Auge zu behalten
    und die Partien zu finden, wo diese Varianten gespielt wurden.
    Wenn Sie erst einmal Ihre Eröffnungen in einer Repertoire-Datei
    gespeichert haben, können Sie jedesmal, wenn Sie eine neue Datei
    mit Partien haben, eine Repertoire-Suche durchführen und die Partien
    mit Ihren favorisierten Eröffnungen betrachten.
  }
  {
    Mit einem <a Reports Opening>Eröffnungsbericht</a> kann man sehr gut
    mehr über eine bestimmte Position lernen. Sie können die Erfolgsrate
    sehen, ob sie zu vielen Kurzremis führt und die typischen
    positionellen Themen.
  }
  {
    Sie können die gebräuchlichsten Kommentarsymbole (!, !?, += etc.)
    dem aktuellen Zug oder der aktuellen Stellung mit Tastenkombinationen
    hinzufügen, ohne den Kommentareditor benutzen zu müssen -- um
    beispielsweise ein "!" hinzuzufügen, tippen Sie "!" und danach die
    Eingabetaste. Zu den Details siehe die Hilfeseite <a Moves>Züge
    eingeben</a>.
  }
  {
    Wenn Sie Eröffnungen in einer Datenbank mit dem <a Tree>Zugbaum</a>
    betrachten, können Sie eine nützlichen Übersicht erhalten, welche
    Erfolge mit der aktuellen Eröffnung in der letzten Zeit und zwischen
    Spitzenspielern erzielt wurden, indem Sie das Statistikfenster
    öffnen (Tastenkombination: Strg-I).
  }
  {
    Sie können die Größe des Hauptfensters ändern, indem Sie die
    <b>Strg</b>-Taste gedrückt halten und dann die Cursortaste
    <b>Links</b> oder <b>Rechts</b> drücken.
  }
  {
    Nach einer <a Searches>Suche</a> können Sie leicht durch alle
    passenden Partien blättern, indem Sie die <b>Strg</b>-Taste gedrückt
    halten und dann die Cursortaste <b>Auf</b> oder <b>Ab</b> drücken,
    um die vorherige bzw. nächste Partie im <a Searches Filter>Filter</a>
    zu laden.
  }
  {
    Über die Tasten <b>F2</b> und <b>F3</b> können direkt die letzten beiden Analyse-Engines gestartet werden, ohne dies nochmal auswählen zu müssen. 
  }
}

############################################################
#
# German Correspondence Chess, thanks to Jochen Jansen for translation
set helpTitle(D,Correspondence) "Fernschach"
set helpText(D,Correspondence) {<h1>Fernschach</h1>

<p>
Derzeit bietet Scid zwei Wege an, um Fernschach 
zu spielen.
</p>
<ul>
   <li><b>Email Schach</b> verschickt die aktuelle Partie per Email an
   Ihren Gegner, sobald Sie Ihren Zug gemacht haben.  Zu diesem Zweck
   wird eine Email in dem von Ihnen bevorzugten Email-Programm
   generiert.  Diese Email enthält eine PGN-Datei mit der aktuellen
   Partie als Anhang.  Von Ihnen evtl. eingefügte Kommentare oder
   Varianten werden selbstverständlich vorher entfernt.
   </li>
   <li><b>Fernschach-Server</b> werden unter Verwendung des sog.
   Xfcc-Protokolls unterstützt. Sofern Sie bei einem solchen Server
   einen Account besitzen, holt ein externes Tool dort Ihre Partien ab
   und synchronisiert Scid entsprechend.  Sobald Sie dann ihren Zug
   gemacht haben, wird auf dem gleichen Wege wieder Ihr Zug vermittels
   Xfcc an Ihren Gegner gesendet. Das Holen und Senden wurde hier
   bewußt über ein externes Tool realisiert, da so später einfachere
   Anpassung möglich sind, falls noch andere Protokolle erscheinen.
   </li>
</ul>
<p>
Sobald eine der Fernschachfunktionen aus dem Menü aufgerufen werden
öffnet sich das Fernschachfenster. Dieses Fenster kann auch aus dem
Fenster-Menü aus geöffnet werden. Das Fernschachfenster enthält die
nötigen Knöpfe zur Navigation durch laufende Partien, um Partien via
Xfcc abzuholen und eMail-Partien zu synchronisieren. Außerdem bietet
es eine einfache Konsole zur Anzeige der gesendeten und Empfangenen
Nachrichten sowie eine Liste aller laufenden Partien aus Scids
Posteingang.
</p>

<p>
Bevor Sie irgendwelche Fernschachfunktionen nutzen können, müssen Sie
zuerst eine Datenbank vom Typ "Fernschach" anlegen! Erstellen Sie also
ggf. eine neue Datenbank ("Datei-> Neu..." oder Strg-C) und setzen Sie
dann im <a Maintenance>Wartungsfenster</a> den Typ der Datenbank auf
"Correspondence Chess" (Briefkastensymbol). Das Setzen dieses
Datenbanktyps ist unabdingbar, da Scid am Typ erkennt, mit welcher DB
es die empfangenen Züge abgleichen soll. Wenn wie im beschriebenen
Fall die Datenbank nach Erstellung ja noch leer ist, behandelt Scid
die empfangenen Partien automatisch als neue Partien und fügt diese an
die Datenbank an.
</p>

<p>
Falls Sie zu diesem Zeitpunkt noch keine Datenbank vom Typ
"Correspondence Chess" geöffnet haben, wird Scid Sie daran erinnern.
Beachten Sie aber, nie mehr als eine Datenbank diesen Typs zu öffnen,
da Scid sonst nicht weiß, welche der Datenbanken es verwenden soll.
</p>

<h3>Grundfunktionen</h3>

<p>
Wenn Sie alles korrekt eingerichtet haben, können Sie die
Fernschachfunktionen ganz einfach über die folgenden Buttons erreichen
<ul>
   <li><button tb_CC_Retrieve> Abholen der Fernschach-Partien.  Ein
   externes Tool wird aufgerufen, das die Partien abholt, und alle so
   abgeholten Partien werden ebenso wie alle Partien, die sich zu
   diesem Zeitpunkt in Scids Posteingang (siehe unten)  mit der aktuell
   geöffneten Fernschachdatenbank synchronisiert.
   </li>
   <li><button tb_CC_Send> Schickt Ihren Zug an Ihren Gegner, in dem
   es entweder eine Email in dem von Ihnen bevorzugten Emailprogramm
   erstellt oder den Zug vermittels des Xfcc-Protokolls an einen
   Fernschachserver sendet.
   </li>
   <li><button tb_CC_delete> leert das Posteingangs- und das
   Postausgangsverzeichnis.
   </li>
   <li><button tb_CC_online> zeigt an, dass die Partienliste in der
   aktuellen Scid-Sitzung online aktualisiert wurde. Der zugehörige
   Infotip zeit das Datum und die Uhrzeit der letzten Aktualisierung.
   </li>
   <li><button tb_CC_offline> zeigt an, dass der Xfcc-Status ohne
   Aktualisierung aus den gespeicherten Werten hergestellt wurde und
   somit nicht aktuell sein muß. Der zugehörige Infotip zeit das Datum
   und die Uhrzeit der letzten Aktualisierung.
   </li>
</ul>
</p>

<p>
Bitte beachten Sie auch die Liste der <a CCIcons>Icons und Statusindikatoren</a>.
</p>

<h3>Konfiguration</h3>

<p>
Fernschach mit Scid funktioniert über eine normale Scid-Datenbank, die
Ihre Fernschachpartien enthält, und einigen externen Tools, die für
Scid den "nichtschachlichen-Teil" abwickeln. Diese Tools und einige
Parameters müssen einmalig eingerichtet werden, die Einstellungen
werden für die spätere Nutzung gespeichert.
</p>

<p>
Sie erreichen den Konfigurationsdialog aus dem Menü über
<menu>Spielen</menu> <menu>Fernschach</menu><menu>Einrichten</menu>.
Einzelheiten finden Sie unter <a CCSetupDialog>Fernschach
einrichten</a>. Das Verlassen dieses Dialogs mittels des <b>[Ok]</b>
Buttons speichert automatisch die von Ihnen getroffenen Einstellungen
ab.
</p>

<h3>Partien empfangen</h3>
<p>
Je nach dem, ob Sie per Email oder über einen Fernschachserver
spielen, ist der Empfangsprozess geringfügig verschieden. Wichtig ist,
dass am Ende Ihre Partie(n) als Datei(en) im PGN-Format in Scids Posteingang
Verzeichnis liegen, ganz gleich, auf welchem Weg Sie dies (evtl. durch
externe Programme) erreicht haben.
</p>
<p>
Sobald die Partien in der Posteingang liegen, wählen Sie im Menü
<menu>Posteingang bearbeiten</menu> und Scid arbeitet sich durch die
Posteingang, wobei es die neuen Züge automatisch an bereits bestehende
Partien in Ihrer Datenbank anhängt. Neue Partien, die noch nicht in
der Datenbank erfasst sind, werden automatisch der Datenbank
hinzugefügt.
</p>

<p><a CCeMailChess>Fernschach per Email</a> beschreibt detailliert
Fernschach via Email, während in <a CCXfcc>Fernschach via Schachserver
</a> Fernschach über entsprechende Fernschach-Server beschrieben wird.
</p>

<h3>Partien durchgehen</h3>
<p>
Nach dem Abholen werden Partien in Scids "clipboard" Datenbank geladen
und neue Partien und Züge werden der zuvor geöffneten
Fernschachdatenbank hinzu gefügt. Im Unterschied zur Funktion aus dem
<menu>Partie</menu> Menü blättern Sie mit diesen Knöpfen nicht durch
die gesamte Datenbank, sondern nur durch die Partien aus Scids Posteingang,
also nur durch die gerade von ihnen gespielten Partien. Die Datenbank
kann ja sehr viele Partien mehr enthalten, und Sie werden
wahrscheinlich nicht erst durch alle diese Partien blättern, nur um
irgendwann die Partie mit dem tatsächlich neuen Zug zu finden.
</p>
<p>
Beachten Sie bitte, dass hierbei Daten aus den Kopfzeilen ausgewertet
werden, die nicht von Scid indiziert wurden. Bei einer extrem großen
Fernschachdatenbank würden die Suchzeiten empfindlich ansteigen.
Sollten Sie sehr viel Fernschach spielen und Ihre Datenbank dadurch
sehr groß werden, so empfiehlt es sich, bereits beendete Partien in
einer Archivdatenbank zu speichern, um so den Suchzugriff auf die
aktuellen Partien zu beschleunigen.  Alternativ können Sie auch
einfach eine neue Datenbank erstellen, da Scid neue Partien dieser ja
dann automatisch hinzufügt. Öffnen Sie einfach diese neue und leere
Datenbank (Typ "Correspondence Chess" nicht vergessen) und wählen Sie
im Menü <menu>Posteingang bearbeiten</menu>, und schon werden alle noch
laufenden Partien importiert.
</p>
<p>
Vergleichbar mit den genannten Knöpfen sind die Menüpunkte
<menu>Vorherige Partie</menu> und <menu>Nächste Partie<menu> aus dem
<menu>Fernschach</menue> Menü.
</p>
<p>
Ein alternativer Weg um eine bestimmte Partie anzuwählen ist ein
Doppelklick auf diese in der Partienliste des Fernschachfensters.
</p>
<p>
Beachten Sie, dass bei korrekt eingegebenen Namen (unter <menu>Meine
Spielernamen</menu>) Scid das Schachbrett immer so drehen wird, dass
Sie "richtig herum", also von unten nach oben, spielen. Sie können
verschiedene Spielernamen verwenden. Einzelheiten finden Sie unter <a
Options MyPlayerNames>Meine Spielernamen</a>.
</p>

<h3>Analyse und Ziehen</h3>
<p>
Für das Fernschach können alle Analysefunktionen verwendet werden.
Varianten, Kommentare etc. können wie gewohnt hinzu gefügt werden.
Scid wird aber immer den letzten Halbzug nehmen und ihn wie den zu
sendenden Zug behandeln. Es wird nicht geprüft, ob nur ein Halbzug
oder auf welcher Seite dieser Zug hinzu gefügt wurde! Wenn Sie über
einen Fernschachserver spielen, überträgt Scid auch den letzten
Kommentar, damit Sie auf diese Weise ihrem Gegner etwas mitteilen
können. Wenn Sie via Email spielen, können Sie ja Kommentare oder
Grüße einfach in die Emailnachricht schreiben. Daher werden bei dieser
Variante alle Kommentare entfernt.
</p>
<p>
Nach dem Drücken des Verschicken-Knopfes <button tb_CC_Send> wird Scid
zunächst bestimmen, welche Art von Fernschachpartie vorliegt (Email
oder Fernschachserver) und entweder Ihr Email-Programm aufrufen oder
Ihren Zug über ein externes Tool an den Fernschachserver übertragen.
Der Menüpunkt <menu>Zug versenden</menu> erfüllt den gleichen Zweck.
Alternativ können Sie die aktuelle Partie auch über den Menüpunkt
<menu>per Email versenden</menu> per Email versenden. Wie beschrieben
erfüllt <menu>Zug versenden</menu> den gleichen Zweck. Auch bei
Vorliegen eines Serverspiels kann so eine Email erzeugt werden. Diese
wird aber wohl keinen Adressaten enthalten, da Fernschachserver diese
Information gewöhnlich nicht übertragen.
</p>

<p><footer>(Updated: Scid 3.6.25, August 2008)</footer></p>
}

set helpTitle(D,CCIcons) "Fernschach: Icons und Statusindikatoren"
set helpText(D,CCIcons) {
<h1>Icons und Statusindikatoren</h1>
<p>
Um die Anzeige in der Partienliste abzukürzen werden einige Symbole
verwendet. Manche von ihnen sind nur unter bestimmten Umständen
sichtbar, andere nur für Xfcc- bzw. eMail-basierte Partien. Diese
Indikatoren werden intern gespeichert und können so auch ohne
Internetverbindung mit dem Status der letzten Aktualisierung angezeigt
werden.
</p>
<ul>
   <li><button tb_CC_online> zeigt an, dass die Partienliste in der
   aktuellen Scid-Sitzung online aktualisiert wurde. Der zugehörige
   Infotip zeit das Datum und die Uhrzeit der letzten Aktualisierung.
   </li>
   <li><button tb_CC_offline> zeigt an, dass der Xfcc-Status ohne
   Aktualisierung aus den gespeicherten Werten hergestellt wurde und
   somit nicht aktuell sein muß. Der zugehörige Infotip zeit das Datum
   und die Uhrzeit der letzten Aktualisierung.
   </li>
   <li><button tb_CC_envelope> zeigt eine eMail-basierte Partie an.
   Durch die Limitationen dieses Mediums sind kaum zusätzliche
   Statusangaben verfügbar.
   </li>
   <li><button tb_CC_yourmove>
   Sie sind am Zug!
   <br>
   <b>Hinweis 1:</b> Dieser Status wird nur aktualisiert, wenn
   mit dem Fernschachserver synchronisiert wird, d. h. er gibt immer
   den Zustand des Servers bei der letzten Synchronisation wieder.
   <br>
   <b>Hinweis 2:</b> Fährt man mit der Maus über dieses Symbol wird
   der letzte Zug des Gegners angezeigt.
   </li>
   <li><button tb_CC_oppmove>
   Der Gegner ist am Zug. Hinweis: Dieser Status wird nur aktualisiert, wenn
   mit dem Fernschachserver synchronisiert wird, d. h. er gibt immer
   den Zustand des Servers bei der letzten Synchronisation wieder.
   </li>
   <li><button tb_CC_draw>
   Es wurde ein Remis vereinbart.
   </li>
   <li><button tb_CC_book>
   Eröffnungsbücher sind für diese Partie erlaubt.
   </li>
   <li><button tb_CC_database>
   Datenbanken sind für diese Partie erlaubt.
   </li>
   <li><button tb_CC_tablebase>
   Endspieltabellen (z. B. Nalimov-Tabellen) sind für diese Partie
   erlaubt.
   </li>
   <li><button tb_CC_engine>
   Schachengines sind für diese Partie erlaubt. Solche Partien werden
   bisweilen als "Advanced Chess" bezeichnet.
   </li>
   <li><button tb_CC_outoftime>
   Der Gegner hat seine Bedenkzeit überschritten. Es kann ein Gewinn
   wegen Zeitüberschreitung beantragt werden.
   </li>
   <li><button tb_CC_message>
   Der Gegner übermittelte mit dem Letzten Zug enie Nachricht. Diese
   wurde zur Partienotation als Kommentar hinzugefügt.
   </li>
</ul>
<b>Hinweis 1:</b> Wenn ein Tournier noch nicht offiziell begonnen hat,
aber die Aufstellung bereits bekannt gegeben wurde werden die Texte in
den Spalten id, Event und Site in <gray><b>grau</b></gray>
dargestellt.
<br>
<b>Hinweis 2:</b> Fährt man mit der Maus über die Site-Spalte, wird
der komplette Sitetag angezeigt. Fährt man über die Event-Spalte wird
ebenfalls der komplette Event-Tag angezeigt, sowie die Zeitkontrollen
der jeweiligen Partie, sofern diese vom Server übermittelt wurden.
Wurden keine Zeitkontrollen übermittelt zeigt Scid <i>10 / 50d (?)</i>
an, d. h. vermutet die offiziellen ICCF-Zeitkontrollen. (Der
ICCF-Server selbst sendet niemals die Zeitvorgaben.)
</p>

<p>
Für Xfcc-basierte Partien wird für jeden Gegner die jeweilige
Landesflagge angezeigt, wenn der Server die nötigen
Länderinformationen übermittelt. Für eMail basierte Partien kann dies
erreicht werden, wenn im PGN-Kopf zwei zustätzliche Zeilen
<i>whiteCountry</i> bzw. <i>blackCountry</i> eingefügt werden, die den
internationalen, dreistelligen Ländercode nach ISO 3166-1 enthalten
(z. B. "EUR" <button flag_eur>, "USA" <button flag_usa>, "GBR" <button
flag_gbr>, "FRA" <button flag_fra>, "RUS" <button flag_rus>, "CHN"
<button flag_chn>...).
</p>

<p>
Beachten Sie auch das Kapitel <a Correspondence>Fernschach</a> für
allgemeine Informationen.
</p>


<p><footer>(Aktualisiert: Scid 3.7b4, Februar 2009)</footer></p>
}

set helpTitle(D,CCXfcc) "Fernschach-Server"
set helpText(D,CCXfcc) {<h1>Fernschach-Server</h1>

<p>
Es gibt diverse Fernschachserver im Internet. Üblicherweise spielt
man auf ihnen ganz einfach mit einem Webbrowser, also ohne
irgendwelche spezielle Software. Dennoch bieten viele dieser Server
die Möglichkeit, ein externes (Schach-) Programm zu benutzen. Dies
geschieht durch die Verwendung des Xfcc-Protokolls.  Dieses
Xfcc-Protokoll wird bei Scid durch externe Tools unterstützt, die man
im <a CCSetupDialog>Einstellungen</a> Dialog konfiguriert.
</p>

<h3>Eine neue Partie beginnen</h3>
<p>
Xfcc selbst erlaubt es nicht, wie bei Email eine neue Partie zu
beginnen, in dem man einfach den ersten Zug an einen Gegner sendet.
Die Suche nach einem Gegner sowie das Beginnen von Partien geschieht
grundsätzlich auf der Webseite der Fernschachserver. Hat man denn dort
eine Partie einmal begonnen, kann man Scid dann im Weiteren benutzen,
um die neuen Züge der Gegner ab zu holen, sie zur eigenen Datenbank
hinzu zu fügen, sie zu analysieren, zu beantworten, usw.. Alle
Funktionen von Scid stehen dem Benutzer zur geneigten Verwendung zur
Verfügung. Klären Sie ggf. vorher im Rahmen der sportlichen Fairness
ab, ob und welche der zahlreichen Hilfsmittel Sie überhaupt verwenden
dürfen.
</p>

<h3>Partien abholen</h3>
<p>
Öffnen Sie die Datenbank, die ihre Fernschachpartien enthält oder
erstellen sie diesselbe neu. Diese Datenbank muss zwingend vom Typ
"Correspondence chess" sein. 
</p>

Anmerkungen: 
<ul>
   <li>Findet Scid keine geöffnete passende Datenbank, so fordert es
   Sie auf, eine solche zu öffnen.
   </li>
   <li>Enthält die Datenbank keine der Partien aus der Posteingang, so
   werden diese automatisch der Datenbank als neue Partien
   hinzugefügt.
   </li>
   <li>Scid wird die erste geöffnete Datenbank vom Typ "Correspondence
   Chess" benutzen. Es wird empfohlen, nur eine Datenbank dieses Typs
   geöffnet zu halten.
   </li>
</ul>
<p>
Xfcc holt immer alle laufenden Partien vom Server, die dort unter
Ihrer Benutzer-ID registriert sind. Zum Abholen drücken Sie einfach
auf das <button tb_CC_Retrieve> Symbol oder wählen <menu>Partien
abholen </menu> aus dem <menu>Fernschach</menu> Menü. Stellen Sie
sicher, dass Ihr System eine funktionierende Netzwerkverbindung hat,
da Scid selbst keine Verbindung herstellt sondern lediglich eine
bestehende nutzt, um sich mit dem Server zu verbinden. Scid holt die
Partien gemäß Ihren Einstellung aus dem <a
CCSetupDialog>Einstellungen</a> Dialog ab und legt die Partien im
PGN-Format in der Scid-Posteingang ab. Dies kann etwas dauern.  Nach
dem Abholen der Partien wird ihre zuvor geöffnete Fernschachdatenbank
entsprechend mit diesen Partien automatisch aktualisiert.
</p>
<p>
<b>Hinweis</b> Mit <button tb_CC_delete> kann das Posteingangs- und
Postausgangsverzeichnis geleert werden.
</p>
<p>
Sobald Ihre Partien abgeholt wurden, sucht Scid in Ihrer geöffneten
Fernschachdatendank nach den entsprechenden Gegenstücken und fügt die
neuen Züge dort automatisch diesen Partien hinzu. Während bei einer
Email-Partie gewöhnlich Halbzug für Halbzug via Email und somit
vermutlich auch via Scid übermittelt wird, kann man Xfcc-Server auf
verschiedene Art erreichen (Web, Mobiltelefon, usw.). Es ist also
denkbar, dass Sie und Ihr Gegner vielleicht einige Züge von
Unterwegs aus ausgeführt haben: Via Mobiltelefon aus der U-Bahn etwa,
und später via Web vom Bürorechner aus, und in der Mittagspause gab es
einen Zug vom PDA aus. Wenn Sie jetzt am Abend diese Partie mit Scid
abholen, hat Scid diese Züge ja alle bis jetzt noch nicht gesehen. Es
wird also alle notwendigen Züge bis zum aktuellen an diese Partie in
Ihrer Datenbank anfügen. Wie gesehen, kann dies eine ganze Anzahl von
Zügen sein. Aus diesem Grunde sollten Sie Anmerkungen und Kommentare
nie in die Hauptpartie schreiben, sondern stets Varianten dafür
verwenden, da es ja wie gesehen sein kann, dass Scid etliche Züge in
Ihrer Datenbank ergänzen muss. Scid wird hierbei immer nur am Ende der
Partie in ihrer Datenbank ergänzen, so dass Änderungen in der
Hauptvariante sehr wahrscheinlich dazu führen, dass eine
Synchronisation unmöglich wird.
</p>
Für Xfcc-basierte Partien kann Scid den status dieser sehr genau
angeben. Diese Informationen stehen aber nur zur Verfügung wenn die
interne Xfcc-Unterstützung verwendet wird.  Folgende Symbole werden
hierzu benutzt:
<ul>
   <li><button tb_CC_draw> Im letzten Zug wurde ein Remis verinbart.
   </li>
   <li><button tb_CC_yourmove> Sie sind am Zug.
   </li>
   <li><button tb_CC_oppmove> Ihr Gegner ist am Zug.
   </li>
   <li><button tb_CC_book> In dieser Partie sind Eröffnungsbücher erlaubt.
   </li>
   <li><button tb_CC_database> In dieser Partie ist die Nutzung von Datenbanken erlaubt.
   </li>
   <li><button tb_CC_tablebase> In dieser Partie ist die Nutzung von Endspieltabellen erlaubt.
   </li>
   <li><button tb_CC_engine> In dieser Partie ist die Nutzung von Schachprogrammen zur Analyse erlaubt.
   </li>
</ul>
<p>
Außerdem zeigt Scid den Stand der Uhren beider Parteien <p>zum
Zeitpunkt der Synchronisation</b> an sowie die Schachvariante.
Allerdings erlaubt Scid derzeit nur Standard Schach.
</p>
<p>
Hinweis: Nur wenn die entsprechenden Symbole (Buch, Datenbank,
Tabelle, Schachprogramm) angezeigt werden dürfen diese Hilfsmittel
verwendet werden. Andernfalls sind sie verboten! Bitte beachten Sie
diese Regeln!
</p>

<p>
<b>NOTABENE:</b> Wenn Sie Züge anders als mit Scid hinzu fügen, etwa
mit einem Texteditor oder einem Emailprogramm, dann achten Sie aus den
genannten Gründen genauestens darauf, nur den einen, von Ihnen
beabsichtigten Zug in die Hauptpartie zu schreiben! Scid weiß zu
keinem Zeitpunkt, an welcher Stelle der Partie sie "eigentlich" dran
waren! Für Scid ist dies <b>immer</b> der letzte Zug in der
"Hauptlinie" der Partie! Wenn Sie also eine mutmaßliche Fortführung
der Partie notieren wollen (was ja an sich als Gedächtnisstütze sehr
sinnvoll ist), so tun Sie dies <b>immer</b> in einer Variante, und
<b>nie</b> in der Hauptpartie! Ihr Gegner würde diese von Ihnen als
Gedächtnisstütze für Ihren Plan notierten Züge sonst übermittelt
bekommen - und Sie wollten sicher nicht Ihren Pläne an Ihren Gegner
senden!
</p>
<p>
<b>Beispiel:</b> Sie spielen ohne Scid, und schreiben die Antwortzüge
im Büro einfach von Hand mit dem Texteditor des Bürorechners hinter
die von Ihrem Gegner geschickten Züge, und wollen diese (wenn der Chef
weg ist) dann später mit Scid versenden. Ihr Gegner schickt Ihnen zum
Auftakt ein 1.e4 * zu. Sie überlegen sich, mit 1...e6 die Partie zu
antworten, und ferner, die Partie wie folgt fort zu setzen: 2. d4 d5
3. Nf3 Nf6 4. Nc3 * . Diese Fortsetzung wollen Sie sich als
Gedächtnisstütze notieren.  Schreiben Sie nun auf keinen Fall "1...e6
2. d4 d5 3. Nf3 Nf6 4. Nc3"!  Scid überträgt 4.Nc3 an den
Schachserver. Dieser Zug ist beim Derzeitigen Stand der Partie
ungültig und wird abgelehnt werden. Der Zug ist hierbei aus mehreren
Gründen Ungültig. Zuänchst erwartet der Schachserver den 2. Halbzug
von Schwarz im ersten Zug. Statt dessen wird aber der 1. Halbzug von
Weiß aus dem 4. Zug gesendet.
</p>
<p>
Notieren Sie stattdessen Ihren Plan als Variante: 
1.e4 e6 (1...e6 2. d4 d5 3. Nf3 Nf6 4. Nc3 )
So wird nur das 1....e6 an Ihren Gegner übermittelt, da dies der
letzte Zug der Hauptvariante ist. Dieser ist gültig und wird
akzeptiert werden.
</p>
<p>
Beachten Sie, dass sämtliche Partien in Scids Posteingang, ganz
gleich, wo diese herstammen, während des Abholprozesses von Scid
verarbeitet werden!  Auf diese Weise können Sie auch Partien, die Sie
auf anderem Weg erhalten haben, auf einfachste Art bearbeiten: Einfach
die Partie (im PGN-Format!) in die Posteingang legen, und wenn Sie in
Scid dann den Knopf <button tb_CC_Retrieve> drücken, wird die Partie
mit bearbeitet.  Scid arbeitet einfach alles ab, was sich in seinem
Posteingang befindet. Wie gewohnt: Bei Partien, die sich schon in
Ihrer geöffneten Fernschachdatenbank befinden, werden die neuen Züge
hinzu gefügt, während unbekannte Partien ganz einfach als neue Partien
an Ihre Datenbank angehangen werden.
</p>
<p>
<b>Hinweis</b> Mit <button tb_CC_delete> kann das Posteingangs- und
Postausgangsverzeichnis geleert werden.
</p>
<p>
<b>Hinweis für Programmierer</b>: Dem Fetch-Tool wird beim Aufruf der
Pfad zur Posteingang als Parameter mit gegeben. Es sollte dann alle
Server-Accounts durchgehen und ordentlich formatierte PGN-Files in den
übergebenen Pfad ablegen. Diese Dateien sollten zusätzliche Felder im
Header enthalten, wie sie von cmail her bekannt sind. (Siehe <a
CCeMailChess>Correspondence Chess via eMail</a> für weitere
Informationen über die benötigten Felder.) </p>

<p><footer>(Updated: Scid 3.6.23, February 2008)</footer></p>
}

#############
# eMail Chess:
set helpTitle(D,CCeMailChess) "Fernschach via Email"
set helpText(D,CCeMailChess) {<h1>Fernschach via Email</h1>

<p>
Das Medium Email bietet eine einfache und bequeme Möglichkeit zum
Fernschach-Spielen.  Die Standard-Anwendung für Un*x-aritge
Betriebssysteme ist bis heute die Kombination xboard/cmail. Man kann
damit recht bequem seine Fernschachpartien abwickeln. Da cmail dabei
nichts anderes produziert als sauberes PGN, ist es das Vorbild für
Scids Email-Funktion. Sofern man die vollständigen PGN-Kopfdaten
erhält, kann man damit mit jedem beliebigen Gegner spielen, der in der
Lage ist, irgendwie PGN-Dateien zu bearbeiten. Es ist also nicht
notwendig, dass beide Spieler einer Partie die gleiche Software
benutzen.
</p>

<h3>Email-Schach</h3>
<p>
Scid kann Ihre Fernschachpartien via Email fast vollautomatisch
verarbeiten.  Dabei bleibt die Kompatibilität mit cmail/xboard aus der
Un*x Welt gewahrt (Sie können also auch gegen Benutzer dieser
Programme spielen). Das Ganze funktioniert über das Versenden von
vollständigen PGN-Dateien als Email-Anhang, wobei die PGN-Kopfdaten
bestimmte wichtige Daten enthalten, die für das Verwalten der Daten
unabdingbar sind. Aus diesem Grunde sollten Sie Kopfdaten nur ändern,
wenn Sie wirklich wissen, was Sie tun. Im Zweifel lassen Sie es
besser, denn einige Felder müssen bestimmte Werte in ganz exakter Form
enthalten. Diese Felder werden von Scid automatisch und korrekt
ausgefüllt, wenn Sie eine Partie mit Scid beginnen. Obacht: Sie dürfen
diese Felder <b>auf keinen Fall</b> löschen, ändern oder
überschreiben!
</p>
<p>
Dies sind unabdingbare Kopfzeilen-Felder:
</p>
<ul>
   <li><term>Event</term>: Standardmäßig "Email correspondence game"
   </li>
   <li><term>Site</term>: muss auf "NET" lauten
   </li>
   <li><term>Mode</term>: muss auf "EM" lauten
   </li>
   <li><term>WhiteNA</term>: enthält die Email-Addresse des weißen
   Spielers. Beachten Sie bitte, dass hier nur die "reine" Adresse
   stehen darf in der Form <term>user@host.org</term>.
   </li>
   <li><term>BlackNA</term>: enthält die Email-Address des schwarzen
   Spielers analog zu WhiteNA.
   </li>
   <li><term>CmailGameName</term>: Enthält eine <b>eindeutige</b>
   Bezeichnung (ID) für die Partie. Diese Bezeichnung muss nicht nur
   eindeutig, sondern auch ein Unikat sein, denn über diese ID werden
   die Partien einander zugeordnet.
   <p>
   Scid könnte bei Datenbank auf Indices zurückgreifen. Das geht aber
   nicht bei "nicht-Datenbank-Daten" wie den cmail-Daten. Dewegen wird
   auf den <term>CmailGameName</term>-Parameter zurück gegriffen, den
   der Anwender vergeben muss.  Dieser Parameter muss <b>eindeutig</>
   sein! Eine einfache und praktible Art ist bspw.
   <term>aa-bb-yyyymmdd</term>, mit aa als Abkürzung des Namens des
   weißen Spielers und bb analog für Schwarz, sowie yyyymmdd für das
   aktuelle Datum.
   </p>
   <p>Bei Xfcc-basierten Partien muss dieses Feld ebnfalls eine
   eindeutige ID enthalten, aber in diesem Fall nimmt man den
   Servernamem und die eindeutige Partienummer auf diesen Server als
   ID, also in der Form <term>MeinXfccServer-12345</term>.
   </p>
   </li>
</ul>
<p>
eMail basierte Partien enthalten nicht so ausführliche
Statusinformationen wie Xfcc-basierte Partien. Allerdings werden diese
Partien mit dem <button tb_CC_envelope> Symbol gekennzeichnet um sie
leicht von anderen unterscheiden zu können.
</p>

<h3>Eine neue Partie beginnen</h3>
<p>
Hier öffnet sich ein Dialog, in welchem man den eigenen und den Namen
des Gegners so eingibt, wie er später in den Kopfzeilen erscheinen
soll sowie in den Emailaddressen beider Parteien. Zusätzlich muss eine
<b>eindeutige</b> Partie-ID vergeben werden.  Eine bewährte Lösung für
ist eine ID in der Form <term>aa-bb-yyyymmdd</term>, mit aa als
Abkürzung des Namens des weißen Spielers und bb analog für Schwarz,
sowie yyyymmdd für das aktuelle Datum.  Diese ID im Text-Format ist
unabdingbar für das Zuordnen der Partien.  Die Nutzer von cmail kennen
diese ID als <i>game name</i>. Sie darf nur Buchstaben, Zahlen, das
Minus-Zeichen und Unterstrichen enthalten.  Vermeiden Sie also alle
anderen (Sonder-) Zeichen.
</p>
<p>
Nachdem Sie durch Drücken des <b>[Ok]</b> Knopfes diesen Dialog
verlassen haben, wird eine neue Partie an die geöffnete
Fernschachdatenbank angehangen, und die PGN-Kopfzeilen sind ordentlich
ausgefüllt. Machen Sie also einfach Ihren Zug und senden sie ihn ab.
</p>

<h3>Partien empfangen</h3>

<p>
Scid bearbeitet nicht Ihr Email-Postfach. Der Programmieraufwand hierfür
wäre angedenk der unzähligen Möglichkeiten von Email-Zugängen viel zu
groß. Außerdem muss man das Rad ja nicht neu erfinden: Ihr gewohntes
Email-Programm kann ja genau das, und deswegen verwendet Scid auch
einfach Ihr gewohntes Email-Programm. Um nun eine so empfangene Partie
in Scid hinein zu bekommen, speichern Sie einfach das an der Email
angehangene PGN-File in Scids Posteingang ab (viele Emailprogramme
erlauben es auch, dies automatisch erledigen zu lassen. Lesen Sie dazu
das Handbuch Ihres Emailprogrammes). Wählen Sie dann einfach
<menu>Partien synchronisieren</menu> (bzw. drücken den <button tb_CC_Retrieve>
Knopf) oder wählen Sie <menu>Posteingang synchronisieren</menu>. Der
Unterschied zwischen beiden Möglichkeiten ist der, dass
<menu>Posteingang synchronisieren</menu> nur die Dateien verarbeitet,
die zu diesem Zeitpunkt im Posteingang liegen, während der erstere
Weg zusätzlich über in externes Programm zunächst weitere Partien (z.B.
via Xfcc) abholt, diese in der Posteingang speichert und dann
sämtliche Dateien, die sich dann in der Posteingang befinden,
bearbeitet.  Wenn Sie also sowohl per Email wie auch über Xfcc-Server
spielen, dürfte der bequemste Weg der sein,  zunächst die per Email
empfangenen Partien in der Posteingang zu speichern und dann <button
tb_CC_Retrieve> zu betätigen.
</p>
<p>
<b>Hinweis</b> Mit <button tb_CC_delete> kann das Posteingangs- und
Postausgangsverzeichnis geleert werden.
</p>
<p>
<b>Notabene:</b> <menu>Partien synchronisieren</menu> bzw. der <button
tb_CC_Retrieve> Knopf haben keine Funktion zum <b>Abholen</b> von
Emails! Sie müssen die Email mit Ihrem Emailprogramm abholen und "von
Hand" in Scids Posteingang speichern. Viele Emailprogramm ermöglichen
das Automatisieren dieses Arbeitsschrittes (auf Un*x-artigen Systemen
könnten Sie leicht die nötigen "mime handler" in Ihrer
<term>.mailcap</term> anpassen, oder Mailfilter für die automatische
Verarbeitung einsetzen.).
</p>

<h3>Antwortzüge verschicken</h3>

<p>
Wenn Sie Ihren Zug gemacht haben, können Sie ihn entweder über den
Menüpunkt <menu>Zug per eMail senden</menu> und <menu>Zug
senden</menu> oder <button tb_CC_Send>. Letzteres wählt die Art des
Versandes in Abhängigkeit der vorliegenden Partie, also wahlweise per
Email oder das Xfcc-Protokoll, während ersteres <b>immer</b> eine
Email mit angehangenem PGN erzeugt.
</p>
<p>
Selbstverständlich versendet Scid nur die reinen Züge. Sämtliche Arten
von Kommentaren und Varianten werden vorher entfernt, da Sie Ihre
Analysen und Pläne ganz sicher nicht Ihrem Gegner übermitteln wollen.
</p>
<p>
Sofern Sie eins der üblichen grafischen Email-Programme benutzen (wie
Thunderbird, Evolution, usw.), dann öffnet sich dort das Fenster zum
Verfassen einer Email. Die Adresse Ihres Gegners ist bereits
ausgefüllt, ebenso wie die Betreffzeile, in der die ID der Partie
eingetragen ist, damit Sie ggf. die Spiele nach Betreff filtern oder
sortieren können. Falls Sie unter <a CCSetupDialog>Einstellungen</a>
eine Adresse für eine Blindkopie angegeben haben, ist auch das
bcc-Feld der Email ausgefüllt (etwa, wenn bei Fernschachturnieren
jeder Zug in Kopie an einen Spielleiter geht). Der eigentlich
Email-Text enthält die aktuelle Stellung als FEN sowie eine Liste der
bis dahin gemachten Züge. Das ermöglich Ihrem Gegner einen "schnellen
Blick" auf die Partie, ohne dass er besondere Fernschachprogramme
einsetzen muss. Und schließlich ist bereits die bis dahin gespielte
Partie vollständig als PGN-Datei an die Email angehangen.
</p>
<p>
Falls Sie ein mailx-kompatibles Programm verwenden, so sehen Sie
nichts vom Sendevorgang, alles läuft im Hintergrund ab. In diesem Fall
wird die PGN-Datei im Textkörper der Nachricht wiederholt.
</p>
<p>
Beachten Sie, dass Sie bei Email-Partien auf keinen Fall mehr als nur
einen Halbzug vor dem Versenden anhängen dürfen! Scid überprüft nicht,
ob und wie viele Züge angehangen wurden. Aus diesem Grund sollten Sie
Analysen und Kommentare <b>nie</b> in die Hauptzeile der Partei
schreiben, sondern immer nur als Variante. Letztere werden automatisch
entfernt, so dass Sie bei so nie Gefahr laufen, Ihrem Gegner Ihre
Pläne zu übermitteln.
</p>
<p><footer>(Updated: Scid 3.6.23, February 2008)</footer></p>
}

#############
# Correspondence Chess setup dialog help:
set helpTitle(D,CCSetupDialog) "Fernschach Setup"
set helpText(D,CCSetupDialog) {<h1>Fernschach Setup</h1>

<p>
Im Einstellungsdialog sind bei bereits einige Standardwerte
eingetragen, die aber nicht notwendigerweise den Begebenheiten auf
Ihren System entsprechen. Sie müssen also einige Anpassungen
vornehmen. Alle getroffenen Einstellungen werden automatisch in der
Konfigurationsdatei gespeichert, sobald Sie den Dialog mit dem
<b>[Ok]</b> Button verlassen. Die Einstellung sind sofort gültig.
</p>

<p><b>Hinweis</b> Scid wird seine Standardeinstellungen so lange
verwenden bis der Einstellungsdialog mit <term>OK</term> bestätigt
wurde. Bis dies geschehen ist verwendet Scid die internen Standards
und wird diese nicht abspeichern. Standardmäßig wird Scid außerdem
versuchen alle Einstellungen automatisch vorzunehmen und die nötigen
Daten im <term>data</term>-Unterverzeichnis seiner Einstellungen zu
speichern. Hier wird auch eine gültige Datenbank für Fernschach
angelegt sowie die Posteingangs- und Postausgangsverzeichnisse
angelegt.
</p>

<p>
<b>Standarddatenbank</b>:
Hier stellen Sie die Datenbank ein, die Sie als Standard für Ihr
Fernschach verwenden wollen, leicht erreichbar über <menu>Datenbank
öffnen</menu> im <menu>Fernschach</menu> Menü.  (Sie können diese
Datei aber auch auf jede andere Art öffnen, die Scid bietet.) Der
Vorteil bei dieser Vorgehensweise ist, dass diese Datenbank nicht in
der Liste kürzlich geöffneten Datenbanken erscheint. Sie können also
eine Datei nur für das Fernschach verwenden.  Diese Datenbank <b>muss
zwingend</b> von Typ "Correspondence chess" sein!
</p>

<p>
<b>Posteingang (Pfad)</b>:
In diesem Verzeichnis schaut Scid nach Partien im PGN-Format. Diese
Partien werden mit Ihrer geöffneten Fernschachdatenbank
synchronisiert. Dabei ist es für Scid völlig egal, wie diese Partien
dorthon gekommen sind. Es arbeitet sich einfach durch alle Dateien
durch, die es dort vorfindet. So können Sie (oder andere Entwickler)
beliebige andere Werkzeuge und Programme verwenden, um Partien abzuholen
und dort zu platzieren. Beispielsweise können Sie aus einem beliebigen
Emailprogramm heraus die empfangene PGN-Datei einfach dort
abspeichern, und Scid wird sie verarbeiten.
</p>
<p>
Scid wird keine Email-Postfächer kontaktieren, sondern einfach alle
PGN-Dateien verarbeiten, die es im Posteingang-Verzeichnis vorfindet.
Beachten Sie, dass dabei dann alle gefundenen Partie mit der zu diesem
Zeitpunkt geöffneten (Fernschach-) Datenbank abgeglichen werden.
Sofern eine Partie aus diesem Verzeichnis nicht in der Datenbank
enthalten sein sollte, wird sie dort automatisch als neue Partie
angehangen.
</p>
<p>
Damit der Abgleich der Partien aus der Posteingang mit der Datenbank
funktioniert, müssen die PGN-Dateien eine Kopfzeilen enthalten, die
peinlich genau dem PGN-Standard entsprechen müssen! Werfen Sie einen
Blick in das Kapitel <a CCeMailChess>Fernschach via
eMail</a> falls Sie diese mit einem eigenen Tool erstellen wollen oder
Sie von einem anderen System migrieren.
</p>

<p>
<b>Postausgang (Pfad)</b>:
Das Gegenstück zur <i>Posteingang</i>. Scid speichert hier die
PGN-Dateien der ausgehenden Dateien. Das ist beim Email-Schach
wichtig, weil dabei diese Dateien an die Emails angehangen werden.
Bei Xfcc, wo nur der aktuelle Zug versendet wird, ist dies zwar
eigentlich nicht nötig, aber die Outbox ist sehr praktisch, falls Sie
bspw. einen PDA mit allen aktuell gespielten Partien abgleichen
wollen.
</p>

<p>
<b>Interne Xfcc-Unterstützung verwenden</b>:
Wenn angewählt werden nicht die beiden exterenn Hilfsprogramme
verwendet um Partien via Xfcc abzuholen oder Züge zu verschicken
sondern statt dessen Scids interne Unterstützung für dieses Protokoll.
Dies ist der einfachste Weg und sollte üblicherweise gewählt werden.
</p>
<p>
Hinweis: Diese Option kann nur gewählt werden wenn die beiden Module
http und tDOM für TCL installiert sind. Diese befinden sich
normalerweise im Lieferumfang der TCL-Installation, müssen aber bei
einigen Systemen explizit installiert werden. Wird eines dieser Pakete
nicht gefunden wird diese Funktion ausgeschaltet
</p>
<p>
<b>Xfcc Einstellungen</b>:
Hier wird der Pfad und Dateiname der Xfcc-Konfigurationsdatei
angegeben. Dieser Parameter wird ggf. auch an die externen
Hilfsprogramme weitergegeben, sollten diese benutzt werden.
</p>

<p>
<b>Hilfsprogramm zum Abholen</b>:
Dieses Hilfsprogramm holt Ihre Partien von einem Fernschachserver ab,
und erzeugt aus den empfangenen Daten ein ordentliches PGN-File mit
korrektem Header. Zur Kommunikation mit Xfcc-Servern sind externe
Werkzeuge die erste Wahl. Sollten sich das Protokoll einmal ändern,
kann man diese Änderungen so einfacher umsetzen. Auch können so
weitere Programme auf dieses Hilfsprogramm zugreifen, wodurch dem
Benutzer beliebig viele Möglichkeiten (z.B. zur Automatisierung)
offenstehen.
</p>
<p>
Beachten Sie: Dieses Hilfsprogramm dient <b>nicht</b> dem Empfang von
Email-Partien!
</p>

<p>
<b>Hilfsprogramm zum Senden</b>: 
Das Gegenstück zum Empfangsprogramms, ebenfalls im Wesentlichen für
das Xfcc-Protokoll gedacht. Dieses Hilfsprogramm wird von Scid mit
folgenden Parametern aufgerufen:
<term>
SendTool Outbox Name GameID MoveCount Move "Comment" resign claimDraw offerDraw acceptDraw
</term>
</p>

<p>
Das verbirgt sich hinter diesen Parametern:
   <ul>
      <li><term>Outbox</term>: Hier wird der Outbox-Pfad gesetzt, wo
      das Versandprogramm ein PGN-File erzeugen soll.
      </li>
      <li><term>Name</term>: Der Spielername und gleichzeitig das
      Xfcc-Login, der identisch ist mit dem Spielernamen im
      PGN-Header.
      </li>
      <li><term>MoveCount</term>: Die Nummer des zu versendenden
      Zuges.
      </li>
      <li><term>Move</term>: Der aktuelle Zug in vereinfachter
      algebraischer Notation (SAN).
      </li>
      <li><term>"Comment"</term>: Ein kommentar, der an den Gegner
      gesendet wird. Scid verwendet hierzu den letzten Kommentar, der
      in der aktuellen Partie enthalten ist, d. h. der letzte
      Kommentar der Partie wird als "für den Gegner bestimmt"
      behandelt. Dieser Kommentar wird in Anführungszeichen übergeben,
      so dass mehrzeilige Kommentare möglich sein sollten.
      </li>
      <li><term>resign</term>: 0 oder 1, gibt an, ob Sie aufgeben. 1
      bedeutet Aufgabe und wird in <menu>Aufgeben</menu> aus dem
      <menu>Fernschach</menu> Menü gesetzt.
      </li>
      <li><term>claimDraw</term>: 0 oder 1, zeigt an, dass Sie Remis
      fordern. Wird 1, falls Sie <menu>Remis erklären</menu> aus dem
      Menü <menu>Fernschach</menu> wählen.
      </li>
      <li><term>offerDraw</term>: 0 oder 1, entspricht Ihrem Wunsch
      nach Remis.  Wird 1, wenn Sie <menu>Remis anbieten</menu> aus
      dem Menü <menu>Fernschach</menu> wählen.
      </li>
      <li><term>acceptDraw</term>: 0 oder 1, wenn Sie Remis annehmen
      wollen.  Wird 1, wenn Sie <menu>Remis annehmen</menu> aus dem
      Menü <menu>Fernschach</menu> wählen.
      </li>
   </ul>
</p>
<p>
Anmerkung: Dieses Hilfsprogramm wird <b>nicht</b> für eMail-Schach verwendet!
</p>

<p>
<b>Mail-Programm</b>:
Gibt den Pfad zu Ihrem bevorzugten Email-Programm an. Dieses Programm
wird dann für das Erstellen und Versenden von Email-Schachpartien
aufgerufen. 
</p>
<p>
<b>(B)CC Address</b>:
Hiermit wird eine Blindkopie der Email an die hier angegebene Adresse
geschickt.  Das macht man z.B., falls man ein Email-Turnier spielt,
bei dem alle Züge in Kopie an einen Spiel- oder Turnierleiter gehen.
</p>

<p>
<b>Modus</b>:
Unglücklicherweiser gibt es unzählige verschiedene Mailprogramme, die
auch noch alle unterschiedlich aufgerufen werden müssen. Einige der
gebräuchlichsten Programme und ihre Aufrufparameter sind hier
gelistet. Falls Sie nicht wissen, wie Sie ihr Mailprogramm aufrufen
müssen, versuchen Sie die Einstellungen aus dieser Liste. Leider
können aber nicht alle Mailprogramme Mails mit Dateianhänge erzeugen,
wenn man sie aus einem anderen Programm heraus aufruft. Dann bleibt
Ihnen leider nur, entweder ein anderes Emailprogramm zu verwenden oder
die Postversand von Hand zu organisieren.
</p>
<p>Hinweis: mailx oder eines seiner Derivate sollten als
Kommandozeilenapplikation in der Lage sein, von eigentlich jeder
Plattform aus wie gewünscht zu funktionieren. Die Einrichtung ist
nicht sehr schwierig, so dass es sie es einmal versuchen sollten,
falls Ihr Mailprogramm nicht funktioniert
</p>
<p>Hinweis: mutt nutzt das systemsinterne Mailsystem (aka
sendmail/exim/postfix). Falls sie dieses (angeblich) recht schwierig
zu konfigurierende Mailsystem korrekt eingerichtet haben, ist mutt ein
prima Alternative. Auf einem "decent Unix" arbeitet es völlig
problemlos, allerdings stellt das einzurichtende Mailsystem eine
gewisse Hürde dar, da immer mehr Linuxsystem eben kein korrekt
eingerichtetes Mailsystem ausliefern.
</p>
<p>
<b>Attachement-Parameter</b>: 
Dieser Parameter gibt die anzuhängende Datei an. Er wird <b>nur</b> im
<term>mailx</term> Modus verwendet.
</p>
<p>
<b>Betreff-Parameter</b>:
Dieser Parameter gibt den Betreff einer Email an. Er wird <b>nur</b>
im <term>mailx</term> Modus verwendet.  </p>
<p><footer>(Updated: Scid 3.6.23, February 2008)</footer></p>
}

#############
# Correspondence Chess setup dialog help:
set helpTitle(D,CCXfccSetupDialog) "Xfcc Servereinstellungen"
set helpText(D,CCXfccSetupDialog) {<h1>Xfcc Servereinstellungen</h1>
<p>
Dieser Dialog liest das aktuell Verwendete Xfcc-Setup aus der
Konfigurationsdatei ein und zeigt alle Server an. Das Dialogfenster
selbst ist in zwei Teile geteilt: im oberen Bereich findet sich eine
Liste aller Servernamen, im unteren Bereich die für diesen Server
aktuell geltenden Parameter.
</p>
<h2>Notwendige Einträge</h2>
<ul>
<li><term>Servername</term>: Hier wird der von Scid verwendete
Servername angegeben. Er wird z. B. verwendet um eindeutige IDs für
die einzelnen Fernschachpartien zu vergeben. Deshalb muß er aus einem
Wort bestehen, darf nur die Buchstaben (a-z und A-Z), Zahlen und die
beiden Zeichen "-" und "_" enthalten. Ferner muß dieser Name eindeutig
sein (Groß/Kleinschreibung wird beachtet).
</li>
<li><term>Loginname</term>: Mit diesem Namen meldet sich Scid beim
ausgewählten Server an. Es ist hilfreich und vermeidet Probleme auch
hier nur Buchstaben, Zahlen und "-" sowie "_" zu verwenden.
</li>
<li><term>Passwort</term>: Gibt das für das Login verwendete Passwort
an. Hier gilt ebenfalls das zum Loginnamen gesagte. <b>Hinweis</b>
Scid speichert die Passworte unverschlüsselt.
</li>
<li><term>URL</term>: Hier wird die Zugangsadresse für den
Xfcc-Service definiert. Diese erfährt man beim jeweiligen
Diensteanbieter. Einige bekannte Server:
<ul>
	<li>SchemingMind: <url
	http://www.schemingmind.com/xfcc/xfccbasic.asmx>
	http://www.schemingmind.com/xfcc/xfccbasic.asmx</url>
	</li>
	<li>ICCF: 
	<url http://www.iccf-webchess.com/XfccBasic.asmx>
	http://www.iccf-webchess.com/XfccBasic.asmx</url>
	</li>
	<li>MeinSchach.de / MyChess.de:
	<url http://www.myChess.de/xfcc/xfccbasic.php4>
	http://www.myChess.de/xfcc/xfccbasic.php4</url>
	</li>
</ul>
</li>
</ul>
<p>
Um die Einstellungen eines Servers zu ändern wählt man diesen einfach
aus der Liste aus. Die zugehörigen Werte werden dann in den
Eingabefeldern angezeigt und können editiert wreden. Das Auswählen
eines anderen Servers aktiviert diese Einstellungen.
</p>
<p>
Um einen neuen Server zu addieren wählt man den <term>Neu</term>
Knopf. Es wird ein neuer Eintrag erzeugt in dem die entsprechenden
Felder ausgefüllt werden können.
</p>
<p>
Um einen Server zu löschen wählt man ihn aus der Liste aus und drückt
den <term>Löschen</term> Knopf. Allen Werte des entsprechenden Servers
wird nun ein Hash (#) vorangestellt. Hat man also versehentlich einen
Server gelöscht entfernt man einfach das # vor jedem Eintrag.
</p>
<p>
Durch drücken von <term>OK</term> speichert Scid die Einstellungen ab.
Hier werden dann auch die als gelöscht markierten Server endgültig
entfernt und alle neu angelegten hinzugefügt. Durch drücken von
<term>Abbruch</term> bleibt alles beim alten.
</p>

<p><footer>(Updated: Scid 3.6.24, May 2008)</footer></p>
}

set helpTitle(D,Appearance) "SCID-Oberfläche anpassen"
set helpText(D,Appearance) {<h1>Scid-Oberfläche anpassen</h1>
  <p>
  Scid hat einige Optionen um die Oberfläche anzupassen.
  </p>
  <h3><name Themes>TTK-Designs konfigurieren</name></h3>
  <p>
    Ab Version 4.7 können neue Oberflächen-Designs installiert und ausgewählt
    werden. Mit den TTK-Designs können verschiedene Erscheinungsbilder der
    SCID-Oberfläche realisiert werden.<br>
    Im Scid-Repository steht ein mehrfarbiges Set von Designs zur direkten Nutzung zur Verfügung:
    <li><url https://sourceforge.net/projects/scid/files/Scid/Additional Files/>https://sourceforge.net/projects/scid/files/Scid/Additional Files</url> -- Home / Scid / Additional Files</li><br><br>
    Weitere Designs können auch unter folgenden Adressen heruntergeladen werden:
    <li><url https://wiki.tcl.tk/48689>https://wiki.tcl.tk/48689</url> -- wiki.tcl.tk List of ttk Themes</li>
    <li><url https://github.com/RedFantom/ttkthemes/tree/master/ttkthemes/themes>https://github.com/RedFantom/ttkthemes</url>
    -- RedFantom Sammlung von ttk-Themen</li><br><br>
    Zur Installation der Designs gehen Sie wie folgt vor:<br>
    <li>Entpacken Sie die scidthemes-Dateien in ein beliebiges Verzeichnis.</li>
    <li>Designs laden: Über den Menüpunkt <b>Optionen - Design - Design(s) laden</b> wählen Sie die Datei pkgIndex.tcl aus dem Verzeichnis,
    in dem die Designs abgelegt sind.</li>
    <li>Design auswählen: Über den Menüpunkt <b>Optionen - Design - "Designname"</b>, z.b. scidblue scidmint scidpurple scidpurple scidsand scidpink,
    wählen Sie das zu verwendendte Design aus.</li>
    <li><b>Hinweis:</b> pkgIndex.tcl sind Dateien, die zusätzlichen tcl-Pakete nachladen. Verwenden Sie nur pkgIndex.tcl Dateien, die auch TTK-Designs enthalten.</li>
    </p>
  <h3><name MenuColors>Farben der Menüs einstellen</name></h3>
  <p>
    TTK-Designs besitzen technischbedingt keine angepassten Eigenschaften und
    Farben für Menüs. Die Farben für Menüs müssen daher gesondert angepasst
    und auf die Farben von Designs abgestimmt werden.<br>
    Unter <b>Optionen - Menüfarben einstellen</b> können die Farben für das Menü konfiguriert werden:<br>
    <li>Hintergrundfarbe des Hauptmenüs</li>
    <li>Vorder- und Hintergrundfarbe aller Menüs</li>
    <li>Vorder- und Hintergrundfarbe des ausgewählten Menüeintrages</li>
    <li>Farbe einer ausgewählten Option im Menü</li>
    <li><b>Hinweis:</b> Unter WINDOWS und MACOS wird für die Hauptmenüleiste die
    Systemeinstellungen verwendet und die konfigurierten Farben ignoriert.</li>
  </p>
  <p>
  Nach dem Konfigurieren des Design und der Menüfarben die Einstellungen mit <b>Optionen - Optionen speichern</b> speichern.
  </p>
  <p><footer>(Erstellt: Scid 4.7, August 2018)</footer></p>
}

set helpTitle(D,SeriousGame) "Ernste Partie"
set helpText(D,SeriousGame) {<h1>Ernste Partie</h1>
  <p>
  Ernste Spiele können gegen jede <term>UCI</term> Engine gespielt werden.
  Eine Liste aller möglichen Gegner ist im Fenster <term>Spiel konfigurieren</term> angegeben. Die
  Schaltfläche <term>UCI-Engine konfigurieren</term> bietet zusätzlich Zugriff auf
  die Motorparameter. Standardmäßig wird die Einstellungen der <a Analyseliste>Analyseliste</a> verwendet.
  </p>
  <p>
  Es kann das Eröffnungsbuch und die Eröffnung ausgewählt werden.
  Das Kontrollkästchen <term>Buch benutzen</term> de/aktiviert die Verwendung eines Eröffnungsbuchs.
  </p>
  <p>
  Die Sektion <term>Zeitmodus</term> erlaubt die Einstellung der Zeit für Spieler und Engine.
  Hier sind verschiedene Einstellungen möglich:
  <ul>
     <li><term>Zeit + Bonus</term> gibt die Zeit für das gesamte Spiel an und eine mögliche
     Bonuszeit pro Zug (Fisher-Uhr). Die
     Standardeinstellung ist 5 Minuten pro Spiel und 10 Sekunden Inkrement pro Spiel.
     Beachten Sie, dass die
     Zeiten für Schwarz und Weiß unabhängig voneinander eingestellen können. Dies ermöglicht
     eine kurze Zeitspanne für den Motor einzustellen und eine längere
     Zeit des Denkens für den Spieler.
     </li>
     <li><term>Feste Tiefe</term> setzt die Zeit pro Spiel nicht, sondern
     die Tiefe, die der Motor in halben Zügen berechnet. Da dies der Engine
     die Möglichkeit nimmt, bei Bedarf tiefer zu rechnen, wird der
     Computer bestimmte Kombinationen nicht sehen, der Motor
     kann schwächer spielen und somit einen besseren Trainingspartner bieten.
     </li><p>
     <b>Tipp</b>: Einige, Engines bieten auch an
     ihre Stärke in Elo-Einheiten einzustellen. Dies ist höchstwahrscheinlich ein
     geeigneterer Algorithmus als die Begrenzung der Suchtiefe.
     </p>
     <li><term>Knoten</term> ist eine ähnlich Einschränkung der Suchtiefe,
     aber hier stoppt der Motor nach der Auswertung eines bestimmten
     Anzahl der Positionen seine Berechnungen. (Die Vorgabe ist 10.000.)
     </li>
     <li><term>Sekunden pro Zug</term> erlaubt dem Motor, eine
     maximale Zeit für die Berechnung eines Zuges. Einige
     Engines ziehen unter bestimmten Umständen schneller, aber sie werden
     das hier eingestellte Zeitlimit nicht überschreiten. Wie <Term>Feste Tiefe</Term>Term
     und <term>Knoten</term> schränkt auch dies die Spielstärke der Engine ein.
     </li>
  </ul>
  </p>
  <p>
  Ernste Partien können ab der aktuellen Brettposition beginnen, wenn die Box
  <term>Von aktueller Position starten</term> aktiviert ist. Dies ermöglicht
  z.B. zum Ausspielen von definierten Mittelspielpositionen, die sich aus einer
  Eröffnung ergeben haben.
  </p>
  <p>
  <term>Berechnung im Hintergrund</term> (manchmal auch Ponder genannt)
  erlaubt es der Engine, während der Spieler am Zug ist zu Rechnen. Wenn das Spiel
  auf eine feste Zeit pro Zug eingestellt ist, schwächt das den
  Motor.
  </p>
  <p>
  <term>Coach schaut zu</term> öffnet ein Dialogangebot, um
  einen Zug zurück, wenn der Spieler einen Fehler gemacht hat (aufgrund der Motoren
  Auswertung seines letzten Zuges).
  </p>
  <p>
  <term>Bewertung hinzufügen</term> fügt im Kommentar der Partie für jeden Zug der Engine
  die Stellungsbewertung der Engine hinzu.
  </p>
  <p>
  Für das Training von Eröffnungen kann die Option <term>Ausgewählte Eröffnung</term>
  verwendet werden. In der Liste kann man die zu spielende Eröffnung auswählen. Der
  Spieler sollte dann der gewählten Eröffnung folgen, ansonsten fragt Scid
  ob der Zug zurückgenommen werden soll.
  </p>
  <p>
  Nach der Einstellung aller Parameter des Spiels und dem Drücken des Knopfes Spielen wird die Partie gestartet.
  Beachten Sie, dass es einige Zeit dauern kann, bis die Schachengine startet und richtig initialisiert ist.
  </p>
  <p><footer>(Erstellt: Scid 4.7, August 2018)</footer></p>
}
### End of file: deutsch.tcl
