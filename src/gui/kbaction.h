/****************************************************************************
*   Copyright (C) 2013 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef KBACTION_H
#define KBACTION_H

#include <QAction>

class KbAction : public QAction
{
    Q_OBJECT

public:
    KbAction(const char* text, QObject* parent, const QKeySequence& key);
    KbAction(const QIcon &icon, const char* text, QObject* parent, const QKeySequence& key);

    QString getKey() const;

public slots:
    void resetKey();
    void resetIcon();
public:
    void saveKeyShortcut();
    void restoreKeyShortcut();

    static void restoreKeyboardLayoutForObject(QObject* pObject);
private:
    QString m_storageKey;
    QKeySequence m_resetKey;
    QIcon m_resetIcon;

    void installShortcut(const char* text, const QKeySequence& key);
};

#endif // KBACTION_H
