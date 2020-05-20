### italian.tcl:
# Italian language support for Scid.
# Added by Giancarlo Bassi.
# Updated by Paolo Montrasi.
# Updated by Michele Rinesi on 19/02/2004.
# Updated by Luciano Salerno on 10/05/2015.
# Untranslated messages are marked with a "***" comment.

proc setLanguage_I {} {

# File menu:
menuText I File "File" 0
menuText I FileNew "Nuovo..." 0 {Crea un nuovo database Scid}
menuText I FileOpen "Apri..." 0 {Apre un database esistente}
menuText I FileClose "Chiudi" 0 {Chiude il database attivo}
menuText I FileFinder "Trova" 0 {Apre la finestra per cercare i file}
menuText I FileBookmarks "Segnalibri" 0 {Menu Segnalibri (chiave: Ctrl+B)}
menuText I FileBookmarksAdd "Aggiungi segnalibro" 0 \
  {Memorizza la partita e la posizione del database attivo}
menuText I FileBookmarksFile "Memorizza segnalibro" 0 \
  {Memorizza un segnalibro per la partita e posizione corrente}
menuText I FileBookmarksEdit "Modifica segnalibri..." 1 \
  {Modifica il menu dei segnalibri memorizzati}
menuText I FileBookmarksList "Mostra i segnalibri come una lista" 3 \
  {Mostra le cartelle dei segnalibri come una lista, senza sottomenu}
menuText I FileBookmarksSub "Mostra i segnalibri come sottomenu" 2 \
  {Mostra le cartelle dei segnalibri con sottomenu, non a lista}
menuText I FileMaint "Gestione" 0 {Strumenti di gestione di un database Scid}
menuText I FileMaintWin "Finestra di gestione" 0 \
  {Apre/chiude la finestra di gestione di un database Scid}
menuText I FileMaintCompact "Compatta database..." 0 \
  {Compatta i file di database, rimuovendo le partite cancellate e i nomi non usati}
menuText I FileMaintClass "Classificazione ECO delle partite..." 2 \
  {Ricalcola il codice ECO di tutte le partite}
menuText I FileMaintSort "Ordina database..." 0 \
  {Ordina tutte le partite nel database}
menuText I FileMaintDelete "Cancella le partite doppie..." 20 \
  {Cerca le partite duplicate e le seleziona per cancellarle}
menuText I FileMaintTwin "Finestra di controllo delle doppie" 1 \
  {Apre/aggiorna la finestra del controllo dei duplicati}
menuText I FileMaintName "Controllo di nomi" 13 {Editor di nomi e controllo ortografico}
menuText I FileMaintNameEditor "Finestra di editor dei nomi" 0 \
  {Apre/chiude la finestra di editor dei nomi}
menuText I FileMaintNamePlayer "Controllo di nomi dei giocatori..." 22 \
  {Controllo di nomi dei giocatori mediante il file di verifica di nomi}
menuText I FileMaintNameEvent "Controllo di nomi di eventi..." 21 \
  {Controllo di nomi degli eventi mediante il file di verifica di eventi}
menuText I FileMaintNameSite "Controllo di nomi di siti..." 21 \
  {Controllo di nomi di siti mediante il file di verifica}
menuText I FileMaintNameRound "Controllo di nomi di turni..." 21 \
  {Controllo di nomi di turni mediante il file di verifica}
menuText I FileReadOnly "Sola lettura..." 5 \
  {Tratta il database attuale in sola lettura, prevenendo variazioni}
menuText I FileSwitch "Attiva un database" 3 \
  {Attiva uno fra i database aperti}
menuText I FileExit "Esci" 0 {Esce da Scid}
menuText I FileMaintFixBase "Ripristina database" 0 {Cerca di ripristinare un database danneggiato}

# Edit menu:
menuText I Edit "Modifica" 0
menuText I EditAdd "Aggiunge una variante" 0 {Aggiunge una variante a questa mossa nella partita}
menuText I EditDelete "Cancella variante" 0 {Cancella una variante per questa mossa}
menuText I EditFirst "Variante principale" 5 \
  {Promuove a variante principale nella lista}
menuText I EditMain "Promuove una variante a linea principale" 9 \
  {Promuove una variante a linea principale}
menuText I EditTrial "Prova una variante" 8 \
  {Attiva/disattiva la modalità di prova di una variante, per provare idee direttamente sulla scacchiera}
menuText I EditStrip "Pulisci" 2 {Elimina commenti o varianti dalla partita corrente}
menuText I EditUndo "Annulla" 0 {Annulla l'ultima modifica della partita}
menuText I EditRedo "Ripeti" 0 {Ripete l'ultima modifica della partita}
menuText I EditStripComments "Commenti" 0 \
  {Elimina tutti i commenti e le annotazioni dalla parita corrente}
menuText I EditStripVars "Varianti" 0 {Elimina tutte le varianti dalla partita corrente}
menuText I EditStripBegin "Mosse dall'inizio" 1 \
  {Elimina le mosse dall'inizio della partita}
menuText I EditStripEnd "Mosse rimanenti" 0 \
  {Elimina le mosse rimanenti}
menuText I EditReset "Vuota la clipbase" 0 \
  {Svuota completamente la clipbase}
menuText I EditCopy "Copia questa partita nella clipbase" 1 \
  {Copia questa partita nella clipbase}
menuText I EditPaste "Incolla l'ultima partita nella clipbase" 0 \
  {Incolla qui la partita della clipbase}
menuText I EditPastePGN "Incolla il testo della clipboard come partita PGN..." 13 \
  {Interpreta il testo della clipboard come una partita in notazione PGN e lo incolla qui}
menuText I EditSetup "Posizione definita..." 10 \
  {Definisce una posizione per la partita corrente}
menuText I EditCopyBoard "Copia posizione" 6 \
  {Copia la posizione della scacchiera corrente in notazione FEN come testo (clipboard)}
menuText I EditPasteBoard "Incolla come posizione di partenza" 17 \
  {Imposta la posizione di partenza dalla selezione del testo corrente (clipboard)}
# ====== TODO To be translated ======
menuText I ConfigureScid "Preferences..." 0 {Configure all options for SCID}

# Game menu:
menuText I Game "Partita" 0
menuText I GameNew "Nuova partita" 0 \
  {Torna alla posizione iniziale, rinunciando ai cambiamenti}
menuText I GameFirst "Carica la prima partita" 13 {Carica la prima partita del filtro}
menuText I GamePrev "Carica la partita precedente" 10 {Carica la precedente partita del filtro}
menuText I GameReload "Ricarica la partita attuale" 0 \
  {Ricarica questa partita, eliminando ogni cambiamento eseguito}
menuText I GameNext "Carica la partita successiva" 18 {Carica la successiva partita del filtro}
menuText I GameLast "Carica l'ultima partita" 9 {Carica l'ultima partita del filtro}
menuText I GameRandom "Carica una partita casuale" 0 {Carica una partita casuale del filtro}
menuText I GameNumber "Carica la partita numero..." 18 \
  {Carica una partita digitando il suo numero}
menuText I GameReplace "Salva: sostituisce la partita..." 8 \
  {Salva questa partita, sostituendo la vecchia versione}
menuText I GameAdd "Salva: aggiunge la nuova partita..." 7 \
  {Salva questa partita come nuova partita nel database}
menuText I GameDeepest "Identifica apertura" 0 \
  {Va alla posizione di partita più profonda listata nel libro ECO}
menuText I GameGotoMove "Vai alla mossa numero..." 0 \
  {Va al numero di mossa specificato nella partita attuale}
menuText I GameNovelty "Trova novità..." 0 \
  {Cerca la prima mossa mai giocata della partita corrente}

# Search Menu:
menuText I Search "Cerca" 0
menuText I SearchReset "Annulla il filtro" 0 {Annulla il filtro per includere tutte le partite}
menuText I SearchNegate "Inverti il filtro" 0 {Inverte il filtro per comprendere solo le partite escluse}
menuText I SearchCurrent "Posizione attuale..." 0 {Cerca la posizione attuale}
menuText I SearchHeader "Intestazione..." 0 {Ricerca per intestazione (giocatore, evento, etc)}
menuText I SearchMaterial "Materiale/schema..." 0 {Ricerca per materiale o schema posizionale}
menuText I SearchUsing "Uso del file di ricerca..." 0 {Ricerca mediante un file di opzioni}

# Windows menu:
menuText I Windows "Finestre" 2
menuText I WindowsComment "Editor di commenti" 10 {Apre/chiude l'editor di commenti}
menuText I WindowsGList "Lista di partite" 0 {Apre/chiude la lista di partite}
menuText I WindowsPGN "Finestra PGN" 9 {Apre/chiude la finestra PGN (notazione della partita)}
menuText I WindowsPList "Trova giocatore" 2 {Apre/chiude la finestra per la ricerca di un giocatore}
menuText I WindowsTmt "Trova torneo" 1 {Apre/chiude la finestra per la ricerca di un torneo}
menuText I WindowsSwitcher "Selezione di database" 11 {Apre/chiude la finestra per la selezione di un database}
menuText I WindowsMaint "Finestra di gestione" 12 {Apre/chiude la finestra per la gestione di un database}
menuText I WindowsECO "Navigazione ECO" 12 {Apre/chiude la finestra di navigazione ECO}
menuText I WindowsStats "Finestra di statistica" 12 {Apre/chiude la finestra di filtro statistico}
menuText I WindowsTree "Finestra di albero" 12 {Apre/chiude la finestra di albero}
menuText I WindowsTB "Finestra di tablebase" 12 \
  {Apre/chiude la finestra Tablebase}
menuText I WindowsBook "Libro di aperture" 2 {Apre/chiude la finestra del libro delle aperture}
menuText I WindowsCorrChess "Gioco per corrispondenza" 10 {Apre/chiude la finestra per il gioco per corrispondenza}

# Tools menu:
menuText I Tools "Strumenti" 0
menuText I ToolsAnalysis "Motore di analisi..." 10 \
  {Avvia/ferma il motore di analisi scacchistica}
menuText I ToolsAnalysis2 "Motore di analisi 2..." 20 \
  {Avvia/ferma il motore di analisi scacchistica}
menuText I ToolsCross "Tabellone" 0 {Mostra il tabellone del torneo per questa partita}
menuText I ToolsEmail "Gestione email" 10 \
  {Apre/chiude la finestra di gestione di partite per posta elettronica}
menuText I ToolsFilterGraph "Grafici relativi" 3 \
  {Apre/chiude la finestra dei grafici sul filtro con valori relativi}
menuText I ToolsAbsFilterGraph "Grafici assoluti" 13 {Apre/chiude la finestra dei grafici sul filtro con valori assoluti}
menuText I ToolsOpReport "Rapporto di apertura" 0 \
  {Genera un rapporto di apertura per l'attuale posizione}
menuText I ToolsOpenBaseAsTree "Apri come albero..." 1   {Apre un database nella finestra per l'albero}
menuText I ToolsOpenRecentBaseAsTree "Apri recente come albero" 2   {Apre un database recente nella finestra per l'albero}
menuText I ToolsTracker "Dislocazione dei pezzi"  0 {Apre/chiude la finestra con la traccia sulla dislocazione dei pezzi}
menuText I ToolsTraining "Allenamento"  0 {Strumenti per l'allenamento (tattica, aperture,...) }
menuText I ToolsTacticalGame "Partita tattica"  8 {Gioca una partita tattica}
menuText I ToolsSeriousGame "Partita seria"  0 {Gioca una partita seria}
menuText I ToolsTrainOpenings "Aperture"  0 {Allenati con un repertorio}
menuText I ToolsTrainReviewGame "Rivedi partita"  0 {Indovina le mosse giocate in partita}
menuText I ToolsTrainTactics "Tattica"  0 {Risolvi problemi tattici}
menuText I ToolsTrainCalvar "Calcolo delle varianti"  0 {Allenati sul calcolo delle varianti}
menuText I ToolsTrainFindBestMove "Trova la mossa migliore"  2 {Trova la mossa migliore}
menuText I ToolsTrainFics "Gioca su internet"  0 {Gioca su freechess.org}
menuText I ToolsEngineTournament "Torneo fra motori"  0 {Inizia un torneo fra motori scacchistici}
menuText I ToolsBookTuning "Raffina il libro delle aperture" 5 {Raffina il libro delle aperture}
menuText I ToolsConnectHardware "Connetti dispositivo" 0 {Connette un dispositivo esterno}
menuText I ToolsConnectHardwareConfigure "Configurazione" 0 {Configura un dispositivo esterno}
menuText I ToolsConnectHardwareNovagCitrineConnect "Connetti Novag Citrine" 10 {Connette la periferica Novag Citrine}
menuText I ToolsConnectHardwareInputEngineConnect "Connetti periferica di input" 10 {Connette periferica di input (p. es. DGT)}
menuText I ToolsPInfo "Informazioni sul giocatore"  17 \
  {Apre/aggiorna la finestra di informazioni del giocatore}
menuText I ToolsPlayerReport "Report del giocatore ..." 3 \
  {Genera un report del giocatore}
menuText I ToolsRating "Grafico del punteggio Elo" 13 \
  {Grafico storico del punteggio Elo dei giocatori della corrente partita}
menuText I ToolsScore "Grafico dei punti" 12 {Mostra il grafico dei punti}
menuText I ToolsExpCurrent "Esporta la partita attuale" 0 \
  {Scrive la partita attuale in un file di testo}
menuText I ToolsExpCurrentPGN "Esporta partita in un file PGN..." 0 \
  {Scrive la partita attuale in un file PGN}
menuText I ToolsExpCurrentHTML "Esporta partita in un file HTML..." 1 \
  {Scrive la partita attuale in un file HTML}
menuText I ToolsExpCurrentHTMLJS "Esporta partita in un file HTML con JavaScript..." 2 {Scrive la partita attuale in un file HTML con JavaScript}
menuText I ToolsExpCurrentLaTeX "Esporta partita in un file LaTeX..." 3 \
  {Scrive la partita attuale in un file LaTeX}
menuText I ToolsExpFilter "Esporta tutte le partite del filtro" 1 \
  {Scrive tutte le partite filtrate in un file di testo}
menuText I ToolsExpFilterPGN "Esporta filtro in file PGN..." 0 \
  {Scrive tutte le partite filtrate in un file PGN}
menuText I ToolsExpFilterHTML "Esporta filtro in file HTML..." 1 \
  {Scrive tutte le partite filtrate in un file HTML}
menuText I ToolsExpFilterHTMLJS "Esporta filtro in file HTML con JavaScript..." 12 {Scrive tutte le partite filtrate in un file HTML con JavaScript}
menuText I ToolsExpFilterLaTeX "Esporta filtro in un file LaTeX..." 3 \
  {Scrive tutte le partite filtrate in un file LaTeX}
menuText I ToolsImportOne "Importa una partita in formato PGN..." 0 \
  {Importa una partita da testo PGN}
menuText I ToolsImportFile "Importa file di partite PGN..." 27 \
  {Importa le partite da uno o più file PGN}
menuText I ToolsStartEngine1 "Avvia il motore 1" 1  {Avvia il primo motore per l'analisi della posizione}
menuText I ToolsStartEngine2 "Avvia il motore 2" 7  {Avvia il secondo motore per l'analisi della posizione}
menuText I ToolsCaptureBoard "Salva la posizione come immagine..." 13  {Salva la posizione corrente come immagine.}

# Play menue
menuText I Play "Gioca" 0

# --- Correspondence Chess
menuText I CorrespondenceChess "Gioco per corrispondenza" 0 {Funzioni per il gioco per corrispondenza tramite email e Xfcc}
menuText I CCConfigure "Configura..." 0 {Configurazione generale e degli strumenti esterni}
menuText I CCConfigRelay "Configura l'osservazione..." 10 {Configura le partite da osservare}
menuText I CCOpenDB "Apri database..." 0 {Apri il database di default per il gioco per corrispondenza}
menuText I CCRetrieve "Trova partite" 0 {Trova le partite tramite l'aiuto esterno Xfcc}
menuText I CCInbox "Processa posta in arrivo" 0 {Processa tutti i file nella posta in arrivo di Scid}
menuText I CCSend "Spedisci mossa" 0 {Spedisce la tua mossa tramite email o Xfcc}
menuText I CCResign "Abbandona" 1 {Abbandona la partita (non tramite email)}
menuText I CCClaimDraw "Richiedi patta" 0 {Spedisce la mossa e richiede la patta (non tramite email)}
menuText I CCOfferDraw "Offri patta" 0 {Spedisce la mossa e offre la patta (non tramite email)}
menuText I CCAcceptDraw "Accetta patta" 3 {Accetta l'offerta di patta (non tramite email)}
menuText I CCNewMailGame "Nuova partita per email..." 0 {Avvia una nuova partita tramite email}
menuText I CCMailMove "Mossa per email..." 0 {Spedisce la mossa all'avversariio tramite email}
menuText I CCGamePage "Pagina della partita..." 2 {Apre la partita tramite il browser web}
menuText I CCEditCopy "Copia la lista di partite nella clipboard" 0 {Copia le partite come lista CSV nella clipbase}

# Options menu:
menuText I Options "Opzioni" 0
menuText I OptionsBoardGraphics "Caselle..." 0 {Seleziona l'aspetto delle caselle}
translate I OptionsBGW {Seleziona l'aspetto delle caselle}
translate I OptionsBoardGraphicsText {Seleziona i file grafici per le caselle bianche e nere:}
menuText I OptionsBoardNames "I nomi dei miei giocatori ..." 0 {Edita i nomi dei miei giocatori}
menuText I OptionsExport "Esportazione" 2 {Cambia le opzioni di importazione testo}
menuText I OptionsFonts "Font" 0 {Cambia font}
menuText I OptionsFontsRegular "Regolare" 0 {Cambia il font normale}
menuText I OptionsFontsMenu "Menu" 0 {Cambia il font dei menu}
menuText I OptionsFontsSmall "Piccolo" 0 {Cambia il font piccolo}
menuText I OptionsFontsTiny "Minuto" 1 {Cambia il font minuto}
menuText I OptionsFontsFixed "Fisso" 0 {Cambia il font di larghezza fissa}
menuText I OptionsGInfo "Informazioni sulla partita" 0 {Opzioni sulle informazioni della partita}
menuText I OptionsLanguage "Lingua" 0 {Sceglie una nuova lingua di menu}
menuText I OptionsMovesTranslatePieces "Traduci i pezzi" 0 {Traduce la prima lettera dei pezzi}
menuText I OptionsMovesHighlightLastMove "Evidenzia l'ultima mossa" 0 {Evidenzia l'ultima mossa}
menuText I OptionsMovesHighlightLastMoveDisplay "Mostra" 0 {Mostra in evidenza l'ultima mossa}
menuText I OptionsMovesHighlightLastMoveWidth "Larghezza" 0 {Spessore della linea}
menuText I OptionsMovesHighlightLastMoveColor "Colore" 0 {Colore della linea}
menuText I OptionsMovesHighlightLastMoveArrow "Includi freccia" 0 {Include una freccia}
menuText I OptionsMoves "Mosse" 0 {Opzioni di immissione delle mosse}
menuText I OptionsMovesAnimate "Intervallo di tempo per l'animazione delle mosse" 4 \
  {Definisce l'intervallo di tempo per l'animazione delle mosse}
menuText I OptionsMovesDelay "Intervallo di tempo per il gioco automatico..." 1 \
  {Definisce l'intervallo di tempo per il gioco automatico}
menuText I OptionsMovesCoord "Immissione mossa per coordinate" 0 \
  {Accetta l'input di mossa mediante coordinate ("g1f3")}
menuText I OptionsMovesSuggest "Mostra le mosse suggerite" 0 \
  {Attiva/disattiva il suggerimento di mosse}
menuText I OptionsShowVarPopup "Mostra la finestra varianti" 2 {Attiva/disattiva la visualizzazione della finestra delle varianti}
menuText I OptionsMovesSpace "Aggiungi gli spazi dopo il numero della mossa" 0 {Aggiunge uno spazio dopo il numero della mossa}
menuText I OptionsMovesKey "Completamento da tastiera" 1 \
  {Attiva/disattiva l'autocompletamento di mosse da tastiera}
menuText I OptionsMovesShowVarArrows "Mostra frecce per le varianti" 4 {Attiva/disattiva le freccie indicanti le mosse delle varianti}
menuText I OptionsMovesGlossOfDanger "Codici di pericolo a colori" 1 {Attiva/disattiva i codici di pericolo a colori}
menuText I OptionsNumbers "Formato dei numeri" 13 {Seglie il formato dei numeri}
menuText I OptionsTheme "Tema" 0 {Cambia l'aspetto dell'interfaccia}
menuText I OptionsWindows "Finestre" 2 {Opzioni di finestra}
menuText I OptionsSounds "Suoni" 2 {Configura i suoni per l'annuncio delle mosse} ;# ***
# ====== TODO To be translated ======
menuText I OptionsResources "Resources..." 0 {Choose resource files and folders}
menuText I OptionsWindowsDock "Aggancia le finestre" 0 {Aggancia le finestre}
menuText I OptionsWindowsSaveLayout "Salva layout" 0 {Salva il layout}
menuText I OptionsWindowsRestoreLayout "Ripristina layout" 0 {Ripristina il layout}
menuText I OptionsWindowsShowGameInfo "Mostra informazioni partita" 0 {Mostra le informazioni sulla partita}
menuText I OptionsWindowsAutoLoadLayout "Carica automaticamente il primo layout" 0 {Carica automaticamente il primo layout all'avvio}
menuText I OptionsECO "Carica il file ECO..." 15 {Carica il file di classificazione ECO}
menuText I OptionsSpell "Carica il file di controllo ortografico" 0 \
  {Carica il file di controllo ortografico}
menuText I OptionsTable "Directory di tablebase" 8 \
  {Seglie un file tablebase; tutte le tablebase nella directory saranno utilizzate}
menuText I OptionsRecent "File utilizzati di recente..." 10 \
  {Cambia il numero di file recentemente utilizzati visualizzato nel menu File}
menuText I OptionsBooksDir "Directory del libro di aperture" 0 {Configura la directory per il libro delle aperture}
menuText I OptionsTacticsBasesDir "Directory dei database" 15 {Configura la directory per i database di allenamento}
# ====== TODO To be translated ======
menuText I OptionsPhotosDir "Photos directory" 0 {Sets the photos bases directory}
# ====== TODO To be translated ======
menuText I OptionsThemeDir "Theme(s) File:"  0 { Load a GUI theme package file }
menuText I OptionsSave "Salva opzioni" 3 \
  "Salva tutte le opzioni definibili nel file $::optionsFile"
menuText I OptionsAutoSave "Salva opzioni all'uscita" 17 \
  {Salva automaticamente tutte le opzioni quando si esce da Scid}

# Help menu:
menuText I Help "Aiuto" 0
menuText I HelpContents "Contenuti" 0 {Mostra il sommario della guida}
menuText I HelpIndex "Indice" 0 {Mostra l'indice della guida}
menuText I HelpGuide "Guida rapida" 0 {Mostra la pagina di aiuto per la guida veloce}
menuText I HelpHints "Suggerimenti" 0 {Mostra la pagina dei suggerimenti}
menuText I HelpContact "Informazioni di contatto" 17 {Mostra la pagina delle informazioni di contatto}
menuText I HelpTip "Suggerimento del giorno" 1 {Mostra utili suggerimenti riguardo Scid}
menuText I HelpStartup "Finestra di avvio" 0 {Mostra la finestra di avvio}
menuText I HelpAbout "Informazioni su Scid" 1 {Informazioni su Scid}

# partita info box popup menu:
menuText I GInfoHideNext "Nascondi la mossa successiva" 0
menuText I GInfoMaterial "Mostra i valori del materiale" 0
menuText I GInfoFEN "Mostra FEN" 7
menuText I GInfoMarks "Mostra caselle e frecce colorate" 2
menuText I GInfoWrap "A capo automatico" 0
menuText I GInfoFullComment "Mostra i commenti completi" 10
menuText I GInfoPhotos "Visualizza foto" 0
menuText I GInfoTBNothing "Tablebase: nulla" 12
menuText I GInfoTBResult "Tablebase: solo risultato" 11
menuText I GInfoTBAll "Tablebase: risultato e mosse migliori" 11
menuText I GInfoDelete "Recupera/cancella questa partita" 9
menuText I GInfoMark "Smarca/marca questa partita" 7
menuText I GInfoInformant "Configura i valori dei simboli per i commenti" 0

# General buttons:
translate I Back {Indietro}
translate I Browse {Browse}
translate I Cancel {Annulla}
translate I Continue {Continua}
translate I Clear {Cancella}
translate I Close {Chiudi}
translate I Contents {Contenuto}
translate I Defaults {Default}
translate I Delete {Cancella}
translate I Graph {Grafico}
translate I Help {Aiuto}
translate I Import {Importa}
translate I Index {Indice}
translate I LoadGame {Carica partita}
translate I BrowseGame {Visualizza partita}
translate I MergeGame {Fondi partita}
translate I MergeGames {Unisci partite}
translate I Preview {Anteprima}
translate I Revert {Rovescia}
translate I Save {Salva}
translate I Search {Cerca}
translate I Stop {Ferma}
translate I Store {Memorizza}
translate I Update {Aggiorna}
translate I ChangeOrient {Modifica l'orientamento della finestra}
translate I ShowIcons {Mostra icone}
translate I None {Nessuna}
translate I First {Prima}
translate I Current {Attuale}
translate I Last {Ultima}

# General messages:
translate I game {partita}
translate I games {Partite}
translate I move {mossa}
translate I moves {mosse}
translate I all {tutto}
translate I Yes {Sì}
translate I No {No}
translate I Both {Entrambi}
translate I King {Re}
translate I Queen {Donna}
translate I Rook {Torre}
translate I Bishop {Alfiere}
translate I Knight {Cavallo}
translate I Pawn {Pedone}
translate I White {Bianco}
translate I Black {Nero}
translate I Player {Giocatore}
translate I Rating {Punteggio}
translate I RatingDiff {Differenza di punteggio (Bianco - Nero)}
translate I AverageRating {Punteggio medio}
translate I Event {Evento}
translate I Site {Sito}
translate I Country {Paese}
translate I IgnoreColors {Ignora i colori}
translate I Date {Data}
translate I EventDate {Data evento}
translate I Decade {Decennio}
translate I Year {Anno}
translate I Month {Mese}
translate I Months {Gennaio Febbraio Marzo Aprile Maggio Giugno Luglio Agosto Settembre Ottobre Novembre Dicembre}
translate I Days {Dom Lun Mar Mer Gio Ven Sab}
translate I YearToToday {Anno ad oggi}
# ====== TODO To be translated ======
translate I YearToTodayTooltip {Set date from 1 year back to today}
translate I Result {Risultato}
translate I Round {Turno}
translate I Length {Lunghezza}
translate I ECOCode {codice ECO}
translate I ECO {ECO}
translate I Deleted {Cancellato}
translate I SearchResults {Risultati della ricerca}
translate I OpeningTheDatabase {Apertura del database}
translate I Database {Database}
translate I Filter {Filtro}
translate I noGames {nessuna partita}
translate I allGames {tutte le partite}
translate I empty {vuoto}
translate I clipbase {clipbase}
translate I score {score}
translate I StartPos {Posizione iniziale}
translate I Total {Totale}
translate I readonly {sola lettura}

# Standard error messages:
translate I ErrNotOpen {Questo database non è aperto.}
translate I ErrReadOnly {Questo database è di sola lettura; non può essere modificato.}
translate I ErrSearchInterrupted {Ricerca interrotta; il risultato è incompleto.}

# Game information:
translate I twin {doppio}
translate I deleted {cancellato}
translate I comment {commento}
translate I hidden {nascosto}
translate I LastMove {Ultima mossa}
translate I NextMove {Mossa Successiva}
translate I GameStart {Inizio di partita}
translate I LineStart {Inizio di variante}
translate I GameEnd {Fine della partita}
translate I LineEnd {Fine della variante}

# Player information:
translate I PInfoAll {Risultati per <b>tutte</b> le partite}
translate I PInfoFilter {Risultati per le partite nel <b>filtro</b>}
translate I PInfoAgainst {Risultati contro}
translate I PInfoMostWhite {Le più frequenti aperture con il Bianco}
translate I PInfoMostBlack {Le più frequenti aperture con il Nero}
translate I PInfoRating {Variazioni Elo}
translate I PInfoBio {Biografia}
translate I PInfoEditRatings {Edita i rating}
# ====== TODO To be translated ======
translate I PInfoEloFile {File}

# Tablebase information:
translate I Draw {Patta}
translate I stalemate {stallo}
translate I withAllMoves {con tutte le mosse}
translate I withAllButOneMove {con tutte le mosse tranne una}
translate I with {con}
translate I only {solo}
translate I lose {persa}
translate I loses {perde}
translate I allOthersLose {tutte le altre perse}
translate I matesIn {matto in}
translate I hasCheckmated {è mattato}
translate I longest {la più lunga}
translate I WinningMoves {Mosse vincenti}
translate I DrawingMoves {Mosse pattanti}
translate I LosingMoves {Mosse perdenti}
translate I UnknownMoves {Mosse senza risultato noto}
# ====== TODO To be translated ======
translate I SideLongest {Side    Longest    %     %     %\n}
# ====== TODO To be translated ======
translate I toNoveMate {to move   mate    Win  Draw  Loss\n}
# ====== TODO To be translated ======
translate I NoSummary {\nNo summary for this tablebase.}
# ====== TODO To be translated ======
translate I MutualZugzwang {Mutual zugzwangs: }
# ====== TODO To be translated ======
translate I SelectetZugzwang {\nSelected zugzwang positions:}
# ====== TODO To be translated ======
translate I WdrawBloss {White draws, Black loses:}
# ====== TODO To be translated ======
translate I BdrawWloss {Black draws, White loses:}
# ====== TODO To be translated ======
translate I AllLoss {" Whoever moves loses:     " }

# Tip of the day:
translate I Tip {Suggerimento}
translate I TipAtStartup {Suggerimento all'avvio}

# Tree window menus:
menuText I TreeFile "File" 0
menuText I TreeFileFillWithBase "Riempi cache con database" 0 {Riempie la cache con tutte le partite nel database attuale}
menuText I TreeFileFillWithGame "Riempi cache con partita" 2 {Riempie la cache con la partita corrente del database attuale}
menuText I TreeFileSetCacheSize "Dimensione cache" 0 {Seleziona la dimensione della cache}
menuText I TreeFileCacheInfo "Informazioni cache" 0 {Visualizza informazioni sull'uso della cache}
menuText I TreeFileSave "Salva il file di cache" 0 \
  {Salva il file (.stc) della cache dell'albero}
menuText I TreeFileFill "Riempie il file di cache" 0 \
  {Riempie il file di cache con le più frequenti posizioni di apertura}
menuText I TreeFileBest "Lista delle migliori partite" 0 {Mostra la lista delle migliori partite}
menuText I TreeFileGraph "Grafico dell'albero" 0 \
  {Mostra il grafico per la corrente variante dell'albero}
menuText I TreeFileCopy "Copia il testo dell'albero nella clipboard" 1 \
  {Copia le statistiche dell'albero nella clipboard}
menuText I TreeFileClose "Chiudi la finestra dell'albero" 0 \
  {Chiudi la finestra dell'albero}
menuText I TreeMask "Maschera" 0
menuText I TreeMaskNew "Nuova" 0 {Nuova maschera}
menuText I TreeMaskOpen "Apri" 0 {Apre una maschera}
menuText I TreeMaskOpenRecent "Apri recente" 1 {Apre una maschera recente}
menuText I TreeMaskSave "Salva" 0 {Salva la maschera}
menuText I TreeMaskClose "Chiudi" 0 {Chiude la maschera}
menuText I TreeMaskFillWithGame "Riempi con partita" 0 {Riempie la maschera con la partita}
menuText I TreeMaskFillWithBase "Riempi con database" 1 {Riempie la maschera con tutte le partite del database}
menuText I TreeMaskInfo "Info" 0 {Mostra le statistiche per la maschera corrente}
menuText I TreeMaskDisplay "Visualizza mappa della maschera" 0 {Mostra i dati della maschera in forma di albero}
menuText I TreeMaskSearch "Cerca" 1 {Cerca nella maschera corrente}
menuText I TreeSort "Ordinamento" 0
menuText I TreeSortAlpha "Alfabetico" 0
menuText I TreeSortECO "Codice ECO" 0
menuText I TreeSortFreq "Frequenza" 0
menuText I TreeSortScore "Punti" 0
menuText I TreeOpt "Opzioni" 0
menuText I TreeOptSlowmode "Modalità lenta" 0 {Modalità lenta per l'aggiornamento (più precisa)}
menuText I TreeOptFastmode "Modalità veloce" 0 {Modalità veloce per l'aggiornamento (nessuna trasposizione di mosse)}
menuText I TreeOptFastAndSlowmode "Modalità lenta e veloce" 0 {Modalità lenta e veloce per l'aggiornamento}
menuText I TreeOptStartStop "Aggiornamento automatico" 0 {Attiva/disattiva l'aggiornamento automatico della finestra di albero}
menuText I TreeOptLock "Blocca" 0 {Blocca/sblocca l'albero al database corrente}
menuText I TreeOptTraining "Allenamento" 0 {Attiva/disattiva la modalità di esercizio dell'albero}
menuText I TreeOptAutosave "File di cache per il salvataggio automatico" 0 \
  {Salva automaticamente il file di cache quando si chiude la finestra dell'albero}
menuText I TreeHelp "Aiuto" 0
menuText I TreeHelpTree "Aiuto per l'albero" 0
menuText I TreeHelpIndex "Indice" 0
translate I SaveCache {Salva cache}
translate I Training {Esercizio}
translate I LockTree {Blocca}
translate I TreeLocked {Bloccato}
translate I TreeBest {Migliore}
translate I TreeBestGames {Migliori partite}
# Note: the next message is the tree window title row. After editing it,
# check the tree window to make sure it lines up with the actual columns.
translate I TreeTitleRow \
  {    Mossa  ECO       Frequenza    Punt.  AvElo Perf AvAnno %Patta}
translate I TreeTotal {TOTALE}
translate I DoYouWantToSaveFirst {Vuoi prima salvare}
translate I AddToMask {Aggiungi alla maschera}
translate I RemoveFromMask {Rimuovi dalla maschera}
translate I AddThisMoveToMask {Aggiungi questa mossa alla maschera}
translate I SearchMask {Cerca nella maschera}
translate I DisplayMask {Visualizza la maschera}
translate I Nag {Codice Nag}
translate I Marker {Marcatore}
translate I Include {Includi}
translate I Exclude {Escludi}
translate I MainLine {Linea principale}
translate I Bookmark {Segnalibro}
translate I NewLine {Nuova linea}
translate I ToBeVerified {Da verificare}
translate I ToTrain {Esercitarsi}
translate I Dubious {Dubbio}
translate I ToRemove {Da rimuovere}
translate I NoMarker {Nessun marcatore}
translate I ColorMarker {Colore}
translate I WhiteMark {Bianco}
translate I GreenMark {Verde}
translate I YellowMark {Giallo}
translate I BlueMark {Blu}
translate I RedMark {Rosso}
translate I CommentMove {Commenta la mossa}
translate I CommentPosition {Commenta la posizione}
translate I AddMoveToMaskFirst {Aggiungi subito la mossa alla maschera}
translate I OpenAMaskFileFirst {Apri subito una maschera}
translate I Positions {Posizioni}
translate I Moves {Mosse}

# Finder window:
menuText I FinderFile "File" 0
menuText I FinderFileSubdirs "Cerca nelle subdirectory" 0
menuText I FinderFileClose "Chiudi la ricerca di file" 0
menuText I FinderSort "Ordina" 0
menuText I FinderSortType "Tipo" 0
menuText I FinderSortSize "Dimensione" 0
menuText I FinderSortMod "Modifica" 0
menuText I FinderSortName "Nome" 0
menuText I FinderSortPath "Percorso" 0
menuText I FinderTypes "Tipi" 0
menuText I FinderTypesScid "Database Scid" 0
menuText I FinderTypesOld "Vecchio formato database Scid" 0
menuText I FinderTypesPGN "File PGN" 0
menuText I FinderTypesEPD "file EPD (libro)" 0
menuText I FinderTypesRep "File di repertorio" 0
menuText I FinderHelp "Aiuto" 0
menuText I FinderHelpFinder "Aiuto sulla ricerca di file" 0
menuText I FinderHelpIndex "Indice" 0
translate I FileFinder {Trova file}
translate I FinderDir {Trova directory}
translate I FinderDirs {Trova directory}
translate I FinderFiles {Trova file}
translate I FinderUpDir {Su}
translate I FinderCtxOpen {Apri}
translate I FinderCtxBackup {Backup}
translate I FinderCtxCopy {Copia}
translate I FinderCtxMove {Sposta}
translate I FinderCtxDelete {Cancella}

# Player finder:
menuText I PListFile "File" 0
menuText I PListFileUpdate "Aggiorna" 0
menuText I PListFileClose "Chiudi Trova giocatore" 0
menuText I PListSort "Ordina" 0
menuText I PListSortName "Nome" 0
menuText I PListSortElo "Elo" 0
menuText I PListSortGames "Partite" 0
menuText I PListSortOldest "Più vecchie" 0
menuText I PListSortNewest "Più recenti" 0

# Tournament finder:
menuText I TmtFile "File" 0
menuText I TmtFileUpdate "Aggiorna" 0
menuText I TmtFileClose "Chiudi Trova torneo" 0
menuText I TmtSort "Ordina" 0
menuText I TmtSortDate "Data" 0
menuText I TmtSortPlayers "Giocatori" 0
menuText I TmtSortGames "Partite" 0
menuText I TmtSortElo "Elo" 0
menuText I TmtSortSite "Siti" 0
menuText I TmtSortEvent "Eventi" 1
menuText I TmtSortWinner "Vincitore" 0
translate I TmtLimit "Limite della lista"
translate I TmtMeanElo "Media Elo più bassa"
translate I TmtNone "Non è stato trovato nessun torneo."

# Graph windows:
menuText I GraphFile "File" 0
menuText I GraphFileColor "Salva come PostScript a colori..." 8
menuText I GraphFileGrey "Salva come PostScript in scala di grigi..." 8
menuText I GraphFileClose "Chiudi la finestra" 6
menuText I GraphOptions "Opzioni" 0
menuText I GraphOptionsWhite "Bianco" 0
menuText I GraphOptionsBlack "Nero" 0
menuText I GraphOptionsBoth "Entrambi" 1
menuText I GraphOptionsPInfo "Informazioni sul giocatore" 0
# ====== TODO To be translated ======
menuText I GraphOptionsEloFile "Elo from rating file" 0
# ====== TODO To be translated ======
menuText I GraphOptionsEloDB "Elo from database" 0
translate I GraphFilterTitle "Grafico del filtro: frequenza ogni 1000 partite"
translate I GraphAbsFilterTitle "Grafico del filtro: frequenza delle partite"
translate I ConfigureFilter {Configura l'asse delle X per Anno, Rating e Mosse}
translate I FilterEstimate "Stima"
translate I TitleFilterGraph "Scid: Grafico del filtro"

# Analysis window:
translate I AddVariation {Aggiungi variante}
translate I AddAllVariations {Aggiungi tutte le varianti}
translate I AddMove {Aggiungi una mossa}
translate I Annotate {Annota}
translate I ShowAnalysisBoard {Mostra la finestra dell'analisi}
translate I ShowInfo {Mostra le informazioni del motore}
translate I FinishGame {Termina la partita}
translate I StopEngine {Ferma il motore}
translate I StartEngine {Avvia il motore}
translate I LockEngine {Blocca il motore alla posizione corrente}
translate I AnalysisCommand {Comando di analisi}
translate I PreviousChoices {Scelta precedente}
translate I AnnotateTime {Imposta il tempo tra le mosse in secondi}
translate I AnnotateWhich {Aggiungi varianti}
translate I AnnotateAll {Per mosse di entrambi i colori}
translate I AnnotateAllMoves {Tutte le mosse}
translate I AnnotateWhite {Solo per le mosse del Bianco}
translate I AnnotateBlack {Solo per le mosse del Nero}
translate I AnnotateBlundersOnly {Quando la mossa è chiaramente un errore}
translate I AnnotateBlundersOnlyScoreChange {L'analisi riporta un errore, con variazione del punteggio da/a:}
translate I BlundersThreshold {Limite}
translate I ScoreAllMoves {Punteggio ad ogni mossa}
translate I LowPriority {Imposta la priorità della CPU a bassa}
translate I ClickHereToSeeMoves {Clicca qui per vedere le mosse}
translate I ConfigureInformant {Valore dei simboli per i commenti}
translate I Informant!? {Mossa interessante}
translate I Informant? {Mossa debole}
translate I Informant?? {Errore grave}
translate I Informant?! {Mossa dubbia}
translate I Informant+= {Il bianco ha un leggero vantaggio}
translate I Informant+/- {Il bianco ha un moderato vantaggio}
translate I Informant+- {Il bianco ha un vantaggio decisivo}
translate I Informant++- {La partita è considerata vinta}
translate I Book {Libro}
translate I OtherBookMoves {Libro dell'avversario}
translate I OtherBookMovesTooltip {Mosse alle quali l'avversario ha una risposta}

# Analysis Engine open dialog:
translate I EngineList {Lista dei motori di analisi}
translate I EngineName {Nome}
translate I EngineCmd {Comando}
translate I EngineArgs {Parametri}
translate I EngineDir {Directory}
translate I EngineElo {Elo}
translate I EngineTime {Data}
translate I EngineNew {Nuovo}
translate I EngineEdit {Modifica}
translate I EngineRequired {I campi in grassetto sono obbligatori; gli altri sono opzionali}

# PGN window menus:
menuText I PgnFile "File" 0
menuText I PgnFileCopy "Copia la partita verso la clipboard" 1
menuText I PgnFilePrint "Stampa su file..." 0
menuText I PgnFileClose "Chiudi la finestra PGN" 0
menuText I PgnOpt "Visualizza" 0
menuText I PgnOptColor "Visualizza il colore" 0
menuText I PgnOptShort "Intestazione compatta (3 righe)" 0
menuText I PgnOptSymbols "Annotazioni simboliche" 0
menuText I PgnOptIndentC "Indenta i commenti" 0
menuText I PgnOptIndentV "Indenta le varianti" 2
menuText I PgnOptColumn "Stile della colonna (una mossa per riga)" 1
menuText I PgnOptSpace "Spazio dopo i numeri delle mosse" 0
menuText I PgnOptStripMarks "Elimina i codici colorati delle caselle/frecce" 0
menuText I PgnOptBoldMainLine "Usa il grassetto per le mosse della linea principale" 0
menuText I PgnColor "Colori" 0
menuText I PgnColorHeader "Intestazione..." 0
menuText I PgnColorAnno "Annotazioni..." 0
menuText I PgnColorComments "Commenti..." 0
menuText I PgnColorVars "Varianti..." 0
menuText I PgnColorBackground "Sfondo..." 0
menuText I PgnColorMain "Linea principale..." 0
menuText I PgnColorCurrent "Sfondo per la mossa attuale..." 1
menuText I PgnHelp "Aiuto" 0
menuText I PgnHelpPgn "Aiuto PGN" 0
menuText I PgnHelpIndex "Indice" 0
translate I PgnWindowTitle {Notazione - partita %u}

# Crosstable window menus:
menuText I CrosstabFile "File" 0
menuText I CrosstabFileText "Stampa in un file di testo..." 21
menuText I CrosstabFileHtml "Stampa in un file HTML..." 18
menuText I CrosstabFileLaTeX "Stampa in un file LaTeX..." 18
menuText I CrosstabFileClose "Chiudi il tabellone" 0
menuText I CrosstabEdit "Edita" 0
menuText I CrosstabEditEvent "Evento" 0
menuText I CrosstabEditSite "Sito" 0
menuText I CrosstabEditDate "Data" 0
menuText I CrosstabOpt "Visualizza" 0
menuText I CrosstabOptAll "Torneo all'italiana" 0
menuText I CrosstabOptSwiss "Sistema svizzero" 0
menuText I CrosstabOptKnockout "Knockout" 0
menuText I CrosstabOptAuto "Auto" 0
menuText I CrosstabOptAges "Età in anni" 8
menuText I CrosstabOptNats "Nazionalità" 0
menuText I CrosstabOptRatings "Punteggi" 0
menuText I CrosstabOptTitles "Titoli" 0
menuText I CrosstabOptBreaks "Spareggio tecnico" 4
menuText I CrosstabOptDeleted "Inserisce le partite cancellate" 8
menuText I CrosstabOptColors "Colori (solo sistema svizzero)" 0
menuText I CrosstabOptColumnNumbers "A colonne (solo tornei all'italiana)" 3
menuText I CrosstabOptGroup "Punti del gruppo" 0
menuText I CrosstabSort "Ordina" 0
menuText I CrosstabSortName "Nome" 0
menuText I CrosstabSortRating "Punteggio" 0
menuText I CrosstabSortScore "Punti" 0
menuText I CrosstabColor "Colore" 0
menuText I CrosstabColorPlain "Testo normale" 0
menuText I CrosstabColorHyper "Ipertesto" 0
menuText I CrosstabHelp "Aiuto" 0
menuText I CrosstabHelpCross "Aiuto tabellone" 0
menuText I CrosstabHelpIndex "Indice" 0
translate I SetFilter {Imposta filtro}
translate I AddToFilter {Aggiungi al filtro}
translate I Swiss {Svizzero}
translate I Category {Categoria}

# Opening report window menus:
menuText I OprepFile "File" 0
menuText I OprepFileText "Stampa in un file di testo..." 21
menuText I OprepFileHtml "Stampa in un file HTML..." 18
menuText I OprepFileLaTeX "Stampa in un file LaTeX..." 18
menuText I OprepFileOptions "Opzioni..." 0
menuText I OprepFileClose "Chiude la finestra del rapporto" 0
menuText I OprepFavorites "Preferiti" 1
menuText I OprepFavoritesAdd "Aggiungi rapporto..." 0
menuText I OprepFavoritesEdit "Edita i rapporti preferiti..." 0
menuText I OprepFavoritesGenerate "Genera rapporto..." 0

menuText I OprepHelp "Aiuto" 0
menuText I OprepHelpReport "Aiuto sul rapporto di apertura" 0
menuText I OprepHelpIndex "Indice" 0

# Header search:
translate I HeaderSearch {Ricerca per intestazione}
translate I EndSideToMove {Lato che ha la mossa alla fine della partita}
translate I GamesWithNoECO {Partite senza ECO?}


translate I GameLength {Lunghezza della partita}
translate I FindGamesWith {Cerca partite con}
translate I StdStart {Inizio standard}
translate I Promotions {Promozioni}
translate I Comments {Commenti}
translate I Variations {Varianti}
translate I Annotations {Annotazioni}
translate I DeleteFlag {Cancellato}
translate I WhiteOpFlag {Apertura del Bianco}
translate I BlackOpFlag {Apertura del Nero}
translate I MiddlegameFlag {Mediogioco}
translate I EndgameFlag {Finale}
translate I NoveltyFlag {Novità}
translate I PawnFlag {Struttura pedonale}
translate I TacticsFlag {Tatticismi}
translate I QsideFlag {Gioco su lato di Donna}
translate I KsideFlag {Gioco su lato di Re}
translate I BrilliancyFlag {Brillantezza}
translate I BlunderFlag {Svista}
translate I UserFlag {Utente}
translate I PgnContains {Testo nel PGN}
translate I Annotator {Commentatore}
translate I Cmnts {Solo partite annotate}

# Game list window:
translate I GlistNumber {Numero}
translate I GlistWhite {Bianco}
translate I GlistBlack {Nero}
translate I GlistWElo {Elo B}
translate I GlistBElo {Elo N}
translate I GlistEvent {Evento}
translate I GlistSite {Sito}
translate I GlistRound {Turno}
translate I GlistDate {Data}
translate I GlistYear {Anno}
translate I GlistEDate {Data evento}
translate I GlistResult {Risultato}
translate I GlistLength {Lunghezza}
translate I GlistCountry {Paese}
translate I GlistECO {ECO}
translate I GlistOpening {Apertura}
translate I GlistEndMaterial {Materiale a fine partita}
translate I GlistDeleted {Cancellato}
translate I GlistFlags {Identificatore}
translate I GlistVars {Varianti}
translate I GlistComments {Commenti}
translate I GlistAnnos {Annotazioni}
translate I GlistStart {Inizio}
translate I GlistGameNumber {numero di partita}
translate I GlistAverageElo {Media Elo}
translate I GlistRating {Rating}
translate I GlistFindText {Cerca testo}
translate I GlistMoveField {Mossa}
translate I GlistEditField {Configura}
translate I GlistAddField {Aggiungi}
translate I GlistDeleteField {Elimina}
translate I GlistWidth {Larghezza}
translate I GlistAlign {Allinea}
translate I GlistAlignL {Alllinea a sinistra}
translate I GlistAlignR {Allinea a destra}
translate I GlistAlignC {Centrato}
translate I GlistColor {Colore}
translate I GlistSep {Separatore}
translate I GlistCurrentSep {-- Currente --}
translate I GlistNewSort {Nuovo}
translate I GlistAddToSort {Aggiungi}
translate I GsortSort {Ordina...}
translate I GsortDate {Data}
translate I GsortYear {Anno}
translate I GsortEvent {Evento}
translate I GsortSite {Sito}
translate I GsortRound {Turno}
translate I GsortWhiteName {Nome del Bianco}
translate I GsortBlackName {Nome del Nero}
translate I GsortECO {ECO}
translate I GsortResult {Risultato}
translate I GsortMoveCount {Numero di mosse}
translate I GsortAverageElo {Media Elo}
translate I GsortCountry {Paese}
translate I GsortDeleted {Cancellata}
translate I GsortEventDate {Data evento}
translate I GsortWhiteElo {Elo Bianco}
translate I GsortBlackElo {Elo Nero}
translate I GsortComments {Commenti}
translate I GsortVariations {Varianti}
translate I GsortNAGs {NAG}
translate I GsortAscending {Ascendente}
translate I GsortDescending {Discendente}
translate I GsortAdd {Aggiungi}
translate I GsortStore {Salva}
translate I GsortLoad {Carica}
translate I GlistRemoveThisGameFromFilter  {Rimuovi questa partita dal filtro}
translate I GlistRemoveGameAndAboveFromFilter  {Rimuovi la partita (e le precedenti) dal filtro}
translate I GlistRemoveGameAndBelowFromFilter  {Rimuovi la partita (e le seguenti) dal filtro}
translate I GlistDeleteGame {Elimina/recupera questa partita}
translate I GlistDeleteAllGames {Elimina tutte le partite nel filtro}
translate I GlistUndeleteAllGames {Recupera tutte le partite nel filtro}
translate I GlistMergeGameInBase {Fondi la partita in un altro database} 

# Maintenance window:
translate I DatabaseName {Nome del database:}
translate I TypeIcon {Tipo di icona:}
translate I NumOfGames {Partite:}
translate I NumDeletedGames {Partite cancellate:}
translate I NumFilterGames {Partite nel filtro:}
translate I YearRange {Intervallo di anni:}
translate I RatingRange {Intervallo di punteggio:}
translate I Description {Descrizione}
translate I Flag {Identificatore}
translate I CustomFlags {Identificatori personalizzati}
translate I DeleteCurrent {Cancella la partita attuale}
translate I DeleteFilter {Cancella le partite del filtro}
translate I DeleteAll {Cancella tutte le partite}
translate I UndeleteCurrent {Recupera la partita attuale}
translate I UndeleteFilter {Recupera il filtro di partite}
translate I UndeleteAll {Recupera tutte le partite}
translate I DeleteTwins {Cancella le partite doppie}
translate I MarkCurrent {Marca la partita attuale}
translate I MarkFilter {Marca partite del filtro}
translate I MarkAll {Marca tutte le partite}
translate I UnmarkCurrent {Smarca la partita attuale}
translate I UnmarkFilter {Smarca le partite del filtro}
translate I UnmarkAll {Smarca tutte le partite}
translate I Spellchecking {Controllo nome}
translate I Players {Giocatori}
translate I Events {Eventi}
translate I Sites {Siti}
translate I Rounds {Turni}
translate I DatabaseOps {Operazioni sul database}
translate I ReclassifyGames {Partite classificate ECO}
translate I CompactDatabase {Compatta database}
translate I SortDatabase {Ordina database}
translate I AddEloRatings {Aggiungi punteggio Elo}
translate I AutoloadGame {Numero della partita che si caricherà automaticamente}
translate I StripTags {Togli i tag PGN}
translate I StripTag {Toglie i tag}
translate I Cleaner {Manutentore globale}
translate I CleanerHelp {La finestra Manutentore globale effetturà tutte le operazioni che verranno selezionate dalla lista sottostante, sul database corrente.

Le impostazioni correnti nelle finestre della classificazione ECO e della cancellazione delle partite doppie verranno applicate se selezionate.}
translate I CleanerConfirm {Una volta attivata l'operazione di pulizia, questa non può più essere interrotta!

Questo può richiedere molto tempo su un grosso database, a seconda delle funzioni selezionate e delle impostazioni correnti.

Sei sicuro di voler procedere con le funzioni selezionate?}

translate I TwinCheckUndelete {per invertire; "u" recupera entrambe}
translate I TwinCheckprevPair {Coppia precedente}
translate I TwinChecknextPair {Coppia seguente}
translate I TwinChecker {Scid: Controllo partite doppie}
translate I TwinCheckTournament {Partite nel torneo:}
translate I TwinCheckNoTwin {Nessuna partita doppia}
translate I TwinCheckNoTwinfound {Nessun doppio è stato trovato per questa partita.\nPer mostrare i doppi con questa finestra, devi prima usare la funzione "Cancella le partite doppie".}
translate I TwinCheckTag {Condividi i tag...}
translate I TwinCheckFound1 {Scid ha trovato $result partite doppie}
translate I TwinCheckFound2 { e ha impostato il loro stato a cancellato}
translate I TwinCheckNoDelete {Non ci sono partite da cancellare nel database.}
translate I TwinCriteria1 {Le impostazioni per trovare le partite doppie possono portare a determinare come doppie partite diverse con mosse simili.}
translate I TwinCriteria2 {Si consiglia di selezionare "No" per "Stesse mosse", e "Sì" per colori, evento, luogo, turno, anno e mese.\nVuoi procedere e cancellare le partite doppie comunque?}
translate I TwinCriteria3 {Si consiglia di selezionare "Sì" per almeno due tra "stesso luogo", "stesso turno" e "stesso anno".\nVuoi procedere e cancellare le partite doppie comunque?}
translate I TwinCriteriaConfirm {Scid: Conferma le impostazioni per le partite doppie}
translate I TwinChangeTag "Cambia i seguenti tag della partita:\n\n"
translate I AllocRatingDescription "Questo comando usa l'attuale controllore ortografico per aggiungere i punteggi Elo alle partite in questo database. Se un giocatore non ha un punteggio ma il suo punteggio al momento della partita è presente nel controllore ortografico, quel punteggio sarà aggiunto."
translate I RatingOverride "Sovrascrivi i punteggi diversi da zero?"

translate I AddRatings "Aggiungi i punteggi a:"
translate I AddedRatings {Scid ha aggiunto $r punteggi Elo in $g partite.}
translate I NewSubmenu "Nuovo sottomenu"

# Comment editor:
translate I AnnotationSymbols  {Simboli di annotazione:}
translate I Comment {Commento:}
translate I InsertMark {Inserisci marcatore}
translate I InsertMarkHelp {
Inserisci/togli marcatore: seleziona colore, tipo, casella.
Inserisci/togli freccia: doppio clic due caselle.
}

# Nag buttons in comment editor:
translate I GoodMove {Mossa buona}
translate I PoorMove {Mossa debole}
translate I ExcellentMove {Mossa ottima}
translate I Blunder {Svista}
translate I InterestingMove {Mossa interessante}
translate I DubiousMove {Mossa dubbia}
translate I WhiteDecisiveAdvantage {Il Bianco è in vantaggio decisivo}
translate I BlackDecisiveAdvantage {Il Nero è in vantaggio decisivo}
translate I WhiteClearAdvantage {Il Bianco sta meglio}
translate I BlackClearAdvantage {Il Nero sta meglio}
translate I WhiteSlightAdvantage {Il Bianco sta leggermente meglio}
translate I BlackSlightAdvantage {Il Nero sta leggermente meglio}
translate I Equality {Parità}
translate I Unclear {Posizione incerta}
translate I Diagram {Diagramma}

# Board search:
translate I BoardSearch {Ricerca posizione}
translate I FilterOperation {Operazione sul filtro corrente:}
translate I FilterAnd {AND (Restrizione del filtro)}
translate I FilterOr {OR (Aggiunta al filtro)}
translate I FilterIgnore {IGNORA (Annulla il filtro)}
translate I SearchType {Tipo di ricerca:}
translate I SearchBoardExact {Posizione esatta (tutti i pezzi sulla stessa casa)}
translate I SearchBoardPawns {Pedoni (stesso materiale, tutti i pedoni sulla stessa casa)}
translate I SearchBoardFiles {Colonne (stesso materiale, tutti i pedoni sulle stesse colonne)}
translate I SearchBoardAny {Dovunque (stesso materiale, pedoni e pezzi dovunque)}
translate I SearchInRefDatabase { Cerca nel database }
translate I LookInVars {Cerca nelle varianti}

# Material search:
translate I MaterialSearch {Ricerca materiale}
translate I Material {Materiale}
translate I Patterns {Schema}
translate I Zero {Nessuno}
translate I Any {Tutti}
translate I CurrentBoard {Posizione corrente}
translate I CommonEndings {Finali comuni}
translate I CommonPatterns {Schemi comuni}
translate I MaterialDiff {Differenze di materiale}
translate I squares {sulle case di}
translate I SameColor {Stesso colore}
translate I OppColor {Colore opposto}
translate I Either {Entrambi}
translate I MoveNumberRange {Intervallo del numero di mosse}
translate I MatchForAtLeast {Confronta per almeno}
translate I HalfMoves {semi-mosse}

# Common endings in material search:
translate I EndingPawns {Finali di pedoni}
translate I EndingRookVsPawns {Torre contro pedoni}
translate I EndingRookPawnVsRook {Torre e pedone contro Torre}
translate I EndingRookPawnsVsRook {Torre e pedoni contro Torre}
translate I EndingRooks {Finali di Torre contro Torre}
translate I EndingRooksPassedA {Finali di Torre contro Torre con un pedone 'a' passato}
translate I EndingRooksDouble {Finali con due Torri per parte}
translate I EndingBishops {Finali di Alfiere contro Alfiere}
translate I EndingBishopVsKnight {Finali di Alfiere contro Cavallo}
translate I EndingKnights {Finali di Cavallo contro Cavallo}
translate I EndingQueens {Finali di Donna contro Donna}
translate I EndingQueenPawnVsQueen {Donna e pedone contro Donna}
translate I BishopPairVsKnightPair {Mediogioco con due Alfieri contro due Cavalli}

# Common patterns in material search:
translate I PatternWhiteIQP {Pedone di Donna bianco isolato}
translate I PatternWhiteIQPBreakE6 {Pedone di Donna bianco isolato: rottura d4-d5 contro e6}
translate I PatternWhiteIQPBreakC6 {Pedone di Donna bianco isolato: rottura d4-d5 contro c6}
translate I PatternBlackIQP {Pedone di Donna nero isolato}
translate I PatternWhiteBlackIQP {Pedoni di Donna bianco e nero isolati}
translate I PatternCoupleC3D4 {Coppia di pedoni bianchi isolati in c3 e d4}
translate I PatternHangingC5D5 {Pedoni neri sospesi in c5 e d5}
translate I PatternMaroczy {Centro Maroczy (con pedoni in c4 ed e4)}
translate I PatternRookSacC3 {Sacrificio di Torre in c3}
translate I PatternKc1Kg8 {O-O-O contro O-O (Rc1 contro Rg8)}
translate I PatternKg1Kc8 {O-O contro O-O-O (Rg1 contro Rc8)}
translate I PatternLightFian {Fianchetti sulle case chiare (Alfiere g2 contro Alfiere b7)}
translate I PatternDarkFian {Fianchetti sulle case scure (Alfiere b2 contro Alfiere g7)}
translate I PatternFourFian {Quattro fianchetti (Alfieri in b2,g2,b7,g7)}

# game saving:
translate I Today {Oggi}
translate I ClassifyGame {Classifica la partita}

# Setup position:
translate I EmptyBoard {Scacchiera vuota}
translate I InitialBoard {Posizione iniziale}
translate I SideToMove {Parte che muove}
translate I MoveNumber {Numero della mossa}
translate I Castling {Arrocco}
translate I EnPassantFile {Colonna presa en passant}
translate I ClearFen {Annulla FEN}
translate I PasteFen {Incolla FEN}
translate I SaveAndContinue {Salva e continua}
translate I DiscardChangesAndContinue {Scarta le modifiche\ne continua}
translate I GoBack {Indietro}

# Replace move dialog:
translate I ReplaceMove {Sostituisci la mossa}
translate I AddNewVar {Aggiunge una nuova variante}
translate I NewMainLine {Nuova linea principale}
translate I ReplaceMoveMessage {Esiste già una mossa qui.

Puoi sostituirla, rinunciando a tutte le mosse dopo di essa, o aggiungere la tua mossa come nuova variante.

(Puoi impedire di vedere questo messaggio in futuro ponendo ad off l'opzione "Domanda prima di sostiture le mosse" nel menu Opzioni:Mosse.)}

# Make database read-only dialog:
translate I ReadOnlyDialog {Se rendi questo database di sola lettura, nessuna modifica sarà permessa.
Nessuna partita può essere salvata o sostituita, e nessun identificatore di cancellazione può essere alterato.
Ogni ordinamento o classificazione ECO risulterà essere temporaneo.

Puoi facilmente rendere ancora scrivibile il database, chiudendolo e riaprendolo.

Desideri veramente rendere questo database di sola lettura?}

# Clear game dialog:
translate I ClearGameDialog {Questa partita è stata modificata.

Sei sicuro di voler continuare ed eliminare le modifiche effettuate?}

# Exit dialog:
translate I ExitDialog {Vuoi veramente uscire da Scid?}
translate I ExitUnsaved {Il database ha partite modificate e non salvate. Se esci adesso queste modifiche saranno perse.}

# Import window:
translate I PasteCurrentGame {Incolla la partita attuale}
translate I ImportHelp1 {Immetti o incolla una partita in formato PGN nella struttura precedente.}
translate I ImportHelp2 {Ogni errore di importazione di partita sarà visualizzato qui.}
translate I OverwriteExistingMoves {Sovrascrivere le mosse presenti?}

# ECO Browser:
translate I ECOAllSections {tutte le sezioni ECO}
translate I ECOSection {sezione ECO}
translate I ECOSummary {Sommario per}
translate I ECOFrequency {Frequenze nell'ambito di}

# Opening Report:
translate I OprepTitle {Rapporto di apertura}
translate I OprepReport {Rapporto}
translate I OprepGenerated {Generato da}
translate I OprepStatsHist {Statistica e storia}
translate I OprepStats {Statistica}
translate I OprepStatAll {tutte le partite della statistica}
translate I OprepStatBoth {Entrambe classificate}
translate I OprepStatSince {Da}
translate I OprepOldest {Le partite più vecchie}
translate I OprepNewest {Le partite più recenti}
translate I OprepPopular {Attuale popolarità}
translate I OprepFreqAll {Frequenza in tutti gli anni: }
translate I OprepFreq1   {Da  1 anno ad oggi:          }
translate I OprepFreq5   {Da  5 anni ad oggi:          }
translate I OprepFreq10  {Da 10 anni ad oggi:          }
translate I OprepEvery {una volta ogni %u partite}
translate I OprepUp {più %u%s da tutti gli anni}
translate I OprepDown {meno %u%s da tutti gli anni}
translate I OprepSame {nessuna variazione da tutti gli anni}
translate I OprepMostFrequent {Giocatori più frequenti}
translate I OprepMostFrequentOpponents {Avversari più frequenti}



translate I OprepRatingsPerf {Punteggi e performance}
translate I OprepAvgPerf {Punteggi medi e performance}
translate I OprepWRating {Punteggio del Bianco}
translate I OprepBRating {Punteggio del Nero}
translate I OprepWPerf {Performance del Bianco}
translate I OprepBPerf {Performance del Nero}
translate I OprepHighRating {Partite con il più alto punteggio medio}
translate I OprepTrends {Tendenze dei risultati}
translate I OprepResults {Risultato, lunghezza e frequenza}
translate I OprepLength {Lunghezza della partita}
translate I OprepFrequency {Frequenza}
translate I OprepWWins {Vittorie del Bianco: }
translate I OprepBWins {Vittorie del Nero: }
translate I OprepDraws {Patte:      }
translate I OprepWholeDB {intero database}
translate I OprepShortest {Vittorie più brevi}
translate I OprepMovesThemes {Mosse e temi}
translate I OprepMoveOrders {Ordini di mosse che raggiungono la posizione considerata}
translate I OprepMoveOrdersOne \
  {C'era solo un ordine di mosse che raggiungeva questa posizione:}
translate I OprepMoveOrdersAll \
  {C'erano %u ordini di mosse che raggiungevano questa posizione:}
translate I OprepMoveOrdersMany \
  {C'erano %u ordini di mosse che raggiungevano questa posizione. I primi %u sono:}
translate I OprepMovesFrom {Mosse dalla posizione del rapporto}
translate I OprepMostFrequentEcoCodes {Codici ECO più frequenti}
translate I OprepThemes {Temi posizionali}
translate I OprepThemeDescription {Frequenza dei temi nelle prime %u mosse di ogni partita}
translate I OprepThemeSameCastling {Arrocchi omogenei}
translate I OprepThemeOppCastling {Arrocchi eterogenei}
translate I OprepThemeNoCastling {Entrambi i Re non arroccati}
translate I OprepThemeKPawnStorm {Attacco di pedoni sull'ala di Re}
translate I OprepThemeQueenswap {Cambio delle Donne}
translate I OprepThemeWIQP {Pedone di Donna bianco isolato}
translate I OprepThemeBIQP {Pedone di Donna nero isolato}
translate I OprepThemeWP567 {Pedone bianco sulla 5/6/7a traversa}
translate I OprepThemeBP234 {Pedone nero sulla 2/3/4a traversa}
translate I OprepThemeOpenCDE {Colonne c/d/e aperte}
translate I OprepTheme1BishopPair {Una parte ha la coppia degli Alfieri}
translate I OprepEndgames {Finali}
translate I OprepReportGames {Partite del rapporto}
translate I OprepAllGames {Tutte le partite}
translate I OprepEndClass {Materiale alla fine di ogni partita}
translate I OprepTheoryTable {Tabella della teoria}
translate I OprepTableComment {Generato da %u partite più quotate.}
translate I OprepExtraMoves {Mosse extra note nella tabella della teoria}
translate I OprepMaxGames {Numero massimo di partite nella tabella della teoria}
translate I OprepViewHTML {Vedi HTML}
translate I OprepViewLaTeX {Vedi LaTeX}

# Player Report:
translate I PReportTitle {Report sul giocatore}
translate I PReportColorWhite {con i pezzi bianchi}
translate I PReportColorBlack {con i pezzi neri}
translate I PReportMoves {dopo %s}
translate I PReportOpenings {Aperture}
translate I PReportClipbase {Svuota la clipbase e copia le partite risultanti in essa}

# Piece Tracker window:
translate I TrackerSelectSingle {Il tasto sinistro del mouse seleziona questo pezzo.}
translate I TrackerSelectPair {Il tasto sinistro del mouse seleziona questo pezzo; il tasto destro seleziona il suo pari.}
translate I TrackerSelectPawn {Il tasto sinistro del mouse seleziona questo pedone; il tasto destro seleziona tutti gli 8 pedoni.}
translate I TrackerStat {Statistiche}
translate I TrackerGames {% di partite con mosse in questa casa}
translate I TrackerTime {% di tempo su ogni casa}
translate I TrackerMoves {Mosse}
translate I TrackerMovesStart {Inserisci il numero di mossa da dove iniziare il tracciamento.}
translate I TrackerMovesStop {Inserisci il numero di mossa dove finire il tracciamento.}

# Game selection dialogs:
translate I SelectAllGames {Tutte le partite nel database}
translate I SelectFilterGames {Solo le partite nel filtro}
translate I SelectTournamentGames {Solo le partite del torneo attuale}
translate I SelectOlderGames {Solo le partite più vecchie}

# Delete Twins window:
translate I TwinsNote {Per essere doppie, due partite devono avere almeno gli stessi due giocatori, e criteri che puoi definire sotto. Quando viene trovata una coppia di doppie, la partita più breve viene cancellata. Suggerimento: è meglio controllare per caratteri il database prima di cancellare le doppie, per migliorare la ricerca delle stesse.}
translate I TwinsCriteria {Criteri: le partite doppie devono avere...}
translate I TwinsWhich {Quali partite esaminare}
translate I TwinsColors {Stessi colori?}
translate I TwinsEvent {Stesso evento?}
translate I TwinsSite {Stesso sito?}
translate I TwinsRound {Stesso turno?}
translate I TwinsYear {Stesso anno?}
translate I TwinsMonth {Stesso mese?}
translate I TwinsDay {Stesso giorno?}
translate I TwinsResult {Stesso risultato?}
translate I TwinsECO {Stesso codice ECO?}
translate I TwinsMoves {Stesse mosse?}
translate I TwinsPlayers {Confronta i nomi dei giocatori:}
translate I TwinsPlayersExact {Stretta uguaglianza}
translate I TwinsPlayersPrefix {Solo le prima 4 lettere}
translate I TwinsWhen {Quando cancellare le partite doppie}
translate I TwinsSkipShort {Ignorare tutte le partite sotto le 5 mosse?}
translate I TwinsUndelete {Recupera prima tutte le partite?}
translate I TwinsSetFilter {Definisci il filtro a tutte le partite doppie cancellate?}
translate I TwinsComments {Mantieni sempre le partite con commenti?}
translate I TwinsVars {Mantieni sempre le partite con varianti?}
translate I TwinsDeleteWhich {Quale partita cancellare:}
translate I TwinsDeleteShorter {La partita più corta}
translate I TwinsDeleteOlder {La partita inserita prima nel database}
translate I TwinsDeleteNewer {La partita inserita dopo nel database}
translate I TwinsDelete {Cancella partite}

# Name editor window:
translate I NameEditType {Tipo di nome da editare}
translate I NameEditSelect {Partite da editare}
translate I NameEditReplace {Sostituisci}
translate I NameEditWith {con}
translate I NameEditMatches {Corrispondenze: digita da Ctrl+1 ad Ctrl+9 per scegliere}
translate I CheckGames {Controlla partite}
translate I CheckGamesWhich {Controlla partite}
translate I CheckAll {Tutte le partite}
translate I CheckSelectFilterGames {Solo le partite nel filtro}

# Classify window:
translate I Classify {Classifica}
translate I ClassifyWhich {Quali partite classificare ECO}
translate I ClassifyAll {Tutte le partite (sovrascrivi i vecchi codici ECO)}
translate I ClassifyYear {Tutte le partite giocate nell'ultimo anno}
translate I ClassifyMonth {Tutte le partite giocate nell'ultimo mese}
translate I ClassifyNew {Solo partite ancora senza codice ECO}
translate I ClassifyCodes {Codici ECO da usare}
translate I ClassifyBasic {Solo codici di base ("B12", ...)}
translate I ClassifyExtended {Estensioni Scid ("B12j", ...)}

# Compaction:
translate I NameFile {File di nomi}
translate I GameFile {File di partite}
translate I Names {Nomi}
translate I Unused {Non usati}
translate I SizeKb {Dimensioni (kB)}
translate I CurrentState {Stato attuale}
translate I AfterCompaction {Dopo la compattazione}
translate I CompactNames {Compatta il file di nomi}
translate I CompactGames {Compatta il file di partite}
translate I NoUnusedNames "Non ci sono nomi inutilizzati, il file dei nomi è già compresso."
translate I NoUnusedGames "Il file delle partite è già compresso."
translate I GameFileCompacted {Il file delle partite del database è stato compresso.}

# Sorting:
translate I SortCriteria {Criteri}
translate I AddCriteria {Aggiungi criteri}
translate I CommonSorts {Ordinamenti comuni}
translate I Sort {Ordina}

# Exporting:
translate I AddToExistingFile {Aggiungi le partite al file esistente?}
translate I ExportComments {Esporta i commenti?}
translate I ExportVariations {Esporta le varianti?}
translate I IndentComments {Indenta i commenti?}
translate I IndentVariations {Indenta le varianti?}
translate I ExportColumnStyle {Stile colonna (una mossa per riga)?}
translate I ExportSymbolStyle {Stile annotazione simbolica:}
translate I ExportStripMarks {Togliere i codici di casella/freccia dai commenti?}

# Goto game/move dialogs:
translate I LoadGameNumber {Inserire il numero della partita da caricare:}
translate I GotoMoveNumber {Vai alla mossa numero:}

# Copy games dialog:
translate I CopyGames {Copia partite}
translate I CopyConfirm {
 Vuoi veramente copiare
 le [::utils::thousands $nGamesToCopy] partite dal filtro
 del database "$fromName"
 nel database "$targetName"?
}
translate I CopyErr {Impossibile copiare le partite}
translate I CopyErrSource {il database di origine}
translate I CopyErrTarget {il database di destinazione}
translate I CopyErrNoGames {non contiene nessuna partita nel suo filtro}
translate I CopyErrReadOnly {è di sola lettura}
translate I CopyErrNotOpen {non è aperto}

# Colors:
translate I LightSquares {Case chiare}
translate I DarkSquares {Case scure}
translate I SelectedSquares {Case selezionate}
translate I SuggestedSquares {Case delle mosse suggerite}
translate I WhitePieces {Pezzi bianchi}
translate I BlackPieces {Pezzi neri}
translate I WhiteBorder {Bordi del Bianco}
translate I BlackBorder {Bordi del Nero}

# Novelty window:
translate I FindNovelty {Trova novità}
translate I Novelty {Novità}
translate I NoveltyInterrupt {Ricerca novità interrotta}
translate I NoveltyNone {Non è stata trovata nessuna novità per la partita corrente}
translate I NoveltyHelp {
Scid cercherà la prima mossa della partita corrente che raggiungerà una posizione non presente nel database selezionato o nel libro delle aperture.
}

# Sounds configuration:
translate I SoundsFolder {Cartella dei file audio}
translate I SoundsFolderHelp {La cartella deve contenere i file King.wav, a.wav, 1.wav, etc}
translate I SoundsAnnounceOptions {Opzioni per l'annuncio delle mosse}
translate I SoundsAnnounceNew {Annuncia l'esecuzione di una nuova mossa}
translate I SoundsAnnounceForward {Annuncia quando si va avanti di una mossa}
translate I SoundsAnnounceBack {Annuncia quando si va indietro di una mossa}
translate I SoundsSoundDisabled {Scid non è riuscito a trovare il pacchetto audio Snack all'avvio;\nSound è disabilitato.}

# Upgrading databases:
translate I Upgrading {Aggiornamento}
translate I ConfirmOpenNew {
Questo è un vecchio formato (Scid 3) di database che non può essere aperto da Scid 4, ma ne è stata appena creata una nuova versione nel formato (Scid 4).

Vuoi aprire la nuova versione del database?
}
translate I ConfirmUpgrade {
Questo è un vecchio formato (Scid 3) di database. Prima di poter utilizzare il database in Scid 4 va creata una nuova versione.

L'aggiornamento creerà una nuova versione del database.

Questo richiederà del tempo, ma è sufficiente farlo una volta sola. Puoi arrestare il processo se durerà a lungo.

Vuoi aggiornare il database ora?
}

# Recent files options:
translate I RecentFilesMenu {Numero di file recentemente utilizzati nel menu File}
translate I RecentFilesExtra {Numero di file recentemente utilizzati nel sottomenu extra}

# My Player Names options:
translate I MyPlayerNamesDescription {
Inserisci sotto una lista di giocatori preferiti, un nome per linea. Caratteri jolly (p. es. "?" per ogni singolo carattere, "*" per ogni sequenza di caratteri) sono accettati.

Ogni volta che viene caricata una partita con un giocatore nella lista, la finestra principale della scacchiera se necessario ruoterà e vi proporrà la partita dal punto di vista di quel giocatore.
}
translate I showblunderexists {mostra gli errori}
translate I showblundervalue {mostra il valore dell'errore}
translate I showscore {mostra il punteggio}
translate I coachgame {partita con maestro}
translate I configurecoachgame {configura la partita con il maestro}
translate I configuregame {Configurazione della partita}
translate I Phalanxengine {Motore Phalanx}
translate I Coachengine {Motore per il maestro}
translate I difficulty {difficoltà}
translate I hard {difficile}
translate I easy {facile}
translate I Playwith {Gioca con}
translate I white {Bianco}
translate I black {Nero}
translate I both {Entrambi}
translate I Play {Gioca}
translate I Noblunder {Nessun errore}
translate I blunder {errore}
translate I Noinfo {-- Nessuna info --}
translate I PhalanxOrTogaMissing {Phalanx o Toga non trovato}
translate I moveblunderthreshold {la mossa è un errore se la perdita è maggiore di}
translate I limitanalysis {limita il tempo di analisi del motore}
translate I seconds {secondi}
translate I Abort {Annulla}
translate I Resume {Riprendi}
translate I OutOfOpening {Fuori dall'apertura}
translate I NotFollowedLine {Non hai seguito la linea}
translate I DoYouWantContinue {Vuoi continuare?}
translate I CoachIsWatching {Il maestro sta osservando}
translate I Ponder {Analisi permanente}
translate I LimitELO {Limite Elo di forza}
translate I DubiousMovePlayedTakeBack {Mossa dubbia, vuoi ripetere?}
translate I WeakMovePlayedTakeBack {Mossa debole, vuoi ripetere?}
translate I BadMovePlayedTakeBack {Mossa cattiva, vuoi ripetere?}
translate I Iresign {Abbandono}
translate I yourmoveisnotgood {La tua mossa non è buona}
translate I EndOfVar {Fine della variante}
translate I Openingtrainer {Allenatore delle aperture}
translate I DisplayCM {Mostra le mosse candidate}
translate I DisplayCMValue {Mostra il valore delle mosse candidate}
translate I DisplayOpeningStats {Mostra le statistiche}
translate I ShowReport {Mostra il report}
translate I NumberOfGoodMovesPlayed {buone mosse giocate}
translate I NumberOfDubiousMovesPlayed {mosse dubbie giocate}
translate I NumberOfMovesPlayedNotInRepertoire {mosse giocate non presenti nel repertorio}
translate I NumberOfTimesPositionEncountered {numero di volte che si è incontrata la posizione}
translate I PlayerBestMove  {Permetti solo le mosse migliori}
translate I OpponentBestMove {L'avversario gioca le mosse migliori}
translate I OnlyFlaggedLines {Solo linee segnate}
translate I resetStats {Azzera le statistiche}
translate I Repertoiretrainingconfiguration {Configurazione dell'allenatore del repertorio}
translate I Loadingrepertoire {Leggo il repertorio}
translate I Movesloaded {Mosse lette}
translate I Repertoirenotfound {Repertorio non trovato}
translate I Openfirstrepertoirewithtype {Apri prima un database come repertorio con tipo/icona a destra}
translate I Movenotinrepertoire {Mossa non presente nel repertorio}
translate I PositionsInRepertoire {Posizioni nel repertorio}
translate I PositionsNotPlayed {Posizioni non giocate}
translate I PositionsPlayed {Posizioni giocate}
translate I Success {Successo}
translate I DubiousMoves {Mossa dubbia}
translate I OutOfRepertoire {Fuori dal repertorio}
translate I ConfigureTactics {Configura gli esercizi di tattica}
translate I ResetScores {Azzera i punteggi}
translate I LoadingBase {Leggo il database}
translate I Tactics {Tattica}
translate I ShowSolution {Mostra la soluzione}
translate I NextExercise {Esercizio successivo}
translate I PrevExercise {Esercizio precedente}
translate I StopTraining {Interrompi l'allenamento}
translate I Next {Seguente}
translate I ResettingScore {Azzero il punteggio}
translate I LoadingGame {Leggo la partita}
translate I MateFound {Scacco matto trovato}
translate I BestSolutionNotFound {La migliore soluzione NON è stata trovata!}
translate I MateNotFound {Scacco matto non trovato}
translate I ShorterMateExists {Esiste uno scacco matto più breve}
translate I ScorePlayed {Punteggio giocato}
translate I Expected {atteso}
translate I ChooseTrainingBase {Scegli il database di allenamento}
translate I Thinking {Sto analizzando}
translate I AnalyzeDone {Analisi eseguita}
translate I WinWonGame {Vinci le partite vinte}
translate I Lines {Linee}
translate I ConfigureUCIengine {Configura il motore UCI}
translate I SpecificOpening {Apertura Specifica}
translate I StartNewGame {Inizia una nuova partita}
translate I FixedLevel {Livello prefissato}
translate I Opening {Apertura}
translate I RandomLevel {Livello casuale}
translate I StartFromCurrentPosition {Parti dalla posizione attuale}
translate I FixedDepth {Profondità fissa}
translate I Nodes {Nodi}
translate I Depth {Profondità}

translate I Time {Tempo}
translate I SecondsPerMove {Secondi per mossa}
translate I Engine {Motore}
translate I TimeMode {Cadenza di gioco}
translate I TimeBonus {Tempo + bonus}
translate I TimeMin {min}
translate I TimeSec {sec}
translate I AllExercisesDone {Tutti gli esercizi eseguiti}
translate I MoveOutOfBook {Mossa fuori dal libro}
translate I LastBookMove {Ultima mossa del libro}
translate I AnnotateSeveralGames {Annota più partite,\ndall'attuale a:}
translate I FindOpeningErrors {Trova errori in apertura}
translate I MarkTacticalExercises {Segna gli esercizi tattici}
translate I UseBook {Usa il libro}
translate I MultiPV {Varianti multiple}
translate I Hash {Memoria hash}
translate I OwnBook {Use il libro del motore}
translate I BookFile {Libro di aperture}
translate I AnnotateVariations {Annota le varianti}
translate I ShortAnnotations {Annotazione breve}
translate I addAnnotatorTag {Aggiungi il tag dell'annotazione}
translate I AddScoreToShortAnnotations {Aggiungi il punteggio alle annotazioni brevi}
translate I Export {Esporta}
translate I BookPartiallyLoaded {Libro caricato parzialmente}
translate I Calvar {Calcolo delle varianti}
translate I ConfigureCalvar {Configurazione}
translate I Reti {Reti}
translate I English {Inglese}
translate I d4Nf6Miscellaneous {1.d4 Cf6 varie}
translate I Trompowsky {Trompowsky}
translate I Budapest {Budapest}
translate I OldIndian {Vecchia Indiana}
translate I BenkoGambit {Gambetto Benko}
translate I ModernBenoni {Moderna Benoni}
translate I DutchDefence {Difesa Olandese}
translate I Scandinavian {Scandinava}
translate I AlekhineDefence {Difesa Alekhine}
translate I Pirc {Pirc}
translate I CaroKann {Caro-Kann}
translate I CaroKannAdvance {Caro-Kann variante di spinta}
translate I Sicilian {Siciliana}
translate I SicilianAlapin {Siciliana Alapin}
translate I SicilianClosed {Siciliana chiusa}
translate I SicilianRauzer {Siciliana Rauzer}
translate I SicilianDragon {Siciliana Dragone}
translate I SicilianScheveningen {Siciliana Scheveningen}
translate I SicilianNajdorf {Siciliana Najdorf}
translate I OpenGame {Partita aperta}
translate I Vienna {Vienna}
translate I KingsGambit {Gambetto di Re}
translate I RussianGame {Partita Russa}
translate I ItalianTwoKnights {Italiana/Due Cavalli}
translate I Spanish {Spagnola}
translate I SpanishExchange {Spagnola variante del cambio}
translate I SpanishOpen {Spagnola aperta}
translate I SpanishClosed {Spagnola chiusa}
translate I FrenchDefence {Difesa Francese}
translate I FrenchAdvance {Francese variante di spinta}
translate I FrenchTarrasch {Francese Tarrasch}
translate I FrenchWinawer {Francese Winawer}
translate I FrenchExchange {Francese variante del cambio}
translate I QueensPawn {Pedone di Donna}
translate I Slav {Slava}
translate I QGA {Gambetto di Donna accettato}
translate I QGD {Gambetto di Donna rifiutato}
translate I QGDExchange {Gambetto di Donna rifiutato variante del cambio}
translate I SemiSlav {Semi-Slava}
translate I QGDwithBg5 {Gambetto di Donna rifiutato con Ag5}
translate I QGDOrthodox {Gambetto di Donna rifiutato Ortodossa}
translate I Grunfeld {Grünfeld}
translate I GrunfeldExchange {Grünfeld variante del cambio}
translate I GrunfeldRussian {Grünfeld sistema russo}
translate I Catalan {Catalana}
translate I CatalanOpen {Catalana aperta}
translate I CatalanClosed {Catalana chiusa}
translate I QueensIndian {Indiana di Donna}
translate I NimzoIndian {Nimzo-Indiana}
translate I NimzoIndianClassical {Nimzo-Indiana variante classica}
translate I NimzoIndianRubinstein {Nimzo-Indiana Rubinstein}
translate I KingsIndian {Indiana di Re}
translate I KingsIndianSamisch {Indiana di Re Sämisch}
translate I KingsIndianMainLine {Indiana di Re linea principale}
translate I ConfigureFics {Configura FICS}
translate I FICSGuest {Entra come guest}
translate I FICSServerPort {Porta server}
translate I FICSServerAddress {Indirizzo IP}
translate I FICSRefresh {Aggiorna}
translate I FICSTimesealPort {Porta timeseal}
translate I FICSSilence {Silenzio}
translate I FICSOffers {Offerte}
translate I FICSConsole {Console}
translate I FICSGames {Partite}
translate I FICSUnobserve {Smetti di osservare la partita}
translate I FICSProfile {Visualizza la tua storia e il tuo profilo}
translate I FICSRelayedGames {Partite ritrasmesse}
translate I FICSFindOpponent {Trova avversario}
translate I FICSTakeback {Ripeti la mossa}
translate I FICSTakeback2 {Ripeti due mosse}
translate I FICSInitTime {Tempo iniziale (min)}
translate I FICSIncrement {Incremento (sec)}
translate I FICSRatedGame {Partita valida per il rating}
translate I FICSAutoColour {Automatico}
translate I FICSManualConfirm {Conferma manualmente}
translate I FICSFilterFormula {Filtra con la formula}
translate I FICSIssueSeek {Esegui ricerca}
translate I FICSChallenge {Sfida}
translate I FICSAccept {accetta}
translate I FICSDecline {rifiuta}
translate I FICSColour {Colore}
translate I FICSSend {invia}
translate I FICSConnect {Connetti}
translate I FICSdefaultuservars {Usa le variabili predefinite}
translate I FICSObserveconfirm {Vuoi osservare la partita}
translate I FICSpremove {Abilita premove}
translate I GameReview {Rivedi partita}
translate I GameReviewTimeExtended {Tempo esteso}
translate I GameReviewMargin {Margine di errore}
translate I GameReviewAutoContinue {Continua quando la mossa è corretta}
translate I GameReviewReCalculate {Usa tempo esteso}
translate I GameReviewAnalyzingMovePlayedDuringTheGame {Analisi della mossa giocata durante la partita}
translate I GameReviewAnalyzingThePosition {Analisi della posizione}
translate I GameReviewEnterYourMove {Immetti la tua mossa}
translate I GameReviewCheckingYourMove {Controlla la tua mossa}
translate I GameReviewYourMoveWasAnalyzed {La tua mossa è stata analizzata}
translate I GameReviewYouPlayedSameMove {Hai giocato la stessa mossa della partita}
translate I GameReviewScoreOfYourMove {Punteggio della tua mossa}
translate I GameReviewGameMoveScore {Punteggio della mossa della partita}
translate I GameReviewEngineScore {Punteggio del motore}
translate I GameReviewYouPlayedLikeTheEngine {Hai giocato come il motore}
translate I GameReviewNotEngineMoveButGoodMove {Non è la mossa del motore ma è comunque buona}
translate I GameReviewMoveNotGood {Questa mossa è debole, il punteggio è}
translate I GameReviewMovesPlayedLike {Mosse giocate come}
translate I GameReviewMovesPlayedEngine {Mosse giocate come il motore}
translate I CCDlgConfigureWindowTitle {Configura il gioco per corrispondenza}
translate I CCDlgCGeneraloptions {Opzioni generali}
translate I CCDlgDefaultDB {Database predefinito:}
translate I CCDlgInbox {Posta in arrivo (percorso):}
translate I CCDlgOutbox {Posta in uscita (percorso):}
translate I CCDlgXfcc {Configurazione di Xfcc:}
translate I CCDlgExternalProtocol {Gestore del protocollo esterno (p. es. Xfcc)}
translate I CCDlgFetchTool {Strumento per la scansione:}
translate I CCDlgSendTool {Strumento per l'invio:}
translate I CCDlgEmailCommunication {Comunicazione via email}
translate I CCDlgMailPrg {Programma di posta:}
translate I CCDlgBCCAddr {Indirizzo (B)CC:}
translate I CCDlgMailerMode {Modo:}
translate I CCDlgThunderbirdEg {p. es. Thunderbird, Icedove...}
translate I CCDlgMailUrlEg {p. es. Evolution}
translate I CCDlgClawsEg {p. es Sylpheed Claws}
translate I CCDlgmailxEg {p. es. mailx, mutt, nail...}
translate I CCDlgAttachementPar {Parametro per l'allegato:}
translate I CCDlgInternalXfcc {Usa il supporto interno per Xfcc}
translate I CCDlgConfirmXfcc {Conferma le mosse}
translate I CCDlgSubjectPar {Parametro per l'oggetto:}
translate I CCDlgDeleteBoxes {Svuota le caselle di posta}
translate I CCDlgDeleteBoxesText {Vuoi veramente svuotare le caselle di posta in entrata e uscita per il gioco per corrispondenza? Questo richiede una nuova sincronizzazione per mostrare l'ultimo stato delle tue partite}
translate I CCDlgConfirmMove {Conferma la mossa}
translate I CCDlgConfirmMoveText {Se confermi, la mossa e il commento seguenti saranno inviati al server:}
translate I CCDlgDBGameToLong {Linea principale non valida}
translate I CCDlgDBGameToLongError {La linea principale nel tuo database è più lunga della partita nella tua Posta in arrivo. Se la Posta in arrivo contiene la partita corrente, dopo una sincronizzazione, alcune mosse sono state aggiunte alla linea principale nel database erroneamente.\nIn questo caso, accorcia la linea principale (al massimo) alla mossa\n}
translate I CCDlgStartEmail {Avvia una nuova partita tramite email}
translate I CCDlgYourName {Il tuo nome:}
translate I CCDlgYourMail {Il tuo indirizzo email:}
translate I CCDlgOpponentName {Nome dell'avversario:}
translate I CCDlgOpponentMail {Indirizzo email dell'avversario:}
translate I CCDlgGameID {ID (univoco) della partita:}
translate I CCDlgTitNoOutbox {Scid: Posta in uscita del gioco per corrispondenza}
translate I CCDlgTitNoInbox {Scid: Posta in arrivo del gioco per corrispondenza}
translate I CCDlgTitNoGames {Scid: Nessuna partita per corrispondenza}
translate I CCErrInboxDir {Directory della posta in arrivo:}
translate I CCErrOutboxDir {Directory della posta in uscita:}
translate I CCErrDirNotUsable {non esiste o non è accessibile\nControlla e correggi le impostazioni.}
translate I CCErrNoGames {non contiene partite!\nEffettua prima una scansione.}
translate I CCDlgTitNoCCDB {Scid: Nessun database per il gioco per corrispondenza}
translate I CCErrNoCCDB {Non è aperto nessun database di tipo "gioco per corrispondenza". Aprine uno usando le funzioni per il gioco per corrispondenza.}
translate I CCFetchBtn {Recupera le partite dal server ed elabora la posta in arrivo}
translate I CCPrevBtn {Vai alla partita precedente}
translate I CCNextBtn {Vai alla partita successiva}
translate I CCSendBtn {Invia mossa}
translate I CCEmptyBtn {Svuota le caselle di posta}
translate I CCHelpBtn {Aiuto sulle icone e gli indicatori.\nPer Aiuto generale premi F1!}
translate I CCDlgServerName {Nome del server:}
translate I CCDlgLoginName  {Login:}
translate I CCDlgPassword   {Password:}
translate I CCDlgShowPassword {Mostra password}
translate I CCDlgURL        {Xfcc-URL:}
translate I CCDlgRatingType {Tipo di rating:}
translate I CCDlgDuplicateGame {ID della partita non unico}
translate I CCDlgDuplicateGameError {Questa partita esiste più di una volta nel tuo database. Elimina tutti i duplicati e compatta il file di partite (File/Gestione/Compatta database).}
translate I CCDlgSortOption {Ordinamento:}
translate I CCDlgListOnlyOwnMove {Solo le partite nelle quali devo muovere}
translate I CCOrderClassicTxt {Sito, Evento, Turno, Risultato, Bianco, Nero}
translate I CCOrderMyTimeTxt {Mio orologio}
translate I CCOrderTimePerMoveTxt {Tempo per mossa fino al prossimo controllo}
translate I CCOrderStartDate {Data di inizio}
translate I CCOrderOppTimeTxt {Orologio dell'avversario}
translate I CCDlgConfigRelay {Configura osservazioni ICCF}
translate I CCDlgConfigRelayHelp {Vai alla pagina delle partite su http://www.iccf-webchess.com e visualizza la partita da osservare.  Se vedi la scacchiera, copia l'URL dal tuo browser nella lista qui sotto. Un solo URL per linea!\nEsempio: http://www.iccf-webchess.com/MakeAMove.aspx?id=266452}
translate I ExtHWConfigConnection {Configura un dispositivo esterno}
translate I ExtHWPort {Porta}
translate I ExtHWEngineCmd {Comando del motore}
translate I ExtHWEngineParam {Parametro del motore}
translate I ExtHWShowButton {Mostra pulsante}
translate I ExtHWHardware {Dispositivo}
translate I ExtHWNovag {Novag Citrine}
translate I ExtHWInputEngine {Motore di input}
translate I ExtHWNoBoard {Nessuna scacchiera}
translate I NovagReferee {Arbitro}
translate I IEConsole {Console del motore di input}
translate I IESending {Mosse inviate per}
translate I IESynchronise {Sincronizza}
translate I IERotate  {Ruota}
translate I IEUnableToStart {Impossibile avviare il motore di input:}
translate I DoneWithPosition {Posizione conclusa}
translate I Board {Scacchiera}
translate I showGameInfo {Mostra informazioni partita}
translate I autoResizeBoard {Ridimensionamento automatico della scacchiera}
translate I DockTop {Sposta in alto}
translate I DockBottom {Sposta in basso}
translate I DockLeft {Sposta a sinistra}
translate I DockRight {Sposta a destra}
translate I Undock {Sgancia}
translate I ChangeIcon {Cambia icona...}
# ====== TODO To be translated ======
translate I NewGameListWindow {New Game List Window}
# ====== TODO To be translated ======
translate I LoadatStartup {Load at startup}
# ====== TODO To be translated ======
translate I ShowHideDB {Show/Hide databases}
# ====== TODO To be translated ======
translate I ChangeFilter {Change filter}
# ====== TODO To be translated ======
translate I ChangeLayout {Load/Save/Change sorting criteria and column layout}
# ====== TODO To be translated ======
translate I ShowHideStatistic {Show/Hide statistics}
# ====== TODO To be translated ======
translate I BoardFilter {Show only games that matches the current board position}
# ====== TODO To be translated ======
translate I CopyGameTo {Copy Game to}
# ====== TODO To be translated ======
translate I FindBar {Find Bar}
# ====== TODO To be translated ======
translate I FindCurrentGame {Find current game}
# ====== TODO To be translated ======
translate I DeleteGame {Delete game}
# ====== TODO To be translated ======
translate I UndeleteGame {Undelete game}
# ====== TODO To be translated ======
translate I ResetSort {Reset sort}
# ====== TODO To be translated ======
translate I ConvertNullMove {Convert null moves to comments}
# ====== TODO To be translated ======
translate I SetupBoard {Setup Board}
# ====== TODO To be translated ======
translate I Rotate {Rotate}
# ====== TODO To be translated ======
translate I SwitchColors {Switch colors}
# ====== TODO To be translated ======
translate I FlipBoard {Flip Board}
# ====== TODO To be translated ======
translate I ImportPGN {Import PGN game}
# ====== TODO To be translated ======
translate I ImportingFiles {Importing PGN files in}
# ====== TODO To be translated ======
translate I ImportingFrom {Importing from}
# ====== TODO To be translated ======
translate I ImportingIn {Import games in}
# ====== TODO To be translated ======
translate I UseLastTag {Use last\ngame's tags}
# ====== TODO To be translated ======
translate I Random {Random}
# ====== TODO To be translated ======
translate I BackToMainline {Go back to mainline}
# ====== TODO To be translated ======
translate I LeaveVariant {Leave variant}
# ====== TODO To be translated ======
translate I Autoplay {Autoplay}
# ====== TODO To be translated ======
translate I ShowHideCoords {Show/Hide Coord.}
# ====== TODO To be translated ======
translate I ShowHideMaterial {Show/Hide Material}
# ====== TODO To be translated ======
translate I FullScreen {Full Screen}
# ====== TODO To be translated ======
translate I FilterStatistic {Filter Statistics}
# ====== TODO To be translated ======
translate I MakeCorrections {Make Corrections}
# ====== TODO To be translated ======
translate I Surnames {Surnames}
# ====== TODO To be translated ======
translate I Ambiguous {Ambiguous}
translate I OptionsToolbar "Barra degli strumenti"
translate I OptionsBoard "Scacchiera"
# ====== TODO To be translated ======
translate I OptionsMenuColor "Menu colors"
# ====== TODO To be translated ======
translate I OptionsBoardSize "Dimensione della scacchiera"
# ====== TODO To be translated ======
translate I OptionsBoardPieces "Stile dei pezzi"
# ====== TODO To be translated ======
translate I OptionsInternationalization "Internationalization"
# ====== TODO To be translated ======
translate I MenuColorForeground "Textcolor"
# ====== TODO To be translated ======
translate I MenuColorBackground "Background"
# ====== TODO To be translated ======
translate I MenuColorSelect "Select"
# ====== TODO To be translated ======
translate I OptionsTablebaseDir "Select up to 4 table base folders:"
}

############################################################
#
# Italian tips section:

set tips(I) {
  {
    SCID ha oltre 30 <a Index>pagine di aiuto</a> e in molte finestre, 
    premendo il tasto funzione <b>F1</b>, apparirà una pagina di aiuto
    relativa alla finestra.
  }
  {
    Molte finestre di SCID (p. es. <a Switcher>selezione di database</a>,
    ecc.) hanno un menu collegato al pulsante destro del mouse. Prova a
    premerlo in ogni finestra per vedere se c'è e quali funzionalità permette.
  }
  {
    SCID ti consente più di una modalità per inserire le mosse, scegli
    quale preferisci. Puoi utilizzare il mouse (con o senza il suggerimento
    della mossa) o la tastiera (con o senza il completamento della mossa).
    Leggi la pagina di aiuto <a Moves>Inserire le mosse</a> per maggiori dettagli. 
  }
  {
    Se utilizzi pochi database che apri spesso, aggiungi un
    <a Bookmarks>segnalibro</a> a ognuno di essi. Potrai poi aprirlo più
    velocemente con il menu Segnalibri.
  }
  {
    Puoi visualizzare tutte le mosse della partita caricata (con ogni
    variante e/o commento) utilizzando la <a PGN>finestra PGN</a>.
    Nella finestra PGN puoi andare a ogni mossa premendo il tasto sinistro
    del mouse quando sei sulla mossa oppure visualizzare la posizione
    premendo il tasto centrale/destro del mouse.
  }
  {
    Puoi copiare partite da un database ad un altro usando la tecnica del
    drag and drop utilizzando il tasto sinistro del mouse nella finestra 
    <a Switcher>Selezione di database</a>.
  }
  {
    SCID può aprire file PGN, anche se sono compressi con Gzip (con il
    suffisso .gz). I file PGN aperti sono di sola lettura ma li puoi
    modificare creando un nuovo database SCID e copiando
    il file PGN in esso utilizzando la <a Switcher>selezione di database</a>.
  }
  {
    Se hai un grosso database e usi spesso la finestra di <a Tree>albero</a>,
    è il caso di utilizzare <b>Riempi cache</b> dal menu File della finestra.
    Ciò memorizza le statistiche dell'albero delle
    più comuni posizioni di apertura, rendendo più veloci gli accessi
    all'albero per quel database.
  }
  {
    La finestra <a Tree>albero</a> visualizza tutte le mosse giocate dalla
    posizione caricata, ma se vuoi vedere tutti gli ordini di mosse che
    raggiungono la posizione devi generare un <a OpReport>rapporto di apertura</a>.
  }
  {
    Nella finestra <a GameList>lista di partite</a>, premendo il tasto sinistro
    o desto del mouse sulla testata di ogni colonna, puoi modificarne la
    larghezza.
  }
  {
    Con la finestra <a PInfo>informazioni sul giocatore</a> (che apri
    cliccando sui nomi dei giocatori nell'area informazioni sotto
    la scacchiera) puoi facilmente, utilizzando il
    <a Searches Filter>filtro</a>, ottenere tutte le partite di un certo
    giocatore con i suoi risultati cliccando su ogni campo che appare
    in <red>rosso</red>.
  }
  {
    Quando studi un'apertura può essere utile effettuare una
    <a Searches Board>ricerca su scacchiera</a> con le opzioni
    <b>Pedoni</b> oppure <b>Colonne</b> sulla posizione che ti
    interessa, in maniera da evidenziarti se altre aperture 
    raggiungono la stessa struttura di pedoni.
  }
  {
    Nell'area di informazioni sulla partita (sotto la scacchiera) puoi
    premere il tasto destro del mouse e visualizzare un menu per 
    personalizzarla. Per esempio, puoi nascondere la prossima mossa e ciò
    è utile quando visualizzi una partita per allenamento e vuoi trovare
    da solo le mosse successive. 
  }
  {
    Se effettui spesso la <a Maintenance>manutenzione</a> di un database
    di grosse dimensioni, puoi effettuare tutte le attività in un colpo
    solo utilizzando il <a Maintenance Cleaner>manutentore globale</a>.
  }
  {
    Se hai un grosso database dove in molte partite è stato utilizzato il campo 
    Data evento e tu vuoi le partite in ordine di data, utilizza
    l'<a Sorting>ordinamento</a> per Data evento/Evento invece di Data/Evento
    cosicché otterrai insieme le partite dello stesso torneo anche se hanno
    date diverse (partendo dal presupposto che abbiano la stessa data evento).
  }
  {
    Prima di <a Maintenance Twins>cancellare le partite doppie</a>, è meglio
    utilizzare il <a Maintenance Spellcheck>controllo ortografico</a> sul tuo
    database, permettendo a SCID di identificare un numero maggiore di
    partite doppie e contrassegnarle per la cancellazione.
  }
  {
    Gli <a Flags>identificatori</a>  sono utili per evidenziare partite con
    caratteristiche particolari da ricercare successivamente (p. es. struttura
    di pedoni, tattica, ecc.). Puoi ricercare le partite contrassegnate
    utilizzando la <a Searches Header>ricerca per intestazione</a>.
  }
  {
    Se stai visualizzando una partita e vuoi provare qualche altra mossa
    senza modificare la partita, puoi entrare nella modalità Prova (con lo
    scorciatoia <b>Ctrl+space</b> oppure cliccando l'apposita icona).
    Quando hai finito, puoi deselezionare la modalità Prova ritornando
    alla partita originale.
  }
  {
    Per cercare le partite maggiormente rilevanti (quelle con i giocatori
    con l'Elo più alto) in una posizione, apri la finestra <a Tree>albero</a>
    e da quella apri la lista delle migliori partite.
    Puoi personalizzare la lista ottenendo solo le partite con un
    certo risultato che ti interessa.
  }
  {
    Un buon modo per studiare le aperture utilizzando un grosso database
    è attivare la modalità esercizio nella finestra <a Tree>albero</a> e
    giocare contro il database per vedere quali linee appaiono spesso.
  }
  {
    Se hai due database aperti e vuoi visualizzare le statistiche
    dell'<a Tree>albero</a> del primo database mentre esamini una
    partita del secondo database premi il pulsante <b>Blocca</b> nella
    finestra albero per bloccare il primo database ed allora selezionare
    il secondo database.
  }
  {
    La finestra <a Tmt>Trova torneo</a> non è utile solo per trovare un
    certo torneo, ma può essere usato anche per vedere quali tornei
    ha giocato recentemente un certo giocatore oppure vedere i tornei
    più importanti giocati in una certa nazione.
  }
  {
    La finestra <a Searches Material>Materiale/schema</a> ti propone
    alcune possibilità, fra le più comuni, facilitandoti ricerche
    per aperture o tipi di mediogioco a fine di studio.
  }
  {
    Quando effettui una ricerca per una particolare posizione
    nella finestra <a Searches Material>Materiale/schema</a> è
    spesso utile restringere la ricerca a partite che permangono
    nella posizione voluta almeno qualche semi-mossa, eliminando
    partite che raggiungono la posizione voluta solo una volta. 
  }
  {
    Se hai un importante database e non vuoi correre il rischio
    di effettuare modifiche inopportune, seleziona <b>Sola lettura...</b>
    dal menu <b>File</b> dopo averlo aperto, oppure cambia gli
    attributi a sola lettura.
  }
  {
    Se usi XBoard oppure WinBoard (o altri programmi scacchistici
    che possono copiare sulla clipboard la posizione attuale in notazione
    standard FEN) e vuoi copiare la tua posizione su SCID, il modo più
    facile e veloce è selezionare <b>Copia posizione</b> dal menu File
    in XBoard/WinBoard e poi <b>Incolla come posizione di partenza</b> dal
    menu Modifica in SCID.  
  }
  {
    In una <a Searches Header>ricerca per intestazione</a>,
    giocatore/evento/luogo/turno non sono sensibili alle maiuscole e
    vengono cercate anche all'interno dei nomi. Puoi scegliere di
    fare una ricerca sensibile alle maiuscole (dove "?" = ogni singolo
    carattere e "*" = zero o più caratteri) digitando la stringa che
    vuoi trovare fra virgolette. Per esempio, digitando "*BEL" (con
    le virgolette) nel campo luogo otterrai tutte le partite giocate
    in Belgio ma non a Belgrado.
  }
  {
    Se vuoi correggere una mossa in una partita senza perdere le mosse
    giocate successivamente, apri la finestra <a Import>Importa</a>,
    premi l'icona <b>Incolla la partita attuale</b>, edita la mossa
    sbagliata e poi premi il pulsante <b>Importa</b>.
  }
  {
    Se hai caricato un file di classificazione ECO, puoi andare alla
    posizione classificata più profonda della partita correntemente
    caricata con <b>Identifica apertura</b> nel menu <b>Partita</b>
    (scorciatoia: Ctrl+Shift+D).
  }
  {
    Se vuoi verificare le dimensioni di un file o la sua data di ultima
    modifica prima di aprirlo, utilizza <a Finder>Trova file</a> per
    aprirlo.
  }
  {
    Un file di <a Repertoire>repertorio</a> è un grande strumento per
    monitorare le tue aperture preferite e per trovare le partite nelle
    quali queste linee sono state giocate. Dopo aver memorizzato le tue
    aperture in un file di repertorio puoi effettuare ricerche tutte le
    volte che acquisisci nuovi file di partite e visualizzare le partite
    che ti interessano.
  }
  {
    Un <a OpReport>rapporto di apertura</a> è molto utile per apprendere
    ulteriori informazioni su una particolare posizione. Puoi vedere il
    punteggio percentuale che raggiunge, se porta a rapide patte, i temi
    posizionali più comuni e molto altro.
  }
  {
    Puoi aggiungere i più comuni simboli di annotazione (!, !?, +=, ecc)
    alla posizione attualmente caricata con scorciatoie da tastiera senza
    necessità di utilizzare l'<a Comment>editor di commenti</a>.
    Per esempio, digitando "!" seguito dal tasto Invio,
    aggiungerete il simbolo "!". Guarda la pagina di aiuto
    <a Moves>Inserire le mosse</a> per maggiori informazioni.
  }
  {
    Se stai visualizzando un'apertura in un database con l'<a Tree>albero</a>
    puoi facilmente vedere i risultati percentuali dell'apertura che stai
    guardando (recenti e fra giocatori di Elo alto) aprendo la finestra
    Statistiche (scorciatoia: Ctrl+I).
  }
  {
    Puoi cambiare le dimensioni della scacchiera premendo i tasti
    <b>Ctrl</b> e <b>Shift</b>, e regolare le dimensioni premendo i tasti
    freccia <b>Sinistra</b> o <b>Destra</b> .
  }
  {
    Dopo una <a Searches>ricerca</a>, puoi facilmente visualizzare tutte le
    partite trovate premendo <b>Ctrl</b> e con i tasti <b>Su</b> o <b>Giù</b>
    caricare la precedente o successiva partita del <a Searches Filter>filtro</a>.
  }
}


# end of italian.tcl
