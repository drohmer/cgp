
# Add all source files of third parties
file(
    GLOB_RECURSE
    src_files_third_party
    ${CMAKE_CURRENT_LIST_DIR}/src/*.[ch]pp
    ${CMAKE_CURRENT_LIST_DIR}/src/*.[ch]
)

# Enable IMGUI to work with GLAD
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)



# GLFW
#
#  - On Linux: expect user to install GLFW3 by its package manager
#     ex. on Ubuntu: apt install libgflw3-dev
#  - On MacOS: Provide pre-compiled library (otherwise can install glfw3 via brew)
#  - On Windows: Provide pre-compiled library for
#     - Visual Studio (compatible VS 2019 and VS 2022)
#     - MinGW64

if(UNIX) 
    if(APPLE AND MACOS_GLFW_PRECOMPILED) # Precompiled library for Apple (comment these lines and use the generic UNIX package if you prefer to use your own version of GLFW3)
        message(STATUS "USE PRECOMPILED GLFW for MACOS")
        set(GLFW_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/precompiled/glfw/macos/include")
        include_directories(${GLFW_INCLUDE_DIRS})
        set(GLFW_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/precompiled/glfw/macos/lib/libglfw.3.dylib")
    else()
        # Generic UNIX/Linux - install GLFW3 from your package manager
        #expect GLFW3 already installed on the system
        find_package(glfw3 REQUIRED) 
        find_package(PkgConfig REQUIRED)
        pkg_search_module(GLFW REQUIRED glfw3)
        include_directories(${GLFW_INCLUDE_DIRS})

    endif()

endif()

# Include GLFW precompiled lib for Windows/Visual Studio
if(WIN32)
    set(GLFW_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/precompiled/glfw/windows/include")
    include_directories(${GLFW_INCLUDE_DIRS})

    if(MSVC) # Lib for Visual Studio
        set(GLFW_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/precompiled/glfw/windows/lib-visual/glfw3.lib")
    else() # Otherwise assume you are using MinGW with Windows
        set(GLFW_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/precompiled/glfw/windows/lib-mingw-w64/libglfw3.a")
    endif()

endif()

