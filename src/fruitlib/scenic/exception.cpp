#include "exception.hpp"
#include <fcppt/string.hpp>
#include <fcppt/exception.hpp>

fruitcut::fruitlib::scenic::exception::exception(
	fcppt::string const &_string)
:
	fcppt::exception(
		_string)
{}

fruitcut::fruitlib::scenic::exception::~exception() throw()
{
}
