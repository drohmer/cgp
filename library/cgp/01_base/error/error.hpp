#pragma once

#include "cgp/cgp_parameters.hpp"
#include <string>
#include <map>


// Macro to handle erros in cgp
//
// - assert_cgp( condition, error message ) : check condition and call error if condition if false. The error message is displayed if the condition is false.
// - assert_cgp_no_msg( condition ) : same as before, but without the need to specify a specific error message.
// - error_cgp( error message ) : call and error with the given message. 
//
// Error in cgp are typically aimed at stopping the program using abort after displaying some debug message.
//   cgp error are related to extra checks. For full efficiency, these check can be disabled in defining CGP_NO_DEBUG
//   The default abort behavior when an error is detected can also be converted to exception using cgp_ERROR_EXCEPTION


namespace cgp
{
[[noreturn]] void call_error(std::string const& error, std::string const& message, std::string const& filename, std::string const& function_name, int line);

void call_warning(std::string const& message_id, std::string const& extra, std::string const& filename, std::string const& function_name, int line);
struct cgp_warning {
	static int max_warning; // change max_warning in your program to 0 to remove all warnings
};
}

#ifndef CGP_NO_DEBUG
#define assert_cgp(X, MSG)   {if( (X)==false ) {cgp::call_error(#X,MSG,__FILE__,__func__ ,__LINE__);} }
#define assert_cgp_no_msg(X) {if( (X)==false ) {cgp::call_error(#X,"",__FILE__,__func__ ,__LINE__);} }
#else
#define assert_cgp(X, MSG) {}
#define assert_cgp_no_msg(X) {}
#endif

#define error_cgp(MSG)  {cgp::call_error("",MSG,__FILE__,__func__,__LINE__); }

// Default behavior of cgp error is to abort the program.
//  The default behavior can be switched to throwing exception in defining cgp_ERROR_EXCEPTION
// #define CGP_ERROR_EXCEPTION


// Handling Warning
//  Warnings are messages displayed on the command line to indicates that something looks odd
//  To avoid spanning too many messages in the animation loop, warnings with similar "message_id" are displayed a limited number of time.
//  Warning contains two parts: 
//      - message_id: The content of the warning that is supposed to be unique. Similar message_id can only be displayed a limited number of time.
//      - extra: Any extra information to this warning that doesn't count as a unique id.

#ifndef CGP_NO_WARNING
	#define warning_cgp(MESSAGE_ID, EXTRA) { cgp::call_warning(MESSAGE_ID, EXTRA, __FILE__, __func__, __LINE__); }
#else
	#define warning_cgp(MESSAGE_ID, EXTRA) {}
#endif

namespace cgp{
	template <typename T> void currently_unused(T const&) {}
}