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
			glGenRenderbuffers(1, &depth_buffer_id); opengl_check;
			glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_id); opengl_check;
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height); opengl_check;
			glBindRenderbuffer(GL_RENDERBUFFER, 0); opengl_check;

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
			texture.width = width;
			texture.height = height;
			texture.format = GL_DEPTH_COMPONENT;
			texture.texture_type = GL_TEXTURE_2D;
			glGenTextures(1, &texture.id); opengl_check;
			glBindTexture(GL_TEXTURE_2D, texture.id); opengl_check;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr); opengl_check;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); opengl_check;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); opengl_check;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); opengl_check;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); opengl_check;
			glBindTexture(GL_TEXTURE_2D, 0); opengl_check;

			

			//float borderColor[] = { 1.0, 1.0, 1.0, 1.0 }; opengl_check;
			//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); opengl_check;

			// Create frame buffer
			glGenFramebuffers(1, &id); opengl_check;
			glBindFramebuffer(GL_FRAMEBUFFER, id); opengl_check;
			// associate the texture to the frame buffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.id, 0); opengl_check;

			// No color buffer is used in depth mode
			#ifndef __EMSCRIPTEN__
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			#else
			opengl_texture_image_structure unused_texture;
			unused_texture.initialize_texture_2d_on_gpu(width, height,  GL_RGB8, GL_TEXTURE_2D);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, unused_texture.id, 0);
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

			if(mode==opengl_fbo_mode::image){
				glBindTexture(GL_TEXTURE_2D, texture.id);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
				glBindTexture(GL_TEXTURE_2D, 0);

				glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_id);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
				opengl_check;
			}
			else if(mode==opengl_fbo_mode::depth){
				glBindTexture(GL_TEXTURE_2D, texture.id);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
				glBindTexture(GL_TEXTURE_2D, 0);
				opengl_check;
			}

			
		}
		

	}

}