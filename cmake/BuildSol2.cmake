# Configuration

set(SOL2_TAG v2.20.6)
set(SOL2_INSTALL_DIR "${CMAKE_BINARY_DIR}/include")
set(SOL2_REPO_URL "https://github.com/ThePhD/sol2/releases/download")

function(download_and_install FILENAME MD5)
    message("Installing '${FILENAME}' (${SOL2_TAG}) from '${SOL2_REPO_URL}'")
    file(DOWNLOAD "${SOL2_REPO_URL}/${SOL2_TAG}/${FILENAME}" "${SOL2_INSTALL_DIR}/${FILENAME}"
         EXPECTED_MD5 "${MD5}"
         SHOW_PROGRESS)
endfunction()

# Download header files

set(SOL2_HPP "sol.hpp")
set(SOL2_HPP_MD5 "8bae033629d61dea6c5060e205bbc89b")
download_and_install(${SOL2_HPP} ${SOL2_HPP_MD5})

set(SOL2_FORWARD_HPP "sol_forward.hpp")
set(SOL2_FORWARD_HPP_MD5 "00e4f2c0d46d70302c00eeb3b9bb37ee")
download_and_install(${SOL2_FORWARD_HPP} ${SOL2_FORWARD_HPP_MD5})

#[[
# Download Repo and generate the single header
include(ExternalProject)

# Needed for building single header for sol2
find_package(PythonInterp 3 REQUIRED)

set(VENDOR_PATH "${CMAKE_BINARY_DIR}/sol2/")

ExternalProject_add(
    sol2
    PREFIX ${VENDOR_PATH}

    GIT_REPOSITORY "https://github.com/ThePhD/sol2.git"
    GIT_TAG ${SOL2_TAG}

    # No CMake commands to run, so tell CMake not to configure
    CONFIGURE_COMMAND ""

    # Generate the single header and put it in ${SOL2_HPP}
    BINARY_DIR ${VENDOR_PATH}/src/sol2
    BUILD_COMMAND
            ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include
        COMMAND
            ${PYTHON_EXECUTABLE} ./single.py -o "${SOL2_HPP}"

    # No install or test command for the library
    INSTALL_COMMAND ""
    TEST_COMMAND "")

# Conditionally turn on SOL_CHECK_ARGUMENTS if using Debug mode
if (CMAKE_BUILD_TYPE MATCHES "[Dd]ebug")
    if (VERBOSE)
        message(STATUS "Turning on SOL_CHECK_ARGUMENTS in Debug mode.")
    endif()
    add_definitions(-DSOL_CHECK_ARGUMENTS)
endif()
]]

# Make sure sol2 is found as a system directory
include_directories(SYSTEM ${CMAKE_BINARY_DIR}/include)
