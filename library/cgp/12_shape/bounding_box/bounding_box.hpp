#pragma once

#include "cgp/11_mesh/mesh.hpp"

namespace cgp 
{

struct bounding_box 
{
    // Store the two extremal corners
    vec3 p_min;
    vec3 p_max;


    // Initialize a bounding box structure from a mesh
    void initialize(mesh const& m);
    // Initialize a bounding box structure from a set of positions
    void initialize(numarray<vec3> const& positions);

    // Extend the bounding box by a given distance d
    //  p_min <- p_min - d; p_max <- p_max + d
    void extends(float d);
    void extends(float dx, float dy, float dz);
    void extends(vec3 const& d);

    // Check is a point is inside the bounding box
    bool inside(vec3 const& p);

    // Check if two bounding boxes collide
    static bool collide(bounding_box const& bb1, bounding_box const& bb2);
    
};

}