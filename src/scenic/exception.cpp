#include "exception.hpp"
#include <fcppt/string.hpp>
#include <fcppt/exception.hpp>

fruitcut::scenic::exception::exception(
	fcppt::string const &_string)
:
	fcppt::exception(
		_string)
{}

fruitcut::scenic::exception::~exception() throw()
{
}
