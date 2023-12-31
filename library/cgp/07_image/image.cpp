#include "image.hpp"

#include "cgp/01_base/base.hpp"
#include "cgp/03_files/files.hpp"
#include "third_party/src/lodepng/lodepng.h"
#include "third_party/src/jpeg/jpge.h"
#include "third_party/src/jpeg/jpgd.h"

#include "cgp/13_opengl/opengl.hpp"

#if defined(__linux__) || defined(__EMSCRIPTEN__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace cgp
{
    static int size_of_component(image_color_type const& type)
    {
        if (type == image_color_type::rgb)
            return 3;
        else
            return 4;
    }

    image_structure::image_structure()
    :width(0), height(0), color_type(image_color_type::rgb), data()
    {}
    image_structure::image_structure(unsigned int width_arg, unsigned int height_arg, image_color_type color_type_arg, numarray<unsigned char> const& data_arg)
        :width(width_arg), height(height_arg), color_type(color_type_arg), data(data_arg)
    {}

    image_structure image_structure::subimage(int start_x, int start_y, int end_x, int end_y) const
    {
        // Sanity check
        assert_cgp_no_msg(start_x < end_x);
        assert_cgp_no_msg(start_y < end_y);
        assert_cgp_no_msg(start_x >= 0);
        assert_cgp_no_msg(start_y >= 0);
        assert_cgp_no_msg(end_x <= width);
        assert_cgp_no_msg(end_y <= height);

        // copy
        image_structure new_image;
        new_image.width = end_x - start_x;
        new_image.height = end_y - start_y;
        new_image.color_type = color_type;

        int s = size_of_component(color_type);
        new_image.data.resize(s * new_image.width * new_image.height);
        for (int kx = 0; kx < new_image.width; ++kx) {
            for (int ky = 0; ky < new_image.height; ++ky) {
                int offset_in = s * ((kx+start_x) + width * (ky+start_y));
                int offset_out = s * (kx + new_image.width*ky);
                for (int ks = 0; ks < s; ++ks) {
                    new_image.data[offset_out + ks] = data[offset_in + ks];
                }
            }
        }

        return new_image;
    }

    image_structure image_load_png(std::string const& filename, image_color_type color_type)
    {
        assert_file_exist(filename);

        LodePNGColorType lodepng_color_type;
        if(color_type==image_color_type::rgb)
            lodepng_color_type = LCT_RGB;
        else if(color_type==image_color_type::rgba)
            lodepng_color_type = LCT_RGBA;
        else
        {
            std::cerr<<"Unkown color type for file"<<filename<<std::endl;
            exit(1);
        }

        image_structure im;
        im.color_type = color_type;

        unsigned w=0, h = 0;
        unsigned error = lodepng::decode(im.data.data, w, h, filename, lodepng_color_type);
        if ( error )
        {
            std::cerr<<"Error Loading png file "<<filename<<std::endl;
            std::cerr<<"Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
            exit(1);
        }
        im.width = w;
        im.height = h;

        return im;
    }

    void image_save_png(std::string const& filename, image_structure const& im)
    {
        LodePNGColorType lodepng_color_type;
        if(im.color_type==image_color_type::rgb)
            lodepng_color_type = LCT_RGB;
        else if(im.color_type==image_color_type::rgba)
            lodepng_color_type = LCT_RGBA;
        else
        {
            std::cerr<<"Unkown color type for file"<<filename<<std::endl;
            exit(1);
        }

        //std::vector<unsigned char> output;
        unsigned error = lodepng::encode(filename, im.data.data, im.width, im.height, lodepng_color_type);
        if ( error )
        {
            std::cerr<<"Error Loading png file "<<filename<<std::endl;
            std::cerr<<"Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
            exit(1);
        }
    }

    void convert(image_structure const& in, grid_2D<vec3>& out)
    {
        size_t const N = size_t(in.width) * size_t(in.height);
        out.resize(in.width, in.height);
        if (in.color_type == image_color_type::rgb)
        {
            for(size_t k=0; k<N; ++k)
                out.data[k] = vec3(in.data[3*k+0], in.data[3*k+1], in.data[3*k+2])/255.0f;
        }
        else if (in.color_type == image_color_type::rgba)
        {
            for(size_t k=0; k<N; ++k)
                out.data[k] = vec3(in.data[4*k+0], in.data[4*k+1], in.data[4*k+2])/255.0f;
        }
    }

    image_structure image_load_jpg(std::string const& filename)
    {
        assert_file_exist(filename);

        int width = 0;
        int height = 0;
        int actual_comps = 0;

        unsigned char* p = jpgd::decompress_jpeg_image_from_file(filename.c_str(), &width, &height, &actual_comps, 3);

        image_structure im;
        im.color_type = image_color_type::rgb;
        im.width = width;
        im.height = height;

        int const N = im.width * im.height * 3;
        im.data.resize(N);
        for (int k = 0; k < N; ++k)
            im.data[k] = p[k];

        return im;
    }

    void image_save_jpg(std::string const& filename, image_structure const& im)
    {
        bool res = jpge::compress_image_to_jpeg_file( filename.c_str(), im.width, im.height, 3, ptr(im.data));
        assert_cgp(res == 1, "Failed to save jpg image " + filename);
    }


    image_structure image_load_file(std::string const& filename)
    {
        size_t const N = filename.size();
        if (N > 4) {
            std::string const ext_4 = filename.substr(N - 4, 4);
            if (ext_4 == ".png")
                return image_load_png(filename);
            else if (ext_4 == ".jpg" || ext_4 == ".JPG")
                return image_load_jpg(filename);
        }
        if (N > 5) {
            std::string const ext_5 = filename.substr(N - 5, 5);
            if (ext_5 == ".jpeg")
                return image_load_jpg(filename);
        }

        error_cgp("Error image_load_file(" + filename + "), could not detect a valid extension while expecting .jpg or .png file\n");
        
    }


    std::vector<image_structure> image_split_grid(image_structure const& image_in, int N_horizontal, int N_vertical)
    {
        // Sanity check
        assert_cgp(N_horizontal > 0, "Split image should have N_horizontal>0");
        assert_cgp(N_vertical > 0, "Split image should have N_vertical>0");

        // Sub-image size
        int const width = image_in.width / N_horizontal;
        int const height = image_in.height / N_vertical;

        // Check dimension coherence
        if ((width * N_horizontal != image_in.width) || (height * N_vertical != image_in.height)) {
            std::cout << "Error in image_split_grid." << std::endl;
            std::cout << "Cannot split image ("<< image_in.width <<"x"<< image_in.height <<") into ("<< N_horizontal <<"x"<< N_vertical <<") blocks" << std::endl;
            abort();
        }

        std::vector<image_structure> subimages;
        subimages.resize(N_horizontal * N_vertical);
        for (int kh = 0; kh < N_horizontal; ++kh) {
            for (int kv = 0; kv < N_vertical; ++kv) {
                subimages[kv+N_vertical*kh] = image_in.subimage(kh * width, kv * height, (kh + 1) * width, (kv + 1) * height);
            }
        }

        return subimages;
    }

    
    image_structure image_structure::mirror_horizontal() const
    {
        image_structure mirrored = *this;
        int d = size_of_component(color_type);
        for (int kx = 0; kx < width; ++kx) {
            for (int ky = 0; ky < height; ++ky) {
                int offset_in  = kx + width * ky;
                int offset_out = (width-kx-1) + width * ky;
                for(int kd=0; kd<d; ++kd)
                    mirrored.data[kd+d*offset_out] = data[kd+d*offset_in];
            }
        }

        return mirrored;
    }


    image_structure image_structure::mirror_vertical() const
    {
        image_structure mirrored = *this;

        int const d = size_of_component(color_type);
        for (int kx = 0; kx < width; ++kx) {
            for (int ky = 0; ky < height; ++ky) {
                int offset_in = kx + width * ky;
                int offset_out = kx + width * (height-ky-1);

                for (int kd = 0; kd < d; ++kd)
                    mirrored.data[kd + d * offset_out] = data[kd + d * offset_in];
            }
        }

        return mirrored;
    }

    image_structure image_structure::rotate_90_degrees_counterclockwise() const
    {
        int const d = size_of_component(color_type);

        image_structure rotated;
        rotated.height = width;
        rotated.width = height;
        rotated.color_type = color_type;
        rotated.data.resize(width * height * d);

        for (int kx = 0; kx < width; ++kx) {
            for (int ky = 0; ky < height; ++ky) {
                int offset_in = (width-kx-1) + width * ky;
                int offset_out = ky + height * kx;

                for (int kd = 0; kd < d; ++kd)
                    rotated.data[kd + d * offset_out] = data[kd + d * offset_in];
            }
        }

        return rotated;
    }
    image_structure image_structure::rotate_90_degrees_clockwise() const
    {
        int const d = size_of_component(color_type);

        image_structure rotated;
        rotated.height = width;
        rotated.width = height;
        rotated.color_type = color_type;
        rotated.data.resize(width * height * d);

        for (int kx = 0; kx < width; ++kx) {
            for (int ky = 0; ky < height; ++ky) {
                int offset_in = kx + width * (height-ky-1);
                int offset_out = ky + height * kx;

                for (int kd = 0; kd < d; ++kd)
                    rotated.data[kd + d * offset_out] = data[kd + d * offset_in];
            }
        }

        return rotated;
    }
        

}