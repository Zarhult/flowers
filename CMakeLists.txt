cmake_minimum_required(VERSION 2.4)

project(flowers)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})
add_definitions(${GTK_CFLAGS_OTHER})

add_executable(${PROJECT_NAME} flowers.c)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)

target_link_libraries(${PROJECT_NAME} ${GTK_LIBRARIES})
