cmake_minimum_required(VERSION 3.0)
project(chomper)

# Use C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror")

# Get all the source files
file(GLOB_RECURSE SOURCES src/*.c)

# Include local dirs
include_directories(src/)

# Get SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Get OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Include CGLM
add_subdirectory(external/cglm)
include_directories(external/cglm/include)

# Include stb
include_directories(external/stb/)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})
# Link the libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})