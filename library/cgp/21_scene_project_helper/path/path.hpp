#pragma once

#include <string>

namespace cgp {
		
	// Helper function trying to find automatically the relative path to the root directory that contains the shaders/ and assets/ directories in classical projects
	//  - executable_path: the argument argv[0]
	//  - directory_to_find: a target directory (default=shaders/) to be found
	// This function should be called at the beginning of the program to initialize the relative path
	// ex. std::string path_project = helper_path_project::initialize(argv[0]);
	std::string project_path_find(std::string const& executable_path, std::string const& directory_to_find = "shaders/");

}
