#ifndef FRUITCUT_SERVER_FORMAT_OUTPUT_SIZE_HPP_INCLUDED
#define FRUITCUT_SERVER_FORMAT_OUTPUT_SIZE_HPP_INCLUDED

#include <string>
#include <sstream>
#include <iomanip>

namespace fruitcut
{
namespace server
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
}

#endif
