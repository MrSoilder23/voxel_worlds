cmake_minimum_required(VERSION 3.31)

if(EXISTS ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif()

project(VoxelWorlds)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROG_NAME prog)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(TBB REQUIRED)

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    VERBATIM
)

add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    VERBATIM
)

add_library(glad STATIC ${CMAKE_SOURCE_DIR}/third_party/glad.c)
target_include_directories(glad PRIVATE ${CMAKE_SOURCE_DIR}/third_party/include)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp")
add_executable(${PROG_NAME} ${SOURCES})
add_dependencies(${PROG_NAME} copy_assets copy_shaders)

target_include_directories(${PROG_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/include    
)

# Link libraries
target_link_libraries(${PROG_NAME} PRIVATE
    glad
    SDL2::SDL2
    SDL2_image::SDL2_image
    TBB::tbb
)

# Windows-specific linking
if(WIN32)
    target_link_libraries(${PROG_NAME} PRIVATE SDL2::SDL2main)
endif()