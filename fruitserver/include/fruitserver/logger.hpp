#ifndef FRUITSERVER_LOGGER_HPP_INCLUDED
#define FRUITSERVER_LOGGER_HPP_INCLUDED

#include <fruitserver/current_datetime.hpp>
#include <ostream>
#include <sstream>


namespace fruitserver
{
class logger
{
public:
	explicit
	logger(
		std::ostream &_stream)
	:
		ss_(),
		stream_(
			_stream)
	{
		ss_ << fruitserver::current_datetime() << ": ";
	}

	template<typename T>
	logger &
	operator<<(
		T const &t)
	{
		ss_ << t;
		return
			*this;
	}

	~logger()
	{
		stream_ << ss_.str() << "\n";
	}
private:
	std::ostringstream ss_;
	std::ostream &stream_;
};
}

#endif
