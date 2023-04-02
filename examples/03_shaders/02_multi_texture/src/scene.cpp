#include "scene.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_y();
	camera_control.look_at({ 3.0f, 2.0f, 2.0f }, {0,0,0}, {0,0,1});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	mesh mesh_grid = mesh_primitive_grid({ -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }, 200, 200);
		
	shape.initialize_data_on_gpu(mesh_primitive_quadrangle());

	shape.shader.load(project::path + "shaders/mesh_multi_texture/mesh_multi_texture.vert.glsl", project::path + "shaders/mesh_multi_texture/mesh_multi_texture.frag.glsl");

	// Load first texture
	shape.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/brick.png");

	// Load second texture
	shape.supplementary_texture["image_texture_2"].load_and_initialize_texture_2d_on_gpu(project::path + "assets/crack.png");

}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

	// Standard draw call
	draw(shape, environment);
	if(gui.display_wireframe)
		draw_wireframe(shape, environment);

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

