/***************************************************************************
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#define VERSION_MAJOR   0
#define VERSION_MINOR   9
#define REVISION        0

#define XGLUE2(a,b) #a "," #b
#define GLUE2(a,b)  XGLUE2(a,b)

#define XGLUE4(a,b,c,d) #a "," #b "," #c "," #d
#define GLUE4(a,b,c,d)  XGLUE4(a,b,c,d)

#define STR_VERSION     GLUE2(VERSION_MAJOR,VERSION_MINOR)
#define STR_REVISION    GLUE4(VERSION_MAJOR,VERSION_MINOR,0,REVISION)

#define COPYRIGHT "Copyright (C) 2012"
