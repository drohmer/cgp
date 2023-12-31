#include "debug.hpp"

#include "cgp/01_base/base.hpp"
#include <iostream>

namespace cgp
{
	std::string opengl_info_display()
	{
        using cgp::str;

		std::string s;
		s += "[VENDOR]      : " + str((char*)(glGetString(GL_VENDOR))) + "\n";
		s += "[RENDERER]    : " + str((char*)(glGetString(GL_RENDERER))) + "\n";
		s += "[VERSION]     : " + str((char*)(glGetString(GL_VERSION))) + "\n";
		s += "[GLSL VERSION]: " + str((char*)(glGetString(GL_SHADING_LANGUAGE_VERSION))) + "\n";

        return s;
	}

	static std::string opengl_error_to_string(GLenum error)
    {
        switch(error)
        {
        case GL_NO_ERROR:
            return "GL_NO_ERROR";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
    #ifndef __EMSCRIPTEN__
        case GL_STACK_UNDERFLOW:
            return "GL_STACK_UNDERFLOW";
        case GL_STACK_OVERFLOW:
            return "GL_STACK_OVERFLOW";
    #endif
        default:
            return "UNKNOWN";
        }
    }
	void check_opengl_error(std::string const& file, std::string const& function, int line)
	{
        GLenum error = glGetError();
        if( error !=GL_NO_ERROR )
        {
            std::string msg = "OpenGL ERROR detected\n"
                    "\tFile "+file+"\n"
                    "\tFunction "+function+"\n"
                    "\tLine "+str(line)+"\n"
                    "\tOpenGL Error: "+opengl_error_to_string(error);

            error_cgp(msg);
        }
	}
}