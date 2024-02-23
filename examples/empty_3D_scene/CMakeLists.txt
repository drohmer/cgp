# This is a generic CMake setup for CGP library use
cmake_minimum_required(VERSION 3.8) 

# Relative path to the CGP library
# => You may need to adapt this directory to your relative path in the case you move your directory
set(PATH_TO_CGP "../../library/" CACHE PATH "Relative path to CGP library location") 

# Set this value to ON if you want to use the precompiled GLFW Library
OPTION(MACOS_GLFW_PRECOMPILED "Use precompiled library for GLFW on MacOS" OFF)


# Check that the path to the library is correct
get_filename_component(ABS_PATH_TO_CGP ${PATH_TO_CGP} ABSOLUTE)
message(STATUS "The relative path to the library is set to ${PATH_TO_CGP}")
message(STATUS "The absolute path to the library is set to ${ABS_PATH_TO_CGP}")
if(NOT EXISTS ${ABS_PATH_TO_CGP})
   message(FATAL_ERROR "\nError: Could not import the CGP library using the relative path \"${PATH_TO_CGP}\".\n Please adjust this path in the CMakeLists.txt=>PATH_TO_CGP or via the cmake-gui\n Note that this relative path should point to the directory cgp/library/ ")
   return()
endif()

# Compile for Release with Debug Info
set(CMAKE_BUILD_TYPE RelWithDebInfo) 
set(CMAKE_CONFIGURATION_TYPES RelWithDebInfo) 
# uncomment the following to activate the other possibilities (Debug, Release)
#set(CMAKE_CONFIGURATION_TYPES RelWithDebInfo; Release; Debug )

# List the files of the current local project 
#    Default behavior: Automatically add all hpp and cpp files from src/ directory, and .glsl from shaders/
#    You may want to change this definition in case of specific file structure
file(GLOB_RECURSE src_files ${CMAKE_CURRENT_LIST_DIR}/src/*.[ch]pp ${CMAKE_CURRENT_LIST_DIR}/shaders/*.glsl)


# Generate the executable_name from the current directory name
get_filename_component(executable_name ${CMAKE_CURRENT_LIST_DIR} NAME)
# Another possibility is to set your own name: set(executable_name your_own_name) 
message(STATUS "Configure steps to build executable file [${executable_name}]")
project(${executable_name})

# Add current src/ directory
include_directories("src")

# Add the lib directory
include_directories(${ABS_PATH_TO_CGP})

# Include files from the CGP library (as well as external dependencies)
message(STATUS "Include CGP lib and external dependencies files from relative path")
include(${ABS_PATH_TO_CGP}/CMakeLists.txt)

add_definitions(-DSOLUTION)

# Uncomment the following line to remove assertion checks from CGP library (for full efficiency)
# add_definitions(-DCGP_NO_DEBUG)

# Set the OpenGL Compatibility Version
add_definitions(-DCGP_OPENGL_3_3)   # for OpenGL 3.3
# add_definitions(-DCGP_OPENGL_4_1) # for OpenGL 4.1
# add_definitions(-DCGP_OPENGL_4_3) # for OpenGL 4.3
# add_definitions(-DCGP_OPENGL_4_6) # for OpenGL 4.6


# Add all files to create executable
#  @src_files: the local file for this project
#  @src_files_cgp: all files of the cgp library
#  @src_files_third_party: all third party libraries compiled with the project
add_executable(${executable_name} ${src_files_cgp} ${src_files_third_party} ${src_files})


# Set Compiler for Unix system
if(UNIX)
   set(CMAKE_CXX_COMPILER g++)                      # Can switch to clang++ if prefered
   add_definitions(-g -O2 -std=c++14 -Wall -Wextra -Wfatal-errors -Wno-pragmas -Wno-unknown-warning-option) # Can adapt compiler flags if needed
   add_definitions(-Wno-sign-compare -Wno-type-limits) # Remove some warnings
endif()


# Set Compiler for Windows/Visual Studio
if(MSVC)
   set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT  ${executable_name} ) # default project (avoids AllBuild)
   set_target_properties( ${executable_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}$<0:> ) # default output in root dir
   set_target_properties( ${executable_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} ) # default debug execution in root dir
   
   # Avoids the warning /W3 overided by /W4 when using Ninja
   if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
   else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
   endif()

    add_definitions(/MP /wd4244 /wd4127 /wd4267 /wd4706 /wd4458 /wd4996 /wd26495 /openmp)   # Parallel build (/MP) + disable some warnings
    source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${src_files})  #Allow to explore source directories as a tree in Visual Studio
endif()



# Link options for Unix
target_link_libraries(${executable_name} ${GLFW_LIBRARIES})
if(UNIX)
   target_link_libraries(${executable_name} dl) #dlopen is required by Glad on Unix
endif()

