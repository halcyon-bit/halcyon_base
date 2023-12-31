﻿cmake_minimum_required(VERSION 3.10)

# 设定工程名
project(base_test)

set(MAIN_PATH ${PROJECT_SOURCE_DIR}/..)

# 存放静态库的文件夹位置
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MAIN_PATH}/lib/linux)

# 存放动态库的文件夹位置
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MAIN_PATH}/bin/linux)

# 存放可执行软件的目录
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MAIN_PATH}/bin/linux)

# 默认存放库文件的位置，如果产生的是静态库并且没有指定 CMAKE_ARCHIVE_OUTPUT_DIRECTORY 则存放在该目录下，动态库也类似
# set(LIBRARY_OUTPUT_PATH ${MAIN_PATH}/)

# 源文件路径
set(SRC_PATH ${MAIN_PATH}/test)

# depends 路径
set(DEPENDS_PATH ${MAIN_PATH}/../depends)

# set(CMAKE_VERBOSE_MAKEFILE on) # for message
# debug release 编译方式
if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
	# 文件生成路径
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MAIN_PATH}/bin/linux_debug)
	# base 模块 lib 路径
	set(BASE_LIB_PATH ${MAIN_PATH}/lib/linux_debug)
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MAIN_PATH}/bin/linux_release)
	set(BASE_LIB_PATH ${MAIN_PATH}/lib/linux_release)
else()
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MAIN_PATH}/bin/linux_release)
	set(BASE_LIB_PATH ${MAIN_PATH}/lib/linux_release)
    # message(${CMAKE_CXX_FLAGS_RELEASE})
endif()

# C++ 标准
if(CMAKE_BUILD_CXX_STANDARD AND (CMAKE_BUILD_CXX_STANDARD STREQUAL "c++11"))
	set(CMAKE_CXX_STANDARD 11)
elseif(CMAKE_BUILD_CXX_STANDARD AND (CMAKE_BUILD_CXX_STANDARD STREQUAL "c++14"))
	set(CMAKE_CXX_STANDARD 14)
else()
	set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${MAIN_PATH}/include)
include_directories(${DEPENDS_PATH}/thirdparty/googletest/1.11.0/include)

# LINK_DIRECTORIES(${MAIN_PATH}/lib/linux)
LINK_DIRECTORIES(${BASE_LIB_PATH})
LINK_DIRECTORIES(${DEPENDS_PATH}/thirdparty/googletest/1.11.0/lib/linux)

# any
add_executable(ut_any ${SRC_PATH}/ut_any.cpp)
target_link_libraries(ut_any gtest pthread)

# base64
add_executable(ut_base64 ${SRC_PATH}/ut_base64.cpp)
target_link_libraries(ut_base64 gtest halcyon_base pthread)

# bind
add_executable(ut_bind ${SRC_PATH}/ut_bind.cpp)
target_link_libraries(ut_bind gtest pthread)

# blocking_queue
add_executable(ut_blocking_queue ${SRC_PATH}/ut_blocking_queue.cpp)
target_link_libraries(ut_blocking_queue pthread)

# count_down_latch
add_executable(ut_count_down_latch ${SRC_PATH}/ut_count_down_latch.cpp)
target_link_libraries(ut_count_down_latch pthread)

# defer
add_executable(ut_defer ${SRC_PATH}/ut_defer.cpp)

# elapsed_timer
add_executable(ut_elapsed_timer ${SRC_PATH}/ut_elapsed_timer.cpp)

# md5
add_executable(ut_md5 ${SRC_PATH}/ut_md5.cpp)
target_link_libraries(ut_md5 gtest halcyon_base pthread)

# message_bus
add_executable(ut_message_bus ${SRC_PATH}/ut_message_bus.cpp)
target_link_libraries(ut_message_bus halcyon_base pthread)

# optional
add_executable(ut_optional ${SRC_PATH}/ut_optional.cpp)
target_link_libraries(ut_optional gtest pthread)

# range
add_executable(ut_range ${SRC_PATH}/ut_range.cpp)

# singleton
add_executable(ut_singleton ${SRC_PATH}/ut_singleton.cpp)
target_link_libraries(ut_singleton pthread)

# string_view
add_executable(ut_string_view ${SRC_PATH}/ut_string_view.cpp)
target_link_libraries(ut_string_view gtest pthread)

# thread
add_executable(ut_thread ${SRC_PATH}/ut_thread.cpp)
target_link_libraries(ut_thread gtest pthread)

# threadpool
add_executable(ut_threadpool ${SRC_PATH}/ut_threadpool.cpp)
target_link_libraries(ut_threadpool halcyon_base pthread)

# timestamp
add_executable(ut_timestamp ${SRC_PATH}/ut_timestamp.cpp)
target_link_libraries(ut_timestamp pthread)

# type
add_executable(ut_type ${SRC_PATH}/ut_type.cpp)

# utility
add_executable(ut_utility ${SRC_PATH}/ut_utility.cpp)
target_link_libraries(ut_utility gtest pthread)

# variant
add_executable(ut_variant ${SRC_PATH}/ut_variant.cpp)
target_link_libraries(ut_variant gtest pthread)

# version
add_executable(ut_version ${SRC_PATH}/ut_version.cpp)
target_link_libraries(ut_version halcyon_base)

# weak_callback
add_executable(ut_weak_callback ${SRC_PATH}/ut_weak_callback.cpp)
target_link_libraries(ut_weak_callback gtest pthread)
