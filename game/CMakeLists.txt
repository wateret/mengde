project(game)

set(CMAKE_MACOSX_RPATH 1)

file(GLOB SOURCES src/*.cc
                  src/gui/*.cc
                  src/gui/app/*.cc
                  src/gui/foundation/*.cc
                  src/gui/uifw/*.cc
                  src/util/*.cc
                  src/core/*.cc)

file(GLOB HEADERS src/*.h
                  src/gui/*.h
                  src/gui/app/*.h
                  src/gui/foundation/*.h
                  src/gui/uifw/*.h
                  src/util/*.h
                  src/core/*.h)

include_directories(src)

add_subdirectory(src/lua)

add_executable(game ${SOURCES} ${HEADERS})

# Project Libraries
target_link_libraries(game lua)

# External Libraries
target_link_libraries(game ${SDL2_LIBRARY})
target_link_libraries(game ${SDL2_TTF_LIBRARIES})
target_link_libraries(game ${Boost_LIBRARIES})
