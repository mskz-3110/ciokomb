include(${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}.cmake)

set(TESTS_ROOT_PATH ${PROJECT_ROOT_PATH}/tests)
include_directories(${TESTS_ROOT_PATH})
set(TEST_NAMES)
list(APPEND TEST_NAMES test_string)
list(APPEND TEST_NAMES test_arist)
list(APPEND TEST_NAMES test_pipe)
foreach(testName IN LISTS TEST_NAMES)
  add_executable(${testName} ${TESTS_ROOT_PATH}/${testName}.cpp)
  target_link_libraries(${testName} ${PROJECT_NAME}${STATIC_LIBRARY_SUFFIX})
endforeach()