cmake_minimum_required(VERSION 3.5)

project(Tetris VERSION 1.0 LANGUAGES CXX)

set(BLOCK_SCALE 40)
set(ROWS_COUNT 17)
set(COLLUMNS_COUNT 11)

set(DEFAULT_TETRIS_MODE OFF)
set(AUDIO ON)
set(INFO ON)
set(INFO_SWAP OFF)

set(SDL2_PATH "/usr/include/x86_64-linux-gnu/SDL2")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
set(STATIC_ASSETS_PATH "${PROJECT_SOURCE_DIR}/static")

set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/inc")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

configure_file(config.h.in ${INCLUDE_DIR}/config.h)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

if(NOT ${SDL2_FOUND})
    message(SEND_ERROR "Failed to find SDL")
    return()

elseif(NOT ${SDL2_image_FOUND})
    message(SEND_ERROR "Failed to find SDL2_image")
    return()

elseif(NOT ${SDL2_ttf_FOUND})
    message(SEND_ERROR "Failed to find SDL_ttf")
    return()

endif()

include_directories(
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_TTF_LIBRARIES}
)

file(GLOB SOURCE_FILES
    "${SOURCE_DIR}/*.cpp"
)

file(GLOB HEADER_FILES
    "${INCLUDE_DIR}/*.h"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)

if(AUDIO)

    if(NOT ${SDL2_mixer_FOUND})
        message(SEND_ERROR "Failed to find SDL_mixer")
        return()
    endif()

    find_package(SDL2_mixer REQUIRED)
    include_directories(${SDL2_MIXER_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} ${SDL2_MIXER_LIBRARIES})

endif(AUDIO)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
