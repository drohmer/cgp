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

	/*
	void skybox_drawable::set_texture_images(image_structure const& x_neg, image_structure const& x_pos, image_structure const& y_neg, image_structure const& y_pos, image_structure const& z_neg, image_structure const& z_pos)
	{
		// Send images to GPU as cubemap
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB8, x_neg.width, x_neg.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(x_neg.data));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB8, x_pos.width, x_pos.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(x_pos.data));

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB8, y_neg.width, y_neg.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(y_neg.data));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB8, y_pos.width, y_pos.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(y_pos.data));

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB8, z_neg.width, z_neg.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(z_neg.data));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB8, z_pos.width, z_pos.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(z_pos.data));


		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	*/

	//void cubemap_image_structure::load_from_single_image(std::string const& file_path)
	//{
	//	image_structure const image = image_load_file(file_path);

	//	int width = image.width / 4;
	//	int height = image.height / 3;

	//	x_neg = image.subimage(0, height, width, 2 * height);
	//	x_pos = image.subimage(2*width, height, 3*width, 2 * height);

	//	y_neg = image.subimage(width, height, 2 * width, 2 * height);
	//	y_pos = image.subimage(3*width, height, 4 * width, 2 * height);

	//	z_neg = image.subimage(width, 2*height, 2 * width, 3 * height);
	//	z_pos = image.subimage(width, 0, 2 * width, 2*height);

	//	image_save_jpg("test.jpg", image);

	//}

	void check_image_have_same_size(image_structure const& x_neg, image_structure const& x_pos, image_structure const& y_neg, image_structure const& y_pos, image_structure const& z_neg, image_structure const& z_pos)
	{
		if (!(x_neg.width > 0 && x_neg.height > 0)) {
			std::cout << "Error in loading Skybox image: Images should have non zero size" << std::endl;
			std::cout << "Current width=" << x_neg.width << ", Current height=" << x_neg.height << std::endl;
			abort();
		}

		bool const condition_square = (x_neg.width == x_neg.height);

		if (!condition_square) {
			std::cout << "Error in loading Skybox image: Images are expected to be squared" << std::endl;
			std::cout << "Current width=" << x_neg.width << ", Current height=" << x_neg.height << std::endl;
			abort();
		}

		bool const condition_width = (x_neg.width == y_neg.width) && (x_neg.width == z_neg.width) && (x_neg.width == x_pos.width) && (x_neg.width == y_pos.width) && (x_neg.width == z_pos.width);
		bool const condition_height = (x_neg.height == y_neg.height) && (x_neg.height == z_neg.height) && (x_neg.height == x_pos.height) && (x_neg.height == y_pos.height) && (x_neg.height == z_pos.height);

		if (!condition_width || !condition_height) {
			std::cout << "Error in loading Skybox image: All 6 images are expected to all have the same size" << std::endl;
			abort();
		}
	}
	/*
	void skybox_drawable::load_texture_from_multiple_files(std::string const& directory_path)
	{
		if (shader.id == 0) {
			std::cout << "Error, try to load texture in Skybox before its initialization" << std::endl;
			std::cout << "Please call skybox.initialize_data_on_gpu() before trying to load the texture" << std::endl;
			abort();
		}



		// Load images
		image_structure const x_neg = image_load_file(directory_path + "x_neg.jpg");
		image_structure const x_pos = image_load_file(directory_path + "x_pos.jpg");
		image_structure const y_neg = image_load_file(directory_path + "y_neg.jpg");
		image_structure const y_pos = image_load_file(directory_path + "y_pos.jpg");
		image_structure const z_neg = image_load_file(directory_path + "z_neg.jpg");
		image_structure const z_pos = image_load_file(directory_path + "z_pos.jpg");
		

		// check images have the same size
		check_image_have_same_size(x_neg, x_pos, y_neg, y_pos, z_neg, z_pos);

		// Send images to GPU as cubemap
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB8, x_neg.width, x_neg.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(x_neg.data));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB8, x_pos.width, x_pos.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(x_pos.data));

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB8, y_neg.width, y_neg.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(y_neg.data));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB8, y_pos.width, y_pos.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(y_pos.data));

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB8, z_neg.width, z_neg.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(z_neg.data));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB8, z_pos.width, z_pos.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(z_pos.data));


		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	}
	*/

	/*
	void skybox_drawable::load_texture(cubemap_image_structure const& cubemap_images)
	{
		if (shader.id == 0) {
			std::cout << "Error, try to load texture in Skybox before its initialization" << std::endl;
			std::cout << "Please call skybox.initialize_data_on_gpu() before trying to load the texture" << std::endl;
			abort();
		}


		int width = cubemap_images.x_neg.width;
		int height = cubemap_images.x_neg.height;

		// Send images to GPU as cubemap
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);



		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(cubemap_images.x_neg.data));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(cubemap_images.x_neg.data));

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(cubemap_images.x_neg.data));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(cubemap_images.x_neg.data));

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(cubemap_images.x_neg.data));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(cubemap_images.x_neg.data));


		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	*/

	void skybox_drawable::initialize_data_on_gpu()
	{
		shader.load_from_inline_text(skybox_vertex_shader, skybox_fragment_shader);
		mesh const cube_mesh = mesh_primitive_cube({ 0,0,0 }, 2.0f);
		
		std::cout << "\n  [info] Skybox texture loaded [ID="<<shader.id<<"]\n" << std::endl;


		// Check if this skybox_drawable is already initialized
		if (vao != 0 || vbo_position.size != 0) {
			std::cout << "Skybox already initialized" << std::endl;
			abort();
		}


		model = mat4::build_identity();


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

	


}