# Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Set your project name
project(OpenGL)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/vendor)

# Add library directories
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Find all source files in the 'src' directory
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Find all ImGui source files
file(GLOB IMGUI_SOURCES "${CMAKE_SOURCE_DIR}/include/vendor/imgui/*.cpp")

# Add the ImGui source files to your project's executable
add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES} "${CMAKE_SOURCE_DIR}/src/glad.c")

# Link against the libraries
target_link_libraries(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/lib/libglfw.3.3.dylib" "-framework OpenGL")

# Set the output directory for executables
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

add_definitions(-Wno-deprecated-declarations)