#include "scene.hpp"


using namespace cgp;




void scene_structure::initialize()
{
	// Uncomment this line if using an orthographic projection
	// camera_projection = camera_projection_orthographic{ -1.1f, 1.1f, -1.1f, 1.1f, -10, 10, window.aspect_ratio() };

	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_y();
	camera_control.look_at({ 0.0f, 0.0f, 3.0f }, { 0,0,0 }, { 0,1,0 });
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

}


// Compute a 3D position of a 2D position given by its screen coordinates for orthographic projection
vec3 unproject(camera_projection_orthographic const& P, mat4 const& camera_view_inverse, vec2 const& p_screen)
{
	// Simple un-project assuming that the viewpoint is an orthogonal projection
	vec4 const p_proj = camera_view_inverse * P.matrix_inverse() * vec4(p_screen, 0.5f, 1.0f);
	return p_proj.xyz();
}

// Compute a 3D position of a 2D position given by its screen coordinates for perspective projection
vec3 unproject(camera_projection_perspective const& P, mat4 const& camera_view_inverse, vec2 const& p_screen)
{
	// Simple un-project assuming that the viewpoint is an orthogonal projection
	vec4 const p_proj = camera_view_inverse * P.matrix_inverse() *  vec4(p_screen, 0.5f, 1.0f);
	return p_proj.xyz()/p_proj.w;
}

void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	for (int k = 0; k < sketch_drawable.size(); ++k)
		draw(sketch_drawable[k], environment);
	
	if (!gui.sketch_mode) {
		draw(global_frame, environment);
	}

}

void scene_structure::display_gui()
{
	bool cancel = ImGui::Button("Cancel last stroke");
	if (cancel)
	{
		// remove last stroke
		int const N_stroke = sketch_drawable.size();
		if (N_stroke > 0) {
			sketch_drawable[N_stroke - 1].clear();
			sketch_drawable.resize(N_stroke - 1);
		}
	}

	ImGui::Checkbox("Sketch Mode", &gui.sketch_mode);
}

void scene_structure::mouse_move_event()
{
	if (gui.sketch_mode) {
		if (inputs.mouse.click.left) {
			// Add the new clicked position
			int k_sketch = sketch_drawable.size() - 1;
			vec3 const p = unproject(camera_projection, camera_control.camera_model.matrix_frame(), inputs.mouse.position.current);
			sketch_drawable[k_sketch].push_back(p);
		}
	}
	else {
		if (!inputs.keyboard.shift)
			camera_control.action_mouse_move(environment.camera_view);
	}
}
void scene_structure::mouse_click_event()
{
	if (!inputs.mouse.on_gui) {
		if (gui.sketch_mode && inputs.mouse.click.last_action == last_mouse_cursor_action::click_left)
		{
			// Create new stroke (curve_dynamic_drawable)
			int k_sketch = sketch_drawable.size();
			sketch_drawable.push_back(curve_drawable_dynamic_extend());
			sketch_drawable[k_sketch].initialize_data_on_gpu();

			// Add the new clicked position
			vec3 const p = unproject(camera_projection, camera_control.camera_model.matrix_frame(), inputs.mouse.position.current);
			sketch_drawable[k_sketch].push_back(unproject(camera_projection, camera_control.camera_model.matrix_frame(), inputs.mouse.position.current));
		}
	}
}

void scene_structure::keyboard_event()
{
	camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
}

