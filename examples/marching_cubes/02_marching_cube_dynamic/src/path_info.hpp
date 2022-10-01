#pragma once

#include <string>

// Helper structure to set relative path to the shaders/ and assets/ directories
struct path_info {

	static std::string root;    // Stores the default path to the root of the project (root directory = directory that contains the shaders/ folder)
	static std::string shaders; // Stores the default path to the shaders
	static std::string assets;  // Stores the default path to the asset


	// Need to call initialize to fill the path values
	static void initialize(std::string const& executable_path);
};

