project(lua)

file(GLOB SOURCES *.cc)
file(GLOB HEADERS *.h)

add_library(lua SHARED ${SOURCES} ${HEADERS})

target_link_libraries(lua ${LUA_LIBRARIES})

install(TARGETS lua DESTINATION ${INSTALL_FOLDER})
