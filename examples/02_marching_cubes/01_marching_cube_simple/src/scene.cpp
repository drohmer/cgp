#include "scene.hpp"
#include "field_function/field_function.hpp"

using namespace cgp;




void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_y();
	camera_control.look_at({ 3.0f, 2.0f, 2.0f }, {0,0,0}, {0,0,1});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	// Create the Implicit Surface
	// ***************************************** //

	// Number of voxels
	int3 const samples = { 50, 50, 50 };
	// Dimension of the domain
	vec3 const length = { 5,3,3 };
	// Isovalue for the marching cube
	float isovalue = 0.5f;

	// Initialize the spatial domain and the field
	spatial_domain_grid_3D domain = spatial_domain_grid_3D::from_center_length({ 0,0,0 }, length, samples);
	grid_3D<float> field = compute_scalar_field(domain);

	// Compute the mesh using marching cube 
	mesh meshed_surface = marching_cube(field, domain, isovalue);
	visual_surface.initialize_data_on_gpu(meshed_surface);

	// Helper to visualize the box of the domain
	domain_box.display_type = curve_drawable_display_type::Segments;
	domain_box.initialize_data_on_gpu(domain.export_segments_for_drawable_border());
}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	// The standard frame
	if (gui.display_frame)
		draw(global_frame, environment);

	if (gui.display_plain)    // Display the implicit surface
		draw(visual_surface, environment);

	if (gui.display_wireframe) // Display the wireframe of the implicit surface
		draw_wireframe(visual_surface, environment, { 0,0,0 });

	if (gui.display_domain)    // Display the boundary of the domain
		draw(domain_box, environment);

}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
	ImGui::Checkbox("Plain", &gui.display_plain);
	ImGui::Checkbox("Domain", &gui.display_domain);
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

