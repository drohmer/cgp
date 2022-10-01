#include "path_info.hpp"

#include "cgp/cgp.hpp"


// User defined directory names for shaders and assets
// ******************************************************* //
// Change these values only if you wish to modify the name of the directory containing the shaders and the assets
static const std::string shaders_dirname = "shaders";
static const std::string assets_dirname  = "assets";
// ******************************************************* //


// Initialize static variables
std::string path_info::root;   
std::string path_info::shaders;
std::string path_info::assets;


static std::string find_root_path(std::string const& directory_to_find, std::string const& executable_dir, std::string const& executable_name, bool& success)
{
	// All the possible path attempts (from the current directory) to reach the root direction
	std::vector<std::string> attempt_root = { "", "../", "../../", "../../../", "../../../../",
		executable_name+"/", "../" + executable_name + "/", "../../" + executable_name + "/", "../../../" + executable_name + "/", "../../../../"+executable_name+"/",
		executable_dir,executable_dir+"../",executable_dir+"../"+executable_name,executable_dir + "../../",executable_dir + "../../../"
	};

	success = false;
	int k_attempt = 0;
	std::string current_root = "";
	for (k_attempt = 0; k_attempt < attempt_root.size() && !success; ++k_attempt) {
		current_root = attempt_root[k_attempt];
		success = cgp::check_path_exist(current_root + directory_to_find);
	}

	return current_root;
}

void split_executable_path(std::string const& executable_path, std::string& executable_dir_out, std::string& executable_name_out)
{
	char character_dir_split = '/';
#if defined(_WIN32)
	character_dir_split = '\\';
#endif

	// look for the last '/'
	size_t pos_last_slash = executable_path.find_last_of(character_dir_split);
	if (pos_last_slash < executable_path.size()) {
		executable_dir_out = executable_path.substr(0, pos_last_slash + 1);
		executable_name_out = executable_path.substr(pos_last_slash + 1, executable_path.size() - 1);
	}
	else { //possibly local directory
		executable_dir_out = "";
		executable_name_out = executable_path;
	}

	// Remove dots in case of .exe
	if (executable_name_out.size() > 4) {
		int N = executable_name_out.size();
		size_t pos_first_dot = executable_name_out.find_first_of('.');
		if (pos_first_dot < N)
			executable_name_out = executable_name_out.substr(0, pos_first_dot);
	}
}


static void display_error_file_access(std::string const& target_dir);
void path_info::initialize(std::string const& executable_path)
{
	std::string executable_dir, executable_name;
	split_executable_path(executable_path, executable_dir, executable_name);

	// Try to find automatically the path to the root directory (with shaders/ directory)
	bool found_shader_dir = false;
	root = find_root_path(shaders_dirname, executable_dir, executable_name, found_shader_dir);

	if (found_shader_dir==false) {
		display_error_file_access(shaders_dirname);
		exit(0); // Note: this is not an error of the program.
	}


	if (root == "") {
		std::cout << "  [info] Found default local path to shaders and assets." << std::endl;
	}
	else {
		std::cout << "  [info] Adapt default path to shaders and assets to " << root << std::endl;
	}


	// OpenGL Directory
	//  - opengl/   for desktop
	//  - opengles/ when using Emscripten
	std::string opengl_dir = "opengl";
#ifdef __EMSCRIPTEN__
	opengl_dir = "opengles";
#endif




	// Fill the default path
	shaders = root + shaders_dirname+"/" + opengl_dir + "/";
	assets = root + assets_dirname+"/";

	// Check the default path exists
	if (cgp::check_path_exist(shaders) == false) {
		std::cout << "Error cannot find the path: " << shaders << std::endl;
		std::cout << "Please check this directory does exists, otherwise the shaders won't be able to load." << std::endl;
		exit(0);
	}

	// Info display
	std::cout << "\t Shaders location: "<< shaders << std::endl;
	std::cout << "\t Assets  location: " << assets << std::endl;
	std::cout << std::endl;
}

void display_error_file_access(std::string const& target_dir) {
	std::cout << "[Parameters ERROR] The program failed to locate the shader ressource directory designed as: " + target_dir + "/" << std::endl;
	std::cout << "  => In most situation, it means that your program is not run from the root directory of this scene, and the directory shaders/ is therefore not accessible. " << std::endl;
	std::cout << "  => To solve this problem, you may need to adjust your IDE settings (or your placement in command line) such that your executable is run from the parent directory of shaders/. Then run again the program. " << std::endl;
	std::cout << "\n\nThe program will now exit" << std::endl;
}