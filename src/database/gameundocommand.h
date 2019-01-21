#ifndef GAMEUNDOCOMMAND_H
#define GAMEUNDOCOMMAND_H

#include "databaseinfo.h"
#include "game.h"

#include <QString>
#include <QObject>
#include <QUndoCommand>
#include <QPointer>
#include <QMetaType>

class DatabaseInfo;
Q_DECLARE_METATYPE(DatabaseInfo*)

class GameUndoCommand : public QUndoCommand
{
public:
    GameUndoCommand(QObject* parent, const Game& from, const Game& to, QString action) :
        QUndoCommand(action),
        m_dbInfo(static_cast<DatabaseInfo*>(parent)),
        m_fromGame(from),
        m_toGame(to),
        m_bInConstructor(true)
        {
        }

    QPointer<DatabaseInfo> m_dbInfo;
    Game m_fromGame;
    Game m_toGame;
    bool m_bInConstructor;

    void undo() { m_dbInfo->restoreState(m_fromGame); }
    void redo() { if (m_bInConstructor) m_bInConstructor=false; else m_dbInfo->restoreState(m_toGame); }
    int id() const  { return m_fromGame.currentMove(); }
    bool mergeWith(const QUndoCommand *other)
    {
        if (m_bInConstructor)
            return false;
        if (other->actionText() != "Edit annotation") // make sure we are merging comments only
            return false;
        if (actionText() != "Edit annotation") // make sure we are merging comments only
            return false;
        if (other->id() != id()) // make sure other applies to the same position
            return false;
        m_toGame = (static_cast<const GameUndoCommand*>(other))->m_toGame;
        return true;
    }
};

#endif // GAMEUNDOCOMMAND_H
