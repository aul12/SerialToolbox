# Adds external dependency via git. Equivalent to submodule + add_subdirectory
function(add_external_dependency DEP_NAME DEP_REPO DEP_TAG)
    configure_file(cmake/external_dep_template.cmake.in ${DEP_NAME}-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${DEP_NAME}-download)
    if (result)
        message(FATAL_ERROR "CMake step for ${DEP_NAME} failed: ${result}")
    endif ()

    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${DEP_NAME}-download)
    if (result)
        message(FATAL_ERROR "Build step for ${DEP_NAME} failed: ${result}")
    endif ()

    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/${DEP_NAME}-src
            ${CMAKE_CURRENT_BINARY_DIR}/${DEP_NAME}-build
            EXCLUDE_FROM_ALL)
endfunction()
