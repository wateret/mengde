set(FLATBUFFERS_REPO_URL "https://github.com/google/flatbuffers.git")
set(FLATBUFFERS_TAG "v1.10.0")

include(ExternalProject)

set(VENDOR_PATH "${CMAKE_BINARY_DIR}")

ExternalProject_add(
    flatbuffers
    PREFIX ${VENDOR_PATH}

    GIT_REPOSITORY ${FLATBUFFERS_REPO_URL}
    GIT_TAG ${FLATBUFFERS_TAG}

    CONFIGURE_COMMAND ""

    BINARY_DIR "${VENDOR_PATH}/flatbuffers"
    BUILD_COMMAND ""

    # No install or test command for the library
    INSTALL_COMMAND ""
    TEST_COMMAND "")

set(FLATBUFFERS_INCLUDE_DIR "${VENDOR_PATH}/src/flatbuffers/include")
