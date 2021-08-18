project(game)

set(CMAKE_MACOSX_RPATH 1)

set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "./;../lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

file(GLOB SOURCES src/main.cc)

include_directories(src)

#add_subdirectory(src/luab)
add_subdirectory(src/util)
add_subdirectory(src/core)
add_subdirectory(src/gui)

add_executable(game ${SOURCES})

# Project Libraries
target_link_libraries(game util core gui)

install(TARGETS game DESTINATION install)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/sce DESTINATION install)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()
