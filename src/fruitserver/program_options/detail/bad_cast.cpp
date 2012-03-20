#include <fruitserver/program_options/detail/bad_cast.hpp>


fruitserver::program_options::detail::bad_cast::bad_cast(
	std::string const &_string)
:
	std::exception(),
	what_(
		_string)
{
}

char const *
fruitserver::program_options::detail::bad_cast::what() const throw()
{
	return what_.c_str();
}

fruitserver::program_options::detail::bad_cast::~bad_cast() throw()
{
}
