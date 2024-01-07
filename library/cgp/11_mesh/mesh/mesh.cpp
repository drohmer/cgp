#include "mesh.hpp"



#include <set>

namespace cgp
{
	mesh& mesh::fill_empty_field()
	{
		size_t const N = position.size();

		if (N == 0) {
			warning_cgp("Trying to fill field of a mesh that has 0 vertex","");
			return *this;
		}
		
		assert_cgp_no_msg(N>0);
		assert_cgp(connectivity.size()>0, "Connectivity doesn't have any triangle");

		if(normal.size()<N)
			normal = normal_per_vertex(position, connectivity);
		if(color.size()<N)
			color.resize(N).fill(vec3{1.0f, 1.0f, 1.0f});
		if(uv.size()<N)
			uv.resize(N).fill(vec2{0.0f, 0.0f});

		return *this;
	}

	mesh& mesh::push_back(mesh const& to_add)
	{
		unsigned int const N_vertex = static_cast<unsigned int>(position.size());

		position.push_back(to_add.position);
		normal.push_back(to_add.normal);
		color.push_back(to_add.color);
		uv.push_back(to_add.uv);


		for(auto const& tri : to_add.connectivity)
			connectivity.push_back( tri + uint3{N_vertex,N_vertex,N_vertex} );

		return *this;
	}


	void normal_per_vertex(numarray<vec3> const& position, numarray<uint3> const& connectivity, numarray<vec3>& normals, bool invert)
	{
		size_t const N = position.size();
		if(normals.size()!=N)
			normals.resize(N);
		else
			normals.fill(vec3{0,0,0});

		size_t const N_tri = connectivity.size();
		for (size_t k_tri = 0; k_tri < N_tri; ++k_tri)
		{
			uint3 const& face = connectivity.at(k_tri);

			//sanity check
			assert_cgp_no_msg(get<0>(face)<N);
			assert_cgp_no_msg(get<1>(face)<N);
			assert_cgp_no_msg(get<2>(face)<N);

			vec3 const& p0 = position.at(get<0>(face));
			vec3 const& p1 = position.at(get<1>(face));
			vec3 const& p2 = position.at(get<2>(face));

			// compute normal of the triangle
			vec3 const p10 = p1-p0;
			vec3 const p20 = p2-p0;

			float const L10 = norm(p10);
			float const L20 = norm(p20);

			// Add the normal direction to all vertices of this triangle
			//  (only if the triangle is not degenerated: norm of edges>0, edges not aligned)
			if (L10 > 1e-6f && L20 > 1e-6f)
			{
				vec3 const n = cross(p10/L10, p20/L20);
				float const Ln = norm(n);

				if (Ln > 1e-6f)
				{
					vec3 const n_unit = n/Ln;
					for(unsigned int idx : face)
						normals.at(idx) += n_unit;
				}


			}
		}

		// Normalize all normals
		for (size_t k = 0; k < N; ++k)
		{
			vec3& n = normals.at(k);
			float const L = norm(n);
			if(L>1e-6f)
				n /= L;
		}

		// Invert normals if asked
		if(invert) for(auto& n : normals) n = -n;

			
	}
	numarray<vec3> normal_per_vertex(numarray<vec3> const& position, numarray<uint3> const& connectivity, bool invert)
	{
		numarray<vec3> normals;
		normal_per_vertex(position, connectivity, normals, invert);
		return normals;
	}

	bool mesh_check(mesh const& m)
	{
		bool ok = true;
		std::string const warning = "Warning [mesh_check]: ";

		size_t const N = m.position.size();
		if (N == 0)
		{std::cout<<warning+"Current mesh has 0 position"<<std::endl; ok =false;}
		if (N>10000000)
		{std::cout<<warning+"Current mesh has more than 10 millions positions"<<std::endl; ok=false;}

		if (m.normal.size()==0)
		{std::cout<<warning+"Mesh doesn't have any per-vertex normal defined"<<std::endl; ok=false;}
		if (m.normal.size()!=N)
		{std::cout<<warning+"Mesh has incoherent size of per-vertex normal"<<std::endl; ok=false;}

		if(m.uv.size()==0)
		{std::cout<<warning+"Mesh doesn't have any per-vertex uv defined"<<std::endl; ok=false;}
		if(m.uv.size()!=N)
		{std::cout<<warning+"Mesh has incoherent size of per-vertex uv"<<std::endl; ok=false;}
		
		if(m.color.size()==0)
		{std::cout<<warning+"Mesh doesn't have any per-vertex color defined"<<std::endl; ok=false;}
		if(m.color.size()!=N)
		{std::cout<<warning+"Mesh has incoherent size of per-vertex color"<<std::endl; ok=false;}

		size_t const N_triangle = m.connectivity.size();
		if (N_triangle == 0)
		{std::cout<<warning+"Current mesh has no connectivity"<<std::endl; ok=false;}
		if (N_triangle>10000000)
		{std::cout<<warning+"Current mesh has more than 10 millions triangles"<<std::endl; ok=false;}

		// Check triangles
		for (size_t kt = 0; kt < N_triangle; ++kt) {
			uint3 const& face = m.connectivity[kt];
			unsigned int f0 = face[0];
			unsigned int f1 = face[1];
			unsigned int f2 = face[2];

			if (f0 > N || f1 > N || f2 > N)
			{
				std::cout<<warning+"Triangle "<<str(kt)<<" has index triplet ("<<str(f0)<<","<<str(f1)<<","<<str(f2)<<") exceeding the size of the position ["<<str(N)<<"]"<<std::endl;
				return false; // critical error
			}

			vec3 const& p0 = m.position[f0];
			vec3 const& p1 = m.position[f1];
			vec3 const& p2 = m.position[f2];

			// Check edge length
			if (norm(p1 - p0) < 1e-6f) {
				std::cout<<warning+"Edge ("+str(f0)+","+str(f1)+") has zero length. position["+str(f0)+"]="+str(p0)+", position["+str(f1)+"]="+str(p1)+"; L_edge = "<<str(norm(p1-p0))<<std::endl;
			}
			if (norm(p2 - p1) < 1e-6f) {
				std::cout<<warning+"Edge ("+str(f2)+","+str(f1)+") has zero length. position["+str(f2)+"]="+str(p2)+", position["+str(f1)+"]="+str(p1)+"; L_edge = "<<str(norm(p2-p1))<<std::endl;
			}
			if (norm(p2 - p0) < 1e-6f) {
				std::cout<<warning+"Edge ("+str(f2)+","+str(f0)+") has zero length. position["+str(f2)+"]="+str(p2)+", position["+str(f0)+"]="+str(p0)+"; L_edge = "<<str(norm(p2-p0))<<std::endl;
			}
		}


		// Check that vertices are indexed
		if(N<6000){ // only check for small meshes (slow)
			for (size_t k = 0; k < N; ++k)
			{
				bool found_vertex = false;
				for (size_t kt = 0; found_vertex==false && kt < N_triangle; ++kt) {
					uint3 const& face = m.connectivity[kt];
					if(k==face[0] || k==face[1] || k==face[2])
						found_vertex = true;
				}
				if (found_vertex == false)
					std::cout<<warning+"Vertex "+str(k)+" at position "+str(m.position[k])+" is not indexed in the connectivity"<<std::endl;
			}
		}

		if (ok == false)
		{
			std::cout<<"\nYou mesh seem to have issues - you should correct it before being able to display it\n"<<std::endl;
			std::cout<<"> If some buffers are empty, make sure you call mesh.fill_empty_field(); to your mesh structure"<<std::endl;
		}

		return ok;
	}

	std::string str(mesh const& m)
	{
		std::string s = "mesh[N_vertex="+str(m.position.size())+"][N_triangle="+str(m.connectivity.size())+"]";
		return s;
	}
	std::string type_str(mesh const&)
	{
		return "mesh";
	}

	mesh& mesh::flip_connectivity()
	{
		size_t const N_tri = connectivity.size();
		for (size_t k = 0; k < N_tri; ++k)
		{
			uint3 const tri = connectivity[k];
			uint3 const tri_flip = {tri[1],tri[0],tri[2]};
			connectivity[k] = tri_flip;
		}
		return *this;
	}
	mesh& mesh::normal_update()
	{
		normal_per_vertex(position, connectivity, normal);
		return *this;
	}

	mesh& mesh::translate(vec3 const& t)
	{
		for (vec3& p : position)
			p += t;
		return *this;
	}
	mesh& mesh::translate(float tx, float ty, float tz)
	{
		return translate({tx,ty,tz});
	}
	mesh& mesh::scale(float s)
	{
		for (vec3& p : position)
			p *= s;
		return *this;
	}
	mesh& mesh::scale(float sx,float sy, float sz)
	{
		for (vec3& p : position){
			p.x *= sx;
			p.y *= sy;
			p.z *= sz;
		}
		normal_update();
		return *this;
	}
	mesh& mesh::rotate(vec3 const& axis, float angle)
	{
		rotation_transform R = rotation_transform::from_axis_angle(axis, angle);
		for (vec3& p : position)
			p = R*p;
		for(vec3& n : normal)
			n = R*n;
		return *this;
	}
	mesh& mesh::apply_transform(mat3 const& M)
	{
		for (vec3& p : position)
			p = M * p;
		normal_update();
		return *this;
	}
	mesh& mesh::apply_transform(mat4 const& M)
	{
		for (vec3& p : position) {
			vec4 q = M * vec4(p, 1.0f);
			p = q.xyz() / q.w;
		}
		normal_update();
		return *this;
	}
	mesh& mesh::apply_transform(cgp::affine const& M) {
		for (vec3& p : position)
			p = M*p;
		for(vec3& n : normal) {
			n = M.rotation*n;
		}
		return *this;
	}
	mesh& mesh::apply_transform(cgp::affine_rt const& M) {
		for (vec3& p : position)
			p = M*p;
		for(vec3& n : normal) {
			n = M.rotation*n;
		}
		return *this;
	}
	mesh& mesh::apply_transform(cgp::affine_rts const& M)
	{
		for (vec3& p : position)
			p = M*p;
		for(vec3& n : normal) {
			n = M.rotation*n;
		}
		return *this;
	}


	numarray<numarray<int> > connectivity_one_ring(numarray<uint3> const& connectivity)
	{
		size_t const N = connectivity.size();
		numarray<std::set<int> > one_ring;
		one_ring.resize(N);
		for (size_t k = 0; k < N; ++k)
		{
			uint3 const& tri = connectivity[k];
			one_ring[ get<0>(tri) ].insert( get<1>(tri) );
			one_ring[ get<0>(tri) ].insert( get<2>(tri) );

			one_ring[ get<1>(tri) ].insert( get<0>(tri) );
			one_ring[ get<1>(tri) ].insert( get<2>(tri) );

			one_ring[ get<2>(tri) ].insert( get<0>(tri) );
			one_ring[ get<2>(tri) ].insert( get<1>(tri) );
		}

		numarray<numarray<int> > one_ring_buffer;
		one_ring_buffer.resize(N);
		for(int k=0; k<N; ++k)
			for(int idx : one_ring[k])
				one_ring_buffer[k].push_back(idx);
		return one_ring_buffer;
	}

	void mesh::get_bounding_box_position(vec3& p_min, vec3& p_max) const
	{
		assert_cgp(position.size() > 0, "Mesh must have more than 1 position");

		p_min = position[0];
		p_max = position[0];
		for (int k = 1; k < position.size(); ++k) {
			vec3 const& p = position[k];
			p_min = vec3(std::min(p_min.x, p.x), std::min(p_min.y, p.y), std::min(p_min.z, p.z));
			p_max = vec3(std::max(p_max.x, p.x), std::max(p_max.y, p.y), std::max(p_max.z, p.z));
		}
	}

	mesh& mesh::centered()
	{
		vec3 p_min, p_max;
		get_bounding_box_position(p_min, p_max);
		vec3 const center = (p_min + p_max) / 2.0f;
		translate(-center);

		return *this;
	}
	
	mesh& mesh::normalize_size_to_position()
	{
		vec3 p_min, p_max;
		get_bounding_box_position(p_min, p_max);
		vec3 L = p_max - p_min;
		float L_max = std::max(std::max(L.x, L.y), L.z);

		assert_cgp( std::abs(L_max) > 1e-5f, "Mesh has a length that seems to be zero");

		scale(1.0f / L_max);
		return *this;
	}
}