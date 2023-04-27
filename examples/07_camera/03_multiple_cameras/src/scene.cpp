#include "scene.hpp"


using namespace cgp;





void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera2_control.initialize(inputs, window);

	camera_control.look_at({ 3,2,2 }, { 0,0,0 }, { 0,0,1 });
	


	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	cube.initialize_data_on_gpu(mesh_primitive_cube());
	cube.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/wood.jpg");

}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	timer.update(); // Update the time

	// Reset the transformation applied to the cube
	cube.model.rotation = rotation_transform(); 
	cube.model.translation = { 0,0,0 };


	// ****************************************************** //
	// Prepare the display with the first camera
	// ****************************************************** //
	environment.camera_view = camera_control.camera_model.matrix_view();
	if (gui.display_frame)
		draw(global_frame, environment);

	draw(cube, environment);
	if (gui.display_wireframe)
		draw_wireframe(cube, environment);


	// ****************************************************** //
	// 	Prepare the display with the second camera
	// ****************************************************** //
	camera2_control.look_at({ 8,0.5,0 }, { 0,0,0 }, { 0,0,1 }); // change the camera2 position
	environment.camera_view = camera2_control.camera_model.matrix_view(); // Update the camera view stored in the environment (the one used in the shader when displaying the mesh_drawable)
	environment.light = { 7,1,0 }; // change the light position
	// apply some transformation to the cube	
	cube.model.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, timer.t);
	cube.model.translation = { 0,2.5,0 };
	// Display the cube with the environment using the second camera
	draw(cube, environment);
	if (gui.display_wireframe)
		draw_wireframe(cube, environment);
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

