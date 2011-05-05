#include "exception.hpp"

fruitcut::exception::exception(
	fcppt::string const &_string)
:
	fcppt::exception(
		_string
	)
{}

fruitcut::exception::~exception() throw()
{
}
