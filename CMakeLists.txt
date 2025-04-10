cmake_minimum_required(VERSION 3.21)
project(voxel_worlds LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform detection
if(WIN32)
    set(PLATFORM "Windows")
elseif(UNIX)
    set(PLATFORM "Linux")
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compiler-specific flags
if(MSVC)
    add_compile_options(/W4 /WX /O2 /Zi)
else()
    add_compile_options( -O3 -g)
endif()

# Find SDL2 libraries
find_library(SDL2_LIBRARY SDL2 
    PATHS ${CMAKE_SOURCE_DIR}/lib 
    NO_DEFAULT_PATH
    REQUIRED
)

find_library(SDL2_IMAGE_LIB SDL2_image 
    PATHS ${CMAKE_SOURCE_DIR}/lib 
    NO_DEFAULT_PATH
    REQUIRED
)

if(PLATFORM STREQUAL "Windows")
    find_library(SDL2_MAIN_LIB SDL2main 
        PATHS ${CMAKE_SOURCE_DIR}/lib 
        NO_DEFAULT_PATH
        REQUIRED
    )
endif()

# GLAD library
add_library(glad STATIC ${CMAKE_SOURCE_DIR}/third_party/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/third_party/include)

# Gather sources (avoid GLOB for serious projects; list files explicitly!)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Main executable (ensure PROG_NAME is defined)
set(PROG_NAME "voxel_worlds")
add_executable(${PROG_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/third_party/include
    ${CMAKE_SOURCE_DIR}/include
)

# Copy assets/shaders (only if directories exist)
if(EXISTS ${CMAKE_SOURCE_DIR}/assets)
    add_custom_command(TARGET ${PROG_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${PROG_NAME}>/assets
    )
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/shaders)
    add_custom_command(TARGET ${PROG_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders
            $<TARGET_FILE_DIR:${PROG_NAME}>/shaders
    )
endif()

# Link libraries
target_link_libraries(${PROG_NAME}
    glad
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIB}
)

if(PLATFORM STREQUAL "Windows")
    # Only link mingw32 if using MinGW (not MSVC)
    if(MINGW)
        target_link_libraries(${PROG_NAME} mingw32)
    endif()

    target_link_libraries(${PROG_NAME} ${SDL2_MAIN_LIB})

    # TBB
    find_library(TBB_LIB tbb 
        PATHS ${CMAKE_SOURCE_DIR}/lib 
        NO_DEFAULT_PATH
        REQUIRED
    )
    target_link_libraries(${PROG_NAME} ${TBB_LIB})
endif()