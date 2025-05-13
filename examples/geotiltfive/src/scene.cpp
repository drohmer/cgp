#include "scene.hpp"
#include "shp.hpp"


using namespace cgp;


void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({ 0.0f, -0.6f, 0.25f }, {0,0,0}, {0,0,1});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame({}, 0.1));
	t5.initialize();

	cone.initialize_data_on_gpu(mesh_primitive_cone());
	numarray<polygon> poly_continents = polygon_load_file_shp("assets/GameOfThrones/continents.shp");
	numarray<polygon> poly_islands = polygon_load_file_shp("assets/GameOfThrones/islands.shp");
	numarray<polygon> poly_lakes = polygon_load_file_shp("assets/GameOfThrones/lakes.shp");
	numarray<polygon> poly_landscape = polygon_load_file_shp("assets/GameOfThrones/landscape.shp");
	numarray<polygon> poly_political = polygon_load_file_shp( "assets/GameOfThrones/political.shp");
	numarray<polygon> poly_regions = polygon_load_file_shp( "assets/GameOfThrones/regions.shp");

	// polylines
	numarray<polygon> poly_rivers = polygon_load_file_shp("assets/GameOfThrones/rivers.shp");
	numarray<polygon> poly_roads = polygon_load_file_shp("assets/GameOfThrones/roads.shp");
	numarray<polygon> poly_wall = polygon_load_file_shp("assets/GameOfThrones/wall.shp");

	// points_load_file_shp parses as shp file and returns numarray of points
	locations = points_load_file_shp("assets/GameOfThrones/locations.shp");
	polygons_to_curves(curves, poly_continents, { 0,0,0 });
	polygons_to_curves(curves, poly_islands, { 0.5,0.5,0.1 });
	polygons_to_curves(curves, poly_lakes, { 0.5,0.5,1 });
	polygons_to_curves(curves, poly_landscape, { 1.,0.,0. });
	polygons_to_curves(curves, poly_political, {0.5,0.5,1});
	polygons_to_curves(curves, poly_regions, {0.9,0.9,0.9});

	polygons_to_curves(curves, poly_rivers, { 0,0,1 });
	polygons_to_curves(curves, poly_roads, { 0.5,0,0 });
	polygons_to_curves(curves, poly_wall, { 0.1,0.1,0.1 });

	islands.initialize_data_on_gpu(polygons_to_mesh(poly_islands));
	islands.material.color = { 0.5,0.5,1. };

	continents.initialize_data_on_gpu(polygons_to_mesh(poly_continents));
	continents.material.color = { 0.8,1,0.8 };
}



void scene_structure::display_frame()
{
 	t5.display_frame(*this);
}

void scene_structure::display_frame_to_bound_fbo()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	vec3 center(25,0,0);
	float scaling = 0.02;
	vec3 translation = -scaling * center;


	if (gui.display_frame)
		draw(global_frame, environment);
	
	glPolygonOffset(0.0, 0.0);
	islands.model.translation = translation;
	islands.model.scaling = scaling;
	continents.model.translation = translation;
	continents.model.scaling = scaling;
	draw(islands, environment);
	draw(continents, environment);
	for (unsigned int i = 0; i < locations.size(); ++i)
	{
		cone.model.translation = scaling*locations[i]+translation;
		cone.model.scaling = 0.3*scaling;
		cone.model.scaling_xyz = { 1, 1, 10 };
		draw(cone, environment);
	}
	glPolygonOffset(10.0, 10.0);
	for (curve_drawable& curve : curves)
	{
		curve.model.translation = translation;
		curve.model.scaling = scaling;
		draw(curve, environment);
	}

	if (gui.display_wireframe) {
		draw_wireframe(islands, environment);
		draw_wireframe(continents, environment);
	}
}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
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

