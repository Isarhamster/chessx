#!/bin/sh
echo 'LOG: Start fix paths'
BIN_FILE="./chessx.app/Contents/MacOS/chessx"
for P in `otool -L $BIN_FILE` 
do 
    if [[ "$P" == *//* ]] 
    then 
        PSLASH=$(echo $P | sed 's,//,/,g')
        install_name_tool -change $P $PSLASH $BIN_FILE
        echo "LOG: install_name_tool -change $P $PSLASH $BIN_FILE"
    fi 
done 
 
QTDIR="/Users/ni/Qt-5-6-0/5.6/clang_64/"
for F in `find $QTDIR/lib $QTDIR/plugins $QTDIR/qml  -perm 755 -type f` 
do 
    for P in `otool -L $F`
	do   
	    if [[ "$P" == *//* ]] 
	    then 
	        PSLASH=$(echo $P | sed 's,//,/,g')
	        install_name_tool -change $P $PSLASH $F
                echo "LOG: install_name_tool -change $P $PSLASH $F"
	    fi 
	 done
done
echo 'LOG: Done fix paths'
