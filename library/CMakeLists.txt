
# Add all source files of CGP library
file(
    GLOB_RECURSE
    src_files_cgp
    ${CMAKE_CURRENT_LIST_DIR}/cgp/*.[ch]pp
)

#include Third party
include("${CMAKE_CURRENT_LIST_DIR}/third_party/CMakeLists.txt")

# Allow to sort and explore source directories as a tree structure in Visual Studio
if(MSVC)
    source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${src_files_cgp} ${src_files_third_party})
endif()
