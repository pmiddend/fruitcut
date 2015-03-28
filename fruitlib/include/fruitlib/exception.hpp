#ifndef FRUITLIB_EXCEPTION_HPP_INCLUDED
#define FRUITLIB_EXCEPTION_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/string.hpp>
#include <fcppt/symbol/class.hpp>

namespace fruitlib
{
class FCPPT_SYMBOL_CLASS exception
:
	public fcppt::exception
{
public:
	explicit
	exception(
		fcppt::string const &);

	FRUITLIB_DETAIL_SYMBOL
	virtual
	~exception() throw();
};
}

#endif
