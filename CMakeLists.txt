cmake_minimum_required(VERSION 3.21)
project(BaseChess VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(COMPILER_FLAGS
        -Wall
        -Wpedantic
    )
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    
    option(WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
    if(WARNINGS_AS_ERRORS)
        list(APPEND COMPILER_FLAGS -Werror)
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(COMPILER_FLAGS /Wall)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
endif()

add_compile_options(${COMPILER_FLAGS})

# Add include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/BaseChess
)

include(BaseChessConfig.cmake)

option(BUILD_EXAMPLES "Build example projects" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()