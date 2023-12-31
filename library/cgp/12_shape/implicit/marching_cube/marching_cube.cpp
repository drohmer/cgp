#include "marching_cube.hpp"

#include "cgp/09_geometric_transformation/interpolation/interpolation.hpp"
#include "helper/marching_cubes_lut.hpp"
#include <unordered_map>

namespace cgp
{

	// Helper structure to hash a pair of index (a,b) as a unique number
	struct hash_edge {
		size_t operator()(std::pair<int, int> const& key) const;
	};

	// Helper structure to store voxels information
	struct cube_parameters {
		std::array<size_t, 8> index;
		std::array<float, 8>  value;
		std::array<vec3, 8>   position;
	};


	mesh marching_cube(grid_3D<float> const& field, spatial_domain_grid_3D const& domain, float iso)
	{
		assert_cgp_no_msg(is_equal(field.dimension, domain.samples));

		// Compute the marching cube
		std::vector<vec3> position;
		std::vector<marching_cube_relative_coordinates> relative;
		int const N = marching_cube(position, field.data.data, domain, iso, &relative);


		// Compute the mesh with non-duplicated vertices
		mesh m;
		int const N_triangle = N / 3;

		// Storage avoiding to duplicate vertices at the same position if they are on the same voxel edge
		std::unordered_map<std::pair<int, int>, int, hash_edge> unique_edge;

		for (int k_tri = 0; k_tri < N_triangle; ++k_tri) {

			uint3 triangle_index;
			for (int k = 0; k < 3; ++k) {

				int const idx = 3 * k_tri + k;

				// The indices of the edge of the current vertex
				std::pair<int, int> const edge = { relative[3 * k_tri+k].k0, relative[3 * k_tri+k].k1 };

				// Check if this edge has already been encountered
				auto const it = unique_edge.find(edge);

				// if it is a new edge, add this vertex as a new one in the mesh
				if (it == unique_edge.end()) {
					int const idx_new_vertex = m.position.size(); // create a new vertex index
					triangle_index[k] = idx_new_vertex; // set the current index to the triangle connectivity
					m.position.push_back(position[idx]); // add this vertex in the mesh
					unique_edge[edge] = idx_new_vertex; // add the edge in the unique structure for further check
				}
				// if the edge has already been encountered - only reuse the index in the connectivity but do not add the vertex again
				else {
					int const idx_existing_vertex = it->second;   // the index that has already be defined
					triangle_index[k] = idx_existing_vertex;      // set this existing index in the triangle connectivity
				}
			}
			m.connectivity.push_back(triangle_index); // add the new triangle
			
			
		}

		m.fill_empty_field();
		return m;
	}


	size_t hash_edge::operator()(std::pair<int, int> const& key) const {
		return std::hash<int>{}(key.first) ^ std::hash<int>{}(key.second);
	}


	void fill_position(vec3& cube_position, float ux, float uy, float uz, vec3 const& domain_min, vec3 const& domain_length)
	{
		cube_position.x = domain_min.x + ux * domain_length.x;
		cube_position.y = domain_min.y + uy * domain_length.y;
		cube_position.z = domain_min.z + uz * domain_length.z;
	}

	void interpolate_position_on_edge(vec3& p, float& alpha, int idx0, int idx1, std::array<vec3, 8> const& cube_position, std::array<float, 8> const& cube_value)
	{
		vec3 const& p0 = cube_position[idx0];
		vec3 const& p1 = cube_position[idx1];

		float const v0 = cube_value[idx0];
		float const v1 = cube_value[idx1];

		alpha = (0 - v0) / (v1 - v0); // iso is expected to be 0

		p.x = (1 - alpha) * p0.x + alpha * p1.x;
		p.y = (1 - alpha) * p0.y + alpha * p1.y;
		p.z = (1 - alpha) * p0.z + alpha * p1.z;
	}



	size_t marching_cube(std::vector<vec3>& position, std::vector<float> const& field, spatial_domain_grid_3D const& domain, float iso, std::vector<marching_cube_relative_coordinates>* relative)
	{
		// Table of correspondance between the 256 type of cube and the edges on which new vertices are created
		static std::array<std::array<int, 16>, 256> const triTable = marching_cube_lut_triTable();
		// Storage of the order of edge visiting on the cube
		static std::array<std::pair<int, int>, 12> const lut_edge_order = marching_cube_lut_edge_order();
		static std::array<int, 256> const edgeTable = marching_cube_lut_edgeTable();

		vec3 const domain_min = domain.center - domain.length / 2.0;
		vec3 const& domain_length = domain.length;

		size_t const Nx = domain.samples.x;
		size_t const Ny = domain.samples.y;
		size_t const Nz = domain.samples.z;

		float const dx = 1 / (Nx - 1.0f);
		float const dy = 1 / (Ny - 1.0f);
		float const dz = 1 / (Nz - 1.0f);

		size_t counter_position = 0;

		// Marching-Cube
		// *************************** //
		cube_parameters cube;
		std::array<vec3, 12> new_vertex;
		std::array<float, 12> new_vertex_alpha;

		std::array<size_t, 8> const offset_cube = { 0, 1, 1+Nx, Nx, Nx*Ny, 1+Nx*Ny, 1+Nx+Nx*Ny, Nx+Nx*Ny };

		bool exist_cube_value_positive;
		bool exist_cube_value_negative;
		for (size_t kz = 0; kz < Nz - 1; ++kz) {
			float const uz = kz * dz;
			for (size_t ky = 0; ky < Ny - 1; ++ky) {
				float const uy = ky * dy;
				for (size_t kx = 0; kx < Nx - 1; ++kx) {
					float const ux = kx * dx;

					size_t const index_corner = kx + Nx * (ky + Ny * kz);
				
					// compute offsets of the cube vertices
					for (size_t k_offset = 0; k_offset < 8; ++k_offset)
						cube.index[k_offset] = index_corner + offset_cube[k_offset];

					// get values
					for (size_t k = 0; k < 8; ++k)
						cube.value[k] = field[cube.index[k]] - iso;

					// check if there is at least one change of sign in the vertices
					exist_cube_value_positive = false;
					exist_cube_value_negative = false;
					for (size_t k = 0; k < 8; ++k) {
						if (cube.value[k] >= 0) exist_cube_value_positive = true;
						if (cube.value[k] <  0) exist_cube_value_negative = true;
					}

					// Only pursue if there is a change of sign
					if (exist_cube_value_positive && exist_cube_value_negative) {

						// Set the type of cube
						int type = 0;
						if (cube.value[0] < 0) type |= 1;
						if (cube.value[1] < 0) type |= 2;
						if (cube.value[2] < 0) type |= 4;
						if (cube.value[3] < 0) type |= 8;
						if (cube.value[4] < 0) type |= 16;
						if (cube.value[5] < 0) type |= 32;
						if (cube.value[6] < 0) type |= 64;
						if (cube.value[7] < 0) type |= 128;

						// 3D positions of the cube vertices
						fill_position(cube.position[0], ux   , uy   , uz   , domain_min, domain_length);
						fill_position(cube.position[1], ux+dx, uy   , uz   , domain_min, domain_length);
						fill_position(cube.position[2], ux+dx, uy+dy, uz   , domain_min, domain_length);
						fill_position(cube.position[3], ux   , uy+dy, uz   , domain_min, domain_length);
						fill_position(cube.position[4], ux   , uy   , uz+dz, domain_min, domain_length);
						fill_position(cube.position[5], ux+dx, uy   , uz+dz, domain_min, domain_length);
						fill_position(cube.position[6], ux+dx, uy+dy, uz+dz, domain_min, domain_length);
						fill_position(cube.position[7], ux   , uy+dy, uz+dz, domain_min, domain_length);


						// Compute vertex at the intersection
						if (edgeTable[type] &    1) 
							interpolate_position_on_edge(new_vertex[0], new_vertex_alpha[0], 0, 1, cube.position, cube.value);
						if (edgeTable[type] &    2)
							interpolate_position_on_edge(new_vertex[1], new_vertex_alpha[1], 1, 2, cube.position, cube.value);
						if (edgeTable[type] &    4)
							interpolate_position_on_edge(new_vertex[2], new_vertex_alpha[2], 2, 3, cube.position, cube.value);
						if (edgeTable[type] &    8)
							interpolate_position_on_edge(new_vertex[3], new_vertex_alpha[3], 3, 0, cube.position, cube.value);
						if (edgeTable[type] &   16)
							interpolate_position_on_edge(new_vertex[4], new_vertex_alpha[4], 4, 5, cube.position, cube.value);
						if (edgeTable[type] &   32)
							interpolate_position_on_edge(new_vertex[5], new_vertex_alpha[5], 5, 6, cube.position, cube.value);
						if (edgeTable[type] &   64)
							interpolate_position_on_edge(new_vertex[6], new_vertex_alpha[6], 6, 7, cube.position, cube.value);
						if (edgeTable[type] &  128)
							interpolate_position_on_edge(new_vertex[7], new_vertex_alpha[7], 7, 4, cube.position, cube.value);
						if (edgeTable[type] &  256)
							interpolate_position_on_edge(new_vertex[8], new_vertex_alpha[8], 0, 4, cube.position, cube.value);
						if (edgeTable[type] &  512)
							interpolate_position_on_edge(new_vertex[9], new_vertex_alpha[9], 1, 5, cube.position, cube.value);
						if (edgeTable[type] & 1024)
							interpolate_position_on_edge(new_vertex[10], new_vertex_alpha[10], 2, 6, cube.position, cube.value);
						if (edgeTable[type] & 2048)
							interpolate_position_on_edge(new_vertex[11], new_vertex_alpha[11], 3, 7, cube.position, cube.value);



						// Construct the new triangles
						for (size_t k = 0; triTable[type][k] != -1; k += 3) { // read the table of correspondance for the triangle

							vec3 const& p0 = new_vertex[triTable[type][k  ]];
							vec3 const& p1 = new_vertex[triTable[type][k+1]];
							vec3 const& p2 = new_vertex[triTable[type][k+2]];

							if (position.size() < counter_position + 3)
								position.resize(1.5 * (counter_position + 3));
							

							position[counter_position] = p0;
							position[counter_position + 1] = p1;
							position[counter_position + 2] = p2;
							

							if (relative != nullptr) {
								if (relative->size() < counter_position + 3) 
									relative->resize(1.5 * (counter_position + 3));
								
								int const idx0 = triTable[type][k];
								(*relative)[counter_position].alpha = new_vertex_alpha[idx0];
								(*relative)[counter_position].k0 = cube.index[lut_edge_order[idx0].first];
								(*relative)[counter_position].k1 = cube.index[lut_edge_order[idx0].second];

								int const idx1 = triTable[type][k+1];
								(*relative)[counter_position+1].alpha = new_vertex_alpha[idx1];
								(*relative)[counter_position+1].k0 = cube.index[lut_edge_order[idx1].first];
								(*relative)[counter_position+1].k1 = cube.index[lut_edge_order[idx1].second];

								int const idx2 = triTable[type][k+2];
								(*relative)[counter_position+2].alpha = new_vertex_alpha[idx2];
								(*relative)[counter_position+2].k0 = cube.index[lut_edge_order[idx2].first];
								(*relative)[counter_position+2].k1 = cube.index[lut_edge_order[idx2].second];
							}

							counter_position += 3;

						}

					}

				}
			}
		}

		return counter_position;

	}
}