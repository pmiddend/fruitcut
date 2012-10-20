#ifndef FRUITLIB_SCENIC_DELTA_CLOCK_HPP_INCLUDED
#define FRUITLIB_SCENIC_DELTA_CLOCK_HPP_INCLUDED

#include <fruitlib/scenic/delta/callback.hpp>
#include <fruitlib/scenic/delta/clock_impl.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/preprocessor/const.hpp>


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
	typedef
	delta::clock_impl::rep
	rep;

	typedef
	delta::clock_impl::period
	period;

	typedef
	delta::clock_impl::duration
	duration;

	typedef
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

	time_point const &
	now() const
	FCPPT_PP_CONST;
private:
	delta::callback const callback_;
	time_point now_;
};
}
}
}

#endif
