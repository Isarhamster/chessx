set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES
    \.git/
    \.DS_Store
    \.cmake/
    \.github/
    \.bat$
    \.iss$
    data/engines/
    data/engines-mac/
    data/timeseal/
    dep/doctest/
    mac_osx/
    Packages/
    tests/
    tools/
    src/chessx\.rc
    src/resource\.h
)

set(CPACK_PACKAGE_FILE_NAME        "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}")

# Automatically escape CPACK_* variables
set(CPACK_VERBATIM_VARIABLES YES)
include(CPack)
