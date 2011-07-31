#ifndef FRUITCUT_APP_EXCEPTION_HPP_INCLUDED
#define FRUITCUT_APP_EXCEPTION_HPP_INCLUDED

#include <fcppt/exception.hpp>
#include <fcppt/string.hpp>

namespace fruitapp
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
