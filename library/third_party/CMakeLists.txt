
# Add all source files of third parties
file(
    GLOB_RECURSE
    src_files_third_party
    ${CMAKE_CURRENT_LIST_DIR}/src/*.[ch]pp
    ${CMAKE_CURRENT_LIST_DIR}/src/*.[ch]
)

# Enable IMGUI to work with GLAD
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

# Include GLFW lib for Unix
if(UNIX)
    #expect GLFW3 already installed on the system
    find_package(glfw3 REQUIRED) 
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    include_directories(${GLFW_INCLUDE_DIRS})
endif()

# Include GLFW precompiled lib for Windows/Visual Studio
if(MSVC)
    set(GLFW_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/precompiled/windows/glfw/include")
    include_directories(${GLFW_INCLUDE_DIRS})
    set(GLFW_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/precompiled/windows/glfw/lib/glfw3.lib")
endif()

