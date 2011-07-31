#include "current_datetime.hpp"
#include "output_tm.hpp"
#include <sstream>
#include <ctime>

std::string const
fruitserver::current_datetime()
{
	std::time_t const _time_count = 
		std::time(
			0);
	std::tm const * const _tm = 
		std::localtime(
			&_time_count);
	if(!_tm)
		return "couldn't get the current date";
	std::ostringstream ss;
	fruitserver::output_tm(
		ss,
		*_tm);
	return 
		ss.str();
}
