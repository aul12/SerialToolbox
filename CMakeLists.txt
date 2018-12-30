cmake_minimum_required(VERSION 3.13)
project(HTermClone)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra -fsanitize=address -fno-omit-frame-pointer")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

add_definitions(${GTKMM_CFLAGS_OTHER})

add_executable(HTermClone main.cpp Util/Serial/Interface.cpp View/MainView.cpp)

target_link_libraries(HTermClone ${GTKMM_LIBRARIES})