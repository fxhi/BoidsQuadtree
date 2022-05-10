# From : https://shot511.github.io/2018-05-29-how-to-setup-opengl-project-with-cmake/
# - - - - - - - - - - - - - - - - - - 
# cmake -G "MinGW Makefiles" .
# cmake CMakeLists.txt
# cmake --build .
# - - - - - - - - - - - - - - - - - - 

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# set the project name and version
project(BoidsQuadtree VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/main.cpp
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)
    
# Add .lib files
link_directories(${CMAKE_SOURCE_DIR}/external/glfw-3.3.7/lib/)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# # We need a CMAKE_DIR with some code to find external dependencies
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
include_directories("${CMAKE_SOURCE_DIR}/external/glfw-3.3.7/include")
set( ENKITS_BUILD_EXAMPLES OFF CACHE BOOL "Build basic example applications" )
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_TESTS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_INSTALL OFF CACHE BOOL  "GLFW lib only" )

# GLAD
add_library(GLAD "${CMAKE_SOURCE_DIR}/external/glad/src/glad.c")
include_directories("${CMAKE_SOURCE_DIR}/external/glad/include")

# GLM
include_directories("${CMAKE_SOURCE_DIR}/external/glm/")

# ImGui
include_directories(
    "${CMAKE_SOURCE_DIR}/external/imgui/"
    "${CMAKE_SOURCE_DIR}/external/imgui/backends"
)

file(GLOB IMGUI_SOURCE_FILE
    ${CMAKE_SOURCE_DIR}/external/imgui/*.cpp
)

add_library(ImGui 
    ${IMGUI_SOURCE_FILE}
    "${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp"
)
    
# Put all libraries into a variable
set(LIBS glfw3 opengl32 GLAD ImGui)

# set flags
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wno-reorder")
# Only add those flags to ours source file, other there are also apply to ImGui and we could have some warning.
set_property(SOURCE ${SOURCE_FILES} PROPERTY COMPILE_FLAGS "-Wall -Wno-reorder")



# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})
