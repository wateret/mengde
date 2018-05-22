project(game)

set(CMAKE_MACOSX_RPATH 1)

set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "./")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
if(NOT INSTALL_FOLDER)
  set(INSTALL_FOLDER, "mengde")
endif()

file(GLOB SOURCES src/main.cc)

include_directories(src)

add_subdirectory(src/lua)
add_subdirectory(src/util)
add_subdirectory(src/core)
add_subdirectory(src/gui)

add_executable(game ${SOURCES})

# Project Libraries
target_link_libraries(game lua util core gui)

install(TARGETS game DESTINATION ${INSTALL_FOLDER})
