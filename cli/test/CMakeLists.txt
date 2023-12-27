# INCLUDE GTEST
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

# example test
# add_executable(example_tests example_tests.cpp)
# target_link_libraries(example_tests PRIVATE GTest::GTest testing_lib)
# add_test(NAME example_gtests COMMAND example_tests WORKING_DIRECTORY ${CMAKE_BINARY_DIR})