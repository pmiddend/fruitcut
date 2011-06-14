#ifndef FRUITCUT_FRUITLIB_PP_FILTER_WRAPPER_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PP_FILTER_WRAPPER_HPP_INCLUDED

#include "base_fwd.hpp"
#include <fcppt/string.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace pp
{
namespace filter
{
class wrapper 
{
public:
	explicit
	wrapper();

	explicit
	wrapper(
		base &,
		fcppt::string const &name,
		bool active = true);

	base &
	filter() const;

	bool 
	active() const;

	void 
	active(
		bool);

	fcppt::string const 
	name() const;
private:
	base *filter_;
	fcppt::string name_;
	bool active_;
};
}
}
}
}

#endif