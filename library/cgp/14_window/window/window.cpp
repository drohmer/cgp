#include "cgp/13_opengl/opengl.hpp"
#include "window.hpp"

#include "cgp/01_base/base.hpp"
#include <iostream>

#ifndef GLFW_TRUE
#define GLFW_TRUE 1
#define GLFW_FALSE 0
#endif

namespace cgp
{
    /** Generate a window using GLFW.
    * Warning: This function should be called at the beginning of the program before any OpenGL calls.
    *          The function should only be called once.
    * The function initialize both GLFW and GLAD for OpenGL function access
    */
    static GLFWwindow* glfw_create_window(int width = 0, int height = 0, std::string const& window_title = "cgp Display", int opengl_version_major = 3, int opengl_version_minor = 3, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);




    static std::string glfw_error_string(int error)
    {
        switch(error)
        {
        case GLFW_NOT_INITIALIZED:
            return "GLFW_NOT_INITIALIZED";
        case GLFW_INVALID_ENUM:
            return "GLFW_INVALID_ENUM";
        case GLFW_INVALID_VALUE:
            return "GLFW_INVALID_VALUE";
        case GLFW_API_UNAVAILABLE:
            return "GLFW_API_UNAVAILABLE";
        case GLFW_VERSION_UNAVAILABLE:
            return "GLFW_VERSION_UNAVAILABLE";
        case GLFW_FORMAT_UNAVAILABLE:
            return "GLFW_FORMAT_UNAVAILABLE";
        case GLFW_PLATFORM_ERROR:
            return "GLFW_PLATFORM_ERROR";
        default:
            return "UNKNOWN GLFW ERROR (Shouldn't happen)";
        }

        // return "IMPOSSIBLE GLFW ERROR! (Should never happen)";
    }


    static void glfw_error_callback(int error, const char* description)
    {
        std::cerr<<"Received GLFW error"<<std::endl;
        std::cerr<<"\t Error "<<glfw_error_string(error)<<" ("<<error<<")"<<std::endl;
        std::cerr<<"\t Description - "<<description<<std::endl;
    }

	GLFWwindow* glfw_create_window(int width, int height, std::string const& window_title, int opengl_version_major, int opengl_version_minor, GLFWmonitor* monitor, GLFWwindow* share)
	{


         // Set GLFW parameter before creating the window
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API); 
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE); // Use only modern OpenGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_version_major); // Expect OpenGL 3.3 or greater
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_version_minor); 

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // Required for MacOS
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);  // Allow possible debug

        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE); // Take focus when created
        glfwWindowHint(GLFW_SAMPLES, 8); // Multisampling
        glfwWindowHint(GLFW_FLOATING, GLFW_FALSE); // Windows is not always on top

#ifdef __APPLE__
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE); // To avoid HiDPI issues with pixel size on Mac
#endif 

        // specific case when compiling with EMScripten for WebGL Output: Force the use of OpenGL ES
#ifdef __EMSCRIPTEN__
        emscripten_set_glfw_window_hint(); // re-write on top of the previous hints
        emscripten_update_window_size(width, height); // Use the canvas size
#endif

        // Creation of the window
        GLFWwindow* window = glfwCreateWindow(width, height, window_title.c_str(), monitor, share);

        if( window==nullptr ) {
            std::cerr<<"Failed to create GLFW Window"<<std::endl;
            std::cerr<<"\t Possible error cause: Incompatible OpenGL version (requesting OpenGL "<<opengl_version_major<<"."<<opengl_version_minor<<")"<<std::endl;
            std::cerr<<"\t Check your current system OpenGL version (ex. glxinfo)"<<std::endl;
            abort();
        }

        glfwMakeContextCurrent(window);


#ifndef __EMSCRIPTEN__
        // Initialize GLAD to get access to OpenGL functions
        const int glad_init_value = gladLoadGL();
        if( glad_init_value == 0 ) {
            std::cout<<"Failed to Init GLAD"<<std::endl;
            abort();
        }
#endif


        // Allows RGB texture in simple format
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	    glPixelStorei(GL_PACK_ALIGNMENT, 1);

        // Ask GLFW to limit its refresh to the monitor frequency 
        //  typically limits refresh rates to 60fps
        glfwSwapInterval(1); 

        return window;
	}


    void window_structure::initialize_glfw() {

        // Set GLFW callback to catch and display error
        glfwSetErrorCallback(glfw_error_callback);

        // Initialize GLFW
        const int glfw_init_value = glfwInit();
        if( glfw_init_value != GLFW_TRUE ) {
            std::string s = "\nError: Failed to Initialize GLFW.\n";
            s += "If you are using WSL or Linux without graphical server, you need to start a X-server\n";
            std::cout<<s<<std::endl;
            abort();
        }
        std::cout<<"\nGLFW initialized"<<std::endl;
    }


    void window_structure::create_window(int width_arg, int height_arg, std::string const& window_title, int opengl_version_major, int opengl_version_minor)
    {
        glfw_window = glfw_create_window(width_arg, height_arg, window_title, opengl_version_major, opengl_version_minor);

        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        screen_resolution_width = mode->width;
        screen_resolution_height = mode->height;

        glfwGetWindowPos(glfw_window, &x_pos, &y_pos);
        glfwGetWindowSize(glfw_window, &width, &height);

#ifdef __EMSCRIPTEN__
    // For Emscripten targets we will invoke some Javascript
    // to find out the dimensions of the canvas in the HTML
    // document. Note that the 'width' and 'height' attributes
    // need to be set on the <canvas /> HTML element, like so:
    // <canvas id="canvas" width="600", height="360"></canvas>
    int displayWidth = static_cast<int>(EM_ASM_INT({
        return document.getElementById('canvas').width;
    }));

    std::cout<<"EM_ASM: "<<displayWidth<<std::endl;
    
#endif
    }

    float window_structure::aspect_ratio() const
    {
        return width / static_cast<float>(height);
    }

    void window_structure::set_full_screen()
    {
        glfwSetWindowMonitor(glfw_window, monitor, 0, 0, screen_resolution_width, screen_resolution_height, GLFW_DONT_CARE);
    }
    void window_structure::set_windowed_screen()
    {
        glfwSetWindowMonitor(glfw_window, nullptr, 100, 100, screen_resolution_width / 2, screen_resolution_height / 2, GLFW_DONT_CARE);
    }

    
    vec2 window_structure::convert_pixel_to_relative_coordinates(vec2 const& p_pixel) const
    {
        assert_cgp_no_msg(width!=0 && height!=0);

        float const x_rel = p_pixel.x / static_cast<float>(width);
        float const y_rel = p_pixel.y / static_cast<float>(height);

        return vec2{
            2.0f * x_rel - 1.0f,
            2.0f * (1.0f - y_rel) - 1.0f
        };
    }

    int window_structure::monitor_width() const {
        return glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
    }
    int window_structure::monitor_height() const {
        return glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
    }

}