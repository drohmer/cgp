

#include "cgp/cgp.hpp" // Give access to the complete CGP library
#include "path_info.hpp"  // Additional info on the system to help setup a scene
#include <iostream> 



// Custom scene of this code
#include "scene.hpp"




// *************************** //
// Custom Scene defined in "scene.hpp"
// *************************** //

scene_structure scene;


// The rest of this code is a generic initialization and animation loop that can be applied to different scenes
// *************************** //
// Start of the program
// *************************** //

window_structure standard_window_initialization(int width = 0, int height = 0);
void initialize_default_shaders();
void animation_loop();

timer_fps fps_record;

int main(int, char* argv[])
{
	std::cout << "Run " << argv[0] << std::endl;



	// ************************ //
	//     INITIALISATION
	// ************************ //

	// Standard Initialization of an OpenGL ready window
	scene.window = standard_window_initialization();

	// Initialize System Info
	path_info::initialize(argv[0]);

	// Initialize default shaders
	initialize_default_shaders();


	// Custom scene initialization
	std::cout << "Initialize data of the scene ..." << std::endl;
	scene.initialize();
	std::cout << "Initialization finished\n" << std::endl;


	// ************************ //
	//     Animation Loop
	// ************************ //
	std::cout << "Start animation loop ..." << std::endl;
	fps_record.start();

#ifndef __EMSCRIPTEN__
	// Default mode to run the animation/display loop with GLFW in C++
	while (!glfwWindowShouldClose(scene.window.glfw_window)) {
		animation_loop();
	}
#else
	// Specific loop if compiled for EMScripten
	emscripten_set_main_loop(animation_loop, 0, 1);
#endif

	std::cout << "\nAnimation loop stopped" << std::endl;

	// Cleanup
	cgp::imgui_cleanup();
	glfwDestroyWindow(scene.window.glfw_window);
	glfwTerminate();

	return 0;
}

void animation_loop()
{

	emscripten_update_window_size(scene.window.width, scene.window.height); // update window size in case of use of emscripten (not used by default)

	scene.camera_projection.aspect_ratio = scene.window.aspect_ratio();
	scene.environment.camera_projection = scene.camera_projection.matrix();
	glViewport(0, 0, scene.window.width, scene.window.height);

	vec3 const& background_color = scene.environment.background_color;
	glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	float const time_interval = fps_record.update();
	if (fps_record.event) {
		std::string const title = "CGP Display - " + str(fps_record.fps) + " fps";
		glfwSetWindowTitle(scene.window.glfw_window, title.c_str());
	}

	imgui_create_frame();
	ImGui::Begin("GUI", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	scene.inputs.mouse.on_gui = ImGui::GetIO().WantCaptureMouse;
	scene.inputs.time_interval = time_interval;


	// Display the ImGUI interface (button, sliders, etc)
	scene.display_gui();

	// Handle camera behavior in standard frame
	scene.idle_frame();

	// Call the display of the scene
	scene.display_frame();


	// End of ImGui display and handle GLFW events
	ImGui::End();
	imgui_render_frame(scene.window.glfw_window);
	glfwSwapBuffers(scene.window.glfw_window);
	glfwPollEvents();
}


void initialize_default_shaders()
{
	// Generate the default directory from which the shaders are found
	//  By default, it should be "shaders/opengl3/"
	std::string default_path_shaders = path_info::shaders;

	// Set standard mesh shader for mesh_drawable
	mesh_drawable::default_shader.load(default_path_shaders +"mesh/vert.glsl", default_path_shaders +"mesh/frag.glsl");
	triangles_drawable::default_shader.load(default_path_shaders +"mesh/vert.glsl", default_path_shaders +"mesh/frag.glsl");

	// Set default white texture
	image_structure const white_image = image_structure{ 1,1,image_color_type::rgba,{255,255,255,255} };
	mesh_drawable::default_texture.initialize_texture_2d_on_gpu(white_image);
	triangles_drawable::default_texture.initialize_texture_2d_on_gpu(white_image);

	// Set standard uniform color for curve/segment_drawable
	curve_drawable::default_shader.load(default_path_shaders +"single_color/vert.glsl", default_path_shaders+"single_color/frag.glsl");
}





//Callback functions
void window_size_callback(GLFWwindow* window, int width, int height);
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
void keyboard_callback(GLFWwindow* window, int key, int, int action, int mods);

// Standard initialization procedure
window_structure standard_window_initialization(int width_target, int height_target)
{
	// Create the window using GLFW
	// ***************************************************** //
	window_structure window;
	window.initialize(width_target, height_target, "CGP Display", CGP_OPENGL_VERSION_MAJOR, CGP_OPENGL_VERSION_MINOR);

	// Display information
	// ***************************************************** //

	// Display window size
	std::cout << "\nWindow (" << window.width << "px x " << window.height << "px) created" << std::endl;
	std::cout << "Monitor: " << glfwGetMonitorName(window.monitor) << " - Resolution (" << window.screen_resolution_width << "x" << window.screen_resolution_height << ")\n" << std::endl;

	// Display debug information on command line
	std::cout << "OpenGL Information:" << std::endl;
	std::cout << cgp::opengl_info_display() << std::endl;

	// Initialize ImGUI
	// ***************************************************** //
	cgp::imgui_init(window.glfw_window);

	// Set the callback functions for the inputs
	glfwSetMouseButtonCallback(window.glfw_window, mouse_click_callback); // Event called when a button of the mouse is clicked/released
	glfwSetCursorPosCallback(window.glfw_window, mouse_move_callback);    // Event called when the mouse is moved
	glfwSetWindowSizeCallback(window.glfw_window, window_size_callback);  // Event called when the window is rescaled        
	glfwSetKeyCallback(window.glfw_window, keyboard_callback);            // Event called when a keyboard touch is pressed/released
	glfwSetScrollCallback(window.glfw_window, mouse_scroll_callback);     // Event called when scrolling the mouse

	return window;
}




// This function is called everytime the window is resized
void window_size_callback(GLFWwindow*, int width, int height)
{
	scene.window.width = width;
	scene.window.height = height;
}

// This function is called everytime the mouse is moved
void mouse_move_callback(GLFWwindow* /*window*/, double xpos, double ypos)
{
	vec2 const pos_relative = scene.window.convert_pixel_to_relative_coordinates({ xpos, ypos });
	scene.inputs.mouse.position.update(pos_relative);
	scene.mouse_move_event();
}

// This function is called everytime a mouse button is clicked/released
void mouse_click_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
{
	scene.inputs.mouse.click.update_from_glfw_click(button, action);
	scene.mouse_click_event();
}

// This function is called everytime the mouse is scrolled
void mouse_scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
{
	scene.inputs.mouse.scroll = yoffset;
	scene.mouse_scroll_event();
}

// This function is called everytime a keyboard touch is pressed/released
void keyboard_callback(GLFWwindow* /*window*/, int key, int, int action, int /*mods*/)
{
	scene.inputs.keyboard.update_from_glfw_key(key, action);
	scene.keyboard_event();

	// Press 'F' for full screen mode
	if (key == GLFW_KEY_F && action == GLFW_PRESS && scene.inputs.keyboard.shift) {
		scene.window.is_full_screen = !scene.window.is_full_screen;
		if (scene.window.is_full_screen)
			scene.window.set_full_screen();
		else
			scene.window.set_windowed_screen();
	}
	// Press 'V' for camera frame/view matrix debug
	if (key == GLFW_KEY_V && action == GLFW_PRESS && scene.inputs.keyboard.shift) {
		auto const camera_model = scene.camera_control.camera_model;
		std::cout << "\nDebug camera (position = " << str(camera_model.position()) << "):\n" << std::endl;
		std::cout << "  Frame matrix:" << std::endl;
		std::cout << str_pretty(camera_model.matrix_frame()) << std::endl;
		std::cout << "  View matrix:" << std::endl;
		std::cout << str_pretty(camera_model.matrix_view()) << std::endl;

	}

}



