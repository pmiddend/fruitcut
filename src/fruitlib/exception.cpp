#include "exception.hpp"
#include <fcppt/string.hpp>
#include <fcppt/exception.hpp>

fruitcut::fruitlib::exception::exception(
	fcppt::string const &_string)
:
	fcppt::exception(
		_string)
{}

fruitcut::fruitlib::exception::~exception() throw()
{
}
