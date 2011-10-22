#include <fcppt/config/external_begin.hpp>
#include <fruitserver/std_error_string.hpp>
#include <cerrno>
#include <cstring>
#include <fcppt/config/external_end.hpp>


std::string const
fruitserver::std_error_string()
{
	return
		::strerror(
			errno);
}
