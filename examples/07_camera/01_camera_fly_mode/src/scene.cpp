#include "scene.hpp"


using namespace cgp;





void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	terrain.initialize_data_on_gpu(mesh_load_file_obj(project::path+"assets/terrain.obj"));
	terrain.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/terrain.jpg");
	terrain.model.scaling = 3.0;
	terrain.model.translation = { 0.0f, -0.2f, 0.0f };

	gui.display_frame = false;
}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

	draw(terrain, environment);
	if (gui.display_wireframe)
		draw_wireframe(terrain, environment);
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

