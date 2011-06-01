#ifndef FRUITCUT_APP_EXCEPTION_HPP_INCLUDED
#define FRUITCUT_APP_EXCEPTION_HPP_INCLUDED

#include <fcppt/exception.hpp>
#include <fcppt/string.hpp>

namespace fruitcut
{
namespace app
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
