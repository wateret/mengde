project(util)

file(GLOB SOURCES *.cc)
file(GLOB HEADERS *.h)

add_library(util SHARED ${SOURCES} ${HEADERS})

target_link_libraries(util ${SDL2_LIBRARY})
target_link_libraries(util ${Boost_LIBRARIES})

install(TARGETS util DESTINATION ${INSTALL_FOLDER})
