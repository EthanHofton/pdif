
# Set the name of your project here
project("pdif_engine")

# Set the version number of your project here (format is MAJOR.MINOR.PATCHLEVEL - e.g. 1.0.0)
set(ENGINE_VERSION_MAJOR "0")
set(ENGINE_VERSION_MINOR "1")
set(ENGINE_VERSION_PATCH "0")
set(ENGINE_VERSION ${ENGINE_VERSION_MAJOR}.${ENGINE_VERSION_MINOR}.${ENGINE_VERSION_PATCH})

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)