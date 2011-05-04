#ifndef FRUITCUT_EXCEPTION_HPP_INCLUDED
#define FRUITCUT_EXCEPTION_HPP_INCLUDED

#include <fcppt/exception.hpp>
#include <fcppt/string.hpp>

namespace fruitcut
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

#endif