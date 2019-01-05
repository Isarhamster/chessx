#!/bin/sh
QTDIR=$1
BIN_FILE="./$2.app/Contents/MacOS/$2"
echo "LOG: Start fix paths for ${BIN_FILE} in ${QTDIR}"

for P in `otool -L $BIN_FILE` 
do 
    if [[ "$P" == *//* ]] 
    then 
        PSLASH=$(echo $P | sed 's,//,/,g')
        install_name_tool -change $P $PSLASH $BIN_FILE
        echo "LOG: install_name_tool -change $P $PSLASH $BIN_FILE"
    fi 
done  

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
