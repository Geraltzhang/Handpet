cmake_minimum_required(VERSION 3.16)
project(DigimonVPetLinux)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-Wno-psabi)

file(GLOB_RECURSE SRC_FILES
    src/VPetLCD/*.cpp
    src/GameLogic/*.cpp
    src/SaveGame/*.cpp
    src/linux_main.cpp
)

# Exclude Arduino-only files
list(FILTER SRC_FILES EXCLUDE REGEX ".*ESP32.*\\.cpp$")
list(FILTER SRC_FILES EXCLUDE REGEX ".*TFT_eSPI_.*\\.cpp$")
list(FILTER SRC_FILES EXCLUDE REGEX ".*/SaveGame/.*\\.cpp$")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(RSVG REQUIRED librsvg-2.0)

add_executable(vpet ${SRC_FILES})
target_include_directories(vpet PRIVATE src ${SDL2_INCLUDE_DIRS} ${CAIRO_INCLUDE_DIRS} ${RSVG_INCLUDE_DIRS})
target_link_libraries(vpet ${SDL2_LIBRARIES} ${CAIRO_LIBRARIES} ${RSVG_LIBRARIES})


