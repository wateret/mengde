cmake_minimum_required(VERSION 3.1)
project(MENGDE CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${MENGDE_SOURCE_DIR}/cmake")

include(CTest)
include(BoostTestHelpers)
include(BuildSol2)

# Set an output directory for our binaries
set(BIN_DIR ${MENGDE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Bump up warning levels appropriately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
else()
    message(FATAL_ERROR "Unsupported C++ compiler '${CMAKE_CXX_COMPILER_ID}'")
endif()

set(BUILD_NATIVE True)
if(DEFINED ROOTFS_ARM)
    set(BUILD_NATIVE False)
endif()

# Look up required packages and add the include directory to our include path
if(BUILD_NATIVE)
    enable_testing()

    find_package(Boost 1.55.0 COMPONENTS filesystem system REQUIRED)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(Lua REQUIRED)
else()
    if(DEFINED ROOTFS_ARM)
        include("cmake/armv7l_settings.cmake")
    endif()
endif()

include_directories(${Boost_INCLUDE_DIR})

# Look in the game subdirectory to find its CMakeLists.txt so we can build the executable
add_subdirectory(game)
