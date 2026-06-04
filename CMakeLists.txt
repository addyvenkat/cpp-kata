cmake_minimum_required(VERSION 3.24)
project(repo C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-O2 -g -fno-omit-frame-pointer -Wall -Wextra -Werror -Wconversion -Wshadow)

# Sanitizers via -DSAN=ASAN/UBSAN/TSAN
if(DEFINED SAN)
  if(SAN STREQUAL "ASAN")
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
  elseif(SAN STREQUAL "UBSAN")
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
  elseif(SAN STREQUAL "TSAN")
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
  endif()
endif()

include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.14.0)
FetchContent_MakeAvailable(googletest)
enable_testing()

# MoveOnlyBuffer build
add_library(moveOnlyBuffer INTERFACE)
target_include_directories(moveOnlyBuffer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/moveOnlyBuffer/inc)
target_compile_definitions(moveOnlyBuffer INTERFACE USE_UNIQUE_PTR) # Comment this for new/delete implementation
add_executable(testMoveOnlyBuffer ${CMAKE_CURRENT_SOURCE_DIR}/moveOnlyBuffer/test/testMoveOnlyBuffer.cpp)
target_link_libraries(testMoveOnlyBuffer PRIVATE moveOnlyBuffer gtest gtest_main)
add_test(NAME moveOnlyBuffer COMMAND testMoveOnlyBuffer)

# RingBuffer build
add_library(RingBuffer ${CMAKE_CURRENT_SOURCE_DIR}/ringBuffer/src/RingBuffer.c)
target_include_directories(RingBuffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ringBuffer/inc)
add_executable(testRingBuffer ${CMAKE_CURRENT_SOURCE_DIR}/ringBuffer/src/testRingBuffer.cpp)
target_link_libraries(testRingBuffer PRIVATE RingBuffer gtest_main)

# AlignedMalloc build
add_library(alignedMalloc ${CMAKE_CURRENT_SOURCE_DIR}/alignedMalloc/src/alignedMalloc.c)
target_include_directories(alignedMalloc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/alignedMalloc/inc)
add_executable(testAlignedMalloc ${CMAKE_CURRENT_SOURCE_DIR}/alignedMalloc/src/testAlignedMalloc.cpp)
target_link_libraries(testAlignedMalloc PRIVATE alignedMalloc gtest_main)
