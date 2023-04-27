#include "scene.hpp"


using namespace cgp;




void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_y();
	display_info();
	

	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	ground.initialize_data_on_gpu(mesh_primitive_quadrangle({ -1.0f,0.0f,-1.0f }, {-1.0f,0.0f, 1.0f }, { 1.0f,0.0f,1.0f }, { 1.0f,0.0f,-1.0f }));
	ground.model.scaling = 50.0f;
	ground.model.translation = { 0.0f, -0.5f, 0.0f };
	ground.texture.load_and_initialize_texture_2d_on_gpu(project::path+"assets/checkboard.png");

	cylinder.initialize_data_on_gpu(mesh_primitive_cylinder(0.05f, { 0,0,0 }, { 0,1,0 }));


	gui.display_frame = false;



}

void scene_structure::display_info()
{
	std::cout << "\nCAMERA CONTROL:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << camera_control.doc_usage() << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;


	std::cout << "\nSCENE INFO:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "This scene allows the character to give the impression of being able to walk on the ground using the mouse and keyboard (keys or WSAD/ZSQD)." << std::endl;
	std::cout << "For game-like mode: Use 'Shift+F' for full screen; 'Shift+C' for mouse capture." << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl ;
}

void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

	draw(ground, environment);
	for (int kx = -20; kx < 20; kx+=2) {
		for (int kz = -20; kz < 20; kz+=2) {
			if (kx != 0 || kz != 0) {
				cylinder.model.translation = { kx,-0.5f, kz };
				draw(cylinder, environment);
				if (gui.display_wireframe)
					draw_wireframe(cylinder, environment);
			}
		}
	}
	if (gui.display_wireframe)
		draw_wireframe(ground, environment);
}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}

void scene_structure::mouse_move_event()
{
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

