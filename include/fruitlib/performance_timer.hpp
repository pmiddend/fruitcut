#ifndef FRUITLIB_PERFORMANCE_TIMER_HPP_INCLUDED
#define FRUITLIB_PERFORMANCE_TIMER_HPP_INCLUDED

#include <fcppt/chrono/high_resolution_clock.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/chrono/time_point.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitlib
{
class performance_timer
{
FCPPT_NONCOPYABLE(
	performance_timer);
public:
	typedef
	fcppt::chrono::high_resolution_clock
	clock;

	typedef
	fcppt::optional<clock::duration>
	optional_threshold;

	typedef
	fcppt::function::object<void(clock::duration const &)>
	optional_callback;

	explicit
	performance_timer(
		optional_threshold const &threshold,
		optional_callback const &);

	~performance_timer();
private:
	optional_threshold const threshold_;
	optional_callback const callback_;
	clock::time_point const beginning_;
};
}

#endif
