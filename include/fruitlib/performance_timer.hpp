#ifndef FRUITLIB_PERFORMANCE_TIMER_HPP_INCLUDED
#define FRUITLIB_PERFORMANCE_TIMER_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/system_clocks.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
class performance_timer
{
FCPPT_NONCOPYABLE(
	performance_timer);
public:
	typedef
	boost::chrono::high_resolution_clock
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