#include "std_error_string.hpp"
#include <cstring>
#include <cerrno>

std::string const
fruitserver::std_error_string()
{
	return 
		::strerror(
			errno);
}
