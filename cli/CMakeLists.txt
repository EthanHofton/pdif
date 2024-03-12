# Set the name of your project here
project("pdif_cli")

# Set the version number of your project here (format is MAJOR.MINOR.PATCHLEVEL - e.g. 1.0.0)
set(PDIF_CLI_VERSION_MAJOR "1")
set(PDIF_CLI_VERSION_MINOR "0")
set(PDIF_CLI_VERSION_PATCH "0")
set(PDIF_CLI_VERSION ${PDIF_CLI_VERSION_MAJOR}.${PDIF_CLI_VERSION_MINOR}.${PDIF_CLI_VERSION_PATCH})

add_subdirectory(include)
add_subdirectory(src)

if (PDIF_BUILD_CLI_TESTS)
    add_subdirectory(test)
endif() # PDIF_BUILD_CLI_TESTS