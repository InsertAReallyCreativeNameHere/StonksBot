cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0079 NEW)

project(StonksBot)

include(cmake/DetectArchitectures.cmake)

if ("amd64" IN_LIST CMAKE_CPU_ARCHITECTURES)
    set(BUILD_ARCH x64)
elseif ("x86" IN_LIST CMAKE_CPU_ARCHITECTURES)
    set(BUILD_ARCH x86)
endif()

if ((NOT DEFINED CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL ""))
    set(CMAKE_BUILD_TYPE "Release")
endif()
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG=1)
endif()

if (WIN32 OR WIN64)
    set(BUILD_PLATFORM Win32)
elseif (UNIX AND NOT APPLE)
    set(BUILD_PLATFORM Linux)
elseif (APPLE)
    set(BUILD_PLATFORM MacOS)
else()
    set(BUILD_PLATFORM "UnknownPlatform")
endif()

set(CMAKE_CXX_STANDARD 20)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_SHARED_LIBRARY_SUFFIX STREQUAL ".dll")
        set(COMPILER "MinGW")
    else()
        set(COMPILER "gcc")
    endif()
    set(BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE INTERNAL "Configuration string in output dir." FORCE)
    set(OUTNAME "${BUILD_ARCH}-${COMPILER}-${BUILD_TYPE}-${BUILD_PLATFORM}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(COMPILER "Clang")
    set(BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE INTERNAL "Configuration string in output dir." FORCE)
    set(OUTNAME "${BUILD_ARCH}-${COMPILER}-${BUILD_TYPE}-${BUILD_PLATFORM}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER "MSVC")
    set(BUILD_TYPE "UnknownBuildType")
    set(OUTNAME "${BUILD_ARCH}-${COMPILER}-${BUILD_PLATFORM}")
else()
    set(COMPILER ${CMAKE_CXX_COMPILER_ID})
    set(BUILD_TYPE "UnknownBuildType")
    set(OUTNAME "${BUILD_ARCH}-Unknown-${BUILD_PLATFORM}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

# sleepy-discord
set(OPENSSL_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/dependencies/${BUILD_ARCH}-OpenSSL")
add_subdirectory(external/sleepy-discord)
target_compile_definitions(sleepy-discord PUBLIC _WEBSOCKETPP_CPP11_THREAD_ _SILENCE_CLANG_COROUTINE_MESSAGE)

# StonksBot

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../lib/${BUILD_ARCH}-${COMPILER}-${CMAKE_BUILD_TYPE}-${BUILD_PLATFORM}/StonksBot)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../lib/${BUILD_ARCH}-${COMPILER}-${CMAKE_BUILD_TYPE}-${BUILD_PLATFORM}/StonksBot)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin/${BUILD_ARCH}-${COMPILER}-${CMAKE_BUILD_TYPE}-${BUILD_PLATFORM}/StonksBot)

file(GLOB_RECURSE STONK configure_depends
    "${CMAKE_CURRENT_LIST_DIR}/src/*.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp"
)

add_executable(StonksBot ${STONK})

target_link_libraries(StonksBot PUBLIC
    sleepy-discord
)
target_include_directories(StonksBot PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/src"
    "${CMAKE_CURRENT_LIST_DIR}/external/sleepy-discord/include"
)