#pragma once
// OpenGL Loader - This file should always be included first in order to have the adequate OpenGL setup
//   By default include glad for OpenGL 3.3
// 
//   Other options includes: 
//     - glad for OpenGL 4.6
//     - OpenGL ES 3 header for EMScripten
//
//   Note: CGP_OPENGL_VERSION_MAJOR and CGP_OPENGL_VERSION_MINOR are defined in cgp_parameters.hpp


#include "cgp_parameters.hpp"




#ifndef __EMSCRIPTEN__

// OpenGL 3.3 (default load)
#if CGP_OPENGL_VERSION_MAJOR==3 && CGP_OPENGL_VERSION_MINOR==3
	#include "third_party/src/glad/opengl33/glad.hpp" 
#endif

// OpenGL 4.6 Latest OpenGL
#if CGP_OPENGL_VERSION_MAJOR==4 && CGP_OPENGL_VERSION_MINOR==6
	#include "third_party/src/glad/opengl46/glad.hpp" 
#endif

// OpenGL 4.1 (for MacOS who do not support OpenGL>=4.1)
#if CGP_OPENGL_VERSION_MAJOR==4 && CGP_OPENGL_VERSION_MINOR==1
#include "third_party/src/glad/opengl41/glad.hpp" 
#endif

// OpenGL 4.3 (for MacOS who do not support OpenGL>=4.3)
#if CGP_OPENGL_VERSION_MAJOR==4 && CGP_OPENGL_VERSION_MINOR==3
#include "third_party/src/glad/opengl43/glad.hpp" 
#endif



// Rem. More information on GLAD can be found at these address
//	  GLAD website: https://github.com/Dav1dde/glad
//	  GLAD online generator: https://glad.dav1d.de/

#endif



// Special case for EMScripten (don't rely on glad)
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <GLES3/gl32.h> // OpenGL ES 3
#endif





