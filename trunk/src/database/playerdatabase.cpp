/***************************************************************************
 *   (C) 2006 Ejner Borgbjerg <ejner@users.sourceforge.net>                * 
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/


/* Documentation for managing QDataset version, see
http://doc.trolltech.com/4.0/qdatastream.html
*/
#include "playerdatabase.h"


static quint32 Magic = (quint32)0xB0D0A0D0; // 'magic' number
static quint32 Version = (quint32)100; // file format version
static QString Mapfile_suffix = ".cpm";
static QString Datafile_suffix = ".cpd";

bool PlayerDatabase::create(const QString& fname)
{
	m_dirty = false;
	m_mapfile.setFileName(fname + Mapfile_suffix);
	m_datafile.setFileName(fname + Datafile_suffix);
	if (m_mapfile.exists() || m_datafile.exists())
		return false;

// set QDataset format version to use
	if (Version == (quint32)100) {
		m_mapds.setVersion(6);
		m_datads.setVersion(6);
	} else {//default
		m_mapds.setVersion(6);
		m_datads.setVersion(6);
	}

	m_mapfile.open(QIODevice::ReadWrite);
	m_mapds.setDevice(&m_mapfile);
	m_mapds << Magic;
	m_mapds << Version;
	m_nplayers = 0;
	m_nplayers_offset = m_mapfile.pos();
	m_mapds << m_nplayers;

	m_datafile.open(QIODevice::ReadWrite);
	m_datads.setDevice(&m_datafile);
	m_datads << Magic;
	m_datads << Version;//use current version for new db's

	m_mapfile.flush();
	m_datafile.flush();

	m_dataFileCurrentPosition = m_datafile.pos();
	m_npending_adds = 0;

	return true;
}

bool PlayerDatabase::open(const QString& fname)
{
	m_dirty = false;
	m_mapfile.setFileName(fname + Mapfile_suffix);
	if (!m_mapfile.open(QIODevice::ReadWrite))
		return false;
	m_mapds.setDevice(&m_mapfile);
	quint32 map_magic;
	quint32 map_version;
	m_mapds >> map_magic;
	m_mapds >> map_version;

	if (map_magic != Magic) {
		m_mapds.unsetDevice();
		m_mapfile.close();
		return false;
	}
// set QDataset format version to use
	if (map_version == (quint32)100) {
		m_mapds.setVersion(6);
	} else {//unknown version
		m_mapds.unsetDevice();
		m_mapfile.close();
		return false;
	}

	quint32 data_magic;
	quint32 data_version;
	m_datafile.setFileName(fname + Datafile_suffix);
	if (!m_datafile.open(QIODevice::ReadWrite)) {
		m_mapds.unsetDevice();
		m_mapfile.close();
		return false;
	}
	m_datads.setDevice(&m_datafile);
	m_datads >> data_magic;
	m_datads >> data_version;

	if (data_magic != Magic) {
		m_mapds.unsetDevice();
		m_mapfile.close();
		m_datads.unsetDevice();
		m_datafile.close();
		return false;
	}
// set QDataset format version to use
	if (data_version == (quint32)100) {
		m_datads.setVersion(6);
	} else {//unknown version
		m_mapds.unsetDevice();
		m_mapfile.close();
		m_datads.unsetDevice();
		m_datafile.close();
		return false;
	}

	if (map_version != data_version) {
		m_mapds.unsetDevice();
		m_mapfile.close();
		m_datads.unsetDevice();
		m_datafile.close();
		return false;
	}

	m_dataFileCurrentPosition = m_datafile.pos();
	m_nplayers_offset = m_mapfile.pos();
	m_mapds >> m_nplayers;
	if (m_nplayers > 0) {
		m_mapds >> m_mapping;
	}
	m_npending_adds = 0;
	return true;

}

bool PlayerDatabase::removeDatabase(const QString& fname)
{
	m_mapfile.setFileName(fname + Mapfile_suffix);
	m_datafile.setFileName(fname + Datafile_suffix);
	return m_mapfile.remove() && m_datafile.remove();
}

void PlayerDatabase::close()
{
	commit();
	m_mapds.unsetDevice();
	m_mapfile.flush();
	m_mapfile.close();
	m_datads.unsetDevice();
	m_datafile.flush();
	m_datafile.close();
}

void PlayerDatabase::rollback()
{
	m_pendingUpdates.clear();
	m_dirty = false;
	m_npending_adds = 0;
}

void PlayerDatabase::commit()
{

	if (m_dirty)//current player was changed
		m_pendingUpdates.insert(m_currentPlayerName, m_currentPlayer);

	m_mapfile.seek(m_nplayers_offset);
	m_mapds << m_nplayers;

	// write non-committed changes
	m_dataFileCurrentPosition = m_datafile.size();
	m_datafile.seek(m_dataFileCurrentPosition);

	QMap<QString, PlayerData>::Iterator it;
	for (it = m_pendingUpdates.begin(); it != m_pendingUpdates.end(); ++it) {
		m_mapping.insert(it.key(), m_datafile.pos());
		m_datads << it.value().dateOfBirth().asString();
		m_datads << it.value().dateOfDeath().asString();
		m_datads << it.value().country();
		m_datads << it.value().title();
		m_datads << it.value().eloListData();
		m_datads << (qint32)(it.value().firstEloListIndex());
		m_datads << (qint32)(it.value().lastEloListIndex());
		m_datads << (qint32)(it.value().estimatedElo());
		m_datads << (qint32)(it.value().peakElo());
		m_datads << it.value().photo();
		m_datads << it.value().biography();
	}
	m_mapds << m_mapping;
	m_mapfile.flush();
	m_datafile.flush();
	m_pendingUpdates.clear();
	m_dirty = false;
	m_nplayers += m_npending_adds;
	m_npending_adds = 0;
}


PlayerData PlayerDatabase::readPlayerData(const QString& playername)
{
	PlayerData pd;
	QMap<QString, qint32>::Iterator it;
	it = m_mapping.find(playername);
	if (it == m_mapping.end()) {
//not in committed data, look in non-committed updates
		QMap<QString, PlayerData>::Iterator it2;
		it2 = m_pendingUpdates.find(playername);
		if (it2 != m_pendingUpdates.end()) {
			return it2.value();
		} else {//give up
			return pd;
		}
	}

	qint32 pos = it.value();
	m_datafile.seek(pos);//pointing to the player data

	QString birthDate;
	QString deathDate;
	QString country;
	QString title;

	m_datads >> birthDate;
	m_datads >> deathDate;
	m_datads >> country;
	m_datads >> title;

	if (birthDate.contains('.'))
		pd.setDateOfBirth(PartialDate(birthDate));
	if (deathDate.contains('.'))
		pd.setDateOfDeath(deathDate);

	pd.setCountry(country);
	pd.setTitle(title);

	QList<qint32> eloList;
	m_datads >> eloList;
	pd.eloFromListData(eloList);

	qint32 firstEloListIndex;
	qint32 lastEloListIndex;
	qint32 estimatedElo;
	qint32 peakElo;
	QImage photo;
	QString biography;

	m_datads >> firstEloListIndex;
	m_datads >> lastEloListIndex;
	m_datads >> estimatedElo;
	m_datads >> peakElo;
	m_datads >> photo;
	m_datads >> biography;

	pd.setFirstEloListIndex((int)firstEloListIndex);
	pd.setLastEloListIndex((int)lastEloListIndex);
	pd.setEstimatedElo((int)estimatedElo);
	pd.setPeakElo((int)peakElo);
	pd.setPhoto(photo);
	pd.setBiography(biography);

	return pd;
}

uint PlayerDatabase::count() const
{
	return m_nplayers + m_npending_adds;
}

bool PlayerDatabase::add(const QString& playername)
{
	if (m_mapping.contains(playername) || m_pendingUpdates.contains(playername)) {
		return false;
	}
	if (m_dirty)//previous current player was changed
		m_pendingUpdates.insert(m_currentPlayerName, m_currentPlayer);
	PlayerData pd;
	m_currentPlayerName = playername;
	m_currentPlayer = pd;
	m_dirty = true;
    ++m_npending_adds;
	return true;
}

QString PlayerDatabase::current() const
{
	return m_currentPlayerName;
}
void PlayerDatabase::setCurrent(const QString& playername)
{
	if (m_currentPlayerName.compare(playername) == 0)
		return;
	if (m_dirty)//previous current player was changed
		m_pendingUpdates.insert(m_currentPlayerName, m_currentPlayer);
	m_currentPlayerName = playername;
	m_currentPlayer = readPlayerData(playername);
	m_dirty = false;
}

bool PlayerDatabase::exists(const QString& playername) const
{
	if (m_mapping.contains(playername))
		return true;
	if (m_pendingUpdates.contains(playername))
		return true;
	return false;
}

PartialDate PlayerDatabase::dateOfBirth() const
{
	return m_currentPlayer.dateOfBirth();
}
void PlayerDatabase::setDateOfBirth(const PartialDate& date)
{
	m_currentPlayer.setDateOfBirth(date);
	if (!m_dirty)
		m_dirty = true;
}

PartialDate PlayerDatabase::dateOfDeath() const
{
	return m_currentPlayer.dateOfDeath();
}
void PlayerDatabase::setDateOfDeath(const PartialDate & date)
{
	m_currentPlayer.setDateOfDeath(date);
	if (!m_dirty)
		m_dirty = true;
}

QString PlayerDatabase::country() const
{
	return m_currentPlayer.country();
}
void PlayerDatabase::setCountry(const QString& country)
{
	m_currentPlayer.setCountry(country);
	if (!m_dirty)
		m_dirty = true;
}

QString PlayerDatabase::title() const
{
	return m_currentPlayer.title();
}
void PlayerDatabase::setTitle(const QString& title)
{
	m_currentPlayer.setTitle(title);
	if (!m_dirty)
		m_dirty = true;
}

int PlayerDatabase::firstEloListIndex()
{
	return m_currentPlayer.firstEloListIndex();
}
int PlayerDatabase::lastEloListIndex()
{
	return m_currentPlayer.lastEloListIndex();
}


int PlayerDatabase::elo(const PartialDate& date) const
{
	return m_currentPlayer.elo(eloList(date));
}
int PlayerDatabase::elo(const int eloList) const
{
	return m_currentPlayer.elo(eloList);
}

int PlayerDatabase::estimatedElo(const PartialDate& date)
{
	return m_currentPlayer.estimatedElo(eloList(date));
}
int PlayerDatabase::estimatedEloNoCache(const PartialDate& date) const
{
	return m_currentPlayer.estimatedEloNoCache(eloList(date));
}

int PlayerDatabase::estimatedElo() const
{
	return m_currentPlayer.estimatedElo();
}
int PlayerDatabase::highestElo() const
{
	return m_currentPlayer.peakElo();
}

void PlayerDatabase::setElo(const int year, const int listIndex, const int elo)
{
	m_currentPlayer.setElo(eloList(year, listIndex), elo);
	if (!m_dirty)
		m_dirty = true;
}


void PlayerDatabase::setEstimatedElo(const int elo)
{
	m_currentPlayer.setEstimatedElo(elo);
	if (!m_dirty)
		m_dirty = true;
}

bool PlayerDatabase::hasPhoto() const
{
	return !m_currentPlayer.photo().isNull();
}
QImage PlayerDatabase::photo() const
{
	return m_currentPlayer.photo();
}
void PlayerDatabase::setPhoto(const QImage& img)
{
	m_currentPlayer.setPhoto(img);
	if (!m_dirty)
		m_dirty = true;
}

bool PlayerDatabase::hasBiography() const
{
	return !m_currentPlayer.biography().isNull();
}
QString PlayerDatabase::biography() const
{
	return m_currentPlayer.biography();
}
void PlayerDatabase::setBiography(const QString& s)
{
	m_currentPlayer.setBiography(s);
	if (!m_dirty)
		m_dirty = true;
}
void PlayerDatabase::appendToBiography(const QString& s)
{
	m_currentPlayer.appendToBiography(s);
	if (!m_dirty)
		m_dirty = true;
}


QStringList PlayerDatabase::playerNames()
{
	QStringList result;
	QMap<QString, qint32>::Iterator it;
	for (it = m_mapping.begin(); it != m_mapping.end(); ++it) {
		result.push_back(it.key());
	}
	return result;
}

QStringList PlayerDatabase::findPlayers(const QString& prefix, const int maxCount, const Qt::CaseSensitivity cs)
{
	QStringList result;
	QMap<QString, qint32>::Iterator it;
	int i = 0;
	for (it = m_mapping.begin(); it != m_mapping.end(); ++it) {
		if (it.key().startsWith(prefix, cs)) {
			if (i >= maxCount)
				break;
			result.push_back(it.key());
            ++i;
		}
	}
	return result;
}


int PlayerDatabase::eloList(const PartialDate date) const
{
	const int year = date.year();
	if (year < 1971)
		return 0;
	if (year < 2001)//2 lists in the year
		return ((year -1971)*2) + 1 + (date.month() / 7);
	return 60 + ((year - 2001)*4) + 1 + (date.month() / 4);//4 lists in the year
}

int PlayerDatabase::eloList(const int year, const int index) const
{
	if (year < 1971)
		return 0;
	if (year < 2001)//2 lists in the year
		return ((year -1971)*2) + index;
	return 60 + ((year - 2001)*4) + index;//4 lists in the year
}

PartialDate PlayerDatabase::eloListToDate(const int index)
{
	if (index < 1)
        return PDInvalidDate;
	int year;
	int rem;
	int month;
	if (index < 61) {
		year = 1970 + ((index + 1) / 2);
		rem = index % 2;
		if (rem == 0)
			month = 7;
		else
			month = 1;
	} else {
		year = 2000 + ((index - 57) / 4);
		rem = index % 4;
		if (rem == 0)
			month = 10;
		else if (rem == 1)
			month = 1;
		else if (rem == 2)
			month = 4;
		else
			month = 7;
	}
	return PartialDate(year, month, 1);
}

