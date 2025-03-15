cmake_minimum_required(VERSION 3.31)
project(VoxelWorlds)

set(CMAKE_FLAGS "-g -O3")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_FLAGS}")

set(PROG_NAME prog)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/third_party/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

if(NOT DEFINED PLATFORM)
    if(WIN32)
        set(PLATFORM "Windows")
    elseif(UNIX)
        set(PLATFORM "Linux")
    else()
        message(FATAL_ERROR "Unsupported platform! Please specify PLATFORM=Windows or PLATFORM=Linux manually.")
    endif()
endif()

find_library(SDL2_LIBRARY NAMES SDL2 PATHS ${CMAKE_SOURCE_DIR}/lib NO_DEFAULT_PATH)
find_library(SDL2_IMAGE_LIB NAMES SDL2_image PATHS ${CMAKE_SOURCE_DIR}/lib NO_DEFAULT_PATH)

if(PLATFORM STREQUAL "Windows")
    message(STATUS "Configuring for windows")
    find_library(SDL2_MAIN_LIB NAMES SDL2main PATHS ${CMAKE_SOURCE_DIR}/lib NO_DEFAULT_PATH)
elseif(PLATFORM STREQUAL "Linux")
    message(STATUS "Configuring for linux")
endif()

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)

add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
)

add_library(glad STATIC ${CMAKE_SOURCE_DIR}/third_party/glad.c)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp")

add_executable(${PROG_NAME} ${SOURCES})

add_dependencies(${PROG_NAME} copy_assets copy_shaders)

set_target_properties(${PROG_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if(PLATFORM STREQUAL "Windows")
    target_link_libraries(${PROG_NAME} mingw32 ${SDL2_MAIN_LIB} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIB} glad)
elseif(PLATFORM STREQUAL "Linux")
    target_link_libraries(${PROG_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIB} glad)
endif()