#ifndef FRUITCUT_FRUITLIB_EXCEPTION_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_EXCEPTION_HPP_INCLUDED

#include <fcppt/exception.hpp>
#include <fcppt/string.hpp>

namespace fruitcut
{
namespace fruitlib
{
class exception
:
	public fcppt::exception
{
public:
	explicit 
	exception(
		fcppt::string const &);

	virtual 
	~exception() throw();
};
}
}

#endif
