cmake_minimum_required(VERSION 2.6)
project(MENGDE)

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${MENGDE_SOURCE_DIR}/cmake")

# Set an output directory for our binaries
set(BIN_DIR ${MENGDE_SOURCE_DIR}/bin)

# Bump up warning levels appropriately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()

# Look up required packages and add the include directory to our include path
find_package(Boost 1.55.0 COMPONENTS filesystem)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Lua REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})

# Look in the game subdirectory to find its CMakeLists.txt so we can build the executable
add_subdirectory(game)
