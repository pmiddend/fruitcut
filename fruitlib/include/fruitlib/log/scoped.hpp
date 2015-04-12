#ifndef FRUITLIB_LOG_SCOPED_HPP_INCLUDED
#define FRUITLIB_LOG_SCOPED_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/log/context_fwd.hpp>
#include <fcppt/log/enabled_level_array.hpp>
#include <fcppt/log/level_fwd.hpp>
#include <fcppt/log/location.hpp>
#include <fcppt/log/optional_object.hpp>


namespace fruitlib
{
namespace log
{
class scoped
{
FCPPT_NONCOPYABLE(
	scoped);
public:
	scoped(
		fcppt::log::context &,
		fcppt::log::location const &,
		fcppt::log::level const &);

	FRUITLIB_DETAIL_SYMBOL
	~scoped();
private:
	fcppt::log::object &object_;
	fcppt::log::enabled_level_array const old_levels_;
};
}
}

#endif
