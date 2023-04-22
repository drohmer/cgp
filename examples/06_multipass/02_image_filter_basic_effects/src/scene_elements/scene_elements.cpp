#include "scene_elements.hpp"
#include "../environment.hpp"

using namespace cgp;

static void deform_terrain(mesh& m);

void scene_elements_structure::initialize() {

	camel.initialize_data_on_gpu(mesh_load_file_obj(project::path + "assets/camel.obj"));
	camel.model.translation = { -1.5f, -2.5f, 0.1f };
	camel.model.scaling = 0.5f;

	float L = 5.0f;
	mesh terrain_mesh = mesh_primitive_grid({ -L,-L,0 }, { L,-L,0 }, { L,L,0 }, { -L,L,0 }, 100, 100);
	deform_terrain(terrain_mesh);
	terrain.initialize_data_on_gpu(terrain_mesh);
	terrain.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/sand.jpg");

	float sea_w = 8.0f;
	float sea_z = -0.8f;
	water.initialize_data_on_gpu(mesh_primitive_grid({ -sea_w,-sea_w,sea_z }, { sea_w,-sea_w,sea_z }, { sea_w,sea_w,sea_z }, { -sea_w,sea_w,sea_z }));
	water.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/sea.png");

	tree.initialize_data_on_gpu(mesh_load_file_obj(project::path + "assets/palm_tree/palm_tree.obj"));
	tree.model.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 2.0f);
	tree.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/palm_tree/palm_tree.jpg", GL_REPEAT, GL_REPEAT);

	cube1.initialize_data_on_gpu(mesh_primitive_cube({ 0,0,0 }, 0.5f));
	cube1.model.rotation = rotation_transform::from_axis_angle({ -1,1,0 }, Pi / 7.0f);
	cube1.model.translation = { 1.0f,1.0f,-0.1f };
	cube1.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/wood.jpg");

	cube2 = cube1;

}

static void deform_terrain(mesh& m)
{
	// Set the terrain to have a gaussian shape
	for (int k = 0; k < m.position.size(); ++k)
	{
		vec3& p = m.position[k];
		float d2 = p.x * p.x + p.y * p.y;
		float z = exp(-d2 / 4) - 1;

		z = z + 0.05f * noise_perlin({ p.x,p.y });

		p = { p.x, p.y, z };
	}

	m.normal_update();
}

void scene_elements_structure::update(float time) {

	cube2.model.translation = { -1.0f, 6.0f + 0.1 * sin(0.5f * time), -0.8f + 0.1f * cos(0.5f * time) };
	cube2.model.rotation = rotation_transform::from_axis_angle({ 1,-0.2,0 }, Pi / 12.0f * sin(0.5f * time));

}

void scene_elements_structure::display(cgp::environment_generic_structure const& environment, bool wireframe) {

	draw(terrain, environment);
	draw(water, environment);
	draw(tree, environment);
	draw(cube1, environment);
	draw(cube2, environment);
	draw(camel, environment);

	if (wireframe) {
		draw_wireframe(terrain, environment);
		draw_wireframe(water, environment);
		draw_wireframe(tree, environment);
		draw_wireframe(cube1, environment);
		draw_wireframe(cube2, environment);
		draw_wireframe(camel, environment);
	}
}

