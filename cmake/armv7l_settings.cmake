# Notice: this file is used in cross building for ARMv7l
# find_package() will be looking for host machine 
# and it will not be correct for cross building for ARM target.
# As RootFS will be prepared with required packages,
# we assume they are installed as described in below folders.

if(NOT DEFINED ROOTFS_ARM)
    message(FATAL_ERROR "ROOTFS_ARM is required for cross building")
endif()
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message(FATAL_ERROR "ARM cross build is currently supported only in Linux")
endif()
if(NOT EXISTS "${ROOTFS_ARM}/lib/arm-linux-gnueabihf")
    message(FATAL_ERROR "ROOTFS_ARM path doesn't seem to be a correct ARM rootfs")
endif()

set(Boost_VERSION 105500)
set(Boost_FOUND True)
set(BOOST_INCLUDEDIR ${ROOTFS_ARM}/usr/include/boost)
set(BOOST_LIBRARYDIR ${ROOTFS_ARM}/usr/lib/arm-linux-gnueabihf)
set(Boost_INCLUDE_DIRS ${ROOTFS_ARM}/usr/include/boost)
set(SDL2_INCLUDE_DIR ${ROOTFS_ARM}/usr/include/SDL2)
set(SDL2_TTF_INCLUDE_DIR ${ROOTFS_ARM}/usr/include/SDL2)
set(LUA_INCLUDE_DIR ${ROOTFS_ARM}/usr/include/lua5.2)

set(SDL2_LIBRARY SDL2main SDL2 pthread)
set(SDL2_TTF_LIBRARIES SDL2_ttf)
set(Boost_LIBRARIES boost_filesystem boost_system)
set(LUA_LIBRARIES lua5.2 m)

# include path for system
include_directories(${ROOTFS_ARM}/usr/include/arm-linux-gnueabihf)

# library path for system
link_directories(${ROOTFS_ARM}/usr/lib/arm-linux-gnueabihf
                 ${BOOST_LIBRARYDIR})
