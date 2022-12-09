set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES
    \.git/
    \.gitignore
    \.gitmodules
    \.DS_Store
    \.cmake/
    \.github/
    \.bat$
    \.iss$
    \.user$
    build
    data/engines/
    data/engines-mac/
    data/timeseal/
    lib/chessx-board/
    lib/chessx-eco/
    lib/chessx-pgn/
    lib/pgn-convert/
    mac_osx/
    Packages/
    tests/
    tools/
    src/chessx\.rc
    src/resource\.h
    vcpkg
)

set(CPACK_PACKAGE_FILE_NAME        "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}")
set(CPACK_GENERATOR "DragNDrop")

# Automatically escape CPACK_* variables
set(CPACK_VERBATIM_VARIABLES YES)
include(CPack)
