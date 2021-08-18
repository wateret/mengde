project(util)

file(GLOB_RECURSE SOURCES *.cc)
file(GLOB_RECURSE HEADERS *.h)

add_library(util SHARED ${SOURCES} ${HEADERS})

target_include_directories(util PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(util ${SDL2_LIBRARY})
target_link_libraries(util ${Boost_LIBRARIES})

install(TARGETS util DESTINATION install)
