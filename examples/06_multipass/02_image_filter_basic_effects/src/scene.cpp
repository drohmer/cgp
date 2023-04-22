#include "scene.hpp"


using namespace cgp;




void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({ 5.0f, -4.0f, 3.5f }, {0.0f, 0.0f, 0.5f}, {0.0f, 0.0f, 1.0f});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());
	environment.background_color = { 0.5,0.95,1.0f };

	// Load default shader effect
	opengl_shader_structure shader_effect;
	shader_effect.load(
		project::path + "shaders/screen_effect_contours/screen_effect_contours.vert.glsl", 
		project::path + "shaders/screen_effect_contours/screen_effect_contours.frag.glsl");

	// Initialize multipass renderer
	multipass_rendering.initialize();
	multipass_rendering.set_shader_pass_2(shader_effect);

	// Init default scene
	scene_elements.initialize();
}



void scene_structure::display_frame()
{
	environment.light = camera_control.camera_model.position();
	timer.update();

	environment.uniform_generic.uniform_float["time"] = timer.t;
	scene_elements.update(timer.t);


	// ************************************** //
	// First rendering pass
	// ************************************* //
	multipass_rendering.update_screen_size(window.width, window.height);

	// 1- Activate the rendering on the FBO
	multipass_rendering.start_pass_1(environment.background_color);

	// 2- Draw what we want in the scene
	if (gui.display_frame)
		draw(global_frame, environment);
	scene_elements.display(environment, gui.display_wireframe);

	// 3- Stop the rendering on the FBO
	multipass_rendering.end_pass_1();


	// ************************************** //
	// Second rendering pass
	// ************************************* //
	multipass_rendering.start_pass_2();
	multipass_rendering.draw_pass_2(environment); // Apply the screen effect
	multipass_rendering.end_pass_2();
}


void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);


	ImGui::Spacing(); ImGui::Spacing();
	ImGui::Separator();
	
	ImGui::Text("Select a shader Effect:");

	// All possible shader names
	std::vector<std::string> shader_names = {"default", "contours", "convolution", "color1", "color2", "color3", "deformation", "deformation2" };
	for (int k = 0; k < shader_names.size(); ++k) {
		bool click_shader = ImGui::Button(shader_names[k].c_str());

		// If click on a shader, load it and set is to the multipass_rendering structure
		if (click_shader) {
			std::string shader_path = project::path + "shaders/screen_effect_" + shader_names[k] + "/screen_effect_" + shader_names[k];
			opengl_shader_structure shader;
			shader.load(shader_path + ".vert.glsl", shader_path + ".frag.glsl");
			multipass_rendering.set_shader_pass_2(shader);
		}

	}
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

