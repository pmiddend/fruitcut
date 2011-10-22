#ifndef FRUITSERVER_OUTPUT_TM_HPP_INCLUDED
#define FRUITSERVER_OUTPUT_TM_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <ctime>
#include <ostream>
#include <fcppt/config/external_end.hpp>


namespace fruitserver
{
std::ostream &
output_tm(
	std::ostream &,
	std::tm const &);
}

#endif
