project(core)

file(GLOB SOURCES *.cc)
file(GLOB HEADERS *.h)

add_library(core SHARED ${SOURCES} ${HEADERS})

target_link_libraries(core lua util)
target_link_libraries(core ${LUA_LIBRARIES})

install(TARGETS core DESTINATION ${INSTALL_FOLDER})
