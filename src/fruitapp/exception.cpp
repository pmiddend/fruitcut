#include "exception.hpp"
#include <fcppt/string.hpp>
#include <fcppt/exception.hpp>

fruitapp::exception::exception(
	fcppt::string const &_string)
:
	fcppt::exception(
		_string)
{}

fruitapp::exception::~exception() throw()
{
}
