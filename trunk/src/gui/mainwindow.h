/***************************************************************************
                          mainwindow.h  -  main window
                             -------------------
    begin                : 10 Oct 2005
    copyright            : (C) 2005-2007 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QList>
#include <QMainWindow>
#include <QUrl>
#include <QAbstractItemView>

#include "common.h"
#include "move.h"
#include "historylist.h"

class PlayerDialog;
class GameList;
class SaveDialog;
class HelpWindow;
class BoardView;
class Game;
class Database;
class DatabaseInfo;
class Output;
class TipOfDayDialog;
class Tablebase;
class EcoThread;
class AnalysisWidget;
class Board;
class ChessBrowser;
class TableView;

class QActionGroup;
class QCloseEvent;
class QLabel;
class QMenu;
class QSplitter;
class QTextBrowser;


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

protected:
	/** Overridden to ask for confirmation */
	void closeEvent(QCloseEvent* e);
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
	/** Open database */
	bool openDatabase(const QString& fname);
	/** Get export filename*/
	QString exportFileName(int& format);
	/** Load game @p index. If @p force is false, does nothing for incorrect @p index .
	With @p force set, creates empty game. */
	void gameLoad(int index, bool force = false);
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
	/** Help window (created when used first) */
	TipOfDayDialog* tipDialog();
	/** Help window (created when used first) */
	HelpWindow* helpWindow();
public slots:
	void slotPlayerDialog();
	/** Show Setup Position dialog and set current board. */
	void slotEditBoard();
	/** Set position's FEN to clipboard. */
	void slotEditCopyFEN();
	/** Set position using FEN from clipboard. */
	void slotEditPasteFEN();
	/** Truncate moves to the end of the game. */
	void slotEditTruncateEnd();
	/** Truncate moves from the beginning of the game. */
	void slotEditTruncateStart();
	void slotReconfigure();
	void slotConfigure();
	/** Display help */
	void slotHelp();
	/** Show tip of the day */
	void slotHelpTip();
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
	/** Modify game on user's request. */
	void slotGameModify(int action, int move);
	/** Update GameView content after game was change. Triggers also @ref slotMoveChanged . */
	void slotGameChanged();
	/** Handle link click in Game View panel */
	void slotGameViewLink(const QUrl& link);
	/** Show/hide GameView source - useful for debugging. */
	void slotGameViewToggle(bool source);
	/** Create new empty game (to be appended to the database. */
	void slotGameNew();
	/** Save game, replacing old one if it is edited, appending if it is new */
	void slotGameSave();
	/** Filter was changed - update status bar information */
	void slotFilterChanged();
	/** Load given game (triggered from Game List) */
	void slotFilterLoad(int index);
	/** Creates an empty chessxdatabase*/
	void slotFileNew();
	/** Open File dialog, choose a database and open it */
	void slotFileOpen();
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
	/** Quit ChessX. Ask for confirmation before */
	void slotFileQuit();
	/** Find games matching single tag. */
	void slotSearchTag();
	/** Find current position */
	void slotSearchBoard();
	/** Reset current filter */
	void slotSearchReset();
	/** Reverse current filter */
	void slotSearchReverse();
	/** Show opening tree */
	void slotSearchTree();
	/** Move @p index was selected in Opening Tree. */
	void slotSearchTreeMove(const QModelIndex& index);
	/** Made given move on the board */
	void slotBoardMove(Square from, Square to);
	/** Board square was clicked */
	void slotBoardClick(Square square, int button);
	/** Browse current game by mouse wheel */
	void slotBoardMoveWheel(int wheel);
	/** Update GUI after current move was changed. Update BoardView to current board,
	updates GameInfo and highlight current move in GameView. */
	void slotMoveChanged();
	/** Show temporary message. */
	void slotStatusMessage(const QString& msg);
	/** Change database */
	void slotDatabaseChange();
	/** Copy games between databases */
	void slotDatabaseCopy();
	/** Database was changed - change informations */
	void slotDatabaseChanged();
	/** Show tablebase move information */
	void showTablebaseMove(Move move, int score);
signals:
	/** Re-read configuration */
	void reconfigure();
	/** Main game has been updated */
	void boardChange(const Board& board);

private slots:
	/** Cleanup after ECO file loaded */
	void ecoLoaded();

private:
	/** Create single menu action. */
	QAction* createAction(const QString& name, const char* slot, const QKeySequence& key = QKeySequence(),
			      const QString& tip = QString());
	/** Create all the menus and actions */
	void setupActions();
	/** Answer simple questions. */
	bool confirm(const QString& title, const QString& question, const QString& proceed = tr("Yes"));

	enum
	{
		GameFirst, GameLast, GameNext, GamePrevious, GameRandom, MoveFirst, MoveLast, MoveNext, MovePrevious,
		MoveNextN, MovePreviousN
	};
	/* Dialogs  */
	PlayerDialog* m_playerDialog;
	GameList* m_gameList;
	SaveDialog* m_saveDialog;
	HelpWindow* m_helpWindow;
	TipOfDayDialog* m_tipDialog;
	/* Main gui parts */
	BoardView* m_boardView;
	QSplitter* m_boardSplitter;
	ChessBrowser* m_moveView;
	ChessBrowser* m_gameView;
	TableView* m_openingTree;
	/* Status */
	QLabel* m_statusFilter;
	/* Menus */
	QMenu* m_menuDatabases;
	QMenu* m_menuView;

	/* Local variables */
	HistoryList m_recentFiles;
	Output* m_output;
	QList<DatabaseInfo*> m_databases;
	QList<QAction*> m_recentFileActions;
	QList<QAction*> m_databaseActions;
	int m_currentDatabase;
	QString m_eco;
	QActionGroup* m_actions;
	bool m_showPgnSource; // for debugging
	Tablebase* m_tablebase;
	EcoThread* m_ecothread;
	AnalysisWidget* m_analysis;
};


#endif
