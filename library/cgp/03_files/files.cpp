#include "files.hpp"

#include "cgp/01_base/base.hpp"

#include <fstream>
#include <iostream>
#include <sys/stat.h>

#if defined(__linux__) || defined(__EMSCRIPTEN__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace cgp
{
    bool check_file_exist(std::string const& filename)
    {
        // Open file
        std::ifstream stream(filename);
        if( !stream.is_open() )
            return false;
        else
        {
            stream.close();
            return true;
        }
    }

    bool check_path_exist(std::string const& pathname)
    {
        struct stat buffer;
        return (stat(pathname.c_str(), &buffer) == 0);
    }

    void assert_file_exist(std::string const& filename)
    {
        // Open file
        std::ifstream stream(filename);

        // Abort program if the file cannot be opened
        if( !stream.is_open() ) {

            std::string msg =
                    "Error cannot access the following file path \n    \""+filename+"\"\n\n"
                    "    => If the file exists, you may need to adapt the path from which you run the program \n"
                    "       Reminder: The program is exprected to run form the root directory\n"
                    "       (In QtCreator: set projects/Run directory)\n"
                    "       (In Command Line: change the directory such that you can access the file)\n"
                    "       (In Visual Studio: you need to copy the directory containing this data at the same place than your executable)";

            error_cgp(msg);
        }

        stream.close();
    }

    std::string read_text_file(std::string const& filename)
    {
        assert_file_exist(filename);

        std::ifstream str(filename);
        std::stringstream buffer;
        buffer << str.rdbuf();
        return buffer.str();
    }


    size_t file_get_size(std::string const& filename)
    {
        assert_file_exist(filename);
        struct stat stat_buf;
        int rc = stat(filename.c_str(), &stat_buf);
        assert_cgp(rc==0, "Cannot stat file_size " + filename);

        return stat_buf.st_size;
    }

    
    std::vector <char> read_from_file_binary(std::string const& filename)
    {
        assert_file_exist(filename);
        size_t s = file_get_size(filename);
        assert_cgp(s > 0, "File "+filename+" is empty");

        std::vector <char> buffer;
        buffer.resize(s);

        
        std::ifstream fid(filename, std::ios::in | std::ios::binary);
        fid.read(&buffer[0], s);
        fid.close();

        return buffer;
    }
}