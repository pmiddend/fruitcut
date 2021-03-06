#ifndef FRUITSERVER_PROGRAM_OPTIONS_DETAIL_BAD_CAST_HPP_INCLUDED
#define FRUITSERVER_PROGRAM_OPTIONS_DETAIL_BAD_CAST_HPP_INCLUDED

#include <exception>
#include <string>


namespace fruitserver
{
namespace program_options
{
namespace detail
{
class bad_cast
:
	public std::exception
{
public:
	explicit
	bad_cast(
		std::string const &);

	char const *
	what() const throw();

	virtual ~bad_cast() throw();
private:
	std::string what_;
};
}
}
}

#endif
