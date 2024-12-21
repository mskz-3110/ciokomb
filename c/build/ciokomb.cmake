set(SRC_ROOT_PATH ${PROJECT_ROOT_PATH}/src)
set(SRCS)
list(APPEND SRCS ${SRC_ROOT_PATH}/ciokomb.c)
list(APPEND SRCS ${SRC_ROOT_PATH}/ciokomb/string.c)
list(APPEND SRCS ${SRC_ROOT_PATH}/ciokomb/arist.c)

set(INCS)
list(APPEND INCS ${PROJECT_ROOT_PATH}/inc)
include_directories(${INCS})

add_library(${PROJECT_NAME}-Shared SHARED ${SRCS})
set_target_properties(${PROJECT_NAME}-Shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
add_library(${PROJECT_NAME}-Static STATIC ${SRCS})
set_target_properties(${PROJECT_NAME}-Static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})