#include "cgp/01_base/base.hpp"
#include "hierarchy_mesh_drawable.hpp"

namespace cgp
{

    static void assert_valid_hierarchy(hierarchy_mesh_drawable const& hierarchy);

    void hierarchy_mesh_drawable::add(hierarchy_mesh_drawable_node const& node)
    {
        name_map[node.name] = static_cast<int>(elements.size());
        elements.push_back(node);
        assert_valid_hierarchy(*this);
    }
    void hierarchy_mesh_drawable::add(mesh_drawable const& element, std::string const& name, std::string const& name_parent, vec3 const& translation, rotation_transform const& rotation)
    {
        affine_rts const transform = affine_rts(rotation, translation, 1.0f);
        hierarchy_mesh_drawable_node const node = {element, name, name_parent, transform};
        add(node);
    }
    void hierarchy_mesh_drawable::add(mesh_drawable const& element, std::string const& name, std::string const& name_parent, affine_rts const& transform)
    {
        hierarchy_mesh_drawable_node const node = {element, name, name_parent, transform};
        add(node);
    }


    hierarchy_mesh_drawable_node& hierarchy_mesh_drawable::operator[](std::string const& name)
    {
        auto it = name_map.find(name);
        if (it == name_map.end())
        {
            std::cerr << "Error: cannot find element [" << name << "] in hierarchy_mesh_drawable" << std::endl;
            std::cerr << "Possibles element names are: ";
            for (auto const& s : name_map) { std::cerr << "[" << s.first << "] "; }
            abort();
        }

        const size_t index = it->second;
        assert_cgp_no_msg(index < elements.size());

        return elements[index];
    }
    hierarchy_mesh_drawable_node const& hierarchy_mesh_drawable::operator[](std::string const& name) const
    {
        auto it = name_map.find(name);
        if (it == name_map.end())
        {
            std::cerr << "Error: cannot find element [" << name << "] in hierarchy_mesh_drawable" << std::endl;
            std::cerr << "Possibles element names are: ";
            for (auto const& s : name_map) { std::cerr << "[" << s.first << "] "; }
            abort();
        }

        const size_t index = it->second;
        assert_cgp_no_msg(index < elements.size());

        return elements[index];
    }


    void hierarchy_mesh_drawable::update_local_to_global_coordinates()
    {
        if(elements.size()==0)
            return ;

        assert_valid_hierarchy(*this);

        std::string const& name_root_parent = elements[0].name_parent;

        int const N = static_cast<int>(elements.size());
        for(int k=0; k<N; ++k)
        {
            hierarchy_mesh_drawable_node& element = elements[k];

            std::string const& parent_name = element.name_parent;

            // Case of root element (or same parent) - local = global
            if( parent_name == name_root_parent ) {
                element.drawable.hierarchy_transform_model = element.transform_local;
            }
            // Else apply hierarchical transformation
            else
            {
                int const parent_id = name_map[parent_name];
                affine_rts const& local = element.transform_local;
                affine_rts const& global_parent = elements[parent_id].drawable.hierarchy_transform_model;

                element.drawable.hierarchy_transform_model = global_parent * local;
            }
        }
    }


    void assert_valid_hierarchy(hierarchy_mesh_drawable const& hierarchy)
    {
        if(hierarchy.elements.size()==0)
            return ;

        {
            // Check that elements and name_map have the same size
            const size_t N1 = hierarchy.elements.size();
            const size_t N2 = hierarchy.name_map.size();
            if(N1!=N2) {
                std::cerr<<"Error: Hierarchy not valid - Number of element ("<<N1<<") != Name_map.size() ("<<N2<<")"<<std::endl;
                abort();
            }
        }

        {
            // Check that the name of the parent of root node doesn't belong to the hierarchy
            const std::string root_name_parent = hierarchy.elements[0].name_parent;
            const auto it = hierarchy.name_map.find(root_name_parent);
            if(it!=hierarchy.name_map.end()) {
                std::cerr<<"Error: Hierarchy not valid - name of the root node ("<<root_name_parent<<") cannot be an element of the hierarchy"<<std::endl;
                abort();
            }
        }

        {
            // Check that all names stored in map are designating the corresponding elements
            for(const auto& e : hierarchy.name_map)
            {
                std::string const& name = e.first;
                int const index = e.second;

                if(index>=int(hierarchy.elements.size()) || index<0)
                {
                    std::cerr<<"Error: Hierarchy not valid - Incorrect index stored in name_map: "<<name<<";"<<index<<std::endl;
                    abort();
                }

                hierarchy_mesh_drawable_node const& designated_element = hierarchy.elements[index];
                if(designated_element.name != name)
                {
                    std::cerr<<"Error: Hierarchy not valid - Incoherent name between map ("<<name<<";"<<index<<") and element ("<<designated_element.name<<") "<<std::endl;
                    abort();
                }
            }
        }

        {
            // Check that all parents are defined before their children (excepted for root parent)
            std::set<std::string> parent_names_visited;
            parent_names_visited.insert(hierarchy.elements[0].name_parent);
            int const N = hierarchy.elements.size();
            for(int k=0; k<N; ++k)
            {
                std::string const& name = hierarchy.elements[k].name;
                std::string const& parent_name = hierarchy.elements[k].name_parent;
                if(parent_names_visited.find(parent_name)==parent_names_visited.end())
                {
                    std::cerr<<"Error: Hierarchy not valid"<<std::endl;
                    std::cerr<<"Element ("<<name<<","<<k<<") has parent name ("<<parent_name<<") used before being defined"<<std::endl;
                    std::cerr << std::endl;
                    std::cerr << "Display hierarchy for debugging: " << std::endl;
                    std::cerr << hierarchy.hierarchy_display() << std::endl;
                    abort();
                }
                else
                    parent_names_visited.insert(name);
            }
        }

    }

    std::string hierarchy_mesh_drawable::hierarchy_display() const
    {
        std::string s;

        int const N = elements.size();
        for (int k = 0; k < N; ++k)
        {
            s += "Element [" + str(k) + "] " + elements[k].name + " has parent: " + elements[k].name_parent + "\n";
        }

        return s;
    }


    void draw(hierarchy_mesh_drawable const& hierarchy, environment_generic_structure const& environment, int instance_count, bool expected_uniforms, uniform_generic_structure const& additional_uniforms)
    {
        int const N = hierarchy.elements.size();
        for (int k = 0; k < N; ++k)
            draw(hierarchy.elements[k].drawable, environment, instance_count, expected_uniforms, additional_uniforms);
    }

    void draw_wireframe(hierarchy_mesh_drawable const& hierarchy, environment_generic_structure const& environment, vec3 const& color, int instance_count, bool expected_uniforms, uniform_generic_structure const& additional_uniforms)
    {
        int const N = hierarchy.elements.size();
        for (int k = 0; k < N; ++k)
            draw_wireframe(hierarchy.elements[k].drawable, environment, color, instance_count, expected_uniforms, additional_uniforms);
    }

}