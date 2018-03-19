project(game)

set(CMAKE_MACOSX_RPATH 1)

file(GLOB SOURCES src/*.cc)

file(GLOB HEADERS src/*.h)

include_directories(src)

add_subdirectory(src/lua)
add_subdirectory(src/util)
add_subdirectory(src/core)
add_subdirectory(src/gui)

add_executable(game ${SOURCES} ${HEADERS})

# Project Libraries
target_link_libraries(game lua util core gui)
