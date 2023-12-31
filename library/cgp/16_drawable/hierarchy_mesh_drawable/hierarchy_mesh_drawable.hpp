#pragma once

#include "cgp/16_drawable/mesh_drawable/mesh_drawable.hpp"

#include <map>
#include <vector>




namespace cgp
{
	struct hierarchy_mesh_drawable_node
	{
		mesh_drawable drawable;
		
		std::string name = "uninitialized";
		std::string name_parent = "global_frame";

		affine_rts transform_local;
	};


	struct hierarchy_mesh_drawable
	{
		
		std::vector<hierarchy_mesh_drawable_node> elements;

		// Lookup table to quickly find the index of an element from its name
		std::map<std::string, int> name_map;
		
		// Add new node to the hierarchy
		// Note: Parent node is expected to be already present in the hierarchy
		// The name of each node must be unique in the hierarchy
		void add(hierarchy_mesh_drawable_node const& node);
		void add(mesh_drawable const& element, std::string const& name, std::string const& name_parent = "global_frame", vec3 const& translation = vec3(), rotation_transform const& = rotation_transform());
		void add(mesh_drawable const& element, std::string const& name, std::string const& name_parent, affine_rts const& transform);

		// Get node by name
		hierarchy_mesh_drawable_node& operator[](std::string const& name);
		hierarchy_mesh_drawable_node const& operator[](std::string const& name) const;


		// Update the global coordinates of the nodes along the hierarchy
		//  This function must be called before draw, and called again if any hierarchical transform is modified
		void update_local_to_global_coordinates();

		// Helper function to display all the hierarchy
		std::string hierarchy_display() const;
	};

	void draw(hierarchy_mesh_drawable const& drawable, environment_generic_structure const& environment = environment_generic_structure(), int instance_count=1, bool expected_uniforms=true, uniform_generic_structure const& additional_uniforms = uniform_generic_structure());

	void draw_wireframe(hierarchy_mesh_drawable const& drawable, environment_generic_structure const& environment = environment_generic_structure(), vec3 const& color = { 0,0,1 }, int instance_count = 1, bool expected_uniforms=true, uniform_generic_structure const& additional_uniforms = uniform_generic_structure());


}