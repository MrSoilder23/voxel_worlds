cmake_minimum_required(VERSION 3.21)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(voxel_worlds LANGUAGES C CXX)

# Set output directory for executables and DLLs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)  # All exe/dlls go to "bin"

# Find packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(TBB REQUIRED)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# GLAD
add_library(glad STATIC third_party/glad.c)
target_include_directories(glad PUBLIC third_party/include)

# Sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    SDL2::SDL2
    SDL2_image::SDL2_image
    TBB::tbb
)

# Copy DLLs for shared library dependencies
foreach(DEP_TARGET IN ITEMS SDL2::SDL2 SDL2_image::SDL2_image TBB::tbb)
    get_target_property(DEP_TYPE ${DEP_TARGET} TYPE)
    if(DEP_TYPE STREQUAL "SHARED_LIBRARY")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:${DEP_TARGET}>"
                "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"  # Copy directly to "bin"
            COMMENT "Copying DLL for ${DEP_TARGET}"
        )
    endif()
endforeach()

# Post-build asset copying (to "bin/assets")
if(EXISTS ${CMAKE_SOURCE_DIR}/assets)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets"  # Assets in "bin/assets"
    )
endif()