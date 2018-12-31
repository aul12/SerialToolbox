project(Tests)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${GTEST_LIBS} ${GTKMM_LIBRARIES} stdc++fs gtest pthread)

add_test(
        NAME ${PROJECT_NAME}
        COMMAND ${PROJECT_NAME}
)

