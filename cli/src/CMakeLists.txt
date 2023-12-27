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
set(PDIF_CLI_SOURCES
    main.cpp
)

set(EXE_NAME pdif_cli)

add_executable(${EXE_NAME} ${PDIF_CLI_SOURCES})

set_target_properties(${EXE_NAME} PROPERTIES OUTPUT_NAME "pdif")

set_target_properties(${EXE_NAME} PROPERTIES
    VERSION ${PDIF_CLI_VERSION}
    FRAMEWORK FALSE
)

target_include_directories(${EXE_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include   # add the binary tree to the search path so that we will find Version.h
)
target_link_libraries(${EXE_NAME} PUBLIC util pdif_engine)

install(TARGETS ${EXE_NAME}
  EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
  ARCHIVE DESTINATION lib COMPONENT libs   # static lib
  RUNTIME DESTINATION bin COMPONENT libs   # binaries
  LIBRARY DESTINATION lib COMPONENT libs   # shared lib
  FRAMEWORK DESTINATION bin COMPONENT libs # for mac
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION include             # headers
)