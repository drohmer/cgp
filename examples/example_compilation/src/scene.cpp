#include "scene.hpp"


// Test scene to check correct compilation and run
//  The code should display a rotating cube and a textured quad on top of a circular ground.

using namespace cgp;


void scene_structure::initialize()
{
	// Set the behavior of the camera and its initial position
	camera_control.initialize(inputs, window);
	camera_control.set_rotation_axis_y();
	camera_control.look_at({ 3.0f, 2.0f, 2.0f }, { 0,0,0 }, { 0,0,1 });

	// Create a visual frame representing the coordinate system
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	// Initialize data to visualize
	mesh const cube_mesh = mesh_primitive_cube(/*center*/{ 0,0,0 }, /*edge length*/ 0.5f); // Create a cube as a mesh
	cube.initialize_data_on_gpu(cube_mesh); // Initialize the VBOs on the GPU
	cube.material.color = { 1,1,0.8 };      // Set values for uniform parameters

	ground.initialize_data_on_gpu(mesh_primitive_disc(1.0f, { 0,0,0 }, { 0,1,0 }));

	quad.initialize_data_on_gpu(mesh_primitive_quadrangle());
	quad.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/coquelicots.jpg");
	quad.model.scaling = 0.75f;
	quad.model.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 3.14f/3);
	quad.model.translation = { 0.25f,0,-0.1f };
}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);


	float const time = timer.t;
	timer.update();


	// Display cube
	// ********************************************* //
	cube.model.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, std::sin(3 * time));
	cube.model.translation = { -0.5f,0.251f,0 };
	draw(cube, environment);
	if(gui.display_wireframe)
		draw_wireframe(cube, environment);

	// Display ground
	// ********************************************* //
	draw(ground, environment);
	if (gui.display_wireframe)
		draw_wireframe(ground, environment);
	
	// Display textured quad
	// ********************************************* //
	draw(quad, environment);
	if (gui.display_wireframe)
		draw_wireframe(quad, environment);
}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
	camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
	camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
}

