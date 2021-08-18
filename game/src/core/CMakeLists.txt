project(core)

file(GLOB SOURCES *.cc)
file(GLOB HEADERS *.h)

add_library(core SHARED ${SOURCES} ${HEADERS})

target_include_directories(core SYSTEM PUBLIC ${LUA_INCLUDE_DIR} ${FLATBUFFERS_INCLUDE_DIR})
target_link_libraries(core util)
target_link_libraries(core ${LUA_LIBRARIES})

install(TARGETS core DESTINATION install)
