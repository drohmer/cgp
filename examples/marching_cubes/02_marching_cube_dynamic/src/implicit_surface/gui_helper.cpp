#include "gui_helper.hpp"

using namespace cgp;

void display_gui_implicit_surface(bool& is_update_field, bool& is_update_marching_cube, bool& is_save_obj, gui_parameters& gui, field_function_structure& field_function)
{
	if (ImGui::CollapsingHeader("Display"))
	{
		ImGui::Checkbox("Frame", &gui.display.frame);
		ImGui::Checkbox("Surface", &gui.display.surface);
		ImGui::Checkbox("Wireframe", &gui.display.wireframe);
		ImGui::Checkbox("Domain", &gui.display.domain);
	}

	if (ImGui::CollapsingHeader("Domain"))
	{
		is_update_field |= ImGui::SliderInt("Samples", &gui.domain.samples, 8, 100);

		is_update_field |= ImGui::SliderFloat("Lx", &gui.domain.length.x, 0.5f, 10.0f);
		is_update_field |= ImGui::SliderFloat("Ly", &gui.domain.length.y, 0.5f, 10.0f);
		is_update_field |= ImGui::SliderFloat("Lz", &gui.domain.length.z, 0.5f, 10.0f);
	}

	if (ImGui::CollapsingHeader("Field Function"))
	{
		ImGui::Text("Position A");
		is_update_field |= ImGui::SliderFloat("Power A", &field_function.sa, 0.0f, 2.0f);
		is_update_field |= ImGui::SliderFloat("Pa-x", &field_function.pa.x, -2.0f, 2.0f);
		is_update_field |= ImGui::SliderFloat("Pa-y", &field_function.pa.y, -2.0f, 2.0f);
		ImGui::Spacing();
		ImGui::Text("Position B");
		is_update_field |= ImGui::SliderFloat("Power B", &field_function.sb, 0.0f, 2.0f);
		is_update_field |= ImGui::SliderFloat("Pb-x", &field_function.pb.x, -2.0f, 2.0f);
		is_update_field |= ImGui::SliderFloat("Pb-y", &field_function.pb.y, -2.0f, 2.0f);
		ImGui::Spacing();
		ImGui::Text("Position C");
		is_update_field |= ImGui::SliderFloat("Power C", &field_function.sc, 0.0f, 2.0f);
		is_update_field |= ImGui::SliderFloat("Pc-x", &field_function.pc.x, -2.0f, 2.0f);
		is_update_field |= ImGui::SliderFloat("Pc-y", &field_function.pc.y, -2.0f, 2.0f);
		ImGui::Spacing();
		ImGui::Text("Noise parameters");
		is_update_field |= ImGui::SliderFloat("Noise Power", &field_function.noise_magnitude, 0.0f, 1.0f);
		is_update_field |= ImGui::SliderFloat("Noise Scale", &field_function.noise_scale, 0.1f, 2.5f);
		is_update_field |= ImGui::SliderFloat("Noise offset", &field_function.noise_offset, -3, 3);
		is_update_field |= ImGui::SliderInt("Noise Octave", &field_function.noise_octave, 1, 8);
		is_update_field |= ImGui::SliderFloat("Noise Persistance", &field_function.noise_persistance, 0.1f, 0.5f);
	}

	ImGui::Spacing();
	is_update_marching_cube |= ImGui::SliderFloat("Isovalue", &gui.isovalue, 0.0f, 1.0f);

	ImGui::Spacing();
	is_save_obj = ImGui::Button("Export mesh as obj");

}