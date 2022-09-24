#pragma once

#include "cgp/cgp.hpp"
#include "field_function.hpp"
#include "gui_helper.hpp"



// All the data used for the Implicit Surface 
// ********************************************** //

// Sub-structure that contains the discrete field data
struct implicit_surface_field_structure {
	cgp::spatial_domain_grid_3D domain;   // The domain where the discrete field is defined
	cgp::grid_3D<float> field;            // The grid storing the value of the field
	cgp::grid_3D<cgp::vec3> gradient;     // The discrete gradient of the field
};

// Sub-structure that contains the data of the surface
struct implicit_surface_data {
	size_t number_of_vertex;              // The valid number of vertex of the surface
	std::vector<cgp::vec3> position;      // Positions of the mesh
	std::vector<cgp::vec3> normal;        // Normals of the mesh
	std::vector<cgp::marching_cube_relative_coordinates> relative; // Relative coordinates of the vertices expressed as an edge in the discrete grid 
};

// Sub-structure that contains the elements that are displayed
struct implicit_surface_drawable_structure {
	cgp::triangles_drawable shape;     // Structure used to display the geometry
	cgp::curve_drawable domain_box;    // Structure used to display the box
};


// Global structure 
struct implicit_surface_structure 
{	
	implicit_surface_data data_param;
	implicit_surface_drawable_structure drawable_param;
	implicit_surface_field_structure field_param;	


	// Helpers functions that should be called in the scene
	// *************************************************** //

	//   Recompute from scratch the field and the marching cube
	void update_field(field_function_structure const& field_function, float isovalue);

	//   Recompute only the marching cube for a different isovalue (while minimize re-allocations)
	void update_marching_cube(float isovalue);

	//   Helper function to quickly set the domain (number of samples, and dimensions)
	void set_domain(int samples, cgp::vec3 const& length);
	
	//   Helper function to update the gui and call the associated update functions
	void gui_update(gui_parameters& gui, field_function_structure& field_function);
};


// Compute a grid filled with the value of some scalar function - the size of the grid is given by the domain
cgp::grid_3D<float> compute_discrete_scalar_field(cgp::spatial_domain_grid_3D const& domain, field_function_structure const& func);

// Compute the gradient of the scalar field using finite differences on the voxels
cgp::grid_3D<cgp::vec3> compute_gradient(cgp::grid_3D<float> const& field);
