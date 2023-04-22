#pragma once

#include "cgp/cgp.hpp"


using cgp::mesh_drawable;

struct scene_elements_structure {
	mesh_drawable terrain;
	mesh_drawable water;
	mesh_drawable tree;
	mesh_drawable cube1;
	mesh_drawable cube2;
	mesh_drawable camel;

	void initialize();
	void update(float time);
	void display(cgp::environment_generic_structure const& environment, bool wireframe);
};
