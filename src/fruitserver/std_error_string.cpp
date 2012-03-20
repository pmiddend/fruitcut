#include <fruitserver/std_error_string.hpp>
#include <cerrno>
#include <cstring>


std::string const
fruitserver::std_error_string()
{
	return
		::strerror(
			errno);
}
