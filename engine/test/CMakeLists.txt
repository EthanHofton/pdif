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

add_executable(test_stream_elem test_stream_elem.cpp)
target_link_libraries(test_stream_elem PRIVATE GTest::GTest pdif_engine)
add_test(NAME gtest_stream_elem COMMAND test_stream_elem WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(test_stream test_stream.cpp)
target_link_libraries(test_stream PRIVATE GTest::GTest pdif_engine)
add_test(NAME gtest_stream COMMAND test_stream WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(test_edit_op test_edit_op.cpp)
target_link_libraries(test_edit_op PRIVATE GTest::GTest pdif_engine)
add_test(NAME gtest_edit_op COMMAND test_edit_op WORKING_DIRECTORY ${CMAKE_BINARY_DIR})