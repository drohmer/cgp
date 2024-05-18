#include "fbo.hpp"

#include "cgp/01_base/base.hpp"


namespace cgp{

	void opengl_fbo_structure::initialize() {

		width = 800;
		height = 800;

		if(mode == opengl_fbo_mode::image) {

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
		}
		else if(mode == opengl_fbo_mode::depth) {

			// Initialize texture
			texture.initialize_texture_2d_on_gpu(width, height, GL_DEPTH_COMPONENT, GL_TEXTURE_2D,GL_REPEAT,GL_REPEAT,GL_NEAREST,GL_NEAREST);
			float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			// Create frame buffer
			glGenFramebuffers(1, &id);
			glBindFramebuffer(GL_FRAMEBUFFER, id);
			// associate the texture to the frame buffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.id, 0);

			// No color buffer is used in depth mode
			#ifndef __EMSCRIPTEN__
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			#endif
		}

		// Reset the standard framebuffer to output on the screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void opengl_fbo_structure::bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		texture.bind();
	}
	
	void opengl_fbo_structure::unbind() const {
		texture.unbind();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}



	void opengl_fbo_structure::update_screen_size(int new_width, int new_height) {

		if (width != new_width || height != new_height) 
		{
			width = new_width;
			height = new_height;

			glBindTexture(GL_TEXTURE_2D, texture.id);
			if(mode==opengl_fbo_mode::image)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			else if(mode==opengl_fbo_mode::depth)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}

}