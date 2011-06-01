#ifndef FRUITCUT_FRUITLIB_LOG_SCOPED_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_LOG_SCOPED_HPP_INCLUDED

#include <fcppt/log/context_fwd.hpp>
#include <fcppt/log/location.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/log/enabled_level_array.hpp>
#include <fcppt/log/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/container/array.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace log
{
class scoped
{
FCPPT_NONCOPYABLE(
	scoped);
public:
	explicit
	scoped(
		fcppt::log::context &,
		fcppt::log::location const &,
		fcppt::log::level::type const &);

	~scoped();
private:
	fcppt::log::object *object_;
	fcppt::log::enabled_level_array old_levels_;
};
}
}
}

#endif
