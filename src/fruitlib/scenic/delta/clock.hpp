#ifndef FRUITLIB_SCENIC_DELTA_CLOCK_HPP_INCLUDED
#define FRUITLIB_SCENIC_DELTA_CLOCK_HPP_INCLUDED

#include <fruitlib/scenic/delta/callback.hpp>
#include <fruitlib/scenic/delta/clock_impl.hpp>
#include <fcppt/chrono/time_point.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitlib
{
namespace scenic
{
namespace delta
{
class clock
{
FCPPT_NONCOPYABLE(
	clock);
public:
	typedef typename
	delta::clock_impl::rep
	rep;

	typedef typename
	delta::clock_impl::period
	period;

	typedef typename
	delta::clock_impl::duration
	duration;

	typedef typename
	delta::clock_impl::time_point
	time_point;

	static bool const is_steady = 
		true;

	explicit
	clock(
		delta::callback const &);

	~clock();

	void
	update();

	time_point
	now() const;
private:
	delta::callback const callback_;
	time_point now_;
};
}
}
}

#endif
