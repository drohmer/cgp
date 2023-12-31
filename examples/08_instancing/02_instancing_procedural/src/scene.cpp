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
	camera_control.set_rotation_axis_z(); // camera rotates around z-axis
	//   look_at(camera_position, targeted_point, up_direction)
	camera_control.look_at(
		{0.0f, -6.0f, 1.5f } /* position of the camera in the 3D scene */,
		{0,0,0} /* targeted point in 3D scene */,
		{0,0,1} /* direction of the "up" vector */);
	display_info();

	// Create the global (x,y,z) frame
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());
	environment.background_color = { 0.85f, 0.94f, 1.0f };

	// load a tree model
	tree.initialize_data_on_gpu(mesh_load_file_obj(project::path + "assets/palm_tree/palm_tree.obj"));
	tree.model.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 2.0f);
	tree.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/palm_tree/palm_tree.jpg", GL_REPEAT, GL_REPEAT);

	// create the flat ground
	float const L = 20.0f; // size of the ground
	ground.initialize_data_on_gpu(mesh_primitive_quadrangle({-L,-L,0.0f}, { L,-L,0.0f }, { L,L,0.0f }, { -L,L,0.0f }));
	ground.material.color = { 0.4f, 0.6f, 0.3f };
	
	// load the blade of grass used for the instancing
	grass.initialize_data_on_gpu(mesh_primitive_quadrangle({ -0.5f,0.0f,0.0f }, { 0.5f,0.0f,0.0f }, { 0.5f,0.0f,1.0f }, { -0.5f,0.0f,1.0f }));
	grass.material.phong = { 1,0,0,1 };
	grass.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/grass.png");
	// to use correctly the instancing, we will need a specific shader able to treat differently each instance of the shape
	grass.shader.load(project::path + "shaders/instancing/instancing.vert.glsl", project::path + "shaders/instancing/instancing.frag.glsl");

	// add a per-instance vertex attribute
	numarray<vec3> instance_colors(gui.max_number_of_instances);
	for(int i=0; i<instance_colors.size(); ++i)
		instance_colors[i] = { rand_uniform(), rand_uniform(), 1.f };
	grass.initialize_supplementary_data_on_gpu(instance_colors, /*location*/ 4, /*divisor: 1=per instance, 0=per vertex*/ 1);
}


// This function is called permanently at every new frame
// Note that you should avoid having costly computation and large allocation defined there. This function is mostly used to call the draw() functions on pre-existing data.
void scene_structure::display_frame()
{

	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();

	// Send the time to the shader to animate the blade of grass
	environment.uniform_generic.uniform_float["time"] = timer.t;
	// Send a scaling parameter to spread the grass in a larger domain
	environment.uniform_generic.uniform_float["scaling_grass"] = gui.scaling_grass;


	timer.update();
	if (gui.display_frame)
		draw(global_frame, environment);
	draw(tree, environment);
	draw(ground, environment);


	// Draw the instances of grass: the third parameter is the number of instances to display
	draw(grass, environment, gui.number_of_instances);
	

	if (gui.display_wireframe) {
		draw_wireframe(ground, environment);
		draw_wireframe(tree, environment);

		draw_wireframe(grass, environment, {0,0,1}, gui.number_of_instances);
	}
	

}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

	// Control the number of instances
	ImGui::SliderInt("Instances", &gui.number_of_instances, 0, gui.max_number_of_instances);
	// Control how much the grass spreads far away in the (x,y) plane
	ImGui::SliderFloat("Scale", &gui.scaling_grass, 0.5f, 10.0f);

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
	std::cout << "This scene shows an example of instancing applied to grass blades." << std::endl;
	std::cout << "The deformation and position are fully set in the corresponding shader (shaders/instancing)." << std::endl;
	std::cout << "The change of color is pre-computed in C++ array and sent to the shader as a VBO attribute with per-instance divisor." << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;
}
