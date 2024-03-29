cmake_minimum_required(VERSION 3.10)
project(SerialToolbox)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/external_dep.cmake)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -mtune=native -march=native")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif ()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5UiTools REQUIRED)


option(BUILD_TESTS "Build Unit Tests for SerialToolbox" ON)

if (BUILD_TESTS)
    add_external_dependency(googletest https://github.com/google/googletest.git main)
endif ()

set(SOURCES ${CMAKE_SOURCE_DIR}/Util/Serial/Interface.cpp
        ${CMAKE_SOURCE_DIR}/View/MainView.cpp
        ${CMAKE_SOURCE_DIR}/View/RepresentationFlowView.cpp
        ${CMAKE_SOURCE_DIR}/Controller/UiController.cpp
        ${CMAKE_SOURCE_DIR}/Controller/SendHandler.cpp
        ${CMAKE_SOURCE_DIR}/Controller/LineBreakStateMachine.cpp
        ${CMAKE_SOURCE_DIR}/Controller/SerialProxy.cpp)

if (UNIX)
    set(SOURCES ${SOURCES} ${CMAKE_SOURCE_DIR}/Util/Serial/InterfacePosix.cpp)
    add_definitions(-DPOSIX)
else ()
    message(FATAL_ERROR "Windows not supported, maybe try using cygwin (or a proper POSIX compliant system)")
endif ()

set(QT_LIBS Qt5::Core Qt5::Widgets Qt5::Quick Qt5::UiTools)
set(LIBS stdc++fs pthread ${QT_LIBS})

add_definitions(-DUI_FILE_LOCATION="${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/main.ui")

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES} ${LIBS})

install(TARGETS ${PROJECT_NAME}
        DESTINATION bin/)
install(FILES Res/main.ui DESTINATION share/${PROJECT_NAME}/)
install(CODE "execute_process(COMMAND xdg-desktop-menu install ${CMAKE_SOURCE_DIR}/aul12-SerialToolbox.desktop)")

add_subdirectory(Tests)

