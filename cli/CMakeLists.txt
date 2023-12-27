# Set the name of your project here
project("pdif_cli")

# Set the version number of your project here (format is MAJOR.MINOR.PATCHLEVEL - e.g. 1.0.0)
set(CLI_VERSION_MAJOR "0")
set(CLI_VERSION_MINOR "1")
set(CLI_VERSION_PATCH "0")
set(CLI_VERSION ${CLI_VERSION_MAJOR}.${CLI_VERSION_MINOR}.${CLI_VERSION_PATCH})

enable_testing()

# add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)