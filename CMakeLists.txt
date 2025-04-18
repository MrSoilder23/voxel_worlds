cmake_minimum_required(VERSION 3.31)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)

project(VoxelWorlds LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROG_NAME prog)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)

add_library(glad STATIC ${CMAKE_SOURCE_DIR}/third_party/glad.c)
target_include_directories(glad PRIVATE ${CMAKE_SOURCE_DIR}/third_party/include)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp")
add_executable(${PROG_NAME} ${SOURCES})

add_custom_command(TARGET ${PROG_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROG_NAME}>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${PROG_NAME}>/assets"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROG_NAME}>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/shaders"
    "$<TARGET_FILE_DIR:${PROG_NAME}>/shaders"
    COMMENT "Copying assets and shaders to build directory"
)

target_include_directories(${PROG_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/include    
)

# Link libraries
target_link_libraries(${PROG_NAME} PRIVATE
    glad
    SDL2::SDL2
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    TBB::tbb
)

# Windows-specific linking
if(WIN32)
    target_link_libraries(${PROG_NAME} PRIVATE SDL2::SDL2main)
endif()