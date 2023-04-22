#include "scene.hpp"


using namespace cgp;



void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({ 3.0f, 2.0f, 2.0f }, {0,0,0}, {0,0,1});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	camel.initialize_data_on_gpu(mesh_load_file_obj(project::path+"assets/camel.obj"));
	camel.model.translation = { -1.0f, -2.0f, 0.5f };
	camel.model.scaling = 0.5f;

	cube.initialize_data_on_gpu(mesh_primitive_cube());
	cube.model.translation = { -1.0f, 1.5f, 0.0f };

	// Second pass = image gradient
	std::string const v_shader_2 = project::path + "shaders/screen_effect_convolution/screen_effect_convolution.vert.glsl";
	std::string const f_shader_2 = project::path + "shaders/screen_effect_convolution/screen_effect_convolution.frag.glsl";

	// Third pass = image thickening
	std::string const v_shader_3 = project::path + "shaders/screen_effect_thickening/screen_effect_thickening.vert.glsl";
	std::string const f_shader_3 = project::path + "shaders/screen_effect_thickening/screen_effect_thickening.frag.glsl";

	opengl_shader_structure shader_2;
	opengl_shader_structure shader_3;
	shader_2.load(v_shader_2, f_shader_2);
	shader_3.load(v_shader_3, f_shader_3);

	multipass_rendering.initialize();
	multipass_rendering.set_shader_pass_2(shader_2);
	multipass_rendering.set_shader_pass_3(shader_3);
	environment.uniform_generic.uniform_int["thickness"] = 5;
}



void scene_structure::display_frame()
{
	environment.light = camera_control.camera_model.position();

	// ************************************** //
	// First rendering pass
	// ************************************* //
	multipass_rendering.update_screen_size(window.width, window.height);

	// 1- Activate the rendering on the FBO
	multipass_rendering.start_pass_1();

	// 2- Display the object of the scene
	draw(camel, environment);
	draw(cube, environment);

	if (gui.display_wireframe) {
		draw_wireframe(camel, environment);
		draw_wireframe(cube, environment);
	}

	// 3- Stop the rendering on the FBO
	multipass_rendering.end_pass_1();

	// ************************************** //
	// Second rendering pass
	// ************************************* //
	multipass_rendering.start_pass_2();
	multipass_rendering.draw_pass_2(environment);
	multipass_rendering.end_pass_2();


	// ************************************** //
	// Third rendering pass
	// ************************************* //

	multipass_rendering.start_pass_3();
	multipass_rendering.draw_pass_3(environment);
	multipass_rendering.end_pass_3();

	// Can display any shape in the scene (displayed on top of the quad)
	if (gui.display_frame)
		draw(global_frame, environment);


}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

	ImGui::SliderInt("Thickness", &environment.uniform_generic.uniform_int["thickness"], 1, 10);
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

