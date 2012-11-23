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

#include <QtGui>

#include "common.h"
#include "move.h"
#include "historylist.h"
#include "output.h"

class EditAction;
class Analysis;
class PlayerDialog;
class GameList;
class PlayerList;
class DatabaseList;
class SaveDialog;
class BoardView;
class Game;
class Database;
class DatabaseInfo;
class EcoThread;
class AnalysisWidget;
class Board;
class ChessBrowser;
class TableView;
class OpeningTree;
class ToolMainWindow;

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
	/** @return active game */
	Game& game();
	/** @return index of active game */
	int gameIndex() const;
	/** Edit comment */
	bool gameEditComment(Output::CommentType type);
	/** Get export filename*/
	QString exportFileName(int& format);
	/** Load game @p index. If @p force is false, does nothing for incorrect @p index .
	With @p force set, creates empty game. */
	void gameLoad(int index, bool force = false, bool reload=0);
	/** Make given number of moves in current game */
	void gameMoveBy(int change);
	/** Update recent files menu */
	void updateMenuRecent();
	/** Update recent files menu */
	void updateMenuDatabases();
	/** Player dialog (created when used first) */
	PlayerDialog* playerDialog();
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
    /** Open Player dialog */
	void slotPlayerDialog();
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
	/** Set position using FEN from clipboard. */
	void slotEditPasteFEN();
    /** Set position's PGN to clipboard. */
    void slotEditCopyPGN();
    /** Set position using PGN from clipboard. */
    void slotEditPastePGN();
    /** Set position's Image to clipboard. */
    void slotEditCopyImage();
    /** Truncate moves to the end of the game. */
	void slotEditTruncateEnd();
	/** Truncate moves from the beginning of the game. */
	void slotEditTruncateStart();
	void slotReconfigure();
	void slotConfigure();
	/** About ChessX */
	void slotHelpAbout();
	/** Display Tracker at ChessX SourceForge */
	void slotHelpBug();
	void slotConfigureFlip();
	/** Go to first move of the game */
	void slotGameMoveFirst()  {gameMoveBy(-999);}
	/** Go to next move of the game */
	void slotGameMoveNext()   {gameMoveBy(1);}
	/** Go to previous move of the game */
	void slotGameMovePrevious() {gameMoveBy(-1);}
	/** Go to first move of the game */
	void slotGameMoveLast()  {gameMoveBy(999);}
	/** Go N moves forward in the game. For now fixed at @p 10 */
	void slotGameMoveNextN()  {gameMoveBy(10);}
	/** Go N moves backward in the game. For now fixed at @p 10 */
	void slotGameMovePreviousN()  {gameMoveBy(-10);}
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
	/** Exit variation. */
	void slotGameVarExit();
	/** Modify game on user's request. */
	void slotGameModify(const EditAction& action);
	/** Update GameView content after game was change. Triggers also @ref slotMoveChanged . */
	void slotGameChanged();
	/** Load pending game. */
	void slotGameLoadPending();
	/** Handle link click in Game View panel */
	void slotGameViewLink(const QUrl& link);
	/** Handle link click in Game View panel */
	void slotGameViewLink(const QString& link);
	/** Show/hide GameView source - useful for debugging. */
	void slotGameViewToggle(bool source);
	/** Create new empty game (to be appended to the database. */
	void slotGameNew();
	/** Save game, replacing old one if it is edited, appending if it is new */
	void slotGameSave();
	/** Add variation to current position. */
	void slotGameAddVariation(const Analysis& analysis);
    /** Add variation to current position. */
    void slotGameAddVariation(const QString& san);
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
	/** Exports games in filter to PGN file. */
	void slotFileExportFilter();
	/** Exports all games to PGN file. */
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
    /** Toggle filter */
    void slotToggleFilter();
    /** Reverse current filter */
	void slotSearchReverse();
    /** Make an Update of the game list after the opening tree was updated */
    void slotTreeUpdate();
    /** Indicate start of tree update */
    void slotTreeUpdateStarted();
    /** Show opening tree */
	void slotSearchTree();
	/** Move @p index was selected in Opening Tree. */
	void slotSearchTreeMove(const QModelIndex& index);
	/** Made given move on the board */
    void slotBoardMove(Square from, Square to, int button);
	/** Board square was clicked */
	void slotBoardClick(Square square, int button);
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
    void slotDatabaseCopySingle();
	/** Database was changed - change informations. */
	void slotDatabaseChanged();
	/** Delete current game. */
	void slotDatabaseDeleteGame();
	/** Delete all game in filter. */
	void slotDatabaseDeleteFilter();
	/** Compact database by removing obsolete information. */
	void slotDatabaseCompact();
    /** Slot that updates internal info upon loading a complete db */
    void slotDataBaseLoaded(DatabaseInfo* db);
    /** Fill up the current game (drag request from game list) */
    void slotGetGameData(Game& g);
    /** Copy game from other database by drag'n'drop */
    void copyGame(QString fileName, const Game& game);

protected slots:
    void loadError(QUrl url);
    void loadReady(QUrl url, QString fileName);

protected:
    void copyGame(int target, const Game& game);

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

private slots:
	/** Cleanup after ECO file loaded. */
	void ecoLoaded();

private:
	/** Create single menu action. */
	QAction* createAction(const QString& name, const char* slot, const QKeySequence& key = QKeySequence(),
										const QString& tip = QString(), QAction::MenuRole menuRole = QAction::NoRole );
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
    /** Query User and save game if game was modified */
    void QuerySaveGame();

	/* Dialogs  */
	PlayerDialog* m_playerDialog;
	GameList* m_gameList;
    PlayerList* m_playerList;
    DatabaseList* m_databaseList;
	SaveDialog* m_saveDialog;
	/* Main gui parts */
	BoardView* m_boardView;
	QSplitter* m_boardSplitter;
	ChessBrowser* m_gameView;
    TableView* m_openingTreeView;
	QProgressBar* m_progressBar;
    ToolMainWindow* m_gameWindow;
    QToolBar* m_gameToolBar;
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
	QActionGroup* m_actions;
	bool m_showPgnSource; // for debugging
	EcoThread* m_ecothread;
	QTime m_operationTime;
	/** Currently updated tree. May be NULL if no updated in progress. */
	OpeningTree* m_openingTree;
	QString m_nagText;

	struct PendingLoad {
		Database* database;
		int game;
		PendingLoad(Database* d, int g)	{database = d; game = g;}
		PendingLoad()	{database = 0; game = 0;}
	};
	PendingLoad m_pending;
    QAction* m_toggleFilter;
    bool m_bGameChange;
};


#endif
