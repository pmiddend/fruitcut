#ifndef FRUITLIB_PP_FILTER_WRAPPER_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_WRAPPER_HPP_INCLUDED

#include <fruitlib/pp/filter/base_fwd.hpp>
#include <fcppt/string.hpp>
#include <fcppt/preprocessor/const.hpp>

namespace fruitlib
{
namespace pp
{
namespace filter
{
class wrapper
{
public:
	wrapper();

	wrapper(
		wrapper const &);

	wrapper(
		base &,
		fcppt::string const &name,
		bool active = true);

	wrapper &
	operator=(
		wrapper const &);

	base &
	filter() const
	FCPPT_PP_CONST;

	bool
	active() const;

	void
	active(
		bool);

	fcppt::string const &
	name() const
	FCPPT_PP_CONST;
private:
	base *filter_;
	fcppt::string name_;
	bool active_;
};
}
}
}

#endif
