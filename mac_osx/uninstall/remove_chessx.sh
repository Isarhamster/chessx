#!/bin/sh

osascript <<EOF
display dialog "This will remove ChessX.app from /Applications.\n\nYou may be asked for your administrator password." \
    buttons {"Cancel", "Remove"} default button "Remove" with icon caution

do shell script "rm -rf /Applications/ChessX.app" with administrator privileges

display dialog "ChessX.app removed successfully." buttons {"OK"} default button "OK"
EOF
