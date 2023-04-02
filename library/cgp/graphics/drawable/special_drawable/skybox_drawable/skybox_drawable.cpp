#include "skybox_drawable.hpp"

#include "cgp/geometry/shape/mesh/mesh.hpp"

namespace cgp {

	static const std::string skybox_fragment_shader = R"(
		#version 330 core
		in struct fragment_data
		{
			vec3 position;
		} fragment;

		layout(location=0) out vec4 FragColor;

		uniform samplerCube image_skybox;

		void main()
		{
			// Apply a symetry on x to preserve the orientation of the image
			//   (left handed convention system on skybox)
			vec3 p = fragment.position * vec3(1.0, 1.0, -1.0);

			FragColor = texture(image_skybox, p);
		}
		)";

	static const std::string skybox_vertex_shader = R"(
		#version 330 core
		layout (location = 0) in vec3 position;

		out struct fragment_data
		{
			vec3 position;
		} fragment;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			fragment.position = position.xyz;

			mat4 modelView = mat4(mat3(view*model)); // remove the translational part
			gl_Position = projection * modelView * vec4(position, 1.0);
		}
		)";








	void skybox_drawable::initialize_data_on_gpu()
	{
		shader.load_from_inline_text(skybox_vertex_shader, skybox_fragment_shader);
		mesh const cube_mesh = mesh_primitive_cube({ 0,0,0 }, 2.0f);
		
		std::cout << "\n  [info] Skybox shader loaded [ID="<<shader.id<<"]\n" << std::endl;


		// Check if this skybox_drawable is already initialized
		if (vao != 0 || vbo_position.size != 0) {
			std::cout << "Skybox already initialized" << std::endl;
			abort();
		}

		model = affine();


		// Send the data to the GPU
		// ******************************************** //
		vbo_position.initialize_data_on_gpu(cube_mesh.position);
		ebo_connectivity.initialize_data_on_gpu(cube_mesh.connectivity);


		// Generate VAO
		glGenVertexArrays(1, &vao); opengl_check;
		glBindVertexArray(vao); opengl_check;
		opengl_set_vao_location(vbo_position, 0);
		glBindVertexArray(0); opengl_check;
		
	}

	void draw(skybox_drawable const& drawable, environment_generic_structure const& environment)
	{
		opengl_check;
		// Initial clean check
		// ********************************** //
		// If there is not vertices or not triangles, returns
		//  (no error + does not display anything)
		if (drawable.vbo_position.size == 0 || drawable.ebo_connectivity.size == 0) {
			std::cout << "Problem in Skybox drawable" << std::endl;;
			return;
		}

		assert_cgp(drawable.shader.id != 0, "Try to draw skybox_drawable without shader ");
		assert_cgp(!glIsShader(drawable.shader.id), "Try to draw skybox_drawable with incorrect shader ");
		assert_cgp(drawable.texture.id != 0, "Try to draw skybox_drawable without texture ");

		// Set the current shader
		// ********************************** //
		glUseProgram(drawable.shader.id); opengl_check;

		// Send uniforms for this shader
		// ********************************** //

		// send the uniform values for the model and material of the mesh_drawable
		opengl_uniform(drawable.shader, "model", drawable.model.matrix());

		// send the uniform values for the environment
		environment.send_opengl_uniform(drawable.shader);

		// Set textures
		// ********************************** //
		glActiveTexture(GL_TEXTURE0); opengl_check;
		drawable.texture.bind();
		opengl_uniform(drawable.shader, "image_skybox", 0);  opengl_check;

		// Draw call
		// ********************************** //
		glBindVertexArray(drawable.vao);   opengl_check;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable.ebo_connectivity.id); opengl_check;
		glDrawElements(GL_TRIANGLES, GLsizei(drawable.ebo_connectivity.size * 3), GL_UNSIGNED_INT, nullptr); opengl_check;



		// Clean state
		// ********************************** //
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0); opengl_check;
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glUseProgram(0);
	}

	


}