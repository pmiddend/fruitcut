#include "generate_datetime.hpp"
#include <sstream>
#include <iomanip>
#include <ctime>

std::string const
fruitcut::server::generate_datetime()
{
	std::time_t const time_numeric = 
		std::time(0);

	std::tm time_struct;
	// call the thread_safe posix function
	::gmtime_r(
		&time_numeric,
		&time_struct);

	std::ostringstream ss;
	// The setw and setfill are verbose, I know
	ss 
		// tm_year is "since 1900"
		<< (1900 + time_struct.tm_year)
		// tm_mon is zero-based (wtf?)
		<< std::setw(2) << std::setfill('0') << (time_struct.tm_mon + 1)
		// tm_mday is not zero-based (wtf?)
		<< std::setw(2) << std::setfill('0') << (time_struct.tm_mday)
		<< 'T'
		<< std::setw(2) << std::setfill('0') << (time_struct.tm_hour)
		<< std::setw(2) << std::setfill('0') << (time_struct.tm_min)
		<< std::setw(2) << std::setfill('0') << (time_struct.tm_sec);

	return 
		ss.str();
}
