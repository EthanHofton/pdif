message(STATUS "Fetching EthanHofton/util.git")
# INCLUDE UTIL
include(FetchContent)

FetchContent_Declare(
  util
  GIT_REPOSITORY https://github.com/EthanHofton/util.git
)

option(UTIL_BUILD_DOCS "Build documentation" OFF)
option(UTIL_BUILD_TESTS "Build tests" OFF)

FetchContent_MakeAvailable(util)
message(STATUS "Fetched EthanHofton/util.git")

message(STATUS "Fetching nlohmann/json.git")
# INCLUDE NLOHMANN JSON
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
)
FetchContent_MakeAvailable(nlohmann_json)
message(STATUS "Fetched nlohmann/json.git")

message(STATUS "Finding qpdf")

find_package(qpdf REQUIRED)

message(STATUS "Found qpdf: ${qpdf_LIBRARIES}")

# set sources
set(PDIF_SOURCES
    lcs_stream_differ.cpp
    pdf.cpp
    stream_meta.cpp
    stream_differ_base.cpp
    meta_edit_op.cpp
    edit_op.cpp
    stream_elem.cpp
    stream.cpp
    logger.cpp
    diff.cpp
    content_extractor.cpp
)

set(LIBRARY_NAME pdif_engine)

add_library(${LIBRARY_NAME} ${PDIF_SOURCES})

# rename to just pdif
set_target_properties(${LIBRARY_NAME} PROPERTIES OUTPUT_NAME "pdif")
target_compile_options(${LIBRARY_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)

set_target_properties(${LIBRARY_NAME} PROPERTIES
    VERSION ${PDIF_ENGINE_VERSION}
    FRAMEWORK FALSE
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include   # add the binary tree to the search path so that we will find Version.h
    ${nlohmann_json_SOURCE_DIR}/include
)
target_link_libraries(${LIBRARY_NAME} PUBLIC
    util
    qpdf::libqpdf
)

install(TARGETS ${LIBRARY_NAME}
  EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
  ARCHIVE DESTINATION lib COMPONENT libs   # static lib
  RUNTIME DESTINATION bin COMPONENT libs   # binaries
  LIBRARY DESTINATION lib COMPONENT libs   # shared lib
  FRAMEWORK DESTINATION bin COMPONENT libs # for mac
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION include             # headers
)