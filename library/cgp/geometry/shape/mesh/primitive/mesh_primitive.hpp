#pragma once

#include "../structure/mesh.hpp"

#include "cgp/geometry/transform/frame/frame.hpp"

namespace cgp
{
	struct frame;

	/** Generate a cylinder
	* @radius: Cylinder radius
	* @p0: starting point on the central axis
	* @p1: ending point of the central axis 
	* @Nu,Nv: sampling in parametric space
	* @is_closed: Is the cylinder closed at the two extremities with a disc
	*/
	mesh mesh_primitive_cylinder(float radius=0.2f, vec3 const& p0={0,0,0}, vec3 const& p1={0,0,1}, int Nu=10, int Nv=20, bool is_closed=false);

	mesh mesh_primitive_triangle(vec3 const& p0={0,0,0}, vec3 const& p1={1,0,0}, vec3 const& p2={0,1,0});

	/**
	* Assume the following order of position (p00,p10,p11,p01).
	*    p01 ----- p11
	*     |         |
	*     |         |
	*    p00 ----- p10
	* Build 2 triangles: (p00,p10,p11) and (p00,p11,p01)
	* uv are set such as p00:(0,0), p10:(1,0), p11:(1,1), p01:(0,1)
	*/
	mesh mesh_primitive_quadrangle(vec3 const& p00={0,0,0}, vec3 const& p10={1,0,0}, vec3 const& p11={1,1,0}, vec3 const& p01={0,1,0});

	mesh mesh_primitive_disc(float radius=1.0f, vec3 const& center={0,0,0}, vec3 const& normal={0,0,1}, int N=40);

	mesh mesh_primitive_sphere(float radius=1.0f, vec3 const& center={0,0,0}, int Nu=40, int Nv=20);

	mesh mesh_primitive_ellipsoid(vec3 scale=vec3{1.0f, 1.0f, 1.0f}, vec3 const& center={0,0,0}, int Nu=40, int Nv=20);

	mesh mesh_primitive_torus(float r_major=1.0f, float r_minor=0.25f, vec3 const& center={0,0,0}, vec3 const& axis_orientation={0,0,1}, int Nu=55, int Nv=15);

	mesh mesh_primitive_grid(vec3 const& p00={0,0,0}, vec3 const& p10={1,0,0}, vec3 const& p11={1,1,0}, vec3 const& p01={0,1,0}, int Nu=10, int Nv=10);

	mesh mesh_primitive_cubic_grid(vec3 const& p000={0,0,0}, vec3 const& p100={1,0,0}, vec3 const& p110={1,1,0}, vec3 const& p010={0,1,0}, vec3 const& p001={0,0,1}, vec3 const& p101={1,0,1}, vec3 const& p111={1,1,1}, vec3 const& p011={0,1,1}, int Nx=10, int Ny=10, int Nz=10);

	mesh mesh_primitive_cube(vec3 const& center={0,0,0}, float edge_length=1.0f);
	mesh mesh_primitive_tetrahedron(vec3 const& p0={0,0,0}, vec3 const& p1={1,0,0}, vec3 const& p2={0,1,0}, vec3 const& p3={0,0,1});



	/**	*/
	mesh mesh_primitive_cone(float radius=0.5f, float height=1.0f, vec3 const& center_of_base={0,0,0}, vec3 const& axis_direction={0,0,1}, bool is_closed_base=true, int Nu=20, int Nv=10);

	/** Mesh representing an arrow: {cylinder+cone} pointing from point p0 to point p1
	* @p0: the base point of the array
	* @p1: the ending point of the array
	* @cylinder_radius: radius of the cylinder
	* @cone_length_scale: length of cone extremity expressed as the scale of the cylinder radius
	* @cone_radius_scale: radius of the cone extremity expressed as the scale of the cylinder_radius
	* @N: number of samples (cylinder and cone)
	* @double_sided: create an arrow also at the base point
	*/
	mesh mesh_primitive_arrow(vec3 const& p0={0,0,0}, vec3 const& p1={1,0,0}, float cylinder_radius=0.05f, float cone_length_scale=4.0f, float cone_radius_scale=2.5f, int N=18);

	mesh mesh_primitive_frame(frame const& f=frame(), float scale=1.0f, vec3 const& color_ux={1,0,0}, vec3 const& color_uy={0,1,0}, vec3 const& color_uz={0,0,1}, vec3 const& color_sphere={1,1,1}, float frame_thickness=0.025f);
}

