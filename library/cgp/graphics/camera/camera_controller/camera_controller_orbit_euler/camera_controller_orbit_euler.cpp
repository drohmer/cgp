
#include "cgp/core/base/base.hpp"
#include "camera_controller_orbit_euler.hpp"


namespace cgp
{


	void camera_controller_orbit_euler::update(mat4& camera_matrix_view)
	{
		camera_matrix_view = camera_model.matrix_view();
	}

	void camera_controller_orbit_euler::action_mouse_move(mat4& camera_matrix_view)
	{
		// Preconditions
		assert_cgp_no_msg(inputs != nullptr);
		assert_cgp_no_msg(window != nullptr);
		if (!is_active) return;

		vec2 const& p1 = inputs->mouse.position.current;
		vec2 const& p0 = inputs->mouse.position.previous;
		vec2 const dp = p1 - p0;

		bool const event_valid = !inputs->mouse.on_gui;
		bool const click_left = inputs->mouse.click.left;
		bool const click_right = inputs->mouse.click.right;
		bool const ctrl = inputs->keyboard.ctrl;

		if (event_valid) { // If the mouse cursor is not on the ImGui area

			if (click_left && !ctrl)     // Rotation of the camera around its center
				camera_model.manipulator_rotate_roll_pitch_yaw(0, dp.y, -dp.x);
			else if (click_left && ctrl) // Translate/Pan the camera in the viewspace plane
				camera_model.manipulator_translate_in_plane(p1 - p0);
			else if (click_right && !ctrl) // Move the camera closer/further with respect to its center
				camera_model.manipulator_scale_distance_to_center((p1 - p0).y);
			else if (click_right && ctrl) // Translate the camera center in front/back
				camera_model.manipulator_translate_front((p1 - p0).y);
		}

		update(camera_matrix_view);
	}
	void camera_controller_orbit_euler::idle_frame(mat4& camera_matrix_view)
	{
		// Preconditions
		assert_cgp_no_msg(inputs != nullptr);
		assert_cgp_no_msg(window != nullptr);
		if (!is_active) return;

		float const angle_magnitude = 2 * inputs->time_interval;
		if (inputs->keyboard.left) {
			camera_model.manipulator_twist_rotation_axis(angle_magnitude);
		}
		if (inputs->keyboard.right) {
			camera_model.manipulator_twist_rotation_axis(-angle_magnitude); 
		}

		update(camera_matrix_view);
	}

	void camera_controller_orbit_euler::set_rotation_axis(vec3 const& rotation_axis)
	{
		camera_model.set_rotation_axis(rotation_axis);
	}
	void camera_controller_orbit_euler::set_rotation_axis_x()
	{
		camera_model.set_rotation_axis({ 1,0,0 });
	}
	void camera_controller_orbit_euler::set_rotation_axis_y()
	{
		camera_model.set_rotation_axis({ 0,1,0 });
	}
	void camera_controller_orbit_euler::set_rotation_axis_z()
	{
		camera_model.set_rotation_axis({ 0,0,1 });
	}

	void camera_controller_orbit_euler::look_at(vec3 const& eye, vec3 const& center, vec3 const& )
	{
		camera_model.look_at(eye, center);
	}

	
}