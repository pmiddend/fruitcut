#include "bad_cast.hpp"

fruitcut::server::program_options::detail::bad_cast::bad_cast(
	std::string const &_string)
:	
	std::exception(),
	what_(
		_string)
{
}

char const *
fruitcut::server::program_options::detail::bad_cast::what() const throw()
{
	return what_.c_str();
}

fruitcut::server::program_options::detail::bad_cast::~bad_cast() throw()
{
}
