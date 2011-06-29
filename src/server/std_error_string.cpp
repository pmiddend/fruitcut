#include "std_error_string.hpp"
#include <cstring>
#include <cerrno>

std::string const
fruitcut::server::std_error_string()
{
	return 
		::strerror(
			errno);
}
