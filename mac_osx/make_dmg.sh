#!/bin/bash
set -e

APP_NAME="ChessX"
VOL_NAME="ChessX"
DMG_NAME="ChessX.dmg"

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$(pwd)"

APP_PATH="${BUILD_DIR}/ChessX.app"
UNINSTALL_APP="${SCRIPT_DIR}/uninstall/Remove ChessX.app"
COPYING_FILE="${SCRIPT_DIR}/../COPYING.md"
CHANGELOG_FILE="${SCRIPT_DIR}/../ChangeLog.md"
BACKGROUND_IMAGE="${SCRIPT_DIR}/background.png"

RW_DMG="${BUILD_DIR}/ChessX-temp.dmg"
FINAL_DMG="${BUILD_DIR}/${DMG_NAME}"

# ---------------- sanity ----------------

for f in "$APP_PATH" "$UNINSTALL_APP" "$COPYING_FILE" "$CHANGELOG_FILE" "$BACKGROUND_IMAGE"; do
  [ -e "$f" ] || { echo "Missing: $f"; exit 1; }
done

rm -f "$RW_DMG" "$FINAL_DMG"

# ---------------- size ----------------

APP_SIZE_MB=$(du -sm "$APP_PATH" | awk '{print $1}')
DMG_SIZE_MB=$((APP_SIZE_MB + 100))

# ---------------- create blank DMG ----------------

hdiutil create \
  -size "${DMG_SIZE_MB}m" \
  -fs HFS+ \
  -volname "$VOL_NAME" \
  "$RW_DMG"

# ---------------- mount ----------------

xattr -cr "$RW_DMG"
DEV=$(hdiutil attach "$RW_DMG" -readwrite -noverify -noautoopen | awk '/^\/dev\// {print $1}')
VOLUME="/Volumes/${VOL_NAME}"

# ---------------- populate ----------------

mkdir -p "$VOLUME/.background"

cp -R "$APP_PATH" "$VOLUME/"
cp -R "$UNINSTALL_APP" "$VOLUME/"
cp "$COPYING_FILE" "$VOLUME/"
cp "$CHANGELOG_FILE" "$VOLUME/"
cp "$BACKGROUND_IMAGE" "$VOLUME/.background/background.png"

ln -s /Applications "$VOLUME/Applications"

# ---------------- hide background ----------------

SetFile -a V "$VOLUME/.background"

# .fsevents may or may not exist yet — hide it if it does
if [ -d "$VOLUME/.fsevents" ]; then
  SetFile -a V "$VOLUME/.fseventsd"
fi

# .fsevents may or may not exist yet — hide it if it does
if [ -d "$VOLUME/.DS_Store" ]; then
  SetFile -a V "$VOLUME/.DS_Store"
fi

# ---------------- Finder layout ----------------

osascript <<EOF
tell application "Finder"
  tell disk "${VOL_NAME}"
    open
    set current view of container window to icon view
    set toolbar visible of container window to false
    set statusbar visible of container window to false
    set bounds of container window to {100, 100, 900, 688}

    set viewOptions to the icon view options of container window
    set arrangement of viewOptions to not arranged
    set icon size of viewOptions to 120
    set text size of viewOptions to 12
    set background picture of viewOptions to file ".background:background.png"

    set position of item "ChessX.app" to {216, 192}
    set position of item "Applications" to {564, 192}
    set position of item "Remove ChessX.app" to {710, 350}
    set position of item "COPYING.md" to {600, 20}
    set position of item "ChangeLog.md" to {700, 20}

    -- Move dev-only internals off-canvas
    if exists item ".background" then
      set position of item ".background" to {1000, 1000}
    end if
    if exists item ".fseventsd" then
      set position of item ".fseventsd" to {1000, 1200}
    end if

    close
    open
    update without registering applications
    delay 2
  end tell
end tell
EOF

# ---------------- bless & detach ----------------

bless --folder "$VOLUME"
hdiutil detach "$VOLUME"

# ---------------- compress ----------------

hdiutil convert "$RW_DMG" \
  -format UDZO \
  -imagekey zlib-level=9 \
  -o "$FINAL_DMG"

rm -f "$RW_DMG"
xattr -cr "$FINAL_DMG"

echo "DMG created successfully:"
echo "  $FINAL_DMG"
exit 0
