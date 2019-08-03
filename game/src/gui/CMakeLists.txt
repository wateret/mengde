project(gui)

file(GLOB SOURCES app/*.cc
                  foundation/*.cc
                  uifw/*.cc)
file(GLOB HEADERS app/*.h
                  foundation/*.h
                  uifw/*.h)

add_library(gui SHARED ${SOURCES} ${HEADERS})

target_include_directories(gui PUBLIC ${SDL2_INCLUDE_DIR})
target_include_directories(gui PUBLIC ${SDL2_TTF_INCLUDE_DIR})
target_link_libraries(gui core util)
target_link_libraries(gui ${SDL2_LIBRARY})
target_link_libraries(gui ${SDL2_TTF_LIBRARIES})
target_link_libraries(gui ${Boost_LIBRARIES})

install(TARGETS gui DESTINATION lib)
