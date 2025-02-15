cmake_minimum_required(VERSION 3.31)
project(VoxelWorlds)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(PROG_NAME prog)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/third_party/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

find_library(SDL2_LIBRARY NAMES SDL2 PATHS ${CMAKE_SOURCE_DIR}/lib NO_DEFAULT_PATH)
find_library(SDL2_IMAGE_LIB NAMES SDL2_image PATHS ${CMAKE_SOURCE_DIR}/lib NO_DEFAULT_PATH)
find_library(SDL2_MAIN_LIB NAMES SDL2main PATHS ${CMAKE_SOURCE_DIR}/lib NO_DEFAULT_PATH)

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

target_link_libraries(${PROG_NAME} mingw32 ${SDL2_MAIN_LIB} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIB} glad)