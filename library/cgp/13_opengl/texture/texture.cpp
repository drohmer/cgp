#include "texture.hpp"

#include "cgp/01_base/base.hpp"

namespace cgp
{
    static GLenum format_to_data_type(GLint format)
    {
        switch (format)
        {
        case GL_RGB8:
        case GL_RGB32F:
            return GL_RGB;
        case GL_RGBA8:
            return GL_RGBA;
        case GL_DEPTH_COMPONENT:
            return GL_DEPTH_COMPONENT24;
        case GL_DEPTH_COMPONENT32F:
            return GL_DEPTH_COMPONENT32F;
        default:
            error_cgp("Unknown format");
        }
        error_cgp("Unreachable");
    }
    static GLenum format_to_component(GLint format)
    {
        switch (format)
        {
        case GL_RGB8:
        case GL_RGBA8:
            return GL_UNSIGNED_BYTE;
        case GL_RGB32F:
            return GL_FLOAT;
        case GL_DEPTH_COMPONENT:
            return GL_UNSIGNED_INT;
        default:
            error_cgp("Unknown format");
        }
        error_cgp("Unreachable");
    }

    template <typename TYPE>
    static GLuint opengl_initialize_texture_2d_on_gpu(int width, int height, TYPE const* data,
        GLint wrap_s, GLint wrap_t,
        GLenum texture_type, GLint format, GLenum gl_format, GLenum data_type, 
        bool is_mipmap, GLint texture_mag_filter, GLint texture_min_filter)
    {
        // Create texture
        GLuint id = 0;
        glGenTextures(1, &id); opengl_check;
        glBindTexture(texture_type, id); opengl_check;

        glTexImage2D(texture_type, 0, format, width, height, 0, gl_format, data_type, data); opengl_check;

        glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, wrap_s); opengl_check;
        glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, wrap_t); opengl_check;

        if (is_mipmap) {
            glGenerateMipmap(texture_type); opengl_check;
        }

        glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, texture_mag_filter); opengl_check;
        glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, texture_min_filter); opengl_check;
        

        glBindTexture(texture_type, 0); opengl_check;

        assert_cgp(glIsTexture(id), "Incorrect texture id");
        return id;
    }

    void opengl_texture_image_structure::bind() const
    {
        assert_cgp(id!=0, "Incorrect texture id");
        glBindTexture(texture_type, id); opengl_check;
    }
    void opengl_texture_image_structure::unbind() const
    {
        glBindTexture(texture_type, 0); opengl_check;
    }
    void opengl_texture_image_structure::clear()
    {
        assert_cgp(id != 0, "Cannot clear texture, ID=0");
        glDeleteTextures(1, &id);
        *this = opengl_texture_image_structure();
    }

    void opengl_texture_image_structure::update_wrap(GLint wrap_s, GLint wrap_t) const
    {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s); opengl_check;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t); opengl_check;
        unbind();
    }

    void opengl_texture_image_structure::initialize_texture_2d_on_gpu(image_structure const& im, GLint wrap_s, GLint wrap_t, bool is_mipmap, GLint texture_mag_filter, GLint texture_min_filter)
    {
        // Store parameters
        width = im.width;
        height = im.height;
        format = (im.color_type==image_color_type::rgba ? GL_RGBA8 : GL_RGB8);
        texture_type = GL_TEXTURE_2D;

        // Initialize texture data on GPU
        id = opengl_initialize_texture_2d_on_gpu(width, height, ptr(im.data), wrap_s, wrap_t, texture_type, format, format_to_data_type(format), format_to_component(format), is_mipmap, texture_mag_filter, texture_min_filter);
    }

    void opengl_texture_image_structure::load_and_initialize_texture_2d_on_gpu(std::string const& filename, GLint wrap_s, GLint wrap_t, bool is_mipmap, GLint texture_mag_filter, GLint texture_min_filter)
    {
        image_structure const im = image_load_file(filename);
        initialize_texture_2d_on_gpu(im, wrap_s, wrap_t, is_mipmap, texture_mag_filter, texture_min_filter);
    }

    void opengl_texture_image_structure::initialize_texture_2d_on_gpu(grid_2D<vec3> const& im, GLint wrap_s, GLint wrap_t, bool is_mippmap, GLint texture_mag_filter, GLint texture_min_filter)
    {
        // Store parameters
        width = im.dimension.x;
        height = im.dimension.y;
        format = GL_RGB32F;
        texture_type = GL_TEXTURE_2D;

        // Initialize texture data on GPU
        id = opengl_initialize_texture_2d_on_gpu(width, height, ptr(im.data),
            wrap_s, wrap_t, texture_type, format, format_to_data_type(format), format_to_component(format),
            is_mippmap, texture_mag_filter, texture_min_filter);

    }

    void opengl_texture_image_structure::initialize_texture_2d_on_gpu(int width_arg, int height_arg, GLint format_arg, GLenum texture_type_arg, GLint wrap_s, GLint wrap_t, GLint texture_mag_filter, GLint texture_min_filter)
    {
        // Store parameters
        width = width_arg;
        height = height_arg;
        format = format_arg;
        texture_type = texture_type_arg;

        // Initialize texture data on GPU
        id = opengl_initialize_texture_2d_on_gpu(width, height, (void*)NULL,
            wrap_s, wrap_t, texture_type, format, format_to_data_type(format), format_to_component(format),
            false, texture_mag_filter, texture_min_filter);

    }


    void opengl_texture_image_structure::initialize_cubemap_on_gpu(image_structure const& x_neg, image_structure const& x_pos, image_structure const& y_neg, image_structure const& y_pos, image_structure const& z_neg, image_structure const& z_pos)
    {
        // Sanity check on cubic texture
        int h = x_neg.width;
        width = h;
        height = h;
        image_color_type color_type = x_neg.color_type;
        format = (color_type == image_color_type::rgba ? GL_RGBA8 : GL_RGB8);
        texture_type = GL_TEXTURE_CUBE_MAP;

        assert_cgp_no_msg(x_neg.height == h);
        assert_cgp_no_msg(h == x_pos.width && h == x_pos.height);
        assert_cgp_no_msg(h == y_neg.width && h == y_neg.height);
        assert_cgp_no_msg(h == y_pos.width && h == y_pos.height);
        assert_cgp_no_msg(h == z_neg.width && h == z_neg.height);
        assert_cgp_no_msg(h == z_pos.width && h == z_pos.height);
        
        
        // Send images to GPU as cubemap
        glGenTextures(1, &id);
        glBindTexture(texture_type, id);

        GLenum const gl_format = format_to_data_type(format);    // expect GL_RGB or GL_RGBA
        GLenum const gl_component = format_to_component(format); // expect GL_UNISNGED_BYTE


        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, h, h, 0, gl_format, gl_component, ptr(x_neg.data));
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, h, h, 0, gl_format, gl_component, ptr(x_pos.data));

        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, h, h, 0, gl_format, gl_component, ptr(y_neg.data));
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, h, h, 0, gl_format, gl_component, ptr(y_pos.data));

        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, h, h, 0, gl_format, gl_component, ptr(z_neg.data));
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, h, h, 0, gl_format, gl_component, ptr(z_pos.data));

        glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(texture_type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


        glBindTexture(texture_type, 0);
    }


    void opengl_texture_image_structure::update(grid_2D<vec3> const& im)
    {
        assert_cgp(glIsTexture(id), "Incorrect texture id");

        glBindTexture(texture_type, id);
        glTexSubImage2D(texture_type, 0, 0, 0, GLsizei(im.dimension.x), GLsizei(im.dimension.y), format_to_data_type(format), format_to_component(format), ptr(im.data));
        glGenerateMipmap(texture_type);
        glBindTexture(texture_type, 0);
    }

    void opengl_texture_image_structure::update(image_structure const& im)
    {
        assert_cgp(glIsTexture(id), "Incorrect texture id");

        glBindTexture(texture_type, id);
        glTexSubImage2D(texture_type, 0, 0, 0, GLsizei(im.width), GLsizei(im.height), format_to_data_type(format), format_to_component(format), ptr(im.data));
        glGenerateMipmap(texture_type);
        glBindTexture(texture_type, 0);
    }

    //void opengl_texture_image_structure::update(GLuint texture_id, grid_2D<vec3> const& im)
    //{
    //    assert_cgp(glIsTexture(texture_id), "Incorrect texture id");

    //    glBindTexture(GL_TEXTURE_2D, texture_id);
    //    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLsizei(im.dimension.x), GLsizei(im.dimension.y), GL_RGB, GL_FLOAT, ptr(im.data));
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //    glBindTexture(GL_TEXTURE_2D, 0);
    //}


    GLuint opengl_load_texture_image(std::string const& filename, GLint wrap_s, GLint wrap_t)
    {
        image_structure const im = image_load_file(filename);
        if (im.width == 0 || im.height == 0) {
            warning_cgp("Warning texture has a size=0", "Filename=" + filename);
        }
        return opengl_load_texture_image(im, wrap_s, wrap_t);
    }

    GLuint opengl_load_texture_image(image_structure const& im, GLint wrap_s, GLint wrap_t)
    {
        GLuint id = 0;
        glGenTextures(1,&id); opengl_check;
        glBindTexture(GL_TEXTURE_2D,id); opengl_check;

        // Send texture on GPU
        if(im.color_type==image_color_type::rgba){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, im.width, im.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ptr(im.data)); opengl_check;
        }
        if(im.color_type==image_color_type::rgb){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, im.width, im.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(im.data)); opengl_check;
        }
        glGenerateMipmap(GL_TEXTURE_2D); opengl_check;

        // Set default texture behavior
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s); opengl_check;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t); opengl_check;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); opengl_check;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); opengl_check;

        glBindTexture(GL_TEXTURE_2D,0); opengl_check;

        return id;
    }
    GLuint opengl_load_texture_image(grid_2D<vec3> const& im, GLint wrap_s, GLint wrap_t)
    {
        GLuint id = 0;
        glGenTextures(1,&id); opengl_check;
        glBindTexture(GL_TEXTURE_2D,id); opengl_check;

        // Send texture on GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, GLsizei(im.dimension.x), GLsizei(im.dimension.y), 0, GL_RGB, GL_FLOAT, ptr(im.data)); opengl_check;
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set default texture behavior
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glBindTexture(GL_TEXTURE_2D,0);

        return id;
    }
    void opengl_update_texture_image(GLuint texture_id, grid_2D<vec3> const& im)
    {
        assert_cgp(glIsTexture(texture_id), "Incorrect texture id");

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, GLsizei(im.dimension.x), GLsizei(im.dimension.y), GL_RGB, GL_FLOAT, ptr(im.data));
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,0);
    }


}