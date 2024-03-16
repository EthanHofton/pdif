
# Set the name of your project here
project("pdif_engine")

# Set the version number of your project here (format is MAJOR.MINOR.PATCHLEVEL - e.g. 1.0.0)
set(PDIF_ENGINE_VERSION_MAJOR "1")
set(PDIF_ENGINE_VERSION_MINOR "0")
set(PDIF_ENGINE_VERSION_PATCH "1")
set(PDIF_ENGINE_VERSION ${PDIF_ENGINE_VERSION_MAJOR}.${PDIF_ENGINE_VERSION_MINOR}.${PDIF_ENGINE_VERSION_PATCH})

add_subdirectory(include)
add_subdirectory(src)

if (PDIF_BUILD_ENGINE_TESTS)
    add_subdirectory(test)
endif() # PDIF_BUILD_ENGINE_TESTS

if (PDIF_BUILD_DOCS)
    add_subdirectory(docs)
endif() # PDIF_BUILD_DOCS