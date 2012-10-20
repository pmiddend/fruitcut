#include <fruitapp/exception.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/string.hpp>


fruitapp::exception::exception(
	fcppt::string const &_string)
:
	fcppt::exception(
		_string)
{}

fruitapp::exception::~exception() throw()
{
}
