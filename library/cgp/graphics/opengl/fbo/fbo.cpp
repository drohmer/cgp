#include "fbo.hpp"

#include "cgp/core/core.hpp"


namespace cgp{

	void opengl_fbo_structure::initialize() {

		width = 640;
		height = 480;

		// Initialize texture
		texture.initialize_texture_2d_on_gpu(width, height, GL_RGB8, GL_TEXTURE_2D);

		// Allocate a depth buffer - need to do it when using the frame buffer
		glGenRenderbuffers(1, &depth_buffer_id);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, max_width, max_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// Create frame buffer
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		// associate the texture to the frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);
		// associate the depth-buffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_id);
		// Reset the standard framebuffer to output on the screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void opengl_fbo_structure::start(vec3 const& background_color) const {

		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glViewport(0, 0, width, height);
		texture.bind();


		glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

	}
	
	void opengl_fbo_structure::stop(vec3 const& background_color) const {

		// quit the framebuffer
		texture.unbind();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		

		// Goes to standard rendering setup
		glViewport(0, 0, width, height);
		glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

	}

	void opengl_fbo_structure::update_screen_size(int new_width, int new_height) {

		if (width != new_width || height != new_height) 
		{
			width = new_width;
			height = new_height;

			glBindTexture(GL_TEXTURE_2D, texture.id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}

}