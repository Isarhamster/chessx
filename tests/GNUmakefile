#############################################################################
# GNUmakefile to add the test tartget to the generated Makefile from qmake
#Testapplication: 	testrunner.app/Contents/MacOS/testrunner
#Date: 			16/01/2007
#Copyrigh: 		(C) 2007 Heinz R. Hopfgartner 
#                           <heinz.hopfgartner@gmx.at>
#############################################################################
#***************************************************************************
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# ***************************************************************************/

# 1.  Create the Makefile with qmake testrunner.pro
# 2.  Run the target make tests

include Makefile


###### Run Tests:

tests:
	-rm ./testlog.log
	-./$(TARGET) >> ./testlog.log
	-more ./testlog.log

testsv:
	-rm ./testlog.log
	-./$(TARGET) -v2 >> ./testlog.log
	-more ./testlog.log