#pragma once

#include "cgp/04_grid_container/grid_container.hpp"
#include "cgp/05_vec/vec.hpp"

namespace cgp
{
	enum class image_color_type {rgb, rgba};
	struct image_structure
	{
		int width;
		int height;
		image_color_type color_type;
		numarray<unsigned char> data;


		image_structure();
		image_structure(unsigned int width_arg, unsigned int height_arg, image_color_type color_type_arg, numarray<unsigned char> const& data_arg);

		// Extract a subimage from the current one
		//  Subimages are defined by their corner coordinates in horizontal/vertical direction
		//  From kh=[start_h..end_h[, and kv=[start_v..end_v[
		//     Note that end_h, end_v are not included
		image_structure subimage(int start_h, int start_v, int end_h, int end_v) const;

		// Return a mirrored image in the horizontal direction
		image_structure mirror_horizontal() const;

		// Return a mirrored image in the vertical direction
		image_structure mirror_vertical() const;


		image_structure rotate_90_degrees_counterclockwise() const;
		image_structure rotate_90_degrees_clockwise() const;



	};

	image_structure image_load_png(std::string const& filename, image_color_type color_type = image_color_type::rgba);
	void image_save_png(std::string const& filename, image_structure const& im);
	image_structure image_load_jpg(std::string const& filename);
	void image_save_jpg(std::string const& filename, image_structure const& im);

	// Generic function to read an image file (expect .png or .jpg format)
	image_structure image_load_file(std::string const& filename);

	// Convert an image into a 2D grid structure 
	//  Each (r,g,b) component in [0,255] in the image is converted into a vec3 with component in [0,1]
	void convert(image_structure const& in, grid_2D<vec3>& out);

	// Split an image into sub-images in a grid made of N_horizontal x N_vertical parts
	//  The splitting must fit to the size of the image
	//  The output vector stores the sub-images as k_vertical + N_vertical*k_horizontal
	//  ex. For N_horizontal = 4, N_vertical = 3
	//    0 3 6  9  
	//    1 4 7 10
	//    2 5 8 11
	std::vector<image_structure> image_split_grid(image_structure const& image_in, int N_horizontal, int N_vertical);
}