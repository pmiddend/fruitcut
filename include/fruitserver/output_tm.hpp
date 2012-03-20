#ifndef FRUITSERVER_OUTPUT_TM_HPP_INCLUDED
#define FRUITSERVER_OUTPUT_TM_HPP_INCLUDED

#include <ctime>
#include <ostream>


namespace fruitserver
{
std::ostream &
output_tm(
	std::ostream &,
	std::tm const &);
}

#endif
