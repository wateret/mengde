find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# add_executable_boost_test(<source> [SRCS <source> ..] [DEPS <library> ...])
#
# Add a target executable with test source files and add a CTest test.

function(add_executable_boost_test NAME)

    # Parse arguments

    set(OPTIONS "")
    set(ONE_VALUE_ARGS "")
    set(MULTI_VALUE_ARGS SRCS DEPS)
    cmake_parse_arguments(ARGPARSE "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" "${ARGN}")

    set(SRCS "${ARGPARSE_SRCS}")
    set(DEPS "${ARGPARSE_DEPS}")

    list(LENGTH SRCS NUM_SRCS)
    if(${NUM_SRCS} LESS 1)
        message(SEND_ERROR "Invalid number of arguments. At least 1 source file must be given.")
        return()
    endif()

    set(NAME "Test.${NAME}")

    # Function content begins

    add_executable(${NAME} ${SRCS})
    target_link_libraries(${NAME} ${DEPS} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

    # This will work only if CTest is enabled

    add_test(NAME "${NAME}"
             COMMAND ${NAME} --catch_system_error=yes)

endfunction(add_executable_boost_test)
