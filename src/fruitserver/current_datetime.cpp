#include <fcppt/config/external_begin.hpp>
#include <fruitserver/current_datetime.hpp>
#include <fruitserver/output_tm.hpp>
#include <ctime>
#include <sstream>
#include <fcppt/config/external_end.hpp>


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
