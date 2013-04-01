/***************************************************************************
 *   (C) 2006 by Michal Rudolf mrudolf@kdewebdev.org                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "chessbrowser.h"
#include "settings.h"
#include "game.h"
#include "databaseinfo.h"

#include <QMenu>
#include <QLCDNumber>
#include <QToolBar>


ChessBrowser::ChessBrowser(QWidget *p, bool showGameMenu) : QTextBrowser(p), toolBar(0), m_gameMenu(NULL)
{
    setObjectName("ChessBrowser");
	setContextMenuPolicy(Qt::CustomContextMenu);
	setupMenu(showGameMenu);

    int fontsize = AppSettings->getValue("/GameText/FontSize").toInt();
    setFontSize(fontsize);

}

void ChessBrowser::setSource(const QUrl&)
{
}

void ChessBrowser::showMove(int id)
{
	if (id)
		selectAnchor(QString("move:%1").arg(id));
	else {	// First move
		QTextCursor cursor = textCursor();
		cursor.movePosition(QTextCursor::Start);
		setTextCursor(cursor);
	}
}

void ChessBrowser::selectAnchor(const QString& href)
{
	for (QTextBlock block = document()->begin(); block != document()->end(); block = block.next()) {
		QTextBlock::iterator it;
		for (it = block.begin(); !it.atEnd(); ++it) {
			QTextFragment fragment = it.fragment();
			if (!fragment.isValid())
				continue;
			QTextCharFormat format = fragment.charFormat();
			if (format.isAnchor() && format.anchorHref() == href) {
				QTextCursor cursor(document());
				cursor.setPosition(fragment.position());
				cursor.setPosition(fragment.position() + fragment.length(), QTextCursor::KeepAnchor);
				setTextCursor(cursor);
				ensureCursorVisible();
				return;
			}
		}
	}
}

void ChessBrowser::saveConfig()
{
    AppSettings->setLayout(this);
}

void ChessBrowser::slotReconfigure()
{
    AppSettings->layout(this);

    int fontSizeSettingValue = AppSettings->getValue("/GameText/FontSize").toInt();
    if( fontSizeSettingValue != m_fontSize )
    {
        setFontSize(fontSizeSettingValue);
    }
}

void ChessBrowser::setupMenu(bool setupGameMenu)
{
	if (setupGameMenu) {
		m_gameMenu = new QMenu(this);
		connect(m_gameMenu, SIGNAL(triggered(QAction*)), SLOT(slotAction(QAction*)));

        m_gameMenu->addAction((m_startComment = createAction(tr("Add start comment..."), EditAction::EditPrecomment)));
        m_gameMenu->addAction((m_addComment = createAction(tr("Add comment..."), EditAction::EditComment)));

		// Nag menus
		QMenu* nagMoveMenu = m_gameMenu->addMenu(tr("Add move symbol"));
        for (int n = MoveNagStart; n <= MoveNagEnd; ++n)
			if (n != SingularMove)
				nagMoveMenu->addAction(createNagAction(Nag(n)));
		QMenu* nagPositionMenu = m_gameMenu->addMenu(tr("Add evaluation symbol"));
		nagPositionMenu->addAction(createNagAction(DrawishPosition));
        for (int n = UnclearPosition; n <= BlackHasADecisiveAdvantage; ++n)
			nagPositionMenu->addAction(createNagAction(Nag(n)));
		nagPositionMenu->addAction(createNagAction(Nag(WhiteHasSufficientCompensationForMaterialDeficit)));
		QMenu* nagSpecialMenu = m_gameMenu->addMenu(tr("Add other symbol"));
        QMenu* subMenu;
        nagSpecialMenu->addAction(createNagAction(WhiteHasTheInitiative));
        nagSpecialMenu->addAction(createNagAction(WhiteHasTheAttack));
        nagSpecialMenu->addAction(createNagAction(WhiteHasModerateCounterplay));
		nagSpecialMenu->addAction(createNagAction(WithTheIdea));
		nagSpecialMenu->addAction(createNagAction(BetterMove));
		nagSpecialMenu->addAction(createNagAction(Novelty));
        nagSpecialMenu->addAction(createNagAction(WhiteIsInZugzwang));
        nagSpecialMenu->addAction(createNagAction(WeakPoint));
		nagSpecialMenu->addAction(createNagAction(EndGame));
        nagSpecialMenu->addAction(createNagAction(WhiteHasSevereTimeControlPressure));
        subMenu = nagSpecialMenu->addMenu(tr("Bishops"));
        subMenu->addAction(createNagAction(WhiteHasAPairOfBishops));
        subMenu->addAction(createNagAction(BishopsOfOppositeColor));
        subMenu->addAction(createNagAction(BishopsOfSameColor));

		m_gameMenu->addAction(m_removeNags = createAction(tr("Remove symbols"), EditAction::ClearNags));
        m_gameMenu->addSeparator();
        m_gameMenu->addAction((m_enumerateVariations1 = createAction(tr("Enumerate Variations A) B) C)"), EditAction::EnumerateVariations1)));
        m_gameMenu->addAction((m_enumerateVariations2 = createAction(tr("Enumerate Variations a) b) c)"), EditAction::EnumerateVariations2)));
		m_gameMenu->addSeparator();
        m_gameMenu->addAction((m_promoteVariation = createAction(tr("Promote to main line"), EditAction::PromoteVariation)));
        m_gameMenu->addAction((m_VariationUp = createAction(tr("Move variation up"), EditAction::VariationUp)));
        m_gameMenu->addAction((m_VariationDown = createAction(tr("Move variation down"), EditAction::VariationDown)));
        m_gameMenu->addSeparator();
        m_gameMenu->addAction((m_removeVariation = createAction(tr("Remove variation"), EditAction::RemoveVariation)));
        m_gameMenu->addAction((m_removePrevious = createAction(tr("Remove previous moves"), EditAction::RemovePreviousMoves)));
        m_gameMenu->addAction((m_removeNext = createAction(tr("Remove next moves"), EditAction::RemoveNextMoves)));
        m_gameMenu->addSeparator();
        m_gameMenu->addAction((m_addNullMove = createAction(tr("Insert threat"), EditAction::AddNullMove)));
	}

	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(slotContextMenu(const QPoint&)));

}

void ChessBrowser::slotContextMenu(const QPoint& pos)
{
	// Handle non-game browser
    if (!m_gameMenu)
    {
		m_mainMenu->exec(mapToGlobal(pos));
		return;
	}

    const Game* game;
    emit queryActiveGame(game);

	// Handle game browser
    if (!game)
		return;
	QString link = anchorAt(pos);
	if (link.isEmpty())
		return;

	m_currentMove = link.section(':', 1).toInt();

    bool isVariation = !game->isMainline(m_currentMove);
    bool atLineStart = game->atLineStart(m_currentMove);
    bool atGameStart = m_currentMove == 0 || game->atGameStart(m_currentMove - 1);
    bool hasComment = !game->annotation(m_currentMove).isEmpty();
    bool hasPrecomment = !game->annotation(m_currentMove, Game::BeforeMove).isEmpty();
    bool hasNags = !game->nags().isEmpty();
    bool atLineEnd = game->atLineEnd(m_currentMove);

	m_startComment->setVisible(atLineStart && !hasPrecomment);
	m_addComment->setVisible(!hasComment);
    m_enumerateVariations1->setVisible(isVariation);
    m_enumerateVariations2->setVisible(isVariation);
	m_promoteVariation->setVisible(isVariation);
	m_removeVariation->setVisible(isVariation);
    m_VariationUp->setVisible(isVariation && game->canMoveVariationUp(m_currentMove));
    m_VariationDown->setVisible(isVariation && game->canMoveVariationDown(m_currentMove));
	m_removeNext->setVisible(!atLineEnd);
	m_removePrevious->setVisible(!atGameStart);
	m_removeNags->setVisible(hasNags);
    m_addNullMove->setVisible(atLineEnd);
	m_gameMenu->exec(mapToGlobal(pos));
}

void ChessBrowser::setFontSize(int size)
{
    QFont f = qApp->font();
    qreal r = size;
    f.setPointSize(r);
    setFont(f);
    m_fontSize = size;
}


void ChessBrowser::slotAction(QAction* action)
{
	if (m_actions.contains(action)) {
		EditAction editAction = m_actions[action];
		editAction.setMove(m_currentMove);
		emit actionRequested(editAction);
	}
}

QAction* ChessBrowser::createAction(const QString& name, EditAction::Type type)
{
	QAction* action = new QAction(name, this);
	m_actions[action] = EditAction(type);
	return action;
}

QAction* ChessBrowser::createNagAction(const Nag& nag)
{
    QAction* action = new QAction(NagSet::nagToMenuString(nag), this);
	m_actions[action] = EditAction(EditAction::AddNag, nag);
	return action;
}

void ChessBrowser::slotDisplayTime(const QString& text, Color color)
{
    if (toolBar)
    {
        QString objectName = QString("Clock") + QString::number(color);
        QLCDNumber* clock = toolBar->findChild<QLCDNumber*>(objectName);
        if (clock)
        {
            clock->display(text);
        }
        objectName = QString("Clock") + QString::number(1-(int)color);
        clock = toolBar->findChild<QLCDNumber*>(objectName);
        if (clock)
        {
            clock->display("");
        }
    }
}
