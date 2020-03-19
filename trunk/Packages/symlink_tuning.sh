#!/bin/sh
if [ -f $DSTROOT/chessx.app/Contents/MacOS/data/engines-mac/uci/ ]; then
    sudo ln -s /library/caches/leelaz_opencl_tuning $DSTROOT/chessx.app/Contents/MacOS/data/engines-mac/uci/leelaz_opencl_tuning
fi
exit 0
