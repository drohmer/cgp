#include "error.hpp"
#include <iostream>

#include <stdexcept>


namespace cgp
{

static std::map<std::string, int> warning_storage;

void call_error(std::string const& assert_arg, std::string const& message, std::string const& filename, std::string const& function_name, int line)
{
    std::string msg = "";

    msg += "\n===========================================\n";
    msg += "ERROR detected !\n\n";
    msg += "Error found at this point of the code:\n";
    msg += "-------------------------------------\n";
    msg += "  - File: "+filename+"\n";
    msg += "  - Line: "+std::to_string(line)+"\n";
    msg += "  - Function: "+function_name+"\n";
    if(assert_arg!="")
        msg += "  - Assert failed on the condition: "+assert_arg+"\n";
    if(message!="")
        msg += "  - Error message: "+message+"\n";
    msg += "\n";

    msg += "For more details on this error: \n";

#if !_WIN32
    msg += "> Run GDB on the executable and check the backtrace.\n";
    msg += "  $ gdb build/pgm \n";
    msg += "    run \n";
    msg += "    backtrace \n\n";
#else
    msg += "> Run the program from Visual Studio, then look at the Call Stack Window once the error occurs.\n";
    msg += "  Visual Studio has internal debugger from which you may debug your code.\n\n";
#endif

    // Default behavior of error is to display the error message on cerr and abort the program
    //  This default behavior can be change to throw exception in defining cgp_ERROR_EXCEPTION
#ifdef CGP_ERROR_EXCEPTION
    throw(std::logic_error(msg));
#else
    // Check your command line to see the error message.
    std::cerr << msg;
    abort();
#endif

}

void call_warning(std::string const& message_id, std::string const& extra, std::string const& filename, std::string const& function_name, int line)
{
    int const max_warning = 4;

    cgp::warning_storage[message_id] = cgp::warning_storage[message_id] + 1;

    if (cgp::warning_storage[message_id] > max_warning)
        return ;
    
    std::string output  = "[Warning cgp] "+message_id+" "+extra; 
    output += "\n  file:"+filename+", function:"+function_name+", "+std::to_string(line)+"\n";
    if (cgp::warning_storage[message_id] == max_warning) {
        output += "\n(The previous warning has been displayed several times and will not be displayed anymore)";
    }

    std::cout<<output<<std::endl;
}

}





