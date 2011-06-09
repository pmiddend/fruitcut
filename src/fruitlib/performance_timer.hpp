#ifndef FRUITCUT_FRUITLIB_PERFORMANCE_TIMER_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PERFORMANCE_TIMER_HPP_INCLUDED

#include <sge/time/duration.hpp>
#include <sge/time/point.hpp>
#include <fcppt/chrono/chrono.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace fruitlib
{
class performance_timer
{
FCPPT_NONCOPYABLE(
	performance_timer);
public:
	typedef
	fcppt::optional<sge::time::duration>
	optional_threshold;

	typedef
	fcppt::function::object<void(sge::time::duration const &)>
	optional_callback;

	explicit
	performance_timer(
		optional_threshold const &threshold,
		optional_callback const &);

	~performance_timer();
private:
	optional_threshold const threshold_;
	optional_callback const callback_;
	sge::time::point const beginning_;
};
}
}

#endif
