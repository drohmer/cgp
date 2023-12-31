
#include "path.hpp"

#include "cgp/01_base/base.hpp"
#include "cgp/03_files/files.hpp"


namespace cgp {

	static std::string find_root_path(std::string const& directory_to_find, std::string const& executable_dir, std::string const& executable_name, bool& success)
	{
		// All the possible path attempts (from the current directory) to reach the root direction
		std::vector<std::string> attempt_root = { "", "../", "../../", "../../../", "../../../../",
			executable_name + "/", "../" + executable_name + "/", "../../" + executable_name + "/", "../../../" + executable_name + "/", "../../../../" + executable_name + "/",
			executable_dir,executable_dir + "../",executable_dir + "../" + executable_name,executable_dir + "../../",executable_dir + "../../../"
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
	std::string project_path_find(std::string const& executable_path, std::string const& directory_to_find)
	{
		std::string executable_dir, executable_name;
		split_executable_path(executable_path, executable_dir, executable_name);

		// Try to find automatically the path to the root directory (with shaders/ directory)
		bool found_shader_dir = false;
		std::string project = find_root_path(directory_to_find, executable_dir, executable_name, found_shader_dir);

		if (found_shader_dir == false) {
			display_error_file_access(directory_to_find);
			exit(0); // Note: this is not an error of the program.
		}

		std::string full_path_to_directory;
		if (project == "") {
			std::cout << "  [info] Found default local path to shaders and assets." << std::endl;
			full_path_to_directory = directory_to_find;
		}
		else {
			std::cout << "  [info] Adapt default path to shaders and assets to " << project << std::endl;
			full_path_to_directory = project + "/" + directory_to_find;
		}

		// Check the default path exists
		if (cgp::check_path_exist(full_path_to_directory) == false) {
			std::cout << "Error cannot find the path: " << full_path_to_directory << std::endl;
			std::cout << "Please check this directory does exists, otherwise the shaders won't be able to load." << std::endl;
			exit(0);
		}

		return project;
	}


	void display_error_file_access(std::string const& target_dir) {
		std::cout << "[Parameters ERROR] The program failed to locate the ressource directory designed as: " + target_dir + "/" << std::endl;
		std::cout << "Please check if this directory exists." << std::endl;
		std::cout << "  => In most situation, it means that your program is not run from the root directory of this scene, and the directory shaders/ is therefore not accessible. " << std::endl;
		std::cout << "  => To solve this problem, you may need to adjust your IDE settings (or your placement in command line) such that your executable is run from the parent directory of shaders/. Then run again the program. " << std::endl;
		std::cout << "\n\nThe program will now exit" << std::endl;
	}

}