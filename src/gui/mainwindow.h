/***************************************************************************
 *   (C) 2005-2009 by Michal Rudolf mrudolf@kdewebdev.org                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "common.h"
#include "historylist.h"
#include "kbaction.h"
#include "move.h"
#include "output.h"

#include <QtGui>
#include <QAction>
#include <QMainWindow>
#include <QUndoGroup>

class Analysis;
class AnalysisWidget;
class Board;
class BoardView;
class ChessBrowser;
class Database;
class DatabaseInfo;
class DatabaseList;
class DownloadManager;
class ECOListWidget;
class EcoThread;
class EditAction;
class EventListWidget;
class Game;
class GameList;
class OpeningTreeWidget;
class PlayerListWidget;
class QLabel;
class QNetworkAccessManager;
class QNetworkReply;
class QProgressBar;
class QSlider;
class QSplitter;
class QTimer;
class QToolBar;
class SaveDialog;
class TableView;
class ToolMainWindow;
class TranslatingSlider;

/**
@defgroup GUI GUI - User interface components
*/

/**
@ingroup GUI
The MainWindow class represents main ChessX window. */

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

    static const int MaxRecentFiles = 10;

protected:
    /** QObjects Eventfilter for QApplication events */
    bool eventFilter(QObject *obj, QEvent *event);
    /** Overridden to ask for confirmation */
    void closeEvent(QCloseEvent* e);
    /** Filter key events. */
    void keyPressEvent(QKeyEvent *e);
    /** @return active database */
    Database* database();
    /** @return active database structure */
    DatabaseInfo* databaseInfo();
    /** @return database name of database @p index (or active, if no parameter). */
    QString databaseName(int index = -1) const;
    /** @return active database structure */
    const DatabaseInfo* databaseInfo() const;
    /** @return database structure */
    DatabaseInfo* databaseInfo(QString name);
    /** @return active game */
    Game& game();
    const Game& game() const;
    /** @return index of active game */
    int gameIndex() const;
    /** Edit comment */
    bool gameEditComment(Output::CommentType type);
    /** Get export filename*/
    QString exportFileName(int& format);
    /** Load game @p index. If @p force is false, does nothing for incorrect @p index .
    With @p force set, creates empty game. */
    void gameLoad(int index, bool force = false, bool reload = 0);
    /** Make given number of moves in current game */
    bool gameMoveBy(int change);
    /** Update recent files menu */
    void updateMenuRecent();
    /** Save Game dialog (created when used first) */
    SaveDialog* saveDialog();
public slots:
    /** Open database */
    void openDatabase(QString fname);
    /** Open database from URL*/
    void openDatabaseUrl(QString fname, bool utf8);
    /** Open a list of databases from a ZIP archive */
    void openDatabaseArchive(QString fname, bool utf8);
    /** Open database from a local File */
    void openDatabaseFile(QString fname, bool utf8);
    /** Add favorite status to a database */
    void setFavoriteDatabase(QString fname);
    /** Promote current variation. */
    void slotEditVarPromote();
    /** Remove current variation. */
    void slotEditVarRemove();
    /** Show Setup Position dialog and set current board. */
    void slotEditBoard();
    /** Edit current comment. */
    void slotEditComment();
    /** Edit current comment. */
    void slotEditCommentBefore();
    /** Set position's FEN to clipboard. */
    void slotEditCopyFEN();
    /** Set position's human readable FEN to clipboard. */
    void slotEditCopyHumanFEN();
    /** Set position using FEN from clipboard. */
    void slotEditPasteFEN();
    /** Set position's PGN to clipboard. */
    void slotEditCopyPGN();
    /** Set position using PGN from clipboard. */
    bool slotEditPastePGN();
    /** Set position using PGN from clipboard. */
    void slotEditPaste();
    /** Set game or position from clipboard */
    void slotEditCopyImage();
    /** Truncate moves to the end of the game. */
    void slotEditTruncateEnd();
    /** Truncate moves from the beginning of the game. */
    void slotEditTruncateStart();
    /**  */
    void slotReconfigure();
    /**  */
    void slotConfigure();
    /** About ChessX */
    void slotHelpAbout();
    /** Display Tracker at ChessX SourceForge */
    void slotHelpBug();
    void slotConfigureFlip();
    /** Go to first move of the game */
    void slotGameMoveFirst()
    {
        gameMoveBy(-999);
    }
    /** Go to next move of the game */
    bool slotGameMoveNext();
    /** Go to previous move of the game */
    void slotGameMovePrevious()
    {
        gameMoveBy(-1);
    }
    /** Go to last move of the game */
    void slotGameMoveLast()
    {
        gameMoveBy(999);
    }
    /** Go N moves forward in the game. For now fixed at @p 10 */
    void slotGameMoveNextN()
    {
        gameMoveBy(10);
    }
    /** Go N moves backward in the game. For now fixed at @p 10 */
    void slotGameMovePreviousN()
    {
        gameMoveBy(-10);
    }
    /** Load first game */
    void slotGameLoadFirst();
    /** Load next game */
    void slotGameLoadNext();
    /** Load previous game */
    void slotGameLoadPrevious();
    /** Load last game */
    void slotGameLoadLast();
    /** Load random game */
    void slotGameLoadRandom();
    /** Choose a game and load it */
    void slotGameLoadChosen();
    /** Enter variation. */
    void slotGameVarEnter();
    /** Enter previous variation. */
    void slotGameVarUp();
    /** Enter next variation. */
    void slotGameVarDown();
    /** Exit variation. */
    void slotGameVarExit();
    /** Modify game on user's request. */
    void slotGameModify(const EditAction& action);
    /** Get a pointer or Null to the active game. */
    void slotGetActiveGame(const Game** game);
    /** Merge the game with index @p gameIndex into the active game. */
    void slotMergeActiveGame(int gameIndex);
    /** Merge all games from the database into the active game. */
    void slotMergeAllGames();
    /** Merge all games from the filter into the active game. */
    void slotMergeFilter();
    /** Update GameView content after game was change. Triggers also @ref slotMoveChanged. */
    void slotGameChanged();
    /** Handle link click in Game View panel */
    void slotGameViewLink(const QUrl& link);
    /** Handle link click in Game View panel */
    void slotGameViewLink(const QString& link);
    /** Get GameView source. */
    void slotGameViewSource();
    /** Dump all node information to the console */
    void slotGameDumpMoveNodes();
    /** Create new empty game (to be appended to the database. */
    void slotGameNew();
    /** Save game, replacing old one if it is edited, appending if it is new
        @return false if the saving was cancelled, true if changes are to be saved or discarded. */
    bool slotGameSave();
    /** Add variation to current position. */
    void slotGameAddVariation(const Analysis& analysis);
    /** Add variation to current position. */
    void slotGameAddVariation(const QString& san);
    /** Remove all comments. */
    void slotGameUncomment();
    /** Remove all variations */
    void slotGameRemoveVariations();
    /** Start / Stop AutoPlay feature */
    void slotToggleAutoPlayer();
    /** Auto Play Timeout - make next move! */
    void slotToggleAutoAnalysis();
    /** Auto Play Timeout - make next move! */
    void slotAutoPlayTimeout();
    /** Auto Play Timeout - make next move! */
    void slotEngineTimeout(const Analysis& analysis);
    /** Filter was changed - update status bar information */
    void slotFilterChanged();
    /** Load given game (triggered from Game List) */
    void slotFilterLoad(int index);
    /** Creates an empty chessxdatabase*/
    void slotFileNew();
    /** Open File dialog, choose a database and open it */
    void slotFileOpen();
    /** Open File dialog, choose a database and open it in UTF8 format */
    void slotFileOpenUtf8();
    /** Save database to current file. */
    void slotFileSave();
    /** Open given database from Recent files list. */
    void slotFileOpenRecent();
    /** Exports game to selected file type. */
    void slotFileExportGame();
    /** Exports games in filter to selected file type. */
    void slotFileExportFilter();
    /** Exports all games to selected file type. */
    void slotFileExportAll();
    /** Close current database. Switches to next active (or clipboard if there is none). */
    void slotFileClose();
    /** Close selected database. */
    void slotFileCloseIndex(int);
    /** Close selected database. */
    void slotFileCloseName(QString fname);
    /** Quit ChessX. Ask for confirmation before */
    void slotFileQuit();
    /** Find games matching single tag. */
    void slotSearchTag();
    /** Find current position */
    void slotSearchBoard();
    /** Receives the signal of a search board operation started */
    void slotBoardSearchStarted();
    /** Receives the signal of a search board operation end */
    void slotBoardSearchUpdate();
    /** Reset current filter */
    void slotSearchReset();
    /** Training mode */
    void slotToggleTraining();
    /** Training mode - auto responder */
    void slotToggleAutoRespond();
    /** Reverse current filter */
    void slotSearchReverse();
    /** Make an Update of the game list after the opening tree was updated */
    void slotTreeUpdate();
    /** Show opening tree */
    void slotSearchTree();
    /** Move @p index was selected in Opening Tree. */
    void slotSearchTreeMove(const QModelIndex& index);
    /** Made given move on the board */
    void slotBoardMove(Square from, Square to, int button);
    /** Board square was clicked */
    void slotBoardClick(Square square, int button, QPoint pos, Square from);
    /** Browse current game by mouse wheel */
    void slotBoardMoveWheel(int wheel);
    /** Update GUI after current move was changed. Update BoardView to current board,
    updates GameInfo and highlight current move in GameView. */
    void slotMoveChanged();
    /** Show temporary message. */
    void slotStatusMessage(const QString& msg);
    /** Show progress bar for open file. */
    void slotOperationProgress(int progress);
    /** Change database. */
    void slotDatabaseChange();
    /** Copy games between databases. */
    void slotDatabaseCopy(int preselect = 1);
    /** Copy games between databases. */
    void slotDatabaseCopySingle(int n);
    /** Database was changed - change informations. */
    void slotDatabaseChanged();
    /** Delete current game. */
    void slotDatabaseDeleteGame(int n);
    /** Delete all game in filter. */
    void slotDatabaseDeleteFilter();
    /** Slot that updates internal info upon loading a complete db */
    void slotDataBaseLoaded(DatabaseInfo* db);
    /** Fill up the current game (drag request from game list) */
    void slotGetGameData(Game& g);
    /** Copy game from other database by drag'n'drop */
    void copyGame(QString fileName, int index);
    /** Copy all games from other database by drag'n'drop */
    void copyDatabase(QString target, QString src);
    /** Request renaming a event in the current database */
    void slotRenameEvent(QString ts);
    /** Request renaming a player in the current database */
    void slotRenamePlayer(QString ts);
    /** Pass a tag changing request from rename dialog to database */
    void slotRenameRequest(QString tag, QString newValue, QString oldValue);

protected slots:
    /** Update recent files menu */
    void updateMenuDatabases();
    /** Receiver for a failed loading of a database */
    void loadError(QUrl url);
    /** Receiver for a successful loading of a database */
    void loadReady(QUrl url, QString fileName);
    /** Remove Color of the square in m_annotationSquare */
    void slotNoColorSquare();
    /** Colorize the square in m_annotationSquare green */
    void slotGreenSquare();
    /** Colorize the square in m_annotationSquare yellow */
    void slotYellowSquare();
    /** Colorize the square in m_annotationSquare red */
    void slotRedSquare();
    /** Remove any arrow */
    void slotNoArrowHere();
    /** Draw a green arrow */
    void slotGreenArrowHere();
    /** Draw a yellow arrow */
    void slotYellowArrowHere();
    /** Draw a red arrow */
    void slotRedArrowHere();
    /** Set the Stay On Top Property */
    void slotToggleStayOnTop();
    /** Create a new, empty Board document */
    void slotCreateBoardView();
    /** Activate an existing board document */
    void slotActivateBoardView(int);
    /** Activate an existing board document */
    void slotCloseBoardView(int n = -1);
    /** Make a screenshot and save it to file */
    void slotScreenShot();
    /** Make a screenshot and save it to file */
    void slotCompileECO();
    /** Support dragging things to non-top tabs */
    void slotAutoSwitchTab();
    /** Edit Accelerators */
    void slotEditActions();
    /** Value for Move Interval is changed */
    void slotMoveIntervalChanged(int);
    /** Value for Move Interval Slider is changed */
    void slotSetSliderText(int);

protected:
    bool pasteFen(QString& errorText, QString fen);
    void copyGame(int target, int index);
    Database* getDatabaseByPath(QString path);
    DatabaseInfo* getDatabaseInfoByPath(QString path);
    void activateBoardView(int n);
    void updateOpeningTree(const Board& b, bool atEnd);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    int m_tabDragIndex;
    QTabBar* m_pDragTabBar;
    QTimer* m_dragTimer;

signals:
    /** Re-read configuration. */
    void reconfigure();
    /** Main game has been updated. */
    void boardChange(const Board& board);
    /** Current database changed. */
    void databaseChanged(DatabaseInfo* databaseInfo);
    /** Emitted upon finishing a file download */
    void LoadFinished(DatabaseInfo*);
    /** Emitted upon a update for the elapsed / clock time */
    void displayTime(const QString&, Color);

    void signalMoveHasVariation(bool);
    void signalVariationHasSibling(bool);
    void signalMoveHasPreviousMove(bool);
    void signalMoveHasNextMove(bool);
    void signalMoveHasParent(bool);
    void signalGameIsEmpty(bool);
    void signalGameAtLineStart(bool);

    void signalCurrentDBisReadWrite(bool);
    void signalCurrentDBcanBeClosed(bool);
    void signalCurrentDBhasGames(bool);

    void signalFirstGameLoaded(bool);
    void signalLastGameLoaded(bool);

    void signalVersionFound(int, int, int);

    void databaseModified();

    void signalDatabaseOpenClose();

private slots:
    /** Cleanup after ECO file loaded. */
    void ecoLoaded(QObject*, bool);
    void slotDatabaseModified();
    void slotHttpDone(QNetworkReply *reply);
    void slotVersionFound(int major, int minor, int build);
    void slotUpdateOpeningTreeWidget();

private:
    /** Create single menu action. */
    QAction* createAction(QString name, const char* slot, const QKeySequence& key = QKeySequence(),
                          QToolBar* pToolBar = 0, QString image = QString(), const QString& tip = QString(), QAction::MenuRole menuRole = QAction::NoRole, QObject *parent=0);
    /** Create single menu action. */
    QAction* createAction(QObject *parent, QString name, const char* slot, const QKeySequence& key = QKeySequence(),
                          QToolBar* pToolBar = 0, QString image = QString(), const QString& tip = QString(), QAction::MenuRole menuRole = QAction::NoRole);
    /** Create single menu action. */
    QAction* createAction(QString name, const char* slot, const QKeySequence& key,
                          QToolBar* pToolBar, QIcon icon = QIcon(), const QString& tip = QString(), QAction::MenuRole menuRole = QAction::NoRole, QObject *parent=0);
    /** Create all the menus and actions */
    void setupActions();
    /** Confirm quitting and save modified databases. */
    bool confirmQuit();
    /** Start operation with progress reporting. Shows progress bar. */
    void startOperation(const QString& msg);
    /** Finish operation with progress reporting. Hides progress bar. */
    void finishOperation(const QString& msg);
    /** Cancel operation with progress reporting. Hides progress bar. */
    void cancelOperation(const QString& msg);
    /** Restore the list of recent files */
    void restoreRecentFiles();
    /** Query User and save game if game was modified
        @return true if the next action shall be performed */
    bool QuerySaveGame();
    /** Save game without query */
    void saveGame();
    /** Load next game without query */
    void loadNextGame();
    /** Save Database without query or progress bar */
    void saveDatabase();
    /** Save Database with query */
    bool QuerySaveDatabase();
    /** Create a new unassociated boardview */
    BoardView* CreateBoardView();
    /** Update Board Information */
    void UpdateBoardInformation();
    /** Update Game Title */
    void UpdateGameTitle();
    /** Update Game Text */
    void UpdateGameText();
    /** Check if an update is available online */
    void StartCheckUpdate();
    /** Check if download of database is an option */
    void StartCheckDatabase();

    /* Dialogs  */
    GameList* m_gameList;
    PlayerListWidget* m_playerList;
    ECOListWidget* m_ecoList;
    EventListWidget* m_eventList;
    DatabaseList* m_databaseList;
    SaveDialog* m_saveDialog;
    /* Main gui parts */
    BoardView* m_boardView;
    QSplitter* m_boardSplitter;
    ChessBrowser* m_gameView;
    OpeningTreeWidget* m_openingTreeWidget;
    QPointer<QProgressBar> m_progressBar;
    QPointer<TranslatingSlider> m_sliderSpeed;
    QLabel* m_sliderText;
    ToolMainWindow* m_gameWindow;
    QToolBar* m_gameToolBar;
    QTabWidget* m_tabWidget;
    QList<BoardView*> m_boardViews;
    /* Status */
    QLabel* m_statusFilter;
    QLabel* m_gameTitle;
    /* Menus */
    QMenu* m_menuDatabases;
    QMenu* m_menuView;

    /* Local variables */
    HistoryList m_recentFiles;
    QStringList m_favoriteFiles;
    Output* m_output;
    QList<DatabaseInfo*> m_databases;
    QList<QAction*> m_recentFileActions;
    QList<QAction*> m_databaseActions;
    int m_currentDatabase;
    QString m_eco;
    QTime m_operationTime;
    /** Currently updated tree. May be NULL if no update in progress. */
    QString m_nagText;
    QTimer* m_autoPlayTimer;
    int m_currentFrom;
    int m_currentTo;
    AnalysisWidget* m_mainAnalysis;
    Board m_AutoInsertLastBoard;
    Square m_annotationSquare;
    Square m_annotationSquareFrom;
    QAction* m_training;
    QAction* m_autoRespond;
    QAction* m_autoPlay;
    QAction* m_autoAnalysis;
    QUndoGroup m_undoGroup;
    QNetworkAccessManager* m_manager;
    DownloadManager* downloadManager;
    bool m_machineHasToMove;
};


#endif
