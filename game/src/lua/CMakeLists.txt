project(lua)

file(GLOB SOURCES *.cc)
file(GLOB HEADERS *.h)

include_directories(.)

add_library(lua SHARED ${SOURCES} ${HEADERS})

target_link_libraries(lua ${LUA_LIBRARIES})
