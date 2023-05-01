#include "scene.hpp"

using namespace cgp;




// This function is called only once at the beginning of the program
// This function can contain any complex operation that can be pre-computed once
void scene_structure::initialize()
{
	std::cout << "Start function scene_structure::initialize()" << std::endl;

	// Set the behavior of the camera and its initial position
	// ********************************************** //
	camera_control.initialize(inputs, window);
	camera_control.look_at({ 4.0f, 4.0f, 1.5f }, { 0,0,0 }, { 0,0,1 });
	display_info();

	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	
	// Unzip the files in assets/ before runing the code
	//auto struct_shape = mesh_load_file_obj_advanced(project::path + "assets/f1/", "F1GenV3Backup.obj");
	auto struct_shape = mesh_load_file_obj_advanced(project::path + "assets/sponza/", "sponza.obj");
	shapes = mesh_obj_advanced_loader::convert_to_mesh_drawable(struct_shape);

}


void scene_structure::display_frame()
{

	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();


	timer.update();
	if (gui.display_frame)
		draw(global_frame, environment);


	for (int k = 0; k < shapes.size(); ++k)
		draw(shapes[k], environment);

	if (gui.display_wireframe) {
		for (int k = 0; k < shapes.size(); ++k)
			draw_wireframe(shapes[k], environment);
	}


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

void scene_structure::display_info()
{
	std::cout << "\nCAMERA CONTROL:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << camera_control.doc_usage() << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;


	std::cout << "\nSCENE INFO:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "This scene shows an example of load of more advanced obj models using the obj_advanced loader." << std::endl;
	std::cout << "The advanced loader uses the tinyobj library and can load multiples objects associated to different textures." << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;
}
