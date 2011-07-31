#include <fruitlib/exception.hpp>
#include <fcppt/string.hpp>
#include <fcppt/exception.hpp>

fruitlib::exception::exception(
	fcppt::string const &_string)
:
	fcppt::exception(
		_string)
{}

fruitlib::exception::~exception() throw()
{
}
