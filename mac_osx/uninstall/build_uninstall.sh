rm -r "Remove ChessX.app" 2>/dev/null
mkdir -p "Remove ChessX.app/Contents/MacOS"
mkdir -p "Remove ChessX.app/Contents/Resources"
cp remove_chessx.sh "Remove ChessX.app/Contents/MacOS/remove_chessx"
chmod +x "Remove ChessX.app/Contents/MacOS/remove_chessx"
cp Info.plist "Remove ChessX.app/Contents/Info.plist"
mkdir RemoveChessX.iconset

sips -z 16 16     RemoveChessX.png --out RemoveChessX.iconset/icon_16x16.png
sips -z 32 32     RemoveChessX.png --out RemoveChessX.iconset/icon_16x16@2x.png
sips -z 32 32     RemoveChessX.png --out RemoveChessX.iconset/icon_32x32.png
sips -z 64 64     RemoveChessX.png --out RemoveChessX.iconset/icon_32x32@2x.png
sips -z 128 128   RemoveChessX.png --out RemoveChessX.iconset/icon_128x128.png
sips -z 256 256   RemoveChessX.png --out RemoveChessX.iconset/icon_128x128@2x.png
sips -z 256 256   RemoveChessX.png --out RemoveChessX.iconset/icon_256x256.png
sips -z 512 512   RemoveChessX.png --out RemoveChessX.iconset/icon_256x256@2x.png
sips -z 512 512   RemoveChessX.png --out RemoveChessX.iconset/icon_512x512.png
sips -z 512 512   RemoveChessX.png --out RemoveChessX.iconset/icon_512x512@2x.png

iconutil -c icns RemoveChessX.iconset -o "Remove ChessX.app/Contents/Resources/RemoveChessX.icns"

cp RemoveChessX.icns "Remove ChessX.app/Contents/Resources/"
plutil -replace CFBundleIconFile -string RemoveChessX.icns "Remove ChessX.app/Contents/Info.plist"

touch "Remove ChessX.app"
