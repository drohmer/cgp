#include "scene.hpp"

using namespace cgp;



void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_y();
	camera_control.look_at({ 3.0f, 2.0f, 2.0f }, {0,0,0}, {0,0,1});
	display_info();

	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	trunk.initialize_data_on_gpu(mesh_load_file_obj(project::path + "assets/trunk.obj"));
	trunk.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/trunk.png");

	branches.initialize_data_on_gpu(mesh_load_file_obj(project::path + "assets/branches.obj"));
	branches.material.color = { 0.45f, 0.41f, 0.34f }; // no textures on branches

	foliage.initialize_data_on_gpu(mesh_load_file_obj(project::path + "assets/foliage.obj"));
	foliage.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/pine.png");

	foliage.shader.load(project::path + "shaders/mesh_transparency/mesh_transparency.vert.glsl", project::path + "shaders/mesh_transparency/mesh_transparency.frag.glsl"); // set the shader handling transparency for the foliage
	foliage.material.phong = { 0.4f, 0.6f, 0, 1 };  // remove specular effect for the billboard

	
}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

	draw(trunk, environment);
	draw(branches, environment);

	if (gui.display_pines)
		draw(foliage, environment);


}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Pines", &gui.display_pines);
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

void scene_structure::display_info()
{
	std::cout << "\nCAMERA CONTROL:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << camera_control.doc_usage() << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;


	std::cout << "\nSCENE INFO:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "This scene displays a model where the leafs are impostors/billboards textures handled using discard in the shader." << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;
}