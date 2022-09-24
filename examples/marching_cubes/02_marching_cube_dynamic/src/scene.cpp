#include "scene.hpp"


using namespace cgp;




void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_y();
	camera_control.look_at({ 3.0f, 2.0f, 2.0f }, {0,0,0}, {0,0,1});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	// Initialization for the Implicit Surface
	// ***************************************** //

	implicit_surface.set_domain(gui.domain.samples, gui.domain.length);
	implicit_surface.update_field(field_function, gui.isovalue);
}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	// The standard frame
	if (gui.display.frame)
		draw(global_frame, environment);

	if (gui.display.surface)    // Display the implicit surface
		draw(implicit_surface.drawable_param.shape, environment);

	if (gui.display.wireframe) // Display the wireframe of the implicit surface
		draw_wireframe(implicit_surface.drawable_param.shape, environment, { 0,0,0 });

	if (gui.display.domain)    // Display the boundary of the domain
		draw(implicit_surface.drawable_param.domain_box, environment);

}

void scene_structure::display_gui()
{
	// Handle the gui values and the updates using the helper methods (*)
	implicit_surface.gui_update(gui, field_function);
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

