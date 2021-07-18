cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0079 NEW)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

if (WIN32)
    set(BUILD_PLATFORM Win32)
elseif (UNIX AND NOT APPLE)
    set(BUILD_PLATFORM Linux)
else()
    set(BUILD_PLATFORM MacOS)
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(BUILD_ARCH x64)
else()
    set(BUILD_ARCH x86)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(StonksBot)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fconcepts -Wall -Wextra -Wno-unknown-pragmas -Wno-sign-compare -Wno-unused-but-set-variable")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif()
    if (CMAKE_SHARED_LIBRARY_SUFFIX STREQUAL ".dll")
        set(COMPILER "MinGW")
    else()
        set(COMPILER "gcc")
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=lld -std=c++20 -pthread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-unknown-pragmas -Wno-sign-compare -Wno-reserved-id-macro")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-comma -Wno-undef -Wno-source-uses-openmp")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors -Wno-exit-time-destructors -Wno-float-equal -Wno-shadow-field-in-constructor")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-newline-eof -Wno-implicit-fallthrough -Wno-padded -Wno-sign-conversion")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-old-style-cast -Wno-mismatched-tags -Wno-shadow -Wno-double-promotion -Wno-reorder-ctor")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-weak-vtables -Wno-unused-template -Wno-zero-as-null-pointer-constant -Wno-cast-qual")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-disabled-macro-expansion -Wno-cast-align -Wno-implicit-int-conversion -Wno-overloaded-virtual")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-extra-semi-stmt -Wno-implicit-int-float-conversion -Wno-used-but-marked-unused")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-suggest-destructor-override -Wno-shadow-field -Wno-shorten-64-to-32 -Wno-unused-variable")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif()
    set(COMPILER "Clang")
else()
    set(BUILD_ARCH "x64")
    set(COMPILER "Unknown")
endif()

# OpenSSL

file (REMOVE ${CMAKE_CURRENT_LIST_DIR}/external/openssl-cmake/build/CMakeCache.txt)

message(STATUS "Configuring OpenSSL.")
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/external/openssl-cmake/build)
execute_process(COMMAND ${CMAKE_COMMAND} .. -G${CMAKE_GENERATOR} WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/external/openssl-cmake/build)
message(STATUS "Configured OpenSSL.")

message(STATUS "Building OpenSSL.")
include(ProcessorCount)
ProcessorCount(Stonk_ProcCount)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/external/openssl-cmake/build --config ${CMAKE_BUILD_TYPE} --target all -j ${Stonk_ProcCount} WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/external/openssl-cmake/build)
message(STATUS "Built OpenSSL.")

file(
    COPY ${CMAKE_CURRENT_LIST_DIR}/external/openssl-cmake/build/ssl/${CMAKE_STATIC_LIBRARY_PREFIX}ssl${CMAKE_STATIC_LIBRARY_SUFFIX}
    DESTINATION ${CMAKE_CURRENT_LIST_DIR}/external/openssl-cmake/build
)
file(
    COPY ${CMAKE_CURRENT_LIST_DIR}/external/openssl-cmake/build/crypto/${CMAKE_STATIC_LIBRARY_PREFIX}crypto${CMAKE_STATIC_LIBRARY_SUFFIX}
    DESTINATION ${CMAKE_CURRENT_LIST_DIR}/external/openssl-cmake/build
)

# sleepy-discord
set(OPENSSL_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/external/openssl-cmake/build")
set(OPENSSL_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/external/openssl-cmake/build/include")
add_subdirectory(external/sleepy-discord)
target_compile_definitions(sleepy-discord PUBLIC _WEBSOCKETPP_CPP11_THREAD_ _SILENCE_CLANG_COROUTINE_MESSAGE)
target_link_libraries(sleepy-discord PUBLIC
    "${CMAKE_STATIC_LIBRARY_PREFIX}ssl${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${CMAKE_STATIC_LIBRARY_PREFIX}crypto${CMAKE_STATIC_LIBRARY_SUFFIX}"
    ws2_32
)

# StonksBot

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../lib/${BUILD_ARCH}-${COMPILER}-${CMAKE_BUILD_TYPE}-${BUILD_PLATFORM}/StonksBot)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../lib/${BUILD_ARCH}-${COMPILER}-${CMAKE_BUILD_TYPE}-${BUILD_PLATFORM}/StonksBot)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin/${BUILD_ARCH}-${COMPILER}-${CMAKE_BUILD_TYPE}-${BUILD_PLATFORM}/StonksBot)

file(GLOB_RECURSE STONK configure_depends
    "${CMAKE_CURRENT_LIST_DIR}/src/*.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp"
)

add_executable(StonksBot ${STONK})

set_target_properties(StonksBot PROPERTIES LINK_SEARCH_START_STATIC 1)
set_target_properties(StonksBot PROPERTIES LINK_SEARCH_END_STATIC 1)

target_link_libraries(StonksBot PUBLIC
    sleepy-discord
)
target_include_directories(StonksBot PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/src"
    "${CMAKE_CURRENT_LIST_DIR}/external/sleepy-discord/include"
)