project(game)

file(GLOB SOURCES src/*.cpp
                  src/gui/*.cpp
                  src/gui/app/*.cpp
                  src/gui/foundation/*.cpp
                  src/gui/uifw/*.cpp
                  src/util/*.cpp
                  src/core/*.cpp
                  src/lua/*.cpp)

file(GLOB HEADERS src/*.h
                  src/gui/*.h
                  src/gui/app/*.h
                  src/gui/foundation/*.h
                  src/gui/uifw/*.h
                  src/util/*.h
                  src/core/*.h
                  src/lua/*.h)

include_directories(src)

add_executable(game ${SOURCES} ${HEADERS})

target_link_libraries(game ${SDL2_LIBRARY})
target_link_libraries(game ${SDL2_TTF_LIBRARIES})
target_link_libraries(game ${LUA_LIBRARIES})
target_link_libraries(game ${Boost_LIBRARIES})
