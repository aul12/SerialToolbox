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

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5UiTools REQUIRED)

set(SOURCES ${CMAKE_SOURCE_DIR}/Util/Serial/Interface.cpp
        ${CMAKE_SOURCE_DIR}/View/MainView.cpp
#        ${CMAKE_SOURCE_DIR}/View/ByteRepresentationWidget.cpp
        ${CMAKE_SOURCE_DIR}/Controller/UiController.cpp
        ${CMAKE_SOURCE_DIR}/Controller/SendHandler.cpp
        ${CMAKE_SOURCE_DIR}/Controller/SerialProxy.cpp)

if(UNIX)
    set(SOURCES ${SOURCES} ${CMAKE_SOURCE_DIR}/Util/Serial/InterfacePosix.cpp)
    add_definitions(-DPOSIX)
else()
    message(FATAL_ERROR "Windows not supported, maybe try using cygwin (or a proper POSIX compliant system)")
endif()

set(QT_LIBS Qt5::Core Qt5::Widgets Qt5::Quick Qt5::UiTools)
set(LIBS stdc++fs pthread ${QT_LIBS})

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES} ${LIBS})

add_subdirectory(Tests)
