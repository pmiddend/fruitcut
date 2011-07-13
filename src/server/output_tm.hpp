#ifndef FRUITCUT_SERVER_OUTPUT_TM_HPP_INCLUDED
#define FRUITCUT_SERVER_OUTPUT_TM_HPP_INCLUDED

#include <ostream>
#include <ctime>

namespace fruitcut
{
namespace server
{
std::ostream &
output_tm(
	std::ostream &,
	std::tm const &);
}
}

#endif
