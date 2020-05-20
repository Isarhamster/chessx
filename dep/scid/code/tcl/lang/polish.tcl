### Polish menus for Scid.
# Contributed by Michal Rudolf and Adam Umiastowski.

proc setLanguage_P {} {

menuText P File "Plik" 0
menuText P FileNew "Nowy..." 0 {Twórz nowa bazê Scid}
menuText P FileOpen "Otwórz..." 0 {Otwórz istniej±c± bazê Scid}
menuText P FileClose "Zamknij" 0 {Zamknij aktywn± bazê Scid}
menuText P FileFinder "Poszukiwacz plików" 0 {Otwórz okno poszukiwacza plików}
menuText P FileBookmarks "Zak³adki" 2 {Menu zak³adek (klawisz: Ctrl+B)}
menuText P FileBookmarksAdd "Dodaj zak³adkê" 0 \
  {Dodaj zak³adkê do aktualnej bazy i pozycji}
menuText P FileBookmarksFile "Wstaw zak³adkê" 0 \
  {Wstaw do wybranego katalogu zak³adkê do aktualnej bazy i pozycji}
menuText P FileBookmarksEdit "Edycja zak³adek..." 0 \
  {Edytuj menu zak³adek}
menuText P FileBookmarksList "Wy¶wietlaj katalogi jako listê" 0 \
  {Wy¶wietlaj katalogi zak³adek jako listê, nie jako zagnie¿d¿one menu}
menuText P FileBookmarksSub "Wy¶wietl katalogi jako menu" 0 \
  {Wy¶wietlaj katalogi zak³adek jako zagnie¿d¿one menu, nie jako listê}
menuText P FileMaint "Obs³uga" 1 {Narzêdzia obs³ugi bazy Scid}
menuText P FileMaintWin "Obs³uga" 0 \
  {Otwórz/zamknij obs³ugê bazy Scid}
menuText P FileMaintCompact "Porz±dkuj bazê..." 0 \
  {Porz±dkuj bazê, usuwaj±c skasowane partie i nieu¿ywane nazwiska}
menuText P FileMaintClass "Klasyfikacja debiutowa partii..." 0 \
  {Przelicz klasyfikacjê debiutowa wszystkich partii}
menuText P FileMaintSort "Sortuj bazê..." 0 \
  {Sortuj wszystkie partie w bazie}
menuText P FileMaintDelete "Usuñ podwójne partie..." 0 \
  {Szukaj podwójnych partii i oznacz je do skasowania}
menuText P FileMaintTwin "Wyszukiwanie podwójnych partii" 0 \
  {Otwórz/uaktualnij wyszukiwanie podwójnych partii}
menuText P FileMaintName "Pisownia" 0 \
  {Edycja nazw/nazwisk i kontrola pisowni}
menuText P FileMaintNameEditor "Edytor nazwisk" 0 \
  {Otwórz/zamknij edytor nazwisk}
menuText P FileMaintNamePlayer "Sprawd¼ pisowniê nazwisk..." 17 \
  {Sprawd¼ pisowniê nazwisk przy pomocy pliku nazwisk}
menuText P FileMaintNameEvent "Sprawd¼ pisowniê nazw zawodów..." 22 \
  {Sprawd¼ pisowniê nazw zawodów przy pomocy pliku turniejów}
menuText P FileMaintNameSite "Sprawd¼ pisowniê nazw miejscowo¶ci..." 22 \
  {Sprawd¼ pisowniê nazw miejscowo¶ci przy pomocy pliku miejscowo¶ci}
menuText P FileMaintNameRound "Sprawd¼ numery rund..." 15 \
  {Sprawd¼ numery rund przy pomocy pliku}
menuText P FileReadOnly "Tylko do odczytu..." 0 \
  {Zabezpiecz bazê przed zapisem}
menuText P FileSwitch "Prze³±cz bazê" 1 \
  {Prze³±cz na inn± otwart± bazê} 
menuText P FileExit "Koniec" 0 {Zamknij Scida}
# ====== TODO To be translated ======
menuText P FileMaintFixBase "Fix corrupted base" 0 {Try to fix a corrupted base}

menuText P Edit "Edytuj" 0
menuText P EditAdd "Dodaj wariant" 0 {Dodaj wariant do ruchu w partii}
menuText P EditDelete "Usuñ wariant" 0 {Usuñ wariant dla tego posuniêcia}
menuText P EditFirst "Twórz pierwszy wariant" 0 \
  {Przesuñ wariant na pierwsze miejsce na li¶cie}
menuText P EditMain "Zmieñ wariant na tekst partii" 0 \
   {Zamieñ wariant i tekst partii}
menuText P EditTrial "Sprawd¼ wariant" 0 \
  {W³±cz/wy³±cz tryb sprawdzania wariantów}
menuText P EditStrip "Usuñ" 2 \
  {Usuñ komentatarze i warianty}
# ====== TODO To be translated ======
menuText P EditUndo "Undo" 0 {Undo last game change}
# ====== TODO To be translated ======
menuText P EditRedo "Redo" 0 {Redo last game change}
menuText P EditStripComments "Komentarze" 0 \
  {Usuñ wszystkie komentarze z aktualnej partii}
menuText P EditStripVars "Warianty" 0 \
  {Usuñ wszystkie warianty z aktualnej partii}
menuText P EditStripBegin "Poprzednie posuniêcia" 0 \
  {Usuñ wszystkie posuniêcia do bie¿±cej pozycji}
menuText P EditStripEnd "Nastêpne posuniêcia" 0 \
  {Usuñ wszystkie posuniêcia od bie¿±cej pozycji do koñca partii}
menuText P EditReset "Opró¿nij schowek" 0 \
  {Opró¿nij schowek bazy}
menuText P EditCopy "Kopiuj partiê do schowka" 0 \
  {Kopiuj partiê do schowka}
menuText P EditPaste "Wklej aktywn± partiê ze schowka" 0 \
  {Wklej aktywn± partiê ze schowka}
menuText P EditPastePGN "Paste Clipboard text as PGN game..." 10 \
  {Interpret the clipboard text as a game in PGN notation and paste it here} ;# ***
menuText P EditSetup "Ustaw pozycjê pocz±tkow±..." 6 \
  {Ustaw pozycjê pocz±tkow± partii}
menuText P EditCopyBoard "Kopiuj pozycjê" 7 \
  {Kopiuj aktualn± pozycjê w notacji FEN do schowka}
menuText P EditPasteBoard "Ustaw pozycjê ze schowka" 3 \
  {Ustaw pozycjê ze schowka}
# ====== TODO To be translated ======
menuText P ConfigureScid "Preferences..." 0 {Configure all options for SCID}

menuText P Game "Partia" 1
menuText P GameNew "Opu¶æ partiê" 0 \
  {Opu¶æ partiê, rezygnuj±c z wszelkich zmian}
menuText P GameFirst "Pierwsza partia" 2 {Wczytaj pierwsz± partiê z filtra}
menuText P GamePrev "Poka¿ poprzedni± partiê" 0 \
  {Wczytaj poprzedni± wyszukan± partiê}
menuText P GameReload "Prze³aduj aktualn± partiê"  10 \
  {Wczytaj partiê ponownie, rezygnuj±c z wszelkich zmian}
menuText P GameNext "Nastêpna partia" 0 \
  {Wczytaj nastêpn± wyszukan± partiê}
menuText P GameLast "Ostatnia partia" 5 {Wczytaj ostatni± partiê z filtra}
menuText P GameRandom "Losowa partia z filtra" 8 {Wczytaj losow± partiê z filtra}
menuText P GameNumber "Wczytaj partiê numer..." 17 \
  {Wczytaj partiê wprowadzaj±c jej numer}
menuText P GameReplace "Zapisz: zast±p partiê..." 3 \
  {Zapisz partiê, zast±p poprzedni± wersjê}
menuText P GameAdd "Zapisz: dodaj now± partiê..." 8 \
  {Zapisz tê partiê jako nowa partiê w bazie}
menuText P GameDeepest "Rozpoznaj debiut" 0 \
  {Przejd¼ do najd³u¿szego wariantu z ksi±¿ki debiutowej}
menuText P GameGotoMove "Przejd¼ do posuniêcia nr..." 13 \
  {Przejd¼ do posuniêcia o podanym numerze}
menuText P GameNovelty "Znajd¼ nowinkê..." 7 \
  {Znajd¼ pierwsze posuniêcie partii niegrane wcze¶niej}

menuText P Search "Szukaj" 0
menuText P SearchReset "Resetuj filtr" 0 \
  {Wstaw wszystkie partie do filtra}
menuText P SearchNegate "Odwróæ filtr" 0 \
  {Zamieñ partie w filtrze i poza nim}
menuText P SearchCurrent "Aktualna pozycja..." 0 \
  {Szukaj aktualnej pozycji}
menuText P SearchHeader "Nag³ówek..." 0 \
  {Szukaj informacji o nag³ówkach (nazwiska, nazwy turnieju itp.)}
menuText P SearchMaterial "Materia³/wzorzec..." 0 \
  {Szukaj wed³ug materia³u lub wzorca}
menuText P SearchUsing "Stosuj plik poszukiwania..." 0 \
  {Szukaj stosuj±c plik z opcjami poszukiwania}

menuText P Windows "Okna" 1
menuText P WindowsComment "Edytor komentarzy" 0 \
  {Otwórz/zamknij edytor komentarzy}
menuText P WindowsGList "Lista partii" 0 {Otwórz/zamknij listê partii}
menuText P WindowsPGN "Okno PGN" 0 {Otwórz/zamknij (zapis partii) PGN }
menuText P WindowsPList "Zawodnicy" 2 {Otwórz/zamknij przegl±darkê zawodników}
menuText P WindowsTmt "Turnieje" 0 {Otwórz/zamknij przegl±darkê turniejów}
menuText P WindowsSwitcher "Prze³±cznik baz" 12 \
  {Otwórz/zamknij prze³±cznik baz}
menuText P WindowsMaint "Zarz±dzanie baz±" 0 \
  {Otwórz/zamknij okno zarz±dzania baz±}
menuText P WindowsECO "Przegl±darka kodów debiutowych" 0 \
  {Otwórz/zamknij przegl±darkê kodów debiutowych}
menuText P WindowsStats "Statystyka" 0 \
  {Otwórz/zamknij statystykê}
menuText P WindowsTree "Drzewo wariantów" 0 {Otwórz/zamknij drzewo wariantów}
menuText P WindowsTB "Tablica koñcówek" 8 \
  {Otwórz/zamknij okno tablicy koñcówek}
# ====== TODO To be translated ======
menuText P WindowsBook "Book Window" 0 {Open/close the Book window}
# ====== TODO To be translated ======
menuText P WindowsCorrChess "Correspondence Window" 0 {Open/close the Correspondence window}

menuText P Tools "Narzêdzia" 0
menuText P ToolsAnalysis "Program analizuj±cy..." 8 \
  {Uruchom/zatrzymaj program analizuj±cy}
menuText P ToolsAnalysis2 "Program analizuj±cy 2..." 21 \
  {Uruchom/zatrzymaj program analizuj±cy}
menuText P ToolsCross "Tabela turniejowa" 0 \
  {Poka¿ tabelê turniejow± dla aktualnej partii}
menuText P ToolsEmail "Zarz±dzanie poczt± e-mail" 0 \
  {Otwórz/zamknij zarz±dzanie adresami e-mail}
menuText P ToolsFilterGraph "Wykres filtra" 7 \
  {Otwórz/zamknij wykres filtra}
# ====== TODO To be translated ======
menuText P ToolsAbsFilterGraph "Abs. Filter Graph" 7 {Open/close the filter graph window for absolute values}
menuText P ToolsOpReport "Raport debiutowy" 0 \
  {Utwórz raport debiutowy dla aktualnej pozycji}
# ====== TODO To be translated ======
menuText P ToolsOpenBaseAsTree "Open base as tree" 0   {Open a base and use it in Tree window}
# ====== TODO To be translated ======
menuText P ToolsOpenRecentBaseAsTree "Open recent base as tree" 0   {Open a recent base and use it in Tree window}
menuText P ToolsTracker "¦ledzenie figur"  10 {Otwórz/zamknij okno ¶ledzenia figur} 
# ====== TODO To be translated ======
menuText P ToolsTraining "Training"  0 {Training tools (tactics, openings,...) }
# ====== TODO To be translated ======
menuText P ToolsTacticalGame "Tactical game"  0 {Play a game with tactics}
# ====== TODO To be translated ======
menuText P ToolsSeriousGame "Serious game"  0 {Play a serious game}
# ====== TODO To be translated ======
menuText P ToolsTrainOpenings "Openings"  0 {Train with a repertoire}
# ====== TODO To be translated ======
menuText P ToolsTrainReviewGame "Review game"  0 {Guess moves played in a game}
# ====== TODO To be translated ======
menuText P ToolsTrainTactics "Tactics"  0 {Solve tactics}
# ====== TODO To be translated ======
menuText P ToolsTrainCalvar "Calculation of variations"  0 {Calculation of variations training}
# ====== TODO To be translated ======
menuText P ToolsTrainFindBestMove "Find best move"  0 {Find best move}
# ====== TODO To be translated ======
menuText P ToolsTrainFics "Play on internet"  0 {Play on freechess.org}
# ====== TODO To be translated ======
menuText P ToolsEngineTournament "Engine tournament"  0 {Start a tournament between chess engines}
# ====== TODO To be translated ======
menuText P ToolsBookTuning "Book tuning" 0 {Book tuning}
# ====== TODO To be translated ======
menuText P ToolsConnectHardware "Connect Hardware" 0 {Connect external hardware}
# ====== TODO To be translated ======
menuText P ToolsConnectHardwareConfigure "Configure..." 0 {Configure external hardware and connection}
# ====== TODO To be translated ======
menuText P ToolsConnectHardwareNovagCitrineConnect "Connect Novag Citrine" 0 {Connect Novag Citrine}
# ====== TODO To be translated ======
menuText P ToolsConnectHardwareInputEngineConnect "Connect Input Engine" 0 {Connect Input Engine (e.g. DGT)}
menuText P ToolsPInfo "Informacje o zawodniku"  0 \
  {Otwórz/od¶wie¿ okno informacji o zawodniku}
menuText P ToolsPlayerReport "Raport o graczu..." 9 \
  {Utwórz raport o graczu} 
menuText P ToolsRating "Wykres rankingu" 0 \
  {Wykres historii rankingu graj±cych partiê}
menuText P ToolsScore "Wykres wyników" 1 {Poka¿ wykres wyników}
menuText P ToolsExpCurrent "Eksportuj partiê" 0 \
  {Zapisz partiê do pliku tekstowego}
menuText P ToolsExpCurrentPGN "Do pliku PGN..." 9 \
  {Zapisz partiê do pliku PGN}
menuText P ToolsExpCurrentHTML "Do pliku HTML..." 9 \
  {Zapisz partiê do pliku HTML}
# ====== TODO To be translated ======
menuText P ToolsExpCurrentHTMLJS "Export Game to HTML and JavaScript File..." 15 {Write current game to a HTML and JavaScript file}  
menuText P ToolsExpCurrentLaTeX "Do pliku LaTeX-a..." 9 \
  {Zapisz partiê do pliku LaTeX-a}
menuText P ToolsExpFilter "Eksportuj wyszukane partie" 1 \
  {Zapisz wyszukane partie do pliku tekstowego}
menuText P ToolsExpFilterPGN "Do pliku PGN..." 9 \
  {Zapisz wyszukane partie do pliku PGN}
menuText P ToolsExpFilterHTML "Do pliku HTML..." 9 \
  {Zapisz wyszukane partie do pliku HTML}
# ====== TODO To be translated ======
menuText P ToolsExpFilterHTMLJS "Export Filter to HTML and JavaScript File..." 17 {Write all filtered games to a HTML and JavaScript file}  
menuText P ToolsExpFilterLaTeX "Do pliku LaTeX..." 9 \
  {Zapisz wyszukane partie do pliku LaTeX}
menuText P ToolsImportOne "Wklej partiê w formacie PGN..." 0 \
  {Pobierz partiê z pliku PGN}
menuText P ToolsImportFile "Importuj plik PGN..." 2 \
  {Pobierz partie z pliku PGN}
# ====== TODO To be translated ======
menuText P ToolsStartEngine1 "Start engine 1" 0  {Start engine 1}
# ====== TODO To be translated ======
menuText P ToolsStartEngine2 "Start engine 2" 0  {Start engine 2}
# ====== TODO To be translated ======
menuText P ToolsCaptureBoard "Capture Current Board..." 0  {Save the current board as an image.}
# ====== TODO To be translated ======
menuText P Play "Play" 0
# ====== TODO To be translated ======
menuText P CorrespondenceChess "Correspondence Chess" 0 {Functions for eMail and Xfcc based correspondence chess}
# ====== TODO To be translated ======
menuText P CCConfigure "Configure..." 0 {Configure external tools and general setup}
# ====== TODO To be translated ======
menuText P CCConfigRelay "Configure observations..." 10 {Configure games to be observed}
# ====== TODO To be translated ======
menuText P CCOpenDB "Open Database..." 0 {Open the default Correspondence database}
# ====== TODO To be translated ======
menuText P CCRetrieve "Retrieve Games" 0 {Retrieve games via external (Xfcc-)helper}
# ====== TODO To be translated ======
menuText P CCInbox "Process Inobx" 0 {Process all files in scids Inbox}
# ====== TODO To be translated ======
menuText P CCSend "Send Move" 0 {Send your move via eMail or external (Xfcc-)helper}
# ====== TODO To be translated ======
menuText P CCResign "Resign" 0 {Resign (not via eMail)}
# ====== TODO To be translated ======
menuText P CCClaimDraw "Claim Draw" 0 {Send move and claim a draw (not via eMail)}
# ====== TODO To be translated ======
menuText P CCOfferDraw "Offer Draw" 0 {Send move and offer a draw (not via eMail)}
# ====== TODO To be translated ======
menuText P CCAcceptDraw "Accept Draw" 0 {Accept a draw offer (not via eMail)}
# ====== TODO To be translated ======
menuText P CCNewMailGame "New eMail Game..." 0 {Start a new eMail game}
# ====== TODO To be translated ======
menuText P CCMailMove "Mail Move..." 0 {Send the move via eMail to the opponent}
# ====== TODO To be translated ======
menuText P CCGamePage "Game Page..." 0 {Call up the game via the web browser}
# ====== TODO To be translated ======
menuText P CCEditCopy "Copy Gamelist to Clipbase" 0 {Copy the games as CSV list to clipbase}

menuText P Options "Opcje" 0
# ====== TODO To be translated ======
menuText P OptionsBoardGraphics "Squares..." 0 {Select textures for squares}
# ====== TODO To be translated ======
translate P OptionsBGW {Select texture for squares}
# ====== TODO To be translated ======
translate P OptionsBoardGraphicsText {Select graphic files for white and black squares:}
menuText P OptionsBoardNames "Moje nazwiska" 0 {Modyfikuj listê moich graczy}
menuText P OptionsExport "Eksport" 0 {Zmieñ opcje eksportu tekstu}
menuText P OptionsFonts "Czcionka" 0 {Zmieñ font}
menuText P OptionsFontsRegular "Podstawowa" 0 {Zmieñ podstawow± czcionkê}
menuText P OptionsFontsMenu "Menu" 0 {Zmieñ czcionkê menu} 
menuText P OptionsFontsSmall "Ma³a" 0 {Zmieñ ma³± czcionkê}
# ====== TODO To be translated ======
menuText P OptionsFontsTiny "Tiny" 0 {Change the tiny font}
menuText P OptionsFontsFixed "Sta³a" 0 {Zmieñ czcionkê sta³ej szeroko¶ci}
menuText P OptionsGInfo "Informacje o partii" 0 {Sposób wy¶wietlania informacji o partii}
menuText P OptionsLanguage "Jêzyk" 0 {Wybierz jêzyk}
# ====== TODO To be translated ======
menuText P OptionsMovesTranslatePieces "Translate pieces" 0 {Translate first letter of pieces}
# ====== TODO To be translated ======
menuText P OptionsMovesHighlightLastMove "Highlight last move" 0 {Highlight last move}
# ====== TODO To be translated ======
menuText P OptionsMovesHighlightLastMoveDisplay "Show" 0 {Display last move Highlight}
# ====== TODO To be translated ======
menuText P OptionsMovesHighlightLastMoveWidth "Width" 0 {Thickness of line}
# ====== TODO To be translated ======
menuText P OptionsMovesHighlightLastMoveColor "Color" 0 {Color of line}
# ====== TODO To be translated ======
menuText P OptionsMovesHighlightLastMoveArrow "include Arrow" 0 {Include Arrow with Highlight}
menuText P OptionsMoves "Posuniêcia" 0 {Wprowadzanie posuniêæ}
menuText P OptionsMovesAnimate "Szybko¶æ animacji" 1 \
  {Ustaw czas przeznaczony na animacjê jednego posuniêcia} 
menuText P OptionsMovesDelay "Automatyczne przegl±danie..." 0 \
  {Ustaw opó¼nienie przy automatycznym przegl±daniu partii}
menuText P OptionsMovesCoord "Posuniêcia w formacie \"g1f3\"" 0 \
  {Akceptuj posuniêcia wprowadzone w formacie "g1f3"}
menuText P OptionsMovesSuggest "Poka¿ proponowane posuniêcia" 1 \
  {W³±cz/wy³±cz proponowanie posuniêæ}
# ====== TODO To be translated ======
menuText P OptionsShowVarPopup "Show variations window" 0 {Turn on/off the display of a variations window}  
# ====== TODO To be translated ======
menuText P OptionsMovesSpace "Add spaces after move number" 0 {Add spaces after move number}  
menuText P OptionsMovesKey "Automatyczne dope³nianie posuniêæ" 1 \
  {W³±cz/wy³±cz automatyczne dope³nianie posuniêæ wprowadzanych z klawiatury}
# ====== TODO To be translated ======
menuText P OptionsMovesShowVarArrows "Show Arrows for Variations" 0 {Turn on/off arrows showing moves in variations}
# ====== TODO To be translated ======
menuText P OptionsMovesGlossOfDanger "Color Coded Gloss of Danger" 0 {Turn on/off color coded gloss of danger}
menuText P OptionsNumbers "Format zapisu liczb" 0 {Wybierz format zapisu liczb}
# ====== TODO To be translated ======
menuText P OptionsTheme "Theme" 0 {Change look of interface}
menuText P OptionsWindows "Okna" 0 {Opcje okien}
menuText P OptionsSounds "Sounds" 2 {Configure move announcement sounds} ;# ***
# ====== TODO To be translated ======
menuText P OptionsResources "Resources..." 0 {Choose resource files and folders}
# ====== TODO To be translated ======
menuText P OptionsWindowsDock "Dock windows" 0 {Dock windows}
# ====== TODO To be translated ======
menuText P OptionsWindowsSaveLayout "Save layout" 0 {Save layout}
# ====== TODO To be translated ======
menuText P OptionsWindowsRestoreLayout "Restore layout" 0 {Restore layout}
# ====== TODO To be translated ======
menuText P OptionsWindowsShowGameInfo "Show game info" 0 {Show game info}
# ====== TODO To be translated ======
menuText P OptionsWindowsAutoLoadLayout "Auto load first layout" 0 {Auto load first layout at startup}
menuText P OptionsECO "Wczytaj ksi±¿kê debiutow±" 16 \
  {Wczytaj plik z klasyfikacja debiutów}
menuText P OptionsSpell "Wczytaj plik sprawdzania pisowni..." 13 \
  {Wczytaj plik do sprawdzania pisowni nazwisk i nazw}
menuText P OptionsTable "Katalog z baz± koñcówek" 10 \
  {Wybierz bazê koñcówek; u¿yte zostan± wszystkie bazy z tego katalogu}
menuText P OptionsRecent "Ostatnie pliki..." 0 \
  {Zmieñ liczbê ostatnio otwartych plików, wy¶wietlanych w menu Plik} 
# ====== TODO To be translated ======
menuText P OptionsBooksDir "Books directory" 0 {Sets the opening books directory}
# ====== TODO To be translated ======
menuText P OptionsTacticsBasesDir "Bases directory" 0 {Sets the tactics (training) bases directory}
# ====== TODO To be translated ======
menuText P OptionsPhotosDir "Photos directory" 0 {Sets the photos bases directory}
# ====== TODO To be translated ======
menuText P OptionsThemeDir "Theme(s) File"  0 { Load a GUI theme package file }
menuText P OptionsSave "Zapamiêtaj opcje" 0 \
  "Zapamiêtaj wszystkie ustawienia w pliku $::optionsFile"
menuText P OptionsAutoSave "Automatycznie zapisuj opcje" 0 \
  {Automatycznie zapisz opcje przy zamykaniu programu}

menuText P Help "Pomoc" 2
menuText P HelpContents "Contents" 0 {Poka¿ spis tre¶ci pomocy} 
menuText P HelpIndex "Spis tre¶ci" 0 {Poka¿ indeks pomocy}
menuText P HelpGuide "Krótki przewodnik" 0 {Poka¿ krótki przewodnik}
menuText P HelpHints "Podpowiedzi" 0 {Poka¿ podpowiedzi}
menuText P HelpContact "Informacja o autorze" 0 \
  {Poka¿ informacjê o autorze i stronie Scid-a}
menuText P HelpTip "Porada dnia" 0 {Poka¿ poradê Scida}
menuText P HelpStartup "Okno powitalne" 2 {Pokazuj okno startowe}
menuText P HelpAbout "O programie" 0 {Informacje o programie Scid}

# Game info box popup menu:
menuText P GInfoHideNext "Ukryj nastêpne posuniêcie" 0
menuText P GInfoMaterial "Poka¿ materia³" 0
menuText P GInfoFEN "Poka¿ pozycjê w formacie FEN" 16
menuText P GInfoMarks "Pokazuj kolorowe pola i strza³ki" 5 
menuText P GInfoWrap "Zawijaj d³ugie linie" 0
menuText P GInfoFullComment "Poka¿ ca³y komentarz" 6
menuText P GInfoPhotos "Poka¿ zdjêcia" 5
menuText P GInfoTBNothing "Tablica koñcówek: nic" 0
menuText P GInfoTBResult "Tablica koñcówek: tylko wynik" 18
menuText P GInfoTBAll "Tablica koñcówek: wszystko" 18
menuText P GInfoDelete "Usuñ/przywróæ tê partiê" 0
menuText P GInfoMark "W³±cz/wy³±cz zaznaczenie tej partii" 0
# ====== TODO To be translated ======
menuText P GInfoInformant "Configure informant values" 0

# General buttons:
translate P Back {Z powrotem}
translate P Browse {Przegl±daj}
translate P Cancel {Anuluj}
# ====== TODO To be translated ======
translate P Continue {Continue}
translate P Clear {Wyczy¶æ}
translate P Close {Zamknij}
translate P Contents {Spis tre¶ci}
translate P Defaults {Domy¶lne}
translate P Delete {Usuñ}
translate P Graph {Wykres}
translate P Help {Pomoc}
translate P Import {Pobierz}
translate P Index {Indeks}
translate P LoadGame {Wczytaj partiê}
translate P BrowseGame {Przegl±daj partiê}
translate P MergeGame {Do³±cz partiê}
# ====== TODO To be translated ======
translate P MergeGames {Merge Games}
translate P Preview {Podgl±d}
translate P Revert {Odwróæ}
translate P Save {Zapisz}
translate P Search {Szukaj}
translate P Stop {Stop}
translate P Store {Zapamiêtaj}
translate P Update {Uaktualnij}
translate P ChangeOrient {Zmieñ po³o¿enie okna}
translate P ShowIcons {Show Icons} ;# ***
translate P None {Brak}
translate P First {Pierwsza}
translate P Current {Aktualn±}
translate P Last {Ostatni±}

# General messages:
translate P game {partia}
translate P games {partie}
translate P move {posuniêcie}
translate P moves {pos.}
translate P all {wszystkie}
translate P Yes {Tak}
translate P No {Nie}
translate P Both {Oba}
translate P King {Król}
translate P Queen {Hetman}
translate P Rook {Wie¿a}
translate P Bishop {Goniec}
translate P Knight {Skoczek}
translate P Pawn {Pion}
translate P White {Bia³e}
translate P Black {Czarne}
translate P Player {Gracz}
translate P Rating {Ranking}
translate P RatingDiff {Ró¿nica rankingów}
translate P AverageRating {¦redni ranking}
translate P Event {Turniej}
translate P Site {Miejsce}
translate P Country {Kraj}
translate P IgnoreColors {Ignoruj kolory}
translate P Date {Data}
translate P EventDate {Turniej data}
translate P Decade {Dekada} 
translate P Year {Rok}
translate P Month {Miesi±c}
translate P Months {Styczeñ Luty Marzec Kwiecieñ Maj Czerwiec Lipiec Sierpieñ Wrzesieñ Pa¼dziernik Listopad Grudzieñ}
translate P Days {N Pn Wt ¦r Cz Pt So}
translate P YearToToday {Ostatni rok}
# ====== TODO To be translated ======
translate P YearToTodayTooltip {Set date from 1 year back to today}
translate P Result {Wynik}
translate P Round {Runda}
translate P Length {D³ugo¶æ}
translate P ECOCode {Kod ECO}
translate P ECO {ECO}
translate P Deleted {Usuniêta}
translate P SearchResults {Wyniki wyszukiwania}
translate P OpeningTheDatabase "Otwieranie bazy"
translate P Database {Bazy}
translate P Filter {Filtr}
translate P noGames {brak partii}
translate P allGames {wszystkie partie}
translate P empty {brak}
translate P clipbase {schowek}
translate P score {punkty}
translate P StartPos {Pozycja pocz±tkowa}
translate P Total {Razem}
translate P readonly {tylko do odczytu}

# Standard error messages:
translate P ErrNotOpen {To nie jest otwarta baza.} 
translate P ErrReadOnly {Ta baza jest tylko do odczytu; nie mo¿na jej zmieniæ.}
translate P ErrSearchInterrupted {Wyszukiwanie zosta³o przerwane. Wyniki bêd± niepe³ne.}

# Game information:
translate P twin {powtórzona}
translate P deleted {usuniêta}
translate P comment {komentarz}
translate P hidden {ukryte}
translate P LastMove {Poprzednie}
translate P NextMove {nastêpne}
translate P GameStart {Pocz±tek partii}
translate P LineStart {Pocz±tek wariantu}
translate P GameEnd {Koniec partii}
translate P LineEnd {Koniec wariantu}

# Player information:
translate P PInfoAll {Wyniki - <b>wszystkie</b> partie}
translate P PInfoFilter {Wyniki - partie z <b>filtra</b>}
translate P PInfoAgainst {Wyniki - }
translate P PInfoMostWhite {Najczêstsze debiuty bia³ymi}
translate P PInfoMostBlack {Najczêstsze debiuty czarnymi}
translate P PInfoRating {Historia rankingu}
translate P PInfoBio {Biografia}
translate P PInfoEditRatings {Modyfikuj rankingi} 
# ====== TODO To be translated ======
translate P PInfoEloFile {File}

# Tablebase information:
translate P Draw {remis}
translate P stalemate {pat}
translate P withAllMoves {po dowolnym posuniêciu}
translate P withAllButOneMove {po dowolnym posuniêciu oprócz}
translate P with {po}
translate P only {tylko}
translate P lose {przegrywaj±}
translate P loses {przegrywa}
translate P allOthersLose {inne posuniêcia przegrywaj±}
translate P matesIn {matuj± w}
translate P hasCheckmated {matuj±}
translate P longest {najlepsze}
translate P WinningMoves {Wygrywaj±ce posuniêcia}
translate P DrawingMoves {Remisuj±ce posuniêcia} 
translate P LosingMoves {Przegrywaj±ce posuniêcia} 
translate P UnknownMoves {Posuniêcia o nieznanej ocenie} 
# ====== TODO To be translated ======
translate P SideLongest {Side    Longest    %     %     %\n}
# ====== TODO To be translated ======
translate P toNoveMate {to move   mate    Win  Draw  Loss\n}
# ====== TODO To be translated ======
translate P NoSummary {\nNo summary for this tablebase.}
# ====== TODO To be translated ======
translate P MutualZugzwang {Mutual zugzwangs: }
# ====== TODO To be translated ======
translate P SelectetZugzwang {\nSelected zugzwang positions:}
# ====== TODO To be translated ======
translate P WdrawBloss {White draws, Black loses:}
# ====== TODO To be translated ======
translate P BdrawWloss {Black draws, White loses:}
# ====== TODO To be translated ======
translate P AllLoss {" Whoever moves loses:     " }

# Tip of the day:
translate P Tip {Porada}
translate P TipAtStartup {Poka¿ poradê przy starcie}

# Tree window menus:
menuText P TreeFile "Plik" 0
# ====== TODO To be translated ======
menuText P TreeFileFillWithBase "Fill Cache with base" 0 {Fill the cache file with all games in current base}
# ====== TODO To be translated ======
menuText P TreeFileFillWithGame "Fill Cache with game" 0 {Fill the cache file with current game in current base}
# ====== TODO To be translated ======
menuText P TreeFileSetCacheSize "Cache size" 0 {Set the cache size}
# ====== TODO To be translated ======
menuText P TreeFileCacheInfo "Cache info" 0 {Get info on cache usage}
menuText P TreeFileSave "Zapisz bufor" 7 {Zapisz plik bufora (.stc)}
menuText P TreeFileFill "Twórz standardowy plik cache" 0 {Wstaw typowe pozycje debiutowe do bufora}
menuText P TreeFileBest "Najlepsze partie" 0 {Poka¿ listê najlepszych partii}
menuText P TreeFileGraph "Poka¿ wykres" 0 {Poka¿ wykres dla tej ga³êzi drzewa}
menuText P TreeFileCopy "Kopiuj drzewo do schowka" 0 \
  {Skopiuj drzewo ze statystykami do schowka}
menuText P TreeFileClose "Zamknij" 0 {Zamknij okno drzewa}
# ====== TODO To be translated ======
menuText P TreeMask "Mask" 0
# ====== TODO To be translated ======
menuText P TreeMaskNew "New" 0 {New mask}
# ====== TODO To be translated ======
menuText P TreeMaskOpen "Open" 0 {Open mask}
# ====== TODO To be translated ======
menuText P TreeMaskOpenRecent "Open recent" 0 {Open recent mask}
# ====== TODO To be translated ======
menuText P TreeMaskSave "Save" 0 {Save mask}
# ====== TODO To be translated ======
menuText P TreeMaskClose "Close" 0 {Close mask}
# ====== TODO To be translated ======
menuText P TreeMaskFillWithGame "Fill with game" 0 {Fill mask with game}
# ====== TODO To be translated ======
menuText P TreeMaskFillWithBase "Fill with base" 0 {Fill mask with all games in base}
# ====== TODO To be translated ======
menuText P TreeMaskInfo "Info" 0 {Show statistics for current mask}
# ====== TODO To be translated ======
menuText P TreeMaskDisplay "Display mask map" 0 {Show mask data in a tree form}
# ====== TODO To be translated ======
menuText P TreeMaskSearch "Search" 0 {Search in current mask}
menuText P TreeSort "Sortowanie" 0
menuText P TreeSortAlpha "Alfabetycznie" 0
menuText P TreeSortECO "Kod ECO" 0
menuText P TreeSortFreq "Czêsto¶æ" 0
menuText P TreeSortScore "Punkty" 0
menuText P TreeOpt "Opcje" 0
# ====== TODO To be translated ======
menuText P TreeOptSlowmode "slow mode" 0 {Slow mode for updates (high accuracy)}
# ====== TODO To be translated ======
menuText P TreeOptFastmode "Fast mode" 0 {Fast mode for updates (no move transposition)}
# ====== TODO To be translated ======
menuText P TreeOptFastAndSlowmode "Fast and slow mode" 0 {Fast mode then slow mode for updates}
# ====== TODO To be translated ======
menuText P TreeOptStartStop "Auto refreshing" 0 {Toggles automatic refreshing of the tree window}
menuText P TreeOptLock "Blokada" 0 {Zablokuj/odblokuj drzewo na aktualnej bazie}
menuText P TreeOptTraining "Trening" 0 {W³±cz/wy³±cz tryb treningowy}
menuText P TreeOptAutosave "Automatyczny zapis bufora" 0 \
  {Automatycznie zapisz plik bufora przy wyj¶ciu}
menuText P TreeHelp "Pomoc" 2
menuText P TreeHelpTree "Drzewo" 0
menuText P TreeHelpIndex "Spis tre¶ci" 0
translate P SaveCache {Zapisz bufor}
translate P Training {Trening}
translate P LockTree {Blokada}
translate P TreeLocked {zablokowane}
translate P TreeBest {Najlepsze}
translate P TreeBestGames {Najlepsze partie}
# Note: the next message is the tree window title row. After editing it,
# check the tree window to make sure it lines up with the actual columns.
translate P TreeTitleRow \
  {    Pos.   ECO       Czêsto¶æ     Wynik  Rav   Rperf Rok   %Remis}
# {    Move   ECO       Frequency    Score  AvElo Perf AvYear %Draws}
translate P TreeTotal {RAZEM}
# ====== TODO To be translated ======
translate P DoYouWantToSaveFirst {Do you want to save first}
# ====== TODO To be translated ======
translate P AddToMask {Add to Mask}
# ====== TODO To be translated ======
translate P RemoveFromMask {Remove from Mask}
# ====== TODO To be translated ======
translate P AddThisMoveToMask {Add this move to Mask}
# ====== TODO To be translated ======
translate P SearchMask {Search in Mask}
# ====== TODO To be translated ======
translate P DisplayMask {Display Mask}
# ====== TODO To be translated ======
translate P Nag {Nag code}
# ====== TODO To be translated ======
translate P Marker {Marker}
# ====== TODO To be translated ======
translate P Include {Include}
# ====== TODO To be translated ======
translate P Exclude {Exclude}
# ====== TODO To be translated ======
translate P MainLine {Main line}
# ====== TODO To be translated ======
translate P Bookmark {Bookmark}
# ====== TODO To be translated ======
translate P NewLine {New line}
# ====== TODO To be translated ======
translate P ToBeVerified {To be verified}
# ====== TODO To be translated ======
translate P ToTrain {To train}
# ====== TODO To be translated ======
translate P Dubious {Dubious}
# ====== TODO To be translated ======
translate P ToRemove {To remove}
# ====== TODO To be translated ======
translate P NoMarker {No marker}
# ====== TODO To be translated ======
translate P ColorMarker {Color}
# ====== TODO To be translated ======
translate P WhiteMark {White}
# ====== TODO To be translated ======
translate P GreenMark {Green}
# ====== TODO To be translated ======
translate P YellowMark {Yellow}
# ====== TODO To be translated ======
translate P BlueMark {Blue}
# ====== TODO To be translated ======
translate P RedMark {Red}
# ====== TODO To be translated ======
translate P CommentMove {Comment move}
# ====== TODO To be translated ======
translate P CommentPosition {Comment position}
# ====== TODO To be translated ======
translate P AddMoveToMaskFirst {Add move to mask first}
# ====== TODO To be translated ======
translate P OpenAMaskFileFirst {Open a mask file first}
# ====== TODO To be translated ======
translate P Positions {Positions}
# ====== TODO To be translated ======
translate P Moves {Moves}

# Finder window:
menuText P FinderFile "Plik" 0
menuText P FinderFileSubdirs "Przeszukuj podkatalogi" 0
menuText P FinderFileClose "Zamknij wyszukiwacza plików" 0
menuText P FinderSort "Sortowanie" 0
menuText P FinderSortType "Typ" 0
menuText P FinderSortSize "Rozmiar" 0
menuText P FinderSortMod "Zmieniony" 0
menuText P FinderSortName "Nazwa" 0
menuText P FinderSortPath "¦cie¿ka" 0
menuText P FinderTypes "Typy" 0
menuText P FinderTypesScid "Bazy Scid-a" 0
menuText P FinderTypesOld "Bazy Scid-a (stary format)" 1
menuText P FinderTypesPGN "Pliki PGN" 0
menuText P FinderTypesEPD "Ksi±¿ki debiutowe EPD" 0
menuText P FinderTypesRep "Pliki repertuaru debiutowego" 0
menuText P FinderHelp "Pomoc" 2
menuText P FinderHelpFinder "Pomoc poszukiwacza plików" 1
menuText P FinderHelpIndex "Spis tre¶ci" 0
translate P FileFinder {Poszukiwacz plików}
translate P FinderDir {Katalog}
translate P FinderDirs {Katalogi}
translate P FinderFiles {Pliki}
translate P FinderUpDir {wy¿ej}
# ====== TODO To be translated ======
translate P FinderCtxOpen {Open}
# ====== TODO To be translated ======
translate P FinderCtxBackup {Backup}
# ====== TODO To be translated ======
translate P FinderCtxCopy {Copy}
# ====== TODO To be translated ======
translate P FinderCtxMove {Move}
# ====== TODO To be translated ======
translate P FinderCtxDelete {Delete}

# Player finder:
menuText P PListFile "Plik" 0
menuText P PListFileUpdate "Uaktualnij" 0
menuText P PListFileClose "Zamknij przegl±darkê zawodników" 0
menuText P PListSort "Sortowanie" 0
menuText P PListSortName "Nazwisko" 0
menuText P PListSortElo "Elo" 0
menuText P PListSortGames "Partie" 0
menuText P PListSortOldest "Najstarsza" 0
menuText P PListSortNewest "Najnowsza" 0

# Tournament finder:
menuText P TmtFile "Plik" 0
menuText P TmtFileUpdate "Uaktualnij" 0
menuText P TmtFileClose "Zamknij turnieje" 0
menuText P TmtSort "Sortowanie" 0
menuText P TmtSortDate "Data" 0
menuText P TmtSortPlayers "Zawodnicy" 0
menuText P TmtSortGames "Partie" 0
menuText P TmtSortElo "Elo" 0
menuText P TmtSortSite "Miejsce" 0
menuText P TmtSortEvent "Turniej" 0
menuText P TmtSortWinner "Zwyciêzca" 0
translate P TmtLimit "Wielko¶æ listy"
translate P TmtMeanElo "Min. ¶rednie ELO"
translate P TmtNone "Nie znaleziono turniejów."

# Graph windows:
menuText P GraphFile "Plik" 0
menuText P GraphFileColor "Zapisz jako kolorowy PostScript" 12
menuText P GraphFileGrey "Zapisz jako zwyk³y PostScript..." 0
menuText P GraphFileClose "Zamknij okno" 6
menuText P GraphOptions "Opcje" 0
menuText P GraphOptionsWhite "Bia³e" 0
menuText P GraphOptionsBlack "Czarne" 0
menuText P GraphOptionsBoth "Oba kolory" 1
menuText P GraphOptionsPInfo "Gracz z Informacji o graczu" 0
# ====== TODO To be translated ======
menuText P GraphOptionsEloFile "Elo from rating file" 0
# ====== TODO To be translated ======
menuText P GraphOptionsEloDB "Elo from database" 0
translate P GraphFilterTitle "Filtr: czêsto¶æ na 1000 partii" 
# ====== TODO To be translated ======
translate P GraphAbsFilterTitle "Filter Graph: frequency of the games"
# ====== TODO To be translated ======
translate P ConfigureFilter {Configure X-Axes for Year, Rating and Moves}
# ====== TODO To be translated ======
translate P FilterEstimate "Estimate"
# ====== TODO To be translated ======
translate P TitleFilterGraph "Scid: Filter Graph"

# Analysis window:
translate P AddVariation {Dodaj wariant}
# ====== TODO To be translated ======
translate P AddAllVariations {Add All Variations}
translate P AddMove {Dodaj posuniêcie}
translate P Annotate {Komentuj}
# ====== TODO To be translated ======
translate P ShowAnalysisBoard {Show analysis board}
# ====== TODO To be translated ======
translate P ShowInfo {Show engine info}
# ====== TODO To be translated ======
translate P FinishGame {Finish game}
# ====== TODO To be translated ======
translate P StopEngine {Stop engine}
# ====== TODO To be translated ======
translate P StartEngine {Start engine}
# ====== TODO To be translated ======
translate P LockEngine {Lock engine to current position}
translate P AnalysisCommand {Program do analizy}
translate P PreviousChoices {Poprzednie programy}
translate P AnnotateTime {Czas miêdzy ruchami (w sekundach)}
translate P AnnotateWhich {Dodaj warianty}
translate P AnnotateAll {Dla obu stron}
# ====== TODO To be translated ======
translate P AnnotateAllMoves {Annotate all moves}
translate P AnnotateWhite {Dla bia³ych}
translate P AnnotateBlack {Dla czarnych}
# ====== TODO To be translated ======
translate P AnnotateBlundersOnly {When game move is an obvious blunder}
# ====== TODO To be translated ======
translate P AnnotateBlundersOnlyScoreChange {Analysis reports blunder, with score change from/to: }
# ====== TODO To be translated ======
translate P BlundersThreshold {Threshold}
# TODO: Translate
translate P ScoreAllMoves {Score all moves}
translate P LowPriority {Niski priorytet CPU} 
# ====== TODO To be translated ======
translate P ClickHereToSeeMoves {Click here to see moves}
# ====== TODO To be translated ======
translate P ConfigureInformant {Configure Informant}
# ====== TODO To be translated ======
translate P Informant!? {Interesting move}
# ====== TODO To be translated ======
translate P Informant? {Poor move}
# ====== TODO To be translated ======
translate P Informant?? {Blunder}
# ====== TODO To be translated ======
translate P Informant?! {Dubious move}
# ====== TODO To be translated ======
translate P Informant+= {White has a slight advantage}
# ====== TODO To be translated ======
translate P Informant+/- {White has a moderate advantage}
# ====== TODO To be translated ======
translate P Informant+- {White has a decisive advantage}
# ====== TODO To be translated ======
translate P Informant++- {The game is considered won}
# ====== TODO To be translated ======
translate P Book {Book}
# ====== TODO To be translated ======
translate P OtherBookMoves {Opponent's book}
# ====== TODO To be translated ======
translate P OtherBookMovesTooltip {Moves to which the opponent has a reply}

# Analysis Engine open dialog:
translate P EngineList {Programy szachowe}
translate P EngineName {Nazwa}
translate P EngineCmd {Polecenie}
translate P EngineArgs {Parametry} 
translate P EngineDir {Katalog}
translate P EngineElo {Elo}
translate P EngineTime {Data}
translate P EngineNew {Dodaj}
translate P EngineEdit {Edytuj}
translate P EngineRequired {Pola wyt³uszczone s± konieczne; reszta opcjonalna} 

# PGN window menus:
menuText P PgnFile "Plik" 0
menuText P PgnFileCopy "Kopiuj partiê do schowka" 0
menuText P PgnFilePrint "Zapisz do pliku..." 7
menuText P PgnFileClose "Zamknij" 0
menuText P PgnOpt "Wygl±d" 0
menuText P PgnOptColor "Wy¶wietlanie w kolorach" 0
menuText P PgnOptShort "Krótki (3-wierszowy) nag³ówek" 0
menuText P PgnOptSymbols "Symbole Informatora" 0
menuText P PgnOptIndentC "Wcinaj komentarze" 7
menuText P PgnOptIndentV "Wcinaj warianty" 7
menuText P PgnOptColumn "Kolumny (jedno posuniêcie w wierszu)" 0
menuText P PgnOptSpace "Spacja po numerze ruchu" 0
menuText P PgnOptStripMarks "Usuñ kody kolorowych pól i strza³ek" 0
menuText P PgnOptBoldMainLine "Use Bold Text for Main Line Moves" 4 ;# ***
menuText P PgnColor "Kolory" 0
menuText P PgnColorHeader "Nag³ówek..." 0
menuText P PgnColorAnno "Uwagi..." 3
menuText P PgnColorComments "Komentarze..." 0
menuText P PgnColorVars "Warianty..." 0
menuText P PgnColorBackground "T³o..." 0
# ====== TODO To be translated ======
menuText P PgnColorMain "Main line..." 0
# ====== TODO To be translated ======
menuText P PgnColorCurrent "Current move background..." 1
menuText P PgnHelp "Pomoc" 2
menuText P PgnHelpPgn "PGN" 0
menuText P PgnHelpIndex "Spis tre¶ci" 0
translate P PgnWindowTitle {Game Notation - game %u} ;# ***

# Crosstable window menus:
menuText P CrosstabFile "Plik" 0
menuText P CrosstabFileText "Zapisz w pliku tekstowym..." 15
menuText P CrosstabFileHtml "Zapisz w pliku HTML..." 15
menuText P CrosstabFileLaTeX "Zapisz w pliku LaTeX-a..." 15
menuText P CrosstabFileClose "Zamknij" 0
menuText P CrosstabEdit "Edytuj" 0
menuText P CrosstabEditEvent "Turniej" 0
menuText P CrosstabEditSite "Miejsce" 0
menuText P CrosstabEditDate "Data" 0
menuText P CrosstabOpt "Wy¶wietlanie" 0
menuText P CrosstabOptAll "Turniej ko³owy" 0
menuText P CrosstabOptSwiss "Szwajcar" 0
menuText P CrosstabOptKnockout "Knockout" 0
menuText P CrosstabOptAuto "Automatycznie" 0
menuText P CrosstabOptAges "Wiek" 0
menuText P CrosstabOptNats "Narodowo¶æ" 0
menuText P CrosstabOptRatings "Ranking" 0
menuText P CrosstabOptTitles "Tytu³" 0
menuText P CrosstabOptBreaks "Punkty pomocnicze" 1
menuText P CrosstabOptDeleted "Uwzglêdniaj usuniête partie" 0
menuText P CrosstabOptColors "Kolory (tylko szwajcar)" 0
menuText P CrosstabOptColumnNumbers "Numerowane kolumny (tylko turniej ko³owy)" 0
menuText P CrosstabOptGroup "Grupuj po liczbie punktów" 0
menuText P CrosstabSort "Sortowanie" 0
menuText P CrosstabSortName "Nazwisko" 0
menuText P CrosstabSortRating "Ranking" 0
menuText P CrosstabSortScore "Punkty" 0
menuText P CrosstabColor "Kolor" 0
menuText P CrosstabColorPlain "Zwyk³y tekst" 0
menuText P CrosstabColorHyper "Hipertekst" 0
menuText P CrosstabHelp "Pomoc" 2
menuText P CrosstabHelpCross "Tabela turniejowa" 0
menuText P CrosstabHelpIndex "Spis tre¶ci" 0
translate P SetFilter {Ustaw filtr}
translate P AddToFilter {Dodaj do filtra}
translate P Swiss {Szwajcar}
translate P Category {Kategoria} 

# Opening report window menus:
menuText P OprepFile "Plik" 0
menuText P OprepFileText "Zapisz w pliku tekstowym..." 15
menuText P OprepFileHtml "Zapisz w pliku HTML..." 15
menuText P OprepFileLaTeX "Zapisz w pliku LaTeX-a..." 15
menuText P OprepFileOptions "Opcje" 2
menuText P OprepFileClose "Zamknij okno raportu" 0
menuText P OprepFavorites "Ulubione" 1 
menuText P OprepFavoritesAdd "Dodaj raport..." 0 
menuText P OprepFavoritesEdit "Modyfikuj ulubione..." 0
menuText P OprepFavoritesGenerate "Twórz raporty..." 0 
menuText P OprepHelp "Pomoc" 2
menuText P OprepHelpReport "Pomoc raportu debiutowego" 0
menuText P OprepHelpIndex "Spis tre¶ci" 0

# Header search:
translate P HeaderSearch {Wyszukiwanie wg nag³ówka}
translate P EndSideToMove {Side to move at end of game} ;# ***
translate P GamesWithNoECO {Partie bez ECO?}
translate P GameLength {D³ugo¶æ}
translate P FindGamesWith {Znajd¼ partie}
translate P StdStart {ca³a partia}
translate P Promotions {z promocj±}
translate P Comments {Komentarze}
translate P Variations {Warianty}
translate P Annotations {Uwagi}
translate P DeleteFlag {Usuwanie}
translate P WhiteOpFlag {Debiut - bia³e}
translate P BlackOpFlag {Debiut - czarne}
translate P MiddlegameFlag {Gra ¶rodkowa}
translate P EndgameFlag {Koñcówka}
translate P NoveltyFlag {Nowinka}
translate P PawnFlag {Struktura pionowa}
translate P TacticsFlag {Taktyka}
translate P QsideFlag {Gra na skrzydle hetmañskim}
translate P KsideFlag {Gra na skrzydle królewskim}
translate P BrilliancyFlag {Nagroda za piêkno¶æ}
translate P BlunderFlag {Podstawka}
translate P UserFlag {Inne}
translate P PgnContains {PGN zawiera tekst}
# ====== TODO To be translated ======
translate P Annotator {Annotator}
# ====== TODO To be translated ======
translate P Cmnts {Annotated games only}

# Game list window:
translate P GlistNumber {Numer}
translate P GlistWhite {Bia³e}
translate P GlistBlack {Czarne}
translate P GlistWElo {B-Elo}
translate P GlistBElo {C-Elo}
translate P GlistEvent {Turniej}
translate P GlistSite {Miejsce}
translate P GlistRound {Runda}
translate P GlistDate {Data}
translate P GlistYear {Rok}
translate P GlistEDate {Turniej-Data}
translate P GlistResult {Wynik}
translate P GlistLength {D³ugo¶æ}
translate P GlistCountry {Kraj}
translate P GlistECO {ECO}
translate P GlistOpening {Debiut}
translate P GlistEndMaterial {Materia³}
translate P GlistDeleted {Usuniêta}
translate P GlistFlags {Oznaczenie}
translate P GlistVars {Warianty}
translate P GlistComments {Komentarze}
translate P GlistAnnos {Uwagi}
translate P GlistStart {Pozycja pocz±tkowa}
translate P GlistGameNumber {Numer partii}
# ====== TODO To be translated ======
translate P GlistAverageElo {Average Elo}
# ====== TODO To be translated ======
translate P GlistRating {Rating}
translate P GlistFindText {Znajd¼ tekst}
translate P GlistMoveField {Przesuñ}
translate P GlistEditField {Konfiguruj}
translate P GlistAddField {Dodaj}
translate P GlistDeleteField {Usuñ}
translate P GlistWidth {Szeroko¶æ}
translate P GlistAlign {Wyrównanie}
# ====== TODO To be translated ======
translate P GlistAlignL {Align: left}
# ====== TODO To be translated ======
translate P GlistAlignR {Align: right}
# ====== TODO To be translated ======
translate P GlistAlignC {Align: center}
translate P GlistColor {Kolor}
translate P GlistSep {Separator}
# ====== TODO To be translated ======
translate P GlistCurrentSep {-- Current --}
# ====== TODO To be translated ======
translate P GlistNewSort {New}
# ====== TODO To be translated ======
translate P GlistAddToSort {Add}
# ====== TODO To be translated ======
translate P GsortSort {Sort...}
# ====== TODO To be translated ======
translate P GsortDate {Date}
# ====== TODO To be translated ======
translate P GsortYear {Year}
# ====== TODO To be translated ======
translate P GsortEvent {Event}
# ====== TODO To be translated ======
translate P GsortSite {Site}
# ====== TODO To be translated ======
translate P GsortRound {Round}
# ====== TODO To be translated ======
translate P GsortWhiteName {White Name}
# ====== TODO To be translated ======
translate P GsortBlackName {Black Name}
# ====== TODO To be translated ======
translate P GsortECO {ECO}
# ====== TODO To be translated ======
translate P GsortResult {Result}
# ====== TODO To be translated ======
translate P GsortMoveCount {Move Count}
# ====== TODO To be translated ======
translate P GsortAverageElo {Average Elo}
# ====== TODO To be translated ======
translate P GsortCountry {Country}
# ====== TODO To be translated ======
translate P GsortDeleted {Deleted}
# ====== TODO To be translated ======
translate P GsortEventDate {Event Date}
# ====== TODO To be translated ======
translate P GsortWhiteElo {White Elo}
# ====== TODO To be translated ======
translate P GsortBlackElo {Black Elo}
# ====== TODO To be translated ======
translate P GsortComments {Comments}
# ====== TODO To be translated ======
translate P GsortVariations {Variations}
# ====== TODO To be translated ======
translate P GsortNAGs {NAGs}
# ====== TODO To be translated ======
translate P GsortAscending {Ascending}
# ====== TODO To be translated ======
translate P GsortDescending {Descending}
# ====== TODO To be translated ======
translate P GsortAdd {Add}
# ====== TODO To be translated ======
translate P GsortStore {Store}
# ====== TODO To be translated ======
translate P GsortLoad {Load}
# ====== TODO To be translated ======
translate P GlistRemoveThisGameFromFilter  {Remove this game from Filter}
# ====== TODO To be translated ======
translate P GlistRemoveGameAndAboveFromFilter  {Remove game (and all above it) from Filter}
# ====== TODO To be translated ======
translate P GlistRemoveGameAndBelowFromFilter  {Remove game (and all below it) from Filter}
# ====== TODO To be translated ======
translate P GlistDeleteGame {(Un)Delete this game} 
# ====== TODO To be translated ======
translate P GlistDeleteAllGames {Delete all games in filter} 
# ====== TODO To be translated ======
translate P GlistUndeleteAllGames {Undelete all games in filter} 
# ====== TODO To be translated ======
translate P GlistMergeGameInBase {Merge Game in other base} 

# Maintenance window:
translate P DatabaseName {Nazwa bazy:}
translate P TypeIcon {Ikona:}
translate P NumOfGames {Liczba partii:}
translate P NumDeletedGames {Liczba usuniêtych partii:}
translate P NumFilterGames {Liczba partii w filtrze:}
translate P YearRange {Data:}
translate P RatingRange {Ranking:}
translate P Description {Opis} 
translate P Flag {Oznaczenie:}
# ====== TODO To be translated ======
translate P CustomFlags {Custom flags}
translate P DeleteCurrent {Usuñ aktualn± partiê}
translate P DeleteFilter {Usuñ partie z filtra}
translate P DeleteAll {Usuñ wszystkie partie}
translate P UndeleteCurrent {Odzyskaj aktualn± partiê}
translate P UndeleteFilter {Odzyskaj partie z filtra}
translate P UndeleteAll {Odzyskaj wszystkie partie}
translate P DeleteTwins {Usuñ powtórzone partie}
translate P MarkCurrent {Zaznacz aktualn± partiê}
translate P MarkFilter {Zaznacz partie z filtra}
translate P MarkAll {Zaznacz wszystkie partie z filtra}
translate P UnmarkCurrent {Usuñ zaznaczenie aktualnej partii}
translate P UnmarkFilter {Usuñ zaznaczenie partii z filtra}
translate P UnmarkAll {Usuñ zaznaczenie wszystkich partii}
translate P Spellchecking {Pisownia}
translate P Players {Zawodnicy}
translate P Events {Turnieje}
translate P Sites {Miejsca}
translate P Rounds {Rundy}
translate P DatabaseOps {Operacje bazodanowe}
translate P ReclassifyGames {Klasyfikacja debiutowa}
translate P CompactDatabase {Uporz±dkuj bazê}
translate P SortDatabase {Sortuj bazê}
translate P AddEloRatings {Dodaj rankingi ELO}
translate P AutoloadGame {Domy¶lna partia}
translate P StripTags {Usuñ znaczniki PGN} 
translate P StripTag {Usuñ znacznik}
translate P Cleaner {Zestaw zadañ}
translate P CleanerHelp {
Zestaw zadañ pozwala wykonaæ od razu kilka operacji porz±dkowania bazy. Operacje wybrane z listy
zostan± wykonane na aktualnej bazie.

Do klasyfikacji debiutowej i usuwania powtórzonych partii u¿yte zostan± aktualne ustawienia.
}
translate P CleanerConfirm {
Kiedy wykonanie zestawu zadañ zostanie rozpoczête, nie bêdzie mo¿na ju¿ go przerwaæ.

Na du¿ej bazie mo¿e to zaj±æ du¿o czasu (zale¿y to równie¿ od wybranego zestawu zadañ i ich
ustawieñ).

Na pewno wykonaæ wybrane zadania?
}
# ====== TODO To be translated ======
translate P TwinCheckUndelete {to flip; "u" undeletes both)}
# ====== TODO To be translated ======
translate P TwinCheckprevPair {Previous pair}
# ====== TODO To be translated ======
translate P TwinChecknextPair {Next pair}
# ====== TODO To be translated ======
translate P TwinChecker {Scid: Twin game checker}
# ====== TODO To be translated ======
translate P TwinCheckTournament {Games in tournament:}
# ====== TODO To be translated ======
translate P TwinCheckNoTwin {No twin  }
# ====== TODO To be translated ======
translate P TwinCheckNoTwinfound {No twin was detected for this game.\nTo show twins using this window, you must first use the "Delete twin games..." function. }
# ====== TODO To be translated ======
translate P TwinCheckTag {Share tags...}
# ====== TODO To be translated ======
translate P TwinCheckFound1 {Scid found $result twin games}
# ====== TODO To be translated ======
translate P TwinCheckFound2 { and set their delete flags}
# ====== TODO To be translated ======
translate P TwinCheckNoDelete {There are no games in this database to delete.}
# ====== TODO To be translated ======
translate P TwinCriteria1 { Your settings for finding twin games are potentially likely to\ncause non-twin games with similar moves to be marked as twins.}
# ====== TODO To be translated ======
translate P TwinCriteria2 {It is recommended that if you select "No" for "same moves", you should select "Yes" for the colors, event, site, round, year and month settings.\nDo you want to continue and delete twins anyway? }
# ====== TODO To be translated ======
translate P TwinCriteria3 {It is recommended that you specify "Yes" for at least two of the "same site", "same round" and "same year" settings.\nDo you want to continue and delete twins anyway?}
# ====== TODO To be translated ======
translate P TwinCriteriaConfirm {Scid: Confirm twin settings}
# ====== TODO To be translated ======
translate P TwinChangeTag "Change the following game tags:\n\n"
# ====== TODO To be translated ======
translate P AllocRatingDescription "This command will use the current spellcheck file to add Elo ratings to games in this database. Wherever a player has no currrent rating but his/her rating at the time of the game is listed in the spellcheck file, that rating will be added."
# ====== TODO To be translated ======
translate P RatingOverride "Overwrite existing non-zero ratings?"
# ====== TODO To be translated ======
translate P AddRatings "Add ratings to:"
# ====== TODO To be translated ======
translate P AddedRatings {Scid added $r Elo ratings in $g games.}
# ====== TODO To be translated ======
translate P NewSubmenu "New submenu"

# Comment editor:
translate P AnnotationSymbols  {Symbole:}
translate P Comment {Komentarz:}
translate P InsertMark {Wstaw znak}
translate P InsertMarkHelp {
Insert/remove mark: Select color, type, square.
Insert/remove arrow: Right-click two squares.
} ;# ***

# Nag buttons in comment editor:
translate P GoodMove {Good move} ;# ***
translate P PoorMove {Poor move} ;# ***
translate P ExcellentMove {Excellent move} ;# ***
translate P Blunder {Podstawka}
translate P InterestingMove {Interesting move} ;# ***
translate P DubiousMove {Dubious move} ;# ***
translate P WhiteDecisiveAdvantage {White has a decisive advantage} ;# ***
translate P BlackDecisiveAdvantage {Black has a decisive advantage} ;# ***
translate P WhiteClearAdvantage {White has a clear advantage} ;# ***
translate P BlackClearAdvantage {Black has a clear advantage} ;# ***
translate P WhiteSlightAdvantage {White has a slight advantage} ;# ***
translate P BlackSlightAdvantage {Black has a slight advantage} ;# ***
translate P Equality {Equality} ;# ***
translate P Unclear {Unclear} ;# ***
translate P Diagram {Diagram} ;# ***

# Board search:
translate P BoardSearch {Wyszukiwanie wg pozycji}
translate P FilterOperation {Operacje na aktualnym filtrze:}
translate P FilterAnd {I (ogranicz filtr)}
translate P FilterOr {LUB (dodaj do filtra)}
translate P FilterIgnore {NOWY (ignoruj poprzedni filtr)}
translate P SearchType {Typ wyszukiwania:}
translate P SearchBoardExact {Identyczna pozycja (bierki na tych samych polach)}
translate P SearchBoardPawns {Pionki (ten sam materia³, pionki na tych samych polach)}
translate P SearchBoardFiles {Kolumny (ten sam materia³, pionki na tych samych kolumnach)}
translate P SearchBoardAny {Materia³ (ten sam materia³, pozycja dowolna)}
# ====== TODO To be translated ======
translate P SearchInRefDatabase { Search in base }
translate P LookInVars {Przeszukuj warianty}

# Material search:
translate P MaterialSearch {Wyszukiwanie wg materia³u}
translate P Material {Materia³}
translate P Patterns {Wzorce}
translate P Zero {Brak}
translate P Any {Dowolny}
translate P CurrentBoard {Aktualna pozycja}
translate P CommonEndings {Typowe koñcówki}
translate P CommonPatterns {Typowe wzorce}
translate P MaterialDiff {Przewaga materialna}
translate P squares {pola}
translate P SameColor {jednopolowe}
translate P OppColor {ró¿nopolowe}
translate P Either {dowolne}
translate P MoveNumberRange {Zakres posuniêæ}
translate P MatchForAtLeast {Pasuje min.}
translate P HalfMoves {pó³ruchy}

# Common endings in material search:
translate P EndingPawns {Pawn endings} ;# ***
translate P EndingRookVsPawns {Rook vs. Pawn(s)} ;# ***
translate P EndingRookPawnVsRook {Rook and 1 Pawn vs. Rook} ;# ***
translate P EndingRookPawnsVsRook {Rook and Pawn(s) vs. Rook} ;# ***
translate P EndingRooks {Rook vs. Rook endings} ;# ***
translate P EndingRooksPassedA {Rook vs. Rook endings with a passed a-pawn} ;# ***
translate P EndingRooksDouble {Double Rook endings} ;# ***
translate P EndingBishops {Bishop vs. Bishop endings} ;# ***
translate P EndingBishopVsKnight {Bishop vs. Knight endings} ;# ***
translate P EndingKnights {Knight vs. Knight endings} ;# ***
translate P EndingQueens {Queen vs. Queen endings} ;# ***
translate P EndingQueenPawnVsQueen {Queen and 1 Pawn vs. Queen} ;# ***
translate P BishopPairVsKnightPair {Two Bishops vs. Two Knights middlegame} ;# ***

# Common patterns in material search:
translate P PatternWhiteIQP {White IQP} ;# ***
translate P PatternWhiteIQPBreakE6 {White IQP: d4-d5 break vs. e6} ;# ***
translate P PatternWhiteIQPBreakC6 {White IQP: d4-d5 break vs. c6} ;# ***
translate P PatternBlackIQP {Black IQP} ;# ***
translate P PatternWhiteBlackIQP {White IQP vs. Black IQP} ;# ***
translate P PatternCoupleC3D4 {White c3+d4 Isolated Pawn Couple} ;# ***
translate P PatternHangingC5D5 {Black Hanging Pawns on c5 and d5} ;# ***
translate P PatternMaroczy {Maroczy Center (with Pawns on c4 and e4)} ;# ***
translate P PatternRookSacC3 {Rook Sacrifice on c3} ;# ***
translate P PatternKc1Kg8 {O-O-O vs. O-O (Kc1 vs. Kg8)} ;# ***
translate P PatternKg1Kc8 {O-O vs. O-O-O (Kg1 vs. Kc8)} ;# ***
translate P PatternLightFian {Light-Square Fianchettos (Bishop-g2 vs. Bishop-b7)} ;# ***
translate P PatternDarkFian {Dark-Square Fianchettos (Bishop-b2 vs. Bishop-g7)} ;# ***
translate P PatternFourFian {Four Fianchettos (Bishops on b2,g2,b7,g7)} ;# ***

# Game saving:
translate P Today {Dzisiaj}
translate P ClassifyGame {Klasyfikacja debiutowa}

# Setup position:
translate P EmptyBoard {Pusta szachownica}
translate P InitialBoard {Pozycja pocz±tkowa}
translate P SideToMove {Na posuniêciu}
translate P MoveNumber {Posuniêcie nr}
translate P Castling {Roszada}
translate P EnPassantFile {Bicie w przelocie}
translate P ClearFen {Kopiuj FEN}
translate P PasteFen {Wklej pozycjê FEN}
# ====== TODO To be translated ======
translate P SaveAndContinue {Save and continue}
# ====== TODO To be translated ======
translate P DiscardChangesAndContinue {Discard changes\nand continue}
# ====== TODO To be translated ======
translate P GoBack {Go back}

# Replace move dialog:
translate P ReplaceMove {Zmieñ posuniêcie}
translate P AddNewVar {Dodaj wariant}
# ====== TODO To be translated ======
translate P NewMainLine {New Main Line}
translate P ReplaceMoveMessage {Posuniêcie ju¿ istnieje.

Mo¿esz je zast±piæ, usuwaj±c dalszy ci±g partii lub dodaæ nowy wariant.

(Mo¿na wy³±czyæ to ostrze¿enie, wy³±czaj±c opcjê  "Zapytaj przed zast±pieniem posuniêæ" w menu
Opcje:Posuniêcia)}

# Make database read-only dialog:
translate P ReadOnlyDialog {Je¶li zabezpieczysz tê bazê przed zapisem, zmiany bêd± zablokowane
¯adna partia nie bêdzie zapisana ani zmodyfikowana, ¿adne flagi nie bêd± zmienione.
Sortowanie i klasyfikacja debiutowa bêd± tylko tymczasowe.

¯eby usun±æ zabezpieczenie przez zapisem, wystarczy zamkn±æ bazê i otworzyæ j± ponownie.

Na pewno zabezpieczyæ bazê przed zapisem?}

# Clear game dialog:
translate P ClearGameDialog {Partia zosta³a zmieniona.

Na pewno kontynuowaæ, rezygnuj±c z wszelkich zmian?
}

# Exit dialog:
translate P ExitDialog {Na pewno zakoñczyæ pracê z programem?}
translate P ExitUnsaved {Nastêpuj±ce bazy zawieraj± niezapisane zmiany. Je¶li zamkniesz program teraz, zmiany zostan± utracone.} 

# Import window:
translate P PasteCurrentGame {Wklej aktualn± partiê}
translate P ImportHelp1 {Wprowad¼ lub wklej partiê w formacie PGN w poni¿sz± ramkê.}
translate P ImportHelp2 {Tu bêd± wy¶wietlane b³êdy przy importowaniu partii.}
# ====== TODO To be translated ======
translate P OverwriteExistingMoves {Overwrite existing moves ?}

# ECO Browser:
translate P ECOAllSections {Wszystkie kody ECO}
translate P ECOSection {Czê¶æ ECO}
translate P ECOSummary {Podsumowanie dla}
translate P ECOFrequency {Czêsto¶ci kodów dla}

# Opening Report:
translate P OprepTitle {Raport debiutowy}
translate P OprepReport {Raport}
translate P OprepGenerated {Utworzony przez}
translate P OprepStatsHist {Statystyka i historia}
translate P OprepStats {Statystyka}
translate P OprepStatAll {Wszystkie partie}
translate P OprepStatBoth {Obaj zawodnicy z Elo}
translate P OprepStatSince {Od}
translate P OprepOldest {Najdawniejsze partie}
translate P OprepNewest {Ostatnie partie}
translate P OprepPopular {Popularno¶æ}
translate P OprepFreqAll {Czêsto¶æ w ca³ej bazie:         }
translate P OprepFreq1   {W ostatnim roku:                }
translate P OprepFreq5   {W ostatnich piêciu latach:      }
translate P OprepFreq10  {W ostatnich dziesiêciu latach:  }
translate P OprepEvery {co %u partii}
translate P OprepUp {wiêcej o %u%s ni¿ w ca³ej bazie}
translate P OprepDown {mniej o %u%s ni¿ w ca³ej bazie}
translate P OprepSame {jak w ca³ej bazie}
translate P OprepMostFrequent {Gracze najczê¶ciej stosuj±cy wariant}
translate P OprepMostFrequentOpponents {Przeciwnicy} 
translate P OprepRatingsPerf {Rankingi i wyniki}
translate P OprepAvgPerf {¦rednie rankingi i wyniki}
translate P OprepWRating {Ranking bia³ych}
translate P OprepBRating {Ranking czarnych}
translate P OprepWPerf {Wynik bia³ych}
translate P OprepBPerf {Wynik czarnych}
translate P OprepHighRating {Partie graczy o najwy¿szym ¶rednim rankingu}
translate P OprepTrends {Wyniki}
translate P OprepResults {D³ugo¶æ partii i czêsto¶ci}
translate P OprepLength {D³ugo¶æ partii}
translate P OprepFrequency {Czêsto¶æ}
translate P OprepWWins {Zwyciêstwa bia³ych:  }
translate P OprepBWins {Zwyciêstwa czarnych: }
translate P OprepDraws {Remisy:              }
translate P OprepWholeDB {ca³a baza}
translate P OprepShortest {Najkrótsze zwyciêstwa}
translate P OprepMovesThemes {Posuniêcia i motywy}
translate P OprepMoveOrders {Posuniêcia prowadz±ce do badanej pozycji}
translate P OprepMoveOrdersOne \
  {Badana pozycja powstawa³a jedynie po posuniêciach:}
translate P OprepMoveOrdersAll \
  {Badana pozycja powstawa³a na %u sposobów:}
translate P OprepMoveOrdersMany \
  {Badana pozycja powstawa³a na %u sposobów. Najczêstsze %u to:}
translate P OprepMovesFrom {Posuniêcia w badanej pozycji}
translate P OprepMostFrequentEcoCodes {Najczêstsze kody ECO} 
translate P OprepThemes {Motywy pozycyjne}
translate P OprepThemeDescription {Czêsto¶æ motywów w pierwszych %u posuniêciach partii} 
translate P OprepThemeSameCastling {Jednostronne roszady}
translate P OprepThemeOppCastling {Ró¿nostronne roszady}
translate P OprepThemeNoCastling {Obie strony bez roszady}
translate P OprepThemeKPawnStorm {Atak pionowy na skrzydle królewskim}
translate P OprepThemeQueenswap {Wymiana hetmanów}
translate P OprepThemeWIQP {Izolowany pion bia³ych} 
translate P OprepThemeBIQP {Izolowany pion czarnych}
translate P OprepThemeWP567 {Bia³y pion na 5/6/7 linii}
translate P OprepThemeBP234 {Czarny pion na 2/3/4 linii}
translate P OprepThemeOpenCDE {Otwarta kolumna c/d/e}
translate P OprepTheme1BishopPair {Jedna ze stron ma parê goñców}
translate P OprepEndgames {Koñcówki}
translate P OprepReportGames {Partie raportu}
translate P OprepAllGames {Wszystkie partie}
translate P OprepEndClass {Materia³ w pozycji koñcowej}
translate P OprepTheoryTable {Teoria}
translate P OprepTableComment {Utworzono z %u partii o najwy¿szym ¶rednim rankingu.}
translate P OprepExtraMoves {Dodatkowe posuniêcia w przypisach}
translate P OprepMaxGames {Maksymalna liczba partii w teorii}
translate P OprepViewHTML {¬ród³o HTML} 
translate P OprepViewLaTeX {¬ród³o LaTeX} 

# Player Report:
translate P PReportTitle {Raport o graczu}
translate P PReportColorWhite {with the White pieces} ;# ***
translate P PReportColorBlack {with the Black pieces} ;# ***
translate P PReportMoves {po %s}
translate P PReportOpenings {Debiuty}
translate P PReportClipbase {Wyczy¶æ schowek i skopiuj do niego wybrane partie}

# Piece Tracker window:
translate P TrackerSelectSingle {Lewy przycisk wybiera tê figurê.}
translate P TrackerSelectPair {Lewy przycisk wybiera tê figurê; prawy obie takie figury.}
translate P TrackerSelectPawn {Lewy przycisk wybiera tego piona; prawy wszystkie 8 pionów.}
translate P TrackerStat {Statystyka}
translate P TrackerGames {% partie z posuniêciem na tym pole}
translate P TrackerTime {% czasu na tym polu}
translate P TrackerMoves {Posuniêcia}
translate P TrackerMovesStart {Podaj numer posuniêcia, od którego zacz±æ ¶ledzenie.}
translate P TrackerMovesStop {Podaj numer posuniêcia, na którym skoñczyæ ¶ledzenie.}

# Game selection dialogs:
translate P SelectAllGames {Wszystkie partie w bazie}
translate P SelectFilterGames {Partie w filtrze}
translate P SelectTournamentGames {Tylko partie z aktualnego turnieju}
translate P SelectOlderGames {Tylko wcze¶niejsze partie}

# Delete Twins window:
translate P TwinsNote {Partie zostan± uznane za identyczne, je¶li zosta³y rozegrane przez tych samych graczy i spe³niaj± ustawione poni¿ej kryteria. Krótsza z partii zostanie usuniêta. Uwaga: dobrze przez wyszukaniem powtórzonych partii sprawdziæ pisowniê nazwisk.}
translate P TwinsCriteria {Kryteria: co musi byæ jednakowe w obu partiach?}
translate P TwinsWhich {Przeszukiwane partie}
translate P TwinsColors {Kolory}
translate P TwinsEvent {Turniej:}
translate P TwinsSite {Miejsce:}
translate P TwinsRound {Runda:}
translate P TwinsYear {Rok:}
translate P TwinsMonth {Miesi±c:}
translate P TwinsDay {Dzieñ:}
translate P TwinsResult {Wynik:}
translate P TwinsECO {Kod ECO:}
translate P TwinsMoves {Posuniêcia:}
translate P TwinsPlayers {Porównywanie nazwisk:}
translate P TwinsPlayersExact {Dok³adne}
translate P TwinsPlayersPrefix {Tylko pierwsze 4 litery}
translate P TwinsWhen {Usuwanie znalezionych powtórzonych partii}
translate P TwinsSkipShort {Pomijaæ partie krótsze ni¿ 5 posuniêæ?}
translate P TwinsUndelete {Odzyskaæ wszystkie partie przed poszukiwaniem?}
translate P TwinsSetFilter {Wstawiæ wszystkie usuniête partie do filtra?}
translate P TwinsComments {Zawsze zachowywaæ partie komentowane?}
translate P TwinsVars {Zawsze zachowywaæ partie z wariantami?}
translate P TwinsDeleteWhich {Któr± partiê usun±æ:} 
translate P TwinsDeleteShorter {Krótsz±} 
translate P TwinsDeleteOlder {O ni¿szym numerze}
translate P TwinsDeleteNewer {O wy¿szym numerze}
translate P TwinsDelete {Usuñ partie}

# Name editor window:
translate P NameEditType {Nazwa do wyboru}
translate P NameEditSelect {Partie do edycji}
translate P NameEditReplace {Zast±p}
translate P NameEditWith {przez}
translate P NameEditMatches {Pasuj±ce: Ctrl+1 do Ctrl+9 wybiera}
# ====== TODO To be translated ======
translate P CheckGames {Check games}
# ====== TODO To be translated ======
translate P CheckGamesWhich {Check games}
# ====== TODO To be translated ======
translate P CheckAll {All games}
# ====== TODO To be translated ======
translate P CheckSelectFilterGames {Only games in filter}

# Classify window:
translate P Classify {Przyporz±dkowanie ECO}
translate P ClassifyWhich {Partie do przyporz±dkowania ECO}
translate P ClassifyAll {Wszystkie partie (zmiana starych kodów ECO)}
translate P ClassifyYear {Wszystkie partie z ostatniego roku}
translate P ClassifyMonth {Wszystkie partie z ostatniego miesi±ca}
translate P ClassifyNew {Tylko partie bez kodu ECO}
translate P ClassifyCodes {Kody ECO}
translate P ClassifyBasic {Tylko podstawowe ("B12", ...)}
translate P ClassifyExtended {Rozszerzone kody Scida ("B12j", ...)}

# Compaction:
translate P NameFile {Plik nazw}
translate P GameFile {Plik z partiami}
translate P Names {Nazwy}
translate P Unused {Nieu¿ywane}
translate P SizeKb {Rozmiar (kb)}
translate P CurrentState {Status}
translate P AfterCompaction {Po uporz±dkowaniu}
translate P CompactNames {Uporz±dkuj nazwy}
translate P CompactGames {Uporz±dkuj partie}
# ====== TODO To be translated ======
translate P NoUnusedNames "There are no unused names, so the name file is already fully compacted."
# ====== TODO To be translated ======
translate P NoUnusedGames "The game file is already fully compacted."
# ====== TODO To be translated ======
translate P GameFileCompacted {The game file for the database was compacted.}

# Sorting:
translate P SortCriteria {Kryteria sortowania}
translate P AddCriteria {Dodaj kryteria}
translate P CommonSorts {Standardowe kryteria}
translate P Sort {Sortuj}

# Exporting:
translate P AddToExistingFile {Dodaæ partie do pliku?}
translate P ExportComments {Eksportowaæ komentarze?}
translate P ExportVariations {Eksportowaæ warianty?}
translate P IndentComments {Wcinaj komentarze?}
translate P IndentVariations {Wcinaj warianty?}
translate P ExportColumnStyle {Kolumny (jedno posuniêcie w wierszu)?}
translate P ExportSymbolStyle {Styl znaków komentarza:}
translate P ExportStripMarks {Usuwaæ z komentarzy kody kolorowania pól/strza³ek?} 

# Goto game/move dialogs:
translate P LoadGameNumber {Podaj numer partii do wczytania:}
translate P GotoMoveNumber {Id¼ do posuniêcia nr:}

# Copy games dialog:
translate P CopyGames {Kopiuj partie}
translate P CopyConfirm {
 Czy na pewno skopiowaæ
 [::utils::thousands $nGamesToCopy] partii z filtra
 w bazie "$fromName"
 do bazy "$targetName"?
}
translate P CopyErr {Nie mo¿na skopiowaæ partii}
translate P CopyErrSource {baza ¼ród³owa}
translate P CopyErrTarget {baza docelowa}
translate P CopyErrNoGames {nie ma partii w filtrze}
translate P CopyErrReadOnly {jest tylko do odczytu}
translate P CopyErrNotOpen {nie jest otwarta}

# Colors:
translate P LightSquares {Jasne pola}
translate P DarkSquares {Ciemne pola}
translate P SelectedSquares {Wybrane pola}
translate P SuggestedSquares {Wybrane posuniêcie}
translate P WhitePieces {Bia³e figury}
translate P BlackPieces {Czarne figury}
translate P WhiteBorder {Kontur bia³ych figur}
translate P BlackBorder {Kontur czarnych figur}

# Novelty window:
translate P FindNovelty {Znajd¼ nowinkê}
translate P Novelty {Nowinka}
translate P NoveltyInterrupt {Poszukiwanie nowinki przerwano}
translate P NoveltyNone {Nie znaleziono nowinki w partii}
translate P NoveltyHelp {
Scid znajdzie pierwsze posuniêcie w partii, po którym powstanie pozycja niewystêpuj±ca ani w bazie, ani w ksi±¿ce debiutowej.
}

# Sounds configuration:
translate P SoundsFolder {Sound Files Folder} ;# ***
translate P SoundsFolderHelp {The folder should contain the files King.wav, a.wav, 1.wav, etc} ;# ***
translate P SoundsAnnounceOptions {Move Announcement Options} ;# ***
translate P SoundsAnnounceNew {Announce new moves as they are made} ;# ***
translate P SoundsAnnounceForward {Announce moves when moving forward one move} ;# ***
translate P SoundsAnnounceBack {Announce when retracting or moving back one move} ;# ***
# ====== TODO To be translated ======
translate P SoundsSoundDisabled {Scid could not find the Snack audio package at startup;\nSound is disabled.}

# Upgrading databases:
translate P Upgrading {Konwersja}
translate P ConfirmOpenNew {
Ta baza jest zapisana w starym formacie (Scid 3) i nie mo¿e zostaæ otwarta w nowszej wersji
Scid-a. Baza zosta³a ju¿ automatycznie przekonwertowana do nowego formatu.

Czy otworzyæ now± wersjê bazy?
}
translate P ConfirmUpgrade {
Ta baza jest zapisana w starym formacie (Scid 3) i nie mo¿e zostaæ otwarta w nowszej wersji Scid-a. ¯eby móc otworzyæ bazê, trzeba przekonwertowaæ j± do nowego formatu.

Konwersja utworzy now± wersjê bazy - stara wersja nie zostanie zmieniona ani usuniêta.

Mo¿e to zaj±æ trochê czasu, ale jest to operacja jednorazowa. Mo¿esz j± przerwaæ, je¶li potrwa za d³ugo.

Przekonwertowaæ bazê?
}

# Recent files options:
translate P RecentFilesMenu {Liczba ostatnich plików w menu Plik} 
translate P RecentFilesExtra {Liczba ostatnich plików w dodatkowym podmenu} 

# My Player Names options:
translate P MyPlayerNamesDescription {
Podaj listê preferowanych nazwisk graczy, po jednym w wierszu. W nazwiskach mo¿na stosowaæ znaki specjalne (np. "?" - dowolny znak, "*" - dowolna sekwencja znaków).

Wszystkie partie grane przez jednego z graczy z listy bêd± wy¶wietlane z jego perspektywy.
}
# ====== TODO To be translated ======
translate P showblunderexists {show blunder exists}
# ====== TODO To be translated ======
translate P showblundervalue {show blunder value}
# ====== TODO To be translated ======
translate P showscore {show score}
# ====== TODO To be translated ======
translate P coachgame {coach game}
# ====== TODO To be translated ======
translate P configurecoachgame {configure coach game}
# ====== TODO To be translated ======
translate P configuregame {Game configuration}
# ====== TODO To be translated ======
translate P Phalanxengine {Phalanx engine}
# ====== TODO To be translated ======
translate P Coachengine {Coach engine}
# ====== TODO To be translated ======
translate P difficulty {difficulty}
# ====== TODO To be translated ======
translate P hard {hard}
# ====== TODO To be translated ======
translate P easy {easy}
# ====== TODO To be translated ======
translate P Playwith {Play with}
# ====== TODO To be translated ======
translate P white {white}
# ====== TODO To be translated ======
translate P black {black}
# ====== TODO To be translated ======
translate P both {both}
# ====== TODO To be translated ======
translate P Play {Play}
# ====== TODO To be translated ======
translate P Noblunder {No blunder}
# ====== TODO To be translated ======
translate P blunder {blunder}
# ====== TODO To be translated ======
translate P Noinfo {-- No info --}
# ====== TODO To be translated ======
translate P PhalanxOrTogaMissing {Phalanx or Toga not found}
# ====== TODO To be translated ======
translate P moveblunderthreshold {move is a blunder if loss is greater than}
# ====== TODO To be translated ======
translate P limitanalysis {limit engine analysis time}
# ====== TODO To be translated ======
translate P seconds {seconds}
# ====== TODO To be translated ======
translate P Abort {Abort}
# ====== TODO To be translated ======
translate P Resume {Resume}
# ====== TODO To be translated ======
translate P OutOfOpening {Out of opening}
# ====== TODO To be translated ======
translate P NotFollowedLine {You did not follow the line}
# ====== TODO To be translated ======
translate P DoYouWantContinue {Do you want yo continue ?}
# ====== TODO To be translated ======
translate P CoachIsWatching {Coach is watching}
# ====== TODO To be translated ======
translate P Ponder {Permanent thinking}
# ====== TODO To be translated ======
translate P LimitELO {Limit ELO strength}
# ====== TODO To be translated ======
translate P DubiousMovePlayedTakeBack {Dubious move played, do you want to take back ?}
# ====== TODO To be translated ======
translate P WeakMovePlayedTakeBack {Weak move played, do you want to take back ?}
# ====== TODO To be translated ======
translate P BadMovePlayedTakeBack {Bad move played, do you want to take back ?}
# ====== TODO To be translated ======
translate P Iresign {I resign}
# ====== TODO To be translated ======
translate P yourmoveisnotgood {your move is not good}
# ====== TODO To be translated ======
translate P EndOfVar {End of variation}
# ====== TODO To be translated ======
translate P Openingtrainer {Opening trainer}
# ====== TODO To be translated ======
translate P DisplayCM {Display candidate moves}
# ====== TODO To be translated ======
translate P DisplayCMValue {Display candidate moves value}
# ====== TODO To be translated ======
translate P DisplayOpeningStats {Show statistics}
# ====== TODO To be translated ======
translate P ShowReport {Show report}
# ====== TODO To be translated ======
translate P NumberOfGoodMovesPlayed {good moves played}
# ====== TODO To be translated ======
translate P NumberOfDubiousMovesPlayed {dubious moves played}
# ====== TODO To be translated ======
translate P NumberOfMovesPlayedNotInRepertoire {moves played not in repertoire}
# ====== TODO To be translated ======
translate P NumberOfTimesPositionEncountered {times position encountered}
# ====== TODO To be translated ======
translate P PlayerBestMove  {Allow only best moves}
# ====== TODO To be translated ======
translate P OpponentBestMove {Opponent plays best moves}
# ====== TODO To be translated ======
translate P OnlyFlaggedLines {Only flagged lines}
# ====== TODO To be translated ======
translate P resetStats {Reset statistics}
# ====== TODO To be translated ======
translate P Repertoiretrainingconfiguration {Repertoire training configuration}
# ====== TODO To be translated ======
translate P Loadingrepertoire {Loading repertoire}
# ====== TODO To be translated ======
translate P Movesloaded {Moves loaded}
# ====== TODO To be translated ======
translate P Repertoirenotfound {Repertoire not found}
# ====== TODO To be translated ======
translate P Openfirstrepertoirewithtype {Open first a repertoire database with icon/type set to the right side}
# ====== TODO To be translated ======
translate P Movenotinrepertoire {Move not in repertoire}
# ====== TODO To be translated ======
translate P PositionsInRepertoire {Positions in repertoire}
# ====== TODO To be translated ======
translate P PositionsNotPlayed {Positions not played}
# ====== TODO To be translated ======
translate P PositionsPlayed {Positions played}
# ====== TODO To be translated ======
translate P Success {Success}
# ====== TODO To be translated ======
translate P DubiousMoves {Dubious moves}
# ====== TODO To be translated ======
translate P OutOfRepertoire {OutOfRepertoire}
# ====== TODO To be translated ======
translate P ConfigureTactics {Configure tactics}
# ====== TODO To be translated ======
translate P ResetScores {Reset scores}
# ====== TODO To be translated ======
translate P LoadingBase {Loading base}
# ====== TODO To be translated ======
translate P Tactics {Tactics}
# ====== TODO To be translated ======
translate P ShowSolution {Show solution}
# ====== TODO To be translated ======
translate P NextExercise {Next exercise}
# ====== TODO To be translated ======
translate P PrevExercise {previous exercise}
# ====== TODO To be translated ======
translate P StopTraining {Stop training}
# ====== TODO To be translated ======
translate P Next {Next}
# ====== TODO To be translated ======
translate P ResettingScore {Resetting score}
# ====== TODO To be translated ======
translate P LoadingGame {Loading game}
# ====== TODO To be translated ======
translate P MateFound {Mate found}
# ====== TODO To be translated ======
translate P BestSolutionNotFound {Best solution NOT found !}
# ====== TODO To be translated ======
translate P MateNotFound {Mate not found}
# ====== TODO To be translated ======
translate P ShorterMateExists {Shorter mate exists}
# ====== TODO To be translated ======
translate P ScorePlayed {Score played}
# ====== TODO To be translated ======
translate P Expected {expected}
# ====== TODO To be translated ======
translate P ChooseTrainingBase {Choose training base}
# ====== TODO To be translated ======
translate P Thinking {Thinking}
# ====== TODO To be translated ======
translate P AnalyzeDone {Analyze done}
# ====== TODO To be translated ======
translate P WinWonGame {Win won game}
# ====== TODO To be translated ======
translate P Lines {Lines}
# ====== TODO To be translated ======
translate P ConfigureUCIengine {Configure UCI engine}
# ====== TODO To be translated ======
translate P SpecificOpening {Specific opening}
# ====== TODO To be translated ======
translate P StartNewGame {Start new game}
# ====== TODO To be translated ======
translate P FixedLevel {Fixed level}
# ====== TODO To be translated ======
translate P Opening {Opening}
# ====== TODO To be translated ======
translate P RandomLevel {Random level}
# ====== TODO To be translated ======
translate P StartFromCurrentPosition {Start from current position}
# ====== TODO To be translated ======
translate P FixedDepth {Fixed depth}
# ====== TODO To be translated ======
translate P Nodes {Nodes} 
# ====== TODO To be translated ======
translate P Depth {Depth}
# ====== TODO To be translated ======
translate P Time {Time} 
# ====== TODO To be translated ======
translate P SecondsPerMove {Seconds per move}
# ====== TODO To be translated ======
translate P Engine {Engine}
# ====== TODO To be translated ======
translate P TimeMode {Time mode}
# ====== TODO To be translated ======
translate P TimeBonus {Time + bonus}
# ====== TODO To be translated ======
translate P TimeMin {min}
# ====== TODO To be translated ======
translate P TimeSec {sec}
# ====== TODO To be translated ======
translate P AllExercisesDone {All exercises done}
# ====== TODO To be translated ======
translate P MoveOutOfBook {Move out of book}
# ====== TODO To be translated ======
translate P LastBookMove {Last book move}
# ====== TODO To be translated ======
translate P AnnotateSeveralGames {Annotate several games\nfrom current to :}
# ====== TODO To be translated ======
translate P FindOpeningErrors {Find opening errors}
# ====== TODO To be translated ======
translate P MarkTacticalExercises {Mark tactical exercises}
# ====== TODO To be translated ======
translate P UseBook {Use book}
# ====== TODO To be translated ======
translate P MultiPV {Multiple variations}
# ====== TODO To be translated ======
translate P Hash {Hash memory}
# ====== TODO To be translated ======
translate P OwnBook {Use engine book}
# ====== TODO To be translated ======
translate P BookFile {Opening book}
# ====== TODO To be translated ======
translate P AnnotateVariations {Annotate variations}
# ====== TODO To be translated ======
translate P ShortAnnotations {Short annotations}
# ====== TODO To be translated ======
translate P addAnnotatorTag {Add annotator tag}
# ====== TODO To be translated ======
translate P AddScoreToShortAnnotations {Add score to short annotations}
# ====== TODO To be translated ======
translate P Export {Export}
# ====== TODO To be translated ======
translate P BookPartiallyLoaded {Book partially loaded}
# ====== TODO To be translated ======
translate P Calvar {Calculation of variations}
# ====== TODO To be translated ======
translate P ConfigureCalvar {Configuration}
# ====== TODO To be translated ======
translate P Reti {Reti}
# ====== TODO To be translated ======
translate P English {English}
# ====== TODO To be translated ======
translate P d4Nf6Miscellaneous {1.d4 Nf6 Miscellaneous}
# ====== TODO To be translated ======
translate P Trompowsky {Trompowsky}
# ====== TODO To be translated ======
translate P Budapest {Budapest}
# ====== TODO To be translated ======
translate P OldIndian {Old Indian}
# ====== TODO To be translated ======
translate P BenkoGambit {Benko Gambit}
# ====== TODO To be translated ======
translate P ModernBenoni {Modern Benoni}
# ====== TODO To be translated ======
translate P DutchDefence {Dutch Defence}
# ====== TODO To be translated ======
translate P Scandinavian {Scandinavian}
# ====== TODO To be translated ======
translate P AlekhineDefence {Alekhine Defence}
# ====== TODO To be translated ======
translate P Pirc {Pirc}
# ====== TODO To be translated ======
translate P CaroKann {Caro-Kann}
# ====== TODO To be translated ======
translate P CaroKannAdvance {Caro-Kann Advance}
# ====== TODO To be translated ======
translate P Sicilian {Sicilian}
# ====== TODO To be translated ======
translate P SicilianAlapin {Sicilian Alapin}
# ====== TODO To be translated ======
translate P SicilianClosed {Sicilian Closed}
# ====== TODO To be translated ======
translate P SicilianRauzer {Sicilian Rauzer}
# ====== TODO To be translated ======
translate P SicilianDragon {Sicilian Dragon}
# ====== TODO To be translated ======
translate P SicilianScheveningen {Sicilian Scheveningen}
# ====== TODO To be translated ======
translate P SicilianNajdorf {Sicilian Najdorf}
# ====== TODO To be translated ======
translate P OpenGame {Open Game}
# ====== TODO To be translated ======
translate P Vienna {Vienna}
# ====== TODO To be translated ======
translate P KingsGambit {King's Gambit}
# ====== TODO To be translated ======
translate P RussianGame {Russian Game}
# ====== TODO To be translated ======
translate P ItalianTwoKnights {Italian/Two Knights}
# ====== TODO To be translated ======
translate P Spanish {Spanish}
# ====== TODO To be translated ======
translate P SpanishExchange {Spanish Exchange}
# ====== TODO To be translated ======
translate P SpanishOpen {Spanish Open}
# ====== TODO To be translated ======
translate P SpanishClosed {Spanish Closed}
# ====== TODO To be translated ======
translate P FrenchDefence {French Defence}
# ====== TODO To be translated ======
translate P FrenchAdvance {French Advance}
# ====== TODO To be translated ======
translate P FrenchTarrasch {French Tarrasch}
# ====== TODO To be translated ======
translate P FrenchWinawer {French Winawer}
# ====== TODO To be translated ======
translate P FrenchExchange {French Exchange}
# ====== TODO To be translated ======
translate P QueensPawn {Queen's Pawn}
# ====== TODO To be translated ======
translate P Slav {Slav}
# ====== TODO To be translated ======
translate P QGA {QGA}
# ====== TODO To be translated ======
translate P QGD {QGD}
# ====== TODO To be translated ======
translate P QGDExchange {QGD Exchange}
# ====== TODO To be translated ======
translate P SemiSlav {Semi-Slav}
# ====== TODO To be translated ======
translate P QGDwithBg5 {QGD with Bg5}
# ====== TODO To be translated ======
translate P QGDOrthodox {QGD Orthodox}
# ====== TODO To be translated ======
translate P Grunfeld {Grünfeld}
# ====== TODO To be translated ======
translate P GrunfeldExchange {Grünfeld Exchange}
# ====== TODO To be translated ======
translate P GrunfeldRussian {Grünfeld Russian}
# ====== TODO To be translated ======
translate P Catalan {Catalan}
# ====== TODO To be translated ======
translate P CatalanOpen {Catalan Open}
# ====== TODO To be translated ======
translate P CatalanClosed {Catalan Closed}
# ====== TODO To be translated ======
translate P QueensIndian {Queen's Indian}
# ====== TODO To be translated ======
translate P NimzoIndian {Nimzo-Indian}
# ====== TODO To be translated ======
translate P NimzoIndianClassical {Nimzo-Indian Classical}
# ====== TODO To be translated ======
translate P NimzoIndianRubinstein {Nimzo-Indian Rubinstein}
# ====== TODO To be translated ======
translate P KingsIndian {King's Indian}
# ====== TODO To be translated ======
translate P KingsIndianSamisch {King's Indian Sämisch}
# ====== TODO To be translated ======
translate P KingsIndianMainLine {King's Indian Main Line}
# ====== TODO To be translated ======
translate P ConfigureFics {Configure FICS}
# ====== TODO To be translated ======
translate P FICSGuest {Login as Guest}
# ====== TODO To be translated ======
translate P FICSServerPort {Server port}
# ====== TODO To be translated ======
translate P FICSServerAddress {IP Address}
# ====== TODO To be translated ======
translate P FICSRefresh {Refresh}
# ====== TODO To be translated ======
translate P FICSTimesealPort {Timeseal port}
# ====== TODO To be translated ======
translate P FICSSilence {Silence}
# ====== TODO To be translated ======
translate P FICSOffers {Offers}
# ====== TODO To be translated ======
translate P FICSConsole {Console}
# ====== TODO To be translated ======
translate P FICSGames {Games}
# ====== TODO To be translated ======
translate P FICSUnobserve {Stop observing game}
# ====== TODO To be translated ======
translate P FICSProfile {Display your history and profile}
# ====== TODO To be translated ======
translate P FICSRelayedGames {Relayed games}
# ====== TODO To be translated ======
translate P FICSFindOpponent {Find opponent}
# ====== TODO To be translated ======
translate P FICSTakeback {Takeback}
# ====== TODO To be translated ======
translate P FICSTakeback2 {Takeback 2}
# ====== TODO To be translated ======
translate P FICSInitTime {Initial time (min)}
# ====== TODO To be translated ======
translate P FICSIncrement {Increment (sec)}
# ====== TODO To be translated ======
translate P FICSRatedGame {Rated Game}
# ====== TODO To be translated ======
translate P FICSAutoColour {automatic}
# ====== TODO To be translated ======
translate P FICSManualConfirm {confirm manually}
# ====== TODO To be translated ======
translate P FICSFilterFormula {Filter with formula}
# ====== TODO To be translated ======
translate P FICSIssueSeek {Issue seek}
# ====== TODO To be translated ======
translate P FICSChallenge {Challenge}
# ====== TODO To be translated ======
translate P FICSAccept {accept}
# ====== TODO To be translated ======
translate P FICSDecline {decline}
# ====== TODO To be translated ======
translate P FICSColour {Colour}
# ====== TODO To be translated ======
translate P FICSSend {send}
# ====== TODO To be translated ======
translate P FICSConnect {Connect}
# ====== TODO To be translated ======
translate P FICSdefaultuservars {Use default variables}
# ====== TODO To be translated ======
translate P FICSObserveconfirm {Do you want to observe game}
# ====== TODO To be translated ======
translate P FICSpremove {Enable premove}
# ====== TODO To be translated ======
translate P GameReview {Game review}
# ====== TODO To be translated ======
translate P GameReviewTimeExtended {Time extended}
# ====== TODO To be translated ======
translate P GameReviewMargin {Error margin}
# ====== TODO To be translated ======
translate P GameReviewAutoContinue {Auto continue when move is correct}
# ====== TODO To be translated ======
translate P GameReviewReCalculate {Use extended time}
# ====== TODO To be translated ======
translate P GameReviewAnalyzingMovePlayedDuringTheGame {Analyzing move played during the game}
# ====== TODO To be translated ======
translate P GameReviewAnalyzingThePosition {Analyzing the position}
# ====== TODO To be translated ======
translate P GameReviewEnterYourMove {Enter your move}
# ====== TODO To be translated ======
translate P GameReviewCheckingYourMove {Checking your move}
# ====== TODO To be translated ======
translate P GameReviewYourMoveWasAnalyzed {Your move was analyzed}
# ====== TODO To be translated ======
translate P GameReviewYouPlayedSameMove {You played the same move as in match}
# ====== TODO To be translated ======
translate P GameReviewScoreOfYourMove {Score of your move}
# ====== TODO To be translated ======
translate P GameReviewGameMoveScore {Game move's score}
# ====== TODO To be translated ======
translate P GameReviewEngineScore {Engine's score}
# ====== TODO To be translated ======
translate P GameReviewYouPlayedLikeTheEngine {You played like the engine}
# ====== TODO To be translated ======
translate P GameReviewNotEngineMoveButGoodMove {Not the engine move, but is also a good move}
# ====== TODO To be translated ======
translate P GameReviewMoveNotGood {This move is not good, score is}
# ====== TODO To be translated ======
translate P GameReviewMovesPlayedLike {Moves played like}
# ====== TODO To be translated ======
translate P GameReviewMovesPlayedEngine {Moves played like engine}
# ====== TODO To be translated ======
translate P CCDlgConfigureWindowTitle {Configure Correspondence Chess}
# ====== TODO To be translated ======
translate P CCDlgCGeneraloptions {General Options}
# ====== TODO To be translated ======
translate P CCDlgDefaultDB {Default Database:}
# ====== TODO To be translated ======
translate P CCDlgInbox {Inbox (path):}
# ====== TODO To be translated ======
translate P CCDlgOutbox {Outbox (path):}
# ====== TODO To be translated ======
translate P CCDlgXfcc {Xfcc Configuration:}
# ====== TODO To be translated ======
translate P CCDlgExternalProtocol {External Protocol Handler (e.g. Xfcc)}
# ====== TODO To be translated ======
translate P CCDlgFetchTool {Fetch Tool:}
# ====== TODO To be translated ======
translate P CCDlgSendTool {Send Tool:}
# ====== TODO To be translated ======
translate P CCDlgEmailCommunication {eMail Communication}
# ====== TODO To be translated ======
translate P CCDlgMailPrg {Mail program:}
# ====== TODO To be translated ======
translate P CCDlgBCCAddr {(B)CC Address:}
# ====== TODO To be translated ======
translate P CCDlgMailerMode {Mode:}
# ====== TODO To be translated ======
translate P CCDlgThunderbirdEg {e.g. Thunderbird, Mozilla Mail, Icedove...}
# ====== TODO To be translated ======
translate P CCDlgMailUrlEg {e.g. Evolution}
# ====== TODO To be translated ======
translate P CCDlgClawsEg {e.g Sylpheed Claws}
# ====== TODO To be translated ======
translate P CCDlgmailxEg {e.g. mailx, mutt, nail...}
# ====== TODO To be translated ======
translate P CCDlgAttachementPar {Attachment parameter:}
# ====== TODO To be translated ======
translate P CCDlgInternalXfcc {Use internal Xfcc support}
# ====== TODO To be translated ======
translate P CCDlgConfirmXfcc {Confirm moves}
# ====== TODO To be translated ======
translate P CCDlgSubjectPar {Subject parameter:}
# ====== TODO To be translated ======
translate P CCDlgDeleteBoxes {Empty In-/Outbox}
# ====== TODO To be translated ======
translate P CCDlgDeleteBoxesText {Do you really want to empty your In- and Outbox folders for Correspondence Chess? This requires a new sync to show the last state of your games}
# ====== TODO To be translated ======
translate P CCDlgConfirmMove {Confirm move}
# ====== TODO To be translated ======
translate P CCDlgConfirmMoveText {If you confirm, the following move and comment will be sent to the server:}
# ====== TODO To be translated ======
translate P CCDlgDBGameToLong {Inconsistent Mainline}
# ====== TODO To be translated ======
translate P CCDlgDBGameToLongError {The mainline in your database is longer than the game in your Inbox. If the Inbox contains current games, i.e. right after a sync, some moves were added to the mainline in the database erroneously.\nIn this case please shorten the mainline to (at max) move\n}
# ====== TODO To be translated ======
translate P CCDlgStartEmail {Start new eMail game}
# ====== TODO To be translated ======
translate P CCDlgYourName {Your Name:}
# ====== TODO To be translated ======
translate P CCDlgYourMail {Your eMail Address:}
# ====== TODO To be translated ======
translate P CCDlgOpponentName {Opponents Name:}
# ====== TODO To be translated ======
translate P CCDlgOpponentMail {Opponents eMail Address:}
# ====== TODO To be translated ======
translate P CCDlgGameID {Game ID (unique):}
# ====== TODO To be translated ======
translate P CCDlgTitNoOutbox {Scid: Correspondence Chess Outbox}
# ====== TODO To be translated ======
translate P CCDlgTitNoInbox {Scid: Correspondence Chess Inbox}
# ====== TODO To be translated ======
translate P CCDlgTitNoGames {Scid: No Correspondence Chess Games}
# ====== TODO To be translated ======
translate P CCErrInboxDir {Correspondence Chess inbox directory:}
# ====== TODO To be translated ======
translate P CCErrOutboxDir {Correspondence Chess outbox directory:}
# ====== TODO To be translated ======
translate P CCErrDirNotUsable {does not exist or is not accessible!\nPlease check and correct the settings.}
# ====== TODO To be translated ======
translate P CCErrNoGames {does not contain any games!\nPlease fetch them first.}
# ====== TODO To be translated ======
translate P CCDlgTitNoCCDB {Scid: No Correspondence Database}
# ====== TODO To be translated ======
translate P CCErrNoCCDB {No Database of type 'Correspondence' is opened. Please open one before using correspondence chess functions.}
# ====== TODO To be translated ======
translate P CCFetchBtn {Fetch games from the server and process the Inbox}
# ====== TODO To be translated ======
translate P CCPrevBtn {Goto previous game}
# ====== TODO To be translated ======
translate P CCNextBtn {Goto next game}
# ====== TODO To be translated ======
translate P CCSendBtn {Send move}
# ====== TODO To be translated ======
translate P CCEmptyBtn {Empty In- and Outbox}
# ====== TODO To be translated ======
translate P CCHelpBtn {Help on icons and status indicators.\nFor general Help press F1!}
# ====== TODO To be translated ======
translate P CCDlgServerName {Server Name:}
# ====== TODO To be translated ======
translate P CCDlgLoginName  {Login Name:}
# ====== TODO To be translated ======
translate P CCDlgPassword   {Password:}
# ====== TODO To be translated ======
translate P CCDlgShowPassword {Show password}
# ====== TODO To be translated ======
translate P CCDlgURL        {Xfcc-URL:}
# ====== TODO To be translated ======
translate P CCDlgRatingType {Rating Type:}
# ====== TODO To be translated ======
translate P CCDlgDuplicateGame {Non-unique game ID}
# ====== TODO To be translated ======
translate P CCDlgDuplicateGameError {This game exists more than once in your database. Please delete all duplicates and compact your game file (File/Maintenance/Compact Database).}
# ====== TODO To be translated ======
translate P CCDlgSortOption {Sorting:}
# ====== TODO To be translated ======
translate P CCDlgListOnlyOwnMove {Only games I have the move}
# ====== TODO To be translated ======
translate P CCOrderClassicTxt {Site, Event, Round, Result, White, Black}
# ====== TODO To be translated ======
translate P CCOrderMyTimeTxt {My Clock}
# ====== TODO To be translated ======
translate P CCOrderTimePerMoveTxt {Time per move till next time control}
# ====== TODO To be translated ======
translate P CCOrderStartDate {Start date}
# ====== TODO To be translated ======
translate P CCOrderOppTimeTxt {Opponents Clock}
# ====== TODO To be translated ======
translate P CCDlgConfigRelay {Configure ICCF observations}
# ====== TODO To be translated ======
translate P CCDlgConfigRelayHelp {Go to the games page on http://www.iccf-webchess.com and display the game to be observed.  If you see the chessboard copy the URL from your browser to the list below. One URL per line only!\nExample: http://www.iccf-webchess.com/MakeAMove.aspx?id=266452}
# ====== TODO To be translated ======
translate P ExtHWConfigConnection {Configure external hardware}
# ====== TODO To be translated ======
translate P ExtHWPort {Port}
# ====== TODO To be translated ======
translate P ExtHWEngineCmd {Engine command}
# ====== TODO To be translated ======
translate P ExtHWEngineParam {Engine parameter}
# ====== TODO To be translated ======
translate P ExtHWShowButton {Show button}
# ====== TODO To be translated ======
translate P ExtHWHardware {Hardware}
# ====== TODO To be translated ======
translate P ExtHWNovag {Novag Citrine}
# ====== TODO To be translated ======
translate P ExtHWInputEngine {Input Engine}
# ====== TODO To be translated ======
translate P ExtHWNoBoard {No board}
# ====== TODO To be translated ======
translate P NovagReferee {Referee}
# ====== TODO To be translated ======
translate P IEConsole {Input Engine Console}
# ====== TODO To be translated ======
translate P IESending {Moves sent for}
# ====== TODO To be translated ======
translate P IESynchronise {Synchronise}
# ====== TODO To be translated ======
translate P IERotate  {Rotate}
# ====== TODO To be translated ======
translate P IEUnableToStart {Unable to start Input Engine:}
# ====== TODO To be translated ======
translate P DoneWithPosition {Done with position}
# ====== TODO To be translated ======
translate P Board {Board}
# ====== TODO To be translated ======
translate P showGameInfo {Show game info}
# ====== TODO To be translated ======
translate P autoResizeBoard {Automatic resize of board}
# ====== TODO To be translated ======
translate P DockTop {Move to top}
# ====== TODO To be translated ======
translate P DockBottom {Move to bottom}
# ====== TODO To be translated ======
translate P DockLeft {Move to left}
# ====== TODO To be translated ======
translate P DockRight {Move to right}
# ====== TODO To be translated ======
translate P Undock {Undock}
# ====== TODO To be translated ======
translate P ChangeIcon {Change icon...}
# ====== TODO To be translated ======
translate P NewGameListWindow {New Game List Window}
# ====== TODO To be translated ======
translate P LoadatStartup {Load at startup}
# ====== TODO To be translated ======
translate P ShowHideDB {Show/Hide databases}
# ====== TODO To be translated ======
translate P ChangeFilter {Change filter}
# ====== TODO To be translated ======
translate P ChangeLayout {Load/Save/Change sorting criteria and column layout}
# ====== TODO To be translated ======
translate P ShowHideStatistic {Show/Hide statistics}
# ====== TODO To be translated ======
translate P BoardFilter {Show only games that matches the current board position}
# ====== TODO To be translated ======
translate P CopyGameTo {Copy Game to}
# ====== TODO To be translated ======
translate P FindBar {Find Bar}
# ====== TODO To be translated ======
translate P FindCurrentGame {Find current game}
# ====== TODO To be translated ======
translate P DeleteGame {Delete game}
# ====== TODO To be translated ======
translate P UndeleteGame {Undelete game}
# ====== TODO To be translated ======
translate P ResetSort {Reset sort}
# ====== TODO To be translated ======
translate P ConvertNullMove {Convert null moves to comments}
# ====== TODO To be translated ======
translate P SetupBoard {Setup Board}
# ====== TODO To be translated ======
translate P Rotate {Rotate}
# ====== TODO To be translated ======
translate P SwitchColors {Switch colors}
# ====== TODO To be translated ======
translate P FlipBoard {Flip Board}
# ====== TODO To be translated ======
translate P ImportPGN {Import PGN game}
# ====== TODO To be translated ======
translate P ImportingFiles {Importing PGN files in}
# ====== TODO To be translated ======
translate P ImportingFrom {Importing from}
# ====== TODO To be translated ======
translate P ImportingIn {Import games in}
# ====== TODO To be translated ======
translate P UseLastTag {Use last\ngame's tags}
# ====== TODO To be translated ======
translate P Random {Random}
# ====== TODO To be translated ======
translate P BackToMainline {Go back to mainline}
# ====== TODO To be translated ======
translate P LeaveVariant {Leave variant}
# ====== TODO To be translated ======
translate P Autoplay {Autoplay}
# ====== TODO To be translated ======
translate P ShowHideCoords {Show/Hide Coord.}
# ====== TODO To be translated ======
translate P ShowHideMaterial {Show/Hide Material}
# ====== TODO To be translated ======
translate P FullScreen {Full Screen}
# ====== TODO To be translated ======
translate P FilterStatistic {Filter Statistics}
# ====== TODO To be translated ======
translate P MakeCorrections {Make Corrections}
# ====== TODO To be translated ======
translate P Surnames {Surnames}
# ====== TODO To be translated ======
translate P Ambiguous {Ambiguous}
translate P OptionsToolbar "Pasek narzêdziowy"
translate P OptionsBoard "Szachownica"
# ====== TODO To be translated ======
translate P OptionsMenuColor "Menu colors"
translate P OptionsBoardSize "Wielko¶æe"
translate P OptionsBoardPieces "Typ figur"
# ====== TODO To be translated ======
translate P OptionsInternationalization "Internationalization"
# ====== TODO To be translated ======
translate P MenuColorForeground "Textcolor"
# ====== TODO To be translated ======
translate P MenuColorBackground "Background"
# ====== TODO To be translated ======
translate P MenuColorSelect "Select"
# ====== TODO To be translated ======
translate P OptionsTablebaseDir "Select up to 4 table base folders:"
}

### Tips of the day in Polish:

set tips(P) {
  {
    Scid ma ponad 30 <a Index>stron pomocy</a> i w wiêkszo¶ci okien Scida
    naci¶niêcie klawisza <b>F1</b> spowoduje wy¶wietlenie odpowiedniej
    strony.
  }
  {
    Niektóre okna Scida (np. informacje pod szachownic±,
    <a Switcher>prze³±cznik baz</a>) maj± menu przywo³ywane prawym przyciskiem
    myszy. Spróbuj nacisn±æ prawy przycisk myszy w ka¿dym oknie, by
    sprawdziæ, czy menu jest dostêpne i jakie funkcje zawiera.
  }
  {
    Scid pozwala wprowadzaæ posuniêcia na kilka ró¿nych sposobów.
    Mo¿esz u¿yæ myszy (z wy¶wietlaniem mo¿liwych posuniêæ lub bez)
    albo klawiatury (z opcjonalnym automatycznym dope³nianiem).
    Wiêcej informacji mo¿na znale¼æ na stronie pomocy
    <a Moves>Wprowadzenie posuniêæ</a>.
  }
  {
    Je¶li masz kilka baz, które otwierasz czêsto, dodaj
    <a Bookmarks>zak³adkê</a> dla ka¿dej z nich. Umo¿liwi to ³atwe
    otwieranie baz z menu.
  }
  {
    Mo¿esz obejrzeæ wszystkie posuniêcia w aktualnej partii
    (z wariantami i komentarzami lub bez) w <a PGN>Oknie PGN</a>.
    W oknie PGN mo¿esz przej¶æ do dowolnego posuniêcia, klikaj±c
    na nim lewym przyciskiem myszy oraz u¿yæ ¶rodkowego lub prawego
    przycisku myszy do obejrzenia aktualnej pozycji.
  }
  {
    Mo¿esz kopiowaæ partie z bazy do bazy przeci±gaj±c je lewym
    przyciskiem myszy w oknie <a Switcher>Prze³±cznika baz</a>.
  }
  {
    Scid mo¿e otwieraæ pliki PGN, nawet je¶li s± one skompresowane
    Gzip-em (z rozszerzeniem .gz). Pliki PGN mog± byæ jedynie
    czytane, wiêc je¶li chcesz co¶ zmieniæ, utwórz now± bazê Scida
    i skopiuj do niej partie z pliku PGN.
  }
  {
    Je¶li masz du¿± bazê i czêsto u¿ywasz okna <a Tree>Drzewa wariantów</a>,
    warto wybraæ polecenie <b>Twórz standardowy plik cache/b>
    z menu Plik okna Drzewo wariantów. Statystyki dla najpopularniejszych
    pozycji debiutowych zostan± zapamiêtane w pliku, co przyspieszy
    dzia³anie drzewa.
  }
  {
    <a Tree>Drzewo wariantów</a> mo¿e pokazaæ wszystkie posuniêcia
    z aktualnej pozycji, ale je¶li chcesz zobaczyæ wszystkie kolejno¶ci
    posuniêæ prowadz±ce do aktualnej pozycji, mo¿esz u¿yæ
    <a OpReport>Raportu debiutowego</a>.
  }
  {
    W <a GameList>li¶cie partii</a> kliknij lewym lub prawym przyciskiem
    myszy na nag³ówku wybranej kolumny, by zmieniæ jej szeroko¶æ.
  }
  {
    W oknie <a PInfo>Informacja o graczu</a> (kliknij na nazwisku gracza
    w polu pod szachownic±, by je otworzyæ) mo¿esz ³atwo ustawiæ
    <a Searches Filter>filtr</a> zawieraj±cy wszystkie partie danego
    gracza zakoñczeone wybranym wynikiem, klikaj±c na dowolnej warto¶ci
    wy¶wietlanej na <red>czerowono</red>.
  }
  {
    Podczas pracy nad debiutem warto u¿yæ funkcji
    <a Searches Board>wyszukiwania pozycji</a> z opcj± <b>Pionki</b> lub
    <b>Kolumny</b>. Pozowli to znale¼æ inne warianty debiutowe z t±
    sam± struktur± pionow±.
  }
  {
    W polu informacji o partii (pod szachownic±) mo¿na u¿yæ prawego
    przycisku myszy, by wy¶wietliæ menu konfiguracji pola. Mo¿na
    np. ukryæ nastêpne posuniêcie, co jest przydatne przy rozwi±zywaniu
    zadañ.
  }
  {
    Je¶li czêsto u¿ywasz funkcji <a Maintenance>obs³ugi</a> na du¿ej
    bazie, mo¿esz u¿yæ okna <a Maintenance Cleaner>Zestaw zadañ</a>
    do wykonania kilka funkcji naraz.
  }
  {
    Je¶li masz du¿± bazê, w której wiêkszo¶æ partii ma ustawiony
    znacznik EventDate, mo¿esz <a Sorting>posortowaæ</a> j±
    wg tego znacznika (zamiast Daty). Dziêki temu wszystkie partie
    z jednego turnieju znajd± siê ko³o siebie.
  }
  {
    Przed u¿yciem funkcji <a Maintenance Twins>usuwania podwójnych partii</a>
    dobrze jest <a Maintenance Spellcheck>sprawdziæ pisowniê</a>
    nazwisk w bazie, co usprawni wyszukiwanie powtórzeñ.
  }
  {
    <a Flags>Flagi</a> s± przydatne do oznaczania partii, które
    zawieraj± wa¿ne motywy taktyczne, strkutury pionowe, nowinki itd.
    Potem mo¿esz znale¼æ takie partie
    <a Searches Header>wyszukiwaniem wg nag³ówka</a>.
  }
  {
    Je¶li przegl±dasz partiê i chcesz sprawdziæ jaki¶ wariant nie
    zmieniaj±c partii, mo¿esz w³±czyæ tryb testowania wariantu
    (klawisz <b>Ctrl+spacja</b> lub ikona na pasku narzêdziowym).
    Po wy³±czeniu trybu testowania powrócisz do pozycji z partii.
  }
  {
    ¯eby znale¼æ najwa¿niejsze partie (z najsilniejszymi przeciwnikami),
    w których powsta³a aktualna pozycja, otwórz <a Tree>Drzewo wariantów</a>
    i wybierz listê najlepszych partii. Mo¿esz nawet wybraæ tylko
    partie zakoñczone konkretnym wynikiem.
  }
  {
    Dobr± metod± na naukê debiutu przy u¿yciu du¿ej bazy jest
    w³±czenie trybu treningu w <a Tree>Drzewie wariantów</a>
    i gra z programem. Pozwala to sprawdziæ, które posuniêcia s±
    grane najczê¶ciej.
  }
  {
    Je¶li masz otwarte dwie bazy i chcesz obejrzeæ
    <a Tree>Drzewo wariantów</a> dla pierwszej bazy, przegl±daj±c
    partiê z drugiej, kliknij przycisk <b>Blokada</b> na drzewie,
    by zablokowaæ je na pierwszej bazie, a nastêpnie prze³±cz siê
    do drugiej bazy.
  }
  {
    Okno <a Tmt>Turnieje</a> jest przydatne nie tylko do znajdowania
    turniejów, ale pozwala tak¿e sprawdziæ, w jakich turniejach gra³
    ostatnio dany zawodnik i jakie turnieje s± rozgrywane w wybranym
    kraju.
  }
  {
    Mo¿esz u¿yæ jednego z wielu typowych wzorców w oknie
    <a Searches Material>Wyszukiwania wg materia³u</a> do znalezienia
    partii do studiowania debiutów lub gry ¶rodkowej.
  }
  {
    W oknie <a Searches Material>Wyszukiwanie wg materia³u</a>, mo¿esz
    ograniczyæ liczbê znajdowanych partii przez warunek, by
    podany stosunek materia³u utrzymywa³ siê przynajmniej przez
    kilka pó³ruchów.
  }
  {
    Je¶li masz wa¿n± bazê, której nie chcesz przez przypadek zmieniæ,
    w³±cz <b>Tylko do odczytu...</b> w menu <b>Plik</b> po jej otwarciu
    (albo zmieñ prawa dostêpu do pliku).
  }
  {
    Je¶li u¿ywasz XBoard-a lub WinBoard-a (albo programu szachowego,
    który pozwala na skopiowania pozycji w notacji FEN do schowka)
    i chcesz skopiowaæ aktualn± pozycjê do Scid-a, wybierz
    <b>Copy position</b> w menu File programu XBoard/Winboard, a potem
    <b>Wklej aktywn± partiê ze schowka</b> z menu Edycja Scid-a.
  }
  {
    W oknie <a Searches Header>Wyszukiwanie wg nag³ówka</a>,
    szukane nazwy graczy/turnieju/miejsca/rundy s± znajdowane niezale¿nie
    od wielko¶ci liter i równie¿ wewn±trz nazw.
    Zamiast tego mo¿esz u¿yæ poszukiwania z symbolami wieloznacznymi
    (gdzie "?" oznacza dowolny znak, za¶ "*" - 0 lub wiêcej znaków),
    wpisuj±c szukany tekst w cudzys³owie. Wielko¶æ liter zostanie
    uwzglêdniona. Na przyk³ad "*BEL" znajdzie wszystkie turnieje grane
    w Belgii (ale nie w Belgradzie).
  }
  {
    Je¶li chcesz poprawiæ posuniêcie nie zmieniaj±c nastêpnych,
    otwórz okno <a Import>Pobierz partiê</a>, wci¶nij
    <b>Wklej aktualn± partiê</b>, zmieñ b³êdne posuniêcie i wci¶nij
    <b>Pobierz</b>.
  }
  {
    Je¶li plik klasyfikacji debiutowej ECO jest wczytany, mo¿esz przej¶æ
    do ostatniej sklasyfikowanej pozycji w partii za pomoc± polecenia
    <b>Rozpoznaj debiut</b> w menu <b>Partia</b> (klawisz Ctrl+Shift+D).
  }
  {
    Je¶li chcesz sprawdziæ wielko¶æ lub datê modyfikacji pliku
    przed jego otwarciem, u¿yj okna <a Finder>Poszukiwacza plików</a>.
  }
  {
    Plik<a Repertoire>repertuaru debiutowego</a> pozwala na ¶ledzenie
    partii w ulubionych wariantach. Kiedy dodasz grane przez siebie
    debiuty do pliku repertuaru debiutowego, bêdziesz móg³ ³atwo
    znajdowaæ je w nowych partiach.
  }
  {
    <a OpReport>Raport debiutowy</a> pozwala dowiedzieæ siê wiêcej
    o konkretnej pozycji. Mo¿esz zobaczyæ wyniki, nazwiska najczê¶ciej
    graj±cych j± zawodników, typowe motywy pozycyjne itd.
  }
  {
    Mo¿esz dodaæ wiêkszo¶æ typowych symboli (!, !?, += itd.) do
    aktualnego posuniêcia lub pozycji za pomoc± skrótów klawiszowych,
    bez potrzeby otwierania okna <a Comment>Edytora komentarzy</a>
    -- np. wci¶niêcie "!" i Enter spowoduje dodanie symbolu "!".
    Na stronie <a Moves>Wprowadzanie posuniêæ</a> mo¿na znale¼æ
    wiêcej informacji.
  }
  {
    Mo¿esz ³atwo przegl±daæ debiuty w bazie w oknie
    <a Tree>Drzewo wariantów</a>. W oknie Statystyka (klawisz Ctrl+I)
    mo¿na znale¼æ informacje o ostatnich wynikach w wariancie oraz
    o partiach granych przez silnych graczy.
  }
  {
    Mo¿esz zmieniæ wielko¶æ szachownicy, naciskaj±c <b>lewo</b> lub <b>prawo</b>
    przy wci¶niêtych klawiszach <b>Ctrl</b> i <b>Shift</b>.
  }
  {
    Po <a Searches>wyszukiwaniu</a> mo¿esz ³atwo przegl±daæ wszystkie
    znalezione partie, naciskaj±c klawisz <b>góra</b> lub <b>dó³</b>
    przy wci¶niêtym <b>Ctrl</b> by obejrzeæ poprzedni±/nastêpn± partiê
    w filtrze.
  }
}

# end of polish.tcl
