#include "obj_advanced.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "third_party/src/tinyobj/tiny_obj_loader.hpp"

namespace cgp
{
	namespace mesh_obj_advanced_loader
	{
		std::vector<cgp::mesh_drawable> convert_to_mesh_drawable(std::vector<shape_element_node> const& elements)
		{
			int N = elements.size();
			std::vector<mesh_drawable> drawables;
			drawables.resize(N);
			for (int k = 0; k < elements.size(); ++k)
			{
				mesh m = elements[k].mesh_element;
				m.fill_empty_field();
				drawables[k].initialize_data_on_gpu(m);
				drawables[k].texture = elements[k].texture_element;
			}
			return drawables;
		}
	}

	std::vector<mesh_obj_advanced_loader::shape_element_node> mesh_load_file_obj_advanced(std::string const& directory, std::string const& filename)
	{
		std::vector<mesh_obj_advanced_loader::shape_element_node> data;

		std::string inputfile = directory + filename; // project::path + "assets/StMaria/StMaria.obj";
		tinyobj::ObjReaderConfig reader_config;
		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(inputfile, reader_config)) {
			if (!reader.Error().empty()) {
				std::cerr << "TinyObjReader: " << reader.Error();
			}
			exit(1);
		}

		if (!reader.Warning().empty()) {
			std::cout << "TinyObjReader: " << reader.Warning();
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& materials = reader.GetMaterials();

		int N_material = materials.size();
		std::vector<opengl_texture_image_structure> texture_array;
		texture_array.resize(N_material);

		for (int k = 0; k < N_material; ++k) {
			std::string texture_filename = materials[k].diffuse_texname;
			if (texture_filename != "") {
				texture_array[k].load_and_initialize_texture_2d_on_gpu(directory + texture_filename, GL_REPEAT, GL_REPEAT);
			}
			else {
				texture_array[k] = mesh_drawable::default_texture;
			}
		}


		// Loop over shapes
		for (int shape_idx = 0; shape_idx < shapes.size(); shape_idx++)
		{


			// Loop over faces(polygon)
			int connectivity_counter = 0;
			size_t index_offset = 0;
			int idx_material_previous = -1;
			mesh mesh_current;
			for (size_t f = 0; f < shapes[shape_idx].mesh.num_face_vertices.size(); f++)
			{

				// per-face material
				int idx_material = shapes[shape_idx].mesh.material_ids[f];
				if (idx_material != idx_material_previous)
				{
					if (idx_material_previous != -1)
					{
						mesh_obj_advanced_loader::shape_element_node node;
						node.mesh_element = mesh_current;
						node.texture_element = texture_array[idx_material_previous];

						data.push_back(node);
						mesh_current = mesh();
						connectivity_counter = 0;

					}
					idx_material_previous = idx_material;
				}

				// Loop over vertices in the face.
				size_t fv = size_t(shapes[shape_idx].mesh.num_face_vertices[f]);
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[shape_idx].mesh.indices[index_offset + v];
					tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
					tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
					tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

					mesh_current.position.push_back({ vx, vy, vz });

					// Check if `normal_index` is zero or positive. negative = no normal data
					if (idx.normal_index >= 0) {
						tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
						tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
						tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

						mesh_current.normal.push_back({ nx, ny, nz });
					}


					// Check if `texcoord_index` is zero or positive. negative = no texcoord data
					if (idx.texcoord_index >= 0) {
						tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
						tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

						mesh_current.uv.push_back({ tx, ty });
					}

				}
				index_offset += fv;


				mesh_current.connectivity.push_back({ connectivity_counter, connectivity_counter + 1, connectivity_counter + 2 });
				connectivity_counter += 3;

				if (f == shapes[shape_idx].mesh.num_face_vertices.size() - 1) {
					mesh_obj_advanced_loader::shape_element_node node;
					node.mesh_element = mesh_current;
					node.texture_element = texture_array[idx_material];
					data.push_back(node);
				}
			}
		}


		return data;
	}
}