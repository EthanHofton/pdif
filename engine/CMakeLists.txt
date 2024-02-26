
# Set the name of your project here
project("pdif_engine")

# Set the version number of your project here (format is MAJOR.MINOR.PATCHLEVEL - e.g. 1.0.0)
set(PDIF_ENGINE_VERSION_MAJOR "0")
set(PDIF_ENGINE_VERSION_MINOR "1")
set(PDIF_ENGINE_VERSION_PATCH "0")
set(PDIF_ENGINE_VERSION ${PDIF_ENGINE_VERSION_MAJOR}.${PDIF_ENGINE_VERSION_MINOR}.${PDIF_ENGINE_VERSION_PATCH})

add_subdirectory(include)
add_subdirectory(src)

if (PDIF_BUILD_ENGINE_TESTS)
    add_subdirectory(test)
endif() # PDIF_BUILD_ENGINE_TESTS