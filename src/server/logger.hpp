#ifndef FRUITCUT_SERVER_LOGGER_HPP_INCLUDED
#define FRUITCUT_SERVER_LOGGER_HPP_INCLUDED

#include "current_datetime.hpp"
#include <ostream>
#include <sstream>

namespace fruitcut
{
namespace server
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
		ss_ << server::current_datetime() << ": ";
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
}

#endif
