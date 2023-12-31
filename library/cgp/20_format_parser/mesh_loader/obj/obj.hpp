#pragma once

#include "cgp/11_mesh/mesh.hpp"

namespace cgp
{
    /** Save a mesh in .obj file
    * Note that OBJ format doesn't stores per-vertex color */
    void save_file_obj(std::string const& filename, mesh const& m);


    /** Minimalist export of triangle soup */
    void save_file_obj(std::string const& filename, std::vector<vec3> const& position, std::vector<vec3> const& normal);

    /** Load a mesh stored as .obj in the filename.
    * Notes: 
    *  - Normals and UV are read, and vertices are duplicated if needed
    *  - .mtl files are not read with this loader (cannot read shading and color)
    *  - Only one mesh is loaded - this parser cannot be used when multiple textures are associated to different objects
    *  - The mesh is triangulated if higher degree polygons are in the file
    */
    mesh mesh_load_file_obj(std::string const& filename);

    /** Load a mesh stored as .obj in the filename. 
    * Outputs the correspondance between the vertex index in the file, and the loaded one */
    mesh mesh_load_file_obj(std::string const& filename, numarray<numarray<int>>& vertex_correspondance);



namespace loader{

    enum class obj_type {
        vertex,                // f %d %d %d
        vertex_texture,        // f %d/%d %d/%d %d/%d
        vertex_texture_normal, // f %d/%d/%d %d/%d/%d %d/%d/%d
        vertex_normal          // f %d//%d %d//%d %d//%d
    };

    /** Simple file reader of the position connectivity assuming triangles (doesn't handle texture and normal connectivity) */
    std::vector<uint3> obj_read_connectivity(const std::string& filename);

    /** Read only vertices position from obj file */
    std::vector<vec3> obj_read_positions(const std::string& filename);
    /** Real only normals from obj file */
    std::vector<vec3> obj_read_normals(const std::string& filename);
    /** Read only texture uv coordinates from obj file */
    std::vector<vec2> obj_read_texture_uv(const std::string& filename);

    /** Read faces information from obj file given a type to read (position + [texture] + [normals])
     * Return buffer < buffer <int3> >
     *          |       |        -> index of position/texture/normals
     *          |       -> vertices of a face (can be arbitrary polygon)
     *          -> buffer of faces
     * Texture and normals are set to -1 if they are not defined
    */

    numarray<numarray<int3>> obj_read_faces(const std::string& filename, obj_type const type);
}


}
