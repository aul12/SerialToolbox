cmake_minimum_required(VERSION 3.10)
project(SerialToolbox)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif ()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

include_directories(SYSTEM ${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

add_definitions(${GTKMM_CFLAGS_OTHER})

set(SOURCES ${CMAKE_SOURCE_DIR}/Util/Serial/Interface.cpp
        ${CMAKE_SOURCE_DIR}/View/MainView.cpp
        ${CMAKE_SOURCE_DIR}/View/ByteRepresentationWidget.cpp
        ${CMAKE_SOURCE_DIR}/Controller/UiController.cpp
        ${CMAKE_SOURCE_DIR}/Controller/SerialProxy.cpp)

if(UNIX)
    set(SOURCES ${SOURCES} ${CMAKE_SOURCE_DIR}/Util/Serial/InterfacePosix.cpp)
    add_definitions(-DPOSIX)
else()
    message(FATAL_ERROR "Windows not supported, maybe try using cygwin (or a proper POSIX compliant system)")
endif()

set(LIBS stdc++fs pthread ${GTKMM_LIBRARIES})

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES} ${LIBS})

add_subdirectory(Tests)
