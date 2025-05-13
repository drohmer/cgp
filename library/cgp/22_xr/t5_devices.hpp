#pragma once

#include "cgp/cgp.hpp"
#include "t5/TiltFiveNative.hpp"

namespace cgp
{
    using Client = std::shared_ptr<tiltfive::Client>;
    using Glasses = std::shared_ptr<tiltfive::Glasses>;
    using Wand = std::shared_ptr<tiltfive::Wand>;

/****************************************************************************
Main application class
*****************************************************************************/
struct t5_devices
{	
	template<typename Scene> void display_frame(Scene& scene);
	bool initialize();

	struct Player {
		// Are these the right defaults? Values were pulled from other
		// source references
		int width = 1216;
		int height = 768;
		float fov = 48.0;

		std::string id;
		Client client;
		Glasses glasses;
		opengl_fbo_structure leftFramebuffer;
		opengl_fbo_structure rightFramebuffer;
		bool isPoseValid;
		// All the poses neeed to make up the view matrix
		affine_rt headPose; // Relative to game board pose
		affine_rt leftEyePose; // Relative to head pose
		affine_rt rightEyePose; // Relative to head pose
		T5_GameboardType gameboard_type = kT5_GameboardType_None;
		T5_CamImage images[10];

		Player(const std::string& id, const Client& client);
		template<typename Scene> void display_frame(Scene& scene);
		void update_gameboard(T5_GameboardType type);
		bool initialize();
		void update();
		void send_frame();
		void get_camera_frame();
	};

	// T5 handles
	Client client;
	
	// List of glasses ID we got from the T5 API
	std::vector<std::string> glassesIds;
	
	// Version number form the T5 service
	std::string serviceVersion;

	std::vector<Player> players;
	
	mesh_drawable gameboard_drawable[4];
	mesh_drawable glasses_drawable;

	template<typename Environment>
	void draw_gameboard(T5_GameboardType type, Environment& environment) const;

private:
	bool connect(const std::string& glassesID);
};

/****************************************************************************
Render the scene. The view matrix is composed from three transforms.
gameboardPose - represents the pose of the gameboard in world frame
headPose - represents the pose of the glasses/head in the gameboard frame
leftEyePose/righteyePose - represents the offset of the eye in head frame
A real application should be watching for glasses connection and disconnection
and handling those event. It should probably also be watching for parameter 
changes like IPD.
*****************************************************************************/
template<typename Scene>
void t5_devices::display_frame(Scene& scene)
{
	const mat4 camera_view = scene.environment.camera_view;
	const mat4 camera_projection = scene.environment.camera_projection;

	for(Player& player : players) {
		player.update();
		player.display_frame(scene);
	}

	scene.environment.camera_projection = camera_projection;
	scene.environment.camera_view = camera_view;
	glViewport(0, 0, scene.window.width, scene.window.height);

	scene.display_frame_to_bound_fbo();
	for(Player& player : players) {
		if(!player.isPoseValid) continue;
		glasses_drawable.model.translation = player.headPose.translation;
		glasses_drawable.model.rotation = player.headPose.rotation;
		draw(glasses_drawable, scene.environment);
		draw_gameboard(player.gameboard_type, scene.environment);
	}
}

template<typename Scene>
void t5_devices::Player::display_frame(Scene& scene)
{
	vec3 const& background_color = scene.environment.background_color;
	scene.environment.camera_projection = projection_perspective(fov * Pi/180, width / (float)height, 0.1f, 100.0f);
	
	scene.environment.camera_view = (headPose * leftEyePose).matrix().inverse_assuming_rigid_transform();
	leftFramebuffer.bind();
	glViewport(0, 0, width, height);
	glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	scene.display_frame_to_bound_fbo();
	leftFramebuffer.unbind();

	scene.environment.camera_view = (headPose * rightEyePose).matrix().inverse_assuming_rigid_transform();
	rightFramebuffer.bind();
	glViewport(0, 0, width, height);
	glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	scene.display_frame_to_bound_fbo();
	rightFramebuffer.unbind();

	send_frame();
}

template<typename Environment>
void t5_devices::draw_gameboard(T5_GameboardType type, Environment& environment) const {
		draw(gameboard_drawable[type - kT5_GameboardType_None], environment);
}

}
