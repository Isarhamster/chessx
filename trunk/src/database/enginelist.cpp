/***************************************************************************
 *   (C) 2010 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "enginelist.h"
#include "settings.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif // _MSC_VER

EngineList::EngineList()
{
}

void EngineList::restoreEmptyFromPath(const QString& path, EngineData::EngineProtocol protocol)
{
    if(path.isEmpty())
    {
        return;
    }

    QStringList engines = QDir(path).entryList(QDir::Executable | QDir::Files | QDir::NoSymLinks);

    for(int i = 0; i < engines.size(); ++i)
    {
        QString name = engines[i];
        EngineData data(name);
        data.command = path + QDir::separator() + name;
        data.directory = path;
        data.protocol = protocol;
        data.logging = false;
        data.sendHistory = true;
        append(data);
    }
}

void EngineList::restoreEmpty()
{
    restoreEmptyFromPath(AppSettings->uciPath(), EngineData::UCI);
    restoreEmptyFromPath(AppSettings->winboardPath(), EngineData::WinBoard);
}

void EngineList::restore()
{
    clear();
    AppSettings->beginGroup("/Engines/");
    QStringList engines = AppSettings->childGroups();
    bool modified = false;
    if(engines.size())
    {
        for(int i = 0; i < engines.size(); ++i)
        {
            QString key = QString::number(i);
            QString name = AppSettings->value(key + "/Name").toString();
            EngineData data(name);
            data.command = AppSettings->value(key + "/Command").toString();
            data.options = AppSettings->value(key + "/Options").toString();
            data.directory = AppSettings->value(key + "/Directory").toString();
            data.logging = AppSettings->value(key + "/Logging").toBool();
            data.sendHistory = AppSettings->value(key + "/SendHistory", "true").toBool();
            QString protocolName = AppSettings->value(key + "/Protocol").toString();
            if(protocolName == "WinBoard")
            {
                data.protocol = EngineData::WinBoard;
            }
            else
            {
                data.protocol = EngineData::UCI;
            }
            AppSettings->getMap(key + "/OptionValues", data.m_optionValues);
            append(data);
        }
    }
    else
    {
        restoreEmpty();
        modified = true;
    }
    AppSettings->endGroup();
    if(modified)
    {
        save();
    }
}

void EngineList::save()
{
    AppSettings->beginGroup("/Engines/");
    AppSettings->remove("");	// Clear group
    for(int i = 0; i < count(); ++i)
    {
        QString key = QString::number(i);
        AppSettings->setValue(key + "/Name", at(i).name);
        AppSettings->setValue(key + "/Command", at(i).command);
        AppSettings->setValue(key + "/Logging", at(i).logging);
        AppSettings->setValue(key + "/SendHistory", at(i).sendHistory);
        if(!at(i).options.isEmpty())
        {
            AppSettings->setValue(key + "/Options", at(i).options);
        }
        if(!at(i).directory.isEmpty())
        {
            AppSettings->setValue(key + "/Directory", at(i).directory);
        }
        if(at(i).protocol == EngineData::WinBoard)
        {
            AppSettings->setValue(key + "/Protocol", "WinBoard");
        }
        else
        {
            AppSettings->setValue(key + "/Protocol", "UCI");
        }
        AppSettings->setMap(key + "/OptionValues", at(i).m_optionValues);
    }
    AppSettings->endGroup();
}

QStringList EngineList::names() const
{
    QStringList engineNames;
    for(int i = 0; i < count(); ++i)
    {
        engineNames.append(at(i).name);
    }
    return engineNames;
}
