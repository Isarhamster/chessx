### francais.tcl:
#
# French language support for Scid.
# Translations contributed by:
# Gilles Maire, Vincent Serisier, Joel Rivat, Pascal Heisel,
# and Besoa Rabenasolo.

proc setLanguage_F {} {

# File menu:
menuText F File "Fichier" 0
menuText F FileNew "Nouvelle..." 0 {Créer une nouvelle base de données Scid}
menuText F FileOpen "Ouvrir..." 0 {Ouvrir une base de données Scid existante}
menuText F FileClose "Fermer" 0 {Fermer la base de données Scid active}
menuText F FileFinder "Explorer..." 0 {Ouvrir la fenêtre de recherche de fichier}
menuText F FileBookmarks "Signets" 0 {Menu des signets (raccourci: Ctrl+B)}
menuText F FileBookmarksAdd "Ajouter un signet" 0 \
  {Poser un signet pour la position et partie courante}
menuText F FileBookmarksFile "Classer un signet" 0 \
  {Classer un signet pour la position et partie courante}
menuText F FileBookmarksEdit "Modifier les signets..." 0 \
  {Modifier les menus de signets}
menuText F FileBookmarksList "Afficher les dossiers comme une simple liste" 0 \
  {Afficher les dossiers comme une simple liste et non comme des sous-menus}
menuText F FileBookmarksSub "Afficher les dossiers comme des sous-menus" 0 \
  {Afficher les dossiers comme des sous-menus et non comme une simple liste}
menuText F FileMaint "Maintenance" 0 {Maintenance de la base}
menuText F FileMaintWin "Fenêtre de maintenance" 0 \
  {Ouvrir/Fermer la fenêtre de maintenance}
menuText F FileMaintCompact "Compacter la base..." 0 \
  {Compacter la base: supprimer les parties effacées et les noms non utilisés}
menuText F FileMaintClass "Classer les parties suivant ECO" 2 \
  {Recalculer le code ECO de toutes les parties}
menuText F FileMaintSort "Trier la base..." 0 {Trier toutes les parties de la base}
menuText F FileMaintDelete "Purger les doublons..." 0 \
  {Trouver les parties en doublons et les marquer pour l'effacement}
menuText F FileMaintTwin "Trouver les doublons" 0 \
  {Ouvrir/Actualiser la fenêtre de recherche de doublons}
menuText F FileMaintName "Orthographe des noms" 0 \
  {correction orthographiques des noms et du classement Elo}
menuText F FileMaintNameEditor "Éditeur de noms" 1 {Ouvrir/Fermer l'éditeur de noms}
menuText F FileMaintNamePlayer "Corriger les noms de joueurs..." 21 \
  {Vérifier l'orthographe des noms de joueurs}
menuText F FileMaintNameEvent "Corriger les événements..." 14 \
  {Vérifier l'orthographe des noms d'événements}
menuText F FileMaintNameSite "Corriger les noms de lieux..." 21 \
  {Vérifier l'orthographe des noms de lieux}
menuText F FileMaintNameRound "Corriger les noms des rondes..." 22 \
  {Vérifier l'orthographe des noms de rondes}
menuText F FileReadOnly "Lecture seule..." 0 \
  {Traiter la base courante en lecture seule, en empêchant les changements}
menuText F FileSwitch "Changer de base" 0 \
  {Changer vers une base ouverte différente}
menuText F FileExit "Quitter" 0 {Quitter Scid}
menuText F FileMaintFixBase "Réparer base" 0 {Essaie de réparer une base corrompue}

# Edit menu:
menuText F Edit "Édition" 1
menuText F EditAdd "Ajouter une variante" 0 {Ajouter une variante}
menuText F EditDelete "Effacer cette variante" 0 {Effacer cette variante}
menuText F EditFirst "Déplacer en tête" 0 \
  {Déplacer cette variante en tête de liste}
menuText F EditMain "Variante vers ligne principale" 13 \
   {Promouvoir une variante en ligne principale}
menuText F EditTrial "Essayer une variante" 0 \
  {Démarrer/Stopper mode d'essai, pour tester une idée sur l'échiquier}
menuText F EditStrip "Épurer" 2 {Épurer les commentaires ou les variantes de cette partie}
menuText F EditUndo "Annuler" 0 {Annuler la dernière modification de cette partie}
menuText F EditRedo "Rétablir" 0 {Refaire la dernière modification de cette partie}
menuText F EditStripComments "Commentaires" 0 \
  {Épurer cette partie de tous les commentaires et annotations}
menuText F EditStripVars "Variantes" 0 {Épurer cette partie des variantes}
menuText F EditStripBegin "Coups depuis le début" 1 \
  {Épurer cette partie des coups depuis le début}
menuText F EditStripEnd "Coups jusqu'à la fin" 0 \
  {Épurer cette partie des coups jusqu'à la fin}
menuText F EditReset "Vider la Clipbase" 0 {Vider la base de données Clipbase}
menuText F EditCopy "Copier dans la Clipbase" 19 \
  {Copier la partie en cours dans la base de données Clipbase}
menuText F EditPaste "Coller depuis la Clipbase" 19 \
  {Copier la partie contenue dans la base Clipbase à cet emplacement}
menuText F EditPastePGN "Coller le presse-papier comme partie PGN..." 1 \
  {Interpréter le texte du presse-papier comme de la notation PGN et le coller comme partie active}
menuText F EditSetup "Définir la position de départ" 0 \
  {Choisir une position de départ pour cette partie}
menuText F EditCopyBoard "Copier la position" 10 \
  {Copier la position en cours en notation FEN vers la sélection de texte (presse-papier)}
menuText F EditPasteBoard "Coller la position de départ" 12 \
  {Initialiser la position de départ à partir de la sélection de texte courante (presse-papier)}
menuText F ConfigureScid "Préférences..." 1 {Configurer toutes les options pour SCID}

# Game menu:
menuText F Game "Partie" 0
menuText F GameNew "Nouvelle partie" 2 \
  {Remettre à zéro la partie}
menuText F GameFirst "Charger la première partie" 4 {Charger la première partie du filtre}
menuText F GamePrev "Partie précédente" 7 \
  {Charger la partie précédente du filtre}
menuText F GameReload "Recharger la partie" 2 \
  {Recharger la partie (les modifications ne seront pas sauvegardées)}
menuText F GameNext "Partie suivante" 7 {Charger la partie suivante du filtre}
menuText F GameLast "Charger la dernière partie" 5 {Charger la dernière partie du filtre}
menuText F GameRandom "Charger une partie au hasard" 1 {Charger au hasard une partie du filtre}
menuText F GameNumber "Partie numéro..." 9 \
  {Charger une partie en donnant son numéro}
menuText F GameReplace "Enregistrer: remplacer la partie..." 13 \
  {Enregistrer cette partie, en écrasant l'ancienne version}
menuText F GameAdd "Enregistrer: ajouter nouvelle partie..." 13 \
  {Enregistrer cette partie en tant que nouvelle partie dans la base}
menuText F GameDeepest "Identifier l'ouverture" 13 \
  {Trouver la partie la plus proche dans la nomenclature ECO}
menuText F GameGotoMove "Aller au coup numéro..." 14 \
  {Aller au coup spécifié dans la partie en cours}
menuText F GameNovelty "Trouver la nouveauté..." 4 \
  {Trouver le premier coup de la partie qui n'a pas été joué auparavant}

# Search menu:
menuText F Search "Rechercher" 0
menuText F SearchReset "Réinitialiser le filtre" 0 \
  {Remettre à zéro le filtre (inclure toutes les parties)}
menuText F SearchNegate "Inverser le filtre" 0 \
  {Inverser le filtre}
menuText F SearchCurrent "Position..." 0 \
  {Rechercher selon la position actuelle}
menuText F SearchHeader "En-tête..." 0 \
  {Rechercher par l'information dans l'en-tête (joueur, tournoi, etc.)}
menuText F SearchMaterial "Matériel ou motif..." 0 \
  {Rechercher un motif ou un matériel particulier sur l'échiquier}
menuText F SearchUsing "En utilisant un fichier d'options..." 3 \
  {Rechercher en utilisant un fichier d'options de recherche}

# Windows menu:
menuText F Windows "Fenêtres" 1
menuText F WindowsComment "Éditeur de commentaires" 1 \
  {Ouvrir/fermer l'éditeur de commentaires}
menuText F WindowsGList "Liste des parties" 0 \
  {Ouvrir/fermer la liste des parties}
menuText F WindowsPGN "Fenêtre PGN" 8 {Ouvrir/fermer la fenêtre PGN}
menuText F WindowsPList "Chercher un joueur" 12 {Ouvrir/fermer la fenêtre de recherche de joueurs}
menuText F WindowsTmt "Chercher un tournoi" 12 {Ouvrir/fermer la fenêtre de recherche de tournois}
menuText F WindowsSwitcher "Changer de base" 0 \
  {Ouvrir/fermer l'outil de changement de base}
menuText F WindowsMaint "Fenêtre de Maintenance" 0 \
  {Ouvrir/fermer la fenêtre de maintenance}
menuText F WindowsECO "Navigateur ECO" 0 {Ouvrir/fermer le navigateur ECO}
menuText F WindowsStats "Statistiques des parties filtrées" 0 \
  {Ouvrir/fermer la fenêtre des statistiques des parties filtrées}
menuText F WindowsTree "Fenêtre d'arbre" 10 \
  {Ouvrir/fermer la fenêtre d'arbre}
menuText F WindowsTB "Fenêtre de tables de finales" 0 \
  {Ouvrir/fermer la fenêtre de tables de finales}
menuText F WindowsBook "Bibliothèque d'ouvertures" 0 {Ouvrir/Fermer la bibliothèque d'ouverture}
menuText F WindowsCorrChess "Jeu par correspondance" 0 {Ouvrir/Fermer la fenêtre de jeu par correspondance}

# Tools menu:
menuText F Tools "Outils" 2
menuText F ToolsAnalysis "Moteur d'analyse..." 9 \
  {Démarrer/Arrêter un moteur d'analyses}
menuText F ToolsAnalysis2 "Moteur d'analyse #2..." 18 \
  {Démarrer/Arrêter un moteur d'analyses}
menuText F ToolsCross "Classement du tournoi" 9 \
  {Montrer le classement du tournoi}
menuText F ToolsEmail "Gestion des messages" 0 \
  {Ouvrir/Fermer la fenêtre de gestion des messages électroniques}
menuText F ToolsFilterGraph "Graphique de filtre" 6 {Ouvrir/Fermer la fenêtre du graphique de filtre}
menuText F ToolsAbsFilterGraph "Graphique de filtre absolu" 7 {Ouvrir/Fermer la fenêtre du graphique de filtre absolu}
menuText F ToolsOpReport "Rapport d'ouverture" 0 {Générer un rapport d'ouvertures à partir de la position courante}
menuText F ToolsOpenBaseAsTree "Ouvrir une base dans l'arbre" 11 {Ouvrir une base dans la fenêtre d'arbre}
menuText F ToolsOpenRecentBaseAsTree "Ouvrir une base récente dans l'arbre" 16 {Ouvrir une base récente dans la fenêtre d'arbre}
menuText F ToolsTracker "Suivi de pièce"  3 {Ouvrir la fenêtre de suivi de pièce}
menuText F ToolsTraining "Entraînement"  0 {Entraînement (tactique, ouvertures, ...) }
menuText F ToolsTacticalGame "Partie tactique"  7 {Jouer une partie tactique}
menuText F ToolsSeriousGame "Partie sérieuse"  7 {Jouer une partie sans handicap}
menuText F ToolsTrainOpenings "Ouvertures"  0 {Entraînement avec un répertoire d'ouvertures}
menuText F ToolsTrainReviewGame "Reviser la partie"  0 {Étudier la partie en devinant les coups joués}
menuText F ToolsTrainTactics "Exercices tactiques"  0 {Résoudre des problèmes tactiques}
menuText F ToolsTrainCalvar "Calcul de variantes"  0 {Entraînement au calcul de variantes}
menuText F ToolsTrainFindBestMove "Trouver le meilleur coup"  0 {Trouver le meilleur coup}
menuText F ToolsTrainFics "Jouer sur internet"  10 {Jouer sur freechess.org}
menuText F ToolsEngineTournament "Tournoi de moteurs"  0 {Démarrer un tournoi entre moteurs d'échecs}
menuText F ToolsBookTuning "Config. bibliothèque" 0 {Configuration fine d'une bibliothèque}
menuText F ToolsConnectHardware "Connecter un matériel" 2 {Connecter un échiquier externe}
menuText F ToolsConnectHardwareConfigure "Configurer..." 0 {Configurer la connexion avec un échiquier externe}
menuText F ToolsConnectHardwareNovagCitrineConnect "Connecter Novag Citrine" 2 {Connecter Novag Citrine}
menuText F ToolsConnectHardwareInputEngineConnect "Connecter via Input Engine" 1 {Connecter via le protocole Input Engine (par ex. un échiquier DGT)}

menuText F ToolsPInfo "Information sur les joueurs"  0 \
  {Ouvrir/Mettre à jour la fenêtre d'information sur les joueurs}
menuText F ToolsPlayerReport "Rapport sur le joueur..." 2 \
  {Générer un rapport pour le joueur}
menuText F ToolsRating "Graphique Elo" 10 \
  {Tracer un graphique du classement Elo des joueurs de cette partie}
menuText F ToolsScore "Graphique de score" 13 {Montrer le graphique de score pendant la partie}
menuText F ToolsExpCurrent "Exporter la partie en cours" 1 \
  {Exporter la partie en cours dans un fichier texte}
menuText F ToolsExpCurrentPGN "Dans un fichier PGN..." 16 \
  {Exporter la partie en cours dans un fichier PGN}
menuText F ToolsExpCurrentHTML "Dans un fichier HTML..." 16 \
  {Exporter la partie en cours dans un fichier HTML}
menuText F ToolsExpCurrentHTMLJS "Dans un fichier HTML et JavaScript..." 24 {Exporter la partie en cours dans un fichier HTML et JavaScript}
menuText F ToolsExpCurrentLaTeX "Dans un fichier LaTeX..." 16 \
  {Exporter la partie en cours dans un fichier LaTeX}
menuText F ToolsExpFilter "Exporter les parties filtrées" 0 \
  {Exporter les parties sélectionnées par un filtre dans un fichier PGN, HTML, LaTeX, ou dans une autre base}
menuText F ToolsExpFilterPGN "Dans un fichier PGN..." 16 \
  {Exporter le filtre dans un fichier PGN}
menuText F ToolsExpFilterHTML "Dans un fichier HTML..." 16 \
  {Exporter le filtre dans un fichier HTML}
menuText F ToolsExpFilterHTMLJS "Dans un fichier HTML et JavaScript..." 24 {Exporter le filtre dans un fichier HTML et JavaScript}
menuText F ToolsExpFilterLaTeX "Dans un fichier LaTeX..." 16 \
  {Exporter le filtre dans un fichier LaTeX}
menuText F ToolsImportOne "Importer une partie en PGN..." 0 \
  {Importer une partie à partir d'un texte PGN}
menuText F ToolsImportFile "Importer des fichiers PGN..." 0 \
  {Importer des parties à partir d'un ou plusieurs fichiers PGN}
menuText F ToolsStartEngine1 "Démarrer moteur 1" 16  {Démarrer moteur 1}
menuText F ToolsStartEngine2 "Démarrer moteur 2" 16  {Démarrer moteur 2}
menuText F ToolsCaptureBoard "Capturer l'échiquier..." 0  {Enregistrer la position actuelle de l'échiquier en tant qu'image.}

# Play menu
menuText F Play "Jouer" 0

# --- Correspondence Chess
menuText F CorrespondenceChess "Échecs par correspondance" 1 {Échecs par correspondance par e-mail ou Xfcc}
menuText F CCConfigure "Configurer..." 0 {Configurer le sous-sytème d'échecs par correspondance par e-mail ou Xfcc}
menuText F CCConfigRelay "Observer des parties..." 2 {Configurer les jeux à observer sur un site web}
menuText F CCOpenDB "Ouvrir la base..." 0 {Ouvrir la base de jeux par correspondance par défaut}
menuText F CCRetrieve "Récupérer les parties" 0 {Récupérer les parties via Xfcc}
menuText F CCInbox "Analyser la boîte de réception" 0 {Analyser tous les fichiers dans la boîte de réception de Scid}
menuText F CCSend "Envoyer le coup" 2 {Envoyer votre coup via e-mail ou un outil externe (Xfcc)}

menuText F CCResign "Abandonner" 1 {Abandonner (par Xfcc)}
menuText F CCClaimDraw "Réclamer la partie nulle" 3 {Envoyer le coup et réclamer une partie nulle (par Xfcc)}
menuText F CCOfferDraw "Proposer la partie nulle" 0 {Envoyer le coup et proposer une partie nulle (par Xfcc)}
menuText F CCAcceptDraw "Accepter la partie nulle" 5 {Accepter la partie nulle (par Xfcc)}

menuText F CCNewMailGame "Nouvelle partie par e-mail..." 0 {Commencer une nouvelle partie par e-mail}
menuText F CCMailMove "e-mailer le coup..." 0 {Envoyer le coup par e-mail à l'adversaire}
menuText F CCGamePage "Page web de la partie..." 5 {Ouvrir la partie dans un navigateur web}

# menu in cc window:
menuText F CCEditCopy "Copier parties dans Clipbase" 0 {Copier les parties comme liste CSV list dans Clipbase}

# Options menu:
menuText F Options "Options" 0
menuText F OptionsBoardGraphics "Images de cases..." 0 {Sélectionner les textures des cases}
translate F OptionsBGW {Sélectionner les textures des cases}
translate F OptionsBoardGraphicsText {Sélection des images pour les cases blanches et noires:}
menuText F OptionsBoardNames "Mes noms de joueurs..." 0 {Éditer mes noms de joueurs}
menuText F OptionsExport "Exportation" 1 {Changer les options d'exportation}
menuText F OptionsFonts "Polices" 0 {Changer les polices}
menuText F OptionsFontsRegular "Normales" 0 {Changer les polices normales}
menuText F OptionsFontsMenu "Menu" 0 {Changer la police des menus}
menuText F OptionsFontsSmall "Petites" 0 {Changer les petites polices}
menuText F OptionsFontsTiny "Minuscule" 1 {Changes les polices minuscules}
menuText F OptionsFontsFixed "Fixe" 0 {Changer les polices de chasse fixe}
menuText F OptionsGInfo "Informations de la partie" 7 {Options d'information de la partie}
menuText F OptionsLanguage "Langue" 0 {Sélectionner la langue utilisée}
menuText F OptionsMovesTranslatePieces "Traduction des pièces" 0 {Convertir la première lettre des noms des pièces en français}
menuText F OptionsMovesHighlightLastMove "Surbrillance dernier coup" 2 {Surbrillance du dernier coup}
menuText F OptionsMovesHighlightLastMoveDisplay "Afficher" 0 {Afficher la surbrillance du dernier coup}
menuText F OptionsMovesHighlightLastMoveWidth "Epaisseur" 0 {Epaisseur de la ligne}
menuText F OptionsMovesHighlightLastMoveColor "Couleur" 0 {Couleur de la ligne}
menuText F OptionsMovesHighlightLastMoveArrow "inclure Flèche" 0 {Afficher la flèche de déplacement du dernier coup}
menuText F OptionsMoves "Coups" 2 {Gestion des coups}
menuText F OptionsMovesAnimate "Temps d'animation" 1 \
  {Régler le temps utilisé pour l'animation des coups}
menuText F OptionsMovesDelay "Délai entre les coups" 3 \
  {Régler le délai entre deux coups en mode automatique}
menuText F OptionsMovesCoord "Saisie de coordonnées" 0 \
  {Accepter l'entrée des coups par coordonnées ("g1f3")}
menuText F OptionsMovesSuggest "Montrer les coups suggérés" 0 \
  {Activer/Désactiver le mode de suggestion de coup}
menuText F OptionsShowVarPopup "Montrer fenêtre des variantes" 1 {Affichage automatique le sélecteur de variantes}
menuText F OptionsMovesSpace "Ajouter un espace après le n° du coup" 1 {Ajouter un espace après le numéro du coup}
menuText F OptionsMovesKey "Auto-complétion clavier" 1 \
  {Activer/Désactiver le mode d'auto-complétion du clavier}
menuText F OptionsMovesShowVarArrows "Montrer flèches pour les variantes" 2 {Montrer des flèches pour les prochains coups des variantes existantes}
menuText F OptionsMovesGlossOfDanger "Couleur codée Gloss of Danger" 0 {Afficher/Cacher les cases colorées avec le code Gloss of Danger}
menuText F OptionsNumbers "Format numérique" 7 {Sélectionner le format des nombres}
menuText F OptionsTheme "Apparence" 0 {Change l'apparence de l'interface}
menuText F OptionsWindows "Fenêtres" 0 {Options des fenêtres}
menuText F OptionsSounds "Sons" 2 {Configurer l'annonce des sonore des coups}
menuText F OptionsResources "Ressources..." 0 {Choose resource files and folders}
menuText F OptionsWindowsDock "Fenêtres groupées" 0 {Fenêtres groupées (nécessite un redémarrage)}
menuText F OptionsWindowsSaveLayout "Enregistrer disposition" 0 {Enregistrer disposition}
menuText F OptionsWindowsRestoreLayout "Remettre disposition" 0 {Remettre disposition}
menuText F OptionsWindowsShowGameInfo "Montrer le panneau d'info" 0 {Montrer le panneau d'information}
menuText F OptionsWindowsAutoLoadLayout "Chargement auto 1ère disposition" 0 {Chargement auto de la première disposition au démarrage}
menuText F OptionsECO "Charger le fichier ECO" 19 \
  {Charger un fichier au format ECO}
menuText F OptionsSpell "Charger le fichier d'orthographe" 21 \
  {Charger un fichier de correction orthographique scid}
menuText F OptionsTable "Répertoire des tables de finales TB" 5 \
  {Sélectionner un répertoire de fichiers TB, tous les fichiers de ce répertoire seront utilisés}
menuText F OptionsRecent "Fichiers récents..." 1 \
  {Changer le nombre de fichiers récents affichés dans le menu Fichier}
menuText F OptionsBooksDir "Répertoire des bibliothèques" 15 {Répertoire des bibliothèques d'ouverture}
menuText F OptionsTacticsBasesDir "Répertoire des bases d'entraînement" 11 {Répertoire des bases pour l'entraînement tactique}
menuText F OptionsPhotosDir "Répertoire des images" 16 {Répertoire des bases pour les images}
menuText F OptionsThemeDir "Charger thème(s)"  0 { Charger un fichier de thèmes pour l'interface graphique }
menuText F OptionsSave "Enregistrer les options" 0 "Enregistrer les options dans le fichier $::optionsFile"
menuText F OptionsAutoSave "Sauvegarde automatique des options" 0 \
  {Sauvegarder automatiquement toutes les options en quittant Scid}

# Help menu:
menuText F Help "Aide" 0
menuText F HelpContents "Contenu" 0 {Montrer la page du contenu de l'aide}
menuText F HelpIndex "Index" 0 {Afficher la table des matières}
menuText F HelpGuide "Aide Rapide" 5 {Montrer la page d'aide rapide}
menuText F HelpHints "Astuces" 0 {Afficher les trucs et astuces}
menuText F HelpContact "Contact" 1 {Afficher les noms des contacts}
menuText F HelpTip "Astuce du jour" 1 {Afficher une astuce Scid}
menuText F HelpStartup "Fenêtre de démarrage" 0 {Afficher la fenêtre de démarrage}
menuText F HelpAbout "A propos de Scid" 2 {Information au sujet de Scid}

# Game info box popup menu:
menuText F GInfoHideNext "Cacher le coup suivant" 0
menuText F GInfoMaterial "Montrer les valeurs de matériel" 0
menuText F GInfoFEN "Montrer la notation FEN" 5
menuText F GInfoMarks "Montrer les cases colorées et les flèches" 5
menuText F GInfoWrap "Découper les longues lignes" 0
menuText F GInfoFullComment "Montrer tous les commentaires" 10
menuText F GInfoPhotos "Montrer les Photos" 5
menuText F GInfoTBNothing "Tables de finales: Rien" 12
menuText F GInfoTBResult "Tables de finales: Seulement le résultat" 12
menuText F GInfoTBAll "Tables de finales: résultat et meilleurs coups" 19
menuText F GInfoDelete "(Restaurer)Supprimer cette partie" 4
menuText F GInfoMark "(Dé)Marquer cette partie" 4
menuText F GInfoInformant "Configurer les valeurs de notation" 0

# General buttons:
translate F Back {Retour}
translate F Browse {Parcourir}
translate F Cancel {Annuler}
translate F Continue {Continuer}
translate F Clear {Effacer}
translate F Close {Fermer}
translate F Contents {Contenu}
translate F Defaults {Par défaut}
translate F Delete {Éliminer}
translate F Graph {Graphique}
translate F Help {Aide}
translate F Import {Importer}
translate F Index {Index}
translate F LoadGame {Charger cette partie}
translate F BrowseGame {Explorer la partie dans une fenêtre séparée}
translate F MergeGame {Fusionner la partie comme variante}
translate F MergeGames {Fusionner les parties}
translate F Preview {Prévisualiser}
translate F Revert {Revenir}
translate F Save {Enregistrer}
translate F Search {Rechercher}
translate F Stop {Arrêter}
translate F Store {Enregistrer}
translate F Update {Mettre à jour}
translate F ChangeOrient {Changer l'orientation de la fenêtre}
translate F ShowIcons {Montrer les icônes}
translate F None {Aucun}
translate F First {Premier}
translate F Current {En cours}
translate F Last {Dernier}

# General messages:
translate F game {partie}
translate F games {parties}
translate F move {coup}
translate F moves {coups}
translate F all {tout}
translate F Yes {Oui}
translate F No {Non}
translate F Both {Les deux}
translate F King {Roi}
translate F Queen {Dame}
translate F Rook {Tour}
translate F Bishop {Fou}
translate F Knight {Cavalier}
translate F Pawn {Pion}
translate F White {Blancs}
translate F Black {Noirs}
translate F Player {Joueur}
translate F Rating {Classement Elo}
translate F RatingDiff {Différence de classement Elo (Blancs - Noirs)}
translate F AverageRating {Classement moyen}
translate F Event {Événement}
translate F Site {Lieu}
translate F Country {Pays}
translate F IgnoreColors {Ignorer les couleurs}
translate F Date {Date}
translate F EventDate {Date d'événement}
translate F Decade {Décennie}
translate F Year {Année}
translate F Month {Mois}
translate F Months {Janvier Février Mars Avril Mai Juin Juillet Août Septembre Octobre Novembre Décembre}
translate F Days {Dim Lun Mar Mer Jeu Ven Sam}
translate F YearToToday {- 1 an}
translate F YearToTodayTooltip {Fixer la date de 1 an en arrière à aujourd'hui}
translate F Result {Résultat}
translate F Round {Ronde}
translate F Length {Longueur}
translate F ECOCode {code ECO}
translate F ECO {ECO}
translate F Deleted {Éliminé}
translate F SearchResults {Résultats de la recherche}
translate F OpeningTheDatabase {Ouverture de la base}
translate F Database {Base}
translate F Filter {Filtre}
translate F noGames {pas de parties}
translate F allGames {toutes les parties}
translate F empty {vide}
translate F clipbase {Clipbase}
translate F score {score}
translate F StartPos {Position de départ}
translate F Total {Total}
translate F readonly {lecture seule}

# Standard error messages:
translate F ErrNotOpen {Ceci n'est pas une base ouverte.}
translate F ErrReadOnly {Cette base est en lecture seule; elle ne peut être modifiée.}
translate F ErrSearchInterrupted {La recherche a été interrompue; les résultats sont incomplets.}

# Game information:
translate F twin {doublon}
translate F deleted {éliminé}
translate F comment {commentaire}
translate F hidden {caché}
translate F LastMove {Dernier coup}
translate F NextMove {Coup suivant}
translate F GameStart {Début de partie}
translate F LineStart {Début de ligne}
translate F GameEnd {Fin de partie}
translate F LineEnd {Fin de ligne}

# Player information:
translate F PInfoAll {Résultats pour <b>toutes</b> les parties}
translate F PInfoFilter {Résultats pour les parties <b>filtrées</b>}
translate F PInfoAgainst {Résultats contre}
translate F PInfoMostWhite {Ouvertures les plus fréquentes avec les blancs}
translate F PInfoMostBlack {Ouvertures les plus fréquentes avec les noirs}
translate F PInfoRating {Historique du classement}
translate F PInfoBio {Biographie}
translate F PInfoEditRatings {Editer les classements}
translate F PInfoEloFile {Fichier}

# Tablebase information:
translate F Draw {Nulle}
translate F stalemate {pat}
translate F withAllMoves {avec tous les coups}
translate F withAllButOneMove {avec tous les coups sauf le dernier}
translate F with {avec}
translate F only {seulement}
translate F lose {perte}
translate F loses {pertes}
translate F allOthersLose {toutes les autres pertes}
translate F matesIn {mat en}
translate F hasCheckmated {a maté}
translate F longest {le plus long}
translate F WinningMoves {Coups gagnants}
translate F DrawingMoves {Coups faisant nulle}
translate F LosingMoves {Coups perdants}
translate F UnknownMoves {Coups dont le résultat est inconnu}
translate F SideLongest {Joueur    +long    %     %     %\n}
translate F toNoveMate {au trait   mat    Gain  Nul  Perte\n}
translate F NoSummary {\nPas de résumé pour cette table de finales.}
translate F MutualZugzwang {Zugzwangs mutuels : }
translate F SelectetZugzwang {\nPositions de zugzwang choisies :}
translate F WdrawBloss {Blanc annule, Noir perd :}
translate F BdrawWloss {Noir annule, Blanc perd :}
translate F AllLoss {" Celui au trait perd:     " }

# Tip of the day:
translate F Tip {Conseil}
translate F TipAtStartup {Conseil au démarrage}

# Tree window menus:
menuText F TreeFile "Fichier" 0
menuText F TreeFileFillWithBase "Remplir le cache avec la base" 0 {Remplir le cache avec toutes les parties de la base courante}
menuText F TreeFileFillWithGame "Remplir le cache avec la partie" 0 {Remplir le cache avec la partie de la base courante}
menuText F TreeFileSetCacheSize "Taille du cache" 0 {Taille du cache}
menuText F TreeFileCacheInfo "Info cache" 0 {Afficher information sur l'utilisation du cache}
menuText F TreeFileSave "Enregistrer le cache" 0
menuText F TreeFileFill "Remplir le cache" 0
menuText F TreeFileBest "Liste des meilleures parties" 0 {Montrer la liste des meilleures parties}
menuText F TreeFileGraph "Graphique" 0 {Graphique de l'arbre}
menuText F TreeFileCopy "Copier l'arbre dans le presse-papier" 0
menuText F TreeFileClose "Fermer la fenêtre d'arbre" 0
menuText F TreeMask "Masque" 0
menuText F TreeMaskNew "Nouveau" 0 {Nouveau masque}
menuText F TreeMaskOpen "Ouvrir" 0 {Ouvrir un masque}
menuText F TreeMaskOpenRecent "Ouvrir récent" 0 {Ouvrir un masque récent}
menuText F TreeMaskSave "Enregistrer" 0 {Enregistrer le masque}
menuText F TreeMaskClose "Fermer" 0 {Fermer le masque}
menuText F TreeMaskFillWithGame "Remplir avec la partie" 0 {Remplir le masque avec la partie courante}
menuText F TreeMaskFillWithBase "Remplir avec la base" 0 {Remplir le masque avec les parties de la base}
menuText F TreeMaskInfo "Info" 0 {Statistiques du masque}
menuText F TreeMaskDisplay "Afficher l'arbre" 0 {Affichage des données du masque sous la forme d'un arbre}
menuText F TreeMaskSearch "Chercher" 0 {Chercher dans le masque courant}
menuText F TreeSort "Trier" 0
menuText F TreeSortAlpha "Alphabétique" 0
menuText F TreeSortECO "code ECO" 0
menuText F TreeSortFreq "Fréquence" 0
menuText F TreeSortScore "Score" 0
menuText F TreeOpt "Options" 0
menuText F TreeOptSlowmode "Mode lent" 0 {Mode lent pour rafraichissement (précis)}
menuText F TreeOptFastmode "Mode rapide" 0 {Mode rapide pour rafraichissement (pas de transpositions)}
menuText F TreeOptFastAndSlowmode "Mode rapide puis lent" 0 {Mode rapide puis lent pour rafraichissement}
menuText F TreeOptStartStop "Rafraîchissement auto" 0 {Rafraîchissement automatique de la fenêtre}
menuText F TreeOptLock "Verrouille" 0 {Dé/verrouiller l'arbre à la base en cours}
menuText F TreeOptTraining "Entraînement" 0 {Démarrer/Arrêter l'arbre du mode d'entraînement}
menuText F TreeOptAutosave "Enregistrer le cache automatiquement" 0
menuText F TreeHelp "Aide" 0
menuText F TreeHelpTree "Aide Arbre" 0
menuText F TreeHelpIndex "Index" 0
translate F SaveCache {Enregistrer le cache}
translate F Training {Entraînement}
translate F LockTree {Verrouiller}
translate F TreeLocked {verrouillé}
translate F TreeBest {Meilleur}
translate F TreeBestGames {Arbre des meilleures parties}
# Note: the next message is the tree window title row. After editing it,
# check the tree window to make sure it lines up with the actual columns.
translate F TreeTitleRow \
  {    Coup   ECO        Fréquence   Score EloMoy Perf AnnéeMoy %Nulle}
translate F TreeTotal {TOTAL}
translate F DoYouWantToSaveFirst {voulez-vous d'abord sauvegarder}
translate F AddToMask {Ajouter au masque}
translate F RemoveFromMask {Enlever du masque}
translate F AddThisMoveToMask {Ajouter ce coup au masque}
translate F SearchMask {Recherche dans masque}
translate F DisplayMask {Affichage du masque}
translate F Nag {Code Nag}
translate F Marker {Marqueur}
translate F Include {Inclure}
translate F Exclude {Exclure}
translate F MainLine {Ligne principale}
translate F Bookmark {Signet}
translate F NewLine {Nouvelle ligne}
translate F ToBeVerified {À vérifier}
translate F ToTrain {S'entrainer}
translate F Dubious {Douteux}
translate F ToRemove {À supprimer}
translate F NoMarker {Aucun marqueur}
translate F ColorMarker {Couleur}
translate F WhiteMark {Blanc}
translate F GreenMark {Vert}
translate F YellowMark {Jaune}
translate F BlueMark {Bleu}
translate F RedMark {Rouge}
translate F CommentMove {Annoter coup}
translate F CommentPosition {Annoter position}
translate F AddMoveToMaskFirst {Ajoutez d'abord le coup au masque}
translate F OpenAMaskFileFirst {Ouvrez d'abord un masque}
translate F Positions {Positions}
translate F Moves {Coups}

# Finder window:
menuText F FinderFile "Fichier" 0
menuText F FinderFileSubdirs "Rechercher dans les sous-répertoires" 0
menuText F FinderFileClose "Fermer l'explorateur de fichiers" 0
menuText F FinderSort "Trier" 0
menuText F FinderSortType "Type" 0
menuText F FinderSortSize "Taille" 0
menuText F FinderSortMod "Modifié" 0
menuText F FinderSortName "Nom" 0
menuText F FinderSortPath "Chemin" 0
menuText F FinderTypes "Types" 0
menuText F FinderTypesScid "Bases Scid" 0
menuText F FinderTypesOld "Bases Scid à l'ancien format" 0
menuText F FinderTypesPGN "fichiers PGN" 0
menuText F FinderTypesEPD "fichiers EPD (théorie)" 0
menuText F FinderTypesRep "fichiers des répertoires" 0
menuText F FinderHelp "Aide" 0
menuText F FinderHelpFinder "Aide de l'explorateur de fichiers" 0
menuText F FinderHelpIndex "Index" 0
translate F FileFinder {Explorateur de fichiers}
translate F FinderDir {Répertoire}
translate F FinderDirs {Répertoires}
translate F FinderFiles {Fichiers}
translate F FinderUpDir {répertoire précédent}
translate F FinderCtxOpen {Ouvrir}
translate F FinderCtxBackup {Sauvegarder}
translate F FinderCtxCopy {Copier}
translate F FinderCtxMove {Déplacer}
translate F FinderCtxDelete {Supprimer}

# Player finder:
menuText F PListFile "Fichier" 0
menuText F PListFileUpdate "Mettre à jour" 0
menuText F PListFileClose "Fermer la recherche de joueur" 0
menuText F PListSort "Trier" 0
menuText F PListSortName "Nom" 0
menuText F PListSortElo "Elo" 0
menuText F PListSortGames "Parties" 0
menuText F PListSortOldest "Les plus vieux" 0
menuText F PListSortNewest "Les plus récents" 0

# Tournament finder:
menuText F TmtFile "Fichier" 0
menuText F TmtFileUpdate "Mettre à jour" 0
menuText F TmtFileClose "Fermer l'explorateur de tournois" 0
menuText F TmtSort "Trier" 0
menuText F TmtSortDate "Date" 0
menuText F TmtSortPlayers "Joueurs" 0
menuText F TmtSortGames "Parties" 0
menuText F TmtSortElo "Elo" 0
menuText F TmtSortSite "Lieux" 0
menuText F TmtSortEvent "Événement" 1
menuText F TmtSortWinner "Vainqueur" 0
translate F TmtLimit "Limite de liste"
translate F TmtMeanElo "Moyenne Elo la plus basse"
translate F TmtNone "Aucun tournoi correspondant n'a été trouvé."

# Graph windows:
menuText F GraphFile "Fichier" 0
menuText F GraphFileColor "Enregistrer comme Postscript en couleurs..." 27
menuText F GraphFileGrey "Enregistrer comme Postscript en niveaux de gris..." 27
menuText F GraphFileClose "Fermer la fenêtre" 0
menuText F GraphOptions "Options" 0
menuText F GraphOptionsWhite "Blancs" 0
menuText F GraphOptionsBlack "Noirs" 0
menuText F GraphOptionsBoth "Les deux" 4
menuText F GraphOptionsPInfo "Informations joueurs" 0
menuText F GraphOptionsEloFile "Classement Elo du fichier" 0
menuText F GraphOptionsEloDB "Classement Elo dans la base" 0
translate F GraphFilterTitle "Graphique de filtre: fréquence pour 1000 parties"
translate F GraphAbsFilterTitle "Graphique de filtre: fréquence des parties"
translate F ConfigureFilter {Configurer l'axe des X par année, classement et coups}
translate F FilterEstimate "Estimer"
translate F TitleFilterGraph "Scid: Graphe de filtre"

# Analysis window:
translate F AddVariation {Ajouter une variante}
translate F AddAllVariations {Ajouter toutes les variantes}
translate F AddMove {Ajouter le coup}
translate F Annotate {Annotation}
translate F ShowAnalysisBoard {Montrer échiquier d'analyse}
translate F ShowInfo {Montrer infos moteur}
translate F FinishGame {Continuer la partie}
translate F StopEngine {Arrêter le moteur}
translate F StartEngine {Démarrer le moteur}
translate F LockEngine {Verrouiller moteur à la position actuelle}
translate F AnalysisCommand {Commande d'analyse}
translate F PreviousChoices {Choix précédents}
translate F AnnotateTime {Temps en secondes par coup}
translate F AnnotateWhich {Ajouter des variantes}
translate F AnnotateAll {Pour les coups des deux côtés}
translate F AnnotateAllMoves {Annoter tous les coups}
translate F AnnotateWhite {Pour les coups des blancs seulement}
translate F AnnotateBlack {Pour les coups des noirs seulement}
translate F AnnotateBlundersOnly {Quand le coup est une erreur}
translate F AnnotateBlundersOnlyScoreChange {Coup faible, le score varie de/à: }
translate F BlundersThreshold {Seuil de l'erreur}
translate F ScoreAllMoves {Valider tous les coups}
translate F LowPriority {Priorité processeur basse}
translate F ClickHereToSeeMoves {Cliquer ici pour afficher les coups}
translate F ConfigureInformant {Valeurs des notations}
translate F Informant!? {Coup intéressant}
translate F Informant? {Coup faible}
translate F Informant?? {Mauvais coup}
translate F Informant?! {Coup douteux}
translate F Informant+= {Les blancs ont un léger avantage}
translate F Informant+/- {Les blancs ont l'avantage}
translate F Informant+- {Les blancs ont un avantage décisif}
translate F Informant++- {La partie est gagnée}

# Book window
translate F Book {Bibliothèque}
translate F OtherBookMoves {Bibliothèque de l'adversaire}
translate F OtherBookMovesTooltip {Coups pour lesquels l'adversaire a une réponse}

# Analysis Engine open dialog:
translate F EngineList {Liste des moteurs d'analyse}
translate F EngineName {Nom}
translate F EngineCmd {Commande}
translate F EngineArgs {Paramètres}
translate F EngineDir {Répertoire}
translate F EngineElo {Elo}
translate F EngineTime {Date}
translate F EngineNew {Nouveau}
translate F EngineEdit {Éditer}
translate F EngineRequired {Les champs en gras sont requis; les autres sont optionnels}

# PGN window menus:
menuText F PgnFile "Fichier" 0
menuText F PgnFileCopy "Copier la partie dans le presse-papier" 0
menuText F PgnFilePrint "Enregistrer sous..." 0
menuText F PgnFileClose "Fermer la fenêtre PGN" 0
menuText F PgnOpt "Affichage" 0
menuText F PgnOptColor "Couleur d'affichage" 0
menuText F PgnOptShort "En-tête court (trois lignes)" 0
menuText F PgnOptSymbols "Annotations symboliques" 0
menuText F PgnOptIndentC "Indentation des commentaires" 0
menuText F PgnOptIndentV "Indentation des variantes" 16
menuText F PgnOptColumn "Style en colonne (un coup par ligne)" 1
menuText F PgnOptSpace "Espace après numéro des coups" 0
menuText F PgnOptStripMarks "Enlever les codes de flèches et de coloration de cases" 1
menuText F PgnOptBoldMainLine "Mettre en gras les coups de la ligne principale" 4
menuText F PgnColor "Couleur" 0
menuText F PgnColorHeader "En-tête..." 0
menuText F PgnColorAnno "Annotations..." 0
menuText F PgnColorComments "Commentaires..." 0
menuText F PgnColorVars "Variantes..." 0
menuText F PgnColorBackground "Couleur du fond..." 11
menuText F PgnColorMain "Main line..." 0
menuText F PgnColorCurrent "Dernier coup..." 1
menuText F PgnHelp "Aide" 0
menuText F PgnHelpPgn "Aide PGN" 0
menuText F PgnHelpIndex "Index" 0
translate F PgnWindowTitle {Notation - partie %u}

# Crosstable window menus:
menuText F CrosstabFile "Fichier" 0
menuText F CrosstabFileText "Écrire dans un fichier texte..." 23
menuText F CrosstabFileHtml "Écrire dans un fichier HTML..." 23
menuText F CrosstabFileLaTeX "Écrire dans un fichier LaTeX..." 23
menuText F CrosstabFileClose "Fermer fenêtre tableau" 0
menuText F CrosstabEdit "Éditer" 0
menuText F CrosstabEditEvent "Événement" 0
menuText F CrosstabEditSite "Lieux" 0
menuText F CrosstabEditDate "Date" 0
menuText F CrosstabOpt "Affichage" 0
menuText F CrosstabOptAll "Toutes rondes" 0
menuText F CrosstabOptSwiss "Suisse" 0
menuText F CrosstabOptKnockout "KO" 0
menuText F CrosstabOptAuto "Auto" 0
menuText F CrosstabOptAges "Ages en années" 8
menuText F CrosstabOptNats "Nationalités" 0
menuText F CrosstabOptRatings "Classements" 0
menuText F CrosstabOptTitles "Titres" 0
menuText F CrosstabOptBreaks "Scores du départage" 4
menuText F CrosstabOptDeleted "Inclure les parties effacées" 8
menuText F CrosstabOptColors "Couleurs (système suisse uniquement)" 1
menuText F CrosstabOptColumnNumbers "Colonnes numérotées (Toutes rondes seulement)" 2
menuText F CrosstabOptGroup "Scores du groupe" 0
menuText F CrosstabSort "Trier" 0
menuText F CrosstabSortName "Nom" 0
menuText F CrosstabSortRating "Elo" 0
menuText F CrosstabSortScore "Score" 0
menuText F CrosstabColor "Couleur" 0
menuText F CrosstabColorPlain "Texte normal" 0
menuText F CrosstabColorHyper "Hypertexte" 0
menuText F CrosstabHelp "Aide" 0
menuText F CrosstabHelpCross "Aide tableau" 0
menuText F CrosstabHelpIndex "Index" 0
translate F SetFilter {Activer le filtre}
translate F AddToFilter {Ajouter au filtre}
translate F Swiss {Suisse}
translate F Category {Catégorie}

# Opening report window menus:
menuText F OprepFile "Fichier" 0
menuText F OprepFileText "Écrire dans un fichier texte..." 23
menuText F OprepFileHtml "Écrire dans un fichier HTML..." 23
menuText F OprepFileLaTeX "Écrire dans un fichier LaTeX..." 23
menuText F OprepFileOptions "Options..." 0
menuText F OprepFileClose "Fermer la fenêtre du rapport" 0
menuText F OprepFavorites "Favoris" 1
menuText F OprepFavoritesAdd "Ajouter au rapport..." 0
menuText F OprepFavoritesEdit "Editer les favoris de rapport..." 0
menuText F OprepFavoritesGenerate "Générer les rapports..." 0

menuText F OprepHelp "Aide" 0
menuText F OprepHelpReport "Aide du rapport" 0
menuText F OprepHelpIndex "Index" 0

# Header search:
translate F HeaderSearch {Rechercher dans l'en-tête}
translate F EndSideToMove {Côté devant jouer à la fin de la partie}
translate F GamesWithNoECO {Parties sans code ECO ?}
translate F GameLength {Longueur de la partie}
translate F FindGamesWith {Chercher les parties avec}
translate F StdStart {Pos. départ non standard}
translate F Promotions {Promotions}
translate F Comments {Commentaires}
translate F Variations {Variantes}
translate F Annotations {Annotations}
translate F DeleteFlag {Marquées supprimées}
translate F WhiteOpFlag {Ouverture des blancs}
translate F BlackOpFlag {Ouverture des noirs}
translate F MiddlegameFlag {Milieu de partie}
translate F EndgameFlag {Finale}
translate F NoveltyFlag {Nouveauté}
translate F PawnFlag {Structure de pions}
translate F TacticsFlag {Tactiques}
translate F QsideFlag {Jeu à l'aile dame}
translate F KsideFlag {Jeu à l'aile roi}
translate F BrilliancyFlag {Spectaculaire}
translate F BlunderFlag {Gaffe}
translate F UserFlag {Utilisateur}
translate F PgnContains {PGN contient le texte}
translate F Annotator {Annotateur}
translate F Cmnts {Jeux annotés uniquement}

# Game list window:
translate F GlistNumber {Numéro}
translate F GlistWhite {Blancs}
translate F GlistBlack {Noirs}
translate F GlistWElo {B-Elo}
translate F GlistBElo {N-Elo}
translate F GlistEvent {Événement}
translate F GlistSite {Lieu}
translate F GlistRound {Ronde}
translate F GlistDate {Date}
translate F GlistYear {Année}
translate F GlistEDate {Date d'événement}
translate F GlistResult {Résultat}
translate F GlistLength {Longueur}
translate F GlistCountry {Pays}
translate F GlistECO {ECO}
translate F GlistOpening {Ouverture}
translate F GlistEndMaterial {Matériel final}
translate F GlistDeleted {Effacé}
translate F GlistFlags {Drapeaux}
translate F GlistVars {Variantes}
translate F GlistComments {Commentaires}
translate F GlistAnnos {Annotations}
translate F GlistStart {Départ}
translate F GlistGameNumber {Partie numéro}
translate F GlistAverageElo {Moyenne Elo}
translate F GlistRating {Classement}
translate F GlistFindText {Rechercher texte}
translate F GlistMoveField {Coup}
translate F GlistEditField {Configurer}
translate F GlistAddField {Ajouter}
translate F GlistDeleteField {Enlever}
translate F GlistWidth {Largeur}
translate F GlistAlign {Aligner}
translate F GlistAlignL {Aligner: à gauche}
translate F GlistAlignR {Aligner: à droite}
translate F GlistAlignC {Aligner: au centre}
translate F GlistColor {Couleur}
translate F GlistSep {Séparateur}
# ====== TODO To be translated ======
translate F GlistCurrentSep {-- Current --}
translate F GlistNewSort {Nouveau}
# ====== TODO To be translated ======
translate F GlistAddToSort {Add}
# ====== TODO To be translated ======
translate F GsortSort {Sort...}
# ====== TODO To be translated ======
translate F GsortDate {Date}
# ====== TODO To be translated ======
translate F GsortYear {Year}
# ====== TODO To be translated ======
translate F GsortEvent {Event}
# ====== TODO To be translated ======
translate F GsortSite {Site}
# ====== TODO To be translated ======
translate F GsortRound {Round}
# ====== TODO To be translated ======
translate F GsortWhiteName {White Name}
# ====== TODO To be translated ======
translate F GsortBlackName {Black Name}
# ====== TODO To be translated ======
translate F GsortECO {ECO}
# ====== TODO To be translated ======
translate F GsortResult {Result}
# ====== TODO To be translated ======
translate F GsortMoveCount {Move Count}
# ====== TODO To be translated ======
translate F GsortAverageElo {Average Elo}
# ====== TODO To be translated ======
translate F GsortCountry {Country}
# ====== TODO To be translated ======
translate F GsortDeleted {Deleted}
# ====== TODO To be translated ======
translate F GsortEventDate {Event Date}
# ====== TODO To be translated ======
translate F GsortWhiteElo {White Elo}
# ====== TODO To be translated ======
translate F GsortBlackElo {Black Elo}
# ====== TODO To be translated ======
translate F GsortComments {Comments}
# ====== TODO To be translated ======
translate F GsortVariations {Variations}
# ====== TODO To be translated ======
translate F GsortNAGs {NAGs}
# ====== TODO To be translated ======
translate F GsortAscending {Ascending}
# ====== TODO To be translated ======
translate F GsortDescending {Descending}
# ====== TODO To be translated ======
translate F GsortAdd {Add}
# ====== TODO To be translated ======
translate F GsortStore {Store}
# ====== TODO To be translated ======
translate F GsortLoad {Load}

# menu shown with right mouse button down on game list.
translate F GlistRemoveThisGameFromFilter  {Enlever cette partie du filtre}
translate F GlistRemoveGameAndAboveFromFilter  {Enlever cette partie (et celles au-dessus) du filtre}
translate F GlistRemoveGameAndBelowFromFilter  {Enlever cette partie (et celles en-dessous) du filtre}
translate F GlistDeleteGame {(Dés-)effacer cette partie} 
translate F GlistDeleteAllGames {Effacer toutes les parties du filtre} 
translate F GlistUndeleteAllGames {Dés-effacer toutes les parties du filtre} 
translate F GlistMergeGameInBase {Fusionner la partie dans une autre base} 

# Maintenance window:
translate F DatabaseName {Nom de base:}
translate F TypeIcon {Type icône:}
translate F NumOfGames {Parties:}
translate F NumDeletedGames {Parties supprimées:}
translate F NumFilterGames {Parties dans le filtre:}
translate F YearRange {Années:}
translate F RatingRange {Classements:}
translate F Description {Description}
translate F Flag {Drapeau}
translate F CustomFlags {Drapeaux personnalisés}
translate F DeleteCurrent {Effacer la partie courante}
translate F DeleteFilter {Effacer le filtre de parties}
translate F DeleteAll {Effacer toutes les parties}
translate F UndeleteCurrent {Récupérer la partie en cours}
translate F UndeleteFilter {Récupérer les parties filtrées}
translate F UndeleteAll {Récupérer toutes les parties}
translate F DeleteTwins {Effacer les parties en doublon}
translate F MarkCurrent {Sélectionner la partie en cours}
translate F MarkFilter {Sélectionner les parties filtrées}
translate F MarkAll {Sélectionner toutes les parties}
translate F UnmarkCurrent {Désélectionner la partie en cours}
translate F UnmarkFilter {Désélectionner les parties filtrées}
translate F UnmarkAll {Désélectionner toutes les parties}
translate F Spellchecking {Vérification orthographique}
translate F Players {Joueurs}
translate F Events {Événements}
translate F Sites {Lieux}
translate F Rounds {Rondes}
translate F DatabaseOps {Opérations sur la base}
translate F ReclassifyGames {ECO-Classifier les parties...}
translate F CompactDatabase {Compacter la base}
translate F SortDatabase {Trier la base}
translate F AddEloRatings {Ajouter les classements Elo}
translate F AutoloadGame {Chargement automatique du numéro de partie}
translate F StripTags {Enlever les marqueurs PGN}
translate F StripTag {Enlever les marqueurs}
translate F Cleaner {Nettoyer}
translate F CleanerHelp {
Le Nettoyeur Scid va réaliser pour la base en cours toutes les actions de maintenance que vous avez sélectionné à partir de la liste ci-dessous.

Les réglages en cours de la classification ECO et les dialogues d'effacement des jumeaux vont s'appliquer si vous avez sélectionné ces fonctions.
}
translate F CleanerConfirm {
Une fois que le nettoyage est commencé, il ne peut être interrompu!

Cela peut prendre beaucoup de temps sur une grande base, suivant les fonctions que vous avez sélectionné et leurs réglages en cours.

Etes vous sûr que vous voulez commencer les fonctions de maintenance que vous avez sélectionné?
}

translate F TwinCheckUndelete {pour permuter; "u" pour annuler l'effacement des deux}
translate F TwinCheckprevPair {Paire précédente}
translate F TwinChecknextPair {Paire suivante}
translate F TwinChecker {Scid: vérificateur de doublon}
translate F TwinCheckTournament {Parties dans le tournoi:}
translate F TwinCheckNoTwin {Pas de doublon  }
translate F TwinCheckNoTwinfound {Aucun doublon n'a été détecté pour ce jeu.\nPour afficher les doublon dans cette fenêtre, vous devez d'abord utiliser la fonction "Purger les doublon...". }
translate F TwinCheckTag {Partager tags...}
translate F TwinCheckFound1 {Scid a trouvé $result doublons}
translate F TwinCheckFound2 { et positionné le marqueur de suppression}
translate F TwinCheckNoDelete {Il n'y a pas de parties dans cette base à effacer.}
translate F TwinCriteria1 { Les paramètres pour trouver des doublons risquent de\nfaire que des parties différentes avec des coups similaires soient marquées comme des doublons.}
translate F TwinCriteria2 {Il est recommandé de sélectionner "Non" pour "mêmes coups", vous devriez sélectionner "Oui" pour les couleurs, événement, site, ronde, année et mois.\nVoulez-vous continuer et effacer les doublons quand même ? }
translate F TwinCriteria3 {Il est recommandé de sélectionner "Oui" pour au moins deux des paramètres "même site", "même ronde" et "même année".\nVoulez-vous continuer et effacer les doublons quand même ?}
translate F TwinCriteriaConfirm {Scid: Confirmer les paramètres de doublons}
translate F TwinChangeTag "Changer les tags de partie suivants:\n\n"
translate F AllocRatingDescription "Cette commande va utiliser les fichiers de vérification de noms et de classement Elo pour les parties de cette base. Chaque fois qu'un joueur n'a pas de classement mais que son classement est listé dans le fichier de vérification de noms à la date de la partie, ce classement est pris en compte."
translate F RatingOverride "Remplacer le classement non nul existant"
translate F AddRatings "ajouter classement à:"
translate F AddedRatings {Scid a ajouté $r classements Elo dans $g parties.}
translate F NewSubmenu "Nouveau sous-menu"

# Comment editor:
translate F AnnotationSymbols {Symboles d'annotation:}
translate F Comment {Commentaire:}
translate F InsertMark {Insère une marque}
translate F InsertMarkHelp {
Insérer/enlever marque: Sélectionner la couleur, le type et la case.
Insérer/enlever flèche: Clic droit sur deux cases.
}

# Nag buttons in comment editor:
translate F GoodMove {Bon coup}
translate F PoorMove {Coup faible}
translate F ExcellentMove {Excellent coup}
translate F Blunder {Gaffe}
translate F InterestingMove {Coup intéressant}
translate F DubiousMove {coup douteux}
translate F WhiteDecisiveAdvantage {Les blancs ont un avantage décisif}
translate F BlackDecisiveAdvantage {Les noirs ont un avantage décisif}
translate F WhiteClearAdvantage {Les blancs ont l'avantage}
translate F BlackClearAdvantage {Les noirs ont l'avantage}
translate F WhiteSlightAdvantage {Les blancs ont un léger avantage}
translate F BlackSlightAdvantage {Les noirs ont un léger avantage}
translate F Equality {Egalité}
translate F Unclear {Peu clair}
translate F Diagram {Diagramme}

# Board search:
translate F BoardSearch {Rechercher selon la position actuelle}
translate F FilterOperation {Opération sur le filtre en cours :}
translate F FilterAnd {ET (Restreint le filtre)}
translate F FilterOr {OU (Ajoute au filtre)}
translate F FilterIgnore {IGNORER (Restaure le filtre initial)}
translate F SearchType {Type de recherche :}
translate F SearchBoardExact {Position exacte, mêmes pièces sur les mêmes cases.}
translate F SearchBoardPawns {Pions, même matériel avec tous les pions sur les mêmes cases.}
translate F SearchBoardFiles {Colonnes, même matériel avec tous les pions sur mêmes colonnes.}
translate F SearchBoardAny {Même matériel, les pions et pièces n'importe où.}
translate F SearchInRefDatabase { Dans la base de référence }
translate F LookInVars {Chercher dans les variantes}

# Material search:
translate F MaterialSearch {Rechercher un matériel ou motif}
translate F Material {Matériel}
translate F Patterns {Motifs}
translate F Zero {Zéro}
translate F Any {Tout}
translate F CurrentBoard {Position en cours}
translate F CommonEndings {Finales}
translate F CommonPatterns {Motifs}
translate F MaterialDiff {Différence en matériel}
translate F squares {cases}
translate F SameColor {Mêmes couleur}
translate F OppColor {Couleurs opposées}
translate F Either {Les deux}
translate F MoveNumberRange {Déplacer de X coups}
translate F MatchForAtLeast {Correspond à la dernière}
translate F HalfMoves {demi-coups}

# Common endings in material search:
translate F EndingPawns {Finales de pions}
translate F EndingRookVsPawns {Tour contre pion(s)}
translate F EndingRookPawnVsRook {Tour et 1 pion contre Tour}
translate F EndingRookPawnsVsRook {Tour et pion(s) contre Tour}
translate F EndingRooks {Finales Tour contre Tour}
translate F EndingRooksPassedA {Finales de Tour contre Tour avec un pion a passé}
translate F EndingRooksDouble {Finales 2 tours}
translate F EndingBishops {Finales Fou contre Fou}
translate F EndingBishopVsKnight {Finales Fou contre Cavalier}
translate F EndingKnights {Finales Cavalier contre Cavalier}
translate F EndingQueens {Finales Dame contre Dame}
translate F EndingQueenPawnVsQueen {Dame et 1 pion contre Dame}
translate F BishopPairVsKnightPair {Milieu de jeu 2 Fous contre 2 Cavaliers}

# Common patterns in material search:
translate F PatternWhiteIQP {White IQP} ;# ***
translate F PatternWhiteIQPBreakE6 {White IQP: d4-d5 break vs. e6} ;# ***
translate F PatternWhiteIQPBreakC6 {White IQP: d4-d5 break vs. c6} ;# ***
translate F PatternBlackIQP {Black IQP} ;# ***
translate F PatternWhiteBlackIQP {White IQP vs. Black IQP} ;# ***
translate F PatternCoupleC3D4 {White c3+d4 Isolated Pawn Couple} ;# ***
translate F PatternHangingC5D5 {Black Hanging Pawns on c5 and d5} ;# ***
translate F PatternMaroczy {Maroczy Center (with Pawns on c4 and e4)} ;# ***
translate F PatternRookSacC3 {Rook Sacrifice on c3} ;# ***
translate F PatternKc1Kg8 {O-O-O vs. O-O (Kc1 vs. Kg8)} ;# ***
translate F PatternKg1Kc8 {O-O vs. O-O-O (Kg1 vs. Kc8)} ;# ***
translate F PatternLightFian {Light-Square Fianchettos (Bishop-g2 vs. Bishop-b7)} ;# ***
translate F PatternDarkFian {Dark-Square Fianchettos (Bishop-b2 vs. Bishop-g7)} ;# ***
translate F PatternFourFian {Four Fianchettos (Bishops on b2,g2,b7,g7)} ;# ***

# Game saving:
translate F Today {Aujourd'hui}
translate F ClassifyGame {Classer la partie}

# Setup position:
translate F EmptyBoard {Vider l'échiquier}
translate F InitialBoard {Réinitialiser l'échiquier}
translate F SideToMove {Côté qui a le trait}
translate F MoveNumber {Coup numéro}
translate F Castling {Roque}
translate F EnPassantFile {Prise en passant}
translate F ClearFen {Effacer FEN}
translate F PasteFen {Coller FEN}
translate F SaveAndContinue {Enregistrer et continuer}
translate F DiscardChangesAndContinue {Ignorer changements\net continuer}
translate F GoBack {Revenir}

# Replace move dialog:
translate F ReplaceMove {Remplacer le coup}
translate F AddNewVar {Ajouter variante}
translate F NewMainLine {Ligne principale}
translate F ReplaceMoveMessage {Un coup existe déjà.

Vous pouvez le remplacer, ou bien détruire tous les coups après ce coup ou ajouter une nouvelle variante.

(Vous pouvez demander à ne plus voir ce message à l'avenir, en désélectionnant l'option "Demander avant de remplacer le coup" dans le menu des options)}

# Make database read-only dialog:
translate F ReadOnlyDialog {Si vous mettez cette base en lecture seule, aucun changement ne sera permis.
Aucune partie ne peut être sauvée ou remplacée, et aucun drapeau d'effacement ne peut être altéré.
Tout les tris ou les résultats de classification ECO seront temporaires.

Vous pouvez facilement rendre la base à nouveau accessible en écriture, en la fermant et en l'ouvrant à nouveau.

Voulez vous vraiment passer cette base en lecture seule?}

# Clear game dialog:
translate F ClearGameDialog {Cette partie a été modifiée.

Voulez vous vraiment continuer et ignorer les changements effectués?
}

# Exit dialog:
translate F ExitDialog {Voulez vous réellement quitter Scid?}
translate F ExitUnsaved {Les bases de données suivantes ont des parties modifiées non sauvegardées. Si vous quittez maintenant, ces modifications seront perdues.}

# Import window:
translate F PasteCurrentGame {Coller la partie en cours}
translate F ImportHelp1 {Entrer ou coller une partie au format PGN dans le cadre ci-dessus.}
translate F ImportHelp2 {Toutes les erreurs durant l'import de la partie seront affichées ici.}
translate F OverwriteExistingMoves {Effacer les coups existants ?}

# ECO Browser:
translate F ECOAllSections {toutes les sections ECO}
translate F ECOSection {section ECO}
translate F ECOSummary {Résumé pour}
translate F ECOFrequency {Fréquence des sous-codes pour}

# Opening Report:
translate F OprepTitle {Rapport sur l'ouverture}
translate F OprepReport {Rapport}
translate F OprepGenerated {Généré par}
translate F OprepStatsHist {Statistiques et Historique}
translate F OprepStats {Statistiques}
translate F OprepStatAll {Toutes les parties du rapport}
translate F OprepStatBoth {Les deux joueurs classés}
translate F OprepStatSince {Depuis}
translate F OprepOldest {Les parties les plus anciennes}
translate F OprepNewest {Les parties les plus récentes}
translate F OprepPopular {Popularité courante}
translate F OprepFreqAll {Fréquence pour toutes les années: }
translate F OprepFreq1   {Depuis 1 an jusqu'à ce jour:      }
translate F OprepFreq5   {Depuis 5 ans jusqu'à ce jour:     }
translate F OprepFreq10  {Depuis 10 ans jusqu'à ce jour:    }
translate F OprepEvery {une fois toutes les %u parties}
translate F OprepUp {%u%s rencontré pour l'ensemble des années}
translate F OprepDown {%u%s non rencontré l'ensemble des années}
translate F OprepSame {sans changement par rapport à l'ensemble des années}
translate F OprepMostFrequent {Joueurs les plus fréquents}
translate F OprepMostFrequentOpponents {Adversaires les plus fréquents}
translate F OprepRatingsPerf {Classements et performance}
translate F OprepAvgPerf {Classements moyens et performance}
translate F OprepWRating {Classement des blancs}
translate F OprepBRating {Classement des noirs}
translate F OprepWPerf {Performance des blancs}
translate F OprepBPerf {Performance des noirs}
translate F OprepHighRating {Parties avec le classement moyen le plus élevé}
translate F OprepTrends {Tendances de résultat}
translate F OprepResults {Résultats longueurs et fréquences}
translate F OprepLength {Longueur de partie}
translate F OprepFrequency {Fréquence}
translate F OprepWWins {Gains blancs: }
translate F OprepBWins {Gains noirs:  }
translate F OprepDraws {Nulles:       }
translate F OprepWholeDB {base complète}
translate F OprepShortest {Gains les plus rapides}
translate F OprepMovesThemes {Coups et Thèmes}
translate F OprepMoveOrders {Ordres de coups atteignant la position du rapport}
translate F OprepMoveOrdersOne \
  {Il n'y a qu'un seul ordre de coups pour atteindre cette position:}
translate F OprepMoveOrdersAll \
  {Il y a %u ordres de coups pour atteindre cette position:}
translate F OprepMoveOrdersMany \
  {Il y a %u ordres de coups pour atteindre cette position. Les %u premiers sont:}
translate F OprepMovesFrom {Coups depuis la position du rapport}
translate F OprepMostFrequentEcoCodes {Codes ECO les plus fréquents}
translate F OprepThemes {Thèmes positionnels}
translate F OprepThemeDescription {Fréquence des thèmes dans les premiers %u coups de chaque partie}
translate F OprepThemeSameCastling {Roques du même côté}
translate F OprepThemeOppCastling {Roques du côté opposé}
translate F OprepThemeNoCastling {Aucun roque}
translate F OprepThemeKPawnStorm {Avalanche de pions à l'aile roi}
translate F OprepThemeQueenswap {Dames échangées}
translate F OprepThemeWIQP {Pion Dame blanc isolé}
translate F OprepThemeBIQP {Pion Dame noir isolé}
translate F OprepThemeWP567 {Pion blanc sur la 5/6/7e rangée}
translate F OprepThemeBP234 {Pion noir sur la 2/3/4e rangée}
translate F OprepThemeOpenCDE {Colonne c/d/e ouverte}
translate F OprepTheme1BishopPair {Paire de fous}
translate F OprepEndgames {Finales}
translate F OprepReportGames {Les parties du rapport}
translate F OprepAllGames {Toutes les parties}
translate F OprepEndClass {Classification du matériel des positions finales}
translate F OprepTheoryTable {Table de Théorie}
translate F OprepTableComment {Généré à partir des %u parties au classement le plus élevé.}
translate F OprepExtraMoves {Nombre de coups additionnels dans la table de théorie}
translate F OprepMaxGames {Nombre maximum de parties dans la table de théorie}
translate F OprepViewHTML {Voir en HTML}
translate F OprepViewLaTeX {Voir en LaTeX}

# Player Report:
translate F PReportTitle {Rapport sur le joueur}
translate F PReportColorWhite {avec les Blancs pièces}
translate F PReportColorBlack {avec les Noirs pièces}
translate F PReportMoves {après %s}
translate F PReportOpenings {Ouvertures}
translate F PReportClipbase {Vider la Clipbase et copier dedans toutes les parties qui correspondent}

# Piece Tracker window:
translate F TrackerSelectSingle {Le bouton gauche de la souris sélectionne cette pièce.}
translate F TrackerSelectPair {Le bouton gauche de la souris sélectionne cette pièce; le bouton droit sélectionne aussi son pendant.}
translate F TrackerSelectPawn {Le bouton gauche de la souris sélectionne ce pion ; le bouton droit sélectionne les 8 pions.}
translate F TrackerStat {Statistiques}
translate F TrackerGames {% parties avec déplacement sur la case}
translate F TrackerTime {% fois sur chaque case}
translate F TrackerMoves {Coups}
translate F TrackerMovesStart {Entrer le numéro du coups à partir duquel la recherche doit commencer.}
translate F TrackerMovesStop {Entrer le numéro du coups auquel la recherche doit s'arrêter.}

# Game selection dialogs:
translate F SelectAllGames {Toutes les parties de la base}
translate F SelectFilterGames {Seulement les parties du filtre}
translate F SelectTournamentGames {Seulement les parties du tournoi courant}
translate F SelectOlderGames {Seulement les parties plus anciennes}

# Delete Twins window:
translate F TwinsNote {Pour être considérées comme doublons, deux parties doivent au moins avoir les deux mêmes joueurs, et avoir les critères que vous pouvez fixer ci-dessous satisfaits. Quand une paire de doublons est trouvée, la partie la plus courte est effacée. Conseil: il est préférable de vérifier l'orthographe dans la base avant d'effacer les doublons, car cela améliore la détection des doublons. }
translate F TwinsCriteria {Critère: Les doublons doivent avoir...}
translate F TwinsWhich {Parties à examiner}
translate F TwinsColors {Même couleurs de joueurs}
translate F TwinsEvent {Même événement}
translate F TwinsSite {Même lieu}
translate F TwinsRound {Même ronde}
translate F TwinsYear {Même année}
translate F TwinsMonth {Même mois}
translate F TwinsDay {Même jour}
translate F TwinsResult {Même résultat}
translate F TwinsECO {Même code ECO}
translate F TwinsMoves {Mêmes coups}
translate F TwinsPlayers {Comparaison des noms des joueurs}
translate F TwinsPlayersExact {Correspondance exacte}
translate F TwinsPlayersPrefix {Les 4 premières lettres seules}
translate F TwinsWhen {En effacant les doublons}
translate F TwinsSkipShort {Ignorer toutes les parties de moins de cinq coups}
translate F TwinsUndelete {Récupérer toutes les parties d'abord}
translate F TwinsSetFilter {Appliquer le filtre à tous les doublons éliminés}
translate F TwinsComments {Toujours garder les parties qui comportent des commentaires}
translate F TwinsVars {Toujours garder les parties qui comportent des variantes}
translate F TwinsDeleteWhich {Partie à effacer}
translate F TwinsDeleteShorter {La partie la plus courte}
translate F TwinsDeleteOlder {La partie avec le plus petit numéro}
translate F TwinsDeleteNewer {La partie avec le plus grand numéro}
translate F TwinsDelete {Effacer les doublons}

# Name editor window:
translate F NameEditType {Taper le nom à éditer}
translate F NameEditSelect {Parties à éditer}
translate F NameEditReplace {Remplacer}
translate F NameEditWith {avec}
translate F NameEditMatches {Correspondances: Presser Alt+1 à Alt+9 pour sélectionner}
# ====== TODO To be translated ======
translate F CheckGames {Check games}
# ====== TODO To be translated ======
translate F CheckGamesWhich {Check games}
# ====== TODO To be translated ======
translate F CheckAll {All games}
# ====== TODO To be translated ======
translate F CheckSelectFilterGames {Only games in filter}

# Classify window:
translate F Classify {Classer}
translate F ClassifyWhich {Choix des parties à classer suivant ECO}
translate F ClassifyAll {Toutes les parties (écrase les anciens codes ECO)}
translate F ClassifyYear {Toutes les parties jouées la dernière année}
translate F ClassifyMonth {Toutes les parties jouées le dernier mois}
translate F ClassifyNew {Seulement les parties qui ne possèdent pas encore de code ECO}
translate F ClassifyCodes {Codes ECO à utiliser}
translate F ClassifyBasic {Codes de base seulement ("B12", ...)}
translate F ClassifyExtended {Extensions de Scid ("B12j", ...)}

# Compaction:
translate F NameFile {Fichier de noms}
translate F GameFile {Fichier de parties}
translate F Names {Noms}
translate F Unused {Non utilisé}
translate F SizeKb {Taille (kb)}
translate F CurrentState {État courant}
translate F AfterCompaction {Après compactage}
translate F CompactNames {Compacter le fichier de noms}
translate F CompactGames {Compacter le fichier de parties}
translate F NoUnusedNames "Il n'y a pas de noms inutilisés, le fichier de noms est donc déjà compacté."
translate F NoUnusedGames "Le fichier des parties est déjà compacté."
translate F GameFileCompacted {Le fichier de parties de la base a été compacté.}

# Sorting:
translate F SortCriteria {Critère}
translate F AddCriteria {Ajouter un critère}
translate F CommonSorts {Tris courants}
translate F Sort {Tri}

# Exporting:
translate F AddToExistingFile {Ajouter les parties à un fichier existant?}
translate F ExportComments {Exporter les commentaires?}
translate F ExportVariations {Exporter les variantes?}
translate F IndentComments {Indenter les commentaires?}
translate F IndentVariations {Indenter les variantes?}
translate F ExportColumnStyle {Style en colonne (un coup par ligne)?}
translate F ExportSymbolStyle {Style d'annotation symbolique:}
translate F ExportStripMarks {Enlever les codes de marquages de flèches et de coloration de cases des commentaires?}

# Goto game/move dialogs:
translate F LoadGameNumber {Entrer le numéro de la partie à charger:}
translate F GotoMoveNumber {Aller au coup numéro:}

# Copy games dialog:
translate F CopyGames {Copier les parties}
translate F CopyConfirm {
 Voulez vous vraiment copier
 les [::utils::thousands $nGamesToCopy] parties filtrées
 depuis la base "$fromName"
 vers la base "$targetName"?
}
translate F CopyErr {Ne peut copier les parties}
translate F CopyErrSource {la base source}
translate F CopyErrTarget {la base destination}
translate F CopyErrNoGames {n'a pas de parties dans son filtre}
translate F CopyErrReadOnly {est en lecture seule}
translate F CopyErrNotOpen {n'est pas ouverte}

# Colors:
translate F LightSquares {Cases blanches}
translate F DarkSquares {Cases noires}
translate F SelectedSquares {Cases sélectionnées}
translate F SuggestedSquares {Cases des coups suggérés}
translate F WhitePieces {Pièces blanches}
translate F BlackPieces {Pièces noires}
translate F WhiteBorder {Bordure des pièces blanches}
translate F BlackBorder {Bordure des pièces noires}

# Novelty window:
translate F FindNovelty {Trouver la nouveauté}
translate F Novelty {Nouveauté}
translate F NoveltyInterrupt {Recherche de nouveauté interrompue}
translate F NoveltyNone {Aucune nouveauté n'a été trouvée pour cette partie}
translate F NoveltyHelp {Scid va trouver le premier coup de la partie en cours qui atteint une position qui ne figure ni dans la base sélectionnée ni dans le répertoire d'ouvertures ECO.}

# Sounds configuration:
translate F SoundsFolder {Répertoire des fichiers sonores}
translate F SoundsFolderHelp {Le répertoire devrait contenir les fichiers King.wav, a.wav, 1.wav, etc}
translate F SoundsAnnounceOptions {Options d'annonce des coups}
translate F SoundsAnnounceNew {Annoncer les coups joués}
translate F SoundsAnnounceForward {Annoncer les coups quand avance d'un coup}
translate F SoundsAnnounceBack {Annoncer quand retour arrière d'un coup}
translate F SoundsSoundDisabled {Scid n'a pas pu trouver le paquet audio de Snack au démarrage;\nle son est désactivé.}

# Upgrading databases:
translate F Upgrading {Mise à jour}
translate F ConfirmOpenNew {
Ceci est une base de données à l'ancien format (Scid 3) qui ne peut être ouverte dans Scid 4, mais une version au nouveau format (Scid 4) a déjà été crée.

Voulez vous ouvrir le nouveau format de la base de données?
}
translate F ConfirmUpgrade {
Ceci est une base de données à l'ancien format (Scid 3). Une version de la base de données au nouveau format doit être créée avant de pouvoir être utilisée dans Scid 4.

Mettre à jour va créer une nouvelle version de la base de données; les fichiers originaux seront effacés.

Cela peut prendre un peu de temps, mais il n'est nécessaire de le faire qu'une fois. Vous pouvez abandonner si cela dure trop longtemps.

Voulez vous mettre à jour cette base de données maintenant?
}

# Recent files options:
translate F RecentFilesMenu {Nombre de fichiers récents dans le menu Fichier}
translate F RecentFilesExtra {Nombre de fichiers récents dans le sous-menu complémentaire}

# My Player Names options:
translate F MyPlayerNamesDescription {
Entrer ci-dessous une liste des noms des joueurs préférés, un nom par ligne. Les caractères spéciaux (i.e. "?" pour un seul caractère, "*" pour n'importe quelle suite de caractères) sont autorisés.

Chaque fois qu'une partie avec un joueur de la liste est chargée, l'échiquier de la fenêtre principale sera tourné si nécessaire de façon à montrer la partie selon le point de vue du joueur.
}
#Coach
translate F showblunderexists {Montrer si erreur}
translate F showblundervalue {Montrer la valeur de l'erreur}
translate F showscore {Montrer score}
translate F coachgame {Entraînement}
translate F configurecoachgame {Configurer une partie avec entraîneur}
translate F configuregame {Configurer une partie sérieuse}
translate F Phalanxengine {moteur Phalanx}
translate F Coachengine {entraîneur}
translate F difficulty {difficulté}
translate F hard {difficile}
translate F easy {facile}
translate F Playwith {Jouer avec}
translate F white {blanc}
translate F black {noir}
translate F both {les deux}
translate F Play {Jouer}
translate F Noblunder {Pas d'erreur}
translate F blunder {erreur}
translate F Noinfo {-- Pas d'info --}
translate F PhalanxOrTogaMissing {Phalanx ou Toga non trouvé}
translate F moveblunderthreshold {le coup est une erreur si les pertes sont supérieures à }
translate F limitanalysis {limiter le temps d'analyse}
translate F seconds {secondes}
translate F Abort {Arrêter}
translate F Resume {Reprendre}
translate F OutOfOpening {Hors de l'ouverture}
translate F NotFollowedLine {Vous n'avez pas suivi la ligne}
translate F DoYouWantContinue {Voulez-vous continuer ?}
translate F CoachIsWatching {L'entraîneur regarde}
translate F Ponder {Réflexion permanente}
translate F LimitELO {Limiter la force Elo}
translate F DubiousMovePlayedTakeBack {Coup douteux joué, voulez-vous reprendre le coup ?}
translate F WeakMovePlayedTakeBack {Coup faible joué, voulez-vous reprendre le coup ?}
translate F BadMovePlayedTakeBack {Mauvais coup joué, voulez-vous reprendre le coup ?}
translate F Iresign {J'abandonne}
translate F yourmoveisnotgood {Votre coup n'est pas bon}
translate F EndOfVar {Fin de variante}
translate F Openingtrainer {Entraînement aux ouvertures}
translate F DisplayCM {Afficher les coups candidats}
translate F DisplayCMValue {Afficher la valeur des coups candidats}
translate F DisplayOpeningStats {Montrer statistiques}
translate F ShowReport {Montrer rapport}
translate F NumberOfGoodMovesPlayed {bons coups joués}
translate F NumberOfDubiousMovesPlayed {coups douteux joués}
translate F NumberOfMovesPlayedNotInRepertoire {coups joués absents du répertoire}
translate F NumberOfTimesPositionEncountered {nombre de fois position rencontrée}
translate F PlayerBestMove  {N'autoriser que les meilleurs coups}
translate F OpponentBestMove {Ne jouer que les meilleurs coups}
translate F OnlyFlaggedLines {Seulement les lignes marquées}
translate F resetStats {Remettre à zéro les scores}
translate F Repertoiretrainingconfiguration {Configuration entraînement répertoire}
translate F Loadingrepertoire {Chargement du répertoire}
translate F Movesloaded {Coups lus}
translate F Repertoirenotfound {Répertoire introuvable}
translate F Openfirstrepertoirewithtype {Ouvrez d'abord une base avec le bon type/icône sur le côté droit}
translate F Movenotinrepertoire {Coup absent du répertoire}
translate F PositionsInRepertoire {Positions dans le répertoire}
translate F PositionsNotPlayed {Positions non jouées}
translate F PositionsPlayed {Positions jouées}
translate F Success {Succès}
translate F DubiousMoves {Coups douteux}
translate F OutOfRepertoire {Hors du répertoire}
translate F ConfigureTactics {Configurer les exercices tactiques}
translate F ResetScores {Remise à zéro des scores}
translate F LoadingBase {Chargement de la base}
translate F Tactics {Tactique}
translate F ShowSolution {Montrer la solution}
translate F NextExercise {Exercice suivant}
translate F PrevExercise {Exercice précédent}
translate F StopTraining {Arrêter l'entraînement}
translate F Next {Suivant}
translate F ResettingScore {Remise à zéro des scores}
translate F LoadingGame {Chargement partie}
translate F MateFound {Mat trouvé}
translate F BestSolutionNotFound {Meilleur coup non trouvé !}
translate F MateNotFound {Mat non trouvé}
translate F ShorterMateExists {Un mat plus rapide existe}
translate F ScorePlayed {score coup joué}
translate F Expected {attendu}
translate F ChooseTrainingBase {Choisir la base d'exercices}
translate F Thinking {Calcul en cours}
translate F AnalyzeDone {Analyse terminée}
translate F WinWonGame {Terminer partie gagnée}
translate F Lines {Lignes}
translate F ConfigureUCIengine {Configurer le moteur UCI}
translate F SpecificOpening {Ouverture spécifique}
translate F StartNewGame {Nouvelle partie}
translate F FixedLevel {Niveau fixe}
translate F Opening {Ouverture}
translate F RandomLevel {Niveau aléatoire}
translate F StartFromCurrentPosition {Jouer depuis la position actuelle}
translate F FixedDepth {Profondeur fixe}
translate F Nodes {Noeuds} 
translate F Depth {Profondeur}
translate F Time {Temps} 
translate F SecondsPerMove {Secondes par coup}
translate F Engine {Moteur}
translate F TimeMode {Mode de réflexion}
translate F TimeBonus {Temps + bonus}
translate F TimeMin {min}
translate F TimeSec {sec}
translate F AllExercisesDone {Tous les exercices sont faits}
translate F MoveOutOfBook {Coup hors bibliothèque d'ouverture}
translate F LastBookMove {Dernier coup de la bibliothèque d'ouverture}
translate F AnnotateSeveralGames {Annoter plusieurs parties\ndepuis l'actuelle jusqu'à :}
translate F FindOpeningErrors {Chercher les erreurs d'ouverture}
translate F MarkTacticalExercises {Marquer exercice tactique}
translate F UseBook {Utiliser une bibliothèque d'ouverture}
translate F MultiPV {Nombre de variantes}
translate F Hash {Mémoire}
translate F OwnBook {Utiliser bibliothèque propre}
translate F BookFile {Bibliothèque d'ouverture}
translate F AnnotateVariations {Annoter les variantes}
translate F ShortAnnotations {Annotations courtes}
translate F addAnnotatorTag {Ajouter Annotateur}
translate F AddScoreToShortAnnotations {Ajouter score aux annotations courtes}
translate F Export {Exporter}
translate F BookPartiallyLoaded {Bibliothèque chargée en partie}
translate F Calvar {Calcul de variantes}
translate F ConfigureCalvar {Configuration}
# Opening names used in tacgame.tcl
translate F Reti {Reti}
translate F English {Anglaise}
translate F d4Nf6Miscellaneous {1.d4 Cf6 divers}
translate F Trompowsky {Trompowsky}
translate F Budapest {Budapest}
translate F OldIndian {Vieille Indienne}
translate F BenkoGambit {Gambit Benkö}
translate F ModernBenoni {Benoni Moderne}
translate F DutchDefence {Défense hollandaise}
translate F Scandinavian {Scandinave}
translate F AlekhineDefence {Défense Alekhine}
translate F Pirc {Pirc}
translate F CaroKann {Caro-Kann}
translate F CaroKannAdvance {Caro-Kann Avance}
translate F Sicilian {Sicilienne}
translate F SicilianAlapin {Sicilienne Alapin}
translate F SicilianClosed {Sicilienne fermée}
translate F SicilianRauzer {Sicilienne Rauzer}
translate F SicilianDragon {Sicilienne Dragon}
translate F SicilianScheveningen {Sicilienne Scheveningen}
translate F SicilianNajdorf {Sicilienne Najdorf}
translate F OpenGame {Partie ouverte}
translate F Vienna {Viennoise}
translate F KingsGambit {Gambit Roi}
translate F RussianGame {Petroff}
translate F ItalianTwoKnights {Italienne/Deux cavaliers}
translate F Spanish {Espagnole}
translate F SpanishExchange {Espagnole échange}
translate F SpanishOpen {Espagnole ouverte}
translate F SpanishClosed {Espagnole fermée}
translate F FrenchDefence {Défense française}
translate F FrenchAdvance {Défense française Avance}
translate F FrenchTarrasch {Défense française Tarrasch}
translate F FrenchWinawer {Défense française Winawer}
translate F FrenchExchange {Défense française Echange}
translate F QueensPawn {Pion Dame}
translate F Slav {Slave}
translate F QGA {GD Accepté}
translate F QGD {GD Décliné}
translate F QGDExchange {GD Echange}
translate F SemiSlav {Semi-Slave}
translate F QGDwithBg5 {GD décliné avec Fg5}
translate F QGDOrthodox {GD décliné Orthodoxe}
translate F Grunfeld {Grünfeld}
translate F GrunfeldExchange {Grünfeld Echange}
translate F GrunfeldRussian {Grünfeld Russe}
translate F Catalan {Catalane}
translate F CatalanOpen {Catalane Ouverte}
translate F CatalanClosed {Catalane Fermée}
translate F QueensIndian {Ouest indienne}
translate F NimzoIndian {Nimzo-Indienne}
translate F NimzoIndianClassical {Nimzo-Indienne Classique}
translate F NimzoIndianRubinstein {Nimzo-Indienne Rubinstein}
translate F KingsIndian {Est-indienne}
translate F KingsIndianSamisch {Est-indienne Sämisch}
translate F KingsIndianMainLine {Est-indienne ligne principale}
translate F ConfigureFics {Configurer FICS}
translate F FICSGuest {Login comme invité}
translate F FICSServerPort {Port du serveur}
translate F FICSServerAddress {Adresse IP}
translate F FICSRefresh {Rafraîchir}
translate F FICSTimesealPort {Port Timeseal}
translate F FICSSilence {Silence}
translate F FICSOffers {Offres}
translate F FICSConsole {Console}
translate F FICSGames {Parties}
translate F FICSUnobserve {Arrêter de suivre la partie}
translate F FICSProfile {Afficher mon historique et mes données}
translate F FICSRelayedGames {Parties retransmises}
translate F FICSFindOpponent {Trouver adversaire}
translate F FICSTakeback {Reprendre un coup}
translate F FICSTakeback2 {Rependre deux coups}
translate F FICSInitTime {Temps initial (min)}
translate F FICSIncrement {Incrément (sec)}
translate F FICSRatedGame {Partie classée}
translate F FICSAutoColour {automatique}
translate F FICSManualConfirm {accepter manuellement}
translate F FICSFilterFormula {Filtrer avec formule}
translate F FICSIssueSeek {Envoyer seek}
translate F FICSChallenge {Challenge}
translate F FICSAccept {accepter}
translate F FICSDecline {refuser}
translate F FICSColour {Couleur}
translate F FICSSend {envoyer}
translate F FICSConnect {Connecter}
translate F FICSdefaultuservars {Variables par défaut}
translate F FICSObserveconfirm {Voulez-vous suivre la partie}
translate F FICSpremove {Autoriser le premove}
translate F GameReview {Revue de partie}
translate F GameReviewTimeExtended {Temps étendu}
translate F GameReviewMargin {Marge d'erreur}
translate F GameReviewAutoContinue {Continuer quand le coup est correct}
translate F GameReviewReCalculate {Utiliser le temps étendu}
translate F GameReviewAnalyzingMovePlayedDuringTheGame {Analyse du coup joué pendant la partie}
translate F GameReviewAnalyzingThePosition {Analyse de la position}
translate F GameReviewEnterYourMove {Jouez votre coup}
translate F GameReviewCheckingYourMove {Vérification de votre coup}
translate F GameReviewYourMoveWasAnalyzed {Votre coup a été analysé}
translate F GameReviewYouPlayedSameMove {Vous avez joué le même coup que dans la partie}
translate F GameReviewScoreOfYourMove {Score de votre coup}
translate F GameReviewGameMoveScore {Score du coup de la partie}
translate F GameReviewEngineScore {Score du moteur}
translate F GameReviewYouPlayedLikeTheEngine {Vous avez joué un coup aussi bon que le moteur}
translate F GameReviewNotEngineMoveButGoodMove {Pas le coup du moteur, mais un coup jouable}
translate F GameReviewMoveNotGood {Ce coup n'est pas bon, le score est}
translate F GameReviewMovesPlayedLike {Coups joués comme}
translate F GameReviewMovesPlayedEngine {coups joués comme le moteur}
translate F CCDlgConfigureWindowTitle {Configurer échecs par correspondance}
translate F CCDlgCGeneraloptions {Options générales}
translate F CCDlgDefaultDB {Base par défaut:}
translate F CCDlgInbox {Réception (chemin):}
translate F CCDlgOutbox {Envoi (chemin):}
translate F CCDlgXfcc {Configuration Xfcc:}
translate F CCDlgExternalProtocol {Protocole externe (par ex. Xfcc)}
translate F CCDlgFetchTool {Outil de réception:}
translate F CCDlgSendTool {Outil d'envoi:}
translate F CCDlgEmailCommunication {Communication e-mail}
translate F CCDlgMailPrg {Programme de mail:}
translate F CCDlgBCCAddr {Addresse de copie:}
translate F CCDlgMailerMode {Mode:}
translate F CCDlgThunderbirdEg {par ex. Thunderbird, Mozilla Mail, Icedove...}
translate F CCDlgMailUrlEg {par ex. Evolution}
translate F CCDlgClawsEg {par ex. Sylpheed Claws}
translate F CCDlgmailxEg {par ex. mailx, mutt, nail...}
translate F CCDlgAttachementPar {commande Joindre:}
translate F CCDlgInternalXfcc {Utiliser Xfcc interne}
translate F CCDlgConfirmXfcc {Confirmer les coups}
translate F CCDlgSubjectPar {commande Objet:}
translate F CCDlgDeleteBoxes {Vider boîtes}
translate F CCDlgDeleteBoxesText {Voulez-vous réellement vider les boîtes entrantes et sortantes pour les échecs par correspondance ? Cela nécessite une nouvelle synchronisation pour afficher le dernier état de vos parties}
translate F CCDlgConfirmMove {Confirmer le coup}
translate F CCDlgConfirmMoveText {Si vous confirmez, le coup ci-dessous et le commentaire sont envoyés au serveur:}
translate F CCDlgDBGameToLong {Ligne principale incohérente}
translate F CCDlgDBGameToLongError {La ligne principale dans votre base de données est plus longue que la partie dans votre boîte de réception. Si la boîte de réception contient les parties en cours, c'est-à-dire juste après une synchronisation, certains coups ont été ajoutés à tort à la ligne principale dans la base de données.\nDans ce cas veuillez raccourcir la ligne principale à ce coup au plus\n}
translate F CCDlgStartEmail {Commencer une nouvelle partie par e-mail}
translate F CCDlgYourName {Votre nom:}
translate F CCDlgYourMail {Votre adresse mail:}
translate F CCDlgOpponentName {Nom adversaire:}
translate F CCDlgOpponentMail {Adresse e-mail adversaire:}
translate F CCDlgGameID {Identifiant Partie (unique):}
translate F CCDlgTitNoOutbox {Scid: Boîte d'envoi}
translate F CCDlgTitNoInbox {Scid: Boîte de réception}
translate F CCDlgTitNoGames {Scid: Pas de parties par correspondence}
translate F CCErrInboxDir {Répertoire de la boîte de réception:}
translate F CCErrOutboxDir {Répertoire de la boîte d'envoi:}
translate F CCErrDirNotUsable {n'existe pas ou inaccessible!\nVeuillez vérifier les paramètres.}
translate F CCErrNoGames {ne contient pas de parties !\nVeuillez d'abord les récupérer.}
translate F CCDlgTitNoCCDB {Scid: pas de base par correspondance}
translate F CCErrNoCCDB {Aucune base du type 'Correspondance' n'est ouverte. Ouvrez-en une avant d'utiliser les fonctions par correspondance.}
translate F CCFetchBtn {Récupérer les parties sur le serveur et les analyser dans la boîte de réception}
translate F CCPrevBtn {Aller à la partie précédente}
translate F CCNextBtn {Aller à la partie suivante}
translate F CCSendBtn {Envoyer le coup}
translate F CCEmptyBtn {Vider les boîtes d'envoi et de réception}
translate F CCHelpBtn {Aide sur les icônes et indicateurs.\nPour plus d'aide appuyer sur F1!}
translate F CCDlgServerName {Nom serveur:}
translate F CCDlgLoginName  {Login:}
translate F CCDlgPassword   {Mot de passe:}
translate F CCDlgShowPassword {Montrer le mot de passe}
translate F CCDlgURL        {Xfcc-URL:}
translate F CCDlgRatingType {Type de classement:}
translate F CCDlgDuplicateGame {Identifiant de partie non-unique}
translate F CCDlgDuplicateGameError {Cette partie existe en plusieurs exemplaires dans la base de données. Veuillez effacer tous les doublons et compacter votre fichier de parties (Fichier/Maintenance/Compacter base).}
translate F CCDlgSortOption {Tri:}
translate F CCDlgListOnlyOwnMove {Parties où c'est mon tour}
translate F CCOrderClassicTxt {Site, Evénement, Ronde, Résultat, Blancs, Noirs}
translate F CCOrderMyTimeTxt {Ma pendule}
translate F CCOrderTimePerMoveTxt {Temps par coup jusqu'au prochain contrôle de temps}
translate F CCOrderStartDate {Date de départ}
translate F CCOrderOppTimeTxt {Pendule de l'adversaire}
translate F CCDlgConfigRelay {Configurer les observations ICCF}
translate F CCDlgConfigRelayHelp {Allez à la page des jeux sur http://www.iccf-webchess.com et affichez le jeu à observer. Si vous voyez l'échiquier, copiez l'URL de votre navigateur dans la liste ci-dessous. Une seule URL par ligne !\nExemple: http://www.iccf-webchess.com/MakeAMove.aspx?id=266452}
translate F ExtHWConfigConnection {Configurer un materiel externe}
translate F ExtHWPort {Port}
translate F ExtHWEngineCmd {Commande moteur}
translate F ExtHWEngineParam {Paramètre moteur}
translate F ExtHWShowButton {Montrer bouton}
translate F ExtHWHardware {Matériel}
translate F ExtHWNovag {Novag Citrine}
translate F ExtHWInputEngine {Protocole Input Engine}
translate F ExtHWNoBoard {Pas d'échiquier}
translate F NovagReferee {Arbitre}
translate F IEConsole {Console Input Engine}
translate F IESending {Coups envoyés pour}
translate F IESynchronise {Synchronizer}
translate F IERotate  {Tourner}
translate F IEUnableToStart {Impossible de démarrer le protocole Input Engine :}
translate F DoneWithPosition {Fin de l'analyse de position}
translate F Board {Échiquier}
translate F showGameInfo {Montrer le panneau d'info}
translate F autoResizeBoard {Redimensionnement automatique}
translate F DockTop {Déplacer en haut}
translate F DockBottom {Déplacer en bas}
translate F DockLeft {Déplacer à gauche}
translate F DockRight {Déplacer à droite}
translate F Undock {Détacher}
translate F ChangeIcon {Changer l'icône...}
translate F NewGameListWindow {Nouvelle fenêtre de liste de parties}
translate F LoadatStartup {Charger au démarrage}
translate F ShowHideDB {Afficher/Masquer les bases de données}
translate F ChangeFilter {Remplacer le filtre}
translate F ChangeLayout {Charger/Enregistrer/Modifier les critères de tri et la disposition des colonnes.}
translate F ShowHideStatistic {Afficher/Masquer les statistiques}
translate F BoardFilter {Afficher uniquement les jeux qui correspondent à la position actuelle du plateau de jeu.}
translate F CopyGameTo {Copier le jeu dans}
translate F FindBar {Barre de recherche}
translate F FindCurrentGame {Trouver le jeu actuel}
translate F DeleteGame {Supprimer le jeu}
translate F UndeleteGame {Restaurer le jeu}
translate F ResetSort {Réinitialiser le tri}
translate F ConvertNullMove {Convertir les mouvements nuls en commentaires}
translate F SetupBoard {Définir la position de départ}
translate F Rotate {Rotation}
translate F SwitchColors {Changer de couleur}
translate F FlipBoard {Retourner l'échiquier}
translate F ImportPGN {Importer un jeu PGN}
translate F ImportingFiles {Importer des fichiers PGN dans}
translate F ImportingFrom {Importation de}
translate F ImportingIn {Importer des jeux dans}
translate F UseLastTag {Utilisez les tags de la dernière partie}
translate F Random {Aléatoire}
translate F BackToMainline {Revenir à la ligne principale}
translate F LeaveVariant {Quitter la variante}
translate F Autoplay {Jeu automatique}
translate F ShowHideCoords {Afficher/Masquer Coord.}
translate F ShowHideMaterial {Afficher/Masquer matériel}
translate F FullScreen {Plein écran}
# ====== TODO To be translated ======
translate F FilterStatistic {Filter Statistics}
translate F MakeCorrections {Corriger}
translate F Surnames {Noms de famille}
translate F Ambiguous {Ambigus}

#Preferences Dialog
translate F OptionsToolbar "Barre d'outils de la fenêtre principale"
translate F OptionsBoard "Échiquier"
# ====== TODO To be translated ======
translate F OptionsMenuColor "Couleurs des menus..."
translate F OptionsBoardSize "Taille d'échiquier"
translate F OptionsBoardPieces "Style des pièces"
# ====== TODO To be translated ======
translate F OptionsInternationalization "Internationalization"
#menuText F OptionsBoardColors "Couleurs..." 0 {Changer les couleurs}
translate F MenuColorForeground "Texte"
translate F MenuColorBackground "Fond"
translate F MenuColorSelect "Sélection"
translate F OptionsTablebaseDir "Sélectionnez jusqu'à 4 dossiers de tables de finales:"

##########
#
# ECO Opening name translations:

# translateECO F {
  # Accelerated {, Accéléré}
  # {: Accelerated} Accéléré
  # Accepted {, accepté}
  # {: Accepted} Accepté
  # Advance Avance
  # {as Black} {avec les Noirs}
  # Attack Attaque
  # Bishop Fou
  # Bishop's Fou
  # Classical Classique
  # Closed Fermé
  # {Closed System} {Système fermé}
  # Counterattack Contre-Attaque
  # Countergambit Contre-Gambit
  # Declined {, Décliné}
  # Defence Défense
  # deferred différé
  # Deferred {, Différé}
  # Early Tôt
  # Exchange Echange
  # Game Partie
  # Improved amélioré
  # King's Roi
  # Knight Cavalier
  # Line Ligne
  # {Main Line} Ligne principale
  # Open Ouvert
  # Opening Ouverture
  # Queen's Dame
  # Queenswap Permutation de dame
  # Symmetrical Symétrique
  # Variation Variante
  # Wing Aile
  # with avec
  # without sans
  # 
  # Alekhine Alekhine
  # Averbakh Averbakh
  # Botvinnik Botvinnik
  # Chigorin Tchigorine
  # Polugaevsky Polugaevsky
  # Rauzer Rauzer
  # Sveshnikov Sveshnikov
  # 
  # Austrian Autrichien
  # Berlin Berlinois
  # Bremen Breme
  # Catalan Catalane
  # Czech Tchèque
  # Dutch Hollandaise
  # English Anglaise
  # French Française
  # Hungarian Hongroise
  # Indian Indienne
  # Italian Italienne
  # Latvian Letton
  # Meran Meran
  # Moscow Moscou
  # Polish Polonais
  # Prague Prague
  # Russian Russe
  # Scandinavian Scandinave
  # Scheveningen Scheveningen
  # Scotch Ecossaise
  # Sicilian Sicilienne
  # Slav Slave
  # Spanish Espagnole
  # Swedish Suédois
  # Swiss Suisse
  # Vienna Viennois
  # Yugoslav Yougoslave
  # 
  # {Accelerated Fianchetto} {Fianchetto Accéléré}
  # {Accelerated Pterodactyl} {Pterodactyle Accéléré}
  # {Alekhine Defence} Défense-Alekhine
  # {Alekhine Variation} Variante-Alekhine
  # {Alekhine: Sämisch Attack} {Alekhine: Système viennois (Attaque Sämisch)}
  # {Anderssen Opening} Ouverture-Anderssen
  # {Anti-King's Indian} Anti-Est-Indienne
  # {Austrian Attack} {Attaque Autrichienne}
  # {Benko Gambit} {Gambit Benkö}
  # {Benko Opening} {Ouverture Benkö}
  # {Berlin Defence} {Défense Berlinoise}
  # Chameleon Cameleon
  # Chelyabinsk Chelyabinsk
  # {Classical Defence} {Défense Classique}
  # {Spanish: Classical Defence} {Espagnole: Défense Classique}
  # {Classical Exchange} {Echange Classique}
  # {Classical Variation} {Variante Classique}
  # {Closed Berlin} {Berlinoise Fermée}
  # {Open Berlin} {Berlinoise Ouverte}
  # {Bird's,} {Bird,}
  # {Bird's Defence} {Défense Bird}
  # {Bird's Deferred} {Bird Différée}
  # {Bishop's Opening} {Ouverture du Fou}
  # {Botvinnik System} {Système Botvinnik}
  # {Central Variation} {Variante du centre}
  # {Centre Attack} {Attaque du centre}
  # {Centre Game} {Ouverture du centre}
  # {Danish Gambit} {Gambit Danois}
  # Dragon Dragon
  # {Dutch Variation} {Variante Hollandaise}
  # {Early Exchange} {Echange rapide}
  # {Early Queenswap} {Echange de Dames rapide}
  # {English Attack} {Attaque Anglaise}
  # {English: King's} {Anglaise: Roi}
  # {English Variation} {Variante Anglaise}
  # {Englund Gambit} {Gambit Englund}
  # {Exchange Variation} {Variante d'échange}
  # {Fianchetto Variation} {Variante du Fianchetto}
  # {Flohr Variation} {Variante Flohr}
  # {Four Knights} {Quatre Cavaliers}
  # {Four Knights Game} {Partie des Quatre Cavaliers}
  # {Four Pawns} {Quatre Pions}
  # {Four Pawns Attack} {Attaque des Quatre Pions}
  # {French Variation} {Variante Française}
  # {From Gambit} {Gambit From}
  # {Goring Gambit} {Gambit Göring}
  # {Grob Gambit} {Gambit Grob}
  # {Hungarian Defence} {Défense Hongroise}
  # {Indian Variation} {Variante Indienne}
  # {Italian Game} {Partie Italienne}
  # KGD {Gambit Roi Décliné}
  # {Classical KGD} {Gambit Roi Décliné Classique}
  # {Keres Variation} {Variante Keres}
  # KGA {Gambit Roi Accepté}
  # {KGA: Bishop's Gambit} {Gambit Roi Accepté: Gambit du Fou}
  # {KGA: King's Knight Gambit} {Gambit Roi Accepté: Gambit du Cavalier}
  # {King's Gambit} {Gambit Roi}
  # {King's Gambit Accepted} {Gambit Roi Accepté}
  # {King's Gambit Accepted (KGA)} {Gambit Roi Accepté}
  # {King's Indian} Est-Indienne
  # KIA {Attaque Est-Indienne}
  # {King's Knight Gambit} {Gambit du Cavalier Roi}
  # {King's Pawn} {Pion Roi}
  # {Lasker Variation} {Variante Lasker}
  # {Latvian Gambit} {Gambit Letton}
  # {Maroczy Bind} {Maroczy}
  # {Marshall Variation} {Variante Marshall}
  # {Modern Attack} {Attaque Moderne}
  # {Modern Steinitz} {Steinitz Moderne}
  # {Modern Variation} {Variante Moderne}
  # {Moscow Variation} {Variante de Moscou}
  # Nimzo-Indian Nimzo-Indienne
  # {Old Benoni} {Vieille Benoni}
  # {Old Indian} {Vieille Indienne}
  # {Old Indian Attack} {Attaque Vieille Indienne}
  # {Old Steinitz} {Vieille Steinitz}
  # {Open Game} {Partie Ouverte}
  # {Poisoned Pawn} {Pion empoisonné}
  # {Polish Variation} {Variante Polonaise}
  # {Polugaevsky Variation} {Variante Polugajewski}
  # {Queen's Gambit} {Gambit Dame}
  # {Queen's Gambit Accepted} {Gambit Dame accepté}
  # QGA {GDA}
  # {Queen's Gambit Accepted (QGA)} {Gambit Dame accepté (GDA)}
  # {Reversed QGA} {GDA inversé}
  # QGD {Gambit Dame Décliné}
  # {Queen's Gambit Declined (QGD)} {Gambit Dame Décliné (GDD)}
  # {Reversed QGD} {GDD inversé}
  # {Queen's Indian} Ouest-Indienne
  # {Queen's Pawn} {Pion Dame}
  # {Queen's Pawn Game} {Partie du Pion Dame}
  # {Reversed Slav} {Slave Inversée}
  # {Rubinstein Variation} {Variante Rubinstein}
  # {Russian Game} {Partie Russe}
  # {Russian Game (Petroff Defence)} {Partie Russe (Défense Petrov)}
  # {Russian-Three Knights Game} {Partie Russe: Trois Cavaliers}
  # {Scandinavian (Centre Counter)} {Scandinave (Centre)}
  # Schliemann Jänisch
  # {Schliemann (Jänisch)} {Gambit Jänisch (Schliemann)}
  # {Scotch Opening} {Ouverture Ecossaise}
  # {Sicilian Defence} {Défense Sicilienne}
  # {Sicilian Variation} {Variante Sicilienne}
  # {Slav Defence} {Défense Slave}
  # Smith-Morra Morra
  # {Smith-Morra Accepted} {Gambit Morra Accepté}
  # {Smith-Morra Gambit} {Gambit Morra}
  # {Spanish (Ruy Lopez)} {Partie Espagnole}
  # {Start position} {Position de départ}
  # {Steinitz Deferred} {Rubinstein retardée}
  # {Swedish Variation} {Variante suédoise}
  # {Swiss Variation} {Variante Suisse}
  # {Tarrasch's Gambit} {Gambit Tarrasch}
  # {Three Knights} {Trois Cavaliers}
  # {3 Knights} {Trois Cavaliers}
  # {Three Knights Game} {Partie des Trois Cavaliers}
  # {Three Pawns Attack} {Attaque des Trois Pions}
  # {Two Knights} {Deux Cavaliers}
  # {Two Knights Defence} {Défense des Deux Cavaliers}
  # {Two Knights Variation} {Variante des Deux Cavaliers}
  # {Two Pawns} {Deux Pions}
  # {Two Pawns Attack} {Attaque des Deux Pions}
  # {Wing Gambit} {Gambit de l'aile}
  # {Yugoslav Attack} {Attaque Yougoslave}
# }

}

############################################################
#
# French tip of the day


set tips(F) {
  {
    Scid a plus de 30 <a Index>pages d'aide</a>, et dans la plupart des fenêtres Scid,
    vous obtiendrez la page d'aide à son sujet en appuyant sur la touche <b>F1</b>.
  }
  {
    Certaines fenêtres de Scid, par exemple la zone d'information du jeu ou le
    <a Switcher>sélecteur de base de données</a>, ont un menu contextuel.  Essayez
    en appuyant sur le bouton droit de la souris dans chaque fenêtre pour voir
    si elle en a un et quelles sont les fonctions disponibles.
  }
  {
    Scid vous offre plus d'une façon de saisir des coups d'échecs, en vous laissant
    le choix de ce qui vous convient le mieux.  Vous pouvez utiliser la souris
    (avec ou sans suggestion de coup) ou le clavier (avec ou sans achèvement du coup).
    Lisez la page d'aide <a Moves>Saisie des coups d'échecs</a> pour les détails.
  }
  {
    Si vous avez des bases de données que vous ouvrez souvent, ajoutez un
    <a Bookmarks>signet</a> pour chacunes d'elles.  Vous pourrez alors
    les ouvrir plus rapidement en utilisant le menu des signets.
  }
  {
    Vous pouvez voir tous les coups de la partie en cours (avec les variantes et
    commentaires éventuels) en utilisant la fenêtre <a PGN>PGN</a>.  De cette fenêtre,
    vous pouvez aller à n'importe quel coup en cliquant dessus avec le bouton gauche
    de la souris, ou utiliser le bouton central ou droit de la souris
    pour voir un aperçu de cette position.
  }
  {
    Vous pouvez copier des parties d'échecs d'une base de données à une autre par
    glisser-déposer avec le bouton gauche de la souris dans la fenêtre
    <a Switcher>Sélecteur de base de données</a>.
  }
  {
    Scid peut ouvrir des fichiers PGN mais ils sont ouverts en lecture
    seule.  Donc, si vous voulez les éditer dans Scid, créez une nouvelle base
    de données Scid et copiez-y les jeux du fichier PGN en utilisant
    le <a Switcher>sélecteur de base de données</a>.
  }
  {
    Si vous avez une grande base de données dont vous utilisez souvent la fenêtre
    <a Tree>Arbre</a>, çà vaut la peine de <b>remplir le fichier cache</b>
    dans le menu Fichier de cette fenêtre.  Cela permet de mémoriser les statistiques
    de l'arbre pour de nombreuses positions d'ouverture, rendant l'accès à l'arbre
    plus rapide pour cette base de données.
  }
  {
    La fenêtre <a Tree>Arbre</a> peut vous montrer tous les coups joués à partir de la
    position actuelle.  Mais si vous voulez aussi voir tous les ordres de coups
    qui ont atteint cette position, vous pouvez les trouver en générant
    un <a OpReport>rapport d'ouverture</a>.
  }
  {
    Dans la fenêtre <a GameList>Liste des jeux</a>, appuyez sur l'en-tête de chaque
    colonne avec le bouton gauche ou droite de la souris pour ajuster sa largeur.
  }
  {
    Il suffit de cliquer sur le nom du joueur dans le panneau d'information sous
    l'échiquier de la fenêtre principale pour ouvrir la fenêtre <a PInfo>Information sur
    le joueur</a>.  De là, vous pouvez facilement régler le <a Searches Filter>filtre
    de recherche</a> sur toutes les parties de ce joueur avec un résultat donné
    en cliquant sur n'importe quelle valeur qui est affichée <rouge>en texte
    rouge</rouge>.
  }
  {
    Lors de l'étude d'une ouverture, il peut être très utile de faire une
    <a Searches Board>recherche de position d'échiquier</a> avec l'option <b>Pions</b>
    ou <b>Colonnes</b> sur une position d'ouverture importante, car cela peut
    révéler d'autres ouvertures qui atteignent la même structure de pions.
  }
  {
    Vous pouvez appuyer sur le panneau d'information sous l'échiquier avec le bouton
    droit de la souris pour ouvrir un menu permettant de le personnaliser.
    Par exemple, vous pouvez faire en sorte que Scid cache le prochain coup de la partie
    en cours, ce qui est utile pour s'entraîner en la rejouant par un jeu de devinettes.
  }
  {
    Si vous faites souvent beaucoup <a Maintenance>maintenance</a> sur une grande base
    de données, vous pouvez effectuer plusieurs tâches de maintenance en même temps
    en utilisant le <a Maintenance Cleaner>Nettoyeur</a>.
  }
  {
    Si vous avez une grande base de données où la plupart des jeux ont une date
    d'événement (EventDate) et vous voulez les jeux dans l'ordre des dates,
    effectuez un <a Sorting>triage</a> par date d'événement puis par événement
    (Event) au lieu de date du match (Date) puis événement, car cela aide à
    garder des parties dans le même tournoi avec des dates différentes ensemble,
    en supposant qu'ils ont tous le même EventDate, bien sûr.
  }
  {
    Avant de <a Maintenance Twins>supprimer les parties en doublon</a>, c'est une
    bonne idée de <a Maintenance Spellcheck>corriger l'orthographe</a> de
    votre base de données puisque ceci  permettra à Scid de trouver d'autres doublons
    et de les marquer pour les supprimer.
  }
  {
    Les <a Flags>drapeaux</a> sont utiles pour marquer les jeux de la base de
    données avec des caractéristiques que vous pourriez vouloir rechercher
    ultérieurement, telles que la structure de pions, les tactiques, etc.  Vous
    pouvez effectuer une recherche par drapeaux avec une
    <a Searches Header>recherche par en-tête</a>.
  }
  {
    Pour trouver les jeux les plus importants (avec des adversaires de haut niveau)
    en arrivant à une position particulière, ouvrir la fenêtre <a Tree>Arbre</a>
    et de là, ouvrir la liste des meilleurs jeux.  Vous pouvez même restreindre
    la liste des meilleurs jeux pour ne montrer que les jeux avec un résultat
    particulier, Blancs ou Noirs gagnants.
  }
  {
    Une excellente façon d'étudier une ouverture en utilisant une grande base de
    données de jeux est d'activer le mode d'entraînement dans la fenêtre
    <a Tree>Arbre</a>, puis jouer contre la base de données pour voir quelles
    lignes se produisent souvent.
  }
  {
    Si vous avez deux bases de données ouvertes, et que vous voulez voir
    l'<a Tree>arbre</a> de la première base de données en examinant un jeu de
    la deuxième base de données, il suffit d'appuyer sur le bouton
    <b>Verrouiller</b> ou le menu <b>Options</b>/<b>Verrouiller</b> de la fenêtre
    d'arbre pour la verrouiller sur la première base de données, puis passez à la
    deuxième base.
  }
  {
    La <a Tmt>recherche de tournois</a> n'est pas seulement utile pour trouver
    un tournoi particulier, mais peut aussi être utilisée pour voir à quels
    tournois un certain joueur a participé récemment. On peut aussi Identifier
    quels sont les tournois de haut niveau de chaque pays.
  }
  {
    Il existe un certain nombre de schémas communs définis dans la fenêtre de
    recherche de <a Searches Material>matériel ou motif</a> que vous pourriez
    trouver utiles pour l'étude des ouvertures ou du milieu de partie.
  }
  {
    Lors de la recherche d'une situation matérielle particulière dans la fenêtre de
    recherche de <a Searches Material>matériel ou motif</a>, c'est souvent utile de
    restreindre la recherche aux parties qui correspondent à au moins quelques
    demi-coups pour éliminer les parties où la situation ne s'est produite que
    brièvement.
  }
  {
    Si vous avez une base de données importante que vous ne voulez pas
    accidentellement altérer, changer les permissions de ses
    fichiers pour qu'ils soient en lecture seule.
  }
  {
    Si vous utilisez XBoard ou WinBoard, ou un autre programme d'échecs qui peut
    copier une position d'échecs en notation FEN standard dans le presse-papiers,
    et que vous voulez copier sa position d'échiquier actuelle dans Scid, la façon
    la plus rapide et la plus simple est de sélectionner <b>Copier Position</b>
    dans le menu Fichier de XBoard ou WinBoard, puis <b>Coller la position
    de départ</b> dans le menu Édition de Scid.
  }
  {
    Dans une
    <a Searches Header>recherche par en-tête</a>, les noms de joueur, d'événement,
    de lieu ou de ronde sont insensibles à la casse et correspondent à n'importe
    quel endroit dans un nom. Vous pouvez choisir d'utiliser à la place une recherche
    sensible à la casse avec des caractères génériques où " ? " représente un seul
    caractère et " * " = zéro ou plusieurs caractères, en entrant le texte de recherche
    "entre guillemets". Par exemple, tapez "*BEL" avec les guillemets dans le champ
    du lieu pour trouver toutes les parties jouées en Belgique mais pas à Belgrade.
  }
  {
    Si vous voulez corriger un coup dans une partie sans perdre tous les coups joués
    après, utilisez le menu <b>Édition</b>/<b>Coller le presse-papier comme partie PGN</b>
    pour ouvrir la fenêtre <a Import>Importer un jeu PGN</a> (raccourci clavier :
    <b>Ctrl+Maj+I</b>), appuyer sur le bouton
    <b>Coller la partie en cours</b>, éditer le mauvais coup et puis appuyez sur
    <b>Importer</b>.
  }
  {
    Si vous avez chargé un fichier de classification ECO, vous pouvez aller à la
    position la plus profonde classée dans la partie actuelle avec le menu <b>Partie</b>
    <b>Identifier l'ouverture</b> (raccourci : <b>Ctrl+Maj+D</b>).
  }
  {
    Si vous voulez vérifier la taille d'un fichier ou sa date de dernière modification
    avant de l'ouvrir, utilisez l'<a Finder>Explorateur de fichier</a>.
  }
  {
    Un <a OpReport>rapport d'ouverture</a> est idéal pour en savoir plus sur une position
    d'échiquier donnée. Vous pouvez voir son score, si elle conduit à de fréquentes
    parties nulles ou à des thèmes positionnels classiques (raccourci : <b>Ctrl+Maj+O</b>).
  }
  {
    Vous pouvez ajouter les symboles d'annotation les plus courants ( !, ! ?, +=, etc.) au
    coup ou position actuelle avec des raccourcis clavier sans avoir besoin d'utiliser
    l'<a Comment>éditeur de commentaire<a>. Par exemple, tapez "!" puis
    la touche Retour pour ajouter un symbole d'annotation "!".  Voir la page d'aide
    <a Moves>Saisie de coups d'échecs</a> pour plus de détails.
  }
  {
    Si vous parcourez les ouvertures d'une base de données avec l'<a Tree>arbre</a>,
    vous pouvez voir un aperçu utile de la qualité de l'ouverture actuelle, comment
    est son score récent entre joueurs de haut niveau, en ouvrant la fenêtre de statistiques
    (raccourci : <b>Ctrl+I</b>).
  }
  {
    Vous pouvez changer la taille du panneau de la fenêtre principale en maintenant les
    touches <b>Ctrl</b> et <b>Maj</b> enfoncées, et en appuyant sur les touches flèchées
    <b>Gauche</b> ou <b>Droite</b>.
  }
  {
    Après une <a Searches>recherche</a>, vous pouvez facilement parcourir tous les
    les jeux correspondants en maintenant enfoncée la touche <b>Ctrl</b> et en appuyant sur
    la touche fléchée <b>Haut</b> ou <b>Bas</b> pour charger la partie précédente ou
    suivante de la liste <a Searches Filter>filtrée</a>.
  }
  {
    Les fenêtres peuvent être ancrées en cochant l'entrée correspondante dans le menu
    <b>Options</b>/<b>Fenêtres groupées</b>.  Les onglets peuvent être glissés et déposés
    d'un panneau à l'autre et organisés par un clic droit sur son onglet.
  }
  {
    L'interface graphique de Scid peut être personnalisée par les TTK-Themes. Cela permet de
    changer l'apparence et le comportement selon la préférence de l'utilisateur. Utilisez le
    menu <b>Options</b>/<b>Charger le thème</b> et regardez dans l'<a Appearance>aide</a>.
  }
}

#### HelpPages Francais

### help.tcl: Help pages for Scid.
### This file uses UTF-8
#################################################

set helpTitle(F,Contents) "Contenu de l'aide de Scid"
set helpText(F,Contents) {
<h1>Contenu de l'aide de Scid</h1>
  
  <h4>Démarrage et aide générale</h4>
  <ul>
  <li><a Guide><b>Guide rapide</b> à l'utilisation de Scid</a> <red>(Lisez d'abord ceci)</red></li>
  <li><a Hints>Les <b>conseils</b> pour obtenir plus de Scid</a></li>
  <li><a MainWindow>La <b>fenêtre principale</b> de Scid</a></li>
  <li><a Menus>Les <b>menus</b> de Scid</a></li>
  <li><a Moves>La saisie de <b>coups d'échecs</b></a></li>
  <li><a Searches><b>Recherche de parties</b> dans Scid</a></li>
  <li><a Clipbase>Utilisation de la base de données <b>Clipbase</b></a></li>
  <li><a Annotating><b>Annotation des parties d'échecs</b></a></li>
  </ul>
  
  <h4>Autres fenêtres Scid</h4>
  <ul>
  <li><a Analysis>Fenêtre <b>Analyse</b></a></li>
  <li><a Tree>Fenêtre <b>Arbre</b></a></li>
  <li><a Book>Fenêtre <b>Bibliothèque d'ouvertures</b></a></li>
  <li><a Crosstable>Fenêtre <b>Classement de tournoi</b></a></li>
  <li><a Finder>Fenêtre <b>Explorateur de fichiers</b></a></li>
  <li><a Comment>Fenêtre <b>Éditeur de commentaires</b></a></li>
  <li><a OpeningTrainer><b>Entraînement aux ouvertures</b></a></li>
  <li><a CalVar><b>Entraînement : calcul des variantes</b></a></li>
  <li><a TacticsTrainer><b>Entraînement : exercices tactiques</b></a></li>
  <li><a ReviewGame><b>Entraînement : reviser une partie</b></a></li>
  <li><a Email>Fenêtre <b>Gestionnaire de messagerie</b></a></li>
  <li><a Import>Fenêtre <b>Importation de jeux</b></a></li>
  <li><a PInfo>Fenêtre <b>Information sur un joueur</b></a></li>
  <li><a GameList>Fenêtre <b>Liste des jeux</b></a></li>
  <li><a PGN>Fenêtre <b>Notation PGN</b> (texte du jeu)</a></li>
  <li><a SeriousGame>Jouer une <b>partie sérieuse</b></a></li>
  <li><a TacticalGame>Jouer une <b>partie tactique</b></a></li>
  <li><a Reports><b>Rapports</b></a></li>
  <li><a PList>Fenêtre <b>Recherche de joueur</b></a></li>
  <li><a Tmt>Fenêtre <b>Recherche de tournoi</b></a></li>
  <li><a Switcher>Fenêtre <b>Sélecteur de base de données</b></a></li>
  <li><a PTracker>Fenêtre <b>Suivi de pièce</b></a></li>
  <li><a Graphs>Fenêtres <b>graphiques</b></a></li>
  <li><a TB>Utilisation de <b>tables de finales</b> dans Scid</a></li>
  </ul>
  
  <h4>Autres utilitaires et informations</h4>
  <ul>
  <li><a Compact><b>Compactage</b> d'une base de données</a></li>
  <li><a Flags><b>Drapeaux</b> de jeu</a></li>
  <li><a Correspondence><b>Échecs par correspondance</b></a></li>
  <li><a ECO>Classement des ouvertures selon l'<b>ECO</b> </a></li>
  <li><a EPD>Fichiers <b>EPD</b></a></li>
  <li><a Export><b>Exportation de jeux</b> vers des fichiers textes</a></li>
  <li><a FICS><b>FICS</b> : jouer sur Internet</a></li>
  <li><a Formats><b>Formats de fichiers</b></a> Scid</li>
  <li><a Options><b>Options</b> et préférences</a></li>
  <li><a Cmdline>Options de <b>ligne de commande</b></a></li>
  <li><a Maintenance>Outils de <b>maintenance de base de données</b></a></li>
  <li><a Appearance><b>Personnaliser</b> l'interface de Scid</a></li>
  <li><a Pgnscid><b>Pgnscid</b> : conversion des fichiers PGN</a></li>
  <li><a Bookmarks><b>Signets</b></a></li>
  <li><a Sorting><b>Tri</b> d'une base de données</a></li>
  <li><a LaTeX>Utilisation de <b>LaTeX</b> avec Scid</a></li>
  <li><a NAGs>Valeurs d'annotation standard <b>NAG</b></a></li>
  <li><a HardwareConfig>Connexion de <b>matériel externe</b></a></li>
  <li><a Author>Contact et information</a></li>
  </ul>
  
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

###############
### Topic Index

set helpTitle(F,Index) "Index de la rubrique d'aide Scid "
set helpText(F,Index) {
<h1>Index de la rubrique d'aide Scid </h1>
  
  <h3>A</h3>
  <ul>
  <li><a Analysis>Analyse</a> window</li>
  <li><a Annotating>Jeux d'annotation</a></li>
  <li><a NAGs>Symboles d'annotation</a></li>
  <li><a Author>Auteur, contact</a></li>
  <li><a MainWindow Autoplay>Mode Autoplay</a></li>
  </ul>
  
  <h3>B</h3>
  <ul>
  <li><a Tree Best>Meilleurs jeux</a> window</li>
  <li><a Searches Board>Recherches sur le tableau</a></li>
  <li><a Book>Book</a> window</li>
  <li><a BookTuning>Book</a> tuning</li>
  <li><a Bookmarks>Signets</a></li>
  <li><a GameList Browsing>Jeux de navigation</a></li>
  </ul>
  
  <h3>C</h3>
  <ul>
  <li><a Maintenance Cleaner>Nettoyeur</a></li>
  <li><a Clipbase>Clipbase</a></li>
  <li><a Cmdline>Options de la ligne de commande</a></li>
  <li><a Comment>Éditeur de commentaire</a></li>
  <li><a Compact>Compactage d'une base de données</a></li>
  <li><a Correspondence>Echecs par correspondance</a></li>
  <li><a CCIcons>Icônes d'Échecs par Correspondance</a></li>
  <li><a Author>Contactez-nous</a></li>
  <li><a Contents>Contenu</a></li>
  <li><a Crosstable>Crosstable</a> window</li>
  <li><a Appearance>Personnaliser l'apparence</a></li>
  </ul>
  
  <h3>D</h3>
  <ul>
  <li><a Compact>Compactage de la base de données</a></li>
  <li><a Formats>Formats de fichiers de la base de données</a></li>
  <li><a Metadata>Information sur la base de données (Metadata)</a></li>
  <li><a Maintenance>Maintenance de la base de données</a></li>
  <li><a Sorting>Triage de la base de données</a></li>
  <li><a Switcher>Switch de la base de données</a> window</li>
  <li><a Maintenance Twins>Suppression des parties de jumeaux</a></li>
  <li><a HardwareConfig>Échiquier électronique DGT</a></li>
  <li><a InputEngine>Utilisation du DGT</a></li>
  <li><a Metadata>Métadonnées de base de Dublin</a></li>
  </ul>
  
  <h3>E</h3>
  <ul>
  <li><a ECO Browser>Browser ECO</a> window</li>
  <li><a ECO Codes>Système de code ECO</a></li>
  <li><a ECO>Classification des ouvertures de l'ECO</a></li>
  <li><a Menus Edit>Menu d'édition</a></li>
  <li><a Email>Gestionnaire d'e-mail</a> window</li>
  <li><a CCeMailChess>Echecs par courriel</a></li>
  <li><a Analysis List>Liste des moteurs</a></li>
  <li><a Moves>Entrer des coups d'échecs</a></li>
  <li><a EPD>Fichiers EPD</a></li>
  <li><a Export>Exportation des jeux dans des fichiers texte</a></li>
  <li><a HardwareConfig>Matériel externe (Novag, DGT...)</a></li>
  </ul>
  
  <h3>F</h3>
  <ul>
  <li><a FICSLogin>FICS Login</a></li>
  <li><a FICSfindOpp>FICS Trouver l'adversaire</a></li>
  <li><a FICS>Lecture sur Internet (FICS)</a></li>
  <li><a FICS Training>Conférences FICS</a></li>
  <li><a Finder>File Finder</a></li>
  <li><a FindBestMove>Entraînement : Trouver le meilleur coup </a></li>
  <li><a Formats>Formats de fichier</a></li>
  <li><a Menus File>Menu Fichier</a></li>
  <li><a Searches Filter>Filtre</a></li>
  <li><a Export>Filtre, export</a></li>
  <li><a Graphs Filter>Graphique du filtre</a></li>
  <li><a Flags>Flags</a></li>
  <li><a Options Fonts>Fonts</a></li>
  </ul>
  
  <h3>G</h3>
  <ul>
  <li><a Flags>Flags de jeu</a></li>
  <li><a GameList>Liste de jeux</a>fenêtre</li>
  <li><a Menus Game>Menu de jeu</a></li>
  <li><a Graphs>Fenêtres graphiques</a></li>
  </ul>
  
  <h3>H</h3>
  <ul>
  <li><a Searches Header>Recherches d'en-tête</a></li>
  <li><a Menus Help>Menu d'aide</a></li>
  <li><a Hints>Hints</a></li>
  <li><a HardwareConfig>Utilisation du matériel (Novag, DGT)</a></li>
  </ul>
  
  <h3>I</h3>
  <ul>
  <li><a CCIcons>Icônes pour les Échecs par Correspondance</a></li>
  <li>Fenêtre d'<a Import>importation</a> de jeux</li>
  <li><a Moves Informant>Symboles d'informateurs</a></li>
  <li><a InputEngine>Pilotes du moteur d'entrée</a></li>
  </ul>
  
  <h3>L</h3>
  <ul>
  <li><a LaTeX>LaTeX</a> format de sortie</li>
  </ul>
  
  <h3>M</h3>
  <ul>
  <li><a MainWindow>Fenêtre principale</a></li>
  <li><a Maintenance>Outils de maintenance</a></li>
  <li><a TreeMasks>Masques pour les arbres</a></li>
  <li><a Searches Matériau>Recherches de matériaux/motifs</a></li>
  <li><a Menus>Menus</a></li>
  <li><a Liste des jeux Browsing>Jeux en fusion</a></li>
  <li><a Metadata>Metadata</a></li>
  <li><a Moves>Entrée de coup</a></li>
  </ul>
  
  <h3>N</h3>
  <ul>
  <li><a Maintenance Editing>Noms, édition</a></li>
  <li><a Maintenance Spellcheck>Noms, vérification de l'orthographe</a></li>
  <li><a NAGs>Valeurs d'annotation NAG</a></li>
  <li><a Annotating Null>Coups nuls</a></li>
  <li><a HardwareConfig>Echiquier électronique Citrine de Novag</a></li>
  <li><a Novag>Novag Utilisation de la citrine </a></li>
  </ul>
  
  <h3>O</h3>
  <ul>
  <li><a ECO>Classification d'ouverture (ECO)</a></li>
  <li><a Reports Opening>Rapport d'ouverture</a> fenêtre</li>
  <li><a OpeningTrainer>Formation : Ouvertures</a></li>
  <li><a Options>Options</a></li>
  </ul>
  
  <h3>P</h3>
  <ul>
  <li><a PGN>PGN</a> window</li>
  <li><a Pgnscid>Pgnscid</a></li>
  <li><a FICS>Lecture sur Internet (FICS)</a></li>
  <li><a PTracker>Suivi de pièce</a> window</li>
  <li>fenêtre de <a PList>recherche de joueur</a></li>
  <li><a PInfo>Info joueur</a> window</li>
  <li><a Reports Player>Rapport du joueur</a> window</li>
  <li><a TacticalGame>Jouer au jeu tactique</a></li>
  <li><a SeriousGame>Jouer au jeu sérieux</a></li>
  </ul>
  
  <h3>Q</h3>
  <ul>
  <li><a Guide>Guide rapide d'utilisation de Scid</a></li>
  </ul>
  
  <h3>R</h3>
  <ul>
  <li><a Graphs Rating>Graphique d'évaluation</a></li>
  </ul>
  
  <h3>S</h3>
  <ul>
  <li><a Searches Filter>Filtre de recherche</a></li>
  <li><a Menus Search>Menu de recherche</a></li>
  <li><a Searches>Recherches</a></li>
  <li><a SeriousGame>Jouer au jeu sérieux</a></li>
  <li><a Sorting>Triage d'une base de données</a></li>
  <li><a Maintenance Spellcheck>Noms de la vérification de l'orthographe</a></li>
  <li><a Switcher>Switcher</a> window</li>
  </ul>
  
  <h3>T</h3>
  <ul>
  <li><a TB>Tableaux de base</a></li>
  <li><a TacticalGame>Jeu tactique</a></li>
  <li><a Menus Tools>Menu des outils</a></li>
  <li><a Tmt>Trouveur de tournois</a></li>
  <li><a FindBestMove>Entraînement : Trouver le meilleur coup </a></li>
  <li><a OpeningTrainer>Formation : Ouvertures</a></li>
  <li><a TacticsTrainer>Formation : Tactique</a></li>
  <li><a FICS Training>Formation : Conférences FICS</a></li>
  <li><a Tree>Fenêtre de l'arbre</a></li>
  <li><a TreeMasks>Masques pour les arbres</a></li>
  <li><a Moves Trial>Mode d'essai</a></li>
  <li><a Maintenance Twins>Jumelles (duplicate) jeux</a></li>
  </ul>
  
  <h3>V</h3>
  <ul>
  <li><a Annotating Vars>variantes</a></li>
  </ul>
  
  <h3>W</h3>
  <ul>
  <li><a Menus Windows>Menu Windows</a></li>
  </ul>

  <h3>X</h3>
  <ul>
  <li><a CCXfcc>Xfcc support</a></li>
  </ul>

  <p><footer>(Mise à jour : Scid 3.6.23, mars 2008)</footer></p>
}

####################
### Quick Guide help:

set helpTitle(F,Guide) "Guide rapide d'utilisation de Scid"
set helpText(F,Guide) {
<h1>Guide rapide d'utilisation de Scid</h1>
  <p>
  Scid est une application pour <b>visualiser, analyser, éditer et gérer des
  collections de parties d'échecs</b>. Avec Scid, vous pouvez
  <a Searches>rechercher, sélectionner et filtrer</a> les matchs selon
  différents critères, et les copier, les organiser ou les exporter facilement
  d'une base à l'autre.
  </p>
  <p>
  Scid utilise son propres <a Formats>format de la base de données</a> en
  trois fichiers qui est très compact et rapide, mais il peut convertir de et vers
  le <a PGN>format standard PGN</a> (Portable Game Notation). La
  <a PGN>fenêtre PGN</a> de Scid affiche le texte de la partie en cours avec
  cette notation.
  </p>
  <p>
  Vous pouvez utiliser Scid pour ajouter des parties d'échecs à une base de données,
  en utilisant le clavier ou la souris pour entrer les coups. Voir la page d'aide
  <a Moves>Saisie des coups d'échecs</a> pour plus de détails.
  </p>
  <p>
  Vous pouvez également utiliser Scid comme navigateur de fichiers <a PGN>PGN</a>,
  en collant le texte PGN  dans la fenêtre <a Import>Importation un jeu PGN</a>
  par le menu <menu>Édition</menu><menu>Coller le presse-papiers comme partie PGN</menu>,
  ou par le menu <menu>Fichier</menu><menu>Ouvrir...</menu>.  Par contre, les fichiers
  PGN ne peuvent pas être édités par Scid car il les ouvre en lecture seule, ils
  utilisent plus de mémoire et sont plus lents à charger. Donc pour les gros fichiers
  PGN, il est recommandé de créer une base de données Scid d'abord avec l'utilitaire
  <a Pgnscid>pgnscid</a>.
  </p>
  <p>
  Les fenêtres peuvent être utilisées en mode groupé ou comme des fenêtres indépendantes
  (voir dans le menu des options). En mode groupé, les fenêtres les plus importantes
  sont encastrées dans une fenêtre principale. Les fenêtres peuvent être réorganisées
  en cliquant avec le bouton droit de la souris sur son onglet et en faisant glisser
  les onglets d'un carnet à l'autre. Un clic sur l'icône à gauche de l'onglet ouvre
  le menu de chaque fenêtre si elle en possède.
  </p>
  <p>
  La <a MainWindow>fenêtre principale</a> de Scid, avec l'échiquier graphique,
  montre les détails de la partie active et la base de données active. À tout moment,
  vous pouvez avoir jusqu'à neuf base de données ouvertes, dont la
  <a Clipbase>Clipbase</a>, et chacune aura sa propre partie active.
  Une partie numérotée 0 indique une partie temporaire qui ne fait pas partie de
  la base de données actuelle. Vous pouvez passer d'une base de données ouverte à
  l'autre à l'aide du <a Menus File>menu Fichier</a>.
  </p>
  <p>
  Pour plus d'information, veuillez lire les autres pages d'aide listées dans la
  <a Contents>Table de matière</a>, l'<a Index>Index</a> ou la
  <a Hints>FAQ</a> de Scid.
  </p>
  <p>
  Voir la page <a Author>contact et information</a> si vous devez contacter
  l'auteur de Scid.
  </p>
  
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

####################
### Hints page:

set helpTitle(F,Hints) "Les conseils de Scid"
set helpText(F,Hints) {
<h1>Les conseils de Scid</h1>
  <p>
  Cette page contient des conseils utiles sous forme de questions et réponses pour
  vous aider à mieux utiliser Scid. Si vous êtes nouveau à Scid, veuillez lire le
  <a Guide>guide rapide</a> d'abord.  La plupart des informations sur cette page
  est disponible plus en détail sur les autres pages d'aide listées dans
  l'<a Index>Index</a>. Si vous estimez qu'une astuce serait utile à ajouter
  à cette page, veuillez l'envoyer à l'<a Author>auteur de Scid</a>.
  </p>

  <h4>Puis-je obtenir de Scid qu'il charge une base de données au démarrage ?</h4>
  <p>
  Oui, vous pouvez ajouter des bases de données, des <a PGN>fichiers PGN</a>
  ou des <a EPD>fichiers EPD</a> à la ligne de commande. Par exemple :
  <ul>
  <b>scid mybase games.pgn</b>
  </ul>
  chargera la base de données Scid <b>mybase</b> et chargera également le
  fichier PGN <b>games.pgn</b>.
  </p>




  <h4>Je m'entraîne en rejouant la partie, donc je ne veux pas que Scid
  imprimer le prochain coup dans le panneau d'information du jeu sous l'échiquier.
  Puis-je le cacher ?</h4>
  <p>
  Vous pouvez cacher le prochain coup en cliquant sur le bouton <b>droit</b> de la souris
  dans le panneau d'information et en sélectionnant <b>Cacher le coup suivant</b> dans
  le menu contextuel qui apparaît.
  </p>

  <h4>Comment puis-je voir le code d'ouverture ECO pour la position actuelle ?</h4>
  <p>
  Le code ECO est affiché sur la ligne inférieure de la zone d'information
  sous l'échiquier dans la <a MainWindow>fenêtre principale</a>,
  si vous avez le fichier de classification ECO (<b>scid.eco</b>) chargé. <br>
  La page d'aide <a ECO>codes ECO</a> explique comment charger la classification
  ECO. Enregistrez les options pour qu'il soit chargé à chaque fois que
  vous démarrez Scid.
  </p>

  <h4>J'édite une partie, et je suis à la hauteur du coup 30, mais je viens de voir
  que le coup 10 était faux. Comment puis-je le corriger et garder tous les coups après
  lui ?</h4>
  <p>
  Vous pouvez utiliser la <a Import>fenêtre Importer un jeu PGN</a> (raccourci
  clavier : <b>Ctrl+Maj+I</b>) ; voir la page d'aide
  <a Moves Mistakes>Saisie de coups</a> pour plus d'informations.
  </p>

  <h4>Comment puis-je copier des parties d'une base de données à une autre ? </h4>
  <p>
  Utiliser la fenêtre <a Switcher>Sélecteur de base de données</a> : glisser
  et déposer avec la souris depuis la source à la base de données cible pour copier
  tous les jeux <a Searches Filter>filtrés</a> de la base de données
  source.
  </p>


  <h4>À chaque fois que je saisis un coup où il y en a déjà un, j'obtiens
  la boîte de dialogue "Remplacer le déplacement ?". Comment puis-je éviter cela ?</h4>
  <p>
  Désactivez-le avec l'option <b>Demander avant d'écraser les coups</b> dans le
  menu <menu>Options</menu><menu>Coups</menu>.
  Ou bien, prenez l'habitude de reprendre les coups en utilisant le bouton droit de
  la souris, ce qui enlève effectivement le coup du jeu si vous êtes au dernier coup.
  </p>




  <h4>Comment puis-je utiliser la fenêtre Arbre sur une sélection de jeux, pas sur
  l'ensemble de ma base de données ?</h4>
  <p>
  Utilisez la <a Clipbase>Clipbase</a>. Configurez votre filtre de base de données
  pour qu'il contienne les parties sur lesquels vous voulez utiliser l'arbre, puis
  copiez-les dans la Clipbase en utilisant le
  <a Switcher>Sélecteur de base de données</a>. Ensuite, il suffit d'ouvrir la
  fenêtre Arbre de la Clipbase.
  </p>

  <h4>L'arbre est lent pour les grandes bases de données. Comment puis-je l'accélérer ? </h4>
  <p>
  Sauvegardez souvent le cache de l'arbre, afin de sauvegarder les résultats de l'arbre
  pour une utilisation ultérieure. Voir la section sur la mise en cache de la page d'aide
  <a Tree>Arbre</a> pour plus de détails.
  </p>

  <h4>Comment puis-je éditer la <a PGN>représentation PGN</a> du jeu directement ? </h4>
  <p>
  Vous ne pouvez pas utiliser la <a PGN>fenêtre PGN</a> pour éditer la partie
  en cours, mais vous pouvez toujours éditer sa représentation PGN en utilisant la
  <a Import>fenêtre Importer un jeu PGN</a>.
  Il suffit de l'ouvrir par le menu
  <menu>Édition</menu><menu>Coller le presse-papiers comme partie PGN</menu>
  (raccourci clavier : <b>Ctrl+Maj+I</b>),appuyer sur le bouton
  <b>[Coller la partie en cours]</b>, éditer le jeu, puis appuyer sur
  <b>[Importer]</b>.
  </p>

  <h4>Ma base de données a plusieurs fautes d'orthographe pour certains noms de joueurs.
  Comment puis-je les corriger tous ?</h4>
  <p>
  Vous pouvez éditer des noms individuels avec le menu
  <menu>Base</menu><menu>Maintenance</menu><menu>Éditeur de nom</menu> ou vérifier
  l'orthographe de tous les noms avec le menu
  <menu>Base</menu><menu>Orthographe des noms</menu>.
  Voir la page <a Maintenance Editing>Maintenance</a>.
  </p>

  <h4>J'ai deux bases de données ouvertes : une avec mes propres jeux, et une grande base
  de données des jeux de grands maîtres. Comment puis-je comparer un de mes jeux à ceux
  de la grande base de données ?</h4>
  <p>
  Il suffit d'ouvrir la fenêtre <a Tree>Arbre</a> pour la base de données de
  référence, et revenir à la partie à comparer avec le Sélecteur de base de données.
  Alternativement, une base peut être directement ouverte en tant qu'arbre via
  le menu <menu>Fichier</menu>.
  </p>

  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

####################
### Main window help:

set helpTitle(F,MainWindow) "La fenêtre principale de Scid"
set helpText(F,MainWindow) {
<h1>La fenêtre principale de Scid</h1>
  <p>La fenêtre principale de Scid affiche la position actuelle de l'échiquier
  pour la <term>partie active</term>, et des informations sur cette partie
  et la <term>base de données active</term>. Vous pouvez :</p>
  <ul>
  <li><b>Charger une partie</b> de la base active dans le
  <a MainWindow Chessboard>plateau de jeu</a> ou</li>
  <li><b>Créer une nouvelle partie</b> en utilisant le <a Menus>menu principal</a>
  ou la <a MainWindow Toolbar>barre d'outils</a>.</li>
  <li><b>Naviguer dans la partie active</b> à la souris ou au clavier.</li>
  <li><b>Éditer la partie active</b> à l'aide de la souris ou du clavier.</li>
  <li><b>Voir les informations sur la base active</b> dans le
  <a MainWindow TitleBar>titre de la fenêtre</a> et le titre de
  l'<a MainWindow BoardTab>onglet de l'échiquier</a>.</li>
  <li><b>Voir les informations sur la partie active</b> grâce à
  la <a MainWindow GameInfoBar>barre de menu de l'échiquier</a> ou
  le <a MainWindow GameInfoArea>panneau d'information</a> en bas
  de la fenêtre.</li>
  </ul>
  <p>Des pages d'aide séparées décrivent toutes les actions disponibles dans les
  <a Menus>menus de Scid</a> et les moyens
  d'<a Moves>entrer des coups d'échecs</a>. D'autres informations plus
  détaillées sont aussi disponibles séparément pour
  la <a GameInfoBar>barre de menu de l'échiquier</a>
  et <a GameInfoArea>panneau d'information</a></p>

  <h3><name Chessboard>Le plateau de jeu</name></h3>
  <p>Le joueur au trait est indiqué par la marque <blue>■</blue> à droite
  de son nom.
  Les coups peuvent être <a Moves>saisis à la souris ou au clavier</a>
  en <term>notation SAN</term>.</p>

  <h4>Navigation avec la souris</h4>
  <p>Le pointeur de la souris affiche des cases en surbrillance
  si cete option est activée</p>
  <ul>
  <li><b>Clic-gauche</b> sur une pièce et une case d'arrivée joue le coup
  s'il est légal.</li>
  <li><b>Clic-droit</b> recule d'un demi-coup.</li>
  <li><b>Ctrl+Clic-gauche</b> marque la case en couleur.</li>
  <li><b>Ctrl+Clic-gauche/glisser</b> dessine une flèche d'une case à l'autre.</li>
  </ul>
  <p>Vous pouvez aussi utiliser les boutons de la
  <a MainWindow GameInfoBar>barre de menu de l'échiquier</a> pour avancer
  ou reculer d'un demi-coup. En plus de cela, le
  <a MainWindow GameInfoArea>panneau d'information</a> permet de naviguer
  dans une variante avec la souris. Vous pouvez aller à un numéro de coup précis en
  utilisant la <a PGN>fenêtre PGN</a>, et sélectionner la couleur des marquages
  dans l'<a Comment>éditeur de commentaire</a>.</p>


  <h4>Navigation avec le clavier</h4>
  <p>Pour naviguer d'un coup à l'autre dans la partie active, les touches clavier
  définis par défaut sont :</p>
  <ul>
  <li>- les touches fléchées <b>Gauche</b> et <b>Droite</b> avancent ou reculent
    d'un demi-coup,</li>
  <li>- les touches <b>Haut</b> et <b>Bas</b> avancent ou reculent de
    dix demi-coups,</li>
  <li>- la touche <b>Début</b> recule vers la position initiale de la partie,</li>
  <li>- la touche <b>Fin</b> va au dernier demi-coup.</li>
  <li>- la touche <b>v</b> pour sélectionner une des variantes du coup suivant, si
    la <term>fenêtre pop-up Variantes</term> ne s'ouvre pas automatiquement.</li>
  <li>- la touche <b>z</b> quitte la ligne de variante actuelle.</li>
  <li>- la touche <b>.</b> (ou <b>Maj+;</b> sur un clavier français) retourne
    l'échiquier.</li>
  <li>- le raccourci <b>Ctrl+A</b> ajoute une nouvelle variante vide.</li>
  <li>- le raccourci <b>Ctrl+U</b> permet d'aller à un numéro de coup précis.</li>
  </ul>
  <p>Voir les pages d'aide <a Moves>Saisie de coups d'échecs</a> et les
  <a Menus>menus</a> de Scid pour plus d'information sur les actions à
  la souris et au clavier.</p>

  <h4>Options du plateau de jeu</h4>
  <p><term><name Toolbar>La barre d'outils</name></term>. La barre d'outils
  au-dessus de l'échiquier facilite l'accès aux fonctions fréquemment utilisés
  des menus de Scid. Cette barre peut être configurée avec le menu
  <menu>Options</menu><menu>Préférences...</menu></p>
  <p><term>Apparence du plateau de jeu</term>.
  L'affichage des pièces et de l'échiquier peut être configurée avec le menu
  <menu>Options</menu><menu>Préférences...</menu>.</p>
  <p><term>Bordure de l'échiquier</term>. Si l'option est activée dans la
  <a GameInfoBar BD_ShowToolbar>barre de menu</a>, les
  <term>coordonnées des cases</term> ou le <term>bilan matériel</term> peut
  être affiché à droite du plateau de jeu par des petits symboles de pièces.
  Par exemple, si les Blancs ont capturé un fou alors que les Noirs ont capturé
  un cavalier, un fou blanc et un cavalier noir s'affichent.</p>
  <p><term>Paramètres pour l'affichage des coups</term> accessible par le menu
  <menu>Options</menu><menu>Coups</menu> :</p>
  <ul>

  <li><menu>Temps d'animation</menu> pour régler le temps utilisé pour l'animation
  des coups</li>
  <li><menu>Délai entre les coups</menu> pour régler le délai entre deux coups
  en mode automatique</li>
  <li><menu>Montrer les coups suggérés</menu>. Le pointeur de la souris affiche des
  cases en surbrillance s'il y a un coup légal.</li>
  <li><menu>Montrer fenêtre des variantes</menu>. Scid ouvrira automatiquement
  la fenêtre de sélection de variantes pour naviguer vers le coup suivant.</li>
  <li><menu>Surbrillance du dernier coup</menu>. Scid marquera le dernier coup par
  un cadre coloré autour des cases concernées.</li>
  <li><menu>Montrer flèches pour les variantes</menu> pour afficher des flèches
  correspondants aux prochains coups de chaque variante existante.</li>
  <li><menu>Couleur codée Gloss of Danger</menu>. Scid marquera les cases attaquées
  selon le niveau de danger en jaune ou rouge (non protégée).</li>
  </ul>

  <h3><name GameStatus>Information sur la partie et la base active</name></h3>

  <h4><name TitleBar>La barre de titre</name></h4>
  <p>La barre de titre montre "<b>Scid -</b>" suivi du nom de la
  <term>base de données active</term>, et d'une fraction indiquant le numéro de
  la <term>partie active</term> et le nombre de parties dans cette base, par exemple
  "<b>1/11</b>". Ce nombre inclut les parties marquées pour suppression, tant que
  la base n'a pas été <a Compact>compactée</a>. Une <b>partie numérotée 0</b>
  indique une partie temporaire qui ne fait pas encore partie de la base de données
  actuelle.</p>

  
  <h4><name BoardTab>L'onglet de l'échiquier</name></h4>
  <p>L'onglet de l'échiquier montre le nom de la <term>base de données active</term>
  entre parenthèses, et les noms des deux joueurs. Si la partie a été éditée sans
  être sauvegardée, un asterisque apparaîtra à côté du nom de la base, comme ceci :
  <b>(MaBase*): ...</b> Scid avertira l'utilisateur d'enregistrer la partie
  modifiée si besoin.</p>

  
  <h4><name GameInfoBar>La barre de menu de l'échiquier</name></h4>
  <p>La <term>barre de menu de l'échiquier</term> se trouve en-dessous du plateau de
  jeu. Elle affiche un résumé des informations qui peuvent aussi apparaître dans le
  <a MainWindow GameInfoArea>panneau d'information</a>
  de la partie active :</p>
  <ul>
  <li>1. la date et le lieu de l'événement, quand l'échiquier est à
  la position initiale, un clic de souris sur le <blue>texte en bleu</blue> ouvre
  le <a Crosstable>classement du tournoi</a>,</li>
  <li>2. le dernier coup joué ou le commentaire correspondant, avec
  le terme <b>(var)</b> pour indiquer si vous êtes dans une variante ou dans
  la ligne principale,</li>
  <li>3. et enfin, le <a ECO>code ECO</a> de l'ouverture avec le nom de la
  variante.</li>
  </ul>
  <p>Si une <term>nouvelle variante est détectée</term>, la barre de menu alerte
  l'utilisateur avec le message <b>New Variation</b>. Cliquer sur le texte en bleu
  <blue>[click to change]</blue> offre des
  <a GameInfoBar BD_Alert>actions supplémentaires</a>. Voir la page
  d'aide de la <a GameInfoBar>barre de menu</a> pour plus
  d'information.</p>

  <h4><name GameInfoArea>Le panneau d'information du jeu</name></h4>
  <p>Le <term>panneau d'information de la partie active</term> peut être affiché ou
  caché avec le menu <menu>Fenêtres</menu><menu>Panneau d'information</menu>.
  Les lignes montrent des informations telles que les joueurs, leurs pays, le
  résultat, la date et le lieu qui sont tirées de l'en-tête PGN, les photos des
  joueurs si disponibles, le <a ECO>code ECO</a> de la position actuelle,
  ainsi que les <a Flags>drapeaux</a> définis pour la partie en cours.
  La quatrième ligne indique la position actuelle dans la partie, et le prochain
  coup.</p>
  <p>En cliquant sur les <blue>textes en bleu</blue> de ce panneau, des fenêtres
  de rapport Scid s'ouvrent : le résultat de tous les matchs et
  <a Crosstable>classement du tournoi</a>,
  l'<a PInfo>information sur un des joueurs</a>, ou la fenêtre de
  <a ECO Browser>navigation ECO</a>. Un clic sur les textes du dernier
  coup, du coup suivant ou de la variante (nommée <b>v1</b>, <b>v2</b>, etc.) déplace
  aussi l'échiquier en avant ou en arrière.</p>

  <p>Le panneau d'information du jeu dispose d'un
  <b><green>menu contextuel</green></b> activé avec un clic-droit de la souris, avec
  une option pour <term>cacher le prochain coup</term> (utile si vous vous
  entraînez en révisant un jeu et voulez deviner chaque coup).
  De ce menu, vous pouvez aussi
  <term>marquer pour suppression ou restauration</term> la partie en cours.
  Voir la page d'aide du <a GameInfoArea>panneau d'information</a>
  pour plus de détails.</p>



  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

    ####################
    ### Menus help screen:
  
set helpTitle(F,Menus) "Les menus de Scid"
set helpText(F,Menus) {
    <h1>Les menus de Scid</h1>
    
    <h3><name File>Fichier</name></h3>
    <ul>
      <li><menu>Nouvelle...</menu> crée une nouvelle base de données Scid vide.</li>
      <li><menu>Ouvrir...</menu> ouvre une base de données Scid existante.</li>
      <li><menu>Trouver</menu> ouvre la fenêtre de <a Finder>recherche de fichier</a>.</li>
      <li><menu>Signets</menu> menu des <a Bookmarks>signets</a>.</li>
      <br>
      <li><menu>Ouvrir une base dans l'arbre</menu> ouvre une base dans la fenêtre
        d'<a Tree>arbre</a>.</li>
      <li><menu>Ouvrir une base récente dans l'arbre</menu> ouvre une base récente
        dans la fenêtre d'<a Tree>arbre</a>.</li>
      <br>
      <li><menu>1/2/3/4/5/.../9/...</menu> liste les base de données récemment
        ouvertes.</li>
      <br>
      <li><menu>Quitter</menu> quitte Scid.</li>
    </ul>
    
    <h3><name Base>Base</name></h3>
    <ul>
      <li><menu>Fermer</menu> ferme la base de données Scid actuelle.</li>
      <br>
      <li><menu>Exporter les parties filtrées</menu> exporte toutes les parties
        sélectionnées par un <a Searches Filter>filtre</a> de la base
        de données active. Consultez la page d'aide <a Export>Exportation
          de fichiers</a>.</li>
      <ul>
        <li>dans un fichier <a PGN>PGN</a>.</li>
        <li>dans un fichier HTML simple.</li>
        <li>dans un fichier HTML avec animation des coups sur l'échiquier en javascript.</li>
        <li>dans un fichier <a LaTeX>LaTeX</a>.</li>
        <li>dans une autre base de données existante.</li>
      </ul>
      <li><menu>Importer des fichiers PGN...</menu> importe des fichiers entiers
        contenant des parties d'échecs au format <a PGN>PGN</a> dans
        la base de données actuelle.</li>
      <br>
      <li><menu>Maintenance</menu> accède aux fonctions de
        <a Maintenance>maintenance de la base de données</a>.</li>
      <ul>
        <li><menu>Fenêtre de maintenance</menu> ouvre ou referme la fenêtre de
          maintenance de la base de données.</li>
        <li><menu>Compacter la base...</menu> <a Compact>compacte</a>
          la base de données.</li>
        <li><menu>Classer les parties suivant ECO</menu> recalcule le
          <a ECO>code ECO</a> de toutes les parties de la base de données.</li>
        <li><menu>Editeur de noms</menu> remplace toutes les occurrences du nom
          d'un joueur, du lieu, de l'événement ou du nom de la ronde dans la base de données.</li>
        
        <li><menu>Enlever les marqueurs PGN</menu> enlève tous
          les marqueurs <a PGN>PGN</a>.</li>
        <li><menu>Purger les doublons...</menu> recherche les parties en
          <a Maintenance Twins>doublon</a> dans la base de données.</li>
        <li><menu>Trouver les doublons</menu> ouvre/actualise la fenêtre de
          recherche de <a Maintenance Twins>doublons</a>.</li>
        
        <li><menu>Nettoyer</menu> réalise toutes les actions de
          <a Maintenance>maintenance</a> sélectionnées pour la base en cours.</li>
      </ul>
      
      <li><menu>Lecture seule</menu> rend la base de données actuelle en lecture seule.</li>
      <li><menu>Orthographie des noms</menu> effectue des corrections orthographiques
        des noms et du classement Elo.</li>
      <ul>
        <li><menu>Corriger les noms de joueurs...</menu> vérifie l'orthographe des noms de joueurs.</li>
        <li><menu>Corriger les événements...</menu> vérifie l'orthographe des noms d'événements.</li>
        <li><menu>Corriger les noms de lieux...</menu> vérifie l'orthographe des noms de lieux.</li>
        <li><menu>Corriger les noms des rondes...</menu> vérifie l'orthographe des noms de rondes.</li>
        
        <li><menu>Ajouter les classements Elo</menu> vérifie et corrige le classement Elo des joueurs.
          Chaque fois qu'un joueur n'a pas de classement mais que son classement est listé dans
          le fichier de vérification de noms à la date de la partie, ce classement est pris en compte.</li>
      </ul>
      <br>
      <li><menu>Base 1/2/3/4/5/.../9</menu> ces commandes vous permettent de basculer entre
        les emplacements de base de données disponibles et la base de données
        <a Clipbase>Clipbase</a>.</li>
    </ul>
    
    <h3><name Edit>Édition</name></h3>
    <ul>
      <li><menu>Annuler</menu> annule la dernière modification de cette partie.</li>
      <li><menu>Rétablir</menu> refait la dernière modification de cette partie.</li>
      <br>
      <li><menu>Définir la position de départ</menu> définit la position de départ du jeu en cours.</li>
      <li><menu>Copier la position</menu> copie la position en cours en notation FEN
        vers la sélection de texte (presse-papiers).</li>
      <li><menu>Coller la position de départ</menu> initialise la position de départ à partir
        de la sélection de texte courante (presse-papiers). Ce texte doit être une notation FEN valide.</li>
      
      <li><menu>Copier la partie dans le presse-papiers</menu> copie la partie en cours
        en texte PGN vers le presse-papiers.</li>
      <li><menu>Coller le presse-papiers comme partie PGN...</menu> ouvre la
        <a Import>fenêtre d'importation</a> pour saisir une partie en notation PGN,
        le texte du presse-papiers est inséré automatiquement ou rempli avec la partie en cours.
        Après modification éventuelle, ce presse-papiers peut être importé comme
        partie active.</li>
      
      <br>
      <li><menu>Épurer</menu> supprime tous les commentaires ou les variantes de cette partie.</li>
      <ul>
        <li><menu>Commentaires</menu> épure cette partie de tous les commentaires et annotations.</li>
        <li><menu>Variantes</menu> épure cette partie des variantes.</li>
        <li><menu>Coups depuis le début</menu> épure cette partie des coups depuis le début.</li>
        <li><menu>Coups jusqu'à la fin</menu> épure cette partie des coups jusqu'à la fin.</li>
      </ul>
      <br>
      <li><menu>Vider la Clipbase</menu> vide la base <a Clipbase>Clipbase</a>
        pour qu'elle ne contienne plus aucune partie.</li>
      <li><menu>Copier dans la Clipbase</menu> copie la partie en cours
        vers la base de données <a Clipbase>Clipbase</a>.</li>
      <li><menu>Coller depuis la Clipbase</menu> colle le jeu actif de
        la base <a Clipbase>Clipbase</a> pour être le jeu actif de l'actuel base de données.</li>
    </ul>
    
    <h3><name Game>Partie</name></h3>
    <ul>
      <li><menu>Nouvelle partie</menu> réinitialise la partie de jeu active
        à un état vide, ignorer toutes les modifications non enregistrées.</li>
      <li><menu>Recharger la partie</menu> recharge la partie en cours,
        en supprimant toutes les modifications apportées.</li>
      <br>
      <li><menu>Enregistrer: remplacer la partie</menu> enregistre la partie en cours,
        en remplaçant sa version originale dans la base de données.</li>
      <li><menu>Enregistrer: ajouter nouvelle partie...</menu> enregistre la partie
        actuelle en tant que nouvelle partie, en l'ajoutant à la fin de la base de données.</li>
      
      <li><menu>Exporter la partie en cours</menu> sauvegarde la partie en cours dans un fichier
        texte au format PGN, HTML ou LaTeX. Voir la page d'aide <a Export>Exporter</a>.</li>
      <ul>
        <li>dans un fichier <a PGN>PGN</a>.</li>
        <li>dans un fichier HTML simple.</li>
        <li>dans un fichier HTML avec animation des coups sur l'échiquier en javascript.</li>
        <li>dans un fichier <a LaTeX>LaTeX</a>.</li>
        
      </ul>
      <br>
      <li><menu>Charger la première partie/Partie précédente/Partie suivante/dernière partie</menu>
        chargent la première partie, la partie précédente, la partie suivante
        ou la dernière partie selon le <a Searches Filter>filtre</a> appliqué à la base de données.</li>
      <li><menu>Charger une partie au hasard</menu> charge au hasard
        une des parties sélectionnées par le <a Searches Filter>filtre</a>.</li>
      
      <li><menu>Partie numéro...</menu> charge la partie en fonction
        de son numéro de jeu dans la base de données actuelle.</li>
      <br>
      <li><menu>Identifier l'ouverture</menu> recherche la plus profonde position dans
        la partie en cours qui se trouve dans la <a ECO>nomenclature ECO</a>.</li>
      <li><menu>Aller au coup numéro...</menu> va au coup numéro spécifié dans la partie en cours.</li>
      <li><menu>Trouver la nouveauté...</menu> recherche le premier coup de
        la partie en cours qui n'a pas été joué auparavant.</li>
    </ul>
    
    <h3><name Search>Rechercher</name></h3>
    <ul>
      
      <li><menu>Position...</menu> recherche des parties de jeu selon la
        <a Searches Board>position actuelle de l'échiquier</a>.</li>
      
      <li><menu>En-tête...</menu> recherche des parties selon les informations contenus
        dans l'<a Searches Header>en-tête</a> telles que le nom du joueur,
        le tournoi, etc. Ce menu permet aussi de rechercher n'importe quel texte
        dans la représentation <a PGN>PGN</a> du jeu.</li>
      <li><menu>Matériel ou motif...</menu> recherche un
        <a Searches Material>matériel ou motif</a> particulier sur l'échiquier.</li>
      <br>
      <li><menu>Chercher un joueur</menu> ouvre ou referme la <a PList>fenêtre de recherche de joueur</a>.</li>
      <li><menu>Chercher un tournoi</menu> ouvre ou referme la <a Tmt>fenêtre de recherche de tournoi</a>.</li>
      <br>
      <li><menu>En utilisant un fichier d'options...</menu> recherche des parties de jeu
        en utilisant les paramètres d'un <a Searches Settings>fichier d'options de recherche</a>.</li>
    </ul>
    
    <h3><name Play>Jouer</name></h3>
    
    <ul>
      <li><menu>Partie sérieuse</menu> démarre une <a SeriousGame>partie sans handicap</a> contre
        un moteur d'échecs UCI, dans les conditions d'un tournoi.</li>
      
      <li><menu>Partie tactique</menu> démarre une <a TacticalGame>partie tactique</a> avec un moteur d'échecs de niveau réglable.</li>
      <li><menu>Jouer sur internet</menu> ouvre la <a FICSLogin>connexion</a> sur
        le serveur <a FICS>Free Internet Chess Server</a> (FICS, freechess.org), pour jouer contre des
        adversaires humains ou des moteurs d'échecs, observer des parties, ou participer
        à des sessions d'entraînement.</li>
      <br>
      
      <li><menu>Entraînement</menu> accède aux différents modes d'entraînement disponibles dans Scid.</li>
      <ul>
        
        <li><menu>Ouvertures</menu> <a OpeningTrainer>entraînement avec un répertoire d'ouvertures</a>.</li>
        <li><menu>Exercices tactiques</menu> résolution de <a TacticsTrainer>problèmes tactiques</a>.</li>
        <li><menu>Reviser la partie</menu> pour <a ReviewGame>étudier la partie</a> en devinant les coups joués.</li>
        <li><menu>Calcul de variantes</menu> entraînement au <a CalVar>calcul de variantes</a>.</li>
      </ul>
      <br>
      <li><menu>Échecs par correspondance</menu> permet de gérer les
        <a Correspondence>parties par correspondance</a>,
        jouées par e-mail ou en utilisant le protocole Xfcc.</li>
      <ul>
        <li><menu>Configurer...</menu> permet la <a CCSetupDialog>configuration
            générale</a> du sous-système d'échecs par correspondance.</li>
        <li><menu>Observer des parties...</menu> permet d'observer des
          parties d'échecs en les récupérant d'un site web.</li>
        <br>
        
        <li><menu>Ouvrir la base...</menu> ouvre la base de données des jeux
          par correspondance par défaut (cf. <a CCSetupDialog>configuration</a>).</li>
        <br>
        
        <li><menu>Récupérer les parties</menu> récupére les parties via
          le protocole <a CCXfcc>Xfcc</a>.</li>
        <li><menu>Analyser la boîte de réception</menu> analyse tous les fichiers
          dans la boîte de réception de Scid pour
          <a Correspondence CCWorkflow>traiter de nouvelles parties
            et des nouveaux coups</a>.</li>
        <br>
        <li><menu>Envoyer le coup</menu> transmet le coup via le protocole <a CCXfcc>Xfcc</a>.</li>
        <li><menu>Abandonner</menu> abandonne la partie en cours (par Xfcc).</li>
        <li><menu>Réclamer la partie nulle</menu> réclame un match nul pour la partie en cours (par Xfcc).</li>
        <li><menu>Proposer la partie nulle</menu> propose un match nul pour la partie en cours (par Xfcc).</li>
        <li><menu>Accepter la partie nulle</menu> accepte la proposition de match nul (par Xfcc).</li>
        <li><menu>Page web de la partie...</menu> ouvre la page web associée à la partie Xfcc dans un navigateur web.</li>
        <br>
        <li><menu>Nouvelle partie par e-mail...</menu> commence une nouvelle
          <a CCeMailChess>partie par e-mail</a>.</li>
        <li><menu>e-mailer le coup...</menu> envoie le coup par e-mail à l'adversaire.</li>
      </ul>
    </ul>
    
    <h3><name Windows>Fenêtres</name></h3>
    <ul>
      <li><menu>Editeur de commentaires</menu> ouvre ou referme
        la fenêtre <a Comment>Éditeur de commentaires</a>.</li>
      <li><menu>Fenêtre PGN</menu> ouvre ou referme la <a PGN>fenêtre PGN</a>.</li>
      <li><menu>Montrer le panneau d'info</menu> affiche ou referme le panneau
        d'information.</li>
      <br>
      <li><menu>Liste des parties</menu> ouvre une nouvelle fenêtre
        <a GameList>Liste des parties</a> de la base de données active.</li>
      <li><menu>Changer de base</menu> ouvre ou referme le
        <a Switcher>Sélecteur de base de données</a>, qui vous permet de passer
        à une autre base de données ou de copier facilement des jeux entre des bases
        de données.</li>
      
      <li><menu>Classement du tournoi</menu> ouvre la
        <a Crosstable>fenêtre de classement</a> montrant le tableau croisé
        de toutes les parties du même tournoi que la partie en cours.</li>
      <li><menu>Navigateur ECO</menu> ouvre ou referme
        la fenêtre <a ECO browser>Navigateur ECO</a>.</li>
      
      <li><menu>Statistiques des parties filtrées</menu> ouvre ou referme la fenêtre de
        <term>statistiques des parties filtrées</term> qui donne un résumé des gains et
        des pertes des parties d'échecs sélectionnées d'après
        le <a Searches Filter>filtre</a> appliqué à la base de données en cours.</li>
      <li><menu>Fenêtre d'arbre</menu> ouvre ou referme la
        <a Tree>fenêtre d'arbre</a>.</li>
      <li><menu>Fenêtre de tables de finales</menu> ouvre ou referme la fenêtre qui
        affiche les informations sur <a TB>tables de finales</a>.</li>
      <li><menu>Bibliothèque d'ouvertures</menu> ouvre ou referme la fenêtre de la
        <a Book>bibliothèque d'ouvertures</a> sélectionnée pour afficher
        les probabilités de coups joués.</li>
      <li><menu>Jeu par correspondance</menu> ouvre ou referme la fenêtre pour le
        <a Correspondence CCWorkflow>traitement des parties 
          d'échecs par correspondance</a>.</li>
    </ul>
    
    <h3><name Tools>Outils</name></h3>
    <ul>
      <li><menu>Moteur d'analyse...</menu> affiche la liste de tous les moteurs d'échecs
        installés pour en sélectionner un et le démarrer comme moteur d'analyse n°1.</li>
      <li><menu>Moteur d'analyse #2...</menu> affiche la liste de tous les moteurs d'échecs
        installés pour en sélectionner un et le démarrer comme moteur d'analyse n°2.</li>
      <li><menu>Démarrer le moteur 1</menu> démarre ou arrête le moteur d'échecs n°1,
        affichant l'évaluation de la position actuelle dans la
        <a Analysis>fenêtre d'analyse</a>.</li>
      <li><menu>Démarrer le moteur #2</menu> démarre ou arrête une seconde analyse
        par le moteur d'échecs n°2.</li>
      <br>
      <li><menu>Gestion des messages</menu> Ouvre/fermeture le
        <a Email>gestionnaire d'emails</a>
        pour la gestion des jeux de correspondance par email.</li>
      <li><menu>Graphique de filtre</menu> génère un
        <a FilterGraph>graphique de filtre relatif</a> par exemple pour étudier
        la popularité de l'ouverture actuelle en comparant la position actuelle avec
        toute la base de données.
      </li>
      <li><menu>Graphique de filtre absolu</menu> génère un
        <a FilterGraph>graphique de filtre absolu</a>
      </li>
      <li><menu>Rapport d'ouverture</menu> génère un
        <a Reports Opening>rapport d'ouverture</a> pour la position
        actuelle.</li>
      <li><menu>Suivi des pièces</menu> ouvre la fenêtre
        <a PTracker>Suivi des pièces</a>.</li>
      <li><menu>Config. bibliothèque</menu> permet d'ajuster la probabilité
        d'un coup joué dans la <a Book>bibliothèque d'ouvertures</a>
        sélectionnée.
      </li>
      <li><menu>Connecter un matériel</menu> permet à un matériel externe d'être
        utilisé pour saisir les coups. (Par exemple, l'échiquier électronique DGT,
        l'échiquier Novag Citrine) 
        <ul>
          <li><menu>Configurer</menu> permet de régler les
            <a HardwareConfig>paramètres de base</a> pour l'utilisation
            de matériel externe.
          </li>
          <li><menu>Connecter via Input Engine</menu> se connecte avec un
            matériel DGT qui fournit un pilote compatible avec le
            <a InputEngine>protocole Input Engine</a> (par exemple
            <url http://dgtdrv.sourceforge.net>site webdgtdrv</url>.)
          </li>
          <li><menu>Connecter Novag Citrine</menu> se connecte avec un
            <a Novag>échiquier Novag Citrine</a>.
          </li>
        </ul>
      </li>
      <br>
      <li><menu>Informations sur le joueur</menu> affiche les
        <a PInfo>informations</a> sur l'un des deux joueurs
        de la partie en cours.</li>
      <ul>
        <li><menu>Blancs</menu> s'informer sur le joueur des blancs.</li>
        <li><menu>Noirs</menu> s'informer sur le joueur des noirs.</li>
      </ul>
      <li><menu>Rapport sur le joueur...</menu> génère un
        <a Reports Player>rapport sur le joueur</a></li>
      <li><menu>Graphique Elo</menu> affiche le graphique de l'évolution du
        <a Graphs Rating>classement Elo</a> des deux joueurs.</li>
      <li><menu>Graphique de score</menu> affiche le
        <a Graphs Score>graphique de score</a> pendant la partie.</li>
    </ul>
    
    <h3><name Options>Options</name></h3>
    <p>
      Ce menu fournit des entrées pour régler la plupart des paramètres configurables
      de Scid. Le menu <menu>Enregistrer les options</menu> enregistre les options
      actuelles dans le fichier "<b>~/.scid[ver]/config/options.dat</b>", avec
      <b>[ver]</b> le numéro de version de Scid; ou dans le fichier
      "<b>options.dat</b>" dans le sous-répertoire "<b>config</b>" des programmes
      exécutables Scid pour les utilisateurs Windows. Ce fichier est chargé
      à chaque démarrage de Scid.
    </p>
    
    <h3><name Help>Aide</name></h3>
    <p>
      Ce menu contient des fonctions d'aide, l'accès à l'astuce du jour,
      ou la fenêtre de démarrage qui fournit des informations sur
      les fichiers que Scid a chargés au démarrage.
    </p>
    
    <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
  }

####################
### Entering moves help:

set helpTitle(F,Moves) "Saisie de coups d'échecs"
set helpText(F,Moves) {
<h1>Saisie de coups d'échecs</h1>
  <p>Dans Scid, vous pouvez entrer les coups d'une partie en utilisant la souris ou
  le clavier. Vous pouvez effectuer les actions suivantes :</p>
  <ul>
  <li><a Moves Play>Jouer un coup</a></li>
  <li><a Moves Retract>Annuler un coup</a></li>
  <li><a Moves Mark>Marquer des cases et des directions</a></li>
  <li><a Moves Mistakes>Corriger les erreurs</a></li>
  <li><a Moves Keyboard>Entrer des coups au clavier</a> avec ou
  sans auto-complétion</li>
  <li><a Moves Null>Entrer des coups nuls</a></li>
  <li><a Moves Informant>Entrer les symboles d'annotation courants</a></li>
  </ul>

  <h3>Actions de saisie courantes</h3>
  <h4><name Play>Jouer un coup</name></h4>
  <p><b>Jouer le coup suggeré</b>. Lorsque vous déplacez la souris
  sur une case, celle-ci et une autre case changera de couleur s'il y a
  un coup légal vers ou depuis cette case. C'est le <term>coup suggéré</term>.
  Pour jouer ce coup, cliquez simplement sur le bouton <b>gauche</b> de la souris.
  Vous pouvez désactiver la suggestion de coup en utilisant le menu
  <menu>Options</menu><menu>Coups</menu><menu>Montrer les coups suggérés</menu>
  si cela vous ennuie.</p>
  <p><b>Jouer un coup non suggeré</b>. Pour tout autre coup que celui suggéré,
  il suffit d'appuyer sur le bouton <b>gauche</b> de la souris sur une pièce,
  et le relâcher sur l'autre case.</p>

  <p>Jouer un coup différent de celui déjà joué ajoute automatiquement une nouvelle
  ligne de variante dans la partie. Les nouvelles variantes sont signalées dans
  la <a GameInfoBar BD_Alert>barre de menu</a> en bas de
  la fenêtre principale pour des actions supplémentaires (annuler, remplacer la
  ligne principale etc.). Selon les réglages du menu
  <menu>Options</menu><menu>Coups</menu> :</p>
  <ul>
  <li><menu>Surbrillance du dernier coup</menu>. Scid marquera le dernier coup par
  un cadre coloré autour des cases concernées.</li>
  <li><menu>Couleur codée Gloss of Danger</menu>. Scid marquera les cases attaquées
  selon le niveau de danger en jaune ou rouge (non protégée).</li>
  </ul>

  <h4><name Retract>Annuler un coup</name></h4>
  <p>Pour reprendre un coup, appuyez sur le bouton <b>droit</b> de la souris,
  ou un des raccoucis clavier <b>Ctrl+Supprimer</b> ou <b>Ctrl+Retour arrière</b>.
  Cela permet de revenir en arrière d'un coup, et de supprimer le coup si c'est
  le dernier coup de la partie ou de la variante.</p>




  <h4><name Mark>Marquer des cases et des directions</name></h4>
  <p>
  Les cases et les flèches de couleur peuvent être saisis directement sur le plateau
  sans l'utilisation de l'<a Comment>éditeur de commentaire</a> en utilisant
  les boutons de la souris.</p>
  <ul>
  <li><b>Ctrl+Clic-gauche</b> marque la case en couleur.</li>
  <li><b>Ctrl+Clic-gauche/glisser</b> dessine une flèche d'une case à l'autre.</li>
  </ul>
  <p>Par défaut verte, la couleur des marquages peut être changée dans
  l'<a Comment>éditeur de commentaire</a>.</p>



  <h4><name Mistakes>Corriger les erreurs</name></h4>
  <p>
  Si vous saisissez une partie et que vous voyez soudain un coup incorrect,
  plusieurs coups plus tôt, il est possible de le corriger sans perdre le supplément
  de coups que vous avez déjà ajoutés. La seule façon est d'éditer la 
  <a PGN>représentation PGN</a> du jeu : ouvrez la
  <a Import>fenêtre Importer un jeu PGN</a> accessible par le menu
  <menu>Édition</menu><menu>Coller le presse-papiers comme partie PGN</menu>
  (raccourci clavier : <b>Ctrl+Maj+I</b>), sélectionner le bouton
  <b>[Coller la partie en cours]</b>, corrigez le coup incorrect, puis
  sélectionnez <b>[Importer]</b>.
  </p>
  
  <h3><name Keyboard>Entrée de coup au clavier</name></h3>
  <p>Pour entrer des coups au clavier, il suffit d'appuyer sur les touches des
  lettres et des chiffres en <term>notation SAN</term>,
  <i>sans</i> le symbole de capture (x) ou le symbole de promotion (=). Les lettres
  pour désigner les pièces sont celles de l'anglais :</p>
  <ul>
    <li>[K] : Roi (King)</li>
    <li>[Q] : Dame (Queen)</li>
    <li>[B] : Fou (Bishop)</li>
    <li>[N] : Cavalier (Knight)</li>
    <li>[R] : Tour (Rook)</li>
    <li>[O][K] : roque du côté du roi</li>
    <li>[O][Q] : roque du côté de la dame</li>
    <li>[a .. h] : nom de colonne</li>
    <li>[1 .. 8] : numéro de ligne</li>
  </ul>
  <p>Les coups sont adaptés à la casse, donc vous pouvez taper
  <b>[n][f][3]</b> au lieu de <b>Nf3</b>, par exemple pour déplacer le cavalier
  en f3 (N : Knight en anglais) -- mais voir la note ci-dessous
  sur les conflits avec les coups de pions. Pour s'assurer qu'aucun coup n'est le
  préfixe d'un autre coup, la notation pour le roque du côté du roi et du côté de
  la dame est <b>[O][K]</b> et <b>[O][Q]</b> respectivement, au lieu des habituels
  O-O et O-O-O.</p>
  <p>Scid peut aussi accepter la saisie de coup en notations coordonnées, par ex.
  <b>g1f3</b> ou <b>g1-f3</b> si l'option
  <menu>Options</menu><menu>Coups</menu><menu>Saisie de coordonnées</menu> est
  activée</p>
  <p>Lorsque vous entrez un coup, la barre de menu en bas de l'échiquier affiche
  la liste des coups correspondants. Vous pouvez appuyer sur la barre
  <b>[Espace]</b> à tout moment pour choisir le premier coup correspondant dans
  la liste et l'ajouter au jeu. Pour supprimer un caractère, appuyez sur
  <b>[Retour arrière]</b> ou <b>[Supprimer]</b>.</p>
  <p><b>Notez</b> qu'une lettre minuscule correspond à un pion en premier, donc un
  [b] peut correspondre à un pion ou à un fou (B : Bishop en anglais), mais s'il y
  a un conflit vous devez utiliser un [B] majuscule pour le coup du fou.</p>
  
  <h4>Auto-Complétion</h4>
  <p>Dans le menu <menu>Options</menu><menu>Coups</menu>, vous pouvez activer ou
  désactiver <menu>Auto-Complétion</menu> de coups.
  Avec l'auto-complétion, un coup est effectué dès que vous avez tapé suffisamment
  pour le distinguer de tout autre coup légal. Par exemple, vous n'auriez qu'à taper
  [n][f] à la place de [n][f][3] pour <b>Nf3</b> à la position initiale de
  l'échiquier.</p>
  
  <h4><name Null>Entrer des coups nuls</name></h4>
  <p>Les <a Annotating Null>coups nuls</a> (vides) peuvent être utiles
  dans les variantes où vous voulez sauter un coup pour un côté. Vous pouvez entrer
  un coup nul avec la  souris en capturant un roi avec l'autre roi, ou avec le
  clavier en en tapant "<b>--</b>", c'est-à-dire en appuyant deux fois sur la touche
  <b>moins</b>.</p>
  
  <h3><name Informant>Entrer les symboles d'annotation courants</name></h3>
  <p>Vous pouvez également ajouter des
  <a NAGs>symboles d'annotation standards</a> en utilisant le clavier
  dans la fenêtre principale, sans avoir besoin d'utiliser la fenêtre de
  l'<a Comment>éditeur de commentaire</a>. La liste suivante indique les
  symboles que vous pouvez ajouter, ainsi que leurs raccourcis clavier :</p>


  <ul>
  <li> ! : [!][Retour] </li>
  <li> ? : [?][Retour] </li>
  <li> !? : [!][?][Retour] </li>
  <li> ?! : [?][!][Retour] </li>
  <li> !! : [!][!][Retour] </li>
  <li> ?? : [?][?][Retour] </li>
  <li> </li>
  <li> +- : [+][-] </li>
  <li> +/- : [+][/] </li>
  <li> += : [+][=] </li>
  <li> = : [=][Retour] </li>
  <li> -+ : [-][+] </li>
  <li> -/+ : [-][/] </li>
  <li> =+ : [=][+] </li>
  </ul>
  <p><b>Note.</b> Scid utilise également certains de ces symboles pour les
  annotations automatiques. Pour cela, ces symboles doivent être associés
  avec des valeurs de pions. Ces valeurs de pions peuvent être définies via le menu
  <menu>Options</menu><menu>Configurer les valeurs de notation</menu>. Notez que
  depuis Scid 4.0, de nombreux glyphes NAG peuvent être affichés symboliquement si
  votre système supporte correctement l'UTF-8. La connexion entre une valeur
  numérique NAG et sa représentation symbolique dans Scid est donnée sur la page
  <a NAGs>Valeurs standards NAG</a>.


  </p>

  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

########################################
### Searches help screen:

set helpTitle(F,Searches) "Recherche de parties dans Scid"
set helpText(F,Searches) {
<h1>Recherche de parties dans Scid</h1>
<p>
Scid peut effectuer différents types de recherche sur une base de données.
Les trois principaux types de recherche s'effectuent :
<ul>
<li>1. selon la <term>position</term> actuelle de l'échiquier,</li>
<li>2. selon le <term>matériel et motif</term> de pièces spécifiés,</li>
<li>3. par des informations d'<term>en-tête</term> telles que les joueurs,
le résultat ou la date.</li>
</ul>
</p>

<p>
En plus de cela, il existe également un mode de recherche automatique appelé
<a Tree>Arbre</a> qui est expliqué séparément.
</p>

<h3><name Filter>Filtre de recherche</name></h3>
<p>
Les recherches dans Scid sont basées sur le concept de
<term>filtre</term>. Un filtre représente un sous-ensemble de la base de données
actuelle, et à tout moment, chaque jeu est soit inclus dans le filtre, soit exclu.
</p>
<p>
Avec chaque type de recherche, vous pouvez choisir de restreindre le filtre
existant, de l'ajouter, ou encore de l'ignorer et ainsi de rechercher dans
toute la base de données. Ce choix permet de construire progressivement des
recherches complexes combinant plusieurs filtres.
</p>
<p>
Vous pouvez également copier tous les jeux du filtre d'une base de données vers
une autre, en utilisant le <a Switcher>sélecteur de base de données</a>.
</p>

<p>
Avec les recherches de position exacte, d'<a Tree>arbre</a> ou
de matériel et de motif, le numéro du coup de la première position concordante
de chaque partie sélectionnée est mémorisé. Donc quand vous chargez chaque partie,
la position correspondante s'affiche automatiquement.
</p>
<p>

<b>Note.</b> Les recherches s'appliquent uniquement aux coups de la ligne
principale d'un jeu, et non aux coups dans les variantes sauf pour
la recherche de position.
</p>

<h3><name Board>Recherche selon la position actuelle de l'échiquier</name></h3>
<p>
Cette catégorie de critères recherche les jeux contenant la position actuellement
affichée, en ignorant le roque et les droits de <i>prise en passant</i>.
</p>
<p>
Il existe quatre types de recherche de position, qui nécessitent tous
d'avoir <i>exactement le même matériel et le même côté au trait</i> pendant le match.
Les quatre types sont :
<ul>
<li>1. <b>Position exacte</b> : les deux positions doivent correspondre sur chaque case
de l'échiquier avec les mêmes pièces sur les mêmes cases,</li>
<li>2. <b>Pions</b> : la structure de pions doit correspondre exactement, le matérel est
le même mais les autres pièces peuvent être n'importe où,</li>
<li>3. <b>Colonnes</b> : le nombre de pions blancs et noirs sur chaque colonne doit
correspondre exactement, mais d'autres pièces peuvent être n'importe où, et enfin</li>
<li>4. <b>Même matériel</b> : les pions et les pièces peuvent être n'importe où.</li>
</ul>
</p>
<p>
La recherche de <term>structure de pions</term> est utile pour étudier les ouvertures
par structure de pions, et la recherche par colonnes et par matériel sont utiles pour
trouver des positions similaires dans une phase finale.
</p>
<p>
Pour rechercher une position différente de la <term>position active</term>,
vous devez d'abord définir la position, par exemple dans le menu
<menu>Éditer</menu><menu>Définir la position de départ</menu>,
puis lancer la recherche.
</p>
<p>
Vous pouvez demander que les résultats de recherche apparaissent dans des variantes
au lieu d'éxaminer seulement les coups réels du jeu en sélectionnant
<b>Chercher dans les variantes</b>, mais cela peut ralentir la recherche si votre
base de données est grande et possède de nombreuses parties avec des variantes.
</p>
<p>
Si vous recherchez une position avec le même matériel sans tenir compte de la couleur
des adversaires, sélectionnez l'option <b>Ignorer les couleurs</b>.
</p>

<h3><name Material>Recherche d'un matériel ou motif sur l'échiquier</name></h3>
<p>
Cette recherche est utile pour trouver des
<term>thèmes de milieu ou de fin de partie</term>.
Vous pouvez spécifier des quantités minimum et maximum de chaque type de matériel,
et trouver des motifs tels qu'un fou sur f7, ou un pion sur la colonne f.
</p>
<p>
Un certain nombre de paramètres de matériel et de motifs courants sont fournis, tels
que des <i>finales de tour contre pion</i> ou des <i>pions dame isolés</i>.
</p>
<p>
<b>Astuce :</b> La vitesse de recherche de motifs peut varier considérablement. Vous
pouvez réduire le temps nécessaire pour une recherche en définissant intelligemment
des restrictions. Par exemple, si vous définissez le nombre de coups minimum à 20 pour
une finale, toutes les parties qui se terminent en moins de 20 coups peuvent être
ignorées.
</p>

<h3><name Header>Recherche dans l'en-tête</name></h3>
<p>
Cette recherche peut être utilisée pour trouver des informations du jeu qui sont
stockés dans l'<term>en-tête PGN</term> comme la date, le résultat, les noms,
les drapeaux et les classements. Elle ne nécessite donc pas de décodage de tous
les coups de la partie.
</p>
<p>
Pour qu'une partie d'échecs corresponde à une recherche d'en-tête, <b>tous</b>
les champs que vous spécifiez doivent correspondre.
</p>
<h4>Recherche avec des caractères jokers</h4>

<p>
Par défaut, la recherche selon les champs de nom "Blancs", "Noirs", "Événement", "Lieu"
et "Ronde" fait correspondre n'importe quelle partie du texte à l'intérieur du nom,
sans distinction de majuscules et en ignorant les espaces.</p>
<p>
Vous pouvez effectuer des recherches sensibles à la casse avec des caractères de
substitution pour ces mêmes champs en mettant le texte recherché entre guillemets
<i>doubles</i>, avec les caractères génériques <b>?</b> représentant un caractère et
<b>*</b> représentant zéro ou plusieurs caractères.</p>
<p>
Par exemple, une recherche selon le champs "Lieu" avec <b>USA</b>
trouvera des villes américaines et aussi La<b>usa</b>nne SUI, ce qui n'est probablement
pas ce que vous vouliez ! Par contre, une recherche selon le champs "Lieu" avec
<b>"* USA"</b> ne correspondra qu'aux villes des États-Unis d'Amérique (pensez à utiliser les guillemets doubles).
</p>
<h4>Options de recherche particuliers</h4>
<p>
Si vous recherchez un joueur particulier ou une paire d'adversaires, en tant que Blancs
ou Noirs et peu importe la couleur qu'ils ont jouée, sélectionnez l'option
<b>Ignorer les couleurs</b>.
</p>
<p>
Si le champ d'en-tête "Annotateur" est réglé correctement, ce champs peut être
recherché sans tenir compte de la casse. La case à cocher <b>Jeux annotés uniquement</b>
agit comme un filtre OU logique pour les drapeaux
<b>Commentaires, Variantes, Annotations</b>, et peut être utilisé pour sélectionner
tous les jeux avec tout type d'annotations.
</p>
<p>
Si une recherche par <a ECO>code ECO</a> est effectuée,
les jeux qui n'ont pas de code ECO sont ignorés, sauf si la case à cocher
<b>Parties sans code ECO ?</b> est cochée.
</p>
<h4>Recherche dans la représentation PGN du jeu</h4>
<p>
La recherche d'en-tête peut aussi être utilisée pour trouver n'importe quel texte
(sensible à la casse et sans caractères génériques) dans la
<a PGN>représentation PGN</a> du jeu. Vous pouvez saisir jusqu'à trois phrases
de texte dans les champs <b>PGN contient le texte</b>, et elles doivent toutes
apparaître dans un jeu pour qu'il y ait une correspondance.
</p>
<p>
Cette recherche est très utile pour rechercher dans les commentaires ou les balises
supplémentaires d'une partie tel que <b>lost on time</b> ou <b>Annotator</b>, ou
pour une séquence de coups comme <b>Fxh7+</b> et <b>Rxh7</b> par exemple pour trouver
une sacrifice de fou sur h7 qui a été accepté.
</p>
<p>
Cependant, ce type de recherche peut être <i>très lent</i> car tous les jeux qui
correspondent à d'autres critères doivent être décodés et analysés pour déterminer
s'ils contiennent les phrases de texte. C'est donc une bonne idée de limiter autant
que possible ces recherches.</p>
<p>
<b>Exemples.</b> Voici quelques exemples.
<ul>
<li>Pour trouver des jeux avec des sous-promotions d'un pion vers une tour,
recherchez <b>=R</b> et définissez également le <a Flags>drapeau</a>
<b>Promotions</b> sur Oui.</li>
<li>Lorsque vous recherchez du texte qui apparaîtrait dans les commentaires, définissez
le <a Flags>drapeau</a> <b>Commentaires</b> sur Oui.</li>
<li>Si vous recherchez les coups <b>Fxh7+</b> et <b>Rxh7</b>, vous pourriez
par exemple limiter la recherche aux jeux avec un résultat 1-0 et au moins 40
demi-coups, ou effectuer d'abord une
<a Searches Material>recherche de matériel ou de motif</a> pour trouver
les parties où un fou blanc se déplace vers h7.</li>
</ul>
</p>

<h3><name Settings>Enregistrement des paramètres de recherche</name></h3>

<p>
Les fenêtres de recherche de "Matériel ou motif" et "En-tête" fournissent un
bouton <b>Enregistrer</b>. Cela vous permet de sauvegarder les paramètres de recherche
courants dans un <a Formats>fichier SearchOptions</a> (avec le suffixe .sso)
pour une utilisation ultérieure. Pour effectuer une recherche à l'aide
d'un fichier <term>SearchOptions</term> précédemment enregistré, sélectionnez le menu
<menu>Rechercher</menu><menu>En utilisant un fichier d'options...</menu>.
</p>

<h3>Temps de recherche et jeux ignorés</h3>
<p>
La plupart des recherches produisent un message indiquant le temps nécessaire
et le nombre des jeux qui ont été ignorés. Un <term>jeu ignoré</term> est
un jeu qui peut être exclu du résultat de recherche sans décoder aucun de ses coups,
sur la base des informations stockées dans l'index. Voir la page d'aide sur les
<a Formats>formats de fichiers</a> pour plus d'information.
</p>

<p><footer>(Mise à jour: Scid 4.7.1, janvier 2020)</footer></p>
}

#################
### Clipbase help:

set helpTitle(F,Clipbase) "Utilisation de la base de données Clipbase"
set helpText(F,Clipbase) {
<h1>Utilisation de la base de données Clipbase</h1>
  <p>En plus des bases de données que vous avez ouvertes, Scid fournit
  une base de données <term>Clipbase</term>, qui est comme n'importe quelle
  autre base sauf qu'elle n'existe qu'en mémoire et n'a pas de fichiers pour
  le stockage permanent.</p>

  <p>La Clipbase permet de <term>copier/coller une partie</term> à partir du menu
  <menu>Édition</menu>, ou de la stocker temporairement lors des entraînements
  aux ouvertures.</p>
  <p>La Clipbase est utile comme <term>base de données temporaire</term>, pour
  fusionner les résultats des recherches sur plus d'une base de données ou
  pour le traitement des résultats d'une recherche séparément des autres bases
  de données.</p>
  <p>Par exemple, supposons que vous vouliez vous préparer contre un adversaire
  et que vous ayez cherché une base de données afin que le
  <a Searches Filter>filtre de recherche</a> ne contienne que
  les jeux où l'adversaire a joué contre les Blancs.
  Vous pouvez copier ces parties dans la Clipbase, en les faisant glisser
  depuis leur base d'origine à la Clipbase dans la fenêtre
  <a Switcher>Sélecteur de bases de données</a>,
  Activez ensuite la Clipbase, puis ouvrez la fenêtre <a Tree>Arbre</a>
  pour examiner le répertoire de ce joueur.</p>
  <p><b>Notes.</b></p>
  <ul>
  <li><b>Copier une collection de jeux.</b> Vous pouvez copier les jeux dans le filtre d'une base de données
  directement vers une autre base de données ouverte sans avoir besoin de
  la Clipbase comme stockage intermédiaire, en utilisant la fenêtre
  <a Switcher>Sélecteur de base de données</a>.</li>
  <li><b>Vider la Clipbase.</b> La Clipbase <i>ne peut pas être fermée</i>. Utiliser le menu
  <menu>Base</menu><menu>Fermer</menu> quand la Clipbase est la
  <term>base active</term> est équivalente à
  <menu>Édition</menu><menu>Vider la Clipbase</menu>.</li>
  <li><b>Limite de la Clipbase.</b> La Clipbase a une limite de 100 000 parties à tout moment, puisqu'elle
  existe en mémoire seulement. Son contenu sera perdu en quittant Scid.</li>
  <li><b>Presse-papiers.</b> Le <term>presse-papiers</term> système permet de copier/coller un texte
  venant d'un autre logiciel, par exemple une position d'échiquier décrite par
  un texte en <term>notation FEN</term>, ou
  d'<a Import>importer une partie en notation PGN</a>. Voir le menu
  <menu>Édition</menu> pour cela.</li>
  </ul>
  
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

#################################
### Variations and comments help:

set helpTitle(F,Annotating) "Annotation des parties d'échecs"
set helpText(F,Annotating) {
<h1>Annotation des parties d'échecs</h1>
  <p>
  Scid vous permet d'ajouter trois types d'annotation à chaque coup d'une partie
  d'échecs :</p>
  <ul>
  <li>des <b>symboles</b> d'évaluation de position,</li>
  <li>un <b>commentaire</b> sur la position, avec la possibilité de colorier des cases ou
  dessiner des flèches sur l'échiquier,</li>
  <li>ou des <b>variantes</b>, des séquences alternatives de coups.</li>
  </ul>
  <p>Cette section décrit les <term>annotations manuelles</term>, voir la section
  <a Analysis Annotate>Analyse</a> pour l'analyse et les
  <term>annotations automatiques</term> par un moteur d'échecs.</p>
  
  <h3>Symboles et commentaires</h3>
  <p>Des symboles sont utilisés pour indiquer l'évaluation d'une position (tels que
  "<b>+-</b>" ou "<b>=</b>"), ou pour indiquer les bons coups ("<b>!</b>") et les
  mauvais coups ("<b>?</b>"). Les commentaires peuvent être n'importe quel
  texte et peut contenir des commandes pour colorier des cases et dessiner des
  flèches sur l'échiquier. Pour ajouter des symboles et des commentaires à un jeu,
  utilisez la fenêtre <a Comment>Éditeur de commentaire</a>.
  La page d'aide <a NAGs>Valeurs standards NAG</a> liste les symboles définies
  dans la norme PGN.</p>
  <p>Notez que chaque coup peut avoir plus d'un symbole d'annotation, mais seulement
  un commentaire. Le commentaire avant le premier coup de la partie est imprimé sous
  forme de texte avant le début du match.</p>
  
  <h3><name Vars>Variantes</name></h3>
  <p>Une <term>variante</term> d'un coup est une séquence alternative de coups
  à un moment particulier dans une partie. Les variantes peuvent aussi contenir
  des commentaires et même récursivement avoir des sous-variantes. En plus des
  commandes à la souris et au clavier disponible pour le
  <a MainWindow Chessboard>plateau de jeu</a> de la fenêtre principale,
  les boutons de la <a GameInfoBar>barre de menu</a> ou du
  <a GameInfoArea>panneau d'information</a> en-dessous de l'échiquier
  et les commandes dans le menu <menu>Édition</menu> peuvent aussi être utilisés pour
  créer, naviguer et éditer les variantes.</p>
  <p><b>Ajouter une variante.</b> Jouer un coup différent de celui déjà joué ajoute
  automatiquement une nouvelle ligne de variante dans la partie. Le raccourci
  <b>Ctrl+A</b> ajoute une nouvelle variante vide.</p>
  
  <p><b>Naviguer dans une variante.</b> Lorsqu'un coup a des variantes, celles-ci
  sont indiquées dans le <a GameInfoArea>panneau d'information</a> de la
  partie. La première sera nommée <b>v1</b>, la seconde <b>v2</b>, etc. Vous pouvez
  cliquer sur une variante pour entrer dans sa ligne de jeu, ou appuyer sur la touche
  <b>v</b> du clavier. Dans dans ce dernier cas, la <term>fenêtre Variantes</term>
  apparaîtra, permettant de sélectionner une variante à l'aide des touches de curseur
  <b>Haut</b>/<b>Bas</b> et la touche <b>Entrée</b>  ou en cliquant dessus avec
  la souris. Pour quitter une variante, vous pouvez utiliser la touche <b>z</b>.
  Au début de la variante, la flèche <b>Haut</b> peut être aussi utilisée
  alternativement.</p>
  <p><b>Notes.</b></p>
  <ul>
  <li>Vous pouvez ajouter explicitement une nouvelle
  <i>variante commençant par le même coup</i> en appuyant sur <b>Ctrl-A</b> et ne
  jouer le coup qu'ensuite. Cela pourrait par exemple simplifier les arbres de
  variantes profondément imbriquées.</li>
  <li><menu>Options</menu><menu>Coups</menu><menu>Montrer fenêtre variantes</menu>
  fera apparaître la fenêtre pop-up Variantes automatiquement, à chaque
  fois qu'un coup avec une variante est trouvé.</li>
  <li>
  <menu>Options</menu><menu>Coups</menu><menu>Montrer flèches pour les variantes</menu>
  affiche les variantes existantes sur l'échiquier. Cela peut être
  particulièrement utile sur les ordinateurs avec un petit écran comme les Netbooks
  si la fenêtre PGN et le panneau d'information sont fermés, voir le menu
  <menu>Fenêtres</menu><menu>Panneau d'information</menu>.</li>
  </ul>
  
  <h3><name Null>Coups nuls</name></h3>
  <p>Parfois, vous pouvez trouver utile dans une variante de sauter un coup
  pour un des joueurs. Par exemple, vous pouvez vouloir ajouter le coup
  <b>14.Fd3</b> à une variante et signalez qu'elle menace
  <b>15.Fxh7+ Rxh7 16.Cg5+</b>
  avec une attaque. Vous pouvez faire cela ajoutant un <term>coup nul</term>
  entre <b>14.Fd3</b> et <b>15.Fxh7+</b>, dans l'exemple ci-dessus. Un coup nul est
  affiché comme "<b>--</b>" et peut être inséré à l'aide de la souris en faisant
  un coup illégal de capture d'un roi avec l'autre, ou en tapant au clavier
  "<b>--</b>" (deux signes moins).</p>
  <p>Notez que les coups nuls ne font pas partie de la <a PGN>norme PGN</a>,
  donc si vous exporter les parties avec des coups nuls vers un fichier PGN, Scid
  fournira (parmi d'autres options d'exportation) une option pour préserver les
  coups nuls ou les convertir en commentaire pour garder la compatibilité avec
  d'autres logiciels. Voir la page d'aide <a Export>Export</a> pour plus de
  détails.</p>
  <p>Notez également que Scid est capable de gérer le déplacement <term>Z0</term>
  comme un coup nul, une notation qui est commune dans certains logiciels d'échecs
  commerciaux.</p>
  
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

###############################
### Comment editor window help:

set helpTitle(F,Comment) "Fenêtre Éditeur de commentaires"
set helpText(F,Comment) {
<h1>Fenêtre Éditeur de commentaires</h1>
  <p>
  La fenêtre <term>Éditeur de commentaires</term> vous permet d'ajouter ou
  de modifier des <a Comment Comments>commentaires</a> et des
  <a Comment Symbols>symboles d'annotation</a> pour les coups dans
  la partie d'échecs active. Les commentaires peuvent contenir des commandes pour
  <a Comment Markers>marquer les cases</a> de l'échiquier en couleur,
  et pour <a Comment Arrows>dessiner des flèches</a>.
  </p>
  
  <h3><name Symbols>Symboles d'annotation</name></h3>
  <p>
  Scid utilise le <a Author Related>standard PGN</a> pour les symboles
  d'annotation, en acceptant les <a NAGs>glyphe d'annotation numérique NAG</a>.
  Les symboles les plus courants tels que <b>[ !? ]</b> ou <b>[+-]</b>
  ont un bouton dans la fenêtre de l'éditeur de commentaire pour
  une saisie rapide. Pour les autres symboles, vous pouvez saisir la
  <a NAGs>valeur numérique NAG</a> appropriée, un nombre de 1 à 255.
  </p>
  <p>
  Par exemple, la valeur 36 du NAG signifie "Les Blancs ont l'initiative" et est
  affiché comme "$36" dans le <a PGN>texte PGN</a> du jeu. Si votre système
  prend correctement en charge les jeux de caractères UTF-8, plusieurs des symboles
  NAG peuvent être affichés par leur <a NAGs>glyphes</a>.
  
  </p>
  <p>
  
  <b>Astuce :</b> vous pouvez ajouter les symboles d'évaluation des coups fréquents
  (!, ?, !!, ??, !? et ?!) dans la fenêtre principale, sans avoir besoin d'utiliser
  la fenêtre de l'éditeur de commentaires, en tapant le symbole suivi de
  la touche [Entrée]. Ceci est particulièrement utile si vous saisissez les
  <a Moves>coups d'échecs en utilisant le clavier</a>. Notez aussi
  que le menu contextuel de la <a PGN>fenêtre PGN</a> offre directement
  un grand nombre des symboles fréquents pour l'annotation.
  </p>
  
  <h3><name Comments>Commentaires</name></h3>
  
  <p>
  Vous pouvez modifier les commentaires en tapant dans la zone de texte prévue à cet
  effet ou les effacer en utilisant les boutons <b>[Effacer]</b>. Le commentaire
  est automatiquement mis à jour dans la <a PGN>notation PGN du jeu</a>.
  </p>
  <p>
  <b>Astuce :</b> sans quitter l'éditeur de commentaires,
  <ul>
  <li>
  
  vous pouvez vous <term>déplacer dans les coups d'une partie</term> avec les touches de curseur
  <b>Gauche</b> ou <b>Droite</b> ou avec <b>Ctrl-U</b> "Aller au coup numéro..." ;
  
  </li>
  <li>pour <term>ajouter un commentaire avant le premier coup</term> dans une variante,
  aller à ce premier coup, appuyer une fois sur la touche fléchée <b>Gauche</b> puis
  insérer le commentaire ;</li>
  <li>en appuyant sur <b>Ctrl-A</b>,
  vous pouvez <term>ajouter une variante directement dans le PGN</term>.
  Cela facilite la création d'un commentaire de pré-coup pour cette nouvelle variante.</li>
  </ul>
  </p>
  
  <h3><name Markers>Coloration des cases</name></h3>

  <p>
  Vous pouvez <term>ajouter des marqueurs de couleur à l'échiquier</term>. Sélectionnez
  le marqueur à utiliser à partir des boutons du tableau, et la couleur à partir des
  boutons de couleur proposés. Il suffit de <b>Ctrl-cliquer</b> avec la souris
  sur une case pour la marquer.
  </p>
  <p>
  Une case peut également être colorée en utilisant une commande spéciale intégrée
  qui peut apparaître n'importe où dans un commentaire. Le format de la commande est :
  </p>
  <ul>
  <b>[%draw marker,square,color]</b>
  </ul>
  <p>
  où <b>square</b> est un nom de case comme d4 et <b>color</b> est n'importe quel
  nom de couleur reconnue (tel que red, blue4, darkGreen, lightSteelBlue, etc.)
  ou un code RGB (un <b>#</b> suivi de six chiffres hexadécimaux, comme #a0b0c8).
  Si la couleur est omise, la valeur par défaut est <red>rouge</red>.
  <b>marker</b> peut être full, circle, disk, x, +, -, ?, !, =, les lettres A à F,
  ou les chiffres 1 à 9.
  </p>
  <p>
  Un commentaire peut contenir un nombre quelconque de commandes de couleur,
  mais chacune doit avoir sa propre balise
  <b>[%draw...]</b>. Par exemple, le texte de commentaire</p>
  <p>
  <green>Maintenant d6 [%draw full,d6,red] est faible et le cavalier peut l'attaquer
  de b5. [%draw circle, b5,#000070]</green>
  </p>
  <p>
  colorera la case d6 en <red>rouge</red> et b5 avec la couleur bleu foncé
  <darkblue>#000070</darkblue>.
  </p>
  
  <h3><name Arrows>Dessin de flèches</name></h3>
  <p>
  Vous pouvez <term>dessiner une flèche</term> d'une case à l'autre avec la souris par
  l'action <b>Ctrl-clic/glisser</b> sur l'échiquier, ou en utilisant une commande de
  commentaire similaire à celle décrite ci-dessus. Le format est :
  </p>
  <ul>
  <b>[%arrow fromSquare,toSquare,color]</b>
  </ul>
  <p>
  où <b>fromSquare</b> et <b>toSquare</b> sont des noms de cases comme d4
  et <b>color</b> est un nom de couleur reconnue (tel que red, blue4, etc)
  ou un code RGB (comme #a0b0c0). Si la couleur est omise, elle est
  par défaut <red>rouge</red>.
  </p>
  <p>
  Par exemple, le texte de commentaire
  </p>
  <p>
    <green>Le cavalier c3 et le fou c4 contrôlent la case faible d5.
   [%arrow c3 d5 red] [%arrow c4 d5 blue]</green>
  </p>
  <p>
  dessinera une flèche rouge de c3 à d5 et une flèche bleue de c4 à d5.
  </p>

  <p>

  <b>Note</b>
  Les cases et les flèches de couleur peuvent être saisies directement sur l'échiquier
  sans passer par l'éditeur de commentaires en utilisant les boutons de la souris.
  Pour les <term>cases colorées</term>, on peut cliquer sur la case en question en
  appyuant sur la touche <b>Ctrl</b>.
  Pour <term>dessiner une flèche</term>, on peut effectuer un <b>Ctrl-clic/glisser</b>
  de le case source à la case cible avec le bouton gauche de la souris.
  Le marqueur de case et la couleur utilisée sont ceux qui ont été sélectionnés
  précédemment dans l'éditeur de commentaires, par défaut un carré vert.
  </p>
  
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

####################
### Crosstable window help:

set helpTitle(F,Crosstable) "Fenêtre Classement de tournoi"
set helpText(F,Crosstable) {
<h1>Fenêtre Classement de tournoi</h1>
  <p>
  La fenêtre de classement montre le tableau croisé du tournoi pour le
  le jeu actuel. Chaque fois que vous rafraîchissez la fenêtre de classement (par
  en appuyant sur son bouton Refresh, en appuyant sur la touche <b>Return</b> dans le
  ou en tapant <b>Control+Shift+X</b> dans la fenêtre
  <a MainWindow>main</a> ou <a GameList>liste des jeux</a> fenêtres), Scid
  recherche toutes les parties du même tournoi que la partie en cours.
  </p>
  <p>
  Toute partie jouée jusqu'à <b>douze mois avant ou après</b> la partie en cours,
  avec les mêmes balises <b>exactes d'événement et de site</b>, est considéré comme étant dans
  le tournoi.
  </p>
  <p>
  Un simple clic sur le bouton gauche de la souris sur n'importe quel résultat dans le tableau croisé
  charge le jeu correspondant.
  Vous pouvez ajouter toutes les parties du tournoi au
  <a Searches Filter>filtre</a>
  avec le bouton <b>Ajouter au filtre</b> dans la fenêtre de classement du tournoi.
  </p>
  
  <h4>Menus des fenêtres croisées</h4>
  <p>
  Le menu <menu>Fichier</menu> vous permet d'imprimer le tableau courant dans un fichier
  en texte brut, en LaTeX ou en format tableau HTML.
  </p>
  <p>
  Le menu <menu>Affichage</menu> permet de choisir le format du tableau :
  <b>Tout-jouer-tout</b>, <b>Suisse</b> ou <b>Knockout</b> ou <b>Auto</b>.
  </p>
  <p>
  Le format tout-jeu (pour les événements de type round-robin) a une limite de 30
  joueurs, mais le format suisse (pour les tournois avec de nombreux joueurs) peut
  afficher jusqu'à 200 joueurs et jusqu'à 20 tours. <b>Auto</b>, qui choisit
  le meilleur format automatiquement pour chaque tournoi, est la valeur par défaut.
  </p>
  <p>
  Notez que Scid utilise le tag <b>Round</b> de chaque jeu pour produire un
  tableau croisé, donc vous ne verrez pas de parties dans la table suisse pour un tournoi
  si ses jeux n'ont pas de valeur numérique : 1, 2, 3, etc.
  </p>
  <p>
  Le menu Affichage vous permet également de personnaliser les données présentées à
  inclure ou exclure les classements, les pays et les titres des joueurs. Vous pouvez également
  choisir si les répartitions de couleurs dans les tables suisses sont affichées.
  </p>
  <p>
  L'option <b>Séparer les groupes de scores</b> n'affecte que la disposition du tableau
  lorsque les joueurs sont triés par score : il provoque l'insertion d'une ligne blanche
  entre chaque groupe de joueurs ayant le même score.
  </p>
  <p>
  Le menu <menu>Trier</menu> permet de trier les joueurs par nom, classement
  ou score ; par score est la valeur par défaut.
  </p>
  <p>
  Le menu <menu>Couleur</menu> vous permet d'activer ou de désactiver l'affichage de la couleur (hypertexte).
  Comme il peut falloir beaucoup de temps pour formater et afficher de grandes tables croisées en
  en sélectionnant <b>Texte simple</b> pour les grands événements, on enregistre un
  beaucoup de temps.
  Cependant, en mode texte, vous ne pouvez pas cliquer sur les joueurs ou les jeux.
  </p>
  
  <h4>Jeux en doublon dans les tableaux croisés</h4>
  <p>
  Pour obtenir de bons résultats avec le tableau croisé, vous devez marquer les parties en double
  pour la suppression et vos jeux doivent avoir une orthographe cohérente du joueur,
  les noms des sites et des événements.
  Voir la page <a Maintenance>maintenance de la base de données</a> pour obtenir de l'aide sur
  la suppression des parties en double et l'édition (ou la vérification de l'orthographe)
  les noms des joueurs/événements/sites.
  </p>
  
  <p><footer>(Mise à jour : Scid 3.6.15, mai 2007)</footer></p>
}

####################
### Database switcher help:

set helpTitle(F,Switcher) "Fenêtre Sélecteur de base de données"
set helpText(F,Switcher) {
<h1>Fenêtre Sélecteur de base de données</h1>
  <p>Vous pouvez ouvrir le <term>Sélecteur de base de données</term> à partir du
  menu <menu>Fenêtres</menu>, ou par le raccourci <b>Ctrl+D</b>. Cette fenêtre
  affiche le nom de chaque base de données, l'état de son
  <a Searches Filter>filtre</a> avec le nombre de jeux sélectionnés,
  et le type de base symbolisé par son icône. Le Sélecteur de base de données
  facilite la bascule entre les bases de données ou la copie des jeux d'une base
  à l'autre. La <term>base de données active</term> est mise en surbrillance avec
  un fond jaune.</p>
  <p><b>Changer la base de données active.</b> Un clic de la souris active une base
  de données et charge automatiquement la <term>partie active</term> de cette base
  dans l'échiquier de la fenêtre principale.</p>
  <p><b>Copier des parties d'une base de données à l'autre.</b> Pour copier tous
  les jeux filtrés d'une base de données vers une autre, faites
  glisser avec le bouton gauche de la souris la base source vers la base cible.
  Une boîte de dialogue de confirmation s'ouvre si la base de données cible n'est
  pas la <a Clipbase>Clipbase</a> et si les jeux peuvent être copiés, ou
  un message d'erreur si les jeux ne peuvent pas être copiés, par exemple si la
  base de données cible est en lecture seule.</p>

  <h3>Menu contextuel</h3>
  <p>Le bouton droit de la souris active un menu contextuel :</p>
  <ul>
  <li><menu>Nouvelle fenêtre de liste de parties</menu> ouvre une fenêtre de liste de parties.</li>
  <li><menu>Ouvrir...</menu> ouvre une nouvelle base de données.</li>
  <li><menu>Importer des fichiers PGN...</menu> dans la base sélectionnée.</li>
  <li><menu>Fermer</menu> la base sélectionnée.</li>
  <li><menu>Compacter la base</menu> ouvre la
  <a Compact>boîte de dialogue de compactage</a> pour nettoyer les parties
  marquées pour suppression.</li>
  <li><menu>Charger au démarrage</menu> marque la base pour être chargée
  automatiquement au démarrage de Scid.</li>
  <li><menu>Changer l'icône</menu> permet de changer le
  <term>type de base de données</term>.</li>

  <li><menu>Montrer les icônes</menu> affiche ou cache l'icône des bases de
  données.</li>
  </ul>

  
  <p><footer>(Mise à jour: Scid 4.7.1, janvier 2020)</footer></p>
}

####################
### File Finder window help:

set helpTitle(F,Finder) "Fenêtre Explorateur de fichiers"
set helpText(F,Finder) {
<h1>Fenêtre Explorateur de fichiers</h1>
  <p>
  Le <term>File Finder</term> vous aide à trouver des fichiers de tout type que vous pouvez
  utilisation dans Scid : bases de données, <a PGN>PGN</a> fichiers, <a EPD>EPD</a> fichiers,
  et <a Répertoire>repertoire</a> fichiers.
  </p>
  <p>
  Le Finder affiche des informations utiles sur chaque fichier, telles que sa taille
  (voir ci-dessous) et la date de la dernière modification. Vous pouvez ouvrir n'importe quel
  en le sélectionnant avec un simple clic gauche de la souris.
  </p>
  
  <h3>Regarder dans les sous-répertoires</h3>
  <p>
  Lorsque vous voulez trouver tous les fichiers dans tous les sous-répertoires de la
  activer la case à cocher <b>Regarder dans les sous-répertoires</b>. Ce
  fera en sorte que Scid examine récursivement chaque sous-répertoire pour les fichiers qui
  peut être ouvert dans Scid. Cela peut prendre beaucoup de temps s'il y a beaucoup
  donc vous ne voudrez peut-être pas le faire pour un répertoire proche du répertoire
  racine du système de fichiers. Vous pouvez interrompre la recherche de fichiers en appuyant sur
  le bouton <b>Stop</b>.
  </p>
  
  <h3>Fichier tailles</h3>
  <p>
  La signification de la taille d'un fichier affichée par le Finder dépend du fichier
  type. Pour les bases de données Scid, c'est le nombre de parties. Pour
  tous les autres fichiers, c'est la taille en kilo octets.
  </p>

  <p>
  Pour faciliter la manipulation des bases de données Scid, le Finder offre un contexte
  menu. A partir de ce menu, accessible par un clic droit de la souris comme d'habitude,
  les fonctions suivantes peuvent être exécutées
  <ul>
      <li><term>Open</term> ouvrira le fichier en question, c'est
      la même chose que de cliquer sur le fichier avec la souris gauche
      bouton.</li>
      <li><term>Sauvegarde</term> copie le fichier sous son nom d'origine
      ajouté par la date et l'heure actuelles.</li>
      <li><term>Copy</term> copie la base de données sélectionnée dans un nouveau
      location.</li>
      <li><term>Move</term> déplace la base de données sélectionnée vers une nouvelle
      location.</li>
      <li><term>Supprimer</term> supprime la base de données sélectionnée.</li>
   </ul>
   Ces fonctions sont particulièrement utiles pour les bases de données Scid car celles-ci
   se composent de plusieurs fichiers.
  </p>
}

####################
### Tournament Finder window help:

set helpTitle(F,Tmt) "Fenêtre Recherche de tournoi"
set helpText(F,Tmt) {
<h1>Fenêtre Recherche de tournoi</h1>
  <p>
  Leaase de données actuelle. Il scanne tous les jeux de bases de données et rassemble les données
  sur les tournois trouvés. Notez que deux jeux sont considérés comme
  être dans le même tournoi s'ils ont le même tag d'événement, le même site
  tag et ont été joués dans les trois mois les uns des autres.
  </p>
  <p>
  Vous pouvez limiter la liste des tournois par le nombre de joueurs et
  jeux, date, classement Elo moyen et pays, en éditant les champs ci-dessous
  la liste des tournois puis en appuyant sur le bouton <b> Mettre à jour </b>.
  </p>
  <p>
  La liste affichée peut être triée par date, nombre de joueurs, nombre
  de jeux, signifie la cote Elo, le site, l'événement ou le nom de famille du gagnant.
  Sélectionnez la catégorie dans le menu <menu> Trier </menu> ou cliquez sur un
  titre de la colonne pour modifier le champ de tri.
  </p>
  <p>
  Pour charger la première partie d'un tournoi affiché, cliquez simplement sur le
  bouton gauche de la souris lorsque sa ligne est en surbrillance. Ce sera également
  mettez à jour la fenêtre <a Crosstable>Classement de tournoi</a> si elle est ouverte.
  Si vous appuyez plutôt sur le bouton droit de la souris, le jeu sera chargé
  et la fenêtre Crosstable sera ouverte même si elle est fermée.
  </p>
  <p>
  Pour accélérer le processus de recherche de tournois, c'est une bonne idée de
  définir une plage de dates assez petite (comme quelques années tout au plus) ou sélectionner
  un pays particulier (par son code standard à trois lettres). Faire
  cela réduira considérablement le nombre de jeux que Scid doit prendre en compte
  lorsque vous essayez de former des tournois à partir des jeux dans la base de données.
  </p>
  
  <p> <footer> (Mise à jour: Scid 3.6.2, décembre 2006) </footer> </p>
}

####################
### GameList window help:

set helpTitle(F,GameList) "Fenêtre Liste des jeux"
set helpText(F,GameList) {
<h1>Fenêtre Liste des jeux</h1>
  <p>
  La fenêtre Liste des jeux affiche un résumé d'une ligne pour chaque jeu inclus
  dans le <term>filtre</term> actuel.
  </p>
  
  <h3>Navigation de la liste de jeu</h3>
  <p>
  Vous pouvez faire défiler la liste des jeux en utilisant la barre de défilement ou les quatre
  sous la liste.
  Vous pouvez également utiliser les boutons [Accueil], [Fin],
  [Page Up], [Page Down] et les touches fléchées pour faire défiler
  en utilisant le clavier.
  </p>
  <p>
  Vous pouvez trouver le jeu suivant dans la liste qui a un certain texte dans son
  Blanc, Noir, Evénement ou Site, en utilisant la case <b>Trouver le texte</b>.
  </p>
  
  <h3>Actions sur les jeux de la liste</h3>
  <p>
  Pour charger une partie de la liste des parties, double-cliquez sur le bouton gauche de la souris
  sur le jeu.
  En cliquant sur le bouton du milieu de la souris, vous pouvez voir les premiers coups d'une partie ;
  cela peut être utile pour vérifier l'ouverture d'une partie avant de la charger.
  </p>
  <p>
  Le bouton droit de la souris produit un menu pour le jeu sélectionné, dans lequel
  vous pouvez parcourir ou fusionner le jeu (voir ci-dessous), supprimer (ou annuler) le
  jeu, ou l'exclure du filtre.
  Notez que la suppression d'une partie n'active que son drapeau de suppression ; elle restera dans
  la base de données jusqu'à ce que vous <a Compact>compact</a> le.
  </p>
  
  <h3>Configurer la liste des jeux</h3>
  <p>
  Cliquez avec le bouton gauche ou droit de la souris sur le titre d'une colonne pour configurer le
  la liste de jeu. Vous pouvez modifier les largeurs, ajouter ou supprimer des colonnes, et modifier
  la couleur pour chaque colonne.
  </p>
  <p>
  Si vous voulez seulement changer la largeur d'une colonne, il y a un raccourci :
  en appuyant sur la touche <b>Control</b> (ou <b>Shift</b>), en appuyant sur
  le bouton gauche de la souris sur le titre de la colonne la rendra plus étroite
  et en appuyant sur le bouton droit de la souris, il sera plus large.
  </p>
  
  <h3>Alterner la taille de la liste de jeu</h3>
  <p>
  La taille de la fenêtre de la liste des jeux est enregistrée dans le fichier d'options
  chaque fois que vous enregistrez des options.
  Donc si vous voulez que la liste des jeux affiche 10 jeux par défaut, il suffit de redimensionner
  la fenêtre de la liste des jeux et ensuite sélectionnez <menu>Save options</menu>
  dans le menu <menu>Options</menu>.
  </p>
  
  <h3><name Browsing>Navigation et fusion de jeux</name></h3>
  <p>
  Le menu de la souris droite de la liste des jeux (et quelques autres fenêtres, comme la
  <a Reports Opening>rapport d'ouverture</a> et la fenêtre
  <a Tree Best>liste des meilleurs jeux</a> de la fenêtre <a Tree>arbre</a>)
  offrent le choix de charger, de parcourir ou de fusionner un jeu.
  </p>
  <p>
  Lorsque <term>Browse game</term> est sélectionné, les coups de la
  (sans commentaires ni variantes) sera affiché dans un
  fenêtre. C'est un moyen utile de prévisualiser un autre jeu sans
  affectant le jeu actuellement chargé. La taille de cette fenêtre peut être
  change par les touches +/- ou la molette Ctrl.
  </p>
  <p>
  La fonction <term>Fusionner le jeu</term> permet d'inclure le
  jeu sélectionné comme une variante du jeu en cours. Scid trouve le
  point le plus profond où le jeu sélectionné diffère du jeu actuel
  (en tenant compte des transpositions) et ajoute une variante
  pour le jeu sélectionné à cette position. Le jeu sélectionné peut également être fusionné
  avec le jeu en cours d'une autre base ouverte (cette base ne doit pas être mise en lecture seule).
  Vous pouvez modifier le nombre
  de coups de la partie sélectionnée à afficher, selon que vous
  sont intéressés par l'ajout de la totalité du jeu ou seulement de sa phase d'ouverture.
  </p>
  
  <p><footer>(Mise à jour : Scid 3.7.2, avril 2009)</footer></p>
}

####################
### Import window help:

set helpTitle(F,Import) "Fenêtre Importation de jeux"
set helpText(F,Import) {
<h1>Fenêtre Importation de jeux</h1>
  <p>
  La fenêtre d'importation de Scid vous permet de coller facilement un jeu
  au format <a PGN>PGN</a> dans Scid à partir d'une autre application ou fenêtre.
  Cette fenêtre est accessible par le menu
  <menu>Édition</menu><menu>Coller le presse-papiers comme partie PGN</menu>
  </p>
  <p>
  Le grand cadre blanc dans cette fenêtre est l'endroit où vous tapez ou collez
  le texte du jeu au format PGN, et le cadre gris en dessous
  fournit un retour d'information sur toute erreur ou avertissement.
  </p>
  
  <h3>Editer la partie courante avec la fenêtre d'importation</h3>
  <p>
  La fenêtre d'importation est également un moyen pratique pour faire quelques
  modifications à la partie en cours : vous pouvez coller le jeu en cours dans
  la fenêtre d'importation avec le bouton <b>[Coller la partie en cours]</b>
  ou le raccourci <b>Alt-P</b>, et l'éditer. Cliquez sur le bouton <b>[Importer]</b>
  (raccourci <b>Alt-I</b>) lorsque vous avez terminé.
  </p>
  
  <h3>Balises PGN dans la fenêtre d'importation</h3>
  <p>
  Scid s'attend à voir des balises d'en-tête <a PGN>PGN</a> telles que
  <ul>
  [Result "*"]
  </ul>
  avant tout coup, mais vous pouvez juste coller un fragment de jeu comme
  <ul>
  <b>1.e4 e5 2.Bc4 Bc5 3.Qh5 ?! Nf6 ?? 4.Qxf7# 1-0</b>
  </ul>
  sans aucune balise d'en-tête et Scid l'importera.
  </p>
  
  <h3>Utilisation de fichiers PGN dans Scid</h3>
  <p>
  Si vous voulez utiliser un fichier au <a PGN>format PGN</a> dans Scid mais ne
  voulez pas le convertir avec l'utilitaire <a Pgnscid>pgnscid</a> d'abord,
  il y a deux moyens possibles.
  </p>
  <p>
  Tout d'abord, vous pouvez importer les parties de jeu du fichier dans une base
  de données existante avec le menu
  <menu>Base</menu><menu>Importer des fichiers PGN...</menu>.
  </p>
  <p>
  L'alternative est d'ouvrir le fichier PGN directement dans Scid avec le menu
  <menu>Fichier</menu><menu>Ouvrir...</menu>. Cependant, les fichiers PGN sont ouverts
  en <b>lecture seule</b> et consomment plus de mémoire qu'un fichier de format base
  de données Scid comparable. Ce n'est donc recommandé que pour les petits fichiers PGN.
  </p>
  
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

####################
### Exporting help:

set helpTitle(F,Export) "Exportation de jeux"
set helpText(F,Export) {
<h1>Exportation de jeux</h1>
  <p>
  Vous pouvez utiliser les commandes du menu <menu>Outils</menu> pour exporter le
  jeu en cours ou tous les jeux du filtre en cours dans un fichier texte.
  </p>
  <p>
  Quatre formats de fichiers texte sont disponibles : <a PGN>PGN</a> (jeu portable
  ), HTML (pour les pages Web), HTML et JavaScript (pour les
  pages Web interactives) et LaTeX (un système de composition populaire).
  </p>
  <p>
  Lors de l'exportation, vous pouvez choisir de créer un nouveau fichier, ou d'ajouter le
  à un fichier existant de jeux exportés par Scid.
  </p>
  
  <h3>Diagrammes</h3>
  <p>
  Lors de l'exportation au format HTML ou LaTeX, Scid ajoutera automatiquement
  un diagramme à chaque fois qu'un diagramme <a NAGs>nag</a> ("D") ou un
  <a Comment>commentaire</a> qui commence par le caractère "#" apparaît dans
  le jeu.
  </p>
  
  <h3><name Null>Null se déplace dans <a PGN>PGN</a> Export</name></h3>
  <p>
  Scid permet <a Annotating Null>null (vide) coups</a> d'être stockés
  dans les jeux, car ils peuvent être utiles pour annoter les jeux en utilisant
  variantes.  Cependant, la norme <a PGN>PGN</a> n'a pas de concept de coup nul. Donc
  si vous exportez des parties de Scid avec des coups nuls vers un fichier <a PGN>PGN</a>, autre
  Le logiciel de lecture de PGN ne pourra pas lire les coups nuls.
  </p>
  <p>
  Pour résoudre ce problème, Scid propose une option supplémentaire,
  <b>Convertir les coups nuls en commentaires</b>, lors de l'exportation de parties au format <a PGN>PGN</a>.
  Si vous voulez créer un fichier <a PGN>PGN</a> que d'autres logiciels peuvent utiliser, tournez ceci
  on et les variantes contenant des coups nuls seront converties en
  commentaires. Cependant, si vous voulez créer un fichier <a PGN>PGN</a> qui peut être importé
  dans Scid plus tard avec les coups nuls préservés, laissez l'option désactivée.
  </p>
  
  <h3>Export HTML</h3>
  <p>
  Scid peut exporter des jeux vers un fichier HTML. Pour que les diagrammes apparaissent, vous
  aura besoin des images du diagramme (distribuées avec Scid dans le répertoire
  "<b>bitmaps/</b>") pour être dans un sous-répertoire <b>bitmaps/</b> sous le
  dans lequel se trouve le fichier HTML.
  </p>

  <h3>HTML avec JavaScript Export</h3>
  <p>
  Alors que l'exportation HTML génère un fichier statique qui peut contenir
  des diagrammes de cartes statiques, ce format offre du HTML dynamique, qui permet
  pour se déplacer dans le jeu de façon interactive avec la souris.
  </p>
  <p>
  Ce format est constitué de plusieurs fichiers qui doivent être stockés dans un
  structure spécifique. Par conséquent, il est conseillé de générer d'abord une
  dossier vide qui contiendra ces fichiers. Le nom du dossier principal
  peut être spécifié et il aura l'extension html (par exemple
  monjeu.html). Ce fichier doit être chargé par le navigateur web. Le fichier
  d'autres fichiers doivent exister à l'endroit exact où l'exportation
  le filtre les place. Cependant, le dossier entier peut facilement être téléchargé
  à un serveur web.
  </p>
  
  <h3>LaTeX Export</h3>
  <p>
  Scid peut exporter des jeux vers un fichier LaTeX.  Les jeux peuvent être imprimés sur deux colonnes
  à une page et les déplacements sont en notation algébrique figurine avec les
  la traduction des symboles NAG. Des diagrammes sont ajoutés chaque fois qu'un
  <term>D</term> commentaire est trouvé.
  </p>
  <p>
  Voir la page d'aide <a LaTeX>Using LaTeX with Scid</a> pour plus d'informations.
  </p>
  
  <p><footer>(Mise à jour : Scid 3.6.26, octobre 2008)</footer></p>
}

####################
### LaTeX help:

set helpTitle(F,LaTeX) "Utilisation de LaTeX avec Scid"
set helpText(F,LaTeX) {
<h1>Utilisation de LaTeX avec Scid</h1>
  <p>
  Scid peut enregistrer les parties et les rapports d'ouverture dans des fichiers au format LaTeX.
  LaTeX est une extension de TeX, un système de composition populaire.
  </p>
  <p>
  Pour composer les fichiers LaTeX produits par Scid, vous devez avoir
  LaTeX (bien sûr) et avoir le paquet de polices d'échecs "chess12" installé.
  Ce paquet de polices ne fait généralement pas partie des installations standard de LaTeX,
  donc même si vous avez LaTeX, vous n'avez peut-être pas la police de caractères pour les échecs.
  </p>
  <p>
  Pour des informations sur le téléchargement et l'installation de la police d'échecs LaTeX,
  visitez le lien
  <url http://scid.sourceforge.net/latex.html>Utilisation de LaTeX avec Scid</url>
  à la page <url http://scid.sourceforge.net/>site Web de Scid</url>.
  </p>
  
  <p><footer>(Mise à jour : Scid 3.6.2, décembre 2006)</footer></p>
}

####################
### PGN window help:

set helpTitle(F,PGN) "Fenêtre Notation PGN"
set helpText(F,PGN) {
<h1>Fenêtre Notation PGN</h1>
  <p>
  La fenêtre PGN de Scid affiche le contenu du jeu en cours dans la
  <term>représentation PGN</term> standard. Dans le texte de la liste des
  coups, les commentaires apparaissent {entre accolades} et les variantes
  apparaissent (entre parenthèses).
  </p>
  
  <h3>Format PGN</h3>
  <p>
  Le format <term>PGN</term> (Portable Game Notation) est une norme courante pour
  le transfert de parties d'échecs entre programmes informatiques. Une notation
  PGN du jeu se compose de deux sections.
  <ul>
    <li>La première section est l'<term>en-tête</term>, qui contient des balises telles que
    <b>[White "Kasparov, Gary"]</b> et <b>[Result "1/2-1/2"] </b>.
    </li>
    <li>La deuxième section contient les coups réels du jeu, en
    <term>notation algébrique standard</term> (SAN) avec toutes les variantes, les
    <a NAGs>symboles d'annotation</a> et les <a Comment>commentaires</a>.
    </li>
  </ul>
  </p>
  
  <h3>Actions dans la fenêtre PGN </h3>
  <p>
  Vous pouvez utiliser la fenêtre PGN pour naviguer dans le jeu :
  <ul>
    <li><b>Cliquer sur un coup avec le bouton gauche</b> de la souris passera
      l'échiquier à ce coup, avec la position des pièces correspondante.</li>
    <li><b>Cliquer sur un coup avec le bouton central</b> de la souris affichera
      un petit échiquier avec la position des pièces après ce coup.</li>
    <li><b>Cliquer avec le bouton de la souris sur un commentaire</b> permet de
      le modifier.</li>
    <li>Les <b>touches fléchées</b> et les touches <b>v</b> et <b>z</b> (pour entrer
      ou quitter les variantes) fonctionnent pour la navigation dans le jeu
      comme dans la fenêtre principale.</li>
    <li>Comme dans la fenêtre principale, si vous appuyez sur <b>v</b>, la fenêtre
      pop-up <term>Variantes</term> s'ouvre pour permettre de sélectionner une variante
      à l'aide des touches de curseur Haut/Bas, et la touche Entrée ou un clic avec
      la souris. Le réglage dans le menu
      <menu>Options</menu><menu>Coups</menu><menu>Montrer fenêtre des variantes</menu>
      fera apparaître cette fenêtre automatiquement chaque fois qu'un coup avec une
      ou des variantes est trouvé pendant la navigation dans le jeu à l'aide des
      touches de curseur. Cela permet de naviguer dans le jeu avec les touches de
      curseur uniquement.</li>
    <li><b>Cliquer sur le bouton droit</b> de la souris fera apparaître un
      menu contextuel.</li>
  </ul>
  </p>
  
  <h3>Menu contextuel</h3>
  <p>
  Dans le <term>menu contextuel</term>, les fonctions suivantes sont disponibles
  <ul>
     <li><menu>Effacer cette variante</menu> supprime la variante actuelle.</li>
     <li><menu>Déplacer en tête</menu>  déplace la variante actuelle à la première
       position de toutes variantes à ce niveau.</li>
     <li><menu>Variante vers ligne principale</menu> promeut la variante actuelle
       vers la ligne principale et rétrograde le ligne principale actuelle à
       une variante.</li>
     <li><menu>Épurer: coups depuis le début</menu> supprime tous les coups
       du début jusqu'au coup sélectionné.</li>
     <li><menu>Épurer: coups jusqu'à la fin</menu> supprime tous les coups
       après le coup sélectionné jusqu'à la fin.</li>
     <li><menu>Épurer: commentaires</menu> supprime tous les commentaires.</li>
     <li><menu>Épurer: Variantes</menu> supprime toutes les variantes.</li>
     <li><menu>! ? +- +/- ...</menu> insére des symboles d'annotation fréquemment utilisés.</li>
     <li><menu>Éditeur de commentaires</menu> ouvre la fenêtre de
     l'<a Comment>éditeur de commentaires</a>.</li>
  </ul>
  </p>

  <h3>Options d'affichage PGN</h3>
  <p>
  Les <term>menus de la fenêtre PGN</term> contiennent des options qui affectent
  son affichage.
  </p>
  <p>
  Menu <menu>Couleur</menu>. Scid peut afficher le jeu en couleur ou en texte brut.
  L'écran couleur est plus facile à lire et vous permet de sélectionner les coups et
  commentaires à l'aide de la souris, mais il est beaucoup plus lent à mettre à jour.
  Pour des parties très longues, vous pouvez sélectionner l'affichage en texte brut.
  </p>
  <p>
  Menu <menu>Affichage</menu>. Vous pouvez également modifier le format des commentaires
  et des variantes, en choisissant de les afficher en retrait sur une ligne distincte
  pour une meilleure visibilité.
  </p>
  <p>
  Les options d'affichage et la taille de la fenêtre PGN sont enregistrées dans le
  fichier d'options chaque fois que vous <menu>enregistrez les options</menu> à partir
  du menu <menu>Options</menu> de la fenêtre principale.
  </p>
  
  <p><footer>(Mise à jour: Scid 4.7.1, janvier 2020)</footer></p>
}

####################
### Piece Tracker help:

set helpTitle(F,PTracker) "Fenêtre Suivi de pièce"
set helpText(F,PTracker) {
<h1>Fenêtre Suivi de pièce</h1>
  <p>
  Le <term>Suivi de pièce</term> est un outil qui suit les coups
  d'une pièce particulière dans tous les jeux du filtre actuel, et
  génère une "empreinte" indiquant la fréquence à laquelle chaque case a été
  visitée par la pièce.
  </p>
  <p>
  Pour utiliser Suivi de pièce, assurez-vous d'abord que le filtre contient le
  les jeux qui vous intéressent, tels que les jeux atteignant un
  position d'ouverture ou tous les jeux où un certain joueur avait les pièces blanches.
  Ensuite, sélectionnez la pièce à suivre et définissez d'autres options de suivi; ceux-ci sont
  expliqué ci-dessous. Appuyez ensuite sur le bouton <b> Mettre à jour </b>.
  </p>
  <p>
  Les informations de coup des pièces suivies sont affichées de deux manières: a
  "empreinte" graphique et une liste de textes avec une ligne de données par case.
  </p>
  
  <h3> Sélection du morceau suivi </h3>
  <p>
  Les pièces d'échecs sont affichées comme dans la position de départ d'échecs standard
  sous le tableau de l'empreinte. Une seule pièce (comme le cavalier blanc b1 ou
  le pion d7 noir) peut être sélectionné avec le bouton gauche de la souris, et
  pièces du même type et de la même couleur (comme tous les pions blancs ou les deux noirs
  tour) peuvent être sélectionnés à l'aide du bouton droit de la souris.
  </p>
  
  <h3> Autres paramètres de suivi des pièces </h3>
  <p>
  La plage de numéros de coup contrôle quand le suivi doit commencer et s'arrêter
  chaque match. La plage par défaut de 1 à 20 (ce qui signifie que le suivi doit s'arrêter après
  Le 20e coup de Black) est approprié pour examiner les thèmes d'ouverture, mais (pour
  exemple) une plage comme 15-35 serait meilleure lorsque vous recherchez un jeu intermédiaire
  les tendances.
  </p>
  <p>
  Il existe deux types de statistiques que le tracker peut générer:
  <ul>
  <li> <b>% de jeux avec passage à la case</b>: indique la proportion de filtre
  les jeux contiennent un déplacement de la pièce suivie vers chaque case. C'est
  le paramètre par défaut et généralement le choix le plus approprié. </li>
  <li> <b>% de temps dans chaque case</b>: indique la proportion de temps pendant laquelle la pièce suivie est restée sur chaque case. </li>
  </ul>
  </p>
  
  <h3> Astuces </h3>
  <p>
  Il y a (au moins) trois bonnes utilisations pour le Tracker pièce: ouverture
  préparation, thèmes de milieu de jeu et préparation des joueurs.
  </p>
  <p>
  Pour la préparation de l'ouverture, utilisez le tracker de pièces avec <a Tree> Tree </a>
  ouvert. En suivant les pièces, vous pouvez voir les tendances de l'ouverture actuelle
  tels que les pousses de pions communs, les avant-postes de cavaliers et où les fous sont
  le plus souvent placé. Il peut être utile de définir la plage de numéros de déplacement
  pour commencer après le coup en cours dans le jeu, donc les coups effectués pour atteindre
  la position actuelle n'est pas incluse dans les statistiques.
  </p>
  <p>
  Pour les thèmes intermédiaires, le tracker de morceau peut être utile lorsque le filtre
  a été défini pour contenir une certaine plage ECO (en utilisant un
  <a Searches Header> Recherche d'en-tête </a>) ou peut-être un modèle tel qu'un
  IQP blanc (en utilisant une <a Searches Material> recherche de matière / motif </a>).
  Réglez la plage de déplacement sur quelque chose de convenable (comme 20-40) et suivez
  pièces pour voir le pion pousser à la fin du milieu de partie ou au début de la fin de partie,
  par exemple.
  </p>
  <p>
  Pour la préparation des joueurs, utilisez une <a Searches Header> recherche d'en-tête </a> ou
  la fenêtre <a PInfo> Informations sur le joueur </a> pour trouver tous les jeux par
  certain joueur avec une couleur. Le Suivi de pièce peut ensuite être utilisé pour
  découvrir la probabilité que le joueur soit aux fous de fianchetto, château
  queenside, ou mettre en place un coin de pion d5 ou e5, par exemple.
  </p>
  
  <p> <footer> (Mise à jour: Scid 3.6.2, décembre 2006) </footer> </p>
}

####################
### Tree window help:

set helpTitle(F,Tree) "Fenêtre Arbre"
set helpText(F,Tree) {
<h1>Fenêtre Arbre</h1>
  <p>
  La fenêtre <term>Arbre</term> affiche des informations sur tous les
  coups effectués à partir de la position actuelle dans les jeux dans la base de données.
  En mode arborescence, la fenêtre de l'arborescence est mise à jour automatiquement à chaque
  modifications du tableau des fenêtres principales. Cela peut être lent pour les grandes bases de données et
  peut donc être désactivé à l'aide du <button tb_search_on>. Pour visualiser
  qu'un arbre n'est pas automatiquement actualisé, ce bouton se transforme en
  <button tb_search_off>.
  </p>
  <p> Pour ouvrir la fenêtre <term> Arborescence </term>, on peut d'abord ouvrir un
  base de données, puis choisissez Fenêtre / Fenêtre arborescente (Ctrl-T) dans le
  ou ouvrez la base sous forme d'arbre à partir du menu Fichier.
  </p>
  <p>
  Notez que chaque fois que la fenêtre de l'arborescence est mise à jour, le
  Le <a Searches Filter> filtre </a> est réinitialisé et seuls les jeux contenant le
  la position actuelle sera incluse. C'est également le cas si par ex. une
  la recherche d'en-tête a été effectuée: une fois le prochain coup entré, l'arborescence
  mises à jour de la fenêtre et le filtre est défini sur les jeux correspondant à l'arbre
  fenêtre.
  </p>
  <p>
  Cliquer sur le bouton gauche de la souris lors d'un déplacement dans la fenêtre de l'arborescence ajoute
  qui passent au jeu.
  </p>
  <h3> Contenu de la fenêtre d'arborescence </h3>
  <p>
  La fenêtre arborescente indique le <a ECO> code ECO </a> (le cas échéant), la fréquence
  (à la fois en nombre de parties et en pourcentage) et le score de chaque coup.
  Le <term> score </term> est toujours calculé à partir du <b> Blanc </b>
  perspective, donc 100% signifie que tous les blancs gagnent et 0% signifie que tous les noirs
  gagne. Les scores sont mis en évidence pour les coups qui ont une bonne (verte) ou mauvaise
  (rouge) résultats. En moyenne, un coup devrait marquer 53,8% pour les blancs,
  la surbrillance apparaît si un coup obtient un résultat supérieur ou égal à 3%
  que cette moyenne et si au moins 15 jeux sont contenus dans le
  base de données. Le <term> AvElo </term> (Elo moyen) correspond au
  le joueur est en coup et <term> Perf </term> (performance) représente le
  la force de l'adversaire. De plus, <term> AvYear </term> montre la
  année moyenne de parties jouées dans ce coup et <term>% nuls </term>
  donne le pourcentage de tirages pour la ligne affichée. Tous ceux-ci
  les valeurs sont calculées pour la base de données affichée dans l'arborescence, et
  dépendent donc bien sûr des jeux de cette base de données.
  </p>
  <p>
  Des informations supplémentaires s'affichent si un <term> masque </term> a été
  ouvert. Pour l'utilisation des <term> masques </term>, voir ci-dessous.
  </p>
  <p>
  Les déplacements dans l'arborescence peuvent être triés par déplacement (par ordre alphabétique),
  Code ECO, fréquence ou score. Vous pouvez modifier la méthode de tri à l'aide de
  le menu <menu> Trier </menu>.
  </p>

  <h3> <name Best> Fenêtre des meilleurs jeux </name> </h3>
  <p>
  La fenêtre de l'arborescence a une commande de menu Fichier et un bouton pour ouvrir
  Fenêtre <term> Meilleurs jeux </term>, qui affiche une liste des
  les jeux les mieux notés dans la branche d'arbre actuellement affichée.
  (De manière équivalente, on peut utiliser le bouton <button tb_list>). Les jeux
  sont classés par ordre moyen, et vous pouvez restreindre la liste
  pour montrer des jeux avec un résultat particulier et aussi limiter le nombre de
  jeux présentés dans cette liste. De plus, jusqu'à 4 drapeaux des jeux
  dans la liste apparaissent dans la liste des meilleurs jeux. Le drapeau spécial
  <b> A </b> marque un jeu comme <b> Annoté </b>, c'est-à-dire. ces jeux contiennent
  soit des commentaires, des variantes ou des NAG (ou toute combinaison de ceux-ci).
  Notez que si une recherche d'en-tête a été exécutée sur la base de données, le meilleur
  La liste des jeux est limitée aux résultats de cette recherche d'en-tête.
  </p>
  
  <h3> <name Graph> Fenêtre de graphique d'arbre </name> </h3>
  <p>
  Les boutons de la fenêtre d'arborescence incluent un bouton marqué <term> Graphique </term>
  (<button tb_bargraph>) qui produit un affichage graphique des
  performance relative de chaque coup par rapport à la position actuelle. Tout
  coups qui ont été joués au moins 1% du temps et au moins 5
  fois, sont affichés. Les scores en pourcentage sont toujours ceux de White
  perspective même quand il est noir de bouger. Les graphiques peuvent être enregistrés
  au format PostScript via le menu fichier.
  </p>
  <p>
  Dans l'arborescence, une ligne rouge est tracée montrant la moyenne de tous les jeux
  de la position actuelle, et la zone entre 50 et 55% (où la plupart des
  les ouvertures standard devraient marquer) est de couleur bleue
  pour aider à la comparaison des coups. Notez que le blanc marque généralement environ 55%
  aux échecs de niveau maître.
  </p>
  
  <h3> <name Lock> Verrouillage de la fenêtre de l'arborescence </name> </h3>
  <p>
  Chaque fenêtre d'arbre est associée à une base spécifique, c'est-à-dire si
  plusieurs bases sont ouvertes simultanément, plusieurs fenêtres arborescentes peuvent
  exister. Si le bouton <term> Verrouiller </term> dans la fenêtre de l'arborescence est
  activé, la fermeture de la fenêtre de l'arborescence fermera également la base de données
  associé à cet arbre spécifique. De plus, cela ferme également
  graphique associé ou meilleures fenêtres de jeux. Si le <term> verrouillage </term>
  le bouton n'est pas coché en fermant l'arbre laissera toutes ces fenêtres
  ouvert et fermez simplement l'arborescence de la base.
  </p>
  <p>
  Notez que l'ouverture d'une base sous forme d'arbre à partir du menu fichier
  verrouille automatiquement la base de données par défaut.
  </p>
  
  <h3> <name Training> Formation </name> </h3>
  <p>
  Lorsque la case <term> Formation </term> dans la fenêtre de l'arborescence est cochée,
  Scid effectuera un coup au hasard chaque fois que vous ajouterez un coup au jeu.
  Le coup choisi par Scid dépend des statistiques de la base de données, donc un coup joué
  dans 80% des jeux de bases de données seront choisis par Scid avec une probabilité de 80%.
  Activer cette fonction, puis masquer (ou iconifier) la fenêtre Arbre et
  jouer des ouvertures contre une grande base de données, est un excellent moyen de tester votre
  connaissance de votre répertoire d'ouverture. Une autre option pour former un
  offres d'ouverture <a OpeningTrainer> Formation / Ouvertures </a>.
  </p>

  
  <h3> Mise en cache pour des résultats plus rapides </h3>
  <p>
  Scid maintient un cache des résultats de la recherche d'arborescence pour les positions avec le
  jeux les plus assortis. Si vous avancez et reculez dans un jeu en mode arborescence,
  vous verrez la mise à jour de la fenêtre d'arbre presque instantanément lorsque la position
  recherché se trouve dans le cache.
  </p>
  <p>
  La fenêtre de l'arborescence a une commande de menu de fichier nommée <term> Enregistrer le cache </term>.
  Lorsque vous sélectionnez cette option, le contenu actuel du cache d'arborescence en mémoire
  sont écrits dans un fichier (avec le suffixe <b> .stc </b>) pour accélérer le futur
  utilisation du mode Tree avec cette base de données.
  </p>
  <p>
  La commande <term> Remplir le fichier cache </term> dans le menu fichier de l'arborescence
  fenêtre remplit le fichier cache avec des données pour de nombreuses positions d'ouverture.
  Il effectue une recherche dans l'arborescence pour environ 100 des positions d'ouverture les plus courantes,
  enregistre ensuite le fichier cache.
  </p>
  <p>
  Le nombre maximum de lignes dans le cache peut être configuré par Fichier /
  Taille du cache. Les valeurs par défaut sont jusqu'à 1 000 lignes.
  </p>
  <p>
  Alternativement, on peut aussi remplir le cache avec le contenu d'un
  base ou un jeu en choisissant File / Fill Cache avec base et File /
  Remplissez le cache avec le jeu, respectivement. Le cache sera rempli de
  le contenu de ceux-ci, y compris toutes les variantes. C'est très utile
  si l'on a une ou plusieurs bases de répertoire pouvant servir d'entrée. (Voir
  également <a OpeningTrainer> Formation / Ouvertures </a> sur ce type de bases.)
  </p>
  <p>
  L'actualisation de l'arborescence peut être considérablement améliorée si la base de données est triée
  par code ECO puis compacté (voir la <a Maintenance> maintenance </a>
  fenêtre). Une fois cet objectif atteint (l'ensemble du processus peut durer plusieurs
  heures), activez l'option <term> Mode rapide </term>. Le rafraîchissement de
  la fenêtre Arbre sera en moyenne 20 fois plus rapide au prix de
  quelques inexactitudes (les jeux qui ne sont pas dans le filtre actuel ne seront pas pris
  en compte). En désactivant l'option <term> Mode rapide </term>, vous
  verra la différence dans le nombre de jeux lorsque tous les
  les transpositions sont prises en compte. Si vous souhaitez obtenir un aperçu
  des statistiques puis obtenir un arbre précis, utilisez l'option <term> rapide et
  mode lent </term>
  </p>
  <p>
  Notez qu'un fichier de cache d'arborescence (.stc) est complètement redondant; vous pouvez supprimer
  sans affecter la base de données, et en fait il est supprimé par Scid
  chaque fois qu'une action se produit qui pourrait la laisser obsolète - par exemple,
  ajouter ou remplacer un jeu ou trier la base de données.
  </p>
  
  <p> <footer> (Mise à jour: Scid 4.3, novembre 2010) </footer> </p>
}


set helpTitle(F,TreeMasks) "Masques pour les arbres"
set helpText(F,TreeMasks) {
<h1>Masques pour les arbres</h1>
  <h3>Notions de base</h3>
  <p>
  La fenêtre <term>arbre</term> affiche des informations sur tous les coups
  faite à partir de la position actuelle dans les jeux de la base de données. Voir aussi
  <a Tree>La fenêtre de l'arbre</a> pour plus de détails.
  </p>
  <p>
  Pour ajouter des informations supplémentaires au-delà des données statistiques pures, un
  <term>Masque</term> peut être défini. On peut imaginer un
  <term>Masque</term> comme un calque transparent au-dessus de l'arbre courant, que
  contient des données supplémentaires, par exemple des commentaires sur les coups ou les positions, des propres
  et son propre répertoire d'ouverture. <term>Masques</term> sont
  stockées dans un fichier de masque (.stm) et sont donc indépendantes de la
  des bases de données à utiliser. C'est-à-dire qu'on peut définir une
  <term>Masque</term> une fois et l'utiliser avec diverses bases de données en
  en le chargeant depuis le menu <term>arbre</term>fenêtres.
  </p>
  <p>
  Dès qu'un fichier <term>Masque</term> est ouvert, les affichages de
  le changement de fenêtre <term>arbre</term>. Tout d'abord, tous les déplacements de
  les positions actuelles qui sont présentes dans le masque sont mises en évidence.
  De plus, les GNA et les marqueurs peuvent apparaître devant un coup ou
  les commentaires concernant le déménagement apparaîtront à la fin de son
  ligne. Enfin, la position actuelle peut également contenir un commentaire.
  </p>
  <p>
  <b>Note</b> : L'indépendance de <term>Masques</term> par rapport à une base de données
  en font un outil très puissant pour gérer les répertoires d'ouverture.
  Contrairement à la façon traditionnelle d'ouvrir les lignes, <term>Masques</term>
  ont l'avantage de traiter les transpositions de manière transparente, simple
  parce qu'ils sont basés sur les positions actuelles au lieu d'une ligne
  qui y mène.
  </p>
  <h3>Utilisation d'un masque</h3>
  <p>
  Comme <term>Masques</term> opèrent sur l'arborescence d'une base de données donnée,
  tout d'abord la vue de l'arbre doit être ouverte soit par <menu>Fenêtre /
  Fenêtre de l'arbre</menu> ou le raccourci <b>ctrl-t</b>. Pour démarrer
  il est logique d'ouvrir une plus grande base de données de référence car
  simplifie l'ajout de coups à un <term>Masque</term>. Cependant,
  Les masques fonctionnent avec toutes les bases de données, même la <term>Clipbase</term>, qui
  est qu'on peut aussi importer une collection de jeux d'un fichier <a PGN>PGN</a> vers
  la base de clips pour configurer un <term>Masque</term>.
  </p>
  <p>
  Maintenant, un fichier de masque doit être créé ou chargé. Ces fichiers utilisent le
  extension .stm. Pour créer un nouveau fichier de masque, sélectionnez <menu>Masque /
  Nouveau</menu> du menu <term>arbre</term> fenêtres. De même, un
  Le masque existant peut être ouvert en utilisant <menu>Masque / Ouvrir</menu>. Enfin,
  <menu>Mask / Open recent</menu> est un raccourci vers le
  <term>Masque</term> fichiers. 
  </p>
  <p>
  Une fois qu'un fichier de masque est ouvert, un nouveau commentaire peut être ajouté à celui-ci
  masque spécifique. Notez à nouveau que le masque est indépendant de la base de données
  utilisé pour sa création. Il peut ensuite être appliqué à n'importe quelle base de données de ceux
  l'amour. Pour un répertoire d'ouverture, il pourrait donc être judicieux de
  générer deux masques, un pour les Blancs et un pour les Noirs
  des ouvertures.
  </p>
  <p>
  Pour ajouter des marqueurs ou des commentaires aux coups, il faut d'abord ajouter le coup
  au masque en choisissant <term>Add to mask</term> dans le contexte
  disponible en cliquant sur la ligne avec le bouton droit de la souris.
  Une fois qu'une ligne a été ajoutée, elle est mise en évidence dans <blue>blue</blue>
  dans la fenêtre de l'arbre. De même, un coup peut être retiré de la fenêtre
  masque par <menu>Supprimer du masque</menu> du menu contextuel.
  </p>
  <p>
  <b>Note</b> : Si le coup à ajouter n'est pas affiché dans l'arbre
  La fenêtre Scid offre une liste de tous les coups possibles au moyen de la
  menu contextuel. Comme cela pourrait être un sacré paquet de coups, ils sont
  à la fin de la liste des éléments de menu contextuel disponibles.
  des choix. Tous sont étiquetés par <menu>Ajouter ce déplacement à mask</menu> et dans
  cas nécessaire numéroté. Il suffit de sélectionner un coup dans l'une de ces listes
  si elle n'apparaît pas dans l'arbre de toute façon.
  </p>
  <p>
  Après qu'un coup ait été ajouté au <term>Masque</term> et que l'on puisse sélectionner
  les marqueurs suivants dans le menu contextuel. Ces marqueurs afficheront
  en haut à gauche du coup annoté :
  <ul>
  Les symboles <li><term>NAG</term> sont les symboles d'annotation les plus simples. Les symboles
  le menu n'affiche que les coups sensibles pour un coup (pas une position)
  ce qui donne les symboles suivants possibles : <term> !!, !, ! ?, ? !,
  ?, ? ?, ~</term>
  </li>
  <li>En sélectionnant <menu>Marqueur 1</menu> et <menu>Marqueur 2</menu> up
  à deux marqueurs graphiques supplémentaires peuvent être attachés à un coup. Ils
  sont principalement destinés à traiter les répertoires d'ouverture. Les options disponibles
  sont :
     <ul>
     <li> <img tb_tick> Inclure la ligne dans le répertoire</li>
     <li> <img tb_cross> Exclure la ligne du répertoire</li>
     <li> <img tb_mainline> Main Line</li>
     <li> <img tb_bkm> Signet</li>
     <li> <img tb_white> Blanc</li>
     <li> <img tb_black> Noir</li>
     <li> <img tb_new> Nouvelle ligne</li>
     <li> <img tb_rfilter> A vérifier par une analyse plus approfondie</li>
     <li> <img tb_msearch> Pour former</li>
     <li> <img tb_help_small> Ligne douteuse</li>
     <li> <img tb_cut> Pour supprimer</li>
     </ul>
	  Pour supprimer un marqueur, il suffit de sélectionner l'élément <menu>No marker</menu>.
  </li>
  <li><term>Couleur</term> Permet d'ajouter un petit carré coloré devant le coup pour sa propre mise en évidence. Pour l'enlever à nouveau
  sélectionner <menu>Blanc</menu></li>
  <li><term>Commentaire move</term> permet d'ajouter un commentaire textuel pour
  la ligne. Ce commentaire est affiché à droite de la ligne qu'il est
  associés. Notez que seule une partie de la première ligne apparaît là
  pour donner un retour visuel que le commentaire existe. L'ensemble
  le commentaire apparaît dans une info-bulle une fois que la souris est déplacée sur la ligne
  en question.
  </li>
  <li><term>Commentaire position</term> peut être utilisé pour ajouter un commentaire pour le
  position actuelle. Ce commentaire est affiché en haut de <term>Tree</term>
  une fois que la position commentée est atteinte. Notez que Scid
  n'affiche que la première ligne du commentaire pour économiser de l'espace dans le
  <term>arbre</term> fenêtre. Cependant, si la souris est déplacée sur cette fenêtre
  l'ensemble du commentaire apparaît dans une info-bulle.
  </li>
  </ul>
  N'oubliez pas de sauver le Masque ! Vous serez invité à le faire
  si vous fermez un Masque qui a été modifié ou si un <term>arbre</term>
  la fenêtre est fermée.
  </p>
  <p>
  Pour rechercher des commentaires, des symboles, etc., utilisez <menu>Masques /
  Recherche</menu>. Ici, on peut sélectionner différentes cases à cocher qui utilisent le
  a sélectionné l'option de recherche comme critère. Après avoir sélectionné le bouton Recherche
  <button tb_search_on> une liste de toutes les positions trouvées est affichée
  en <term>notation FEN</term> suivi du coup en question et
  le commentaire s'il y en a un.
  </p>
  <p>
  <menu>Afficher le masque</menu> affichera le masque actuel sur une ligne
  style. En se plaçant à la position actuelle, tous les coups suivants sont
  triés en un arbre dépliable pour donner une vue d'ensemble de la
  des lignes de jeu similaires à celles que l'on trouve dans de nombreux livres de répertoire.
  Notez que toutes les informations ne sont pas affichées (par exemple, les commentaires sont
  pour s'adapter à l'écran). De plus, comme <term>Masques</term>
  travailler sur des positions plutôt que de déplacer des séquences ; elles peuvent contenir des boucles
  (c'est-à-dire les transpositions) qui ne peuvent pas être dépliées en ligne
  s'affichent parfaitement. C'est-à-dire que cet affichage peut être coupé à un certain
  point.
  </p>
  <h3>Conversion en masques</h3>
  <p>
  La mise en place d'un masque peut être une tâche fastidieuse, surtout pour les
  des répertoires d'ouverture. Toutefois, si un tel répertoire est disponible en tant que
  Base de données Scid ou un nombre de <a PGN>PGN</a> jeux, ou de lignes stockées dans les
  d'échecs, Scid peut utiliser ces informations pour mettre en place des
  <term>Masques</term> automatiquement.
  </p>
  <p>
  Il faut d'abord charger l'information dans une base de données Scid.
  Dans le cas où l'information est déjà disponible dans une base de données Scid, cette
  est aussi facile que de l'ouvrir. Dans le cas où un fichier <a PGN>PGN</a> est utilisé, il devrait être
  soit importée dans une base de données Scid, soit on peut utiliser la
  <term>Clipbase</term> pour l'importer temporairement. Dans ce cas, un
  doit s'assurer que le <term>Clipbase</term> est vide avant
  l'importation. (<menu>Edition / Vider la base de données des clips</menu>).
  </p>
  <p>
  L'étape suivante consiste à ouvrir l'arbre de la base de données Scid qui vient d'être ouverte.
  Ensuite, il faut créer un nouveau <term>Masque</term> ou en ouvrir un existant. <b>Note</b> que cette fonction peut être utilisée pour consolider
  plusieurs bases en un seul <term>Masque</term>.
  </p>
  <p>
  Maintenant, le <term>Masque</term> peut être rempli automatiquement avec le jeu
  contenu de la base de données. Dans ce processus, les commentaires au sein des jeux
  seront convertis pour déplacer les commentaires (en les ajoutant à ceux qui sont déjà dans la
  éventuellement) dans le <term>Masque</term>. Les symboles <term>NAG</term> vont
  soit également ajouté. Pour lancer ce processus, on peut choisir soit
  <menu>Masques / Remplir avec jeu</menu> pour remplir le <term>Masque</term>
  avec le contenu d'un seul jeu, ou <menu>Remplir avec
  Base de données</menu> pour boucler tous les jeux de la base de données.
  </p>
  <p>
  <b>Note</b> : surtout le remplissage d'un <term>Masque</term> avec un
  peut prendre beaucoup de temps.
  </p>
  <p>
  <b>Note</b> : Le <term>masque</term> est rempli de tous les coups jusqu'à ce que le
  la fin du jeu, y compris toutes les variantes à l'intérieur d'un jeu. Donc,
  il est raisonnable de n'utiliser pour cette procédure que des bases qui mettent fin à la
  dès que le jeu du milieu est atteint.
  </p>

  <p><footer>(Mise à jour : Scid 4.3, novembre 2010)</footer></p>
}

####################
### Compaction help:

set helpTitle(F,Compact) "Compactage de base de données"
set helpText(F,Compact) {
<h1>Compactage de base de données</h1>
  <p>
  Base de données <term>compactage</term> est un type spécifique de
  <a Maintenance>maintenance</a> qui maintient une base de données aussi petite et
  efficace que possible.
  Compacter une base de données signifie supprimer tout espace inutilisé dans ses fichiers.
  Il en existe deux types : le compactage des fichiers de noms et des fichiers de jeu.
  </p>
  
  <h3>Nom du fichier de compactage</h3>
  <p>
  Au fil du temps, vous pouvez trouver une base de données commence à contenir un certain nombre de joueurs,
  les noms d'événements, de sites ou de tours qui ne sont plus utilisés dans aucun jeu. Cela permettra de
  se produisent souvent après que vous ayez vérifié l'orthographe des noms. Les noms non utilisés gaspillent de l'espace dans
  le fichier de noms, et peut ralentir les recherches de noms.
  Le compactage du fichier de noms supprime tous les noms qui ne sont pas utilisés dans les jeux.
  </p>
  
  <h3>Compactage du fichier de jeu</h3>
  <p>
  Chaque fois qu'une partie est remplacée ou supprimée, il reste de l'espace perdu dans la partie
  (le plus grand des trois fichiers d'une base de données Scid). Fichier de jeu
  Le compactage supprime tout l'espace gaspillé, ne laissant aucun jeu supprimé dans le
  base de données. A noter que cette opération est irréversible : après compactage,
  les jeux supprimés sont partis pour toujours !
  </p>
  <p>
  Le compactage des fichiers de jeu est également recommandé après <a Sorting>tri</a> a
  pour que l'ordre du fichier de jeu reste cohérent avec les
  fichier d'index.
  </p>
  
  <p><footer>(Mise à jour : Scid 3.6.2, décembre 2006)</footer></p>
}

####################
### Database maintenance tools help:

set helpTitle(F,Maintenance) "Maintenance de base de données"
set helpText(F,Maintenance) {
<h1>Maintenance de base de données</h1>
  <p>
  Scid fournit un certain nombre d'outils pour la maintenance des bases de données,
  disponible à partir du menu Scid <a Menus File>File</a>. Le menu
  base de données <a Compact>compactage</a> et <a Sorting>triage</a>
  sont expliquées dans des pages d'aide séparées.
  </p>
  
  <h3>Fenêtre de maintenance</h3>
  <p>
  La maintenance de la base de données Scid peut être effectuée en grande partie à partir du menu Maintenance
  qui peut être ouverte à partir de la fenêtre <menu>Fichier : Maintenance</menu>
  ou <menu>Windows</menu> menus ou la touche de raccourci <b>Ctrl+M</b>.
  </p>
  <p>
  Vous pouvez utiliser cette fenêtre pour maintenir les <a Flags>drapeaux de jeu</a>,
  Vérifiez l'orthographe des noms, <a Compact>compact</a> ou <a Sorting>tri</a>
  une base de données. Notez que toutes les opérations qui ne sont pas disponibles
  pour la base de données actuelle (par exemple, parce qu'elle peut être en lecture seule
  ou un fichier <a PGN>PGN</a>) sera grisé.
  </p>
  
  <h3><name Twins>Suppression des jeux de jumeaux</name></h3>
  <p>
  Le menu <menu>Fichier : Maintenance</menu> a une commande
  <menu>Supprimer les jeux jumeaux...</menu> pour détecter les copies supplémentaires
  (jumeaux) de jeux dans la base de données.
  Cette commande permet de trouver toutes les paires de jeux qui sont des jumeaux et, pour chaque paire,
  marque la partie la plus courte supprimée, laissant la partie la plus longue non supprimée.
  Deux jeux sont considérés comme des jumeaux si leurs joueurs
  (et toute autre balise que vous pouvez éventuellement spécifier) correspondent exactement.
  </p>
  <p>
  Si vous spécifiez l'option "mêmes coups", chaque paire de jeux doit avoir le
  les mêmes coups réels jusqu'à la durée de la partie la plus courte (ou jusqu'au coup 60,
  qui arrive en premier) pour être des jumeaux.
  </p>
  <p>
  Lorsque vous avez supprimé des jumeaux, il est bon de vérifier que chaque
  jeu supprimé est en fait une copie d'un autre jeu.
  Vous pouvez le faire facilement si vous avez sélectionné l'option
  "<b>Set filter to all deleted games</b>" dans l'option
  boîte de dialogue de suppression des jumeaux. Le filtre contient maintenant tous les jeux supprimés.
  Vous pouvez les parcourir (en utilisant les touches <b>p</b> et <b>n</b>) avec
  la fenêtre <term>twins checker</term> (disponible auprès de la maintenance
  ou la touche de raccourci <b>Ctrl+Shift+T</b>) pour vérifier que chaque jeu est
  supprimé parce qu'il est en fait un jumeau d'un autre jeu.
  </p>
  
  <h3><name Editing>Edition des noms de joueurs, d'événements, de sites et de tours</name></h3>
  <p>
  Il se peut que vous trouviez des noms mal orthographiés dans vos bases de données et que vous vouliez les corriger.
  Vous pouvez le faire dans Scid avec la fenêtre <term>Éditeur de noms</term>.
  (touche de raccourci : <b>Control+Shift+N</b>),
  disponible à partir du sous-menu <menu>Fichier : Maintenance</menu>.
  </p>
  <p>
  Chaque nom unique n'est enregistré qu'une seule fois dans le fichier de noms, donc changer un nom
  change en fait toutes les occurrences de celui-ci.
  </p>
  
  <h3><name Spellcheck>Vérification orthographique des noms</name></h3>
  <p>
  Scid est livré avec un fichier <term>vérification orthographique</term> nommé <b>épellation.ssp</b>,
  pour la correction des noms des joueurs, des événements, des sites et des rondes.
  Scid essaiera de charger le fichier de correction orthographique à chaque démarrage ; si le
  ne se charge pas, vous pouvez le charger à partir du menu <menu>Options</menu>.
  </p>
  <p>
  Une fois que le fichier de vérification orthographique est chargé, vous pouvez l'utiliser sur un
  une base de données Scid en utilisant les commandes de vérification orthographique dans le
  <menu>Fichier : Maintenance</menu> menu, ou à partir de la fenêtre de maintenance.
  </p>
  <p>
  Lorsque vous vérifiez l'orthographe d'une base de données, Scid produit une liste de corrections que vous
  peut éditer avant de faire des corrections, donc vous pouvez supprimer tout
  des corrections que vous ne voulez pas faire.
  </p>
  <p>
  La vérification orthographique est particulièrement utile pour normaliser une base de données afin que tous les
  Les instances d'un joueur particulier sont écrites de la même façon.
  Par exemple, avec le fichier de vérification orthographique standard, les noms "Kramnik,V.",
  "Vladimir Kramnik", et "V. Kramnik" seraient tous corrigés
  à "Kramnik, Vladimir".
  </p>
  <p>
  Le fichier de vérification de l'orthographe a un
  utilisation supplémentaire : lorsqu'il est chargé, ses données de lecteur sont
  utilisé pour améliorer la fenêtre <a PInfo>information du joueur</a> et la fenêtre
  <a Crosstable>crosstable</a> fenêtre :
  vous verrez le titre du master FIDE
  (<b>gm</b> = Grand Maître International, <b>im</b> = Maître International, etc)
  et des informations sur le pays pour tout joueur qui est
  listé dans le fichier de vérification orthographique. Plus de 6500 joueurs du passé et
  présents sont listés dans le fichier <b>spelling.ssp</b> qui est fourni avec Scid.
  </p>
  
  <h3><name Classements>Ajouter les classements Elo aux jeux</name></h3>
  <p>
  Le bouton "Ajouter des évaluations Elo..." dans la fenêtre de maintenance provoque le Scid
  pour rechercher dans la base de données actuelle les parties où un joueur n'a pas
  un classement, mais le fichier de vérification orthographique a un classement Elo listé pour cela
  joueur à la date du match. Scid ajoutera toutes ces évaluations
  automatiquement. Ceci est très utile pour une base de données de jeux de niveau maître
  qui a peu d'audience.
  </p>
  <p>
  Le fichier de vérification orthographique "spelling.ssp" qui est fourni avec Scid ne contient pas
  les informations de classement Elo nécessaires pour cette fonction, mais une version plus grande
  de celui-ci appelé " ratings.ssp " est disponible auprès de la
  <url http://scid.sourceforge.net/>Site web de Scid</url>.
  </p>
  
  <h3><name Cleaner>Le Nettoyeur</name></h3>
  <p>
  Le Scid <term>Nettoyeur</term> (disponible dans la fenêtre Maintenance) est
  un outil pour effectuer un certain nombre de tâches de maintenance sur une base de données en une
  action. Vous pouvez choisir les tâches que vous voulez faire, et Scid va
  les exécuter sur la base de données actuelle sans nécessiter d'interaction de la part de l'utilisateur.
  Ceci est particulièrement utile pour la maintenance de grandes bases de données.
  </p>
  
  <h3>Réglage du jeu d'autochargement de la base de données</h3>
  <p>
  Le jeu <term>autoload</term> d'une base de données est le jeu automatiquement
  chargé à chaque fois que cette base de données est ouverte. Pour changer le jeu d'autochargement de
  une base de données, utilisez le bouton "Autoload game number...". Si vous voulez toujours
  le dernier jeu d'une base de données à ouvrir (quel que soit le nombre réel
  de jeux dans la base de données), il suffit de le régler sur un nombre très élevé tel que
  9999999.
  </p>
  
  <h3>Réparer une base</h3>
  <p>
  Dans les rares cas où une base de données Scid est corrompue, on peut essayer de
  le réparer à l'aide de la base Fichier / Maintenance / Réparation. Pour que cela fonctionne,
  la base en question ne doit pas être ouverte (ce qui n'est pas possible dans
  la plupart des cas de toute façon). Scid fera alors de son mieux pour obtenir la base de données
  dans un état cohérent et utilisable.
  </p>

  <p><footer>(Mise à jour : Scid 3.6.26, octobre 2008)</footer></p>
}

####################
### Sorting help:

set helpTitle(F,Sorting) "Tri d'une base de données"
set helpText(F,Sorting) {
<h1>Tri d'une base de données</h1>
  <p>
  La fonction <term>tri</term> trie tous les jeux dans une base de données.
  Vous pouvez sélectionner un certain nombre de critères de tri.
  Lorsque deux jeux sont égaux selon les premiers critères, ils
  sont triés selon le deuxième critère, etc.
  </p>
  
  <h3> Critères de tri </h3>
  <p>
  Les critères de tri disponibles sont:
  </p>
  <ul>
  <li> Date (les jeux les plus anciens en premier) </li>
  <li> Année (identique à la date, mais en utilisant l'année uniquement) </li>
  <li> Nom de l'événement </li>
  <li> Nom du site </li>
  <li> Pays (3 dernières lettres du nom du site) </li>
  <li> Nom rond </li>
  <li> Nom blanc </li>
  <li> Note (moyenne des notes blanches et noires, la plus élevée en premier) </li>
  <li> Nom noir </li>
  <li> Résultat (les blancs gagnent, puis les nuls, puis les noirs gagnent) </li>
  <li> Durée (nombre de coups complets dans le jeu) </li>
  <li> ECO (le <a ECO> code de l'Encyclopédie des ouvertures d'échecs </a>) </li>
  </ul>
  
  <h3> Trier les résultats </h3>
  <p>
  Lorsque vous triez une base de données Scid qui n'est pas en lecture seule, le tri
  les résultats sont enregistrés de sorte que l'ordre des jeux dans la base de données est
  définitivement changé. Si vous voulez que les résultats du tri soient
  temporaire, mettez d'abord la base de données en lecture seule à l'aide du fichier <b>:
  Commande de menu en lecture seule </b>.
  </p>
  <p>
  Lorsque vous triez une base de données en lecture seule ou en fait un PGN
  fichier, les résultats du tri ne peuvent pas être enregistrés, donc l'ordre de tri des jeux
  sera perdu à la fermeture du fichier.
  </p>
  <p>
  Notez que le tri d'une base de données réinitialise la <a Searches Filter> recherche
  filtre </a> pour contenir tous les jeux.
  </p>
  
  <h3> Remarque importante sur le tri des bases de données: </h3>
  <p>
  Lorsqu'une base de données est triée, le fichier d'index est modifié mais le fichier de jeu
  n'est pas modifié. Cela signifie que le tri d'une base de données laissera le fichier du jeu
  enregistrements dans un ordre brouillé par rapport au fichier d'index. Ceci peut
  vraiment <b> ralentir </b> <a Tree> arbre </a>, position et matériau / motif
  <a Searches> recherches </a>, vous devez donc réorganiser le fichier du jeu en
  <a Compact> compactage </a> après le tri de la base de données à maintenir
  bonnes performances de recherche.
  </p>
  <p>
  Notez que seule une base de données triée par <a ECO> ECO </a> codes peut utiliser le
  modes de recherche d'arbre. Cependant, cela nécessite également de compacter le
  base de données après la procédure de tri!
  </p>
  
  <p> <footer> (Mise à jour: Scid 3.6.26, octobre 2008) </footer> </p>
}

####################
### Flags help:

set helpTitle(F,Flags) "Drapeaux de jeu"
set helpText(F,Flags) {
<h1>Drapeaux de jeu</h1>
  
  <p>
  Un <term>drapeau</term> est un indicateur d'une caractéristique qui peut être
  activé ou désactivé pour chaque partie d'échecs de la base de données. Cela
  permet de les rechercher facilement dans Scid.
  </p>
  <h3>Drapeaux prédéfinis</h3>
  <p>
  Il y a treize drapeaux que l'utilisateur peut directement modifier pour
  chaque partie. Parmi ceux-ci, seul le drapeau <b>Supprimer</b> a une
  signification spéciale : les jeux dont le drapeau Supprimer est activé sont
  marqués pour la suppression et seront supprimés lorsque la base de données sera
  <a Compact>compactée</a>.
  </p>
  <p>
  Les douze autres drapeaux modifiables par l'utilisateur et leurs symboles sont :
  </p>
  <ul>
  <li>Ouverture des Blancs (W)</li>
  <li>Ouverture des Noirs (B)</li>
  <li>Milieu de partie (M)</li>
  <li>Finale (E)</li>
  <li>Nouveauté (N)</li>
  <li>Structure de pions (P)</li>
  <li>Tactiques (T)</li>
  <li>Jeu à l'aile dame (Q)</li>
  <li>Jeu à l'aile roi (K)</li>
  <li>Spéctaculaire (!)</li>
  <li>Gaffe (?)</li>
  <li>Utilisateur (U)</li>
  </ul>
  <h3>Drapeaux personnalisables</h3>
  <p>
  Depuis Scid 4.0, six drapeaux supplémentaires sont disponibles pour une
  utilisation personnalisée.  Ces drapeaux sont spécifiques à chaque base et leur
  description textuelle peut être définie via la
  <a Maintenance>fenêtre de maintenance</a>. Dans la liste des jeux, la zone
  d'information sur la partie en cours etc., ils s'affichent comme les
  nombres (1) .. (6). Pour la <a Searches Header>recherche par en-tête</a>,
  ils se comportent exactement comme les drapeaux prédéfinis ci-dessus.
  </p>
  <h3>Utilisation</h3>
  <p>
  Un drapeau peut être défini pour le jeu en cours, tous les jeux de d'un filtre ou tous
  les jeux d'une base de données en utilisant la
  <a Maintenance>fenêtre de maintenance</a>.
  </p>
  <p>
  Vous pouvez utiliser une <a Searches Header>recherche dans l'en-tête</a>
  pour trouver tous les jeux dans une base de données qui ont un drapeau donné activé
  ou désactivé, ou utiliser des drapeaux dans le cadre de recherches plus complexes.
  </p>
  <p>
  Puisque tous les drapeaux paramétrables par l'utilisateur (sauf le drapeau Supprimer)
  n'ont pas de signification pour Scid, vous pouvez les utiliser dans n'importe quel but
  selon vos besoins. Par exemple, vous pourriez utiliser le drapeau Jeu à l'aile roi (K)
  pour les batailles de pions côté roi, ou les attaques de pièces lourdes côté roi,
  ou même pour les finales avec tous les pions à l'aile roi. Cependant, en fonction de
  l'usage, un drapeau personnalisé pourrait être plus approprié.
  </p>
  <p>
  Notez qu'une gestion judicieuse des drapeaux peut accélérer les recherches
  nettement !
  </p>
  <p>
  Les fonctions suivantes de Scid définissent ou requièrent des drapeaux :
  <ul>
  <li><a OpeningTrainer>Entraînement aux ouvertures</a> : peut évaluer les
  drapeaux (B) et (W).</li>
  <li><a FindBestMove>Trouver le meilleur coup</a> : évalue le drapeau (T).</li>
  <li><a Analysis Annotate>Trouver l'exercice tactique</a> : coche le
  drapeau (T).</li>
  </ul>
  </p>
  
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

####################
### Analysis window help:

set helpTitle(F,Analysis) "Fenêtre Analyse"
set helpText(F,Analysis) {
<h1>Fenêtre Analyse</h1>
  <p>
  La fenêtre d'analyse Scid montre l'analyse par un programme d'échecs
  (connu sous le terme de <term>moteur</term>) de la position actuelle de l'échiquier.
  À chaque fois que l'échiquier change, Scid envoie la nouvelle position au moteur
  pour qu'il mette à jour son évaluation.
  Actuellement, on peut faire tourner jusqu'à deux moteurs simultanément. Pour démarrer le
  premier moteur d'analyse, le raccourci <term>F2</term> peut être utilisé,
  pour le second moteur <term>F3</term>. Pour fermer rapidement le moteur
  la fenêtre vient d'appuyer sur <term>ESC</term>.
  </p>
  <p>
  Les principales informations de l'analyse actuelle sont présentées sur
  en haut de la fenêtre. Le premier chiffre indique la valeur d'évaluation
  en unités de gage.  Le score affiché dans la fenêtre d'analyse est toujours
  du point de vue des Blancs, donc un score négatif indique que les Noirs
  est mieux. <b>Profondeur:</b> indique la profondeur de recherche déjà atteinte par
  les calculs des moteurs en demi-coups. Le nombre après
  <b>Nœuds:</b> donne le nombre de positions analysées pour le courant
  alors que le nombre de positions par seconde (kn/s) est indiqué en
  des parenthèses.  <b>Time:</b> indique finalement le temps passé pour
  l'analyse actuelle.
  </p>
  <p>
  Des informations supplémentaires sont accessibles en utilisant le bouton
  <button tb_engineinfo>. Une nouvelle ligne sera ajoutée à la zone d'état
  montrant le coup courant analysé, le nombre de coups de la table de finales, un
  nombre plus exact de nœuds analysés par seconde, le filigrane de la
  le hash des moteurs et la charge actuelle du processeur. Habituellement, cette information est
  caché pour gagner de la place pour l'analyse.
  </p>
  <p>
  Si le moteur n'analyse que la ligne, il considère que la
  continuation, le cadre inférieur dans la fenêtre (avec la barre de défilement)
  montre l'historique des évaluations produites par le moteur pour le
  poste actuel, afin que vous puissiez voir comment l'évaluation a changé.
  </p>
  <p>
  De nombreux moteurs récents <term>UCI</term> permettent cependant d'analyser
  plusieurs lignes à la fois. En utilisant cette fonction <term>Multi-PV</term>, le
  l'utilisateur peut également voir ce que le moteur pense être le deuxième ou le troisième meilleur
  la continuation.  La meilleure ligne est toujours en haut et mise en évidence pour
  faciliter la lecture.  Si un moteur permet de <term>Multi-PV
  analyse</term>, la boîte de spin sous les lignes d'analyse peut être utilisée
  pour fixer le nombre de variantes principales à calculer
  et montré. Dans ce cas, au lieu de l'historique des calculs, seulement
  les lignes principales résultantes sont indiquées. Toutefois, si le nombre de
  lignes principales est réglé sur 1, on peut à nouveau observer l'évaluation
  l'histoire. La spin box est désactivée, si un moteur offre cette
  fonctionnalité.
  </p>
  <p>
  <b>Note</b> la sortie de l'analyse actuelle peut être cachée/affichée
  en cliquant avec le bouton droit de la souris dans la fenêtre d'analyse. Dans
  dans ce mode, seule l'analyse actuelle est affichée dans la ligne d'état.
  </p>
  <p>
  Pour ajouter le meilleur coup choisi par le moteur en tant que nouveau coup dans la
  jeu, appuyez sur la touche <b>Add move</b> bouton <button tb_addmove>. Si le
  toute la ligne principale du moteur doit être ajoutée, il suffit d'utiliser la commande <b>Add
  Variante</b> bouton <button tb_addvar>. Dans le cas où un moteur offre
  le mode <term>Multi-PV</term>, en utilisant le <b>Ajouter toutes les variantes</b>
  (<button tb_addallvars>) peut être utilisé pour ajouter tous les principaux
  variantes du jeu. L'insertion de variantes du
  ajoutera également le nom du moteur comme commentaire à la fenêtre d'analyse
  le jeu.
  </p>
  <p>
  Pour interrompre temporairement le processus de calcul, on peut utiliser la
  bouton d'arrêt du moteur (<button tb_eng_off>). Une fois pressé, ce bouton
  change le bouton de démarrage du moteur (<button tb_eng_on>) qui
  redémarrez l'analyse des moteurs. Notez, cependant, que la plupart des moteurs
  recommencer toute l'analyse, en oubliant tous les résultats que
  réalisé lors d'un cycle précédent. Seuls quelques moteurs sont capables de réutiliser les
  les résultats qu'ils ont calculés jusqu'à l'arrêt de l'analyse. Pour les
  premier moteur d'analyse, le moteur à bascule marche/arrêt est mappé sur le
  touche de raccourci <term>F4</term> tandis que pour le deuxième moteur <term>F5</term>
  peut être utilisé.
  </p>
  <p>
  Pour paramétrer un moteur pour analyser une position en arrière-plan tout en
  d'autres fonctions de Scid sont utilisées on peut <term>bloquer le
  position</term> en utilisant le bouton <button tb_lockengine>. Scid maintenant
  s'arrête pour envoyer au moteur toute modification de la carte mère, et le
  le moteur reste en calculant la position.
  </p>
  <p>
  Pour terminer le jeu en cours par le moteur (mode "shoot out" ou "démo")
  on peut utiliser le bouton de fin de partie <button tb_finish_off>. Pour indiquer
  le mode d'utilisation de l'appareil, cette touche devient <button tb_finish_on>. Ce
  n'est disponible que pour le premier moteur d'analyse.
  </p>

  <h3><name List>La liste des moteurs d'analyse</name></h3>
  <p>
  Scid maintient une liste des moteurs que vous avez utilisés, ainsi qu'un
  estimation de la note Elo (si vous en devinez une ; en plus quelques pages sur
  l'Internet offrent ces valeurs comme résultats de la grande motorisation
  tournois) et la date de la dernière utilisation de chaque moteur. Vous pouvez
  trier la liste des moteurs par nom, par classement Elo ou par date.
  Sélectionnez les boutons <b>Nouveau</b> ou <b>Editer</b> pour ajouter un nouveau moteur à
  la liste ou modifier les détails d'une entrée existante.
  </p>
  <h3><name Start>Commandes moteur et répertoires</name></h3>
  <p>
  Pour chaque moteur, vous devez spécifier le fichier exécutable à exécuter et
  dans quel répertoire Scid doit le lancer.
  </p>
  <p>
  La cause la plus probable des problèmes de démarrage du moteur est le choix
  dans quel répertoire le moteur doit fonctionner. Certains moteurs nécessitent un
  initialisation ou ouverture du fichier bibliothèque dans leur répertoire de départ pour l'exécution
  correctement.  D'autres moteurs (comme Crafty) écrivent des fichiers de log dans le
  dans lequel ils démarrent, vous devrez donc les exécuter dans un répertoire
  où vous avez un accès en écriture.  Si le paramètre de répertoire d'un moteur
  est ".", Scid va juste démarrer le moteur dans le répertoire courant.
  </p>
  <p>
  Donc si un moteur qui devrait fonctionner correctement dans le Scid ne démarre pas, essayez
  en modifiant le paramétrage de son répertoire. Pour éviter que les moteurs ne créent des fichiers de log
  dans de nombreux annuaires différents, je recommande de démarrer les moteurs dans
  le répertoire des fichiers de l'utilisateur Scid (c'est là que se trouve le fichier <b>scid.exe</b>
  est situé sous Windows, ou <b>~/.scid/</b> sous Unix) ; il y a un fichier
  dans la boîte de dialogue pour l'édition des détails du moteur marqué
  <b>scid.exe dir</b> sous Windows ou <b>~/.scid</b> sous Unix qui permet
  vous avez configuré le moteur pour qu'il démarre dans ce répertoire.
  </p>
  <p>
  Si un moteur a besoin de paramètres supplémentaires pour le démarrage (p. ex.
  une bibliothèque d'ouvertures spécifique), ils peuvent être spécifiés dans le
  <term>Paramètres</term> champ. Veuillez vous référer aux moteurs
  la documentation sur les paramètres requis, le cas échéant.
  </p>
  <p><term>URL</term> permet de définir la page d'accueil du moteur. Ce
  est utile pour vérifier les mises à jour, par exemple, ou pour jeter un coup d'œil aux récentes
  développements. En appuyant sur le bouton <term>Open...</term> s'ouvre
  cette page dans le navigateur web.
  </p>
  <p>
  Si le moteur utilise le protocole <term>UCI</term>, veuillez cocher la case
  case à cocher associée. En général, il y a deux protocoles principaux de moteur
  disponible aujourd'hui : xboard parfois appelé winboard (UCI doit
  pas être vérifiés) que beaucoup, surtout les moteurs plus anciens, utilisent, et le
  Protocole UCI qui peut être trouvé dans la plupart des moteurs d'échecs modernes.
  Le principal avantage d'un moteur <term>UCI</term> est que ce
  permet à l'utilisateur de configurer les paramètres des moteurs à partir de
  dans son interface graphique préférée comme Scid, tandis que pour l'autre
  les moteurs utilisent généralement des fichiers de configuration qui doivent être édités.
  Par conséquent, si le moteur est un moteur <term>UCI</term>, il faut appuyer sur la touche
  Le bouton <term>Configurer le moteur UCI</term> offre un dialogue graphique
  où tous les paramètres du moteur peuvent être réglés à la convenance de l'utilisateur. Ce
  Le dialogue sera différent pour chaque moteur car chaque moteur a
  d'autres paramètres qui peuvent être réglés. La plupart d'entre eux influenceront le
  en jouant sur le style du moteur. Veuillez vous référer aux moteurs
  documentation.
  </p>

  
  <h3>Entraînement</h3>
  <p>
  Avec le bouton <b>Entraînement</b>, vous pouvez jouer des coups contre l'analyse
  moteur. Le temps pour chaque coup est fixe, et les résultats de l'analyse sont
  ne s'affiche pas lorsque le mode d'entraînement est activé.
  </p>
  
  <h3><name Annotate>Annoter une partie</name></h3>
  <p>
  Le bouton <b>Ajouter une variante</b> (<button tb_addvar> ou
  <button tb_addallvars>) dans la fenêtre d'analyse ajoute le
  le score et la meilleure ligne de jeu comme une nouvelle variante dans le jeu.
  </p>
  <p>
  Vous pouvez le faire automatiquement pour un certain nombre de coups (en annotant le
  jeu) en appuyant sur le bouton <b>Annoter</b>, <button tb_annotate>.
  En outre, les meilleures lignes des moteurs aussi <a Moves Informant>Informant</a>
  Les symboles d'évaluation de style sont ajoutés dans ce processus.  Tout d'abord, le
  les paramètres pour les annotations automatiques doivent être définis :
  <ul>
     <li><term>Régler le temps entre les coups en secondes:</term> ceci
     donne le temps, le moteur peut passer sur l'analyse avant de passer à autre chose
     au prochain coup. </li>
     <li><term>Ajouter des variantes</term> : ici, on a le choix si
     les déplacements pour les deux côtés ou une seule couleur doit être ajoutée, si tous
     les coups devraient être annotés ou seulement si le coup de la partie n'est pas
     le meilleur coup selon l'évaluation des moteurs. De plus,
     on ne peut ajouter une annotation que si le coup de la partie est une bavure. Le
     définit la baisse de l'évaluation nécessaire pour signifier
     une bévue, le nombre est donné en unités de pions (c'est-à-dire que 0,2 signifie
     que si l'évaluation baisse de plus de 2 centimètres pions, le jeu
     est une bévue).
     </li>
     <li><term>Annoter les variantes</term> inclura les variantes
     au sein du jeu dans l'analyse par le moteur.</li>
     <li><term>Courtes annotations</term> n'ajouteront que des minima
     c'est-à-dire les lignes pures et les codes NAG sans les annotations
     et généralement sans les scores actuels.</li>
     <li><term>Ajouter un score aux annotations</term> ajoutera les moteurs
     l'évaluation en unités de gage aux annotations. Ces informations peuvent
     être utilisé plus tard pour dessiner un <a Score>Graphique de score</a> de la partie
     comme une visualisation des positions atteintes.
     </li>
     <li><term>Ajouter un tag annotator</term> ajoutera un tag "Annotator" à
     la tête de jeu. Ceci est destiné au <term>Short
     annotations</term> mode qui ne signifie pas quel moteur était
     utilisé pour obtenir les évaluations et les lignes actuelles.
     </li>
     <li><term>Utiliser la bibliothèque</term> permet de spécifier une 
     bibliothèque d'ouvertures. Les coups qui sont contenus dans cette bibliothèque d'ouverture sont ignorés
     dans le processus d'annotation, c'est-à-dire que l'annotation commence
     automatiquement dans le milieu de jeu. </li>
     <li><term>Annoter plusieurs parties</term> Scid va d'abord annoter
     le jeu en cours avec les paramètres spécifiés, stocker les résultats,
     puis charge automatiquement le jeu suivant et l'annote également.
     Ceci est répété jusqu'à ce que le jeu chargé atteigne le nombre indiqué dans
     la boîte à spin. Par cette procédure, de nombreux jeux peuvent être annotés
     automatiquement sans aucune intervention de l'utilisateur.
     </li>
     <li><term>Trouver les erreurs d'ouverture</term> vérifiera la phase d'ouverture
     jusqu'au coup spécifié pour les gaffes. De plus, le
     Annotator-tag obtient une entrée "opBlunder X" où X est le déplacement de la
     une bévue s'est produite.
     </li>
     <li><term>Marquer les exercices tactiques</term> Cela peut être utilisé pour
     générer des exercices pour la fonction d'entraînement
     <a FindBestMove>Trouver le meilleur coup</a>. Cette option est seulement
     disponible pour les moteurs <term>UCI</term>.
     </li>
  </ul>
  Après avoir appuyé sur la touche Ok, le mode de lecture automatique est activé et le
  le moteur commence à analyser le jeu.  Lorsque le mode autoplay est utilisé et que
  la fenêtre d'analyse est ouverte, une variante contenant le score et
  la meilleure ligne de jeu est automatiquement ajoutée pour chaque position comme
  Le mode autoplay se déplace dans le jeu.  Seules les positions du
  position actuelle jusqu'à la fin de la partie (ou jusqu'à ce que vous sortiez
  mode autoplay) sont annotés, vous pouvez donc sauter l'annotation de l'ouverture
  se déplace en se plaçant sur une position de milieu de jeu avant de commencer l'autoplay.
  </p>
  <p>
  Pour annuler l'annotation à tout moment, il suffit de désactiver le mode de lecture automatique, par exemple en
  en appuyant sur la touche <b>Ecape</b> dans la fenêtre principale.
  </p>
  <p>
  Notez que pour des raisons de simplicité, le bouton <b>Annoter</b> n'est disponible que
  dans la fenêtre ouverte en tant que moteur d'analyse 1. Si vous ouvrez un moteur en tant que
  moteur d'analyse 2, vous ne pouvez pas l'utiliser pour annoter le jeu.
  </p>
  
  <h3>Échiquier d'analyse</h3>
  <p>
  Appuyer sur le bouton <button tb_coords> dans une fenêtre d'analyse
  permet d'afficher ou masquer l'échiquier d'analyse, qui indique la position à la
  fin de la meilleure ligne de jeu actuellement trouvée par le moteur. Cela fonctionne
  pour la plupart des moteurs compatibles Scid mais ne fonctionne pas forcément pour
  tous ; cela dépend la notation de coup qu'utilise le moteur.
  </p>
  
  <h3>Priorité du moteur</h3>
  <p>
  Si un moteur utilise trop de temps CPU et affecte l'utilisation de Scid
  ou d'autres applications, en activant le bouton <b>Faible priorité du CPU</b>
  <button tb_cpu> peut aider ; il donne au moteur une faible priorité pour le CPU
  la planification. Sous Windows, les moteurs sont exécutés par défaut en basse priorité.
  </p>
  <p>
  <b>Note</b> que sur les systèmes Unix la priorité des moteurs ne peut pas être définie
  de retour à la normale.
  </p>

  <p><footer>(Mise à jour : Scid 3.6.26, octobre 2008)</footer></p>
}

####################
### Calvar window help:

set helpTitle(F,CalVar) "Entraînement : calcul de variantes"
set helpText(F,CalVar) {
<h1>Entraînement : calcul de variantes</h1>
  <p>
   Cet exercice d'entraînement est également connu sous le nom d'exercice Stoyko.
   Le but est d'analyser une position complexe, d'évaluer le plus grand nombre possible
   de lignes de jeu, et de donner une évaluation correcte pour chacune d'entre elles.
  </p> 
  <h3>Configuration</h3>
  <p>
  Trois paramètres peuvent être configurés :
  <ul>
     <li>le <term>moteur d'échecs UCI</term> qui va analyser les différentes lignes de jeu,</li>
     <li>le temps, en secondes, que le moteur va utiliser pour analyser la position,</li>
     <li>le temps, en secondes, que le moteur va utiliser pour analyser chaque ligne saisie par l'utilisateur.</li>
  </ul>
  </p>

  <h3>Saisie des lignes de jeu</h3>
  <p>
  Les coups sont <a Moves>saisis</a> comme d'habitude avec des clics de souris sur l'échiquier
  mais ils ne seront pas affichés. À la fin d'une ligne de jeu, l'utilisateur doit fournir
  une évaluation en cliquant sur un des boutons avec les <a NAGs>codes NAG</a>.
  </p>

  <h3>Vérification de l'évaluation</h3>
  <p>
  À chaque fois qu'une évaluation est donnée pour une ligne de jeu, le moteur de jeu
  calcule sa propre évaluation et ajoute la ligne qu'il a trouvée avec son score, comme
  variante juste en dessous de celles de l'utilisateur dans la <a PGN>fenêtre PGN</a>
  de la partie en cours.
  </p>

  <h3>Fin de l'analyse de position</h3>
  <p>
   Lorsqu'il pense avoir trouvé toutes les meilleures lignes de jeu, l'utilisateur met un
   terme à l'exercice en appuyant sur le bouton <b>[Fin de l'analyse de position]</b>.
   Le moteur de jeu ajoutera alors dans la <a PGN>fenêtre PGN</a> de la partie,
   avec le commentaire <term>Missed line</term>, les lignes de jeu qui ont un score
   supérieur à la meilleure ligne saisie par l'utilisateur.
  </p>

  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

####################
### EPD files help:

set helpTitle(F,EPD) "Fichiers EPD"
set helpText(F,EPD) {
<h1>Fichiers EPD</h1>
  <p>
  Un fichier EPD (description étendue de poste) est un ensemble de postes,
  où chaque position a un texte associé. Comme <a PGN>PGN</a>, il
  est un standard commun pour l'information sur les échecs.
  </p>
  <p>
  Un fichier EPD comporte un certain nombre de <term>opcodes</term> (champs) définis
  qui sont stockées séparées par des points-virgules (<b>;</b>) dans le fichier
  mais sont affichés sur des lignes séparées dans une fenêtre Scid EPD pour faciliter l'édition.
  Un point-virgule dans un champ EPD est stocké comme "<b>\s</b>" par Scid pour
  le distinguer d'un marqueur de fin de champ.
  Chaque position et ses codes d'opération associés sont stockés sur une seule ligne
  dans le fichier EPD.
  </p>
  <p>
  Les codes d'opération EPD standard comprennent :
  <ul>
  <li> <b>acd</b> Compte d'analyse : profondeur recherchée.</li>
  <li> <b>acn</b> Compte d'analyse : nombre de nœuds recherchés.</li>
  <li> <b>acs</b> Compte d'analyse : temps de recherche en secondes.</li>
  <li> <b>bm</b> Meilleurs coups : coup(s) jugé(s) meilleur(s) pour une raison quelconque.</li>
  <li> <b>ce</b> Évaluation des centipaires : évaluation en centièmes d'a
  Pion du point de vue du côté <b> à déplacer</b> -- notez ceci
  diffère de la fenêtre d'analyse qui affiche les évaluations en pions de
  La perspective des Blancs. </li>
  <li> <b>cX</b> Commentaire (où <b>X</b> est un chiffre, 0-9).</li>
  <li> <b>eco</b> <a ECO>ECO</a> code d'ouverture du système.</li>
  <li> <b>id</b> Identification unique pour cette position.</li>
  <li> <b>nic</b> <i>New In Chess</i> code d'ouverture du système.</li>
  <li> <b>pm</b> Coup prédit : le premier coup de la PV.</li>
  <li> <b>pv</b> variante prédite : la ligne de meilleur jeu.</li>
  </ul>
  </p>
  
  <p>
  Les fichiers EPD ont plusieurs usages : Scid utilise un fichier EPD pour classer
  parties selon la <a ECO>Encyclopédie des Ouvertures d'Échecs</a> (ECO)
  et vous pouvez créer un fichier EPD pour votre répertoire d'ouverture,
  en ajoutant des commentaires pour les postes que vous atteignez régulièrement dans les jeux.
  </p>
  <p>
  Vous pouvez créer un nouveau fichier EPD ou ouvrir un fichier existant, à partir de la fenêtre
  <menu>Nouveau</menu> et <menu>Ouverture</menu> des commandes de la
  <menu>Fichier</menu> menu. Au maximum quatre fichiers EPD peuvent être ouverts à tout moment.
  </p>
  
  <h3>Fenêtres EPD</h3>
  <p>
  Pour chaque fichier EPD ouvert, vous verrez une fenêtre qui affiche le texte de
  la position actuelle. Vous n'avez pas besoin d'appuyer sur le bouton Store pour mémoriser
  toute modification apportée à un texte de position ; le texte sera mémorisé chaque fois que
  vous vous déplacez à une position différente dans le jeu.
  </p>
  
  <h3>Navigation des fichiers EPD</h3>
  <p>
  Pour parcourir les postes d'un fichier EPD, utilisez la fonction
  <menu>Position suivante</menu> et <menu>Position précédente</menu> commandes
  de la fenêtre EPD <menu>Outils</menu>, ou utiliser le raccourci
  touches <b>Ctrl+Flèche vers le bas</b> et <b>Ctrl+Flèche vers le haut</b>.
  Ces commandes permettent de passer à la position suivante/précédente dans le fichier, en effaçant
  le jeu en cours et de définir sa position de départ.
  </p>

  <h3>Annotation</h3>
  <p>
  Les fichiers EPD peuvent être annotés automatiquement en sélectionnant Outils /
  Annoter la position. Le dialogue à venir demande le temps que
  doit être utilisé pour l'analyse, alors le <a Analysis List>moteur
  list</a> est ouvert pour la sélection d'un moteur à utiliser. <b>Note</b> Si un
  est déjà ouverte, l'analyse se fait avec cette
  sans demander à l'utilisateur au préalable. Ensuite, le moteur est
  a commencé et le résultat a été ajouté à l'EPD. Les balises EPD utilisées sont :
  acd, acn, ce et pv.
  </p>

  <h3>Découper les champs EPD</h3>
  <p>
  Les fichiers EPD que vous trouvez sur Internet peuvent contenir des champs qui ne
  vous intéressent, et ils peuvent gaspiller beaucoup d'espace dans le dossier.
  Par exemple, un fichier EPD d'évaluations informatiques pourrait avoir ce, acd,
  les champs acn, pm, pv et id mais vous n'aurez peut-être besoin que des champs ce et pv.
  </p>
  <p>
  Vous pouvez supprimer un opcode EPD de toutes les positions du fichier EPD en utilisant
  le <menu>Découper le champ EPD</menu> de la fenêtre EPD <menu>Outils</menu>
  menu.
  </p>

  <h3>La barre d'état de la fenêtre EPD</h3>
  <p>
  La barre d'état de chaque fenêtre EPD s'affiche :
  <ul>
  <li>- l'état du fichier (<b>--</b> signifie inchangé, <b>XX</b> signifie
  a changé, et <b>%%</b> signifie en lecture seule) ; </li>
  <li>- le nom du fichier ; </li>
  <li>- le nombre de positions dans le fichier ; </li>
  <li>- les coups légaux de la position actuelle atteignent une autre position
  dans ce fichier EPD.</li>
  </ul>
  </p>
  
  <p><footer>(Mise à jour : Scid 3.6.26, octobre 2008)</footer></p>
}

####################
### Email window help:

set helpTitle(F,Email) "Fenêtre Gestionnaire de messagerie"
set helpText(F,Email) {
<h1>Fenêtre Gestionnaire de messagerie</h1>
  <p>
  La fenêtre du gestionnaire de courriers électroniques de Scid vous offre un moyen de
  gérer les parties d'échecs par correspondance jouées par courriel.
  Si vous ne jouez pas aux <a CCeMailChess>échecs par courriel</a>, cela ne vous
  intéressera pas. Mais si vous y jouez, vous pouvez envoyer votre courriel directement
  de Scid !
  </p>
  <h3>Utilisation</h3>
  <p>
  Pour utiliser le gestionnaire de messagerie :
  <ul>
  <li><b>1.</b> Créez le(s) partie(s) de jeu pour votre adversaire dans la
  base de données.</li>
  <li><b>2.</b> Dans la fenêtre du gestionnaire de messagerie, sélectionnez
  <b>[Ajouter]</b>, et entrez les détails de vos adversaires : nom, adresse
  email, et les numéros de jeu dans cette base de données.</li>
  <li><b>3.</b> Sélectionnez <b>[Envoyer email]</b> à chaque fois que vous
  avez ajouté des coups à la/aux partie(s) et voulez envoyer un message.</li>
  </ul>
  </p>
  <p>
  La boîte à liste <b>[Temps]</b> permet de sélectionner "Reçus aujourd'hui",
  "Envoyés aujourd'hui", ou "Édition".
  </p>
  <p>
  Lorsque vous envoyez un message électronique, Scid génère le contenu du message avec
  les jeux en <a PGN>format PGN</a> <b>sans</b> commentaires, annotations ou
  variantes, puisque vous ne voudriez généralement pas que votre adversaire voie votre
  analyse. Vous pouvez éditer le message avant de l'envoyer pour ajouter des coups
  conditionnels ou d'autres textes.
  </p>
  <p>
  Pour chaque adversaire, vous pouvez avoir n'importe quel nombre de parties ; une ou
  deux étant le plus commun. Notez que Scid ne vérifie pas si les numéros de jeu
  changent, donc après la mise en place des détails de vos adversaires,
  <b>faites attention à ne pas effacer ou trier les parties</b> de votre base de données
  de jeux par email, car cela réorganisera les jeux et les numéros de jeu pour chaque
  adversaire seront incorrects.
  </p>
  
  <h3>Limitations</h3>
  <p>
  Scid n'a pas encore la possibilité de vérifier votre dossier de courrier électronique,
  donc vous devez toujours ajouter manuellement les coups de vos adversaires
  aux parties en cours.
  </p>
  
  <h3>Configuration</h3>
  <p>
  Une copie de chaque message électronique envoyé par Scid est stockée dans le fichier
  <b>~/.scid4.7/scidmail.log</b>. Si vous voulez qu'ils soient stockés
  dans un autre fichier, vous devrez éditer le fichier <b>tcl/start.tcl</b>
  et recompiler Scid.
  </p>
  <p>
  Scid peut envoyer des messages électroniques en utilisant un serveur SMTP ou sendmail.
  Utilisez le bouton <b>[Configurer...]</b> dans le gestionnaire de messagerie
  pour spécifier celui que vous voulez utiliser.
  </p>
  <p>
  Scid stocke les détails de l'adversaire pour une base de données dans un fichier
  avec le même nom que cette base et le suffixe "<b>.sem</b>".
  </p>
  
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

####################
### Reports help:

set helpTitle(F,Reports) "Rapports"
set helpText(F,Reports) {
<h1>Rapports</h1>
  <p>
  Un <term> rapport </term> dans Scid est un document contenant des informations sur
  une position et / ou un joueur particulier. Il existe deux types de rapports que Scid peut
  générer: Rapports d'ouverture et Rapports de joueur.
  </p>
  
  <h3> <name Opening> Rapports d'ouverture </name> </h3>
  <p>
  Scid peut produire un <term> rapport d'ouverture </term> qui affiche des informations intéressantes
  faits sur une position d'ouverture. Pour générer un rapport d'ouverture, faites d'abord
  assurez-vous que la position affichée est celle pour laquelle vous souhaitez un rapport, puis sélectionnez
  <b> Ouverture du rapport </b> dans le menu <b> Outils </b>.
  </p>
  <p>
  La fenêtre <term> Ouverture du rapport </term> affiche les résultats du rapport
  Généré par Scid. Le menu <b> Fichier </b> contient des commandes pour enregistrer le rapport
  vers un fichier au format texte brut, HTML ou <a LaTeX> LaTeX </a>.
  </p>
  <p>
  Les premières sections du rapport présentent des informations sur les jeux
  atteindre la position du rapport et les coups joués à partir de la position. Vous pouvez
  voir si l'ouverture devient plus populaire, si elle a de nombreux tirages courts,
  et quels ordres de déplacement (transpositions) sont utilisés pour l'atteindre.
  </p>
  <p>
  La section Thèmes positionnels indique la fréquence de certains
  thèmes positionnels dans les jeux de rapports. Pour cela, les 20 premiers coups de
  chaque partie (d'où les 40 premières positions de chaque partie après le début
  sont examinés. Pour être considéré comme contenant un thème, un jeu doit
  contenir ce thème particulier dans au moins 4 positions de ses 20 premiers
  se déplace. Cela évite la brève occurrence d'un thème (tel qu'un
  pion-dame qui est rapidement capturé) déforme les résultats.
  </p>
  <p>
  La dernière et la plus grande partie du rapport est le tableau théorique. Lors de l'enregistrement
  le rapport dans un fichier, vous pouvez choisir d'enregistrer uniquement la table de théorie, un compact
  rapport sans la table théorique, ou le rapport entier.
  </p>
  <p>
  Presque toutes les sections du rapport peuvent être activées ou désactivées ou ajustées dans
  les options d'ouverture du rapport, de sorte que vous pouvez personnaliser un rapport pour afficher uniquement
  les informations qui vous intéressent.
  </p>
  <p>
  La plupart des informations dans la fenêtre de rapport qui sont affichées en couleur,
  invoquer une action lorsqu'elle est sélectionnée avec le bouton gauche de la souris. Par exemple,
  vous pouvez cliquer sur une référence de jeu pour charger ce jeu, ou cliquer sur un
  thème positionnel pour définir le filtre pour ne contenir que les jeux de rapports où
  ce thème s'est produit.
  </p>
  <p>
  Si vous choisissez <term> Fusionner les jeux </term>, les jeux seront fusionnés
  ouverture du rapport dans le jeu actuellement affiché. Cela se confond dans
  l'intégralité des jeux en tant que variantes du jeu, y compris
  référence.
  </p>
  
  <h4> Favoris </h4>
  <p>
  Le menu <menu> Favoris </menu> de la fenêtre de rapport vous permet de conserver une
  collection de positions de rapport d'ouverture préférées et générer l'ouverture
  rapports pour tous ces postes facilement. Sélectionner "Ajouter un rapport ..." dans
  le menu Favoris ajoutera la position actuelle en tant que rapport favori
  position; vous serez invité à entrer un nom qui sera utilisé comme
  le nom du fichier lors de la génération des rapports favoris.
  </p>
  <p>
  Sélectionnez "Générer des rapports ..." dans le menu Favoris pour générer un rapport
  pour chacun de vos rapports préférés en utilisant la base de données actuelle. Une boîte de dialogue
  apparaîtra vous permettant de spécifier le type et le format du rapport, et
  un répertoire où les fichiers de rapport seront enregistrés. Un suffixe approprié pour le
  le format que vous avez sélectionné (par exemple ".html" pour le format HTML) sera ajouté à chaque
  nom du fichier de rapport.
  </p>
  
  <h3> <name Player> Rapports des joueurs </name> </h3>
  <p>
  Un <term> Rapport du joueur </term> est très similaire à un rapport d'ouverture, mais il
  contient des informations sur les jeux d'un seul joueur avec le blanc ou
  pièces noires. Vous pouvez générer un rapport de joueur depuis le menu Outils ou depuis
  la fenêtre <a PInfo> Infos joueur </a>.
  </p>
  <p>
  Un rapport de joueur peut être généré soit pour tous les jeux par le joueur spécifié
  avec les pièces spécifiées, ou uniquement pour le sous-ensemble des jeux qui atteignent
  la position actuelle sur l'échiquier de la fenêtre principale.
  </p>
  
  <h3> Limites </h3>
  <p>
  Il y a une limite de 2000 jeux pour la plupart des données générées par les rapports, donc
  si la position du rapport se produit dans plus de 2000 jeux, certains résultats peuvent
  être légèrement incorrect.
  </p>
  <p>
  De plus, il y a une limite de 500 jeux pour la table théorique. Si le rapport
  position se produit dans plus de 500 jeux, seuls les 500 jeux avec le plus haut
  la note Elo moyenne est utilisée pour générer la table théorique. Vous pouvez régler
  nombre de jeux utilisés pour générer la table théorique dans les options du rapport.
  </p>
  
  <p> <footer> (Mise à jour: Scid 3.6.26, octobre 2008) </footer> </p>
}

####################
### Player List help:

set helpTitle(F,PList) "Fenêtre Recherche de joueur"
set helpText(F,PList) {
<h1>Fenêtre Recherche de joueur</h1>
   <p>
   La fenêtre <term>Chercher un joueur</term> affiche une liste de noms de
   joueurs dans la base de données actuelle. La sélection d'un joueur ouvrira la
   fenêtre <a PInfo>information sur un joueur</a> pour afficher
   des informations plus détaillées à propos de ce joueur.
   </p>
   <p>
   Cinq colonnes sont affichées montrant le nom de chaque joueur, son pic de
   classement Elo, le nombre de matchs joués et l'année de son plus ancienne et
   nouvelle partie. Cliquez sur un titre de colonne en haut de la liste pour la trier
   selon cette colonne.
   </p>
   <p>
   Les commandes au-dessus de la liste vous permettent d'en filtrer le contenu.
   Vous pouvez modifier la taille maximale de la liste, entrer un préfixe de nom
   de joueur (par exemple "ada" pour rechercher "Adams", c'est insensible à la casse),
   et restreindre la gamme de classement Elo et le nombre de parties jouées.
   </p>
  
   <p><footer>(Mise à jour: Scid 4.7.1, janvier 2020)</footer></p>
}

####################
### Player Info help:

set helpTitle(F,PInfo) "Fenêtre Information sur un joueur "
set helpText(F,PInfo) {
<h1>Fenêtre Information sur un joueur </h1>
  <p>
  
  La fenêtre <term>Information sur un joueur</term> est générée ou mise à jour chaque fois
  vous cliquez avec le bouton gauche de la souris sur un nom de joueur dans les informations du jeu
  (sous l'échiquier) ou dans la fenêtre <a Crosstable>Classement de tournoi</a>.
  </p>
  <p>
  Il affiche (espérons-le) des informations utiles sur le joueur, y compris
  succès avec le blanc et le noir, ouvertures préférées (par <a ECO> code ECO </a>),
  et l'historique des notations. <br>
  Les données de l'historique Elo peuvent éventuellement être extraites de la base de données actuelle
  ou à partir du fichier d'orthographe / fichier de notation. Si vous utilisez un fichier de notation réel, l'heure
  des périodes peuvent également être affichées, pour lesquelles il n'y a pas de jeux dans la base de données. <br>
  Téléchargez un fichier d'évaluation avec plus de 300.000 joueurs FIDE enregistrés avec leur
  Classements ELO de <url https://sourceforge.net/projects/scid/files/Player Data />
  https://sourceforge.net/projects/scid/files/Player Data / </url>.
  </p>
  <p>
  Tous les pourcentages affichés sont un score attendu (taux de réussite), du
  point de vue du joueur - si haut est toujours meilleur pour le joueur, qu'il
  sont blancs ou noirs.
  </p>
  <p>
  Vous pouvez voir l'historique des notes du joueur dans un graphique en appuyant sur la touche
  Bouton <a Graphs Rating> Graphique d'évaluation </a>.
  </p>
  <p>
  Tout nombre imprimé en rouge peut être cliqué avec le bouton gauche de la souris pour définir
  le <a Searches Filter> filtre </a> aux jeux qu'il représente.
  </p>
  <p>
  Si une version actuelle du fichier de notation (cf.
  <url http://scid.sourceforge.net/> Site Web Scid </url>) est un lien en ligne vers
  des ressources supplémentaires peuvent être présentes qui seront accessibles par le
  navigateur Web des utilisateurs à condition qu'un accès Internet soit disponible. Ces infos
  peut être consulté par les symboles suivants dans les informations du joueur
  fenêtre (notez que tous les symboles peuvent ne pas être présents!):
  <ul>
  <li> <button fidelnk> Accédez à la page de profil FIDE des joueurs </li>
  <li> <button iccflnk> Accédez à la page de profil ICCF des joueurs </li>
  <li> <button dsblnk> Accédez au profil des joueurs au Deutscher
  Schachbund. </li>
  <li> <button bcflnk> Accédez au profil des joueurs à la Fédération britannique des échecs. </li>
  <li> <button smlnk> Accédez au profil des joueurs sur SchemingMind. </li>
  <li> <button wikiplnk> Appelez la page de profil Wikipedia des joueurs.
  Scid essaiera d'utiliser la langue définie pour Scid lui-même. Si aucune page n'est
  disponible, il aura recours à l'allemand (pour des raisons purement techniques). </li>
  <li> <button dnblnk> Appelez la bibliothèque nationale allemande pour les
    livres de ou sur le joueur. </li>
  <li> <button seealsolnk> Une autre façon d'accéder à Wikipedia. </li>
  <li> <button viaflnk> Appeler les informations du joueur sur le Virtual
  Authority File (VIAF). Cela pourrait être utilisé pour appeler des livres de / par ce
  dans les bibliothèques nationales contributrices. </li>
  </ul>
  </p>
  
  <p> <footer> (Mise à jour: Scid 4.7, novembre 2018) </footer> </p>
}

####################
### Graphs help:

set helpTitle(F,Graphs) "Fenêtres graphiques"
set helpText(F,Graphs) {
<h1>Fenêtres graphiques</h1>
  <p>
  Scid dispose d'un certain nombre de fenêtres qui affichent des informations sous forme graphique.
  Elles sont expliquées ci-dessous.
  </p>
  
  <h3><name FilterGraph>Fenêtres FilterGraph relatives et absolues</name></h3>
  <p>
  La fenêtre <term>Graphique de filtre relatif</term> montre les tendances par date ou par
  Le classement Elo des jeux dans le filtre actuel, par rapport à la
  toute la base de données. Par exemple, il est utile lorsque le <a Tree>arbre</a>
  est ouvert comme un outil montrant comment la position d'ouverture actuelle a changé
  en popularité au cours des dernières années ou décennies, ou si elle est particulièrement
  populaire parmi les joueurs de haut niveau comme les grands maîtres.
  Chaque point sur le graphique représente le nombre de parties dans le filtre
  par 1000 jeux dans l'ensemble de la base de données, pour une date particulière ou Elo
  de l'échelle de notation.
  </p>
  <p>
  La deuxième fenêtre du graphique du Filtre est le <term>filtre absolu
  Graphique</term> fenêtre. Ce graphique montre la quantité absolue de jeux
  dans le filtre selon les critères sélectionnés. Il est possible de sélectionner
  les critères : Décennie, Année, Classement et Coups.<br>
  Le petit bouton près du bouton de fermeture ouvre la boîte de dialogue pour configurer
  le graphique. Vous pouvez sélectionner la plage (de, à) et la taille de
  pour les axes X. Les critères de "décennie" ne peuvent pas être configurés,
  utilisez plutôt Year. Si "Estimation" est sélectionné, une cote manquante sera
  comme décrit ci-dessous. Dans les autres cas, aucune estimation n'est faite
  et les cotes manquantes sont comptées comme zéro. Cela correspond au Min. Elo
  dans la fenêtre statistique. Utilisez "Update" pour commencer une nouvelle
  l'analyse avec la valeur réelle sans fermer le dialogue.
  </p>
  <p>
  Remarque : Le calcul peut prendre un certain temps sur de grandes plages et
  des petits intervalles, en plus la lisibilité diminue. 
  </p>
  <p>
  Pour tracer le graphique du filtre par notation, Scid utilise la moyenne (average)
  pour chaque jeu. Estimer les cotes (comme celles du fichier d'orthographe)
  ne sont pas utilisés. Si un joueur dans une partie a une cote mais que l'adversaire
  ne le fait pas, l'adversaire est présumé avoir le même jusqu'à une limite de 2200.
  Par exemple, si un joueur a une cote de 2500 et que l'adversaire n'a pas de cote,
  la cote moyenne est de (2500+2200)/2=2350.
  </p>
  
  <h3><name Classement>Fenêtre du graphique de classement</name></h3>
  <p>
  La fenêtre <term>Graphique de classement</term> montre l'historique de classement d'un
  ou les deux joueurs de la partie en cours.
  Vous pouvez produire le graphique pour un seul joueur en appuyant sur la touche
  <b>Graphique de classement</b> bouton dans le <a PInfo>information du joueur</a>
  ou le produire pour les deux joueurs de la partie en cours en
  en sélectionnant <b>Graphique de classement</b> dans le menu <menu>Outils</menu>.
  </p>
  
  <h3><name Score>Fenêtre du graphique de score</name></h3>
  <p>
  La fenêtre <term>Graphique de score</term> montre les évaluations numériques (scores)
  stocké dans les commentaires du jeu en cours sous forme de graphique.
  Vous pouvez cliquer avec le bouton gauche de la souris n'importe où dans le graphique des scores pour aller à la page
  position correspondante dans le jeu.
  </p>
  <p>
  Deux types de commentaires d'évaluation sont reconnus : 
  <ul>
  Tous les commentaires qui contiennent un numéro signé, par exemple 
  <li><b>+1,23, -0,23</b></li>
  </ul>
  <ul>
  Étiquettes avec le nom [%eval .. ]
  <li><b>[%emt 0:00:19][%eval -6.64/23]</b></li>
  </ul>
  </p>
  <p>
    Certains scores ne sont présentés que du point de vue des blancs. Vous pouvez inverser le score pour les Blancs ou les Noirs
    dans le menu des options pour corriger cela. Sélectionnez Blanc et/ou Noir.
  </p>
  <p>
    Dans un <term> Diagramme de temps</term> on peut aussi afficher le temps de la consommation de temps par coup ou le temps restant sur l'horloge. Pour l'évaluation du temps, les balises [%clk 0:01:19] et [%emt 0:00:19] doivent être contenues dans le commentaire.
    Les valeurs sont toujours affichées en minutes.
    Le menu permet de sélectionner la combinaison des diagrammes qui doit être affichée :
    <li><b>Les deux:</b> L'évaluation du jeu et le temps sont affichés.</li>
    <li><b>Time:</b> Seul le graphique du temps est affiché ("Both" doit être désélectionné !)</li>
    <li><b>Somme:</b> Dans l'affichage du temps, la somme du temps consommé est affichée si le temps est disponible en [%emt 00:12].</li>
  </p>
  
  <h3><name Tree>Fenêtre du graphique de l'arbre</name></h3>
  <p>
  La fenêtre <term>Graphique de l'arbre</term> est disponible à partir de l'arbre
  fenêtre. Il montre l'exécution des coups les plus populaires de la fenêtre
  position actuelle. Pour plus de renseignements, veuillez consulter le site Web de l
  <a Tree Graph>Arbre</a> page d'aide.
  </p>
  
  <p><footer>(Mise à jour : Scid 4.7, juin 2018)</footer></p>
}

####################
### Tablebases help:

set helpTitle(F,TB) "Utilisation de tables de finales"
set helpText(F,TB) {
<h1>Utilisation de tables de finales</h1>
  
  <p>
  Une <term>table de finales</term> est un fichier contenant le résultat parfait
  des informations sur toutes les positions d'une configuration matérielle particulière,
  tels que King and Rook contre King and Pawn. Des bases de table pour tous
  des situations matérielles jusqu'à cinq hommes (y compris les rois) ont été
  généré, et quelques simples bases de table pour 6 hommes sont également disponibles.
  </p>
  <p>
  Scid peut utiliser des bases de table au format Nalimov qui sont utilisées par de nombreux
  moteurs d'échecs. Celles-ci se terminent souvent par le suffixe de fichier <b> .nbw.emd </b>
  ou <b> .nbb.emd </b>. Toutes les bases Nalimov pour 3, 4 et 5 hommes peuvent être
  utilisé dans Scid.
  </p>
  
  <h3> Utilisation des bases de table dans Scid </h3>
  <p>
  Pour utiliser les fichiers de tables de finales dans Scid, définissez simplement leurs répertoires par
  en sélectionnant <b> Répertoire des tables de finales ... </b> dans le menu <menu> Options </menu>.
  Vous pouvez sélectionner jusqu'à 4 répertoires dans lesquels vos fichiers de tables de finales sont stockés.
  Vous pouvez appuyer sur un bouton <b> ... </b> à droite d'une entrée pour choisir un
  fichier, pour spécifier que le répertoire de ce fichier doit être utilisé.
  </p>
  <p>
  Lorsqu'une position trouvée dans un fichier de base est atteinte, les informations du jeu
  (sous l'échiquier) affichera les informations sur la base de la table. Vous pouvez
  configurer la quantité d'informations affichées en cliquant avec le bouton droit de la souris
  dans cette zone ou en sélectionnant <b> Informations sur le jeu </b> dans le
  Menu <menu> Options </menu>. Sélection de l'option "résultat et meilleurs coups"
  donne les informations les plus utiles, mais est beaucoup plus lent que
  l'option "résultat uniquement".
  </p>
  
  <h3> La fenêtre Tablebase </h3>
  <p>
  Vous pouvez obtenir encore plus d'informations de base sur la position actuelle
  en ouvrant la <term> fenêtre Tablebase </term> (menu <menu> Windows </menu>,
  raccourci: Ctrl + Maj + =). Cette fenêtre montre le résultat avec un jeu parfait
  de tous les coups légaux de la position actuelle.
  </p>
  <p>
  La fenêtre a deux parties principales. Le cadre récapitulatif (à gauche) montre
  les bases de table Scid trouvées sur votre ordinateur et un résumé pour chacune
  des tables de finales. Le cadre de résultats (à droite) montre des résultats optimaux pour
  tout se déplace de la position actuelle affichée dans la fenêtre principale.
  </p>
  
  <h4> Le cadre récapitulatif </h4>
  <p>
  La partie supérieure du cadre de résumé vous permet de sélectionner une
  table de finales. Celles dont vous disposez sont affichées en bleu et indisponibles
  les bases de table sont affichées en gris, mais vous pouvez sélectionner n'importe quelle table de finales.
  La partie inférieure du cadre récapitulatif affiche des informations récapitulatives
  table sélectionnée. (Toutes les bases n'ont pas de résumé enregistré dans
  Scid encore.)
  </p>
  <p>
  Le résumé comprend la fréquence (combien de jeux par million atteignent un
  position avec ce matériau, calculé à partir d'une base de données de plus de
  600 000 jeux de niveau maître), le plus long compagnon de chaque côté, et le
  nombre de zugzwangs mutuels (ou "réciproques"). Un zugzwang mutuel est un
  position où le blanc pour se déplacer dessine et le noir pour se déplacer perd, ou où
  blanc pour se déplacer perd et noir pour se déplacer dessine, ou où quiconque se déplace
  perd.
  </p>
  <p>
  Pour certaines bases avec des zugzwangs mutuels, le résumé comprend également
  une liste de toutes les positions zugzwang ou une sélection d'entre elles. Plein
  liste pour chaque table de finales n'est pas possible car certaines bases de table ont
  des milliers de zugzwangs mutuels.
  </p>
  <p>
  Vous pouvez définir une position aléatoire à partir de la table de finales sélectionnée en appuyant sur
  le bouton <b> Aléatoire </b>.
  </p>
  
  <h4> Le cadre de résultats </h4>
  <p>
  Le cadre de résultats est mis à jour chaque fois que l'échiquier dans la fenêtre principale
  changements. La première ligne montre combien de coups gagnent (+), tirent (=), perdent (-),
  ou ont un résultat inconnu (?). Le reste du cadre donne une image plus détaillée
  liste des résultats, en les classant du plus petit au plus long, puis dessine,
  puis les pertes les plus longues aux plus courtes. Toutes les distances sont à mater.
  </p>
  
  <h4> Le tableau des résultats </h4>
  <p>
  Dans une position de table, il est souvent utile de connaître les résultats de la table
  serait si toutes les pièces dans la position actuelle étaient sur leurs
  cases actuelles mais une pièce particulière a été déplacée ailleurs.
  Par exemple, vous pouvez déterminer à quelle distance un roi doit être
  un pion passé pour gagner ou dessiner une position particulière. Dans les livres de fin de partie
  cette information est souvent appelée la <i> zone gagnante </i> ou
  <i> zone de dessin </i> d'une pièce dans une position.
  </p>
  <p>
  Vous pouvez trouver ces informations dans Scid en appuyant sur le bouton
  l'image d'un échiquier, pour afficher le <term> tableau des résultats </term> dans le
  fenêtre de table.
  Lorsque vous appuyez sur le bouton gauche de la souris sur n'importe quelle pièce de ce tableau, 
  un symbole est dessiné dans chaque case vide montrant ce que le résultat de la table de finales
  serait (avec le même côté à déplacer que la position actuelle de la fenêtre principale)
  si la pièce sélectionnée était sur cette case.
  </p>
  <p>
  Une case peut avoir cinq symboles différents:
  un <b>#</b> blanc signifie que les blancs gagnent;
  un <b>#</b> noir signifie que le noir gagne;
  un <b>=</b> bleu signifie que la position est dessinée;
  un <b>X</b> rouge signifie que la position est illégale (parce que les rois sont
  adjacent ou le côté à déplacer donne un échec); et
  un <b>?</b> rouge signifie que le résultat est inconnu car le nécessaire
  le fichier de table de finales n'est pas disponible.
  </p>
  
  <h3> Obtention de fichiers Tablebase </h3>
  <p>
  Consultez la section <a Author Related> liens connexes </a> pour obtenir de l'aide sur la recherche
  fichiers de table de finales sur Internet.
  </p>
  
  <p> <footer> (Mise à jour: Scid 3.6.2, décembre 2006) </footer> </p>
}

####################
### Bookmarks help:

set helpTitle(F,Bookmarks) "Signets"
set helpText(F,Bookmarks) {
<h1>Signets</h1>
  <p>
  Scid vous permet de <term>bookmark</term> jeux importants pour faciliter
  référence future. Le menu des signets est disponible à partir du
  <menu>Fichier</menu>, la barre d'outils, ou la touche de raccourci <b>Ctrl+B</b>.
  </p>
  <p>
  Lorsque vous sélectionnez un jeu dans le menu Signets, Scid
  ouvrir sa base de données si nécessaire, trouver cette partie, et se déplacer vers la partie
  la position à laquelle il a été mis en signet.
  </p>
  <p>
  Seulement les jeux dans une base de données au format Scid (pas un fichier <a PGN>PGN</a> ou la base de données des clips)
  peut être mis en signet.
  </p>
  <p>
  Si la base de données d'un jeu bookmarké est triée ou compactée, le bookmark
  les détails peuvent devenir obsolètes. Lorsque cela se produit, Scid va chercher dans le
  base de données pour le meilleur jeu correspondant (comparaison des noms des joueurs, du site, etc.)
  lorsque le signet est sélectionné, donc le jeu marqué devrait toujours être
  chargé. Cependant, si les détails du jeu marqué changent, il est possible
  qu'un jeu différent correspondra mieux aux détails des signets et sera
  chargé à la place. C'est donc une bonne idée de re-marquer un jeu si vous éditez
  ses joueurs, son site, son résultat, son cycle ou son année.
  </p>
  
  <h3>Edition des signets</h3>
  <p>
  Avec l'éditeur de signets, vous pouvez modifier le texte de menu affiché pour
  chaque jeu marqué d'un signet et ajouter des dossiers pour classer les signets.
  </p>
  
  <h3>Conseils</h3>
  <p>
  Vous pouvez utiliser des signets pour un accès rapide aux bases de données que vous utilisez souvent
  en mettant en signet un jeu de chaque base de données. Une autre bonne utilisation pour
  est d'ajouter les jeux importants que vous trouvez en étudiant un
  une ouverture d'échecs particulière.
  </p>
  <p>
  Le menu des signets contient une entrée pour contrôler l'affichage de
  dossiers de signets : ils peuvent être affichés sous forme de sous-menus (utile lorsqu'il y a
  de nombreux signets), ou comme une seule liste.
  </p>
  
  <p><footer>(Mise à jour : Scid 3.6.2, décembre 2006)</footer></p>
}

####################
### Command-line options help:

set helpTitle(F,Cmdline) "Options de ligne de commande"
set helpText(F,Cmdline) {
<h1>Options de ligne de commande</h1>
  <p>
  Lorsque vous démarrez Scid depuis un shell ou une console, vous pouvez spécifier
  des options de ligne de commande. Les bases de données au format Scid
  (avec ou sans suffixe comme ".si4") et les fichiers <a PGN>PGN</a> à ouvrir
  peuvent être donnés. Par exemple :
  <ul>
    <b>scid mybase newgames.pgn</b>
  </ul>
  démarrera Scid et ouvrira la base de données appelée "mybase" et le
  fichier PGN nommé "newgames.pgn".
  </p>
  <p>
  Il y a aussi des arguments optionnels pour contrôler quels fichiers Scid doit
  rechercher et utiliser quand il démarre :
  <ul>
    <li>l'option <b>-xtb</b> ou <b>-xt</b> désactive l'utilisation de
  <a TB>tables de finales</a>,</li>
    <li>l'option <b>-xeco</b> ou <b>-xe</b> évite de charger le fichier
      de <a ECO>classement des ouvertures ECO</a>,</li>
    <li>l'option <b>-xspell</b> ou <b>-xs</b> évite de charger le fichier de
      <a Maintenance Spellcheck>vérification orthographique</a>,</li>
    <li>l'option <b>-fast</b> ou <b>-f</b> fait les trois, 
      donc <b>scid -f</b> est équivalent à <b>scid -xeco -xspell -xtb</b></li>
  </ul>
  </p>

  <p>
  Enfin, un fichier filtre (.sso) peut être utilisé en ligne de commande.
  <ul>
  <b>scid mybase myfilter</b>
  </ul>
  ouvrira la base de données "mybase" et lancera immédiatement "myfilter" sur elle
  pour sélectionner des jeux. Ceci peut par exemple être utilisé pour sélectionner
  une liste de jeux inachevés dans un fichier PGN.
  </p>
  
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

####################
### Pgnscid help:

set helpTitle(F,Pgnscid) "Pgnscid"
set helpText(F,Pgnscid) {
<h1>Pgnscid</h1>
  <p>
  <term>Pgnscid</term> est le programme distinct que vous devez utiliser pour
  convertir les fichiers <a PGN> PGN </a> (notation de jeu portable) en bases de données Scid.
  </p>
  <p>
  Pour convertir un fichier nommé <i> monfichier.pgn </i>, tapez simplement:
  <ul>
  <li> <b> pgnscid myfile.pgn </b> </li>
  </ul>
  et la base de données scid (constituée de <i> myfile.si3 </i>, <i> myfile.sg3 </i>
  et <i> monfichier.sn3 </i>) seront créés.
  Toutes les erreurs ou avertissements seront consignés dans le fichier <i> monfichier.err </i>.
  </p>
  <p>
  Si vous souhaitez que la base de données soit créée dans un répertoire différent ou
  un nom différent, vous pouvez ajouter le nom de la base de données à la ligne de commande,
  par exemple:
  <ul>
  <li> <b> pgnscid myfile.pgn mybase </b> </li>
  </ul>
  va créer une base de données composée des fichiers <i> mybase.si3 </i>,
  <i> mybase.sg3 </i> et <i> mybase.sn3 </i>.
  </p>
  <p>
  Notez que pgnscid (et scid) peut lire les fichiers <a PGN> PGN </a> compressés
  (par exemple, <b> mybase.pgn.gz </b>)
  directement, donc si vous avez un gros fichier <a PGN> PGN </a> compressé avec Gzip pour enregistrer
  l'espace disque, vous n'avez pas besoin de le décompresser en premier.
  </p>
  
  <h3> Options </h3>
  <p>
  Il y a deux arguments facultatifs que pgnscid peut accepter avant le nom de fichier:
  <b> -f </b> et <b> -x </b>.
  </p>
  <p>
  L'option <b> -f </b> force l'écrasement d'une base de données existante; par
  par défaut, pgnscid ne sera pas converti en une base de données qui existe déjà.
  </p>
  <p>
  L'option <b> -x </b> fait que pgnscid ignore tout le texte entre les jeux.
  Par défaut, le texte entre les jeux est stocké en tant que commentaire d'avant-match du
  jeu qui suit. Cette option affecte uniquement le texte entre les jeux; la norme
  les commentaires à l'intérieur de chaque jeu sont toujours convertis et stockés.
  </p>
  
  <h3> Formatage des noms des joueurs </h3>
  <p>
  Pour réduire le nombre d'orthographes multiples de noms faisant référence à la
  même joueur, le formatage de base des noms de joueurs est effectué par pgnscid.
  Par exemple, le nombre d'espaces après chaque virgule est standardisé à un,
  tous les espaces au début et à la fin d'un nom sont supprimés et un point
  la fin d'un nom est supprimée.
  Les préfixes néerlandais tels que "van den" et "Van Der" sont également normalisés
  un V majuscule et un petit d.
  </p>
  <p>
  Vous pouvez modifier (et même vérifier l'orthographe) les noms des joueurs, des événements, des sites et des rondes dans
  Scid; consultez la page d'aide de <a Maintenance Editing> Maintenance </a> pour plus de détails.
  </p>
  
  <p> <footer> (Mise à jour: Scid 3.6.2, décembre 2006) </footer> </p>
}

####################
### File formats help:

set helpTitle(F,Formats) "Formats de fichiers Scid"
set helpText(F,Formats) {
<h1>Formats de fichiers Scid</h1>
  <p>
  Les bases de données Scid sont constituées de trois fichiers essentiels : un fichier Index, un fichier Name
  et un fichier Game. Tous ont un suffixe de trois lettres commençant par "s" :
  ".si3" pour les fichiers d'index, ".sn3" pour les fichiers de noms, et ".sg3" pour les fichiers de jeu.
  </p>
  
  <h3>Le fichier d'index (.si3) </h3>
  <p>
  Ce fichier contient une description de la base de données et un petit
  pour chaque jeu. La taille actuelle est de 41 octets par jeu.
  De ce nombre, environ 28 octets sont des informations essentielles telles que le résultat,
  date, nom du joueur/de l'événement/site numéros d'identification (les noms réels se trouvent dans la
  Nom du fichier), etc.
  </p>
  <p>
  Les 13 octets restants contiennent des informations redondantes mais utiles sur le
  jeu qui sert à accélérer les recherches de positions, de matériaux et de motifs.
  Voir la section <a Formats Fast>Recherches rapides</a> ci-dessous pour plus d'informations.
  </p>
  
  <h3>Le fichier de nom (.sn3)</h3>
  <p>
  Ce fichier contient tous les noms de joueurs, d'événements, de sites et de tours utilisés dans le
  base de données. Chaque nom n'est stocké qu'une seule fois, même s'il apparaît dans de nombreux jeux.
  Le fichier de noms est généralement le plus petit des trois fichiers essentiels d'un
  base de données.
  </p>

  <h3>Le jeu (.sg3) file</h3>
  <p>
  Ce fichier contient les coups réels, les variantes et les commentaires de chaque partie.
  Le format d'encodage des coups est très compact : la plupart des coups ne prennent qu'un octet
  de stockage.
  </p>
  <p>
  Lorsqu'un jeu est remplacé, sa nouvelle version est enregistrée dans le <i>fin</i> de
  le fichier du jeu, donc l'espace gaspillé peut s'accumuler au fil du temps. Vous pouvez restaurer
  une base de données à sa taille minimale par <a Compact>compacting</a>lui.
  </p>
  
  <h3>Autres fichiers Scid</h3>
  <p>
  Un fichier <a EPD>EPD</a> (suffixe : ".epd")
  contient un certain nombre de positions d'échecs, chacune avec un commentaire textuel.
  Le format de fichier EPD est décrit dans la norme <a Author Related>PGN</a>.
  </p>
  <p>
  Un fichier d'email (suffixe : " .sem ") pour une base de données stocke les détails des adversaires
  à qui vous envoyez des messages électroniques.
  </p>
  <p>
  Un fichier SearchOptions (suffixe : ".sso") contient Scid
  <a Searches Header>header</a> ou
  <a Searches Matériau>matériau/modèle</a> paramètres de recherche.
  </p>
  
  <h3><name Fast>Recherches rapides dans Scid</name></h3>
  <p>
  Comme mentionné ci-dessus, le fichier d'index stocke quelques données redondantes mais utiles
  des informations sur chaque jeu pour accélérer les recherches de positions ou de matériel.
  </p>
  <p>
  Par exemple, l'article de la position finale est enregistré. Si vous recherchez
  pour les terminaisons de tours et de pions, puis toutes les parties qui se terminent avec une dame, un fou
  ou cavalier sur le plateau (et n'ont pas de promotions de pions) sera rapidement
  a sauté.
  </p>
  <p>
  Un autre élément d'information utile est l'ordre dans lequel les pions
  quitter leurs cases d'origine (en se déplaçant ou en étant capturé). Ceci est utilisé
  pour accélérer la recherche d'arbres ou de positions exactes, notamment pour l'ouverture
  positions. Par exemple, lors de la recherche de la position de départ du
  La défense française (1.e4 e6), chaque partie commence avec 1.e4 c5, ou 1.d4, etc.
  sont ignorées, mais les parties commençant par 1.e4 e5 devront toujours être recherchées.
  </p>
  
  <p><footer>(Mise à jour : Scid 3.6.2, décembre 2006)</footer></p>
}

####################
### Options and Fonts help:

set helpTitle(F,Options) "Options et préférences"
set helpText(F,Options) {
<h1>Options et préférences</h1>

  <p>
  De nombreuses options et préférences Scid (telles que la taille du tableau, les couleurs, les polices,
  et paramètres par défaut) sont réglables dans le menu <menu> Options </menu>.
  Tout cela (et plus encore, comme le dernier répertoire à partir duquel vous avez chargé une base de données
  et les tailles de certaines fenêtres) sont enregistrées dans un fichier d'options lorsque
  vous sélectionnez <b> Enregistrer les options </b> dans le menu Options.
  Le fichier d'options est chargé à chaque démarrage de Scid.
  </p>
  <p>
  Si vous utilisez Windows, le fichier d'options est <b> scid.opt </b> dans le répertoire
  où se trouve le fichier de programme Scid <b> scid.exe </b>. Pour les utilisateurs d'Unix
  systèmes d'exploitation (tels que Solaris ou Linux) le fichier
  est <b> ~ / .scid / scidrc </b>.
  </p>
  
  <h3> <name MyPlayerNames> Définition des noms de vos joueurs </name> </h3>
  <p>
  Il peut y avoir un nom de joueur (ou plusieurs noms) pour qui, à chaque fois qu'une partie
  est chargé, vous souhaitez que l'échiquier de la fenêtre principale soit affiché à partir de
  la perspective de ce joueur. Vous pouvez configurer une liste de ces noms
  en utilisant <b> Mes noms de joueurs ... </b> dans les <menu> Options / Échiquier </menu>
  menu. Dans la boîte de dialogue qui apparaît, entrez un nom de joueur sur chaque ligne.
  Caractères génériques ("<b>? </b>" pour exactement un caractère et "<b> * </b>"
  pour une séquence de zéro ou plusieurs caractères) peut être utilisé.
  </p>
  
  <h3> <name Fonts> Définition des polices </name> </h3>
  <p>
  Scid a trois polices de base qu'il utilise dans la plupart de ses fenêtres, et vous pouvez
  personnalisez-les tous. Ils sont appelés <b> réguliers </b>, <b> petits </b> et
  <b> corrigé </b>.
  </p>
  <p>
  La police fixe doit être une police à largeur fixe (non proportionnelle). C'est utilisé
  pour les fenêtres <a Tree> </a> et <a Crosstable>Classement de tournoi</a>.
  </p>
  
  <p> <footer> (Mise à jour: Scid 3.6.2, décembre 2006) </footer> </p>
}

####################
### NAG values help:

set helpTitle(F,NAGs) "Valeurs standards NAG"
set helpText(F,NAGs) {
<h1>Valeurs standards NAG</h1>
  <p>
  Les valeurs NAG (Numeric Annotation Glyph) standard définies dans la
  <a Author Related>norme PGN</a> sont :
  </p>
  <cyan>
  <ul>
  <li> 1 Bon coup                                                                : !            </li>
  <li> 2 Mauvais coup                                                            : ?            </li>
  <li> 3 Excellent coup                                                          : !!           </li>
  <li> 4 Très mauvais coup                                                       : ??           </li>
  <li> 5 Coup intéressant                                                        : !?           </li>
  <li> 6 Coup douteux                                                            : ?!           </li>
  <li> 7 Coup forcé (tous les autres coups perdent rapidement)                   : forcé        </li>
  <li> 8 Seul coup (pas d'alternatives raisonnable)                              : □            </li>
  <li> 9 Le plus mauvais coup                                                    : le pire      </li>
  <li> 10 Position égale                                                         : =            </li>
  <li> 11 Chances égales, position équilibrée                                    : =, équilibrée</li>
  <li> 12 Chances égales, position dynamique                                     : ↹            </li>
  <li> 13 Position peu claire                                                    : ~            </li>
  <li> 14 Les Blancs ont un léger avantage                                       : +=           </li>
  <li> 15 Les Noirs ont un léger avantage                                        : =+           </li>
  <li> 16 Les Blancs ont un avantage modéré                                      : +/-          </li>
  <li> 17 Les Noirs ont un avantage modéré                                       : -/+          </li>
  <li> 18 Les Blancs ont un avantage décisif                                     : +-           </li>
  <li> 19 Les Noirs ont un avantage décisif                                      : -+           </li>
  <li> 20 Les Blancs ont un avantage écrasant                                    : +--          </li>
  <li> 21 Les Noirs ont un avantage écrasant                                     : --+          </li>
  <li> 22 Les Blancs sont en zugzwang                                            : ⊙            </li>
  <li> 23 Les Noirs sont en zugzwang                                             : ⊙            </li>
  <li> 24 Les Blancs ont un léger avantage d'espace                              : ◯            </li>
  <li> 25 Les Noirs ont un léger avantage d'espace                               : ◯            </li>
  <li> 26 Les Blancs ont un avantage d'espace modéré                             : ◯◯           </li>
  <li> 27 Les Noirs ont un avantage d'espace modéré                              : ◯◯           </li>
  <li> 28 Les Blancs ont un avantage spatial décisif                             : ◯◯◯          </li>
  <li> 29 Les Noirs ont un avantage spatial décisif                              : ◯◯◯          </li>
  <li> 30 Les Blancs ont un léger avantage de développement                      : ↻            </li>
  <li> 31 Les Noirs ont un léger avantage de développement                       : ↺            </li>
  <li> 32 Les Blancs ont un avantage de développement modéré                     : ↻↻           </li>
  <li> 33 Les Noirs ont un avantage de développement modéré                      : ↺↺           </li>
  <li> 34 Les Blancs ont un avantage de développement décisif                    : ↻↻↻          </li>
  <li> 35 Les Noirs ont un avantage de développement décisif                     : ↺↺↺          </li>
  <li> 36 Les Blancs ont l'initiative                                            : ↑            </li>
  <li> 37 Les Noirs ont l'initiative                                             : ↓            </li>
  <li> 38 Les Blancs ont une initiative durable                                  : ⇑            </li>
  <li> 39 Les Noirs ont une initiative durable                                   : ⇓            </li>
  <li> 40 Les Blancs sont à l'attaque                                            : →            </li>
  <li> 41 Les Noirs sont à l'attaque                                             : ←            </li>
  <li> 42 Les Blancs ont une compensation insuffisante pour le matériel sacrifié : &/-      </li>
  <li> 43 Les Noirs ont une compensation insuffisante pour le matériel sacrifié  : &/+      </li>
  <li> 44 Les Blancs ont une compensation suffisante pour le matériel sacrifié   : =/&      </li>
  <li> 45 Les Noirs ont une compensation suffisante pour le matériel sacrifié    : =/&      </li>
  <li> 46 Les Blancs ont une nette compensation pour le matériel sacrifié        : +/&      </li>
  <li> 47 Les Noirs ont une nette compensation pour le matériel sacrifié         : -/&      </li>
  <li> 48 Les Blancs ont un léger avantage au centre                             : ⊞            </li>
  <li> 49 Les Noirs ont un léger avantage au centre                              : ⊞            </li>
  <li> 50 Les Blancs ont un avantage modéré au centre                            : ⊞⊞           </li>
  <li> 51 Les Noirs ont un avantage modéré au centre                             : ⊞⊞           </li>
  <li> 52 Les Blancs ont un avantage décisif au centre                           : ⊞⊞⊞          </li>
  <li> 53 Les Noirs ont un avantage décisif au centre                            : ⊞⊞⊞          </li>
  <li> 54 Les Blancs ont un léger avantage à l'aile-roi                          : ⟩            </li>
  <li> 55 Les Noirs ont un léger avantage à l'aile-roi                           : ⟩            </li>
  <li> 56 Les Blancs ont un avantage modéré à l'aile-roi                         : ⟫            </li>
  <li> 57 Les Noirs ont un avantage modéré à l'aile-roi                          : ⟫            </li>
  <li> 58 Les Blancs ont un avantage décisif à l'aile-roi                        : ⋙            </li>
  <li> 59 Les Noirs ont un avantage décisif à l'aile-roi                         : ⋙            </li>
  <li> 60 Les Blancs ont un léger avantage à l'aile-dame                         : ⟨            </li>
  <li> 61 Les Noirs ont un léger avantage à l'aile-dame                          : ⟨            </li>
  <li> 62 Les Blancs ont un avantage modéré à l'aile-dame                        : ⟪            </li>
  <li> 63 Les Noirs ont un avantage modéré à l'aile-dame                         : ⟪            </li>
  <li> 64 Les Blancs ont un avantage décisif à l'aile-dame                       : ⋘            </li>
  <li> 65 Les Noirs ont un avantage décisif à l'aile-dame                        : ⋘            </li>
  <li> 66 les Blancs sont vulnérables sur la première rangée                     :              </li>
  <li> 67 Les Noirs sont vulnérables sur la première rangée                      :              </li>
  <li> 68 La première rangée des Blancs est bien protégée                        :              </li>
  <li> 69 La première rangée des Noirs est bien protégée                         :              </li>
  <li> 70 Le roi blanc n'est pas bien protégé                                    :              </li>
  <li> 71 Le roi noir n'est pas bien protégé                                     :              </li>
  <li> 72 Le roi blanc est bien protégé                                          :              </li>
  <li> 73 Le roi noir est bien protégé                                           :              </li>
  <li> 74 Le roi blanc est mal placé                                             :              </li>
  <li> 75 Le roi noir est mal placé                                              :              </li>
  <li> 76 Le roi blanc est ben placé                                             :              </li>
  <li> 77 Le roi noir est bien placé                                             :              </li>
  <li> 78 Les Blancs ont une structure de pions très faible                      :              </li>
  <li> 79 Les Noirs ont une structure de pions très faible                       :              </li>
  <li> 80 Les Blancs ont une structure de pions relativement faible              :              </li>
  <li> 81 Les Noirs ont une structure de pions relativement faible               :              </li>
  <li> 82 Les Blancs ont une structure de pions assez forte                      :              </li>
  <li> 83 Les Noirs ont une structure de pions assez forte                       :              </li>
  <li> 84 Les Blancs ont une structure de pions très solide                      :              </li>
  <li> 85 Les Noirs ont une structure de pions très solide                       :              </li>
  <li> 86 Le(s) cavalier(s) blanc(s) est(sont) mal placé(s)                      :              </li>
  <li> 87 Le(s) cavalier(s) noir(s) est(sont) mal placé(s)                       :              </li>
  <li> 88 Le(s) cavalier(s) blanc(s) est(sont) bien placé(s)                     :              </li>
  <li> 89 Le(s) cavalier(s) noir(s) est(sont) bien placé(s)                      :              </li>
  <li> 90 Le(s) fou(s) blanc(s) est(sont) mal placé(s)                           :              </li>
  <li> 91 Le(s) fou(s) noir(s) est(sont) mal placé(s)                            :              </li>
  <li> 92 Le(s) fou(s) blanc(s) est(sont) bien placé(s)                          : ↗            </li>
  <li> 93 Le(s) fou(s) noir(s) est(sont) bien placé(s)                           : ↖            </li>
  <li> 94 La(les) tour(s) blanche(s) est(sont) mal placée(s)                     :              </li>
  <li> 95 La(les) tour(s) noire(s) est(sont) mal placée(s)                       :              </li>
  <li> 96 La(les) tour(s) blanche(s) est(sont) bien placée(s)                    : ⇈            </li>
  <li> 97 La(les) tour(s) noire(s) est(sont) bien placée(s)                      : ⇊            </li>
  <li> 98 La dame blanche est mal placée                                         :              </li>
  <li> 99 La dame noire est mal placée                                           :              </li>
  <li>100 La dame blanche est bien placée                                        :              </li>
  <li>101 La dame noire est bien placée                                          :              </li>
  <li>102 Les pièces blanches sont mal coordonnées                               :              </li>
  <li>103 Les pièces noires sont mal coordonnées                                 :              </li>
  <li>104 Les pièces blanches sont bien coordonnées                              :              </li>
  <li>105 Les pièces noires sont bien coordonnées                                :              </li>
  <li>106 Les Blancs ont très mal joué l'ouverture                               :              </li>
  <li>107 Les Noirs ont très mal joué l'ouverture                                :              </li>
  <li>108 Les Blancs ont mal joué l'ouverture                                    :              </li>
  <li>109 Les Noirs ont mal joué l'ouverture                                     :              </li>
  <li>110 Les Blancs ont bien joué l'ouverture                                   :              </li>
  <li>111 Les Noirs ont bien joué l'ouverture                                    :              </li>
  <li>112 Les Blancs ont très bien joué l'ouverture                              :              </li>
  <li>113 Les Noirs ont très bien joué l'ouverture                               :              </li>
  <li>114 Les Blancs ont très mal joué le milieu de partie                       :              </li>
  <li>115 Les Noirs ont très mal joué le milieu de partie                        :              </li>
  <li>116 Les Blancs ont mal joué le milieu de partie                            :              </li>
  <li>117 Les Noirs ont mal joué le milieu de partie                             :              </li>
  <li>118 Les Blancs ont bien joué le milieu de partie                           :              </li>
  <li>119 Les Noirs ont bien joué le milieu de partie                            :              </li>
  <li>120 Les Blancs ont très bien joué le milieu de partie                      :              </li>
  <li>121 Les Noirs ont très bien joué le milieu de partie                       :              </li>
  <li>122 Les Blancs ont très mal joué la fin de partie                          :              </li>
  <li>123 Les Noirs ont très mal joué la fin de partie                           :              </li>
  <li>124 Les Blancs ont mal joué la fin de partie                               :              </li>
  <li>125 Les Noirs ont mal joué la fin de partie                                :              </li>
  <li>126 Les Blancs ont bien joué la fin de partie                              :              </li>
  <li>127 Les Noirs ont bien joué la fin de partie                               :              </li>
  <li>128 Les Blancs ont très bien joué la fin de partie                         :              </li>
  <li>129 Les Noirs ont très bien joué la fin de partie                          :              </li>
  <li>130 Les Blancs ont un léger contre-jeu                                     : ⇄            </li>
  <li>131 Les Noirs ont un léger contre-jeu                                      : ⇆            </li>
  <li>132 Les Blancs ont un contre-jeu modéré                                    : ⇄⇄           </li>
  <li>133 Les Noirs ont un contre-jeu modéré                                     : ⇆⇆           </li>
  <li>134 Les Blancs ont un contre-jeu décisif                                   : ⇄⇄⇄          </li>
  <li>135 Les Noirs ont un contre-jeu décisif                                    : ⇆⇆⇆          </li>
  <li>136 Les Blancs sont pressés par le temps                                   : ⊕            </li>
  <li>137 Les Noirs sont pressés par le temps                                    : ⊖            </li>
  <li>138 Les Blancs sont sévèrement pressés par le temps                        : ⊕⊕           </li>
  <li>139 Les Noirs sont sévèrement pressés par le temps                         : ⊖⊖           </li>
  </ul>
  </cyan>
  
  <p>
  Les autres valeurs NAG proposées pour les publications de Chess Informant sont :
  </p>
  <cyan>
  <ul>
  <li>140 Avec l'idée ...                  : △  </li>
  <li>141 Visé contre ...                  :    </li>
  <li>142 Meilleur coup                    : ⌓  </li>
  <li>143 Pire coup                        :    </li>
  <li>144 Coup équivalent                  : R  </li>
  <li>145 Remarque de l'éditeur            : RR </li>
  <li>146 Nouveauté                        : N  </li>
  <li>147 Point faible                     : ×  </li>
  <li>148 Finale                           : ⊥  </li>
  <li>149 Ligne                            : ⟺  </li>
  <li>150 Diagonale                        : ⇗  </li>
  <li>151 Les Blancs ont une paire de fous : ◫  </li>
  <li>152 Les Noirs ont une paire de fous  :    </li>
  <li>153 Fous de couleur opposée          : ◨  </li>
  <li>154 Fous de même couleur             : ⊶  </li>
  </ul>
  </cyan>
  
  <p>
  D'autres valeurs sont suggérées :
  </p>
  <cyan>
  <ul>
  <li>190 Etc.            : ǁ  </li>
  <li>191 Pions doublés   : ⡁  </li>
  <li>192 Pion isolé      : ⚯ </li>
  <li>193 Pions connectés : ⚮ </li>
  <li>194 Pions pendants  :    </li>
  <li>195 Pion arriéré    :    </li>
  </ul>
  </cyan>
  
  <p>
  Les symboles définis par Scid pour son propre usage sont :
  </p>
  <cyan>
  <ul>
  <li>201 Diagramme             : D ou #</li>
  <li>210 voir                  : —     </li>
  <li>211 Échec et mat          : #     </li>
  <li>212 Pion passé            : ⚨     </li>
  <li>213 Pions supplémentaires :       </li>
  <li>214 avec                  : ⌊     </li>
  <li>215 sans                  : ⌋     </li>
  </ul>
  </cyan>
  
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}


set helpTitle(F,ECO) "Classement des ouvertures selon l'ECO"
set helpText(F,ECO) {
<h1>Classement des ouvertures selon l'ECO</h1>
  <p>
  Scid peut classer les parties d'échecs selon la <term>classification ECO</term>
  (Encyclopédie des Ouvertures d'Échecs). Un code ECO standard se compose d'une lettre
  (A..E) suivie de deux chiffres. Il existe donc 500 codes ECO standard distincts.
  </p>
  
  <h3>Extensions Scid du système ECO</h3>
  <p>
  Le système ECO est très limité et ne suffit pas pour les parties d'échecs modernes.
  Certains des 500 codes ne sont presque plus jamais rencontrés, tandis que d'autres
  le sont très fréquemment. Pour améliorer cette situation, Scid permet une
  extension aux codes ECO de base : chaque code peut être étendu avec une
  lettre (a..z). Une extension supplémentaire (un autre chiffre, 1..4) est aussi
  possible mais pas encore utilisé dans le fichier standard Scid ECO.
  Ainsi, un code Scid ECO étendu ressemble à "<b>A41e</b>" ou "<b>E99b2</b>".
  Plusieurs des codes ECO les plus communs trouvés dans les jeux modernes de niveau
  Maître sont définies dans le fichier Scid ECO.
  </p>
  
  <h3><name Browser>Fenêtre Navigateur ECO</name></h3>
  <p>
  La fenêtre <term>Navigateur ECO</term> montre les positions qui sont
  utilisées pour classer chaque code ECO, ainsi que la fréquence et la performance des
  codes ECO dans la base de données active.
  </p>
  <p>
  Le volet supérieur indique la fréquence de chaque code ECO dans la
  base de données actuelle. Les barres du graphique ont trois sections : la partie basse
  (couleur la plus claire) est le nombre de Blancs qui gagnent, la partie au milieu est
  le nombre de nuls, et la partie la plus haute (la plus sombre) est le nombre de victoires
  des Noirs. Cela vous permet de voir d'un coup d'œil les caractéristiques
  d'une ouverture : par exemple, si les Blancs marquent très bien,
  ou si les parties nulles sont très fréquentes.
  </p>

  <p>
  Pour <b>aller à un niveau ECO inférieur</b> plus détaillé, cliquez avec le bouton gauche
  de la souris sur une barre dans le graphique (ou tapez la lettre ou le chiffre auquel il
  correspond). Pour <b>revenir au niveau ECO supérieur</b>, cliquez avec le bouton droit de
  la souris n'importe où dans le graphique, ou selon votre système, appuyez sur la touche
  Gauche, Supprimer ou Revenir en arrière.
  </p>
  <p>
  Le volet inférieur indique les positions qui composent un code ECO particulier,
  selon le fichier ECO que vous avez chargé.
  </p>
  
  <h3>Chargement du fichier Scid ECO</h3>
  <p>
  Le fichier ECO fourni avec Scid s'appelle <b>scid.eco</b>, et Scid essaie de le charger
  au démarrage. Si Scid ne le trouve pas, vous devrez faire ce qui suit pour activer la
  classification ECO :
  <ul>
  <li>(a) Dans Scid, utilisez le menu
  <menu>Options</menu><menu>Charger le fichier ECO...</menu> et sélectionnez
  le fichier <b>scid.eco</b>.</li>
  <li>(b) Sauvegardez le chemin et le nom du fichier ECO choisi avec le menu <menu>Options</menu><menu>Enregistrer les options</menu>.</li>
  </ul>
  Après cela, ce fichier ECO sera chargé à chaque fois que vous lancerez Scid.
  </p>
  
  <h3><name Codes>Système de code ECO</name></h3>
  <p>
  La structure de base du système ECO est la suivante :
  </p>
  <p>
  <b><blue><run ::windows::eco::Refresh A>A</run></blue></b>
  1.d4 Cf6 2... ; 1.d4 ... ; 1.c4 ; 1.divers
  <ul>
  <li><b>A00 à A09</b> 1.tout sauf 1. e4 ; 1. d4 et 1. c4
  (<b>A01</b> 1.b3 : <i>Ouverture Larsen</i>;
  <b>A02-A03</b> 1.f4 : <i>Ouverture Bird</i>;
  <b>A04-A09</b> 1.Cf3 : <i>Zukertort-Réti, Attaque est-indienne</i>) </li>
  <li><b>A1</b> 1.c4 ... : <i>Anglaise</i></li>
  <li><b>A2</b> 1.c4 e5 : <i>Anglaise, sicilienne inversée</i></li>
  <li><b>A3</b> 1.c4 c5 : <i>Anglaise, symétrique </i> </li>
  <li><b>A4</b> 1.d4 ... : <i>Ouverture du pion dame</i></li>
  <li><b>A5</b> 1.d4 Cf6 2.c4 .. : <i>Défense indienne </i></li>
  <li><b>A6</b> 1.d4 Cf6 2.c4 c5 3.d5 e6 : <i>Défense Benoni moderne</i></li>
  <li><b>A7</b> A6 + 4.Cc3 exd5 5.cxd5 d6 6.e4 g6 7.Cf3 </li>
  <li><b>A8</b> 1.d4 f5 : <i>Défense hollandaise</i></li>
  <li><b>A9</b> 1.d4 f5 2.c4 e6 : <i>Défense hollandaise</i></li>
  </ul>
  </p>
  
  <p>
  <b><blue><run ::windows::eco::Refresh B>B</run></blue></b>
  1.e4 c5; 1.e4 c6; 1.e4 d6; 1.e4 <i>divers</i>
  <ul>
  <li><b>B0</b>  1.e4 ...
  (<b>B02-B05</b> 1.e4 Cf6: Défense Alekhine;
  <b>B07-B09</b> 1.e4 d6: <i>Pirc</i>) </li>
  <li><b>B1</b>  1.e4 c6: <i>Défense Caro-Kann</i></li>
  <li><b>B2</b>  1.e4 c5: <i>Défense sicilienne</i></li>
  <li><b>B3</b>  1.e4 c5 2.Cf3 Cc6: <i>Défense sicilienne</i></li>
  <li><b>B4</b>  1.e4 c5 2.Cf3 e6: <i>Défense sicilienne, variante Paulsen</i></li>
  <li><b>B5</b>  1.e4 c5 2.Cf3 d6: <i>Défense sicilienne</i></li>
  <li><b>B6</b>  B5 + 3.d4 cxd4 4.Cxd4 Cf6 5.Cc3 Cc6 </li>
  <li><b>B7</b>  B5 + 4.Cxd4 Cf6 5.Cc3 g6: <i>Défense sicilienne, variante du dragon</i></li>
  <li><b>B8</b>  B5 + 4.Cxd4 Cf6 5.Cc3 e6: <i>Défense sicilienne, variante de Scheveningen</i></li>
  <li><b>B9</b>  B5 + 4.Cxd4 Cf6 5.Cc3 a6: <i>Défense sicilienne, variante Najdorf</i></li>
  </ul>
  </p>
  
  <p>
  <b><blue><run ::windows::eco::Refresh C>C</run></blue></b>
  1.e4 e5; 1.e4 e6
  <ul>
  <li><b>C0</b>  1.e4 e6: <i>Défense française</i></li>
  <li><b>C1</b>  1.e4 e6 2.d4 d5 3.Cc3: <i>Défense française, Winawer ou classique</i></li>
  <li><b>C2</b>  1.e4 e5: <i>Début ouvert</i></li>
  <li><b>C3</b>  1.e4 e5 2.f4: <i>Gambit du roi</i></li>
  <li><b>C4</b>  1.e4 e5 2.Cf3: <i>Partie ouverte</i></li>
  <li><b>C5</b>  1.e4 e5 2.Cf3 Cc6 3.Fc4: <i>Partie italienne; défense des deux cavaliers</i></li>
  <li><b>C6</b>  1.e4 e5 2.Cf3 Cc6 3.Fb5: <i>Partie espagnole (Ruy Lopez)</i></li>
  <li><b>C7</b>  1.e4 e5 2.Cf3 Cc6 3.Fb5 a6 4.Fa4: <i>Partie espagnole</i></li>
  <li><b>C8</b>  C7 + 4...Cf6 5.O-O: <i>Partie espagnole, ouvert et fermé</i>
  (<b>C80-C83</b>  5.O-O Cxe4: <i>Partie espagnole, variante ouverte</i>;
  <b>C84-C89</b>  5.O-O Fe7: <i>Partie espagnole, système fermé</i>) </li>
  <li><b>C9</b>  C8 + 5...Fe7 6.Re1 b5 7.Fb3 d6: <i>Partie espagnole, système fermé</i></li>
  </ul>
  </p>
  
  <p>
  <b><blue><run ::windows::eco::Refresh D>D</run></blue></b>
  1.d4 d5; 1.d4 Cf6 2.c4 g6 avec 3...d5
  <ul>
  <li><b>D0</b>  1.d4 d5: <i>Début fermé</i></li>
  <li><b>D1</b>  1.d4 d5 2.c4 c6: <i>Défense slave</i></li>
  <li><b>D2</b>  1.d4 d5 2.c4 dxc4: <i>Gambit dame accepté (GDA)</i></li>
  <li><b>D3</b>  1.d4 d5 2.c4 e6: <i>Gambit dame refusé (GDR)</i></li>
  <li><b>D4</b>  D3 + 3.Cc3 Cf6 4.Cf3 c5/c6: <i>Défense semi-Tarrasch; semi-slave</i></li>
  <li><b>D5</b>  D3 + 3.Cc3 Cf6 4.Fg5: <i>GDR classique</i></li>
  <li><b>D6</b>  D5 + 4...Fe7 5.e3 O-O 6.Cf3 Cbd7: <i>GDR, défense orthodoxe</i></li>
  <li><b>D7</b>  1.d4 Cf6 2.c4 g6 with 3...d5: <i>Défense Grünfeld</i></li>
  <li><b>D8</b>  1.d4 Cf6 2.c4 g6 3.Cc3 d5: <i>Défense Grünfeld</i></li>
  <li><b>D9</b>  1.d4 Cf6 2.c4 g6 3.Cc3 d5 4.Cf3: <i>Défense Grünfeld, variante des trois cavaliers</i></li>
  </ul>
  </p>
  
  <p>
  <b><blue><run ::windows::eco::Refresh E>E</run></blue></b>
  1.d4 Cf6 2.c4 e6; 1.d4 Cf6 2.c4 g6
  <ul>
  <li><b>E0</b>  1.d4 Cf6 2.c4 e6: <i>Ouverture catalane, partie catalane</i></li>
  <li><b>E1</b>  1.d4 Cf6 2.c4 e6 3.Cf3 (b6): <i>Défenses bogo-indienne et ouest-indienne</i></li>
  <li><b>E2</b>  1.d4 Cf6 2.c4 e6 3.Cc3 (Fb4): <i>Défense nimzo-indienne, etc</i></li>
  <li><b>E3</b>  E2 + 4.Fg5 ou 4.Dc2: <i>Défense nimzo-indienne, variante de Léningrad, variante classique</i></li>
  <li><b>E4</b>  E2 + 4.e3: <i>Défense nimzo-indienne, variante Rubinstein</i></li>
  <li><b>E5</b>  E4 + 4...O-O 5.Cf3: <i>Défense nimzo-indienne, ligne principale</i></li>
  <li><b>E6</b>  1.d4 Cf6 2.c4 g6: <i>Défense est-indienne</i></li>
  <li><b>E7</b>  1.d4 Cf6 2.c4 g6 3.Cc3 Fg7 4.e4: <i>Défense est-indienne</i></li>
  <li><b>E8</b>  E7 + 4...d6 5.f3: <i>Défense est-indienne, variante Sämisch</i></li>
  <li><b>E9</b>  E7 + 4...d6 5.Cf3: <i>Défense est-indienne, variante classique</i></li>
  </ul>
  </p>
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}


set helpTitle(F,Author) "Contact et Information"
set helpText(F,Author) {
<h1>Contact et Information</h1>
  <p>
  La page Web de Scid se trouve à l'adresse suivante <br>
  <b><url http://scid.sourceforge.net/>http://scid.sourceforge.net/</url></b>
  </p>
  <p>
  La dernière version de Scid et d'autres ressources
  y sont disponibles pour le téléchargement.
  </p>
  <p>
  Veuillez envoyer vos commentaires, questions, suggestions ou rapports de bogue à
  la liste de diffusion de Scid <b>scid-users@lists.sourceforge.net</b>.
  </p>
  
  <h3><name Related>Liens connexes</name></h3>
  <p>
  Si vous aimez utiliser Scid, vous trouverez peut-être les sites Web suivants intéressants :
  </p>
  <ul>
  <li><url http://scid.sourceforge.net/>http://scid.sourceforge.net</url> --
  Le programme sur lequel Scid est fortement basé. Scid a été écrit par
  Shane Hudson, avec l'aide d'autres personnes.</li>
  <li><url http://www.tim-mann.org/chess.html>http://www.tim_mann.org/chess.html</url> --
  Page d'accueil du programme de Tim Mann <b>XBoard et WinBoard</b> qui est un lecteur
  de fichier PGN et une interface pour les serveurs d'échecs sur Internet. Son site a
  aussi de bonnes informations sur Crafty, GNUchess et d'autres programmes d'échecs
  libres.</li>
  
  <li><url http://theweekinchess.com/twic>http://theweekinchess.com/twic</url> --
  TWIC (la semaine aux échecs), un excellent bulletin hebdomadaire de
  tournois d'échecs internationaux disponibles en format PGN.</li>
  
  <li><url http://www.saremba.de/chessgml/standards/pgn/pgn-complete.htm>http://www.saremba.de/chessgml/standards/pgn/pgn-complete.htm</url> --
  le <b>Standard PGN</b>, créé par Steven J. Edwards en 1994.
  Ce fichier explique en détail les formats <a PGN>PGN</a> et
  <a EPD>EPD</a>.</li>
  </ul>
  
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

# Book window help

set helpTitle(F,Book) "Fenêtre Bibliothèque d'ouvertures"
set helpText(F,Book) {
<h1>Fenêtre Bibliothèque d'ouvertures</h1>
  <p>
  Une <term>bibliothèque d'ouvertures</term> est un fichier contenant des lignes de jeu
  qu'un moteur d'échec peut utiliser pour les débuts de partie. Le format est celui
  utilisé par PolyGlot et les moteurs comme Fruit ou Toga, avec l'extension .bin.
  Pour construire pour de nouvelles bibliothèques, il faut utiliser
  <term>polyglot</term> en ligne de commande. Pour cela, veuillez vous référer à la
  documentation sur polyglot.
  </p>
  <p>
  La <term>bibliothèque active</term> peut être sélectionnée à partir de la liste
  déroulante en haut de la fenêtre. Pour spécifier le répertoire où Scid
  doit chercher les bibliothèques d'ouvertures, sélectionner le menu
  <menu>Options</menu><menu>Répertoire des bibliothèques...</menu>.
  </p>
  <p>
  Pour chaque position, la fenêtre affiche <b>tous les coups possibles</b>
  de la bibliothèque et leurs <b>poids relatifs en pourcentage</b>. Ces données peuvent
  être modifiées par le menu <menu>Outils</menu><menu>Config. bibliothèque...</menu>.
  </p>
  <p>
  <b>Note.</b> N'oubliez pas de sauvegarder avec le menu
  <menu>Options</menu><menu>Enregistrer les options</menu>.
  </p>
  <p>
  <term>Cliquer sur un coup dans la bibliothèque</term> jouera ce coup dans la position
  actuelle de l'échiquier. Lorsqu'un coup est joué, ces informations sont mises à jour.
  </p>
  <p>
  En bas de la fenêtre, le bouton <term>Bibliothèque de l'adversaire</term> affiche les
  coups qui ne sont pas présentes dans la bibliothèque mais qui font partie de lignes
  de jeu qui sont présentes dans le répertoire du moteur d'échecs.
  </p>
  <p>
  Cela permet à un moteur de jouer les meilleurs coups, et d'avoir des réponses si
  l'adversaire choisit des coups qui ne sont pas dans la bibliothèque. L'affichage de
  ces <i>autres coups</i> montre où il y a des manques dans une bibliothèque.
  </p>
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

# Tactical game window help

set helpTitle(F,TacticalGame) "Jouer une partie tactique"
set helpText(F,TacticalGame) {
<h1>Jouer une partie tactique</h1>
  <p>
  Une partie tactique se joue contre un moteur simulant un adversaire avec un
  niveau donné de classement Elo. L'adversaire est joué par Phalanx et l'entraîneur
  par Toga ou un moteur UCI. Par défaut à l'installation de Scid, les bons moteurs sont
  automatiquement sélectionnés par leur noms et doivent être correctement configurés.
  </p>
  <p>
  <b>Notez</b> qu'une <term>version spéciale du moteur Phalanx</term> est
  obligatoire. Cette version spéciale est livrée avec le package Scid.
  </p>
  <h3>Configurer une partie tactique avec entraîneur</h3>
  <p>
  Les paramètres suivants doivent être configurés :
  <ul>
  <li><term>Niveau fixe</term> définit un classement Elo fixe pour l'adversaire.</li>
  <li><term>Niveau aléatoire</term> choisit un niveau aléatoire entre les
  niveaux minimum et maximum spécifiés par les curseurs gauche et droit
  respectivement.</li>
  <li><term>Ouverture</term>
  <ul>
     <li><term>Nouvelle partie</term> démarre un nouveau jeu en choisissant un
     ouverture aléatoire.</li>
     <li><term>Jouer depuis la position actuelle</term> laisse le jeu commencer
     avec la position actuelle de l'échiquier.</li>
     <li><term>Ouverture spécifique</term> : l'adversaire jouera une ouverture
      spécifique, qui peut être choisie dans la liste en-dessous. C'est
     utile pour l'entraînement aux ouvertures.</li>
  </ul>
  </li>
  <li><term>Limiter le temps d'analyse</term> permet de limiter le temps utilisé par
  le <term>moteur d'échecs entraîneur</term> pour vérifier les coups des joueurs et
  trouver les erreurs. Si ce temps n'est pas limité, l'entraîneur est autorisé à
  réfléchir en arrière-plan.</li>
  </ul>
  Pour démarrer une partie tactique, appuyez sur le bouton <b>[Jouer]</b>.
  </p>
  <p><b>Astuce</b> : si vous activez le <a Graphs>graphique des scores</a>, vous
  pouvez voir l'utilisation du temps et l'évaluation de la position pendant le jeu.</p>

  <p><b>Remarque.</b> Une fonction similaire peut être obtenue en
  <a SeriousGame>jouant une partie sérieuse</a> contre tout moteur UCI qui
  autorise à limiter sa force.
  </p>
  <p><footer>(Mise à jour: Scid 4.7.1, janvier 2020)</footer></p>
}

# Opening Trainer window help

set helpTitle(F,OpeningTrainer) "Entraînement aux ouvertures"
set helpText(F,OpeningTrainer) {
<h1>Entraînement aux ouvertures</h1>
  <p>
  Une base de données Scid peut être utilisée comme répertoire, chaque jeu représentant
  une première ligne. Les jeux peuvent contenir des variantes et peuvent également
  contiennent des valeurs NAG pour spécifier des valeurs pour chaque ligne, et donc
  permettant de marquer des lignes préférées ou douteuses. Pour être une base de données valide
  pour le formateur d'ouverture, il doit également être de type approprié.
  Autrement dit, si les ouvertures pour le blanc doivent être formés, la base de données a
  être de type <term> Ouvertures pour Blanc </term> (similaire pour Noir et
  les deux couleurs). Le type peut être défini via la <a Maintenance> Maintenance
  fenêtre </a> en sélectionnant l'icône ou via les sélecteurs de base de données
  menu contextuel.
  </p>
  <p>
  <h2> Création d'une base de répertoire </h2>
  Avant d'exécuter le formateur d'ouverture, veuillez noter que l'ensemble
  la base de données du répertoire sera chargée dans le formateur. Pour ça
  C'est pourquoi il est bon d'utiliser une base de données plus petite. C'est une bonne idée de
  assembler d'abord une grande base de données de jeux de qualité, puis filtrer
  base de données pour une ouverture spécifique (par exemple le Ruy Lopez). Ces filtré
  les jeux pourraient ensuite être copiés dans une nouvelle base de données pour ce spécifique
  ouverture que vous aviez en tête (veuillez le nommer intelligemment, afin que vous puissiez
  comprendre facilement à quelle ouverture il est destiné). Alors au lieu d'un
  ensemble de 2 millions de bases de données de jeu environ, vous avez un beaucoup plus petit
  base de données de répertoire qui sera chargée rapidement dans l'ouverture
  entraîneur.
  </p>
  <p>
  Vous pouvez également acheter ou télécharger des bases de données spéciales couvrant
  ouvertures particulières, bien que nous ne puissions pas vous orienter dans la direction
  de toutes les sources possibles dans ce fichier d'aide.
  </p>
  <p>
  Une fois que vous avez une base de données plus petite destinée uniquement à l'ouverture
  le cas échéant, définissez le type comme indiqué à
  <a Maintenance> Fenêtre de maintenance </a>. Vous devrez redémarrer Scid
  d'utiliser le formateur d'ouverture avec cette nouvelle base de données de répertoire.
  </p>
  <p>
  Si vous le souhaitez, pour que le formateur d'ouverture fonctionne mieux, vous pouvez signaler
  certains jeux comme "Ouverture blanche" ou "Ouverture noire". Un exhaustif
  Pour ce faire, il faudrait parcourir toute la base de données »en
  main »et attribuer les drapeaux individuellement. Un moyen plus rapide, peut être de
  filtrez votre base de données pour les ouvertures blanches, puis définissez le drapeau pour tous
  ces jeux filtrés, et de même pour les ouvertures noires. Souviens-toi
  peut avoir un répertoire pour le blanc et le noir dans la même base de données.
  Vous pouvez définir les indicateurs de cette manière via la <a Maintenance> Maintenance
  fenêtre </a>. La façon dont vous choisissez de filtrer les jeux à signaler est
  entièrement à l'utilisateur. Une façon possible est de filtrer les jeux 1-0 pour
  ouvertures blanches et 0-1 pour les ouvertures noires. Cette façon n'est pas la meilleure
  cependant, et il appartient à l'utilisateur de trouver le système qu'il ressent
  le plus à l'aise avec.
  </p>
  <p>
  Enfin, pour mieux fonctionner, le Formateur d'ouverture peut utiliser les valeurs NAG pour comprendre
  comment un coup dans l'ouverture doit être évalué. Veuillez vous référer au <a NAGs> NAG
  aide d'annotation </a> pour plus d'informations.
  </p>
  <p>
  <h2> Utilisation du simulateur d'ouverture </h2>
  Pour utiliser le formateur d'ouverture, ouvrez d'abord la base de données du répertoire
  créé ou l'un des types appropriés. Ensuite, choisissez Play / Training / Opening Trainer.
  Le dialogue à venir propose quelques choix sur la prochaine formation
  session
  <ul>
      <li> <term> blanc </term> / <term> noir </term> / <term> les deux </term> est
      utilisé pour sélectionner le côté de l'ouverture à former
      </li>
      <li> <term> Autoriser uniquement les meilleurs coups </term> traitera les coups moins bien notés
      comme erreur. La cote d'une ligne est donnée par les valeurs NAG. </li>
      <li> <term> L'adversaire joue les meilleurs coups </term> Scid jouera toujours
      la meilleure continuation selon la base de données du répertoire. Si
      Scid non coché est autorisé à choisir également des notes inférieures
      continuations. Ceci est également utile pour apprendre les réfutations.
      </li>
      <li> <term> Seules les lignes marquées </term> Scid ne jouera que les lignes
      des jeux marqués comme <term> Ouverture pour les blancs </term> ou
      <term> Ouverture pour le noir </term>.
      </li>
      <li> <term> Réinitialiser les statistiques </term> réinitialisera les statistiques
      avant la session de formation. </li>
  </ul>
  Appuyez sur le bouton <term> Continuer </term> pour poursuivre la formation.
  </p>
  <p>
  Le formateur d'ouverture réel apparaîtra. Ici on peut afficher
  possibles coups candidats (masqués par défaut) et afficher également leur
  valeurs en activant simplement ces fonctions. Si <term> Afficher
  statistiques </term> est activé, les statistiques actuelles sont affichées dans le
  champs codés par couleur sous la case à cocher. Sinon, l'ensemble
  les statistiques sont accessibles par le bouton <term> Afficher le rapport </term>.
  </p>
  
  <h3> Ouverture des statistiques du formateur </h3>
  <p>
  Quatre chiffres sont affichés montrant les statistiques de la position actuelle:
  <ul>
  <li> <term> Vert </term> le nombre de coups joués par l'utilisateur qui
  sont de bons coups </li>
  <li> <term> Jaune </term> le nombre de coups douteux joués qui sont
  dans le répertoire </li>
  <li> <term> Rouge </term> le nombre de coups joués qui ne sont pas dans le
  répertoire </li>
  <li> <term> Blanc </term> le nombre de fois où la position a été
  rencontré </li>
  </ul>
  </p>
  
  <h3> Le rapport </h3>
  <p>
  Le rapport présente des statistiques pour l'ensemble du répertoire et donne
  informations sur les connaissances de l'utilisateur:
  <ul>
  <li> <term> Positions dans le répertoire </term> le nombre total de
  positions dans le répertoire (toutes les lignes de la base de données) </li>
  <li> <term> Positions non jouées </term> les positions qui n'ont jamais été
  rencontré </li>
  <li> <term> Positions jouées </term> les positions qui ont été jouées et
  la somme du nombre de fois où ils ont été rencontrés. Notez que ceci
  la figure représente la somme des occurrences d'une position dans tous
  lignes d'ouverture: ce nombre peut rapidement devenir important pour les postes
  qui sont proches de la position de départ </li>
  <li> <term> Succès </term> le nombre de bons coups effectués par le joueur </li>
  <li> <term> Coups douteux </term> le nombre de coups faibles effectués par le
  joueur </li>
  <li> <term> Hors répertoire </term> le nombre de coups effectués par le
  joueur qui n'était pas dans le répertoire </li>
  </ul>
  </p>
  <p> <footer> (Mise à jour: Scid 3.6.26, octobre 2008) </footer> </p>
}

####################
### Correspondence Chess help:

set helpTitle(F,Correspondence) "Échecs par correspondance"
set helpText(F,Correspondence) {
<h1>Échecs par correspondance</h1>

<p>
Scid offre actuellement deux fonctionnalités principales pour la correspondance
les échecs.
</p>
<ul>
   <li><b>eMail Chess</b> procède en envoyant la partie en cours par eMail
   à votre adversaire une fois que vous avez fait votre coup. A cette fin, un message eMail
   est créé dans votre programme de messagerie préféré avec le jeu en cours
   qui lui est attaché au format <a PGN>PGN</a>. Bien entendu, tous les commentaires et
   les variantes sont supprimées avant.
   </li>
   <li><b>Correspondence Chess Servers</b> sont supportés au moyen de la
   Protocole Xfcc. Ici, des outils externes sont utilisés pour récupérer les jeux à partir
   votre (vos) compte(s) et les livrer à Scid pour la synchronisation. Après
   vous avez fait votre coup ce coup est envoyé à votre adversaire également au moyen de
   L'outil d'extraction et d'envoi est implémenté en tant qu'outil externe pour
   extension facile si d'autres protocoles se présentent.
   </li>
   <li><b>Jeu postal</b> : Scid permet le support du jeu postal par
	ajouter un fichier PGN avec les informations d'en-tête appropriées dans sa boîte de réception
	répertoire. Si le champ d'en-tête supplémentaire <b>Mode</b> est défini sur
	<b>Postal</b> ces jeux ont leur propre icône affichée. Note :
	que les champs d'en-tête <b>CmailGamename</b> et <b>Gameid</b>
	devrait être présent. 
	---TODO--- : L'impression des cartes postales est sur le todo, cette fonction
	nécessitera les champs d'en-tête supplémentaires WhiteAddress / BlackAddress
	pour contenir l'adresse postale des deux joueurs.
   </li>
   <li><b>L'observation des jeux</b> est prise en charge en ajoutant simplement l'URL
   pour le fichier <a PGN>PGN</a> à la liste proposée par <menu>Lecture / Correspondance
   Échecs / Observer le jeu</menu>. Pour que cette fonction fonctionne un connecteur
	pour le site en question est requis. (Voir ci-dessous)
   </li>
</ul>
<p>
Si l'on accède à l'une des fonctions d'échecs par correspondance à partir de la
une nouvelle fenêtre s'ouvre. Elle peut être ouverte manuellement à partir de la fenêtre
ou on peut utiliser le raccourci clavier F12. Il y a deux
des raccourcis supplémentaires :
</p>
<ul>
   <li><b>Ctrl-F12</b>
      Ceci ouvrira la fenêtre d'échecs par correspondance et la fenêtre par défaut
      base de données. Il traitera également votre boîte de réception actuelle.
   </li>
   <li><b>Alt-F12</b>
      Ceci ouvrira la fenêtre d'échecs par correspondance, la fenêtre par défaut
      et initie également la récupération des jeux Xfcc en une seule fois.
    </li>
</ul>
<p>
Veuillez voir ci-dessous pour plus de détails.
</p>

<p>
Cette fenêtre contient les boutons nécessaires pour naviguer dans
les jeux en cours, les touches de raccourci pour récupérer les jeux au moyen du Xfcc
protocole et la synchronisation dans les jeux basés sur l'email ainsi qu'une console
quels messages ont été envoyés ou récupérés. De plus, cette fenêtre
contient une liste des jeux en cours récupérés dans le répertoire de votre boîte de réception.
</p>

<p>
Pour utiliser les fonctions d'échecs par correspondance, une base de données du type
Les "échecs par correspondance" doivent être ouverts avant d'appeler
fonctions d'échecs par correspondance. Si vous ne disposez pas d'une telle base de données
pourtant, il suffit de créer une nouvelle base de données et de définir son type sur "Correspondance
chess" au moyen de la fonction <a Maintenance>Maintenance</a>.
Le réglage du type est important car Scid reconnaîtra la base de données pour
synchronisation par ce type. Comme cette base de données est vide après le
création Scid traitera toutes les parties d'échecs par correspondance qu'elle recevra à
première synchronisation comme de nouveaux jeux et les ajouter à cette base de données.
</p>

<p>
Si aucune base de données du type "Échecs par correspondance" n'est actuellement ouverte
Scid vous le rappellera. Cependant, n'ouvrez pas plus d'une
de ce type car Scid ne peut alors pas reconnaître celui à utiliser.
</p>

<h3><name CCWorkflow>Fonctionnalité de base</name></h3>

<p>
Si tout est correctement configuré, l'utilisation de la correspondance
les fonctions échiquéennes sont facilement accessibles par les boutons suivants :
<ul>
   <li><button tb_CC_Retrieve> Récupérer les parties d'échecs par correspondance.
   A cette fin, l'outil de recherche externe est appelé et toutes les parties sont récupérées
   par ce moyen ainsi que tous les autres jeux qui sont stockés dans la boîte de réception de Scid
   (voir ci-dessous) sont synchronisés dans le jeu d'échecs par correspondance actuel
   base de données.
   </li>
   <li><button tb_CC_Send> Envoie votre coup à l'adversaire soit
   en créant un nouveau message électronique dans votre programme de messagerie préféré ou en
   en envoyant le coup à un serveur d'échecs dans le cas de Xfcc.
   </li>
   <li><button tb_CC_delete> vide vos répertoires d'entrée et de sortie.
   </li>
   <li><img tb_CC_online> est affiché si la liste de jeu a été rafraîchie
   du serveur dans la session Scid en cours. L'info-bulle pour
   cette icône indique la date et l'heure du dernier rafraîchissement.
   </li>
   <li><img tb_CC_offline> indique que les icônes d'état Xfcc sont
   restauré à partir des résultats sauvegardés. Aucune mise à jour n'a eu lieu dans le
   session Scid en cours. L'info-bulle de cette icône indique la date et
   l'heure du dernier rafraîchissement.
   </li>
   <li><img tb_CC_pluginactive> indique, que les Échecs par Correspondance
   Les plugins ont été chargés et activés au démarrage.
   </li>
</ul>
</p>
<p>
Voir aussi la liste des <a CCIcons>Icônes et indicateurs d'état</a>.
</p>

<h3>Configuration</h3>

<p>
Les Échecs par Correspondance dans le Scid sont basés sur un Scid normal
base de données qui contient les jeux et quelques aides externes à Scid qui
s'occuper des "pièces non salissantes". Ces outils et paramètres doivent être réglés
et sont stockés par la suite pour une utilisation ultérieure.
</p>

<p>
Pour accéder au dialogue de configuration, choisissez <menu>Play</menu>
<menu>Correspondance Échecs</menu><menu>Configurer</menu> à partir de la
menu. Les détails sont décrits dans <a CCSetupDialog>Correspondance
Chess Setup</a>. En quittant ce dialogue par le bouton <b>[Ok]</b>, vous
stocker automatiquement vos options.
</p>

<h3>Récupération des jeux</h3>
<p>
Selon que vous jouez aux échecs par correspondance par courriel ou par un
serveur d'échecs, le processus de récupération réel diffère légèrement. Généralement
il en résulte un ensemble de jeux au format <a PGN>PGN</a> situé dans la boîte de réception de Scid
répertoire. Cela offre également la possibilité d'une récupération automatique via
logiciel externe.
</p>
<p>
Une fois que les jeux sont dans la boîte de réception de Scid en invoquant <menu>Procéder à la boîte de réception</menu>
du menu fonctionnera dans la Boîte de réception et ajoutera de nouveaux coups dans la
des jeux déjà dans la base de données. De plus, il ajoutera les jeux qui ne sont pas
trouvé dans la base de données d'échecs par correspondance actuelle comme nouvelles parties.
</p>

<p><a CCeMailChess>Correspondence Chess via eMail</a> décrit la
détails pour l'utilisation de l'eMail, alors que dans <a CCXfcc>Correspondance
Chess via Chess Servers</a> décrit la même chose pour la correspondance
des serveurs d'échecs.
</p>

<h3>Faire un pas dans les jeux</h3>
<p>
Une fois les jeux récupérés, ils sont chargés dans la Clipbase de Scid
et de nouveaux coups sont ajoutés et stockés dans les échecs par correspondance
base de données ouverte. La différence par rapport à
les fonctions du menu <menu>Games</menu> est, que ces deux
Les boutons ne font défiler qu'entre les jeux de la boîte de réception de Scid qui sont
censé être vos jeux en cours. Bien sûr, le
La base de données de Correspondence Chess pourrait contenir beaucoup plus de parties, mais
normalement vous ne voulez pas passer par tout cela pour découvrir ce que votre
l'adversaire s'est déplacé dans une partie en cours.
</p>
<p>
Notez qu'une recherche d'en-tête est requise en incorporant certains champs qui
ne sont pas indexés par Scid. Par conséquent, le stockage de vos échecs par correspondance
jeux dans une énorme base de données de référence pourrait ne pas être conseillé car le
les temps de recherche peuvent être assez longs. Si vous jouez beaucoup et que votre propre
la base de données devient assez grande, les temps de recherche peuvent être réduits en déplaçant
jeux terminés dans une base de données d'archives, ou en créant simplement un nouveau
pour les jeux en cours. Scid traitera tous les jeux qui n'existent pas
dans la base de données des échecs par correspondance déjà comme nouvelles parties et les ajouter
automatiquement. Ainsi, il suffit d'ouvrir une base de données vide de
tapez "Correspondence chess" et appelez <menu>Process Inbox</menu> à
d'importer tous les jeux en cours.
</p>
<p>
Les éléments <menu>Précédent sont équivalents aux deux boutons mentionnés
Jeu</menu> et <menu>Suivant Jeu</menu> du <menu>Correspondance
Echecs</menu> menu.
</p>
<p>
Une autre façon de sauter à un jeu spécifique est de double-cliquer sur
dans la liste de jeu.
</p>
<p>
Notez que si vous avez correctement configuré vos noms de joueurs (au moyen de
<menu>Mes noms de joueurs</menu>) Scid fera tourner le plateau pour que vous
jouer toujours vers le haut. Vous pouvez avoir plusieurs noms de joueurs. Voir aussi
<a Options MyPlayerNames>My Player Names</a> pour plus de détails.
</p>

<h3>Analysez et faites un coup</h3>
<p>
Toutes les fonctions d'analyse peuvent être utilisées pour les parties d'échecs par correspondance.
Des variantes, annotations etc. peuvent être ajoutées comme dans une partie normale
l'analyse. Une fois terminé, Scid prendra le dernier demi-coup ajouté à
le jeu et le traiter comme le coup actuel à envoyer. On ne vérifie pas si
seulement un demi-coup a été ajouté ou quel côté déplacer est fait ici,
par conséquent, seul un demi-coup vers la ligne principale doit être ajouté !  Dans le cas où un
le serveur d'échecs est utilisé Scid envoie aussi le dernier commentaire ajouté à la
qui pourrait être utile pour la communication avec l'adversaire. Dans
Echecs par courriel : ceci peut être fait par le message normal, donc là tout
les commentaires sont supprimés.
</p>
<p>
En appuyant sur le bouton Send <button tb_CC_Send> vous aurez Scid to
déterminer le type de jeu d'échecs par correspondance affiché (eMail ou
un jeu de serveur) et appelez soit votre programme d'eMail, soit l'envoi externe
outil pour soumettre votre déménagement. Appeler <menu>Envoyer le coup</menu> est équivalent
à ce bouton. Alternativement, <menu>Mail move</menu> peut être utilisé pour
envoyer le jeu actuel par eMail. Dans le cas d'un jeu par eMail, ce
est équivalent à <menu>Envoyer move</menu>. Dans le cas d'un serveur
un message électronique est généré. Notez cependant qu'il
ne contiennent pas nécessairement un destinataire approprié, car les adresses de courriel ne sont pas
échangé dans les échecs par correspondance sur serveur.
</p>

<h3>Observer les jeux par correspondance</h3>
L'observation d'une partie d'échecs par correspondance se fait techniquement par
récupérer les jeux <a PGN>PGN</a> notation du serveur web.
Ces jeux sont ensuite introduits dans le workflow habituel de la
Correspondence Chess fonctionne et s'y présente comme les parties habituelles. Pour
cette fonction pour faire fonctionner un fichier nommé "connector.xml" doit être disponible
dans ~/.scid/data. Si ce fichier n'existe pas, Scid va essayer de générer
un qui convient à l'observation des jeux de l'ICCF Webchess. Autres
peuvent être ajoutés à ce fichier en ajoutant des serveurs
des blocs de serveurs.	

Pour observer une partie sur ICCF Webchess, pointez votre navigateur web vers la page
où la notation et l'échiquier sont affichés. Copiez l'URL de
cette page dans le presse-papiers et l'ajouter à la liste affichée par
en choisissant <menu>Jouer / Échecs par correspondance / Observer la partie</menu>. Utilisez
une nouvelle ligne pour chaque jeu à observer.

Fermez le dialogue en sélectionnant Ok et récupérez la correspondance
des jeux d'échecs. Maintenant, les parties observées nouvellement ajoutées devraient apparaître dans le
Correspondance Liste des parties d'échecs.

<h3>Caractéristiques supplémentaires de la Fenêtre des Échecs par Correspondance</h3>
<p>
La fenêtre Échecs par Correspondance offre des fonctions supplémentaires au moyen
de son propre menu, qui est identique à celui des échecs par correspondance. Il
est également possible d'utiliser les fonctions de défilement habituelles de Scid, comme
en passant à la partie suivante (précédente) par Ctrl-Down (Ctrl-Up). En survolant
avec la souris sur certains champs de la liste de jeu donnera également
des informations supplémentaires telles que fournies par le serveur. Si
vous voulez copier la liste de jeu actuelle dans le presse-papiers que l'on peut utiliser
les raccourcis habituels Ctrl-Ins (ou Ctrl-C).
</p>

<p><footer>(Mise à jour : Scid 4.3, Jan 2011)</footer></p>
}


set helpTitle(F,CCIcons) "Icônes et indicateurs d'état"
set helpText(F,CCIcons) {
<h1>Icônes et indicateurs d'état</h1>
<p>
Pour raccourcir l'affichage, un ensemble d'icônes est utilisé dans la liste des jeux. Quelques
d'entre eux ne sont présents que dans certaines circonstances, certains ne sont
pour les jeux basés sur le Xfcc, certains pour les jeux basés sur l'eMail. Ces indicateurs sont
stockées en interne et sont restaurées à la dernière mise à jour du serveur
si aucune connexion interet n'est disponible.
</p>
<ul>
   <li><img tb_CC_online> est affiché si la liste de jeu a été rafraîchie
   du serveur dans la session Scid en cours. L'info-bulle pour
   cette icône indique la date et l'heure du dernier rafraîchissement.
   </li>
   <li><img tb_CC_offline> indique que les icônes d'état Xfcc sont
   restauré à partir des résultats sauvegardés. Aucune mise à jour n'a eu lieu dans le
   session Scid en cours. L'info-bulle de cette icône indique la date et
   l'heure du dernier rafraîchissement.
   </li>
   <li><img tb_CC_pluginactive> indique, que les Échecs par Correspondance
   Les plugins ont été chargés et activés au démarrage.
   </li>
   <li><img tb_CC_envelope> C'est un jeu basé sur l'email. Dans ces
   plusieurs des drapeaux de statut utilisés dans les jeux basés sur Xfcc ne sont pas
   disponible en raison de la limitation du milieu.
   </li>
   <li><img tb_CC_relay> C'est un jeu observé, c'est-à-dire que vous êtes juste
   le chercher et vous ne ferez aucun coup dans ces jeux.
   </li>
   <li><img tb_CC_postal> Le mode de ce jeu est réglé sur
   <i>postal</i> (c'est-à-dire un véritable jeu de correspondance classique par voie postale
   service). Scid affichera ces jeux mais les ignorera dans la mise à jour
   car aucun <a PGN>PGN</a> fichier de jeu n'existera dans le jeu postal.
   </li>
   <li><img tb_CC_yourmove>
   C'est à vous de jouer.
   <b>Note 1:</b> cet état n'est mis à jour que si vous
   synchroniser vos jeux avec le serveur, c'est-à-dire qu'il se réfère toujours
   à l'état des serveurs lors de la dernière synchronisation.
   <br>
   <b>Note 2:</b> En survolant cette icône avec la souris, vous verrez
   le dernier coup de l'adversaire.
   </li>
   <li><img tb_CC_oppmove>
   C'est le coup des adversaires. Note : ce statut n'est mis à jour que si vous
   synchroniser vos jeux avec le serveur, c'est-à-dire qu'il se réfère toujours
   à l'état des serveurs lors de la dernière synchronisation.
   </li>
   <li><img tb_CC_draw>
   La paix a été conclue par un tirage au sort.
   </li>
   <li><img tb_CC_book>
   L'utilisation de bibliothèques d'ouvertures est autorisée pour ce jeu.
   </li>
   <li><img tb_CC_database>
   L'utilisation de bases de données est autorisée pour ce jeu.
   </li>
   <li><img tb_CC_tablebase>
   L'utilisation de bases de données de fin de partie (par exemple les tables de finales Nalimov, etc.) est
   permis dans ce jeu.
   </li>
   <li><img tb_CC_engine>
   Les moteurs d'échecs sont autorisés pour ce jeu. Parfois, ces jeux sont
   aussi appelé "Échecs Avancés".
   </li>
   <li><img tb_CC_outoftime>
   Votre adversaire a manqué de temps. Vous pouvez prétendre à une victoire dans les temps.
   </li>
   <li><img tb_CC_message>
   Votre adversaire a envoyé un message avec son dernier coup. Vérifiez le
   la notation du jeu.
   </li>
</ul>
<p>
<b>Note 1:</b> Si un événement (par exemple un tournoi) n'a pas encore commencé, mais
la configuration a déjà été récupérée via Xfcc l'id, l'événement et le site
Le texte des colonnes s'affiche en <gray><b>gris foncé</b></gray>.
</p>
<p>
<b>Note 2:</b> En survolant la colonne du site, vous verrez le site entier
en survolant la colonne événement, vous verrez la balise événement entière et la balise
le contrôle du temps s'il a été envoyé avec le jeu. Si aucun temps
a été envoyé, Scid l'indiquera comme <i>10 / 50d ( ?)</i>, c'est-à-dire,
devinant le calendrier officiel par défaut de l'ICCF de 10 coups en 50 jours. (Le
Le serveur ICCF lui-même n'envoie jamais de timings).
</p>

<p>
Dans les jeux Xfcc, chaque pays adversaire peut être affiché par le
associé, si le serveur fournit cette information. Pour le courrier électronique
cela peut être réalisé en ajoutant des balises <a PGN>PGN</a> supplémentaires pour les jeux basés sur
<i>paysblanc</i> et <i>pays noir</i>, chacun étant suivi de la
code pays international à trois lettres selon l'ISO 3166-1
(par exemple "EUR" <img flag_eur>, "USA" <img flag_usa>,
"GBR" <img flag_gbr>, "FRA" <img flag_fra>,
"RUS" <img flag_rus>, "CHN" <img flag_chn>...).
</p>
<p>
Pour montrer les drapeaux des pays pour d'autres types de jeux (postaux, observés), il suffit de
ajouter les en-têtes WhiteCountry/BlackCountry aux jeux de votre base de données.
Notez que les valeurs des en-têtes dans la base de données Scid écrasent les valeurs
fournis dans les jeux de la boîte de réception de Scid. C'est même pour les jeux Xfcc qui
on peut ajouter des drapeaux de pays ou écraser le mode jeux. (Ce dernier est
requis par exemple pour les jeux mixtes sur ICCF où l'interface ICCFs Xfcc
fournit également des jeux même s'ils sont joués par la poste).
</p>

<p>
Voir aussi le chapitre <a Correspondence>Echecs par Correspondance</a> pour
informations générales.
</p>


<p><footer>(Updated : Scid 4.3, Jun 2010)</footer></p>
}


set helpTitle(F,CCXfcc) "Serveurs d'Échecs par Correspondance"
set helpText(F,CCXfcc) {
<h1>Serveurs d'Échecs par Correspondance</h1>

<p>
Il existe plusieurs serveurs d'échecs par correspondance à travers le
Internet. Généralement, ils sont utilisés au moyen d'un navigateur Web, donc pas de
un logiciel spécifique est nécessaire. Cependant, beaucoup d'entre eux offrent également un
interface vers un logiciel d'échecs spécialisé via un protocole appelé Xfcc.
L'intégration de Xfcc se fait dans Scid via un ensemble d'outils d'aide externes
dans le <a CCSetupDialog>Configuration</a> dialogue pour la correspondance
les échecs.
</p>

<h3>Démarrer une nouvelle partie</h3>
<p>
Xfcc ne permet pas de démarrer un nouveau jeu lui-même. La recherche d'un
et le début d'une partie est plutôt géré par le serveur d'échecs
sur leur site web. Une fois le jeu lancé, Scid peut être utilisé pour
récupérer les coups de l'adversaire, les ajouter à la base de données interne,
les analyser et ainsi de suite. Toutes les fonctionnalités de Scid sont à la disposition des utilisateurs
bien que certains modes de jeu puissent ne pas les autoriser (par exemple, les jeux normaux
ne permettent généralement pas l'utilisation de moteurs d'échecs pour l'analyse).
</p>

<h3>Retrouver les jeux</h3>
<p>
Ouvrez une base de données qui contient des parties d'échecs par correspondance. Cette base de données
doit être du type "Échecs par correspondance". 
</p>

Notes : 
<ul>
   <li>Si Scid ne trouve pas de base de données d'échecs par correspondance, il
   vous informer pour en ouvrir un.
   </li>
   <li>Si la base de données ne contient pas les jeux qui sont récupérés de
   le serveur ; ils sont traités comme de nouveaux jeux et ajoutés à la base de données
   automatiquement.
   </li>
   <li>Scid utilisera la première base de données de type "Correspondence Chess".
   qui est actuellement ouvert. Pour cette raison, un seul de ces DB devrait être
   ouvert à la fois.
   </li>
</ul>
<p>
Xfcc récupère toujours tous les jeux hébergés sur un serveur spécifié pour votre
l'ID utilisateur en même temps. Pour récupérer les jeux, il suffit d'appuyer sur le <button tb_CC_Retrieve>
ou sélectionnez <menu>Retrieve Games</menu> dans le
<menu>Correspondance Échecs</menu> menu. Comme une connexion au serveur est
nécessaire pour récupérer de nouveaux jeux, assurez-vous que le système a un réseau
l'accès. Scid appellera l'outil d'extraction configuré dans le
<a CCSetupDialog>Configuration</a> dialogue qui va placer les jeux dans
Format PGN dans la boîte de réception de Scid. Cela peut prendre un certain temps pour récupérer le
répondre, alors soyez patient. Après avoir récupéré les jeux, les
la base de données des échecs par correspondance est mise à jour en conséquence.
</p>

<p>
<b>Note</b>En utilisant le <button tb_CC_delete> vous pouvez vider votre ensemble
Répertoires des boîtes d'entrée et de sortie.
</p>

<p>
Une fois que les jeux sont récupérés, leur contrepartie est recherchée dans le
les échecs par correspondance db et les nouveaux coups sont ajoutés en conséquence. Comme Xfcc
Les serveurs peuvent offrir différentes façons d'insérer des coups (via le web ou le mobile ou
d'autres programmes...) il se pourrait bien que Scid doive ajouter la moitié de
le jeu à la db. Cela ne pose aucun problème. Scid va ajouter tous les coups
retournée dans le jeu depuis le serveur. Scid ne remplacera cependant pas
le jeu depuis le début car alors toute votre analyse peut être perdue.
Il est donc <b>important de noter</b> que vous ne devez pas insérer de coups à
la ligne principale au-delà de votre propre dernier coup ! Pour ajouter des suites, veuillez utiliser des variantes !
</p>
<p>
Les jeux de base Xfcc offrent un affichage étendu du statut dans la liste des jeux.
Cette information n'est cependant disponible que si les jeux Xfcc internes de Scid
support est utilisé.  Les icônes suivantes sont pour l'affichage visuel :
<ul>
   <li><img tb_CC_draw> Une nulle a été convenue avec le dernier coup.
   </li>
   <li><img tb_CC_yourmove> Vous êtes au trait.
   </li>
   <li><img tb_CC_oppmove> Votre adversaire est au trait.
   </li>
   <li><img tb_CC_book> Ce jeu permet d'utiliser des bibliothèques d'ouvertures.
   </li>
   <li><img tb_CC_database> Ce jeu permet l'utilisation de bases de données.
   </li>
   <li><img tb_CC_tablebase> Ce jeu permet l'utilisation de tables de finales.
   </li>
   <li><img tb_CC_engine> Ce jeu permet l'utilisation de moteurs d'échecs.
   </li>
</ul>
</p>
<p>
De plus, Scid affichera l'horloge pour les deux parties <b>à la
temps de synchronisation</b> ainsi que la variante d'échecs jouée. Notez cependant
que Scid ne supporte actuellement que les échecs standards.
</p>
<p>
Note : seulement si l'icône appropriée (bibliothèque, base de données, table de finales, moteur)
est affiché, l'utilisation de ces outils est autorisée. Il est interdit
sinon. Soyez juste et respectez ces règles.
</p>
<p>
Note : si d'autres sources ont placé des jeux dans votre boîte de réception (par exemple de
vos échecs par correspondance) ils sont également synchronisés dans le
dans la base de données au fur et à mesure que la boîte de réception est parcourue.
Cela permet d'ajouter des jeux par eMail dans la boîte de réception, puis de passer à Scid,
appuyez sur <button tb_CC_Retrieve> et tous les jeux sont à jour.  Les jeux qui ne sont pas
mais trouvés dans la base de données sont traités comme de nouveaux jeux et ajoutés à la
base de données.
</p>
<p>
<b>Note</b>En utilisant le <button tb_CC_delete> vous pouvez vider votre ensemble
Répertoires des boîtes d'entrée et de sortie.
</p>
<p>
<b>Note pour les programmeurs</b> : l'outil de récupération est appelé avec le chemin de la boîte de réception comme
paramètre. Il est censé fonctionner avec tous les comptes du serveur et placer
correctement formaté <a PGN>PGN</a> fichiers dans le chemin qui lui est passé. Ces fichiers
doit contenir des champs d'en-tête supplémentaires, tels qu'ils sont connus par le système cmail
outil. (Voir <a CCeMailChess>Correspondence Chess via eMail</a> pour
informations sur les champs obligatoires).
</p>

<p><footer>(Mise à jour : Scid 3.6.23, mars 2008)</footer></p>
}

#############
# eMail Chess:

set helpTitle(F,CCeMailChess) "Echecs par correspondance via eMail"
set helpText(F,CCeMailChess) {
<h1>Echecs par correspondance via eMail</h1>

<p>
eMail offre une façon très pratique de jouer aux échecs par correspondance. Le
l'application standard dans le monde Un*x pour ce jusqu'à aujourd'hui est xboard
avec son aide au courrier. Comme il permet une gestion presque automatique de
le traitement des courriers électroniques d'échecs par correspondance et n'ajoute pas non plus
tout ce qui n'est pas conforme à <a PGN>PGN</a> c'est aussi le modèle que Scid doit gérer
Echecs par courriel. En conservant simplement l'en-tête <a PGN>PGN</a> complet, de telles parties peuvent être
joué avec tout adversaire qui possède un outil pour manipuler la PGN.
</p>

<h3>eMail Chess</h3>
<p>
Scid peut gérer les parties d'échecs par correspondance par courriel presque automatiquement.
La façon dont cela est fait est compatible avec l'utilitaire cmail qui
est livré avec xboard sur les systèmes Un*x. (Ceci dit, cela implique que vous
peut jouer contre un adversaire en utilisant cmail/xboard). Cela fonctionne en envoyant
les jeux dans leur ensemble <a PGN>PGN</a> aussi et de là en tant que pièces jointes à un mail, alors que
l'en-tête contient certaines balises qui permettent de les reconnaître et
triés ensemble. Pour cette raison, l'utilisateur doit faire attention à
l'édition des champs d'en-tête.  Notez que les champs avec des valeurs explicites ont
pour être réglé exactement à cette valeur pour les échecs par courriel. Commencer une partie avec
Scid fera cela automatiquement, mais vous <b>ne devez pas</b> écraser
ou les supprimer !
</p>
<p>
Les champs d'en-tête essentiels sont :
</p>
<ul>
   <li><term>Event</term> : par défaut "Jeu par correspondance".
   </li>
   <li><term>Site</term> : doit être "NET"
   </li>
   <li><term>Mode</term> : doit être "EM"
   </li>
   <li><term>WhiteNA</term> : contient l'adresse eMail du lecteur blanc. Note :
   que seule l'adresse nue y est stockée sous la forme
   <term>user@host.org</term>.
   </li>
   <li><term>BlackNA</term> : contient l'adresse eMail du joueur noir
   similaire à l'ADN blanc.
   </li>
   <li><term>CmailGameName</term> : Contient un identifiant <b>unique</b> pour
   le jeu. Ceci est utilisé pour trier les jeux ensemble.
   <p>
   Bien que Scid puisse utiliser un index de base de données, ce n'est pas possible pour
   des outils non basés sur la BD comme cmail. Pour cette raison, le
   Le paramètre <term>CmailGameName</term> est fourni par l'utilisateur. Il doit être
   unique ! Le moyen le plus simple est quelque chose de la forme
   <term>xx-yy-yyyymmdd</term> où xx est un raccourci pour le blanc
   joueur, yy un pour le joueur noir, et yyyymmdd la date actuelle.
   </p>
   <p>Pour les jeux basés sur Xfcc, ce champ doit également être défini sur un
   mais il y a le nom du serveur et le numéro de jeu unique sur
   ce serveur peut être utilisé, c'est-à-dire que cet identifiant est de la forme
   <term>MyXfccServer-12345</term>.
   </p>
   </li>
</ul>
<p>
Les échecs basés sur le courrier électronique ne contiennent pas les codes d'état étendus comme Xfcc.
Ces parties montrent l'icône <img tb_CC_envelope> pour les notifier comme
Basé sur le courrier électronique. Il en va de même pour les jeux observés, désignés par <img tb_CC_relay>
et les jeux postaux qui affichent l'icône <img tb_CC_postal>.
</p>

<h3>Démarrer une nouvelle partie</h3>
<p>
Cela ouvre un dialogue pour l'entrée du propre nom et du nom de l'adversaire comme
ils doivent apparaître dans l'en-tête ainsi que les adresses eMail de
les deux parties. De plus, un <b>unique</b> ID de jeu doit être inséré.
Le moyen le plus simple pour cette ID est quelque chose de la forme
<term>xx-yy-yyyymmdd</term> où xx est un raccourci pour le blanc
joueur, yy un pour le joueur noir, et yyyymmdd la date actuelle.
Cet identifiant est un texte et il est important d'identifier les jeux de façon unique.
Les utilisateurs de cmail connaîtront également cet identifiant sous la forme <i>nom du jeu</i>. Il doit
ne contiennent que des lettres et des chiffres, le signe moins et le trait de soulignement.
Veuillez éviter les autres caractères.
</p>
<p>
Après avoir quitté la boîte de dialogue en appuyant sur le bouton <b>[Ok]</b>, un nouveau
le jeu est ajouté à la base de données d'échecs par correspondance actuellement chargée
et l'en-tête <a PGN>PGN</a> est correctement défini. Faites votre coup et envoyez-le comme
mentionnés ci-dessous.
</p>

<h3>Retrouver les jeux</h3>

<p>
Scid ne gère pas votre boîte aux lettres automatiquement. Cela pourrait,
compte tenu du large éventail de configurations possibles du courrier de nos jours, impliquent
une énorme quantité de code. Pour cette raison, Scid s'appuie sur votre
Programme d'eMail qui est beaucoup plus approprié à cet effet que Scid
ne pourra jamais être. Pour obtenir une partie dans Scid, il suffit de sauvegarder le fichier ci-joint <a PGN>PGN</a>.
dans la boîte de réception de Scid et traiter la boîte de réception soit par <menu>Retrieve
Games</menu> ou le bouton <button tb_CC_Retrieve> ou
<menu>Process Inbox</menu>. La différence entre les deux est que
le premier va aussi chercher et remplir la boîte de réception avec
les jeux provenant d'une autre source (disons Xfcc) soit par le Xfcc interne
ou un outil de récupération externe appelé. D'où <button tb_CC_Retrieve>
est le moyen le plus pratique si vous utilisez les deux types de parties d'échecs par correspondance.
</p>
<p>
<b>Note</b> Le menu <menu>Retrieve Games</menu> ou le bouton <button tb_CC_Retrieve>
<b>ne récupère pas</b>  vos messages eMail ! Vous
devez enregistrer vos fichiers <a PGN>PGN</a> dans la boîte de réception de Scid à la main. Cela peut probablement
être automatisé par votre programme eMail. Sur les systèmes Un*x, mettre en place un
mime-handler est assez facile au moyen de <term>.mailcap</term>.
</p>
<p>
<b>Note</b>En utilisant le <button tb_CC_delete> vous pouvez vider votre ensemble
Répertoires des boîtes d'entrée et de sortie.
</p>

<h3>Envoyer la réponse</h3>

<p>
Après avoir effectué votre déplacement, envoyez-le soit par le <menu>Mail Move</menu>
dans le menu via <menu>Envoyer move</menu> ce qui équivaut à
<button tb_CC_Send>. Ce dernier devra reconnaître le jeu
comme correspondance par courriel et l'envoyer par la poste alors que l'ancienne méthode
forcera Scid à générer un message électronique.
</p>
<p>
Bien sûr, Scid dépouille le jeu de tout commentaire et de toute variante
avant de le joindre à l'e-mail sortant car vous ne voulez probablement pas envoyer
votre analyse.
</p>
<p>
Si vous utilisez une interface graphique, sa fenêtre de composition habituelle s'ouvre. Le site
l'adresse de votre adversaire est renseignée ainsi qu'un sujet générique
contenant l'id du jeu pour un filtrage facile et l'adresse bcc si
spécifié dans le dialogue <a CCSetupDialog>Configuration</a>. Le courrier
est réglé pour contenir la <term>notation FEN</term> de la position finale et
la liste des coups effectués jusqu'à présent. De cette façon, l'adversaire peut rapidement regarder
à ton tour. Enfin, Scid attache le jeu en cours, y compris votre
se déplacer dans le format <a PGN>PGN</a> au message de courrier.
</p>
<p>
Lorsque vous utilisez un outil compatible avec mailx, aucune fenêtre n'est ouverte et le courrier est
envoyé de manière invisible en invoquant l'outil spécifié en arrière-plan. Dans
Dans ce cas, le courrier généré contient le <a PGN>PGN</a> également dans le corps du courrier.
</p>
<p>
Notez que comme les échecs par courriel fonctionnent en envoyant le fichier entier <a PGN>PGN</a> vous devez
ne pas ajouter plus que votre demi-coup. Scid ne vérifie pas ici si
plus d'un demi-coup a été ajouté à la ligne principale, tout comme le fait Scid
ne pas savoir quel coup c'était, quand tu as envoyé le tien.
</p>
<p><footer>(Mise à jour : Scid 3.6.23, mars 2008)</footer></p>
}

#############
# Correspondence Chess setup dialog help:

set helpTitle(F,CCSetupDialog) "Configuration des échecs par correspondance"
set helpText(F,CCSetupDialog) {
<h1>Configuration des échecs par correspondance</h1>

<p>
Les entrées dans le dialogue de configuration sont préréglées à une valeur par défaut
paramètres mais ceux-ci ne doivent pas correspondre à la réalité de votre système, donc
l'adoption sera sûrement nécessaire. Tous les paramètres sont stockés dans le
automatiquement après la fermeture de la boîte de dialogue via le fichier de configuration
<b>[Ok]</b> bouton. Les paramètres sont effectifs immédiatement.
</p>

<p><b>Note</b> que Scid va utiliser les valeurs par défaut jusqu'à ce que ce dialogue de configuration
est appelé et quittez en sélectionnant le bouton <term>OK</term>. Le site
Les valeurs par défaut ne sont jamais stockées dans le fichier de configuration interne de Scid avant
alors. De plus, si Scid ne peut pas trouver une configuration valide, il essaiera
pour mettre en place automatiquement les fonctions d'échecs par correspondance. Par défaut
il stockera toutes les données dans le sous-répertoire <term>data</term> de Scid's
configuration. Ici aussi une DB valide ainsi que la configuration nécessaire
Les boîtes d'entrée/sortie sont créées.
</p>

<p>
<b>Base de données par défaut</b> :
Ceci définit la base de données par défaut pour
des parties d'échecs par correspondance. Il est facilement accessible via
<menu>Open Database</menu> dans le <menu>Correspondence Chess</menu>
menu. L'avantage de cette méthode est que cette base de données ne
apparaissent dans les <i>bases de données récentes</i> listes et que vous pouvez avoir un
dossier spécifique pour cela. Cette base de données <b> doit être </b> de type
"Échecs par correspondance". L'ouverture d'une base de données de ce type par tout autre
means est également correct, donc vous voudrez probablement ignorer ce paramètre
(e.g. si vous appelez Scid avec votre base de données d'échecs par correspondance sur
démarrage.)
</p>

<p>
<b>Boîte de réception (chemin)</b> :
Dans ce répertoire, Scid cherchera les parties d'échecs par correspondance stockées
au format <a PGN>PGN</a>. Ces jeux sont utilisés pour la synchronisation des
base de données d'échecs par correspondance. En général, Scid ne se soucie pas de la façon dont la
les jeux viennent dans ce répertoire. Cela va juste fonctionner à travers tous les fichiers <a PGN>PGN</a>.
situé là. Cela offre la possibilité d'utiliser certains outils externes
pour aller chercher des jeux à cet endroit. De plus, dans les échecs par courriel, un
devrait juste sauvegarder les fichiers <a PGN>PGN</a> reçus de l'adversaire dans ce
répertoire.
</p>
<p>
Scid ne lira pas une boîte aux lettres de quelque sorte que ce soit, il gère juste
tous les <a PGN>PGN</a> fichiers placés dans ce répertoire. Notez également qu'il va
synchroniser les jeux avec la base de données actuelle. Cependant, si un jeu
de ce répertoire n'existe pas encore dans la base de données, c'est
traité comme un nouveau jeu et ajouté à la base de données.
</p>
<p>
Pour que le processus de synchronisation fonctionne, les fichiers <a PGN>PGN</a> doivent contenir
des informations d'en-tête supplémentaires qui sont en parfait accord avec
la norme <a PGN>PGN</a>. Veuillez consulter <a CCeMailChess>Correspondance
Chess via eMail</a> si vous voulez créer votre propre outil ou si vous êtes
la migration des données d'un autre système.
</p>

<p>
<b>Outbox (path)</b> :
L'inverse du <i>Inbox</i>. Le scid place ici <a PGN>PGN</a> les fichiers de la
les jeux sortants. Pour les échecs par courriel, c'est essentiel car les fichiers <a PGN>PGN</a> ont
pour être joint à un message électronique.  Pour Xfcc, où seul le déplacement est
envoyé, cela ne serait pas nécessaire, cependant le répertoire Outbox offre
une façon pratique de se connecter à votre PDA ou pour toute autre utilisation comme le
Les fichiers PGN contenus dans la Boîte d'envoi contiendront également votre dernier coup.
</p>

<p>
<b>Utiliser le support Xfcc interne</b> :
Si coché, Scid n'utilisera pas les outils externes spécifiés comme externes
mais utilise son support Xfcc interne pour récupérer les jeux et les
envoyer des coups. Ce sera le moyen le plus pratique d'accéder à un Xfcc
et doit être utilisé par défaut.
</p>
<p>
Cette fonctionnalité nécessite le support de http et tDOM pour l'installation de TCL.
Habituellement, ces modules sont distribués avec votre installation TCL,
Cependant, sur certains systèmes, ils doivent être installés explicitement. Si
soit on ne trouve pas cette fonction est désactivée.
</p>
<p>
<b>Xfcc Configuration</b> :
Donnez le chemin et le nom du fichier de configuration du protocole xfcc
maître-chien. Ce chemin est également transmis aux gestionnaires de protocole externes
pour être utilisé par eux.
</p>

<p>
<b>Outil d'extraction</b> :
ce programme est appelé pour récupérer les échecs par correspondance
des parties d'un serveur d'échecs par correspondance. Cette aide doit juste
récupérer les jeux de la source de son choix, générer un PGN correct
contenant l'en-tête nécessaire <a PGN>PGN</a>. Outils pour récupérer des jeux
des serveurs Xfcc existent en tant que programmes externes et ce sont les
des outils pour s'installer ici. Pour les futurs protocoles, on pourrait facilement générer
un outil de récupération externe qui gère ce protocole. Aussi l'automatisation
est possible si cette fonctionnalité est réalisée en externe.
</p>
<p>
Note : Cet outil <b>n'est pas</b> appelé pour la récupération des messages
des échecs par eMail !
</p>

<p>
<b>Outil d'envoi</b> : 
c'est l'inverse de l'outil d'extraction, principalement destiné à Xfcc
ou tout autre protocole futur qui pourrait survenir. L'outil d'envoi,
cependant, est appelé depuis Scid avec plusieurs paramètres où l'appel
ressemble à :
<term>
SendTool Outbox Name GameID MoveCount Move "Comment" resign claimDraw offerDraw acceptDraw
</term>
</p>

<p>
<b>Confirmer les coups</b> : ouvre un dialogue de confirmation après avoir appuyé
sur le bouton d'envoi pour chaque coup qui est envoyé au serveur.
</p>

<p>
<b>Seules les parties que j'ai le coup</b> : Vérifier ce paramètre raccourcira
les parties listées dans la fenêtre Échecs par Correspondance à seulement celles
les jeux où le joueur doit se déplacer.
</p>

<p>
<b>Triage</b> : Permet de trier les jeux selon différents critères. Ce tri
prend effet lors de la prochaine mise à jour de la liste des jeux, par exemple en rouvrant
la fenêtre Échecs par Correspondance, en allant chercher des parties ou en traitant les
Boîte de réception.
</p>

<p>
La signification des paramètres est la suivante :
   <ul>
      <li><term>Outbox</term> : Le chemin de la boîte de sortie défini dans ce dialogue. Le chemin
      L'outil d'envoi est destiné à générer un fichier correctement formaté <a PGN>PGN</a>.
      là.
      </li>
      <li><term>Nom</term> : Le nom du joueur à déplacer comme indiqué
      dans l'en-tête <a PGN>PGN</a>. Pour Xfcc, ce serait le nom de connexion. Il s'agit de
      identique au nom du lecteur dans l'en-tête <a PGN>PGN</a>.
      </li>
      <li><term>MoveCount</term> : Le numéro du coup à envoyer.
      </li>
      <li><term>Move</term> : Le déplacement réel dans le SAN.
      </li>
      <li><term> "Commentaire"</term> : Un commentaire envoyé à l'adversaire. Scid
      insère le dernier commentaire du jeu. C'est-à-dire que ces commentaires sont
      traités comme des commentaires à l'adversaire. Notez que le commentaire est
      cité, de sorte que des commentaires multilignes devraient être possibles.
      </li>
      <li><term>resign</term> : 0 ou 1, en précisant si l'utilisateur
      veut démissionner. Réglé à 1 si l'utilisateur invoque
      <menu>Démission</menu> du <menu>Echecs par Correspondance</menu>
      menu.
      </li>
      <li><term>claimDraw</term> : 0 ou 1, en spécifiant si l'utilisateur
      veut réclamer un match nul. Réglé à 1 si l'utilisateur invoque
      <menu>Claim Draw</menu> du <menu>Correspondence Chess</menu>
      menu.
      </li>
      <li><term>offerDraw</term> : 0 ou 1, en spécifiant si l'utilisateur
      veut offrir un tirage au sort. Mettre à 1 si l'utilisateur invoque <menu>Offer
      Dessin</menu> du menu <menu>Correspondance Échecs</menu>.
      </li>
      <li><term>acceptDraw</term> : 0 ou 1, en spécifiant si l'utilisateur
      veut accepter un match nul offert par l'adversaire. Régler à 1 si le
      l'utilisateur invoque <menu>Accepter Dessin</menu> à partir de la
      <menu>Correspondance Échecs</menu>
      menu.
      </li>
   </ul>
</p>
<p>
Note : Cet outil est <b>not</b> appelé pour les échecs par courriel !
</p>

<p>
<b>Programme de messagerie</b> :
Ceci donne le chemin d'accès à votre programme de courrier électronique préféré. Ce programme est
a appelé aux échecs par courriel pour composer le message à l'adversaire.
</p>

<p>
<b>(B)CC Adresse</b> :
Une copie de l'annonce est envoyée à cette adresse en tant que copie cachée.
Notez cependant que si un mailer GUI est utilisé, il possède normalement son propre
le traitement du courrier sortant. Par conséquent, la définition de cette adresse peut dupliquer
des messages. Il peut cependant être utilisé pour transférer un jeu à une autre adresse.
</p>

<p>
<b>Mode</b> :
Malheureusement, il existe une large gamme de clients de messagerie et ils utilisent
des conventions d'appel très différentes. Quelques conventions communes, et
des exemples de programmes qui les utilisent, sont énumérés ici. Le programme de courrier
sera appelé avec la convention sélectionnée. Au cas où elle ne serait pas connue
quelle convention est utilisée ; l'une de celles proposées pourrait correspondre et faire
un tour. Notez cependant qu'un certain nombre de programmes de courrier ne sont pas
capable d'envoyer des pièces jointes lorsqu'il est appelé depuis un autre programme. Dans
dans ce cas, vous devrez soit changer de client de messagerie, soit ajouter le
dans la boîte d'envoi de Scid à la main.
</p>
<p>Hint : mailx ou l'un de ses nombreux clones devrait être disponible en tant que
application en ligne de commande sur presque toutes les plateformes comme une
outil. Au cas où aucune des conventions ne fonctionnerait avec votre
ou ce client ne peut pas gérer les mails avec pièces jointes par des appels
depuis la ligne de commande, l'installation de mailx serait une option.
</p>
<p>Hint : mutt utilise le système de transport de courrier (alias
sendmail/exim/postfix). Pour se connecter avec ceux (sans doute) pas faciles à
L'installation d'outils mutt est une option parfaite. Sur un Un*x décent avec un
la configuration devrait être la façon la plus indolore de gérer les échecs par courriel.
(Bien qu'il n'existe pas beaucoup de systèmes correctement configurés, en particulier dans le
le monde de Linux).
</p>
<p>
<b>Paramètre d'attachement</b> : 
Ce paramètre est utilisé pour spécifier une pièce jointe. Il est <b>seulement</b>
utilisé en mode <term>mailx</term>.
</p>
<p>
<b>Paramètre sujet</b> :
Ce paramètre est utilisé pour spécifier le sujet du message. Il s'agit de
est <b>seulement</b> utilisé en mode <term>mailx</term>.
</p>
<p><footer>(Mise à jour : Scid 3.6.24, mars 2008)</footer></p>
}

#############
# Correspondence Chess setup dialog help:

set helpTitle(F,CCXfccSetupDialog) "Configuration de serveur Xfcc"
set helpText(F,CCXfccSetupDialog) {
<h1>Configuration de serveur Xfcc</h1>
<p>
La boîte de dialogue de configuration du serveur Xfcc lit dans le fichier xfcc actuellement spécifié
et affiche tous les serveurs spécifiés dans le fichier de configuration.
La boîte de dialogue est séparée en deux parties : la partie supérieure liste tous les serveurs
noms définis, tandis que la partie inférieure énumère tous les
les valeurs de configuration de ces fichiers.
</p>
<h2>Entrées nécessaires</h2>
<ul>
<li><term>Nom du serveur</term> : Ceci spécifie le nom utilisé pour ce
et de générer des identifiants de jeu uniques. Le nom doit être constitué d'un
mot unique ne contenant que des caractères (a-z et A-Z), des chiffres et le
les caractères "-" et "_". Il est traité en tenant compte de la casse.
</li>
<li><term>Nom de connexion</term> : spécifie le nom utilisé pour se connecter à un
serveur spécifique. Il est sage de n'utiliser que des caractères, des chiffres
et "-" ainsi que "_" dans ce nom.
</li>
<li><term>Mot de passe</term> : définit le mot de passe utilisé pour la connexion. Le site
les mêmes règles s'appliquent que pour le nom de connexion. <b>Note</b> Scid actuellement
stocke vos mots de passe sur le disque dur sous forme non cryptée. Pour cela
raison de garder le répertoire en sécurité.
</li>
<li><term>URL</term> : C'est l'URL de base pour l'interface Xfcc de la
serveur d'échecs par correspondance. Il peut être trouvé à la page d'accueil des serveurs.
Quelques exemples pour les serveurs communs sont :
<ul>
	<li>SchemingMind : <url http://www.schemingmind.com/xfcc/xfccbasic.asmx>
	http://www.schemingmind.com/xfcc/xfccbasic.asmx</url>
	</li>
	<li>ICCF : 
	<url https://www.iccf.com/XfccBasic.asmx>
	https://www.webchess.com/XfccBasic.asmx</url>
	</li>
	<li>MeinSchach.de / MyChess.de :
	<url http://www.myChess.de/xfcc/xfccbasic.php4>
	http://www.myChess.de/xfcc/xfccbasic.php4</url>
	</li>
</ul>
</li>
</ul>
<p>
Pour basculer entre les différents paramètres du serveur, il suffit de sélectionner l'option
pour changer de serveur dans la zone de liste supérieure. Ses valeurs actuelles seront alors
s'affiche dans les zones de saisie et peut être repris. En cliquant sur
un autre serveur dans la liste activera les nouveaux paramètres.
</p>
<p>
Pour ajouter un nouveau serveur, il suffit d'appuyer sur le bouton <term>Add</term>. Une nouvelle entrée
sera créé qui est pré-rempli avec du texte à remplacer. S'il vous plaît
Gardez à l'esprit que le nom du serveur doit être unique dans votre configuration.
</p>
<p>
Pour supprimer un serveur, sélectionnez-le dans la liste et appuyez sur la touche
Bouton <term>Supprimer</term>. Toutes les valeurs pour ce serveur spécifique seront
être précédé d'un dièse (#) marquant cette entrée comme étant supprimée.
Par conséquent, si un serveur a été supprimé par accident, il suffit de supprimer le dièse
marques devant les entrées.
</p>
<p>
En appuyant sur <term>OK</term>, Scid devra mémoriser votre configuration actuelle. Sur
ce point, tous les serveurs marqués comme supprimés sont supprimés, tous les nouveaux serveurs
sont ajoutés à la configuration. En appuyant sur <term>Annuler</term> toutes les modifications
sont perdus, l'ancienne installation reste en place.
</p>

<p><footer>(Mise à jour : Scid 3.6.24, mai 2008)</footer></p>
}

# Serious game window help

set helpTitle(F,SeriousGame) "Jouer une partie sérieuse"
set helpText(F,SeriousGame) {
<h1>Jouer une partie sérieuse</h1>
<p>Les parties sérieuses peuvent être jouées contre n'importe quel
  <term>moteur UCI</term> configuré pour l'utilisation avec Scid. Vous pouvez paramétrer
  le <a SeriousGame Timing>mode de réfléxion</a> :
  les temps de jeu, profondeur d'analyse, coups calculés par seconde,
  réflexion permanente pour <term>régler la force de l'adversaire</term>;
  et les paramètres de
  l'<a SeriousGame TrainingMode>entraîneur</a> : évaluation des coups,
  entraînement aux ouvertures.
</p>
<h3>Configurer une partie sérieuse avec entraîneur</h3>
<h4><name Opponent>Paramétrage du moteur</name></h4>
<p>Une liste de tous les adversaires possibles est
  donnée en haut de la fenêtre <term>Configurer une partie sérieuse</term>.
  Si besoin, le bouton <b>[Configurer le moteur UCI]</b> donne accès aux
  paramètres du moteur. Par défaut, ces paramètres sont les mêmes que ceux définis
  dans la boîte de dialogue de configuration de la
  <a Analysis List>liste des moteurs d'analyse</a> du menu
  <menu>Outils</menu><menu>Moteur d'analyse...</menu>. Ensuite, la bibliothèque d'ouvertures peut être choisie parmi
  celles disponibles pour Scid. Décocher la case
  <term>Utiliser une bibliothèque d'ouvertures</term>
  désactivera l'utilisation d'une bibliothèque et le moteur démarrera
  le calcul des coups dès le début du match.
</p>
<h4><name Timing>Mode de réflexion</name></h4>
<p>Ces options permettent de régler le timing
  utilisé pour le moteur. Différents réglages sont possibles ici:
  <ul>
  <li><term>Temps + bonus</term> spécifie le temps pour la totalité de la partie
  et un incrément possible par coup (horloge Fisher).
  la valeur par défaut est fixée à 5 minutes par match et à un incrément de 10
  secondes par coup, ce qui est un paramètre habituel pour les jeux Blitz. Notez que
  les temps pour les Noirs et les Blancs peuvent être réglés indépendamment. Ceci
  permet de définir un court laps de temps pour le moteur et donner un plus long
  temps de réflexion pour le joueur, renforçant ainsi la
  possibilité d'analyse du joueur tout en affaiblissant le moteur d'échecs dans le
  cas où <term>Réflexion permanente</term> est désactivée (voir ci-dessous).
  </li>
  <li><term>Profondeur fixe</term> ne définit pas le temps par partie mais
  la profondeur limite que le moteur calculera en nombre de demi-coups. Au delà
  de cette profondeur, l'ordinateur ne verra pas certains mats et combinaisons,
  le moteur d'échecs peut jouer plus faible et offrir ainsi un meilleur partenaire
  pour l'entraînement.
  <b>Remarque</b> : certains moteurs, notamment commerciaux, proposent
  d'affaiblir leur force par des unités Elo. Cela offre très probablement un
  algorithme plus approprié que de limiter la profondeur de recherche. Dans Scid,
  ce type de jeu est également proposé en tant que
  <a TacticalGame>jeux tactiques</a> contre le moteur Phalanx gratuit.
  </li>
  <li><term>Noeuds</term> revient à limiter la profondeur de recherche,
  mais ici, le moteur doit jouer après l'évaluation d'un certain
  nombre de positions. La valeur par défaut est 10 000.
  </li>
  <li><term>Secondes par coup</term> permet au moteur de passer un
  un certain temps au maximum pour une position donnée. Certains
  moteurs joueront plus rapidement dans certaines circonstances, mais ils
  ne dépasseront pas le délai fixé ici. Comme "Profondeur fixe"
  et "Noeuds", cela limite également la force des moteurs d'échecs,
  mais donne également un jeu assez réactif.</li>
  </ul>
</p>
<h4><name TrainingMode>Paramètres d'ouvertures et entraînements</name></h4>
<p>
  <ul>
  <li><term>Ouverture spécifique</term> peut être cochée pour l'entraînement à
  l'ouverture choisie dans la liste en-dessous. Le joueur doit alors suivre la ligne
  choisie, sinon Scid demandera que le coup soit être repris.</li>
  <li><term>Joueur depuis la position actuelle</term>. Les parties sérieuses peuvent
  commencer à partir de la position actuelle de l'échiquier si cette case est cochée.
  Ceci permet par exemple de jouer des positions de milieu de partie qui découlent
  d'une ouverture.</li>
  <li><term>Réflexion permanente</term> (parfois simplement appelée réflexion)
  permet au moteur de calculer pendant le temps du joueur. Si elle n'est pas cochée,
  le moteur arrêtera d'analyser la position si le joueur a le trait.
  Si le match est défini pour une durée fixe par coup, cela affaiblira le
  moteur. En revanche, le moteur peut jouer immédiatement si le
  le joueur a joué le coup qu'il a analysé pendant le temps du joueur.</li>
  <li><term>L'entraîneur regarde</term> ouvrira une boîte de dialogue pour
  revenir sur un coup si le joueur a fait une erreur (selon
  l'évaluation de son dernier coup par le moteur).</li>
  </ul>
</p>
<p>
  Après avoir défini tous les paramètres du jeu et avoir cliqué sur le
  bouton <b>[Jouer]</b>, Scid mettra en place le moteur adversaire,
  affichera les horloges et commencera le jeu.
  <b>Le joueur ne doit pas jouer avant que les horloges apparaissent</b>.
  Notez que le moteur d'échecs peut mettre un certain temps à démarrer et s'initialiser
  correctement.
</p>

<p><footer>(Mise à jour: Scid 4.7.1, janvier 2020)</footer></p>
}

# Tactics Trainer

set helpTitle(F,TacticsTrainer) "Entraînement : exercices tactiques"
set helpText(F,TacticsTrainer) {
<h1>Entraînement : exercices tactiques</h1>
  <p>
  Scid offre la possibilité de résoudre des puzzles d'échecs. Pour cette fonctionnalité
  des bases de données spéciales contenant les puzzles sont nécessaires. Celles-ci
  les bases de données sont recherchées dans le répertoire <term> Bases </term>, qui peut
  être spécifié via le menu Options / Bases du répertoire. contrairement à d'autres
  bases ces bases ne contiennent généralement pas de jeux complets, mais juste un
  position de départ, comme on le sait dans le puzzle tactique habituel
  livres. En fait, chaque livre de puzzle peut facilement être converti en un tel
  base de formation en configurant simplement les positions et en les stockant dans
  le répertoire <term> Bases </term>. De plus, il faut régler le
  type de la base à <term> Tactics </term> au moyen du
  Fenêtre <a Maintenance> Maintenance </a>.
  </p>
  <p>
  Après avoir choisi Play / Training / Tactics, un dialogue apparaît
  permet de choisir une base d'entraînement. Là, le nom de la base est
  affiché, suivi du nombre de puzzles résolus et du total
  nombre de puzzles dans la base entre parenthèses. Autrement dit, Scid stocke le
  casse-tête que l'utilisateur a déjà résolu avec la base. Choisir une base et
  cliquez sur le bouton <term> Réinitialiser les scores </term> pour réinitialiser ce
  informations marquant tous les puzzles comme non résolus.
  </p>
  <p>
  <b> Remarque </b>, il est conseillé de fermer la fenêtre <a PGN> PGN </a> et de vérifier
  <term> masquer le prochain coup </term> dans le menu contextuel des informations
  zone de la fenêtre principale de Scid. Sinon, la tricherie est assez simple.
  </p>
  <p>
  Pour obtenir une solution pour les puzzles dans une base, les positions sont
  analysé par le moteur d'échecs Toga (inclus avec Scid). le
  Le curseur <term> Limiter le temps d'analyse du moteur </term> permet de limiter la
  temps le moteur doit résoudre la position. Plusieurs secondes devraient être
  assez pour la plupart des cas, car les puzzles tactiques contiennent généralement un
  poursuite forcée qui sera retrouvée très rapidement par presque tous
  moteurs d'échecs.
  </p>
  <p>
  Cliquez sur le bouton <term> Continuer </term> pour démarrer l'exercice,
  affichant le premier puzzle non résolu. Autrement dit, il est possible de
  continuer à travailler à travers une base. Aussi, si un puzzle a été sauté
  la première session ou non résolue, elle sera réaffichée. Une nouvelle fenêtre
  apparaîtra contenant une horloge et plusieurs boutons. L'horloge est
  seulement pour que l'utilisateur vérifie combien de temps il a pensé à la position à
  main. Aucune évaluation n'est effectuée sur le temps requis pour résoudre un problème.
  </p>
  <p>
  Certains exercices tactiques ne se terminent pas par un compagnon, mais le bon
  donne un net avantage. Si l'on veut jouer ces
  scénarios et ne comptez l'exercice résolu en cas de victoire, juste
  cochez l'option <term> Win won game </term>. (Cette option n'a pas
  ce qui signifie en cas de solution claire.)
  </p>
  <p>
  Si un problème ne peut être résolu, on peut vérifier le
  Case à cocher <term> Afficher la solution </term>. La solution s'affiche alors
  dessous dans le champ gris foncé.
  </p>
  <p>
  Cliquer sur le bouton <term> Suivant </term> permet à l'utilisateur de sauter un
  puzzle et passer à autre chose, le bouton <term> Abandonner </term> mettra fin à la tactique
  formation et fermer la base de données. Outre le <term> Suivant </term>
  bouton, l'utilisateur peut choisir un exercice par tous les moyens
  sélectionner un jeu dans une base de données dans Scid, par ex. en choisissant un aléatoire
  jeu via Ctrl-? ou sauter à un signet ou tout simplement par le
  <a GameList> liste de jeux </a>.
  </p>
  <p> <footer> (Mise à jour: Scid 3.6.26, octobre 2008) </footer> </p>
}

# Find best move 

set helpTitle(F,FindBestMove) "Entraînement : trouver le meilleur coup"
set helpText(F,FindBestMove) {
<h1>Entraînement : trouver le meilleur coup</h1>
  <p>
  Lors de l'annotation des parties avec un moteur d'échecs, le moteur peut rechercher
  des opportunités tactiques dans un jeu. Cela peut être réalisé en fixant
  <term>Marquer l'exercice tactique</term> dans la fenêtre
  <a Analysis>Annotation</a> accessible depuis l'analyse. Dans le cas où une
  opportunité tactique est trouvée, Scid signalera alors le jeu avec le drapeau 
  <term>T (Tactique)</term> et ajoutera un commentaire spécial qui est
  évalué dans le cadre de cet exercice.
  </p>
  <p>

  Cette méthode d'entraînement requiert une base de données correctement préparée.
  Ceci peut être réalisé en annotant par lot un ensemble de jeux. On peut aussi les
  télécharger à partir du
  <url http://scid.sourceforge.net/>Site web de Scid</url>.
  Après avoir ouvert une base de données correctement préparée, il suffit
  de sélectionner le menu
  <menu>Jouer</menu><menu>Entraînement</menu><menu>Exercices tactiques</menu>.
  Scid sautera ensuite à l'endroit suivant d'un coup tactique de la position de jeu
  actuelle. Si nécessaire, un nouveau jeu approprié sera chargé et Scid affichera
  la position critique. L'utilisateur est alors tenu de trouver la meilleure
  continuation. Pour sauter à la position tactique suivante, il suffit de faire un
  clic droit sur le bouton <button tb_BD_Forward> pour aller à la fin de partie.
  </p>
  <p>
  Pour cet exercice, il est conseillé de fermer la <a PGN>fenêtre PGN</a> et de
  <term>cacher le coup suivant</term> du
  <a MainWindow GameInfo>panneau d'information</a> (cf. menu contextuel).
  </p>
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

# FICS Login

set helpTitle(F,FICSLogin) "Connexion FICS"
set helpText(F,FICSLogin) {
<h1>Connexion FICS</h1>
  <p>
  Le serveur d'échecs gratuit sur Internet (FICS) est un bon endroit pour jouer aux échecs
  en ligne avec des gens du monde entier à des niveaux de jeu variés.
  Un des grands avantages est que l'on peut jouer contre des adversaires humains.
  De plus, FICS offre un système de classement qui permet de tester le
  et de surveiller ses propres progrès. En outre, les relais FICS
  des tournois internationaux qui peuvent être observés et discutés de la vie
  sur le serveur et il existe plusieurs options de formation. Pour un
  description détaillée des services offerts, sur la façon de devenir un
  membre de la FICS, etc., veuillez vous référer à la page d'accueil de la FICS à l'adresse
  http://www.freechess.org.
  </p>
  <p>
  Bien qu'en principe FICS puisse être utilisé avec un programme telnet simple
  Jouer avec un vrai programme d'échecs offre plusieurs avantages.
  Ainsi, Scid offre une interface à ce serveur. Pour établir une
  connexion au serveur, il faut d'abord se connecter et plusieurs
  sont nécessaires :
  <ul>
      <li><term>Login:</term> Ceci est votre nom de connexion sur la FICS
      serveur. Sa valeur est stockée dans la configuration de Scid pour une utilisation ultérieure
      réutilisation.</li>
      <li><term>Mot de passe:</term> spécifie votre mot de passe pour la connexion.
      <b>Note</b> que ce mot de passe est affiché lorsque vous le tapez,
      permettant à tout le monde de le lire. De plus, ce mot de passe est
      stocké en texte clair dans la configuration de Scid. Si vous êtes
      en utilisant un système accessible au public, veuillez vous assurer d'assigner
      les droits appropriés au répertoire de configuration de Scid.
      </li>
      <li><term>Sceau de temps</term> si vérifié, toutes les connexions à la
      Les serveurs FICS sont acheminés par le biais du <term>timeseal</term>
      programme. Le but de <term>semaine</term> est de faire face aux
      de réseau et de maintenir les horloges dans le bon ordre, c'est-à-dire
      que vous ne devriez pas perdre un match à l'heure simplement parce que vous êtes sur
      une connexion réseau très lente. Le système
      <term>sous-marche </term> programme pour votre système d'exploitation doit
      être sélectionné dans la ligne d'entrée suivante. Il est disponible gratuitement,
      mais doit être téléchargé séparément de
      http://www.freechess.org.
      </li>
      <li><term>Port du serveur</term> spécifie le port sur le serveur.
      La valeur par défaut est 5000 et devrait être correcte pour presque tous les besoins.</li>
      <li><term>Port de synchronisation</term> spécifie le port où le
      <term>sous-marche </term> le programme est à l'écoute. La valeur par défaut est 5001
      et devrait convenir pour presque tous les besoins.</li>
   </ul>
   Tous ces réglages sont mémorisés avec la configuration de Scid et
   ne doivent donc être saisis qu'une seule fois.
   </p>
  <p>
  Après avoir vérifié les paramètres, vous pouvez vous connecter au serveur FICS en
  en cliquant sur le bouton <term>Connect</term>. Sinon, Sicd propose
  un login anonyme en choisissant <term>Login en tant que guest</term>. Dans ce
  vous pouvez y essayer toutes les fonctions de FICS de façon anonyme, mais
  vous ne pourrez pas jouer à des jeux classés ou obtenir un classement
  toi-même. Pour la différence entre le compte invité et le compte habituel
  veuillez vous référer à la documentation sur la page d'accueil de la FICS.
  </p>
  <p>
  <b>Note</b> En plus des échecs standards, FICS offre aussi un lot d'échecs
  des variantes comme bughouse ou crazyhouse etc. Scid ne supporte aucun
  de ces variantes, donc vous ne pouvez pas les jouer en utilisant Scid. Si vous
  vous voulez jouer des variantes, vous voudrez peut-être aller voir d'autres interfaces.
  FICS propose un ensemble d'interfaces graphiques à télécharger sur son site
  page d'accueil.
  </p>
  <p><footer>(Mise à jour : Scid 3.6.26, octobre 2008)</footer></p>
}

# FICS Find Opponent

set helpTitle(F,FICSfindOpp) "FICS Trouver un adversaire"
set helpText(F,FICSfindOpp) {
<h1>FICS Trouver un adversaire</h1>
  <p>
  Ce dialogue offre un moyen pratique de mettre les autres joueurs au défi de
  un jeu, dans FICS parlez à <term>placez un seek</term>. Pour cela, plusieurs
  Les paramètres doivent être réglés :
  <ul>
      <li><term>Temps initial (min)</term> définit le temps initial pour
      le jeu en quelques minutes. </li>
      <li><term>Increment (sec)</term> définit l'incrément par coup,
      c'est l'équivalent d'un <term>Fisher Clock</term>. En le réglant sur
      0 fera durer le jeu <term>Temps initial</term> minutes.
      </li>
      <li><term>jeu coté</term> Si coché, le jeu sera coté,
      décochez-le pour jouer à un jeu de loisir. </li>
      <li><term>color</term> Si elle est réglée sur automatique, c'est à la
      adversaire de choisir la couleur qu'il veut jouer, vous accepterez
      soit il choisit. Si vous voulez jouer au noir ou au blanc explicitement,
      marquer l'un ou l'autre de ces choix.
      </li>
      <li><term>Limiter la cote entre</term> permet de proposer un jeu
      aux joueurs dans l'intervalle de classement donné. En utilisant cette option, vous pouvez
      vous assure de jouer des adversaires dans votre fourchette de classement et ainsi
      en train de faire un bon match.
      </li>
      <li><term>Confirmer manuellement</term> permet de confirmer ou de refuser
      le jeu, une fois qu'un autre joueur a accepté votre défi.</li>
      <li><term>Filtre avec formule</term> Sur FICS chaque joueur peut
      établir une formule qui décrit les contestations qui seront refusées
      automatiquement. Cette formule peut être activée en cochant cette case.
      </li>
   </ul>
   En cliquant sur le bouton <term>Issue seek</term>, vous placerez votre
   sur le serveur.
  </p>
  <p><footer>(Mise à jour : Scid 3.6.26, octobre 2008)</footer></p>
}

# FICS

set helpTitle(F,FICS) "FICS : Jouer sur Internet"
set helpText(F,FICS) {
<h1>FICS : Jouer sur Internet</h1>
  <p>
  La fenêtre est divisée horizontalement en deux parties principales. En haut, la
  La fenêtre de la console est affichée. Ici, tous les messages envoyés au serveur ou
  récupérées sur le serveur se présentent. Les messages normaux sont écrits en
  vert, les offres de jeu des autres joueurs apparaissent en rouge, les commandes sont envoyées
  au serveur sont précédés d'une invite.
  </p>
  <p>
  L'interaction avec le FICS est pilotée par les commandes. Par conséquent, en dessous de la
  La fenêtre de la console est la ligne de commande. Ici, l'utilisateur peut entrer des commandes
  envoyé au serveur en appuyant soit sur la touche Entrée, soit sur la touche <term>send</term>
  bouton. La description de toutes les commandes de FICS dépasse la portée de ce
  Cependant, la FICS offre un système d'aide en ligne très complet. À
  voir une liste des commandes possibles que l'on peut émettre <term>help
  commandes</term>. Pour voir la documentation détaillée d'une commande
  préfixer son nom par <term>help</term>. Notez qu'un
  peut avoir plusieurs pages d'aide. Pour faire défiler vers le bas, lancez la commande
  Commande <term>suivant</term>.
  </p>
  <p>
  Sous la ligne de commande, le dialogue est divisé en deux parties. Sur la ligne de commande
  à gauche, les horloges se montrent, une pour les blancs et une pour les noirs.
  Cliquer sur une horloge l'arrêtera, à l'intérieur des horloges analogiques une
  la représentation numérique est montrée.
  </p>
  <p>
  Sur la droite des horloges apparaissent les boutons de commande :
  <ul>
    <li><term>Silence</term> filtrera la plupart des messages sur
    FICS qui ne s'adressent pas explicitement à l'utilisateur. Il s'agit de
    équivalent à l'envoi des commandes FICS 
    <ul>
    <li>set gin 0</li>
    <li>set seek 0</li>
    <li>set silence 0</li>
    <li>set chanoff 1</li>
    </ul>
    </li>
    <li><term>Offres</term> affichera un affichage graphique de la
    les offres de jeux actuelles. L'axe des y indique le classement Elo du
    l'adversaire (plus son niveau est élevé, plus l'offre apparaît en haut),
    tandis que l'axe des x du graphique montre le timing des jeux (la
    plus à droite, plus le jeu est lent). La première ligne rouge
    marque standard <term>Blitz</term> timing (5 min., sans incrément)
    tandis que la deuxième ligne rouge marque le standard <term>Rapide</term>
    (15 min., sans incrément). De plus, les offres utilisent le
    après le codage :
    <ul>
       <li><green>Vert</green> : offres des adversaires humains</li>
       <li><blue>Bleu</blue> : offres des adversaires de l'ordinateur</li>
       <li><red>Rouge</red> : jeux avec un temps total de plus de 1 heure</li>
       <li><gray>Gray</gray> : offres anonymes, c'est à dire des offres provenant de logins invités</li>
       <li>Boîtes : jeux non notés</li>
       <li>Circles : jeux classés</li>
    </ul>
    </li>
    <li> En passant la souris sur une offre spécifique, vous pouvez voir tous les détails
      dans une forme textuelle :
    <ul>
       <li>le numéro de jeu</li>
       <li>les joueurs se connectent handle</li>
       <li> la note du joueur entre parenthèses</li>
       <li>le temps initial / l'incrément de temps</li>
       <li>le mot "noté" ou "non noté" selon l'offre</li>
       <li>le mot " [blanc] " ou " [noir] " si l'adversaire le souhaite
         jouer la couleur spécifiée seulement</li>
    </ul>
    Pour accepter une offre, il suffit de cliquer sur le symbole.</li>
  <li><term>Trouver l'adversaire</term> ouvre un <a FICSfindOpp>dialogue</a>
  qui permet de placer une offre.</li>
  <li><term>Draw</term> offre un draw dans la partie en cours</li>
  <li><term>Abandonner</term> abandonne la partie en cours</li>
  <li><term>Démission</term> Démissionne de la partie en cours</li>
  <li><term>Takeback</term> reprend un demi-coup, utile dans une analyse</li>
  <li><term>Takeback 2</term> reprend un coup complet, utile dans une analyse</li>
  <li><term>Games</term> donne une liste des jeux en cours sur le serveur.
  Notez que la console FICS est considérée comme ayant 80 caractères de large, donc,
  pour obtenir une belle table, il faudrait redimensionner la fenêtre FICS.
  </li>
  <li><term>Fermer</term> ferme la connexion à FICS</li>
  </ul>
  </p>

  <h3><name Training>Conférences FICS</name></h3>
  <p>
  FICS offre plusieurs options pour l'entraînement aux échecs. L'une des plus
  les bots de conférence <term>LectureBot</term> et
  <term>WesBot</term>. Ils tournent tout le temps sur FICS et offrent diverses
  qui peuvent être visitées à l'aide de Scid. Le début de chaque
  est annoncée sur le <term>Canal 67</term> de la FICS. Par conséquent,
  pour voir ces annonces, il faut d'abord ajouter ce canal à la liste
  liste d'observation personnelle. Cela peut être fait par <term>+canal
  67</term> (il peut être supprimé à nouveau par <term>-canal 67</term>).
  Une fois que par exemple LectureBot annonce une session de formation, on peut participer
  en émettant <term>observe lecturebot</term>. Veuillez vous référer à la
  la documentation en ligne de FICS pour les fonctionnalités supplémentaires des Bots et
  également d'autres robots disponibles.
  </p>

  <h3><name Observe>Observez les parties sur FICS</name></h3>
  <p>
  De temps à autre, FICS <term>relais</term> événements majeurs en
  les échecs internationaux. Dans ces cas, on peut observer la vie des parties
  sur le serveur et en discuter avec d'autres utilisateurs sur FICS. Pour savoir
  quels sont les jeux actuellement relayés (s'il y en a) ; on peut demander au relais
  une liste par <term>relay listgames</term>. Comme la FICS s'attend à ce que le terminal
  pour avoir 80 caractères de largeur, il peut être nécessaire de redimensionner les FICS de Scid
  pour avoir une belle table. Les entrées de la table sont, tout d'abord, les
  numéro de jeu, deux points, que les adversaires, le résultat du jeu (*
  signifiant un jeu en cours comme d'habitude) et le <a ECO>code ECO</a> de
  l'ouverture. Pour observer un jeu spécifique, on peut simplement <term>observer
  numéro de jeu</term>. Scid affichera alors la carte courante
  les horloges afficheront les valeurs correctes envoyées par le
  et il récupère tous les coups de la partie jusqu'à présent, plus les
  <a PGN>PGN</a> champs d'en-tête. Si un joueur se déplace, ce coup est effectué dans Scid
  comme d'habitude. Pour discuter du jeu avec d'autres observateurs,
  <term>whisper</term> et <term>kibitz</term> peuvent être utilisés. S'il vous plaît
  se référer à l'aide en ligne de ces commandes. Toutes ces conversations
  peut être lu dans la console.
  </p>
  <p>
  Notez qu'un seul jeu peut être observé au sein de Scid.
  </p>

  <p><footer>(Mise à jour : Scid 3.6.26, octobre 2008)</footer></p>
}

# Book tuning

set helpTitle(F,BookTuning) "Configuration d'une bibliothèque d'ouvertures"
set helpText(F,BookTuning) {
<h1>Configuration d'une bibliothèque d'ouvertures</h1>
  <p>
  La <term>configuration d'une bibliothèque d'ouvertures</term> permet d'ajuster la
  <term>probabilité attribuée à chaque coup</term> dans cette
  <a Book>bibliothèque d'ouvertures</a>. Cette valeur indique la probabilité
  en pourcentage que Scid utilisera ce coup lors d'un match. Pour la configurer,
  <ul>
    <li>En premier, <b>les pièces de l'échiquier doivent être placées dans la position</b>
    où les lignes de jeu doivent être ajustées. Pour naviguer à travers les branches de
    la bibliothèque, il suffit de cliquer sur le coup suivant de la ligne de jeu
    dans la fenêtre d'ajustement ou <a Moves>se déplacer dans la partie</a>
    comme d'habitude dans Scid.</li>
    <li>Le menu <menu>Outils</menu><menu>Config. bibliothèque</menu> ouvre la fenêtre
    indiquant les coups en bibliothèque et leurs pourcentages.</li>
    <li>Il suffit d'<term>ajuster la probabilité d'un coup</term>, par exemple augmenter
    la probabilité d'une seule variante. Bien que les autres valeurs restent les mêmes,
    Scid recalculera tout au moment d'enregistrer.</li>
  </ul>
  </p>
  <p>
  <b>Note.</b> Seules les valeurs entières sont affichées, donc un 0 peut signifier
  que ce coup a une probabilité de "moins que 1%". Cela se produit certainement
  dans les grandes collections de jeux. Tous les chiffres s'additionnent à 100% bien sûr.
  </p>
  <p>
  Le bouton <b>[Exporter]</b> permet
  d'<term>exporter une branche de la bibliothèque</term> de la position actuelle
  en une seule partie. La suite avec la plus grande probabilité constituera
  la ligne principale tandis que toutes les autres sont stockées dans des
  variantes. Cela permet de sélectionner manuellement les lignes de jeu à inclure
  pour la <term>création d'une nouvelle bibliothèque</term>.
  </p>
  <p>
  <b>Note.</b> Scid peut gérer 3000 coups dans une seule partie, donc il ne sera
  probablement pas possible ni très judicieux d'exporter toute une bibliothèque
  d'ouvertures en un seul jeu. Notez également que l'export peut être fait de façon
  progressive. C'est-à-dire que de nouvelles lignes sont ajoutées aux lignes déjà
  existantes. Cela permet également de
  <term>fusionner plusieurs bibliothèques d'ouvertures</term>.
  </p>
<p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

# Novag Citrine

set helpTitle(F,Novag) "Connexion un échiquier Novag Citrine"
set helpText(F,Novag) {
<h1>Connexion un échiquier Novag Citrine</h1>
   <p>
   L'échiquier Novag Citrine est un échiquier en bois qui peut être interfacé
   avec un ordinateur PC par le biais d'une connexion série. Il peut être utilisé
   avec Scid pour saisir les coups d'une partie de jeu, jouer contre un adversaire
   informatique ou sur le <a FICS>serveur FICS</a>
   en offrant une interface d'échecs "naturelle".
   </p>
   <p>
   <b>Note</b> : Avant de pouvoir utiliser l'échiquier, il faut
   <a HardwareConfig>configurer le port à utiliser.</a>
   </p>
   <p>
     Une fois que le bon port est défini, choisissez le menu
 <menu>Outils</menu><menu>Connecter un matériel</menu><menu>Connecter Novag Citrine</menu>
     pour connecter l'échiquier, ou cliquez simplement sur le bouton de connexion
     matérielle <button tb_eng_disconnected>. Ce bouton
   indique également <a HardwareStatus>l'état de la connexion.</a>

   ###--- Description détaillée nécessaire ---###
   </p>
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}


set helpTitle(F,HardwareConfig) "Connexion de matériel externe"
set helpText(F,HardwareConfig) {
<h1>Connexion de matériel externe</h1>
  Il y a deux types de matériel qui peuvent être utilisés avec Scid pour jouer
  ou des jeux d'entrée. Sélectionnez dans la liste <term>Hardware</term> celui
  que vous souhaitez utiliser :
  <ul>
    <li><term>Novag Citrine</term> (et les cartes compatibles) utilisent un
    pilote interne qui a été développé spécifiquement pour la Novag
    Citrine board, mais il peut exister d'autres conseils qui soutiennent
    ce protocole.  Ce driver permet d'entrer des coups, mais les cartes
    la logique ne reconnaît pas la position établie sur le tableau ni la
    pièces en tant que telles.
    </li>
    <li><term>Input Engine</term> les pilotes sont censés être gratuits,
    interface ouverte pour connecter n'importe quel matériel à une interface graphique comme Scid. Il
    est modelé sur le style habituel d'un moteur d'échecs. Le site
    spécifications du protocole ainsi qu'un pilote approprié pour
    les échiquiers électroniques DGT peuvent être trouvés sur le site
    <url http://dgtdrv.sourceforge.net>site web dedgtdrv</url>.  En fonction de
    le matériel ; la carte peut connaître les positions mises en place et les
    pièces par leur coup. C'est par exemple le cas pour les plateaux de la DGT.
    </li>
  </ul>
  <p>
  <b>Note</b> Scid n'inclut pas de moteur d'entrée par défaut. Entrée
  sont conçus comme des programmes indépendants qui peuvent être utilisés avec un
  variété d'interfaces graphiques.
  </p>
  <p>
  Après avoir sélectionné le matériel pour utiliser au moins le port, ce matériel
  est connecté à, doit être spécifié. En fonction de l'utilisation
  et la carte pour utiliser ce port varie. Cependant, habituellement
  le matériel externe est connecté par un port série, USB ou BlueTooth
  où les deux derniers ne sont que des dérivés de la connexion sérielle
  connexions. En fonction de la plate-forme, les noms habituels du port en
  les questions sont :
  </p>
  <ul>
    <li><term>Unix</term> (y compris <term>Linux</term>) : généralement un fichier
    dans le répertoire <term>/dev</term> est utilisé. Pour les ports série, le répertoire
    Le nom commun est <term>/dev/ttyS0</term>, pour la première série
    port, <term>/dev/ttyS1</term>, pour la seconde et ainsi de suite. Pour
    Les noms communs des périphériques USB série comprennent <term>/dev/ttyUSB0</term>
    pour le premier, <term>/dev/ttyUSB1</term> pour le second et ainsi de suite.
    D'autres noms moins courants sont <term>/dev/usb/tts/*</term> ou
    <term>/dev/usbdev*</term> (où l'astérisque * représente certains
    nom supplémentaire). Les appareils BlueTooth apparaissent généralement sous la forme
    <term>/dev/rfcomm0</term> pour le premier, <term>/dev/rfcomm1</term>
    pour la seconde et ainsi de suite.
    </li>
    <li><term>MacOS</term> :
    ###--- détails pour le nommage sous Mac OS X requis ---###
    </li>
    <li><term>Windows</term> suit la convention DOS habituelle, où
    les ports série sont étiquetés <term>COM1:</term>,
    <term>COM2:</term> et ainsi de suite. Ce nommage s'applique également à
    mais ils obtiennent parfois des chiffres supérieurs à 4.
    </li>
    <li><term>Show button</term> activera 
    <a HardwareStatus>bouton de connexion du matériel</a> dans la barre d'outils si
    vérifié. S'il n'est pas coché, le bouton n'apparaîtra pas pour économiser de l'espace.
    La suppression du bouton doit redémarrer Scid.
    </li>
  </ul>
  <p>
  <b>Note</b> : si une carte est connectée au moyen d'un USB à un port série ou
  BlueTooth en convertisseur série, ces adaptateurs apparaissent généralement dans le
  de l'autre côté de la frontière.
  </p>
  <p>
  Si un pilote compatible Input Engine doit être utilisé, les éléments suivants
  Les champs doivent être remplis (pour le pilote Novag, ils sont
  désactivé) :
  </p>
  <ul>
     <li><term>Commande moteur</term> : Le nom du programme qui est
     pour être utilisé comme un moteur de conduite. Un nom pleinement qualifié peut être
     requis, comme dans la configuration d'un
     <a Analysis List>moteur d'échecs</a>.
     (Par exemple, le moteur du pilote DGT sur un
     est généralement appelé <term>dgtdrv2.i686</term>).
     </li>
     <li><term>Paramètre moteur</term> : Le texte donné dans ce champ
     est transmis en tant que paramètres au moteur du conducteur. Les valeurs
     requises ici dépendent du conducteur et doivent être indiquées dans son
     documentation. (Par exemple, le moteur du pilote de la DGT a besoin d'un
     code spécifiant l'orientation du plateau et les coups qui doivent
     être envoyé au GUI. La première lettre peut être <term>l</term> ou
     <term>r</term> en fonction de l'horloge assise à gauche des Blancs ou
     à droite, la deuxième lettre peut être <term>a</term>, <term>b</term> ou
     <term>w</term> selon que tous, seulement les noirs ou seulement les blancs
     les coups doivent être envoyés).
     </li>
  </ul>
  <p>
  En appuyant sur <term>OK</term>, la configuration sera stockée dans la configuration de Scid
  et démarrer immédiatement le pilote sélectionné.
  </p>
  <p>
  L'état de la carte externe peut être surveillé par le
  <a HardwareStatus>Etat du matériel</a> bouton. Ce bouton peut aussi être
  utilisé pour connecter immédiatement un matériel déjà configuré sans
  en appelant le dialogue <term>Configuration</term>.
  </p>


  <p><footer>(Mise à jour : Scid 3.6.27, octobre 2008)</footer></p>
}


set helpTitle(F,HardwareStatus) "État du matériel externe"
set helpText(F,HardwareStatus) {
<h1>État du matériel externe</h1>
   <p>
   Ce bouton est situé à droite de la barre d'outils de Scid. En fonction de
   l'état du matériel externe, il affiche différentes icônes :
   </p>
   <ul>
   <li><button tb_eng_disconnected>
   C'est l'état normal après le démarrage du programme et montre qu'aucun
   matériel externe n'est actif pour le moment. Appuyer sur ce bouton
   connectera le matériel configuré.
   </li>
   <li><button tb_eng_connecting>
   Scid a démarré la communication avec le matériel externe,
   L'initialisation est en cours. En fonction du pilote et du matériel
   connectés, ça peut prendre quelques secondes pour que les choses
   se mettent en ordre.
   </li>
   <li><button tb_eng_error>
   Lors de la communication avec le matériel externe, une erreur s'est produite.
   Les <term>sources habituelles de problèmes matériel</term> sont, par exemple,
   qu'un autre programme à l'accès au matériel externe, que le matériel n'est
   pas connecté au PC ou simplement n'est pas allumé. Dans de rares cas,
   le matériel peut également se comporter de manière inattendue. Tenter de se
   reconnecter pourrait remédier au problème car le matériel est généralement
   réinitialisé lors de la déconnexion.
   Il est également possible de débrancher et/ou d'éteindre l'appareil dans
   ce cas. En appuyant sur ce bouton, Scid essaie de nouveau de se brancher
   sur le matériel configuré.
   </li>
   <li><button tb_eng_ok>
   La communication est établie, l'appareil est prêt et peut être
   utilisé pour la saisie de coup. Une pression sur ce bouton déconnectera le
   matériel externe.
   </li>
   <li><button tb_eng_dgt>
   Dans le cas où un échiquier électronique DGT est connecté et prêt, ce bouton
   apparaît. En appuyant sur ce bouton, vous déconnectez le
   matériel externe.
   </li>
   <li><button tb_eng_query>
   La boîte de dialogue <a HardwareConfig>configuration du matériel</a>
   est ouvert. Terminez la configuration avant de raccorder le matériel.
   </li>
   </ul>

  <p><footer>(Mise à jour : Scid 3.6.27, octobre 2008)</footer></p>
}


set helpTitle(F,InputEngine) "Console du pilote du moteur d'entrée"
set helpText(F,InputEngine) {
<h1>Console du pilote du moteur d'entrée</h1>
   <p>
   En haut de cette fenêtre, une console s'affiche qui surveille le
   communication entre Scid et le moteur du conducteur. D'habitude, c'est
   d'aucun intérêt pour l'utilisateur, mais il peut fournir des conseils utiles dans
   cas de dysfonctionnement. Toutes les commandes envoyées par Scid sont précédées d'un
   flèche vers la droite, toutes les réponses récupérées du moteur du conducteur par
   une flèche vers la gauche.  Sous la console, l'invocation réelle de
   le pilote de Scid se présente pour information.
   </p>
   <p>
   <term>Coups envoyés pour</term> permet de limiter les coups envoyés par le
   matériel externe. Pour l'entrée du jeu, le tableau doit bien sûr envoyer
   les coups pour les deux côtés. Pour jouer une partie, cependant, le
   les coups de l'adversaire doivent être ignorés, comme le ferait Scid
   sinon les notifier comme "coups illégaux".
   </p>
   <p>
   Le petit tableau de droite indique la position actuelle.
   </p>
   <p>
   Si un déplacement est effectué, dans la zone entre les boutons et le petit
   l'échiquier le coup effectué (<term>surface du coup</term>) sur le
   Le tableau est représenté en notation algébrique longue de la figurine. Coups valides
   apparaissent sur un fond <green>vert</green> alors que des coups illégaux
   apparaissent sur un fond <red>rouge</red>. De plus, les coups illégaux
   sont annoncés par le son d'alerte, si Scid a activé le son.
   Selon les réglages, les coups sont également annoncés.
   </p>
   <p>
   Le bouton <term>Info</term> appelle les informations du pilote
   moteur. L'étendue de ces informations dépend du moteur, mais
   il devrait généralement donner autant que possible sur le
   et le pilote lui-même. De plus, le pilote externe
   renverra la position actuelle sur la carte externe en <term>notation FEN</term> qui
   est ensuite comparée à la représentation interne de Scid. Divergences
   se montrer dans la console et émettre une alerte.
   </p>
   <p>
   Au cas où Scid et la carte externe seraient désynchronisés, le
   Le bouton <term>Synchroniser</term> peut être utilisé. Un nouveau jeu est mis en place
   et la position de départ est réglée sur la position sur le
   conseil d'administration.  </p>
   <p>
   Le bouton <term>Fermer</term> déconnecte finalement le
   et ferme la console.
   </p>

   <h2>Caractéristiques spéciales</h2>
   <p>
   Les caractéristiques suivantes dépendent du moteur et du matériel du pilote
   qui est utilisé. Leur description est basée sur les fonctions disponibles
   avec le <url http://www.dgt-projects.com>DGT Échecs électroniques
   Board</url> et spécifié dans le
   <url http://dgtdrv.sourceforge.net>Protocole du moteur d'entrée</url>
   description. Certaines de ces fonctions reposent toutefois sur le fait que
   le matériel est capable de reconnaître les pièces sur l'échiquier et leur
   placement actuel.
   </p>

   <h3>Configuration de la position</h3>
   <p>
   Le mode de réglage de la position est invoqué en retirant les deux rois de la
   conseil d'administration. Dans ce mode, le <term>déplacement de la zone</term> est coloré en jaune
   et montre le roi blanc et la corde <b>Setup</b>.
   </p>
   <p>
   En mode de réglage, il est possible de régler n'importe quelle position valide sur le
   mais la représentation interne de Scid n'est pas mise à jour tant que
   fini.
   </p>
   <p>
   Pour mettre fin au mode setup, les deux rois sont remis sur la planche. Note,
   que le roi du camp à déplacer est en retrait <b>dernier</b>, c'est-à-dire
   si les Blancs doivent être en coup, faites d'abord reculer le roi noir, puis
   le roi blanc. Maintenant, la <term>zone de coup</term> montre la dame noire
   et la chaîne de caractères <b>OK</b>.
   </p>

   <h3>Finir une partie</h3>
   <p>
   Dans l'entrée de jeu ou dans le jeu, on peut terminer la partie en cours en réglant
   les deux rois dans les cases centrales du plateau. Le résultat est
   donné comme suit :
   </p>
   <ul>
      <li><term>1-0</term> (victoire des Blancs) : Mettez les deux rois sur les cases
      e4 et d5 dans n'importe quel ordre, c'est-à-dire vers les cases centrales blanches.
      L'aire de jeu est colorée en blanc, montrant le Roi blanc et
      le résultat.
      </li>
      <li><term>0-1</term> (victoire des Noirs) : Mettez les deux rois sur les cases
      e5 et d4 dans n'importe quel ordre, c'est-à-dire vers les cases centrales noires.
      L'aire de coup est colorée en gris, montrant le Roi noir et
      le résultat.
      </li>
      <li><term>1/2-1/2</term> (Draw) : Mettez un roi sur une case centrale blanche et
        l'autre roi sur une case centrale noire dans n'importe quel ordre. 
        L'aire de déplacement est colorée en noir et blanc, montrant le résultat.
      </li>
   </ul>
   <p>
   Dans les deux cas, le dialogue <term>Save / Add game</term> apparaît
   pour permettre de remplir les balises de jeu et de stocker le jeu dans le
   base de données actuelle.
   </p>

   <h3>Démarrer une nouvelle partie</h3>
   <p>
   Un nouveau jeu peut être regardé en entrant dans le mode de configuration (c'est-à-dire en supprimant les deux
   rois) et la mise en place de la position de départ. Pour signaler, que Scid a
   a reconnu l'appel pour un nouveau jeu le <term>déplacer zone</term> est
   coloré en bleu montrant le roi blanc et la corde
   <term>OK</term>.
   </p>


  <p><footer>(Mise à jour : Scid 3.6.27, octobre 2008)</footer></p>
}


set helpTitle(F,ReviewGame) "Entraînement : reviser une partie"
set helpText(F,ReviewGame) {
<h1>Entraînement : reviser une partie</h1>
<p>
Cette fonctionnalité est conçue pour étudier une partie en essayant de deviner les coups
joués pendant le match. Donc, vous ouvrez d'abord une partie à partir de n'importe
quelle base de Scid, et sélectionner cette fonction avec la commande
<menu>Reviser la partie</menu> dans le menu <menu>Jouer</menu>. Comme d'habitude, pour
jouer avec les Noirs, vous devez d'abord <term>retourner l'échiquier</term> car
vous jouez toujours du bas du plateau vers le haut.
</p>
<p>
Pour un entraînement efficace, les paramètres suivants doivent être
désactivés :
<ul>
  <li>Enlever l'<term>affichage du dernier coup</term> par le menu
  <menu>Options</menu><menu>Coups</menu><menu>Surbrillance dernier coup</menu>.</li>
  <li><term>Cacher le coup suivant</term>, voir le menu contextuel
  <menu>Cacher le coup suivant</menu> du panneau d'information
  (cf. <menu>Fenêtres</menu><menu>Panneau d'information</menu>).</li>
  <li>Et enfin, fermer la fenêtre de notation PGN.</li>
</ul>
</p>
<p>

Le processus est le suivant:
<ul>
<li>- les moteurs réfléchissent au coup joué pendant la partie,</li>
<li>- le moteur réfléchit à la position et trouve le meilleur coup,</li>
<li>- l'utilisateur est alors autorisé à entrer son coup,</li>
<li>- le moteur réfléchit en coup joué par l'utilisateur,</li>
</ul>
</p>
<p>
Si le coup joué par l'utilisateur est soit le coup réellement joué
pendant la partie ou le coup choisi par le moteur ou un coup dont le score
est proche du meilleur coup trouvé par le moteur (en utilisant la marge d'erreur
définie par l'utilisateur), le coup est alors considéré comme bon. Si l'utilisateur
n'est pas d'accord avec le moteur, il peut utiliser le bouton
<b>[Utiliser le temps étendu]</b> pour obtenir une confirmation de l'analyse du moteur.
</p>
<p>
<b>Remarques :</b>
<ul>
<li>- Le camp Blancs ou Noirs doit être choisi avant la révision de la partie.
  Pour jouer les Noirs, il faut donc <term>retourner l'échiquier</term>.</li>
<li>- L'utilisateur joue toujours à partir du bas du plateau vers le haut.</li>
</ul>
</p>
  <p><footer>(Mise à jour: Scid 4.7.1, janvier 2020)</footer></p>
}


set helpTitle(F,Metadata) "Métadonnées du noyau de Dublin"
set helpText(F,Metadata) {
<h1>Métadonnées du noyau de Dublin</h1>
   <p>
     L'élément de métadonnées <url http://dublincore.org>Dublin Core</url>
   définit un moyen standardisé et largement utilisé pour échanger
   l'information sur les ressources électroniques (métadonnées, c.-à-d. les données sur
   données). Le Dublin Core a été défini en 2003 par la norme ISO 15836, et
   Norme NISO Z39.85-2007.
   </p>
   <p>
   Certaines bases de données utilisées avec Scid peuvent contenir des
   des informations sur les métadonnées. Ces informations peuvent se référer à la licence de
   une base de données, la source de la base de données (dans le cas où elle est publiée sur
   le Web), la date d'une base de données et ainsi de suite.
   </p>
   <p>
   Ces informations peuvent être consultées ou ajoutées à une base de données par
   en choisissant <term>Informations de la base de données</term>. Une nouvelle fenêtre s'ouvre
   qui montre les éléments individuels du Dublin Core et permet de modifier
   eux.
   </p>
   <p>
   En interne, les métadonnées sont stockées dans des fichiers <term>sme</term> qui peuvent
   viennent avec une base de données. Si aucun fichier <term>sme</term> n'existe
   l'ouverture du dialogue <term>Informations de la base de données</term> définira
   valeurs par défaut basées sur la base de données actuelle. Ceci est signifié par
   les étiquettes apparaissant dans <blue>blue</blue>.
   </p>
   <p>
   Après avoir choisi <term>OK</term> Scid stocke les métadonnées dans le
   <term>sme</term> fichier pour utilisation avec Scid. De plus, un fichier

   <term>XML</term> est généré et contient exactement le même
   mais en format XML pour faciliter l'échange de données.
   </p>
   <p>
   <b>Note</b> : Scid ne se soucie pas du fichier XML lui-même. Il utilise
   uniquement le fichier <term>sme</term> pour ses procédures internes.
   Cependant, d'autres applications pourraient bien préférer la structure XML.
   </p>

  <p><footer>(Mise à jour : Scid 3.6.27, octobre 2008)</footer></p>
}


set helpTitle(F,Appearance) "Personnaliser l'interface de Scid"
set helpText(F,Appearance) {
<h1>Personnaliser l'interface de Scid</h1>
  <p>
  Scid offre plusieurs options pour personnaliser l'interface graphique.
  </p>
  <h3><name Themes>Configurer les thèmes TTK</name></h3>
  <p>
  A partir de la version 4.7, de nouveaux thèmes peuvent être installés et
  sélectionnés. Les <term>thèmes TTK</term> personnalisent l'apparence
  de l'interface utilisateur. Le dépôt Scid fournit un ensemble
  de thèmes multicolores pour une utilisation directe :
  <ul>
  <li><url https://sourceforge.net/projects/scid/files/Scid/Additional Files/>https://sourceforge.net/projects/scid/files/Scid/Additional Files</url>
  -- Home/Scid/Additional Files</li>
  </ul>
  D'autres thèmes peuvent également être téléchargés aux adresses suivantes :
  <ul>
  <li><url https://wiki.tcl.tk/48689>https://wiki.tcl.tk/48689</url>
    -- wiki.tcl.tk Liste des thèmes de ttk</li>
  <li><url https://github.com/RedFantom/ttkthemes/tree/master/ttkthemes/themes>https://github.com/RedFantom/ttkthemes</url>
    -- Collection of thèmes TTK RedFantom</li>
  </ul>
  </p>
  <p>
  Pour installer les thèmes Scid, procédez comme suit :
  <ul>
  <li>1. Extrayez les fichiers scidthemes dans n'importe quel répertoire.</li>
  <li>2. Charger les thèmes avec le menu
  <menu>Options</menu><menu>Apparence</menu><menu>Charger thème(s)...</menu>
  et sélectionnez le fichier pkgIndex.tcl dans le répertoire dans lequel les
  thèmes sont stockés.</li>
  <li>3. Sélectionnez le thème à utiliser en choisissant son nom dans le même menu
  <menu>Options</menu><menu>Apparence</menu><menu>"Nom du thème"</menu>,
  par exemple scidblue scidmint etc.</li>
  </ul>
  </p>
  <p><b>Note :</b> pkgIndex.tcl sont des fichiers qui chargent des paquets tcl
  supplémentaires. N'utilisez que les fichiers pkgIndex.tcl qui contiennent
  des thèmes TTK.
  </p>
  <h3><name MenuColors>Couleurs des menus</name></h3>
  <p>
  Les thèmes TTK n'ont techniquement pas de propriétés et de couleurs adaptées pour
  les menus qui doivent donc être ajustées séparément et adaptées aux couleurs des
  thèmes en utilisant le menu <menu>Options</menu><menu>Préférences...</menu>. Les
  choix disponibles sont les suivantes :
  <ul>
  <li>- couleur de fond du menu principal,</li>
  <li>- couleur de fond et d'arrière-plan de tous les menus</li>
  <li>- couleur de fond et d'arrière-plan de l'élément de menu sélectionné</li>
  <li>- couleur d'une option sélectionnée dans le menu</li>
  </ul>
  </p>
  <p><b>Note :</b> les systèmes MS Windows et MacOS ignorent les couleurs configurées
  pour la barre de menu principale et utilisent les paramètres système.
  </p>
  <p>
  Après avoir configuré le thème et les couleurs du menu, enregistrez les paramètres
  avec <menu>Options</menu><menu>Enregistrer les options</menu>.
  </p>
  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

####################
### GameInfoBar help:

set helpTitle(F,GameInfoBar) "La barre de menu de l'échiquier"
set helpText(F,GameInfoBar) {
<h1>La barre de menu de l'échiquier</h1>

  <p>La <term>barre de menu de l'échiquier</term> se trouve en-dessous du plateau de
  jeu. Elle affiche un résumé des informations qui peuvent aussi apparaître dans le
  <a GameInfoArea>panneau d'information</a> de la partie active. Les informations
  affichées sont les suivantes :</p>
  <ul>
  <li>1. les date et lieu de l'événement, quand l'échiquier est à
    la position initiale, un clic de souris sur le <blue>texte en bleu</blue> ouvre
    le classement du tournoi,</li>
  <li>2. le dernier coup joué ou le commentaire correspondant, avec
    le terme <b>(var)</b> pour indiquer si vous êtes dans une variante ou dans
    la ligne principale,</li>
  <li>3. et enfin, le <a ECO>code ECO</a> de l'ouverture avec
    le nom de la variante.</li>
  </ul>
  <p>Cette barre permet aussi de naviguer d'un coup à l'autre dans la partie active
  et offre les fonctions suivantes :</p>

  <h3><name BD_Alert>Message d'alerte</name></h3>
  <p>Si une <term>nouvelle variante est détectée</term>, la barre de menu alerte
  l'utilisateur avec le message <b>New Variation</b>. Cliquer sur le texte en bleu
  <blue>[click to change]</blue> offre les options suivantes :</p>
  <ul>
    <li><button tb_BD_Undo> Annuler l'action.</li>
    <li><button tb_BD_Replace> Remplacer le coup de la ligne principale et
    <b>efface les coups suivants</b>.</li>
    <li><button tb_BD_NewMainline> Promouvoir la variante comme ligne
    principale.</li>
    <li><button tb_BD_NewVar> Ajouter une variante.</li>
  </ul>

  <h3><name BD_Back>Menu contextuel du bouton Reculer</name></h3>
  <p><button tb_BD_Back> Ce bouton recule d'un demi-coup (raccourci :
  flèche <b>Gauche</b>). Un Clic-Droit avec la souris ouvre le
  <green>menu contextuel</green> suivant :</p>
  <ul>
  <li><button tb_BD_BackToMainline> Revenir à la ligne principale.</li>
  <li><button tb_BD_VarDelete> Effacer cette variante du fichier PGN.</li>
  <li><button tb_BD_VarLeave> Quitter la variante.</li>
  <li><button tb_BD_Start> Début de partie (raccourci :
    touche <b>Début</b>).</li>
  </ul>

  <h3><name BD_ShowToolbar>Menu du bouton Afficher</name></h3>
  <p><button tb_BD_ShowToolbar> Ce bouton affiche le <green>menu</green> suivant
  par un Clic-Gauche avec la souris :</p>
  <ul>
  <li><button tb_BD_SetupBoard> Définir la position de départ.</li>
  <li><button tb_BD_Flip> Retourner l'échiquier.</li>
  <li><button tb_BD_Coords> Afficher/Masquer les coordonnées de
    l'échiquier.</li>
  <li><button tb_BD_Material> Afficher/Masquer le bilan matériel à droite
    de l'échiquier.</li>
  <li><button tb_BD_Fullscreen> Activer l'affichage Plein Écran.</li>
  </ul>

  <h3><name BD_Forward>Menu contextuel du bouton Avancer</name></h3>
  <p><button tb_BD_Forward> Ce bouton avance d'un demi-coup (raccourci :
  flèche <b>Droite</b>). Un Clic-Droit avec la souris ouvre le
  <green>menu contextuel</green> suivant :</p>
  <ul>
  <li><button tb_BD_Autoplay> Jeu automatique.</li>
  <li><button tb_BD_End> Fin de partie (raccourci :
    touche <b>Fin</b>).</li>
  </ul>

  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

####################
### GameInfo Area help:

set helpTitle(F,GameInfoArea) "Le panneau d'information"
set helpText(F,GameInfoArea) {
<h1>Le panneau d'information</h1>
  <p>Le <term>panneau d'information</term> de la partie active peut être affiché ou
  caché avec le menu <menu>Fenêtres</menu><menu>Panneau d'information</menu>.
  Les lignes d'information sont tirées des en-têtes PGN
  si elles sont correctement renseignées.
  <b>Tous les <blue>textes en bleu</blue> sont des liens cliquables</b> à la souris
  dans ce panneau et ouvrent des fenêtres de rapport Scid.</p>

  <p>Le panneau d'information dispose d'un
  <a GameInfoArea GInfo_ContextMenu>menu contextuel</a>
  activé avec le bouton droit de la souris, avec des options pour
  <term>cacher le prochain coup</term> (utile
  si vous vous entraînez en utilisant un jeu et voulez deviner chaque coup) et
  pour <term>supprimer ou restaurer la partie en cours</term>.</p>

  <h3>Contenu des lignes d'information</h3>
  <p>Les lignes d'information sont les suivantes :</p>
  <ul>
  <li>1. <b>Partie #</b> : le numéro de la partie et les noms des joueurs. Cliquer sur
  un nom ouvre la fenêtre d'<a PInfo>information sur le joueur</a> correspondant.</li>
  <li>2. <b>Résultat du match</b> (avec le nombre de coups), et le code ECO de
  l'ouverture qui ouvre la fenêtre de
  <a ECO Browser>navigation ECO</a>.</li>
  <li>3. <b>Événement</b> : le lieu de l'événement et les drapeaux du jeu, un clic ouvre
  la fenêtre <a Crosstable>Classement du tournoi</a> pour voir le résultat
  de tous les matchs.</li>
  <li>4. <b>Dernier coup</b> et <b>Coup suivant</b>. L'annotation <green>(Var)</green>
  indique si le coup actuel est dans une ligne de variante ou dans la ligne
  principale du fichier PGN. Un clic sur les textes de ces coups
  déplace l'échiquier en avant ou en arrière.</li>
  <li>5. <b>Variantes</b> : liste cliquable de toutes les variantes du coup suivant dans
  cette partie. La première sera nommée <b>v1</b>, la seconde <b>v2</b>, etc.</li>
  <li>6. <b>Commentaire</b> s'il y en a. Un clic sur ce texte permet d'ouvrir
    l'<a Comment>éditeur de commentaires</a>.</li>
  <li>7. <b>ECO</b> : le code ECO de la position actuelle, si la position apparaît dans
  le fichier ECO utilisé, avec le lien vers le
  <a ECO Browser>navigation ECO</a>.</li>
  </ul>
  
  <h4>Drapeaux et liens bibliographiques</h4>
  <p>De plus, les <a Flags>drapeaux</a> définis pour la partie en cours sont
  affichés. Ici, les drapeaux personnalisés n'apparaissent que par leur numéro
  respectif alors que les drapeaux standards montrent également leur signification
  textuelle.</p>
  <p>A côté des drapeaux, un lien appelé <term>Bib</term> peut apparaître au cas où
  l'en-tête PGN contient un lien bibliographique, c'est-à-dire un lien vers
  la littérature sur les échecs. Ce lien doit être placé dans un champ d'en-tête
  spécial appelé <b>Bib</b> et relie le jeu à une base de données BibTeX
  contenant l'information bibliographique demandée. Cette mise en relation se fait
  par une structure LaTeX presque standard, c'est-à-dire que le champ
  <b>Bib</b> contient d'abord la clé BibTeX.</p>
  <p>Des informations supplémentaires
  (par exemple le numéro de page, numéro de jeu...) peuvent être ajouté par
  <b>, addinfo</b>. Pour indiquer plus d'un lien bibliographique, plusieurs de ces
  entrées peuvent être séparées par <b> ; </b> (c'est-à-dire espace point-virgule
  espace). Pour résoudre la référence bibliographique, Scid recherche d'abord
  un fichier .bib nommé comme la base de données en cours
  dans le répertoire de cette base. S'il ne peut pas trouver un tel fichier, il
  recourt au fichier <b>~/.scid4.7/data/Bookshelf.bib</b>.</p>
  
  <h4>Photos des joueurs</h4>
  <p>Si Scid peut trouver un fichier photo approprié (soit dans ~/.scid4.7 ou dans
  le fichier répertoire de partage dans le dossier photos) et si des photos pour
  les joueurs existent, ces photos apparaissent dans le panneau d'information.
  Les fichiers de photos peuvent être téléchargés à partir du
  <url http://scid.sourceforge.net/>Site Web de Scid</url> ;
  une documentation sur leurs formats est contenue dans ces fichiers. Cependant,
  les photos ont tendance à cacher le texte du panneau d'information. Par conséquent,
  ils peuvent être minimisés temporairement en cliquant dessus.</p>

  <h4>Tables de finales</h4>
  <p>Le panneau d'information sur le jeu affiche également les résultats de la table
  de finales lorsque la position affichée atteint une configuration matérielle
  trouvée dans un des fichiers de tables de finales. Voir la page d'aide
  <a TB>Tables de finales</a> pour plus de détails.</p>

  <h3><name GInfo_ContextMenu>Menu contextuel du panneau</name></h3>
  <p>Les items du menu contextuel du panneau d'information sont :</p>
  <ul>
    <li><menu>Cacher le coup suivant</menu></li>
    <li><menu>Montrer les valeurs de matériel</menu></li>
    <li><menu>Montrer la notation FEN</menu></li>
    <li><menu>Montrer les cases colorées et les flèches</menu></li>
    <li><menu>Découper les longues lignes</menu></li>
    <li><menu>Montrer tous les commentaires</menu></li>
    <li><menu>Montrer les Photos</menu></li>
    <li><menu>Tables de finales: Rien</menu></li>
    <li><menu>Tables de finales: Seulement le résultat</menu></li>
    <li><menu>Tables de finales: résultat et meilleurs coups</menu></li>
    <li><menu>(Récupérer)Effacer cette partie</menu></li>
  </ul>

  <p><footer>(Mise à jour : Scid 4.7.1, janvier 2020)</footer></p>
}

# end of francais.tcl
