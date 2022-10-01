#pragma once


// *************************************************************** //
// OpenGL Version
// *************************************************************** //

// Default is OpenGL 3.3 (compatible with WebGL)
#define CGP_OPENGL_VERSION_MAJOR 3
#define CGP_OPENGL_VERSION_MINOR 3

// To activate OpenGL 4.6, change to
// #define CGP_OPENGL_VERSION_MAJOR 4
// #define CGP_OPENGL_VERSION_MINOR 6



// *************************************************************** //
// CGP ASSERTION
//
// Uncomment the following definition to remove cgp runtime debug checks
//   (+) Speed-up execution: remove all run-time checks in array computation
//   (-) No more assert check and debuging help.
//   This should be only removed for final code  */
// *************************************************************** //
// #define CGP_NO_DEBUG
