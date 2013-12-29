/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#include "kbaction.h"
#include "settings.h"
#include <QObject>

#ifdef _DEBUG
static QStringList s_allKeys;
#endif

KbAction::KbAction(const char* text, QObject* parent, const QKeySequence &key)
: QAction(tr(text),parent),
  m_resetKey(key)
{
    installShortcut(text, key);
}

KbAction::KbAction(const QIcon &icon, const char* text, QObject* parent, const QKeySequence &key)
: QAction(icon,tr(text),parent),
  m_resetKey(key),
  m_resetIcon(icon)
{
    installShortcut(text, key);
}

void KbAction::installShortcut(const char *text, const QKeySequence &key)
{
    m_storageKey = text;
    m_storageKey.remove('&');
    if(!key.isEmpty())
    {
#ifdef _DEBUG
        _ASSERTE(!s_allKeys.contains(m_storageKey));
        s_allKeys.add(m_storageKey);
#endif
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
