
# Set the name of your project here
project("pdif_engine")

# Set the version number of your project here (format is MAJOR.MINOR.PATCHLEVEL - e.g. 1.0.0)
set(PDIF_ENGINE_VERSION_MAJOR "1")
set(PDIF_ENGINE_VERSION_MINOR "0")
set(PDIF_ENGINE_VERSION_PATCH "4")
set(PDIF_ENGINE_VERSION ${PDIF_ENGINE_VERSION_MAJOR}.${PDIF_ENGINE_VERSION_MINOR}.${PDIF_ENGINE_VERSION_PATCH})

set(PDIF_RES_PATH "${CMAKE_BINARY_DIR}/res")

add_custom_target(pdif_copy_res ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res ${PDIF_RES_PATH}
    COMMENT "Copying res to build directory"
)

add_subdirectory(include)
add_subdirectory(src)

if (PDIF_BUILD_ENGINE_TESTS)
    add_subdirectory(test)
endif() # PDIF_BUILD_ENGINE_TESTS

if (PDIF_BUILD_DOCS)
    add_subdirectory(docs)
endif() # PDIF_BUILD_DOCS