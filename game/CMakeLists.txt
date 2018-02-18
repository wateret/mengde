project(game)

file(GLOB SOURCES src/*.cc
                  src/gui/*.cc
                  src/gui/app/*.cc
                  src/gui/foundation/*.cc
                  src/gui/uifw/*.cc
                  src/util/*.cc
                  src/core/*.cc
                  src/lua/*.cc)

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
