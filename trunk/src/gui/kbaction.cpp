/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "kbaction.h"
#include "settings.h"
#include <QObject>

#ifdef _DEBUG
static QStringList s_allKeys;
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

KbAction::KbAction(QString text, QObject* parent, const QKeySequence &key)
: QAction(text,parent),
  m_resetKey(key)
{
    installShortcut(text, key);
}

KbAction::KbAction(const QIcon &icon, QString text, QObject* parent, const QKeySequence &key)
: QAction(icon,text,parent),
  m_resetKey(key),
  m_resetIcon(icon)
{
    installShortcut(text, key);
}

void KbAction::installShortcut(QString text, const QKeySequence &key)
{
    m_storageKey = text;
    m_storageKey.remove('&');
    if(!key.isEmpty())
    {
        setShortcut(key);
    }
}

void KbAction::resetKey()
{
    setShortcut(m_resetKey);
}

void KbAction::resetIcon()
{
    setIcon(m_resetIcon);
}

QString KbAction::getKey() const
{
    return m_storageKey;
}

void KbAction::saveKeyShortcut()
{
    QString key = getKey();
    QString value = shortcut().toString(QKeySequence::PortableText);
    AppSettings->setValue(key,value);
}

void KbAction::restoreKeyShortcut()
{
    QString key = getKey();
    QVariant varValue = AppSettings->getValue(key);
    if (varValue.isValid())
    {
        setShortcut(QKeySequence(varValue.toString(),QKeySequence::PortableText));
    }
}

void KbAction::restoreKeyboardLayoutForObject(QObject* pObject)
{
    QList<KbAction*> actions = pObject->findChildren<KbAction*>();
    AppSettings->beginGroup("/Keyboard/");
    foreach (KbAction* action, actions)
    {
        action->restoreKeyShortcut();
    }
    AppSettings->endGroup();
}
