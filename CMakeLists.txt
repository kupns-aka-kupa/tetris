cmake_minimum_required(VERSION 3.5)

project(Tetris VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

configure_file(config.h.in ../include/config.h)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#set(SDL2_INCLUDE_DIR /usr/local/include/SDL2)
#set(SDL2_LIBRARY /usr/local/lib/libSDL2.so)

find_package(SDL2 REQUIRED)

if(NOT SDL2_FOUND)
        message(SEND_ERROR "Failed to find SDL")
        return()
else()
        include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})
endif()

set(SUB_DIRS
    "${PROJECT_SOURCE_DIR}/src/*"
    "${PROJECT_SOURCE_DIR}/include"
)

#include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${SUB_DIRS})

file(GLOB SOURCES
    "src/main.cpp"
    "src/*/*.cpp"
)

file(GLOB HEADERS
    "include/*.h"
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${SDL2_LIBRARIES}
)

add_definitions(-Wall -O2)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
