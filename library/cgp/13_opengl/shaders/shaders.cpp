#include "shaders.hpp"

#include "cgp/01_base/base.hpp"
#include "cgp/03_files/files.hpp"
#include "cgp/13_opengl/debug/debug.hpp"
#include <iostream>

namespace cgp
{
    // Initialization of the static variable for the cache
    cache_uniform_location_structure opengl_shader_structure::cache_uniform_location;


    /** Load and compile shaders from glsl file sources
    * Display warnings and errors if the file cannot be accessed.
    * Display debug info when the shader is succesfully compiled. */
    GLuint opengl_load_shader(std::string const& vertex_shader_path, std::string const& fragment_shader_path, bool adapt_opengles = true);

    /** Compile shaders from direct text input.
    * Display no debug information in case of success */
    GLuint opengl_load_shader_from_text(std::string const& vertex_shader, std::string const& fragment_shader, bool* load_shader_ok=nullptr);



#ifdef __EMSCRIPTEN__
    static void replace_header_for_opengles(std::string& shader_txt)
    {
        std::string const target_string = "#version 330 core";

        std::size_t const pos = shader_txt.find(target_string);
        if (pos == std::string::npos) {
            std::cout << "Warning, could not find [" << target_string << "] in the shader to compile for OpenGL ES" << std::endl;
            return;
        }

        std::string const replacement_string = "#version 300 es\nprecision mediump float;";
        shader_txt.replace(pos, target_string.size(), replacement_string);

    }
#endif



    void opengl_shader_structure::load(std::string const& vertex_shader_path, std::string const& fragment_shader_path, bool adapt_opengles)
    {
        id = opengl_load_shader(vertex_shader_path, fragment_shader_path, adapt_opengles);
    }

    void opengl_shader_structure::load_from_inline_text(std::string const& vertex_shader_text, std::string const& fragment_shader_text, bool* load_shader_ok)
    {
        if (id != 0) {
            std::cout << " Warning: try to load a shader (" << vertex_shader_text << "," << fragment_shader_text << ") on a non empty shader_structure" << std::endl;
        }
#ifndef __EMSCRIPTEN__
        id = opengl_load_shader_from_text(vertex_shader_text, fragment_shader_text, load_shader_ok);
#else
        std::string new_vertex_shader = vertex_shader_text;
        std::string new_fragment_shader = fragment_shader_text;
        replace_header_for_opengles(new_vertex_shader);
        replace_header_for_opengles(new_fragment_shader);
        id = opengl_load_shader_from_text(new_vertex_shader, new_fragment_shader, load_shader_ok);
#endif
    }


    GLint opengl_shader_structure::query_uniform_location(std::string const& uniform_name) const
    {
        return cache_uniform_location.query(id, uniform_name);
    }

    void opengl_shader_structure::clear_cache_uniform_location()
    {
        cache_uniform_location.cache_data.clear();
    }
    std::string opengl_shader_structure::debug_dump_cache_uniform_location()
    {
        return str(cache_uniform_location);
    }


    static bool check_compilation(GLuint shader)
    {
        GLint is_compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);

        // get info on compilation
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog( static_cast<size_t>(maxLength)+1 );
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

        if( maxLength >= 1 )
        {
            std::cout << "[Info from shader compilation]"<< std::endl;
            std::cout << &infoLog[0] << std::endl;
        }

        if( is_compiled==GL_FALSE )
        {
            std::cout << "Compilation Failed" <<std::endl;
            glDeleteShader(shader);
            return false;
        }
        return true;
    }

    static bool check_link(GLuint vertex_shader, GLuint fragment_shader, GLuint program)
    {
        GLint is_linked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &is_linked);

        // Get info on Link
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog( static_cast<size_t>(maxLength)+1 );
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        if( maxLength >= 1 )
        {
            std::cout << "[Info from shader Link]"<< std::endl;
            std::cout << &infoLog[0] << std::endl;
        }
        if( is_linked==GL_FALSE ) //if failed link
        {
            // clean memory
            glDeleteProgram( program );
            glDeleteShader( vertex_shader);
            glDeleteShader( fragment_shader);

            std::cout << "Failed to link shader program" << std::endl;
            return false;
        }
        return true;

    }

	static bool compile_shader(const GLenum shader_type, std::string const& shader_str, GLuint& shader_id)
    {
        shader_id = glCreateShader(shader_type);
        assert_cgp( glIsShader(shader_id), "Error creating shader" );

        char const* const shader_cstring = shader_str.c_str();
        glShaderSource(shader_id, 1, &shader_cstring, nullptr);

        // Compile shader
        glCompileShader(shader_id);

        bool valid = check_compilation(shader_id);
        return valid;
    }


    
	GLuint opengl_load_shader_from_text(std::string const& vertex_shader_txt, std::string const& fragment_shader_txt, bool* load_shader_ok)
	{
        GLuint vertex_shader_id; 
        GLuint fragment_shader_id; 
        bool vertex_ok = compile_shader(GL_VERTEX_SHADER, vertex_shader_txt, vertex_shader_id);
        bool fragment_ok = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_txt, fragment_shader_id);

        if (!vertex_ok || !fragment_ok) {
            std::cout << "Error compiling shader" << std::endl;
            if(load_shader_ok!=nullptr)
                *load_shader_ok = false;
            return 0;
        }

        assert_cgp_no_msg( glIsShader(vertex_shader_id) );
        assert_cgp_no_msg( glIsShader(fragment_shader_id) );

        // Create Program
        GLuint const program_id = glCreateProgram();
        assert_cgp_no_msg( glIsProgram(program_id) );

        // Attach Shader to Program
        glAttachShader( program_id, vertex_shader_id );
        glAttachShader( program_id, fragment_shader_id );

        // Link Program
        glLinkProgram( program_id );

        bool link_ok = check_link(vertex_shader_id, fragment_shader_id, program_id);
        if (link_ok == false) {
            std::cout << "Error linking shader" << std::endl;
            if (load_shader_ok != nullptr)
                *load_shader_ok = false;
            return 0;
        }

        // Shader can be detached.
        glDetachShader( program_id, vertex_shader_id);
        glDetachShader( program_id, fragment_shader_id);

        if (load_shader_ok != nullptr)
            *load_shader_ok = true;

        return program_id;
	}



#ifdef __EMSCRIPTEN__
    GLuint opengl_load_shader(std::string const& vertex_shader_path, std::string const& fragment_shader_path, bool adapt_opengles)
#else
    GLuint opengl_load_shader(std::string const& vertex_shader_path, std::string const& fragment_shader_path, bool )
#endif
    {
        // Check the file are accessible
        if (check_file_exist(vertex_shader_path) == 0) {
            std::cout << "Warning: Cannot read the vertex shader at location " << vertex_shader_path << std::endl;
            std::cout << "If this file exists, you may need to adapt the directory from where your program is executed \n" << std::endl;
        }
        if (check_file_exist(fragment_shader_path) == 0) {
            std::cout << "Warning: Cannot read the fragment shader at location " << vertex_shader_path << std::endl;
            std::cout << "If this file exists, you may need to adapt the directory from where your program is executed \n" << std::endl;
        }

        // Stop the program here if the file cannot be accessed
        assert_file_exist(vertex_shader_path);
        assert_file_exist(fragment_shader_path);

        // Read the files
        std::string vertex_shader_text = read_text_file(vertex_shader_path);
        std::string fragment_shader_text = read_text_file(fragment_shader_path);


#ifdef __EMSCRIPTEN__
        if (adapt_opengles) {
            // Replace the header line of openGL version in the shader for become OpenGL ES
            // #version 330 core => #version 300 es + precision mediump float;
            replace_header_for_opengles(vertex_shader_text);
            replace_header_for_opengles(fragment_shader_text);

            std::cout << vertex_shader_text << std::endl;
        }
#endif
        


        // Compile the programs
        GLuint vertex_shader_id   = 0; 
        bool const vertex_shader_valid   = compile_shader(GL_VERTEX_SHADER  , vertex_shader_text  , vertex_shader_id);
        if (vertex_shader_valid == false) {
            std::cout << "===> Failed to compile the Vertex Shader [" << vertex_shader_path << "]" << std::endl;
            std::cout << "The error message from the compiler should be listed above. The program will stop." << std::endl;
            error_cgp("Failed to compile vertex shader "+ vertex_shader_path);
        }

        GLuint fragment_shader_id = 0;
        bool const fragment_shader_valid = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_text, fragment_shader_id);
        if (fragment_shader_valid == false) {
            std::cout << "===> Failed to compile the Fragment Shader [" << fragment_shader_path << "]" << std::endl;
            std::cout << "The error message from the compiler should be listed above. The program will stop." << std::endl;
            error_cgp("Failed to compile fragment shader " + fragment_shader_path);
        }

        assert_cgp_no_msg(glIsShader(vertex_shader_id));
        assert_cgp_no_msg(glIsShader(fragment_shader_id));


        // Create Program
        GLuint const program_id = glCreateProgram();
        assert_cgp_no_msg(glIsProgram(program_id));

        // Attach Shader to Program
        glAttachShader(program_id, vertex_shader_id);
        glAttachShader(program_id, fragment_shader_id);

        // Link Program
        glLinkProgram(program_id);

        bool const shader_program_valid = check_link(vertex_shader_id, fragment_shader_id, program_id);
        if (shader_program_valid == false) {
            std::cout << "Failed to link the shaders into a fragment program with the following shaders [" << vertex_shader_path<<","<< fragment_shader_path << "]" << std::endl;
            error_cgp("Failed to link shaders into a program");
        }


        // Shader can be detached.
        glDetachShader(program_id, vertex_shader_id);
        glDetachShader(program_id, fragment_shader_id);


        // Debug info
        std::string msg = "  [info] Shader compiled succesfully [ID=" + str(program_id) + "]\n";
        msg            += "         (" + vertex_shader_path + ", " + fragment_shader_path + ")\n";
        std::cout << msg << std::endl;

        return program_id;
    }


}