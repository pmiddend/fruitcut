#include "exception.hpp"
#include <fcppt/string.hpp>
#include <fcppt/exception.hpp>

fruitcut::app::exception::exception(
	fcppt::string const &_string)
:
	fcppt::exception(
		_string)
{}

fruitcut::app::exception::~exception() throw()
{
}
