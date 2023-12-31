#include "bounding_box.hpp"

namespace cgp 
{

void bounding_box::initialize(mesh const& m) {
    initialize(m.position);
}
void bounding_box::initialize(numarray<vec3> const& position) 
{
    assert_cgp(position.size()>0, "Must be at least 1 position for a bounding box");

    p_min = position[0];
    p_max = position[0];
    for (int k = 1; k < position.size(); ++k) {
        vec3 const& p = position[k];
        p_min = vec3(std::min(p_min.x, p.x), std::min(p_min.y, p.y), std::min(p_min.z, p.z));
        p_max = vec3(std::max(p_max.x, p.x), std::max(p_max.y, p.y), std::max(p_max.z, p.z));
    }

}

bool bounding_box::inside(vec3 const& p){
    return (p.x>=p_min.x && p.x<=p_max.x)
    && (p.y>=p_min.y && p.y<=p_max.y)
    && (p.z>=p_min.z && p.z<=p_max.z);
}

bool bounding_box::collide(bounding_box const& bb1, bounding_box const& bb2)
{
    return (bb1.p_min.x<=bb2.p_max.x && bb1.p_max.x>=bb2.p_min.x)
        && (bb1.p_min.y<=bb2.p_max.y && bb1.p_max.y>=bb2.p_min.y)
        && (bb1.p_min.z<=bb2.p_max.z && bb1.p_max.z>=bb2.p_min.z);  
}

void bounding_box::extends(float d)
{
    p_min = sub(p_min, d);
    p_max = add(p_max, d);
}
void bounding_box::extends(float dx, float dy, float dz)
{
    extends({dx,dy,dz});
}
void bounding_box::extends(vec3 const& d)
{
    p_min = p_min-d;
    p_max = p_max+d;
}

}