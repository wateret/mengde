project(game)

file(GLOB SOURCES src/*.cpp
                  src/view/*.cpp
                  src/view/app/*.cpp
                  src/view/foundation/*.cpp
                  src/view/uifw/*.cpp
                  src/util/*.cpp
                  src/core/*.cpp
                  src/lua/*.cpp)

file(GLOB HEADERS src/*.h
                  src/view/*.h
                  src/view/app/*.h
                  src/view/foundation/*.h
                  src/view/uifw/*.h
                  src/util/*.h
                  src/core/*.h
                  src/lua/*.h)

include_directories(src)

add_executable(game ${SOURCES} ${HEADERS})

target_link_libraries(game ${SDL2_LIBRARY})
target_link_libraries(game ${SDL2_TTF_LIBRARIES})
target_link_libraries(game ${LUA_LIBRARIES})
