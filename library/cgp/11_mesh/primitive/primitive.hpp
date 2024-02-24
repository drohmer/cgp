#pragma once

#include "../mesh.hpp"

#include "cgp/09_geometric_transformation/frame/frame.hpp"

namespace cgp
{
    struct frame;

    /** Generate a cylinder - parameterized by an axis [p0,p1] and a radius around it.
    * - radius: Cylinder radius
    * - p0, p1: start/end point on the central axis
    * - Nu,Nv: sampling in parametric space (u=along [p0,p1], v=around the cylinder)
    * - is_closed: Is the cylinder closed at the two extremities with a disc
    */
    mesh mesh_primitive_cylinder(float radius=0.2f, vec3 const& p0={0,0,0}, vec3 const& p1={0,0,1}, int Nu=10, int Nv=20, bool is_closed=false);

    // A simple triangle parameterized by its three points
    mesh mesh_primitive_triangle(vec3 const& p0={0,0,0}, vec3 const& p1={1,0,0}, vec3 const& p2={0,1,0});

    /**
     * Build a quadrangle given by its four corner positions
     * 
     * Assume the following order of position (p00,p10,p11,p01).
     *    p01 ----- p11
     *     |         |
     *     |         |
     *    p00 ----- p10
     * Build 2 triangles: (p00,p10,p11) and (p00,p11,p01)
     * uv are set such as p00:(0,0), p10:(1,0), p11:(1,1), p01:(0,1) */
    mesh mesh_primitive_quadrangle(vec3 const& p00={0,0,0}, vec3 const& p10={1,0,0}, vec3 const& p11={1,1,0}, vec3 const& p01={0,1,0});

    /** Build a planar disc parameterized by a central point, a radius, and a normal.
     *	
     * The disc is bild by triangles that links the central point to consecutive samples along the circumference.
     * - radius: the radius of the disc
     * - center: the central point of the disc (an actual vertex of the mesh)
     * - normal: the normal of the disc (all vertices have the same normal)
     * - N: The number of sampled vertex in the circumference of the disc
     * */
    mesh mesh_primitive_disc(float radius=1.0f, vec3 const& center={0,0,0}, vec3 const& normal={0,0,1}, int N=40);

    /** Build a sphere parameterized by a radius, and a center, sampled using a spherical coordinate parameterization
     * - radius: the radius of the sphere
     * - center: the position of the central point inside the sphere
     * - Nu, Nv: the number of samples in the two parametric directions */
    mesh mesh_primitive_sphere(float radius=1.0f, vec3 const& center={0,0,0}, int Nu=40, int Nv=20);

    /** Build an ellipsoid elongated along the (x,y,z) directions.
     * - scale: the elongation of the disc along x/y/z
     * - center: the central point of the ellipsoid
     * - Nu, Nv: the number of samples in the two parametric directions (use a spherical coordinate parameterization) */
    mesh mesh_primitive_ellipsoid(vec3 scale=vec3{1.0f, 1.0f, 1.0f}, vec3 const& center={0,0,0}, int Nu=40, int Nv=20);

    /** Build a torus parameterized as 
     * x = (r_max + r_min * cos(theta) ) * cos(phi)
     * y = (r_max + r_min * cos(theta) ) * sin(phi)
     * z = r_min * sin(theta)
     * 
     * - r_max: radius around the large revolution axis
     * - r_min: radius around the smaller revolution axis
     * - center: central point of the torus
     * - orientation: orientation of the central axis of revolution of the torus
     * - Nu, Nv: samples in the parameteric space
     * */
    mesh mesh_primitive_torus(float r_max=1.0f, float r_min=0.25f, vec3 const& center={0,0,0}, vec3 const& axis_orientation={0,0,1}, int Nu=55, int Nv=15);

    /** Build plane sampled as a regular grid. The plane is parameterized by its 4 extremitites and the size of the sampling.
     * 
     *   p01 ----- p11
     *    |         |
     * v↑ |         |
     *  p00 ----- p10
     *      -> u
    */
    mesh mesh_primitive_grid(vec3 const& p00={0,0,0}, vec3 const& p10={1,0,0}, vec3 const& p11={1,1,0}, vec3 const& p01={0,1,0}, int Nu=10, int Nv=10);

    /** Build cube oriented along the x/y/z direction sampled uniformly on all its faces using a regular grid. 
     * 
     * - p000, p100, p110, p010, p001, p101, p111, p011 are the corners of the cube.
     * 
     *         p011-----------p111
     *         /|            /|
     *       / |           / |
     *   p001-----------p101 |
     *      |  |         |   |
     *      |  |         |   |
     *      |  p010------|---p110
     *      | /          |  /
     *      |/           | /
     *     p000---------p100
     * 
     * - Nx, Ny, Nz: are the sampling on each of the face oriented along the x, y, and z direction
    */
    mesh mesh_primitive_cubic_grid(vec3 const& p000={0,0,0}, vec3 const& p100={1,0,0}, vec3 const& p110={1,1,0}, vec3 const& p010={0,1,0}, vec3 const& p001={0,0,1}, vec3 const& p101={1,0,1}, vec3 const& p111={1,1,1}, vec3 const& p011={0,1,1}, int Nx=10, int Ny=10, int Nz=10);

    /** Build a cube oriented along the x/y/z direction parameterized by a center and edge length 
     * Each face of the cube is made by two triangles. */
    mesh mesh_primitive_cube(vec3 const& center={0,0,0}, float edge_length=1.0f);

    /** Build a tetrahedron described by its three corners.*/
    mesh mesh_primitive_tetrahedron(vec3 const& p0={0,0,0}, vec3 const& p1={1,0,0}, vec3 const& p2={0,1,0}, vec3 const& p3={0,0,1});



    /**	Build a cone 
     *        /\        ^ height
     *       /  \       |
     *      /    \      |
     *     /      \     |
     *    /        \    |    ↑: axis_direction
     *   /          \   |
     *  /            \  |
     * /______________\ v   
     *        | 
     *        center_of_base
     * <-------------->
     *      radius 
     * 
     * - radius: the radius at the base of the cylinder
     * - height: the height of the cylinder along 
     * - center_of_base: position at the center of the base disc
     * - axis_direction: direction of the central axis
     * - is_closed_base: fill the base with a disc if true
     * - Nu, Nv: samples along the height and radial direction
     * 
    */
    mesh mesh_primitive_cone(float radius=0.5f, float height=1.0f, vec3 const& center_of_base={0,0,0}, vec3 const& axis_direction={0,0,1}, bool is_closed_base=true, int Nu=20, int Nv=10);

    /** Mesh representing an arrow: {cylinder+cone} pointing from point p0 to point p1
     *       /\     ^
     *      /  \    | 
     *     /    \   | cone_height_scale = ||p1-p0|| * cone_length_scale
     *    /      \  |
     *   /        \ |
     *  /____p1____\v  
     *      |  | cone_radius = cylinder_radius * cone_radius_scale
     *      |  |    
     *      |  |    
     *      |__| cylinder_radius   
     *       p0
     *   
     * p0: the base point of the array
     * p1: the ending point of the array
     * cylinder_radius: radius of the cylinder
     * cone_height_scale: length of cone extremity expressed as the scale of the cylinder radius
     * cone_radius_scale: radius of the cone extremity expressed as the scale of the cylinder_radius
     * N: number of samples (cylinder and cone)
     * double_sided: create an arrow also at the base point
    */
    mesh mesh_primitive_arrow(vec3 const& p0={0,0,0}, vec3 const& p1={1,0,0}, float cylinder_radius=0.05f, float cone_height_scale=4.0f, float cone_radius_scale=2.5f, int N=18);

    /** Build an oriented frame made of 3 orthogonal arrows to depict an x/y/z direction */
    mesh mesh_primitive_frame(frame const& f=frame(), float scale=1.0f, vec3 const& color_ux={1,0,0}, vec3 const& color_uy={0,1,0}, vec3 const& color_uz={0,0,1}, vec3 const& color_sphere={1,1,1}, float frame_thickness=0.025f);
}

