#pragma once

#include "cgp/16_drawable/mesh_drawable/mesh_drawable.hpp"

namespace cgp
{

	namespace mesh_obj_advanced_loader
	{
		struct shape_element_node {
			mesh mesh_element;
			opengl_texture_image_structure texture_element;
		};

		std::vector<cgp::mesh_drawable> convert_to_mesh_drawable(std::vector<shape_element_node> const& elements);
	}

	std::vector<mesh_obj_advanced_loader::shape_element_node> mesh_load_file_obj_advanced(std::string const& directory, std::string const& filename);


}