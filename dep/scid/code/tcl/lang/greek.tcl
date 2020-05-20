
# Κείμενο για ονόματα των μενού και των μηνυμάτων βοήθειας της γραμμής κατάστασης στα Ελληνικά.
# Τμήμα του Scid (Shane's Chess Information Database).
#
# Οδηγίες προσθήκης νέας γλώσσας:
#
# (4) Γράψτε την μετάφραση. Κάτι προϋποθέτει την αλλαγή ο,τιδήποτε "μέσα σε εισαγωγικά"
#     ή σε {μύστακες} για τις εντολές menuText, helpMsg και translate.
#
#     Μια εντολή μενού έχει την μορφή:
#         menuText L tag "Name..." underline {ΜήνυμαΒοήθειας...}
#
#     Ένα μήνυμα βοήθειας στην γραμμή κατάστασης έχει την μορφή:
#         helpMsg L tag {ΜήνυμαΒοήθειας...}
#
#     Η μετάφραση μιας γενικής λέξης ή μηνύματος έχει την μορφή:
#         translate L tag {μήνυμα...}
#
#     όπου "L" το σύμβολο για την γλώσα, "tag" το όνομα του μενού
#     ή το όνομα του κουμπιού της εφαρμογής, και "underline" είναι ο 
#     δείκτης του υπογραμμισμένου γράμματος μετρώντας από το μηδέν ως πρώτο γράμμα. Τα μενού
#     που εμφανίζονται στο ίδιο μενού πρέπει να έχουν διαφορετικό υπογραμμισμένο γράμμα.
#     Αν έχετε αμφιβιλίες, βάλτε σε όλους τους δείκτες "0" και θα τα αντιστοιχίσω με με υπογραμμισμένα
#     γράμματα όταν οι μεταφράσεις ενσωματωθούν στο Scid.
#
# Έχετε να προσθέσετε ή να διορθώσετε; Email: sgh@users.sourceforge.net
#
# Αυτή είναι μια πρώτη μετάφραση του παρόντος αρχείου. Οι συμβουλευτικές προτάσεις και
# διορθώσεις όσων χρησιμοποιούν το Scid είναι απαραίτητες, καθώς πολλά μηνύματα βοήθειας
# έχουν αποδοθεί αρκετά ελεύθερα ή, αντιθέτα, κυριολεκτικά! Είναι άμεση ανάγκη για την βελτίωση
# του Scid και της μετάφρασής του στα Ελληνικά, να επικοινωνήσετε με τον μεταφραστή του παρόντος
# κειμένου στη διεύθυνση tripalatos@gmail.com για κάθε σφάλμα ή παράλειψη!

proc setLanguage_G {} {

# File menu:
menuText G File "Αρχείο" 0
menuText G FileNew "Νέα..." 0 {Δημιουργείστε μια νέα βάση δεδομένων Scid}
menuText G FileOpen "Άνοιγμα..." 0 {Ανοίξτε μια υπάρχουσα βάση δεδομένων Scid}
menuText G FileClose "Κλείσμο" 0 {Κλείστε την ενεργή βάση δεδομένων Scid}
menuText G FileFinder "Αναζήτηση" 0 {Ανοίγει το παράθυρο Αναζήτησης Αρχείου}
menuText G FileBookmarks "Σελιδοδείκτες" 0 {Μενού σελιδοδεικτών (συντόμευση: Ctrl+B)}
menuText G FileBookmarksAdd "Προσθήκη σελιδοδείκτου" 0 \
  {Θέστε σελιδοδείκτη στην τρέχουσα βάση δεδομένων παρτίδων και στην θέση}
menuText G FileBookmarksFile "Αρχειοθέτηση σελιδοδείκτου" 0 \
  {Αρχειοθετήστε έναν σελιδοδείκτη για την τρέχουσα παρτίδα και θέση}
menuText G FileBookmarksEdit "Επεξεργασία σελιδοδεικτών..." 0 \
  {Επεξεργαστείτε τα μενού των σελιδοδεικτών}
menuText G FileBookmarksList "Εμφάνιση των καταλόγων ως απλή λίστα" 0 \
  {Εμφάνιση των καταλόγων των σελιδοδεικτών ως απλής λίστα και όχι ως υπομενού}
menuText G FileBookmarksSub "Εμφάνιση των καταλόγων ως υπομενού" 0 \
  {Εμφάνιση των καταλόγων των σελιδοδεικτών ως υπομενού και όχι ως απλή λίστα}
menuText G FileMaint "Συντήρηση" 0 {Εργαλείο συντήρησης βάσεων δεδομένων Scid}
menuText G FileMaintWin "Παράθυρο συντήρησης" 0 \
  {Ανοίξτε/Κλείστε το παράθυρο συντήρησης βάσης δεδομένων Scid}
menuText G FileMaintCompact "Συμπίεση βάσης δεδομένων..." 0 \
  {Συμπιέστε τα αρχεία βάσης δεδομένων απομακρύνοντας διαγραμμένες παρτίδες και μη χρησιμοποιημένα ονόματα}
menuText G FileMaintClass "Παρτίδες ταξινομημένες κατά ECO..." 2 \
  {Επανυπολογισμός των κωδικών ECO για όλες τις παρτίδες}
menuText G FileMaintSort "Ταξινόμηση βάσης δεδομένων..." 0 \
  {Ταξινομήστε όλες τις παρτίδες της βάσης δεδομένων}
menuText G FileMaintDelete "Διαγραφή δίδυμων παρτίδων..." 0 \
  {Βρήτε τις δίδυμες παρτίδες και προσδιορίστε τα για διαγραφή}
menuText G FileMaintTwin "Παράθυρο ελέγχου δίδυμων παρτίδων" 0 \
  {Άνοιγμα/ενημέρωση του παραθύρου ελέγχου δίδυμων παρτίδων}
menuText G FileMaintName "Ορθογραφία ονομάτων" 0 {Εργαλείο επεξεργασίας και ορθογραφίας ονομάτων}
menuText G FileMaintNameEditor "Επεξεργαστής ονομάτων" 0 \
  {Άνοιγμα/κλείσιμο του παραθύρου επεξεργαστή ονομάτων}
menuText G FileMaintNamePlayer "Έλεγχος ορθογραφίας των ονομάτων των παικτών..." 11 \
  {Ελέγξτε την ορθογραφία των ονομάτων των παικτών με την χρήση του αρχείου ελέγχου οθρογραφίας}
menuText G FileMaintNameEvent "Έλεγχος ορθογραφίας των ονομάτων των διοργανώσεων..." 11 \
  {Ελέγξτε την ορθογραφία των ονομάτων των διοργανώσεων με τη χρήση του αρχείου ελέγχου ορθογραφίας}
menuText G FileMaintNameSite "Έλεγχος ορθογραφίας των ονομάτων των τοποθεσιών..." 11 \
  {Ελέγξτε την ορθογραφία των ονομάτων των τοποθεσιών με την χρήση του αρχείου ελέγχου ορθογραφίας}
menuText G FileMaintNameRound "Έλεγχος οθρογραφίας των ονομάτων των περιόδων..." 11 \
  {Έλεγχος των ονομάτων των περιόδων του αγώνα με την χρήση του αρχείου ελέγχου ορθογραφίας}
menuText G FileReadOnly "Μόνο για ανάγνωση (Read-only)..." 0 \
  {Επεξεργαστείτε την τρέχουσα βάση δεδομένων ως μόνον για ανάγνωση, εμποδίζοντας τις αλλαγές}
menuText G FileSwitch "Μετάβαση στην βάση δεδομένων" 0 \
  {Μεταβείτε σε μια διαφορετική ανοιγμένη βάση δεδομένων}
menuText G FileExit "Έξοδος" 1 {Έξοδος από το Scid}
menuText G FileMaintFixBase "Διόρθωση βάσης" 0 {Προσπαθείστε να επισκευάσετε μια χαλασμένη βάση}
  
# Edit menu:
menuText G Edit "Επεξεργασία" 0
menuText G EditAdd "Προσθήκη βαριάντας" 0 {Προσθέστε την βαριάντα μιας κίνησης της παρτίδας}
menuText G EditDelete "Διαγραφή βαριάντας" 0 {Διαγράψτε την βαριάντα για αυτή την κίνηση}
menuText G EditFirst "Πρώτη βαριάντα" 5 \
  {Προαγωγή μιας βαριάντας στην πρώτη θέση της λίστας}
menuText G EditMain "Προαγωγή βαριάντας σε κύρια γραμμή" 21 \
  {Προαγωγή μιας βαριάντας ως κύρια γραμμή}
menuText G EditTrial "Έλεγχος βαριάντας" 0 \
  {Έναρξη/διακοπή της λειτουργίας ελέγχου μιας ιδέας στην σκακιέρα}
menuText G EditStrip "Απομάκρυνση" 2 {Απομακρύνετε τα σχόλια ή τις βαριάντες από αυτή τη παρτίδα}
menuText G EditUndo "Επαναφορά" 0 {Επαναφέρετε την τελευταία αλλαγή στην παρτίδα}
# ====== TODO To be translated ======
menuText G EditRedo "Redo" 0 {Redo last game change}
menuText G EditStripComments "Σχόλια" 0 \
  {Απομακρύνετε όλα τα σχόλια και τον υπομνηματισμό από αυτή τη παρτίδα}
menuText G EditStripVars "Βαριάντες" 0 {Απομακρύνετε όλες τις βαριάντες από αυτή τη παρτίδα}
menuText G EditStripBegin "Κινήσεις από την έναρξη" 1 \
  {Απομακρύνετε τις κινήσεις από την έναρξη της παρτίδας}
menuText G EditStripEnd "Κινήσεις από το φινάλε" 0 \
  {Απομακρύνετε τις κινήσεις φινάλε της παρτίδας}
menuText G EditReset "Άδειασμα της προσωρινής βάσης (Clipbase)" 0 \
  {Μηδενίστε την προσωρινή βάση δεδομένων (Clipbase) ώστε να αδειάσει πλήρως}
menuText G EditCopy "Αντιγραφή της παρτίδας στην προσωρινή βάση" 0 \
  {Αντιγραφή αυτής της παρτίδας στην προσωρινή βάση δεδομένων}
menuText G EditPaste "Επικόλληση της τελευταίας παρτίδας της προσωρινής βάσης (Clipbase)" 0 \
  {Επικολλήστε εδώ την ενεργή παρτίδα της προσωρινής βάσης}
menuText G EditPastePGN "Επικόλληση του κειμένου της προσωρινής μνήμης ως παρτίδας PGN..." 18 \
  {Μετατρέψτε το κείμενο της προσωρινής μνήμης ως παρτίδας με σχολιασμό PGN και επικολλήστε την εδώ}
menuText G EditSetup "Στήσιμο αρχικής σκακιέρας..." 0 \
  {Καθορίστε την θέση έναρξης αυτής της παρτίδας}
menuText G EditCopyBoard "Αντιγραφή θέσης" 6 \
  {Αντιγράψτε την τρέχουσα σκακιέρα στην προσωρινή μνήμη με σχολιασμό FEN}
menuText G EditPasteBoard "Επικόλληση σκακιέρας έναρξης" 12 \
  {Καθορίστε την σκακιέρα από την προσωρινή μνήμη}
# ====== TODO To be translated ======
menuText G ConfigureScid "Preferences..." 0 {Configure all options for SCID}

# Game menu:
menuText G Game "Παρτίδα" 0
menuText G GameNew "Νέα παρτίδα" 0 \
  {Επαναφορά σε κενή παρτίδα, αναιρώντας κάθε αλλαγή}
menuText G GameFirst "Φόρτωση πρώτης παρτίδας" 5 {Φορτώστε την πρώτη φιλτραρισμένη παρτίδα}
menuText G GamePrev "Φόρτωση προηγούμενης παρτίδας" 5 {Φορτώστε την προηγούμενη φιλτραρισμένη παρτίδα}
menuText G GameReload "Επαναφόρτωση τρέχουσας παρτίδας" 3 \
  {Επαναφορτώστε την παρτίδα αυτή, αναιρώντας κάθε αλλαγή}
menuText G GameNext "Φόρτωση επόμενης παρτίδας" 7 {Φορτώστε την επόμενη φιλτραρισμένη παρτίδα}
menuText G GameLast "Φόρτωση τελευταίας παρτίδας" 8 {Φορτώστε την τελευταία φιλτραρισμένη παρτίδα}
menuText G GameRandom "Φόρτωση τυχαίας παρτίδας" 8 {Φορτώστε μια παρτίδα τυχαίου φιλτραρίσματος}
menuText G GameNumber "Φόρτωση αριθμού παρτίδας..." 5 \
  {Φορτώστε μια παρτίδα εισάγοντας τον αριθμό της}
menuText G GameReplace "Αποθήκευση: Αντικατάσταση παρτίδας..." 6 \
  {Αποθηκεύστε αυτή τη παρτίδα αντικαθιστώντας την παλαιότερη εκδοχή της}
menuText G GameAdd "Αποθήκευση: Προσθήκη νέας παρτίδας..." 6 \
  {Αποθηκεύστε αυτή τη παρτίδα ως νέα παρτίδα στην βάση δεδομένων}
menuText G GameDeepest "Αναγνώριση ανοίγματος" 0 \
  {Μετάβαση στην πιο προχωρημένη θέση της παρτίδας που αναφέρεται στην ECO}
menuText G GameGotoMove "Μετάβαση στην κίνηση με αριθμό..." 5 \
  {Μετάβαση στην κίνηση με συγκεκριμένο αριθμό στην τρέχουσα παρτίδα}
menuText G GameNovelty "Αναζήτηση νεοτερισμού..." 7 \
  {Βρείτε την πρώτη κίνηση αυτής της παρτίδας που δεν έχει παιχθεί ποτέ}

# Search Menu:
menuText G Search "Αναζήτηση" 0
menuText G SearchReset "Μηδενισμός φίλτρων" 0 {Μηδενίστε τα φίλτρα ώστε να συμπεριληφθούν όλες οι παρτίδες}
menuText G SearchNegate "Αναίρεση φίλτρου" 0 {Αναιρέστε το φίλτρο ώστε να συμπεριληφθούν μόνο εξαιρεταίες παρτίδες}
menuText G SearchCurrent "Τρέχουσα σκακιέρα..." 0 {Αναζητήστε την τρέχουσα θέση της σκακιέρας}
menuText G SearchHeader "Κεφαλίδα..." 0 {Αναζήτηση βάσει των πληροφοριών κεφαλίδας (παίκτης, διοργάωνση, κλπ)}
menuText G SearchMaterial "Υλικό/Διάταξη..." 0 {Αναζήτηση για υλικό ή διάταξη σκακιέρας}
menuText G SearchUsing "Με χρήση αρχείου αναζήτησης..." 0 {Αναζήτηση με την χρήση ενός αρχείου SearchOptions}

# Windows menu:
menuText G Windows "Παράθυρα" 0
menuText G WindowsComment "Συντάκτης σχολίων" 0 {Ανοίξτε/κλείστε τον συντάκτη σχολίων}
menuText G WindowsGList "Λίστα παρτίδων" 0 {Ανοίξτε/κλείστε το παράθυρο με την λίστα των παρτίδων}
menuText G WindowsPGN "Παράθυρο PGN" 0 \
  {Ανοίξτε/κλείστε το παράθυρο PGN (σχολιασμός παρτίδας)}
menuText G WindowsPList "Αναζήτηση παίκτου" 2 {Ανοίξτε/κλείστε την εύρεση παίκτου}
menuText G WindowsTmt "Αναζήτηση προκριματικών" 2 {Άνοιγμα/κλείσιμο της αναζήτησης τουρνουά}
menuText G WindowsSwitcher "Εναλλαγή βάσης δεδομένωνDatabase switcher" 0 \
  {Άνοιγμα/κλείσιμο του παραθύρου εναλλαγής βάσης δεδομένων}
menuText G WindowsMaint "Παράθυρο συντήρησης" 0 \
  {Άνοιγμα/κλείσιμο του παραθύρου συντήρησης}
menuText G WindowsECO "Φυλλομετρητής ECO" 0 {Άνοιγμα/κλείσιμο του παραθύρου του φυλλομετρητή ECO}
menuText G WindowsStats "Παράθυρο στατιστικών" 0 \
  {Άνοιγμα/κλείσιμο του παραθύρου φιλτραρίσματος στατιστικών}
menuText G WindowsTree "Παράθυρο δένδρου" 0 {Άνοιγμα/κλείσιμο του παραθύρου δένδρου}
menuText G WindowsTB "Παράθυρο βάσης δεδομένων φινάλε" 1 {Άνοιγμα/κλείσιμο του παραθύρου βάσης δεδομένων φινάλε}
menuText G WindowsBook "Παράθυρο βιβλίου" 0 {Άνοιγμα/κλείσιμο του παραθύρου του βιβλίου}
menuText G WindowsCorrChess "Παράθυρο αλληλογραφίας" 0 {Άνοιγμα/κλείσιμο του παραθύρου αλληλογραφίας}

# Tools menu:
menuText G Tools "Εργαλεία" 0
menuText G ToolsAnalysis "Μηχανή ανάλυσης..." 0 \
  {Εκκίνηση/διακοπή της σκακιστικής μηχανής ανάλυσης}
menuText G ToolsAnalysis2 "Μηχανής ανάλυσης #2..." 17 \
  {Εκκίνηση/διακοπή της δεύτερης σκακιστικής μηχανής ανάλυσης}
menuText G ToolsCross "Συγκριτικός πίνακας" 0 {Εμφανήστε τον συγκριτικό πίνακα προκριματικών γι' αυτή τη παρτίδα}
menuText G ToolsEmail "Διαχείριση ηλ. αλληλογραφίας" 0 {Άνοιγμα/κλείσιμο του παραθύρου διαχείρισης σκακιστικής ηλ. αλληλογραφίας}
menuText G ToolsFilterGraph "Γράφημα σχετικού φίλτρου" 12 {Άνοιγμα/κλείσιμο του παραθύρου των φίλτρων γραφημάτων σχετικών τιμών}
menuText G ToolsAbsFilterGraph "Γράφημα απόλυτου φίλτρου" 7 {Άνοιγμα/κλείσιμο του παραθύρου των φίλτρων γραφημάτων απολύτων τιμών}
menuText G ToolsOpReport "Αναφορά ανοίγματος" 0 {Δημιουργήστε μια αναφορά για το άνοιγμα της τρέχουσας θέσης}
menuText G ToolsOpenBaseAsTree "Άνοιγμα βάσης ως δένδρου..." 0   {Ανοίξτε μια βάση και χρησιμοποιείστε την στο παράθυρο δένδρου}
menuText G ToolsOpenRecentBaseAsTree "Άνοιγμα πρόσφατης βάσης ως δένδρου" 0   {Ανοίξτε μια πρόσφατη βάση και χρησιμοποιήστε την μέσα από το παράθυρο του δένδρου}
menuText G ToolsTracker "Ανιχνευτής κομματιού"  6 {Ανοίξτε το παράθυρο του ανιχνευτή κομματιού}
menuText G ToolsTraining "Προπόνηση"  0 {Προπονητικά εργαλεία (τακτικές, ανοίγματα...)}
menuText G ToolsTacticalGame "Παρτίδα τακτικής"  0 {Παίξτε μια παρτίδα τακτικής}
menuText G ToolsSeriousGame "Σοβαρή παρτίδα"  0 {Παίξτε μια σοβαρή παρτίδα}
menuText G ToolsTrainOpenings "Ανοίγματα"  0 {Προπονηθείτε με ένα ρεπερτόριο}
menuText G ToolsTrainReviewGame "Επισκόπηση παρτίδας"  0 {Μαντέψτε τις κινήσεις που έγιναν σε μια παρτίδα}
menuText G ToolsTrainTactics "Τακτικές"  0 {Επιλύστε τακτικές}
menuText G ToolsTrainCalvar "Υπολογισμός βαριαντών"  0 {Προπόνηση στον υπολογισμό βαριαντών}
menuText G ToolsTrainFindBestMove "Εύρεση καλύτερης κίνησης"  0 {Βρείτε την καλύτερη κίνηση}
menuText G ToolsTrainFics "Παιχνίδι στον FICS"  0 {Παίξτε στο freechess.org}
# ====== TODO To be translated ======
menuText G ToolsEngineTournament "Engine tournament"  0 {Start a tournament between chess engines}
menuText G ToolsBookTuning "Συντονισμός βιβλίου" 0 {Συντονισμός βιβλίου}
menuText G ToolsConnectHardware "Σύνδεση περιφερειακού" 8 {Συνδέστε εξωτερικό περιφερειακό}
menuText G ToolsConnectHardwareConfigure "Ρύθμιση..." 0 {Ρυθμίστε εξωτερικό περιφερειακό και συνδέσεις}
menuText G ToolsConnectHardwareNovagCitrineConnect "Σύνδεση με Novag Citrine" 8 {Συνδέστε Novag Citrine με το Scid}
menuText G ToolsConnectHardwareInputEngineConnect "Σύνδεση μηχανής εισόδου δεδομένων" 8 {Συνδέστε μηχανή εισόδου (π.χ. DGT board) με το Scid}

menuText G ToolsPInfo "Πληροφορίες παίκτη"  0 \
  {Άνοιγμα/ενημέρωση του παραθύρου πληροφοριών παίκτη}
menuText G ToolsPlayerReport "Δελτίο παίκτη..." 3 \
  {Δημιουργήστε δελτίο για έναν παίκτη}
menuText G ToolsRating "Γράφημα βαθμολόγησης" 0 \
  {Δημιουργήστε γράφημα του ιστορικού βαθμολόγησης των παικτών της τρέχουσας παρτίδας}
menuText G ToolsScore "Γράφημα επιδόσεων" 0 {Εμφανίστε το παράθυρο γραφήματος των σκορ}
menuText G ToolsExpCurrent "Εξαγωγή τρέχουσας παρτίδας" 8 \
  {Γράψτε την τρέχουσα παρτίδα σε ένα αρχείο κειμένου}
menuText G ToolsExpCurrentPGN "Εξαγωγή παρτίδας σε ένα αρχείο PGN..." 15 \
  {Γράψτε την τρέχουσα παρτίδα σε ένα αρχείο PGN}
menuText G ToolsExpCurrentHTML "Εξαγωγή παρτίδας σε αρχείο HTML..." 15 \
  {Γράψτε την τρέχουσα παρτίδα σε ένα αρχείο HTML}
menuText G ToolsExpCurrentHTMLJS "Εξαγωγή παρτίδας σε αρχεία HTML και JavaScript..." 15 {Γράψτε την τρέχουσα παρτίδα σε αρχεία HTML και JavaScript}  
menuText G ToolsExpCurrentLaTeX "Εξαγωγή παρτίδας σε αρχείο LaTeX..." 15 \
  {Γράψτε την τρέχουσα παρτίδα σε ένα αρχείο LaTeX}
menuText G ToolsExpFilter "Εξαγωγή όλων των φιλτραρισμένων παρτίδων" 1 \
  {Γράψτε όλες τις φιλτραρισμένες παρτίδες σε ένα αρχείο κειμένου}
menuText G ToolsExpFilterPGN "Εξαγωγή φίλτρου σε αρχείο PGN..." 17 \
  {Γράψτε όλες τις φιλτραρισμένες παρτίδες σε ένα αρχείο PGN}
menuText G ToolsExpFilterHTML "Εξαγωγή φίλτρου σε αρχείο HTML..." 17 \
  {Γράψτε όλες τις φιλτραρισμένες παρτίδες σε ένα αρχείο HTML}
menuText G ToolsExpFilterHTMLJS "Εξαγωγή φίλτρου σε αρχεία HTML και JavaScript..." 17 {Γράψτε όλες τις φιλτραρισμένες παρτίδες σε αρχεία HTML και JavaScript}  
menuText G ToolsExpFilterLaTeX "Εξαγωγή φίλτρου σε αρχείο LaTeX..." 17 \
  {Γράψτε όλες τις φιλτραρισμένες παρτίδες σε ένα αρχείο LaTeX}
menuText G ToolsImportOne "Εισαγωγή παρτίδας PGN..." 0 \
  {Εισάγετε μια παρτίδα από ένα κείμενο PGN}
menuText G ToolsImportFile "Εισαγωγή αρχείου παρτίδων PGN..." 7 {Εισάγετε παρτίδες από ένα αρχείο PGN}
menuText G ToolsStartEngine1 "Εκκίνηση μηχανής 1" 13  {Εκκινήστε την μηχανή 1}
menuText G ToolsStartEngine2 "Εκκίνηση μηχανής 2" 13  {Εκκινήστε την μηχανή 2}
menuText G ToolsCaptureBoard "Σύλληψη τρέχουσας σκακιέρας..." 5  {Αποθηκεύστε την τρέχουσα σκακιέρα ως εικόνα.}

# Play menue
menuText G Play "Παιχνίδι" 0

# --- Correspondence Chess
menuText G CorrespondenceChess "Σκάκι δι' αλληλογραφίας" 0 {Λειτουργίες σκακιού δι' αλληλογραφίας βάσει eMail και Xfcc}
menuText G CCConfigure "Ρυθμίσεις..." 0 {Προσαρμογή εξωτερικών εργαλείων και γενικές ρυθμίσεις}
menuText G CCConfigRelay "Παρακολούθηση παρτίδων..." 10 {Ρυθμίστε τις παρτίδες προς παρακολούθηση}
menuText G CCOpenDB "Άνοιγμα βάσης δεδομένων..." 0 {Ανοίξτε την προκαθορισμένη βάση δεδομένων αλληλογραφίας}
menuText G CCRetrieve "Ανάκτηση παρτίδων" 0 {Ανακτήστε παρτίδες μέσω εξωτερικής εφαρμογής (Xfcc-)helper}
menuText G CCInbox "Επεξεργασία εισερχομένων" 8 {Επεξεργαστείτε όλα τα αρχεία στα εισερχόμενα του Scid}
menuText G CCSend "Αποστολή κίνησης" 0 {Στείλτε την κίνησή σας μέσω eMail ή εξωτερικής εφαρμογής (Xfcc-)helper}

menuText G CCResign "Παραίτηση" 1 {Παραιτηθείτε (όχι μέσω eMail)}
menuText G CCClaimDraw "Αίτηση ισοπαλίας" 6 {Στείλτε την κίνησή σας και ζητήστε ισοπαλία (όχι μέσω eMail)}
menuText G CCOfferDraw "Παραχώρηση ισοπαλίας" 1 {Στείλτε την κίνησή σας και παραχωρήστε ισοπαλία (όχι μέσω eMail)}
menuText G CCAcceptDraw "Αποδοχή ισοπαλίας" 0 {Δεχθείτε μια παραχώρηση ισοπαλίας (όχι μέσω eMail)}

menuText G CCNewMailGame "Νέα παρτίδα eMail..." 2 {Ξεκινήστε μια νέα παρτίδα μέσω eMail}
menuText G CCMailMove "Αποστολή κίνησης..." 0 {Ταχυδρομήστε την κίνησή σας μέσω eMail προς τον αντίπαλο}
menuText G CCGamePage "Σελίδα παρτίδας..." 0 {Εμφανήστε την παρτίδα μέσα από τον περιηγητή ιστού}

# menu in cc window:
menuText G CCEditCopy "Αντιγραφή λίστας παρτίδων στην προσωρινή μνήμη (Clipbase)" 0 {Αντιγράψτε τις παρτίδες ως λίστα CSV στην μνήμη clipbase}

#  B    GHiJKL    Q  TUV XYZ

# Options menu:
menuText G Options "Επιλογές" 0
menuText G OptionsBoardGraphics "Τετράγωνα..." 0 {Επιλέξτε την υφή των τετραγώνων}
translate G OptionsBGW {Επιλέξτε την υφή των τετραγώνων}
translate G OptionsBoardGraphicsText {Επιλέξτε το αρχείο γραφικών για τα λευκά και τα μαύρα τετράγωνα:}
menuText G OptionsBoardNames "Τα ονόματά μου..." 0 {Επεξεργαστείτε τα ονόματά σας στις παρτίδες}
menuText G OptionsExport "Εξαγωγές" 0 {Αλλάξτε τις επιλογές εξαγωγής κειμένου}
menuText G OptionsFonts "Γραμματοσειρές" 0 {Αλλάξτε τις γραμματοσειρές}
menuText G OptionsFontsRegular "Συνήθης" 0 {Αλλάξτε την τακτική γραμματοσειρά}
menuText G OptionsFontsMenu "Μενού" 0 {Αλλάξτε την γραμματοσειρά των μενού}
menuText G OptionsFontsSmall "Μικρή" 0 {Αλλάξτε την μικρή γραμματοσειρά}
menuText G OptionsFontsTiny "Πολύ μικρή" 0 {Αλλάξτε την πολύ μικρή γραμματοσειρά}
menuText G OptionsFontsFixed "Σταθερή" 0 {Αλλάξτε την γραμματοσειρά σταθερού πλάτους}
menuText G OptionsGInfo "Πληροφορίες παρτίδας" 0 {Επιλογές πληροφοριών παρτίδας}
menuText G OptionsLanguage "Γλώσσα" 0 {Επιλέξτε την γλώσσα των μενού}
menuText G OptionsMovesTranslatePieces "Μετάφραση κομματιών" 0 {Μεταφράστε το πρώτο γράμμα των κομματιών}
menuText G OptionsMovesHighlightLastMove "Ανάδειξη τελευταίας κίνησης" 0 {Αναδείξτε την τελευταία κίνηση}
menuText G OptionsMovesHighlightLastMoveDisplay "Εμφάνιση" 0 {Προβολή της τελευταίας κίνησης με ανάδειξη}
menuText G OptionsMovesHighlightLastMoveWidth "Πλάτος" 0 {Το πάχος της γραμμής}
menuText G OptionsMovesHighlightLastMoveColor "Χρώμα" 0 {Το χρώμα τηςη γραμμής}
# ====== TODO To be translated ======
menuText G OptionsMovesHighlightLastMoveArrow "include Arrow" 0 {Include Arrow with Highlight}
menuText G OptionsMoves "Κινήσεις" 0 {Επιλογές εισαγωγής κίνησης}
menuText G OptionsMovesAnimate "Χρόνος απεικόνισης κίνησης" 1 \
  {Καθορίστε την διάρκεια απεικόνισης της κίνησης}
menuText G OptionsMovesDelay "Χρονική καθυστέρηση αυτόματου παιχνιδιού..." 1 \
  {Καθορίστε την διάρκεια της χρονικής καθυστέρησης για την λειτουργία αυτόματου παιξίματος}
menuText G OptionsMovesCoord "Εισαγωγή συντεταγμένης κίνησης" 0 \
  {Αποδοχή εισαγωγής κίνησης σε στυλ συντεταγμένων (π.χ. "g1f3")}
menuText G OptionsMovesSuggest "Εμφάνιση προτεινόμενων κινήσεων" 0 \
  {Ενεργοποίηση/απενεργοποίηση προτεινόμενης κίνησης}
menuText G OptionsShowVarPopup "Εμφάνιση παραθύρου βαριαντών" 0 {Ενεργοποίηση/απενεργοποίηση την εμφάνιση του παραθύρου βαριαντών}  
menuText G OptionsMovesSpace "Προσθήκη διαστημάτων μετά τον αριθμό της κίνησης" 0 {Προσθήκη διαστήματος μετά τον αριθμό της κίνησης}  
menuText G OptionsMovesKey "Ολοκλήρωση πληκτρολογίου" 0 \
  {Ενεργοποίηση/απενεργοποίηση της αυτόματης ολοκλήρωσης κινήσεων μέσω πληκτρολογίου}
menuText G OptionsMovesShowVarArrows "Εμφάνιση βέλους για βαριάντα" 0 {Ενεργοποίηση/απενεργοποίηση του βέλους που δείχνει τις κινήσεις των βαριαντών}
# ====== TODO To be translated ======
menuText G OptionsMovesGlossOfDanger "Color Coded Gloss of Danger" 0 {Turn on/off color coded gloss of danger}
menuText G OptionsNumbers "Μορφή αριθμών" 0 {Επιλέξτε τη μορφή των αριθμών}
menuText G OptionsTheme "Θέμα" 0 {Αλλάξτε την όψη του προγράμματος}
menuText G OptionsWindows "Παράθυρα" 0 {Επιλογές παραθύρων}
menuText G OptionsSounds "Ήχοι" 2 {Ρυθμίστε τους ήχους αναγγελίας κίνησης}
# ====== TODO To be translated ======
menuText G OptionsResources "Resources..." 0 {Choose resource files and folders}
menuText G OptionsWindowsDock "Προσκόληση παραθύρων" 0 {Προσκολήστε τα παράθυρα (χρειάζεται επανεκκίνηση)}
menuText G OptionsWindowsSaveLayout "Αποθήκευση διάταξης" 0 {Αποθηκεύστε την διάταξη των παραθύρων}
menuText G OptionsWindowsRestoreLayout "Αποκατάσταση διάταξης" 0 {Αποκαταστήστε προηγούμενη διάταξη παραθύρων}
menuText G OptionsWindowsShowGameInfo "Εμφάνιση πληροφοριών παρτίδας" 0 {Εμφανίστε τις πληροφορίες για την παρτίδα}
menuText G OptionsWindowsAutoLoadLayout "Αυτόματη φόρτωση της πρώτης διάταξης" 0 {Αυτόματη φόρτωση κατά την εκκίνηση της πρώτης διάταξης}
menuText G OptionsECO "Φόρτωση αρχείου ECO..." 7 {Φορτώστε το αρχείο ταξινόμησης κατά ECO}
menuText G OptionsSpell "Φόρτωση αρχείου ορθογραφικού ελέγχου" 11 \
  {Φορτώστε το αρχείο ορθογραφικού ελέγχου του Scid}
menuText G OptionsTable "Κατάλογος βάσης φινάλε" 10 \
  {Επιλέξτε μια βάση δεδομένων φινάλες. Θα χρησιμοποιηθούν όλες οι βάσεις δεδομένων φινάλε του καταλόγου αυτού}
menuText G OptionsRecent "Πρόσφατα αρχεία..." 0 {Αλλάξτε το πλήθος των πρόσφατων αρχείων που εμφανίζονται στο μενού Αρχείο}
menuText G OptionsBooksDir "Κατάλογος βιβλίων" 0 {Καθορίζει τον κατάλογο των βιβλίων ανοιγμάτων}
menuText G OptionsTacticsBasesDir "Bases directory" 0 {Sets the tactics (training) bases directory}
# ====== TODO To be translated ======
menuText G OptionsPhotosDir "Photos directory" 0 {Sets the photos bases directory}
# ====== TODO To be translated ======
menuText G OptionsThemeDir "Theme(s) File:"  0 { Load a GUI theme package file }
menuText G OptionsSave "Αποθήκευση επιλογών" 0 "Αποθηκεύστε όλες τις μεταβολές ρυθμίσεων στο αρχείο $::optionsFile"
menuText G OptionsAutoSave "Αυτόματη αποθήκευση επιλογών κατά την έξοδο" 0 \
  {Να αποθηκεύονται αυτόματα οι επιλογές κατά την έξοδο από το Scid}

# Help menu:
menuText G Help "Βοήθεια" 0
menuText G HelpContents "Περιεχόμενα" 0 {Εμφανήστε την σελίδα των περιεχομένων της βοήθειας}
menuText G HelpIndex "Κατάλογος" 0 {Εμφανήστε τον κατάλογο βοήθειας}
menuText G HelpGuide "Γρήγορος οδηγός" 0 {Εμφανίζει την σελίδα της βοήθειας με τον γρήγορο οδηγό}
menuText G HelpHints "Υποδείξεις" 0 {Εμφανήστε την σελίδα της βοήθειας με τις νύξεις}
menuText G HelpContact "Πληροφορίες επικοινωνίας" 1 {Εμφανίζει την σελίδα της βοήθειας με τις πληροφορίες επικοινωνίας}
menuText G HelpTip "Συμβουλή της ημέρας" 0 {Εμφάνιση χρήσιμων συμβουλών για το Scid}
menuText G HelpStartup "Παράθυρο έναρξης" 0 {Εμφανίζει το παράθυρο έναρξης}
menuText G HelpAbout "Για το Scid" 0 {Πληροφορίες για το Scid}

# Game info box popup menu:
menuText G GInfoHideNext "Απόκρυψη επόμενης κίνησης" 0
menuText G GInfoMaterial "Εμφάνισης της αξίας των κομματιών" 0
menuText G GInfoFEN "Εμφάνιση FEN" 5
menuText G GInfoMarks "Εμφάνιση έγχρωμων τετρατώνων και βελών" 5
menuText G GInfoWrap "Αναδύπλωση γραμμών" 0
menuText G GInfoFullComment "Εμφάνιση πλήρους σχολιασμού" 10
menuText G GInfoPhotos "Εμφάνιση φωτογραφιών" 5
menuText G GInfoTBNothing "Βάση φινάλε: Τίποτα" 12
menuText G GInfoTBResult "Βάση φινάλε: Μόνον αποτελέσματα" 12
menuText G GInfoTBAll "Βάση φινάλε: Αποτέλεσα και καλύτερες κινήσεις" 19
menuText G GInfoDelete "Διαγραφή/ανάκτηση αυτής της παρτίδας" 4
menuText G GInfoMark "(Μη-)Σημείωση αυτής της παρτίδας" 4
menuText G GInfoInformant "Ρύθμιση τιμών πληροφορίας" 0

# General buttons:
translate G Back {Πίσω}
translate G Browse {Αναζήτηση}
translate G Cancel {Άκυρο}
translate G Continue {Συνέχεια}
translate G Clear {Καθαρισμός}
translate G Close {Κλείσιμο}
translate G Contents {Περιεχόμενα}
translate G Defaults {Αρχικό}
translate G Delete {Διαγραφή}
translate G Graph {Γράφημα}
translate G Help {Βοήθεια}
translate G Import {Εισαγωγή}
translate G Index {Κατάλογος}
translate G LoadGame {Φώρτωση παρτίδας}
translate G BrowseGame {Περιήγηση παρτίδας}
translate G MergeGame {Συνένωση παρτίδας}
translate G MergeGames {Συνένωση παρτίδων}
translate G Preview {Προεπισκόπηση}
translate G Revert {Επαναφορά}
translate G Save {Αποθήκευση}
translate G Search {Αναζήτηση}
translate G Stop {Διακοπή}
translate G Store {Αποθήκευση}
translate G Update {Ενημέρωση}
translate G ChangeOrient {Αλλαγή προσανατολισμού παραθύρου}
translate G ShowIcons {Εμφάνιση εικόνων}
translate G None {Κανένα}
translate G First {Πρώτο}
translate G Current {Τρέχων}
translate G Last {Τελευταίο}

# General messages:
translate G game {παρτίδα}
translate G games {παρτίδες}
translate G move {κίνηση}
translate G moves {κινήσεις}
translate G all {όλες}
translate G Yes {Ναι}
translate G No {Όχι}
translate G Both {Αμφότεροι}
translate G King {Βασιλιάς}
translate G Queen {Βασίλισσα}
translate G Rook {Πύργος}
translate G Bishop {Αξιωματικός}
translate G Knight {Ίππος}
translate G Pawn {Πιόνι}
translate G White {Λευκά}
translate G Black {Μαύρα}
translate G Player {Παίκτης}
translate G Rating {Βαθμολογία}
translate G RatingDiff {Διαφορά βαθμολογίας (Λευκά - Μαύρα)}
translate G AverageRating {Μέση βαθμολογία}
translate G Event {Διοργάνωση}
translate G Site {Τόπος}
translate G Country {Χώρα}
translate G IgnoreColors {Αγνόηση χρωμάτων}
translate G Date {Ημερομηνία}
translate G EventDate {Ημερομηνία διοργάνωσης}
translate G Decade {Δεκαετία}
translate G Year {Έτος}
translate G Month {Μήνας}
translate G Months {Ιανουάριος Φεβρουάριος Μάρτιος Απρίλιος Μάιος Ιούνιος Ιούλιος Αύγουστος Σεπτέμβριος Οκτώβριος Νοέμβριος Δεκέμβριος}
translate G Days {Κυρ Δευ Τρ Τε Πε Πα Σα}
translate G YearToToday {Τρέχον έτος}
# ====== TODO To be translated ======
translate G YearToTodayTooltip {Set date from 1 year back to today}
translate G Result {Αποτέλεσμα}
translate G Round {Περίοδος}
translate G Length {Έκταση}
translate G ECOCode {Κώδικας ECO}
translate G ECO {ECO}
translate G Deleted {Διαγράφηκε}
translate G SearchResults {Αναζήτηση αποτελεσμάτων}
translate G OpeningTheDatabase {Βάση δεδομένων ανοιγμάτων}
translate G Database {Βάση δεδομένων}
translate G Filter {Φίλτρο}
translate G noGames {καμία παρτίδα}
translate G allGames {όλες οι παρτίδες}
translate G empty {κενό}
translate G clipbase {προσωρινή μνήμη clipbase}
translate G score {βαθμός}
translate G StartPos {Εναρκτήρια θέση}
translate G Total {Σύνολο}
translate G readonly {μόνο για ανάγνωση}

# Standard error messages:
translate G ErrNotOpen {Αυτή η βάση δεδομένων δεν είναι ανοικτή.}
translate G ErrReadOnly {Αυτή η βάση δεδομένων είναι μόνον για ανάγνωση. Δεν μπορεί να αντικατασταθεί.}
translate G ErrSearchInterrupted {Η αναζήτηση διακόπηκε. Τα αποτελέσματα δεν είναι πλήρη.}

# Game information:
translate G twin {δίδυμες}
translate G deleted {διεγραμμένη}
translate G comment {σχόλιο}
translate G hidden {κρυμένη}
translate G LastMove {Τελευταία κίνηση}
translate G NextMove {Επόμενη}
translate G GameStart {Αρχή παρτίδας}
translate G LineStart {Αρχή γραμμής}
translate G GameEnd {Τέλος παρτίδας}
translate G LineEnd {Τέλος γραμμής}

# Player information:
translate G PInfoAll {Αποτελέσματα για <b>όλες</b> τις παρτίδες}
translate G PInfoFilter {Αποτελέσματα για τις <b>φιλτραρισμένες</b> παρτίδες}
translate G PInfoAgainst {Αποτελέσματα εναντίον}
translate G PInfoMostWhite {Τα κοινότερα ανοίγματα ως Λευκά}
translate G PInfoMostBlack {Τα κοινότερα ανοίγματα ως Μαύρα}
translate G PInfoRating {Ιστορικό βαθμολόγησης}
translate G PInfoBio {Βιογραφία}
translate G PInfoEditRatings {Επεξεργασία βαθμολογήσεων}
# ====== TODO To be translated ======
translate G PInfoEloFile {File}

# Tablebase information:
translate G Draw {Ισοπαλία}
translate G stalemate {πατ}
translate G withAllMoves {με όλες τις κινήσεις}
translate G withAllButOneMove {με όλες τις κινήσεις εκτός μιας}
translate G with {με}
translate G only {μόνο}
translate G lose {απώλεια}
translate G loses {χάνει}
translate G allOthersLose {όλες οι άλλες χάνουν}
translate G matesIn {ματ σε}
translate G hasCheckmated {έκανε ματ}
translate G longest {μεγαλύτερη}
translate G WinningMoves {Κινήσεις που κερδίζουν}
translate G DrawingMoves {Κινήσεις που οδηγούν σε ισοπαλία}
translate G LosingMoves {Κινήσεις που χάνουν}
translate G UnknownMoves {Κινήσεις αγνώστου αποτελέσματος}
# ====== TODO To be translated ======
translate G SideLongest {Side    Longest    %     %     %\n}
# ====== TODO To be translated ======
translate G toNoveMate {to move   mate    Win  Draw  Loss\n}
# ====== TODO To be translated ======
translate G NoSummary {\nNo summary for this tablebase.}
# ====== TODO To be translated ======
translate G MutualZugzwang {Mutual zugzwangs: }
# ====== TODO To be translated ======
translate G SelectetZugzwang {\nSelected zugzwang positions:}
# ====== TODO To be translated ======
translate G WdrawBloss {White draws, Black loses:}
# ====== TODO To be translated ======
translate G BdrawWloss {Black draws, White loses:}
# ====== TODO To be translated ======
translate G AllLoss {" Whoever moves loses:     " }

# Tip of the day:
translate G Tip {Συμβουλή}
translate G TipAtStartup {Συμβουλές κατά την εκκίνηση}

# Tree window menus:
menuText G TreeFile "Αρχείο" 0
menuText G TreeFileFillWithBase "Γέμισμα της μνήμης με την βάση" 0 {Γεμίστε την προσωρινή μνήμη cache με όλα τις παρτίδες της τρέχουσας βάσης}
menuText G TreeFileFillWithGame "Γέμισμα της μνήμης με παρτίδα" 0 {Γεμίστε την προσωρινή μνήμη cache με την τρέχουσα παρτίδα στην τρέχουσα βάση}
menuText G TreeFileSetCacheSize "Μέγεθος προσωρινής μνήμης" 0 {Καθορίστε το μέγεθος της προσωρινής μνήμης cache}
menuText G TreeFileCacheInfo "Αποθήκευση πληροφοριών στην μνημη" 0 {Μεταβιβάστε τις πληροφορίες στην μνήμη cache}
menuText G TreeFileSave "Αποθήκευση του αρχείου προσωρινής μνήμης" 0 {Αποθηκεύστε το αρχείο δένδρου της προσωρινής μνήμης (.stc)}
menuText G TreeFileFill "Γέμισμα του αρχείου προσωρινής μνήμης" 0 \
  {Γεμίστε το αρχείο προσωρινής μνήμης με συνήθεις θέσεις ανοιγμάτων}
menuText G TreeFileBest "Λίστα καλύτερων παρτίδων" 0 {Εμφάνιση του καλύτερου δένδρου λίστας παρτίδων}
menuText G TreeFileGraph "Παράθυρο γραφήματος" 0 {Εμφάνιση του γραφήματος γι' αυτόν τον κλάδο δένδρου}
menuText G TreeFileCopy "Αντιγραφή του κειμένου του δένδρου στην προσωρινή μνήμη Clipboard" 1 \
  {Αντιγράψτε το δένδρο των στατιστικών στην προσωρινή μνήμη clipboard}
menuText G TreeFileClose "Κλείσιμο του παραθύρου δένδρου" 0 {Κλείστε το παράθυρο του δένδρου}
menuText G TreeMask "Μάσκα" 0
menuText G TreeMaskNew "Νέα" 0 {Νέα μάσκα}
menuText G TreeMaskOpen "Άνοιγμα" 0 {Ανοίξτε μια μάσκα}
menuText G TreeMaskOpenRecent "Άνοιγμα πρόσφατης" 0 {Ανοίξτε πρόσφατη μάσκα}
menuText G TreeMaskSave "Αποθήκευση" 0 {Αποθηκεύστε την μάσκα}
menuText G TreeMaskClose "Κλείσιμο" 0 {Κλείστε τη μάσκα}
menuText G TreeMaskFillWithGame "Γέμισμα με παρτίδα" 0 {Γεμίστε την μάσκα με μια παρτίδα}
menuText G TreeMaskFillWithBase "Γέμισμα με βάση" 0 {Γεμίστε τη μάσκα με όλες τις παρτίδες της βάσης}
menuText G TreeMaskInfo "Info" 0 {Show statistics for current mask}
menuText G TreeMaskDisplay "Εμφάνιση χάρτη μάσκας" 0 {Εμφανίστε τα δεδομένα της μάσκας σε μορφή δένδρου}
menuText G TreeMaskSearch "Αναζήτηση" 0 {Αναζήτηση στην τρέχουσα μάσκα}
menuText G TreeSort "Ταξινόμηση" 0
menuText G TreeSortAlpha "Αλφαβητική" 0
menuText G TreeSortECO "Κώδικας ECO" 0
menuText G TreeSortFreq "Συχνότητα" 0
menuText G TreeSortScore "Βαθμολογία" 0
menuText G TreeOpt "Επιλογές" 0
menuText G TreeOptSlowmode "Αργή λειτουργία" 0 {Αργή λειτουργία των ενημερώσεων (μεγάλη ακρίβεια)}
menuText G TreeOptFastmode "Γρήγορη λειτουργία" 0 {Γρήγορη λειτουργία των ενημερώσεων (χωρίς αντιμετάθεση κίνησης)}
menuText G TreeOptFastAndSlowmode "Αργή και γρήγορη λειτουργία" 0 {Γρήγορη λειτουργία και έπειτα αργή λειτουργία για ενημερώσεις}
menuText G TreeOptStartStop "Αυτόματη ανανέωση" 0 {Εναλλάσει την αυτόματη ανανέωση του παραθύρου του δένδρου}
menuText G TreeOptLock "Lock" 0 {Lock/unlock the tree to the current database}
menuText G TreeOptTraining "Προπόνηση" 0 {Ενεργοποίηση/απενεργοποίηση της λειτουργίας εκπαίδευσης}
menuText G TreeOptAutosave "Αυτόματη αποθήκευση του αρχείου προσωρινής μνήμης" 0 \
  {Αποθηκεύει αυτόματα το αρχείο προσωρινής μνήμης όταν κλείσετε το παράθυρο δένδρου}
menuText G TreeHelp "Βοήθεια" 0
menuText G TreeHelpTree "Δένδρο βοήθειας" 0
menuText G TreeHelpIndex "Κατάλογος βοήθειας" 0
translate G SaveCache {Αποθήκευση προσωρινής μνήμης Cache}
translate G Training {Προπόνηση}
translate G LockTree {Κλείδωμα}
translate G TreeLocked {κλειδωμένο}
translate G TreeBest {Καλύτερο}
translate G TreeBestGames {Οι καλύτερες παρτίδες του δένδρου}
# Note: the next message is the tree window title row. After editing it,
# check the tree window to make sure it lines up with the actual columns.
translate G TreeTitleRow \
  {    Move   ECO       Frequency    Score  AvElo Perf AvYear %Draws}
translate G TreeTotal {ΣΥΝΟΛΟ}
translate G DoYouWantToSaveFirst {Θέλετε να αποθηκεύσετε πρώτα;}
translate G AddToMask {Προσθήκη στην μάσκα}
translate G RemoveFromMask {Απομάκρυνση από την μάσκα}
translate G AddThisMoveToMask {Προσθήκη αυτής της κίνησης στην μάσκα}
translate G SearchMask {Αναζήτηση στην μάσκα}
translate G DisplayMask {Εμφάνιση μάσκας}
translate G Nag {Κώδικας Nag}
translate G Marker {Μαρκαδόρος}
translate G Include {Περιλαμβάνει}
translate G Exclude {Εξαιρεί}
translate G MainLine {Κύρια γραμμή}
translate G Bookmark {Σελιδοδείκτης}
translate G NewLine {Νέα γραμμή}
translate G ToBeVerified {Να εξακριβωθεί}
translate G ToTrain {Να εξασκηθεί}
translate G Dubious {Αμφίβολη}
translate G ToRemove {Προς απομάκρυνση}
translate G NoMarker {Χωρίς μαρκαδόρο}
translate G ColorMarker {Χρώμα}
translate G WhiteMark {Λευκό}
translate G GreenMark {Πράσινο}
translate G YellowMark {Κίτρινο}
translate G BlueMark {Γαλάζιο}
translate G RedMark {Κόκκινο}
translate G CommentMove {Σχολιασμός κίνησης}
translate G CommentPosition {Σχολιασμός θέσης}
translate G AddMoveToMaskFirst {Πρώτα να προστεθεί η κίνηση στην μάσκα}
translate G OpenAMaskFileFirst {Πρώτα να ανοιχτεί ένα αρχείο μάσκας}
translate G Positions {Θέσεις}
translate G Moves {Κινήσεις}

# Finder window:
menuText G FinderFile "Αρχείο" 0
menuText G FinderFileSubdirs "Αναζήτηση στους υποκαταλόγους" 0
menuText G FinderFileClose "Κλείσιμο αναζήτησης αρχείου" 0
menuText G FinderSort "Ταξινόμηση" 0
menuText G FinderSortType "Τύπος" 0
menuText G FinderSortSize "Μέγεθος" 0
menuText G FinderSortMod "Τροποποιήθηκε" 0
menuText G FinderSortName "Όνομα" 0
menuText G FinderSortPath "Διαδρομή" 0
menuText G FinderTypes "Τύποι" 0
menuText G FinderTypesScid "Βάσεις δεδομένων Scid" 0
menuText G FinderTypesOld "Βάσεις δεδομένων Scid παλαιάς μορφής" 0
menuText G FinderTypesPGN "Αρχεία PGN" 0
menuText G FinderTypesEPD "Αρχεία EPD" 0
menuText G FinderTypesRep "Αρχεία ρεπερτορίου" 0
menuText G FinderHelp "Βοήθεια" 0
menuText G FinderHelpFinder "Βοήθεια για την αναζήτηση αρχείων" 0
menuText G FinderHelpIndex "Κατάλογος βοήθειας" 0
translate G FileFinder {Αναζήτηση αρχείου}
translate G FinderDir {Κατάλογος}
translate G FinderDirs {Κατάλογοι}
translate G FinderFiles {Αρχεία}
translate G FinderUpDir {επάνω}
translate G FinderCtxOpen {Άνοιγμα}
translate G FinderCtxBackup {Αντίγραφο ασφαλείας}
translate G FinderCtxCopy {Αντιγραφή}
translate G FinderCtxMove {Μεταφορά}
translate G FinderCtxDelete {Διαγραφή}
 
# Player finder:
menuText G PListFile "Αρχείο" 0
menuText G PListFileUpdate "Ανανέωση/Ενημέρωση" 0
menuText G PListFileClose "Κλείσιμο αναζήτησης αναπαραγωγής" 0
menuText G PListSort "Ταξινόμηση" 0
menuText G PListSortName "Όνομα" 0
menuText G PListSortElo "Elo" 0
menuText G PListSortGames "Παρτίδες" 0
menuText G PListSortOldest "Παλαιότερη" 0
menuText G PListSortNewest "Νεότερη" 2

# Tournament finder:
menuText G TmtFile "Αρχείο" 0
menuText G TmtFileUpdate "Ανανέωση" 0
menuText G TmtFileClose "Κλείσιμο αναζήτησης προκριματικών" 0
menuText G TmtSort "Ταξινόμηση" 0
menuText G TmtSortDate "Ημερομηνία" 0
menuText G TmtSortPlayers "Παίκτες" 0
menuText G TmtSortGames "Παρτίδες" 0
menuText G TmtSortElo "Elo" 0
menuText G TmtSortSite "Τοποθεσία" 0
menuText G TmtSortEvent "Διοργάνωση" 1
menuText G TmtSortWinner "Νικητής" 0
translate G TmtLimit "Όριο λίστας"
translate G TmtMeanElo "Μέσος Elo"
translate G TmtNone "Δεν βρέθηκαν προκριματικοί που να ταιριάζουν."

# Graph windows:
menuText G GraphFile "Αρχείο" 0
menuText G GraphFileColor "Αποθήκευση ως έγρωμο PostScript..." 8
menuText G GraphFileGrey "Αποθήκευση ως γρκίζο PostScript..." 8
menuText G GraphFileClose "Κλείσιμο παραθύρου" 6
menuText G GraphOptions "Επιλογές" 0
menuText G GraphOptionsWhite "Λευκά" 0
menuText G GraphOptionsBlack "Μαύρα" 0
menuText G GraphOptionsBoth "Και τα δύο" 1
menuText G GraphOptionsPInfo "Πληροφορίες παίκτη" 0
# ====== TODO To be translated ======
menuText G GraphOptionsEloFile "Elo from rating file" 0
# ====== TODO To be translated ======
menuText G GraphOptionsEloDB "Elo from database" 0
translate G GraphFilterTitle "Φίλτρο γραφήματος: συχνότητα ανά 1000 παρτίδες"
translate G GraphAbsFilterTitle "Φίλτρο γραφήματος: συχνότητα παρτίδας"
translate G ConfigureFilter "Προσαρμογή των αξόνων Χ ως προς Έτος, Βαθμολογία και Κινήσεις"
translate G FilterEstimate "Εκτίμηση"
translate G TitleFilterGraph "Scid: Φιλτράρισμα γραφήματος"

# Analysis window:
translate G AddVariation {Προσθήκη βαριάντας}
translate G AddAllVariations {Προσθήκη όλων των βαριαντών}
translate G AddMove {Προσθήκκη κίνησης}
translate G Annotate {Υπομνηματισμός}
translate G ShowAnalysisBoard {Εμφάνισης της σκακιέρας ανάλυσης}
translate G ShowInfo {Εμφάνιση πληροφοριών μηχανής}
translate G FinishGame {Ολοκλήρωση παρτίδας}
translate G StopEngine {Διακοπή μηχανής}
translate G StartEngine {Εκκίνηση μηχανής}
translate G LockEngine {Κλείδωμα της μηχανής στην τρέχουσα θέση}
translate G AnalysisCommand {Εντολή ανάλυσης}
translate G PreviousChoices {Προηγούμενες επιλογές}
translate G AnnotateTime {Καθορίστε τον χρόνο μεταξύ των κινήσεων σε δευτερόλεπτα}
translate G AnnotateWhich {Προσθήκη βαριαντών}
translate G AnnotateAll {Για κινήσεις και των δύο πλευρών}
translate G AnnotateAllMoves {Υπομνηματισμός όλων των κινήσεων}
translate G AnnotateWhite {Για τις κινήσεις μόνο των Λευκών}
translate G AnnotateBlack {Για τις κινήσεις μόνο των Μαύρων}
translate G AnnotateBlundersOnly {Όταν η κίνηση είναι σφάλμα}
translate G AnnotateBlundersOnlyScoreChange {Η ανάλυση αναφέρει σφάλμα και το σκορ άλλαξε από/σε: }
translate G BlundersThreshold {Αφετηρία}
translate G ScoreAllMoves {Βαθμολόγηση όλων των κινήσεων}
translate G LowPriority {Χαμηλή προτεραιότητα της CPU}
translate G ClickHereToSeeMoves {Πατήστε εδώ για να δείτε τις κινήσεις}
translate G ConfigureInformant {Informant}
translate G Informant!? {Ενδιαφέρουσα κίνησηInteresting move}
translate G Informant? {Φτωχή κίνηση}
translate G Informant?? {Σφάλμα}
translate G Informant?! {Αμφίβολη κίνηση}
translate G Informant+= {Τα Λευκά έχουν μικρό πλεονέκτημα}
translate G Informant+/- {Τα Λευκά έχουν μέτριο πλεονέκτημα}
translate G Informant+- {Τα Λευκά έχουν αποφασιστικό πλεονέκτημα}
translate G Informant++- {Η παρτίδα θεωρείται κερδισμένη}
  
# Book window
translate G Book {Βιβλίο}
translate G OtherBookMoves {Βιβλίο αντιπάλου}
translate G OtherBookMovesTooltip {Κινήσεις για τις οποίες ο αντίπαλος διαθέτει απάντηση}

# Analysis Engine open dialog:
translate G EngineList {Λίστα μηχανής ανάλυσης}
translate G EngineName {Όνομα}
translate G EngineCmd {Εντολή}
translate G EngineArgs {Παράμετροι}
translate G EngineDir {Κατάλογος}
translate G EngineElo {Elo}
translate G EngineTime {Ημερομηνία}
translate G EngineNew {Νέα}
translate G EngineEdit {Επεξεργασία}
translate G EngineRequired {Τα παιδία με έντονη γραμματοσειρά είναι απαραίτητα, τα άλλα είναι προαιρετικά}

# PGN window menus:
menuText G PgnFile "Αρχείο" 0
menuText G PgnFileCopy "Αντιγραφή παρτίδας στην προσωρινή μνήμη Clipboard" 0
menuText G PgnFilePrint "Εκτύπωση σε αρχείο..." 0
menuText G PgnFileClose "Κλείσιμο παραθύρου PGN" 10
menuText G PgnOpt "Εμφάνιση" 0
menuText G PgnOptColor "Έγχρωμη εμφάνιση" 0
menuText G PgnOptShort "Μικρή (3 γραμμών) κεφαλίδα" 0
menuText G PgnOptSymbols "Συμβολικός υπομνηματισμός" 1
menuText G PgnOptIndentC "Εσοχή σχολίων" 0
menuText G PgnOptIndentV "Εσοχή βαριαντών" 7
menuText G PgnOptColumn "Είδος στήλης (μια κίνηση ανά γραμμή)" 1
menuText G PgnOptSpace "Διάστημα μετά τον αριθμό της κίνησης" 1
menuText G PgnOptStripMarks "Απομάκρυνση έγχρωμων κωδικών τετραγώνων/βελών" 1
menuText G PgnOptBoldMainLine "Χρήση έντονου κειμένου για τις κινήσεις κύριας γραμμής" 4
menuText G PgnColor "Χρώματα" 0
menuText G PgnColorHeader "Κεφαλίδα..." 0
menuText G PgnColorAnno "Υπομνηματισμοί..." 0
menuText G PgnColorComments "Σχόλια..." 0
menuText G PgnColorVars "Βαριάντες..." 0
menuText G PgnColorBackground "Υπόβαθρο..." 0
menuText G PgnColorMain "Κύρια γραμμή..." 0
menuText G PgnColorCurrent "Υπόβαθρο τρέχουσας κίνησης..." 1
menuText G PgnHelp "Βοήθεια" 0
menuText G PgnHelpPgn "Βοήθεια για PGN" 0
menuText G PgnHelpIndex "Κατάλογος" 0
translate G PgnWindowTitle {Σημείωση - παρτίδα %u}

# Crosstable window menus:
menuText G CrosstabFile "Αρχείο" 0
menuText G CrosstabFileText "Εκτύπωση κειμένου σε αρχείο..." 9
menuText G CrosstabFileHtml "Εκτύπωση σε αρχείο HTML..." 9
menuText G CrosstabFileLaTeX "Εκτύπωση σε αρχείο LaTeX..." 9
menuText G CrosstabFileClose "Κλείσιμο του παραθύρου συγκριτικού πίνακα" 0
menuText G CrosstabEdit "Επεξεργασία" 0
menuText G CrosstabEditEvent "Διοργάνωση" 0
menuText G CrosstabEditSite "Τοποθεσία" 0
menuText G CrosstabEditDate "Ημερομηνία" 0
menuText G CrosstabOpt "Εμφάνιση" 0
menuText G CrosstabOptAll "All-play-all" 0
menuText G CrosstabOptSwiss "Ελβετικού τύπου" 0
menuText G CrosstabOptKnockout "Knockout" 0
menuText G CrosstabOptAuto "Αυτόματα" 1
menuText G CrosstabOptAges "Ηλικία σε έτη" 8
menuText G CrosstabOptNats "Εθνικότητες" 0
menuText G CrosstabOptRatings "Βαθμολογίες" 0
menuText G CrosstabOptTitles "Τίτλοι" 0
menuText G CrosstabOptBreaks "Βαθμολογίες μη δεσμευτικού συστήματος" 4
menuText G CrosstabOptDeleted "Να περιλαμβάνονται διεγραμμένες παρτίδες" 8
menuText G CrosstabOptColors "Χρώματα (Πίνακες Ελβετικού τύπου μόνον)" 0
menuText G CrosstabOptColumnNumbers "Αριθμημένες στήλες (πίνακες τύπου All-play-all μόνον)" 2
menuText G CrosstabOptGroup "Ομαδοποίηση βαθμολογιών" 0
menuText G CrosstabSort "Ταξινόμηση" 0
menuText G CrosstabSortName "Όνομα" 0
menuText G CrosstabSortRating "Βαθμολόγηση" 0
menuText G CrosstabSortScore "Επίδοση" 0
menuText G CrosstabColor "Χρώμα" 0
menuText G CrosstabColorPlain "Απλό κείμενο" 0
menuText G CrosstabColorHyper "Υπερκείμενο" 0
menuText G CrosstabHelp "Βοήθεια" 0
menuText G CrosstabHelpCross "Βοήθεια συγκριτικών πινάκων" 0
menuText G CrosstabHelpIndex "Κατάλογος βοήθειας" 0
translate G SetFilter {Καθορισμός φίλτρου}
translate G AddToFilter {Προσθήκη σε φίλτρο}
translate G Swiss {Ελβετικού τύπου}
translate G Category {Κατηγορία}

# Opening report window menus:
menuText G OprepFile "Αρχείο" 0
menuText G OprepFileText "Εκτύπωση σε αρχείο κειμένου..." 9
menuText G OprepFileHtml "Εκτύπωση σε αρχείο HTML..." 9
menuText G OprepFileLaTeX "Εκτύπωση σε αρχείο LaTeX..." 9
menuText G OprepFileOptions "Επιλογές..." 0
menuText G OprepFileClose "Κλείσιμο παραθύρου αναφορών" 0
menuText G OprepFavorites "Προτιμώμενα" 1
menuText G OprepFavoritesAdd "Προσθήκη αναφοράς..." 0
menuText G OprepFavoritesEdit "Επεξεργασία προτιμήσεων αναφορών..." 0
menuText G OprepFavoritesGenerate "Δημιουργία αναφορών..." 0
menuText G OprepHelp "Βοήθεια" 0
menuText G OprepHelpReport "Άνοιγμα της βοήθειας για αναφορές" 0
menuText G OprepHelpIndex "Κατάλογος βοήθειας" 0

# Header search:
translate G HeaderSearch {Αναζήτηση κεφαλίδας}
translate G EndSideToMove {Η πλευρά που έχει την κίνηση στο τέλος της παρτίδας}
translate G GamesWithNoECO {Παρτίδες χωρίς ECO;}
translate G GameLength {Μήκος παρτίδας}
translate G FindGamesWith {Εύρεση παρτίδων με σημαίες}
translate G StdStart {Μη πρότυπη έναρξη}
translate G Promotions {Προαγωγές}
translate G Comments {Σχόλια}
translate G Variations {Βαριάντες}
translate G Annotations {Υπομνηματισμοί}
translate G DeleteFlag {Διαγραφή σημαιών}
translate G WhiteOpFlag {Άνγοιγμα Λευκών}
translate G BlackOpFlag {Άνοιγμα Μαύρων}
translate G MiddlegameFlag {Μέσον παρτίδας}
translate G EndgameFlag {Φινάλε}
translate G NoveltyFlag {Νεοτερισμός}
translate G PawnFlag {Πιονοδομή}
translate G TacticsFlag {Τακτική}
translate G QsideFlag {Παιχνίδι στην πλευρά της Βασίλισσας}
translate G KsideFlag {Παιχνίδι στην πλευρά του Βασιλιά}
translate G BrilliancyFlag {Ευφυές}
translate G BlunderFlag {Σφάλμα}
translate G UserFlag {Χρήστης}
translate G PgnContains {Το PGN περιέχει κείμενο}
translate G Annotator {Υπομνηματιστής}
translate G Cmnts {Μόνον υπομνηματισμένες παρτίδες}

# Game list window:
translate G GlistNumber {Αριθμός}
translate G GlistWhite {Λευκά}
translate G GlistBlack {Μαύρα}
translate G GlistWElo {Elo Λευκών}
translate G GlistBElo {Elo Μαύρων}
translate G GlistEvent {Διοργάνωση}
translate G GlistSite {Τοποθεσία}
translate G GlistRound {Γύρος}
translate G GlistDate {Ημερομηνία}
translate G GlistYear {Έτος}
translate G GlistEDate {Ημερομηνία Διοργάνωσης}
translate G GlistResult {Αποτέλεσμα}
translate G GlistLength {Μήκος}
translate G GlistCountry {Χώρα}
translate G GlistECO {ECO}
translate G GlistOpening {Άνοιγμα}
translate G GlistEndMaterial {Υλικό φινάλε}
translate G GlistDeleted {Διεγραμμένα}
translate G GlistFlags {Σημαίες}
translate G GlistVars {Βαριάντες}
translate G GlistComments {Σχόλια}
translate G GlistAnnos {Υπομηματισμοί}
translate G GlistStart {Έναρξη}
translate G GlistGameNumber {Αριθμός παρτίδας}
# ====== TODO To be translated ======
translate G GlistAverageElo {Average Elo}
# ====== TODO To be translated ======
translate G GlistRating {Rating}
translate G GlistFindText {Εύρεση κειμένου}
translate G GlistMoveField {Μετακίνηση}
translate G GlistEditField {Προσαρμογή}
translate G GlistAddField {Προσθήκη}
translate G GlistDeleteField {Απομάκρυνση}
translate G GlistWidth {Πλάτος}
translate G GlistAlign {Ευθυγραμμιση}
# ====== TODO To be translated ======
translate G GlistAlignL {Align: left}
# ====== TODO To be translated ======
translate G GlistAlignR {Align: right}
# ====== TODO To be translated ======
translate G GlistAlignC {Align: center}
translate G GlistColor {Χρώμα}
translate G GlistSep {Διαχωριστής}
# ====== TODO To be translated ======
translate G GlistCurrentSep {-- Current --}
# ====== TODO To be translated ======
translate G GlistNewSort {New}
# ====== TODO To be translated ======
translate G GlistAddToSort {Add}

# ====== TODO To be translated ======
translate G GsortSort {Sort...}
# ====== TODO To be translated ======
translate G GsortDate {Date}
# ====== TODO To be translated ======
translate G GsortYear {Year}
# ====== TODO To be translated ======
translate G GsortEvent {Event}
# ====== TODO To be translated ======
translate G GsortSite {Site}
# ====== TODO To be translated ======
translate G GsortRound {Round}
# ====== TODO To be translated ======
translate G GsortWhiteName {White Name}
# ====== TODO To be translated ======
translate G GsortBlackName {Black Name}
# ====== TODO To be translated ======
translate G GsortECO {ECO}
# ====== TODO To be translated ======
translate G GsortResult {Result}
# ====== TODO To be translated ======
translate G GsortMoveCount {Move Count}
# ====== TODO To be translated ======
translate G GsortAverageElo {Average Elo}
# ====== TODO To be translated ======
translate G GsortCountry {Country}
# ====== TODO To be translated ======
translate G GsortDeleted {Deleted}
# ====== TODO To be translated ======
translate G GsortEventDate {Event Date}
# ====== TODO To be translated ======
translate G GsortWhiteElo {White Elo}
# ====== TODO To be translated ======
translate G GsortBlackElo {Black Elo}
# ====== TODO To be translated ======
translate G GsortComments {Comments}
# ====== TODO To be translated ======
translate G GsortVariations {Variations}
# ====== TODO To be translated ======
translate G GsortNAGs {NAGs}
# ====== TODO To be translated ======
translate G GsortAscending {Ascending}
# ====== TODO To be translated ======
translate G GsortDescending {Descending}
# ====== TODO To be translated ======
translate G GsortAdd {Add}
# ====== TODO To be translated ======
translate G GsortStore {Store}
# ====== TODO To be translated ======
translate G GsortLoad {Load}

# menu shown with right mouse button down on game list. 
translate G GlistRemoveThisGameFromFilter  {Απομάκρυνση αυτής της παρτίδας από το φίλτρο}
translate G GlistRemoveGameAndAboveFromFilter  {Απομάκρυνση παρτίδας (και όλων των από πάνω) από το φίλτρο}
translate G GlistRemoveGameAndBelowFromFilter  {Απομάκρυνση παρτίδας (και όλων των από κάτω) από το φίλτρο}
translate G GlistDeleteGame {Διαγραφή/ανάκτηση αυτής της παρτίδας} 
translate G GlistDeleteAllGames {Διαγραφή όλων των παρτίδων από το φίλτρο} 
translate G GlistUndeleteAllGames {Ανάκτηση όλων των παρτίδων από το φίλτρο}  
translate G GlistMergeGameInBase {Συνένωση παρτίδας σε άλλη βάση δεδομένων}

# Maintenance window:
translate G DatabaseName {Όνομα βάσης δεδομένων:}
translate G TypeIcon {Εικόνα τύπου:}
translate G NumOfGames {Παρτίδες:}
translate G NumDeletedGames {Διεγραμμένες παρτίδες:}
translate G NumFilterGames {Παρτίδες στο φίλτρο:}
translate G YearRange {Εύρος έτους:}
translate G RatingRange {Εύρος βαθμολόγησης:}
translate G Description {Περιγραφή}
translate G Flag {Σημαία}
translate G CustomFlags {Προσαρμοζόμενες σημαίες}
translate G DeleteCurrent {Διαγραφή τρέχουσας παρτίδας}
translate G DeleteFilter {Διαγραφή φιλτραρισμένων παρτίδων}
translate G DeleteAll {Διαγραφή όλων των παρτίδων}
translate G UndeleteCurrent {Ανάκτηση τρέχουσας παρτίδας}
translate G UndeleteFilter {Ανάκτηση φιλτραρισμένων παρτίδων}
translate G UndeleteAll {Ανάκτηση όλων των παρτίδων}
translate G DeleteTwins {Διαγραφή δίδυμων παρτίδων}
translate G MarkCurrent {Μαρκάρισμα τρέχουσας παρτίδας}
translate G MarkFilter {Μαρκάρισμα φιλτραρισμένων παρτίδων}
translate G MarkAll {Μαρκάρισμα όλων των παρτίδων}
translate G UnmarkCurrent {Ξεμαρκάρισμα τρέχουσας παρτίδας}
translate G UnmarkFilter {Ξεμαρκάρισμα φιλτραρισμένων παρτίδων}
translate G UnmarkAll {Ξεμαρκάρισμα όλων των παρτίδων}
translate G Spellchecking {Έλεγχος ορθογραφίας}
translate G Players {Παίκτες}
translate G Events {Διοργανώσεις}
translate G Sites {Τοποθεσίες}
translate G Rounds {Γύροι}
translate G DatabaseOps {Χειρισμοί βάσης δεδομένων}
translate G ReclassifyGames {Ταξινόμηση παρτίδων κατά ECO}
translate G CompactDatabase {Συμπίεση βάσης δεδομένων}
translate G SortDatabase {Ταξινόμηση βάσης δεδομένων}
translate G AddEloRatings {Προσθήκη βαθμολογίας κατά Elo}
translate G AutoloadGame {Αυτόματη φόρτωση αριθμού παρτίδας}
translate G StripTags {Απομάκρυνση ετικετών ΡGN}
translate G StripTag {Απομάκρυνση ετικετών}
translate G Cleaner {Καθαρισμός}
translate G CleanerHelp {
Ο Καθαρισμός του Scid θα εφαρμόσει στην τρέχουσα βάση δεδομένων όλες τις ενέργειες συντήρησης που επιλέξατε από την λίστα που ακολουθεί.

Οι τρέχουσες ρυθμίσεις της ταξινόμησης κατά ECO και η διαγραφή δίδυμων παρτίδων θα εφαρμοσθούν εφόσον επιλέξατε αυτές τις ενέργειες.
}
translate G CleanerConfirm {
Όταν ο Καθαρισμός ξεκινήσει, δεν μπορείτε να τον σταματήσετε!

Η διαδικασία αυτή ίσως διαρκέσει αρκετό χρόνο αν η βάση δεδομένων είναι μεγάλη, αναλόγως των ενεργειών που έχετε επιλέξει και των ρυθμίσεών τους.

Είστε βέβαιος/αιη ότι θέλετε να ξεκινήσουν οι ενέργειες συντήρησης που επιλέξατε;
}
# Twinchecker
translate G TwinCheckUndelete {to flip; "u" undeletes both)}
translate G TwinCheckprevPair {Προηγούμενο ζεύγος}
translate G TwinChecknextPair {Επόμενο ζεύγος}
translate G TwinChecker {Scid: έλεγχος δίδυμων παρτίδων}
translate G TwinCheckTournament {Παρτίδες προκριματικού:}
translate G TwinCheckNoTwin {Δεν υπάρχουν δίδυμα }
translate G TwinCheckNoTwinfound {Δεν εντοπίστηκαν δίδυμα για την παρτίδα αυτή.\nΓια να δείτε τις δίδυμες παρτίδες χρησιμοποιώντας το παράθυρο αυτό, πρέπει πρώτα να χρησιμοποιήσετε την λειτουργία "Διαγραφή δίδυμων παρτίδων...". }
translate G TwinCheckTag {Κοινή χρήση ετικετών...}
translate G TwinCheckFound1 {Το Scid βρήκε $result δίδυμες παρτίδες}
translate G TwinCheckFound2 { και έθεσε γι' αυτές σημαίες διαγραφής}
translate G TwinCheckNoDelete {Σε αυτή τη βάση δεδομένων δεν υπάρχουν παρτίδες προς διαγραφή.}
translate G TwinCriteria1 {Οι ρυθμίσεις σας για την αναζήτηση δίδυμων παρτίδων πιθανόν να προκαλούν το μαρκάρισμα  μη-δίδυμων παρτίδων ως δίδυμες.}
translate G TwinCriteria2 {Αν έχετε επιλέξει "Όχι" για τις "όμοιες κινήσεις", συνιστούμε να επιλέξετε "Ναι" στις ρυθμίσεις για χρώματα, διοργανώσεις, τοποθεσίες, έτος και μήνες.\nΘέλετε να συνεχίσετε και να διαγραφούν οι δίδυμες;}
translate G TwinCriteria3 {Συνιστούμε να επιλέξετε "Ναι" τουλάχιστον για δύο ρυθμίσεις "ίδια τοποθεσία", "ίδιος γύρος" και "ίδιο έτος".\nΘέλετε να συνεχίσετε και να διαγραφούν οι δίδυμες;}
translate G TwinCriteriaConfirm {Scid: Επιβεβαίωση ρυθμίσεων διδύμων}
translate G TwinChangeTag "Αλλαγή των ακόλουθων ετικετών παρτίδας:\n\n"
translate G AllocRatingDescription "Η εντολή αυτή θα χρησιμοποιήσει το τρέχον αρχείο ορθογραφικού ελέγχου για να προσθέσει βαθμολογίες Elo σε αυτή την βάση δεδομένων. Αν ο παίκτης/παίκτρια δεν έχει τρέχουσα βαθμολογία αλλά η βαθμολογία του/της την στιγμή της παρτίδας εμφανίζεται στο αρχείο ορθογραφικού ελέγχου, αυτή η βαθμολογία θα προστεθεί."
translate G RatingOverride "Να αντικατασταθούν οι υπάρχουσες μη-μηδενικές βαθμολογίες;"
translate G AddRatings "Προσθήκη βαθμολογίας στον/στην:"
translate G AddedRatings {Τ Scid πρόσθεσε $r βαθμούς Elo σε $g παρτίδες.}

#Bookmark editor
translate G NewSubmenu "Νέο υπομενού"

# Comment editor:
translate G AnnotationSymbols  {Σύμβολα υπομνηματισμούς:}
translate G Comment {Σχόλιο:}
translate G InsertMark {Εισαγωγή σημαδιού}
translate G InsertMarkHelp {
Εισαγωγή/απομάκρυνμση σημαδιού: Επιλέξτε χρώμα, είδος, τετράγωνο.
Εισαγωγή/απομάκρυνση βέλους: Δεξί πάτημα δύο τετράγνω.
}

# Nag buttons in comment editor:
translate G GoodMove {Καλή κίνηση}
translate G PoorMove {Φτωχή κίνηση}
translate G ExcellentMove {Εξαίρετη κίνηση}
translate G Blunder {Σφάλμα}
translate G InterestingMove {Ενδιαφέρουσα κίνηση}
translate G DubiousMove {Αμφίβολη κίνηση}
translate G WhiteDecisiveAdvantage {Τα Λευκά έχουν αποφασιστικό πλεονέκτημα}
translate G BlackDecisiveAdvantage {Τα Μαύρα έχουν αποφασιστικό πλεονέκτημα}
translate G WhiteClearAdvantage {Τα Λευκά έχουν σαφές προβάδισμα}
translate G BlackClearAdvantage {Τα Μαύρα έχουν σαφές προβάδισμα}
translate G WhiteSlightAdvantage {Τα Λευκά έχουν μικρό πλεονέκτημα}
translate G BlackSlightAdvantage {Τα Μαύρα έχουν μικρό πλεονέκτημα}
translate G Equality {Ισότητα}
translate G Unclear {Ασαφής}
translate G Diagram {Διάγραμμα}

# Board search:
translate G BoardSearch {Αναζήτηση σκακιέρας}
translate G FilterOperation {Χειρισμός για το τρέχον φίλτρο:}
translate G FilterAnd {AND (Περιορισμός φίλτρου)}
translate G FilterOr {OR (Προσθήκη στο φίλτρο)}
translate G FilterIgnore {IGNORE (Επαναφορά φίλτρου)}
translate G SearchType {Ειδος αναζήτησης:}
translate G SearchBoardExact {Ακριβής θέση (όλα τα κομμάτια στα ίδια τετράγωνα)}
translate G SearchBoardPawns {Πιόνια (ίδιο υλικό, όλα τα πιόνια στα ίδια τετράγωνα)}
translate G SearchBoardFiles {Γραμμή (ίδιο υλικό, όλα τα πιόνια στις ίδιες γραμμές)}
translate G SearchBoardAny {Οπουδήποτε (ίδιο υλικό, πιόνια και κομμάτια οπουδήποτε)}
translate G SearchInRefDatabase {Αναζήτηση στην βάση αναφοράς}
translate G LookInVars {Αναζήτηση στις βαριάντες}

# Material search:
translate G MaterialSearch {Αναζήτηση υλικού}
translate G Material {Υλικό}
translate G Patterns {Διάταξη}
translate G Zero {Μηδέν}
translate G Any {Ό,τιδήποτε}
translate G CurrentBoard {Τρέχουσα σκακιέρα}
translate G CommonEndings {Κοινά φινάλε}
translate G CommonPatterns {Κοινές διατάξεις}
translate G MaterialDiff {Διαφορά υλικού}
translate G squares {τετράγωνα}
translate G SameColor {Ιδίου χρώματος}
translate G OppColor {Αντίθετου χρώματος}
translate G Either {Οποιοδήποτε}
translate G MoveNumberRange {Εύρος αριθμού κίνησης}
translate G MatchForAtLeast {Να ταιριάζουνβ τουλάχιστον}
translate G HalfMoves {μισές κινήσεις}

# Common endings in material search:
translate G EndingPawns {Φινάλε πιονιών}
translate G EndingRookVsPawns {Πύργων εναντίον πιονιών}
translate G EndingRookPawnVsRook {Πύργος και 1 Πύργος εναντίον πύργου}
translate G EndingRookPawnsVsRook {Πύργος και πιόνι(α) εναντίον Πύργου}
translate G EndingRooks {Φινάλε Πύργου εναντίον Πύργου}
translate G EndingRooksPassedA {Φινάλε Πύργου εναντίον Πύργου με προχωρημένο α-πιόνι}
translate G EndingRooksDouble {Φινάλε διπλού Πύργου}
translate G EndingBishops {Φινάλε Αξιωματικού εναντίον Αξιωματικού}
translate G EndingBishopVsKnight {Φινάλε Αξιωματικού εναντίον Ίππου}
translate G EndingKnights {Φινάλε Ίππου εναντίον Ίππου}
translate G EndingQueens {Φινάλε Βασίλισσας εναντίον Βασίλισσας}
translate G EndingQueenPawnVsQueen {Βασίλισσα και 1 Πιόνι εναντίον Βασίλισσας}
translate G BishopPairVsKnightPair {Μέσον παρτίδας με δύο Αξιωματικούς εναντίον δυο Αξιωματικών}

# Common patterns in material search:
translate G PatternWhiteIQP {Λευκό ΑΠΒ}
translate G PatternWhiteIQPBreakE6 {Λευκό ΑΠΒ: διείσδηση d4-d5 έναντι e6}
translate G PatternWhiteIQPBreakC6 {Λευκό ΑΠΒ: διείσδησηW d4-d5 έναντι c6}
translate G PatternBlackIQP {Μαύρο ΑΠΒ}
translate G PatternWhiteBlackIQP {Λευκό ΑΠΒ εναντίον Μαύρου ΑΠΒ}
translate G PatternCoupleC3D4 {Ζεύγος Λευκών πιονιών c3+d4}
translate G PatternHangingC5D5 {Μαύρα κρεμασμένα Πιόνια στα c5 και d5}
translate G PatternMaroczy {Κέντρο Maroczy (με Πιόνια στα c4 και e4)}
translate G PatternRookSacC3 {Θυσία Πύργου στο c3}
translate G PatternKc1Kg8 {O-O-O εναντίον O-O (Kc1 εναντίον Kg8)}
translate G PatternKg1Kc8 {O-O εναντίον O-O-O (Kg1 εναντίον Kc8)}
translate G PatternLightFian {Φιανκέττο λευκού τετραγώνου (Αξιωματικός-g2 εναντίον Αξιωματικού-b7)}
translate G PatternDarkFian {Φιανκέττο μαύρου τετραγώνου (Αξιωματικός-b2 εναντίον Αξιωματικού-g7)}
translate G PatternFourFian {Τέσσερα Φιανκέττα (Αξιωματικοί στα b2,g2,b7,g7)}

# Game saving:
translate G Today {Σήμερα}
translate G ClassifyGame {Ταξινόμηση παρτίδας}

# Setup position:
translate G EmptyBoard {Κενή σκακιέρα}
translate G InitialBoard {Αρχική σκακιέρα}
translate G SideToMove {Πλευρά που παίζει}
translate G MoveNumber {Αριθμός κίνησης}
translate G Castling {Ροκέ}
translate G EnPassantFile {Γραμμή en passant}
translate G ClearFen {Καθαρισμός FEN}
translate G PasteFen {Επικόληση FEN}

translate G SaveAndContinue {Αποθήκευση και συνέχιση}
translate G DiscardChangesAndContinue {Ακύρωση αλλαγών \nκαι συνέχιση}
translate G GoBack {Προς τα πίσω}

# Replace move dialog:
translate G ReplaceMove {Αντικατάσταση κίνησης}
translate G AddNewVar {Προσθήκη νέας βαριάντας}
translate G NewMainLine {Νέα κύρια γραμμή}
translate G ReplaceMoveMessage {Υπάρχει ήδη μια κίνηση εδώ.

Μπορείτε να την αντικαταστήσετε ακυρώνοντας όλες τις κινήσεις μετά από αυτήν ή να προσθέσετε την κίνησή σας ως νέα βαριάντα.

(Μπορείτε να αποφύγετε την εμφάνιση αυτού του μηνύματος στο μέλλον απενεργοποιώντας την επιλογή "Ερώτηση πριν την αντικατάσταση κίνησης" από το μενού Επιλογές:Κινήσεις.)}

# Make database read-only dialog:
translate G ReadOnlyDialog {Αν δηλώσετε αυτή την βάση δεδομένεν μόνον για ανάγνωση, δεν θα επιτρέπονται οι αλλαγές.
Δεν θα μπορούν να αποθηκευθούν ή να αντικατασταθούν παρτίδες και δεν θα μπορούν να αντικατασταθούν οι σημαίες διαγραφής.
Κάθε ταξινόμηση ή αποτελέσματα ταξινομημένα κατά ΕCO, θα είναι προσωρινά.

Μπορείτε εύκολα να δηλώσετε την βάση δεδομένων και πάλι ως εγγράψιμη, κλείνοντας και ανοίγοντάς την εκ νέου.

Θέλετε στ' αλήθεια να γίνει αυτή η βάση μόνον για ανάγνωση;}

# Clear game dialog:
translate G ClearGameDialog {Η παρτίδα αυτή έχει αντικατασταθεί.

Θέλετε στ' αλήθεια να συνεχίσετε και να ακυρώσετε τις αλλαγές που κάνατε σε αυτήν;
}

# Exit dialog:
translate G ExitDialog {Θέλετε στ' αλήθεια να βγείτε από το Scid;}
translate G ExitUnsaved {Οι βάσεις δεδομένων που ακολουθούν έχουν μη αποθηκευμένες αλλαγές παρτίδων. Αν βγείτε τώρα οι αλλαγές αυτές θα χαθούν.}

# Import window:
translate G PasteCurrentGame {Επικόλληση τρέχουσας παρτίδας}
translate G ImportHelp1 {Εισάγετε ή επικολλήστε παρτίδα σε μορφη PGN στο παραπάνω πλαίσιο.}
translate G ImportHelp2 {Εδώ θα εμφανίζεται οποιοδήποτε σφάλμα κατά την εισαγωγή παρτίδας.}
translate G OverwriteExistingMoves {Αντικατάσταση υπαρχόντων κινήσεων;}

# ECO Browser:
translate G ECOAllSections {όλες οι ενότητες ECO}
translate G ECOSection {ενότητας ECO}
translate G ECOSummary {Περίληψη της}
translate G ECOFrequency {Συχνότητα υποκωδίκων για}

# Opening Report:
translate G OprepTitle {Έκθεση ανοίγματος}
translate G OprepReport {Έκθεση/αναφορά}
translate G OprepGenerated {Δημιουργήθηκε από τον/την}
translate G OprepStatsHist {Στατιστικά και Ιστορικό}
translate G OprepStats {Στατιστικά}
translate G OprepStatAll {Όλες οι παρτίδες της αναφοράς}
translate G OprepStatBoth {Βαθμολογήθηκαν}
translate G OprepStatSince {Από}
translate G OprepOldest {Παλαιότερες παρτίδες}
translate G OprepNewest {Νεότερες παρτίδες}
translate G OprepPopular {Τρέχουσα δημοτικότητα}
translate G OprepFreqAll {Συχνότητα σε όλα τα χρόνια:   }
translate G OprepFreq1   {Από το έτος  1 έως σήμερα: }
translate G OprepFreq5   {Από το έτος  5 έως σήμερα: }
translate G OprepFreq10  {Από το έτος 10 έως σήμερα: }
translate G OprepEvery {μια φορά κάθε %u παρτίδες}
translate G OprepUp {%u%s έτη και άνω}
translate G OprepDown {%u%s έτη και κάτω}
translate G OprepSame {χωρίς αλλαγή σε όλα τα έτη}
translate G OprepMostFrequent {Πιο συχνοί παίκτες}
translate G OprepMostFrequentOpponents {Πιο συχνοί αντίπαλοι}
translate G OprepRatingsPerf {Βαθμολογήσεις και επιδόσεις}
translate G OprepAvgPerf {Μέσες βαθμολογήσεις και επιδόσεις}
translate G OprepWRating {Βαθμολόγηση Λευκών}
translate G OprepBRating {Βαθμολόγηση Μαύρων}
translate G OprepWPerf {Επίδοση Λευκών}
translate G OprepBPerf {Επίδοση Μαύρων}
translate G OprepHighRating {Παρτίδες με υψηλότερη μέση βαθμολόγηση}
translate G OprepTrends {Τάσεις αποτελεσμάτων}
translate G OprepResults {Μήκη και συχνότητες αποτελεσμάτων}
translate G OprepLength {Μήκος παρτίδας}
translate G OprepFrequency {Συχνότητα}
translate G OprepWWins {Τα Λευκά κερδίζουν: }
translate G OprepBWins {Τα Μαύρα κερδίζουν: }
translate G OprepDraws {Ισοπαλίες:      }
translate G OprepWholeDB {ολόκληρη η βάση δεδομένων}
translate G OprepShortest {Οι συντομότερες νίκες}
translate G OprepMovesThemes {Κινήσεις και Θέματα}
translate G OprepMoveOrders {Σειρές κινήσεων που καταλήγουν στην θέση}
translate G OprepMoveOrdersOne \
  {Υπάρχει μόνο μια σειρά κινήσεων που καταλήγει στην θέση αυτή:}
translate G OprepMoveOrdersAll \
  {Υπάρχουν %u σειρές κινήσεων που καταλήγουν στην θέση αυτή:}
translate G OprepMoveOrdersMany \
  {Υπάρχουν %u σειρές κινήσεων που καταλήγουν στην θέση αυτή. Οι πρώτες %u είναι:}
translate G OprepMovesFrom {Κινήσεις από την θέση}
translate G OprepMostFrequentEcoCodes {Συχνότεροι κωδικοί ECO}
translate G OprepThemes {Θέματα θέσης}
translate G OprepThemeDescription {Συχνότητα θεμάτων κατά τις πρώτες %u κινήσεις κάθε παρτίδας}
translate G OprepThemeSameCastling {Ομόπλευρα ροκέ}
translate G OprepThemeOppCastling {Αντίθετα ροκέ}
translate G OprepThemeNoCastling {Και οι δυο Βασιλιάδες χωρίς ροκέ}
translate G OprepThemeKPawnStorm {Επίθεση πιονιών Βασιλιά}
translate G OprepThemeQueenswap {Ανταλλαγή Βασιλισσών}
translate G OprepThemeWIQP {Λευκό απομονωμένο πιόνι Βασίλισσας}
translate G OprepThemeBIQP {Μαύρο απομονωμένο πιόνι Βασίλισσας}
translate G OprepThemeWP567 {Λευκό πιόνι στην 5/6/7η σειρά}
translate G OprepThemeBP234 {Μαύρο πιόνι στην 2/3/4η σειρά}
translate G OprepThemeOpenCDE {Ανοικτή Open c/d/e στήλη}
translate G OprepTheme1BishopPair {Μόνον η μια πλευρά διαθέτει ζεύγος Αξιωματικών}
translate G OprepEndgames {Φινάλε}
translate G OprepReportGames {Αναφορά παρτίδων}
translate G OprepAllGames    {Όλες οι παρτίδες}
translate G OprepEndClass {Υλικό στο τέλος κάθε παρτίδας}
translate G OprepTheoryTable {Πίνακας ανοιγμάτων}
translate G OprepTableComment {Δημιουργήθηκε από τις %u με την υψηλότερη βαθμολογία.}
translate G OprepExtraMoves {Επιπλέον κινήσεις στις σημειώσεις του πίνακα ανοιγμάτων}
translate G OprepMaxGames {Μέγιστος αριθμός παρτίδων στον πίνακα ανοιγμάτων}
translate G OprepViewHTML {Εμφάνιση HTML}
translate G OprepViewLaTeX {Εμφάνιση LaTeX}

# Player Report:
translate G PReportTitle {Έκθεση παίκτη}
translate G PReportColorWhite {με τα Λευκά κομμάτια}
translate G PReportColorBlack {με τα Μαύρα κομμάτια}
translate G PReportMoves {μετά από %s}
translate G PReportOpenings {Ανοίγματα}
translate G PReportClipbase {Διαγραφή της προσωρινής μνήμης clipbase και αντιγραφή σε αυτήν των παρτίδων που ταιριάζουν}

# Piece Tracker window:
translate G TrackerSelectSingle {Το αριστερό πλήκτρο του ποντικιού επιλέγει το κομμάτι αυτό.}
translate G TrackerSelectPair {Το αριστερό πλήκτρο του ποντικιού επιλέγει το κομμάτι αυτό, το δεξιό πλήκτρο επιλέγει το ταίρι του.}
translate G TrackerSelectPawn {Το αριστερό πλήκτρο επιλέγει το πιόνι αυτό, το δεξιό πλήκτρο επιλέγει και τα 8 πιόνια.}
translate G TrackerStat {Στατιστικά}
translate G TrackerGames {% παρτίδες με κίνηση στο τετράγωνο}
translate G TrackerTime {% χρόνο σε κάθε τετράγωνο}
translate G TrackerMoves {Κινήσεις}
translate G TrackerMovesStart {Εισάγετε τον αριθμό της κίνησης από την οποία θα ξεκινήσει η ανίχνευση.}
translate G TrackerMovesStop {Εισάγετε τον αριθμό της κίνησης στην οποία θα σταματήσει η ανίχνευση.}

# Game selection dialogs:
translate G SelectAllGames {Όλες οι παρτίδες της βάσης δεδομένων}
translate G SelectFilterGames {Μόνο παρτίδες στο φίλτρο}
translate G SelectTournamentGames {Μόνο παρτίδες του προκριματικού}
translate G SelectOlderGames {Μόνο παλαιότερες παρτίδες}

# Delete Twins window:
translate G TwinsNote {Για να είναι δίδυμες, δυο παρτίδες πρέπει να έχουν τουλάχιστον τους ίδιους παίκτες και τα κριτήρια που θα ορίσετε πιο κάτω. Όταν εντοπίζεται κάποιο ζεύγος δίδυμων, η μικρότερης διάρκειας παρτίδα διαγράφεται. Συμβουλή: πρέπει να ελέγχετε την ορθογραφία της βάσης δεδομένων πριν διαγράψετε δίδυμες παρτίδες, διότι βελτιώνει τον εντοπισμό τους.}
translate G TwinsCriteria {Κριτήρια: Οι δίδυμες παρτίδες πρέπει να διαθέτουν...}
translate G TwinsWhich {Εξέταση ποιάς παρτίδας}
translate G TwinsColors {Με παίκτη ιδίου χρώματος;}
translate G TwinsEvent {Με ίδια διοργάνωση;}
translate G TwinsSite {Με ίδια τοποθεσία;}
translate G TwinsRound {Με ίδιο γύρο;}
translate G TwinsYear {Με ίδιο έτος;}
translate G TwinsMonth {Με ίδιο μήνα;}
translate G TwinsDay {Με ίδια ημέρα;}
translate G TwinsResult {Με ίδιο αποτέλεσμα;}
translate G TwinsECO {Με ίδιο κωδικό ECO;}
translate G TwinsMoves {Με ίδιες κινήσεις}
translate G TwinsPlayers {Σύγκριση ονομάτων παικτών:}
translate G TwinsPlayersExact {Ταιριάζουν ακριβώς}
translate G TwinsPlayersPrefix {Μόνο τα 4 πρώτα γράμματα}
translate G TwinsWhen {Όταν διαγράφονται δίδυμες παρτίδες}
translate G TwinsSkipShort {Να αγνοούνται όλες οι παρτίδες με μήκος μικρότερο των 5 κινήσεων;}
translate G TwinsUndelete {Πρώτα να διαγραφούν όλες οι παρτίδες;}
translate G TwinsSetFilter {Να μπουν φίλτρα σε όλες τις διαγραμμένες δίδυμες παρτίδες;}
translate G TwinsComments {Πάντοτε διατήρηση των παρτίδων με σχόλια;}
translate G TwinsVars {Πάντοτε διατήρηση των παρτίδων με βαριάντες;}
translate G TwinsDeleteWhich {Διαγραφή της παρτίδας:}
translate G TwinsDeleteShorter {Συντομότερη παρτίδα}
translate G TwinsDeleteOlder {Ο μικρότερος αριθμός παρτίδας}
translate G TwinsDeleteNewer {Ο μεγαλύτερος αριθμός παρτίδας}
translate G TwinsDelete {Διαγραφή παρτίδας}

# Name editor window:
translate G NameEditType {Τύπος του προς επεξεργασία ονόματος}
translate G NameEditSelect {Παρτίδες για επεξεργασία}
translate G NameEditReplace {Αντικατάσταση}
translate G NameEditWith {με}
translate G NameEditMatches {Ταιριάσματα: Για επιλογή, πατήστε Ctrl+1 έως Ctrl+9}

# Check games window:
translate G CheckGames {Έλεγχος παρτίδων}
translate G CheckGamesWhich {Έλεγχος παρτίδων}
translate G CheckAll {Όλες οι παρτίδες}
translate G CheckSelectFilterGames {Μόνον οι παρτίδες του φίλτρου}

# Classify window:
translate G Classify {Ταξινόμηση}
translate G ClassifyWhich {Ταξινόμηση κατά ECO των εξής παρτίδων}
translate G ClassifyAll {Όλες οι παρτίδες (αντικατάσταση των παλαιών κωδίκων κατά ECO)}
translate G ClassifyYear {Όλες οι παρτίδες που παίχτηκαν την προηγούμενη χρονιά}
translate G ClassifyMonth {Όλες οι παρτίδες που παίχτηκαν τον προηγούμενο μήνα}
translate G ClassifyNew {Μόνον παρτίδες χωρίς ακόμη κώδικα ECO}
translate G ClassifyCodes {Κωδικοί ECO προς χρήση}
translate G ClassifyBasic {Μόνον βασικοί κωδικοί ("B12", ...)}
translate G ClassifyExtended {Επεκτάσεις Scid ("B12j", ...)}

# Compaction:
translate G NameFile {Αρχείο ονομάτων}
translate G GameFile {Αρχείο παρτίδων}
translate G Names {Ονόματα}
translate G Unused {Μη χρησιμοποιημένα}
translate G SizeKb {Μέγεθος (kb)}
translate G CurrentState {Τρέχουσα κατάσταση}
translate G AfterCompaction {Μετά την συμπίεση}
translate G CompactNames {Συμπιεσμένο αρχείο ονομάτων}
translate G CompactGames {Συμπιεσμένο αρχείο παρτίδων}
translate G NoUnusedNames "Δεν υπάρχουν ονόματα που δεν χρησιμοποιούνται, γι' αυτό το αρχείο ονομάτων είναι πλήρως συμπιεσμένο."
translate G NoUnusedGames "Το αρχείο παρτίδων είναι ήδη πλήρως συμπιεσμένο."
translate G GameFileCompacted {Το αρχείο παρτίδων για την βάση δεδομένων συμπιέστηκε.}

# Sorting:
translate G SortCriteria {Κριτήρια}
translate G AddCriteria {Προσθήκη κριτηρίων}
translate G CommonSorts {Κοινές ταξινομήσεις}
translate G Sort {Ταξινόμηση}

# Exporting:
translate G AddToExistingFile {Να προστεθούν οι παρτίδες σε υπάρχον αρχείο;}
translate G ExportComments {Να εξαχθούν τα σχόλια;}
translate G ExportVariations {Να εξαχθούν οι βαριάντες}
translate G IndentComments {Να έχουν εσοχές τα σχόλια;}
translate G IndentVariations {Να έχουν εσοχές οι βαριάντες;}
translate G ExportColumnStyle {Είδος στήλης (μια κίνηση ανά γραμμή);}
translate G ExportSymbolStyle {Είδος συμβολικού υπομνηματισμού:}
translate G ExportStripMarks {Να απομακρυνθούν τα σημάδια τετραγώνου/βέλους από τα σχόλια;}

# Goto game/move dialogs:
translate G LoadGameNumber {Εισάγετε τον αριθμό παρτίδας για φόρτωση:}
translate G GotoMoveNumber {Μετάβαση στην κίνηση αριθμός:}

# Copy games dialog:
translate G CopyGames {Αντιγραφή παρτίδων}
translate G CopyConfirm {
 Θέλετε πράγματι να αντιγράψετε τις
 [::utils::thousands $nGamesToCopy] φιλτραρισμένες παρτίδες
 της βάσης δεδομένων "$fromName"
 στην βάση δεδομένων "$targetName";
}
translate G CopyErr {Αδύνατη η αντιγραφή των παρτίδων}
translate G CopyErrSource {η αρχική βάση δεδομένων}
translate G CopyErrTarget {η τελική βάση δεδομένων}
translate G CopyErrNoGames {δεν διαθέτει παρτίδες στο φίλτρο της}
translate G CopyErrReadOnly {είναι μόνον για ανάγνωση}
translate G CopyErrNotOpen {δεν είναι ανοικτή}

# Colors:
translate G LightSquares {Λευκά τετράγωνα}
translate G DarkSquares {Μαύρα τετράγωνα}
translate G SelectedSquares {Επιλεγμένα τετράγωνα}
translate G SuggestedSquares {Τετράγωνα που προτείνονται για την κίνηση}
translate G WhitePieces {Λευκά κομμάτια}
translate G BlackPieces {Μαύρα κομμάτια}
translate G WhiteBorder {Λευκό περίγραμμα}
translate G BlackBorder {Μαύρο περίγραμμα}

# Novelty window:
translate G FindNovelty {Εύρεση νεοτερισμού}
translate G Novelty {Νεοτερισμός}
translate G NoveltyInterrupt {Η αναζήτηση νεοτερισμού διακόπηκε}
translate G NoveltyNone {Δεν βρέθηκε νεοτερισμός για αυτή την παρτίδα}
translate G NoveltyHelp {
Το Scid θα βρει την πρώτη κίνηση της τρέχουσας παρτίδας η οποία καταλήγει σε θέση που δεν υπάρχει στην επιλεχθείσα βάση δεδομένων ή στο βιβλίο ECO των ανοιγμάτων.
}

# Sounds configuration:
translate G SoundsFolder {Κατάλογος αρχείων ήχου}
translate G SoundsFolderHelp {Ο κατάλογος πρέπει να περιέχει τα αρχεία King.wav, a.wav, 1.wav, κλπ.}
translate G SoundsAnnounceOptions {Επιλογές αναγγελίας κίνησης}
translate G SoundsAnnounceNew {Αναγγελία των νέων κινήσεων καθώς εκτελούνται}
translate G SoundsAnnounceForward {Αναγγελία όταν πρόκειται για μια κίνηση προς τα εμπρός}
translate G SoundsAnnounceBack {Αναγγελία όταν πρόκειται για αναδρομή ή για κίνηση προς τα πίσω}
# ====== TODO To be translated ======
translate G SoundsSoundDisabled {Scid could not find the Snack audio package at startup;\nSound is disabled.}

# Upgrading databases:
translate G Upgrading {Ενημέρωση}
translate G ConfirmOpenNew {
Αυτή η βάση δεδομένων είναι παλαιάς μορφής (Scid 3) την οποία δεν μπορεί να ανοίξει το Scid 4, αλλά έχει ήδη δημιουργηθεί μια έκδοσή της νέας μορφής (Scid 4).

Θέλετε να ανοίξετε την βάση δεδομένων νέας μορφής;
}
translate G ConfirmUpgrade {
Αυτή η βάση δεδομένων είναι παλαιάς μορφής (Scid 3). Πρέπει να δημιουργηθεί μια βάση νέας μορφής για το Scid 4.

Η ανανέωση θα δημιουργήσει μια νέα έκδοση της βάσης δεδομένων και κατόπιν θα διαγράψει τα αρχικά αρχεία.

Αυτό ίσως διαρκέσει λίγο χρόνο, αλλά θα χρειαστεί να το κάνετε μόνο μια φορά. Μπορείτε να ακυρώσετε την διαδικασία αν διαρκεί πάρα πολύ.

Θέλετε να ενημερώσετε αυτή την βάση δεδομένων τώρα;
}

# Recent files options:
translate G RecentFilesMenu {Πλήθος των πρόσφατων αρχείων του μενού Αρχείο}
translate G RecentFilesExtra {Πλήθος των πρόσφατων αρχείων του επιπλέον υπομενού}

# My Player Names options:
translate G MyPlayerNamesDescription {
Δημιουργήστε παρακάτω μια λίστα των ονομάτων των προτιμόμενων παικτών, ένα για κάθε γραμμή. Τα συμβολα μπαλαντέρ (π.χ. "?" για κάνε έναν μοναδικό χαρακτήρα, "*" για οποιαδήποτε ακολουθία χαρακτήρων) επιτρέπονται.

Κάθε φορά που θα φορτώνεται μια παρτίδα με παίκτη που υπάρχειστην λίστα, αν είναι απαραίτητο η σκακιέρα του κυρίως παραθύρου θα περιστρέφεται ώστε η παρτίδα να προβάλεται από την μεριά αυτού του παίκτη.
}

#Coach
translate G showblunderexists {εμφάνιση ότι το σφάλμα υπάρχει}
translate G showblundervalue {εμφάνιση της αξίας του σφάλματος}
translate G showscore {εμφάνιση σκορ}
translate G coachgame {διδασκαλία παρτίδας}
translate G configurecoachgame {Προσαρμογή τακτικής παρτίδας}
translate G configuregame {Προσαρμογή παρτίδας}
translate G Phalanxengine {Μηχανή Phalanx}
translate G Coachengine {Μηχανή Coach}
translate G difficulty {δυσκολία}
translate G hard {δύσκολη}
translate G easy {εύκολη}
translate G Playwith {Παιχνίδι με}
translate G white {λευκά}
translate G black {μαύρα}
translate G both {και τα δυο}
translate G Play {Παιχνίδι}
translate G Noblunder {Χωρίς σφάλματα}
translate G blunder {σφάλμα}
translate G Noinfo {-- Καμια πληροφορία --}
translate G PhalanxOrTogaMissing {Δεν βρέθηκε Phalanx ή Toga}
translate G moveblunderthreshold {η κίνηση είναι σφάλμα αν η απώλεια είναι μεγαλύτερη από}
translate G limitanalysis {περιορισμός του χρόνου ανάλυσης της μηχανής}
translate G seconds {δευτερόλεπτα}
translate G Abort {Ματαίωση}
translate G Resume {Επανάληψη}
translate G OutOfOpening {Εκτός ανοίγματος}
translate G NotFollowedLine {Δεν ακολοθήσατε την γραμμή}
translate G DoYouWantContinue {Θέλετε να συνεχίσετε;}
translate G CoachIsWatching {Ο προπονητής παρακολουθεί}
translate G Ponder {Μόνιμη σκέψη}
translate G LimitELO {Περιορισμός της δύναμης ELO}
translate G DubiousMovePlayedTakeBack {Η κίνηση που παίχτηκε είναι αμφίβολη, θέλετε να την αναιρέσετε;}
translate G WeakMovePlayedTakeBack {Η κίνηση που παίχτηκε είναι αδύναμη, θέλετε να την αναιρέσετε;}
translate G BadMovePlayedTakeBack {Η κίνηση που παίχτηκε είναι κακή, θέλετε να την αναιρέσετε;}
translate G Iresign {Παραιτούμαι}
translate G yourmoveisnotgood {η κίνησή σας δεν είναι καλή}
translate G EndOfVar {Τέλος βαριάντας}
translate G Openingtrainer {Προπόνηση ανοιγμάτων}
translate G DisplayCM {Εμφάνιση υποψήφιων κινήσεων}
translate G DisplayCMValue {Εμφάνιση των αξιών των υποψήφιων κινήσεων}
translate G DisplayOpeningStats {Εμφάνιση στατιστικών}
translate G ShowReport {Εμφάνιση έκθεσης}
translate G NumberOfGoodMovesPlayed {οι καλές κινήσεις που παίχτηκαν}
translate G NumberOfDubiousMovesPlayed {οι αμφίβολες κινήσεις που παίχτηκαν}
translate G NumberOfMovesPlayedNotInRepertoire {οι κινήσεις που παίχτηκαν δεν υπάρχουν στο ρεπερτόριο}
translate G NumberOfTimesPositionEncountered {φορές εμφάνισης της θέσης}
translate G PlayerBestMove  {Να επιτρέπονται μόνον οι καλύτερες κινήσεις}
translate G OpponentBestMove {Ο αντίπαλος παίζει τις καλύτερες κινήσεις}
translate G OnlyFlaggedLines {Μόνον γραμμές με σημαίες}
translate G resetStats {Μηδενισμός στατιστικών}
translate G Repertoiretrainingconfiguration {Προσαρμογή προπόνησης ρεπερτορίου}
translate G Loadingrepertoire {Φόρτωση ρεπερτορίου}
translate G Movesloaded {Κινήσεις που φορτώθηκαν}
translate G Repertoirenotfound {Το ρεπερτόριο δεν βρέθηκε}
translate G Openfirstrepertoirewithtype {Πρώτα άνοιγμα μιας βάσης δεδομένων ρεπερτορίου με την εικόνα ή/και τον τύπο τοποθετημένη στην δεξιά πλευρά}
translate G Movenotinrepertoire {Η κίνηση δεν υπάρχει στο ρεπερτόριο}
translate G PositionsInRepertoire {Θέσεις στο ρεπερτόριο}
translate G PositionsNotPlayed {Θέσεις που δεν παίχτηκαν}
translate G PositionsPlayed {Θέσεις που παίχτηκαν}
translate G Success {Επιτυχία}
translate G DubiousMoves {Αμφίβολες κινήσεις}
translate G OutOfRepertoire {Εκτός ρεπερτορίου}
translate G ConfigureTactics {Προσαρμογή τακτικής}
translate G ResetScores {Μηδενισμός σκορ}
translate G LoadingBase {Φόρτωση βάσης}
translate G Tactics {Τακτική}
translate G ShowSolution {Εμφάνιση λύσης}
translate G NextExercise {Επόμενη άσκηση}
translate G PrevExercise {Προηγούμενη άσκηση}
translate G StopTraining {Διακοπή προπόνησης}
translate G Next {Επόμενο}
translate G ResettingScore {Μηδενισμός σκορ}
translate G LoadingGame {Φόρτωση παρτίδας}
translate G MateFound {Βρέθηκε ματ}
translate G BestSolutionNotFound {Δεν βρέθηκε βέλτιστη λύση !}
translate G MateNotFound {Δεν βρέθηκε ματ}
translate G ShorterMateExists {Υπάρχει συντομότερο ματ}
translate G ScorePlayed {Score played}
translate G Expected {αναμενόμενη}
translate G ChooseTrainingBase {Επιλογή προπονητικής βάσης δεδομένων}
translate G Thinking {Ανάλυση}
translate G AnalyzeDone {Η ανάλυση ολοκληρώθηκε}
translate G WinWonGame {Κερδισμένη παρτίδα}
translate G Lines {Γραμμές}
translate G ConfigureUCIengine {Προσαρμογή μηχανής UCI}
translate G SpecificOpening {Συγκεκριμένο άνοιγμα}
translate G StartNewGame {Έναρξη νέας παρτίδας}
translate G FixedLevel {Σταθερό επίπεδο}
translate G Opening {Άνοιγμα}
translate G RandomLevel {Τυχαίο επίπεδο}
translate G StartFromCurrentPosition {Εκκίνηση από την τρέχουσα θέση}
translate G FixedDepth {Σταθερού βάθους}
translate G Nodes {Κόμβοι}
translate G Depth {Βάθος}
translate G Time {Χρόνος} 
translate G SecondsPerMove {Δευτερόλεπτα ανά κίνηση}
translate G Engine {Μηχανή}
translate G TimeMode {Λειτουργία χρόνου}
translate G TimeBonus {Χρόνος + δώρο}
translate G TimeMin {min}
translate G TimeSec {sec}
translate G AllExercisesDone {Ολοκληρώθηκαν όλες οι ασκήσεις}
translate G MoveOutOfBook {Κίνηση εκτός βιβλίου}
translate G LastBookMove {Κίνηση τελευταίου βιβλίου}
translate G AnnotateSeveralGames {Υπομνηματισμός πολλαπλών παρτίδων \nαπό την τρέχουσα σε :}
translate G FindOpeningErrors {Εύρεση σφαλμάτων ανοιγμάτων}
translate G MarkTacticalExercises {Σημάδεμα ασκήσεων τακτικής}
translate G UseBook {Χρήση του βιβλίου}
translate G MultiPV {Πολλαπλές βαριάντες}
translate G Hash {Μνήμη κατατεμαχισμού (hash)}
translate G OwnBook {Χρήση του βιβλίου της μηχανής}
translate G BookFile {Βιβλίο ανοιγμάτων}
translate G AnnotateVariations {Υπομνηματισμός βαριαντών}
translate G ShortAnnotations {Σύντομος υπομνηματισμός}
translate G addAnnotatorTag {Προσθήκη εκτικέτας του υπομνηματιστή}
translate G AddScoreToShortAnnotations {Προσθήκη σκορ στους υπομνηματισμούς}
translate G Export {Εξαγωγή}
translate G BookPartiallyLoaded {Το βιβλίο φορτώθηκε μερικώς}
translate G Calvar {Υπολογισμο βαριαντών}
translate G ConfigureCalvar {Προσαρμογή}
# Opening names used in tacgame.tcl
translate G Reti {Reti}
translate G English {Αγγλική}
translate G d4Nf6Miscellaneous {1.d4 Nf6 Διάφορες άλλες}
translate G Trompowsky {Trompowsky}
translate G Budapest {Βουδαπέστης}
translate G OldIndian {Παλαιά Ινδική}
translate G BenkoGambit {Γκαμπί Benko}
translate G ModernBenoni {Σύγχρονη Benoni}
translate G DutchDefence {Ολλανδική άμυνα}
translate G Scandinavian {Σκανδιναβική}
translate G AlekhineDefence {Άμυνα Αλιέχιν}
translate G Pirc {Pirc}
translate G CaroKann {Caro-Kann}
translate G CaroKannAdvance {Caro-Kann Advance}
translate G Sicilian {Σικελική}
translate G SicilianAlapin {Σικελική Αλάπιν}
translate G SicilianClosed {Σικελική Κλειστή}
translate G SicilianRauzer {Σικελική Rauzer}
translate G SicilianDragon {Σικελικός Δράκος}
translate G SicilianScheveningen {Σικελική Σεβενίγκεν}
translate G SicilianNajdorf {Sicilian Νάιντορφ}
translate G OpenGame {Ανοικτή παρτίδα}
translate G Vienna {Βιέννη}
translate G KingsGambit {Γκαμπί του Βασιλιά}
translate G RussianGame {Ρωσική παρτίδα}
translate G ItalianTwoKnights {Ιταλική/Δυο Ίππων}
translate G Spanish {Ισπανική}
translate G SpanishExchange {Ισπανική Ανταλλαγής}
translate G SpanishOpen {Ισπανικό άνοιγμα}
translate G SpanishClosed {Ισπανική Κλειστή}
translate G FrenchDefence {Γαλλική Άμυνα}
translate G FrenchAdvance {Γαλλική Advance}
translate G FrenchTarrasch {Γαλλική Τάρρας}
translate G FrenchWinawer {Γαλλική Γουίναγουερ}
translate G FrenchExchange {Γαλλική Ανταλλαγής}
translate G QueensPawn {Πιόνι Βασίλισσας}
translate G Slav {Σλαβική}
translate G QGA {Γκαμπί της Βασίλισσας Αποδεκτό (QGA)}
translate G QGD {Γκαμπί της Βασίλισσας μη Αποδεκτό (QGD)}
translate G QGDExchange {Γκαμπί της Βασίλισσας μη Αποδεκτό Ανταλλαγής}
translate G SemiSlav {Ημι-Σλαβική}
translate G QGDwithBg5 {Γκαμπί της Βασίλισσας μη Αποδεκτό με Bg5}
translate G QGDOrthodox {Ορθόδοξο Γκαμπί της Βασίλισσας μη Αποδεκτό}
translate G Grunfeld {Γρκύνφελντ}
translate G GrunfeldExchange {Γκρύνφελντ Ανταλλαγής}
translate G GrunfeldRussian {Γκρύνφελντ Ρωσική}
translate G Catalan {Καταλανική}
translate G CatalanOpen {Καταλανική Ανοικτή}
translate G CatalanClosed {Καταλανική Κλειστή}
translate G QueensIndian {Ινδική της Βασίλισσας}
translate G NimzoIndian {Νιμτσοϊνδική}
translate G NimzoIndianClassical {Κλασσική Νιμτσοϊνδική}
translate G NimzoIndianRubinstein {Νιμτσοϊνδική Ρουμπινστάιν}
translate G KingsIndian {Ινδική του Βασιλιά}
translate G KingsIndianSamisch {Ινδική του Βασιλιά Σέμις}
translate G KingsIndianMainLine {Κύρια Γραμμή Ινδικής του Βασιλιά}

# FICS
translate G ConfigureFics {Προσαρμογή FICS}
translate G FICSGuest {Σύνδεση ως Καλεσμένος/η}
translate G FICSServerPort {Θύρα Διακομηστή}
translate G FICSServerAddress {Διεύθυνση IP}
translate G FICSRefresh {Ανανέωση}
translate G FICSTimesealPort {Θύρα Timeseal}
translate G FICSSilence {Φίλτρο κονσόλας}
translate G FICSOffers {Προσφορές}
translate G FICSConsole {Κονσόλα}
translate G FICSGames {Παρτίδες}
translate G FICSUnobserve {Διακοπή παρακολούθησης παρτίδας}
translate G FICSProfile {Εμφάνιση του ιστορικού και του προφίλ σας}
translate G FICSRelayedGames {Αναμεταδιδόμενες παρτίδες}
translate G FICSFindOpponent {Εύρεση αντιπάλου}
translate G FICSTakeback {Αναίρεση}
translate G FICSTakeback2 {Αναίρεση 2}
translate G FICSInitTime {Αρχικός χρόνος (min)}
translate G FICSIncrement {Προσαύξηση (sec)}
translate G FICSRatedGame {Παρτίδα βαθμολόγησης}
translate G FICSAutoColour {αυτόματο}
translate G FICSManualConfirm {χειροκίνηση επιβεβαίωση}
translate G FICSFilterFormula {Φίλτρο με φόρμουλα}
translate G FICSIssueSeek {Αναζήτηση τεύχους}
translate G FICSChallenge {Πρόκληση}
translate G FICSAccept {αποδεκτή}
translate G FICSDecline {μη αποδεκτή}
translate G FICSColour {Χρώμα}
translate G FICSSend {αποστολή}
translate G FICSConnect {Σύνδεση}
translate G FICSdefaultuservars {Χρήση αρχικών μεταβλητών}
translate G FICSObserveconfirm {Θέλετε να παρακολουθήσετε την παρτίδα}
# ====== TODO To be translated ======
translate G FICSpremove {Enable premove}

# Game review
translate G GameReview {Επισκόπηση παρτίδας}
translate G GameReviewTimeExtended {Ο χρόνος επεκτάθηκε}
translate G GameReviewMargin {Περιθώριο λάθους}
translate G GameReviewAutoContinue {Αυτόματη συνέχιση όταν η κίνηση είναι σωστή}
translate G GameReviewReCalculate {Χρήση εκτεταμένου χρόνου}
translate G GameReviewAnalyzingMovePlayedDuringTheGame {Γίνεται ανάλυση της κίνησης που παίχτηκε κατά την διάρκεια της παρτίδας}
translate G GameReviewAnalyzingThePosition {Γίνεται ανάλυση της θέσης}
translate G GameReviewEnterYourMove {Εισάγετε την κίνησή σας}
translate G GameReviewCheckingYourMove {Ελέγχεται η κίνησή σας}
translate G GameReviewYourMoveWasAnalyzed {Η κίνησή σας αναλύθηκε}
translate G GameReviewYouPlayedSameMove {Παίξατε την ίδια κίνηση όπως και στο ματς}
translate G GameReviewScoreOfYourMove {Το σκορ της κίνησής σας}
translate G GameReviewGameMoveScore {Το σκορ κίνησης της παρτίδας}
translate G GameReviewEngineScore {Το σκορ της μηχανής}
translate G GameReviewYouPlayedLikeTheEngine {Παίξατε το ίδιο καλά με την μηχανή}
translate G GameReviewNotEngineMoveButGoodMove {Η κίνηση δεν είναι της μηχανής, αλλά είναι μια καλή κίνηση}
translate G GameReviewMoveNotGood {Η κίνηση αυτή δεν είναι καλή, το σκορ είναι}
translate G GameReviewMovesPlayedLike {Κινήσεις που παίχτηκαν σαν}
translate G GameReviewMovesPlayedEngine {Κινήσεις που παίχτηκαν σαν της μηχανής}

# Correspondence Chess Dialogs:
translate G CCDlgConfigureWindowTitle {Προσαρμογή Σκακιού Δι' αλληλογραφίας}
translate G CCDlgCGeneraloptions {Γενκές επιλογές}
translate G CCDlgDefaultDB {Αρχική Βάση δεδομένων:}
translate G CCDlgInbox {ΕισερχόμεναInbox (διαδρομή):}
translate G CCDlgOutbox {Εξερχόμενα (διαδρομή):}
translate G CCDlgXfcc {Παραμετροποίηση Xfcc:}
translate G CCDlgExternalProtocol {Χειριστής Εξωτερικού Πρωτοκόλου (π.χ. Xfcc)}
translate G CCDlgFetchTool {Εργαλείο ανάκτησης:}
translate G CCDlgSendTool {Εργαλείο αποστολής:}
translate G CCDlgEmailCommunication {Επικοινωνία eMail}
translate G CCDlgMailPrg {Πρόγραμμα αλληλογραφίας:}
translate G CCDlgBCCAddr {Διεύθυνση (B)CC:}
translate G CCDlgMailerMode {Λειτουργία:}
translate G CCDlgThunderbirdEg {π.χ. Thunderbird, Mozilla Mail, Icedove...}
translate G CCDlgMailUrlEg {π.χ. Evolution}
translate G CCDlgClawsEg {π.χ. Sylpheed Claws}
translate G CCDlgmailxEg {π.χ. mailx, mutt, nail...}
translate G CCDlgAttachementPar {Παράμτερος επισυναπτόμενου:}
translate G CCDlgInternalXfcc {Χρήση της εσωτερικής υποστήριξης Xfcc}
translate G CCDlgConfirmXfcc {Επιβεβαίωση κινήσεων}
translate G CCDlgSubjectPar {Παράμετρος Θέματος:}
translate G CCDlgDeleteBoxes {Κενό Εισ/Εξερχόμενα}
translate G CCDlgDeleteBoxesText {Θέλετε πράγματι να αδειάσετε τους καταλόγους Εισερχομένων και Εξερχομένων;\nΚάτι τέτοιο απαιτεί έναν νέο συγχρονισμός για να εμφανιστεί η πρόσφατη κατάσταση των παρτίδων σας.}
translate G CCDlgConfirmMove {Επιβεβαίωση κίνησης}
translate G CCDlgConfirmMoveText {Αν επιβεβαιώσετε, θα σταλούν στον διακομηστή η ακόλουθη κίνηση και το ακόλουθο σχόλιο:}
translate G CCDlgDBGameToLong {Αντιφατική Mainline}
translate G CCDlgDBGameToLongError {Η mainline της βάσης δεδομένων σας είναι μεγαλύτερη από ό,τι η παρτίδα στα Εισερχόμενά σας. Αν τα Εισερχόμενα περιέχουν τρέχουσες παρτίδες, δηλαδή, αμέσως μετά από έναν συγχρονισμός, κάποιες κινήσεις προστέθηκαν στην mainline της βάσης δεδομένων με τρόπο λανθασμένο.

Στην περίπτωση αυτή συντομεύστε την mainline στην κίνηση (μέγιστο)
}


translate G CCDlgStartEmail {Έναρξη νέας παρτίδας με eMail}
translate G CCDlgYourName {Το όνομά σας:}
translate G CCDlgYourMail {Η ηλ. διεύθυνσή σας:}
translate G CCDlgOpponentName {Όνομα αντιπάλου:}
translate G CCDlgOpponentMail {Ηλ. διεύθυνση αντιπάλου:}
translate G CCDlgGameID {Ταυτότητα παρτίδας (μοναδική):}

translate G CCDlgTitNoOutbox {Scid: Εξερχόμενα Σκακιού Δι' Αλληλογραφίας}
translate G CCDlgTitNoInbox {Scid: Εισερχόμενα Σκακιού Δι' Αλληλογραφίας}
translate G CCDlgTitNoGames {Scid: Δεν υπάρχουν παρτίδες δι' αλληλογραφίας}
translate G CCErrInboxDir {Κατάλογος εισερχομένων σκακιού δι' αλληλογραφίας:}
translate G CCErrOutboxDir {Κατάλογος εξερχομένων σκακιού δι' αλληλογραφίας:}
translate G CCErrDirNotUsable {δεν υπάρχει ή δεν είναι προσβάσιμος!\nΠαρακαλώ ελέγξτε και διορθώστε τις ρυθμίσεις.}
translate G CCErrNoGames {does not contain any games!\nPlease fetch them first.}

translate G CCDlgTitNoCCDB {Scid: Δεν υπάρχει βάση δεδομένων αλληλογραφίας}
translate G CCErrNoCCDB {Δεν ανοίχτηκε κάποια βάση δεδομένων τύπου 'Αλληλογραφία'. Παρακαλώ ανοίξτε μια πριν χρησιμοποιήσετε τις λειτουργίες σκακιού δι' αλληλογραφίας.}

translate G CCFetchBtn {Ανάκτηση παρτίδων από τον διακομηστή και επεξεργασία Εισερχομένων}
translate G CCPrevBtn {Μετάβαση σε προηγούμενη παρτίδα}
translate G CCNextBtn {Μετάβαση σε επόμενη παρτίδα}
translate G CCSendBtn {Αποστολή κίνησης}
translate G CCEmptyBtn {Εκκένωση Εισερχομένων και Εξερχομένων}
translate G CCHelpBtn {Βοήθεια για τα εικονίδια και τους δείκτες κατάστασης.\nΓια γενική Βοήθεια πατήστε F1!}

translate G CCDlgServerName {ΌΝομα διακομηστή:}
translate G CCDlgLoginName  {Όνομα σύνδεσης:}
translate G CCDlgPassword   {Κωδικός:}
# ====== TODO To be translated ======
translate G CCDlgShowPassword {Show password}
translate G CCDlgURL        {Xfcc-URL:}
translate G CCDlgRatingType {Τύπως αξιολόγησης:}

translate G CCDlgDuplicateGame {Μη μοναδική ταυτότητα παρτίδας}
translate G CCDlgDuplicateGameError {Η παρτίδα αυτή υπάρχει περισσότερες από μια φορές στην βάση δεδομένων σας. Παρακαλώ διαγράψτε τις διπλότυπες και συμπιέστε το αρχείο παρτίδων σας (Αρχείο/Συντήρηση/Συμπίεση βάσης δεδομένων).}

translate G CCDlgSortOption {Ταξινόμηση:}
translate G CCDlgListOnlyOwnMove {Μόνον παρτίδες στις οποίες παίζω εγώ}
translate G CCOrderClassicTxt {Τοποθεσία, Διοργάνωση, Γύρος, Αποτέλεσμα, Λευκά, Μαύρα}
translate G CCOrderMyTimeTxt {Το ρολόι μου}
translate G CCOrderTimePerMoveTxt {Χρόνος ανά κίνηση μέχρι τον επόμενο έλεγχο χρόνου}
translate G CCOrderStartDate {Ημερομηνία έναρξης}
translate G CCOrderOppTimeTxt {Ρολόι αντιπάλου}

translate G CCDlgConfigRelay {Παρακολούθηση παρτίδων}
translate G CCDlgConfigRelayHelp {Μετάβαση στην σελίδα παρτίδας http://www.iccf-webchess.com και εμφάνιση της παρτίδας για παρακολούθηση. Αν βλέπετε την σκακιέρα αντιγράψτε την διεύθυνση URL από τον φυλλομετρητή σας στην παρακάτω λίστα. Μόνον ένα URL ανά γραμμή!\nΠαράδειγμα: http://www.iccf-webchess.com/MakeAMove.aspx?id=266452}

# Connect Hardware dialogs
translate G ExtHWConfigConnection {Προσαρμογή εξωτερικών συσκευών}
translate G ExtHWPort {Θύρα}
translate G ExtHWEngineCmd {Εντολή μηχανής}
translate G ExtHWEngineParam {Παράμετροι μηχανής}
translate G ExtHWShowButton {Εμφάνιση κουμπιού}
translate G ExtHWHardware {Συσκευές}
translate G ExtHWNovag {Novag Citrine}
translate G ExtHWInputEngine {Μηχανής εισόδου}
translate G ExtHWNoBoard {No board}
translate G NovagReferee {Διαιτητής}

# Input Engine dialogs
translate G IEConsole {Κονσόλα εισόδου μηχανής}
translate G IESending {Κινήσεις που εστάλησαν για}
translate G IESynchronise {Συγχρονισμός}
translate G IERotate  {Περιστροφή}
translate G IEUnableToStart {Αδυναμία εκκίνησης της μηχανής εισόδου:}


# Calculation of Variations
translate G DoneWithPosition {Ολοκλήρωση της θέσης}

translate G Board {Σκακιέρα}
translate G showGameInfo {Εμφάνιση πληροφοριών παρτίδας}
translate G autoResizeBoard {Αυτόματη αλλαγή μεγέθους σκακιέρας}
translate G DockTop {Μετάβαση στην κορυφή}
translate G DockBottom {Μετάβαση στο κάτω μέρος}
translate G DockLeft {Μετάβαση αριστερά}
translate G DockRight {Μετάβαση δεξιά}
translate G Undock {Αποκόληση}

# Switcher window
translate G ChangeIcon {Αλλαγή εικονιδίου...}
# ====== TODO To be translated ======
translate G NewGameListWindow {New Game List Window}
# ====== TODO To be translated ======
translate G LoadatStartup {Load at startup}
# ====== TODO To be translated ======
translate G ShowHideDB {Show/Hide databases}
# ====== TODO To be translated ======
translate G ChangeFilter {Change filter}
# ====== TODO To be translated ======
translate G ChangeLayout {Load/Save/Change sorting criteria and column layout}
# ====== TODO To be translated ======
translate G ShowHideStatistic {Show/Hide statistics}
# ====== TODO To be translated ======
translate G BoardFilter {Show only games that matches the current board position}
# ====== TODO To be translated ======
translate G CopyGameTo {Copy Game to}
# ====== TODO To be translated ======
translate G FindBar {Find Bar}
# ====== TODO To be translated ======
translate G FindCurrentGame {Find current game}
# ====== TODO To be translated ======
translate G DeleteGame {Delete game}
# ====== TODO To be translated ======
translate G UndeleteGame {Undelete game}
# ====== TODO To be translated ======
translate G ResetSort {Reset sort}
# ====== TODO To be translated ======
translate G ConvertNullMove {Convert null moves to comments}
# ====== TODO To be translated ======
translate G SetupBoard {Setup Board}
# ====== TODO To be translated ======
translate G Rotate {Rotate}
# ====== TODO To be translated ======
translate G SwitchColors {Switch colors}
# ====== TODO To be translated ======
translate G FlipBoard {Flip Board}
# ====== TODO To be translated ======
translate G ImportPGN {Import PGN game}
# ====== TODO To be translated ======
translate G ImportingFiles {Importing PGN files in}
# ====== TODO To be translated ======
translate G ImportingFrom {Importing from}
# ====== TODO To be translated ======
translate G ImportingIn {Import games in}
# ====== TODO To be translated ======
translate G UseLastTag {Use last\ngame's tags}
# ====== TODO To be translated ======
translate G Random {Random}
# ====== TODO To be translated ======
translate G BackToMainline {Go back to mainline}
# ====== TODO To be translated ======
translate G LeaveVariant {Leave variant}
# ====== TODO To be translated ======
translate G Autoplay {Autoplay}
# ====== TODO To be translated ======
translate G ShowHideCoords {Show/Hide Coord.}
# ====== TODO To be translated ======
translate G ShowHideMaterial {Show/Hide Material}
# ====== TODO To be translated ======
translate G FullScreen {Full Screen}
# ====== TODO To be translated ======
translate G FilterStatistic {Filter Statistics}
# ====== TODO To be translated ======
translate G MakeCorrections {Make Corrections}
# ====== TODO To be translated ======
translate G Surnames {Surnames}
# ====== TODO To be translated ======
translate G Ambiguous {Ambiguous}
# ====== TODO To be translated ======
translate G OptionsToolbar "Γραμμή εργαλειών"
translate G OptionsBoard "Σκακιέρα"
# ====== TODO To be translated ======
translate G OptionsMenuColor "Menu colors"
translate G OptionsBoardSize "Μέγεθος"
translate G OptionsBoardPieces "Είδος κομματιών"
# ====== TODO To be translated ======
translate G OptionsInternationalization "Internationalization"
# ====== TODO To be translated ======
translate G MenuColorForeground "Textcolor"
# ====== TODO To be translated ======
translate G MenuColorBackground "Background"
# ====== TODO To be translated ======
translate G MenuColorSelect "Select"
# ====== TODO To be translated ======
translate G OptionsTablebaseDir "Select up to 4 table base folders:"
}
# end of greek.tcl
