#include "scene.hpp"


using namespace cgp;

void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_y();
	camera_control.look_at({ 3.0f, 2.0f, 2.0f }, {0,0,0}, {0,0,1});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	
	image_structure image_skybox_template = image_load_file(project::path+"assets/skybox_01.jpg");
	// Other possibilities:
	//   image_structure image_skybox_template = image_load_file("assets/skybox_02.jpg");
	//   image_structure image_skybox_template = image_load_file("assets/skybox_debug.png");


	// Split the image into a grid of 4 x 3 sub-images
	std::vector<image_structure> image_grid = image_split_grid(image_skybox_template, 4, 3);


	skybox.initialize_data_on_gpu();
	skybox.texture.initialize_cubemap_on_gpu(image_grid[1], image_grid[7], image_grid[5], image_grid[3], image_grid[10], image_grid[4]);
	// Look at skybox_debug.png to see the correspondance of the image index


	shapes["torus"].initialize_data_on_gpu(mesh_primitive_torus());
	shapes["sphere"].initialize_data_on_gpu(mesh_primitive_sphere());
	shapes["cylinder"].initialize_data_on_gpu(mesh_primitive_cylinder());
	shapes["camel"].initialize_data_on_gpu(mesh_load_file_obj(project::path+"assets/camel.obj"));
	shapes["camel"].model.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, -Pi / 2.0f);

	opengl_shader_structure shader_environment_map;
	shader_environment_map.load(project::path + "shaders/environment_map/environment_map.vert.glsl", project::path + "shaders/environment_map/environment_map.frag.glsl");

	for (auto& shape_it : shapes) {
		mesh_drawable& shape = shape_it.second;
		shape.shader = shader_environment_map;
		shape.supplementary_texture["image_skybox"] = skybox.texture;
	}


}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	

	//  Must be called before drawing the other shapes and without writing in the Depth Buffer
	glDepthMask(GL_FALSE); // disable depth-buffer writing
	draw(skybox, environment);
	glDepthMask(GL_TRUE);  // re-activate depth-buffer write

	draw(shapes[gui.shape_to_display], environment);

	// Can now draw the other shapes of the scene
	if (gui.display_frame)
		draw(global_frame, environment);

	if (gui.display_wireframe) {
		draw_wireframe(shapes[gui.shape_to_display], environment);
	}

}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

	ImGui::Text("Surface to display:");
	if (ImGui::Button("Torus"))
		gui.shape_to_display = "torus";
	ImGui::SameLine();

	if (ImGui::Button("Sphere"))
		gui.shape_to_display = "sphere";
	ImGui::SameLine();

	if (ImGui::Button("Cylinder"))
		gui.shape_to_display = "cylinder";
	ImGui::SameLine();

	if (ImGui::Button("Camel"))
		gui.shape_to_display = "camel";
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

