# INCLUDE UTIL
include(FetchContent)

FetchContent_Declare(
  util
  GIT_REPOSITORY https://github.com/EthanHofton/util.git
)

option(UTIL_BUILD_DOCS "Build documentation" OFF)
option(UTIL_BUILD_TESTS "Build tests" OFF)

FetchContent_MakeAvailable(util)


# set sources
set(PDIF_SOURCES
    test.cpp
)

set(LIBRARY_NAME pdif_engine)

add_library(${LIBRARY_NAME} ${PDIF_SOURCES})

# rename to just pdif
set_target_properties(${LIBRARY_NAME} PROPERTIES OUTPUT_NAME "pdif")

set_target_properties(${LIBRARY_NAME} PROPERTIES
    VERSION ${PDIF_ENGINE_VERSION}
    FRAMEWORK FALSE
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include   # add the binary tree to the search path so that we will find Version.h
)
target_link_libraries(${LIBRARY_NAME} PUBLIC util)

install(TARGETS ${LIBRARY_NAME}
  EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
  ARCHIVE DESTINATION lib COMPONENT libs   # static lib
  RUNTIME DESTINATION bin COMPONENT libs   # binaries
  LIBRARY DESTINATION lib COMPONENT libs   # shared lib
  FRAMEWORK DESTINATION bin COMPONENT libs # for mac
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION include             # headers
)