/***************************************************************************
                          outputoptions.h - description
                             -------------------
    begin                : 
    copyright            : (C) yyyy [name] <[email]>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __OUTPUTOPTIONS_H__
#define __OUTPUTOPTIONS_H__

#include <QString>
#include <QMap>
#include <QStringList>

class OutputOptions
{
   public : 
      enum OutputOptionType { Boolean, String, Integer, Color };
      OutputOptions();

      bool createOption (const QString& optionName, OutputOptionType optionType, const QString& allowValues, 
            const QString& defaultValue, const QString& description);
      bool createOption (const QString& optionName, const QString& optionType, const QString& allowValues, 
            const QString& defaultValue, const QString& description);
      bool createDefaultOptions ();

      /* Setting values */
      bool setOption(const QString& optionString);
      bool setOption(const QString& optionName, const QString& optionValue);
      bool setOption(const QString& optionName, bool optionValue);
      bool setOption(const QString& optionName, int optionValue);

      /* Retrieving values */
      int getOptionAsInt (const QString& optionName);
      QString getOptionAsString (const QString& optionName);
      bool getOptionAsBool (const QString& optionName);
      QString getOptionDescription (const QString& optionName);
      QStringList getOptionList ();
      QMap<QString,QString> getOptionListAndDescription ();

   private :

      QMap<QString,QString> m_list;
      QMap<QString,OutputOptionType> m_type;
      QMap<QString,QString> m_default;
      QMap<QString,QString> m_allow;
      QMap<QString,QString> m_description;

      bool validateValue(const QString& optionName, const QString& value);
      OutputOptionType optionString2Type(const QString& optionTypeStr);


};

#endif	// __OUTPUTOPTIONS_H__

