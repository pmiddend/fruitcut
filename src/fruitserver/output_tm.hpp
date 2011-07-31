#ifndef FRUITSERVER_OUTPUT_TM_HPP_INCLUDED
#define FRUITSERVER_OUTPUT_TM_HPP_INCLUDED

#include <ostream>
#include <ctime>

namespace fruitserver
{
std::ostream &
output_tm(
	std::ostream &,
	std::tm const &);
}

#endif
