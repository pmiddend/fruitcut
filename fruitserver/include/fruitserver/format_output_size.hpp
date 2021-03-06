#ifndef FRUITSERVER_FORMAT_OUTPUT_SIZE_HPP_INCLUDED
#define FRUITSERVER_FORMAT_OUTPUT_SIZE_HPP_INCLUDED

#include <iomanip>
#include <sstream>
#include <string>


namespace fruitserver
{
template<typename T>
std::string
format_output_size(
	T const &t)
{
	std::ostringstream ss;
	ss << std::hex << std::setw(8) << std::setfill('0') << t;
	return ss.str();
}
}

#endif
