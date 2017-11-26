project(game)

file(GLOB SOURCES src/*.cpp
                  src/controller/*.cpp
                  src/view/*.cpp
                  src/view/ui/*.cpp
                  src/util/*.cpp
                  src/core/*.cpp
                  src/lua/*.cpp)

file(GLOB HEADERS src/*.h
                  src/controller/*.h
                  src/view/*.h
                  src/view/ui/*.h
                  src/util/*.h
                  src/core/*.h
                  src/lua/*.h)

include_directories(src
                    src/controller
                    src/view
                    src/util
                    src/core
					src/lua)

add_executable(game ${SOURCES} ${HEADERS})

target_link_libraries(game ${SDL2_LIBRARY})
target_link_libraries(game ${SDL2_TTF_LIBRARIES})
target_link_libraries(game ${LUA_LIBRARIES})
