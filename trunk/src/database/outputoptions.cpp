/***************************************************************************
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "outputoptions.h"
#include <QRegExp>

OutputOptions::OutputOptions()
{
	createDefaultOptions();
}
bool OutputOptions::createDefaultOptions()
{
	m_list.clear();
	m_type.clear();
	m_default.clear();
	m_allow.clear();
    createOption("ColumnStyle", Boolean, "", "false", tr("Notation in Column Style"));
    createOption("SymbolicNag", Boolean, "", "false", tr("Show symbolic Nags"));
    createOption("TextWidth", Integer, "0:200", "80", tr("Text width"));
    createOption("VariationIndentLevel", Integer, "1:200", "4", tr("Indent variations from Level"));
    createOption("VariationIndentSize", Integer, "1:10", "3", tr("Variation Indentation"));
    createOption("CommentIndent", String, "Always|OnlyMainline|Never", "OnlyMainline", tr("Comment Indenation"));

    createOption("MainLineMoveColor", Color, "", "black", tr("Main Line Color"));
    createOption("VariationColor", Color, "", "blue", tr("Variation Color"));
    createOption("CommentColor", Color, "", "green", tr("Comment Color"));
    createOption("NagColor", Color, "", "red", tr("Nag Color"));
    createOption("HeaderColor", Color, "", "blue", tr("Header Color"));
    createOption("ShowHeader", Boolean, "", "true", tr("Show Header"));

    createOption("ShowDiagram", Boolean, "", "false", tr("Show Diagrams"));
    createOption("DiagramSize", Integer, "0:500", "200", tr("Diagram Size"));
	return true;
}

bool OutputOptions::createOption(const QString& optionName, const QString& optionType, const QString& allowValues,
				 const QString& defaultValue, const QString& description)
{
	return createOption(optionName, optionString2Type(optionType), allowValues, defaultValue, description);

}

bool OutputOptions::createOption(const QString& optionName, OutputOptionType optionType, const QString& allowValues,
				 const QString& defaultValue, const QString& description)
{
	m_list[optionName] = defaultValue;
	m_type[optionName] = optionType;
	m_allow[optionName] = allowValues;
	m_default[optionName] = defaultValue;
	m_description[optionName] = description;

	if (!setOption(optionName, defaultValue)) {
		m_list.remove(optionName);
		m_type.remove(optionName);
		m_allow.remove(optionName);
		m_default.remove(optionName);
		m_description.remove(optionName);
		qWarning("Option '%s' could not be created", optionName.toLatin1().constData());
		return false;
	}
	return true;
}

bool OutputOptions::setOption(const QString& optionString)
{
	QStringList temp = optionString.split('=');
	return setOption(temp[0], temp[1]);
}

bool OutputOptions::setOption(const QString& optionName, const QString& optionValue)
{
	if (!validateValue(optionName, optionValue)) {
		qWarning("Option '%s' could not be set", optionName.toLatin1().constData());
		return false;
	}
	m_list[optionName] = optionValue;
	return true;
}

bool OutputOptions::setOption(const QString& optionName, bool optionValue)
{
	if (optionValue) {
		return setOption(optionName, QString("true"));
	} else {
		return setOption(optionName, QString("false"));
	}
}

bool OutputOptions::setOption(const QString& optionName, int optionValue)
{
	return setOption(optionName, QString::number(optionValue));
}

int OutputOptions::getOptionAsInt(const QString& optionName) const
{
    if (m_list.contains(optionName))
        return m_list.value(optionName).toInt();
    return 0;
}

QString OutputOptions::getOptionAsString(const QString& optionName) const
{
    if (m_list.contains(optionName))
        return m_list.value(optionName);
    return "";
}

bool OutputOptions::getOptionAsBool(const QString& optionName)
{
    return ((m_list[optionName] == "true") || (m_list[optionName] == "1"));
}

bool OutputOptions::validateValue(const QString& optionName, const QString& value)
{
	QStringList allowValues;
	QStringList allowColors;

	if (!m_list.contains(optionName)) {
		// Option does not exist
		return false;
	}

	allowValues = m_allow[optionName].split('|');
	switch (m_type[optionName]) {
	case Integer:
		/* If the allow string is empty, then any number is allowed */
		if (m_allow[optionName].isEmpty()) {
			return true;
		}
		/* See if value is in on of the ranges */
		for (QStringList::iterator it = allowValues.begin(); it != allowValues.end(); ++it) {
			if ((*it).indexOf(':') >= 0) {
				QStringList limits = it->split(':');
				int val = value.toInt();
				int ulimit = limits[1].toInt();
				int llimit = limits[0].toInt();
				if ((val >= llimit) && (val <= ulimit)) {
					return true;
				}
			}
		}
		if (allowValues.contains(value)) {
			return true;
		}
		return false;
	case String:
		if (m_allow[optionName].isEmpty()) {
			return true;
		}
		if (allowValues.contains(value)) {
			return true;
		}
		return false;
	case Boolean:
		if ((value == "1") || (value == "0") || (value == "true") || (value == "false")) {
			return true;
		} else {
			return false;
		}
	case Color:
		allowColors.append("red");
		allowColors.append("blue");
		allowColors.append("black");
		allowColors.append("white");
		allowColors.append("green");
		allowColors.append("purple");
		if ((allowColors.contains(value)) || (value.contains(QRegExp("^#[0-9a-fA-F]{6}")))) {
			return true;
		} else {
			return false;
		}
		break;
	default :
		/* Unknown type */
		return false;
	}

	/* Error because method should have returned by now. */
	return false;
}


QString OutputOptions::getOptionDescription(const QString& optionName)
{
	return m_description[optionName];
}

QStringList OutputOptions::getOptionList()
{
	return m_list.keys();
}

OutputOptions::OutputOptionType OutputOptions::optionString2Type(const QString& optionTypeStr)
{
	if (optionTypeStr == "String") {
		return String;
	} else if (optionTypeStr == "Integer") {
		return Integer;
	} else if (optionTypeStr == "Boolean") {
		return Boolean;
	} else if (optionTypeStr == "Color") {
		return Color;
	} else {
		return String;
	}
}
