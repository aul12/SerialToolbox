
if (BUILD_TESTS)
    project(Tests)

    enable_testing()

    include_directories(${GTEST_INCLUDE_DIR})

    set(TEST_SOURCES
            ${CMAKE_SOURCE_DIR}/Tests/Util/Listener.cpp
            ${CMAKE_SOURCE_DIR}/Tests/Controller/SerialProxy.cpp
            ${CMAKE_SOURCE_DIR}/Tests/Controller/UiController.cpp
            ${CMAKE_SOURCE_DIR}/Tests/Controller/LineBreakStateMachine.cpp
            ${CMAKE_SOURCE_DIR}/Tests/Util/Serial/Interface.cpp)

    include_directories(${CMAKE_SOURCE_DIR})

    add_executable(${PROJECT_NAME} main.cpp ${SOURCES} ${TEST_SOURCES})

    target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES} ${LIBS} gmock gtest)

    add_test(
            NAME ${PROJECT_NAME}
            COMMAND ${PROJECT_NAME}
    )
endif ()
